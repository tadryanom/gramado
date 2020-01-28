/*
 * File: execve/sci/gde/gde_serv.c 
 *
 *       (SCI) = SYSTEM CALL INTERFACE
 *
 * Esse � o arquivo principal da sci.
 *
 *       GERENCIA OS SERVI�OS OFERECIDOS PELAS INTERFACES /sys.h E /system.h.   
 *
 * Obs: Todas as rotinas chmadas por esse m�dulo devem vir apenas das interfaces
 * /sys.h e /system.h.
 *
 *
 * In this file:
 *   + services (main function)
 *   + servicesPutChar
 *   + pty_send_message_to_thread
 *   + serviceCreateWindow
 *
 * History:
 *     2015 - Created by Fred Nora.
 *     2016 - Revision.
 *     2017 - Revision.
 *     //...
 */


#include <kernel.h>


//
// Constantes internas.
//

//#BUGBUG
// Esse m�ximo de servi�os para a interrup��o 200 n�o existir� mais.
// um n�mero grande de servi�os poder� ser atendido por essa interup��o,
// esses 255 servi�os s�o os servi�os que ser�o atendidos em kernel mode
// aqui no kernel base.
#define SERVICE_NUMBER_MAX 255


//
// Vari�veis internas.
//

//int servicesStatus;
//int servicesError;
//...

// Create Window support.
int cwFlag;                //flag (alerta que os argumentos est�o dispon�veis)
unsigned long cwArg1;      //WindowType
unsigned long cwArg2;      //WindowStatus
unsigned long cwArg3;      //WindowView
char *cwArg4;              //WindowName
unsigned long cwArg5;      //WindowX
unsigned long cwArg6;      //WindowY
unsigned long cwArg7;      //WindowWidth
unsigned long cwArg8;      //WindowHeight
struct window_d * cwArg9;  //gui->screen Parent window
int  cwArg10;               // desktopID 
unsigned long cwArg11;     // WindowClientAreaColor
unsigned long cwArg12;     // WindowColor


//
// ====
//

void *gde_extra_services ( unsigned long number, 
                     unsigned long arg2, 
                     unsigned long arg3, 
                     unsigned long arg4 );
                     
                     




