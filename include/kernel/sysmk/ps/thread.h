/*
 * File: mk/pc/thread.h
 *
 * Descri��o:
 *     Header para threads.
 *     Pertence ao m�dulo microkernel, dentro do kernel.
 *
 * History:
 *     2015 - Created by Fred Nora.
 *     2018 - Revision.
 */
 


//O primeiro �ndice na contagem das threads do sistema e o
//primeiro �ndice na contagem das threads dos usu�rios.
#define SYSTEM_BASE_TID 0  
#define USER_BASE_TID 100  

//
// Preempt support.
//

//?? Se pode ou n�o entrar em preemp��o. 
#define  PREEMPTABLE 1    //PODE ENTRAR
#define  UNPREEMPTABLE 0  //NAO PODE ENTRAR EM PREEMP��O

  
// Identificadores de thread.
#define IDLE_ID  0 
#define IDLE  IDLE_ID 
#define THREAD_IDLE  IDLE_ID  
//... 
 
 
#define THREAD_MAGIC 1234
 
 
 
/*
 * Globais.
 */ 
 
 
 
/*
 * Estruturas para threads.
 */
 
 
 	//#importante
	//raz�es para esperar
	//#todo: isso precisa ser inicializado.
	//@todo: tem que fazer um enum para enumerar as raz�es.
	//o �ndice � o selecionador da raz�o pela 
	//qual a thread est� esperando.
	//existem umas 20 raz�es pra esperar.
	// 0 - esperando por mensagem.(presa num loop)
	// 1 - esperando outra thread finalizar. wait4tid
	// 2 - esperando um processo finalizar. wait4pid
	// 3 - esperando um objeto. (BLOCKED)(espera gen�rica)
	// ...


// #importante
// isso ser� usado na estrutura de thread em wait_reason[]
// Com limite de 10 por enquanto.

typedef enum {
	
	WAIT_REASON_NULL,
	WAIT_REASON_LOOP,           
	WAIT_REASON_EXIT,
	WAIT_REASON_BLOCKED,
	WAIT_REASON_PREMMPTED,      // ?? Esperando a preemp��o de thread de menor prio.
	WAIT_REASON_SEMAPHORE,      // ?? Sem�foro.
	WAIT_REASON_WAIT4PID,       // Esperando o processo filho morrer.
	WAIT_REASON_WAIT4TID,       // Esperando uma thread morrer.
	WAIT_REASON_TEST            // # Usada pelo desenvolvedor para testes.

	//continua... @todo
}thread_wait_reason_t;




/*
 * thread_type_t:
 *     Enumerando os tipos de threads:
 *
 *   idle     - Threads do tipo idle.     
 *   rr       - Threads do tipo round robin.
 *   periodic - Threads do tipo peri�dicas. 
 *   system   - Threads do tipo system.
 *   ...
 *
 * Ordem de implanta��o:
 * ====================
 *     (De acordo com a ordem de constru��o de um sistema).
 *
 *    TYPE_NULL     0
 *    TYPE_SYSTEM   1  // Fundamentais para o funcionamento do sistema.  
 *                     // Primeiras tarefas rodando quando o O.S. est� sendo criado.
 *                    
 *    TYPE_IDLE     2  // Exclusivo para Threads do tipo idle. 
 *                     // ( V�rios tipos de rotinas podem ser realizadas durante o per�odo
 *                     // que o processador estiver ocioso. O usu�rio pode configurar
 *                     // o que o sistema deve fazer nesses momentos de ociosidade).
 *					   
 *    TYPE_PERIODIC 3  // Tarefas peri�dicas. 
 *                     // (Rodam de tempos em tempos, como o deadthread collector).
 *
 *    TYPE_RR       4  // Threads tipo round robin. 
 *                     //(Confinadas em um processador, N�o importa a prioridade, nem o deadline).
 *
 *    TYPE_REALTIME 5  // Realtime + round robin. 
 *                     // (Confinadas em um processador, Importa a prioridade o dead line, o step
 *                     //  principalmente � sistemicamente importante que se cumpra a execu��o em tempo.) 
 *
 *    TYPE_UI       6  // UI user interface thread @todo:
 * 
 *    TYPE_IO       7  // i/o thread @todo
 *
 *    Continua ...
 */
 
typedef enum {
	
	TYPE_NULL,
	TYPE_SYSTEM,     // first-come-first-served.
	TYPE_IDLE,       // 
	TYPE_PERIODIC,   // periodic threads with predefined intervals.
	TYPE_RR,         // first-come-first-served cooperative.
	TYPE_REALTIME,
	//TYPE_UI, //@todo
	//TYPE_IO, //@todo
	//...

}thread_type_t;


