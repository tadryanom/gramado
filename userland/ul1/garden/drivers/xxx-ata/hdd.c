/*
 * File: dd/hdd.c 
 * 
 * Descri��o:
 *     Interface de acesso a discos do tipo HDD.
 *     Deve haver um driver para cada marca suportada.
 *     Driver IDE presente dentro do kernel.
 *
 *     Rotinas de hardware. N�o s�o opera��es no
 *     sistema de arquivos.
 *
 *     O sistema de arquivos chama essas rotinas.
 *     O gerenciamento de discos chama essas rotinas.
 *
 * Obs: Esse m�dulo � um gerenciador de controlador de hard disk
 *      Aqui n�o importa se � ATA ou Serial ATA. Rotinas espec�ficas
 * ser�o executadas pela classe apropriada de interface, aqui � ais geral.
 * Por exemplo: Sondagem por tipos de dispositivos de armazenamento.
 * H� muitos tipos de hard drive. Esse m�dulo � somente para hard drives.
 *
 *     Ambiente: (RING 0).
 *
 * #obs: Vamos montar dispositivos em /DEV
 *
 *    2013 - Created by Fred Nora.
 *    2016 - Revision.
 */

 
 
/*
 hd info:
 =======
 PIIX3 ATA: LUN#0: disk, PCHS=963/4/17, total number of sectors 65536. (Oracle Virtualbox)
 estat�stica de hoje:
 (apenas leitura, usando PIO mode)
00:01:59.902737 /Devices/IDE0/ATA0/Unit0/AtapiDMA            0 times
00:01:59.902742 /Devices/IDE0/ATA0/Unit0/AtapiPIO            0 times
00:01:59.902747 /Devices/IDE0/ATA0/Unit0/DMA                 0 times
00:01:59.902753 /Devices/IDE0/ATA0/Unit0/PIO              1699 times  ***
00:01:59.902760 /Devices/IDE0/ATA0/Unit0/ReadBytes      869376 bytes  ***
00:01:59.902766 /Devices/IDE0/ATA0/Unit0/WrittenBytes        0 bytes
 */ 
 
 
#include <kernel.h>


/*
 * Externs.
 */
 
extern void os_read_sector();
extern void os_write_sector();
extern void reset_ide0();	


//Usadas por read e write.
extern unsigned long hd_buffer;
extern unsigned long hd_lba;



//Vari�veis internas
int hddStatus;
int hddError;
//...



//interna
static void hdd_ata_pio_read ( int p, void *buffer, int bytes ){
	
    __asm__ __volatile__ (\
                "cld;\
                rep; insw":: "D" (buffer),\
                "d" ( ide_ports[p].base_port + 0 ),\
                "c" (bytes/2));
};


void hdd_ata_pio_write ( int p, void *buffer, int bytes ){
	
    __asm__ __volatile__ (\
                "cld;\
                rep; outsw"::"S"(buffer),\
                "d"( ide_ports[p].base_port + 0 ),\
                "c"(bytes/2));
};


uint8_t hdd_ata_status_read (int p){

    //#bugbug: 
	//rever o offset

	//return inb(ata[p].cmd_block_base_addr + ATA_REG_STATUS);
    return (uint8_t) inportb ( (int) ide_ports[p].base_port + 7);
};
 

int hdd_ata_wait_not_busy (int p){
	
    while( hdd_ata_status_read(p) & ATA_SR_BSY )
    if ( hdd_ata_status_read(p) & ATA_SR_ERR )
        return 1;

    return (int) 0;
};


void hdd_ata_cmd_write ( int port, int cmd_val ){

    // no_busy      	
	hdd_ata_wait_not_busy(port);
	
	//outb(ata.cmd_block_base_address + ATA_REG_CMD,cmd_val);
	
	outportb ( (int) ide_ports[port].base_port + 7 , (int) cmd_val );
	
	// Esperamos 400ns
	ata_wait (400);  
};


int hdd_ata_wait_no_drq (int p){
	
    while( hdd_ata_status_read(p) &ATA_SR_DRQ)
    if(hdd_ata_status_read(p) &ATA_SR_ERR)
    return 1;

    return 0;
};


/*
 ******************************************************************
 * pio_rw_sector
 * 
 * IN:
 *   buffer - Buffer address
 *	lba - LBA number 
 *   rw - Flag read or write.	
 *
 *   //inline unsigned char inportb (int port)
 *   //outportb ( int port, int data )
 *   (IDE PIO)	
 */