void *gde_extra_services ( unsigned long number, 
                           unsigned long arg2, 
                           unsigned long arg3, 
                           unsigned long arg4 )
{

    // #todo
    // Talvez precisaremos disso e do argumento
    // que passe o endere�o do buffer.
    // unsigned long *msg = (unsigned long *) buffer;


	//generic file pointer
    FILE *__fp;

	//bmp file pointer.
    FILE *__bmfp;


    
    // read()   #todo;
    //usam a lista de arquivos abertos do processo. p->Streams[i]
    if ( number == 260 )
    {
        return (void *) sys_read ( 
                            (unsigned int) arg2, 
                            (char *) arg3, 
                            (int) arg4 );
    }
     
    
    
    // write() #implementada! (revisar!)
    //usam a lista de arquivos abertos do processo. p->Streams[i]
    if ( number == 261 )
    {
        return (void *) sys_write ( 
                            (unsigned int) arg2, 
                            (char *) arg3, 
                            (int) arg4 );
    }



    // read on virtual console!
    // range: 0 ~ 3
    // chamado por read_VC em ring3.
    // IN: fd, buf, count


    if ( number == 262 )
    {
        return (void *) console_read ( 
                            (int) arg2, 
                            (const void *) arg3, 
                            (size_t) arg4 );
    }

    // write on virtual console!
    // range: 0 ~ 3
    // chamado por write_VC em ring3.
    // IN: fd, buf, count
    
    if ( number == 263 )
    {
        return (void *) console_write ( 
                            (int) arg2, 
                            (const void *) arg3, 
                            (size_t) arg4 );
    }



    // yield the current thread.
    // O seu tempo rodando vai para perto do fim.
    if (number == 265)
    {
        yield (current_thread);
        return NULL;
    }


    // Pega o n�mero da tty de um processo, dado o pid.
    // IN: PID.
    // OUT: tty id.
    if (number == 266)
    {
        return (void *) process_get_tty ( (int) arg2 );
    }


    // Ligar duas tty dados os pids dos processos que possuem as tty.
    // IN: master pid, slave pid.
    if (number == 267)
    {
        return (void *) pty_link_by_pid ( (int) arg2, (int) arg3 );
    }
    



    // read_ttyList em ring3 vai chamar isso.
    // See: unistd.c em garden/lib/libcore.           
    if (number == 268)
    {
           // IN: fd, buf, count.         
           return (void *) tty_read ( (unsigned int) arg2,  //channel 
                               (char *) arg3,                //buf
                               (int) arg4 );                 //nr
    }

    // write_ttyList em ring3  vai chamar isso.
    if (number == 269)
    {
        // IN: fd, buf, count.         
        return (void *) tty_write ( (unsigned int) arg2,  //channel 
                            (char *) arg3,                //buf
                            (int) arg4 );                 //nr
    }


    //
    // Terminal support
    //
    
    // 270 ~ 276

    // Create a terminal given the window.
    // IN: window, father's pid.
    if ( number == 270 )
    {
         return (void *) vt_create ( (struct window_d *) arg2, (int) arg3 );
    }
    
    
    // Set child pid given in a vt structure.
    // Only the father will configurate this thing.
    // IN: window, child's pid
    if ( number == 271 )
    {
         return (void *) vt_set_child ( (struct window_d *) arg2, (int) arg3 );
    }



    // more terminal stuff.


    if (number == 277 )
    {
        return (void *) console_get_current_virtual_console ();
    }

    if (number == 278 )
    {
        //#todo: precisa de privil�gio.
        console_set_current_virtual_console ( (int) arg2 );
        return NULL;
    }
    

    // Update window.
    // Atualiza a �rea de cliente enviando uma mensage MSG_PAINT.
    // Isso funcionou.
    if ( number == 279 )
    {
        windowUpdateWindow ( (struct window_d *) arg2 );
        return NULL;
    }


    // Returns the current runlevel.
    if ( number == 288 )
    {
        return (void *) current_runlevel;
    }
    
    // Serial debug print.
    // See: sm/debug/debug.c
    if ( number == 289 )
    {
		debug_print ( (char *) arg2 );
        return NULL;
    }
    
    /*    
    // memory size
    // baseado no valor passado pelo bl.bin
    unsigned long __mm_size_mb = 0;
    if ( number == 292 )
    {
		//see gdef.h
		// ( (adress/1024)/1024) MB
        __mm_size_mb = ( (blSavedLastValidAddress/0x400) / 0x400 );
   
        return (void *) __mm_size_mb;
    }
    */

    unsigned long __mm_size_mb = 0;
    if ( number == 292 )
    {
        __mm_size_mb = ( memorysizeTotal/0x400);
        return (void *) __mm_size_mb;
    }


    // Updates a status bar of a given window.
    if ( number == 300 )
    {
        return (void *) UpdateStatusBar ( (struct window_d *) arg2, 
                            (unsigned char *) arg3, 
                            (unsigned char *) arg4 );
    }
    
    
    /*
    // Close a process, given it's pid and the exit code.
    if ( number == 333 )
    {
        //#todo: Criar um request que chamar� exit_process(...)
        return NULL;
    }
    */
    
    
    struct window_d *__window;
    
    //veja se essa � overlapped.
    if (number == 400)
    {
		//#bugbug todo: fazer checks
        
        __window = (struct window_d *) windowList[arg2];
        
        if ( __window->type == WT_OVERLAPPED )
        {
			return (void *) 1;
        }
        return (void *) 0;		
    }

    //pega o ponteiro de um janela.
    if (number == 401)
    {
		//#bugbug todo: fazer checks
        return (void *) windowList[arg2];
    }


    /*
	// 512 - get x server PID
    //rotina repetida
    if ( number == SYS_GET_X_SERVER )
    {
		return NULL; //#todo
        //return (void *) g_ws_pid;
    }
    */
    
    /*
	// 513 - set x server PID
    //rotina repetida
    if ( number == SYS_SET_X_SERVER )
    {
		//g_ws_pid = (int) arg2;
		return NULL;
    }
    */

    struct desktop_d *__desktop;
    
	// 514 - get wm PID
	// IN: desktop
    if ( number == SYS_GET_WM_PID )
    {
        // pega o wm de um dado desktop.
        __desktop = ( struct desktop_d *) arg2;
        if ( (void *) __desktop != NULL )
        {
            if ( __desktop->desktopUsed == 1 && 
                 __desktop->desktopMagic == 1234 )
            {
                return (void *) __desktop->wm; 
            }
        }
        return NULL; //#bugbug: Isso pode significar pid 0.
    }


	// 515 - set wm PID
	// IN: desktop, pid
    if ( number == SYS_SET_WM_PID )
    {
        __desktop = ( struct desktop_d *) arg2;
        if ( (void *) __desktop != NULL )
        {
            if ( __desktop->desktopUsed == 1 && 
                 __desktop->desktopMagic == 1234 )
            {
                 __desktop->wm = (int) arg3;
                return (void *) 1;  //ok 
            }
        }
        return NULL; //fail
    }



    // 516 - show x server info
    // ws para o desktop atual
    if ( number == SYS_SHOW_X_SERVER_INFO )
    {
        kprintf ("516: ws PID=%d \n", CurrentDesktop->ws);
        refresh_screen ();
        return NULL;
    }

    // 517 - show wm info
    // wm para o desktop atual
    if ( number == SYS_SHOW_WM_INFO )
    {
        kprintf ("window manager info: PID=%d \n", CurrentDesktop->wm);
        refresh_screen ();
        return NULL;
    }
    
    
    // Repinta todas as janelas que foram invalidadas.
    // Isso ser� usado pelo compositor do window server. 
    // Ou pelo window manager.
    if ( number == 518 )
    {
        redraw_screen();
        return NULL;
    }

    // get current desktop
    if (number == 519)
    {
        return (void *) CurrentDesktop;
    }

    // ??
    // setup net buffer for a process.
    struct process_d *__net_process;
    if (number == 550)
    {
        __net_process = (struct process_d *) processList[arg2];
        if ( (void *) __net_process != NULL )
        {
             if ( __net_process->used == 1 && 
                  __net_process->magic == 1234 )
             {
                 __net_process->net_buffer = (char *) arg3;
                 return (void *) 0;
             }
        }
        //?? fail
        return (void *) -1;
    }


	// 600 - dup
    if ( number == 600 )
    {
        return (void *) sys_dup ( (int) arg2 );
    }

	// 601 - dup2
    if ( number == 601 )
    {
        return (void *) sys_dup2 ( (int) arg2, (int) arg3 );
    }

	// 602 - dup3
    if ( number == 602 )
    {
        return (void *) sys_dup3 ( (int) arg2, (int) arg3, (int) arg4 );
    }

	// 605 - fileno
    if ( number == 605 )
    {
		return NULL;
        //return (void *) fileno ( (FILE *) arg2 );
    }

    // 606 - ungetc
    if ( number == 606 )
    {
		return NULL;
        //return (void *) ungetc ( (int) arg2, (FILE *) arg3 );
    }

	// 607 - fread
    if ( number == 607 )
    {
		return NULL;
		//return (void *) fread ( (void *) ptr, (size_t) 1, (size_t) n, (FILE *) fp );
        //return (void *) fread ( (void *) arg2, (size_t) 1, (size_t) arg3, (FILE *) arg4 );
    }


	//608 - fwrite
    if ( number == 608 )
    {
		return NULL;
        //size_t fwrite (const void *ptr, size_t size, size_t n, FILE *fp) 
        //return (void *) fwrite ( (const void *) arg2, 
          //                  (size_t) 1, 
            //                (size_t) arg3, 
              //              (FILE *) arg4 );
    }

	//609
    if ( number == 609 )
    {
		return NULL;
        //rewind ( (FILE *) arg2 );
        return NULL;
    }


	//610 - setbuf
    if ( number == 610 )
    {
		//void setbuf(FILE *stream, char *buf);
        //setbuf ( (FILE *) arg2, (char *) arg3 );
        return NULL;
    }

	//611 - setbuffer
    if ( number == 611 )
    {
		//void setbuffer(FILE *stream, char *buf, size_t size);
        //setbuffer ( (FILE *) arg2, (char *) arg3, (size_t) arg4 );
        return NULL;
    }

	//612 - setlinebuf
    if ( number == 612 )
    {
		//void setlinebuf(FILE *stream);
        //setlinebuf ( (FILE *) arg2 );
        return NULL;
    }


	//613 - setvbuf
	//#todo: precisamos do argumento size.
    if ( number == 613 )
    {
		return NULL;
		//int setvbuf(FILE *stream, char *buf, int mode, size_t size);
        //return (void *) setvbuf ( (FILE *) arg2, (char *) arg3, (int) arg4, (size_t) 512 );
    }


     //write
    //libc ssize_t write (int fd, const void *buf, size_t count)
    //#todo: essa rotina pode virar write(...) da libc
    //FILE *____stream;
    //struct process_d *____p;
    if ( number == 614 )
    {
		return NULL;
		//____p = (void *) processList[current_process];
		//____stream = (void *) ____p->Streams[arg2];
		
        // coloca no buffer dp fd do processo atual se o arquivo estuiver aberto.
        // obter um ponteiro de estrutura dado um fd.
        //return (void *) fwrite ( (const void *) arg3, 
          //                  (size_t) 0, 
            //                (size_t) arg4, 
              //              (FILE *) ____stream );
    }
    
    
    
	// 700 - atualiza o fluxo padr�o do processo atual
    if ( number == 700 )
    {
		return NULL;
		//stdio.c
        //return (void *) update_standard_stream ( (int) current_process, 
          //                  (FILE *) arg2, 
            //                (FILE *) arg3, 
              //              (FILE *) arg4 );
    }
    
    
    struct desktop_d *_Desktop;
    
	// 714 - get ws PID
	// IN: desktop
    if ( number == 714 )
    {
        // pega o ws de um dado desktop.
        _Desktop = ( struct desktop_d *) arg2;
        if ( (void *) _Desktop != NULL )
        {
            if ( _Desktop->desktopUsed == 1 && 
                 _Desktop->desktopMagic == 1234 )
            {
                return (void *) _Desktop->ws; 
            }
        }
        return NULL; //#bugbug: Isso pode significar pid 0.
    }


	// 715 - set ws PID
	// IN: desktop, pid
    if ( number == 715 )
    {
        _Desktop = ( struct desktop_d *) arg2;
        if ( (void *) _Desktop != NULL )
        {
            if ( _Desktop->desktopUsed == 1 && 
                 _Desktop->desktopMagic == 1234 )
            {
                 _Desktop->ws = (int) arg3;
                return (void *) 1;  //ok 
            }
        }
        return NULL; //fail
    }

    
 
 
    if (number == 770)
    {
		systemShowDevicesInfo ();
        return NULL;
    }
    
    // cpu usage for idle thread.
    if (number == 777)
    {
		return (void *) profiler_percentage_idle_thread;
    }
    
 
    /*
    //arp request test.
    if ( number == 800 )
    {   
		return (void *) 0;
    }
    */

    
    //get host name
    if ( number == 801 )
    {
		return (void *) __gethostname ( (char *) arg2);
    }

    //set host name
    if ( number == 802 )
    {
		return (void *) __sethostname ( (const char *) arg2); 
    }

    //get user name
    if ( number == 803 )
    {
		return (void *) __getusername ( (char *) arg2);
    }

    //set user name
    if ( number == 804 )
    {
		return (void *) __setusername ( (const char *) arg2); 
    }


    // #todo
    // supporting ptsname libc function
    // get_ptsname
    // IN: fd do master, buffer em ring3 para o nome, buflen
    //
    if ( number == 808 )
    {
		return (void *) __ptsname ( (int) arg2, (char *) arg3, (size_t) arg4  ); 
    }
    
    //#todo
    //supporting ptsname_r libc function
    //IN: fd do master, buffer e buflen.
    if ( number == 809 )
    {
		return (void *) __ptsname ( (int) arg2, (char *) arg3, (size_t) arg4  ); 
    } 
    
    
    // copia o prompt[] do terminal para o buffer do tty atual.
    //size_t __prompt_size = 0;
    if ( number == 810 )
    {
        //__prompt_size = (size_t) strlen (__prompt_size) + 1
        
        //copiando.
        //strcpy ( CurrentTTY->stdout->_base, (const char *) arg3 );
        memcpy ( (void *) CurrentTTY->stdout->_base, (const void *) arg3, 64 ); 
        return NULL;
    }
    
    // copia do buffer do tty atual para o buffer do child do terminal.
    if ( number == 811 )
    {

        //copiando.
        //strcpy ( arg3,  (const char *) CurrentTTY->stdout->_base );
        memcpy ( (void *) arg3, (const void *) CurrentTTY->stdout->_base, 64 );         
        return NULL;
    }
    
    // Get process stats given pid
    // IN: pid, number
    if ( number == 880 )
    {
       return (void *) __GetProcessStats ( (int) arg2, (int) arg3 );
    }

    // get thread stats given tid
    // IN: tid, number
    if ( number == 881 )
    {
        return (void *) __GetThreadStats ( (int) arg2, (int) arg3 );
    }



    //get process name
    if ( number == 882 )
    {
		return (void *) getprocessname ( (int) arg2, (char *) arg3);
    }

    //get thread name
    if ( number == 883 )
    {
		return (void *) getthreadname ( (int) arg2, (char *) arg3);
    }
 
 
 
 
 
	// t900
	// Clona e executa o filho dado o nome do filho.
	// Isso � usado pelo terminal virtual. (noraterm)
	// See: See: ps/action/process.c
	
    if ( number == 900 )
    { 
        return (void *) do_clone_execute_process ( (char *) arg2 );
    }


	// t901
	// Clona um processo, retorna para o pai e inicializa o processo.
	// Filho do seu entrypoint. (#test)
	// See: ps/action/process.c

    if ( number == 901 )
    {
        return (void *) do_fork_process2 ();
    }
    
    // get screen window.
    // #todo. checar validade
    if ( number == 955 )
    {
        return (void *) gui->screen;
    }    

    if ( number == 956 )
    {
        return (void *) gui->background;
    } 
    
    // get main window.
    // #todo. checar validade
    if ( number == 957 )
    {
        return (void *) gui->main;
    }    

    //o processo est� se comunicando com o driver de rede.
    //um descritor de soquete � enviado via argumento.
    if ( number == 966 )
    {
		return NULL;
        //return (void *) network_procedure ( NULL,
          //              (int) 1000,//enviando descritor para atualizar a stream
            //            (unsigned long) arg2,   //fd
              //          (unsigned long) arg3 );  //fd
    }


    //o processo est� se comunicando com o driver de rede.
     //um descritor de soquete � enviado via argumento.
    if ( number == 967 )
    {
        return (void *) network_procedure ( NULL,
                        (int) 2000, //envie dados para o processo.
                        (unsigned long) arg2,   //fd
                        (unsigned long) arg3 );  //fd
    }


    // A interrup��o n�o conseguir� retornar para a mesma thread.
    // Chamar� o scheduler por conta pr�pria;
    // IN: reson, reason
    if ( number == 970 )
    {
            sys_create_request ( (unsigned long) 15, // number 
                (int) 1,                             // status 
                (int) 0,                             // timeout. 0=imediatamente.
                (int) current_process,               // target_pid
                (int) current_thread,                // target_tid
                NULL,                                // window 
                (int) 0,                             // msg  
                (unsigned long) arg2,                // long1  
                (unsigned long) arg3 );              // long2
                
		//wait_for_a_reason ( current_thread, (int) arg2 );
        return NULL;
    }


	// t18
	// O arg2 � o PID.
	// Devemos retornar o ponteiro para o stdout do terminal
	// associado com o aplicativo.
    if ( number == 1000 )
    {
        return (void *) CurrentTTY->stdout;
    }


    if ( number == 1001 )
    {
		//printf ("service1001: Configurando CurrentTTY->stdout \n",
		  //  arg2 );
		//CurrentTTY->stdout = (FILE *) arg2;
		//stdout = (FILE *) arg2;
        return NULL;
    }


	// Pega um char na stream, 
	// o �ltimo que foi pego CurrentTTY->stdout,
	// n�o o �ltimo que foi colocado.
	// #importante: uma no terminal deve chamar isso.
	// mas o terminal precisa ser alertado de que tem mensagens
	//para ele o tty, ent�o modemos enviar uma MSG_TTY?? vaisando que
	//o terminal pode pegar sua mensagem na tty atrav�s dessa chamada aqui.


    // N�O enviaram uma stream. 

    int __xxx_ch;
    if ( number == 1002 )
    {
        __xxx_ch = (int) *CurrentTTY->stdout_last_ptr;

		//apaga.
        *CurrentTTY->stdout_last_ptr = 0;

		//incrementa e circula
        CurrentTTY->stdout_last_ptr++;
        if ( CurrentTTY->stdout_last_ptr >= CurrentTTY->stdout_limit )
        {
            CurrentTTY->stdout_last_ptr = CurrentTTY->stdout->_base;
        }

		//retorna o que pegou.
        return (void *) __xxx_ch;
    }



    // #importante
    // Set terminal PID for the current TTY.
    if ( number == 1003 )
    {
        CurrentTTY->terminal_pid = (int) arg2;
        return NULL;
    }


    // #importante
    // Set terminal PID for the current TTY.
    if ( number == 1004 )
    {
        return (void *) CurrentTTY->terminal_pid;
    }

    // #importante
    // Isso � usado por terminal/child por enquanto.
    // Inicializa o stdout da CurrentTTY.
    // #bugbug: Isso est� fazendo o mesmo que 1001.
    if ( number == 1005 )
    {
		//#todo: Checar a validade
        CurrentTTY->stdout = stdout;
        //fprintf (stdout, "dirty\n"); //debug
        return NULL;
    }


	 //get sdtout
    if ( number == 1006 )
    {
		//#todo: Checar a validade
        return (void *) CurrentTTY->stdout;
        //return (void *) stdout;
    }


    //#todo: Testar isso.
    /*
    // Agora vamos ler de uma tty indicada pelo processo.
    int __xxx_ch;
    int __tty_id;
    if ( number == 1007 )
    {
		//
		__tty_id = (int) arg2;
		CurrentTTY = (struct tty_d *) ptsList[ __tty_id ]
		
		//pega
        __xxx_ch = (int) *CurrentTTY->stdout_last_ptr;

		//apaga.
        *CurrentTTY->stdout_last_ptr = 0;

		//incrementa e circula
        CurrentTTY->stdout_last_ptr++;
        if ( CurrentTTY->stdout_last_ptr >= CurrentTTY->stdout_limit )
        {
            CurrentTTY->stdout_last_ptr = CurrentTTY->stdout->_base;
        }

		//retorna o que pegou.
        return (void *) __xxx_ch;
    } 
    */

    // retorna o id da tty atual.
    if (number == 1008)
    {
        return (void *) CurrentTTY->index;
    }
    
    /*
    // retorna o id da tty do processo atualatual.
    struct process_d * __P;
    if (number == 1009)
    {
		__P = (struct process_d *)processList[current_process];

        return (void *) __P->tty->index;
    }
    */


	 // bmp:
	 // Carrega o bmp na mem�ria e decodifica no backbuffer.
	 // Isso n�o mostra na tela.
    if ( number == 4000 )
    {
		// load and show bmp
		//sys_fopen ( (const char *) arg2, (const char *) arg3 );
		//__bmfp = sys_fopen ( "folder.bmp", "r+" );
        __bmfp = sys_fopen ( (const char *) arg2, "r+" );

		 //decodificando no backbuffer.
		 bmpDisplayBMP ( __bmfp->_base, arg3, arg4 );

		//refresh_rectangle ( 100, 100, 200, 200 ); 
        return NULL;
    }

	// bmp:
	// Carrega o bmp na mem�ria e decodifica diretamente no frontbuffer.
	// Isso mostra na tela.
    if ( number == 4001 )
    {
		// load and show bmp
		//sys_fopen ( (const char *) arg2, (const char *) arg3 );
		//__bmfp = sys_fopen ( "folder.bmp", "r+" );
        __bmfp = sys_fopen ( (const char *) arg2, "r+" );

		//bmp_change_color_flag = BMP_CHANGE_COLOR_TRANSPARENT;
		//bmp_selected_color = COLOR_WHITE;
        bmpDirectDisplayBMP ( __bmfp->_base, arg3, arg4 );
		//bmp_change_color_flag = 0;
		//bmp_selected_color = 0;

        return NULL;
    }

     //api - load file (string ???)
     //#todo: Tem que retornar algum identificador para a api.
     //poderia ser um indice na tabela de arquivos abertos pelo processo.
    if ( number == 4002 )
    {
        return (void *) sys_fopen ( (const char *) arg2, "r+" );
    }


    //api - load file (string ??)
    //#todo: Tem que retornar algum identificador para a api.
    //#bugbug: Podemos retornar o endere�o base. Isso pode dar problemas?
    if ( number == 4003 )
    {
        __fp = sys_fopen ( (const char *) arg2, "r+" );
        return (void *) __fp->_base;
    }
    


    
    //socket() la libc.
    //chamaremos a rotina na crts/klibc
    //family, type, protocol
    if ( number == 7000 )
    {
        return (void *) socket ( (int) arg2, (int) arg3, (int) arg4 );
    }

    //See: execve/sm/sys/io.c
    //#bugbug: rever os arguentos, principalmente o �ltimo.
    // O primeiro argumento seleciona um dispositivo.
    // J� que � um descritor de arquivos e os dispositivos
    // s�o representados por arquivos.
    if ( number == 8000 )
    {
		//fd, request, arg
		return (void *) sys_ioctl ( (int) arg2, 
		                            (unsigned long) arg3, 
		                            (char *) arg4 );
    }


    //test
    //raise window.
    if ( number == 9700 )
    {
		return (void *) raise_window ( (struct window_d *) arg2 );
    }

    //ps2 mouse controller dialog
    // msg, long1, long2
    if ( number == 9800 )
    {
		return (void *) ps2_mouse_dialog ( (int) arg2, 
		                    (unsigned long) arg3, 
		                    (unsigned long) arg4 );
    }
    
    //button down
    //quando um bot�o � clicado ou pressionado,
    //ele ser� repintado com a apar�ncia de bot�o apertado.
    if ( number == 9900 )
    {
		return (void *) button_down ( (struct window_d *) arg2 );
    }

    //#todo button_up
    if ( number == 9901 )
    {
		return (void *) button_up ( (struct window_d *) arg2 );
    }
 
    // chamado por gde_get_pid na api.
    // See: system.c
    if ( number == 9999 )
    {
        return (void *) system_get_pid ( (int) arg2 );
    }

//fail
    return NULL;  
}