/*
 * thread_state_t:
 *    Enumeram os estados de uma threads, (8 estados).
 *    Existem 2 grupos, 'Earth' e 'Space'.
 *    Obs: Rodam no 'espa�o'.
 *
 * Earth: (INITIALIZED, STANDBY, ZOMBIE, DEAD).
 * Space: (READY, RUNNING, WAITING, BLOCKED).
 *
 *  INITIALIZED,    //Earth, Criado o contexto e par�metros.
 *  STANDBY,        //Earth, Pronta para rodar pela primeira vez. Ir para o 'espa�o'.
 *  ZOMBIE,         //Earth, Terminou a execu��o. Voltou para a 'terra'.
 *  DEAD,	        //Earth, Deleted.	
 
 *  READY,          //Space, Thread is ready to run again.
 *  RUNNING,        //Space, Thread is currently running.
 *  WAITING,        //Space, Thread is waiting.	
 *  BLOCKED,        //Space, Thread is blocked by an event.
 *
 * Obs: 
 *     Na pr�tica, a troca de status est� seguindo um organograma de 
 * movimentos. 
 *     @todo: Descrever esses movimentos na documenta��o. Incluir a 
 * visualiza��o atrav�s do gr�fico.
 */
 
typedef enum {

	INITIALIZED,    //0 Earth, Criado o contexto e par�metros.
	STANDBY,        //1 Earth, Pronta para rodar pela primeira vez. Ir para o 'espa�o'.
	ZOMBIE,         //2 Earth, Terminou a execu��o. Voltou para a 'terra'.
	DEAD,           //3 Earth, Deleted.
	READY,          //4 Space, Thread is ready to run again.
	RUNNING,        //5 Space, Thread is currently running.
	WAITING,        //6 Space, Thread is waiting.
	BLOCKED,        //7 Space, Thread is blocked by an event.

}thread_state_t;

 
/*
 ***********************************************************
 * thread_d: 
 *
 *    TCB - Thread Control Block.
 *
 * A estrutura onde guarda informa��es sobre a thread.
 * #todo: 
 * Come�ar com informa��es sobre disco e arquivos.
 * #obs: 
 * Deve ficar por �ltimo o que for referenciado com menos frequ�ncia.
 */

struct thread_d 
{	
	object_type_t objectType;
	object_class_t objectClass;
	
	//object control
	struct object_d *object;

	
	//call back //d
	
    //
	// Identificadores.
	//
	
	int tid;                //c, thread ID.   	
	int ownerPID;           //ID do processo ao qual o thread pertencer. 
	//int ownerPPID;        //Acho que isso n�o � necess�rio !! 
	
	//#importante:
	//Isso pode ser unsigne long mesmo.
	unsigned long used;     
    unsigned long magic;    
	
    //
	// type: 
	// Tipo de tarefa.
    // (SYSTEM, PERIODIC, RR, IDLE).	
	thread_type_t type;	
	
	thread_state_t state;    //flag, Estado atual da tarefa. ( RUNNING, DEAD ...).	


	// flag. 
	// 1 = Sinaliza que a thread est� dando a prefer�ncia
	// e que deve sair quando for seguro fazer isso.
    int _yield;


	// error. @todo:
	//unsigned long error;
	
	//ext.
	
    //plano de execu��o.
	int plane;
	
	//
	// Names.
	//
	
	//char *name;  //@todo: Usar isso.
	unsigned long name_address;
	unsigned long name_pointer;   	
	char short_name[4];
	char *cmd;

    //#test
    //Assim fica mais f�cil enviar para o aplicativo.
    char __threadname[64];    // HOSTNAME_BUFFER_SIZE
    size_t threadName_len;    // len 



    //
	// ## CPU support ##
	//
    
	int cpuID;            //Qual processador.
	int confined;         //Flag, confinado ou n�o.
	int CurrentProcessor;
	int NextProcessor;
	//int IdealProcessor;
	
	
	//
	// ORDEM: O que segue � referenciado durante a interrup��o de timer.
	//

	
	//...
	
	//
	// ORDEM: O que segue � referenciado durante o processo de task switch.
	//
	
	
	//
	//  ## Directory ##
	//	
	
    // COLOCAR O DIRET�RIO DE P�GINAS QUE A THREAD USA, ISSO AJUDA NA 
	// HORA DO TASKSWITCH.

	//unsigned long DirectoryVA;
	unsigned long DirectoryPA;
	
	
	//IOPL of the task. (ring).
	//@todo: isso pode ser um char.
	unsigned long iopl; 
	
	
	// Context. 
	// #todo: 
	// Usars uma estrutura.
	// #todo: 
	// Isso deve virar um ponteiro de estrutura.

