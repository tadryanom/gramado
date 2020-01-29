/*
 * File: debug.c
 *
 * Modulo Debug.
 * MB - M�dulos inclu�dos no Kernel Base.
 *	
 * Descri��o:
 *     Kernel Debugger.
 *     Arquivo princial do m�dulo debug do executive do kernel.
 *     Checar se h� falhas no sistema. 
 *     Como um dr watson.
 * 
 * Obs:
 *     As fun��es aqui s�o usadas pelo kernel.
 *     Mas pode haver um aplicativo que utilizem essas fun��es.
 *
 * Obs:
 *     @todo: Come�ar a an�lise pela estrutura 'platform' e todo
 * o que est� dentro dela.
 *
 * This is a spiritual quote.
 *
 * #importante 
 * #todo: Criar aqui nesse documento fun��es de exibi��o de texto 
 *        pr�prias para debug, que classifiquem o erro por n�veis.
 *
 *    ex: debug_print( int level, ...)
 *
 * 2015 - Created by Fred Nora.
 *
 */

 
#include <kernel.h>


// Vari�veis internas.
//int debugStatus;
//int debugError;
//...

//
// # spiritual quote.
// Estruturas para fases de inicializa��o.
// De uso interno no debug.
// Just for fun. (user's mental model)
// ux4, ux3, ux2, ux1
// hal/microkernel/executive/gramado
// thinking, logic, creativity, action
//

typedef enum {
   MYTHIC, 
   ROMANTIC,   
   REALIST,   
   NATURALIST,   
}MindType_t;


struct Mind_d
{
    unsigned long Mythic;
    unsigned long Romantic;
    unsigned long Realist;
    unsigned long Naturalist;
};
struct Mind_d Mind;


//Internas.
//void InitMind();
//void SetMind(int Type);




/*
 * debug_check_inicialization:
 *     Checar se o kernel e os m�dulos foram inicializados.
 *     Checa o valor das flags.
 *     checar todos contextos de tarefas v�lidas.
 */

int debug_check_inicialization (void){
	
    int Status = 0;
	
	// Check phase.
	
	if ( KeInitPhase != 3 )
	{
	   Status = 1;
	   
	   printf ("debug_check_inicialization: KeInitPhase phase={%d}\n",
	       KeInitPhase );
	   
	   goto fail;
	};
	
    //
    // check personas
    //	

	// gramado.
	/*  @todo: Inda n�o implementado
	if(Initialization.gramado != 1){
	   Status = 1;
	   printf("debug_check_inicialization: gramado fail!\n");
	   goto fail;
	};
	*/
	
	
	//SetMind(NATURALIST);
	
	// Executive.
	if( Initialization.executive != 1 )
	{
	   Status = 1;
	   printf("sm-debug-debug_check_inicialization: executive\n");
	   goto fail;
	};
	//SetMind(REALIST);
	
	
	// Microkernel.
    if( Initialization.microkernel != 1 )
	{
	   Status = 1;
	   printf("sm-debug-debug_check_inicialization: microkernel\n");
	   goto fail;
	};
	//SetMind(ROMANTIC);
	
	// Hal.
    if( Initialization.hal != 1 )
	{
	   Status = 1;
	   printf("sm-debug-debug_check_inicialization: hal\n");
	   goto fail;
	};
	//SetMind(MYTHIC);
	
	
	//More?!
	
// Done! 
done:
    return (int) Status;
fail:
	die(); 
};




/*
 * debug_check_drivers:
 *    Checar se os drivers est�o inicializados.
 */
int debug_check_drivers (void){
	
	int Status = 0;
    
	if(g_driver_hdd_initialized != 1){
	    //erro
	};
	
	if(g_driver_keyboard_initialized != 1){
	    //erro
	};
	
    if(g_driver_pci_initialized != 1){
	    //erro
	};
	
    if(g_driver_rtc_initialized != 1){
	    //erro
	};
	
    if(g_driver_timer_initialized != 1){
	    //erro
	};
	

    return (int) Status;
}


/*
 ********************************************
 * debug_breakpoint:
 *     Para a execu��o do sistema.
 *     @todo: isso pode ir para o arquivo debug.c.
 */

void debug_breakpoint (void){
	
    printf ("debug_breakpoint:\n");
	die ();
}


//retorna um checksum dado um buffer e um tamanho.

unsigned long 
debug_compute_checksum ( unsigned char *Buffer, 
                         unsigned long Lenght )
{
    unsigned long CheckSum = 0;

    while (Lenght > 0)
	{	
		CheckSum = CheckSum + (unsigned long) *Buffer++;
	    
		Lenght--;	
	};

	return (unsigned long) CheckSum;
}



/*
 * debug:
 *     Checa por falhas depois de cumpridas as tr�s fases de inicializa��o.
 */
 
int debug (void){
	
	int Status;
		
	//printf("debug: Starting..\n");
	
	// Checa inicializa��o. Fases, vari�veis e estruturas.
	
	Status = (int) debug_check_inicialization();
	if(Status == 1)
	{
	   printf("sm-debug-debug: debug_check_inicialization\n");
	   die();
	};
	
    // 'processor' struct.
	if( (void *) processor == NULL )
	{
	    printf ("sm-debug-debug: processor\n");
		die();
	};

	//Check drivers status. 
	//( Ver se os principais drivers est�o inicializados )
	debug_check_drivers();
	
	
	/*
	 * @todo: 
	 *     Checar se existe componentes do sistema como mbr, root, fat 
	 * e arquivos e programas b�sicos do sistema.
	 */
	 
	 
	/* 
	 * @todo: 
	 *     Checar por falhas no sistema de arquivos.
	 */
	 
	 
	/*
     * @todo:	
	 *     Checar por falhas nas estruturas de tarefas.
	 */

    //
	// Antes do logon s� tem a Thread idle. 
	// Checar a estrutura da PID 0.
	// 
	 
	//if( (void*) threadList[0] == NULL ){
	//    printf("debug fail: TID 0!\n");
	//	refresh_screen();
	//	while(1){}
	//};
	
	//...

    //printf("debug:OK!\n");
    //printf("debug: Done.\n");


    return 0; 
}




void debug_print ( char *data ){
	
	uint32_t i;
	
	for ( i=0; data[i] != '\0'; i++ )
	{
		serial_write_char (data[i]);
	}
}


//
// End.
//