/*
 ****************************************
 * gde_services:
 *     Rotina que atende os pedidos feitos pelos aplicativos em user mode 
 *     via int 200. Ou ainda o kernel pode chamar essa rotina diretamente.
 *     S�o v�rios servi�os.
 *
 *
 * Obs: 
 * TODOS OS SERVI�OS DESSA ROTINA PODEM CHAMAR ROTINAS DE CLASSES 'system.x.x'.
 *
 * @todo: 
 *    Pode haver algum tipo de autoriza��o para essa rotina.
 *    Ou ainda, autoriza��o por grupo de servi�os. Sendo os servi�os
 *    de i/o os de maior privil�gio.
 *
 *    +Identificar o processo que fez a chamada e i/o e
 *    configurar a vari�vel que identifica esse processo.
 *    'caller_process_id' � uma vari�vem encapsulada em process.c
 *    'set_caller_process_id(int pid)' configura a vari�vel.
 *
 *    Uma chamada como OpenDevice pode liberar o acesso
 *    para um determinado processo e CloseDevice cancela o acesso.  
 * 
 *    @todo: O Nome da fun��o, para pertencer � essa pasta, deveria ser:
 *           servicesSystemServices(....).
 *
 *@todo: *importante: Essa rotina deve identificar quem est� chamando, PID TID.
 *
 *
 * *IMPORTANTE:
 *  DEPENDEND DO N�MERO DO SERVI�O ELE PODER� SER TRATADO EM OUTRO ARQUIVO
 *  E N�O NO KERNEL BASE.
 */

