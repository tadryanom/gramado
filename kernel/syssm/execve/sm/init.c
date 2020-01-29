/*
 * File: init.c 
 *
 * Descri��o:
 *     Arquivo principal do m�dulo INIT kernel.
 *     Faz inicializa��es.
 *     Esse m�dulo est� em kernel base.
 *
 * In this file:
 *    +init
 *    +init_test_disk
 *    +init_video
 *    +init_globals
 *
 * Hist�rico:
 *     Vers�o 1.0, 2015 - Esse arquivo foi criado por Fred Nora.
 *     Vers�o 1.0, 2016 - Aprimoramento geral das rotinas b�sicas.
 *     //...
 */ 

 
#include <kernel.h>


//Onde ficam os c�digos e arquivos de configura��o usados na inicializa��o.
//A ideia � que se a inicializa��o precisar de algum arquivo, deve procur�-lo
//nos diret�rios previamente combinados.
//melhor que sejam poucos e que sempre sejam os mesmos.

#define INIT_ROOT_PATH  "root:"
#define INIT_BIN_PATH   "root:/volume2/bin"
//...


//externs
//@todo:
//isso dever� ser definido em /gramado.
//extern char *GramadoSystemRoot;
//extern char *GramadoMajorVersion;
//extern char *GramadoMinorVersion;
//extern char *GramadoVersionString;
//...


//Vari�veis internas.
//int initStatus;
//...


/*
void init_set_current_runlevel ( int number )
void init_set_current_runlevel ( int number )
{
	//#todo: limits.
    current_runlevel = number;
}
*/

/*
int init_get_current_runlevel (void)
int init_get_current_runlevel (void)
{
    return (int) current_runlevel;
}
*/



/*
 * save_kernel_args:
 *     Salvando em estrutura os argumentos recebidos.
 */

void save_kernel_args (void){
	
    //KernelArgs.arg1 = ;
    //KernelArgs.arg2 = ;
	//KernelArgs.arg3 = ;
	//KernelArgs.arg4 = ;
}


/*
 ***********************************
 * init_architecture_dependent:
 *     Rotina de inicializa��o dependente da arquitetura atual da m�quina.
 *     A fase 1 identificou o tipo de processador. Aqui chamaremos a rotina 
 * de inicializa��o de acordo com o tipo identificado.
 *     A fase 1 inicializou a estrutura processor. Aqui checamos se ela � 
 * v�lida.
 *  GDT, IDT, ... TR ...
 *
 * Obs: Essa � a fase 2 de inicializa��o.
 *
 * Obs: Dependente significa dependente da marca do procesador.
 */

int init_architecture_dependent (void){
	
	int Status = 0;
	unsigned char Type;
	
	
	debug_print ("init_architecture_dependent\n");
	
	//
	// Fase. (Verificar se essa rotina foi chamada 
	// na fase certa de inicializa��o.)
    //
	
	if ( KeInitPhase != 1 )
	{
		panic ("init_architecture_dependent: KeInitPhase\n");	
	}


	// #### IMPORTANTE ####
	//
	// VAMOS ANTECIPAR ESSA INICIALIZA��O NA TENTATIVA DE
	// ANTECIPARMOS O USO DE MENSAGENS.
    // >>> mas essa rotina precisa do kmalloc ,,,
	//ent�o tem que ser depois da inicializa��o do stdio.
	
	
	// Os par�metros de tela dependem das propriedades de hardware
	// como monitor e placa de v�deo.
	
	//screenInit();

    //printf("init_architecture_dependent: #Debug");
    //refresh_screen();
    //while(1){};		
	
	
	//
	// * A estrutura para informa��es sobre o processador. 
	//
	
	// Check structure.
	if ( (void *) processor == NULL )
	{
	    panic ("sm-init-init_architecture_dependent: processor\n");
    }

	// Sonda pra ver qual � a marca do processador.
	// @todo: � a segunda vez que fazemos a sondagem ?!
	
	Type = (int) hal_probe_processor_type();
	
	//Type.
	processor->Type = (int) Type;
	
	
	//
	//Inicializa de acordo com o tipo de processador..
	//
	
	switch (Type)
	{
	    //Intel. (pega os par�metros do processador intel e coloca na estrutura).
	    case Processor_INTEL: 
		    init_intel(); 
			break;

        //AMD. (pega os par�metros do processador amd e coloca na estrutura).
	    case Processor_AMD: 
		    init_amd(); 
			break;

        //Continua ...
			
		//@todo: Aqui � um erro fatal.	
		default:
		    panic ("sm-init-init_architecture_dependent default Type\n");
            break;		
	};
	
	
	//
	// @todo: GDT, IDT (usar extern).
	//	
	
	
	// o contexto � depedente.
	//Inicializando o Process manager.
	init_process_manager ();




    //
    // Continua ...
    //
	
    // Done.
    
	return (int) Status;
}



