/*
 * File: ps/action/process.c 
 *
 * Descri��o:
 *     Gerenciamento de processos.
 *     PM - Process Manager (Parte fundamental do Kernel Base).
 *     Interfaces para o kernel chamar as rotinas de gerenciamento de
 * processos. 
 *     As rotinas aqui s�o independentes da arquitetura, quem se preocupa
 * com a arquitetura s�o as rotinas de inicializa��o comecadas por Ki.
 *     'Ki' para m�dulos internos do kernel.
 *
 * Atribui��es:
 *     Rotinas de gerenciamento das processos, cria��o, inicializa��o,
 * finaliza��o, task switch e outros.
 *
 *    Tipos de Processos:
 *
 *    PB - Processos inclu�dos no Kernel Base.
 *    P0 - Processos em ring0.
 *    P1 - Processos em ring1.
 *    P2 - Processos em ring2.
 *    P3 - Processos em ring3, User Mode.
 *
 * @todo: 
 * Essas interfaces devem ser padronizadas, obedecendo roteiros de abertura, 
 * fechamento, salvamento e restaura��o de pilha ou argumento.
 * +Criar a fun��o get_current_pid. (obs: Isso j� existe)
 *
 * In this file:
 * ============
 *     +processSendSignal
 *     +create_process 
 *     +CloseAllProcesses
 *     +show_process_information
 *     +muitas outras rotinas...
 *
 * History:
 *     2015 - Create by Fred Nora.
 */
 
 
/*
    Fluxo padr�o:
	
	The Standard Streams.
	
	Every process is initialized with three open file descriptors, 
	stdin, stdout, and stderr. 
	stdin is an abstraction for accepting input (from the keyboard or from pipes) and 
	stdout is an abstraction for giving output (to a file, to a pipe, to a terminal).

    That's a very simplified explanation but true nonetheless. 
	Those three file descriptors are collectively called 'The Standard Streams'.	
    
	Standard input - this is the file handle that your process reads to 
	get information from you.
    Standard output - your process writes normal information to this file handle.
    Standard error - your process writes error information here.
	
    Quando um programa inicia, 
	o sistema operacional automaticamente define quem � 
	a entrada padr�o (geralmente o teclado) e quem � 
	a sa�da padr�o (geralmente a tela).
*/ 
 
 
#include <kernel.h>



//
// Fun��es importadas.
//

extern unsigned long get_page_dir (void);


//
// Vari�veis internas.
//

//int processmanagerStatus;
//int processmanagerError;
//...

int caller_process_id;

int processNewPID;   


/*
// Chamada pelo timer.c
int process_profiler();
int process_profiler()
{
    return -1;
}
*/



unsigned long __GetProcessStats ( int pid, int index )
{
	
    struct process_d *p;

    //#todo:
    //checar validade dos argumentos.

	//Struct.
    p = (void *) processList[pid];

    if ( (void *) p == NULL )
    {
        printf ("__GetProcessStats: struct \n");
        return 0; 

    } else {
        //checar validade.
		//...
    };

	
	
	
	
    switch ( index )
    {

        case 1:
           return (unsigned long) p->pid;
           break;            
            
        case 2:
           return (unsigned long) p->ppid;
           break;            

        case 3:
           return (unsigned long) p->uid;
           break;            

        case 4:
           return (unsigned long) p->gid;
           break; 
           
        //state    
        case 5:
            return (unsigned long) p->state;
            break;                      
           
        case 6:
            return (unsigned long) p->plane;
            break;              
           
        case 7:
            return (unsigned long) p->input_type;
            break;              

        case 8:
            return (unsigned long) p->personality;
            break;              

        case 9:
            return (unsigned long) p->appMode;
            break;              

        
        case 10:
            return (unsigned long) p->private_memory_size;
            break;  
           
        case 11:
            return (unsigned long) p->shared_memory_size;
            break;          

        case 12:
            return (unsigned long) p->workingset_size;
            break;          

        case 13:
            return (unsigned long) p->workingset_peak_size;
            break;          

        case 14:
            return (unsigned long) p->pagefaultCount;
            break;          

        case 15:
            return (unsigned long) p->DirectoryPA;
            break;
           
        case 16:
            return (unsigned long) p->DirectoryVA;
            break;

        //image address.    
        case 17:
            return (unsigned long) p->Image;
            break;

        case 18:
            return (unsigned long) p->ImagePA;
            break;


        case 19:
            return (unsigned long) p->childImage;
            break;
           
        case 20:
           return (unsigned long) p->childImage_PA;
           break;
          
        case 21:
            return (unsigned long) p->Heap;
            break;

        case 22:
            return (unsigned long) p->HeapEnd;
            break;
           
        case 23:
            return (unsigned long) p->HeapSize;
            break;


        case 24:
            return (unsigned long) p->HeapPointer;
            break;

        case 25:
            return (unsigned long) p->HeapLastValid;
            break;

        case 26:
            return (unsigned long) p->HeapLastSize;
            break;

        case 27:
            return (unsigned long) p->Stack;
            break;
           
        case 28:
            return (unsigned long) p->StackEnd;
            break;


        case 29:
            return (unsigned long) p->StackSize;
            break;

        case 30:
           return (unsigned long) p->StackOffset;
           break;


        case 31:
            return (unsigned long) p->iopl;
            break;
           
        case 32:
            return (unsigned long) p->base_priority;
            break;
           
        case 33:
            return (unsigned long) p->priority;
            break;

        case 34:
            return (unsigned long) p->step;
            break;


        case 35:
            return (unsigned long) p->quantum;
            break;

        case 36:
            return (unsigned long) p->timeout;
            break;
           
           
        case 37:
            return (unsigned long) p->ticks_remaining;
            break;

        case 38:
            return (unsigned long) p->profiler_percentage_running;
            break;
           
           
        case 39:
            return (unsigned long) p->profiler_ticks_running;
            break;

        case 40:
            return (unsigned long) p->profiler_last_ticks;
            break;

        case 41:
            return (unsigned long) p->threadCount;
            break;

        case 42:
            return (unsigned long) p->bound_type;
            break;

        case 43:
            return (unsigned long) p->preempted;
            break;

        case 44:
            return (unsigned long) p->saved;
            break;

        case 45:
            return (unsigned long) p->PreviousMode;
            break;


        case 46:
            return (unsigned long) p->wait4pid;
            break;

        case 47:
            return (unsigned long) p->exit_code;
            break;

        case 48:
            return (unsigned long) p->signal;
            break;
           
        case 49:
            return (unsigned long) p->signal_mask;
            break;

        case 50:
            return (unsigned long) p->dialog_address;
            break;
           
        // Image size.
        // Isso � importante.
        case 51:
            return (unsigned long) p->ImageSize;
            break;
           
        // #todo:
        // Precisamos da quantidade de p�ginas usadas.
    
        // ...
    };
    
    return 0;
}



//pega o nome do processo.
int getprocessname ( int pid, char *buffer )
{
	struct process_d *p;
	
    char *name_buffer = (char *) buffer;

    //#todo
    //checar validade dos argumentos.
 
    p = (struct process_d *) processList[pid]; 

    if ( (void *) p == NULL )
    {
		return -1;
    }else{
    
        if ( p->used != 1 || p->magic != 1234 )
        {
             return -1;
        }
        
        // 64 bytes
        strcpy ( name_buffer, (const char *) p->__processname );       
        
        return (int) p->processName_len;
    };
    return -1;
}



/*
 ****************************************************
 * do_clone_execute_process:  
 * 
 *     servi�o 900.
 *     Clona o processo atual e executa o processo filho.
 *     O processo pai continua rodando.
 *     Isso � usado pelo processo noraterm.
 *     #obs: Isso funciona.
 */

// Se o processo filho herdar o floxo padr�o, ent�o o processo filho
// pode escrever no seu stdout e o processo pai pode ler no seu
// pr�prio stdout.


// #importante
// >>>> Isso funciona. 
// ( * N�o * ) mexa pois ainda estamos
// trabalahndo os outros m�todos.

// IN: ??
// OUT: ??