void *gde_services ( unsigned long number, 
                     unsigned long arg2, 
                     unsigned long arg3, 
                     unsigned long arg4 )
{
	//
	// Declara��es.
	//


    // array de longs.
    unsigned long *a2 = (unsigned long*) arg2;
    unsigned long *a3 = (unsigned long*) arg3;
    unsigned long *a4 = (unsigned long*) arg4;


	//Args. (strings)
    unsigned char *stringZ = (unsigned char *) arg2;
    unsigned char *stringZZ = (unsigned char *) arg3;

    //para strings
    char *aa2 = (char *) arg2;
    char *aa3 = (char *) arg3;
    char *aa4 = (char *) arg4;



    char *tokenList[8];

    *tokenList = (char *) arg3;


	//Char and string support.
    char *argChar;
    unsigned char* argString;

	//Retorno.
    void *Ret;


	//Function(CreateWindow)
	//arg2 =Type
	//arg3=Status
	//arg4=Name
	
	//Window.
    unsigned long WindowType;      //Tipo de janela.
    unsigned long WindowStatus;    //Status, ativa ou n�o.
    unsigned long WindowRect;
    unsigned long WindowView;      //Min, max.
    char *WindowName;            


	//
	// Window stuffs:
	//

    int ActiveWindow;          //Current id.

    struct window_d *hWnd;
	//struct window_d *cWnd;     //Current window.
	//struct window_d *aWnd;     //Active Window. 
    struct window_d *focusWnd;   //Janela com foco de entrada.
    struct window_d *NewWindow;  //Ponteiro para a janela criada pelo servi�o.


    //void *kHWND;
    void *kMSG;
    //void *kLONG1;
    //void *kLONG2;
	//...


	//Para identificarmos qual processo e qual thread est� chamando.
	//struct window_d *Thread;
    struct process_d *p;
    struct thread_d *t;


	//generic file pointer
    FILE *__fp;

	//bmp file pointer.
    FILE *__bmfp;


	//
	// Setup.
	//

	//Window.
    hWnd = (void *) arg2;


	// #todo: 
	// Antes de utililizar as dimens�es da �rea de trabalho
	// vamos atribuir valores negligenciados para as dimens�es.
	
	//Inicializando dimens�es.
	//Obs: Emulando telas de celulares.
	// 320x480.
	// Terminal abaixo dos di�logos.
    unsigned long WindowX = (2*(800/20));  //100;   >  
    unsigned long WindowWidth  = 320;  

    unsigned long WindowY = (2*(600/20)); //100;   V  
    unsigned long WindowHeight = 480;  

    unsigned long WindowColor = COLOR_WINDOW;  
    unsigned long WindowClientAreaColor = COLOR_WINDOW;  

    struct rect_d *r;




	//
	// ## message support ##
	//

	//o endere�o do array passado pelo aplicativo
	//usaremos para enviar uma mensagem com 4 elementos.
    unsigned long *message_address = (unsigned long *) arg2;

    unsigned char SC;
	//struct window_d *wFocus;

    int desktopID;



    int __len;  //usado para strings.
    int __i;    //usado na string
    
    //
    // Profiler
    //
    
    // Contando as interrup��es desse tipo.
    g_profiler_ints_gde_services++;



	//
	// ================================
	// save context.
	//


    //save_current_context ();
    //Current->saved = 1; 	//#todo

	//
	// ================================
	// Color scheme.
	//
	

	// *Importante: 
	// Checando se o esquema de cores est� funcionando.

    if ( (void *) CurrentColorScheme == NULL )
    {
		panic ("gde_services: CurrentColorScheme");
		//die ();
    }else{

		if ( CurrentColorScheme->used != 1 || 
		     CurrentColorScheme->magic != 1234 )
		{
		    panic ("gde_services: CurrentColorScheme validation");
		    //die ();
		}
		//Nothing.
    };


	//Configurando cores.
	WindowColor = CurrentColorScheme->elements[csiWindowBackground];  
	WindowClientAreaColor = CurrentColorScheme->elements[csiWindow]; 	
	
	 
	desktopID = (int) get_current_desktop_id ();
	
	
	// #bug
	// Se a �rea de trabalho for v�lida, usaremos suas dimens�es, 
	// caso n�o seja, temos um problema.

    if (gui->main == NULL)
    {
        panic ("gde_services: gui->main");
    }



    //
    // ===============  Special ============
    //

    if ( number > 256 )
    {
        return (void *) gde_extra_services ( number, arg2, arg3, arg4 );
    }
    
    
    //
    // ===============  Switch ============
    //
    

	// 0-256
    switch (number)
    {
	    //0 - Null, O processo pode ser malicioso.
	    case SYS_NULL: 
	        return NULL;
			break; 

		//1 (i/o) Essa rotina pode ser usada por um driver em user mode.
		case SYS_READ_LBA: 
			sys_my_read_hd_sector ( (unsigned long) arg2, 
			    (unsigned long) arg3, 0 , 0 ); 
			break;

		//2 (i/o) Essa rotina pode ser usada por um driver em user mode.
		case SYS_WRITE_LBA: 
			sys_my_write_hd_sector ( (unsigned long) arg2, 
			    (unsigned long) arg3, 0 , 0 ); 
		    break;

		//3 fopen (i/o)
		//Carregar um arquivo, dado o nome e o endere�o.
		//#obs: chamar rotinas de interface sys_ simplificam 
        //o atendimento �s system calls.
		// A fun�ao esta' em fs/fs.c
		//IN: name. address
		case SYS_READ_FILE:
			
			//funciona, nao mexer nesse
		    //return (void *) sys_read_file ( (unsigned long) a2, (unsigned long) arg3 );
			
			//#importante: estamos usando esse.
			//testando novos recursos,
			return (void *) sys_read_file2 ( (unsigned long) a2, 
			                    (unsigned long) arg3 );
			break;


		//4 (i/o)
        case SYS_WRITE_FILE:
            taskswitch_lock ();
            scheduler_lock ();

            sys_fsSaveFile ( (char *) message_address[0],    //name
                (unsigned long) message_address[1],  //3, //@todo: size in sectors 
                (unsigned long) message_address[2],  //255, //@todo: size in bytes
                (char *) message_address[3],         //arg3,//address
                (char) message_address[4] );         //,arg4 ); //flag

            scheduler_unlock ();
            taskswitch_unlock ();
            break;

		// 5 Vertical Sync. 
		// Usada pelos servidores.
		// #todo: chamar kgws_vsync ()
        case SYS_VSYNC:
            sys_vsync (); 
            break;


        // 6 - Put pixel. 
        // Coloca um pixel no backbuffer.
        // Isso pode ser usado por um servidor. 
        // cor, x, y, 0.
		// todo: chamar kgws_backbuffer_putpixel
        case SYS_BUFFER_PUTPIXEL:
            sys_backbuffer_putpixel ( (unsigned long) a2, 
                (unsigned long) a3, (unsigned long) a4, 0 ); 
            break;

		// 7
		// desenha um char no backbuffer.
		// Obs: Esse funciona, n�o deletar. :)
		// (x,y,color,char)
		// (left+x, top+y,color,char)
		// devemos usar o left e o top da janela com foco de entrada.
        //
		// Obs: A biblioteca c99 em user mode usa essa chamada para pintar um caractere
		// quando implementa a fun��o printf(.). Por�m esse caractere � pintado
		// na janela com o foco de entrada. A inten��o � que o carctere seja pintado 
		// na pr�pria janela do aplicativo.
		// Outro caso � a digita��o de caractere. Quando um caractere �
		// digitado desejamos que ele seja pintado na janela com o foco de entrada.
		//
		// ?? Quando um caractere � pintado em uma janela que n�o est� com o foco 
		//    de entrada ?? ex: shells, logs ...
		//
		// Supondo que os aplicativos escrever�o mais em terminal por enquanto 
		// a cor padr�o de fonte ser� a cor de terminal.
		
		// #importante
		// Aqui est� pintando o caractere na janela com o foco de entrada.

        case SYS_BUFFER_DRAWCHAR:
			focusWnd = (void *) windowList[window_with_focus];
			if ( (void *) focusWnd == NULL )
			{ 
			    break; 
			};
			
			//x, y, color, char.
            sys_my_buffer_char_blt ( (unsigned long) (focusWnd->left + arg2),             
                (unsigned long) (focusWnd->top + arg3),  
                CurrentColorScheme->elements[csiTerminalFontColor],   
                (unsigned long) arg4 ); 
            break;

		//8 @todo: BugBug, aqui precisamos de 4 par�metros.
        case SYS_BUFFER_DRAWLINE:
            sys_my_buffer_horizontal_line ( (unsigned long) a2, 
                (unsigned long) a3, (unsigned long) a4, COLOR_WHITE ); 
            break;

		//9 @todo: BugBug, aqui precisamos de 5 par�metros.
        case SYS_BUFFER_DRAWRECT:
            sys_drawDataRectangle ( 0, (unsigned long) a2, (unsigned long) a3, 
                (unsigned long) a4, COLOR_WHITE );  
            break;

		// 10 Create window.
        // # suspensa.
        case SYS_BUFFER_CREATEWINDOW: 
            return NULL;
            break;


		//11, Coloca o conte�do do backbuffer no LFB.
        case SYS_REFRESHSCREEN: 
            sys_refresh_screen ();
            break;

        //rede: 12,13,14,15	
			
			
		//i/o:  16,17,18,19	
			
		// open()
		// Retorna um i'ndice na tabela de arquivos abertos do proceso atual.
		// unistd.c
		// O tipo mode_t precisa de kernel/sys/types.h
		// pathname, flags, mode
        case 16:
            return (void *) sys_open ( (const char *) arg2, 
                                (int) arg3, (mode_t) arg4 );
            break;


        case 17:
            return (void *) sys_close ( (int) arg2 );
            break;


		//24
		// window.c
        case 24:
            return (void *) sys_show_window_rect ( (struct window_d *) arg2 );
            break;

        //livre
        //case 33:
            //break;


        // 34 - Setup cursor.
        case SYS_VIDEO_SETCURSOR: 
            sys_set_up_cursor ( (unsigned long) arg2, 
                (unsigned long) arg3 );
            return NULL;
            break; 


        // #todo: 
        // 35 - Getting message from the thread's queue. 
        // Vamos pegar uma mensagem na fila de mesagens da thread
        // e colocarmos nos elementos de single message para o aplicativo pegar.
        // See: ps2kbd.c
        case 35:
            return (void *) __do_35 ( (unsigned long) &message_address[0] );
            break;


		//36
        //O teclado envia essa mensagem para o procedimento ativo.
        case SYS_KSENDMESSAGE: 
		    g_nova_mensagem = 1;    //flag = Existe uma nova mensagem.
            sys_system_dispatch_to_procedure ( (struct window_d *) arg2, 
			    (int) arg2, (unsigned long) arg4, (unsigned long) 0);
            break;    
      
		//37 - Chama o procedimento procedimento default. 
		// #todo
		// Vamos passar na forma de mesagens para que tenhamos quatro argumentos.
		case SYS_CALLSYSTEMPROCEDURE: 
			return (void *) sys_system_procedure ( (struct window_d *) message_address[0], 
							    (int) message_address[1], 
								(unsigned long) message_address[2], 
								(unsigned long) message_address[3] );
            break;    
        
        //42 - Load bitmap 16x16. #cancelada.
        case SYS_LOADBMP16X16 :  
            return NULL;
            break;

        // 45 - Message Box. #cancelada 
        case SYS_MESSAGEBOX:
            return NULL;
			break;
		
        //47, cancelada.		
        case SYS_BUFFER_CREATEWINDOWx:
			return NULL;
			break;
		
		//48, cancelada.
		case SYS_BUFFER_CREATEWINDOW1:
			return NULL;
			break;
		
		//49, cancelada.
		case SYS_BUFFER_CREATEWINDOW2:
			return NULL;
			break;	

		//50 resize window (handle,x,y)
		case SYS_BUFFER_RESIZEWINDOW:
		    return (void *) sys_resize_window ( (struct window_d *) arg2, arg3, arg4);
		    break;
		
		//51 redraw window. (handle)
		case SYS_BUFFER_REDRAWWINDOW:
		    return (void *) sys_redraw_window ( (struct window_d *) arg2, arg3 );
		    break;
		
		//52  replace window. (handle,x,y)
		case SYS_BUFFER_REPLACEWINDOW:
		    return (void *) sys_replace_window ( (struct window_d *) arg2, arg3, arg4);
		    break;
		
		//53 maximize window 
		//(handle)
		case SYS_BUFFER_MAXIMIZEWINDOW:
		    sys_MaximizeWindow ((struct window_d*) arg2);
		    break;
		
		//54 minimize window
		//(handle)
		case SYS_BUFFER_MINIMIZEWINDOW:
		    sys_MinimizeWindow ( (struct window_d *) arg2);
		    break;
		
		//55 Get foreground window.
		case SYS_BUFFER_GETFOREGROUNDWINDOW:
		    return (void *) sys_windowGetForegroundWindow ();
		    break;
		
		//56 set foreground window.
		case SYS_BUFFER_SETFOREGROUNDWINDOW:
		    return (void *) sys_windowSetForegroundWindow ( (struct window_d *) arg2 );
		    break;
		
		
		//57.	
		case SYS_REGISTERWINDOW: 
			return (void *) sys_RegisterWindow ( (struct window_d *) hWnd );
			break;
			
		//58.	
		case SYS_CLOSEWINDOW: 
			sys_CloseWindow ( (struct window_d *) hWnd ); 
			break;

        //59 - nothing
        
        //60
		case SYS_SETACTIVEWINDOW:
			sys_set_active_window (hWnd);
			break;

        //61
		//Id. (int).	
		case SYS_GETACTIVEWINDOW:
            return (void *) sys_get_active_window (); 
			break;

        //62
		case SYS_SETFOCUS: 
			sys_SetFocus ( (struct window_d *) hWnd ); 
			break;
			
        //63 id
		case SYS_GETFOCUS: 
		    return (void *) window_with_focus;  
			break;
			
        //64
		case SYS_KILLFOCUS:
			sys_KillFocus ( (struct window_d *) hWnd ); 
			break;

		// 65
		// Coloca um char usando o 'terminal mode' de stdio.
		// selecionado em _outbyte.
		// stdio_terminalmode_flag = n�o transparente.
		// Chama fun��o interna. (servicesPutChar)
			
		// #importante:	
		// >> putchar usando o cursor gerenciado pelo kernel.
		// A biblioteca em user mode, altera o cursor do kernel e usa essa rotina 
		// para imprimir. Essa rotina faz o refresh do ret�ngulo. 
		// Mas nossa inten��o � mesmo escrever na stream de sa�da no processo. 
			
		// #importante: 
		// Como printf � uma fun��o usada pelo terminal virtual, deve-se 
		// considerar as cores usadas no terminal virtual.
		// obs: Como faremos para pintar dentro da janela do terminal ?
        // obs: A rotina de configura��o do terminal dever� ajustar as 
		// margens usadas pela rotina de impress�o de caracteres, ent�o 
		// nesse momento devemos considerar que as margens ja est�o 
		// ajustadas.		

        // #importante:
		// putchar pertence a libc e todo o sistema tem obedecido a sua maneira 
		// de imprimir chars ... n�o podemos mudar putchar assim t�o facilmente.
		// refresh_rectangle obedece os deslocamentos usados por putchar.

		//queremos usar rotina dentro do servidor de terminal, em kserver/output.c
			
		// Essa rotina foi chamada por putchar em ring3 e est� pintando na tela
		// que pertence a um terminal. Mas isso � trabalho do terminal em si.
		
		// #importante	
		// Lembrando que o terminal virtual que est� em user mode usa rotinas
		// do X server para imprimir os caracteres e n�o as rotinas
		// da libc. Ele usa as rotinas da libc apenas para colocar os
		// caracteres dentro do arquivo de sa[ida.
			
		// #importante
		// N�o devemos usar rotinas da libc para imprimir caracteres na tela
		// e sim rotina do x server ou kgws. As rotinas da libc apenas 
		// lidam com arquivos do fluxo padr�o. Ent�o nesse servi�o vamos chamar
		// alguma rotina do kgws para imprimir no terminal e isso deve 
		// ficar expl�cito.


        // see: tty/console.c
        // IN: ch, console id.
        case SYS_KGWS_PUTCHAR:
            console_putchar ( (int) arg2, (int) arg3 ); 
            break;


		// 66
		// Isso funcionou.
		// � usado pelas rotinas de prompt em libc03 do atacama.
		// n�o deletar :)
		//IN: string, len 
        case 66:
            // Imprime uma string no console atual.
            // IN: string
            //printf ("%s", (char *) arg2); //funciona, mas falta refresh.
            
            //pega o tamanho da string.
            __len = strlen( (const char *) arg2 );
            
            //compara com o len enviado.
            if( __len > arg4){ __len = arg4; }
            if( __len > 128){ __len = 128; }   //tamanho da linha.
                           
            for ( __i=0; __i<__len; __i++ )
            {
				//IN: ch, console id.
                console_putchar ( (int) aa2[__i], (int) arg3 );
                //console_putchar ( (int) arg2[i], (int) arg3 );
            }
            //pular linha depois de imprimir a string do buffer.
            console_putchar ( '\n', (int) arg3);
            return NULL;
            break;


		//67- reservado pra input de usu�rio.
		//case 67:
		//	break;

		//68- Reservado pra input de mouse.
		//case 68:
		//	break;


        //#usando para debug 
		// 69 - Reservado pra input de teclado.
        case 69:
            printf ("Service 69: #todo\n");

            // #obs: Isso � bem pesado. Mas funciona.
            //refresh_rectangle ( 0, 0, 20, 20 );
            //bmpDisplayMousePointerBMP (terminalIconBuffer, 0, 0 ); 		
            break;

        //
        // ## EXIT ##
        //

		// 70 - Exit.
		// Atende a fun��o exit() da libc. 
		// Criaremos um 'request' que ser� atendido somente quando 
		// houver uma interrup��o de timer. 
		// Enquanto isso a thread deve esperar em um loop.
		// IN: ??

        case SYS_EXIT:
        
            //#todo: criar um wrapper em sci/sys.c ou kernel/exit.c
            sys_create_request ( (unsigned long) 12, // number 
                (int) 1,                             // status 
                (int) 0,                             // timeout. 0=imediatamente.
                (int) current_process,               // target_pid
                (int) current_thread,                // target_tid
                NULL,                                // window 
                (int) 0,                             // msg  
                (unsigned long) arg2,                // long1  
                (unsigned long) arg3 );              // long2
            break;


        // 71
        // #suspenso: Logo abaixo temos uma implementa��o
        // de um handler de uma interrup��o especial para o fork()
        // da libc. que chamar� do_fork_process();
        case SYS_FORK: 
            return NULL;
            break;


        // 72 - Create thread.
        // #todo: 
        // Enviar os argumentos via buffer.
        case SYS_CREATETHREAD:
            return (void *) sys_create_thread ( 
                                NULL,             // room ? 
                                NULL,             // desktop
                                NULL,             // w.
                                arg2,             // init eip
                                arg3,             // init stack
                                current_process,  // pid
                                (char *) a4 );    // name
            break; 



        // 73 - Create process.
        // ??
        // #todo: enviar os argumentos via buffer.
        // #todo: Ok, nesse momento, precisamos saber qual � o 
        // processo pai do processo que iremos criar. Esse deve ser 
        // o processo atual ...  
        // PPID = 0. Nesse momento todo processo criado ser� filho 
        // do processo n�mero 0. mas n�o � verdade. 
        // @todo: Precisamos que o aplicativo em user mode nos passe 
        // o n�mero do processo pai, ou o proprio kernel identifica 
        // qual � o processo atual e determina que ele ser� o 
        // processo pai. 

        case SYS_CREATEPROCESS:
            return (void *) sys_create_process ( NULL, NULL, NULL, 
                                arg2, arg3, 
                                0, (char *) a4, 
                                RING3, 
                                (unsigned long ) CreatePageDirectory() ); 
            break;



		//80 Show current process info.
		//#todo: Mostrar em uma janela pr�pria.
		//#todo: Devemos chamar uma fun��o que mostre informa��es 
		//apenas do processo atual. 
		case SYS_CURRENTPROCESSINFO:
		    sys_show_currentprocess_info ();
		    break;
			
		//81
		//#bugbug Isso est� retornando o ID do processo atual.
		//O que queremos � o ID do processo que est� chamando
		case SYS_GETPPID: 
		    return (void *) sys_getppid ();
			break;
		
		//82
		case SYS_SETPPID: 
			break;
			
			
		// 83
		// Suporte � chamada da libc waitpid(...).
		// schedi.c
		// #todo.
		// TID, PID 
		// TID � a thread atual.
		// PID veio via argumento.
        case SYS_WAIT4PID: 
            //pid, status, option
			return (void *) sys_do_waitpid ( (pid_t) arg2, (int *) arg3, (int) arg4 );
			//block_for_a_reason ( (int) current_thread, (int) arg2 ); //suspenso
			break;
			
		//84 - nothing
			
		// 85
		// #bugbug: 
		// Isso est� retornando o ID do processo pai do processo atual.
		// O que queremos � o ID do processo pai do processo que est� chamando.
		case SYS_GETPID: 
		    return (void *) sys_getpid ();
			break;
		
		//86
		case SYS_SETPID: 
			break;
		
		// 87 Down.
		case SYS_SEMAPHORE_DOWN:
		    return (void *) sys_Down ( (struct semaphore_d *) arg2);
		    break;
			
		//Testa se o processo � v�lido
        //se for v�lido retorna 1234		
		//testando...
		case SYS_88:   
            return (void *) sys_processTesting (arg2);
			break;
			
		// 89 Up. 	
		case SYS_SEMAPHORE_UP:
		    return (void *) sys_Up ( (struct semaphore_d *) arg2 );
		    break;
		
		//90 Coletor de threads Zombie. (a tarefa idle pode chamar isso.)
		case SYS_DEADTHREADCOLLECTOR: 
			sys_dead_thread_collector ();
			break;
		// 91 92 93
		
		//94	
		//REAL (coloca a thread em standby para executar pela primeira vez.)
		// * MOVEMENT 1 (Initialized --> Standby).
		case SYS_STARTTHREAD:
		    //t = (struct thread_d *) arg2;
            //sys_SelectForExecution (t);
			sys_SelectForExecution ( (struct thread_d *) arg2 );
			break;
		
		
		
		
		// 99
		// Pega 'hwnd' na fila da janela com o foco de entrada.
		case SYS_GETHWINDOW:
		    return NULL;
		    break;
		// #bugbug
		// Pega 'msg' na fila da janela com o foco de entrada.
		// Pegando a mensagem na fila da janela com o foco de entrada.
		case SYS_GETKEYBOARDMESSAGE:
			return (void *) sys_windowGetMessage ( (struct window_d *) WindowWithFocus );
			break;
		//****   Pega 'long1' na fila da janela com o foco de entrada.	
		case SYS_GETLONG1:
			return (void *) sys_windowGetLong1 ( (struct window_d *) WindowWithFocus );
			break;
		//****   Pega 'long2' na fila da janela com o foco de entrada.	
		case SYS_GETLONG2:
			return (void *) sys_windowGetLong2 ( (struct window_d *) WindowWithFocus );
			break;

 
		//103, SYS_RECEIVEMESSAGE	
        //Um processo consumidor solicita mensagem deixada em seu PCB.
        //Argumentos: servi�o, produtor, consumidor, mensagem.		
        //@todo: 
		case SYS_RECEIVEMESSAGE:
			break;
			
		//104, SYS_SENDMESSAGE
		//Um processo produtor envia uma mensagem para o PCB de outr processo.
		//Argumentos: servi�o, produtor, consumidor, mensagem.
		//@todo:		
		case SYS_SENDMESSAGE:	
			break;



        // 110 - Reboot.
        case SYS_REBOOT: 
            sys_reboot ();
            break;


        // 111 - Getting single message.
        // No queue. See: thread structure.
        // rotina interna, veja nesse documento.
        // IN: buffer para mensagens.
        // // See: ps2kbd.c
        case 111:
            return (void *) __do_111 ( (unsigned long) &message_address[0] );
            break;



		// 112
		// Enviar uma mensagem para a thread de controle de um processo.
		// arg2 = endere�o do buffer da mensagem, pid
		// arg3 = pid
		// #importante: 
		// O kernel l� o buffer e dentro do buffer tem uma mensagem 
		// que ser� colocada na thread de cotrole do processo;
        case SYS_SENDMESSAGETOPROCESS:
            //printf ("112: PID=%d\n", arg3 );
            pty_send_message_to_process ( (unsigned long) &message_address[0], 
               (int) arg3 );
            //printf ("112: done\n");
            break;


		// Envia uma mensagem PAINT para o aplicativo atualizar a 
		// �rea de cliente.
		case 113:
		    sys_windowUpdateWindow ( (struct window_d *) arg2 );
			break;


		// 114
		// Envia uma mensagem para a thread atual.
		// endere�o do buffer da mensagem, tid.
        case SYS_SENDMESSAGETOCURRENTTHREAD:
            pty_send_message_to_thread ( (unsigned long) &message_address[0], 
                (int) current_thread );
            break;


		// 115 - ## IMPORTANTE ## 
		// Usado por servidores do sistema para se comunicarem 
		// com o kernel.
		// >> magic 1234: Acoplar taskman.
		// >> magic 4321: Desacoplar taskman
		// >> magic 12345678: Pegar mensagem
		// #todo: Chamar uma fun��o que trate o argumento com um switch.
		case 115:
		
			if ( arg3 == 1234 )
			{
				current_taskman_server = (int) arg4;
				//printf("115: acoplando ...\n");
				//refresh_screen();
                return NULL; 
			}; 
			
			if ( arg3 == 1234 )
			{
				if( current_taskman_server == arg4 )
				{
				    current_taskman_server = (int) 0;
				    //printf("115: desacoplando ...\n");
				    //refresh_screen();
				    return NULL;
				} 
			};
			
			if ( arg3 == 12345678 )
			{
				if( current_taskman_server == arg4 )
				{
					if( gui->taskmanWindow->newmessageFlag == 0 )
					{
			            message_address[0] = (unsigned long) 0;
			            message_address[1] = (unsigned long) 0; //*importante: mensagem nula.
			            message_address[2] = (unsigned long) 0;
			            message_address[3] = (unsigned long) 0;
					    gui->taskmanWindow->newmessageFlag = 0;
                        return NULL;
					}
					//se existe uma mensagem na janela do servidor taskman.
					if( gui->taskmanWindow->newmessageFlag == 1 )
					{
			            message_address[0] = (unsigned long) gui->taskmanWindow->msg_window;
			            message_address[1] = (unsigned long) gui->taskmanWindow->msg; 
			            message_address[2] = (unsigned long) gui->taskmanWindow->long1;
			            message_address[3] = (unsigned long) gui->taskmanWindow->long2;
					    gui->taskmanWindow->newmessageFlag = 0;
						return NULL;
					};
				};
			};
			break;




		// Envia uma mensagem de teste para o servidor taskman	
        // #todo: deletar isso.
		case 116:
	        gui->taskmanWindow->msg_window = NULL;
		    gui->taskmanWindow->msg = (int) arg2;                //123=temos uma mensagem. 
		    gui->taskmanWindow->long1 = (unsigned long) arg3;    //0;
		    gui->taskmanWindow->long2 = (unsigned long) arg4;    //0;
            gui->taskmanWindow->newmessageFlag = 1;
		    break;


		// 117.
		// Envia uma mensagem para uma thread, dado o tid.
        case 117:
            pty_send_message_to_thread ( (unsigned long) &message_address[0], 
                 (int) arg3 );
            break;


        // CREATE WINDOW
        // Cria uma janela usando os recursos do kgws.
        // Chamamos uma fun��o nesse documento.
        case SYS_118:
            return (void *) serviceCreateWindow ( (char *)  arg2 );
            break;


		//119
		case SYS_SELECTCOLORSCHEME:
		    return (void *) sys_windowSelectColorScheme ( (int) arg2 );
			break;


		// 124	
		// Defered system procedure call.
		// #todo: 
		// Precisamos armazenasr os argumentos em algum lugar.
		// #bugbug: Precisamos criar um request.
		case 124:
		    kernel_request = KR_DEFERED_SYSTEMPROCEDURE;
			break;


		// 125
		// system procedure call.
        case 125:
            return (void *) sys_system_procedure ( NULL, 
                                arg2, arg3, arg4 );
            break;


		// 126
		// Permitindo que drivers e servidores em usermode acessem
		// as portas.
        case SYS_USERMODE_PORT_IN:
			//#bugbug
			//#todo: Tem que resolver as quest�es de privil�gios.
			//bits, port
            return (void *) sys_portsx86_IN ( (int) arg2, 
                                (unsigned long) arg3 );
            break;


		// 127
		// Permitindo que drivers e servidores em usermode acessem
		// as portas.
        case SYS_USERMODE_PORT_OUT:
			//#bugbug
			//#todo: Tem que resolver as quest�es de privil�gios.
			//bits, port, value
            sys_portsx86_OUT ( (int) arg2, 
                (unsigned long) arg3, (unsigned long) arg4 );
            return NULL;
            break;



		// #especial
		// 129
		// Um driver confirmando que foi inicializado.
		// Efetuaremos a rotina de liga��o do driver com o sistema.
		// O sistema linka o driver com o sistema operacional.
	    //@todo: Essa rotina precisa ser aprimorada. Mas a chamada deve ser 
		// essa mesma.
		//
		case SYS_DRIVERINITIALIZED: 
		    return (void *) sys_systemLinkDriver (arg2,arg3,arg4); 
			break;
			
			
		// 130
		// Pinta um texto em uma dada janela.
		// #todo: Se n�o for expecificado a janela, ent�o � pra pintar na janela principal. gui->main.
		// Repensar se qualquer um pode pintar na janela principal.	
		// args: window, x, y, color, string.
			
		case SYS_DRAWTEXT:
			//argString = (unsigned char *) arg4; //??
		    sys_draw_text ( (struct window_d *) message_address[0], 
			    (unsigned long) message_address[1],  
				(unsigned long) message_address[2],  
				(unsigned long) message_address[3],   
				(unsigned char *) message_address[4] ); 
			break;
			
			
		//131
		// Pintar o caractere especificamente na janela com o foco de entrada.          
		case SYS_BUFFER_DRAWCHAR_WWF: 
			focusWnd = (void *) windowList[window_with_focus];
			if ( (void *) focusWnd == NULL )
			{
			    break;	
			};
			
			sys_my_buffer_char_blt( (unsigned long) (arg2 + focusWnd->left), //x.
			                    (unsigned long) (arg3 + focusWnd->top),  //y.
								COLOR_BLACK,                             //color. 
								(unsigned long) arg4);                   //char.
    		break;


        // save rect ?
		case 132:
		    //#bugbug: pagefault
			//save_rect (  message_address[0],  message_address[1],  message_address[2],  message_address[3] );
			break;


        // show a saved rect ?
        case 133: 		
		    //#bugbug: pagefault
			//show_saved_rect (  message_address[0],  message_address[1],  message_address[2],  message_address[3] );
			break; 


		// 134
		// Pegar informa��es sobre a �rea de cliente de uma janela;
		// #bugbug: temos que checar mais validade de estrutura.
		// obs: No come�o dessa fun��o, colocamos o arg3 como ponteiro para a3.
		// um buffer de longs.
		case 134:
				hWnd = (struct window_d *) arg3;

				if ( (void *) hWnd != NULL )
				{	
					a3[0] = (unsigned long) hWnd->rcClient->left;
					a3[1] = (unsigned long) hWnd->rcClient->top;     
					a3[2] = (unsigned long) hWnd->rcClient->width;
					a3[3] = (unsigned long) hWnd->rcClient->height;
					a3[4] = (unsigned long) hWnd->rcClient->bg_color;
				}
			break;


		// 135
		// Coloca caracteres na estrutura de terminal, para aplicativos pegarem
        case SYS_FEEDTERMINAL:
            //@todo:
			// Colocar o caractere enviado no argumento para 
			//a estrutura de terminal do terminal indicado no argumento.
			//terminalFeed( (int) arg2, (int) arg3 );
			
			//arg2 = n�mero do terminal
			//arg3 = ch 
			//arg4 - ch 
			
			//terminal.h
			//teminalfeedCH = (char) arg3;
			//teminalfeedCHStatus = (int) 1;
		    break;	


		//136
		//fgetc
		case 136:
			return NULL;
			//return (void *) sys_fgetc ( (FILE *) arg2 );
			break;


		// 137
		// Isso � usado pela biblioteca stdio em user mode
		// na fun��o 'getchar()'
		// Pega caractere no stdin do teclado.
		// Isso funciona.
        case SYS_GETCH:  
			return (void *) sys_thread_getchar ();
            break;

			
		//138 - get key state.	
		//#importante: 
		//#todo: isso precisa ir para a API.
        case 138:
		    return (void *) sys_keyboardGetKeyState ( (unsigned char) arg2 );
            break;


		//139
        case SYS_GETSCANCODE:
		    return (void *) sys_get_scancode ();
            break;


        //140
        case SYS_SET_CURRENT_KEYBOARD_RESPONDER:
            sys_set_current_keyboard_responder (arg2);
		    break;

		//141	
		case SYS_GET_CURRENT_KEYBOARD_RESPONDER:
		    return (void *) sys_get_current_keyboard_responder();
		    break;
			
		//142	
        case SYS_SET_CURRENT_MOUSE_RESPONDER:
		    sys_set_current_mouse_responder(arg2);
			break;
			
		//143	
		case SYS_GET_CURRENT_MOUSE_RESPONDER:
		    return (void *) sys_get_current_mouse_responder ();
			break;

			
		//144	
		//Pega o ponteiro da client area.	
		case SYS_GETCLIENTAREARECT:	
		    //#bugbug: pegamos o ponteiro mas n�o temos permiss�o para acessar a estrutura.
			return (void *) sys_getClientAreaRect ();	
			break;
		
		//145
        //configura a client area	
        //@todo: O ponteiro para estrutura de ret�ngulo � passado via argumento.
		case SYS_SETCLIENTAREARECT:
			sys_setClientAreaRect ( arg2, arg3, arg4, 0);
            break;

		//146	
		//#bugbug: isso n�o parece seguro, precismos checar a validade da estrutura antes,
        //mas vai ficar assim por enquanto.
		case 146:	
		    //return (void *) gwsScreenWindow(); //#todo
		    return (void *) gui->screen;
			break;
			
		//147	
		//#bugbug: isso n�o parece seguro, precismos checar a validade da estrutura antes,
        //mas vai ficar assim por enquanto.
        case 147:
		    //return (void *) gwsMainWindow(); //#todo
			return (void *) gui->main;
			break;


		// 148 - Create grid and itens.
		// window, n, view. 
        case 148:
           return (void *) sys_grid ( (struct window_d *) arg2, 
                               (int) arg3, (int) arg4 );
            break;



        // 149 - Testing system menu.
        // Essa � uma rotina de teste, qua chama v�rias fun��es.
        case 149:
            sys_MainMenu ( (struct window_d *) arg2 );
            break;



		//152 - get user id
		case SYS_GETCURRENTUSERID:
		    return (void *) current_user;
			break;
		
		//154 - get group id	
		case SYS_GETCURRENTGROUPID:
		    return (void *) current_group;
			break;

        //156 - SYS_SHOWUSERINFO
        case 156:
            ShowUserInfo (current_user);
            return NULL;
            break;

		//157 - get user session id	
        case SYS_GETCURRENTUSERSESSION:
            return (void *) current_usersession;
            break;


		//158 - get window station id		
		case SYS_GETCURRENTWINDOWSTATION:	
		    return (void *) current_room; 
			break;
			
		//159 - get desktop id
        case SYS_GETCURRENTDESKTOP:
		    return (void *) current_desktop; 
			break;
			
		// 160
		// Criar um socket e retornar o ponteiro para a estrutura.
		// Gramado API socket support. (not libc)	
		case 160:
            return (void *) sys_create_socket ( (unsigned long) arg2, (unsigned short) arg3 );
			break;
			
		// 161
        // get socket IP
		// Gramado API socket support. (not libc)
		case 161:
		    return (void *) sys_getSocketIP ( (struct socket_d *) arg2 );
            break;		

		// 162
        // get socket port		
		// Gramado API socket support. (not libc)	
		case 162:
		    return (void *) sys_getSocketPort( (struct socket_d *) arg2 );
            break;		
			
		// 163
        // update socket  
        // retorno 0=ok 1=fail		
        // Gramado API socket support. (not libc)	
		case 163:
            return (void *) sys_update_socket ( (struct socket_d *) arg2, 
                                (unsigned long) arg3, (unsigned short) arg4 );
			break;

		//#todo: a chamada est� no shell em net.c
		//netStream
        case 164:
            //IN: ( service, (unsigned long) stream, option, option )
			break;	

		//#todo: a chamada est� no shell em net.c	
		//netSocket
        case 165:
            //IN: ( service, (unsigned long) socket, option, option );	
			break;

		//#todo: a chamada est� no shell em net.c	
		//netBuffer
        case 166:
            //IN:  ( service, buffer_address, option, option );	
			break;

 
       // pegando a stream dado um fd;
       // see: process.c
       case 167:
           return NULL;
           // IN: pid, fd
           //return (void *) get_stream_from_fd ( (int) arg2, (int) arg3 );
           break;
 
 
       //livre
       case 168:
           // IN: filename, argv, envp
           return (void *) __execute_new_process ( (const char *) arg2, 
                               (char **) arg3, 
                               (char **) arg4 );
           break;



       // write
       // See: unistd.c em garden/lib/libcore.
       case 169:
           //essa rotina mudou de n�mero.
           return NULL;
           // IN: fd, buf, count.         
           //return (void *) tty_write ( (unsigned int) arg2,  //channel 
             //                  (char *) arg3,                //buf
               //                (int) arg4 );                 //nr
           break;


		//170
        //pwd ...
        //Cada processo tem seu pr�prio pwd.
        //Essa rotina mostra o pathname usado pelo processo.	
		case SYS_PWD:
			sys_fs_print_process_pwd (current_process);
			break;	
		
		//171 - retorna o id do volume atual.
		case SYS_GETCURRENTVOLUMEID:
		    return (void *) current_volume;
            break;	

		//172 - configura o id do volume atual.
		//#bugbug: Estamos modificando, sem aplicar nenhum filtro.
		case SYS_SETCURRENTVOLUMEID:
		    current_volume = (int) arg2;
            break;	

		//173 - Lista arquivos de um diret�rio, dado o n�mero do disco,
        //o n�mero do volume e o n�mero do diret�rio,
        //args in: disk id, volume id, directory id
        case SYS_LISTFILES:
            sys_fsListFiles ( arg2, arg3, arg4 );  
			break;
			
			
		//174
		case SYS_SEARCHFILE:
		    return (void *) sys_KiSearchFile ( (unsigned char *) arg2, 
                                (unsigned long) arg3 );
			break;
			
		//175
		// cd.
        //Atualiza o pathname na estrutura do processo atual.
        //Atualiza o pathname na string global.
		case 175:
		    sys_fsUpdateWorkingDiretoryString ( (char *) arg2 );
            sys_fsLoadFileFromCurrentTargetDir ();
			break;
			
		//176	
        //Remove n nomes de diret�rio do pathname do processo indicado no argumento.
        //Copia o nome para a string global.
		case 176:	
		    sys_fs_pathname_backup ( current_process, (int) arg3 );
			break;
			
		//177
		//'dir'
		//comando dir no shell.
		//Listando arquivos em um diret�rio dado o nome.	
		case 177:
		    sys_fsList ( (const char *) arg2 );
            break;

		 //#test
		 //implementando esse servi�o.
        case 178:
            taskswitch_lock ();
            scheduler_lock ();
			//name , address.
            Ret = (void *) sys_fsGetFileSize ( (unsigned char *) arg2 ); 
            scheduler_unlock ();
            taskswitch_unlock ();
            return (void *) Ret; 
            break;


		//179
		//#bugbug: isso � um teste
		//isso faz o programa rodar na thread clone ClonedThread e n�o n a IdleThread,
		// 0 = idle ; 216 = cloned.	
		case 179:
			//servi�o, name, (arg)(endere�o da linha de comando), env
			
           return (void *) do_gexecve ( 216, 
                                (const char *) arg2, 
                                (const char *) arg3, 
                                (const char *) arg4 ); 
			break;
			

		//184
		//pega o endere�o do heap do processo dado seu id.	
        case SYS_GETPROCESSHEAPPOINTER:
            return (void *) sys_GetProcessHeapStart ( (int) arg2 );
			break;	

		// feof	
		case 193:	
		    return NULL;
		    //return (void *) sys_feof ( (FILE *) arg2 );
            break;
			
		//ferror	
		case 194:	
		    return NULL;
		    //return (void *) sys_ferror ( (FILE *) arg2 );
            break;
			
		//fseek	
		case 195:	
		    return NULL;
		    //return (void *) sys_fseek ( (FILE *) arg2, (long) arg3, (int) arg4 );
            break;



        // fputc
        case 196:
            return NULL;
            //return (void *) sys_fputc ( (int) arg2 , (FILE *) arg3 );
            break;


        //197
		//scroll de �rea de cliente de uma janela;
        case 197:
		    //essa � uma boa rotina, mas precisamos tabalhar nisso,
			//est� dando page fault.
			//scroll_client_window ( (struct window_d *) arg2 );
            break; 
			

		//199 - Garbage Collector.	
		//A ideia � que os utilit�rios de ger�ncia de mem�ria possam usar isso.
		case SYS_GC:
		    sys_gc ();
			//return (void *) gc (); 
		    break;
		
		//200 - Envia um sinal para um processo.
		//argumentos (process handle, signal number).
		case SYS_SENDSIGNAL:
		    sys_signalSend ( (void *) a2, (int) arg3 );
		    break;
			
		//...

        //205
		//block for a reason.
        //bloqueamos uma thead por um dos motivos indicados em argumento.
        case SYS_GENERICWAIT:
		    //TID, reason.
            //sys_block_for_a_reason ( (int) arg2, (int) arg3 );
			break;		
			
		//210
		case SYS_CREATETERMINAL: 
            break;
			
		//211
        case SYS_GETCURRENTTERMINAL:
            return (void *) current_terminal;
			break;
			
	    //212
        case SYS_SETCURRENTTERMINAL:
            current_terminal = (int) arg2;
			break;
			
		//213
        case SYS_GETTERMINALINPUTBUFFER:	
            return NULL;
			break;
			
		//214
        case SYS_SETTERMINALINPUTBUFFER:
		    break;

		// 215
		// Get terminal window.	
		// retorna o ID.
		// O ID fica em terminal_window.
		case SYS_GETTERMINALWINDOW: 
			return (void *) sys_systemGetTerminalWindow (); 
			break;

		// 216
		// Set terminal window	
		// Configura qual vai ser a janela do terminal virtual. 
		// #obs: O refresh de stdout podera' ocorrer em ts.c
		// O ID fica em terminal_window.	
		case SYS_SETTERMINALWINDOW:	
		   sys_systemSetTerminalWindow ( (struct window_d *) arg2 );	
		   break;
		   
		//217
		case SYS_GETTERMINALRECT:
			return NULL; 
			break;

		//218
        //configura na estrutura do terminal corrente,
        //qual vai ser o ret�ngulo a ser usado. 		
		case SYS_SETTERMINALRECT:
		
		   //#bugbug: Pagefault.
		   
		    // Estamos usando default por falta de argumento.
            //systemSetTerminalRectangle( DEFAULT_TERMINAL_LEFT, 
			//                            DEFAULT_TERMINAL_TOP, 
			//							arg3, 
			//							arg4 );		
		    break;


		//219
        case SYS_DESTROYTERMINAL:
            break; 
 
       //220 - reboot             #todo
       //221 - execute a program. #todo


		// 222 - Create timer.
		// IN: window, ms e tipo
        case 222:
            return (void *) sys_create_timer ( (struct window_d *) arg2, 
                                (unsigned long) arg3, (int) arg4 );
            break;


        //223 - get sys time info.
        // informa��es variadas sobre o sys time.		
		case 223:
		    return (void *) sys_get_systime_info ( (int) arg2 );
            break;		


		//224 - get time	
		case SYS_GETTIME:	
		    return (void *) sys_get_time ();
			break;


		//225 - get date
		case SYS_GETDATE:
		    return (void *) sys_get_date ();
            break;		



        // Obs: 
        // #todo: 
        // Poderia ser uma chamada para configurar o posicionamento 
        // e outra para configurar as dimens�es.		
        //226 - get
        case SYS_GET_KERNELSEMAPHORE:
            return (void *) __spinlock_ipc;
            break;


        // 227 - close critical section	
        case SYS_CLOSE_KERNELSEMAPHORE:
            __spinlock_ipc = 0;
            break;


        // 228 - open critical section
        case SYS_OPEN_KERNELSEMAPHORE:
            __spinlock_ipc = 1;
            break;


		//232 - fclose
		case 232:
			return NULL;
			//return (void *) sys_fclose ( (FILE *) arg2);
			break;
			
		// 233 - fflush.
		case 233:
			return NULL;
			//return (void *) sys_fflush ( (FILE *) arg2);
			break;
			
		// 234 - fprintf.	
		case 234:
			return NULL;
			//return (void *) sys_fprintf ( (FILE *) arg2, (const char *) arg3 );
			break;
			
		//235 - fputs
		case 235:
			return NULL;
			//return (void *) sys_fputs ( (const char *) arg2, (FILE *) arg3 );
			break;
			
		// tty ... 236 237 238 239.	
			
		//236 - get tty id
		case 236:	
			return (void *) current_tty;
			break;	
		
			
		//240
		case SYS_GETCURSORX:
		    return (void *) sys_get_cursor_x();
		    break;

		//241
		case SYS_GETCURSORY:
		    return (void *) sys_get_cursor_y();
		    break;
			
		//244 enable text cursor.	
		case 244:
		    sys_timerEnableTextCursor ();
		    //timerShowTextCursor = 1;
		    //gwsEnableTextCursor ();
            break;		
		
		//245 disable text cursor.
		case 245:
		    sys_timerDisableTextCursor ();
		    //timerShowTextCursor = 0;
		    //gwsDisableTextCursor ();
            break;
			
		//246 ~ 249 reservado para libc support.	
			
			
			
		// 246	
		// #todo
		// Essa rotina est� servido � chamada openat() da libc03.
		// IN: dirfd, pathname, flags.
		case 246:
			return (void *) __openat ( (int) arg2, (const char *) arg3, (int) arg4 ); 
			break;

	
		// pipe	
		case 247:
			return (void *) sys_pipe ( (int *) arg2 ); 
			break;
			
			
			
		// execve	
		// executa um programa usando o processo atual
		// t� usando a thread atual e transformando ela em thread de controle.
		case 248:
			//servi�o, name, (arg)(endere�o da linha de comando), env
            return (void *) do_execve ( 0, 
                                (const char *) arg2, 
                                (const char *) arg3, 
                                (const char *) arg4 ); 
			break;


		// 249 - ftell
		case 249:
            return NULL;
            //return (void *) ftell ( (FILE *) arg2 );
			break;
			
			
			
		//Info. (250 ~ 255).



		// 250 - Get system metrics
        case SYS_GETSYSTEMMETRICS:
            return (void *) sys_systemGetSystemMetrics ( (int) arg2 );
            break;


		//251
		//Informa��es sobre o disco atual.
		case SYS_SHOWDISKINFO:
		    sys_diskShowCurrentDiskInfo();
			refresh_screen();
			break;

		//252
		//Informa��es sobre o volume atual.
		case SYS_SHOWVOLUMEINFO:
		    sys_volumeShowCurrentVolumeInfo();
			refresh_screen();
			break;
		
		//253
		case SYS_MEMORYINFO:
		    sys_memoryShowMemoryInfo();
			refresh_screen();
			break;
			
			
		//254 - Show PCI info.	
		case SYS_SHOWPCIINFO: 
			sys_systemShowDevicesInfo();
			refresh_screen();
			break;
			
		//255 - Mostra informa��es sobre o kernel.	
		case SYS_SHOWKERNELINFO: 
			sys_KiInformation ();
			//refresh_screen ();
			break;



		// #todo:
		// Need to check the system call ID to ensure it's valid 
		// If it s invalid, return ENOSYS Function not implemented error

        default:
            printf ("services: Default {%d}\n", number );
            refresh_screen ();
            return NULL;
            break;
    };


	//Debug.
	//printf("SystemService={%d}\n",number);
   

	// * importante:
	//   Depois de esgotados os 'cases', vamos para a sa�da da fun��o.
	//    No caso de um aplicativo ter chamado essa rotina, 
	// o retorno ser� para o ISR da int 0x80, feito em assembly.
	//    No caso do kernel ter chamado essa rotina, apenas retorna.


done:
    //Debug.
    //printf("Done\n",number);
    //refresh_screen();
    return NULL;
}



