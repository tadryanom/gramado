/*
 * File: ata.c  
 *
 * IDE/AHCI support.
 *
 * History: 
 *     2018 - Created by Nelson Cole. <nelsoncole72@gmail.com>
 *     2019 - Revision by Fred Nora.
 *     ...
 */

/*
const char *dev_type[] = {
    "ATA",
    "ATAPI"
};
*/

/*
static const char *ata_sub_class_code_register_strings[] = {
    "Unknown",
    "IDE Controller",
    "Unknown",
    "Unknown",
    "RAID Controller",
    "Unknown",
    "AHCI Controller"
};
*/
 
//
// Obs: 
// o foco est� na lista de discos. diskList
//


#include <kernel.h>



extern st_dev_t *current_dev;



void ata_wait (int val){
	
    val /= 100;
   
    while(val--)io_delay();
    //while(val--)delay();      
};


// TODO: Nelson, ao configurar os bits BUSY e DRQ 
// devemos verificar retornos de erros.

unsigned char ata_wait_not_busy (){
	
    while(ata_status_read() &ATA_SR_BSY)
    if(ata_status_read() &ATA_SR_ERR)
    return 1;

    return 0;
};


unsigned char ata_wait_busy (){
	
    while(!(ata_status_read() &ATA_SR_BSY))
    if(ata_status_read() &ATA_SR_ERR)
    return 1;

    return 0;
};


unsigned char ata_wait_no_drq()
{
    while(ata_status_read() &ATA_SR_DRQ)
    if(ata_status_read() &ATA_SR_ERR)
    return 1;

    return 0;
};


unsigned char ata_wait_drq()
{
    while(!(ata_status_read() &ATA_SR_DRQ))
    if(ata_status_read() &ATA_SR_ERR)
    return 1;

    return 0;
};

 
void ata_soft_reset (){
	
    unsigned char data = inb (ata.ctrl_block_base_address + 2);
    
	outb ( ata.ctrl_block_base_address, data | 0x4 );
    outb ( ata.ctrl_block_base_address, data & 0xfb );    
};


//#bugbug
//L� o status de um disco determinado, se os valores  
//na estrutura estiverem certos.

unsigned char ata_status_read (){
	
   	return inb ( ata.cmd_block_base_address + ATA_REG_STATUS );
};


void ata_cmd_write (int cmd_val){
           
    // no_busy      	
	ata_wait_not_busy();
	outb ( ata.cmd_block_base_address + ATA_REG_CMD, cmd_val );
	
	// Esperamos 400ns
	ata_wait (400);  
};


unsigned char ata_assert_dever (char nport){

    switch (nport){

    case 0:
        ata.channel = 0;
        ata.dev_num = 0;
        break;
			
    case 1:   
        ata.channel = 0;
        ata.dev_num = 1;
        break;
			
    case 2:
        ata.channel = 1;
        ata.dev_num = 0;
        break;
			
    case 3:
        ata.channel = 1;
        ata.dev_num = 1;
        break;
    
    default:
        kprintf ("Port %d, volue not used\n", nport );
        return -1;
        break;
    };

    set_ata_addr (ata.channel);
	
    return 0;
};


/*
 *********************************************
 * ide_identify_device:
 */

