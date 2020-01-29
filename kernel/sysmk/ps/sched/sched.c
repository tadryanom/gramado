/*
 * File: ps/sched/sched.c
 *
 * Descri��o:
 *     O escalonador de processos do Kernel.
 *     Faz parte do Process Manager, parte fundamental do Kernel Base.
 *     The kernel schedules threads, not processes.
 *     Priority: +boosts for GUI foreground.
 *
 * Atribui��es:
 *     + Salva o contexto da tarefa OLD.
 *     + Incrementa a tarefa atual. (RR).
 *     + Retorna o contexto da tarefa NEW.
 *
 * Obs:
 *     O Scheduler n�o salva nem restaura contexto de tarefa.
 * Apenas troca a tarefa atual e faz um agendamento das pr�ximas
 * threads a rodarem.
 *     Ser� poss�vel usar mais de um m�todo de escalonamento. No momento,
 * o m�todo usado � escalonamento Round Robin, preemptivo,
 * com m�ltiplas filas e prioridade.
 *
 * @todo:
 *     N�o por interface nessa rotina. Perde velocidade.
 *
 * Hist�rico:
 *     Vers�o 1.0, 2015 - Esse arquivo foi criado por Fred Nora.
 *     Vers�o 1.0, 2016 - Aprimoramento geral das rotinas b�sicas.
 *     //...
 */

 /*
  * #importante:
  * Para implementarmos um scheduler como um processo, devemos separar 
  * as fun��es em fun��es de biblioteca e fun��es primitivas, que ser�o 
  * aquelas presentes no stub do scheduler. As fun��es primitivas 
  * ser�o as mesmas para todos os schedulers que criarmos.
  */
 

#include <kernel.h>


//
// Vari�veis internas.
//

//int schedulerStatus;
//int schedulerError;
//int schedulerType;
//...



/*
 * pick_next_thread:
 *     Selecionamos a 'next_thread' olhando nas filas em QUEUE[].
 *     QUEUE[] cont�m ponteiros para listas encadeadas.
 *     Se n�o tiver nada nas filas ent�o usaremos a InitThread.
 *
 *     #bugbug: Essa InitThread est� configurada corretamente. ?
 *     #bugbug: E se n�o existir IfleThread? 
 */

struct thread_d *pick_next_thread (void){

    int i;      //id da fila selecionada
    int old;    //salva id da current thread

    struct thread_d *t;
    struct thread_d *next;


	//salva antiga thread
    old = next_thread;
	
	
	//se temos um ponteiro para fila de drivers.
    if ( QUEUES[0] != 0 ){

        i = 0;

	//se temos um ponteiro para a fila de servidores.

    } else if ( QUEUES[1] != 0 ){

        i = 1;

	//nos resta a fila de apps de usu�rio.
    }else{
        i = 2;
    };


	//
	// # Checando o conductor. #
	//


	//se o elemento tem um valor n�o nulo..
    if (QUEUES[i] != 0)
    {
	    //Ok temos uma fila.
		//vamos pegar a primeira thread da fila.
		
        t = (void *) QUEUES[i];

    }else{

		// nenhuma thread est'a no estado de READY ... entao nenhuma das
		// filas foi construida.
		// nos reata usarmos a thread idle. 
		
		//podemos fica nessa condi�~ao at'e que uma thread seja acordada ...
		//talvez ela esteja esperando alguma recurso,.
		//quando ela acordar ir'a pra alguma fila.
		
		
		
		// #todo
		// Mudar para ____IDLE;
		// Selecionamos a idle.
		// Isso est� errado, estamos selecionando a thread de controle
		// do processo init.bin, mas queremos a idle do kernel.
        t = InitThread;
        //t = ____IDLE;
    };



	//
	// Checando a thread selecionada.
	//

    if ( ( void *) t == NULL )
    {
	    //fail
		next_thread = old;
		
    }else{

        // #bugbug
        // Se a estrutura falhou ent�o n�o podemos usar essa thread.
        // Tem que abortar a tentativa.
        
        if ( t->used != 1 || t->magic != 1234 )
        {
            next_thread = old;
            goto prepare_next;
        }

		//Ok.
        next_thread = t->tid;
    };


prepare_next:

	//
	// # Checando a validade da next thread. #
	//

    next = (void *) threadList[next_thread];

    if ( ( void *) next == NULL )
    {
	    //fail
	    // #debug
	    //N�o conseguimos selecionar nenhuma thread como pr�xima.
		//n�o temos nem mesmo uma thread idle para inicializarmos o round.
			
		//#debug
		//printf ("#DEBUG\n");
        printf ("pick_next_thread: No next_thread, we could't initialize the round\n");
        die ();

    }else{
	

        if ( next->used != 1 || next->magic != 1234 )
        {
	        //fail
	        // #debug
	        //N�o conseguimos selecionar nenhuma thread como pr�xima.
		    //n�o temos nem mesmo uma thread idle para inicializarmos o round.
		    //#debug
			//printf ("#DEBUG\n");
            printf ("pick_next_thread: No next_thread, we could't initialize the round\n");
            die ();
        }

        if ( next->state == READY )
        {
            return (struct thread_d *) next;
        }
    };


    return NULL;
}




/*
 ***************************************************************
 * scheduler:
 *    Troca a thread atual, escolhe uma nova thread atual 
 * para rodar no momento.
 *    O m�todo � cooperativo, Round Robing.
 *
 * Ordem de escolha:
 * ================
 *  +fase 1 - Pega a pr�xima indicada na estrutura.
 *  +fase 2 - Pega nos slots a de maior prioridade.
 *  +fase 3 - Pega a Idle. 
 *            @todo: Nessa fase devemos usar a idle atual, 
 *            indicada em current_idle_thread.  
 *  //...
 *
 * Obs:
 *     O que estamos fazendo aqui � incrementar a tarefa atual e olhando se a
 * pr�xima tarefa da lista threadList[] est� pronta pra rodar.
 *
 * Obs:
 *     Pega na fila ReadyQueue.
 *     O scheduler deve sempre pegar da fila do dispatcher.
 */
 
//#todo
//podemos contar os rounds;