/*
 ******************************************************
 * serviceCreateWindow:
 *     Cria uma janela com base nos argumentos passados no buffer.
 *     Essa rotina � o servi�o 118.
 */

// #todo
// Esse servi�o deve ir para o m�dulo /kgws

unsigned long serviceCreateWindow ( char *message_buffer ){

    unsigned long *message_address = (unsigned long *) message_buffer;

	//Ponteiro para a janela criada pelo servi�o.
    struct window_d *NewWindow;  
    struct window_d *ParentWindow;  

    cwArg1 = message_address[0];             // Type. 
    cwArg2 = message_address[1];             // WindowStatus 
    cwArg3 = message_address[2];             // view
    cwArg4 = (char *) message_address[3];    // a4 Window name.
    cwArg5 = message_address[4];             // x
    cwArg6 = message_address[5];             // y
    cwArg7 = message_address[6];             // width
    cwArg8 = message_address[7];             // height

	//parent window.
	//message_address[8];
	//cwArg9 = gui->screen;  //@todo: O argumento arg4 est� enviando parent window. 
    cwArg9 = (struct window_d *) message_address[8];    //parent
    ParentWindow = (struct window_d *) message_address[8];    //parent

	//onde?
	//message_address[9];
	//cwArg10 = arg4;  //desktop ID 

    cwArg11 = message_address[10];    //cor da area de cliente.
    cwArg12 = message_address[11];    //cor da janela.

    //==========

	//Window.
    unsigned long WindowType;      //Tipo de janela.
    unsigned long WindowStatus;    //Status, ativa ou n�o.
	//unsigned long WindowRect;
    unsigned long WindowView;      //Min, max.
    char *WindowName;  

    unsigned long WindowX = (2*(800/20));    //100;   >          
    unsigned long WindowWidth  = 320;               
    unsigned long WindowY = (2*(600/20));    //100;   V                
    unsigned long WindowHeight = 480;  

    unsigned long WindowClientAreaColor = COLOR_WINDOW;  
    unsigned long WindowColor = COLOR_WINDOW;  


	//#todo: Checar a validade da esturtura,
	//WindowClientAreaColor = CurrentColorScheme->elements[csiWindow];  
	//WindowColor = CurrentColorScheme->elements[csiWindowBackground];  


    WindowType = cwArg1; 
    WindowStatus = cwArg2; 
    WindowView = cwArg3; 
    WindowName = (char *) cwArg4; 

    WindowX = cwArg5; 
    WindowY = cwArg6; 
    WindowWidth = cwArg7; 
    WindowHeight = cwArg8;


	//#todo
	//gui->screen  = cwArg9; 
	//desktopID = cwArg10; 

	//Obs: 11 - A cor da �rea de cliente ser� escolhida pelo app.   
    WindowClientAreaColor = (unsigned long) cwArg11;  
    WindowColor = (unsigned long) cwArg12;     


    struct thread_d *t;
    int desktopID;


    desktopID = (int) get_current_desktop_id ();


    //
    // Create.
    //
    
    
    // #test
    // Ok isso funcionou.
    if ( (void *) ParentWindow != NULL )
    {
        WindowX = ParentWindow->left + WindowX;
        WindowY = ParentWindow->top + WindowY;
    }

    // Importante:
	// Nesse momento � fundamental saber qual � a parent window da janela que 
	// vamos criar pois a parent window ter� as margens que nos guiam.
	// Essa parent window pode ser uma aba de navegador por exemplo.
	// >> O aplicativo deve enviar o ponteiro da janela m�e.
	
	//Criando uma janela, mas desconsiderando a estrutura rect_d passada por argumento.
	//@todo: #bugbug a estrutura rect_d apresenta problema quando passada por argumento
	//com um endere�o da �rea de mem�ria do app.
	
	
	//#test
	// testando novo m�todo de pintura.

    NewWindow = (void *) kgws_create_window ( WindowType, WindowStatus, 
                             WindowView, WindowName, 
                             WindowX, WindowY, WindowWidth, WindowHeight,
                             cwArg9, desktopID, 
                             (unsigned long) WindowClientAreaColor, 
                             (unsigned long) WindowColor );


    if ( (void *) NewWindow == NULL )
    { 
        //?? Mensagem. 
        return 0;

    }else{

		//se a janela foi criada com sucesso, podemos desativar a flag.
		//*importante, nesse momento precisamos desativar a flag.

		cwFlag = 0;                  
		
        // Obs: 
		// Quem solicitou a cria��o da janela pode estar em user mode
        // por�m a estrutura da janela est� em kernel mode. #bugbug
		// Obs:
		// Para preencher as informa��es da estrutura, a aplica��o
		// pode enviar diversas chamadas, Se n�o enviar, ser�o considerados
		// os valores padr�o referentes ao tipo de janela criada.
		// Cada tipo tem suas caracter�sticas e mesmo que o solicitante
		// n�o seja espec�fico nos detalhes ele ter� a janela do tipo que deseja.
		
        //  
        //@todo: Pode-se refinar os par�metros da janela na estrutura.
		//NewWindow->
		//...
		
		//@todo: N�o registrar, quem criou que registre a janela.
		//RegisterWindow(NewWindow);
		
		// #importante
		// Se a tarefa atual est� pintando, vamos melhorar a sua prioridade.
		// Possivelmente a thread de controle da janela � a thread atual.
		
		t = (void *) threadList[current_thread];
		
		set_thread_priority ( t, PRIORITY_MAX );
		
		NewWindow->control = t;


        return (unsigned long) NewWindow;
    };
}