pid_t do_clone_execute_process (char *filename){

    int PID;
    int Ret = -1;

    struct process_d *Current;
    struct process_d *Clone;


    unsigned long *dir;
    unsigned long old_dir_entry1;
    
    // retorno da fun��o de carregamento.
    //int __load_ret = -1;
    int Status = -1;



    //
    // Convertendo o formato do nome do arquivo.
    //
    
    // >>> "12345678XYZ"
    
    read_fntos ( (char *) filename );


    //
    // Searching for the file only on the root dir.
    //
    int __Status = -1;
    __Status = (int) KiSearchFile ( filename, VOLUME1_ROOTDIR_ADDRESS );
    if (__Status == 1){
         printf("found\n");
    }else{
         printf("not found\n");
         goto fail;
    };


	//unsigned long old_image_pa; //usado para salvamento.

	//#debug message.
    //printf ("do_clone_execute_processs: clona o pai e executa o filho..\n");


	// ## Current ##
	// Checando a validade do processo atual.

	//if ( current_process < 0 )
	//    return 0;

	Current = (struct process_d *) processList[current_process];
	
	if ( (void *) Current == NULL )
	{
		printf ("do_clone_execute_process: Current struct \n");
		goto fail;
	
	}else{
		
		if ( Current->used != 1 || Current->magic != 1234 )
		{    
			printf ("do_clone_execute_process: Current validation \n");
			goto fail;		
		}
		
		//#test
        dir = (unsigned long *) Current->DirectoryVA;	
		old_dir_entry1 = dir[1]; //salvando
		
		//salvando o endere�o f�sico da imagem que existe no processo.
		//old_image_pa = (unsigned long) virtual_to_physical ( Current->Image, gKernelPageDirectoryAddress ); 
		
	    //#debug
	    //printf(">>> check current process: %d %d \n", current_process, Current->pid );
		goto do_clone;
		//...
	};
	
	
	//
	// ## Clone ##
	//
	
do_clone:
	
	//Cria uma estrutura do tipo processo, mas n�o inicializada.

    Clone = (struct process_d *) processObject ();

    if ( (void *) Clone == NULL ){
        printf ("do_clone_execute_process: Clone struct fail \n");
        goto fail;

    } else {
		
		// Obt�m um �ndice para um slot vazio na lista de processos.
		
        PID = (int) getNewPID ();

		//if ( PID <= 0 )
		if ( PID == -1 || PID == 0 )
		{	
			printf ("do_clone_execute_process: getNewPID fail %d \n", PID );
			goto fail;
		}

		Clone->pid = PID;
		
		Clone->used = 1;
		Clone->magic = 1234;
		
		//#obs: Na hora de copiar o processo, a estrutura do clone 
		//receber� os valores da estrutura do processo atual,
		//at� mesmo o endere�o do diret�rio de p�ginas.
		
		//...
		
		//Salvando na lista.
		
		processList[PID] = (unsigned long) Clone;
		
		
		
		
		//
		// Clone.
		//
		
		// Copia a mem�ria usada pela imagem do processo.

        processCopyMemory ( Current );


		//
		// Debug nessages
		//

        //unsigned long *xxx = (unsigned long *) Current->DirectoryVA;	
        //printf ("DirectoryVA = %x \n",Current->DirectoryVA);
        //printf ("Directory Entry 1 = %x \n", xxx[1] );
        //printf ("childImage_PA = %x \n",Current->childImage_PA);	
        //mmShowPDEForAllProcesses (1);
        //printf ("DirectoryVA = %x \n",Current->DirectoryVA);
        //printf ("Directory Entry 1 = %x \n", xxx[1] );
        //kprintf ("debug *breakpoint");	
        //refresh_screen();
        //while (1){}	
		
		
		//
		// Clone the process. 
		//

 
		// Isso cria um diret�rio de p�ginas para o processo clone;
		
        Ret = processCopyProcess ( Current->pid, Clone->pid );

        if ( Ret != 0 ){
            panic ("do_clone_execute_process: processCopyProcess fail\n");
        }


		//
		// Load file.
		//


        printf ("do_clone_execute_process: %s\n",filename);


        // #importante: 
        // Carregando a imagem do porcesso filho.
        // Se o carregamento falhar, temos que abortar a clonagem
        // caso contr�rio executa a c�pia da imagem do pai.

        // #bugbug
        // Essa rotina come�ou a falhar aqui.
        // Convertendo num formato errado.
        
        // #test
        // Movemos essa convers�o para o in�cio dessa fun��o,
        // onde checaremos se o arquivo est� no diret�rio.
        
        // read_fntos ( (char *) filename );

        printf ("do_clone_execute_process: %s\n",filename);
        
        Status = (int) fsLoadFile ( VOLUME1_FAT_ADDRESS, 
                           VOLUME1_ROOTDIR_ADDRESS, 
                           filename, 
                           (unsigned long) Clone->Image );

       // Se falhou o carregamento.
       if ( Status != 0 )
       {

            // #bugbug
            // Isso n�o est� funcionando direito.
            // E uma thread defeituosa fica remanescente quando
            // digitamos um comando errado.
            
            
            // kill thread.
            Clone->control->used = 0;
            Clone->control->magic = 0;
            Clone->control->state = DEAD;
            Clone->control == NULL;
            
            // kill process.
            Clone->used = 0;
            Clone->magic = 0;  
            Clone = NULL;
            
            printf ("do_clone_execute_process: Couldn't load the file %s\n", 
                filename );
            goto fail;
       }
       

       // Check ELF signature.
       // OK. O comando existe e o arquivo foi carregado, mas 
       // precisamos saber se a assinatura de ELF � v�lida.
       Status = (int) fsCheckELFFile ( (unsigned long) Clone->Image );
       
       if ( Status != 0 )
       {
            // kill thread.
            Clone->control->used = 0;
            Clone->control->magic = 0;
            Clone->control == NULL;
            
            // kill process.
            Clone->used = 0;
            Clone->magic = 0;            
            Clone = NULL;
            
            printf ("do_clone_execute_process: file format fail \n");
            goto fail;
        };


        //
        // Page table.
        //
        
        // #obs:
        // Lembrando que ja criamos o diret�rio de p�ginas.

        // #importante
        // Remapeando a imagem, mas agora no diret�rio de p�ginas
        // do processo filho.
        // >> Esse n�mero de entrada � para o endere�o virtual padr�o
        // para aplicativos em ring3. = 0x400000
     
        CreatePageTable ( (unsigned long) Clone->DirectoryVA, 
            ENTRY_USERMODE_PAGES, 
            Clone->ImagePA );     //Current->childImage_PA );

        Clone->Image = 0x400000; 


        // name.

        Clone->processName_len = (size_t) strlen ( (const char *) filename );
        strcpy ( Clone->__processname, (const char *) filename );   
        
        //debug para len.
        //printf ("len %d",Clone->processName_len); 
        //refresh_screen();
        //while(1){}



        //
        // eip
        //

        // Entry point para clonagem.
        Clone->control->eip = 0x400000 + 0x1000;


        //
        // esp
        //


		//====
		// #bugbug : 
		// Essa pilha est� dentro da imagem. ...
		// Como o gramado core n�o existe mais. Vamos
		// aproveitar para colocar a pilha num lugar mais confot�vel
		// dentro dos 4MB da �rea de aplicativo.
		// veja o exemplo da thread do processo init.
		// e se o aplicativo tiver mais que 63KB.???
		//Clone->control->esp = 0x400000 + (1024 * 63);
		//Clone->control->esp = 0x400000 + (1024 * 200);   //funciona
		Clone->control->esp = 0x004FFFF0; // funcionou.
		//Clone->control->eip = Current->control->eip; //#bug fail
		//Clone->control->esp = Current->control->esp; //#bug fail
		//====




		//
		// Debug
		//
		
		
		// mmShowPDEForAllProcesses (1);
		// show_thread_information (); 

		// [current]
		// printf ("\n");
		// mostra_reg (Current->control->tid);
		// printf ("Current.DirectoryVA = %x \n",Current->DirectoryVA);
		// printf ("Current.DirectoryPA = %x \n",Current->DirectoryPA);
		// printf ("Current.Image = %x \n",Current->Image);
		// printf ("Current.ImagePA = %x \n",Current->ImagePA);
		// printf ("Current.control.eipVA = %x \n",Current->control->eip);


		// [clone]
		//printf ("\n");
		//mostra_reg (Clone->control->tid);
		//printf ("Clone.DirectoryVA = %x \n",Clone->DirectoryVA);
		//printf ("Clone.DirectoryPA = %x \n",Clone->DirectoryPA);
		//printf ("Clone.Image = %x \n",Clone->Image);
		//printf ("Clone.ImagePA = %x \n",Clone->ImagePA);
		//printf ("Clone.control.eip = %x \n",Clone->control->eip);
		//printf ("Clone.control.esp = %x \n",Clone->control->esp);
		// printf ("\n : *breakpoint");
		//refresh_screen();
		//while(1){}



		//#hackhack

		// [pai]
		Current->control->state = READY;


		// [filho]
		Clone->control->saved = 0;
		SelectForExecution (Clone->control);


        // Se o processo filho herdar o floxo padr�o, ent�o o 
        // processo filho pode escrever no seu stdout e o processo 
        // pai pode ler no seu pr�prio stdout.


		//#test
		//Clone->control->stdin  = stdin; //Current->control->stdin;
		//Clone->control->stdout = stdout; //Current->control->stdout;
		//Clone->control->stderr = stderr; //Current->control->stderr;
		//Clone->Streams[0] = (unsigned long) stdin; //Current->control->stdin;
		//Clone->Streams[1] = (unsigned long) stdout; //Current->control->stdout;
		//Clone->Streams[2] = (unsigned long) stderr; //Current->control->stderr;

		
		//
		// Return.
		//
		
		// #obs:
		// Retornamos para o pai o PID do filho.
		// Igual acontece no fork().
		
		//pai
        current_thread = Current->control->tid;
        current_process = Current->pid;

        return (pid_t) Clone->pid;
	};

	// Fail.

fail:
    refresh_screen ();
    return (pid_t) -1;
}



/*
 ***************************************
 * do_fork_process2 (t901)
 *     (Fun��o em desenvolvimento)
 *     Isso est� clonando o processo atual e executando o processo filho.
 *     O Processo pai continua rodando.
 *     #todo: A inten��o � modificar essa rotina para ela ser um fork();
 */

// t901
//clona um processo, retorna par ao pai e inicializa o processo
//filho do seu entrypoint. (#test)