	//stack frame;
    unsigned short ss;
    unsigned long esp;
    unsigned long eflags;
    unsigned short cs;
    unsigned long eip;	//usado com o pd do processo

	// para o kernel saltar para o novo processo.
    unsigned long ring0_eip;  //usado com o pd do kernel		
    unsigned long eipPA;	


    unsigned short ds;
    unsigned short es;
    unsigned short fs;
    unsigned short gs;

    unsigned long eax;
    unsigned long ebx;
    unsigned long ecx;
    unsigned long edx;
    unsigned long esi;
    unsigned long edi;
    unsigned long ebp;
	//continua o contexto ...	
	
	//O endere�o incial, para controle.
	unsigned long initial_eip;	
	
	//#todo
	//isso � muito necess�rio.
	struct i386tss_d *tss;
	
	//
	// ORDEM: O que segue � referenciado durante o processo de scheduler.
	//
	
	//poderia ser base_priority e dinamic_priority.
	unsigned long base_priority;    //Prioridade b�sica.
	unsigned long priority;         //Prioridade din�mica.
	
	
	
	/*
	 * preempted:
	 *     flag ~ Sinaliza que uma tarefa pode ou n�o sofrer preemp��o.
     *	   Uma tarefa de menor prioridade pode deixar o estado running 
	 * para assumir o estado ready em favor de uma tarefa de maior prioridade
	 * que assumir� o estado running.
	 * @todo: isso pode ser int, bool ou char.
	 */
	unsigned long preempted;
	
	//
	// ORDEM: O que segue � referenciado durante o processo de dispatch.
	//
	
	/*
	 * save ~ Sinaliza que a tarefa teve o seu contexto salvo.
	 @todo: isso pode ser int, bool ou char.
	 */
	unsigned long saved;
	
	
	/*
	 * HEAP and STACK:
     * @todo: Usar a estrutura. 
	 */
	//struct heap_d *heap;
	
	unsigned long Heap;
	unsigned long Stack;
	unsigned long HeapSize;
	unsigned long StackSize;
	

	//endere�o de um array contendo ponteiros para variso servi�os
	//que a thread pode usar.
	//unsigned long ServiceTable;
	
    //
	// ## Temporizadores  ##
	//
	
	//
    // @todo: Ticks and Deadline.
	//
	// Quanto tempo passou, mesmo quando a tarefa n�o esteve rodando.
	// Quando tempo a tarefa tem para que ela complete a sua execu��o.
	//
	//unsigned long Ticks;
    //unsigned long DeadLine.
	
	//Steps.
	//Quantas vezes a tarefa usou o processador. 
	unsigned long step; 

	// sys time inicial da thread.
	// quando ela foi criada.
	unsigned long initial_time_ms;
	
	//ms total..
	unsigned long total_time_ms; 
	
	
	//Quantum. (time-slice, igual cota) 
	unsigned long quantum;         //tempo que a thread tem.
	unsigned long quantum_limit;   //limite quando dado boost. tempo limite rodando. 		
	
	//unsigned long  RemainingTime;  //??
	
	unsigned long standbyCount;
	
	//Contando tempo rodando.
	//ticks rodando antes de parar.
	unsigned long runningCount; 
	
	//ms rodando antes de parar.
	unsigned long runningCount_ms; 

	//Obs: A soma das 3 esperas � a soma do tempo de espera
	//depois que ela rodou pela primeira vez.
	
    //Contando o tempo nos estados de espera.
	unsigned long readyCount;   //tempo de espera para retomar a execu��o.
	unsigned long ready_limit;
	
	//Esperando por eventos.
	unsigned long waitingCount; //tempo esperando algo.	
	unsigned long waiting_limit;   //tempo limite que uma tarefa ready fica sem rodar.
    
	unsigned long blockedCount;
	unsigned long blocked_limit;
	
	
	//Ticks remaining. (tempo para a tarefa chegar ao fim, tempo total-tempo percorrito)
	unsigned long ticks_remaining; //rt, quanto tempo a tarefa tem dispon�vel para ser concluida.


	//
	// Working set support.
	// 
	
	//profiler support 
	
	//quanto por cento do tempo o processo ficou rodando.
	//� a soma do quanto ficou rodando todas as suas threads.
	unsigned long profiler_percentage_running;
	unsigned long profiler_percentage_running_res;
	unsigned long profiler_percentage_running_mod;
	unsigned long profiler_ticks_running;
	unsigned long profiler_last_ticks;

