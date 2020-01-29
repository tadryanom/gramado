/*
 * File: rtc.c 
 *
 * Descri��o:
 *     Real Time Controller ?!!
 *     Controlador RTC, Data e Hora.
 * @todo: Criar m�todos para acessar os valores salvos na estrutura Rtc.
 *     
 * Ambiente: (RING 0).
 *
 * "CMOS" is a tiny bit of very low power static memory that lives on the same 
 * chip as the Real-Time Clock. (osdev)
 *
 * History:
 *     2013 - Created by Fred Nora.
 *     2017 - Revision.
 */


/*
 A typical OS will use the APIC or PIT for timing purposes. However, the RTC 
 works just as well. RTC stands for Real Time Clock. It is the chip that keeps 
 your computer's clock up-to-date. Within the chip is also the 64 bytes of 
 CMOS RAM. (osdev).
*/ 
/*
    070 CMOS RAM/RTC, also NMI enable/disable (AT,PS/2, see RTC)
	071 CMOS RAM data  (AT,PS/2)
*/
 
 
#include <kernel.h>

//Internas.
unsigned long rtcTicks;
int rtcStatus;
int rtcError;
//...


//
//  @todo:    
//      As portas do controlador ainda est�o subutilizadas.
//      fazer um driver mais completo utilizando melhor o controlador.
//




//
// Vari�veis internas.
//
//int rtcStatus;
//int rtcError;
//...
 

 
/*
 * KiRtcIrq:
 *     irq8 interrupt handler.
 *     System CMOS, Realtime clock.
 *     Isso � uma interface para chamar a rotina verdadeira.
 */

void KiRtcIrq (void){
	

	// Se o rtc n�o estiver inicializado !
    if ( __breaker_rtc_initialized == 0 )
        return;



	// Contando as interrup��es desse tipo.
	g_profiler_ints_irq8++;	
	
	//...
	
    rtc_irq ();
}
 
 
/*
 *******************************************
 * rtc_irq: 
 *     irq8 interrupt handler.
 *     System CMOS, Realtime clock. 
 */

void rtc_irq (void){
	
	unsigned i;

	g_ticks++;
	
	//_BLINK; ??
	
    // Save contents of I/O port 0x70.
	i = inportb(0x70);
    
	// acknowledge IRQ 8 at the RTC by reading register C.
	outportb(0x70, 0x0C);
	(void)inportb(0x71); 
	outportb(0x70, i);
    
	// @todo: Checar esse EOI.
	// acknowledge IRQ 8 at the PICs
	//outportb(0xA0, 0x20);
	//outportb(0x20, 0x20);
}


/* 
 **************************************************
 * read_cmos_bcd:
 *     L� da CMOS um determinado registro. 
 */

unsigned long read_cmos_bcd ( unsigned reg ){
	
	unsigned long high_digit, low_digit;

	outportb ( 0x70, ( inportb(0x70) & 0x80) | (reg & 0x7F) );
	high_digit = low_digit = inportb(0x71);

	// Converte BCD para bin�rio. 
	high_digit >>= 4;

	high_digit &= 0x0F;
	low_digit  &= 0x0F;

	return (unsigned long) (10*high_digit) + low_digit;
}


/*
 *************************************************************** 
 * get_time:
 * Pega o hor�rio armazenado na CMOS.
 * Formato: Cada unidade representa 1 segundo. 
 *
 * todo: Essa fun��o pode ser trabalhada sem riscos ao sistema.
 * STATUS: N�o funciona muito bem. @todo: rever isso
 * Obs: Na verdade pode estar funcionando e o rel�gio da m�quina virtual
 * est� desatualizado.
 */

unsigned long get_time (void){
	
	unsigned long time = 0;

	time  = read_cmos_bcd(0);
	time += read_cmos_bcd(2) * 60;
	time += read_cmos_bcd(4) * (60*60);	
	
	return (unsigned long) time;
}


/*
 ***************************************************************
 * get_date: 
 * Pega a data armazenada na CMOS. 
 * Formato(bytes): YYMD 
 *
 * todo: Essa fun��o pode ser trabalhada sem riscos ao sistema.
 * sTATUS: fUNCIONA BEM.
 */

unsigned long get_date (void){
	
	unsigned long date = 0;

	date  = read_cmos_bcd(9);
	date += read_cmos_bcd(8) * 31;
	date += read_cmos_bcd(7) * (31*12);
	
	return (unsigned long) date;
}


/* 
 ************************************************************
 * rtcGetExtendedMemory:
 *     Get total memory info via CMOS. 
 *
 *  15h		Low byte of base memory size
 *				100h = 256k
 *				200h = 512k
 *				280h = 640k
 *	16h		High byte of 15h above
 *	17h		Low expansion memory byte
 *	18h		High expansion memory byte
 *				200h = 512k
 *				400h = 1024k
 *				600h-3C00h = 1536-15,360k
 *	19h		Extended type byte: Hard drive 1
 *	1Ah		Extended type byte: Hard drive 2
 *	1Bh-2Dh		reserved
 *	2Eh		check-sum for addresses 10h-2Dh  (word)
 *	2Fh		  see above
 *	30h		Low expansion memory byte
 *	31h		High expansion memory byte
 *				200h = 512k
 *				400h = 1024k
 *				600h-3C00h = 1536-15,360k
 *				
 *	Limite de uma 'word' ??			
 */

