/*
 * File: timer.c 
 *  
 * Descri��o:
 *     Arquivo principal do driver do PIT. 
 *     Trara a irq0.
 *     Rotinas envolvendo tempo.
 *
 *  Ambiente: 
 *     (RING 0).
 *      M�dulo interno, dentro do kernenel base.
 *
 * @todo: Criar fun��es que criam a estrutura timer ...
 * que ser� usada peloa aplicativos na forma de objeto .
 * elas devem ter um contador que enviar� mensagens para o 
 * aplicativo sempre que se esgota a contagem.
 *
 * Hist�rico:
 *     Vers�o: 1.0, 2013 - Esse arquivo foi criado por Fred Nora.
 *     Vers�o: 1.0, 2014 - Revis�o. 
 *     Vers�o: 1.0, 2015 - Aprimoramento geral das rotinas b�sicas.
 *     Vers�o: 1.0, 2016 - Revis�o.
 *     ...
 */

 
/*
I/O port     Usage
0x40         Channel 0 data port (read/write)
0x41         Channel 1 data port (read/write)
0x42         Channel 2 data port (read/write)
0x43         Mode/Command register (write only, a read is ignored)

Bits         Usage
 6 and 7      Select channel :
                 0 0 = Channel 0
                 0 1 = Channel 1
                 1 0 = Channel 2
                 1 1 = Read-back command (8254 only)
 4 and 5      Access mode :
                 0 0 = Latch count value command
                 0 1 = Access mode: lobyte only
                 1 0 = Access mode: hibyte only
                 1 1 = Access mode: lobyte/hibyte
 1 to 3       Operating mode :
                 0 0 0 = Mode 0 (interrupt on terminal count)
                 0 0 1 = Mode 1 (hardware re-triggerable one-shot)
                 0 1 0 = Mode 2 (rate generator)
                 0 1 1 = Mode 3 (square wave generator)
                 1 0 0 = Mode 4 (software triggered strobe)
                 1 0 1 = Mode 5 (hardware triggered strobe)
                 1 1 0 = Mode 2 (rate generator, same as 010b)
                 1 1 1 = Mode 3 (square wave generator, same as 011b)
 0            BCD/Binary mode: 0 = 16-bit binary, 1 = four-digit BCD
*/ 
 
 
 
/*
 PIT info:
 ========
 PIT: mode=2 count=0x48d3 (18643) - 64.00 Hz (ch=0)
 PIT: mode=3 count=0x10000 (65536) - 18.20 Hz (ch=0) (Oracle, Virtualbox).
 PIT: mode=3 count=0x2e9a (11930) - 100.01 Hz (ch=0) (Oracle, Virtualbox).
 PIT: mode=3 count=0x2e9b (11931) - 100.00 Hz (ch=0) (Oracle, Virtualbox).
 */ 
 
#include <kernel.h>



//
// Vari�veis internas.
//

//Status do m�dulo.
int timerStatus;

//Contador de ticks.
//unsigned long timerTicks;

//??  
int timerColor;

//??
unsigned long timerLine;

//??
unsigned long timerIdleState;

//??
int timerLock;

//??
int timerError;



int timerShowTextCursor;     //se t� habilitado ou n�o
int timerTextCursorStatus;   //0=apaga 1=acende 

//??
//unsigned long timerCountSeconds;  //Count Seconds.
//...


//
// Fun��es internas.
//


//Rotina principal.
void timer (void);        
//...


/*
 * KiTimer:
 *     Chama o handler do kernel que est� no kernel base.
 * #todo: Observar alguns procedimentos antes de chamar a rotina.
 */
 
void KiTimer (void){
	
	// Se o timer n�o estiver inicializado !
    if ( __breaker_timer_initialized == 0 )
        return;


	//
	// Profiler
	//
	
	// Contando as interrup��es desse tipo.
	g_profiler_ints_irq0++;
	
	
	//#debug
	//vamos checar se o primeiro iret est� trazendo
	//o fluxo para c�.
	//quando n�o houver falha vai fazer uma bagun�a...
	//mas no caso de falha vai ficar limpo.
	//tamb�m podemos usar o debug via porta serial.
	//kprintf ("#debug KiTimer ");
	//refresh_screen();
	//refresh_screen();
	//while(1){}
	
	timer ();
}



/*
 *****************************************************
 * timer: 
 *     Handler chamado pelo ISR do timer (IRQ 0).
 *     (Contagem de tempo, tempo das tarefas, 
 * quantum ...).
 *     (tick tick tick)
 */