	//unsigned long alarm;            //Tempo para o pr�ximo alarme, dado em ticks.
	
	//??iopl??
	//@todo: isso n�o precisa ser unsigned long.
	unsigned long PreviousMode;	

	
	/*
     * @todo: afinidade e liga��o entre tarefas.
	 */
    //int idealprocessornumber;
		
		
	//
	// ORDEM: O que segue � referenciado com pouca frequencia.
	//
	
	//lista de arquivos ??
	//fluxo padr�o. stdio, stdout, stderr
	//unsigned long iob[8];
    
	//#bugbug: o vetor Stream[] conter� essas stream tamb�m.
	//ponteiros para as streams do fluxo padr�o.
	//O processo tem streams ... Stream[] ...
	//cada tread pode ter suas stream ... mesmo que herde streams 
	//de processo ...
	// ?? threads diferentes do mesmo processo podem atuar em streams 
	// diferentes ??
	//unsigned long standard_streams[3];
	//unsigned long Streams[8];
	
	//Obs: Cada processo est� atuando em um diret�rio,
	// mas ser� cada thread precisa atuar em um diret�rio diferente??
	//
	//struct _iobuf *root;	// 4 root directory
	struct _iobuf *pwd;	    // 5 (print working directory) 
	//...
		
	
	//@todo: Uma thread pode estar esperando varias outras por motivos diferenes.
	//struct wait_d WaitBlock;
	
	
	//
	// tty support
	//
	
	// ID da tty usada.
	int tty_id;
	

    //process.
	//� qual processo pertence a thread.
	struct process_d *process; 

	
	/*
	 * Janela e procedimento.
	 */
	struct room_d *room;  //Window Station da thread.
	struct desktop_d *desktop;         //Desktop da thread.
	
	unsigned long procedure; //Endere�o do procedimento de janela da tarefa. 
	//unsigned long control_menu_procedure; //procedimento do control menu.
	
	
	// #ORDEM: 
	// O que segue � referenciado durante as trocas de mensagens.
	// utiliza��o de canais e IPC.

    //
    // Single message.
    //

	
	//4 argumentos padr�o;
	struct window_d *window;    //arg1.
	int msg;                    //arg2.
	unsigned long long1;        //arg3.
	unsigned long long2;        //arg4.
	
	//argumentos extra usados pelos drivers e servidores.
	unsigned long long3;
	unsigned long long4;
	unsigned long long5;
	unsigned long long6;
	unsigned long long7;
	unsigned long long8;
	unsigned long long9;
	unsigned long long10;
	unsigned long long11;
	unsigned long long12;
	//...
	
	int newmessageFlag;         //flag avisando que tem nova mensagem.
	
	
	
	
	struct window_d *window_list[32];
	int msg_list[32];
	unsigned long long1_list[32];
	unsigned long long2_list[32];
	int tail_pos;
	int head_pos;
	
    //
    // Message Queue
    //

    // Coloca-se em tail, quande chegar ao fim do buffer, recome�a.
    // Se o tail encontrar o head � porque o processo n�o est� 
    // respondendo.
    unsigned long MsgQueue[32];
    int MsgQueueHead;  //retira. 
    int MsgQueueTail;  //coloca.



	//?? mensagens pendentes.
	//struct thread_d *sendersList; //Lista encadeada de threads querendo enviar mensagem
	//struct thread_d *nextSender;  //pr�xima thread a enviar mensagem.
	

	// Objeto pelo qual a thread est� esperando.
	// #todo: mudar esses nomes, pode confundir com o header no in�cio da 
	// estrutura. (waiting_object_type ... woType woClass )
	object_type_t woType;   //obType;      //woType
	object_class_t woClass; //obClass;	   //woClass
		
	//#importante
	//raz�es para esperar
	//#todo: isso precisa ser inicializado.
	//@todo: tem que fazer um enum para enumerar as raz�es.
	//o �ndice � o selecionador da raz�o pela 
	//qual a thread est� esperando.
	//existem umas 20 raz�es pra esperar.
	// 0 - esperando por mensagem.(presa num loop)
	// 1 - esperando outra thread finalizar. wait4tid
	// 2 - esperando um processo finalizar. wait4pid
	// 3 - esperando um objeto. (espera gen�rica)
	// ...
	
	int wait_reason[10]; 
	
	int wait4pid;   //id do processo que a thread est� esperando moorrer.
	int wait4tid;   //id da thread que a thread est� esperando moorrer.
	
	//
	// ## Exit support ##
	//
		
    //Motivo da thread fechar.
	int exit_code; 	
	

