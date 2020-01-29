/*
 * File: kgwm/kgwm/ww.c 
 *
 *  wm. - Window Manager.
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
 * #todo: 
 * Criar uma fun��o que copie o conte�do de um buffer dedicado de janela para
 * o LFB,(FrontBuffer).
 *       
 *
 * Hist�rico:
 *     Vers�o: 1.0, 2013 - Esse arquivo foi criado por Fred Nora.
 *     Vers�o: 1.0, 2014 - Aprimoramento geral das rotinas b�sicas. 
 *     Vers�o: 1.0, 2015 - Revis�o. 
 *     Vers�o: 1.0, 2016 - Revis�o.
 *     ...
 */
 
//#obs:
// Tem coisas aqui que pertencem ao window server e
// coisas que pertencem ao window manager ou s�o de suporte ao window manager. 

 
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
//wm.
int save_window (struct window_d *window){

    if ( (void *) window == NULL )
    {
		//#debug
        printf ("save_window: struct\n");
        return 1;

    }else{
	
        if ( window->used != 1 || window->magic != 1234 )
        {
			//#debug
            printf ("save_window: validation\n");
            return 1;
         }

        //#debug
        //Ok. est� pegando os valores certos.
        //printf ("l=%d t=%d w=%d h=%d \n", window->left, window->top, window->width, window->height );
        //refresh_screen();
        //while(1){}


        save_rect ( window->left, window->top, window->width, window->height );
    };
}


//mostrar uma janela que tem seu ret�ngulo salvo no buffer de salvamento.
//#obs: antes de chamarmos essa fun��o podemos chamar a fun��o replace_window
//para mudar a janela de lugar.

//wm.

int show_saved_window (struct window_d *window){
	
    if( (void *) window == NULL )
    {
		return 1;

	}else{
	
        if ( window->used != 1 || window->magic != 1234 )
        {
			return 1;
		}

		show_saved_rect ( window->left, window->top, 
		    window->width, window->height );
        
        //... 		
	};		
}


/*
 ***********************************************
 * show_window_rect:
 *     Mostra o ret�ngulo de uma janela que est� no backbuffer.
 *     Tem uma janela no backbuffer e desejamos enviar ela para o frontbuffer.
 *     A rotina de refresh rectangle tem que ter o vsync
 *     #todo: criar um define chamado refresh_window.
 */

int show_window_rect (struct window_d *window){
	
    if ( (void *) window == NULL )
    {
		//#debug
		printf ("show_window_rect: window\n");
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

			    //window->width = window->width +4;
				//window->height = window->height +4;
			    
				//refresh_rectangle ( window->left, window->top, 
				//    window->width +2, window->height +2 ); 
				//return (int) 0;
			//}
			
            refresh_rectangle ( window->left, window->top, 
                window->width, window->height ); 

			return 0;
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
}


/*
 *windowSetTimer:
 * 
 */
// A janela receber� uma mensagem MSG_TIMER 
// sempre que o contador chegar a '0'.
// +Essa rotina deve utilizar o driver de timer 
// para conseguir a contagem que precisa.
// #teste, come�aremos contagem simples para teste.

// IN:
// janela, id do timer, tempo (medida indefinida ainda)

int
windowSetTimer ( struct window_d *window, 
                int id,  
                int time )  
{
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
}


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
//wm.

int windowGetWindowID ( struct window_d *window ){
	
	if ( (void *) window != NULL )
	{
		if ( window->used == 1 && window->magic == 1234 )
		{	
		    return (int) window->id; 	
		}
	};

	return (int) -1;
}


// Pegar o id da janela main.
// Para os aplicativos lidarem com a �rea de trabalho. 

// wm.

int windowGetMainWindowDescriptor (void){
	
	if ( (void *) gui == NULL )
	{
		goto fail;
	
	}else{
		
		if ( (void *) gui->main == NULL )
		{
		    goto fail;
		}
		
		return (int) gui->main->id;
	};
	
fail:	
	
	return (int) -1;
}


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
//wm.
void *GetParentWindow (struct window_d * hwnd){
	
	if ( (void *) hwnd == NULL )
	{
		return NULL;
	}
	
	return (void *) hwnd->parent;
}


//get the desktop of a window.
//wm.
void *GetWindowDesktop (struct window_d * hwnd){
	
	if ( (void *) hwnd == NULL )
	{
		return NULL;
	}
	
	return (void *) hwnd->desktop;
}


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
    humility = (void *) kmalloc ( sizeof(struct color_scheme_d) );
    
	if( (void *) humility == NULL )
	{
		panic ("windowSetUpColorScheme: humility");
		
	}else{
		
		//Object.
		humility->objectType = ObjectTypeColorScheme;
		humility->objectClass = ObjectClassGuiObjects;
		

		humility->used = 1;
		humility->magic = 1234;
		humility->name = "Humility";
		
		//Colors
		//Definidas em ws.h
		humility->elements[csiNull] = 0;                             //0
        humility->elements[csiDesktop] = HUMILITY_COLOR_BACKGROUND;  //1		
		humility->elements[csiWindow] = HUMILITY_COLOR_WINDOW;       //2
        humility->elements[csiWindowBackground] = HUMILITY_COLOR_WINDOW_BACKGROUND;	 //3	
		humility->elements[csiActiveWindowBorder] = HUMILITY_COLOR_ACTIVE_WINDOW_BORDER;  //4
        humility->elements[csiInactiveWindowBorder] = HUMILITY_COLOR_INACTIVE_WINDOW_BORDER;  //5		
		humility->elements[csiActiveWindowTitleBar] = HUMILITY_COLOR_ACTIVE_WINDOW_TITLEBAR;  //6
        humility->elements[csiInactiveWindowTitleBar] = HUMILITY_COLOR_INACTIVE_WINDOW_TITLEBAR;  //7	 	
		humility->elements[csiMenuBar] = HUMILITY_COLOR_MENUBAR;                //8
        humility->elements[csiScrollBar] = HUMILITY_COLOR_SCROLLBAR;            //9  
		humility->elements[csiStatusBar] = HUMILITY_COLOR_STATUSBAR;            //10
        humility->elements[csiMessageBox] = HUMILITY_COLOR_MESSAGEBOX;		    //11
		humility->elements[csiSystemFontColor] = HUMILITY_COLOR_SYSTEMFONT;		//12
		humility->elements[csiTerminalFontColor] = HUMILITY_COLOR_TERMINALFONT;	//13
		//...
		
		//Sanvando na estrutura padr�o para o esquema humility.
		HumilityColorScheme = (void*) humility;
	};	
	
	//
	// * PRIDE 
	//
	
    //Criando o esquema de cores PRIDE. (colorido)
    pride = (void *) kmalloc ( sizeof(struct color_scheme_d) );
    
	if ( (void *) pride == NULL )
	{
		panic ("windowSetUpColorScheme: pride");
	}else{
		
		//Object.
		pride->objectType = ObjectTypeColorScheme;
		pride->objectClass = ObjectClassGuiObjects;

		pride->used = 1;
		pride->magic = 1234;
		pride->name = "Pride";
		
		//Colors
		//Definidas em ws.h
		pride->elements[csiNull] = 0;
        pride->elements[csiDesktop] = PRIDE_COLOR_BACKGROUND;  
		pride->elements[csiWindow] = PRIDE_COLOR_WINDOW;
        pride->elements[csiWindowBackground] = PRIDE_COLOR_WINDOW_BACKGROUND;
		pride->elements[csiActiveWindowBorder] = PRIDE_COLOR_ACTIVE_WINDOW_BORDER;  
        pride->elements[csiInactiveWindowBorder] = PRIDE_COLOR_INACTIVE_WINDOW_BORDER;  
		pride->elements[csiActiveWindowTitleBar] = PRIDE_COLOR_ACTIVE_WINDOW_TITLEBAR;    
        pride->elements[csiInactiveWindowTitleBar] = PRIDE_COLOR_INACTIVE_WINDOW_TITLEBAR;		
		pride->elements[csiMenuBar] = PRIDE_COLOR_MENUBAR;
        pride->elements[csiScrollBar] = PRIDE_COLOR_SCROLLBAR;  		
		pride->elements[csiStatusBar] = PRIDE_COLOR_STATUSBAR;    
        pride->elements[csiMessageBox] = PRIDE_COLOR_MESSAGEBOX;
		pride->elements[csiSystemFontColor] = PRIDE_COLOR_SYSTEMFONT;    //12
		pride->elements[csiTerminalFontColor] = PRIDE_COLOR_TERMINALFONT;  //13		
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
}


//seleciona o tipo ...isso � um servi�o.
int windowSelectColorScheme (int type){
	
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

    if ( (void *) PrideColorScheme == NULL )
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
		
	    printf ("Pride selected\n"); 
		CurrentColorScheme = PrideColorScheme;	
	    goto done;
	};		

done:

    return 0;	
    
fail:

    printf ("fail\n");
    return 1;
}