pid_t do_fork_process2 (void){

	int PID;	
	int Ret = -1;	

	struct process_d *Current;	
	struct process_d *Clone;
	
	unsigned long *dir;
	unsigned long old_dir_entry1; 
	
 
	//unsigned long old_image_pa; //usado para salvamento.
	
    //#debug message.	
	printf ("do_fork_process2: Cloning the current process..\n");
	
	
	// ## Current ##
	// Checando a validade do processo atual.
	
	//if ( current_process < 0 )
	//    return 0;
	
	Current = (struct process_d *) processList[current_process];
	
	if ( (void *) Current == NULL )
	{
		printf("do_fork_process2: current, struct \n");
		goto fail;
	
	}else{
		
		if ( Current->used != 1 || Current->magic != 1234 )
		{    
			printf ("do_fork_process2: current, validation \n");
			goto fail;		
		}
		
		//#test
        dir = (unsigned long *) Current->DirectoryVA;	
		old_dir_entry1 = dir[1]; //salvando
		
		//salvando o endere�o f�sico da imagem que existe no processo.
		//old_image_pa = (unsigned long) virtual_to_physical ( Current->Image, gKernelPageDirectoryAddress ); 		
		
	    //#debug
	    //printf(">>> check current process: %d %d \n", current_process, Current->pid );		
		goto do_clone;
		//...
	};
	
	
	//
	// ## Clone ##
	//
	
do_clone:
	
	//Cria uma estrutura do tipo processo, mas n�o inicializada.
	
	Clone = (struct process_d *) processObject ();
	
	if ( (void *) Clone == NULL )
	{
		printf ("do_fork_process2: Clone struct fail \n");
		
		goto fail;
	
	} else {
		
		
		// Obt�m um �ndice para um slot vazio na lista de processos.
		
	    PID = (int) getNewPID ();
		
		//if ( PID <= 0 ){
			
		if ( PID == -1 || PID == 0 )
		{	
			printf ("do_fork_process2: getNewPID fail %d \n", PID );
			goto fail;
		}
		
		Clone->pid = PID;
		
		Clone->used = 1;
		Clone->magic = 1234;
		
		//#obs: Na hora de copiar o processo, a estrutura do clone 
		//receber� os valores da estrutura do processo atual,
		//at� mesmo o endere�o do diret�rio de p�ginas.
		
		//...
		
		//Salvando na lista.
		
		processList[PID] = (unsigned long) Clone;
		
		//
		// ## clone  ##
		//
		
		// Copia a mem�ria usada pela imagem do processo.
		
		processCopyMemory ( Current );	
		
		//
		// Debug messages.
		//
		
        //unsigned long *xxx = (unsigned long *) Current->DirectoryVA;	
	    //printf ("DirectoryVA = %x \n",Current->DirectoryVA);
	    //printf ("Directory Entry 1 = %x \n", xxx[1] );
	    //printf ("childImage_PA = %x \n",Current->childImage_PA);	
	
	    //mmShowPDEForAllProcesses (1);
	    
		//printf ("DirectoryVA = %x \n",Current->DirectoryVA);
	    //printf ("Directory Entry 1 = %x \n", xxx[1] );
	    
		//kprintf ("debug *breakpoint");	
	    //refresh_screen();
	    //while (1){}	
		
		
		//
		// Clone process.
		//
		
		// Isso cria um diret�rio de p�ginas para o processo clone;
		
	    Ret = processCopyProcess ( Current->pid, Clone->pid );
		
	    if ( Ret != 0 )
	    {
		    panic ("do_fork_process2: processCopyProcess fail\n");
		    //goto fail;	
	    }
		
        CreatePageTable ( (unsigned long) Clone->DirectoryVA, ENTRY_USERMODE_PAGES, 
		    Current->childImage_PA );		
		
		
		//#test
		// recuperamos a informa��o que o pai perdeu quando 
		// sobrescrevemos uma pagedir.
		//Current->Image = va da imagem do processo pai
		//unsigned long old_image_pa;
		//old_image_pa = (unsigned long) virtual_to_physical ( Current->Image, gKernelPageDirectoryAddress ); 
		
		//usando valores salvos anteriormente. � do pai.
		//CreatePageTable ( (unsigned long) Current->DirectoryVA, ENTRY_USERMODE_PAGES, old_image_pa );			
		    
		//dir[1] = old_dir_entry1;
		
		// Ok, retornando o n�mero do processo clonado.

				
		//
		// Current thread.
		//
		
        //#test
		// Vamos associar ao primeiro tty, mesmo que seja um aplicatibo GUI.
		// Se ele for um aplicativo GUI ele ir� atualizar o foco.
		// Se for um aplicativo de terminal ent�o ter� uma janela 
		// para rodar. Pois o ldisc manda mensagens para a thread de controle 
		// da janela com foco de entrada. Vamos fazer isso manualmente.
		
		/*
		 ## bugbug: 
		 Essa rotina faz o input n�o funcionar quando retornamos para o pai.
		
		if ( (void *) CurrentTTY != NULL )
		{
			if ( CurrentTTY->used == 1 && CurrentTTY->magic == 1234 )
			{
				current_tty = CurrentTTY->index;
				
				Clone->control->tty_id = current_tty;
				
				// #terminal window.
				window_with_focus = CurrentTTY->window->id;
                terminal_window = CurrentTTY->window->id;
				
				//#importante
				//a thread de controle da janela, para qual
				//ser�o enviadas as mensagens pelo ldisc
				CurrentTTY->window->control = Clone->control;
			}
			
		}else{
		    //Thread->tty_id = 0; //-1
        } 
		*/
		
		/*
#bug: Quando o kernel salta 
para eip do novo processo ele est� usando o seu pr�prio endere�amento. Mas o kernel
deve considerar o endere�amento do novo processo, pois isso aponta para
um nodo eip fisico.
os processo anteriores deram certo pois os endere�amentos eram iguais, todos clones do endere�amento do kernel.		
		*/
		
		
		//
		// Debug messages.
		//
		
		
		// mmShowPDEForAllProcesses (1);
		
		// show_thread_information (); 
		
		// printf ("\n");
		
		// [current]
		// mostra_reg (Current->control->tid);
		// printf ("Current.DirectoryVA = %x \n",Current->DirectoryVA);
		// printf ("Current.DirectoryPA = %x \n",Current->DirectoryPA);
		// printf ("Current.Image = %x \n",Current->Image);
		// printf ("Current.ImagePA = %x \n",Current->ImagePA);
		// Current->control->eipPA = (unsigned long) virtual_to_physical ( Current->control->eip, Current->DirectoryVA ); 
		// printf ("Current.control.eipVA = %x \n",Current->control->eip);
		// printf ("Current.control.ring0_eipVA = %x \n",Current->control->ring0_eip);
		// printf ("Current.control.eipPA = %x \n",Current->control->eipPA);

		
		// [clone]
		// mostra_reg (Clone->control->tid);
		// printf ("Clone.DirectoryVA = %x \n",Clone->DirectoryVA);
		// printf ("Clone.DirectoryPA = %x \n",Clone->DirectoryPA);
		// printf ("Clone.Image = %x \n",Clone->Image);
		// printf ("Clone.ImagePA = %x \n",Clone->ImagePA);
		// #bugbug: Esse � o endere�o l[ogico em que deve estar a imagem do clone
		// na vis�o do diret�rio do clone.
		// Clone->Image = 0x400000;
		// Clone->ImagePA = (unsigned long) virtual_to_physical ( Clone->Image  , Clone->DirectoryVA ); 
		// agora visto com o diret�rio do processo clone.
		// printf ("Clone.Image = %x \n",Clone->Image);
		// printf ("***Clone.ImagePA = %x \n",Clone->ImagePA);
		// Clone->control->eipPA = (unsigned long) virtual_to_physical ( Clone->control->eip  , Clone->DirectoryVA ); 		
		// printf ("Clone.control.eipVA = %x \n",Clone->control->eip);
		// printf ("Clone.control.ring0_eipVA = %x \n",Clone->control->ring0_eip);
		// printf ("Clone.control.eipPA = %x \n",Clone->control->eipPA);
		
		
		// printf ("\ndo fork process: *breakpoint");
		// refresh_screen();
		// while(1){}
		
		//
		// Load child image.
		//
		
		// #importante: 
	    // Carregando a image do processo filho.
		
		
        //fsLoadFile ( VOLUME1_FAT_ADDRESS, VOLUME1_ROOTDIR_ADDRESS, 
	     //   "GDESHELL BIN", (unsigned long) Clone->Image );		
		
		
		//
		// ## TEST ##
		//
		
		
		//#test - Clonando manualmente a thread de controle.
		//s� a imagem ... falta a pilha.
		memcpy ( (void *) Clone->Image, (const void *) Current->Image, ( 0x50000 ) ); 
		//====
		Clone->control->type  = Current->control->type; 
		Clone->control->plane = Current->control->plane;
		Clone->control->base_priority = Current->control->base_priority;
		Clone->control->priority = Current->control->priority;
		Clone->control->iopl = Current->control->iopl;
		Clone->control->preempted = Current->control->preempted;
		Clone->control->step = Current->control->step;
		Clone->control->quantum = Current->control->quantum;
		Clone->control->quantum_limit = Current->control->quantum_limit;
		Clone->control->standbyCount = Current->control->standbyCount;
		Clone->control->runningCount = Current->control->runningCount;
		Clone->control->initial_time_ms = Current->control->initial_time_ms;
		Clone->control->total_time_ms = Current->control->total_time_ms;
		Clone->control->runningCount_ms = Current->control->runningCount_ms;
		Clone->control->readyCount = Current->control->readyCount;
		Clone->control->ready_limit = Current->control->ready_limit;
		Clone->control->waitingCount = Current->control->waitingCount;
		Clone->control->waiting_limit = Current->control->waiting_limit;
		Clone->control->blockedCount = Current->control->blockedCount;
		Clone->control->blocked_limit = Current->control->blocked_limit;
		Clone->control->ticks_remaining = Current->control->ticks_remaining;
		Clone->control->signal = Current->control->signal;
		Clone->control->signalMask = Current->control->signalMask;
		Clone->control->ss = Current->control->ss;
		Clone->control->esp = Current->control->esp;   //>>>
		Clone->control->eflags = Current->control->eflags;
		Clone->control->cs = Current->control->cs;
		Clone->control->eip = Current->control->eip; // >>>
		Clone->control->initial_eip = Current->control->initial_eip;
		Clone->control->ds = Current->control->ds;
		Clone->control->es = Current->control->es;
		Clone->control->fs = Current->control->fs;
		Clone->control->gs = Current->control->gs;
		Clone->control->eax = 0; //Current->control->  >>>
		Clone->control->ebx = Current->control->ebx;
		Clone->control->ecx = Current->control->ecx;
		Clone->control->edx = Current->control->edx;
		Clone->control->esi = Current->control->esi;
		Clone->control->edi = Current->control->edi;
		Clone->control->ebp = Current->control->ebp;
		Clone->control->tss = Current->control->tss;
		//process	
		// for wait_reason[w]
		//exit_code	
		//====
		
		//#bugbug ATEN��O ATEN��O
		//Funciona que iniciamos o filho no seu entrypoint,
		//mas n�o funciona quando iniciamos no mesmo eip do pai.
		//A diferen�a entre os dois casos � s� o eip.
		
			//====
		Clone->control->eip = 0x400000 + 0x1000;
		Clone->control->esp = 0x450000 - 0x1000;
		//Clone->control->eip = Current->control->eip; //#bug fail
		//Clone->control->esp = Current->control->esp; //#bug fail
		//====


		//ok
		//printf ("Current: %s\n", Current->Image + 0x1000);
		//printf ("Clone: %s\n", Clone->Image + 0x1000);

        //mostra_reg (Clone->control->tid);		
		//refresh_screen();
		//while(1){}
		
		//#hackhack

		// [pai]
		Current->control->quantum = 50;
		//Current->control->quantum = 100;
		//Current->control->saved = 0;
		Current->control->state = READY;
        //SelectForExecution (Current->control);
			
		// [filho]
		Clone->control->quantum = 50;
		//Clone->control->quantum = 200;
		Clone->control->saved = 0;
		Clone->control->state = READY;
		SelectForExecution (Clone->control);

        //
		// return
		//
		
		// #importante:
		// Retornamos para o processo pai o PID do filho.
		
		//pai
		current_thread = Current->control->tid;	
		current_process = Current->pid;
        return (pid_t) Clone->pid;
		
		//filho
		//current_thread = Clone->control->tid;	
		//current_process = Clone->pid;
        //return (pid_t) 0;

		
	};

    // Fail.	
    	
fail:
	
    return (pid_t) -1;	
}




/*
 ***************************************
 * do_fork_process 
 *     Isso est� clonando o processo atual e executando o processo filho.
 *     Essa � a rotina que implementa a fun��o fork() padr�o da libc. 
 *     O Processo pai continua rodando.
 * 
 *     Esse rotina � chamada pela fun��o gde_fork() em gdeserv.c
 *      
 */
 
// #todo
// Criar um fork() usando essa rotina.
// Estamos testando com o aplicativo gdeshell, usando o comando fork.
// * Falta pouco. :)

