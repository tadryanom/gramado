/*
 * File: gws/window.c 
 *
 * Descri��o:
 *     Arquivo principal do Window Manager.
 *     Window Manager.
 *     MB - M�dulos inclu�dos no Kernel Base.
 *     Rotinas envolvendo a feitura de janelas.     
 *     Parte da interface gr�fica oferecida em Kernel Mode.
 *     A interface gr�fica oferecida pelo Kernel Base deve ser leve e veloz.
 *
 * Obs: Muitas funcionalidades devem ser iplementadas em user mode e n�o aqui.
 *      aqui devem ficar as fun��es mais primitivas. e outras pra ganhar desempenho
 *      mas n�o tudo. 
 *
 * @todo: Criar uma fun��o que copie o conte�do de um buffer dedicado de janela para
 * o LFB,(FrontBuffer).
 *       
 * In this file:
 * ============
 * +
 * +
 * +
 *
 * Hist�rico:
 *     Vers�o: 1.0, 2013 - Esse arquivo foi criado por Fred Nora.
 *     Vers�o: 1.0, 2014 - Aprimoramento geral das rotinas b�sicas. 
 *     Vers�o: 1.0, 2015 - Revis�o. 
 *     Vers�o: 1.0, 2016 - Revis�o.
 *     ...
 */

 
#include <kernel.h>
 
//int window_getch_lock;
 
// 
// Argumentos passados pelo Boot Loader em head.s. 
//

extern unsigned long kArg1;    //Video Mode.	
extern unsigned long kArg2;	   //LFB - Linear Frame Buffer.
extern unsigned long kArg3;	   //??.
extern unsigned long kArg4;	   //??.	



/*
 #importante:
 Isso � uma rotina de movimento de janela.
 vamos construir uma fun��o pra isso.
 
 					save_window ( (struct window_d *) windowList[window_with_focus] );
					replace_window ( (struct window_d *) windowList[window_with_focus], 20, 20 );
					show_saved_window ( (struct window_d *) windowList[window_with_focus] );
					show_window_rect ( (struct window_d *) windowList[window_with_focus] );
 */


//salva o ret�ngulo de uma janela no buffer de salvamento.
//isso ser� usado para mover a janela.
int save_window (struct window_d *window){
	
    if( (void *) window == NULL )
    {
		return 1;
	} else {
	
        if ( window->used != 1 || window->magic != 1234 )
        {
			return 1;
		}

		save_rect ( window->left, window->top, window->width, window->height );
        //... 		
		
	};		
};


//mostrar uma janela que tem seu ret�ngulo salvo no buffer de salvamento.
//#obs: antes de chamarmos essa fun��o podemos chamar a fun��o replace_window
//para mudar a janela de lugar.
int show_saved_window (struct window_d *window){
	
    if( (void *) window == NULL )
    {
		return 1;
	} else {
	
        if ( window->used != 1 || window->magic != 1234 )
        {
			return 1;
		}

		show_saved_rect ( window->left, window->top, window->width, window->height );
        //... 		
		
	};		
};



/*
 ***********************************************
 * show_window_rect:
 *     Mostra o ret�ngulo de uma janela que est� no backbuffer.
 *     Tem uma janela no backbuffer e desejamos enviar ela para o frontbuffer.
 *     A rotina de refresh rectangle tem que ter o vsync
 */

int show_window_rect (struct window_d *window){
	
    if ( (void *) window == NULL )
    {
		return (int) 1;
		
	} else {
	
        if ( window->used == 1 || window->magic == 1234 )
        {
			
			//#shadow 
			// ?? E se a janela tiver uma sombra, 
			// ent�o precisamos mostrar a sombra tamb�m. 
			
			//#bugbug
			//Extranhamente essa checagem atraza a pintura da janela.
			//Ou talvez o novo tamanho favore�a o refresh rectangle,
			//ja que tem rotinas diferentes para larguras diferentes
			
			//if ( window->shadowUsed == 1 )
			//{
				
			    window->width = window->width +4;
				window->height = window->height +4;	
			    
				//refresh_rectangle ( window->left, window->top, 
				//    window->width +2, window->height +2 ); 					
				//return (int) 0;	
			//}
			
	        refresh_rectangle ( window->left, window->top, window->width, 
		        window->height ); 		
			
			return (int) 0;
		}		
	};

	//fail.
    return (int) 1;	
}


int windowKillTimer ( struct window_d *window, int id ){
	
    int Status = 0;
	
	if( (void *) window == NULL )
	{
		Status = 1;
		goto fail;
		
	}else{
		
		if ( window->used != 1 || window->magic != 1234 )
		{
			Status = 1;
		    goto fail;
		}
    
	    //...
	};

fail:
//done:	
    return (int) Status;	
};


/*
 *windowSetTimer:
 * 
 */
// A janela receber� uma mensagem MSG_TIMER 
// sempre que o contador chegar a '0'.
// +Essa rotina deve utilizar o driver de timer 
// para conseguir a contagem que precisa.
// #teste, come�aremos contagem simples para teste.

int
windowSetTimer( struct window_d *window, //janela
                int id,                  //id do timer
				int time )               //tempo(medida indefinida ainda)
{
    int Status = 0;
	
	// ## Window  ##
	
	if( (void *) window == NULL )
	{
		Status = 1;
		goto fail;
	}else{
		
		if ( window->used != 1 ||
		     window->magic != 1234 )
		{
			Status = 1;
		    goto fail;
		}
		
		//Ok.
		// Aqui devemos registra na estrutura 
		// da janela o id do timer e o tempo 
        //desejado. 		
		
		//obs: temos um limite para ids.
		//window->timer[id].id = id;
		//window->timer[id].time = time;
		
		//...
	};
	
	
fail:
done:	
    return (int) Status;
};



/*
  test - queue support *******************************
 */
 
 
 /*
 
#define MAX_ITEMS    4*4
typedef struct circularQueue_s
{
    int     first;
    int     last;
    int     validItems;
    
	//int     data[MAX_ITEMS];
	unsigned long data[MAX_ITEMS];
	
} circularQueue_t;


    unsigned long readValue;
    circularQueue_t   myQueue;

void initializeQueue(circularQueue_t *theQueue);
 
int isEmpty(circularQueue_t *theQueue);
 
int putItem(circularQueue_t *theQueue, unsigned long theItemValue);
 
int getItem(circularQueue_t *theQueue, unsigned long *theItemValue);
 
//void printQueue(circularQueue_t *theQueue);

*/

/*
void initializeQueue(circularQueue_t *theQueue)
{
    int i;
    theQueue->validItems  =  0;
    theQueue->first       =  0;
    theQueue->last        =  0;
    for(i=0; i<MAX_ITEMS; i++)
    {
        theQueue->data[i] = (unsigned long) 0;
    }        
    return;
}
*/

/*
int isEmpty(circularQueue_t *theQueue)
{
    if(theQueue->validItems==0)
        return(1);
    else
        return(0);
}
*/

/*
int putItem(circularQueue_t *theQueue, unsigned long theItemValue)
{
    if(theQueue->validItems>=MAX_ITEMS)
    {
        printf("The queue is full\n");
        printf("You cannot add items\n");
        return(-1);
    }
    else
    {
        theQueue->validItems++;
        theQueue->data[theQueue->last] = (unsigned long) theItemValue;
        theQueue->last = (theQueue->last+1)%MAX_ITEMS;
    }
}
*/

/*
int getItem(circularQueue_t *theQueue, unsigned long *theItemValue)
{
    if(isEmpty(theQueue))
    {
        printf("isempty\n");
        return(-1);
    }
    else
    {
        *theItemValue =  theQueue->data[theQueue->first];
        theQueue->first = (theQueue->first+1)%MAX_ITEMS;
        theQueue->validItems--;
        return(0);
    }
}
*/

/*
void printQueue(circularQueue_t *theQueue)
{
    int aux, aux1;
    aux  = theQueue->first;
    aux1 = theQueue->validItems;
    while(aux1>0)
    {
        printf("Element #%d = %d\n", aux, theQueue->data[aux]);
        aux=(aux+1)%MAX_ITEMS;
        aux1--;
    }
    return;
}
 
*/

/*
 **************************************
*/ 


//retorna o id de uma janela.
int windowGetWindowID ( struct window_d *window ){
	
	if ( (void *) window != NULL )
	{
		if ( window->used == 1 && window->magic == 1234 ){
			
		    return (int) window->id; 	
		}
	};

	return (int) -1;
};


// Pegar o id da janela main.
// Para os aplicativos lidarem com a �rea de trabalho. 
int windowGetMainWindowDescriptor (){
	
	if( (void *) gui == NULL ){
		goto fail;
	}else{
		
		if( (void *) gui->main == NULL ){
		    goto fail;
		}
		return (int) gui->main->id;
	};
	
fail:	
	return (int) -1;
};


/*
 * windowInitializeBrowserSupport:
 *      (KSOC - Kernel Standard Output Container)
 *       gerenciador de ambientes de janelas ... como no caso de window station/desktop/window...
 * s� que com abas.
 *
 *     Inicializa��o do sistema de suporte ao navegador shell.
 * *Importante: Essa rotina ser� bem longa..
 * devemos inicializar todos os recursos que ser�o usados pelo navagador ...
 * O kernel deve criar uma janela uma janela maximizada que ser� o navegador ...
 * deve criar 12 abas que ser�o atendidas atrav�s do procedimento de janelas do sistema.
 * 
 * *Importante: essa rotina deve ser chamada antes de rodar qualquer processo em user mode.
 * portanteo um bom lugar pra essa rotina ser chamada � no final de main.c, antes de dar iretd
 * para a thread idle.
 *
 */

//#obs: podemos suspender isso. 
 
int windowInitializeBrowserSupport (){
	
/*

    //suspensa	
	
	int i;
	
	//#debug
	//printf("Initializing browser support\n");
	
	for (i=0; i< TABWINDOW_COUNT_MAX; i++){
		browsertabList[i] = (unsigned long) 0;
	}
	
	BROWSERWINDOW = NULL;
	TABWINDOW = NULL;
	current_tab = 0;
	
	
	//
	// @todo: Inicializar todos os recursos que o navegador vai precisar.
	//
	
	//Criando a janela do navegador ... isso requer uma ponteiro exclusivo
	//Esse � a janela com moldura onde as abas ser�o criadas.
	//Essas abas ser�o janelas filhas da janela do navegador KSOC.
	
	struct window_d *w;
	
    //KSOC - Kernel Standard Output Container
    w = (void *) CreateWindow( 3, 0, 0,"KSOC", 
			        0, 0, 800, 600, 
					gui->screen, 0, COLOR_WINDOW, 0xC0C0C0 );

    if( (void *) w == NULL )
	{
	    printf("windowInitializeBrowserSupport:");
		die();
	}else{
		
		if( w->used != 1 || w->magic != 1234 ){
	        printf("windowInitializeBrowserSupport: fail");
		    die();
		}
		
		RegisterWindow(w);
        set_active_window(w);
		SetFocus(w);

        //Ponteiro exclusivo.
        BROWSERWINDOW = (void *) w;		
	};


	// Criando as estruturas para as 12 abas do navegador.
	// Criaremos apenas 4 abas ... rotinas de erro perceber�o a as faltantes
	// cada aba tem uma estrutura de aba, um ponteiro par a asua janela dentro da estrutura.
	
	struct browser_tab_d *bt; //browser tabs.
	
	
	// Importante: 
	//   Come�aremos de 1 pois a aba 0 � a janela gui->screen 
	
	for ( i=1; i<4; i++ )
	{
		bt = (void *) malloc( sizeof(struct browser_tab_d) );
	    
		if ( (void *) bt == NULL )
		{
	        printf("erro tab struct");
		    die();
	    }		
		
		bt->used = 1;
		bt->magic = 1234;
		bt->type = 1;  //do mesmo tipo por enquanto.
		
		bt->name = "TAB NAME";
		bt->view = 1;
		
		//As abas s�o janelas do tipo 1, pop up simples.
		//os tamanhos ser�o revistos.
		//brancas � princ�pio.
		//
		// Importante: Pintar todas essas janelas sobrepostas demanda tempo 
        // podemos criar janelas minimizadas, que n�o precisam pintar. 		
		
        w = (void *) CreateWindow ( 1, 0, 0,"TAB", 
			            0, 24, 800, 600-24, 
						gui->screen, 0, COLOR_WINDOW, COLOR_WINDOW );		
		
	    if( (void *) w == NULL )
		{
	        printf(".");
		    die();
	    }else{   
		    RegisterWindow(w);
		    //...
	    };
	
	    //Ponteiro da janela da aba.
	    bt->window = (void*) w;
		
		bt->next =  NULL;
		
		//salvando o ponteiro para a tab na lista de ponteiros de tab.
		browsertabList[i] = (unsigned long) bt;
	
	};
	

   //
   // Testando uma aba.
   //   
   
    //testando a aba 1.
    bt = (void *) browsertabList[1];
    
	if ( (void *) bt == NULL )
	{
	    printf("~erro tab struct");
		die();
	}	   
   
    if ( bt->used != 1 || bt->magic != 1234 )
	{
	    printf("windowInitializeBrowserSupport: bt fail");
		die();		
	}

	//Determinado que a tab 1 � a tab atual.
	//Obs; Isso n�o � a janela com o foco de entrada.
    current_tab = 1;
	
	//janela da aba. (a aba propriamente dita.
    w = (void *) bt->window;	

    if ( (void *) w == NULL )
	{
	    printf("windowInitializeBrowserSupport:~fail");
		die();
		
	}else{
		
	    if ( w->used != 1 || w->magic != 1234 )
		{
	        printf("windowInitializeBrowserSupport: ~fail");
	        die();			
	    }
		
		//Aqui temos uma janela v�lida. Essa janela � uma aba.
		//vamos escrever nela pra testar.
		
		//Ponteiro para a janela da aba atual.
		//Obs: Rotinas que desejam pintar na aba atual usam esse ponteiro.
        TABWINDOW = (void *) w;

        
		//Test: Criando alguns elementos gr�ficos dentro da aba no navegador.
		draw_text ( TABWINDOW, 0, 0, COLOR_TEXT, "bt: This is a text on the browser tab number 1");	
        
		draw_button ( TABWINDOW, "bt: Button label 11 11", 
		    1, 0, 0, 
			11, 11, 56, 24, COLOR_BUTTONFACE); 
		
		StatusBar ( TABWINDOW, "bt: Status bar, string 1", "bt: Status bar, string 2");
	};

	
   //
   // Deve haver muito o que fazer ainda  ...
   //
   
    printf("done\n");

	*/
	
	return (int) -1;
};


