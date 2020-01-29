/*
 * File: ps/action/dispatch.c
 *
 * Descri��o:
 *     Arquivo principal do dispatcher do kernel.
 *     Faz parte do Process Control, parte fundamental do 
 * Kernel Base.    
 *     Coloca tarefas pra rodar que estejam em suas 
 * respectivas filas.
 *     Alterna entre as filas de acordo com o tipo de dispatcher.
 * As op��es s�o:
 *     +System dispatcher. 
 *     +Idle dispatcher.
 *     +Periodic dispatcher.
 *     +round robin dispatcher.
 *     +realtime dispatcher.
 *
 * Observa��o:
 *    Somente um tipo de dispatcher est� ativo no momento.
 *
 * History
 *     2015 - Created by Fred Nora.
 *     2018 - Revision.
 *     //...
 */


#include <kernel.h>


//
// Vari�veis internas.
//

//int dispatchStatus;
//int dispatchError;
//...


//
// #todo:
// Prot�tipo de fun��es internas.
//

int syscoopDispatcher (void);
int usercoopDispatcher (void);
int sysconcDispatcher (void);
int userconcDispatcher (void);
int systemDispatcher (void);
int readyDispatcher (void);
int idleDispatcher (void);
int periodicDispatcher (void);
int rrDispatcher (void);
int realtimeDispatcher (void);
int dispatch_Default (void);



/*
 *********************************************************
 * dispatcher:
 *     Despacha a thread atual que foi escolhida pelo scheduler. 
 *     Despacha, colocando ela no estado RUNNING e restaurando os valores 
 * dos registradores.
 *     Essa fun��o � chamada no final da rotina task_switch, antes dela 
 * retornar.
 *     Obs: 
 *     Despacha a tarefa de acordo dom o tipo de dispatcher.
 *     Por�m os tipos diferentes de dispacher ainda n�o est�o habilitados, 
 * s� um funciona.
 */

// #todo
// Change the return type and return with error
// if something goes wrong. So this way we can try another thing.
// int dispatcher ( int type ){