pid_t do_fork_process (void){

    int PID;

    struct process_d *Current;
    struct process_d *Clone;

    unsigned long *dir;
    unsigned long old_dir_entry1; 
	//unsigned long old_image_pa; //usado para salvamento.

    int Ret = -1;
    int i;
    int w;



    // #debug: 
    // Message.
    //printf ("do_fork_process: Cloning the current process..\n");


	// ## Current ##
	// Checando a validade do processo atual.
	
	//if ( current_process < 0 )
	//    return 0;


	Current = (struct process_d *) processList[current_process];
	
	if ( (void *) Current == NULL )
	{
		printf ("do_fork_process: Current struct \n");
		goto fail;
	
	}else{
		
		if ( Current->used != 1 || Current->magic != 1234 )
		{    
			printf ("do_fork_process: Current validation \n");
			goto fail;
		}
		
		//#test
        dir = (unsigned long *) Current->DirectoryVA;
		old_dir_entry1 = dir[1];    //salvando
		
		//salvando o endere�o f�sico da imagem que existe no processo.
		//old_image_pa = (unsigned long) virtual_to_physical ( Current->Image, gKernelPageDirectoryAddress ); 		
		
	    //#debug
	    //printf(">>> check current process: %d %d \n", current_process, Current->pid );		
		goto do_clone;
		//...
	};


	//
	// # Clone #
	//

	// Cria uma estrutura do tipo processo, mas n�o inicializada.
	
do_clone:

    Clone = (struct process_d *) processObject ();

    if ( (void *) Clone == NULL )
    {
		printf ("do_fork_process: Clone struct fail \n");
		goto fail;
    } else {
		
		// Obt�m um �ndice para um slot vazio na lista de processos.
		
        PID = (int) getNewPID ();

        //#todo: melhorar essa checagem.
		//if ( PID <= 0 ){
		if ( PID == -1 || PID == 0 )
		{	
			printf ("do_fork_process: getNewPID fail %d \n", PID );
			goto fail;
		}

		Clone->pid = PID;
		
		Clone->used = 1;
		Clone->magic = 1234;

		// #obs: 
		// Na hora de copiar o processo, a estrutura do clone 
		// receber� os valores da estrutura do processo atual,
		// at� mesmo o endere�o do diret�rio de p�ginas.


		// Salvando na lista.
        processList[PID] = (unsigned long) Clone;


		//
		// # clone #
		//

		// Copia a mem�ria usada pela imagem do processo.
		// Isso copiar� a imagem do processo e colocar� o novo endere�o
		// em Current->childImage  e Current->childImage_PA
		processCopyMemory ( Current );

		//
		// Debug messages.
		//
		
        //unsigned long *xxx = (unsigned long *) Current->DirectoryVA;	
	    //printf ("DirectoryVA = %x \n",Current->DirectoryVA);
	    //printf ("Directory Entry 1 = %x \n", xxx[1] );
	    //printf ("childImage_PA = %x \n",Current->childImage_PA);	
	
	    //mmShowPDEForAllProcesses (1);
	    
		//printf ("DirectoryVA = %x \n",Current->DirectoryVA);
	    //printf ("Directory Entry 1 = %x \n", xxx[1] );
	    
		//kprintf ("debug *breakpoint");	
	    //refresh_screen();
	    //while (1){}	
		
		
		//
		// Clone process.
		//
		
		// Isso cria um diret�rio de p�ginas para o processo clone;
        Ret = processCopyProcess ( Current->pid, Clone->pid );

        if ( Ret != 0 )
        {
            panic ("do_fork_process: processCopyProcess fail\n");
        }

        CreatePageTable ( (unsigned long) Clone->DirectoryVA, 
            ENTRY_USERMODE_PAGES, 
            Current->childImage_PA );


		//#test
		// recuperamos a informa��o que o pai perdeu quando 
		// sobrescrevemos uma pagedir.
		//Current->Image = va da imagem do processo pai
		//unsigned long old_image_pa;
		//old_image_pa = (unsigned long) virtual_to_physical ( Current->Image, gKernelPageDirectoryAddress ); 
		
		//usando valores salvos anteriormente. � do pai.
		//CreatePageTable ( (unsigned long) Current->DirectoryVA, ENTRY_USERMODE_PAGES, old_image_pa );			
		    
		//dir[1] = old_dir_entry1;
		
		// Ok, retornando o n�mero do processo clonado.
		
		//printf ("do_fork_process: done\n");
				
		//
		// Current thread.
		//
		
        //#test
		// Vamos associar ao primeiro tty, mesmo que seja um aplicatibo GUI.
		// Se ele for um aplicativo GUI ele ir� atualizar o foco.
		// Se for um aplicativo de terminal ent�o ter� uma janela 
		// para rodar. Pois o ldisc manda mensagens para a thread de controle 
		// da janela com foco de entrada. Vamos fazer isso manualmente.
		
		/*
		 ## bugbug: 
		 Essa rotina faz o input n�o funcionar quando retornamos para o pai.
		
		if ( (void *) CurrentTTY != NULL )
		{
			if ( CurrentTTY->used == 1 && CurrentTTY->magic == 1234 )
			{
				current_tty = CurrentTTY->index;
				
				Clone->control->tty_id = current_tty;
				
				// #terminal window.
				window_with_focus = CurrentTTY->window->id;
                terminal_window = CurrentTTY->window->id;
				
				//#importante
				//a thread de controle da janela, para qual
				//ser�o enviadas as mensagens pelo ldisc
				CurrentTTY->window->control = Clone->control;
			}
			
		}else{
		    //Thread->tty_id = 0; //-1
        } 
		*/


		/*
        #bug: 
        Quando o kernel salta para eip do novo processo ele est� 
        usando o seu pr�prio endere�amento. Mas o kernel deve 
        considerar o endere�amento do novo processo, pois isso 
        aponta paraum nodo eip f�sico.
        Os processo anteriores deram certo pois os endere�amentos 
        eram iguais, todos clones do endere�amento do kernel.
		*/

		
		//
		// Debug messages.
		//
		
		
		// mmShowPDEForAllProcesses (1);
		
		// show_thread_information (); 
		
		// printf ("\n");
		
		// [current]
		// mostra_reg (Current->control->tid);
		// printf ("Current.DirectoryVA = %x \n",Current->DirectoryVA);
		// printf ("Current.DirectoryPA = %x \n",Current->DirectoryPA);
		// printf ("Current.Image = %x \n",Current->Image);
		// printf ("Current.ImagePA = %x \n",Current->ImagePA);
		// Current->control->eipPA = (unsigned long) virtual_to_physical ( Current->control->eip, Current->DirectoryVA ); 
		// printf ("Current.control.eipVA = %x \n",Current->control->eip);
		// printf ("Current.control.ring0_eipVA = %x \n",Current->control->ring0_eip);
		// printf ("Current.control.eipPA = %x \n",Current->control->eipPA);

		
		// [clone]
		// mostra_reg (Clone->control->tid);
		// printf ("Clone.DirectoryVA = %x \n",Clone->DirectoryVA);
		// printf ("Clone.DirectoryPA = %x \n",Clone->DirectoryPA);
		// printf ("Clone.Image = %x \n",Clone->Image);
		// printf ("Clone.ImagePA = %x \n",Clone->ImagePA);
		// #bugbug: Esse � o endere�o l[ogico em que deve estar a imagem do clone
		// na vis�o do diret�rio do clone.
		// Clone->Image = 0x400000;
		// Clone->ImagePA = (unsigned long) virtual_to_physical ( Clone->Image  , Clone->DirectoryVA ); 
		// agora visto com o diret�rio do processo clone.
		// printf ("Clone.Image = %x \n",Clone->Image);
		// printf ("***Clone.ImagePA = %x \n",Clone->ImagePA);
		// Clone->control->eipPA = (unsigned long) virtual_to_physical ( Clone->control->eip  , Clone->DirectoryVA ); 		
		// printf ("Clone.control.eipVA = %x \n",Clone->control->eip);
		// printf ("Clone.control.ring0_eipVA = %x \n",Clone->control->ring0_eip);
		// printf ("Clone.control.eipPA = %x \n",Clone->control->eipPA);
		
		
		// printf ("\ndo fork process: *breakpoint");
		// refresh_screen();
		// while(1){}
		
		//
		// Load child image.
		//
		
		// #importante: 
	    // Carregando a image do processo filho.
		
		
        //fsLoadFile ( VOLUME1_FAT_ADDRESS, VOLUME1_ROOTDIR_ADDRESS, 
	     //   "GDESHELL BIN", (unsigned long) Clone->Image );
		

		
		
		// Clonando manualmente a thread de controle.
		// S� a imagem ... falta a pilha.

        // #bugbug: size ???
        // Ja n�o fizemos isso quando chamamos processCopyMemory ???
        // l� copiamos 200kb
		memcpy ( (void *) Clone->Image, (const void *) Current->Image, ( 0x50000 ) ); 
		//memcpy ( (void *) Clone->Image, (const void *) Current->Image, (1024*200) );  //bugbug
		//====
		Clone->control->ownerPID = Clone->pid;
		Clone->control->type  = Current->control->type; 
		Clone->control->plane = Current->control->plane;
		Clone->control->base_priority = Current->control->base_priority;
		Clone->control->priority = Current->control->priority;
		Clone->control->iopl = Current->control->iopl;
		Clone->control->preempted = Current->control->preempted;
		Clone->control->step = Current->control->step;
		Clone->control->quantum = Current->control->quantum;
		Clone->control->quantum_limit = Current->control->quantum_limit;
		
		// A thread do processo clone ainda n�o rodou.
		Clone->control->standbyCount = 0;
		Clone->control->runningCount = 0;
		Clone->control->initial_time_ms = get_systime_ms();
		Clone->control->total_time_ms = 0;
		Clone->control->runningCount_ms = 0;
		Clone->control->readyCount = 0;
		Clone->control->ready_limit = READY_LIMIT;
		Clone->control->waitingCount = 0;
		Clone->control->waiting_limit = WAITING_LIMIT;
		Clone->control->blockedCount = 0;
		Clone->control->blocked_limit = BLOCKED_LIMIT;
		
		for ( w=0; w<8; w++ ){
			Clone->control->wait_reason[w] = (int) 0;
		}

		Clone->control->ticks_remaining = Current->control->ticks_remaining;
		Clone->control->signal = Current->control->signal;
		Clone->control->signalMask = Current->control->signalMask;
		
		// CPU context
		
		// stack
		Clone->control->ss          = Current->control->ss;
		Clone->control->esp         = Current->control->esp;  
		//Clone->control->esp = 0x400000 + (1024 * 100);    //bugbug
		Clone->control->eflags      = Current->control->eflags;
		Clone->control->cs          = Current->control->cs;
		Clone->control->eip         = Current->control->eip; 
		Clone->control->initial_eip = Current->control->initial_eip;

		// more registers.
		Clone->control->ds = Current->control->ds;
		Clone->control->es = Current->control->es;
		Clone->control->fs = Current->control->fs;
		Clone->control->gs = Current->control->gs;
		Clone->control->eax = 0;
		Clone->control->ebx = Current->control->ebx;
		Clone->control->ecx = Current->control->ecx;
		Clone->control->edx = Current->control->edx;
		Clone->control->esi = Current->control->esi;
		Clone->control->edi = Current->control->edi;
		Clone->control->ebp = Current->control->ebp;
		

		
		// #bugbug 
		// ATEN��O ATEN��O
		// Funciona que iniciamos o filho no seu entrypoint,
		// mas n�o funciona quando iniciamos no mesmo eip do pai.
		// A diferen�a entre os dois casos � s� o eip.

		//====
		//Clone->control->eip = 0x400000 + 0x1000;
		//Clone->control->eip = Current->control->eip; //#bug fail
		//====

       //#debug
       //printf ("#debug Current_control_eip = %x *hang \n",Current->control->eip);
       //refresh_screen();
       //while(1){}
		
		// tss
		Clone->control->tss = Current->control->tss;
        
        Clone->control->Next = NULL;
        Clone->control->exit_code = 0;
        //====


       Clone->ppid = Current->pid; 
       Clone->uid = Current->uid;
       Clone->gid = Current->gid;
       Clone->tty = Current->tty;
       //strncpy(proc->name, curr_proc->name, NAME_MAX);
       Clone->plane = Current->plane;
       Clone->personality = Clone->personality;
       Clone->base_priority = Current->base_priority;       
       Clone->priority = Current->priority;
       Clone->step = 0;
       Clone->quantum = Current->quantum;
       Clone->bound_type = Current->bound_type;
       Clone->preempted = Current->preempted;
       Clone->usession = Current->usession;
       Clone->room = Current->room;
       Clone->desktop =Current->desktop;
       Clone->wait4pid = 0;
       Clone->exit_code = 0;
       Clone->nchildren = 0;
       
       Current->nchildren++;
       
       //Clone->signal = 0;
       Clone->signal_mask = Current->signal_mask;
       
       Clone->iopl = Current->iopl;
       //Clone->prev = NULL;
       Clone->next = NULL;
       
       // lista de arquivos abertos.
       for (i=0; i<NUMBER_OF_FILES; i++)
       {  Clone->Streams[i] = Current->Streams[i]; }
       
       for (i=0; i<64; i++)
       {  Clone->Objects[i] = Current->Objects[i]; }


		//ok
		//printf ("Current: %s\n", Current->Image + 0x1000);
		//printf ("Clone: %s\n", Clone->Image + 0x1000);

        //mostra_reg (Clone->control->tid);
		//refresh_screen();
		//while(1){}
		
		
		
		// #importante
		// O dispatcher precis disso na hora que restaura
		// o contexto.
		// Mas isso ja foi feito antes quando copiamos o processo.
		
		Clone->control->DirectoryPA = Clone->DirectoryPA;
		

        // #hackhack

        // #bugbug
        // Chamar a fun��o que muda a prioridade. Ela afeta o quantum.

        // [pai]
        Current->control->quantum = 30;
        Current->control->saved = 0;
        Current->control->state = READY;


        // [filho]
        Clone->control->quantum = 30;
        Clone->control->saved = 1;
        Clone->control->state = READY;



        //
        // return
        //


		// #importante:
		// Retornamos para o processo pai o PID do filho.
		
		//pai
		current_thread = Current->control->tid;
		current_process = Current->pid;
		
		// Isso retornou corretamente para o processo pai
		// o pid do filho.
        return (pid_t) Clone->pid;
    };

    // Fail.

fail:

    return (pid_t) -1;
}


/*
 * processObject:
 *     Cria uma estrutura do tipo processo, mas n�o inicializada.
 *     #todo: Criar a mesma rotina para threads e janelas.
 */
 