/*
 ******************************************************************
 * init_architecture_independent:
 *
 *    Rotina de inicializa��o da parte do sistema que � independente da 
 * arquitetura presente. Independente da tipo de processador. 
 *    Obs: Essa � a fase 1 de inicializa��o.
 */

int init_architecture_independent (void){
	
    int Status;
	
	debug_print ("init_architecture_independent\n");
		
    if (KeInitPhase != 0)
	{
		panic ("init_architecture_independent: KeInitPhase\n");
	}; 

	
	//
	//  ## HAL ##
	//
    
	// #bugbug
	// Se � HAL � dependente da arquitetura.
	// Isso deveria ficar na outra rotina.
	// N�o mudaremos por enquanto.
	
#ifdef EXECVE_VERBOSE	
    printf ("init_architecture_independent: Initializing HAL..\n");
#endif		
	
	Status = init_hal ();	
	
	if (Status != 0)
	{
	    panic ("init-init_architecture_independent: init_hal\n");
	}

	
    //
	//  ## MICREOKERNEL ##
	//
	
	
	// Microkernel:
#ifdef EXECVE_VERBOSE
	// Obs: O Microkernel lida com informa��es dependentes da arquitetura,
	// por�m inicializa a gerencia de processos e threads e de comunica��o
	//entre processos.
	//#bugbug @todo: Se � microkernel � processo � registrador ... acho que leva em considera��o a arquitetura.
	printf ("init_architecture_independent: Initializing Microkernel..\n");
#endif	
	
	Status = init_microkernel ();
	
	if (Status != 0)
	{
	    panic ("init-init_architecture_independent: init_microkernel\n");
	}
	
	
	//
	// ## EXECUTIVE ##
	//
	
	
    // Executive:
#ifdef EXECVE_VERBOSE
	// Obs: O executive n�o � t�o dependente da arquitetura, ele �
	//uma camada mais alta, por�m ser� inicializado aqui para
	//efeito de ordem, j� que estamos inicializando os tres m�dulos
	//b�sicos do kernel base nesse momento.
	printf ("init_architecture_independent: Initializing Executive..\n");
#endif	
	
	Status = init_executive ();
	
	if (Status != 0)
	{
	    panic ("init-init_architecture_independent: init_executive\n"); 
	}
	
	//
	// ## GWS ?? ##
	//
	
	// Gramado:
#ifdef EXECVE_VERBOSE
    printf ("init_architecture_independent: Initializing Gramado..\n");
#endif
	
	Status = init_gramado ();
	
	if (Status != 0)
	{
	    panic ("init-init_architecture_independent: init_gramado\n"); 
	}
	
	
	
	
	
	/*
	//
	// TTY
	//
	
	
	// tty support.
	// As mensagens do kernel precisam usar esses par^ametros.
	// o kernel usa a tty0.
	
	//#importante
	//Logo antes user session, room e desktop.
	//Assim essas informa�~oes ficar~ao na estrutura de tty.
	//assim saberemos qual usu'ario est'a usando a tty0
	// deve ser o 'root'.
	
	//iniciar a tty 0.
	
	ttyInit (0);	
	*/

	
	
	
	
	/*
	// User Info:
	// =========
	//
	// @todo: Usu�rio e sess�o devem ser independentes do modo de v�deo. 
	//        Text ou Graphics.
	// @todo: Essas informa��es s�o independentes da arquitetura,
	//      Essa rotina pode ir pra outro lugar.
	
//UserInfo:
	
#ifdef EXECVE_VERBOSE	  
    printf ("init-init_architecture_independent: init_user_info\n");
#endif
	
	//initialize user info structure.
    init_user_info ();  
	*/
    
	// User Session, Window Station and Desktop.
	// @todo: Essas informa��es s�o independentes da arquitetura,
	//      Essa rotina pode ir pra outro lugar.
	
	
	
	/*
//UserSession:
	
#ifdef EXECVE_VERBOSE	
    printf ("init-init_architecture_independent: init_user_session\n");   
#endif
	
	//initialize user session.
	init_user_session ();   	 
	*/
	
	
	
	/*
	// (ROOM)
	
//WindowStation:
	
#ifdef EXECVE_VERBOSE
    printf ("init-init_architecture_independent: init_room_manager\n");   
#endif  
	
	init_room_manager ();
    */
	
	
	/*
	// DESKTOP
	
//Desktop:
	
#ifdef EXECVE_VERBOSE
    printf ("init-init_architecture_independent: init_desktop\n");   
#endif    
	
	init_desktop (); 
	*/
 
	
	//
	//  ## WINDOWS ##
	//
	
	// Window manager. 
	// (Inicializa janelas e cria o logon).
	// @todo: 
	// Essas informa��es s�o independentes da arquitetura,
	// Essa rotina pode ir pra outro lugar.
	
//WindowManager:	

	//Caso n�o se use gui.
	
	if (g_useGUI != 1)
	{
	    SetProcedure ( (unsigned long) &system_procedure);		
		
		// #BUGBUG
		// Aqui cacelamos a inicializa��o do window manager
		// caso n�o estejamos no modo gr�fico.
		// Como n�o temos suporte para modo texto, dever�amos abortar.
		
	}else{
        
		// Window manager.
#ifdef EXECVE_VERBOSE	    
		printf ("init-init_architecture_independent: init_window_manager\n");
#endif
		
		init_window_manager ();
	};
	
	
	
	// tty support.
	// As mensagens do kernel precisam usar esses par^ametros.
	// o kernel usa a tty0.
	//ttyInit ();

	
	
	//
	// terminal support.
	//
	
	//init_system_terminal();


    // Continua ...	 
	
	
	
done:
	
#ifdef EXECVE_VERBOSE
    //debug
    printf ("init_architecture_independent: Done\n");
	refresh_screen();
    //while(1){}
#endif	
	
    return 0;
}


