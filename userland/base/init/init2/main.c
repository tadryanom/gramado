/*
 * File: main.c 
 *
 * Environment:

 * 
 * Purpose:
 *     + To call interrupt 129 to enable maskable interrupts.
 *     + Hang forever. Some new process will reuse this process.
 * 

 *
 *     Esse programa dever� ser chamado sempre que o sistema estiver ocioso,
 * ou com falta de op��es vi�veis. Ent�o esse programa deve ficar respons�vel 
 * por alguma rotina de manuten��o do equil�brio de sitema, ou por ger�ncia de 
 * energia, com o objetivo de poupar energia nesse momento de ociosidade.
 *
 * O processo idle pode solicitar que processos de gerencia de energia entrem em
 * atua��o. Pois a chamada do processo idle em si j� � um indicativo de ociosidade
 * do sistema. Pode-se tamb�m organizar bancos de dados, registros, mem�ria, buffer,
 * cache etc.
 *
 *     O sistema pode configurar o que esse processo faz quando a m�quina 
 * est� em idle mode. Quando n�o h� nenhum processo pra rodar ou a cpu se 
 * encontra ociosa, pode-se usar alguma rotina otimizada presente neste 
 * programa. Parece que a intel oferece sujest�es pra esse caso, n�o sei.
 * 
 * Obs: O entry point est� em head.s
 *      Agora idle pode fazer systemcalls. 
 *
 * @todo: Criar argumento de entrada.
 *
 * Hist�rico:
 *     Vers�o 1.0, 2015 - Esse arquivo foi criado por Fred Nora.
 *     Vers�o 1.0, 2016 - Revis�o.
 *     ...
 */
 
 
//
// Includes.
// 
 
#include "init.h"

/*
 Example:
ID 	Name 	Description
0 	Halt 	Shuts down the system.
1 	Single-user mode 	Mode for administrative tasks.[2][b]
2 	Multi-user mode 	Does not configure network interfaces and does not export networks services.[c]
3 	Multi-user mode with networking 	Starts the system normally.[1]
4 	Not used/user-definable 	For special purposes.
5 	Start the system normally with appropriate display manager (with GUI) 	Same as runlevel 3 + display manager.
6 	Reboot 	Reboots the system.  
 */
int __current_runlevel;

//
// Vari�veis internas.
//

//Idle application support.
int idleStatus;
int idleError;
//...

//
//  ## Status do servidor INIT ##
//

int ServerStatus;
//...


/*
struct idle
{
	struct thread_d *current_idle_thread;
	//int
}
*/

//
// Prot�tipos.
//





//...
void enable_maskable_interrupts();


//
// ==========
//



//interna
void __debug_print (char *string)
{
    gramado_system_call ( 289, 
        (unsigned long) string,
        (unsigned long) string,
        (unsigned long) string );
}


static inline void pause (void){
	
    asm volatile("pause" ::: "memory"); 
}; 


/* REP NOP (PAUSE) is a good thing to insert into busy-wait loops. */
static inline void rep_nop (void){
	
	__asm__ __volatile__ ("rep;nop": : :"memory");
};


#define cpu_relax()  rep_nop()


 


// interna
// Uma interrup��o para habilitar as interrup��es mascar�veis.
// S� depois disso a interrup��o de timer vai funcionar.
void enable_maskable_interrupts()
{
    asm ("int $129 \n");
}

/*
 ********************** 
 * main:
 * 
 */

// See: /home/nora/gramado/kernel/arch/x86/entry/head/sw.asm 
 
// O objetivo � chamar a interrup��o 129.
// Uma interrup��o para habilitar as interrup��es mascar�veis.
// S� depois disso a interrup��o de timer vai funcionar.
 