/*
 * @todo: Talvez aque devamos apenas atualizar a flag de view da janela.
int windowShowWindow(struct window_d *window, int msg);
int windowShowWindow(struct window_d *window, int msg)
{
	int Status;
	
	if( (void*) window == NULL ){
		return (int) 1;
	};
	
	
	switch(cmd)
	{
		case MSG_HIDE:
		case MSG_MAXIMIZE:
		case MSG_RESTORE:
		case MSG_SHOWDEFAULT:
		default:
	}
	
	//@todo: Copiar o conteudo do buffer dedicado para o frontbuffer.
	
	//na estrutura de janela:
	//a vari�vel dedicatedbuffer precisa estar correta.
	//a vari�vel frontbuffer precisa estar correta.
	
	return (int) Status;
}
*/



/*
 * windowSetParent:
 * Atribui uma nova janela mae � uma dada janela filha.
 */
/*
void *windowSetParent(struct window_t *clild, struct window_d *new_parent);
void *windowSetParent(struct window_t *clild, struct window_d *new_parent)
{
	//@implementar.
	//Obs: Se o par�metro new_parent for NULL ent�o a janela principal (main ou desktop) 
	//ser� a nova janela mae.
	return NULL;
}
*/

/*
 * windowIsWindow:
 *     Testa atrav�s do handle passado, se � uma janela ou n�o.
 *     0 = Yes; 1 = Not; 
 */
/* 
int windowIsWindow(struct window_d *window);
int windowIsWindow(struct window_d *window)
{
	//@todo:Implementar.
	return 0;
};
*/

/*
void* windowGetWindowWithFocus();
void* windowGetWindowWithFocus()
{
	return (void*) WindowWithFocus;
}
*/

//configuramos qual janela ficar� por cima de todas as outras.
/*
void windowSetForegroundWindow(struct window_d *window);
void windowSetForegroundWindow(struct window_d *window)
{
	//set focus 
	//coloca no topo da lista
	return;
}
*/


//get parent window
void *GetParentWindow(struct window_d * hwnd)
{
	if( (void*) hwnd == NULL ){
		return NULL;
	}
	return (void*) hwnd->parent;
};


//get the desktop of a window.
void *GetWindowDesktop(struct window_d * hwnd)
{
	if( (void*) hwnd == NULL ){
		return NULL;
	}
	return (void*) hwnd->desktop;
};


/*
 * =====================================================
 * windowSetUpColorScheme:
 *     Isso configura os esquemas de cores utilizados 
 * pelo sistema.
 *     Essa rotina � chamada apenas uma vez na inicializa��o
 * do kernel.
 *     O esquema de cores a ser utilizado deve estar 
 * salvo no perfil do usu�rio que fez o logon.
 * Os dois esquemas padr�o chamam-se: 'humility' e 'pride'.
 * + O esquema 'humility' s�o cores com tema cinza, 
 * lembrando interfaces antigas.
 * + O esquema 'pride' s�o cores modernas 
 *   ( Aquele verde e preto e cinza, das primeiras vers�es, 
 * com imagens publicadas. )
 * @todo: � preciso criar rotinas que selecionem entre os 
 * modo criados e habilitados.
 * � preciso criar rotinas que permitam que aplicativos 
 * em user mode criem esquemas de cores e habilite eles.
 */

void windowSetUpColorScheme (int type){
	
    struct color_scheme_d *humility;
    struct color_scheme_d *pride;	
	
	//
	// * HUMILITY
	//
	
    //Criando o esquema de cores humility. (cinza)
    humility = (void *) malloc ( sizeof(struct color_scheme_d) );
    
	if( (void*) humility == NULL )
	{
		printf("windowSetUpColorScheme: humility");
		die();
		
	}else{
		
		//Object.
		humility->objectType = ObjectTypeColorScheme;
		humility->objectClass = ObjectClassGuiObjects;
		

		humility->used = 1;
		humility->magic = 1234;
		humility->name = "Humility";
		
		//Colors
		humility->elements[csiNull] = 0;                               //0
        humility->elements[csiDesktop] = COLOR_BACKGROUND;             //1		
		humility->elements[csiWindow]  = COLOR_WINDOW;                 //2
        humility->elements[csiWindowBackground] = xCOLOR_GRAY1;	       //3	
		humility->elements[csiActiveWindowBorder] = xCOLOR_GRAY2;      //4
        humility->elements[csiInactiveWindowBorder] = xCOLOR_GRAY3;    //5		
		humility->elements[csiActiveWindowTitleBar] = xCOLOR_GRAY2;    //6
        humility->elements[csiInactiveWindowTitleBar] = xCOLOR_GRAY3;  //7	 	
		humility->elements[csiMenuBar] = COLOR_MENUBAR;                //8
        humility->elements[csiScrollBar] = COLOR_SEASHELL;   //xCOLOR_GRAY3;		       //9  
		humility->elements[csiStatusBar] = COLOR_STATUSBAR3;             //10
        humility->elements[csiMessageBox] = xCOLOR_GRAY2;		       //11
		humility->elements[csiSystemFontColor] =   COLOR_BLACK;		   //12
		humility->elements[csiTerminalFontColor] = COLOR_WHITE;	       //13
		//...
		
		//Sanvando na estrutura padr�o para o esquema humility.
		HumilityColorScheme = (void*) humility;
	};	
	
	//
	// * PRIDE 
	//
	
    //Criando o esquema de cores PRIDE. (colorido)
    pride = (void *) malloc ( sizeof(struct color_scheme_d) );
    
	if ( (void *) pride == NULL )
	{
		printf("windowSetUpColorScheme: pride");
		die();
	
	}else{
		
		//Object.
		pride->objectType = ObjectTypeColorScheme;
		pride->objectClass = ObjectClassGuiObjects;

		pride->used = 1;
		pride->magic = 1234;
		pride->name = "Pride";
		
		//Colors
		pride->elements[csiNull] = 0;
        pride->elements[csiDesktop] = COLOR_BACKGROUND;  
		pride->elements[csiWindow] = COLOR_WINDOW;
        pride->elements[csiWindowBackground] = xCOLOR_GRAY1;
		pride->elements[csiActiveWindowBorder] = 0x0080FFFF; //(LIGHT GREEN) 
        pride->elements[csiInactiveWindowBorder] = 0x0080FFFF;  
		pride->elements[csiActiveWindowTitleBar] = 0x0080FFFF;    
        pride->elements[csiInactiveWindowTitleBar] = 0x0080FFFF;		
		pride->elements[csiMenuBar] = COLOR_GRAY;
        pride->elements[csiScrollBar] = COLOR_SEASHELL; //COLOR_GRAY;		
		pride->elements[csiStatusBar] = 0x2EB982;//COLOR_STATUSBAR3;   
        pride->elements[csiMessageBox] = COLOR_GRAY;
		pride->elements[csiSystemFontColor] = COLOR_BLACK;    //12
		pride->elements[csiTerminalFontColor] = COLOR_WHITE;  //13		
		//...
		
		//Sanvando na estrutura padr�o para o esquema pride.
		PrideColorScheme = (void *) pride;
	};	
		
	
	// Configurando qual ser� o esquema padr�o.
	// @todo; Criar uma fun��o que selecione qual dois esquemas ser�o usados
	//        apenas selecionando o ponteiro da estrutura.  
	
    switch (type){
		
		case ColorSchemeNull:
		    CurrentColorScheme = (void *) humility;
		    break;
		
		case ColorSchemeHumility:
		    CurrentColorScheme = (void *) humility;
		    break;
		
		case ColorSchemePride:
	        CurrentColorScheme = (void *) pride; 
		    break;
		
		default:
		    CurrentColorScheme = (void *) humility;
			break;
	};	
};


//seleciona o tipo ...isso � um servi�o.
int windowSelectColorScheme ( int type ){
	
	//#debug
	//printf("windowSelectColorScheme: type={%d} \n", type);
	
    switch (type)
	{
		case ColorSchemeHumility:
		    goto do_humility;
		    break;
			
		case ColorSchemePride:
		    goto do_pride;
			break;
			
		default:
		    printf("windowSelectColorScheme: Type not defined\n");
			goto fail;
			break;
	};

	
do_humility:
    if ( (void *) HumilityColorScheme == NULL )
    {
		printf("HumilityColorScheme fail\n");
        goto fail;     	    	
	}else{
		
	    if ( HumilityColorScheme->used != 1 || 
		     HumilityColorScheme->magic != 1234 )
		{
			printf("HumilityColorScheme sig fail\n");
			goto fail;
		}
		
		printf("Humility selected\n");
	    CurrentColorScheme = HumilityColorScheme;	
	    goto done;
	};		
	
	
do_pride:	
    if( (void *) PrideColorScheme == NULL )
    {
		printf("PrideColorScheme fail\n");
        goto fail;     	    	
	}else{
	    if(	PrideColorScheme->used != 1 || 
		    PrideColorScheme->magic != 1234 )
		{
			printf("PrideColorScheme sig fail\n");
			goto fail;
		}
		
	    printf("Pride selected\n"); 
		CurrentColorScheme = PrideColorScheme;	
	    goto done;
	};		

done:
    return (int) 0;	
fail:
    printf("fail\n");
    return (int) 1;	    
};


/*
 * windowShowWWFMessageBuffers:
 *     Mostra o buffer de mensagens da janela com foco de entrada.
 *     #Rotinas de teste. @todo: Enviar para a pasta /test.
 *     Esse rotina funcionou e se mostrou bem �til.
 */
void windowShowWWFMessageBuffers (){
	
	struct window_d *wFocus;
    wFocus = (void *) windowList[window_with_focus];

	int i=0;
	
	//Obs: Tipo.
	for ( i=0; i<32; i++ )
	{
	    printf("%d ", wFocus->msgList[i]);
    };	
};


/*
 *****************************************************************************
 * windowSendMessage:
 *     Uma mensagem ser� enviada para uma janela.
 *
 *    Uma mensagem deve ser enviada para a estrutura da janela com o 
 * foco de entrada.
 *    A classe system.devices.unblocked. pois essa rotina � um m�todo dela.
 *
 * Sobre a fila de mensagem da janela com o foco de entrada:
 * ========================================================
 *  *IMPORTANTE: 
 *      O que importa � qual janela est� com o foco de entrada.
 *      Quando sabemos qual janela est� com o foco de entrada, ent�o
 * sabemos qual � a janela que � a �rea de cliente, sabemos e provavelmente
 * a sua janela m�e � a janela ativa, se ela mesma n�o for a janela ativa.
 *      Uma mensagem pode ser enviada para a janela com o foco de entrada,
 * e a mensagem afetar� apenas a janela com o foco de entrada.
 *      Se a janela que receber a mensagem for a janela ativa, ela n�o
 * ter� janela m�e, nessa caso uma mensagem para fechar uma janela ativa
 * implica em fechar o processo ao qual ela est� vinculada.
 *      (N�o importa qual processo foi interrompido pela interrup��o de 
 * teclado. A mensagem deve ir para a fila de mensagens da janela com o foco
 * de entrada.)
 * @todo: Criar uma rotina semelhante, mas exclusivamente para a janela com
 * foco de entrada.
 * Ex: void windowSendMessageWWW(unsigned long arg1, unsigned long arg2, 
 * unsigned long arg3, unsigned long arg4)
 */	
 
void 
windowSendMessage ( unsigned long arg1, 
                    unsigned long arg2, 
					unsigned long arg3, 
					unsigned long arg4 )
{    
	struct window_d *wFocus;
	
	// lembrando: O arg1 por enquanto ser� ignorado ;;; ele deveria 
	//conter a janela que devemos enviar a mensagem...
	// pegaremos por conta pr�pria o penteiro pra janela com o foco de entrada.
	
    // N�o estamos falando de fila de mensagens na estrutura da janela,
	// portando se uma mensagem for enviada para uma estrutura de janela
	// ela ir� sobrepor a mensagem antiga caso a mensagem antiga ainda n�o 
	// tenha sido consumida.

	// @todo: 
	// Na verdade, aqui tem que mandar a mensagem para a janela com o foco 
	// de entrada, mesmo que seja uma janela filha.

	//lenbrando que esse � um endere�o f�sico, o m�nimo que uma rotina em user mode deve fazer 
    //com essa informa��o � converte-la em endere�o l�gico.
    //Ou devemos enviar um endere�o l�gico ??	
	
	wFocus = (void *) windowList[window_with_focus];
	
	// Aqui estamos apenas enviando para a janela com foco de entrada.
	// N�o fazer nada caso n�o haja uma janela com o foco de entrada, 
	// pois quem decide que janela tem o foco de entrada � o usu�rio.
		
	if( (void *) wFocus == NULL )
	{
		printf("windowSendMessage: wFocus fail\n");
		refresh_screen();
	    return;
		
	}else{
		
		if ( wFocus->used == 1 && wFocus->magic == 1234 )
		{
			wFocus->msg_window = (struct window_d *) arg1;
			wFocus->msg = (int) arg2;      
			wFocus->long1 = (unsigned long) arg3;
			wFocus->long2 = (unsigned long) arg4;
			wFocus->newmessageFlag = 1;
			
			// #bugbug: 
			// Acho que isso n�o � usado.
			// isso � um teste.
			// Para facilitar vamos colocar a mensagem num lugar mais acessivel.
			
			gNextKeyboardMessage = (int) 0;
			
		}else{
			//fail.
		};
	};
};