/*
 * windowShowWWFMessageBuffers:
 *     Mostra o buffer de mensagens da janela com foco de entrada.
 *     #Rotinas de teste. @todo: Enviar para a pasta /test.
 *     Esse rotina funcionou e se mostrou bem �til.
 */

// #debug stuff.

void windowShowWWFMessageBuffers (void){

	struct window_d *wFocus;
	int i = 0;	

    wFocus = (void *) windowList[window_with_focus];

	if ( (void *) wFocus == NULL )
		return;
	
	//Obs: Tipo.
	
	for ( i=0; i<32; i++ )
	{
	    printf ("%d ", wFocus->msgList[i]);
    };	
}


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

// window server stuff. ipc.

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

//#suspensa: deletar.
 
void *windowGetHandleWindow (struct window_d *window)
{
   return NULL;
}


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
 
//#suspensa ?
void *windowGetMessage (struct window_d *window){
	
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
	
	if ( keybuffer_head >= 128 ){
	    keybuffer_head = 0;
	}
		
	//#bugbug
	//alguma coisa est� imprimindo o char duas vezes ...
	//Talvez o pr�prio aplicativo esteja imprimeiro tamb�m.
	//
	
    //isso coloca a mensagem na fila da thread atual.
	
	KEYBOARD_SEND_MESSAGE ( SC );	
	

	
	wFocus = (void *) windowList[window_with_focus];
	
	if( wFocus->newmessageFlag == 0 ){
		return NULL;
	}
	
	return (void *) wFocus->msg;
	
	//Nothing.

//fail:

    return NULL;
}


//#suspensa ?
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
}


//#suspensa: deletar.
void *windowGetLong2 (struct window_d *window)
{
	return NULL;	
}


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
 
// window server. low level.
//pode ser que esse aplicativo n�o tenha janela,
//mas esteja rodando na janela do shell.
	 