void timer (void){

    // Timers.
    int i = 0;

    struct timer_d *t;



    unsigned long __cWidth = gwsGetCurrentFontCharWidth();
    unsigned long __cHeight = gwsGetCurrentFontCharHeight();

    if ( __cWidth == 0  ||  __cHeight == 0 ){
        panic ("timer: char size\n");
    }






    //
    // Jiffies.
    //

    // Contador de ticks.
    // Incrementa o Tempo total de funcionamento do kernel.

    jiffies++;




	//
	// ## sys time ##
	//
	
	//Mais 10ms para um sistema funcionando a 100HZ
    //sys_time_ms = sys_time_ms + 10;
	
	sys_time_ms = (unsigned long) sys_time_ms + (1000/sys_time_hz);

	
	//
	// ## threads time ##
	//

	//Atribui um n�mero de request a ser atendido futuramente.
	//Tratar� o tempo das tarefas de acordo com o tipo.
	//#obs: Isso poderia ser usado para atualizar o time dos processos.
	
	//#suspenso
	//estou repensando isso.
	
	//kernel_request = KR_TIME;  
	
	
	//
	// Working set support.
	//
    
    // #todo
    // As vari�veis de working set est�o em timer.h
	

	//Working set and profiler support.
	
	//apenas incremente.
	thread_profiler (1);
	    
    if ( jiffies % profiler_ticks_limit == 0 )
	{   
	    //quantidade de frames num determinado per�odo de tempo.
	    // mm_profiler (); 
	    
	    //calcule.
	    thread_profiler (2);
	    //process_profiler (?);    
	}


    // Whatch dogs
    if ( jiffies % 100 == 0 )
    {
       // Incrementa o tempo em que o usu�rio est� sem digitar.
        ____whatchdog_ps2_keyboard++;
        
        // Incrementa o tempo em que o usu�rio est� sem usar o mouse. 
        ____whatchdog_ps2_mouse++;
        
        
        if ( ____whatchdog_ps2_keyboard > 100 )
        {
            ____whatchdog_ps2_keyboard = 0;
            //printf ("whatchdog timer: keyboard!\n");
            //mostra_slot (current_thread);
            //mostra_reg (current_thread);
            //refresh_screen();

            // Isso n�o surtiu efeito na m�quina real.
            // printf ("whatchdog timer: Reinitializing ps2 controller ...\n");
            // refresh_screen();
            // ps2 ();

            // mostra os registrador da thread, em sua fase ring0.
            //asm ("int $3 \n");
        }

        if ( ____whatchdog_ps2_mouse > 200 )
        {
            ____whatchdog_ps2_mouse = 0;
            //printf ("whatchdog timer: mouse!\n");
            //refresh_screen();        
        }
        
    }



	//
	//  ## extra ## 
	//
	
	// Aciona uma flag para que o ts.c realize atividades extras,
	// como gc, dead thread collector, request.

    // #todo
    // Podemos fazer isso com menos frequ�ncia.

    if ( jiffies % 100 == 0 )
    {
        extra = 1;
    }


	//
	// ## mouse blink ##
	//
	
	//#todo:
	//podemos chamar uma fun��o
	
	//#todo rever isso.
	//de tempos em tempos atualiza o cursor
	//a cada segundo. sendo ele 100 ou 1000 ... tanto faz.
	//#todo: mas poderia ser exatamente o hz configurado par ao mouse
	//if ( jiffies % mouse_cursor_hz == 0 )
	//if ( jiffies % sys_time_hz == 0 )	
    if ( jiffies % 70 == 0 )
    {
        // Se o cursor piscante est� habilitado.
        // Essa flag � acionada pelo aplicativo.

        if (timerShowTextCursor == 1)
        {
            // Apaga.
            if ( timerTextCursorStatus != 1 )
            { 
                refresh_rectangle ( (TTY[current_vc].cursor_x + 1)  * __cWidth, 
                    TTY[current_vc].cursor_y * __cHeight, 
                    16, 16 );

                timerTextCursorStatus = 1;
                goto mouseExit;
            }

            // Acende.
            if ( timerTextCursorStatus == 1 )
            {
                bmpDisplayCursorBMP ( cursorIconBuffer, 
                    (TTY[current_vc].cursor_x + 1) * __cWidth, 
                    TTY[current_vc].cursor_y * __cHeight );
         
                timerTextCursorStatus = 0;
                goto mouseExit;
            }
        };
    };



mouseExit:

    // Lista de timers.
    // Podemos percorrer a lista de timer e decrementar,
    // quando um timer chegar a 0, mandamos uma mensagem
    // para a thread associada � esse timer..


    for ( i=0; i<32; i++ )
    {
		//pega um da lista
	    t = (struct timer_d *) timerList[i];

        //checa
        if ( (void *) t != NULL )
        {
			//valida��o.
			if ( t->used == 1 && t->magic == 1234 )
			{
				if ( t->count_down > 0 )
				{
				    t->count_down--;
                    
                    //Chegamos ao 0.
                    if ( t->count_down == 0 )
                    {
						//enviamos a mensagem para a thread que � dona do timer.
						//#todo: podemos usar uma fun��o para isso.
						
						//se a thread for v�lida.
						if ( (void *) t->thread != NULL )
						{
							if ( t->thread->used == 1 && t->thread->magic == 1234 )
							{
								
		                        t->thread->window = t->window;
		                        t->thread->msg = (int) MSG_TIMER;    
		                        t->thread->long1 = t->times;   //quantas vezes esse timer se esgotou.      
		                        t->thread->long2 = t->status;  //?/status.     
		
		                        t->thread->newmessageFlag = 1;			
								
							}
							
						}
						
						//analisando o tipo.
						//dependendo do tipo, devemos parar ou recome�ar.
						
						//one shot
						if ( t->type == 1 )
						{
						   t->count_down = 0;	
						}
						
						//intermitente
						if ( t->type == 2 )
						{
						   t->count_down = t->initial_count_down;	
						}
						
                        //...						
					}						
				}
			    
			};
		};			
		
		//nothing
	};
	
	

done:
    	
	//#todo
	//if(time_out > 0){
	//	time_out--;
	//};
	
	return;
}