/*
 **********************************************************************
 * windowGetHandleWindow:
 *     Pega uma mensagem na estrutura da janela com o foco de entrada.
 *     #importante: N�o devemos nos preocupar em saber qual foi o dispositivo 
 *                  gerador do evento, apenas pegar a mensagem na fila da    
 *                  janela com o foco de entrada.
 *
 * Obs: 
 *     Esse mecanismo deve ser reproduzido para os parametros hwnd, long1 e long2.
 *     As fun��es precisam ser criadas ainda. semelhantes a essa.
 *     Mas o que nos importa agora � somente o par�metro 'msg'.
 * @todo: Quem chamou essa rotina, deseja pegar a mensagem de qualquer janela 
 *ou somente a da janela com o foco de entreada.???
 * @todo: Criar uma rotina semelhante, mas exclusivamente para a janela com foco de entrada.
 * Ex: void *windowGetMessageWWF()
 */

// Salvando a estrutura para o app pegar elemento por eemento.	(#teste)
struct message_d *xxxxsavemessage; 
 
void *windowGetHandleWindow (struct window_d *window){
	
	//if( window_getch_lock == 1){
	//	return NULL;
	//}
	
	//#importante.
	//Essa rotina ainda n�o � chamada pelo consumidor,
	//apenas as outras 3.
	
	//@todo: Precisa fazer essa fun��o.
	
//fast_return:
	return NULL;
	
	//fast way 
	//@todo: melhorar isso
	struct window_d *wFocus;
	
	wFocus = (void *) windowList[window_with_focus];
	
	//@todo: habilitar esse if.
	//if( wFocus->newmessageFlag == 0 ){
	//	return NULL;
	//}
	
	return (void*) wFocus->msg_window;
	
	//Nothing.
fail:	
	return NULL;
};


/*
 * windowGetMessage:
 *     Pega uma mensagem na estrutura da janela com o foco de entrada.
 *
 * Obs: 
 *     Esse mecanismo deve ser reproduzido para os parametros hwnd, long1 e long2.
 *     As fun��es precisam ser criadas ainda. semelhantes a essa.
 *     Mas o que nos importa agora � somente o par�metro 'msg'.
 * @todo: Quem chamou essa rotina, deseja pegar a mensagem de qualquer janela 
 *ou somente a da janela com o foco de entreada.???
 * @todo: Criar uma rotina semelhante, mas exclusivamente para a janela com foco de entrada.
 * Ex: void *windowGetMessageWWF()
 */
void *windowGetMessage(struct window_d *window)
{
	unsigned char SC;
	//fast way 
	//@todo: melhorar isso
	struct window_d *wFocus;	
	
	
	//if( window_getch_lock == 1){
	//	return NULL;
	//}
	
	//
	// E se nesse momento j� existe uma mensagem para 
	// ser consumida, que foi colocada por outro 
	// dispositivo de input. ??
	//
	
	
	//
	// ## Keyboard ##
	//
	
	//#bugbug:
	//Obs: Isso deve ser feito em outra ocasi�o,
	// Talvez a interrup��o de teclado pode 
	//acinar um 'request' dizendo que o 
	//kernel deve enviar uma mensagem da fila de teclado 
	// para a janela com o foco de entrada.
	//
	SC = (unsigned char) keybuffer[keybuffer_head];
	
	//Limpa.
	keybuffer[keybuffer_head] = 0;
	
	keybuffer_head++;
	
	if( keybuffer_head >= 128 ){
	    keybuffer_head = 0;	
	}
		
	//#bugbug
	//alguma coisa est� imprimindo o char duas vezes ...
	//Talvez o pr�prio aplicativo esteja imprimeiro tamb�m.
	//
	
    //isso coloca a mensagem na fila da thread atual.
	
	KEYBOARD_LINE_DISCIPLINE ( SC );	
	

	
	wFocus = (void *) windowList[window_with_focus];
	
	if( wFocus->newmessageFlag == 0 ){
		return NULL;
	}
	
	return (void *) wFocus->msg;	
	
	
	//Nothing.
//fail:
	
	return NULL;
};



void *windowGetLong1(struct window_d *window)
{
	//fast way 
	//@todo: melhorar isso
	struct window_d *wFocus;
	
	
	//if( window_getch_lock == 1){
	//	return NULL;
	//}

	
	wFocus = (void *) windowList[window_with_focus];
	
	if( wFocus->newmessageFlag == 0 ){
		return NULL;
	}
	return (void*) wFocus->long1;
	
	//Nothing.
fail:	
	return NULL;
};



void *windowGetLong2(struct window_d *window)
{
	
	//if( window_getch_lock == 1){
	//	return NULL;
	//}
	
	//@todo: Liberar long2.
	return NULL;
	
	//fast way 
	//@todo: melhorar isso
	struct window_d *wFocus;
	
	wFocus = (void *) windowList[window_with_focus];
	
	if( wFocus->newmessageFlag == 0 ){
		return NULL;
	}
	
	//sinaliza que a mensagem foi lida, e que n�o temos nova mensagem.
	wFocus->newmessageFlag = 0;
	
	return (void *) wFocus->long2;
	

	//Nothing.
fail:	
	return NULL;
};


/*
 **********************************************************
 * window_getch:
 *
 * Esse � o servi�o 137.
 * Isso � usado pela biblioteca stdio em user mode
 * na fun��o getchar()
 * #bugbug: N�o est� pegando todos os caracteres digitados.
 *
 * um aplicativo que roda no shell pode estar chamando isso.
 *
 */
 
int window_getch (){
	
	//pode ser que esse aplicativo n�o tenha janela,
	//mas esteja rodando na janela do shell.
	
	struct window_d *wFocus;	
	
	unsigned char SC;
	int save;
	
	
	//
	// Bloqueia pra que nenhum aplicativo pegue mensagens 
	// na estrutura de janela at� que window_getch termine.
	//
	
	//window_getch_lock = 1;
	
	//
	// Pegamos um scancode na fila do teclado,
	// transformamos ela em mensagem e colocamos a 
	// mensagem na estrutura da janela com o foco de entrada.
	//
	

	SC = (unsigned char) keybuffer[keybuffer_head];
	
	//Limpa o offset na fila de teclado 
	//onde pegamos o scancode.
	
	keybuffer[keybuffer_head] = 0;
	
	//Circulamos a fila de teclado.
	
	keybuffer_head++;
	
	if ( keybuffer_head >= 128 )
	{
	    keybuffer_head = 0;	
	}

 
    //isso coloca a mensagem na fila da thread atual.
	
	KEYBOARD_LINE_DISCIPLINE ( SC );	

	
	// Agora vamos pegar a somente a parte da mensagem 
	// que nos interessa, que � o caractere armazenado em long1.
	// Obs: Somente queremos o KEYDOWN. Vamos ignorar as outras 
	// digita��es.
	
	//fast way 
	//@todo: melhorar isso
	
	wFocus = (void *) windowList[window_with_focus];
	
	if ( (void *) wFocus == NULL )
	{
		//fail 
		//free(wFocus);
		goto fail;
	
	} else {
		
		if ( wFocus->msg != MSG_KEYDOWN ){
		    goto fail;	
		}
		
		save = (int) wFocus->long1;
			
		wFocus->msg_window = 0;
		wFocus->msg = 0;
		wFocus->long1 = 0;
		wFocus->long2 = 0;
			
	    //sinaliza que a mensagem foi lida, e que n�o temos nova mensagem.
	    wFocus->newmessageFlag = 0;
	
	    //window_getch_lock = 0;
		return (int) save;
	};

	
fail:
done:
   // window_getch_lock = 0;
	return (int) -1; //erro	
};



/*
 * windowCreateDedicatedBuffer: 
 *     Cria um buffer dedicado de acordo com as dimens�es da janela.
 */
int windowCreateDedicatedBuffer(struct window_d *window) 
{
	size_t bufferSize;
	
	//Check;
	if((void*) window == NULL ){ 
	    return (int) 1;  //Fail. 
	};
	
    //
    // Buffer size.
    //
	
	//@todo: Usar vari�vel para quantidade de bytes por pixel.
	bufferSize = (size_t) (window->width * 3) + (window->height * 3);
	
	//
	// Alocando mem�ria no Heap do kernel.
	//
	
	if(bufferSize == 0){
	    return (int) 1;    //Fail.	
	}
	else
	{
	    window->DedicatedBuffer = (void*) malloc(bufferSize);	
	    if( (void*) window->DedicatedBuffer == NULL ){
		    return (int) 1;    //Fail.	
		}
		goto done;
	};
	//Nothing
done:
	return (int) 0;  //Success.
};


/*
 * windowLock:
 *     Bloqueia uma janela.
 *     @todo: Quem pode realizar essa opera��o??
 */
 
void windowLock (struct window_d *window){
	
	if ( (void *) window == NULL )
	{
	    return;
    };
	
    window->locked = (int) WINDOW_LOCKED;  //1.
};


/*
 * windowUnlock:
 *     Desbloqueia uma janela.
 *     @todo: Quem pode realizar essa opera��o??
 */
 
void windowUnlock (struct window_d *window){
	
	if ( (void *) window == NULL )
	{
	    return;
    };  
    
	window->locked = (int) WINDOW_UNLOCKED;  //0.
};


/*
 * set_current_window:
 *     @todo: Mudar para windowSetCurrentWindow
 */
 
void set_current_window (struct window_d *window){
	
	if ( (void *) window == NULL )
	{ 
	    return; 
	};  
    
	CurrentWindow = (void *) window;
	current_window = (int) window->id;
};


/*
 * get_current_window: 
 *     Get current window pointer.
 */
void *get_current_window (){
	
	return (void *) CurrentWindow;
};


/*
int get_current_window_id();
int get_current_window_id()
{
    return (int) current_window;	
};
*/


/*
 *******************************************************
 * RegisterWindow: 
 *     Registrando uma janela numa lista de janelas.
 */
 
int RegisterWindow (struct window_d *window){
	
    struct window_d *Empty; 
	
	int Offset = 0; 
	
    if ( (void *) window == NULL )
	{
        return (int) 1;    //Erro, estrutura inv�lida.
    };
   
	// Contagem de janelas e limites.
	// (� diferente de id, pois id representa a posi��o
	// da janela na lista de janelas).
	
	windows_count++;
	
	if ( windows_count >= WINDOW_COUNT_MAX )
	{
	    printf("RegisterWindow: Limits");
		die();
	};
    
	// Create empty.	
	
//Loop.	
get_next:

	Offset++;
	
	//Limite da lista, volta ao in�cio da lista.
	
	if (Offset >= WINDOW_COUNT_MAX){
	   Offset = 0;
	};	
    
	//Get empty.
 	Empty = (void *) windowList[Offset]; 
	
    if ( (void *) Empty != NULL )
	{
        goto get_next;
		
    }else{
	    
		//coloca na lista.
		//Onde est� vazio, registra o ponteiro.
	    windowList[Offset] = (unsigned long) window; 	
		
		//configura o id.
		window->id = (int) Offset;
		
		//...
	};  		

    return (int) 0;
};


/*
 * windowShowWindowList:
 *     Show Window List
 *     Mostra a lista de janelas registradas. 
 */
 
//#debug
//usado para debug.
 