struct process_d *processObject (void){
	
	struct process_d *p;
	
	p = (void *) kmalloc ( sizeof(struct process_d) );


    if ( (void *) p == NULL )
    {
        panic ("process-processObject:");
    };
	
	return (struct process_d *) p;
}


/*
 * getNewPID:
 *     Pegar um slot vazio na lista de processos.
 *     +Isso pode ser usado para clonar um processo.
 */
 
pid_t getNewPID (void){
	
	struct process_d *p;
	
	
	// Come�aremos a busca onde come�a o range de IDs de processos de usu�rio.
	
	int i = USER_BASE_PID;
	
	while ( i < PROCESS_COUNT_MAX )
	{
		
	    p = (struct process_d *) processList[i];	
		
		// Se encontramos um slot vazio.
		// retornaremos o �ndice.
		
		if ( (void *) p == NULL )
		{	
			return (pid_t) i;
		}
		
		i++;
	};
	
	// Fail.
	
    return (pid_t) -1;
}


/*
 * processTesting:
 *     Testando se o processo � v�lido. Se for v�lido retorna 1234.
 *     @todo: repensar os valores de retorno. 
 * system call (servi�o 88.)
 */

int processTesting (int pid){
	
	struct process_d *P;
	
	P = (void *) processList[pid];
	
	if ( (void *) P == NULL )
	{
		return 0;
		
	}else{		
		
		if ( P->used == 1 && P->magic == 1234 )
		{
			// magic.
	        return (int) 1234;
		}
	};
	
	// Fail.
	
	return 0;
}


/*
 * processSendSignal:
 *     Envia um sinal para um processo.
 *     Se o sinal e o processo forem v�lidos, um sinal � colocado
 * no PCB do processo.
 *     @todo: Rotinas envolvendo sinais devem ir para outro arquivo.
 */

int processSendSignal (struct process_d *p, unsigned long signal){
	
	//SIGNAL_COUNT_MAX
	
	//Limit
    //if(signal >= 32){
	//	return 1;
	//}
	
	if (signal == 0)
	{
		return 1;
	}
	
	//struct fail
	//if( (void*) p == NULL ){
	//	return 1;
	//}		
	
//ok:	
	//Ok
	if ( (void*) p != NULL )
	{	
		p->signal = (unsigned long) signal;
		return 0; //(int) signalSend(p,signal);
	}
	
	//...
	
//fail:
	
	return 1;
}


/*
 ****************************
 * processCopyMemory:
 *     Copia a imagem.
 *     Usado no fork.
 */

int processCopyMemory ( struct process_d *process ){

    unsigned long __new_base;


	// #todo: 
	// Mensagem de erro.

    if ( (void *) process == NULL )
        return -1;


	//#todo: Mensagem.	
	//if ( (void *) clone == NULL )
	//	return -1;
	
    //
	// Image base.
	//
	
	
	// 200 KB.
	// Alocando mem�ria para a imagem do processo.
	//>>  ring 3 ??.

    __new_base = (unsigned long) allocPages ( (1024*200)/4096 ); 

    if ( __new_base == 0 ){
        printf ("processCopyMemory: fail\n");
        return -1;
    }


	//
	// Copying memory.
	//
	
	//#debug
	//printf ("copying memory ...\n");
	
	//copia 200 KB
    memcpy ( (void *) __new_base, 
             (const void *) process->Image, 
             ( 1024 * 200 ) );


	// Transformando o endere�o virtual em f�sico.
	unsigned long new_base_PA = (unsigned long) virtual_to_physical ( __new_base, gKernelPageDirectoryAddress ); 
	
	
	//#debug
	//printf ("base da imagem new_base_PA=%x  \n", new_base_PA );
	
	
	//
	// directory
	//
	
	//printf ("DirectoryPA=%x \n",process->DirectoryPA);
	//printf ("DirectoryPA=%x \n",clone->DirectoryPA);
	
	// Load here.
	// Altera uma pagetable do diret�rio de p�ginas de um processo.
	// IN: endere�o f�sico do diret�rio de p�ginas do processo, indice da entrada
	// que vamos alterar, endere�o f�sico da regi�o de 4MB que vamos mapear.
	// ENTRY_USERMODE_PAGES referece ao indice para o endere�o virtual 0x400000
	
	// #bugbug
	// Essa fun��o est� falhando. #PF.
	// >> Essa parte est'a falhando na m'aquina real, mas n�o na VirtualBox.
	
	//status: 0 = fail; address = ok
	
 

	// O processo est� num novo endere�o segundo o diret�rio 
	// de p�ginas do kernel.
    process->childImage = (unsigned long) __new_base;
    process->childImage_PA = (unsigned long) new_base_PA;



// Done.
	
	//#debug
	//printf ("processCopyMemory: ok\n");
	//refresh_screen ();

    return 0;
}


/*
 ****************************************
 * processCopyProcess
 *     + Copia os elementos da estrutura de processo.
 *     + Cria um diret�rio de p�ginas e salva os endere�os 
 *       virtual e f�sico dele na estrutura de processo.
 *
 *     Isso � chamado por do_fork_process.
 */
 
// 1 = atual.
// 2 = clone. 
 
int processCopyProcess ( pid_t p1, pid_t p2 ){

    struct process_d *Process1;
    struct process_d *Process2;

    int Status = 0;



    if ( p1 == p2 ){
        printf ("processCopyProcess: same PID\n");
        goto fail;
    }


	//Check limits
	//if( p1 < 1 ...
	//if( p2 < 1 ...

    Process1 = (struct process_d *) processList[p1];
    Process2 = (struct process_d *) processList[p2];


    // Check Process1
    if ( (void *) Process1 == NULL )
    {
        printf ("processCopyProcess: Process1\n");
        goto fail;

    }else{

        if ( Process1->used != 1 || Process1->magic != 1234 )
        {
           printf ("processCopyProcess: Process1 used magic \n");
           goto fail;
        }
    };


    // Check Process2
    if ( (void *) Process2 == NULL )
    {
        printf ("processCopyProcess: Process1\n");
        goto fail;
    }else{

        if ( Process2->used != 1 || Process2->magic != 1234 )
        {
           printf ("processCopyProcess: Process2 used magic \n");
           goto fail;
        }
    };


    //
    // Copy.
    //

    // Object.
    Process2->objectType = Process1->objectType;
    Process2->objectClass = Process1->objectClass;


	//Identificadores.
    Process2->pid  = (int) p2;          // PID.  O pid do clone.
    Process2->ppid = Process1->pid;     // PPID. O parent do clone � o pid do pai. 
    Process2->uid  = Process1->uid;     // UID. 
    Process2->gid  = Process1->gid;     // GID. 

    // validation.
    Process2->used = Process1->used;
    Process2->magic = Process1->magic;


	//State of process
    Process2->state = Process1->state;  

	//Plano de execu��o.
    Process2->plane = Process1->plane;


	//Process->name_address = NULL;

    Process2->framepoolListHead = Process1->framepoolListHead;





	//
	// Directory
	//

	// #bugbug
	// Precisamos clonar o diret�rio de p�ginas
	// sen�o alguma altera��o feita na pagetable da imagem pode
	// corromper o processo que est� sendo clonado.

    // #importante:
    // Deve retornar o endere�o do diret�rio de p�ginas criado,
    // que � um clone do diret�rio de p�ginas do kernel.
    // Retornaremos o endere�o virtual, para que a fun��o create_process 
    // possa usar tanto o endere�o virtual quanto o f�sico.

	// #bugbug
	// Na verdade precisamos clonar o diret�rio do processo e n�o o 
	// diret�rio do kernel.

	// #importante
	// Isso clona o diret�rio de p�ginas do kernel. Isso facilita as coisas.
	// Retorna o endere�o virtual do novo diret�rio de p�ginas.

	// #importante:
	// Vamos converter porque precisamos de endere�o f�sico para colocarmos no cr3.
	// Mas o taskswitch faz isso pegando o endere�o que estiver na thread, ent�o
	// esse endere�o precisa ir pra thread.

    Process2->DirectoryVA = (unsigned long) CreatePageDirectory ();

    if ( (void *) Process2->DirectoryVA == NULL ){
        panic ("processCopyProcess: DirectoryVA fail");
    }

    Process2->DirectoryPA = (unsigned long) virtual_to_physical ( Process2->DirectoryVA, 
                                                gKernelPageDirectoryAddress ); 




	// ??
	// #bugbug
	// Se o endere�o for virtual, ok fazer isso. 
	// Usaremos o mesmo endere�o virtual da imagem.
	// #importante: se bem que esse endere�o virtual de imagem
	// pode ser diferente para o kernel. Pois no momento
	// que ele alocar mem�ria para a imagem ele ter� o
	// endere�o l�gico retornado pelo alocador.

    // #bugbug
    // Conseguimos o endere�o da imagem copiada,
    // mas teremos que refazer isso mais a frente quando
    // carregarmos, (isso no caso da rotina de clonagem)
    // Isso � v�lido s� para o fork.

	//Process2->Image = Process1->Image;
    Process2->Image = Process1->childImage;
    Process2->ImagePA = Process1->childImage_PA;
    Process2->childImage = 0;
    Process2->childImage_PA = 0;

    //heap
    Process2->Heap = Process1->Heap;    
    Process2->HeapEnd = Process1->HeapEnd; 
    Process2->HeapSize = Process1->HeapSize;

	//stack
    Process2->Stack = Process1->Stack;   
    Process2->StackEnd = Process1->StackEnd; 
    Process2->StackSize = Process1->StackSize;
    Process2->StackOffset = Process1->StackOffset;


    Process2->iopl = Process1->iopl;

    Process2->base_priority = Process1->base_priority;
    Process2->priority = Process1->priority;

    
    //Herdar os primeiros arquivos.
    //podemos herdar todos os abertos.

    int i;
    for (i=0;i<3;i++)
    {
        Process2->Objects[i] = Process1->Objects[i];
    }



	//
	// ========================
	// Thread de controle
	//


	// Vamos clonar a thread de controle do processo pai.
	
	// obs:
	// Me parece que a fun��o que clona apenas a thread de controle 
	// chama-se fork1. #todo
	
	// #todo: Precisamos copiar todas as threads
	// vamos come�ar pela thread de controle.
	// teoriacamente elas precisam ter o mesmo endere�o virtual ...
	// mas est�o em endere�os f�sicos diferentes.
	// #bugbug precisamos clonar a thread.
	
	// ############### #IMPORTANTE #################
	// #bugbug
	// Ainda n�o temos um salvamento de contexto apropriado para essa system call.
	// S� o timer tem esse tipo de salvamento.
	// Precisamos salvar o contexto antes de chamarmos o servi�o fork()
	// Pois se n�o iremos retomar a thread clone em um ponto antes de 
	// chamarmos o fork, que � onde est� o �ltimo ponto de salvamento.
	
	// Clonando a thread de controle.


    Process2->control = (struct thread_d *) threadCopyThread ( Process1->control );

	//#todo Checar
	//if ( (void *) Process2->control == NULL )
	//{
	//}
	


    //
    // Directory.
    //

	// #importante
	// Um diret�rio de p�ginas para a thread de controle.
	// O diret�rio de p�ginas da thread de controle ser� o mesmo
	// do processo.
	// � importante deixarmos esse endere�o na estrutura da thread, pois
	// � a� que o taskswitch espera encontra-lo.


    Process2->control->DirectoryPA = Process2->DirectoryPA;



    Process2->control->ownerPID = Process2->pid;


	//?? herda a lista de threads ??
    Process2->threadListHead = Process1->threadListHead;

    Process2->zombieChildListHead = Process1->zombieChildListHead;

    Process2->dialog_address = Process1->dialog_address;

    //#bugbug
    //deleta isso.
	//message support.
    //Process2->window = Process1->window;    //arg1. 
    //Process2->msg    = Process1->msg;       //arg2.
    //Process2->long1  = Process1->long1;     //arg3.
    //Process2->long2  = Process1->long2;     //arg4.



    //++
    // tty support
    Process2->tty = ( struct tty_d *) tty_create ();  
       
    if ( (void *) Process2->tty == NULL ){
         panic ("processCopyProcess: Couldn't create tty\n");
    }
    tty_start (Process2->tty);
    //--


    Process2->exit_code = Process1->exit_code;

    Process2->prev = Process1->prev; 
    Process2->next = Process1->next; 

    Status = 0;
    goto done;



fail:
    Status = 1;
    printf ("processCopyProcess: fail\n");


done:
    return (int) Status;
}