void timerEnableTextCursor (void)
{
    timerShowTextCursor = 1;
}


void timerDisableTextCursor (void)
{
    timerShowTextCursor = 0;
}


int new_timer_id (void){
	
    int i;
    unsigned long new;
	
    for ( i=0; i<32; i++ )
    {
		new = (unsigned long) timerList[i];
		
		if ( new == 0 )
		{
			return (int) i;
		}
	}		
	
	//fail
	return (int) -1;
}


//#todo 
//precisamos pegar um slot na lista de timers.

struct timer_d *create_timer ( struct window_d *window, 
                               unsigned long ms, 
                               int type  )
{
    struct timer_d *t;
    
    int ID = -1;  //erro;



	//limits
	//limite de 1 tick.
	
	if (ms < (1000/sys_time_hz) )
	{
		printf("create_timer: ajust ms\n");
		ms = (1000/sys_time_hz);
	}
	
	if ( type < 1 || type > 10 )
	{
		panic ("create_timer: type fail\n");
		
		///printf("create_timer: type fail\n");
		//refresh_screen ();
		//return NULL;
	}

	
	t = (void *) kmalloc ( sizeof(struct timer_d) );
	
	if ( (void *) t == NULL )
	{
		panic ("create_timer: t fail \n");
		
		//printf ("create_timer: t fail \n");
		//refresh_screen ();
		//return NULL; 

	}else{
		
		ID = (int) new_timer_id ();
		
		//erro ao obter um novo id.
		if (  ID < 0 || ID > 32 )
		{
		    panic ("create_timer: ID fail \n");
		    
		    //printf("create_timer: ID fail \n");
		    //refresh_screen ();
		    //return NULL;
		}else{
			
			t->id = ID; 	
		
		    t->used = 1;
		    t->magic = 1234;
		
		    // ms/(ms por tick)
		    t->initial_count_down = (unsigned long) ( ms / (1000/sys_time_hz) );
	 	    t->count_down = t->initial_count_down;
		
	        //1 = one shot 
	        //2 = intermitent
		    t->type = (int) type;
		
		    //colca na lista.
		    timerList[ID] = (unsigned long) t;
		};
		
		
	    //thread.
		
		if ( (void *) window == NULL)
		{
		    printf("create_timer: window fail \n");
		    refresh_screen ();
		    return NULL;
		}else{
			
			if ( window->used != 1 || window->magic != 1234 )
			{
				
		        printf("create_timer: window validation fail \n");
		        refresh_screen();
			    return NULL;
			}
			
			//temos uma janela v�lida 
			t->window = window;
			
			if ( (void *) window->control == NULL )
			{
		        printf("create_timer: thread fail \n");
		        refresh_screen();
				return NULL;
			}
			
			if ( window->control->used != 1 || 
			     window->control->magic != 1234 )
			{

		        printf("create_timer: thread validation fail \n");
		        refresh_screen ();
			    return NULL;
			}
			
			//#importante 
			//agora o timer tem uma thread para enviar mensagens.
			//quancdo o tempo se esgotar.
			
		    t->thread = (struct thread_d *) window->control;
		};
	};


	//#debug
    //printf("create_timer: done \n");
    //refresh_screen ();


    return (struct timer_d *) t;
}