int ide_identify_device ( uint8_t nport ){

    unsigned char status;
    unsigned char lba1, lba2;
		
	struct disk_d *disk;
	

    ata_assert_dever (nport);

	// Ponto flutuante
	//Sem unidade conectada ao barramento
    
    if ( ata_status_read() == 0xff )
    {
        return (int) -1;
    }

    outb ( ata.cmd_block_base_address + ATA_REG_SECCOUNT, 0 );  // Sector Count 7:0
    outb ( ata.cmd_block_base_address + ATA_REG_LBA0, 0 );      // LBA 7-0
    outb ( ata.cmd_block_base_address + ATA_REG_LBA1, 0 );      // LBA 15-8
    outb ( ata.cmd_block_base_address + ATA_REG_LBA2, 0 );      // LBA 23-16


    // Select device,
    outb ( ata.cmd_block_base_address + ATA_REG_DEVSEL, 0xE0 | ata.dev_num << 4 );
    ata_wait(400);

    // cmd
    ata_cmd_write (ATA_CMD_IDENTIFY_DEVICE); 
    
    // ata_wait_irq();
    // Nunca espere por um IRQ aqui
    // Devido unidades ATAPI, ao menos que pesquisamos pelo Bit ERROR
    // Melhor seria fazermos polling

    ata_wait(400);


	//Sem unidade no canal

    if ( ata_status_read() == 0 )
    {  
        return (int) -1;
    }


    lba1 = inb ( ata.cmd_block_base_address + ATA_REG_LBA1 );
    lba2 = inb ( ata.cmd_block_base_address + ATA_REG_LBA2 );

    //
    //    ## type ## 
    //


	//PATA
    if ( lba1 == 0 && lba2 == 0 )
    {
        // kputs("Unidade PATA\n");
        // aqui esperamos pelo DRQ
        // e eviamoos 256 word de dados PIO

        ata_wait_drq();
        ata_pio_read ( ata_identify_dev_buf, 512 );

        ata_wait_not_busy();
        ata_wait_no_drq();
		
		
		ide_ports[nport].channel = ata.channel;
		ide_ports[nport].dev_num = ata.dev_num;		

        //salvando o tipo em estrutura de porta.
        ide_ports[nport].id = (uint8_t) nport;
        ide_ports[nport].used = (int) 1;
        ide_ports[nport].magic = (int) 1234;
        ide_ports[nport].name = "PATA";	
        ide_ports[nport].type = (int) idedevicetypesPATA;
		
		
		disk = (struct disk_d *) malloc (  sizeof(struct disk_d) );
		if ((void *) disk != NULL )
		{
			disk->channel = ata.channel;
			disk->dev_num = ata.dev_num;
			disk->id = nport;  
			disk->used = 1;
			disk->magic = 1234;
			disk->name = "PATA-TEST";
			disk->diskType = DISK_TYPE_PATA;
			diskList[nport] = (unsigned long) disk;
		}

        return (int) 0;


	//SATA
    }
    else if ( lba1 == 0x3C && lba2 == 0xC3 ){

        //kputs("Unidade SATA\n");   
        // O dispositivo responde imediatamente um erro ao cmd Identify device
        // entao devemos esperar pelo DRQ ao invez de um BUSY
        // em seguida enviar 256 word de dados PIO.

        ata_wait_drq(); 
        ata_pio_read ( ata_identify_dev_buf, 512 );
        ata_wait_not_busy();
        ata_wait_no_drq();
		
		ide_ports[nport].channel = ata.channel;
		ide_ports[nport].dev_num = ata.dev_num;			

        //salvando o tipo em estrutura de porta.
        ide_ports[nport].id = (uint8_t) nport;
        ide_ports[nport].used = (int) 1;
        ide_ports[nport].magic = (int) 1234;
        ide_ports[nport].name = "SATA";	
        ide_ports[nport].type = (int) idedevicetypesSATA;
		
		disk = (struct disk_d *) malloc (  sizeof(struct disk_d) );
		if ((void *) disk != NULL )
		{
			
			disk->channel = ata.channel;
			disk->dev_num = ata.dev_num;
			
			disk->id = nport;  
			disk->used = 1;
			disk->magic = 1234;
			disk->name = "SATA-TEST";
			disk->diskType = DISK_TYPE_SATA;
			diskList[nport] = (unsigned long) disk;
		}

        return (int) 0;

    //PATAPI
    }
    else if ( lba1 == 0x14 && lba2 == 0xEB )
    {
        //kputs("Unidade PATAPI\n");   
        ata_cmd_write(ATA_CMD_IDENTIFY_PACKET_DEVICE);
        ata_wait(400);
        ata_wait_drq(); 
        ata_pio_read ( ata_identify_dev_buf, 512 );
        ata_wait_not_busy();
        ata_wait_no_drq();

		ide_ports[nport].channel = ata.channel;
		ide_ports[nport].dev_num = ata.dev_num;			
		
        //salvando o tipo em estrutura de porta.
        ide_ports[nport].id = (uint8_t) nport;
        ide_ports[nport].used = (int) 1;
        ide_ports[nport].magic = (int) 1234;
        ide_ports[nport].name = "PATAPI";
        ide_ports[nport].type = (int) idedevicetypesPATAPI;
		
		
		disk = (struct disk_d *) malloc (  sizeof(struct disk_d) );
		if ((void *) disk != NULL )
		{
			disk->channel = ata.channel;
			disk->dev_num = ata.dev_num;
			
			disk->id = nport;  
			disk->used = 1;
			disk->magic = 1234;
			disk->name = "PATAPI-TEST";
			disk->diskType = DISK_TYPE_PATAPI;
			diskList[nport] = (unsigned long) disk;
		}

        return (int) 0x80;

    //SATAPI
    }
    else if (lba1 == 0x69  && lba2 == 0x96){

        //kputs("Unidade SATAPI\n");   
        ata_cmd_write(ATA_CMD_IDENTIFY_PACKET_DEVICE);
        ata_wait(400);
        ata_wait_drq(); 
        ata_pio_read(ata_identify_dev_buf,512);
        ata_wait_not_busy();
        ata_wait_no_drq();
		
		
		ide_ports[nport].channel = ata.channel;
		ide_ports[nport].dev_num = ata.dev_num;			

        //salvando o tipo em estrutura de porta.
        ide_ports[nport].id = (uint8_t) nport;
        ide_ports[nport].used = (int) 1;
        ide_ports[nport].magic = (int) 1234;
        ide_ports[nport].name = "SATAPI";
        ide_ports[nport].type = (int) idedevicetypesSATAPI;
		
		disk = (struct disk_d *) malloc (  sizeof(struct disk_d) );
		if ((void *) disk != NULL )
		{
			
			disk->channel = ata.channel;
			disk->dev_num = ata.dev_num;
			
			disk->id = nport;  
			disk->used = 1;
			disk->magic = 1234;
			disk->name = "SATAPI-TEST";
			disk->diskType = DISK_TYPE_SATAPI;
			diskList[nport] = (unsigned long) disk;
		}

        return (int) 0x80;

    }
	
	return (int) 0;   
}