void dispatcher ( int type ){

    int Status;
    struct thread_d *dispatch_Pointer;


	// Obs: 
	// @todo:
	// (Fase de teste). 
	// Usando apenas um tipo de dispatcher. 
	// Deteminando o tipo de dispacher por enquanto


    if ( type != DISPATCHER_CURRENT )
    {
        type = DISPATCHER_CURRENT;
    }


	//
	// Seleciona o tipo.
	//

    // #todo:
    // All the different types of dispatch need to be implemented.

//SelectDispatcherType:


    switch (type)
    {

        // It's not implemented.
        case DISPATCHER_NULL:
            goto dispatchCurrent;     
            break;

        // It's not implemented.
        case DISPATCHER_SYSCOOP:
            goto dispatchSyscoop;
            break;

        // It's not implemented.
        case DISPATCHER_USERCOOP:
            goto dispatchUsercoop; 
            break;

        // It's not implemented.
        case DISPATCHER_SYSCONC: 
            goto dispatchSysconc;
            break;

        // It's not implemented.
        case DISPATCHER_USERCONC: 
            goto dispatchUserconc;
            break;

        // It's not implemented.
        //System.
        case DISPATCHER_SYSTEM:
            goto dispatchCurrent;
            break;

        // It's not implemented.
        //Idle.
        case DISPATCHER_IDLE:
            goto dispatchCurrent;
            break;

        // It's not implemented.
        //Periodic. 
        case DISPATCHER_PERIODIC:
            goto dispatchCurrent;
            break;

        // It's not implemented.
        //Round Robin. (RR).
        case DISPATCHER_RR:
            goto dispatchCurrent;
            break;

        // It's not implemented.
        //Realtime.
        case DISPATCHER_REALTIME:
            goto dispatchRealtime;
            break;


        // This is working ...
        case DISPATCHER_CURRENT:
            goto dispatchCurrent;
            break;
 
        // It's not implemented.
        //Despacha da fila do dispatcher(ready queue)
        case DISPATCHER_READY:
            goto dispatchReady;
            break; 

        //default.
        default:
            goto dispatchCurrent;
            break;
     };


	// Obs: 
	// E se escapar do la�o acima ?!


//Dispatch sys coop.
dispatchSyscoop: 
    current_thread = syscoopDispatcher();
    goto do_dispatch;
  
//Dispatch user coop.
dispatchUsercoop:
    current_thread = usercoopDispatcher();
    goto do_dispatch;
   
//Dispatch system conc.
dispatchSysconc:
    current_thread = sysconcDispatcher();
    goto do_dispatch;
   
//Dispatch user conc.
dispatchUserconc:
    current_thread = userconcDispatcher();
    goto do_dispatch; 

//Dispatch realtime.
dispatchRealtime:
    current_thread = realtimeDispatcher();
    goto do_dispatch;

//Dispatch ready.
dispatchReady:
    current_thread = readyDispatcher();
    goto do_dispatch;


//Dispatch current.
dispatchCurrent:
    //current_thread = current_thread;
    goto do_dispatch;


    //
    //    ####  DO DISPATCH ####
    //

//----------------------------------------
// Do Dispatch: Dispatch 'current_thread'.
//----------------------------------------

do_dispatch:


	// Checa estrutura.

    dispatch_Pointer = (void *) threadList[current_thread];

    if ( (void *) dispatch_Pointer == NULL )
    {
        printf ("dispatch-dispatcher: struct\n");
        die ();
    }


	// Checa o 'state'.

    if ( dispatch_Pointer->state != READY )
    {
        printf ("dispatch-dispatcher: State ERROR\n");
        die ();
    }


	// #importante
	// * MOVEMENT 4 (Ready --> Running).
	// A thread passa para o estado RUNNING.
	// Reinicia a contagem.


    if ( dispatch_Pointer->state == READY )
    {
        dispatch_Pointer->state = RUNNING;
        dispatch_Pointer->runningCount = 0;

        queue_insert_data ( queue, 
            (unsigned long) dispatch_Pointer, QUEUE_RUNNING );
    }


	//
	// ## RESTORE CONTEXT ##
	//

	// #importante
	// Flag sinalizando que o contexto n�o est� mais salvo.
	// Esse flag � acionada quando o contexto � salvo no in�cio 
	// da task switch.

    dispatch_Pointer->saved = 0;

	// #importante
	// Chama a rotina que colocar� as informa��es da estrutura de thread 
	// nas vari�veis usadas pelo assembly para configurar os registradores 
	// antes do iretd.

    restore_current_context ();


// #todo
// Change the return type and return with error
// if something goes wrong. So this way we can try another thing.
    //return 0;

    return;
}


//
//  ## IMPORTANTE  ##
//  Todos os outros modelos de dispacher abaixo ainda 
//  n�o foram habilitados.
//

/*
 * readyDispatcher:
 *     Dispatcher principal. 
 *     Pega da fila do dispatcher. Que � a fila de READY tasks.
 *     Pega a head da fila de ready quando vencer o tempo dela de espera.
 *     Obs: Esse tipo de dispacher ainda n�o foi habilitado.
 */

// #todo
// It's not implemented.

int readyDispatcher (void)
{
	struct thread_d *dispatch_Pointer;
	
    dispatch_Pointer = (void*) queue_get_data( queue, QUEUE_READY);
	
	if( (void*) dispatch_Pointer != NULL && 
	            dispatch_Pointer->used == 1 && 
				dispatch_Pointer->magic == 1234)
	{
        dispatch_Pointer->quantum = 0;
        goto done;
	}
	else
	{
	    dispatch_Pointer = (void*) threadList[0];
		dispatch_Pointer->quantum = 0;
        goto done; 
	};
	
	//
	// @todo: Check.
	//
	
	/*
	if( (void*) dispatch_Pointer == NULL )
	{
	    //todo
	};
    */
	
//Done.
done:
	current_thread = (int) dispatch_Pointer->tid;
	return (int) current_thread;
}



/*
 * syscoopDispatcher:
 *  Obs: Esse tipo de dispacher ainda n�o foi habilitado.
 */
 
// #todo
// It's not implemented.

