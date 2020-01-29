/*
 * File: rt/runtime.c
 *
 * Descri��o:
 *     Arquivo principal do m�dulo '/rt' do kernel base.
 *     Interfaces para rotinas de runtime do kernel.
 *
 * Objetivo:
 *    Fazer a inicializa��o do mecanismo de aloca��o din�mica, incluindo 
 * configura��es de heaps, stacks, mmblocks ... 
 *
 * History:
 *    2015 - Created by Fred Nora.
 *    2016 - Some basic functions.
 *    ...
 */


#include <kernel.h>


//Vari�veis internas.
//int runtimeStatus;
//...


/*
 *********************************
 * init_runtime:
 *     Initializing run time. 
 *     Kernel heap and stack stuffs.
 *     @todo: Mudar para runtimeInit().
 */

int init_runtime (void){
	
	// Init Memory Manager:
	// Heap, Stack, Pages, mmblocks, memory sizes, memory zones ...
	// ?? onde fica ??

	init_mm ();
	
	
    //printf ("init_runtime: debug breakpoint, real machine, gigabyte/intel ..\n");
    //refresh_screen (); 
    //while(1){}	
	
	
	// Nessa hora a mem�ria j� funciona e o malloc tambem. e mensagens.
	//
	// o video esta usando ainda as configura��es de buffer e lfb faitas pelo boot loader.
	
	//
	//@todo: Suspensa a configura��es de p�ginas
	//       por enquanto fica tudo como foi configurado pelo bootloader.
	//
	
	
	//Cria o diret�rio de p�ginas do processo kernel, page tables e 
	//area para pageframes de mem�ria paginada.
	//pages.c

    SetUpPaging ();

	
    //printf ("init_runtime: 2 debug breakpoint, real machine, gigabyte/intel ..\n");
    //refresh_screen (); 
    //while(1){}

	
    // Continua ...
	
    g_module_runtime_initialized = 1;


    return 0; 
}


/*
 ********************************
 * KiInitRuntime: 
 *     Init runtime.
 *     +Clear bss segment.
 *     +Init heap, malloc.
 *     +...
 *     @todo: 
 *         Criar aqui, apenas a interface que chama a rotina 
 *         de inicializa��o da runtime.
 */

int KiInitRuntime (void){

    int Status = 0;

    debug_print("KiInitRuntime\n");


	//#todo 
	//preparar a tela para as mesagens;
	//mas somente se a flag de debug estiver acionada.

    Status = (int) init_runtime ();


	// #### importante ####
	// provavelmente aqui � o primeiro lugar onde as mensagens funcionam.
	
	//#bugbug
	//mas temos um problema no Y.

	//#bugbug
#ifdef EXECVE_VERBOSE
	backgroundDraw ( (unsigned long) COLOR_BLUE ); 
#endif


	//#todo:
	//podemos analisar o status aqui.
	
#ifdef BREAKPOINT_TARGET_AFTER_RUNTIME
    //#debug 
	//a primeira mensagem s� aparece ap�s a inicializa��o da runtime.
	//por isso n�o deu pra limpar a tela antes.
	printf (">>>debug hang: after runtime initialization");
	refresh_screen(); 
	while (1){ asm ("hlt"); };
#endif


    return (int) Status;
}


/*
int runtimeInit(){
	;
};
*/


//
// End.
//