void windowShowWindowList (){
	
	int i = 0;
	struct window_d *hWnd;
	struct window_d *hWindow;
	
	unsigned long left;
	unsigned long top;
	unsigned long width;
	unsigned long height;
	
	//Se n�o estivermos em modo gr�fico, n�o h� o que mostrar.	
    
	if ( VideoBlock.useGui != 1 )
	{
	    return;	
	};
	
	// A janela da �rea de trabalho.
	if ( (void *) gui->main == NULL)
	{
	    return;
	}else{
		
	    left = gui->main->left;
	    top = gui->main->top;
	
	    width = gui->main->width;
	    height = gui->main->height;	
		
	    //...
	};
	
	
		
	//@todo: Chamar m�todo.	
	//Cursor.
	g_cursor_x = (left/8);
	g_cursor_y = (top/8);  
	//set_up_cursor(0,10);
	
	//
	// Se estamos no modo gr�fico.
	//
	
	
	//
	// @todo: Usar esquema de cores padr�o.
	//
	
	if( VideoBlock.useGui == 1 )
	{
		//Parent window.
	    //#warning: J� checamos isso anteriormente.
		
		if( (void *) gui->main == NULL)
		{
	        return;
	    };
		
	    hWindow = (void *) CreateWindow ( 3, 0, VIEW_MAXIMIZED, "Window List", 
	                        left, top, width, height, 
			     			gui->main, 0, 
							KERNEL_WINDOW_DEFAULT_CLIENTCOLOR, 
							KERNEL_WINDOW_DEFAULT_BGCOLOR ); 

	    if ( (void *) hWindow == NULL )
		{
	        printf("show_window_list:\n");
		    return;
        }else{
		    RegisterWindow(hWindow);
			set_active_window(hWindow);
			SetFocus(hWindow);
	    };										
		
		
		// Auterando as margens.
		// Essas margens s�o usadas pela fun��o printf.
		// Obs: As medidas s�o feitas em n�meros de caracteres.
		// Obs: @todo: Devemos usar aqui o ret�ngulo da �rea de cliente,
		// e n�o as margens da janela.
		// A estrutura de janela deve nos oferecer os valores para a m�trica do 
		// ret�ngulo da �rea de cliente.
		// Obs: @todo:Acho que essa n�o � a forma correta de configurar isso. Uma 
		//rotina deveria perceber as dimens�es da janela de do caractere e determinar
		//as margens.
		
		g_cursor_left = (hWindow->left/8);
		g_cursor_top = (hWindow->top/8) + 4;   //Queremos o in�cio da �rea de clente.
		
		g_cursor_right = g_cursor_left + (width/8);
		g_cursor_bottom = g_cursor_top  + (height/8);
		
		//cursor (0, mas com margem nova).
		g_cursor_x = g_cursor_left; 
		g_cursor_y = g_cursor_top; 
		
        //Mostrando as informa��es de todas as janelas registradas.		
        while( i < WINDOW_COUNT_MAX )
        {	
	        hWnd = (void *) windowList[i];
		
		    
		    // @todo: BUGBUG.
		    // O valor de hWnd tem que estar num limite v�lido, n�o adianta apenas ser
		    // diferente de NULL.
		    
		
		    if( (void *) hWnd != NULL )
			{
	            printf ("i={%d} h={%x} dBuf={%x} Name={%s}\n", 
				    i, hWnd, hWnd->DedicatedBuffer, hWnd->name );
					
				//draw_text( hWindow, 8,  1*(400/16), COLOR_WINDOWTEXT, "F1 Help.");
	        };
			
		    i++;
	    };
		
		show_active_window();
        show_window_with_focus();
        SetFocus(hWindow);
	
		
		//voltando a margem normal a margem
		g_cursor_left = (left/8);    //0;
		g_cursor_top = (top/8);        //0;
		
		g_cursor_right = (width/8);   
		g_cursor_bottom = (height/8);  
		
		//cursor (0, mas com margem nova)
		g_cursor_x = g_cursor_left; 
		g_cursor_y = g_cursor_top;
        //set_up_cursor(g_cursor_left,g_cursor_top); 		


		StatusBar ( hWindow, "Esc=EXIT","Enter=?" );		
		//Nothing.
	};	   

    // Continua ...
	
//done:

    //@todo; 
	//Aqui podemos dar refresh apenas na janela
	
	refresh_screen();
    
	//SetFocus(hWindow);
    //return;
};


/*
 *********************************************************
 * redraw_window:
 *     (Em fase de desenvolvimento)
 *     Repinta uma janela de acordo com os par�metros na estrutura.
 *     Isso depende do tipo. Para alguns tipos o trabalho ser� f�cil. 
 *     Repinta uma janela v�lida. Tem que estar registrada.  
 *     O OBJETICO DESSA ROTINA DEVE SER APENAS REPINTAR 
 * UM RET�NGULO QUE FOI PINTADO DENTRO DA �REA DE CLIENTE 
 * DA JANELA QUE FOI PASSADA VIA ARGUMENTO. (isso para o caso 
 * de janelas do tipo overlapped)
 * 
 * Obs: Esse ret�ngulo deve pertencer a janela que foi passada 
 * por argumento. 
 * Na estrutura de ret�ngulo deve haver um ponteiro para 
 * a janela � qual ele pertence.
 * Tamb�m deve-se repintar os elementos incluidos na 
 * �rea de cliente pelo programa aplicativo.
 *
 * A estrutura de janela deve registrar todos os elementos 
 * que foram inclu�dos na hora da sua cria��o.
 * *****************
 *
 *     @todo: Mudar para windowRedrawWindow(.)
 *
 * Obs: Esse � o tipo de rotina que exige o recurso 
 * de buffer dedicado.??
 * Pois podemos redezenhar a janela em seu buffer dedicado e 
 * depois copiar o conte�do do buffer dedicado para o 
 * backbuffer... na hora de atualizar a tela
 * � so copiar o backbuffer no frontbuffer(LFB).
 *
 * Pois bem, essa janela vai ter muitos elementos para 
 * incluir isso leva muito tempo.
 * Devemos registrar na estrutura da janela todos os 
 * bot�es que ela tem na hora de cri�-la.
 * Todos os textos gravados nela... todas as cores etc..
 * Obs: Talvez essa rotina seja recursiva assim como 
 * a fun��o create window.
 *
 * 
 * >>> e se a janela for um bot�o ?? ...
 *     gostar�amos de mudar o status do bot�o e depois repint�-lo 
 **   Criar a fun��o que muda o status do bot�o 
 *    usar redraw_window para repintar uma janela do tipo bot�o. 
 *    assim poderemos setar o foco em janela e em bot�o.
 */
 
//int windowRedrawWindow(struct window_d *window) 