int window_getch (void){

	struct window_d *wFocus;
	unsigned char SC;
	int save;
	
	// Bloqueia pra que nenhum aplicativo pegue mensagens 
	// na estrutura de janela at� que window_getch termine.
	
	//window_getch_lock = 1;
	
	// Pegamos um scancode na fila do teclado,
	// transformamos ela em mensagem e colocamos a 
	// mensagem na estrutura da janela com o foco de entrada.
	
	// #bugbug
	// Rever esse buffer. Talvez as coisas mudaram um pouco por l�.

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
	
	KEYBOARD_SEND_MESSAGE ( SC );	

	
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
		
		if ( wFocus->msg != MSG_KEYDOWN )
		{
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
  //
done:

   // window_getch_lock = 0;
	return (int) -1;    //erro
}



/*
 * windowCreateDedicatedBuffer: 
 *     Cria um buffer dedicado de acordo com as dimens�es da janela.
 */

// #bugbug
// Essas rotinas de buffer dedicado precisam serem revistas. 

// Buffer size.
// Alocando mem�ria no Heap do kernel.
// #todo: 
// Usar vari�vel para quantidade de bytes por pixel. 
 
int windowCreateDedicatedBuffer (struct window_d *window){

	size_t bufferSize = 0;
	
	if ((void *) window == NULL )
	{ 
	    return (int) 1;  
	};
	
	bufferSize = (size_t) (window->width * 3) + (window->height * 3);
	
	if (bufferSize == 0)
	{
	    return (int) 1;    
	
	}else{
		
	    window->DedicatedBuffer = (void *) kmalloc (bufferSize);	
	    
	    if( (void *) window->DedicatedBuffer == NULL )
	    {
		    return (int) 1;    
		}
		
		return 0;
	};
	
//fail:	
	return 1;
}


/*
 * windowLock:
 *     Bloqueia uma janela.
 *     @todo: Quem pode realizar essa opera��o??
 */

// wm.

void windowLock (struct window_d *window){
	
	if ( (void *) window == NULL )
	{
	    return;

    }else{
		
        window->locked = (int) WINDOW_LOCKED;
	};
}


/*
 * windowUnlock:
 *     Desbloqueia uma janela.
 *     @todo: Quem pode realizar essa opera��o??
 */
 
// wm.
 
void windowUnlock (struct window_d *window){
	
	if ( (void *) window == NULL )
	{
	    return;
    
    }else{
		
    	window->locked = (int) WINDOW_UNLOCKED;		
	};
}


/*
 * set_current_window:
 *     @todo: Mudar para windowSetCurrentWindow
 */

// wm.
 
void set_current_window (struct window_d *window){
	
	if ( (void *) window == NULL )
	{ 
	    return; 
	};  
    
	CurrentWindow = (void *) window;
	
	current_window = (int) window->id;
}


/*
 * get_current_window: 
 *     Get current window pointer.
 */

// #bugbug
// Isso � o handle. Mas normalmente manipulamos o descritor.
// wm.

void *get_current_window (void){
		
	return (void *) CurrentWindow;
}


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
 
// window server stuff. 
 
int RegisterWindow (struct window_d *window){

    struct window_d *Empty; 
    int Offset = 0; 

    if ( (void *) window == NULL )
    {
        return (int) 1;
    }


	// Contagem de janelas e limites.
	// (� diferente de id, pois id representa a posi��o
	// da janela na lista de janelas).
	
	windows_count++;
	
	if ( windows_count >= WINDOW_COUNT_MAX )
	{
	    panic ("RegisterWindow: Limits");
	};
    
	// Create empty.	
	
//Loop.	
get_next:

	Offset++;
	
	//Limite da lista, volta ao in�cio da lista.
	
	if (Offset >= WINDOW_COUNT_MAX)
	{
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


    return 0;
}


/*
 * windowShowWindowList:
 *     Show Window List
 *     Mostra a lista de janelas registradas. 
 */
 
//#debug
//usado para debug.
 
void windowShowWindowList (void){

	int i = 0;

	struct window_d *hWindow;
	struct window_d *hWnd;

	unsigned long left;
	unsigned long top;
	unsigned long width;
	unsigned long height;

	//Se n�o estivermos em modo gr�fico, n�o h� o que mostrar.

    if ( VideoBlock.useGui != 1 )
        return;


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
	TTY[current_vc].cursor_x = (left/8);
	TTY[current_vc].cursor_y = (top/8);  
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

        hWindow = (void *) CreateWindow ( 3, 0, VIEW_MAXIMIZED, "window-list", 
                               left, top, width, height, 
                               gui->main, 0, 
                               KERNEL_WINDOW_DEFAULT_CLIENTCOLOR, 
                               KERNEL_WINDOW_DEFAULT_BGCOLOR ); 

        if ( (void *) hWindow == NULL )
        {
            printf ("windowShowWindowList:\n");
            return;
 
        }else{

            RegisterWindow (hWindow);

            set_active_window (hWindow);

            SetFocus (hWindow);
        };


		// Alterando as margens.
		// Essas margens s�o usadas pela fun��o printf.
		// Obs: As medidas s�o feitas em n�meros de caracteres.
		// Obs: @todo: Devemos usar aqui o ret�ngulo da �rea de cliente,
		// e n�o as margens da janela.
		// A estrutura de janela deve nos oferecer os valores para a m�trica do 
		// ret�ngulo da �rea de cliente.
		// Obs: @todo:Acho que essa n�o � a forma correta de configurar isso. Uma 
		//rotina deveria perceber as dimens�es da janela de do caractere e determinar
		//as margens.

		// #bugbug
		// Cuidado para n�o dividir por '0'.

		TTY[current_vc].cursor_left = (hWindow->left/8);
		TTY[current_vc].cursor_top = (hWindow->top/8) + 4;   //Queremos o in�cio da �rea de clente.
		
		TTY[current_vc].cursor_right = TTY[current_vc].cursor_left + (width/8);
		TTY[current_vc].cursor_bottom = TTY[current_vc].cursor_top  + (height/8);
		
		//cursor (0, mas com margem nova).
		TTY[current_vc].cursor_x = TTY[current_vc].cursor_left; 
		TTY[current_vc].cursor_y = TTY[current_vc].cursor_top; 

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
		TTY[current_vc].cursor_left = (left/8);    //0;
		TTY[current_vc].cursor_top = (top/8);        //0;
		
		TTY[current_vc].cursor_right = (width/8);   
		TTY[current_vc].cursor_bottom = (height/8);  
		
		//cursor (0, mas com margem nova)
		TTY[current_vc].cursor_x = TTY[current_vc].cursor_left; 
		TTY[current_vc].cursor_y = TTY[current_vc].cursor_top;
        //set_up_cursor(g_cursor_left,g_cursor_top); 


		StatusBar ( hWindow, "Esc=EXIT","Enter=?" );
		//Nothing.
	};	   

    // Continua ...

    //@todo; 
	//Aqui podemos dar refresh apenas na janela
	
	// #bugbug
	// Isso � lento. Podemos apenas efetura refresh em uma parte.
	
	refresh_screen ();
    
	//SetFocus(hWindow);
}


/*
 *********************************************************
 * redraw_window:
 * 
 *  #todo: Isso deveria ficar no mesmo lugar que createw.
 * 
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
 
// int windowRedrawWindow(struct window_d *window) 

// window server stuff.

//#todo:
// chamar isso quando chamarmos raise_window.

// IN: redesenha a janela, mostra na tela ou n�o.

int redraw_window (struct window_d *window, unsigned long flags ){

    int Status; 
    unsigned long border_size = 0;
    unsigned long border_color = COLOR_BORDER;

    unsigned long __tmp_color;

	
	
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


	// Ok. 
	// Pelo jeito j� temos uma estrutura v�lida.

	//@todo: Pra que serva a flag redraw ??
	// � para indicar que repecisamos repintar. ??
	//if( window->redraw != 1 ){ return (int) 0;}
	
	
	// Minimized ? 
	// Se tiver minimizada, n�o precisa repintar.
	
	Status = (int) is_window_minimized (window);
    
	if (Status == 1)
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
	// Devo lembrar que as caracter�sticas da janela est�o salvas 
	// na estrutura e s�o muitas caracter�sticas. 
	// N�o exige c�lculos, toda a m�trica j� est� pronta.
	// *********************************


	//Buffers ??
	//window->DedicatedBuffer
	//window->BackBuffer
	//window->FrontBuffer
	
	
	// *Importante: 
	// Checando se o esquema de cores est� funcionando.
	// obs: Essa checagem est� se tornando repetitiva.
	
	if ( (void *) CurrentColorScheme == NULL )
	{
		panic ("redraw_window: CurrentColorScheme");
		
	} else {
		
		if ( CurrentColorScheme->used != 1 || 
		     CurrentColorScheme->magic != 1234 )
		{
		    panic ("redraw_window: CurrentColorScheme validation");
		}
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
	// na estrutura de janela, bem como o pr�prio estilo 
	// de design.
	
//redrawBegin:

	//#bugbug
    if ( window->view == VIEW_NULL )
    { goto fail; }



    // Sombra.
    // Para overlapped?
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

			if (window->focus == 1)
			{ __tmp_color = xCOLOR_GRAY1; }    //mais escuro
			if (window->focus == 0)
			{ __tmp_color = xCOLOR_GRAY2; }    //mais claro

			drawDataRectangle ( window->left +1, window->top  +1, 
				window->width  +1 +1, window->height +1 +1, 
				__tmp_color );             
        };
        
        //...                
    };


    // Background.
    // Para todos os tipos.
    if (window->backgroundUsed == 1)
    {
        drawDataRectangle ( window->left, window->top, 
            window->width, window->height, window->bg_color ); 
    };




	// Borda.
	// Para os casos de editbox por exemplo.
	// Para v�rios tipos.
    if ( window->borderUsed == 1 )
    {
		//#importante:
		//devemos tratar a borda para cada tipo de janela individualmente.
		
		//bot�o
		if ( window->type == WT_BUTTON )
		{
			//se o bot�o tiver o foco.
			if ( window->focus == 1 ){
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
			if ( window->focus == 1 ){
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
			if ( window->focus == 1 ){
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
			if ( window->focus == 1 ){
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




    // T�tulo + borda (frame).
    // Repinta ret�ngulo e string para todos os tipos.
    if (window->titlebarUsed == 1)
    { 
		//#importante:
		//@todo:Ainda h� muito o que levar em considera��o 
		//na hora de repintar uma janela com rela��o 
		//ao fato de serem ativas ou n�o ou de terem o foco ou n�o.
 
        //no caso de janela m�e.
        //if (window->id == active_window){
			//window->bg_color = CurrentColorScheme->elements[csiActiveWindowTitleBar];
		//}else{
			//window->bg_color = CurrentColorScheme->elements[csiInactiveWindowTitleBar]; 
		//};
	
		//Focus.
		//Cores diferentes se tiver foco e se n�o tiver.
		//no caso de janelas filhas
		//if (window->id == window_with_focus){
		    //window->bg_color = CurrentColorScheme->elements[csiActiveWindowTitleBar]; 
		//}else{
			//window->bg_color = CurrentColorScheme->elements[csiInactiveWindowTitleBar]; 
		//};
		
		//@todo: String color.
		
		//@todo: String size.
		
		//@todo: Se tiver barra de rolagem, a largura deve ser maior.
		//if()
		
		//Rectangle and string.
		
        drawDataRectangle ( window->left, window->top, 
			window->width +1 +1, window->height +1 +1, 
			window->bg_color );

        // #bugbug
        // na m�quina real, falhou logo ap�s pintar a string da barra de t�tulos.
        // Vamos usar breakpoint at� acharmos o problema.

       // barra de t�tulos; 
       //todo: usar o esquema de cores.       
			if (window->focus == 1)
			{ __tmp_color = xCOLOR_GRAY1; }        // mais escuro
			if (window->focus == 0)
			{ __tmp_color = window->bg_color; }    // escolhida pelo aplicativo;
        
        drawDataRectangle ( window->left +2, window->top +2, 
            window->width -4, 30, 
            __tmp_color );        


		//@todo: 
		//string da barra de t�tulos.
		//Se estivermos em full screen, n�o teremos string.				   
		draw_string ( window->left +8 +16 +8, window->top +8 +4, 
			COLOR_TERMINALTEXT2, window->name );  		 
		
		//#debug
		//printf ("* real machine breakpoint");
		//refresh_screen();
		//while(1){}
		
	    //Isso � um teste.
	    //A janela nem foi registrada ainda e j� estamos passando o handle
	    //via argumento.
		
	    //So criamos o bot�o na barra de t�tulos se tivermos uma barra de t�tulos.
		//ent�o esse � o lugar certo para essa rotina.
		
		// #todo: 
		// Se estivermos em full screen, n�o teremos bot�o.	
		/*
		if (window->minimizebuttonUsed == 1)
		{        
			if ( window->minimize != NULL )
			{
				if ( window->minimize->used == 1 && 
				     window->minimize->magic == 1234 )
				{
			        //draw_button ( window, "V", 
			        //    1, 0, 0, 
			        //    window->minimize->left, window->minimize->top, window->minimize->width, window->minimize->height, 
				    //    window->minimize->bg_color );
				}
				
			}
	    };
		*/
		
		/*
		if (window->maximizebuttonUsed == 1)
		{
			if ( window->maximize != NULL )
			{
				if ( window->maximize->used == 1 && 
				     window->maximize->magic == 1234 )
				{
			        //draw_button ( window, "^", 
			        //    1, 0, 0, 
			        //    window->maximize->left, window->maximize->top, window->maximize->width, window->maximize->height, 
				    //    window->maximize->bg_color );	
				}
				
			}					
		}
		*/
		
		// #todo: 
		// Se estivermos em full screen, n�o teremos bot�o.
	    
	    /*
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
	    */
	    				 
		//...
	};		
	

        //funcionou at� aqui na m�quina real.
		//#debug
		//printf ("* real machine breakpoint");
		//refresh_screen();
		//while(1){}


    // Client Area. 
    // #bugbug
    // Precisamos repintar a client area, mas a rotina de resize
    // n�o modificou a client area. #todo.
	if (window->clientAreaUsed == 1)
	{
		// Obs: 
		// A Client Area � apenas um ret�ngulo ??

		// #todo: 
		// Passar a estrutura de janela.
		
		// #BUGBUG: 
		// ( N�O ) PODEMOS REALOCAR NOVAMENTE ... 
		// TEMOS QUE CHECAR SE A ESTRUTURA � V�LIDA...
		// SE TRATA APENAS DE UMA ESTRUTTURA DE RET�NGULO, 
		// N�O ESPERAMOS MUITOS PROBLEMAS.
		 
		// #bugbug
		// Existem janelas que n�o possuem reClient.
		// Elas n�o merecem falhar; 
		 
		//if ( (void *) window->rcClient == NULL )
		//{
			//panic ("redraw_window: rcClient \n");
			
		//}
			
		if ( (void *) window->rcClient != NULL )
		{		
			//conferir validade.
			if ( window->rcClient->used != 1 || 
			     window->rcClient->magic != 1234 )
			{
			    panic ("redraw_window: rcClient validation \n");
			}
			
			// Aqui devemos respeitar a forma que a �rea de cliente 
			// foi desenhada.
			// Mas todos os valores que precisamos est�o salvos na estrutura 
			// de �rea de cliente.	
			
            drawDataRectangle ( (unsigned long) window->rcClient->left, 
		        (unsigned long) window->rcClient->top, 
				(unsigned long) window->rcClient->width, 
				(unsigned long) window->rcClient->height, 
				(unsigned long) window->rcClient->bg_color );
		};
		
		//Nothing.
	};


	
    // #obs: 
	// Talvez isso pode ficar dentro do if de client window.
    
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
						// #todo										
						// Desenhar o bot�o	 
					}						 
					
				}
				
				
			    if ( window->scrollbar->scrollbar_button2 != NULL )
				{
					if ( window->scrollbar->scrollbar_button2->used == 1 &&
					     window->scrollbar->scrollbar_button2->magic == 1234 )
					{
						// #todo										
						// Desenhar o bot�o	 
					}						 
					
				}
				
				
			    if ( window->scrollbar->scrollbar_button3 != NULL )
				{
					if ( window->scrollbar->scrollbar_button3->used == 1 &&
					     window->scrollbar->scrollbar_button3->magic == 1234 )
					{
						// #todo										
						// Desenhar o bot�o	 
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
		        drawDataRectangle ( window->statusbar->left, 
		            window->statusbar->top, 
			        window->statusbar->width -1, 
			        window->statusbar->height, 
			        window->statusbar->bg_color ); 

		        draw_string ( window->statusbar->left +8, 
		            window->statusbar->top +8, 
		            COLOR_TEXT, window->statusbar->name ); 
			}
		}
	}

       //funcionou ate aqui na maquina real
		//#debug
		//printf ("* real machine breakpoint");
		//refresh_screen();
		//while(1){}


	//
	// ## Button ##
	//
	
	// Vamos apenas repintar a janela tipo bot�o do mesmo jeito que repintamos
	// as outras e apenas atualizar a estrutura se for necess�rio.
	
	if ( window->type == WT_BUTTON )
	{
	    if ( window->isButton == 1 )
        {
			if ( (void *) window->button != NULL )
			{
				if ( window->button->used == 1 && 
				     window->button->magic == 1234 )
				{
					// #importante
				    // Redesenhar o bot�o com base nas informa��es da estrutura.    
					// #todo: Checar sytle state type 
					// #obs: Com focus a borda fica azul. 
					// O redraw pode ter vindo ap�s uma atualiza��o do bot�o.
					
					switch (window->button->state)
					{
						case BS_FOCUS:
						    window->button->border1 = COLOR_BLUE;
						    window->button->border2 = COLOR_BLUE; 
							break;
							
						case BS_PRESS:
						    window->button->border1 = COLOR_BUTTONHIGHLIGHT2;
						    window->button->border2 = COLOR_BUTTONSHADOW2; 
							break;
							
						case BS_DEFAULT:
						    window->button->border1 = COLOR_BUTTONSHADOW2;
						    window->button->border2 = COLOR_BUTTONHIGHLIGHT2; 
							break;
								
						case BS_DISABLED:
						    window->button->border1 = COLOR_GRAY;
						    window->button->border2 = COLOR_GRAY; 
						    window->button->color = COLOR_GRAY;
							break;
							
						case BS_HOVER:
							window->button->color = ( window->button->color - 20);
							break;
							
						//default:
						//	printf ("redraw button default\n");
							//break;
					};
					
	                //bg 
	                drawDataRectangle ( window->left, window->top, 
	                    window->button->width, window->button->height, 
	                    window->button->color );
					
					//board1, borda de cima e esquerda.
	                drawDataRectangle ( window->left, 
	                    window->top, 
	                    window->button->width, 1, 
	                    window->button->border1 );
		
	                drawDataRectangle ( window->left, 
	                    window->top, 
	                    1, window->button->height, 
	                    window->button->border1 );

	                //board2, borda direita e baixo.
	                drawDataRectangle ( window->left + window->button->width -1, 
	                    window->top, 
		                1, window->button->height, 
		                window->button->border2 );
					   
	                drawDataRectangle ( window->left, 
	                    window->top + window->button->height -1, 
		                window->button->width, 1, 
		                window->button->border2 );
				    
					//#todo: if aqui tem duas op��es de draw string.
                    if ( window->button->selected == 1 )
					{
					    draw_string ( window->left +8, 
					        window->top +8, 
			                COLOR_WHITE, window->button->string );
                    }
					
                    if ( window->button->selected == 0 )
                    {
					    draw_string ( window->left +8, 
					        window->top +8, 
			                COLOR_TERMINALTEXT, window->button->string );
					}						
				}				
			}
		}			
	};



    // Icon.
    if (window->type == WT_ICON)
    {
        bmpDisplayBMP ( appIconBuffer, 
            window->left +8, window->top +8 );
            
        draw_string ( window->left +8, window->top +16 +8 +8, 
            COLOR_TERMINALTEXT2, window->name );  
    }



    //#test
    //manda uma mensagem para thread atual
    //para ela mostrar os elementos da �rea de cliente.

    // isso ser� usado na fun��o update window.()
    /*
    // n�o podemos fazer isso para todos os tipos de janelas.
    // pois entraria num loop de recursividade.
    if ( window->type == WT_OVERLAPPED || window->type == WT_SIMPLE )
    {
		// Validade da thread.
        if ( (void *) window->control != NULL )
        {
			// Validade da thread.
			if ( window->control->used == 1 || 
			     window->control->magic == 1234 )
			{
			   // mandamos a mensagem
			   // o aplicativo decide o que fazer com ela.
			    window->control->window = window;
			    window->control->msg = MSG_PAINT;
			    window->control->long1 = 0;
			    window->control->long2 = 0;
			    window->control->newmessageFlag = 1;
			}
		}
    };
    */

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
		// #obs:
		// Quando uma rotina muda a posi��o da janela.
		// E em seguida repinta. Esse ser� o ret�ngulo que 
		// ser� mostrado no front buffer, mesmo se a rotina
		// de repintura falhar.
		
		// #test
		// Vamos mostrar um pouco mais.
		// #todo: Esse extra deve ser do tamanho da sombra.

    if ( flags == 1 )
    {
        refresh_rectangle ( window->left, window->top, 
            window->width +4, window->height +4 );
    }


	//Continua ...