/*
 ******************************************************
 * create_process:
 *     Cria process para ring 3.
 *     Preenche o PCB (Process Control Block).
 *
 * @todo: 
 *     Um processo na verdade inclui tudo sobre a tarefa,
 *     Desde o nome do arquivo at� o carregamento, registro e execu��o.
 *
 * Obs:
 *     Um job � um conjunto de processos.
 *     Tudo depende do tipo (funcionalidade): 
 *     SYSTEM, PERIODIC, ROUND_ROBIN, IDLE.
 *
 *    Classes de processos (tipo de n�vel):
 *
 *    PB - Processos inclu�dos no Kernel Base.
 *    P0 - Processos em ring0.
 *    P1 - Processos em ring1.
 *    P2 - Processos em ring2.
 *    P3 - Processos em ring3, User Mode.
 *
 * @todo: 
 * Esse fun��o deve chamar uma rotina de cria��o de diret�rio. 
 * Quando um processo � criado deve-se criar o seu diret�rio e criar 
 * as pagetables que o processo vai usar de acordo com o tamanho do 
 * processo. 
 *
 * @todo: processCreateProcess(...)
 *
 * @todo: 
 * Aumetar o n�mero de argumentos para dar o suporte necess�rio para 
 * criar um processo do jeito que for necess�rio
 */

// Cria uma estrutura de processo.
// Cria uma tty pra esse processo.

struct process_d *create_process ( struct room_d *room,
                                   struct desktop_d *desktop,
                                   struct window_d *window,
                                   unsigned long base_address, 
                                   unsigned long priority, 
                                   int ppid, 
                                   char *name, 
                                   unsigned long iopl,
                                   unsigned long directory_address )
{

    pid_t PID;
    struct process_d *Process;
    

    // Para a entrada vazia no array de processos.
    struct process_d *Empty; 
    
    int i=0;

	// @todo:
	// Melhorar esse esquema de numera��o e 
	// contagem de processos criados.
	// processNewPID � global ?

    if ( processNewPID < USER_BASE_PID || processNewPID >= PROCESS_COUNT_MAX )
    {
        processNewPID = (int) USER_BASE_PID;
    }



    //
    // Process.
    //

    Process = (void *) kmalloc ( sizeof(struct process_d) );

	// #todo: 
	// Aqui pode retornar NULL.
    if ( (void *) Process == NULL ){
        panic ("process-create_process: Process");
    }



	// Loop.
	// #BugBug: 
	// Isso pode virar um loop infinito.

get_next:


/*
	i++;
	
	if ( i >= PROCESS_COUNT_MAX )
	{
		// #bugbug: 
		// Isso deixa o sistema devagar caso n�o apare�a 
		// a mensagem.
		
		printf("pc-process-create_process: End of list");
        refresh_screen();
		
		return NULL;
		//while(1){}
	};
	
*/



	// Get empty.
	// Obt�m um �ndice para um slot vazio na lista de processos.

    PID = (int) processNewPID;  // deletar!!
    
    PID = (int) getNewPID ();

    // #todo: Limits. Max ?
    
    if ( PID == -1 || PID == 0 ){
        printf ("create_process: getNewPID fail %d \n", PID);
        refresh_screen ();
        return NULL;
    }



	//Se o slot estiver ocupado tentaremos o pr�ximo.
	//Na verdade podemos usar aquela fun��o que procura por um vazio. 


    Empty = (void *) processList[PID];

    if ( (void *) Empty != NULL )
    {
        goto get_next;

    }else{

		//Object.
        Process->objectType = ObjectTypeProcess;
        Process->objectClass = ObjectClassKernelObjects;

        //Validation.
        Process->used = 1;
        Process->magic = 1234;


        processNewPID = (int) PID;
        
		// Identificadores.
		// PID. PPID. UID. GID.

        Process->pid  = (int) PID; 
        Process->ppid = (int) ppid; 
        Process->uid  = (int) GetCurrentUserId (); 
        Process->gid  = (int) GetCurrentGroupId (); 


		//State of process

        Process->state = INITIALIZED;  

		//@TODO: ISSO DEVERIA VIR POR ARGUMENTO
        Process->plane = FOREGROUND;

		//Error.
		//Process->error = 0;

		//Name.
		Process->name = (char *) name; //@todo: usar esse.
		//Process->cmd = NULL;  //nome curto que serve de comando.
        //Process->pathname = NULL;
        
        
        //#test
        //64 bytes m�x.
        strcpy ( Process->__processname, (const char *) name); 
        

		// Lista de streams ...
		// Zerando essa lista e criando 3 streams para o processo.
		// Mas vamos improvisar e usar os ponteiros do kernel.
		
        for ( i=0; i< NUMBER_OF_FILES; i++ )
        {
            Process->Streams[i] = 0;
        }

        Process->Streams[0] = (unsigned long) stdin;
        Process->Streams[1] = (unsigned long) stdout;
        Process->Streams[2] = (unsigned long) stderr;

        // #todo
        // Podemos colocar 3 arquivos em Objects[]
        // Ou seriam tty ? 

        for ( i=0; i<64; i++ )
        {
            Process->Objects[i] = 0;
        }


		//Process->terminal =

		//
		//    ****  Banco de dados ****
		//

		//bancos de dados e contas do processo.
		//Process->kdb =
		//Process->gdbListHead =
		//Process->ldbListHead =
		//Process->aspaceSharedListHead =
		//Process->aspacePersonalListHead =
		//Process->dspaceSharedListHead =
		//Process->dspacePersonalListHead =
		
		// Inicializando a lista de framepools do processo.
		// @todo: Todo processo deve ser criado com pelo menos um 
		// frame pool, o que � equivalente a 4MB. (uma parti��o)
		// Obs: Um framepool indica onde � a �rea de mem�ria fisica
		// que ser� usada para mapeamento das p�ginas usadas pelo processo.

        Process->framepoolListHead = NULL;

		//Thread inicial.
		//Process->thread =
		
		//Process->processImageMemory =
		//Process->processHeapMemory =
		//Process->processStackMemory =
		
	    // ORDEM: 
		// O que segue � referenciado durante o processo de task switch.

		// Page Directory: 
		//     Alocar um endere�o f�sico para o diret�rio de p�ginas do 
		// processo a ser criado, depois chamar a fun��o que cria o diret�rio.
		//
		// @todo:
		// *IMPORTANTE: Por enquanto os processos s�o criadas usando o 
		// diret�rio de p�ginas do processo Kernel. Mas temos que criar 
		// um diret�rio novo pra cada processo criado.
		// O diret�rio de todos os processos de usu�rio ser�o iguais. 
		// Ter�o uma �rea de us�rio particular e uma �rea compartilhada 
		// em kernel mode.
		//
		//@todo: Alocar um endere�o f�sico antes, depois chamar a fun��o que 
		// cria o pagedirectory.
		//@todo: 
        //op��o: KERNEL_PAGEDIRECTORY; //@todo: Usar um pra cada processo.

		// #obs:
		// Vari�vel recebida via argumento.

        if (directory_address == 0)
        {
            printf ("create_process: page directory address fail\n");
            return NULL;
        }

        Process->DirectoryVA = (unsigned long ) directory_address;
        Process->DirectoryPA = (unsigned long) virtual_to_physical ( directory_address, 
                                                   gKernelPageDirectoryAddress );

		// cancelados. 
		// Process->mmBlocks[32]
		// Process->mmblockList[32]
		
		
		// Process->processMemoryInfo
		
		
		// #todo: 
		// Precisa alocar espa�o na mem�ria f�sica.
		// Precisa criar page tables para essas areas de cada processo.
		// Os endere�os virtuais dessas areas dos processos s�o sempre os mesmos.
		// mas os endere�os f�sicos dessas areas variam de processo pra processo.

		// Imagem do processo.
		// ?? Provavelmente esse endere�o � virtual.
		// Queremos que esse endere�o seja padronizado e que todos 
		// os processos usem o mesmo endere�o.
		
		// #bugbug
		// Todos os processos de usu�rio come�am no mesmo endere�o virtual.
		// Por�m temos os processos em kernel mode e os processos do gramado core
		// que usam endere�os virtuais diferentes.
		// #todo: Rever isso.
		// #todo: estamos suspendendo essa informa��o.
		
		//
		// # IMPORTANTE 
		//
		
		// Base da imagem do processo.
		// Na verdade precisamos aceitar o endere�o passado via 
		// argumento, pois nem todos processos come�am no endere�o 
		// default.

        // Endere�o virtual e endere�o f�sico.
        Process->Image = base_address;  
        Process->ImagePA = (unsigned long) virtual_to_physical ( Process->Image, 
                                               gKernelPageDirectoryAddress ); 
                                               
        
        // Endere�o virtual e endere�o f�sico de um processo filho.
        // Isso � usado durante a clonagem.
        Process->childImage = 0;
        Process->childImage_PA = 0;


        // #todo
        // Precisamos saber o tamanho da imagem do processo para
        // calcularmos quantas p�ginas ele vai usar.
        // Precisamos dividir a imagem em code, data, heap e stack
        // Pois a �rea de dados poder� sofrer swap.

		// Tamanho da imagem do processo.
		// Temos que chamar a fun��o que pega o tamanho de um arquivo,
		// #bugbug: Porem, no momento o kernel n�o consegue ler arquivos
		// que est�o em subdiret�rios corretamente e os programas est�o 
		// em subdiret�rios.
		// #obs: O tamanho tamb�m poderia ser passado por arguemento.
		// #ou um argumento com ponteiro pra estrutura de informa��o 
		// sobre uma imagem.
        Process->ImageSize = 0;

		//#todo: estrutura com informa��es sobre a imagem do processo.
        Process->image_info = NULL;

		// Heap e Stack:
		//
		// @todo: #BugBug 
		// O Heap e a Stack devem estar dentro da �rea de mem�ria do processo.
		// Uma pagetable do diret�rio � para o heap e outra para a stack.
        // Cada pagetable no diret�rio do processo � pra uma coisa.
        //
		// Obs: O endere�o virtual do heap e da stack dos processos ser�o 
		// os mesmos para todos os processos, assim como o endere�o virtual 
		// de carregamento da imagem.
		
		// Heap and Stack. 
		// #importante: (Endere�os virtuais).
		// Por isso pode ser o mesmo para todos os processos.
		
		
		// #### HEAP ####
		
		// directory va, index, region pa
		//CreatePageTable ( Process->DirectoryVA, 512, 0 );
		
		//Process->Heap = (unsigned long) 0x00400000; //funciona
		//Process->Heap = (unsigned long) 0xC0C00000; //funciona
		
		// g_heappool_va
		// endere�o virtual do pool de heaps.
		// os heaps nessa �rea ser�o dados para os processos.
		// base + (n*size)

        if ( g_heap_count < 0 || g_heap_count >= g_heap_count_max )
        {
            panic ("create_process: g_heap_count limits");
        }


        Process->Heap = (unsigned long) g_heappool_va + (g_heap_count * g_heap_size);
        Process->HeapSize = (unsigned long) g_heap_size;
        Process->HeapEnd = (unsigned long) (Process->Heap + Process->HeapSize); 

        g_heap_count++;

		//Process->Heap = (unsigned long) allocPages (64); 
		//Process->Heap = (unsigned long) kmalloc (1024*32); //32kb

		// Endere�o do in�cio do Heap do processo.
		// #bubug: Endere�o do fim do heap.
		// Tamanho do heap, dado em KB.
	    //Process->Heap = UPROCESS_DEFAULT_HEAP_BASE;    
	    //Process->HeapEnd = 0; // @todo: (UPROCESS_DEFAULT_HEAP_BASE + UPROCESS_DEFAULT_HEAP_SIZE);
		//Process->HeapSize = (UPROCESS_DEFAULT_HEAP_SIZE/1024);    

		//Process->HeapPointer
		//Process->HeapLastValid
		//Process->HeapLastSize
	    
		// Endere�o do in�cio da Stack do processo.
		// Endere�o do fim da stack do processo.
		// Tamanho da pilha, dada em KB.
		// #importante: Deslocamento do endere�o do in�cio da pilha em rela��o 
		// ao in�cio do processo. 

        Process->Stack = UPROCESS_DEFAULT_STACK_BASE;   
        Process->StackEnd = 0; // @todo: (UPROCESS_DEFAULT_STACK_BASE+UPROCESS_DEFAULT_STACK_SIZE);
        Process->StackSize = (UPROCESS_DEFAULT_STACK_SIZE/1024);   	
        Process->StackOffset = UPROCESS_DEFAULT_STACK_OFFSET; 


		//ring.
        Process->iopl = iopl; 

		//PPL - (Process Permition Level).(gdef.h)
		//Determina as camadas de software que um processo ter� acesso irrestrito.
		//Process->ppl = pplK0;


        //Process->callerq          //head of list of procs wishing to send.
        //Process->sendlink;        //link to next proc wishing to send.
        //Process->message_bufffer  //pointer to message buffer.
        //Process->getfrom_pid      //from whom does process want to receive.
        //Process->sendto_pid       //pra quem.

        //Signal
        //Process->signal = 0;
        //Process->signalMask = 0;

		//cancelada.
		//Process->process_message_queue[8]

		//Outras coisas.
		
		//Prioridade.
        Process->base_priority = (unsigned long) priority;
        Process->priority = (unsigned long)  Process->base_priority;

        //Que tipo de scheduler o processo utiliza. (rr, realtime ...).
        //Process->scheduler_type = ; 

		//Process->step
		//Process->quantum
		//Process->timeout
		//Process->ticks_remaining

		//As threads do processo iniciam com esse quantum.
		//Process->ThreadQuantum   


		//Process->threadCount = 0;    //N�mero de threads do processo.
		
		//Process->tList[32] 
		
		//Lista de threads.
        Process->threadListHead = NULL;

        Process->control = NULL;

        //Process->window_id 


		//Process->event
		

		// #importante
		// user session, room and desktop.

		// #bugbug: 
		// N�o temos informa��o sobre a user session, 
		// devemos pegar a estrutura de current user session. 
		// Para isso ela deve ser configurada na inicializa��o do gws,
		// antes da cria��o dos processo.
		
        Process->usession = CurrentUserSession;  // Current.
        Process->room = room;                    // Passado via argumento.
        Process->desktop = desktop;              // Passado via argumento.

		//Process->base_priority

	
        // wait4pid: 
        // O processo esta esperando um processo filho fechar.
        // Esse � o PID do processo que ele est� esperando fechar.

        Process->wait4pid = (pid_t) 0;
        
        // N�mero de processos filhos.
        Process->nchildren = 0;

        Process->zombieChildListHead = NULL;

        Process->exit_code = 0;

		// ?? 
		// Procedimento eem ring 0 por enquanto.
        Process->dialog_address = (unsigned long) &system_procedure;

        Process->signal = 0;
        Process->signal_mask = 0;

		//
		// Msg
		//

    //#bugbug
    //deleta isso.
		//Msg support.
		//Argumentos do procedimento de janela.
		//@todo: Isso pode ser um ponteiro de estrutura,
		//a fila de mensgens pode ser uma fila de ponteiros.
        //Process->window = NULL;    //arg1. 
        //Process->msg = 0;          //arg2.
        //Process->long1 = 0;        //arg3.
        //Process->long2 = 0;        //arg4.




        // tty support

        Process->tty = ( struct tty_d *) tty_create ();         
        if ( (void *) Process->tty == NULL ){
			
			//printf ("pid %d \n",Process->pid );
            panic ("create_process: Couldn't create tty\n");
        }
        tty_start (Process->tty);



        Process->prev = NULL; 
        Process->next = NULL; 

		// List.
		// Coloca o processo criado na lista de processos.

        processList[PID] = (unsigned long) Process;
    };


	// More ?

    return (void *) Process;
}

 
/*
 ************************************************
 * CloseAllProcesses:
 *     Bloqueia todos os processos da lista de processos.
 *     Menos o processo '0'.
 *     processCloseAllProcesses();    
 */