int redraw_window (struct window_d *window, unsigned long flags ){
	
	int Status; 
	unsigned long border_size = 0;
	unsigned long border_color = COLOR_BORDER;	
	
	//printf ("redraw_window: debug \n");
	
	if ( (void *) window == NULL )
	{
		goto fail;
	}else{
		
	    if ( window->used != 1 || window->magic != 1234 )
		{
		    goto fail;
	    }
		
		//...
	};
	
	
	//@todo: Pra que serva a flag redraw ??
	// � para indicar que repecisamos repintar. ??
	//if( window->redraw != 1 ){ return (int) 0;}
	
	//
	// Ok, pelo jeito j� temos uma estrutura v�lida.
	//
	
	// Minimized? Se tiver minimizada, n�o precisa repintar.
	Status = (int) is_window_minimized (window);
    if(Status == 1)
	{
		//?? tem qua mudar alguma flag antes ??
	    goto done;
	}	
	
	//E se ela estiver travada ??
	//O que significa travada?? n�o pode se mover??
	// ?? travada pra quem ??
	//if(window->locked == 1){}
	
	//@todo:
	//docked ?? num canto da tela.

	
	//Pintar ?? Onde ?? backbuffer ?? dedicated buffer ??
//drawWindow:	
	
	// *********************************
	// Devo lembrar que as caracter�sticas da janela est�o salvas na estrutura
	// e s�o muitas caracter�sticas. N�o exige c�lculos, toda a m�trica j� est� pronta.
	// *********************************
	
	
	//Buffers ??
	//window->DedicatedBuffer
	//window->BackBuffer
	//window->FrontBuffer
	
	
	// *Importante: 
	// Checando se o esquema de cores est� funcionando.
	// obs: Essa checagem est� se tornando repetitiva.
	
	if ( (void *) CurrentColorScheme == NULL ){
		panic ("redraw_window: CurrentColorScheme");
		
	} else {
		
		if ( CurrentColorScheme->used != 1 || 
		     CurrentColorScheme->magic != 1234 )
		{
		    panic ("redraw_window: CurrentColorScheme validation");		
		};
	};	
	
	
	// ****************************
	// Importante:
	// Agora, na hora de efetivamente redezenhar, 
	// podemos ver na estrutura quais s�o os elementos 
	// presentes nela. Lembrando que esses elementos podem ter 
	// sido modificados, ent�o o melhor lugar para pegar essas
	// informa��es � na estrutura mesmo, pois l� ser�o salvos 
	// as prefer�ncias atuais do usu�rio em rela��o a janela 
	// que deve ser repintada.
	// Ex: O usu�rio pode ter redimencionado ou arrastado 
	// elementos da janela, ou ocultado apenas. 
	// Afinal, feito escolhas ... 
	//******************************
	
    //
	// # Redraw #
	//	
	
	// Cada estilo de design tem suas caracter�sticas,
	// essas caracter�sticas precisas ser registradas 
	// na estrutura de janel, bem como o pr�prio estilo 
	// de design.
	
//redrawBegin:

	if ( window->view == VIEW_NULL )
	{
		//#bugbug
	    goto fail;
	};		

    //Sombra.	
	if (window->shadowUsed == 1)
	{
		//#bugbug
		//A sombra deve ter suas dimens�es registradas tamb�m.
		if ( window->type == WT_OVERLAPPED )
		{
            // @todo: Adicionar a largura das bordas verticais 
			// e barra de rolagem se tiver.
			// @todo: Adicionar as larguras das 
			// bordas horizontais e da barra de t�tulos.
			// Cinza escuro.  CurrentColorScheme->elements[??] 
			// @TODO: criar elemento sombra no esquema. 
			
			drawDataRectangle ( window->left +1, window->top  +1, 
				window->width  +1 +1, window->height +1 +1, xCOLOR_GRAY1 );             
        };                
	};

    //Background.
	if (window->backgroundUsed == 1)
	{		
		drawDataRectangle ( window->left, window->top, 
			window->width, window->height, window->bg_color );  	
	};
	
    //printf ("redraw_window: breakpoint \n");
	//refresh_screen();
	//while(1){}
	
	//borda.
	//para os casos de editbox por exemplo.
	
	if ( window->borderUsed == 1 )
	{
		//#importante:
		//devemos tratar a borda para cada tipo de janela individualmente.
		
		//bot�o
		if ( window->type == WT_BUTTON )
		{
			//se o bot�o tiver o foco.
			if ( window->focus == 1 )
			{
				border_color = COLOR_BLUE;
			    border_size = 2;
			}else{
			    border_color = COLOR_INACTIVEBORDER;	
			    border_size = 1;
			}
		};
		
		//editbox
		if ( window->type == WT_EDITBOX )
		{
			//se tiver o foco.
			if ( window->focus == 1 )
			{
				border_color = COLOR_BLUE;
			    border_size = 2;
			}else{
			    border_color = COLOR_INACTIVEBORDER;	
			    border_size = 1;
			}
		};		
		
		//overlapped (app)
		if ( window->type == WT_OVERLAPPED )
		{
			//se tiver o foco.
			if ( window->focus == 1 )
			{
				border_color = COLOR_BLUE;
			    border_size = 2;
				
				if (window->active == 1){
				    border_size = 3;	
				}
				
			}else{
			    border_color = COLOR_INACTIVEBORDER;	
			    border_size = 1;
			}
		};	

		//simple.
		if ( window->type == WT_SIMPLE )
		{
			//se tiver o foco.
			if ( window->focus == 1 )
			{
				border_color = COLOR_GRAY;
			    border_size = 2;
			}else{
			    border_color = COLOR_INACTIVEBORDER;	
			    border_size = 1;
			}
		};		
		
		
	    //board1, borda de cima e esquerda.    
		drawDataRectangle ( window->left, window->top, 
			window->width, border_size, border_color );
						   
	    drawDataRectangle ( window->left, window->top, 
			border_size, window->height, border_color );

	    //board2, borda direita e baixo.
	    drawDataRectangle ( window->left +window->width -1, window->top, 
		    border_size, window->height, border_color );
					   
	    drawDataRectangle ( window->left, window->top +window->height -1, 
			window->width, border_size, border_color );		
	};
	
	
	
    //T�tulo + borda(frame).	
	if (window->titlebarUsed == 1)
	{ 
		//#importante:
		//@todo:Ainda h� muito o que levar em considera��o 
		//na hora de repintar uma janela com rela��o 
		//ao fato de serem ativas ou n�o ou de terem o foco ou n�o.
 
        //no caso de janela m�e.
        if (window->id == active_window)
		{
			window->bg_color = CurrentColorScheme->elements[csiActiveWindowTitleBar];
		}else{
			window->bg_color = CurrentColorScheme->elements[csiInactiveWindowTitleBar]; 
		};
	
		//Focus.
		//Cores diferentes se tiver foco e se n�o tiver.
		//no caso de janelas filhas
		if(window->id == window_with_focus)
		{
		    window->bg_color = CurrentColorScheme->elements[csiActiveWindowTitleBar]; 
		}else{
			window->bg_color = CurrentColorScheme->elements[csiInactiveWindowTitleBar]; 
		};
		
		//@todo: String color.
		
		//@todo: String size.
		
		//@todo: Se tiver barra de rolagem, a largura deve ser maior.
		//if()
		
		//Rectangle and string.
		
        drawDataRectangle ( window->left, window->top, 
			window->width +1 +1, window->height +1 +1, window->bg_color );
						   
						   
		//@todo: 
		//string da barra de t�tulos.
		//Se estivermos em full screen, n�o teremos string.				   
		draw_string ( window->left +8 +16 +8, window->top +8 +4, 
			COLOR_TERMINALTEXT2, window->name );  		 
					 
	    //Isso � um teste.
	    //A janela nem foi registrada ainda e j� estamos passando o handle
	    //via argumento.
		
	    //So criamos o bot�o na barra de t�tulos se tivermos uma barra de t�tulos.
		//ent�o esse � o lugar certo para essa rotina.
		
		//@todo: Se estivermos em full screen, n�o teremos bot�o.	
		if(window->minimizebuttonUsed == 1)
		{        
			if( window->minimize != NULL )
			{
				if ( window->minimize->used == 1 && window->minimize->magic == 1234 )
				{
			        //draw_button ( window, "V", 
			        //    1, 0, 0, 
			        //    window->minimize->left, window->minimize->top, window->minimize->width, window->minimize->height, 
				    //    window->minimize->bg_color );						
				}
				
			}
	    };
		
		if(window->maximizebuttonUsed == 1)
		{
			if( window->maximize != NULL )
			{
				if ( window->maximize->used == 1 && window->maximize->magic == 1234 )
				{
			        //draw_button ( window, "^", 
			        //    1, 0, 0, 
			        //    window->maximize->left, window->maximize->top, window->maximize->width, window->maximize->height, 
				    //    window->maximize->bg_color );						
				}
				
			}					
		}
		//@todo: Se estivermos em full screen, n�o teremos bot�o.
	    if (window->closebuttonUsed == 1)
		{			
			if( window->close != NULL )
			{
				if ( window->close->used == 1 && window->close->magic == 1234 )
				{
			        //draw_button ( window, "X", 
			        //    1, 0, 0, 
			        //    window->close->left, window->close->top, window->close->width, window->close->height, 
				    //    window->close->bg_color );						
				}
				
			}		
	    };					 
					
		//More??...			 		 
	};		
	
    //printf ("redraw_window: breakpoint after title bar \n");
	//refresh_screen();
	//while(1){}	

    // Client Area. 	
	if (window->clientAreaUsed == 1)
	{
		//
		// Obs: A Client Area � apenas um ret�ngulo.
		//

		//@todo: Passar a estrutura de janela.
		
		
		// #BUGBUG :: 
		// ( N�O ) PODEMOS REALOCAR NOVAMENTE ... 
		// TEMOS QUE CHECAR SE A ESTRUTURA � V�LIDA...
		// SE TRATA APENAS DE UMA ESTRUTTURA DE RET�NGULO, 
		// N�O ESPERAMOS MUITOS PROBLEMAS.
		
		 
		if ( (void *) window->rcClient == NULL )
		{
			printf("redraw_window: rcClient");
			die();
		}else{
			
			//conferir validade.
			if( window->rcClient->used != 1 || 
			    window->rcClient->magic != 1234 )
			{
			    printf("redraw_window: rcClient validation");
			    die();							
			}
			
			// Aqui devemos respeitar a forma que a �rea de cliente 
			// foi desenhada.
			// Mas todos os valores que precisamod devar estar 
			// salvos na estrutura de �rea de cliente.	
			
			//draw
            drawDataRectangle ( (unsigned long) window->rcClient->left, 
		                        (unsigned long) window->rcClient->top, 
						        (unsigned long) window->rcClient->width, 
						        (unsigned long) window->rcClient->height, 
						        (unsigned long) window->rcClient->bg_color );         
							    
		};
		//Nothing.
	};

    //printf ("redraw_window: breakpoint after client area \n");
	//refresh_screen();
	//while(1){}	
	
    //#obs: Talvez isso pode ficar dentro do if de client window.
    if (window->scrollbarUsed == 1)
    {
	    if ( window->scrollbar != NULL )
		{
			if ( window->scrollbar->used == 1 && 
			     window->scrollbar->magic == 1234 )
			{
                drawDataRectangle ( (unsigned long) window->scrollbar->left, 
		                          (unsigned long) window->scrollbar->top, 
						          (unsigned long) window->scrollbar->width, 
						          (unsigned long) window->scrollbar->height, 
						          (unsigned long) window->scrollbar->bg_color );
            
			
			    if ( window->scrollbar->scrollbar_button1 != NULL )
				{
					if ( window->scrollbar->scrollbar_button1->used == 1 &&
					     window->scrollbar->scrollbar_button1->magic == 1234 )
					{

			            //draw_button ( window->scrollbar, "^", 
			            //    1, 0, 0, 
			            //    window->scrollbar->scrollbar_button1->left, 
						//	window->scrollbar->scrollbar_button1->top, 
						//	window->scrollbar->scrollbar_button1->width, 
						//	window->scrollbar->scrollbar_button1->height, 
				        //    window->scrollbar->scrollbar_button1->bg_color );											
							 
					}						 
					
				}
				
				
			    if ( window->scrollbar->scrollbar_button2 != NULL )
				{
					if ( window->scrollbar->scrollbar_button2->used == 1 &&
					     window->scrollbar->scrollbar_button2->magic == 1234 )
					{

			            //draw_button ( window->scrollbar, "=", 
			            //    1, 0, 0, 
			            //    window->scrollbar->scrollbar_button2->left, 
						//	window->scrollbar->scrollbar_button2->top, 
						//	window->scrollbar->scrollbar_button2->width, 
						//	window->scrollbar->scrollbar_button2->height, 
				        //    window->scrollbar->scrollbar_button2->bg_color );											
							 
					}						 
					
				}
				
				
			    if ( window->scrollbar->scrollbar_button3 != NULL )
				{
					if ( window->scrollbar->scrollbar_button3->used == 1 &&
					     window->scrollbar->scrollbar_button3->magic == 1234 )
					{

			            //draw_button ( window->scrollbar, "v", 
			            //    1, 0, 0, 
			            //    window->scrollbar->scrollbar_button3->left, 
						//	window->scrollbar->scrollbar_button3->top, 
						//	window->scrollbar->scrollbar_button3->width, 
						//	window->scrollbar->scrollbar_button3->height, 
				        //   window->scrollbar->scrollbar_button3->bg_color );											
							 
					}						 
					
				}
			
		        //outros bot�es...
			
			};								
		};
	};		
	
    
	//status bar
	if (window->statusbarUsed == 1)
    {
	    if ( window->statusbar != NULL )
		{
			if ( window->statusbar->used == 1 && 
			     window->statusbar->magic == 1234 )
			{
		        drawDataRectangle ( window->statusbar->left, window->statusbar->top, 
			        window->statusbar->width -1, window->statusbar->height, window->statusbar->bg_color ); 	
						   
		        draw_string ( window->statusbar->left +8, window->statusbar->top +8, COLOR_TEXT,  
			        window->statusbar->name );  
				
			}
		}
	}
	
    //printf ("redraw_window: breakpoint status bar \n");
	//refresh_screen();
	//while(1){}	
	
	//#bugbug: 
	//At� agora repintamos a janela relativa ao bot�o. Provavelmente 
	//referese � apenas seu background. Por�m para redesenharmos o bot�o
	//precisamos criar uma fun��o redraw_button, semelhante a fun��o draw_button.
	
	//#bugbug
	//talvez isso n�o seja necess�rio. 
	//poderemos apenas repintar a janela tipo bot�o do mesmo jeito que repintamos
	//as outras e apenas atualizar a estrutura se for necess�rio.
	
	if ( window->type == WT_BUTTON )
	{
	    if ( window->isButton == 1 )
        {
			if ( (void *) window->button != NULL )
			{
				if ( window->button->used == 1 && window->button->magic == 1234 )
				{
				    //redesenhar o bot�o com base nas informa��es da estrutura.
					

    
	                
					//#todo: checar sytle state type 
					
					//com focus a borda fica azul.
					//pois redraw pode ter vindo ap�s uma atualiza��o do bot�o.
					if (window->button->state == BS_FOCUS )
					{
						window->button->border1 = COLOR_BLUE;
						window->button->border2 = COLOR_BLUE; 
					}
					
					//se o bot�o foi pressionado, mudaram o state e precisam repintar.
					if (window->button->state == BS_PRESS )
					{
						window->button->border1 = COLOR_BUTTONHIGHLIGHT2;
						window->button->border2 = COLOR_BUTTONSHADOW2; 
					}		

					if (window->button->state == BS_DEFAULT )
					{
						window->button->border1 = COLOR_BUTTONSHADOW2;
						window->button->border2 = COLOR_BUTTONHIGHLIGHT2; 
					}	

					if (window->button->state == BS_DISABLED )
					{
						window->button->border1 = COLOR_GRAY;
						window->button->border2 = COLOR_GRAY; 
						window->button->color = COLOR_GRAY;
					}	

					if (window->button->state == BS_HOVER )
					{
						window->button->color = ( window->button->color - 20);
					}					
					
					
	                //bg
	                drawDataRectangle ( window->left + window->button->x, window->top + window->button->y, 
	                    window->button->width, window->button->height, window->button->color );					
					
					//board1, borda de cima e esquerda.
	                drawDataRectangle ( window->left + window->button->x, window->top + window->button->y, 
	                    window->button->width, 1, window->button->border1 );
		
	                drawDataRectangle ( window->left + window->button->x, window->top + window->button->y, 
	                    1, window->button->height, window->button->border1 );

	                //board2, borda direita e baixo.
	                drawDataRectangle ( window->left + window->button->x + window->button->width -1, window->top + window->button->y, 
		                1, window->button->height, window->button->border2 );
					   
	                drawDataRectangle ( window->left + window->button->x, window->top + window->button->y + window->button->height -1, 
		                window->button->width, 1, window->button->border2 );					
				    
					//#todo: if aqui tem duas op��es de draw string.
                    if ( window->button->selected == 1 )
					{
					    draw_string ( window->left + window->button->x +8, window->top + window->button->y +8, 
			                COLOR_WHITE, window->button->string );
                    }
					
                    if ( window->button->selected == 0 )
                    {
					    draw_string ( window->left + window->button->x +8, window->top + window->button->y +8, 
			                COLOR_TERMINALTEXT, window->button->string );					
					}						
				}				
			}
		}			
	};

	 
		
	//
	// Outros elementos ainda n�o implementados ...
	//
	
	//checar na estrutura se essa janela tinha:
	//bot�es ?? bot�es de radio ?? textos ?? barras ??
	//etc ...


	// A rotina de repintura deve estar em sintonia com a rotina de pintura, elas 
	// devem ser semelhantes, porem a rotina de repintura deve levar em considera��o 
	// os elementos inclu�dos depois da cria��o da janela e que ficam gravados na 
	// estrutura. como por exemplo: arquivos abertos.

	
	//poderemos ter mais valores em flags no futuro.
	if ( flags == 1 )
	{
	    refresh_rectangle ( window->left, window->top, window->width, 
		    window->height );
	};
	
	//Continua ...
	
done:
    return (int) 0; 
fail:
    return (int) 1;
};


/*
 ****************
 * redraw_screen:
 *
 *     Repinta todas as janelas com base na zorder.
 *     @todo: 
 * Obs: Ao repintar cada janela a rotina redraw_window dever� 
 * incluir todos os elementos da janela. 
 * Do mesmo jeito que o usu�rio modificou de acordo com suas prefer�ncias.
 */
 
int redraw_screen (){
	
	int z;
    int RedrawStatus;	
	
	struct window_d *zWindow;
	
	// Vamos procurar na lista por ponteiros v�lidos.
	// Repintaremos todas as janelas com ponteiros v�lidos.
	
	for ( z = 0; z < ZORDER_COUNT_MAX; z++ )
	{
	    zWindow = (void *) zorderList[z];
	
		//Pegando um ponteiro de janela v�lido na zorderList.
		if ( (void *) zWindow != NULL )
		{
			if ( zWindow->used == 1 && zWindow->magic == 1234 )
            {
				//compara os �ndices.
				if ( zWindow->zIndex != z ){
					
					printf("redraw_screen: z index error\n");
					goto fail;
				};
				
				//retira o foco.
				//KillFocus(zWindow);
				
				//Repinta uma janela.
				RedrawStatus = (int) redraw_window(zWindow, 1);
				
				if (RedrawStatus == 1){
					
					printf("redraw_screen: redraw error\n");
					goto fail;
				};
				//Nothing.
			};
        //Nothing.			
		};
       //nothing.
	};	

	//#bugbug ?? Repintaremos duas vezes a �ltima janela.
    // Repintaremos novamente a �ltima janela.
    //Agora com foco de entrada.	

    if ( (void *) zWindow != NULL )
	{
		if ( zWindow->used == 1 && zWindow->magic == 1234 )
		{
            set_active_window(zWindow);	
            SetFocus(zWindow);

	        //Repinta uma janela.
	        RedrawStatus = (int) redraw_window(zWindow, 1);
			
	        if (RedrawStatus == 1){
				
		        printf("redraw_screen: redraw error\n");
		        goto fail;
	        };
		};
	};
	
	
//Seo for terminar corretamente � porque repintamos tudo o que foi poss�vel.	
//Nothing.
done:	
    return (int) 0;	
fail:
    printf("redraw_screen: FAIL.\n");
    die();
};


/*
 *********************************************
 * resize_window:
 *     Muda as dimens�es da janela.
 */	
 
int 
resize_window( struct window_d *window, 
               unsigned long cx, 
			   unsigned long cy )
{
    
	if ( (void *) window == NULL )
	{
		//Erro, estrutura inv�lida.
	    return (int) 1;    
	
	} else {	
	    
		//@todo: Checar limites.
	
        window->width  = (unsigned long) cx;
        window->height = (unsigned long) cy;	
	};

//done:

    return (int) 0;
};


/*
 ****************************************************
 * replace_window:
 *     Muda os valores do posicionamento da janela.
 */	
 
int 
replace_window ( struct window_d *window, 
				 unsigned long x, 
				 unsigned long y )
{
    if ( (void *) window == NULL ){
		return (int) 1;
	
	} else {
		
        //@todo: Checar limites.
	
        window->left = (unsigned long) x;
        window->top = (unsigned long) y;
	};

    return 0;
}


/*
 ************************************************
 * is_window_full:
 *     Checar se est� no modo tela cheia.
 *
 *     retorno 1, modo tela cheia.
 * @todo: Rever o retorno.
 */
 
int is_window_full ( struct window_d *window ){
	
    if ( (void *) window == NULL )
	{
	    return (int) 0;    //FALSE.
	
	} else {
		
	    //Checa modo tela cheia.
        if ( window->view == VIEW_FULL ){
	        return (int) 1;    //TRUE.
	    }
		
		//...
	};

	return (int) 0;
};


/*
 ********************************************
 * is_window_maximized:
 *     Checar se est� no modo maximizado.
 *
 *     retorno 1, modo maximizada.
 * @todo: Rever o retorno.
 */
 