done:
    return 0; 


fail:
    return (int) 1;
}


/*
 ******************************
 * redraw_screen:
 *
 *     Repinta todas as janelas com base na zorder.
 *     @todo: 
 * Obs: Ao repintar cada janela a rotina redraw_window dever� 
 * incluir todos os elementos da janela. 
 * Do mesmo jeito que o usu�rio modificou de acordo com suas prefer�ncias.
 */
 
//wm.

// Vamos procurar na lista por ponteiros v�lidos.
// Repintaremos todas as janelas com ponteiros v�lidos. 

    // #todo
    // Repintar todas as janelas de baixo para cima
    // mas s� as que estiverem invalidadas.


// funcionou mas est� piscando ...
// como se estivesse pintando a mesma janela duas vezes.
// provavelmente a �ltima; 
 
int redraw_screen (void){


    int z;
    int RedrawStatus;

    struct window_d *__window;


    for ( z=0; z < KGWS_ZORDER_MAX; z++ )
    {
        __window = (void *) Windows[z];

        if ( (void *) __window != NULL )
        {
            if ( __window->used == 1 && __window->magic == 1234 )
            {
                // Compara os �ndices.
                if ( __window->z != z )
                {
                    panic ("wm-redraw_screen: index\n");
                }

                //retira o foco.
                //KillFocus(__window);

                // durty.
                // se ela foi marcada como suja e precisa ser repintada.
                //if ( __window->invalidated == 1 )
                //{
                    //Repinta uma janela.
                    RedrawStatus = (int) redraw_window (__window, 1);
                    if (RedrawStatus == 1)
                    {
                        panic ("redraw_screen: status\n");
                    }
                //}
            }
        };
    };


    /*
    if ( (void *) __window != NULL )
	{
		if ( __window->used == 1 && __window->magic == 1234 )
		{
            //set_active_window (__window);
            SetFocus (__window);

            //Repinta uma janela.
            RedrawStatus = (int) redraw_window (__window, 1);

            if (RedrawStatus == 1)
            {
                panic ("redraw_screen: status again\n");
            }
        };
    };
    */

    // #obs
    // Se for terminar corretamente � porque 
    // repintamos tudo o que foi poss�vel.

done:

    return 0;	

//fail:
    //panic ("redraw_screen: \n");
}