void CloseAllProcesses (void){

    int Index = 0;
    struct process_d *P;

	// #importante:
	// Menos o 0, pois � o kernel. 

    for ( Index = 1; Index < PROCESS_COUNT_MAX; Index++ )
    {
		//Pega, bloqueia e tira da lista.
        P = (void *) processList[Index];
        P->state = PROCESS_BLOCKED;

        processList[Index] = (unsigned long) 0;
    };


	//Check process 0.
    P = (void *) processList[0];

    if ( (void *) P == NULL ){
        panic ("CloseAllProcesses: P\n");
    }
}


void show_currentprocess_info (void){

    struct process_d *Current;


    if ( current_process < 0 || current_process >= PROCESS_COUNT_MAX )
    {
        //printf("show_process_information: current_process fail\n");
        return;
    }


	//Struct.
    Current = (void *) processList[current_process];

    if ( (void *) Current == NULL )
    {
        printf ("show_currentprocess_info: struct \n");
        return; 

    } else {

		//Index.
        printf ("PID={%d} PPID={%d} UID={%d} GID={%d} \n",
            Current->pid, Current->ppid, Current->uid, Current->gid );
		//Name
        //printf ("Name={%s} \n", Current->name_address );
        printf ("Name={%s} \n", Current->name );
        
		//Image Address.
        printf ("ImageAddress={%x} \n", Current->Image );

		//Directory Address. *IMPORTANTE.
        printf (">>DirectoryPA={%x} \n", Current->DirectoryPA );
        printf (">>DirectoryVA={%x} \n", Current->DirectoryVA );

		//Heap and stack.
        printf("Heap={%x}  HeapSize={%d KB}  \n", Current->Heap, 
            Current->HeapSize );

        printf("Stack={%x} StackSize={%d KB} \n", Current->Stack, 
            Current->StackSize );

		//...
    };


    refresh_screen ();
}


/*
 * show_process_information:
 *     Mostra informa��es sobre os processos. 
 *     #todo: na verdade um aplicativo em user mode deve fazer esse trabalho
 * solicitando informa��es sobre cada processo atrav�s de chamadas.
 */

// Mostrar informa��es sobre os processos da lista.
// obs: as imagens s�o carregadas em endere�os virtuais diferentes
// e o endere�o mostrado � em rela��o ao diret�rio de p�ginas do kernel
// pois o kernel � que controla o posicionamento das imagens.

void show_process_information (void){

    int i=0;
    struct process_d *p;


    printf ("show_process_information: \n");


    for ( i=0; i<PROCESS_COUNT_MAX; i++ )
    {
        p = (void *) processList[i];

        if ( (void *) p != NULL && 
                      p->used == 1 && 
                      p->magic == 1234 )
        { 
            // #bugbug
            // #todo Change that thing.
            
			printf ("PID=%d PPID=%d State=%d Base=%x Size=%d DirPA=%x DirVA=%x iopl=%d prio=%d wait4pid=%d Name={%s}\n\n", 
				p->pid, 
			    p->ppid,
				p->state,
				p->Image,
				p->ImageSize,
				p->DirectoryPA,
				p->DirectoryVA,
				p->iopl,
				p->priority,
				p->wait4pid,
				p->name ); //p->name_address );
        }

		//Nothing.
    };


    //printf ("done\n");
    refresh_screen ();
}


/*
 **************************************************************
 * SetProcessDirectory:
 *     Configura o endere�o do diret�rio de p�ginas do processo.
 *     @todo: Isso pode ser um servi�o oferecido pelo kernel,
 * para um gerenciador de processos em user mode usar.
 * @todo: processSetDirectory(...)
 */

void 
SetProcessDirectory ( struct process_d *process, unsigned long Address )
{
    if ( (void *) process != NULL )
    {
        process->DirectoryPA = (unsigned long) Address;  
    }
}


/*
 ************************************************************
 * GetProcessDirectory:
 *     Pega o endere�o do diret�rio de p�ginas do processo.
 *     @todo: Isso pode ser um servi�o oferecido pelo kernel,
 * para um gerenciador de processos em user mode usar.
 * @todo: processGetDirectory(...)
 */

unsigned long GetProcessDirectory ( struct process_d *process ){

    if( (void *) process != NULL )
    {
		//@todo: checar used e magic.
        return (unsigned long) process->DirectoryPA;
    }


    return (unsigned long) 0;
}


/*
 ****************************************************
 * GetPageDirValue:
 *     Pega o endere�o do diret�rio de p�ginas do processo.
 *     processGetPageDirValue()
 */

unsigned long GetPageDirValue (void)
{
    return (unsigned long ) get_page_dir ();
}



/*
 ***************************************************************
 * init_task:
 *     Inicia um processo.
 *     @todo: Mudar o nome para init_process().
 *            Inicializa a estrutura de um processo criado.
 *            Quando cria o processo, cria apenas sua estrutura,
 *            ficando a cargo dessa rotina inicializar a estrutura criada. 
 */ 

int init_task (int id)
{ 
	//@todo: inicializar uma estrutura para um processo criado.
	//obs: ja tem essa rotina feita em aalgum lugar..
	
    return 0;   
}



/*
 *************************************************************
 * init_tasks: 
 *     Inicia as vari�veis.
 *
 * @todo: 
 *     Bug bug, problemas na inicializa��o das estruturas.
 *     Implementar a aloca��o din�mica na cria��o das tarefas.
 */

//#bugbug
//rever e deletar, se poss'ivel.

//#bugbug
//os conceitos de tasks ainda est�o misturados, hora � thread e hora � processo

void init_tasks (void)
{
    init_processes ();
}


/*
 ***********************************************************
 * init_processes:
 *    Inicaliza o process manager.
 *   #todo: rever esse nome, pois na verdade estamos inicializando variaveis 
 * usadas no gerenciamento de processo.
 */
 
void init_processes (void){
	
    int i;
	
	//
	// Iniciando vari�veis globais.
	//
	
	kernel_request = 0;    // O que fazer com a tarefa atual.
	
	
	// ?? Contagem de tempo de execu��o da tarefa atual.
	//n�o precisa, isso � atualizado pelo request()
	//kernel_tick = 0;                                 

    kernel_switch = 0;     // ?? Ativa o kernel switch do scheduler.

    current_process = 0;


	//
	// Inicializando a lista de processos.
	//

    i=0;

    while (i < PROCESS_COUNT_MAX)
    {
        processList[i] = (unsigned long) 0;
        i++;
    };

    // More ?
}