int 
pio_rw_sector ( unsigned long buffer, 
                unsigned long lba, 
				int rw, 
				int port,
                int master )
{

    unsigned long tmplba = (unsigned long) lba;
	
	
	if ( port < 0 || port >= 4 )
		return -1;
	
	
	//Selecionar se � master ou slave.
	//outb (0x1F6, slavebit<<4)
	//0 - 3		In CHS addressing, bits 0 to 3 of the head. 
	//          In LBA addressing, bits 24 to 27 of the block number
	//4	DRV	Selects the drive number.
	//5	1	Always set.
	//6	LBA	Uses CHS addressing if clear or LBA addressing if set.
	//7	1	Always set.
	
	//0x01F6 ; Port to send drive and bit 24 - 27 of LBA
	tmplba = tmplba >> 24;	
	
	// no bit 4.
	// 0 = master 
	// 1 = slave
	
	//master. bit 4 = 0
	if (master == 1)
	{
		tmplba = tmplba | 0x000000E0;    //1110 0000b;
	}
	
	//slave. bit 4 = 1
	if (master == 0)
	{
		tmplba = tmplba | 0x000000F0;    //1111 0000b;
	};
	
	outportb ( (int) ide_ports[port].base_port + 6 , (int) tmplba );
	
	
	//0x01F2 ; Port to send number of sectors
	outportb ( (int) ide_ports[port].base_port + 2 , (int) 1 );
	
	
	//0x1F3  ; Port to send bit 0 - 7 of LBA
	tmplba = lba;
	tmplba = tmplba & 0x000000FF;	
	outportb ( (int) ide_ports[port].base_port + 3 , (int) tmplba );
	
	
	//0x1F4  ; Port to send bit 8 - 15 of LBA
	tmplba = lba;
	tmplba = tmplba >> 8;
	tmplba = tmplba & 0x000000FF;
	outportb ( (int) ide_ports[port].base_port + 4 , (int) tmplba );
	

	//0x1F5  ; Port to send bit 16 - 23 of LBA
	tmplba = lba;
	tmplba = tmplba >> 16;
	tmplba = tmplba & 0x000000FF;
	outportb ( (int) ide_ports[port].base_port + 5 , (int) tmplba );
	
	
	// 0x1F7       ; Command port
	//rw
	rw = rw & 0x000000FF;	
	outportb ( (int) ide_ports[port].base_port + 7 , (int) rw );
	
	
	//PIO or DMA ??
	//If the command is going to use DMA, set the Features Register to 1, otherwise 0 for PIO.
	//outb (0x1F1, isDMA)
	
	
	//
	// #timeout sim, n�o podemos esperar para sempre.
	//
	
	unsigned char c; 
	
	unsigned long timeout = 4444*512;
	
again:
	
	c = (unsigned char) inportb ( (int) ide_ports[port].base_port + 7);
	
	c = ( c & 8 );
	
	if ( c == 0 )
	{
		timeout--;
		if ( timeout == 0 )
		{
			printf("rw sector timeout fail;\n");
			return -3;
		}
	   //#bugbug: isso pode enrroscar aqui.	
	   goto again;	
	}
	
	
	switch (rw)
	{
		//read
		case 0x20:
		    hdd_ata_pio_read ( (int) port, (void *) buffer, (int) 512 );
		    break;

		//write
		case 0x30:
		    hdd_ata_pio_write ( (int) port, (void *) buffer, (int) 512 );
			
            //Flush Cache
    
	        //ata_cmd_write(p,ATA_CMD_FLUSH_CACHE);
	        hdd_ata_cmd_write( (int) port, (int) ATA_CMD_FLUSH_CACHE );
            hdd_ata_wait_not_busy(port);
            if ( hdd_ata_wait_no_drq(port) != 0)
	        {
                return -1;
            }	
			break;
        
		//fail
		default:
            printf ("pio_rw_sector: fail *hang");
			die();
			break; 		
	};
		
    return (int) 0;	
}


/*
 *****************************************
 * my_read_hd_sector:
 * eax - buffer
 * ebx - lba
 * ecx - null
 * edx - null
 * Op��o: void hddReadSector(....)
 */
 
void 
my_read_hd_sector ( unsigned long ax, 
					unsigned long bx, 
					unsigned long cx, 
					unsigned long dx )
{

	//=========================================== ATEN�AO ==============================
	// #IMPORTANTE:
    //#todo
	//s'o falta conseguirmos as variaveis que indicam o canal e se 'e master ou slave.

	// (buffer, lba, rw flag, port number, master )
	pio_rw_sector ( (unsigned long) ax, 
					(unsigned long) bx, 
					(int) 0x20, 
					(int) 0,    //channel
					(int) 1 );  //master=1 slave=0

	/*
	 // #antigo.
	// Suspenso.
	 // Passando os argumentos.	
	 hd_buffer = (unsigned long) ax;    //arg1 = buffer. 
	 hd_lba = (unsigned long) bx;       //arg2 = lba.
	
	// Read sector. (ASM)
	os_read_sector(); 	

	//#todo: deletar esse return.
	//testar sem ele antes.
	*/

	return;
}


/*
 *************************************
 * my_write_hd_sector:
 * eax - buffer
 * ebx - lba
 * ecx - null
 * edx - null
 * Op��o: void hddWriteSector(....)
 */

void 
my_write_hd_sector ( unsigned long ax,
					 unsigned long bx,
					 unsigned long cx,
					 unsigned long dx )
{

	//=========================================== ATEN�AO ==============================
	// #IMPORTANTE:
    //#todo
	//s'o falta conseguirmos as variaveis que indicam o canal e se 'e master ou slave.
	
	// (buffer, lba, rw flag, port number, master )
	pio_rw_sector ( (unsigned long) ax, 
					(unsigned long) bx, 
					(int) 0x30, 
					(int) 0,     //channel
					(int) 1 );   //master=1 slave=0

/*
	//antigo.
	// Passando os argumentos.
	hd_buffer = (unsigned long) ax;    //arg1 = buffer. 
	hd_lba = (unsigned long) bx;       //arg2 = lba.

	// Write sector. (ASM)
	// entry/x86/head/hwlib.inc

	os_write_sector(); 

*/	
	//#todo: deletar esse return.
	//testar sem ele antes.	
	return;
};


/*
 ***************************************
 * init_hdd:
 *     Inicializa o driver de hd.
 *     @todo: Mudar para hddInit().
 */
int init_hdd (){

	//
	// @todo: We need to do something here.
	//	

//done:

	g_driver_hdd_initialized = (int) 1;

	return (int) 0;
};


/*
int hddInit();
int hddInit()
{
    hddStatus = 0;
    hddError = 0;
    //...

	init_hdd();
    return (int) 0;	
};
*/

//
// End.
//