//#aten��o.
extern st_dev_t *current_dev;


void set_ata_addr (int channel){

    switch (channel){
			
        case ATA_PRIMARY:
            ata.cmd_block_base_address  = ATA_BAR0;
            ata.ctrl_block_base_address = ATA_BAR1;
            ata.bus_master_base_address = ATA_BAR4;
            break;
			
        case ATA_SECONDARY:
            ata.cmd_block_base_address  = ATA_BAR2;
            ata.ctrl_block_base_address = ATA_BAR3;
            ata.bus_master_base_address = ATA_BAR4 + 8;
            break;
			
        //default:
            //PANIC
            //break;
    };
};



/* *
 *
 * Obs: O que segue s�o rotinas de suporte ao controlador IDE.
 *
 */



st_dev_t *current_dev;       // A unidade atualmente selecionada.
st_dev_t *ready_queue_dev;   // O in�cio da lista.

uint32_t  dev_next_pid = 0;  // O pr�ximo ID de unidade dispon�vel. 


/*
 ********************************************************
 * ide_mass_storage_initialize:
 *     Rotina de inicializa��o de dispositivo de armazenamento de dados.
 */

void ide_mass_storage_initialize (){
	
	int port;

    //
    // Vamos trabalhar na lista de dispositivos.
    //	
	
	// Iniciando a lista.
	ready_queue_dev = ( struct st_dev * ) kmalloc ( sizeof( struct st_dev) );
	
	//#todo
	//We need to check the pointer validation.
	//if ( (void *) ready_queue_dev == NULL )
		//fail
	
	current_dev = ( struct st_dev * ) ready_queue_dev;
    current_dev->dev_id      = dev_next_pid++;
    current_dev->dev_type    = -1;
    current_dev->dev_num     = -1;
    current_dev->dev_channel = -1;
    current_dev->dev_nport   = -1;
    current_dev->next        = NULL;

    // ??
	ata_identify_dev_buf = ( unsigned short * ) kmalloc (4096);
	
	//#todo: check null pointer

	//
	// Sondando dispositivos e imprimindo na tela.
	//
	
    // As primeiras quatro portas do controlador IDE.    
	for ( port=0; port < 4; port++ ){
		
        ide_dev_init(port);
	}
};


