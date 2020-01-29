/*
 * File: main.c 
 *
 * Environment:
 *     grammgr
 *     ring 3.
 * 
 * Purpose:
 *     
 */
 
/*
    /grammgr - 
        Main program to manage the critical processes. 
        Those one that is essential to keed the system alive.
        
ps: 
It will not manage the ws and the wm, it's because ws and wm are 
part of the garden system. So /gramado is gonna live with a 
minimal window server (kgws), just enough to print chars in the screen.
                 
        grammgr.bin will initialize the ili environment. 
The ili environment is gonna initialize gws and gwm.

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

//fun��es internas.
void idleLoop();
void driverInitialize();      // processo sendo considerado um driver servidor.
void driverUninitialize();    // desinicializa.
int idleInit();               // processo sendo consideredo cliente.
unsigned long idleServices(unsigned long number);  //Principal.

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


//Another loop.
void idleLoop (){
	
    while (1){
		
		pause();
		pause();
		pause();
		pause();
		//pause();
		
		cpu_relax();
	}	
}




/*
 *********************************************************
 * driverInitialize:
 *     O Kernel solicitou a rotina de inicializa��o do processo Idle na forma 
 * de driver. Faremos uma chamada ao Kernel dizendo que o driver est� 
 * inicializado. Na chamada passaremos alguns par�metros, como: O n�mero do 
 * servi�o, 129 e o c�digo de confirma��o, 1234. 4321.
 */

void driverInitialize (){
	
	// Inicializando o servidor.
	ServerStatus = 1;
		
	//printf("Idle: Initializing driver ...\n");
	//system_call( 129, 4321, 4321, 4321 );	
	
	printf("IDLE.BIN: Initialized\n");
};


/*
 *********************************************************
 * driverUninitialize:
 *     This method is called to uninitialize the driver object. In a real 
 * driver, this is where the driver would clean up any resources held by 
 * this driver. (m$)
 */

void driverUninitialize (){
	
	// Finalizando o servidor.
	ServerStatus = 0;
	
	//
	// D�vidas??
	// Devemos nos preparar para desinicializar o driver.
	// Desalocaremos os recursos usadados pelo driver, como mem�ria, 
	// arquivos abertos ...
	// Obs: N�o sei se atribui��o do pr�prio driver liberar os recursos.
	// Mas de toda forma, no m�nimo devemos avisar o Kernel que os recursos 
	// n�o s�o mais necess�rios.
	// @todo: Podemos enviar um comando para o kernel pedindo pra que ele n�o 
	// link esse driver ao sistema, ou que deixe ele n�o inicializado.
	//

	//printf("Idle: Unitializing driver ...\n");
	
	//Faremos uma chamada para o Kernel 'deslinkar' o driver.
	//talvez 128. 127 126..???
	//system_call( ?? , 4321, 4321, 4321 ); 		
	
	printf("IDLE.BIN: Uninitialized\n");
};


/*
 *****************************************************************************
 * idleServices:
 *     Essa fun��o oferece servi�os de acordo com o n�mero passado via 
 * argumento.
 * Essa deve ficar por �ltimo e ter acesso � qualquer rotina acima ou em
 * bibliotecas inclu�das.
 */
unsigned long idleServices (unsigned long number){
  
    //
    //  ## O servidor precisa estar inicializado ##
    //
	
	// Checando se o servidor est� inicializado.
	if (ServerStatus != 1)
	{
		// Erro !
		return (unsigned long) 1;    
	};
	
	
	// Selecionar o servi�o.
	
    switch (number)
    {
		case 0:
		    printf("Idle Driver: NULL service.\n");
		    idleLoop();
			break;
			
		case 1:
		    printf("Idle Driver: service 1.\n");
		    //idleLoop();
			break;

        //...			
		
		default:
		    printf("Idle Driver: default service.\n");        
			//idleLoop();
			break;
	};	
	
    return (unsigned long) 0;	
}


/*
 * idleInit:
 *     Inicializando a aplica��o Idle.
 */

int idleInit (){
	
	idleStatus = 0;
	idleError = 0;
	
	//printf("Idle: Initializing idle application ..\n");

	//...
	return 0;
}


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
 
int main ( int argc, char *argv[] ){

    char runlevel_string[64];


    __debug_print ("grammgr: Initializing init process ...\n");

    //
    // Chamando a interrup��o 129.
    //

	//window, x, y, color, string.
    apiDrawText ( NULL, 
        0, 0, COLOR_GREEN, 
        "grammgr is alive!" );
        
     refresh_screen();
     while (1){}
 
     // ==== cut here ====
 
 
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
            __debug_print ("Gramado Core: RUNLEVEL 0\n");
            //do_reboot();
            break;

		// 1 	Single-user mode 	
		// Mode for administrative tasks.
        case 1:
            __debug_print ("Gramado Core: RUNLEVEL 1\n");
            //do_single_usermode();
            break;

		// 2 	Multi-user mode 	
		// Does not configure network interfaces and 
		// does not export networks services.
        case 2:
            __debug_print ("Gramado Core: RUNLEVEL 2\n");
            //do_mu_no_network();
            break;

		// 3 	Multi-user mode with networking 	
		// Starts the system normally.
        // Full multi-user text mode.
        case 3:
            __debug_print ("Gramado Core: RUNLEVEL 3\n");
            //do_mu_with_network();
            break;

		// 4 	Not used/user-definable 	
		// For special purposes.
        case 4:
            __debug_print ("Gramado Core: RUNLEVEL 4\n");
            //do_special();
            break;

		// 5 	Start the system normally with appropriate 
		// display manager (with GUI) 	
		// Same as runlevel 3 + display manager. 
		// Full multi-user graphical mode.
        case 5:
            __debug_print ("Gramado Core: RUNLEVEL 5\n");
            //do_mu_full ();
            break;    

		// 6 	Reboot 	Reboots the system. 
        case 6:
             __debug_print ("Gramado Core: RUNLEVEL 6\n");
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


    goto done;


//
// ===============================================
//


// fail 1.
fail1:

    // serial debug
    // __debug_print ("Gramado Core: Run level fail");
    
	//window, x, y, color, string.
    apiDrawText ( NULL, 
        0, 0, COLOR_YELLOW, 
        "Gramado Core: Run level fail" );


    // N�o sairemos, ficaremos no loop.
    // Isso porque o gramado core executar� um novo processo
    // usando o processo INIT.


done:

    __debug_print ("Gramado Core: init done.\n");

 
    while (1)
    {
        asm ("pause");
    }

    //
    // Not reached!
    //
    
    return (int) -1;
}



//
// End.
//