int scheduler (void){

    int Index;
    struct thread_d *Thread;


#ifdef SERIAL_DEBUG_VERBOSE
    debug_print(" [*SCHEDULER*] ");
#endif


	//#debug
	//printf ("scheduler: Running Threads %d \n", ProcessorBlock.threads_counter );
	//refresh_screen();
	
	// spiritual quote:
	// "Constr�i um caminho de vag�es para o condutor andar".

	//Usado para task switch.
	//Conductor = (void *) rootConductor;
	//Inicia a lista.
 	//Conductor2 = (void *) rootConductor;
	//Next thread.
	//Conductor2->Next = (void *) threadList[next_thread];  
	
	
	//#importante 
	//rootConductor � um ponteiro v�lido,
	//mas a estrutura n�o foi inicializada.
	
	//Inicia a lista.
	//marca o in�cio, mas n�o usa esse ponteiro.
  
  //#debug
  //printf ("scheduler: 1\n");

    Conductor2 = (void *) rootConductor;

	
	//
	// ## preparando 'next_thread' ##
	//
	
	//N�o sabemos quem est� em next_thread.
	//Checaremos a possibilidade de n�o termos filas configuradas.
	//nesse caso a idle thread ser� a primeira trhead do round
	//n�o havendo outra, pelo menos ela vai rodar.
	
  //#debug
  //printf ("scheduler: 2\n");
	
  // #debug
  // supendendo essa rotina que falhou em gigabyte/intel
	// Vamos substituir a thread que a fun��o retorna pela current_thread
  // que esperamos que seja v�lida. Talvez isso resolva o problema por enquanto.
  // Mas trabalharemos nessa fun��o depois.
  
  //pick_next_thread ();
	
  // HACK HACK
  //next_thread = current_thread;
	
	//
	//  ## PRIMEIRO PONTEIRO V�LIDO ##
	//
  
   //#debug
 // printf ("scheduler: 3\n"); 
	
	
	//Next thread.
    //Conductor2->Next = (void *) threadList[next_thread];
    Conductor2->Next = (void *) ____IDLE;
    
    
    
    
	//
	//  ## MARCADOR GLOBAL ##
	//
	
  //#debug
 // printf ("scheduler: 4\n");
  
	//TID=0
	//vamos come�ar a lista dessa aqui.
    Conductor = (void *) Conductor2->Next;

	
	// Obs: 
	// ## IMPORTANTE  ##
	// Os primeiros tipos a se pegar s�o os de 
	// prioridade maior.
	// @todo: Elevar a prioridade da threads interativas,
	// como teclado e mouse, e n�o elevar das threads de 
	// i/o de disco.
	// Elevar a prioridade da thread associada a janela 
	// com o foco de entada.

	// Obs: 
	// ## IMPORTANTE  ##	
	// A thread idle somente � usada quando o sistema 
	// estiver ocioso ou quando ela for a �nica thread.
	// E � importante que a thread idle seja usada, pois 
	// ela tem as instru��es sti/hlt que atenua a utiliza��o 
	// da CPU, reduzindo o consumo de energia.

	// Agora, antes de tudo, devemos pegar as threads 
	// nas listas onde est�o as threads de maior prioridade.

	//Encontra o id da thread de maior prioridade entre as 
	// threads que est�o no estado READY.
	//KiFindHigherPriority();

	// ## Importante  ##
	// Daqui pra baixo pegaremos na lista threadList[] 
	// onde est�o todas as threads.

    //@todo pegar primeiro por prioridade.
	
  //#debug
 // printf ("scheduler: 5\n");


	//READY.
    for ( Index=0; Index < THREAD_COUNT_MAX; Index++ )
    {
        Thread = (void *) threadList[Index];

		if ( (void *) Thread != NULL )
		{
			if ( Thread->used == 1 && 
			     Thread->magic == 1234 && 
				 Thread->state == READY )
			{
			    Conductor2 = (void *) Conductor2->Next; 
				Conductor2->Next = (void *) Thread;
			};
		    //Nothing.
		};
		//Nothing.
    };


	//
	// Continua pegando os tipos diferente.
	//

  //#debug
  // printf ("scheduler: 6\n");

	//finaliza a lista
    Conductor2 = (void *) Conductor2->Next; 
    Conductor2->Next = NULL;
    
    return (int) Conductor2->tid;

    // #option:
    //Conductor2->Next = (void *) rootConductor; //circular infinita. 
    //return (int) rootConductor->tid; //come�ando do in�cio da lista.
}


/*
 *************************************************
 * scheduler_start:
 *     +Inicializa o sheduler.
 *     +Trava o scheduler.
 *     +Torna IDLE a tarefa atual.
 *     //...
 *
 *     @todo: nao seria init_scheduler
 *     @todo: Mudar para schedulerInit,ou schedulerStart. ??
 */

void scheduler_start (void){
	
	//Lock Scheduler.
	//Set current.
	
    scheduler_lock ();  

    // #bugbug 
	// Esse neg�cio de selecionar pelo impondo um ID d� problemas.
	// Temos que saber qual � o ID da thread que queremos.
	
	set_current (IDLE);     
	//...
}


/* 
 * scheduler_lock: 
 */
void scheduler_lock (void)
{
    g_scheduler_status = (unsigned long) LOCKED;
}


/* 
 * scheduler_unlock: 
 */  
void scheduler_unlock (void)
{
    g_scheduler_status = (unsigned long) UNLOCKED;
}


/*
 * scheduler_get_status:
 *     Pega o status do scheduler, se ele est� travado ou n�o.
 */
 
unsigned long scheduler_get_status (void)
{
    return (unsigned long) g_scheduler_status;
}


/*
 * new_task_scheduler: 
 *     ?? #deletar
 */

/* CANCELADA !*/ 

void new_task_scheduler (void)
{   
    return;
}


/*
 * init_scheduler:
 *    Inicaliza o scheduler.
 *    @todo: Mudar para schedulerInit()
 *    #burbug: faz o mesmo que scheduler_start.
 */


void init_scheduler (void)
{
	// #todo: 
	// Implementar inicializa��o de variaveis do scheduler.
	// O nome poderia ser schedulerInit().
	// Formato de classes.Init � um m�todo. 
}


/*
 constructor.
int schedulerScheduler()
{
}
*/

/*
int schedulerInit()
{
}
*/


//
// End.
//