/*
 **************************************************
 * Init globals: 
 *     Inicia vari�veis globais do Kernel Base.
 *     Obs: Inicializar por categorias.
 */

void init_globals (void){

#ifdef EXECVE_VERBOSE	
    printf("sm-init-init_globals:\n");
	debug_print("init_globals:\n");
#endif
	
	//Outros.
	errno = 0;
	
	//alocando mem�ria para as estruturas do fluxo padr�o.
	//#bugbug: vamos deixar que stdioInitialize fa�a isso.
	//stdin = (void *) kmalloc( sizeof(FILE) );
	//stdout = (void *) kmalloc( sizeof(FILE) );
	//stderr = (void *) kmalloc( sizeof(FILE) );
	
	//kstdin  = (void*) kmalloc( sizeof(FILE) );
	//kstdout = (void*) kmalloc( sizeof(FILE) );
	//kstderr = (void*) kmalloc( sizeof(FILE) );
	
    //inicializa as estruturas do fluxo padr�o.	
	stdioInitialize ();
	
	
	//#importante
	//>>> � a partir daqui que temos mensagens!!!
	
    screenInit ();
	
	//
	// ## FIRST MESSAGE !! ##
	//
	
	debug_print("init_globals: WE HAVE MESSAGES NOW !!!\n");
	printf("init_globals: first message!\n");
	
	
	//
	// Runlevel
	//
	
	// 5 	Start the system normally with appropriate 
	// display manager (with GUI) 	
	// Same as runlevel 3 + display manager.
	// Full multi-user graphical mode. 
	current_runlevel = 5;
	
    //===================	
	//vamos atrazar configura��o de janela em favor de configura��o de mensagem

	//Atalho para a pr�xima mensagem de teclado..(test) debug
	gNextKeyboardMessage = (int) 0;
	
	//Essa flag bloqueia e impede que uma janela obtenha o foco.
	gFocusBlocked = (int) 0;
	
	
	//
	// ==== Profiler ====
	//
	
	// See: include/kernel/profiler/pints.h
	
	// Interrup��o para servi�os do sistema.
	g_profiler_ints_gde_services = 0;

    // Legacy hardware interrupts (irqs) (legacy pic)
    g_profiler_ints_irq0 = 0;  
    g_profiler_ints_irq1 = 0;  
    g_profiler_ints_irq2 = 0;  
    g_profiler_ints_irq3 = 0;  
    g_profiler_ints_irq4 = 0;  
    g_profiler_ints_irq5 = 0;  
    g_profiler_ints_irq6 = 0;  
    g_profiler_ints_irq7 = 0;  
    g_profiler_ints_irq8 = 0;   
    g_profiler_ints_irq9 = 0;   
    g_profiler_ints_irq10 = 0;  
    g_profiler_ints_irq11 = 0;  
    g_profiler_ints_irq12 = 0;  
    g_profiler_ints_irq13 = 0;  
    g_profiler_ints_irq14 = 0;  
    g_profiler_ints_irq15 = 0;  



	//
	// As globais relativas � usu�rio s�o independentes do 
	// ambiente gr�fico.
	//
	
	//Group, User.
	current_group = 0;
	current_user  = 0;
	
	//User Session, Window Station, Dasktop.
	current_usersession = 0;
	current_room = 0;
	current_desktop = 0;
	
	//Process, Thread.
	current_process = (int) 0;
	current_thread  = (int) 0;	   
	
	// Kernel.
	kernel_request = 0;
	
	// System.
	// @todo:
	
	
	//Globais no ambiente GUI.
	if ( g_useGUI == 1 )
	{
		//N�O PRECISAMOS MAIS DISSO, DELETAR;
		//Pr�ximo procedimento de janela.
	    g_next_proc = (unsigned long) &system_procedure; 
		
		//Windows globals.
		current_usersession = (int) 0;    //>Current User Session.
		current_room = (int) 0;  //>>Current Window Station.
	    current_desktop = (int) 0;        //>>>Current Desktop.
		current_window = (int) 0;         //>>>>Current Window.
	    windows_count = (int) 0;          //Window count.
	    window_with_focus = (int) 0;      //Window with focus.
		current_menu = (int) 0;           //Current Menu.
        //Continua...	

		// windows, menus ...
		init_windows();
        init_menus();	
	
        //Continua...		
	};
		
	
	//Messages.
	g_nova_mensagem = 0;
	g_new_message = 0;



    // FS type.
    // type 1, fat16.
    g_filesystem_type = FS_TYPE_FAT16;    
    fatbits = (int) 16;



	//===================
	
	// #importante:
	// provavelmente isso altere o comportamento do cursor,
	// levando o cursor par ao in�cio da tela. Ent�o precisamos
	// repintar o background para recome�armos.
	
	//printf("#breakpoint glob");
    //refresh_screen(); 
    //while(1){}	
	
	//#bugbug isso esta dando problemas.
#ifdef EXECVE_VERBOSE
	backgroundDraw ( (unsigned long) COLOR_BLUE ); 
#endif

	
	//printf("#breakpoint glob");
    //refresh_screen(); 
    //while(1){}
	
	//
	// Keyboard support.
	//
	
	//ldisc_init_modifier_keys();
	//ldisc_init_lock_keys();	
	
	//Continua ...

};