/*
 ********************************************************
 * ide_dev_init:
 *     ?? Alguma rotina de configura��o de dispositivos.
 */

int ide_dev_init (char port){
	
    int data;

    st_dev_t *new_dev;	
	
    new_dev = ( struct st_dev * ) kmalloc ( sizeof( struct st_dev) );
    
	if ( (void *) new_dev ==  NULL )
	{
		
		printf("ide_dev_init: struct");
		die ();
	}
	
	
	data = (int) ide_identify_device (port);
	
    if ( data == -1 )
	{
		//@todo: 
		//   Message.
        
		return (int) 1;
	};
	
	if ( data == 0 )
	{
        // Unidades ATA.

        new_dev->dev_type   = (ata_identify_dev_buf[0] &0x8000)? 0xffff: ATA_DEVICE_TYPE;
        new_dev->dev_access = (ata_identify_dev_buf[83]&0x0400)? ATA_LBA48: ATA_LBA28;
        
		if(ATAFlag == FORCEPIO)
		{
			//com esse s� funciona em pio
		    new_dev->dev_modo_transfere = 0;	
		}else{
		    //com esse pode funcionar em dma
		    new_dev->dev_modo_transfere = (ata_identify_dev_buf[49]&0x0100)? ATA_DMA_MODO: ATA_PIO_MODO;
        };
		
		new_dev->dev_total_num_sector   = ata_identify_dev_buf[60];
        new_dev->dev_total_num_sector  += ata_identify_dev_buf[61];
		
        new_dev->dev_byte_per_sector = 512;
		
        new_dev->dev_total_num_sector_lba48  = ata_identify_dev_buf[100];
        new_dev->dev_total_num_sector_lba48 += ata_identify_dev_buf[101];
        new_dev->dev_total_num_sector_lba48 += ata_identify_dev_buf[102];
        new_dev->dev_total_num_sector_lba48 += ata_identify_dev_buf[103];
		
        new_dev->dev_size = (new_dev->dev_total_num_sector_lba48 * 512);

        
    }else if( data == 0x80 )
	      {

              // Unidades ATAPI.

              new_dev->dev_type = (ata_identify_dev_buf[0]&0x8000)? ATAPI_DEVICE_TYPE: 0xffff;
              
			  new_dev->dev_access = ATA_LBA28;
              
			  if(ATAFlag == FORCEPIO)
			  {
                  //com esse s� funciona em pio 				  
			      new_dev->dev_modo_transfere = 0; 
			  }else{
			      //com esse pode funcionar em dma
			      new_dev->dev_modo_transfere = (ata_identify_dev_buf[49]&0x0100)? ATA_DMA_MODO: ATA_PIO_MODO;
              };
              			  
			    
			  
			  new_dev->dev_total_num_sector  = 0;
              new_dev->dev_total_num_sector += 0;
              
			  new_dev->dev_byte_per_sector = 2048; 
              
			  new_dev->dev_total_num_sector_lba48  = 0;
              new_dev->dev_total_num_sector_lba48 += 0;
              new_dev->dev_total_num_sector_lba48 += 0;
              new_dev->dev_total_num_sector_lba48 += 0;
              
			  new_dev->dev_size = (new_dev->dev_total_num_sector_lba48 * 2048);

    
          }else{
               
			   // @todo: Message.
			   // Identificar o erro
               return (int) 1;
		  };

    //Dados em comum.

    new_dev->dev_id = dev_next_pid++;
    new_dev->dev_num = ata.dev_num;
	new_dev->dev_channel= ata.channel;
    new_dev->dev_nport = port;
    
	//
	// port
	//
	
	switch ( port ){
			
        case 0:
            dev_nport.dev0 = 0x81;
            break;
            
		case 1:
            dev_nport.dev1 = 0x82;
            break;
        
		case 2:
            dev_nport.dev2 = 0x83;
            break;
            
		case 3:
            dev_nport.dev3 = 0x84;
            break;
			
		//#aten��o
		//Essa estrutura � para 32 portas.
		//para listar as portas AHCI.
		//Mas aqui est� apenas listando as 4 portas IDE.	
		
    };


#ifdef KERNEL_VERBOSE
    //#todo
	//kprintf("[ Detected Disk type: %s ]\n", dev_type[new_dev->dev_type] );
	//refresh_screen();
#endif

    new_dev->next = NULL;

    //
    // Add no fim da lista (ready_queue_dev).
	//
	
    st_dev_t *tmp_dev; 
	
	tmp_dev = ( struct st_dev * ) ready_queue_dev;

	//#todo:
	//Checar a validade da estrutura.
	
    while ( tmp_dev->next )
	{
        tmp_dev = tmp_dev->next;
    };
    
    tmp_dev->next = new_dev;

	return (int) 0;
};