int is_window_maximized (struct window_d *window){
	
    if ( (void *) window == NULL )
	{
	    return (int) 0;    //FALSE.
		
	} else {
	    
		//Checa se est� maximizada.
        if ( window->view == VIEW_MAXIMIZED ){
	        return (int) 1;    //TRUE.
	    };		
		//...
	};

	return (int) 0;    //FALSE.
};


//todo: rever o retorno
int is_window_minimized (struct window_d *window){
	
    
	if ( (void *) window == NULL)
	{
	    return (int) 0;    //FALSE.
	
	} else {
		
	    //Checa se est� minimizada.
        if ( window->view == VIEW_MINIMIZED ){
	        return (int) 1;    //TRUE.
	    }		
		//...
	};

	return (int) 0;    //FALSE.
};


/*
 ***********************************************
 * CloseWindow:
 *     Fecha uma janela. (Destr�i).
 *
 *     Obs: O ato de fechar uma janela est� associado
 *          ao ato de encerrar um processo. 
 * Mas nem sempre.
 * *Importante: @todo: Fechar  ajanela qui deve 
 * signifcicar apenas sinalizar para o GC que ele 
 * pode atuar sobre a estrutura.
 */
 
void CloseWindow ( struct window_d *window ){
	
	int Offset;
	int z;
	
	if ( (void *) window == NULL )
	{ 
	    return; 
		
	}else{

	    //Obs:
	    // N�o fechamos a janela principal.
	    //Se for a janela principal.
	    //if( (void*) window == (void*) gui->main ){
	    //	return;
	    //}
	
		//...
		
	    // Focus.
	    KillFocus(window);
		
		
        //Se temos uma janela m�e v�lida. Ela herda o foco.
		
	    if ( (void *) window->parent != NULL )
	    {
            if( window->parent->used == 1 && 
			    window->parent->magic == 1234 )
            {			
                set_current_window(window->parent); 
	            set_active_window(window->parent);
                SetFocus(window->parent);
            }; 		
        };
		
		
	    // devemos retirar a janela da zorder list 
	
	    z = (int) window->zIndex;
	
	    if ( z >= 0 && z < ZORDER_COUNT_MAX )
	    {
	        zorderList[z] = (unsigned long) 0;	
	    
		    //atualiza o contador.
            zorderCounter--;
	   
	        if (zorderCounter < 0 ){
		        zorderCounter = 0;
            }
	    };
	
        //Sinaliza para o GC.
	    
		window->used = WINDOW_GC;       //216;
	    window->magic = WINDOW_CLOSED;  //4321;		
		
		//...
	};
	
	//...
	
//done:

    //return;
};


/*
 ******************************************************
 * DestroyWindow:
 *     Destr�i uma janela.
 *
 *     Obs: Quando a janela � do tipo Overlapped,
 *          destruir janela significa tamb�m fechar 
 *          o processo e as threads associados � ela.
 *
 *          Pois Overlapped � o tipo de janela usado
 *          como janela principal de um processo.
 *   Obs: Apenas sinalizaremos para o GC.
 */
 
void DestroyWindow ( struct window_d *window ){
	
    CloseWindow (window);
};


/*
 * get_active_window:
 *     Obtem o id da janela ativa.
 *     @todo: Mudar para windowGetActiveWindowId().
 */
 
int get_active_window (){
	
    return (int) active_window;  
};


/*
 ****************************************************
 * set_active_window:
 *     Seleciona qual ser� a janela ativa.
 *     Ativa uma janela.
 */
 
void set_active_window (struct window_d *window){
	
	//devemos desativar a antiga janela ativa.
	int save = active_window;
	
	struct window_d *old;
	
	
	//Check window.
	
	if ( (void *) window == NULL )
	{ 
	    return; 
	
	} else {

	    if ( window->used == 1 && window->magic == 1234 )
		{
			
			//#importante:
			//se a janela j� for a janela ativa, n�o precisamos ativa.
			if( window->id == active_window )
			{ 
		        return;
			}
			
			// Se n�o tem uma janela m�e ou 
            // se ajanela m�e for uma das janelas b�sicas.			
			if ( window->parent == NULL ||
			     window->parent == gui->main ||
				 window->parent == gui->screen )
			{
		        window->active = 1;
				window->relationship_status = (unsigned long) WINDOW_REALATIONSHIPSTATUS_FOREGROUND;

			    //Estrutura global
	            ActiveWindow = (void *) window;

		        //Vari�vel global
                active_window = (int) window->id;								
                goto exit;				
			}			
			
			//se tem janela m�e e a janela m�e 
			//n�o for uma das janelas b�sicas.
			//ent�o a janela m�o ser� ativada.
			
			if ( window->parent != NULL && 
			     window->parent != gui->main && 
			     window->parent != gui->screen )
			{
				
	            set_active_window (window->parent);
                goto exit;				
			}

            //??
            //return; 			
		}
	};
	//Nothing.
	
    	
	
//desativar a antiga janela ativa.
exit:

    old = (void *) windowList[save];
	
	if ( (void *) old != NULL )
	{
		if ( old->used == 1 && old->magic == 1234 )
		{
            old->relationship_status = (unsigned long) WINDOW_REALATIONSHIPSTATUS_BACKGROUND;				
		    old->active = 0;
		}
		
	};

    return;	
};


/*
 * change_active_window:
 *     @todo: Trocar a janela ativa
 */
void change_active_window (int id){
	
	// @todo: Limits. Max.
    if(id < 0){
	    return;
	};
	active_window = (int) id;
};


/*
 * show_active_window:
 *     Mostra o id da janela ativa.
 *     @todo: Mudar o nome para windowShowActiveWindowId() 
 */
void show_active_window (){
	
	printf ("ActiveWindowId={%d}\n", (int) active_window);
};

/*
 * show_window_with_focus:
 *     Mostra o id da janela com o foco de entrada..
 */
void show_window_with_focus (){
	
	printf ("wwf_Id={%d}\n", (int) window_with_focus );
};


/*
 *****************************************
 * CloseActiveWindow:
 *     Fecha a janela ativa.
 */
void CloseActiveWindow (){
	
	int Offset;
    struct window_d *Window;
	
    Offset = (int) get_active_window();	
	
	if (Offset < 0)
	{
       return;	
	};
	
	//struct.
	Window = (void *) windowList[Offset];
	
	//Nothing to do.
	if ( (void *) Window == NULL )
	{
        return;
    }else{
	    
		//Torna ativa a pr�xima janela.
	    if ( (void *) Window->next != NULL )
		{
	        set_active_window(Window->next);
	    }else{
	        //Torna ativa a janela m�e.
	        if ( (void *) Window->parent != NULL )
			{
	            set_active_window(Window->parent);
	        };
		};
	};	
	
    DestroyWindow (Window);
};


//
// @todo: windowSetFocus(.) e windowGetFocus() windowSwitchFocus()
//


void windowBlockFocus (){
	
	gFocusBlocked = (int) 1;
};


void windowUnblockFocus (){
	
	gFocusBlocked = (int) 0;
};


/*
 *****************************************************
 * SetFocus:
 *     + Seta o foco em uma janela.
 *     + Configura o cursor.
 *     + Reinicia um buffer.
 *           
 * @todo:
 *     Tentando configurar o cursor dentro da �rea de cliente.
 *     Se a janela for o frame de aplicativo, ent�o o cursor 
 *     fica dentro dos limites da �rea de cliente, mesmo que esteja 
 * em full screen.
 *     Se a janela for um editbox, faz mais sentido ainda que 
 * o corsor fique dentro dos limites da �rea de cliente.
 *
 * #importante:
 *  Isso depende do tipo de janela. Se for um editbox tem que colocar o 
 * cursor no in�cio da �rea de edi��o.
 *
 * ...
 */
 
void SetFocus ( struct window_d *window ){
	
	int i=0;
	int WindowID;
	
	//Imposs�vel mudar o focus.
	//Isso manter� o foco na janela do desenvolvedor
	//durante a fase de cria��o da interface gr�fica.
	//if( _lockfocus == 1){
	//	return;
	//};
	
	// Quando uma janela recebe o foco de entrada, ela vira a janela ativa, 
	// por�m se ela tiver uma janela m�e, ent�o a janela m�e ser� a 
	// janela ativa.
	
    if ( (void *) window == NULL )
	{
		//Isso e recursivo, pode dar problema.
		//SetFocus(gui->screen);    
		goto fail;
	
	} else {
		
		if ( window->used != 1 || window->magic != 1234 )
		{
			goto fail;
		}
		
		
		//#importante:
		//se a estrutura de janel � v�lida, vamos associar 
		//a janela com o foco de entrada � thread atual,
		//pois � ela quem chamou essa rotina. Apesar que 
		//o proprio kernel pode ter chamado isso na inicializa��o,
		//nesse caso a thread ser� inv�lida.
		
		window->control = (struct thread_d *) threadList[current_thread];
		
		if ( (void *) window->control != NULL )
        {
			if ( window->used != 1 || window->magic != 1234 ){
				
			    window->control = NULL;	
			}
		}			
		
		//Salvando id localmente.
		WindowID = (int) window->id; 
			
		//Se a janela j� tem o foco n�o precisa fazer nada.
		if ( window->id == window_with_focus ){
			
			window->focus = 1; 
			
			goto setup_wwf;
		}
			
		//Se a janela � a janela ativa.
		//Ent�o atribuimos o foco e configuramos o procedimento de janela.
		if ( window->id == active_window )
		{
	        //set wwf id.
		    window_with_focus = (int) window->id;

		    //set wwf pointer.
		    WindowWithFocus = (void *) window;
			
			window->focus = 1; 	
		    
			//bugbug
			//Procedure.
		    //?? N�o sei se � o ideal.
		    SetProcedure((unsigned long) window->procedure);
			
			goto setup_wwf;
		};
			
			// Se ela n�o � a janela ativa, tentamos ativar sua janela m�e.
			// Isso se a janela m�o j� n�o for a janela ativa.
            // As janelas filhas nunca s�o janelas ativas. Se uma janela filha 
			// tem o foco de entrada, ent�o sua janela m�e � a ativa. 			
		if( window->id != active_window )
		{
				
			    // Se a janela m�e tem um ponteiro inv�lido. Ent�o ela n�o tem uma 
				// janela m�e, ent�o ativamos a janela filha.
                // ?? #bugbug talvez n�o seja essa ideia certa.			
		    if( (void*) window->parent == NULL ){
		        set_active_window(window);				
		    }else{

					// Testando a validade da janela m�e
				    // Ativar a janela m�e se ela tem um ponteiro v�lido
					// Pois janela filha nunca � a janela ativa, mesmo tendo o foco.
			    if( window->parent->used == 1 && 
				    window->parent->magic == 1234 )
				{
			        set_active_window(window->parent);	
			    };				
			};

                //Obs: Nesse momento a janela ativa est� configurada.
				// a janela ativa � a pr�pria janela ou a sua janela m�e.
                
				
                // J� podemos setar o foco de entrada e configurarmos o 
				// procedimento de janela.				

	            //set wwf id.
			window_with_focus = (int) window->id;
            
            window->focus = 1;   			
		    
			    //set wwf pointer.
			WindowWithFocus = (void *) window;

		        //Procedure.
		        //?? N�o sei se � o ideal.
		    SetProcedure((unsigned long) window->procedure);
			
			set_top_window( (int) window->id );

            goto setup_wwf;				
		};
			
			
//
// Nesse momento a janela j� deve estar com o cofo de entrada,
// se n�o estiver � porque falhamos.
//		

    //if( WindowID != window_with_focus )
    //{
	       //window_with_focus = main->id;;      
    //};	
			

//
// Configurando a janela com o foco de entrada.
//
			
        setup_wwf:		

			// Temos que posicionar o cursor caso a janela seja um editbox.	
			
			// Se a janela for um editbox, ela precisa ter seu input resetado..
			// para que o procedimento de janela do kernel possa us�-lo.
			// por enquanto, faremos isso para todos os tipos de janelas.
			
			//
			// Reiniciando as margens para input em janela.
			//
			
			//Resetando o input do procedimento de janela.
			
			// #importante:
			// Salvar a configura��o de cursor espec�fica para essa janela.
			// Pois cada janela vai querer o cursor em um lugar.
			
			// #importante 
			// Tentando colocar o cursor dentro dos limites da �rea de cliente
			// no momento em que setamos o foco.
			
			// Usaremos a �rea de cliente caso ela exista,
			// pois tem janela que simplesmente n�o tem, o ponteiro � nulo.
			
			if ( (void *) window->rcClient != NULL )
			{
			    if( window->rcClient->used == 1 && 
				    window->rcClient->magic == 1234 )
			    {
			        g_cursor_left = (window->rcClient->left/8);
	                g_cursor_top = (window->rcClient->top/8) +1; //?? Ajuste tempor�rio.   
	                g_cursor_right  = g_cursor_left + (window->rcClient->right/8);
	                g_cursor_bottom = g_cursor_top + (window->rcClient->height/8);					
			    }
				 
				// Nothing. 
				
			}else{
				
			    // Configurando o cursor global gerenciado pelo kernel base.
			    g_cursor_left = (window->left/8);
	            g_cursor_top = (window->top/8);   
	            
				g_cursor_right = g_cursor_left + (window->right/8);
	            g_cursor_bottom = g_cursor_top + (window->height/8);			
			};
			
	
            //#importante: 
			//Posicionando o cursor. 
			
			g_cursor_x = g_cursor_left; 
	        g_cursor_y = g_cursor_top; 

            if ( window->isEditBox == 1 )
            {
				//#teste
				//para ficar na segunda linha do editbox.
				//tem que trabalhar isso melhor.
				
				g_cursor_y++; 
			}				

	
	        //configurando o cursor espec�fico da janela com o foco de entrada.
			window->CursorX = g_cursor_x;
			window->CursorY = g_cursor_y;
            window->CursorColor = COLOR_TEXT;
			
			
			// Buffer do controle editbox.
			// Esse buffer pode ser o arquivo usado pelo editbox.
	
	        for ( i=0; i<PROMPT_MAX_DEFAULT; i++ )
	        {
		        prompt[i] = (char) '\0';
		        prompt_out[i] = (char) '\0';
		        prompt_err[i] = (char) '\0';
	        };
            prompt_pos = 0;				
		    
			//... 				
		
	};
	
//...	
done:	
	return;
fail:
    return;	
};