/*
 ***********************
 * servicesPutChar:
 *     Movendo para terminal/output.c 
 *     Coloca um char usando o 'terminal mode' de stdio selecionado 
 * em _outbyte.
 * stdio_terminalmode_flag = n�o transparente.
 */

void servicesPutChar ( int c )
{
    console_putchar ( (int) c, current_vc );
}



/*
 ***************************************
 * gde_fork:
 *     Implementa a fun��o fork() da libc padr�o.
 *
 *     Essa rotina foi chamada pela interrup��o 133,
 *     cujo handler est� em: x86/entry/head/sw.inc 
 *     na fun��o: _int133_fork.
 *
 *     Essa fun��es tem sua pr�pria interrup��o. 
 */

	// #todo:
	// Tendo a fork() uma chamada s� pra ela, ent�o
	// podemos usar um conjunto de argumentos de forma confort�vel.

void *gde_fork ( unsigned long number, 
                 unsigned long arg2, 
                 unsigned long arg3, 
                 unsigned long arg4 )
{
    void *ret;


	//Salva contexto dos registradores.
    save_current_context ();


    // Chama a rotina que implementa a fun��o fork() em ring 0.
    // See: ps/action/process.c

    ret = (void *) do_fork_process ();


	//Restaura os registradores e o cr3.
    restore_current_context ();

		// #debug
		// Mostrando informa��es.

        /*
		//#importante
		//#DEBUG #DEBUG #DEBUG #DEBUG #DEBUG
		//MOSTRAR AS INFORMA��ES DO PROCESSO CLONE.
		//show_currentprocess_info ();
		//show_process_information ();
		//mostra_slot (current_thread);
		//mostra_reg (current_thread);
		//printf ("*breakpoint\n\n");
		//refresh_screen();
		//while(1){}
        */

    return (void *) ret; 
}

//
// End.
//