/*
 *********************************************
 * resize_window:
 *     Muda as dimens�es da janela.
 */	

// wm. 
 
int 
resize_window ( struct window_d *window, 
               unsigned long cx, 
               unsigned long cy )
{
 
	if ( (void *) window == NULL )
	{
	    return 1;    
	
	} else {	
	    
		//@todo: Checar limites.
	
	    // Ok mudamos as dimens�es da janela principal.
	    // Mas e se ela tiver uma janela client window?
        window->width = (unsigned long) cx;
        window->height = (unsigned long) cy;

        
        if (window->clientAreaUsed == 1)
        {
			if ( (void *) window->rcClient != NULL )
			{
				//validade da estrutura de ret�ngulo
				if ( window->rcClient->used == 1 &&
				     window->rcClient->magic == 1234 )
				{
					if ( window->type == WT_SIMPLE )
					{
					    window->rcClient->width = window->width;
					    window->rcClient->height = window->height;
					}
					
                    if ( window->type == WT_OVERLAPPED )
                    {
                        window->rcClient->width = (unsigned long) (window->width -1 -1);
                        window->rcClient->height = (unsigned long) (window->height -2 -32 -2 -24 -2);
			        }
			        
                    if ( window->type == WT_EDITBOX || 
                         window->type == WT_EDITBOX_MULTIPLE_LINES )
                    {
                        window->rcClient->width = (unsigned long) (window->width -1 -1);
                        window->rcClient->height = (unsigned long) (window->height -1 -1);
                    }
				} 
			}
        };
        
        if (window->statusbarUsed == 1)
        {
			if ( (void *) window->statusbar != NULL )
			{
			    window->statusbar->width = window->width;
			    window->statusbar->height = 25;
			    //window->statusbar->bg_color = COLOR_PINK;
			    
			    //ajuste
			    window->bottom = window->top + window->height;
			    window->statusbar->top = window->bottom -25-1; 
		    }
        };
        
        
        if (window->scrollbarUsed == 1)
        {
			if ( (void *) window->scrollbar != NULL )
			{
                window->scrollbar->width  = 35;
                window->scrollbar->height = (window->height -2 -35 -2 -24 -2);
                //window->scrollbar->bg_color = COLOR_PINK;
                
                //ajuste
                window->right = window->left + window->width;
                window->scrollbar->left = window->right -37;
            }
        }
        
        //validade da thread,
		if ( (void *) window->control != NULL )
        {
			if ( window->control->used == 1 || window->control->magic == 1234 )
			{
			    // mandamos a mensagem
			    // o aplicativo decide o que fazer com ela.
			    //window->control->window = window;
			    //window->control->msg = MSG_SIZE;
			    //window->control->long1 = 0;
			    //window->control->long2 = 0;
			    //window->control->newmessageFlag = 1;
			}
		};
	};

    return 0;
}


/*
 ****************************************************
 * replace_window:
 *     Muda os valores do posicionamento da janela.
 */
 
// wm. 
 
int 
replace_window ( struct window_d *window, 
                 unsigned long x, 
                 unsigned long y )
{
    if ( (void *) window == NULL )
    {
        return 1;
    } else {
		
        //@todo: Checar limites.
	
        window->left = (unsigned long) x;
        window->top = (unsigned long) y;
        
        
        
        if (window->clientAreaUsed == 1)
        {
			if ( (void *) window->rcClient != NULL )
			{
				//validade da estrutura de ret�ngulo
				if ( window->rcClient->used == 1 &&
				     window->rcClient->magic == 1234 )
				{
					if ( window->type == WT_SIMPLE )
					{
                        window->rcClient->left = (unsigned long) (window->left);
                        window->rcClient->top = (unsigned long) (window->top);
					}
					
                    if ( window->type == WT_OVERLAPPED )
                    {
                        window->rcClient->left = (unsigned long) (window->left +1);
                        window->rcClient->top = (unsigned long) (window->top  +2 +32 +2);
			        }
			        
                    if ( window->type == WT_EDITBOX || 
                         window->type == WT_EDITBOX_MULTIPLE_LINES )
                    {
                        window->rcClient->left = (unsigned long) (window->left +1);
                        window->rcClient->top = (unsigned long) (window->top  +1);
                    }
				} 
			}
        };
        
        
        if (window->statusbarUsed == 1)
        {
			if ( (void *) window->statusbar != NULL )
			{
				window->bottom = window->top + window->height; 
			    window->statusbar->left = window->left +1;
			    window->statusbar->top  = window->bottom -25 -1;
            }
        };	


        if (window->scrollbarUsed == 1)
        {
			if ( (void *) window->scrollbar != NULL )
			{
				window->right = window->left + window->width;
                window->scrollbar->left = window->right -37;
                window->scrollbar->top = window->top +2 +32 +2;
            }
        }
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

// wm. 
 
int is_window_full ( struct window_d *window ){
	
    if ( (void *) window == NULL )
	{
	    return 0;
	
	} else {
		
	    //Checa modo tela cheia.
        if ( window->view == VIEW_FULL )
        {
	        return (int) 1;
	    }
		
		//...
	};

	return 0;
}


/*
 ********************************************
 * is_window_maximized:
 *     Checar se est� no modo maximizado.
 *
 *     retorno 1, modo maximizada.
 * @todo: Rever o retorno.
 */
 
//wm.
 
int is_window_maximized (struct window_d *window){
	
    if ( (void *) window == NULL )
	{
	    return 0;    
		
	} else {
	    
		//Checa se j� est� maximizada.
        if ( window->view == VIEW_MAXIMIZED )
        {
	        return (int) 1;
	    };		
		//...
	};

	return 0;
}



/*
 * is_window_minimized:
 */
 
// wm stuff

int is_window_minimized (struct window_d *window){	
    
	if ( (void *) window == NULL)
	{
	    return 0;
	
	} else {
		
	    // Checa se j� est� minimizada.
        if ( window->view == VIEW_MINIMIZED )
        {
	        return (int) 1;
	    }		
		//...
	};
	
	return 0;    
}


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
 
// wm.
 
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
                set_current_window (window->parent);
                 
	            set_active_window (window->parent);
	            
                SetFocus (window->parent);
            }; 		
        };
		
		
	    // devemos retirar a janela da zorder list 
	
	    z = (int) window->z;
	
	    if ( z >= 0 && z < KGWS_ZORDER_MAX )
	    {
	        Windows[z] = (unsigned long) 0;	
	    
		    //atualiza o contador.
            zorderCounter--;
	   
	        if (zorderCounter < 0 )
	        {
		        zorderCounter = 0;
            }
	    };
	
        //Sinaliza para o GC.
	    
		window->used = WINDOW_GC;       //216;
	    window->magic = WINDOW_CLOSED;  //4321;		
		
		//...
	};
	
	//...
}


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
 
// wm.
 
void DestroyWindow ( struct window_d *window ){
	
    CloseWindow (window);
}


/*
 * get_active_window:
 *     Obtem o id da janela ativa.
 */
 
int get_active_window (void){
		
    return (int) active_window;  
}