/*
 *************************************************
 * GetFocus: 
 *     Pega o ponteiro para a estrutura da janela 
 * com o foco de entrada.
 */
void *GetFocus (){
	
    return (void *) windowList[window_with_focus];	
};



/*
 ***********************************************
 * windowGetForegroundWindow:
 *     Recupera o handle da janela que o usu�rio 
 * est� trabalhando, ou seja, a janela em primeiro plano.
 */
void *windowGetForegroundWindow (){
	
    return (void *) windowList[window_with_focus];		
};


/*
 **********************************************
 * windowSetForegroundWindow:
 *     Coloca uma janela em primeiro plano para o 
 * usu�rio trabalhar nela.
 *     @todo: aumentar aprioridade da thread que 
 * configurou a janela de primeiro plano.
 *     +o input do teclado deve vi pra essa janela.
 *     +modifica��es visuais deve ocorrer na janela 
 * que est� em primeiro plano, para 
 * que ela se destaque.
 * retorno: 0=OK  , 1=error.
 */
int windowSetForegroundWindow ( struct window_d *window ){
	
	if ( (void *) window == NULL )
	{
		printf("windowSetForegroundWindow: window\n");
		goto fail;
	}else{
		
	    SetFocus(window);
	    set_active_window(window); 
		//...
	};
	
	//@todo: aumentar a prioridade da thread.
done:	
	return (int) 0;
fail:
    return (int) 1;
};


/*
void 
focus( struct window_d * window );

void 
focus( struct window_d * window )
{
    SetFocus(window);	
};

*/


/*
 muda qual sra a janela ativa.
 Obs: Quando mudamos qual ser� a janela ativa
 temos que atualizar qum ser� a janela com o foco de entrada.
 ... e as janes com o foco de entrada devem ficar numa lista.
 .. que provavelmente � a lista de janelas filhas.
void windowSwitchActiveWindow();
void windowSwitchActiveWindow()
{}
*/


/*
 ********************************
 * windowSwitchFocus:
 *     Muda a janela que est� com foco de entrada.
 *     @todo: Selecionar a janela indicada como next 
 * na estrutura.
 *     Em cada momento o foco est� em uma lista de janelas
 * e quando trocamos o foco, percorremos a lista atual.
 * mas a lista muda dependendo do ambeinte gr�fico que 
 * estamos.
 * Muda quando trocamos a janela ativa, muda quando 
 * trocamos o desktop.
 * 
 */
 
void windowSwitchFocus (){
	
	int Max;
	int CurrentID;
	int NextID;

    struct window_d *window;
	struct window_d *next;	
	//...
	
	//Max e Current.
	Max     = (int) windows_count;
	CurrentID = (int) window_with_focus;	//salva
	
	//
	// @todo: 
	// Essa rotina precisa ser refeita. 
	// Seguindo uma lista linkada de janelas.
	//
	
	window = (void *) windowList[window_with_focus];	
	
	if ( (void *) window == NULL )
	{
		printf("windowSwitchFocus: window\n");
	    goto fail; 
	}else{
		
	    KillFocus(window);  
		
		//Se a pr�xima janela � v�lida.
		if( (void*) window->next != NULL )
		{
			//Get next.
		    next = (void*) window->next;  
		   	    
			//Current id.	
		    NextID = (int) next->id;     
		   
		    //Se estiver dentro dos limites usaremos a pr�xima.
		    if(NextID > 0 && NextID < Max){
		        window_with_focus = (int) NextID;
	        }else{
			    window_with_focus = (int) CurrentID;	
			}			

		    window = (void*) windowList[window_with_focus];			
            SetFocus(window);
			goto done;			
		}else{
		    window_with_focus = (int) CurrentID;	
		    window = (void*) windowList[window_with_focus];			
            SetFocus(window);
            goto done;  			
		};
		//Fail.
	};
	//Nothing.
done:
    redraw_window(window,1);
	
    //Debug message:  
	//@todo: Criar mecanismo melhor pra mostrar a janela com o foco.
    //printf("Focus={%d}\n", (int) window_with_focus);
	//refresh_screen();
	
	return;
	
fail:
    return;
};


/*
 *************************************
 * KillFocus:
 *     Uma janela perde o foco.
 */
 
void KillFocus ( struct window_d *window ){
	
	if( (void *) window == NULL )
	{
		printf("KillFocus: window\n");
	    goto fail; 
	};
	
	//Focus.
	//window->focus = (int) 0;
    
	// ?? @todo: Quem deve receber o foco nesse caso ??
	// ?? um �ndice antes no zorder ??
	
	if( (void *) gui->main != NULL )
	{
		if( gui->main->used == 1 && 
		    gui->main->magic == 1234 )
		{
	        window_with_focus = (int) gui->main->id;
		};
	}
	//
	// Pra onde vai o foco ??
	//
	
	//set next window
	if ( (void *) window->parent != NULL )
	{
		if( window->parent->used == 1 && 
		    window->parent->magic == 1234 )
		{
		    SetFocus(window->parent); 	
		}; 	    
    };	
	
	// Nothing.
	
done:
	return;
fail:
    return;
};


/*
 ******************************************
 * MinimizeWindow:
 *     Minimiza uma janela.
 *     @todo windowMinimize()
 */
 
void MinimizeWindow (struct window_d *window){
	
	int Status;
	
    if( (void *) window == NULL )
	{
		printf("MinimizeWindow: window\n");
	    return;
		//goto fail;
		
	}else{
		
	    Status = (int) is_window_minimized (window);
        if(Status == 1)
	    {
		    printf ("MinimizeWindow: Status\n");
	        return;
			//goto fail; 
	    };	
		
        KillFocus(window);
	    window->view = (int) VIEW_MINIMIZED;		
	};
};


/*
 **************************************
 * MaximizeWindow:
 *     Maximiza uma janela.
 *     @todo: windowMazimize()
 */
void MaximizeWindow (struct window_d *window){
	
	int Status;
	
    if( (void *) window == NULL)
	{
		printf("MaximizeWindow: window\n");
	    goto fail; 
	};
	
	Status = (int) is_window_maximized(window);
    if(Status == 1)
	{
		printf("MaximizeWindow: Status\n");
	    goto fail; 
	};

    //redimensionar.
	if( gui->main != NULL )
	{
        window->left = gui->main->left;             
        window->top = gui->main->top;                   
	    
		window->width = gui->main->width;             
        window->height = gui->main->height;
	}; 	

	//
	//@todo: Redraw ?!
	//
	
done:
    set_active_window(window);
	SetFocus(window);
	window->view = (int) VIEW_MAXIMIZED;
    return;
fail:
    return;	
};


/*
 *************************************************
 * init_window_manager:
 *     Inicializa o gerenciamento de janelas.
 *     @todo windowmanagerInit()
 */
 
int init_window_manager (){
	
    // Aloca mem�ria para a estrutura do procedimento 
	// de janela da thread atual.

    // #bugbug 
    // N�o sei se estamos realmente usando isso.	
	
	WindowProcedure = (void *) malloc ( sizeof( struct window_procedure_d ) );
	
	if ( (void *) WindowProcedure == NULL )
	{
	    printf("init_window_manager: WindowProcedure\n");
		die();
		
	}else{
		
 	    // Configura a janela ativa. 
	    // Configura a janela com o foco de entrada. 
	    // Se a janela com o foco de entrada for uma 
		// janela filha,ent�o a janela m�e ser� a 
		// janela ativa.
	    	
	    WindowProcedure->active_window = (int) 0;
	    WindowProcedure->window_with_focus = (int) 0;
		//...
	};


	// @todo:  
	//     Continua fazendo inicializa��es de 
	// procedimento de janela.	
	
//done:

//#ifdef KERNEL_VERBOSE
//    printf("done\n");
//#endif

    return (int) 0;
};


/*
 *****************************************
 * init_windows:
 *     Inicializa a lista de janelas.
 *     Inicializa globais relativas � janelas.
 */
 
int init_windows (){
		
	// #debug
	// Inicializa a lista de janelas.
	// printf("init_windows:\n");  	
	
	int Offset = 0;
	int k = 0;
	
	for ( Offset=0; Offset < WINDOW_COUNT_MAX; Offset++ ){
		
	    windowList[Offset] = (unsigned long) 0;
	}
	
	windows_count = 0;
	
	// Set current.
	set_current_window(NULL);
	
	//
	// ## Client Area ##
	// Inicializando a estrutura do ret�ngulo da �rea de cliente.
	//
	
	rectClientArea = (void *) malloc ( sizeof(struct rect_d) );
    
	if ( (void *) rectClientArea == NULL )
	{	
	    printf("init_windows: rectClientArea\n");
		die();
		
	} else {
	    setClientAreaRect( 0, 0, 0, 0);	
	};
	
	
	//
	// # cursor #
	// Desabilitar o cursor de textos.
	// Cada aplicativo habilita quando for usar.
	//
		
	//gwsDisableTextCursor ();
	//timerShowTextCursor = 0; 
    timerDisableTextCursor ();	
	
	
	//
    // Set fonts. @todo: Criar uma estrutura para 
	// caracter�sticas do char.
	//
		
	// 8x8 
	// 0x000FFA6E;    //ROM.
	g8x8fontAddress  = (unsigned long) BIOSFONT8X8; 
	//g8x16fontAddress = (unsigned long) 0x000FFA6E; 
	//...
	
		//#bugbug: 
		//Na verdade window.c n�o tem acesso a essa vari�vel,
		//� preciso chamar o servidor atrav�s de um m�todo para 
        //configur�-la.
        //@todo: gwsSetCurrentFontAddress( g8x8fontAddress );
	gws_currentfont_address = (unsigned long) g8x8fontAddress;
	
	//
	// Char parameters.
	//
	
	//@todo: Create SetCharParam(.,.)
	gcharWidth = (int) 8;
	gcharHeight = (int) 8;	
	
	//...	
	
	//
	// @todo:
	// Pega o video mode passado pelo Boot Loader e 
	// registra na global e na estrutura.
	//
	
	/*
	
	//video mode
	if( kArg1 > 0)
	{
	    printf("init_windows: Configurando modo de video.");
	    SetVideoMode(kArg1);
	};
	
	//LFB
	if( kArg2 != LFB_BASE)
    {
        printf("init_windows: LFB error");
		//@todo hang
    };	
	
	*/
	
	//Inicializando vari�veis.
	current_room = 0;
	current_desktop = 0;
	
	current_window = 0;
	current_menu = 0;
	
	// ## Contagens ##
    windowstations_count = 0;  //room counts
    desktops_count = 0;
	windows_count = 0;
	
	fullscreen_window = 0;
	active_window = 0;
	window_with_focus = 0;
	mouseover_window = 0;
	top_window = 0;
	editbox_window = 0;
	combobox_window = 0;
	terminal_window = 0;
    //Continua...		

	
	//inicializando os esquemas de cores.
	//@todo: Isso poderia ter um argumento, selecionado entre o 
	//tipo 1 e 2, humility e pride.
	//ColorSchemeHumility ou ColorSchemePride
	windowSetUpColorScheme(ColorSchemeHumility);
	//windowSetUpColorScheme(ColorSchemePride);	
	
	//input buffer support ??
	//output buffer support ??
	
	
	//inicializar as estruturas de backbuffer e frontbuffer.
	
//
// Backbuffer support. (espelho da mem�ria de video)
//

//BackBufferSupport:
	
	BackBufferInfo = (void *) malloc ( sizeof(struct backbufferinfo_d) );
    
	if ( (void *) BackBufferInfo == NULL )
	{
	    //goto fail;
	}else{
		
	    BackBufferInfo->used = 1;
        BackBufferInfo->magic = 1234;
        //BackBufferInfo->start = ?
        //BackBufferInfo->end = ?
        //BackBufferInfo->size = ?
        //...		
	};		
	

//
// Frontbuffer support. (mem�ria de v�deo)
//
	
//FrontBufferSupport:

	FrontBufferInfo = (void *) malloc ( sizeof(struct frontbufferinfo_d) );
    
	if ( (void *) FrontBufferInfo == NULL )
	{	
	     //goto fail;	
	}else{
		
		//Algumas informa��es foram enviadas pelo boot loader.
	    FrontBufferInfo->used = 1;
        FrontBufferInfo->magic = 1234;
        //FrontBufferInfo->start = ?
        //FrontBufferInfo->end = ?
        //FrontBufferInfo->size = ?	

        //FrontBufferInfo->width = ?	
        //FrontBufferInfo->height = ?	
        //FrontBufferInfo->bpp = ?			
		
        //... 

        //?? console ??		
	};		

	
	/*
	  test - testando queue para teclado.
	 ************************************
	 */
	 
	//inicializando a fila do sistema usada pelo teclado. 
	//initializeQueue(&myQueue); 
	 
	 
	keyboard_message_head = 0;
    keyboard_message_tail = 0;	
	
    //inicializando fila de teclado.
	
	//#todo:
	//size max.
	
	for ( k=0; k<128; k++ )
	{
		keybuffer[k] = 0; 
	}
	
	keybuffer_tail = 0;
	keybuffer_head = 0;

	
	//
	// #importante:
	// Nesse momento n�o existem estruturas prontas 
	// para carregar arquivos.
	//
   		
	//
	// Continua ...
	//
	
done:	
    //printf("Done.\n");
	return (int) 0;
fail:
	return (int) 1;
};