/*
 ******************************************
 * init:
 *     Base initializations.
 *     (Four phases).
 *     Isso foi chamado por systemStartUp() em execve/sm/system.c
 *   
 *    #todo: 
 *    Descrever aqui as fazer dessa rotina.
 */ 

int init (void){
	
    int Status = 0;


	debug_print ("==== init:\n");
	
	//Check kernel phase.
	
	if ( KeInitPhase != 0 )
	{
		debug_print ("init: KeInitPhase fail\n");
		panic ("sm-init-init: KeInitPhase\n");
	}
	
	
	//
	// ### IMPORTANTE ###
	//
	
	
    //Globals.
	init_globals ();
	
	
	// #importante
	// � partir daqui podemos exibir strings na tela.
	

	
	
	
#ifdef EXECVE_VERBOSE	
	printf("sm-init-init: init_globals ok\n");     
#endif  
	
	
	//#bugbug:
	//Depois de iniciar as globais, provavelmente o cursor mude 
	//para o in�cio da tela.
	//ent�o ap�s iniciarmos as globais temos que repintar o background e 
	// fazer um refresh da tela se a flag de verbose estiver ligada.
	
    //Object manager.	
#ifdef EXECVE_VERBOSE	
	printf ("sm-init-init: init_object_manager\n");
#endif	
	
	init_object_manager();
	
	//i/o Manager.
#ifdef EXECVE_VERBOSE	
	printf("sm-init-init: ioInit\n");	
#endif	
	
	ioInit ();

    // Inicializa o gerenciamento de dispositivos.
    // Inicializa a lista de dispositivos.
    init_device_manager ();
	
    //
	// =================== ## STORAGE ## ===========================
	//
	
    // #ordem:
	// +storage
	// +disk
	// +volume 
	// +file system
	
	//#importante 
	//A estrutura 'storage' vai ser o n�vel mais baixo.
	//� nela que as outras partes devem se basear.
		
	storage = (void *) kmalloc ( sizeof(struct storage_d) );
	
	if ( (void *) storage == NULL )
	{
	    panic ("sm-init-init: storage");
	}	
	
		
#ifdef EXECVE_VERBOSE	
	printf("sm-init-init: disk_init\n");
#endif  
	
	disk_init ();
	
#ifdef EXECVE_VERBOSE	
	printf("sm-init-init: volume_init\n");
#endif
	
	volume_init ();
	
	
	
#ifdef EXECVE_VERBOSE	
	printf ("fsInit: VFS..\n");
#endif
	
	vfsInit ();
	
	
//deletar	
#ifdef EXECVE_VERBOSE	
	printf ("sm-init-init: fsInit\n");
#endif   
	
	fsInit ();
	    

#ifdef EXECVE_VERBOSE	
	printf("sm-init-init: initialize_system_message_queue\n");
#endif	
	
	initialize_system_message_queue (); 
    
    
	//
	// Network
	//
	
	networkInit ();
	
	
	
    //
	// Initialize Platform structure.
	//
	
#ifdef EXECVE_VERBOSE
	printf("sm-init-init: Platform\n");	
#endif

    // #important
    // This is the Root struct. :)

    Platform = (void *) kmalloc ( sizeof(struct platform_d) );

    if( (void *) Platform ==  NULL )
    {
        panic ("sm-init-init: Platform\n");	
    }else{

		//Hardware
        Hardware = (void *) kmalloc ( sizeof(struct hardware_d) );
		
	    if( (void *) Hardware ==  NULL )
		{
		    panic ("sm-init-init: Hardware\n");	
		}else{
		    Platform->Hardware = (void *) Hardware;
            //printf(".");			
		};
		
		//Firmware
	    Firmware = (void *) kmalloc ( sizeof(struct firmware_d) );
	    
		if( (void *) Firmware ==  NULL )
		{
		    panic ("sm-init-init: Firmware\n");
		}else{
		    Platform->Firmware = (void *) Firmware;
            //printf(".");  			
		};

		
		//System (software)
	    
		//
		// *IMPORTATE: Aqui estamos inicializando a estrutura do systema.
		//
		
		System = (void *) kmalloc ( sizeof(struct system_d) );
		
	    if( (void *) System ==  NULL )
		{
		    panic ("sm-init-init: System\n");
		}else{
			
			System->used = 1;    //Sinaliza que a estrutura esta em uso.
			System->magic = 1234; //sinaliza que a estrutura n�o esta corrompida.
			
		    Platform->System = (void *) System;
            //printf(".");			
		};
		
		//printf(" Done!\n");	
		//...
    };

	
	//Fase 1: Inicia a parte independente da arquitetura.
    Status = (int) init_architecture_independent ();
    if (Status != 0)
    {
        panic ("init: init_architecture_independent fail\n"); 
    }
	KeInitPhase = 1;
    
	//Fase 2: Inicia a parte de arquitetura especifica da m�quina atual.
	//        Ou seja, considera a marca do processador.
    Status = (int) init_architecture_dependent ();
    if (Status != 0)
    {
        panic ("init: init_architecture_dependent fail\n"); 
    }


	
	// #importante
	// So podemos carregar o diretorio raiz depois que inicializamos o 
    // controlador de IDE e as estruturas de sistema de arquivos.
	
	fs_load_rootdir ();
	
	
	KeInitPhase = 2;
	
	
    //#debug
   // printf ("init: *breakpoint\n"); 
   // refresh_screen();
   // while(1){}	
	
	//
	//  ==============  #### LOGON #### ===============
	//
	
	
	
	
	//printf("#breakpoint before logon");
	//refresh_screen();
	//while(1){}	
	
	//
	// Logon.
	//
	
	//Fase3: Logon. 
//Logon:

	//
	// Logon. 
    // Cria Background, main window, navigation bar.
    // de acordo com predefini��o.
    //

	if ( g_useGUI == 1 )
	{

#ifdef EXECVE_VERBOSE
	printf ("sm-init-init: calling create_logon ...\n");
#endif
		create_logon ();

#ifdef EXECVE_VERBOSE
	printf ("sm-init-init: calling init_logon ...\n");
#endif
		//Libera. (Aceita argumentos).
		init_logon (0,0);    

        //Obs: *IMPORTANTE Usa-se o procedimento de janela do Logon.
	};	
	KeInitPhase = 3; 
	

    //#debug
    //printf ("init: *breakpoint :) \n"); 
    //refresh_screen();
    //while(1){}


	// Continua ...
	

// * Fase 3:
// *     classe system.device.unblocked.
// *	   @todo: Inicializar dispositivos LPC/super io.
// *            Keyboard, mouse, TPM, parallel port, serial port, FDC. 
// *
 //* Fase 4:
 //*     classe system.device.unblocked.
 //*     @todo: Dispositivos PCI, ACPI ...
 //*
	
	
// Done.
//done:
    //printf("Done!\n");	
	//refresh_screen();
	//@todo: Deve retornar a vari�vel Status.
	
	
#ifdef BREAKPOINT_TARGET_AFTER_INIT
    //#debug 
	//a primeira mensagem s� aparece ap�s a inicializa��o da runtime.
	//por isso n�o deu pra limpar a tela antes.
	printf("debug hang: after init");
	refresh_screen(); 
	while (1){ asm ("hlt"); }
#endif


    debug_print ("==== init: done\n");

    return 0;  
}
 

//
// End.
//