/*
 ******************************************
 * timerInit8253:
 *    @todo: Compreender melhor isso.
 *
 * Seta a frequ�ncia de funcionamento do 
 * controlador 8253. "3579545 / 3" 
 * instead of 1193182 Hz. 
 * Pois o valor � mais preciso, considera at� os 
 * quebrados. 
 * Reprograma o timer 8253 para operar 
 * � uma frequencia de "HZ".
 * Obs: Essa rotina substitui a rotina init_8253.
 */
 
//#importante 
//Essa rotina poder� ser chamada de user mode,
//talvez precisaremos de mais argumentos. 
 
void timerInit8253 ( unsigned long hz ){
	
	//#todo:
	//podemos fazer filtros.
	
	unsigned short clocks_per_sec = (unsigned short) hz;
	
	unsigned short period =  ( (3579545L/3) / clocks_per_sec );
	
	
	// Canal 0, LSB/MSB, modo 3, contar em bin�rio.
	outportb ( 0x43, 0x36 );			  
	
	// LSB.
	outportb ( 0x40, period & 0xFF ); 
	//outportb ( 0x40, 0xA9 );  //test 1000
	
	
	// MSB.
	outportb ( 0x40, period >> 8 );	 
	//outportb ( 0x40, 0x04 );   // test 1000
	
	//#BUGBUG N�o faremos isso aqui,
	//faremos quando ermos spawn da idle thread.
	//irq_enable(0x00); // Timer
	
	// #importante
	// Isso ser� uma vari�vel para fazermos testes de desempenho. 
	
	sys_time_hz = (unsigned long) hz;
}


/* set_quantum: */
void set_quantum ( unsigned long q){
	
    quantum = (unsigned long) q;
}


/* get_quantum: */
unsigned long get_quantum (void){
	
    return (unsigned long ) quantum;
}


/* set_current_quantum: */
void set_current_quantum (unsigned long q){
	
    current_quantum = (unsigned long) q;
}


/* get_current_quantum: */
unsigned long get_current_quantum (void){
	
    return (unsigned long ) current_quantum;
}


/* set_next_quantum: */
void set_next_quantum (unsigned long q){
	
    next_quantum = (unsigned long) q;
}


/* get_next_quantum: */
unsigned long get_next_quantum (void){
	
    return (unsigned long ) next_quantum;
}


/* systime in ms */
unsigned long now (void){
	
    return (unsigned long) get_systime_ms ();
}


void set_systime_hz ( unsigned long hz )
{
    sys_time_hz = hz;
}

/* systime hz */
unsigned long get_systime_hz (void){
	
    return (unsigned long) sys_time_hz;
}


/* systime in ms */
unsigned long get_systime_ms (void){
	
    return (unsigned long) sys_time_ms;
}


/* get_systime_totalticks: */
unsigned long get_systime_totalticks (void)
{
    return (unsigned long) jiffies;
}


//#todo
/*
unsigned long get_jiffies (void);
unsigned long get_jiffies (void)
{
    return (unsigned long) jiffies;
}
*/

/*
 ***********************
 * get_systime_info:
 *     
 */
unsigned long get_systime_info (int n){
	
	//#todo criar um enum para isso.
	
    switch (n){
		
		case 1:
		    return (unsigned long) get_systime_hz ();
            break;
			
		case 2:
		    return (unsigned long) get_systime_ms ();
            break;
			
		case 3:
		    return (unsigned long) get_systime_totalticks ();
			break;
		
		//...
		
		default:
		    return (unsigned long) 0;
		    break;
	};	
}


/*
 ***************************************
 * sleep: #todo
 *     Apenas uma espera, um delay.
 *     Essa n�o � a fun��o que coloca uma 
 * tarefa pra dormir no caso de evento.
 *   #todo: Usar o ms do contador do sys_time
 */

void sleep (unsigned long ms){
	
    unsigned long t = (unsigned long) ( ms * 512 );
	
	while(t > 0){
		t--;
	}
}


/* set_timeout: #todo */
void set_timeout ( unsigned long ticks ){

	time_out = (unsigned long) ticks;
}


/* get_timeout: #todo */
unsigned long get_timeout (void){
	
	return (unsigned long) time_out;
}


/*
 **************************************
 * timerTimer: 
 *     Constructor.
 *     Inciaializa��o de vari�veis do m�dulo.
 */