/*
 ***************************************************
 * exit_process:
 *
 *     Exit process.
 *     Coloca o processo no estado PROCESS_TERMINATED, mas n�o destr�i a 
 * estrutura do processo. Outra rotina destruir� as informa��es.
 *
 * @todo: 
 * Liberar a mem�ria e os recursos usado pelo processo. Ou ainda apenas 
 * sinalizar a flag magic para que o GC a reutilize.
 *
 * @todo: 
 * Fechar as threads seguindo a lista encadeada.
 */

void exit_process ( pid_t pid, int code ){

    int i;

    struct process_d *Process;
    
    //tmp
    struct thread_d *__Thread;    
    
    //lista.
    struct thread_d *Threads;
    
    
    struct thread_d *Next;
	//...


    //debug_print ("exit_process:\n");

//#ifdef MK_VERBOSE
	// Debug:
    printf ("exit_process: Terminating process %d\n", pid );
    refresh_screen ();
//#endif


	//Limits. 

    if ( pid < 0 || pid >= PROCESS_COUNT_MAX )
    {
        return;
    }


	// Pega o ponteiro para a estrutura, 
	// muda o c�digo de sa�da e o status.


    Process = (struct process_d *) processList[pid];

    // Oh Dude, don't do that!
    if ( Process == KernelProcess )
    {
        return;
    }

    if ( (void *) Process == NULL )
    {
        return;
    }else{

        if ( Process->used != 1 || Process->magic != PROCESS_MAGIC )
        {
            return;
        }


        Process->exit_code = (int) code; 
        Process->state = PROCESS_TERMINATED; 
		//...
    };


    // #todo
    // Tem que liberar todos os recursos usados pelo processo.
    // >>Streams[]
    // memory ??
    // ...
 

//#ifdef MK_VERBOSE
	//Debug:
	printf ("exit_process: Terminating threads..\n");
	refresh_screen ();
//#endif


    // #obs
    // Acho que ainda n�o estamos usando a lista.
    // Mas podemos ao fim disso deletarmos a thread de controle
    // caso ela ainda exista.

	// Agora temos que terminar as threads que est�o na lista 
	// de threads do processo.
	// Pegaremos a primeira da lista.
	// Se o head da list n�o foi inicializado corretamente 
	// d� page fault.

    __Thread = (void *) Process->threadListHead;

    // Se n�o h� nada na head.
    if ( __Thread == NULL )
    {
		// #todo: 
		// Talvez haja mais o que fazer.
		// Podemos checar se existe uma thread de controle.
  
        goto done;

    }else{
		
		// Ok, se o primeiro da lista � v�lido, podemos 
		// tentar fechar todas.
        // ... 
    };


	// Se a primeira thread da lista � v�lida, 
	// ent�o tentaremos fechar toda a lista.

    while (1)
    {
		// ?? Qual deve fechar depois. ??
		
		printf (".\n");
		refresh_screen ();
		
		// Salva o ponteiro para o pr�ximo thread.
		Next = (void *) __Thread->Next;
		
		// Confere se chegamos ao fim da lista.
		// 'Thread' fecha agora.
		
        if( __Thread == NULL )
		{
		    goto done;
		
		}else{
    
#ifdef MK_VERBOSE    
		    //fecha a thread.
		    printf ("exit_process: Killing thread %d\n", __Thread->tid );
#endif			
			
			// Kill !
			
			kill_thread ( __Thread->tid ); 

			// Prepara qual deve fechar agora.
		    // Hav�amos salvo e agora � vez dela.
		    // Obs: Estamos reusando o ponteiro.
			
			__Thread = (void *) Next;
		 };
        //Nothing.
    };

	//nothing

done:
	
	//@todo:
	//    Escalonar o processo atual. Se o processo fechado foi o processo 
	// atual, precisamos de um novo processo atual. Usaremos o processo zero 
	// por enquanto. Obs: Devemos fazer aqui apenas escalonameto de processo
	// e n�o de thread.	

	//Zerando por enquanto.

	//?? Analizar essa parte.
	//@todo: Select next process (idle)




	
	// #todo: 
	// chamar o scheduler.
	
	//scheduler ();
	
	//#test
	current_process = KernelProcess->pid; 
	current_thread = ____IDLE->tid;
    KernelProcess->state = READY;
    ____IDLE->state = READY;

    return;
}


// ??
int get_caller_process_id (void)
{
    return (int) caller_process_id;
}


// ??
void set_caller_process_id (int pid)
{
    caller_process_id = (int) pid;
}


/*
 *************************************************
 * init_process_manager:
 *     Initialize process manager.
 *     processInitializeProcessManager();
 */

int init_process_manager (void){

    caller_process_id = (int) 0;

    processNewPID = (int) USER_BASE_PID;

	//...

    return 0;
}


/*
 Constructor.
int processmanagerProcessmanager ()
{
}
*/


/*
int processmanagerInit ()
{
};
*/


/*
 * GetProcessHeapStart:
 *     Pega o endere�o do heap do processo.
 */

unsigned long GetProcessHeapStart ( pid_t pid ){

    struct process_d *process;

	//Limits.

    if ( pid < 0 || pid >= PROCESS_COUNT_MAX )
    {
        goto fail; 
    }


    process = (struct process_d *) processList[pid];
    
    if ( (void *) process == NULL )
    {
		goto fail;
		 
    }else{
		
		if ( process->used != 1 || process->magic != 1234 )
		{
			goto fail;
		}
		
		//Ok.
		return (unsigned long) process->Heap;
    };


fail:
	
    return (unsigned long) 0;
}


/*
 * GetProcessPageDirectoryAddress:
 *
 */

unsigned long GetProcessPageDirectoryAddress ( pid_t pid ){

    struct process_d *process;

	//Limits.

    if( pid < 0 || pid >= PROCESS_COUNT_MAX )
    {
        goto fail; 
    }


    process = (struct process_d *) processList[pid];

    if ( (void *) process == NULL )
    {
		goto fail;
		 
    }else{
		
		if ( process->used != 1 || process->magic != 1234 ){
			goto fail;
		}

        return (unsigned long) process->DirectoryPA;
    };


fail:

    return (unsigned long) 0;
}


// Isso vai alterar uma das entradas do diret�rio de p�ginas do processo.
// Criando uma pagetable e mapeando nela 4MB da mem�ria f�sica.
// Usaremos isso para carregar a imagem do programa em 0x400000.
// #bugbug: Na verdade precisamos saber quantas p�ginas o programa precisa.
// e Carregarmos s� as p�ginas que ele precisa, ou menos caso a pagina��o
// por demanda esteja funcionando..

/*
int xxxloadHere4MB ( struct process_d *process, unsigned long region4MB_PA );
int xxxloadHere4MB ( struct process_d *process, unsigned long region4MB_PA )
{
    
    //#todo melhorar esses filtros.
    
    if ( (void *) process == NULL )
        return -1;
    
    if ( region4MB_PA == 0 )
        return -1;
    
    
    // Cria uma pagetable em um dado diret�rio de p�ginas.
    // Uma regi�o de 4MB da mem�ria f�sica � mapeanda nessa pt.
    
    CreatePageTable ( process->DirectoryVA, ENTRY_USERMODE_PAGES, region4MB_PA );
    
    return 0;
}
*/




//#todo
//encontrar um slot vazio em Process->Streams[]
//retornar o indice.
/*
int process_find_empty_stream_slot ( struct process_d *process );
int process_find_empty_stream_slot ( struct process_d *process ){

 	int i = 0;
	
	if ( (void *) process == NULL )
	{
		return -1;
		 
	}else{
		
		if ( process->used != 1 || process->magic != 1234 ){
			
			return -1;
		}
		
	    for ( i=0; i< NUMBER_OF_FILES; i++ )
	    {
	        if ( Process->Streams[i] == 0 )
		    {
		        return i;
		    }
	    }
	};  
	
	return -1;
} 
*/


    
//=============

// Pega uma stream na lista de arquivos dado o fd.

FILE *get_stream_from_fd ( int pid, int fd )
{
    FILE *fp;
    struct process_d *p;


    if ( pid < 0)
        return NULL;


    // Get process pointer.
    p = (struct process_d *) processList[ pid ];



    if (fd<0)
        return NULL;

    // Get fp from list of open files.
    return ( FILE * ) p->Streams[fd];  
}

//===============
  
  
  
// cria um novo process, uma thread e carrega a imagem.
int 
__execute_new_process ( const char *filename, 
                        char *argv[], 
                        char *envp[] )
{
    struct process_d *p;
    struct thread_d *t;
    
    
    int __pid = (int) getNewPID ();
    
    if (__pid < 0)
    {
		panic ("__execute_new_process: pid");
        return -1;
    }
    
    p = (struct process_d *) create_process ( NULL, NULL, NULL, 
                                   (unsigned long) 0x00400000, 
                                   PRIORITY_HIGH, 
                                   __pid,
                                   (char *) filename, 
                                   RING3, 
                                   (unsigned long ) CreatePageDirectory() ); 

    if ( (void *) p == NULL )
    {
		panic ("__execute_new_process: p");
        return -1;
    }
    
    t = (struct thread_d *) sys_create_thread ( 
                                NULL,             // room ? 
                                NULL,             // desktop
                                NULL,             // w.
                                (unsigned long) 0x00401000,                 // init eip
                                (unsigned long) (0x00401000 + (1024*200)),   // init stack
                                __pid,  // pid
                                "new-thread" );    // name

    if ( (void *) t == NULL )
    {
		panic ("__execute_new_process: t");
        return -1;
    }
    
    t->DirectoryPA = p->DirectoryPA;
    
    //load image.

	// loading image.
	
    int fileret = -1;
    
    fileret = (unsigned long) fsLoadFile ( VOLUME1_FAT_ADDRESS, 
                                  VOLUME1_ROOTDIR_ADDRESS, 
                                  (unsigned char *) filename, 
                                  (unsigned long) 0x00400000 );


    if ( fileret != 0 )
    {
        panic ("__execute_new_process: fileret \n");
    }
    
    //t->tss = current_tss;
    SelectForExecution (t);    // >>standby
    
    
    
    printf ("__execute_new_process: done\n");

    //debug
    show_slot (t->tid);
    show_reg  (t->tid);

    printf ("__execute_new_process: *breakpoint { Work in progress } \n");    
    refresh_screen();
    while(1){}
    
    return 0;
    //return __pid;
    //...
}
      
 

// Pega o n�mero da tty de um processo, dado o pid.
int process_get_tty ( int pid )
{
    // Usada para debug.

    
    struct process_d *p;
    struct tty_d *tty;


    //3debug
	//printf ("process_get_tty: pid %d \n", pid);
	//refresh_screen();


    // #todo
    // Overflow ?
    
    if ( pid < 0 )
    {
		//printf ("pid fail\n");
		//refresh_screen();
        return -1;
    }

    p = (struct process_d *) processList[pid];

    if ( (void *) p == NULL )
    {
		//printf ("p fail\n");
		//refresh_screen();
        return -1;
    }

    tty = p->tty;    


    if ( (void *) tty == NULL )
    {
		//printf ("tty fail\n");
		//refresh_screen();
        return -1;
    }


    //printf ("tty %d belongs to %d\n", tty->index, p->pid );
    //refresh_screen ();

    return (int) tty->index;
}                   
                      
 
                      
                      
    

//
// End.
//