int syscoopDispatcher (void)
{
	struct thread_d *dispatch_Pointer;
	
    dispatch_Pointer = (void*) queue_get_data( queue, QUEUE_SYSCOOP);
	
	if( (void*) dispatch_Pointer != NULL && 
	            dispatch_Pointer->used == 1 && 
				dispatch_Pointer->magic == 1234)
	{
        dispatch_Pointer->waitingCount++;	
		if(dispatch_Pointer->waitingCount > dispatch_Pointer->waiting_limit)
		{
		    dispatch_Pointer->waitingCount = 0;
			current_thread = (int) dispatch_Pointer->tid;
		    return (int) dispatch_Pointer->tid;
        }	
		
	}
	else
	{
	    current_thread = 0;
	    return (int) current_thread;
	};
	
fail:
    //todo: hang
    return (int) current_thread;
}


/*
 * usercoopDispatcher:
 *  Obs: Esse tipo de dispacher ainda n�o foi habilitado.
 */
 
// #todo
// It's not implemented.

int usercoopDispatcher (void)
{
	struct thread_d *dispatch_Pointer;
	
    dispatch_Pointer = (void*) queue_get_data( queue, QUEUE_USERCOOP);
	
	if( (void*) dispatch_Pointer != NULL && 
	            dispatch_Pointer->used == 1 && 
				dispatch_Pointer->magic == 1234)
	{
        dispatch_Pointer->waitingCount++;	
		if(dispatch_Pointer->waitingCount > dispatch_Pointer->waiting_limit)
		{
		    dispatch_Pointer->waitingCount = 0;
			current_thread = dispatch_Pointer->tid;
		    return (int) dispatch_Pointer->tid;
        }	
		
	}
	else
	{
	    current_thread = 0;
	    return (int) current_thread;
	};

fail:
    //todo: hang
    return (int) current_thread;
}


/*
 * sysconcDispatcher:
 *  Obs: Esse tipo de dispacher ainda n�o foi habilitado.
 */
 
// #todo
// It's not implemented.

int sysconcDispatcher (void)
{
	struct thread_d *dispatch_Pointer;
	
    dispatch_Pointer = (void*) queue_get_data( queue, QUEUE_SYSCONC);
	
	if( (void*) dispatch_Pointer != NULL && 
	            dispatch_Pointer->used == 1 && 
				dispatch_Pointer->magic == 1234)
	{
        dispatch_Pointer->waitingCount++;	
		if(dispatch_Pointer->waitingCount > dispatch_Pointer->waiting_limit)
		{
		    dispatch_Pointer->waitingCount = 0;
			current_thread = dispatch_Pointer->tid;
		    return (int) dispatch_Pointer->tid;
        }	
		
	}
	else
	{
	    current_thread = 0;
	    return (int) current_thread;
	};

fail:
    //todo: hang
    return (int) current_thread;
}


/*
 * userconcDispatcher:
 *  Obs: Esse tipo de dispacher ainda n�o foi habilitado.
 */

// #todo
// It's not implemented.

int userconcDispatcher (void)
{
	struct thread_d *dispatch_Pointer;
	
    dispatch_Pointer = (void*) queue_get_data( queue, QUEUE_USERCONC);
	
	if( (void*) dispatch_Pointer != NULL && 
	            dispatch_Pointer->used == 1 && 
				dispatch_Pointer->magic == 1234)
	{
        dispatch_Pointer->waitingCount++;	
		if(dispatch_Pointer->waitingCount > dispatch_Pointer->waiting_limit)
		{
		    dispatch_Pointer->waitingCount = 0;
			current_thread = dispatch_Pointer->tid;
		    return (int) dispatch_Pointer->tid;
        }	
		
	}
	else
	{
	    current_thread = 0;
	    return (int) current_thread;
	};
	
fail:
    //todo: hang
    return (int) current_thread;
}


/*
 ******************************************************
 * systemDispatcher:
 *     System dispatcher.
 *     ?? Penso que isso seja uma interface para 
 * chamar o dispacher. 
 */

// #todo
// It's not implemented.

// # suspensa #
int systemDispatcher (void)
{ 
    return 0; 
}