unsigned short rtcGetExtendedMemory (void){
	
    unsigned short total = 0;
    
	unsigned char lowmem;
	unsigned char highmem;
 
    //Low. (Low extended memory byte)
    outportb(0x70, RTC_LOWBYTE_EXTENDEDMEMORY);
    lowmem = inportb(0x71);
    
	//High. (High extended memory byte)
	outportb(0x70, RTC_HIGHBYTE_EXTENDEDMEMORY);
    highmem = inportb(0x71);
	
    //Total.
    total = lowmem | highmem << 8;
	
    return (unsigned short) total;
}


/* 
 **************************************************************
 * rtcGetBaseMemory:
 *     Get base memory info via CMOS. 
 *
 *  15h		Low byte of base memory size
 *				100h = 256k
 *				200h = 512k
 *				280h = 640k
 *	16h		High byte of 15h above
 *	17h		Low expansion memory byte
 *	18h		High expansion memory byte
 *				200h = 512k
 *				400h = 1024k
 *				600h-3C00h = 1536-15,360k
 *	19h		Extended type byte: Hard drive 1
 *	1Ah		Extended type byte: Hard drive 2
 *	1Bh-2Dh		reserved
 *	2Eh		check-sum for addresses 10h-2Dh  (word)
 *	2Fh		  see above
 *	30h		Low expansion memory byte
 *	31h		High expansion memory byte
 *				200h = 512k
 *				400h = 1024k
 *				600h-3C00h = 1536-15,360k
 *				
 *	Limite de uma 'word' ??			
 */
 
unsigned short rtcGetBaseMemory (void){
	
    unsigned short total = 0;
	
    unsigned char lowmem;
	unsigned char highmem;
 
    //Low. (Low base memory byte)
    outportb(0x70, RTC_LOWBYTE_BASEMEMORY);
    lowmem = inportb(0x71);
    
	//High. (High base memory byte)
	outportb(0x70, RTC_HIGHBYTE_BASEMEMORY);
    highmem = inportb(0x71);
	
    //Total.
    total = lowmem | highmem << 8;
	
    return (unsigned short) total;
}


/*
 *****************************************************************
 * get_cmos_info:
 *     Obs: Essa fun��o deve ser chamada apenas uma vez na inicializa��o
 * do m�dulo. @todo: Criar m�todos que pegam esses valores salvos na 
 * estrutura.
 */

// #bugbug
// Alocando mem�ria toda vez que chama a fun��o.
// Issa aloca��o deveria ser feita apenas uma vez
// na inicializa��o, depois somente atualizados os valores.

void *get_cmos_info (void){
	
	Rtc = (void *) kmalloc ( sizeof(struct rtc_d) );
	
    if ( (void *) Rtc == NULL)
	{
	    printf ("get_cmos_info fail: Struct\n");
		refresh_screen();
		
		//free(Rtc);
		return NULL;
	
	}else{
		
	    //time
	    Rtc->Seconds = read_cmos_bcd (0);  // Seconds.
	    Rtc->Minutes = read_cmos_bcd (2);  // Minutes.
	    Rtc->Hours   = read_cmos_bcd (4);  // Hours.

	    //date.
	    Rtc->Year = read_cmos_bcd(9);    
	    Rtc->Year = (2000 + Rtc->Year);
		Rtc->Month = read_cmos_bcd(8);    
	    Rtc->DayOfMonth = read_cmos_bcd(7);    
    };
	
	//struct
	if ( (void *) Hardware == NULL )
	{
		//erro
	    printf("get_cmos_info: Hardware\n");
		refresh_screen();
		
		//free(Rtc);
		return NULL;		 
	}else{
		Hardware->Rtc = Rtc;	//Save.	
	};

//show_message:
	
#ifdef KERNEL_VERBOSE	
	printf("Time=%d:%d:%d\n", Rtc->Hours, Rtc->Minutes, Rtc->Seconds );
	printf("Date=%d/%d/%d\n", Rtc->DayOfMonth, Rtc->Month, Rtc->Year );
#endif	
	
	return (void *) Rtc;
}


/*
 ************************************************************
 * init_clock: 
 *     Inicia a data e a hora do controlador.
 *
 * @todo: 
 *     Essa fun��o n�o deveria mostrar informa��es na tela.
 * tem que criar fun��o pra isso.
 * essa aqui so deveria pegar as informa��es e colocar em estrutura.
 */

int init_clock (void){


    __breaker_rtc_initialized = 0;

	/*
	 * @todo: criar uma estrutura para RTC.
	 */
	 
	 /*
	  *@todo:
	  * alocar memoria para a estrutura rtc.
	  * inicializar algumas variaveis da estrutura rtc.
	  */
	
	//unsigned long Time, Date; 
	
	//Time = get_time();
	//Date = get_date();
		  
	//printf("CLOCK INFORMATION:\n");
	//printf("Time=%d Date=%d\n", Time, Date);
	
	get_cmos_info();
	
    g_driver_rtc_initialized = (int) 1;	
	
	printf("Done!\n");	
	
    __breaker_rtc_initialized = 1;

    return 0;
}


//
// End.
//