//pegando a z-order de uma janela.
int get_zorder ( struct window_d *window ){
	
    if ( (void *) window != NULL )
	{
		return (int) window->zIndex;
	}

	return (int) -1;
};


	//refletindo ??
    //se um argumento for passado teremos que examinar a ordem das janelas filhas.	
	//sendo assim cada estrutura de janela precisa de uma lista de janelas filhas 
	// e a zorder delas ... 
	//
	//se nenhuma janela for passada como argumento 
	// teremos que olhar as janelas filhas da janela principal que � a gui->main ...
	//a janela gui->main pode ser a janela mae de todas as outras ...
	// sendo a janela gui->main a janela principal do processo kernel.
	//o processo explorador de arquivos poder� ser o processo que 
	//seja a m�e de todos os aplicativos de usu�rio ... ent�o a 
	//z order na estrutura da janela desse aplicativo indica a ordem 
	//das janelas principais dos seus processos filhos ...
	//??
	// Uma estrutura de janela pode ter uma vari�vel top_window indicando o id 
	// da janela que est� no topo da z-order da janela.
	//
	
//pegando a o id da janela que est� no topo da lista de uma janela.

struct window_d *getTopWindow (struct window_d *window){
	
	if ( (void *) window == NULL )
	{
		return (struct window_d *) windowList[top_window];
	};
	
	return NULL;
}; 


int get_top_window (){
	
	return (int) top_window;
};


//Setando a top window.
void set_top_window (int id){
	
	top_window = (int) id;
};


//fecha a janela ativa.
void closeActiveWindow (){
	
    int ID;
	struct window_d *w;
	
	ID = get_active_window();
	
	w = (void *) windowList[ID];
	
	CloseWindow (w);
};


//encontrando um slot livre na z-order global de 
//overlapped windows.

int z_order_get_free_slot (){
	
	int response;
	int z; 
	struct window_d *zWindow;

	for( z=0; z < ZORDER_COUNT_MAX; z++ )
	{
	    zWindow = (void*) zorderList[z];
        
		//Obtendo um espa�o vazio.
		//Se for NULL, ent�o n�o tinha um ponteiro no slot.
		if( (void*) zWindow == NULL )
		{
			response = (int) z; 
			
			zorderCounter++;
			if(zorderCounter >= ZORDER_COUNT_MAX){
				printf("z_order_get_free_slot: zorderCounter\n");
				goto fail;
			}
            
			goto done; 				
		};		
		//Nothing
	};
	
fail:
    return (int) -1;
done:
    return (int) response;	
};


/*
 ***********************************
 * windowLoadGramadoIcons:
 *     Carrega alguns i�cones do sistema.
 *     Obs: Isso funcionou bem.
 */
 
int windowLoadGramadoIcons (){
	
	unsigned long fRet;
	
	//#debug
	//printf("windowLoadGramadoIcons:\n");
	
	//
	//  ## Icon support ##
	//
	
//iconSupport:	

	// Carregando alguns �cones b�sicos usados pelo sistema.
	
	// ## size ##
	// Vamos carregar �cones pequenos.
	//@todo checar a validade dos ponteiros.
    
	appIconBuffer      = (void *) allocPages (4);
	fileIconBuffer     = (void *) allocPages (4);
	folderIconBuffer   = (void *) allocPages (4);
	terminalIconBuffer = (void *) allocPages (4);
	cursorIconBuffer   = (void *) allocPages (4);
	//...
	
	if ( (void *) appIconBuffer == NULL )
	{
		printf("init_windows: appIconBuffer\n");
	    die();	
	}

	if ( (void *) fileIconBuffer == NULL )
	{
		printf("init_windows: fileIconBuffer\n");
		die();
	}

	if ( (void *) folderIconBuffer == NULL )
	{
		printf("init_windows: folderIconBuffer\n");
		die();
	}

	if ( (void *) terminalIconBuffer == NULL )
	{
		printf("init_windows: terminalIconBuffer\n");
		die();
	}
	
	if ( (void *) cursorIconBuffer == NULL )
	{
		printf("init_windows: cursorIconBuffer\n");
		die();
	}	
	
	//
	// # Load #
	//

    fRet = (unsigned long) fsLoadFile ( VOLUME1_FAT_ADDRESS,
							   VOLUME1_ROOTDIR_ADDRESS, 
						       "APP     BMP", 
	                           (unsigned long) appIconBuffer );	
    if ( fRet != 0 )
	{
		printf ("init_windows: APP.BMP\n");
		die();		
	}		
    
	fRet = (unsigned long) fsLoadFile ( VOLUME1_FAT_ADDRESS,
						       VOLUME1_ROOTDIR_ADDRESS, 
							   "FILE    BMP", 
	                           (unsigned long) fileIconBuffer );
    if ( fRet != 0 )
	{
		printf("init_windows: FILE.BMP\n");
		die();				
	}
	
	fRet = (unsigned long) fsLoadFile ( VOLUME1_FAT_ADDRESS, 
						       VOLUME1_ROOTDIR_ADDRESS, 
							   "FOLDER  BMP", 
	                           (unsigned long) folderIconBuffer );	
    if ( fRet != 0 )
	{
		printf("init_windows: FOLDER.BMP\n");
		die();				
	}
	
	fRet = (unsigned long) fsLoadFile ( VOLUME1_FAT_ADDRESS, 
						       VOLUME1_ROOTDIR_ADDRESS, 
							   "TERMINALBMP", 
	                           (unsigned long) terminalIconBuffer );	
    if ( fRet != 0 )
	{
		printf("init_windows: TERMINAL.BMP\n");
		die();				
	}	
	
	fRet = (unsigned long) fsLoadFile ( VOLUME1_FAT_ADDRESS, 
						       VOLUME1_ROOTDIR_ADDRESS, 
							   "CURSOR  BMP", 
	                           (unsigned long) cursorIconBuffer );	
    if ( fRet != 0 )
	{
		printf("init_windows: CURSOR.BMP\n");
		die();				
	}		
	
	// #debug
    // bmpDisplayBMP( appIconBuffer, 1, 1 );
	// refresh_rectangle( 1, 1, 16, 16 );
	// die();	
	
    return 0;
}


/*
 ************************* 
 * windowScan:
 *     Escaneia as janelas existentes procurando uma 
 * que contenha o posicionamento do cursor.
 * Ou seja, o posicionamento do cursor deve estar dentro 
 * da �rea da janela.
 *
 * Obs:
 *    A fun��o retorna na primeira janela encontrada.
 *    Mas dever�amos observar se essa janela possui 
 * janelas filhas, pois o ponteiro pode passar em cima 
 * de uma janela que est� dentro de outra janela.  
 */
 
int windowScan ( unsigned long x, unsigned long y ){

	struct window_d *w;
	
	//register int i;
	int i = 0;
	int WID;
	

	
	// #bugbug
	// Nesse for, o n�mero de compara��es � insuficiente.
	// Precisamos sondar toda a lista de janelas.
	// pois pode haver fragmenta��o na lista de janelas.
	// Ent�o mesmo com uma contagem baixa de janelas pode haver 
	// janelas v�lidas em posi��es altas.
	// Ent�o vamos sondar por toda a lista.
	
	//for ( i=0; i <= windows_count; i++ )
	for ( i=0; i < WINDOW_COUNT_MAX; i++ )	
	{
		w = (struct window_d *) windowList[i];
		
		//Ignorando as janelas principais.
		if ( (void *) w != NULL )
		{	
			if ( w->used == 1 && w->magic == 1234 )
			{
				
				if ( x > w->left && 
				     x < w->right && 
				     y > w->top &&
				     y < w->bottom )
				{
                    if( w->type == WT_EDITBOX ||
                        w->type == WT_OVERLAPPED ||	
                        w->type == WT_CHECKBOX ||	
                        w->type == WT_SCROLLBAR ||	
                        w->type == WT_EDITBOX_MULTIPLE_LINES ||	
						w->type == WT_BUTTON ||	 
                        w->type == WT_STATUSBAR )						
					{						
					   WID = w->id;
					   window_mouse_over = w->id;
					   goto done;
					}
				}; 
			};
				
		};
		
	};
	
fail:
    return (int) -1;	
done:	
    return (int) WID;	
};


//Envia uma mensagem PAINT para o aplicativo atualizar a �rea de trabalho.
void windowUpdateWindow ( struct window_d *window ){
	
    windowSendMessage ( (unsigned long) window, (unsigned long) MSG_PAINT, 
	    (unsigned long) 0, (unsigned long) 0 );	
};



//
// ## Full screen support ##
//


//salvando os valores usados para configura o modo 
//full screen
unsigned long save_client_rect_left;
unsigned long save_client_rect_top;
unsigned long save_client_rect_width;
unsigned long save_client_rect_height;

int fsStatus;

// uma janela entra ou sai do modo full screen.
// @todo: isso pode ir pra outro lugar, provavelmente gws.
int windowSwitchFullScreen (){
	
	struct window_d *window;
	
	//1 = entrar ; 0 = sair.
	int flag;
	
	// #debug
	// printf ("windowSwitchFullScreen: Initializing ...\n");
	
	//#teste
	
	//se a janela ativa j� � a fullscreen 
	//ent�o � hora de sair.
	
	if ( fsStatus == 1 )
	{
	    //sair
		current_window    = active_window;
	    window_with_focus = active_window;
	    fullscreen_window = 0;		
		flag = 0;
		fsStatus = 0;
		goto doswitch;
	}
	
	
	//selecionando a janela ativa para entrar em fullscreen.
	
	//entrar.
	current_window    = active_window;
	window_with_focus = active_window;
	fullscreen_window = active_window;
	flag = 1;  
    fsStatus = 1;
	
doswitch:
	
	
	//
	// # Window #
	//
	
	window = (struct window_d *) windowList[current_window];

    if ( (void *) window ==  NULL  )
    {
		printf("windowSwitchFullScreen: window\n");
		goto fail;
		
	}else{
		
		if ( window->used != 1 || window->magic != 1234 )
		{
			printf("windowSwitchFullScreen: window validation\n");
			goto fail;
		}
		
		//validade da estrutura do ret�ngulo do cliente.
		if ( (void *) window->rcClient == NULL )
		{
			printf("windowSwitchFullScreen: rcClient\n");
			goto fail;
		}else{
			
			if( window->rcClient->used != 1 || 
			    window->rcClient->magic != 1234 )
			{
			    printf("windowSwitchFullScreen: rect validation\n");
			    goto fail;						
			}
			
				//entrar
			    if( flag == 1 )	
				{
					printf("flag1\n");
					refresh_screen();
					//salvamos o antigo ret�ngulo.
					save_client_rect_left   = window->rcClient->left;
					save_client_rect_top    = window->rcClient->top;
					save_client_rect_width  = window->rcClient->width;
					save_client_rect_height = window->rcClient->height;
					
				    window->rcClient->left = (unsigned long) 0;
				    window->rcClient->top  = (unsigned long) 0;
				    window->rcClient->width  = (unsigned long) systemGetSystemMetrics( 1 ); //screen width. 1
				    window->rcClient->height =  (unsigned long) systemGetSystemMetrics( 2 ); //screen height.2
			        
                    window->view = VIEW_FULL;
					
                    backgroundDraw( (unsigned long) window->rcClient->bg_color );  
					refresh_screen();					
				};
			
				//sair
			    if( flag == 0 )	
				{
					printf("flag0\n");
					refresh_screen();
					//Nesse caso temos que recuperar os valores  
					//do ret�ngulo da �rea de cliente.
				    window->rcClient->left   = (unsigned long) save_client_rect_left;
				    window->rcClient->top    = (unsigned long) save_client_rect_top;
				    window->rcClient->width  = (unsigned long) save_client_rect_width; 
				    window->rcClient->height = (unsigned long) save_client_rect_height; 
			    
				    window->view = VIEW_NORMAL;				
					
					//flag = 1 ( refresh rectangle.)
					redraw_window ( (struct window_d *) window, 1 );
				};			
			
		};
		
		//Isso tamb�m atualiza o cursor.
		//pode ser um problema quando sa�mos do modo fullscreen.
		SetFocus (window);		
		 
	};		
	
	
    ldisc_init_modifier_keys();
	ldisc_init_lock_keys();
		
	//refresh_screen();

done:
    printf("DONE\n");
    refresh_screen();
    return 0; 	
fail:
    printf("FAIL\n");
    refresh_screen();
    return 1;
};

//

int scroll_client_window ( struct window_d *window )
{
/*
   //#bugbug: Page Fault.. we need to work on this thing.
   //but its a good code.:)   
    
	//hWnd = (struct window_d *) arg2;
    if ( (void *) window != NULL )
    {	
                    //#bugbug: temos que checar mais validade de estrutura.
					
					//obs: No come�o dessa fun��o, colocamos o arg3 como ponteiro para a3.
					//um buffer de longs.
					
	    unsigned long left = (unsigned long) window->rcClient->left;
	    unsigned long top = (unsigned long) window->rcClient->top;     
	    unsigned long width = (unsigned long) window->rcClient->width;
	    unsigned long height = (unsigned long) window->rcClient->height;
	    //a3[4] = (unsigned long) hWnd->rcClient->bg_color;
					//...
				
		save_rect (  left,  top, width,  height );
		
		top = top +8;
		show_saved_rect (  left,  top, width,  height );
	}

*/	
	return 0;
};

//
// End.
//