/*
 ****************************************************
 * set_active_window:
 *     Seleciona qual ser� a janela ativa.
 *     Ativa uma janela.
 */
 
// wm. 
 
void set_active_window (struct window_d *window){
	
	/*
	
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
	
	
	*/

    return;	
}


/*
 * change_active_window:
 *     @todo: Trocar a janela ativa
 */

// wm.

void change_active_window (int id){
	
	//todo: Limits. Max.
    
	if (id < 0)
	    return;

	active_window = (int) id;
}


/*
 * show_active_window:
 *     Mostra o id da janela ativa.
 *     @todo: Mudar o nome para windowShowActiveWindowId() 
 */

//#debug 

void show_active_window (void){
		
	printf ("ActiveWindow=%d \n", (int) active_window);
}


/*
 * show_window_with_focus:
 *     Mostra o id da janela com o foco de entrada..
 */

//#debug 

void show_window_with_focus (void){
		
	printf ("wwf=%d \n", (int) window_with_focus );
}


/*
 * CloseActiveWindow:
 *     Fecha a janela ativa.
 */

// wm.

void CloseActiveWindow (void){
	
    struct window_d *Window;	
	int Offset;
	
	
    Offset = (int) get_active_window ();	
	
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
}



// #todo: 
// windowSetFocus(.) e windowGetFocus() windowSwitchFocus()


void windowBlockFocus (void){
		
	gFocusBlocked = (int) 1;
}


void windowUnblockFocus (void){
	
	gFocusBlocked = (int) 0;
}


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
 * o cursor fique dentro dos limites da �rea de cliente.
 *
 * #importante:
 *  Isso depende do tipo de janela. Se for um editbox tem que colocar o 
 * cursor no in�cio da �rea de edi��o.
 *
 * ...
 */
 
// wm.
 
// #bugbug
// Revendo a quest�o de repintar a janela m�e quando se seta o foco. 
 
void SetFocus ( struct window_d *window ){
	
    // priority stuff
    struct thread_d *thread;


	// #debug
	// Testando com uma vers�o simplificada, pois isso est� falhado na m�quina real.
	// Sem foco do teclado n�o funciona.
	
	if ( (void *) window == NULL )
	{
		panic ("SetFocus: window");
    }else{
	
	    if ( window->used != 1 || window->magic != 1234 )
	    {
		    panic ("SetFocus: validation");
		}

		// ... ok

        window->focus = 1; 
        window_with_focus = (int) window->id;
        WindowWithFocus = (void *) window;
	};
	
	
	// Priority and quantum.
	if ( (void *) window->control != NULL )
	{
	    set_thread_priority ( (struct thread_d *) window->control,
	        PRIORITY_MAX );
	
	}


	// #bugbug
	// Send message ?
	// Isso falhou na m�quina real.
	// Tem que fazer testes.
	
	/*
		// Validade da thread.
		if ( (void *) window->control != NULL )
        {
			// Validade da thread.
			if ( window->control->used == 1 || 
			     window->control->magic == 1234 )
			{
			   // mandamos a mensagem
			   // o aplicativo decide o que fazer com ela.
			    window->control->window = window;
			    window->control->msg = MSG_SETFOCUS;
			    window->control->long1 = 0;
			    window->control->long2 = 0;
			    window->control->newmessageFlag = 1;
			}
		}
	*/


	//return;
}


/*
 * GetFocus: 
 *     Pega o ponteiro para a estrutura da janela com o foco de entrada.
 */

// wm.

void *GetFocus (void){
	
	if ( window_with_focus < 0 )
		return NULL;


    return (void *) windowList[window_with_focus];
}


/*
 ***********************************************
 * windowGetForegroundWindow:
 *     Recupera o handle da janela que o usu�rio est� trabalhando, ou seja, 
 * a janela em primeiro plano.
 */

// wm.

void *windowGetForegroundWindow (void)
{
    return (void *) windowList[window_with_focus];
}


/*
 **********************************************
 * windowSetForegroundWindow:
 *     Coloca uma janela em primeiro plano para o usu�rio trabalhar nela.
 *     @todo: aumentar aprioridade da thread que 
 * configurou a janela de primeiro plano.
 *     +o input do teclado deve vi pra essa janela.
 *     +modifica��es visuais deve ocorrer na janela 
 * que est� em primeiro plano, para 
 * que ela se destaque.
 * retorno: 0=OK  , 1=error.
 */

//wm

//#obs
// raise_window deve chamar isso e depois repintar e mostrar.

int windowSetForegroundWindow ( struct window_d *window ){
	
	if ( (void *) window == NULL )
	{
		printf ("windowSetForegroundWindow: window\n");
		goto fail;
		
	}else{
		
	    SetFocus (window);
	    set_active_window (window);
	     
		//...
	};
	
	// #todo: 
	// Aumentar a prioridade da thread.


done:
    return 0;

fail:
    return (int) 1;
}


//coloca a janela acima das outras.
int raise_window ( struct window_d *window ){


    struct window_d *parent;

	if ( (void *) window == NULL )
	{
		return -1;
	}else{

      // #bugbug
      // a janela filha nunca � a janela ativa.
      // somente se a janela m�e � a gui->main
      // Uma janela filha pode ser a janela com o foco de entrada.

      //se ja somos a janela filha com o foco de entrada,
      //ent�o n�o precisamos fazer coisa alguma.
      if ( window->parent->child_with_focus == window )
      {
      }
      
      parent = window->parent;
      parent->child_with_focus = window;

      windowSetForegroundWindow (window);

       //redesenha e mostra.
      redraw_window (window,1);
	};

    return 0;
}

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
 * Muda quando trocamos a janela ativa, muda quando trocamos o desktop.
 */
 
// wm ? 
 