/* dev_switch:
 *     ?? Porque esse tipo ?? */

static inline void dev_switch (void){
	
	// ??
    // Pula, se ainda n�o tiver nenhuma unidade.
	
    if ( !current_dev )
	{
        return;
	}
	
    // Obter a pr�xima tarefa a ser executada.
    // Se ca�mos no final da lista vinculada, 
    // comece novamente do in�cio.

    current_dev = current_dev->next;    
    
	if ( !current_dev ){
		
        current_dev = ready_queue_dev;
    }
};


/*
 * getpid_dev:
 *     ?? Deve ser algum suporte a Processos.
 */
static inline int getpid_dev (){
	
	//#todo:
	//Checar a validade da estrutura.
	
    return current_dev->dev_id;
};


/*
 * getnport_dev: */

static inline int getnport_dev (){

	//#todo:
	//Checar a validade da estrutura.
	
    return current_dev->dev_nport;
};


/*
 * nport_ajuste:
 */
int nport_ajuste ( char nport ){
	
    char i = 0;
	
    while ( nport != getnport_dev() )
	{
        if ( i == 4 )
		{ 
		    return (int) 1; 
		}
        
		dev_switch();
        
		i++;
    };
	
    if ( getnport_dev() == -1 )
	{ 
	    return (int) 1; 
	}
	
    return (int) 0;
}



/** 
 * Obs: O que segue s�o rotinas de suporte a ATA.
 */

void ata_pio_read ( void *buffer, int bytes ){
	
    __asm__ __volatile__ (\
                "cld;\
                rep; insw"::"D"(buffer),\
                "d"(ata.cmd_block_base_address + ATA_REG_DATA),\
                "c"(bytes/2) );
				
};


void ata_pio_write ( void *buffer, int bytes ){
	
    __asm__ __volatile__ (\
                "cld;\
                rep; outsw"::"S"(buffer),\
                "d"(ata.cmd_block_base_address + ATA_REG_DATA),\
                "c"(bytes/2) );

};



/**
 * Obs: 
 * O que segue s�o rotinas de suporte a IDE ATAPI.
 */

 
extern st_dev_t *current_dev;
extern uint8_t *dma_addr;


/*
 * atapi_pio_read:
 */
 
static inline void atapi_pio_read ( void *buffer, uint32_t bytes ){
	
    __asm__ __volatile__ (\
                "cld;\
                rep; insw"::"D"(buffer),\
                "d"(ata.cmd_block_base_address + ATA_REG_DATA),\
                "c"(bytes/2) );
};

 
extern st_dev_t *current_dev;
unsigned char *dma_addr;



 
/*
 ********************
 * diskATAInitialize:
 *     Inicializa o IDE e mostra informa��es sobre o disco.
 */
 