int main3 ( int argc, char *argv[] );
int main3 ( int argc, char *argv[] ){

    char runlevel_string[64];


    __debug_print ("init2.bin: Initializing ...\n");

    //
    // Chamando a interrup��o 129.
    //

	//window, x, y, color, string.
    apiDrawText ( NULL, 
        0, 0, COLOR_YELLOW, 
        "init2.bin: Init is alive! Calling int 129" );
 
 
    //
    // Runlevel
    // 
 
    // Initialize with error value.
    __current_runlevel = (int) -1; 


    // Get the current runlevel.
    __current_runlevel = (int) gramado_system_call ( 288, 0, 0, 0 );  
 
 
    switch ( __current_runlevel )
    {
		// 0 	Halt 	
		// Shuts down the system. 
        case 0:
            __debug_print ("init: RUNLEVEL 0\n");
            //do_reboot();
            break;

		// 1 	Single-user mode 	
		// Mode for administrative tasks.
        case 1:
            __debug_print ("init: RUNLEVEL 1\n");
            //do_single_usermode();
            break;

		// 2 	Multi-user mode 	
		// Does not configure network interfaces and 
		// does not export networks services.
        case 2:
            __debug_print ("init: RUNLEVEL 2\n");
            //do_mu_no_network();
            break;

		// 3 	Multi-user mode with networking 	
		// Starts the system normally.
        // Full multi-user text mode.
        case 3:
            __debug_print ("init: RUNLEVEL 3\n");
            //do_mu_with_network();
            break;

		// 4 	Not used/user-definable 	
		// For special purposes.
        case 4:
            __debug_print ("init: RUNLEVEL 4\n");
            //do_special();
            break;

		// 5 	Start the system normally with appropriate 
		// display manager (with GUI) 	
		// Same as runlevel 3 + display manager. 
		// Full multi-user graphical mode.
        case 5:
            __debug_print ("init: RUNLEVEL 5\n");
            //do_mu_full ();
            break;    

		// 6 	Reboot 	Reboots the system. 
        case 6:
             __debug_print ("init: RUNLEVEL 6\n");
            //do_reboot();
            break;   
            
        // Fail!    
        default:
            goto fail1;
            break; 
    };
 
    itoa (__current_runlevel, runlevel_string);
 
 
     //#test
    apiDrawText ( NULL, 
        400, 0, COLOR_YELLOW, runlevel_string );        
        
        
     refresh_screen ();
     //while(1){} //debug


    //
    // Habilita as interrup��es mascaraveis.
    //
    
    enable_maskable_interrupts ();
    //asm ("int $129 \n");
    
    
    //
    // =============
    //
    
    
    
    // #debug
    // Testando a inicializa��o da libc usada pelo init.
    
    //printf ("init2: Testing printf ...\n");
    
    //fprintf (stdout,"init2: Testing stdout ...");
    //fflush (stdout);


    
    // Est� funcionando e lan�ando os filhos.
    //gramado_system_call (900, (unsigned long)"launcher.bin", 0, 0);        
    
    
    //funciona.
     //gramado_system_call (900, (unsigned long)"gwm.bin", 0, 0);        
    //gramado_system_call (900, (unsigned long)"gramcode.bin", 0, 0);   
    //gramado_system_call (900, (unsigned long) "gfe.bin", 0, 0);                
        
    //falha ao abrir o child.    
    //gramado_system_call (900, (unsigned long)"noraterm.bin", 0, 0); 
    //gramado_system_call (900, (unsigned long)"gws.bin", 0, 0);

    //fail
      gramado_system_call (900, (unsigned long)"gdeshell.bin", 0, 0);     
    //gramado_system_call (900, (unsigned long)"spr.bin", 0, 0); 
    //gramado_system_call (900, (unsigned long)"sysmon.bin", 0, 0);         
    
    

    //#testing
    //gramado_system_call (900, (unsigned long)"sysmon.bin", 0, 0);         




    //fail
    /*
    int Execve_Ret = -1;
    Execve_Ret = (int) gexecve ( "gwm.bin", 
                           (const char *) 0, 
                           (const char *) 0 );           

    if ( Execve_Ret != 0 )
    {
		printf ("init2.bin: gexecve fail\n");
    }
    */
    
    //#debug
   // while(1){}



        //yield. test
        gramado_system_call ( 265,0,0,0); 
        
// ===============================================
__loop:

    while (1){
        asm ("pause");
    }

    goto __loop;

// ===============================================
fail1:

    __debug_print ("init2: fail \n");

    // serial debug
    // __debug_print ("Gramado Core: Run level fail");
    
	//window, x, y, color, string.
    apiDrawText ( NULL, 
        0, 0, COLOR_YELLOW, 
        "init2: fail" );

    return -1;
}





int main ( int argc, char *argv[] ){

    __debug_print ("init2.bin: Initializing ...\n");

    apiDrawText ( NULL, 
        0, 0, COLOR_YELLOW, 
        "init2.bin: Init is alive! Calling int 129" );
    refresh_screen ();

    //
    // Habilita as interrup��es mascaraveis.
    //
    
    
    // #DEBUG
    // Olhando eflags.
    // asm ("int $3 \n");
    
    enable_maskable_interrupts ();
    //asm ("int $129 \n");
    

    // #DEBUG
    // Olhando eflags.
    //asm ("int $3 \n");


    

    
    gramado_system_call ( 900, 
        (unsigned long)"gdeshell.bin", 
        0, 
        0 );     


    // #todo
    // Testar na m�quina real, sem o pause.
 
	while (1){
		asm ("pause");
	}   
}


//
// End.
//