void windowSwitchFocus (void){
	
	int Max;
	int CurrentID;
	int NextID;

    struct window_d *window;
	struct window_d *next;	
	//...
	
	//Max e Current.
	
	Max = (int) windows_count;
	CurrentID = (int) window_with_focus;	//salva
	

	// @todo: 
	// Essa rotina precisa ser refeita. 
	// Seguindo uma lista linkada de janelas.

	
	window = (void *) windowList[window_with_focus];	
	
	if ( (void *) window == NULL )
	{
		printf("windowSwitchFocus: window\n");
	    goto fail; 
		
	}else{
		
	    KillFocus (window);  
		
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

    redraw_window ( window, 1 );
	
    // #debug  
	// #todo: Criar mecanismo melhor pra mostrar a janela com o foco.
	
    //printf("Focus={%d}\n", (int) window_with_focus);
	//refresh_screen();
	
	return;
	
fail:

    return;
}


/*
 *************************************
 * KillFocus:
 *     Uma janela perde o foco.
 */
 
void KillFocus ( struct window_d *window ){
	
	
	// #debug
	// suspensa para testes na m�quina real
	
	return;
	
	//
	// ====== cut here for now =====
	//
	
	
	if ( (void *) window == NULL )
	{
		printf ("KillFocus: window\n");
	    goto fail; 
	}else {
		
		if ( window->used == 1 && window->magic == 1234 )
		{
			window->focus = 0;
			
			window_with_focus = 0; //#test
			WindowWithFocus = NULL;
			

            // Validade da thread.
            if ( (void *) window->control != NULL )
            {
                // Validade da thread.
                if ( window->control->used == 1 || 
                     window->control->magic == 1234 )
			    {
			        // mandamos a mensagem
			        // o aplicativo decide o que fazer com ela.
			        window->control->window = window;
			        window->control->msg = MSG_KILLFOCUS;
			        window->control->long1 = 0;
			        window->control->long2 = 0;
			        window->control->newmessageFlag = 1;
			    }
		    }
		
		};
	};


fail:
    return;
}


/*
 ******************************************
 * MinimizeWindow:
 *     Minimiza uma janela.
 *     @todo windowMinimize()
 */
 
// wm.
 
void MinimizeWindow (struct window_d *window){
	
	int Status;
	
    if( (void *) window == NULL )
	{
		printf ("MinimizeWindow: window\n");
	    return;
		//goto fail;
		
	}else{
		
	    Status = (int) is_window_minimized (window);
	    
        if (Status == 1)
	    {
		    printf ("MinimizeWindow: Status\n");
	        return;
			//goto fail; 
	    };	
		
        KillFocus (window);
	    window->view = (int) VIEW_MINIMIZED;		
	};
}


/*
 **************************************
 * MaximizeWindow:
 *     Maximiza uma janela.
 *     @todo: windowMazimize()
 */

// wm. 
 
void MaximizeWindow (struct window_d *window){
	
	int Status;
	
    if( (void *) window == NULL)
	{
		printf ("MaximizeWindow: window\n");
	    goto fail; 
	};
	
	Status = (int) is_window_maximized(window);
	
    if (Status == 1)
	{
		printf ("MaximizeWindow: Status\n");
	    goto fail; 
	};

    //redimensionar.
	if ( gui->main != NULL )
	{
        window->left = gui->main->left;             
        window->top = gui->main->top;                   
	    
		window->width = gui->main->width;             
        window->height = gui->main->height;
	}; 	

	// todo: 
	// Redraw ?!
	
done:

    set_active_window (window);
    
	SetFocus (window);
	
	window->view = (int) VIEW_MAXIMIZED;
	
    return;
    
fail:

    return;	
}


/*
 *************************************************
 * init_window_manager:
 *     Inicializa o gerenciamento de janelas.
 *     @todo windowmanagerInit()
 */
 
// wm. 
 
int init_window_manager (void){
	
    // Aloca mem�ria para a estrutura do procedimento 
	// de janela da thread atual.

    // #bugbug 
    // N�o sei se estamos realmente usando isso.	
	
	WindowProcedure = (void *) kmalloc ( sizeof( struct window_procedure_d ) );
	
	if ( (void *) WindowProcedure == NULL )
	{
	    panic ("init_window_manager: WindowProcedure\n");
		
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

	// todo:  
	// Continua fazendo inicializa��es de procedimento de janela.	

    return 0;
}


/*
 *****************************************
 * init_windows:
 *     Inicializa a lista de janelas.
 *     Inicializa globais relativas � janelas.
 */
 
int init_windows (void){
		
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
	
	rectClientArea = (void *) kmalloc ( sizeof(struct rect_d) );
    
	if ( (void *) rectClientArea == NULL )
	{	
	    panic ("init_windows: rectClientArea\n");
		
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
	
	//Contagens 
    rooms_count = 0; 
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
	
	BackBufferInfo = (void *) kmalloc ( sizeof(struct backbufferinfo_d) );
    
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

	FrontBufferInfo = (void *) kmalloc ( sizeof(struct frontbufferinfo_d) );
    
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

        //?? terminal ??		
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


	// #importante:
	// Nesse momento n�o existem estruturas prontas para carregar arquivos.
   		
	//
	// Continua ...
	//
	
done:	

	return 0;
	
fail:

	return (int) 1;
}


//pegando a z-order de uma janela.
// wm

int get_zorder ( struct window_d *window ){
	
    if ( (void *) window != NULL )
	{
		return (int) window->z;
	}

	return (int) -1;
}


	//refletindo ??
    //se um argumento for passado teremos que examinar a ordem das janelas filhas.	
	//sendo assim cada estrutura de janela precisa de uma lista de janelas filhas 
	// e a zorder delas ... 
	//
	//se nenhuma janela for passada como argumento 
	// teremos que olhar as janelas filhas da janela principal que � a gui->main.
	//a janela gui->main pode ser a janela mae de todas as outras ...
	// sendo a janela gui->main a janela principal do processo kernel.
	//o processo explorador de arquivos poder� ser o processo que 
	//seja a m�e de todos os aplicativos de usu�rio ... ent�o a 
	//z order na estrutura da janela desse aplicativo indica a ordem 
	//das janelas principais dos seus processos filhos ...
	//??
	// Uma estrutura de janela pode ter uma vari�vel top_window indicando o id 
	// da janela que est� no topo da z-order da janela.
	
//pegando a o id da janela que est� no topo da lista de uma janela.
// wm

struct window_d *getTopWindow (struct window_d *window){
	
	if ( (void *) window == NULL )
	{
		return (struct window_d *) Windows[KGWS_ZORDER_TOP];
	}
	
	return NULL;
}


//Obtendo a janela de n�vel mais alto.
// wm
int get_top_window (void){
		
	return (int) top_window;
}


//Setando a top window.
// wm
void set_top_window (int id){

    struct window_d *w;

    w = (struct window_d *) windowList[id];
    
    if ( (void *) w != NULL )
    {
        Windows[KGWS_ZORDER_TOP] = (unsigned long) w;    
        top_window = (int) id;
    }

}


// Fecha a janela ativa.

void closeActiveWindow (void){
	
    int ID;
	struct window_d *w;
	
	ID = get_active_window();
	
	if (ID<0)
		return;
	
	w = (void *) windowList[ID];
	
	CloseWindow (w);
}


//encontrando um slot livre na z-order global de 
//overlapped windows.

// wm

int z_order_get_free_slot (void){
	
	int response;
	int z; 
	struct window_d *zWindow;

	for( z=0; z < KGWS_ZORDER_MAX; z++ )
	{
	    zWindow = (void*) Windows[z];
        
		//Obtendo um espa�o vazio.
		//Se for NULL, ent�o n�o tinha um ponteiro no slot.
		if( (void*) zWindow == NULL )
		{
			response = (int) z; 
			
			zorderCounter++;
			if (zorderCounter >= KGWS_ZORDER_MAX)
			{
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
}


/*
 ***********************************
 * windowLoadGramadoIcons:
 *     Carrega alguns �cones do sistema.
 *     It's a part of the window system's initialization.
 */

int windowLoadGramadoIcons (void){

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
		panic ("init_windows: appIconBuffer\n");
	}

	if ( (void *) fileIconBuffer == NULL )
	{
		panic ("init_windows: fileIconBuffer\n");
	}

	if ( (void *) folderIconBuffer == NULL )
	{
		panic ("init_windows: folderIconBuffer\n");
	}

	if ( (void *) terminalIconBuffer == NULL )
	{
		panic ("init_windows: terminalIconBuffer\n");
	}

	if ( (void *) cursorIconBuffer == NULL )
	{
		panic ("init_windows: cursorIconBuffer\n");
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
		panic ("init_windows: APP.BMP\n");
	}
    
	fRet = (unsigned long) fsLoadFile ( VOLUME1_FAT_ADDRESS,
						       VOLUME1_ROOTDIR_ADDRESS, 
							   "FILE    BMP", 
	                           (unsigned long) fileIconBuffer );
    if ( fRet != 0 )
	{
		panic ("init_windows: FILE.BMP\n");
	}

	fRet = (unsigned long) fsLoadFile ( VOLUME1_FAT_ADDRESS, 
						       VOLUME1_ROOTDIR_ADDRESS, 
							   "FOLDER  BMP", 
	                           (unsigned long) folderIconBuffer );
    if ( fRet != 0 )
	{
		panic ("init_windows: FOLDER.BMP\n");
	}
	
	fRet = (unsigned long) fsLoadFile ( VOLUME1_FAT_ADDRESS, 
						       VOLUME1_ROOTDIR_ADDRESS, 
							   "TERMINALBMP", 
	                           (unsigned long) terminalIconBuffer );
    if ( fRet != 0 )
	{
		panic ("init_windows: TERMINAL.BMP\n");
	}	
	
	fRet = (unsigned long) fsLoadFile ( VOLUME1_FAT_ADDRESS, 
						       VOLUME1_ROOTDIR_ADDRESS, 
							   "CURSOR  BMP", 
	                           (unsigned long) cursorIconBuffer );
    if ( fRet != 0 )
	{
		panic ("init_windows: CURSOR.BMP\n");
	}

	// More ?

    return 0;
}



/*
 **************** 
 * top_at: 
 * 
 */
// pega a janela que est� mais ao topo da zorder e que
// corresponda �s cordenadas do mouse.
// retorna window id
int top_at ( int x, int y )
{
    int z = 0;
    
    
    int wID = -1;
    struct window_d *__last_found;
    struct window_d *tmp;
        
    //max 1024 janelas.
    for ( z=0; z<KGWS_ZORDER_MAX; z++ )
    {
        //pega a pr�xima na zorderlist;
        tmp = (struct window_d *) Windows[z];
        
        //check
        if ( (void *) tmp != NULL )
        {
            if ( tmp->used == 1 && tmp->magic == 1234 )
            {

                 // #importante
                 // Checando coordenadas.
                  if ( x > (tmp->left)  && 
                       x < (tmp->left + tmp->width)  && 
                       y > (tmp->top)  &&
                       y < (tmp->top + tmp->height)  )
                 {
					 //printf ("%d",z);
                      // salva essa.
                      __last_found = (struct window_d *) tmp;
                 }
            }
        }

    };

__found:

    window_mouse_over = __last_found->id;

    return (int) __last_found->id;
}


/*
 *******************************************************************
 * windowScan:
 *     Escaneia as janelas existentes procurando uma que contenha o 
 * posicionamento do cursor.
 * Ou seja, o posicionamento do cursor deve estar dentro da �rea da janela.
 *
 * Obs:
 *    A fun��o retorna na primeira janela encontrada.
 *    #todo: Mas dever�amos observar se essa janela possui janelas filhas, 
 * pois o ponteiro pode passar em cima de uma janela que est� dentro de 
 * outra janela.  
 */

// wm.
 
// #bugbug
// No caso dos bot�es o deslocamento encontrado � relativo
// � sua janela m�e. Ent�o tamb�m precisamos considerar
// o posicionamento da janela m�e ?? 

// IN: posicionamento do cursor.
// OUT: ID da janela.
 
int windowScan ( unsigned long x, unsigned long y ){

    struct window_d *w;

	//register int i;
    int i = 0;
    int WID;


	// #bugbug
	// Vamos sondar toda a lista de janelas.
	// pois pode haver fragmenta��o na lista de janelas.
	// Ent�o mesmo com uma contagem baixa de janelas pode haver 
	// janelas v�lidas em posi��es altas.
	// Ent�o vamos sondar por toda a lista.


	// #BUGBUG
	// O problema nessa t�cnica s�o as sobreposi��o de janelas.
	// Quando uma janela est� dentro da outr, ent�o duas janelas
	// est�o �ptas a serem selecionadas.
	// Talvez devamos filtrar e s� aceitarmos sondar procurando 
	// por controles.


    for ( i=0; i < WINDOW_COUNT_MAX; i++ )
    {
        w = (struct window_d *) windowList[i];

        if ( (void *) w != NULL )
        {
            if ( w->used == 1 && w->magic == 1234 )
            {

				// #bugbug
				// Precisamos considerar o deslocamento da janela m�e
				// para sabermos seu posicionamaento em rela��o � tela.

				// Para o botao vamos considerar o deslocamento
				// da janela mae.
				// Dentro da �rea da janela.

				// >> BUTTON or EDITBOX <<

                if ( w->type == WT_BUTTON || w->type == WT_EDITBOX )
                {
                    if ( (void *) w->parent == NULL )
                    {
                        panic ("windowScan: parent\n");
                    }

			        //printf ("X=%d Y=%d l=%d t=%d w=%d h=%d \n",
			            //x, y, w->left, w->top, w->width, w->height);

                    if ( x > (w->left)  && 
                         x < (w->left + w->width)  && 
                         y > (w->top)  &&
                         y < (w->top + w->height)  )
                    {

                        WID = w->id;
                        window_mouse_over = w->id;

                        return (int) WID;
                    }
                }



				// Dentro da �rea da janela.
				/*
				if ( x > w->left   && 
					 x < w->right  && 
				     y > w->top    &&
				     y < w->bottom )
				{
					
					// #todo
					// Aqui precisamos de um tratamento diferenciado para cada um dos tipos.
					// Vamos come�ar com os bot�es.
					// Nesse momento cada um pode ter um compotamento diferente.
					// Pega o ID.
					
					if ( w->type == WT_BUTTON )
					{    
						WID = w->id;
					    window_mouse_over = w->id;    
						return (int) WID;
					}
					
					if ( w->type == WT_EDITBOX )
					{    
						WID = w->id;
					    window_mouse_over = w->id;    
						return (int) WID;
					}
					
					// ...
				}; 
				*/



			};
		};

	}; //for


    return (int) -1;
}


int windowOverLappedScan ( unsigned long x, unsigned long y ){

    struct window_d *w;

	//register int i;
    int i = 0;
    int WID;


	// #bugbug
	// Vamos sondar toda a lista de janelas.
	// pois pode haver fragmenta��o na lista de janelas.
	// Ent�o mesmo com uma contagem baixa de janelas pode haver 
	// janelas v�lidas em posi��es altas.
	// Ent�o vamos sondar por toda a lista.


	// #BUGBUG
	// O problema nessa t�cnica s�o as sobreposi��o de janelas.
	// Quando uma janela est� dentro da outr, ent�o duas janelas
	// est�o �ptas a serem selecionadas.
	// Talvez devamos filtrar e s� aceitarmos sondar procurando 
	// por controles.


    for ( i=0; i < WINDOW_COUNT_MAX; i++ )
    {
        w = (struct window_d *) windowList[i];

        if ( (void *) w != NULL )
        {
            if ( w->used == 1 && w->magic == 1234 )
            {

				// #bugbug
				// Precisamos considerar o deslocamento da janela m�e
				// para sabermos seu posicionamaento em rela��o � tela.

				// Para o botao vamos considerar o deslocamento
				// da janela mae.
				// Dentro da �rea da janela.

				// >> BUTTON or EDITBOX <<

                if ( w->type == WT_OVERLAPPED )
                {
                    if ( (void *) w->parent == NULL )
                    {
                        panic ("windowScan: parent\n");
                    }

                    //drawDataRectangle ( w->left, w->top, 
			        //    w->width, w->height, COLOR_RED );
			        //refresh_rectangle ( w->left, w->top, w->width, w->height );     
			        
			       // printf ("X=%d Y=%d l=%d t=%d w=%d h=%d \n",
			          //  x, y, w->left, w->top, w->width, w->height);
			    
                    if ( x > (w->left)  && 
                         x < (w->left + w->width)  && 
                         y > (w->top)  &&
                         y < (w->top + w->height)  )
                    {
                        //printf ("o ");
                        WID = w->id;
                        window_mouse_over = w->id;

                        return (int) WID;
                    }
                }



				// Dentro da �rea da janela.
				/*
				if ( x > w->left   && 
					 x < w->right  && 
				     y > w->top    &&
				     y < w->bottom )
				{
					
					// #todo
					// Aqui precisamos de um tratamento diferenciado para cada um dos tipos.
					// Vamos come�ar com os bot�es.
					// Nesse momento cada um pode ter um compotamento diferente.
					// Pega o ID.
					
					if ( w->type == WT_BUTTON )
					{    
						WID = w->id;
					    window_mouse_over = w->id;    
						return (int) WID;
					}
					
					if ( w->type == WT_EDITBOX )
					{    
						WID = w->id;
					    window_mouse_over = w->id;    
						return (int) WID;
					}
					
					// ...
				}; 
				*/



			};
		};

	}; //for


    return (int) -1;
}






//Envia uma mensagem PAINT para o aplicativo atualizar a �rea de trabalho.
void windowUpdateWindow ( struct window_d *window )
{
    // bugbug: esse send est� falhando.
    //windowSendMessage ( (unsigned long) window, 
        //(unsigned long) MSG_PAINT, 
        //(unsigned long) 0, 
        //(unsigned long) 0 );
        
        
		//window = (struct window_d *) arg2;

		if ( (void *) window == NULL )
		{  return; }
		
        if ( window->type == WT_OVERLAPPED || window->type == WT_SIMPLE )
        {
		    // Validade da thread.
            if ( (void *) window->control != NULL )
            {
			    // Validade da thread.
			    if ( window->control->used == 1 || 
			         window->control->magic == 1234 )
			    {
			       // mandamos a mensagem
			       // o aplicativo decide o que fazer com ela.
			        window->control->window = window;
			        window->control->msg = MSG_PAINT;
			        window->control->long1 = 0;
			        window->control->long2 = 0;
			        window->control->newmessageFlag = 1;
			    }
		    }
        };

}


/*
 ***************************************
 * windowSwitchFullScreen:
 *     Switch full screen support.
 */

//salvando os valores usados para configura o modo 
//full screen

unsigned long save_client_rect_left;
unsigned long save_client_rect_top;
unsigned long save_client_rect_width;
unsigned long save_client_rect_height;

int fsStatus;

// uma janela entra ou sai do modo full screen.
// #todo: isso pode ir pra outro lugar, provavelmente gws.

// wm

int windowSwitchFullScreen (void){
	
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
    printf ("FAIL\n");
    refresh_screen();
    
    return 1;
}


// wm
int scroll_client_window ( struct window_d *window ){

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
}


//
// End.
//