	//Signal
	unsigned long signal;
	unsigned long signalMask;
	
	//Next: 
    //Um ponteiro para a pr�xima thread da lista linkada. 
	struct thread_d *Next;
};



// Ponteiro para a idle thread atual
// Sempre que mudar a idle thread devemos usar esse ponteiro
// para mostrar qual ser� a nova idle thread.
// Cada idle thread pode prestar um servi�o diferente, como o
// gerenciamento de energia.
struct thread_d *____IDLE;


// Essa � a criada para o processo kernel.
// Ela roda em ring0 e ser� usada como idle.
// RING0 IDLE Thread. TID=3
struct thread_d *RING0IDLEThread;    


// Essa � a thread de controle do processo init2.bin
// � o primeiro processo em ring3.
// Idle Thread. TID=0
struct thread_d *InitThread;         


// Ponteiro para a thread usada na hora da clonagem de processos.
struct thread_d *ClonedThread;





/* 
 * Listas encadeadas de threads.
 * Usadas no gerenciamento de rounds 
 */
struct thread_d *Conductor;
struct thread_d *Conductor2;
struct thread_d *rootConductor;
int conductorIndex;



// #todo
// Podemos planejar o uso de listas nesse formato.
// Mas estamos trabalhando em filas de tamanho fixo no formato de array.
//struct thread_d *blocked_list_head;
//struct thread_d *waiting_list_head;
//struct thread_d *ready_list_head;




//
// Thread list.
//

// #Aten��o
// Esse � a lista principal. Cont�m todas as threads.
 
// N�mero m�ximo de threads.
#define THREAD_COUNT_MAX 1024  

unsigned long threadList[THREAD_COUNT_MAX];



/* thread_list_d: */ 
typedef struct thread_list_d thread_list_t; 
struct thread_list_d 
{   
	struct thread_d *Threads;    //List.	
	unsigned long priority;      //Prioridade.
	//...
};
//thread_list_t DispatcherList[10]; //list
//thread_list_t DispathcerPriorityQueue[10];
//unsigned long DispatcherList[10];

//DispatcherList[2].Threads[4].tid


//clona uma thread e retorna o ponteira da clone.
struct thread_d *threadCopyThread ( struct thread_d *thread );


/* 
 * create_thread: 
 */ 

struct thread_d *create_thread ( struct room_d *room,
                                 struct desktop_d *desktop,
                                 struct window_d *window,
                                 unsigned long init_eip, 
                                 unsigned long init_stack, 
                                 int pid, 
                                 char *name );


void *GetCurrentThread (void);

void *FindReadyThread (void);

int GetThreadState (struct thread_d *Thread);
int GetThreadType (struct thread_d *Thread);
void dispatch_thread (struct thread_d *thread);
void set_thread_priority (struct thread_d *t, unsigned long priority);


// ## Page directory support ## 

// Pega o endere�o do diret�rio de p�ginas usado pela thread.
unsigned long GetThreadDirectory( struct thread_d *thread );

// Altera o endere�o do diret�rio de p�ginas de uma thread.
void SetThreadDirectory ( struct thread_d *thread, unsigned long Address );

/* Thread heap support. */
unsigned long GetThreadHeapStart (struct thread_d *thread);
//...

/*
 * Thread Stack support.
 */
unsigned long GetThreadStackStart(struct thread_d *thread);
//...

void SelectForExecution (struct thread_d *Thread);



// Show info about all threads.
void show_slots (void);

// Show info about a thread.
void show_slot(int tid);

// Show the register of the context.
void show_reg (int tid);



void show_thread_information (void); 

int init_threads (void);

int GetCurrentThreadId (void);


// Usadas durante a inicializa��o e execu��o de uma thread.
// @todo: mudar apenas o prot�tipo para thread.h
void spawn_thread (int id);


// ## Finaliza��es ##


// Liberar uma thread que estava bloqueada ou esperando.
void release ( int tid );


//Torna zumbi uma thread.
void exit_thread ( int tid );       


//Destr�i uma thread.
void kill_thread (int tid);       


void dead_thread_collector (void);


void kill_all_threads (void);


int thread_getchar (void);


// se a flag estiver habilitada, ent�o devemos acorar a
// thread do dead thread collector.
void check_for_dead_thread_collector (void);

int thread_profiler( int service );


// pegar a porcentagem de vezes que a thread rodou durante um determinado
//per�odo.
unsigned long 
thread_get_profiler_percentage ( struct thread_d *thread);

void thread_show_profiler_info (void);


int getthreadname ( int tid, char *buffer );


unsigned long __GetThreadStats ( int tid, int index );

//
// End.
//