/*
 *****************************************************
 * idleDispatcher:
 *     Despachar a thread idle atual.
 *     current_idle_thread.
 *         
 * Obs:
 *     A tarefa idle pode ter qualquer id.
 *     Devemos despachar a idle quando o sistema estiver 
 * ocioso ou quanto a thread idle � a �nica thread.
 *     Se o kernel detectar quen n�o h� mas nenhuma thread 
 * no sistema ent�o o kernel deve selecionar uma nova 
 * idle atual e despacha-la. 
 */
 
// #todo
// It's not implemented.

// ## suspensa ##

int idleDispatcher (void)
{ 
    //current_idle_thread

    return 0; 
}


/*
 * periodicDispatcher:
 *     Periodic dispatcher.
 *     Obs: Esse tipo de dispacher ainda n�o foi habilitado.
 */

// #todo
// It's not implemented.

int periodicDispatcher (void)
{ 
    // struct thread_d *New;
   
    return 0; 
}


/*
 * rrDispatcher:
 *     Round robin dispatcher.
 *     Obs: Esse tipo de dispacher ainda n�o foi habilitado.
 */
 
// #todo
// It's not implemented.

int rrDispatcher (void)
{ 
    return 0; 
}


/*
 * realtimeDispatcher:
 *     ## bugbug, na verdade ainda estou aprendendo sobre isso. :) sorry.
 *     Real time dispatcher.
 *     Pega uma tarefa na fila de tarefas com prioridade e tipo realtime.
 *
 * * IMPORTANTE:
 *   Normalmente essas tarefas ficam confinadas em um processador, destinado 
 * pra tarefas de tempo real.
 * Obs:
 *     Na verdade n�o � t�o real time assim.
 *     Obs: Esse tipo de dispacher ainda n�o foi habilitado.
 */

// #todo
// It's not implemented.

int realtimeDispatcher (void)
{
	
/*
 	
    struct thread_d *New;
	  
    //Real time status ~ Round robin.     
    if(realtimeStatus == 1)
    { 
        New = (void*) queue_get_data( queue, QUEUE_REALTIME);
	    if( (void*) New != NULL && 
	                New->used == 1 && 
				    New->magic == 1234 && 
				    New->type == TYPE_REALTIME &&
				    New->priority == PRIORITY_MAX )
	    {   
	        return (int) New->tid;
	    };  
    }; 
	
//
// Fail, use tID 0.
//

*/

fail:
    return 0; //idle.
}


/*
 * dispatch_Default:
 *  Obs: Esse tipo de dispacher ainda n�o foi habilitado.
 */
 
// #todo
// It's not implemented.