int diskATAInitialize ( int ataflag ){
	
	int Status = 1;  //error
	int port;
	
	unsigned long data;
	
	unsigned char bus;
	unsigned char dev;
	unsigned char fun;
	
	
	// Configurando flags do driver.
	
	ATAFlag = (int) ataflag;
	
	//
	// Messages
	//
	
#ifdef KERNEL_VERBOSE
    kprintf("sm-disk-disk-diskATAInitialize:\n");
    kprintf("Initializing IDE/AHCI support ...\n");
	//refresh_screen();
#endif

    // Sondando a interface PCI para encontrarmos um dispositivo
    // que seja de armazenamento de dados.
	
	//PCI_CLASSCODE_MASS	
    data = (unsigned long) diskPCIScanDevice(PCI_CLASSE_MASS);
	
	if( data == -1 )
	{
		kprintf ("sm-disk-disk-diskATAInitialize: diskPCIScanDevicefail. ret={%d} \n", 
		    (unsigned long) data );
		
	    // Abortar.
		Status = (int) (PCI_MSG_ERROR);
		goto fail;
	}
    
    bus = ( data >> 8 & 0xff );
    dev = ( data >> 3 & 31 );
    fun = ( data      & 7 );

	//
	// Vamos saber mais sobre o dispositivo enconrtado. 
	//
	
    data = (unsigned long) diskATAPCIConfigurationSpace ( bus, dev, fun );

    if( data == PCI_MSG_ERROR )
	{
        kprintf("sm-disk-disk-diskATAInitialize: Error Driver [%X]\n",data);
		Status = (int) 1;
		goto fail;  
	
	}else if( data == PCI_MSG_AVALIABLE )
	      {
              kprintf("sm-disk-disk-diskATAInitialize: RAID Controller Not supported.\n");
		      Status = (int) 1;
		      goto fail;  
          };
		  
	//
    // Salvando informa��es.
    //	
	
	// Aqui estamos pegando informa��es na estrutura ata_pci sobre as BARs 
	// e manipulando essas informa��es.
	// ?? N�o sei o que est� fazendo aqui, talvez procurando endere�o de porta.

    // Initialize base address
    // AHCI/IDE Compativel com portas IO IDE legado
	
    ATA_BAR0 = ( ata_pci.BAR0 & ~7 )   + ATA_IDE_BAR0 * ( !ata_pci.BAR0 );
    ATA_BAR1 = ( ata_pci.BAR1 & ~3 )   + ATA_IDE_BAR1 * ( !ata_pci.BAR1 );       
    ATA_BAR2 = ( ata_pci.BAR2 & ~7 )   + ATA_IDE_BAR2 * ( !ata_pci.BAR2 );
    ATA_BAR3 = ( ata_pci.BAR3 & ~3 )   + ATA_IDE_BAR3 * ( !ata_pci.BAR3 );
    
	ATA_BAR4 = ( ata_pci.BAR4 & ~0x7 ) + ATA_IDE_BAR4 * ( !ata_pci.BAR4 );
    ATA_BAR5 = ( ata_pci.BAR5 & ~0xf ) + ATA_IDE_BAR5 * ( !ata_pci.BAR5 );
	
	//
	// Colocando nas estruturas.
	//
	
	ide_ports[0].base_port = (unsigned short) ATA_BAR0;
	ide_ports[1].base_port = (unsigned short) ATA_BAR1;
	ide_ports[2].base_port = (unsigned short) ATA_BAR2;
	ide_ports[3].base_port = (unsigned short) ATA_BAR3;
	//tem ainda a porta do dma na bar4

	
	//
	// De acordo com o tipo.
	//
	
	//
	// Se for IDE.
	//
	
	// Type
    if ( ata.chip_control_type == ATA_IDE_CONTROLLER )
	{
        //Soft Reset, defina IRQ
        
		outb ( ATA_BAR1, 0xff );
        outb ( ATA_BAR3, 0xff );
        outb ( ATA_BAR1, 0x00 );
        outb ( ATA_BAR3, 0x00 );

        ata_record_dev = 0xff;
        ata_record_channel = 0xff;

#ifdef KERNEL_VERBOSE	
	    printf("Initializing IDE Mass Storage device ...\n");
	    refresh_screen();
#endif    
	
	    //
	    // As estruturas de disco ser�o colocadas em uma lista encadeada.
	    //
	
	    //ide_mass_storage_initialize();
		

        //
        // Vamos trabalhar na lista de dispositivos.
        //	
	
	    // Iniciando a lista.
	    ready_queue_dev = ( struct st_dev * ) kmalloc ( sizeof( struct st_dev) );
	
	    //#todo:
	    //Checar a validade da estrutura.
	
	    current_dev = ( struct st_dev * ) ready_queue_dev;
    
	    current_dev->dev_id = dev_next_pid++;
	    current_dev->dev_type = -1;
        current_dev->dev_num = -1;
        current_dev->dev_channel = -1;
        current_dev->dev_nport = -1;
        current_dev->next = NULL;

        // ??
	    ata_identify_dev_buf = ( unsigned short  * ) kmalloc (4096);

        // Sondando dispositivos e imprimindo na tela.
	
        // As primeiras quatro portas do controlador IDE.    
	    for ( port=0; port < 4; port++ )
	    {
            ide_dev_init(port);
	    };		
		
			
		//
		// Agora se for AHCI.
		//

    }else if( ata.chip_control_type == ATA_AHCI_CONTROLLER )
	      {
			  
              // Nothing for now !!!		  

              // Aqui, vamos mapear o BAR5
              // Estou colocando na marca 28MB
    
//#ifdef KERNEL_VERBOSE	
              // printf("ata_initialize: mem_map para ahci\n");
              // refresh_screen();
//#endif
	
	          //mem_map( (uint32_t*)0x01C00000, (uint32_t*) ATA_BAR5, 0x13, 2);

              //kputs("[ AHCI Mass Storage initialize ]\n");
              //ahci_mass_storage_init();

          }else{
			  
			   //
			   // Panic !!
			   //
               
			   kprintf(" # Panic! # \n");
			   kprintf("sm-disk-disk-diskATAInitialize:\n");
		       kprintf("IDE and AHCI not found\n");
			   die();
          };
	
    // Ok
    
	Status = 0;
    goto done;
	
// fail
fail:
    kprintf("sm-disk-disk-diskATAInitialize: fail\n");    	
done:

//#ifdef KERNEL_VERBOSE 
    //#debug
	//kprintf("done!\n");
    //refresh_screen();
//#endif 
	
    return (int) Status;	
};
 