int timerTimer (void){

    //
    //   Jiffies !!!
    //

    jiffies = 0;


    sys_time_hz = 0;
    sys_time_ms = 0;


    //
    // Profiler
    //
    
    profiler_ticks_count = 0;
    profiler_ticks_limit = PROFILER_TICKS_DEFAULT_LIMIT;
    
    //...

    return 0;
}


/*
 ********************************************
 * timerInit:
 *     Inicializa o driver de timer.
 *     Inicializa as vari�veis do timer.
 *     @todo: KiTimerInit 
 * (unsigned long CallBackExemplo); 
 */

int timerInit (void){

    __breaker_timer_initialized = 0;

    int i;
	
	//Constructor.
	timerTimer();
	
	
	for ( i=0; i<32; i++ ){
		timerList[i] = (unsigned long) 0;
	}
	
	
    // timerLock = 0;

    //set handler.
   
    //
    // @todo: Habilitar esse configura��o pois � mais precisa.
    //
   
    //config frequ�ncias...
    //@todo: Isso poderia ser por �ltimo.
    //?? Isso pertence a i386 ??
    //?? Quais m�quinas possuem esse controlador ??
    
	// #importante
	// Come�aremos com 100 HZ
	// Mas o timer poder� ser atualizado por chamada.
	// e isso ir� atualizar a vari�vel que inicializamos agora.
	
	sys_time_hz = (unsigned long) HZ;
	timerInit8253 ( sys_time_hz );
   
   
    /*
     * @todo: criar a estrutura do timer.
	 */

	//
	//@todo:
	//    alocar memoria para a estrutura do timer.
	//    inicializar algumas variaveis do timer.
	//    por enquanto estamos usando variaveis globais.
	//    ?? N�o se se ja foi configurado o timer.
	// ou devemos chamr init_8253() agora. ou depois.
	//
	
	

	
	//timerCountSeconds = 0;
	
	//Configura quantum.
	
	set_current_quantum (QUANTUM_BASE);
	
	set_next_quantum (QUANTUM_BASE);
    
	set_quantum (QUANTUM_BASE);

    //timeout 
	 
	set_timeout (0);
	
	
    // Initializing whatchdogs.
    // Eles ser�o zerados pelas interrup��es dos dipositivos e
    // incrementados pelo timer.
    // A condi��o cr�tica � alcan�ar um limite, um timeout.
    ____whatchdog_ps2_keyboard = 0;
    ____whatchdog_ps2_mouse = 0;
    //...


	
	//Continua...
	
    //Done.

    g_driver_timer_initialized = (int) 1;
	
//#ifdef EXECVE_VERBOSE
//    printf("timerInit: Done\n");
//#endif


    __breaker_timer_initialized = 1;
        
    return 0;
}



/*
 * early_timer_init:
 *     Inicializa��o pr�via do m�dulo timer.
 *     Uma outra inicializa��o mais aourada poder� ser feita
 * posteriormente.
 *     Por�m ainda existe uma inicializa��o feita em Assembly
 * quando o kernel � inicialazado.
 * 
 */
 
int early_timer_init (void){

    __breaker_timer_initialized = 0;

    int i;
	
	//Constructor.
	timerTimer();
	
	
	for ( i=0; i<32; i++ ){
		timerList[i] = (unsigned long) 0;
	}


    // 400:
    // 100 deixa a digita��o muito lenta.
	//sys_time_hz = (unsigned long) HZ * 4;

    //set_systime_hz ( HZ * 4 );
    set_systime_hz ( HZ * 6 );

    timerInit8253 ( sys_time_hz );

    set_current_quantum (QUANTUM_BASE);
    
   set_next_quantum (QUANTUM_BASE);
    
	set_quantum (QUANTUM_BASE);

    //timeout 
	 
	set_timeout (0);
	
	
    // Initializing whatchdogs.
    // Eles ser�o zerados pelas interrup��es dos dipositivos e
    // incrementados pelo timer.
    // A condi��o cr�tica � alcan�ar um limite, um timeout.
    ____whatchdog_ps2_keyboard = 0;
    ____whatchdog_ps2_mouse = 0;
    //...


	
	//Continua...
	
    //Done.

    g_driver_timer_initialized = (int) 1;
	
//#ifdef EXECVE_VERBOSE
//    printf("timerInit: Done\n");
//#endif


    __breaker_timer_initialized = 1;
        
	return 0;
}




/*
struct timer_d *timerObject();
struct timer_d *timerObject()
{
	//@todo: criar uma estrutura e retornar o ponteiro.
	return (struct timer_d *) x;
}
*/


//
// End.
//