int dispatch_Default (void){
	
	struct thread_d *New;
	struct thread_d *Current;
	
	Current = NULL;
	
	
	/*
	 * Fase1 - Tenta tarefa de quantum vencido.
	 */
	int qNext;
    qNext = (int) check_quantum();
    if(qNext != 0)
	{
	    New = (void *) threadList[qNext];
	    
		// todo: check structure.
		
		New->Next = (void*) Current;
		New->quantum = 0;
	    
		return (int) qNext;
    };		
	

	
	/*
	 * Fase 2: 
	 *     Preempt, gethigher priority, no return.
	 *     + Coloca em preemp��o a tarefa atual se ela for de baixa prioridade.
	 *     + Pega uma tarefa de prioridade alta nos slots e torna ela READY,
	 *       colocando ela no fim da fila de tarefas READY. 
	 *     + N�o retorna.
	 */
	
	
	/*
	 * Checar se a tarefa atual pode entrar em preemp��o. 
	 * Se ela n�o pode entrar em premmp��o n�o faz nada. 
	 * ent�o continua no estado READY, mas vai para o fim da fila das 
	 * tarefas ready. 
	 * obs: a tarefa tem que estar no estado ready.
	 */
	
	Current = (void *) threadList[current_thread]; 
	
	if ( Current->preempted == PREEMPTABLE && 
	     Current->priority == PRIORITY_LOW )
	{
	    //preempt ..fifo
	    //S� pode entrar se o estado � ready.
        if( Current->state == READY)
		{
		    
			//manda para o fim da fila de ready.
		    //Current->event = EVENT_PREMMPTED; //Motivo.   
			
			Current->waitingCount = 0;             //Zera o tempo de espera.
			queue_insert_data(queue, (unsigned long) Current, QUEUE_SYSCOOP);
		};
	};
	

	/*
	 * Pega nos slots uma tarefa de prioridade mais alta.
	 * Executa imediatamente se ela for ready. 
	 * mas antes coloca no fim da fila uma de menor prioridade.
	 * e coloca no fim da fila de ready.
	 */
	int nextId; 
	nextId = find_higher_priority();
	if( nextId > 0 )
	{
	    New = (void *) threadList[nextId]; 
		if( (void*) New == NULL )
		{
		    printf("scheduler fail: findhigerpriority return null");
			die();
		}
		//inicio da fila, lifo
		if( New->state == READY)
		{
		    New->waitingCount = New->waiting_limit;    //Tempo limite de espera.
			if( New->tid != 0)
			{
			    New->quantum = 0;
				New->Next = (void*) Current;
			    return (int) New->tid;
			};
		};       		
	};
	
	
	
	/*
	 * Fase 3: Pega a pr�xima se houver.
	 */
	
	//se existe uma proxima tarefa. pega ela.
	//se ela estiver no estado ready o scheduler retorna.
	//se estiver no estado initialized, da spawn.
	if( (void*) Current->Next != NULL )
    {
	    New = (void*) Current->Next;
        Current->Next = NULL; //zera o indicador.
		
		if(New->state == READY)
		{
		    New->waitingCount = 0;
			if(New->tid != 0)
			{
			    New->quantum = 0;
			    New->Next = (void *) Current;
				return (int) New->tid;
            };				
		};	
    };	
	
	 
	/*
	 * Fase 4: Pega a idle.
	 */ 
	 
	New = (void *) threadList[0];
	
	if ( (void *) New != NULL )
	{
	    New->quantum = 0;
		New->Next = (void *) Current;
		
		return (int) New->tid;
	};
}



/*
 * dispatch_thread2:
 *     Restaura o contexto e retorna.
 *     #bugbug. Nem sei se essa rotina est� em uso.
 */
 
void dispatch_thread2 (void){
	
    struct thread_d *t;  
	
	//
	// *RESTORE CONTEXT. (current_thread)
	//
	
	restore_current_context();
	
	//
	// Struct.
	//
	
	t = (void *) threadList[current_thread]; 
	
	if ( (void *) t == NULL )
	{
		//deveria ser a current_idle_thread
	    current_thread = (int) 0;  //current_idle_thread
	    return;
		
	}else{
	    t->saved = 0;         //N�o est� mais salvo o contexto.
	    t->state = RUNNING;   //Movimento 2 ou 4. ??
	};
}



/*
 *****************************************************
 * dispatch_thread:
 *     #bugbug. Nem sei se essa rotina est� em uso.
 */
 
void dispatch_thread (struct thread_d *thread){

    int Status = 0;

	//
	// Structure.
	//

    if ( (void *) thread == NULL )
    {
        printf ("dispatch-dispatch_thread: thread tid={%d}", current_thread ); 
        die ();

    }else{

	    // Context.
		// #bugbug: N�o estamos mais usando esse filtro
        // que seleciona apenas threads em ring 3.		
	    //Status = contextCheckThreadRing3Context(thread->tid);
	    //if(Status == 1){
	    //    printf("pc-dispatch-dispatch_task: contextCheckThreadRing3Context\n");
		//    die();
	    //};
	    //...
	};

	
	/*
	 * State:
	 *     Dispacha de acordo com o status.
	 *     +Spawn no caso de INITIALIZED.
	 */
	 
 	switch (thread->state)
	{
	    //Se vai rodar pela primeira vez
		case INITIALIZED:
		    thread->saved = 0;
			KiSpawnTask(current_thread);
			//spawn_task(current_thread);
	        break;
		//Continua ?? ...	
		
        //Nothing for now.
		default:
            printf("dispatch_thread fail: State!\n");
		    break;
	};


fail:
    printf ("dispatch-dispatch_thread: fail");
    die ();
}