/*
 ******************************************************
 * show_ide_info:
 *     Mostrar as informa��es obtidas na inicializa��es 
 * do controlador.
 */

void show_ide_info (){
	
	int i;
	
	printf ("\n dd-disk1-show_ide_info:\n");
	
	for ( i=0; i<4; i++ )
	{
				
		//printf ("\n\n");
		
		printf ("channel=%d dev_num=%d \n", ide_ports[i].channel, ide_ports[i].dev_num);
					
		printf ("id=%d\n", ide_ports[i].id );
		
		printf ("used=%d\n", ide_ports[i].used );
		printf ("magic=%d\n", ide_ports[i].magic );
		
		printf ("type=%d\n", ide_ports[i].type );
		
		printf ("name=%s\n", ide_ports[i].name );
		
		printf ("base_port=%x\n", ide_ports[i].base_port );
	}	
	
	
	
	//primary secondary  ... master slave
	//printf ( " channel=%d dev=%d \n", ata.channel, ata.dev_num );
	
     
	
	/*
	// Estrutura 'ata'
	// Qual lista ??
	
	//pegar a estrutura de uma lista.
	
	//if( ata != NULL )
	//{
		printf("ata:\n");
 	    printf("type={%d}\n", (int) ata.chip_control_type);
	    printf("channel={%d}\n", (int) ata.channel);
	    printf("devType={%d}\n", (int) ata.dev_type);
	    printf("devNum={%d}\n", (int) ata.dev_num);
	    printf("accessType={%d}\n", (int) ata.access_type);
	    printf("cmdReadMode={%d}\n", (int) ata.cmd_read_modo);
	    printf("cmdBlockBaseAddress={%d}\n", (int) ata.cmd_block_base_address);
	    printf("controlBlockBaseAddress={%d}\n", (int) ata.ctrl_block_base_address);
		printf("busMasterBaseAddress={%d}\n", (int) ata.bus_master_base_address);
		printf("ahciBaseAddress={%d}\n", (int) ata.ahci_base_address);
	//};
	
	*/
	
	// Estrutura 'atapi'
	// Qual lista ??
	
	
	// Estrutura 'st_dev'
	// Est�o na lista 'ready_queue_dev'	

    //refresh_screen();
}


//
// End.
//