/*
 *************************************
 * init_dispatcher:
 *     inicializa o dispacher.
 *
 * dispatcherReadyList
 *     Esta � a lista do dispatcher.
 *
 * *IMPORTANTE:
 *  As threads aqui est�o no estado READY e ordenadas 
 * por prioridade.
 *     
 *  +A �ltima thread da lista � a thread de maior prioridade.
 *  +Cada elemento dessa lista � o elemento que est� na HEAD 
 * de uma lista
 *  +Cada uma dessas lista � uma lista para uma prioridade 
 * espec�fica.
 *  +Apenas a head de cada uma das listas � colocada aqui 
 * nessa lista do dispacher.
 *
 * OBS: 
 * #bugbug
 * Nesse momento, essa lista do dispacher n�o est� em uso.  
 * O kernel est� usando a lista de threads criadas, 
 * threadList[], n�o considerando a prioridade. :) sorry.
 *
 * @todo: Mudar o nome para dispachInit();.
 */ 
 
int init_dispatcher (void){

    int i;

	//
	// Para um dispatcher na forma de array.
	//


//dispatcher_array:
	
	//Index
    dispatcherQueueIndex = (int) 0;

	//Seleciona o tipo de dispatcher.
    dispatcherType = DISPATCHER_SYSTEM;

	//inicializa a fila do dispacher.

    for ( i=0; i <= PRIORITY_MAX; i++ )
    {
        dispatcherReadyList[i] = (unsigned long) 0;
    };


	//Idle.
	dispatcherReadyList[0] = (unsigned long) InitThread;


	//
	// (Desliga realtime dispatcher.) ??
	//
	
	
	//
	// Para um dispatcher na forma de Linked List.
	//

//dispatcher_linked_list:
	
	// Inicializa o indice de condutores.
    conductorIndex = 0;
	
	//inicializa a lista
	//Conductor = (void*) IdleThread;
		
    rootConductor = (void *) kmalloc ( sizeof(struct thread_d) );
	
    if ( (void *) rootConductor == NULL )
    {
        panic ("init_dispatcher: rootConductor");
    }

	// Usado para task switch.
    Conductor = (void *) rootConductor;

	// #bugbug 
	// Dever�amos iniciar com a idle thread e n�o com a thread 0.

	//Inicia a lista.

    Conductor2 = (void *) rootConductor;
    Conductor2->Next = (void *) threadList[0]; 


    return 0;
}



/*
 ************************************************
 * IncrementDispatcherCount:
 *     Mensura os crit�rios de escolha.
 *     Contagem por crit�rio de sele��o.
 *     Faz uma contagem de quantas vezes o dispatcher 
 * fez uso de cada crit�rio.
 * Obs: Esse modelo apresentou bons resultados por muito 
 * tempo. Vamos preserva-lo. 
 */

	// #todo
	// Create error messages.

void IncrementDispatcherCount ( int type ){

	// Testing struct.

    if ( (void *) DispatchCountBlock == NULL )
    {
        return;
    }

	// Limits.

    if (type < 0 || type > 10)
    {
        return;
    }


	// Activating the selected type.

    switch (type)
    {
        case SELECT_IDLE_COUNT:
            DispatchCountBlock->SelectIdleCount++;
            break;

        case SELECT_INITIALIZED_COUNT:
            DispatchCountBlock->SelectInitializedCount++;
            break;

        case SELECT_NEXT_COUNT:
            DispatchCountBlock->SelectNextCount++;
            break;

        case SELECT_CURRENT_COUNT:
            DispatchCountBlock->SelectCurrentCount++;
            break;

       case SELECT_ANY_COUNT:
            DispatchCountBlock->SelectAnyCount++;
            break;

       case SELECT_IDEAL_COUNT:
            DispatchCountBlock->SelectIdealCount++;
            break;

       case SELECT_DISPATCHER_COUNT:
            DispatchCountBlock->SelectDispatcherQueueCount++;
            break;

		//...

       // Nothing.
       // Aqui poderia ter um contador de indefini��es.
       default:
           break;
    };

	// Nothing.
	//     #bugbug
	//     Obs: @todo: O la�o acima pode n�o selecionar nada.

}


/*
   Constructor.
int dispatchDispatch()
{};
*/


/*
int dispatchInit()
{};
*/


//
// End.
//

