/*
 * File: gws/gws/createw.c
 *
 * Descri��o: 
 *     Gerencia os m�todos de cria��o de molduras de janelas e de janelas simples.
 *     Rotina principal de cria��o de janelas.
 *     Faz parte do m�dulo Window Manager do tipo MB.
 *
 * Obs: Os m�todos de cria��o de moldura de janelas podem coexistirem e ser�o
 *      gerenciados por esse m�dulo.
 *
 * *Importante:
 * Um dos m�todos inclui v�rias chamadas ao sistema, sendo que a promeira 
 * chamada ser� uma uma inicializa��o da rotina de constru��o da moldura. Essa
 * inicializa��o deve garantir exclusividade � um processo, para que outro processo
 * n�o interrompa a cria��o da moldura enquanto a rotina em user mode estiver
 * enviando par�metros para o kernel.
 *
 *
 * @todo: 
 * Toda vez que criarmos uma estrutura de janela devemos alocar mem�ria para 
 * o seu buffer dedicado de acordo com as dimens�es da janela e registrar o 
 * ponteiro para o buffer na estrutura de janela.
 * Uma das primeiras coisa a fazer � criar o buffer dedicado, que ser�
 * passado para as rotinas de pintura, para a pintura ser feita no buffer
 * dedicado. 
 *
 * IMPORTANTE: De toda forma, se a rotina de pintura, n�o pintar a janela no 
 * seu buffer dedicado, ela pintar� no buffer default, que � o backbuffer e
 * perceberemos o erro facilmente, para corrigi-lo. Ent�o, na hora de passar o 
 * conte�do dos buffers para o LFB deve-se seguir a ordem de uma lista linkada 
 * cujo primeiro ponteiro � o Backbuffer. Por observa��o poderemos ver se
 * ela � flutuante ou n�o. Se � flutuante, � porque foi pintada em buffer dedicado,
 * se n�o � flutuante � porque foi pintada no buffer default, que � o Backbuffer.
 *
 * History:
 *     2015 - Created by Fred Nora.
 *     2016 - 2018 New features.
 *     ...
 */

 
/*
 Importante:
    
    Sobre janelas e frames:
    =======================

    Uma janela na verdade � um container, na forma de ret�ngulo, que
abrigar� elemntos gr�ficos como bot�es, textos, etiquetas  etc...
    Uma Janela pode ou n�o conter uma moldura, chamada de Frame,
que � formada pela barra de t�tulos, pela borda, pela sombra...
    Uma janela em modo full screen n�o cont�m frame.

   Sobre a fun��o CreateWindow(.):
   ==============================
   O prop�sito dessa fun��o � a cria��o de v�rios tipos de janelas
   que ser�o utilizadas pelas aplica��es. A fun��o pode criar uma janela
   que contenha frame ou n�o.
   No momento da cria��o da janela de um aplicativo, a fun��o CreateWindow
   chamar� o procedimento de janela do aplicativo para ver as solicita��es
   de cria��o de elementos extras, que devem ser inclu�dos na janela.

 */

/*
    *Importante:
	 Poder� haver mais de um m�todo de cria��o de uma frame window.   
 */
 

#include <kernel.h>


//Herdadas do Boot Loader.
// De onde vem isso ?? head.s
// @todo: Devemos chamar o m�dulo hal para obtermos esses valores.
//depois salvamos em vari�veis internas usadas pela gui.

extern unsigned long SavedBootBlock;
extern unsigned long SavedLFB;
extern unsigned long SavedX;
extern unsigned long SavedY;
extern unsigned long SavedBPP; 

//
// Identifica��o do processo chamador e da janela a ser criada.
//

/*
struct process_d *xxxCallerProcess;
struct window_d *xxxNewWindow;

//
// Status: Qual � o status? se estiver aberto � porque algum processo est�
//         utilizando a rotina de cria��o de frame.
//

int createw_in_use;  //@todo: Essa vari�vel precisa ser inicializada.

int createwInitialize();
int createwInitialize()
{
	int pid;
	
	//Ocupado.
	if(createw_in_use == 1){
		goto fail;
	}
	
	//desocupado.
	if(createw_in_use == 0)
	{
		createw_in_use = 1;
	    pid = (int) current_process; //@todo: Criar rotina para pegar o id.
		xxxCallerProcess = (void*) processList[pid];
		return (int) 0;
	}
	//Nothing.
fail:
    return (int) 1;	
};


void *createwNewWindow();
void *createwNewWindow()
{
	xxxNewWindow = (void*) malloc(...);
	if( (void*) xxxNewWindow == NULL  ){
		return NULL;
	}
	
	return (void*) xxxNewWindow;
}

 ... @todo: criar mais rotinas primitivas.

*/


/*
 ***************************************************************************
 * CreateWindow: 
 *     Cria uma janela com base em uma struct. Retorna o endere�o da estrutura 
 * da janela criada, para que possa ser registrada na lista windowList[].
 * Obs: A contagem � feita quando registra a janela.
 *
 * @todo: � necess�rio definir claramente os conceitos de window e window frame...
 *        A constru��o dos elementos gr�ficos precisam de organiza��o e hierarquia.
 *
 * Obs: Essa rotian pode criar tanto uma janela, quanto um frame ou um bot�o.
 *
 * @todo: Corrigir as dimens�es da janela: 
 * *Importante: OS ARGUMENTOS DE DIMENS�ES S�O PARA O RET�NGULO QUE FORMA UMA �REA
 * QUE INCLUI DA �REA DO CLIENTE + BARRA DE FERRAMENTAS + BARRA DE MENU.
 * a barra de t�tulos faz parte da moldura.
 * O AS DIMENS�ES DO FRAME PODEM SER VARIADOS, DEPENDENDO DO ESTILO DA JANELA.
 *
 * Cria a janela dependendo do tipo:                              
 * =================================
 * WT_NULL          0 
 * WT_SIMPLE        1
 * WT_EDITBOX       2  // igual simples, mais uma bordinha preta.
 * WT_OVERLAPPED    3  // sobreposta(completa)(barra de titulo + borda +client area)
 * WT_POPUP         4  // um tipo especial de sobreposta,  //usada em dialog ou 
 *                        message box. (com ou sem barra de titulo ou borda)					   
 * WT_CHECKBOX      5  // Caixa de sele��o. Caixa de verifica��o. Quadradinho.
 * WT_SCROLLBAR     6  // Cria uma scroll bar. Para ser usada como janela filha.
 * CONTINUA ...
 */

//1  - Tipo de janela (popup,normal,...) 
//2  - Estado da janela. (poderia ter v�rios bits ??)
//3  - (min, max ...)
//4  - T�tulo.
//5  - Deslocamento em rela��o �s margens do Desktop.
//6  - Deslocamento em rela��o �s margens do Desktop.
//7  - Largura da janela.
//8  - Altura da janela.
//9  - Endere�o da estrutura da janela m�e.
//10 - desktop ID. (# Para levarmos em conta as caracter�sticas de cada desktop).
//11 - Client Area Color.
//12 - color (bg) (para janela simples)
 
void *CreateWindow ( unsigned long type, 
                     unsigned long status, 
                     unsigned long view, 
                     char *windowname, 
                     unsigned long x, 
                     unsigned long y, 
                     unsigned long width, 
                     unsigned long height, 
                     struct window_d *pWindow, 
                     int desktopid, 
                     unsigned long clientcolor, 
                     unsigned long color ) 
{	
	// @todo: O argumento style est� faltando.
	//        cada tipo de tanela poder� ter v�rios estilos.
	// Obs: Podemos ir usando apenas um estilo padr�o por enquanto.
	
	// (min, max ...).
	int View;	

	// Bars.
	int TitleBar = 0;
	int MenuBar = 0;
	int ToolBar = 0;
	int StatusBar = 0;
	int ScrollBar = 0;
	//...
	
	// Title bar buttons. [v] [X] 
	int MinimizeButton = 0; 
	int MaximizeButton = 0; 
	int CloseButton = 0;    
    //..
	
	// Items.
	int Background = 0;
	int ClientArea = 0;
	int Shadow = 0;
	int ButtonDown = 0; //??
	int ButtonUp = 0;   //??
	int ButtonSysMenu = 0;  //system menu na barra de t�tulos.	
	int Border = 0;         //New !!! usado no edit box.
	//int ScrollBar;
	//...
	
	// Desktop support.
	int ParentWindowDesktopId;    //Id do desktop da parent window.
	int WindowDesktopId;          //Id do desktop da janela a ser criada.

	
	//controle de janela
	struct window_d *windowButton1;  //minimize	
    struct window_d *windowButton2;	 //maximize
	struct window_d *windowButton3;  //close
	
	//bot�es na barra de rolagem.
	struct window_d *windowButton4;
	struct window_d *windowButton5;
	struct window_d *windowButton6;
	
    // Structs.	
	struct window_d *window;
	struct window_d *Parent;
	struct desktop_d *Desktop;
    struct rect_d *clientRect;  //rect
	
	
	// #improvisando uma largura de borda.
	// @todo: isso deve ir para outro lugar.
	unsigned long border_size = 0;
	unsigned long border_color = COLOR_BORDER;
    	
	
	//salvar para depois restaurar os valores originais no fim da rotina.
	//unsigned long saveLeft;
	//unsigned long saveTop;
	
	
	// Desktop:
	// @todo: Configurar desktop antes de tudo. 
	// @todo: Quando criamos uma janela temos de definir que ela
	// pertence ao desktop atual se n�o for enviado por argumento 
	// o desktop que desejamos que a janela perten�a.

	
	// O argumento onde:
	// Indica onde devemos pintar a janela. Serve para indicar as janelas 
	// principais. Ex: Se o valor do argumento for 0, indica que devemos 
	// pintar na tela screen(background) etc...


	// full screen mode ??
	// Se a janela a ser criada estiver no modo full screen, ela n�o deve ter
	// um frame, ent�o as dimens�es da janela ser�o as dimens�es do ret�ngulo
	// que forma a janela. Talvez chamado de Client Area.
	
	//
	// Parent window.
	//
	
	// Se a parent window enviada por argumento for inv�lida, 
	// ent�o usaremos a janela gui->screen. ?? 
	// Talvez o certo fosse retornar com erro.
	// ?? Qual deve ser a janela m�e ? Limites ?
	// @todo: devemos checar used e magic da janela m�e.
	// #bugbug: E quando formos criar a gui->screen, quem ser� a janela m�e ?
	
	if ( (void *) pWindow == NULL ){
		
		Parent = (void *) gui->screen;	
	
	} else {
		Parent = (void *) pWindow;
	};
	
	
	// Devemos checar se a janela est� no mesmo desktop 
	// que a ajnela m�e.
	// No caso aqui, criarmos uma janela no mesmo desktop que a 
	// janela m�e.
	
	// Devemos setar uma flag que indique que essa 
	// � uma janela filha, caso seja uma. Essa flag 
	// deve ser passada via argumento @todo.

	// @todo: Checar se � uma janela filha, 
	// se for uma janela filha e n�o tiver uma janela m�e associada a ela, 
	// n�o permita e encerre a fun��o.

	
	//if(FlagChild == 1)
	//{
		//if(pWindow = NULL) return NULL;
	//}
	

	// @todo: A atualiza��o da contagem de janela deve ficar aqui,
	// mas me parece que est� em outro lugar, ou n�o tem. ainda.
	
	// @todo: Se essa n�o for uma janela filha, ent�o temos que resetar 
	// as informa��es sobre a janela m�e. porque n�o procedem.	
	
	//ms. e se essa � uma janela filha de uma janela m�e que pertence �
	//outra thread e n�o est� no desktop ??
	
	
	// *Importante: 
	// Checando se o esquema de cores est� funcionando.
	
	if ( (void *) CurrentColorScheme == NULL )
	{
		panic ("CreateWindow: CurrentColorScheme");
		//die();
		
	}else{
		
		if ( CurrentColorScheme->used != 1 || 
		     CurrentColorScheme->magic != 1234 )
		{
		    panic ("CreateWindow: CurrentColorScheme validation");
		    //die();			
		}
		
		//Nothing.
	}
	
	
	//
    // ## New window ##
    //	
	
//CreateStruct:
	
	//Alocando mem�ria para a estrutura da janela.
	
	window = (void *) malloc ( sizeof(struct window_d) );	
	
	if ( (void *) window == NULL )
	{
        // @todo: 
		// Onde o Kernel colocar� essa mensagem de erro ?
		// Por enquanto no backbuffer.
		
		// Erro.
		printf("CreateWindow: window struct \n");
		refresh_screen();	
		
		return NULL; 
		
    }else{
        
		// Obs: 
		// Conseguimos alocar mem�ria para a estrutura da janela. 
		// Agora vamos inicializar os elementos da estrutura de acordo 
		// com os par�metros passados via argumento.
		
		// Object support.
		window->objectType = ObjectTypeWindow;
		window->objectClass = ObjectClassGuiObjects;

		// #importante
		// Id. A janela recebe um id somente na hora de registrar.
		// window->id = ??.

	    //Name.
		window->name = windowname;
		
		// Seguran�a.
		window->used = 1;
		window->magic = 1234;
		
	    // Window type.
        // Tipo � unsigned long pois poder� ser um conjunto de flags.		
	    window->type = (unsigned long) type;
		
		//@todo: Criar inst�ncia.
		
		//@todo: � importante definir o procedimento de janela desde j�.
		//sen�o d� problemas quando cham�-lo e ele na� estiver pronto.

		//Procedure support.
		//@todo: Devemos receber esses par�metros e configurar aqui.
		//window->procedure = (unsigned long) &system_procedure;
		//window->wProcedure = NULL;  //Estrutura.
		
		
		//Modo de exibi��o:
		window->view = (int) view;
		
		// Se n�o estivermos no modo gr�fico, n�o h� o que pintar.
		if ( g_useGUI == 0 )
		{ 
		    window->view = (int) VIEW_MINIMIZED;
            //#bugbug: Abortar.			
		};

        //Se n�o foi oferecido um modo de exibi��o, ent�o temos um problema.
        //?? Talvez devamos retornar um erro. 
		//if( (int) view == VIEW_NULL ){ return NULL; };		
		
		//??Qual � o status da janela, se ela � a janela ativa ou n�o.
		//?? Devemos definir quais s�o os status poss�veis da janela.
		//
		
		//active 
		if( status == WINDOW_STATUS_ACTIVE )
		{ 
		    active_window = (int) window->id;  
            //set_active_window(window); 		
		    //window->active = WINDOW_STATUS_ACTIVE;
			//window->status = (unsigned long) WINDOW_STATUS_ACTIVE;
			window->relationship_status = (unsigned long) WINDOW_REALATIONSHIPSTATUS_FOREGROUND; 
            window->zIndex = 0; // ?? inicializando apenas. @todo:getNextZAxisOrder()
			//...			
		};
		
		//inactive
		if( status == WINDOW_STATUS_INACTIVE )
		{ 
		    //window->active = WINDOW_STATUS_INACTIVE;
			//window->status = (unsigned long) WINDOW_STATUS_INACTIVE;
			window->relationship_status = (unsigned long) WINDOW_REALATIONSHIPSTATUS_BACKGROUND;
            window->zIndex = 0; //inicializando apenas. @todo:getNextZAxisOrder()
			//...			
		};	
		
		
		//focus	
		//window->focus = 0; //determinando que n�o temos o foco.

		// @todo:
		// Se for uma janela filha o posicionamento deve ser somado �s margens 
		// da �rea de cliente da janela que ser� a janela m�e.

		// #bugbug @todo 
		// Esses valores de dimens�es recebidos via argumento na verdade 
		// devem ser os valores para a janela, sem contar o frame, que 
		// inclui as bordas e a barra de t�tulos.

		// Dimens�es.	
        window->width = width;
        window->height = height;  
		
		// Margens.
		window->left = x;    
        window->top = y;
        window->right = (unsigned long) ( window->left + window->width );
        window->bottom = (unsigned long) ( window->top + window->height );       

		// Deslocamentos em rela��o �s margens.
		// Os deslocamentos servem para inserir elementos na janela, como barras, bot�es e textos.
		window->x = 0;
        window->y = 0;		
		
		// ?? saving.
		//saveLeft = window->left;
		//saveTop  = window->top;
		
		//
		//  ## color ##
		//
		
		// Background support.
		window->bg_color = (unsigned long) color; 
		window->clientrect_bg_color = (unsigned long) clientcolor;
		
		// ??
		// O ret�ngulo da janela.
		//window->rcWindow = NULL;
		
		//
		// # Cursor support #
		//
		
		window->CursorX = 0;
		window->CursorY = 0;
		window->CursorColor = COLOR_WINDOWTEXT; 
		//@todo: As outras caracter�sticas do cursor.
		//Caracter�sticas.		
		
		//Estrutura para cursor.
		window->cursor = NULL;
		//@todo: Uma op��o � inicializarmos a estrutura de ponteiro depois ...
		//pois tem janela que n�o tem ponteiro. JA QUE NO MOMENTO ESTAMOS ENFRENTANDO ALGUNS TRAVAMENTOS.
		//
		//window->cursor = (void*) malloc( sizeof(struct cursor_d) );
		
		//@@todo: Criar uma fun��o: Inicializarcursor(struct cursor_d *cursor).
		//if(window->cursor != NULL)
		//{
		//    window->cursor->objectType = ObjectTypeCursor;
		//    window->cursor->objectClass = ObjectClassGuiObjects;
		//	window->cursor->x = 0;
		//	window->cursor->y = 0;
		//	window->cursor->imageBuffer = NULL;
		//	window->cursor->imagePathName = NULL;
			//window->cursor->cursorFile = ??; //@todo: Dif�cil definir o tipo.
		//	window->cursor->cursorType = cursorTypeDefault;
		//};
		
		//cursores (parar de usar e come�ar usar apenas a estrutura.)
		window->cursor_x = 0;
		window->cursor_y = 0;
		
		//Barras.
		//As flags que representam a presen�a de cada uma das barras
		//ser�o acionadas mais tarde, na hora da pintuda, de acordo com
		//o tipo de janela � ser pintada.
		
		//Current message.
		//Msg support.
		window->msg_window = (void *) window;
		window->msg = 0;
		window->long1 = 0;
		window->long2 = 0;
		
		//Se tem ou n�o uma nova mensagem.
		window->newmessageFlag = 0;
		
		//Filas de mensagens.
		//S�o dessas filas que os programas em user mode
		//s�o pegar as mensagens, solicitando atrav�s de system call.
		window->hwndList[0] = 0;
		window->msgList[0] = 0;
		window->long1List[0] = 0;
		window->long2List[0] = 0;
		
		//Offsets
		window->sendOffset = 0;
		window->receiveOffset = 0;
		
		//Buffers support.
		
		//Dedicated buffer: 
		//Cria um buffer dedicado de acordo com as dimens�es da janela.		
		window->DedicatedBuffer = (void*) windowCreateDedicatedBuffer(window);
		
		//O backbuffer � a janela do desktop ao qual a janela pertence.
		//window->BackBuffer = (void*) windowGetBackbufferAddress(); //@todo: Criar fun��o.
		//window->BackBuffer = (void*) g_backbuffer_address;
		window->BackBuffer = (void*) g_backbuffer_va;
		
		//o frontbuffer � o endere�o da mem�ria de v�deo para o monitor
		//que se deseja mostrar a janela. (LFB, cr3)
		window->FrontBuffer = (void*) g_frontbuffer_pa;
		
	    //@todo:
	    //Se n�o for uma Child, deve-se resetar todas as informa��es relativas
	    //� parent window.
		
		//Parent support
		window->parent = (void*) Parent;
		window->parentid = (unsigned long) Parent->id;
		
		//Child window linked list.
		window->childListHead = NULL;
		
		// (#importante)
		// Client window support.
		// Obs: A �rea de cliente ser� um ret�ngulo e n�o uma janela.
		// Mas nda impede da estrutra gerenciar uma janela que fique 
		// em cima da �rea de cliente.
		window->client_window = NULL;  //window. 
		window->rcClient = NULL;       //rect. (#importante)
		
		// #importante
		// Terminal support.
		// Suporte n�o tradicional � terminais.
		// manipulando vari�veis ao inv�s de uma estrutura.
		window->terminal_used = (int) 0;
		window->terminal_magic = (int) 0;
		window->terminal_tab = (int) 0;
		window->teminal_left = (unsigned long) 0;
		window->teminal_top = (unsigned long) 0;
		window->teminal_width = (unsigned long) 0;
		window->teminal_height = (unsigned long) 0;
		
		//Desktop support.
		//window->desktop = (void*) Desktop; //configurado anteriormente.
		//window->desktop_id = Desktop->id;  //@todo: verificar elemento.
		
		// # Menu support #
	    window->menu_window = NULL;    
		window->sysMenu = NULL;  
		window->barMenu = NULL;  
		window->defaultMenu = NULL; 
		window->isMenu = 0;
		window->isButton = 0;
		window->isEditBox = 0;
		window->selected = 0;  //Caso a janela seja um �tem de menu.
		//window->text = NULL; //Texto, caso a janela seja um �tem de menu

		//Draw support.
		window->draw = 0;      //@todo: Cuidado com isso.
		window->redraw = 0;
	    window->show = 1; //Inicialmente presumimos que precisamos mostrar essa janela.
	    //Continua ...
	
        //Abaixo, elementos referenciados com menor frequ�ncia.
	    
		window->desktop = NULL; //@todo: Definir � qual desktop a janela perence.
		window->process = NULL; //@todo: Definir � qual processo a janela perence.
		
		//Trava.
		window->locked = 0;
		
		//Linked list.
		//window->linkedlist = NULL;
		
		//Prev e next.
		window->prev = (void*) Parent;
		window->next = NULL;
			
		//#debug
		//printf("config1 %s %d %d %d %d \n",
		//    window->name, window->left, window->top, window->width, window->height );
	};
	
    //Exemplos de tipos de janelas, segundo MS.	
    //Overlapped Windows
    //Pop-up Windows
    //Child Windows
    //Layered Windows
    //Message-Only Windows	
	
	//
	// Preparando os parametros da pintura de acordo com o tipo.
	//	

	// De acordo com o tipo de janela, preparamos a configura��o
	// e a pr�pria rotina create window est� pintando.
	// Por�m nesse momento o 'case' pode chamar rotinas de pintura em 
	// draw.c e retornar. 
	// CreateWindow � para criar a moldura principal ...
	// para so outros tipos de janelas podemos usar draw.c
	// pois quando chamarmos draw.c a estrutura de janela ja deve estar 
	// inicializada.
	// Rotinas grandes como pintar um barra de rolagem podem ficar em draw.c
	
	// #importante
	// Deveria ter uma vari�vel global indicando o tipo de 
	// design atual, se � 3D ou flat.
	
	
	// Configurando os elementos de acordo com o tipo.
	// @todo: Salvar as flags para os elementos presentes
	// na estrutura da janela.
	
    switch (type)
    {
		//0) Null.
		//Ainda n�o implementada.
		case WT_NULL:
		    return NULL; 
		    break;
		
		//    **** WINDOW ****
		// A window is an undecorated Frame.
		//1) Simple rect, (Sem barra de t�tulos).
		//Obs: para o tipo 1 � fundamental considerar a cor passada por argumento.
		case WT_SIMPLE:
	        Background = 1;    //bg.
			window->backgroundUsed = 1;
		    break;

		// 
		//2) Edit box, (Simples + borda preta).
        //editbox n�o tem sombra, tem bordas. 
		case WT_EDITBOX:
            Background = 1;    //bg.
	        window->backgroundUsed = 1;
            Border = 1;
		    //window->borderUsed = 1;@todo: isso ainda n�o existe na extrutura ??	
		    break;

		//   **** FRAME ****
		// 3) Overlapped, (completa, para aplicativos).
		// Obs: Nessa caso, como se trata de uma janela de aplicativo,
		// poder�amos configurar a �rea de cliente.
		case WT_OVERLAPPED:
	        Shadow = 1;           // Sombra.
	        Background = 1;       // bg.
	        TitleBar = 1;         // T�tulo + Borda.
		    ClientArea = 1;
			
			//essas flags far�o a chamar createwindow 
			//recursivamente para desenhar os bot�es.
			
			//Controle de janela.
			//Bot�es da barra de t�rulos.
			MinimizeButton = 1;   
		    MaximizeButton = 1;   
			CloseButton = 1;       
			
		    //MenuBar       = 1;  // Barra de menu. 
	        //ButtonSysMenu = 1;    // System menu button. ??
		    window->shadowUsed = 1;
		    window->backgroundUsed = 1;
		    window->titlebarUsed = 1;
		    window->clientAreaUsed = 1;
		    //@todo:
		    //Se for do tipo overlapped pode ser ou n�o a janela ativa.
		    //set_active_window(window);
		    break;

		//4) Popup (um tipo de overlapped mais simples).
		case WT_POPUP:
	        Shadow = 1;        //sombra.
	        Background = 1;    //bg.
		    window->shadowUsed = 1;
		    window->backgroundUsed = 1;		
		    //if(titulo){} TitleBar = 1;    //titulo + borda	
		    break;
 
		//5) Check box, (Simples + borda preta).
        //   Caixa de sele��o. Caixa de verifica��o. Quadradinho.
		case WT_CHECKBOX:
	        Background = 1;    //bg.
		    window->backgroundUsed = 1;
            //bordas.
            Border = 1;
 			//window->borderUsed = 1;@todo: isso ainda n�o existe na extrutura ??
		    break;
			
		//6) scroll bar.
        // Cria uma scroll bar. Para ser usada como janela filha.
		case WT_SCROLLBAR:
			//Background = 1;    //bg.
		    //window->backgroundUsed = 1;
            //bordas.
            //Border = 1;
 			//window->borderUsed = 1;@todo: isso ainda n�o existe na extrutura ??
		    break;
			
		// 8) button
        //Obs: existe uma rotina que desenha um bot�o.
        //quando create window for chamada para criarmos um bot�o 
        //podemos chamar essa rotina que desenha o bot�o ...
        //apenas isso.	
        //aqui nesse case podemos selecionar algumas caractrer�sticas 
        //do bot�o que s�o acionadas atrav�s das caracter�sticas 
        //da janela.  		
		case WT_BUTTON:	
            //Nothing for now ...
            //Deixaremos a rotina de desenhar o bot�o fazer tudo por enquanto.			
			break;	
			
		//9) Status bar.	
		case WT_STATUSBAR:
	        Background = 1;    //bg.
			window->backgroundUsed = 1;
		    break;
			
		//barra de rolagem
		//bot�es de radio .. 
		//...

        // Continua ...
		
		//Ainda n�o implementada.
        default:
            return NULL;  		
		    break;
	};	
	
	// Continua...
	
	// * Draw !
	// Hora de pintar. Os elementos ser�o inclu�dos se foram 
	// selecionados anteriormente.
	
//drawBegin:	

	// Obs: Se for uma janela, pintaremos apenas a janela.
	//      Se for um frame, pintaremos todos os elementos
	//      presentes nesse frame de acordo com as flags.
	// Obs:
	// A janela precisa ser pintada em seu buffer dedicado.
	// Nesse momento o buffer dedicado da janela j� est� na estutura
	// da janela. Rotinas de pintura que tenham acesso � estrutura da
	// janela devem utilizar o buffer dedicado que for indicado na estrutura.
	// Para que seja poss�vel pintar uma janela em seu buffer dedicado,
	// devemos passar um ponteiro para a estrutura da janela em todas
	// as rotinas de pintura chamadas daqui pra baixo.
	//
	// @todo: Passar estrutura de janela via argumento, para a rotina
	// de pintura ter acesso ao buffer dedicado.
	
	//if(DedicatedBuffer == 1){};
	
	
	// se o view for igual NULL talvez signifique 
	if ( window->view == VIEW_NULL )
	{
		//#bugbug: fail.
		//window->show = 0;
		//window->redraw = 0;
		//return (void*) window;
	}
	
	// Minimized ? 
	// Se tiver minimizada, n�o precisa mostrar a janela, por�m
	// � necess�rio pintar a janela no buffer dedicado, se essa t�cnica 
	// estiver dispon�vel.
	// Talvez antes de retornarmos nesse caso seja necess�rio configurar 
	// mais elementos da estrutura.
	
	//#bugbug
	//se estamos contruindo a janela, ent�o ela n�o foi registrada 
	//n�o podemos checar as coisas na estrutura ainda,
	//mas a estrutura ja existe a algumas coisas foram inicializadas.
	
	View = 0;
	View = (int) is_window_minimized(window);
	
    if ( View == 1 )
	{
		//window->draw = 1; //Devemos pint�la no buffer dedicado.
		window->show = 0;
		window->redraw = 0;
		//...
		
		//@todo: N�o retornar. 
		//como teste estamos retornando.
		//goto done;
	    return (void *) window;
	}
	
	// @todo: Maximized ?
	// Para maximizar devemos considerar as dimens�es da �rea de cliente
	// da janela m�e.
	// Se a jenela estiver maximizada, ent�o deve ter 
	// o tamanho da �rea do cliente, caso ela seja uma janela filha.
	// Essa �rea de cliente poder� ser a �rea de trabalho, caso a
	// janela m�e seja a janela principal.
	// Obs: se estiver mazimizada devemos usar as dimens�o e coordenadas 
	// da janela gui->main.
	
	View = 0;
	View = (int) is_window_maximized(window);
	
    if ( View == 1 )
	{
		//Dimens�es.	
        window->width  = gui->main->width;
        window->height = gui->main->height;  
		
		//Margens.
		window->left = gui->main->x;    
        window->top = gui->main->y;
        window->right = (unsigned long) window->left + window->width;
        window->bottom = (unsigned long) window->top  + window->height;       

		//Deslocamentos em rela��o �s margens.
		// Os deslocamentos servem para inserir elementos na janela, como barras, bot�es e textos.
		window->x = 0;
        window->y = 0;		
	}	

    //  # FULL SCREEN #

	//??
	//entrar no modo full screen deve ser sempre uma iniciativa do usu�rio.
	//apenas um comando do usu�rio pode colocar uma janela no modo full screen.
	
    //View = 0;
	//View = (int) is_window_full(window);
    //if(View == 1){
	//	//...
	//}		
	
	//Se estivermos em full screen, mudaremos algumas caracter�ticas.
	// N�o usaremos frame: nem borda nem barra de t�tulos.
	//if( window->view == VIEW_FULL )
	//{
    //    Shadow = 0;
	//	TitleBar = 0;
		
        //Dimens�es:
		//??@todo: dever�amos chamar m�todos que peguem
		//a m�trica do dispositivo. ex getmetricsDeviceWidth();
	//	window->left = 0; 
	//	window->top = 0; 
	//	window->width = 800;  //@todo: getmetricsDeviceWidth();
	//	window->height = 600;
						           
	//	window->titlebarUsed = 0;
	//}
	

	// Color:
	// Obs: @todo: Isso foi definido acima, foi passado por argumento e
	// colocado na estrutura. Fiacr� assim somente por teste,
	// depois deletaremos essa defini��o aqui.
	//*Importante: Devemos utilizar a cor que foi passada via argumento?!
	// sen�o todas as janelas ter�o a mesma cor.
	
	// Sombra:
	//     A sombra pertence � janela e ao frame.
	//     A sombra � maior que a pr�pria janela.
	//     ?? Se estivermos em full screen n�o tem sombra ??
	
	//  ## Shadow ##
	
	if ( Shadow == 1 )
	{
		//flag.
		window->shadowUsed = 1;
		
		//CurrentColorScheme->elements[??]
		
		//@todo: 
		// ?? Se tiver barra de rolagem a largura da 
		// sombra deve ser maior. ?? N�o ...
		//if()
		
        // @todo: Adicionar a largura das bordas verticais 
		// e barra de rolagem se tiver.
		// @todo: Adicionar as larguras das 
		// bordas horizontais e da barra de t�tulos.
		// Cinza escuro.  CurrentColorScheme->elements[??] 
		// @TODO: criar elemento sombra no esquema. 
		
		if ( (unsigned long) type == WT_OVERLAPPED )
		{	
			drawDataRectangle ( window->left +1, window->top +1, 
				window->width +1 +1, window->height +1 +1, xCOLOR_GRAY1 );             
        }

        // ??
		// E os outros tipos, n�o tem sombra ??
		// Os outros tipos devem ter escolha para sombra ou n�o ??
		// Flat design pode usar sombra para definir se o bot�o 
		// foi pressionado ou n�o.
		
		//...
	}
	
	// ## Background ##
	//    Background para todo o espa�o ocupado pela janela e pelo seu frame.
			
	// Background.
	if ( Background == 1 )
	{
		// Flag.
        window->backgroundUsed = 1;
		
		window->bg_color = CurrentColorScheme->elements[csiWindowBackground]; 
	    
		// O argumento 'color' ser� a cor do bg para alguns tipos.
		// Talvez n�o deva ser assim. Talvez tenha que se respeitar o tema instalado.
		if( (unsigned long) type == WT_SIMPLE ){ window->bg_color = color; };
		if( (unsigned long) type == WT_POPUP ){ window->bg_color = color; };
		if( (unsigned long) type == WT_EDITBOX){ window->bg_color = color; }
		if( (unsigned long) type == WT_CHECKBOX){ window->bg_color = color; }
		//if( (unsigned long) type == WT_SCROLLBAR){ window->bg_color = color; }
		//...
		
		//Pintar o ret�ngulo.
		// @todo: ?? width Adicionar a largura da bordas bordas verticais.
		// @todo: ?? height Adicionar as larguras das bordas horizontais e da barra de t�tulos.
		
		if ( (unsigned long) type == WT_STATUSBAR )
		{
		    drawDataRectangle ( window->left, window->top, 
			    window->width -1, window->height, window->bg_color ); 	
						   
		    draw_string ( window->left +8, window->top +8, COLOR_TEXT,  
		  	  window->name ); 	
			goto done;
		}
		
		drawDataRectangle ( window->left, window->top, 
			window->width, window->height, window->bg_color ); 
		

        //?? More ...	
	}

	
	// ## Title bar ##
    // T�tulo + borda(frame).
    
    // #importante: 
    // Sobre bordas. 
    // Isso pinta a barra de t�tulos e as bordas para janelas de 
    // aplicativos. Ou seja, as bordas n�o s�o pintadas individualmente nesse 
    // momento, mas logo abaixo, pintaremos as bordas para as janelas onde 
    // esse for o caso. S� falta criar elemento na estrutura para gerenciar 
    // as bordas.	
    
    //parametros 16=icone 8=paddings
	
	if ( TitleBar == 1 )
	{ 
        //flag.
        window->titlebarUsed = 1;
        
		window->bg_color = color;  
		
		//@todo: String color.
		
		//@todo: String size.
		
		//@todo: Se tiver barra de rolagem, a largura deve ser maior.
		//if()
		
		//Rectangle and string.
		
		//#importante
		//ret�ngulao
		//??width  @todo: Adicionando a largura da bordas bordas verticais.
		//??height @todo: Adicionando as larguras das bordas horizontais e da barra de t�tulos.
						   
		drawDataRectangle ( window->left, window->top, 
			window->width +1 +1, window->height +1 +1, window->bg_color );
        
		// String		
		draw_string ( window->left +16 +8 +8, window->top +(32/3), 
			COLOR_TERMINALTEXT2, window->name );  
		
								 		
	    // ??
		// S� criamos o bot�o na barra de t�tulos se tivermos uma barra de t�tulos.
		// Ent�o esse � o lugar certo para essa rotina.
		// @todo: Se estivermos em full screen, n�o teremos bot�o.	
		
		if (MinimizeButton == 1)
		{
			window->minimizebuttonUsed = 1;
			
            // Create button.			
			windowButton1 = CreateWindow ( WT_BUTTON, 1, 1, "V", 
	                           (window->width -32 -32 -1 -32 -1), 2, 32, 32,									  
			                   window, 0, 
							   (unsigned long) COLOR_TERMINAL2, (unsigned long) COLOR_TERMINAL2);	
			
			RegisterWindow (windowButton1);
			window->minimize = windowButton1;
	    }
		
		if (MaximizeButton == 1)
		{
			window->maximizebuttonUsed = 1;
			
            // Create button.			
			windowButton2 = CreateWindow ( WT_BUTTON, 1, 1, "^", 
	                           (window->width -32 -32 -1), 2, 32, 32,									  
			                   window, 0, 
							   (unsigned long) COLOR_TERMINAL2, (unsigned long) COLOR_TERMINAL2);	
			
			RegisterWindow (windowButton2);
			window->minimize = windowButton2;
	    }
	
	    if (CloseButton == 1)
		{
			window->closebuttonUsed = 1;
			
			// Create button.
			windowButton3 = CreateWindow ( WT_BUTTON, 1, 1, "X", 
	                            (window->width -32), 2, 32, 32,									  
			                    window, 0, 
								(unsigned long) COLOR_TERMINAL2, (unsigned long) COLOR_TERMINAL2 );	
            
			RegisterWindow (windowButton3);
			window->close = windowButton3;
	    }					 
		
		//...			 
	};	
	
	
	//#debug
    //if ( (unsigned long) type == WT_OVERLAPPED )
	//{
	//    printf("CreateWindow: #debug");
	//	refresh_screen();
	//	while(1){}
	//}
	
	// ## Border ##
	// BORDA COLOR_INACTIVEBORDER 
	// Borda para as janelas.
	// Obs: As bordas dependem do tipo de janela e do 
	// estilo de janela.
	
	//vamos pintar a borda de acordo com o status da janela.
	//se a janela for um bot�o, e pintarmos essa borda, 
	//na hora de desenhar o bot�o essa borda vai desaparecer.
	//para isso podemos simplesmente, desenha um bot�o menor que 
	//a janela dele.
	
	if ( Border == 1 )
	{
		//flag.
		window->borderUsed = 1;
		
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
		



		
		
		// A largura da borda pode sinalizar o status (ativo ou inativo) 
		// de uma caixa de edi��o.
		
		//#todo: a largura da borda deve ficr registrado na estrutura.
		
		//if ( status == 0 ){ 
		//    border_size = 1;
        //    border_color = COLOR_INACTIVEBORDER; 			
		//}
		
		//if ( status == 1 ){ 
		//    border_size = 2; 
		//    border_color = COLOR_BLUE;
		//}
		//if( status == 2 ){ border_size = 3; } //just for fun
		
	    //board1, borda de cima e esquerda.    
		drawDataRectangle( window->left, 
		    window->top, window->width, border_size, border_color );
						   
	    drawDataRectangle( window->left, 
		    window->top, border_size, window->height, border_color );

	    //board2, borda direita e baixo.
	    drawDataRectangle( window->left +window->width -1, 
	        window->top, border_size, window->height, border_color );
					   
	    drawDataRectangle ( window->left, window->top +window->height -1, 
			window->width, border_size, border_color );
		
	};	
	
	
	//#debug
    //if ( (unsigned long) type == WT_OVERLAPPED )
	//{
	//    printf("CreateWindow: #debug");
	//	refresh_screen();
	//	while(1){}
	//}
	
	// # Client Area #
	// #importante
    // @todo: 
    //     RESPEITAR A IMPORT�NCIA DESSE ELEMENTO.	
    //     PERMITIR QUE MAIS TIPOS DE JANELA TENHAM CLIENT AREA.
    // Nesse caso deveremos considerar o tipo de janela na hora de  
    // criarmos a client area, pois tem janela que tem barra de t�tulos 
    // e tem janela que n�o tem, tem janela que bordas e tem janela que 
    // n�o tem.
    // #Testando para diferentes tipos de janela.
	
    if ( ClientArea == 1 )
	{
		//flag.
		window->clientAreaUsed = 1;
		
		// Obs: A Client Area � apenas um ret�ngulo.
		//@todo: Passar a estrutura de janela.
		
		clientRect = (void *) malloc ( sizeof(struct rect_d) ); 
		
		if ( (void *) clientRect == NULL )
		{
			//free(clientRect);
		    window->rcClient = NULL;
			
            // O que acontece nesse caso?? 
			// Ficaremos sem �rea de cliente ??
			// #bugbug
			// Nenhum alerta ser� emitido aqui por enquanto.
			
		} else {
			
			//@todo: Criar fun��o rectSetup(....)
			// Testando, isso pode falhar.
		    
			//
			// ?? DIMENS�ES DA JANELA: ??
			// A janela � um ret�ngulo que pode conter elementos como:
			// barra de menu, barra de ferramentas.
			// O frame n�o pertence a jenela. O frame � composto
			//pela barra de t�tulos e a borda.
			//
			// Obs: A �rea de cliente deve ter as dimens�es passadas
			//  via argumento.
			//

			//Salva o ponteiro.
			window->rcClient = (void*) clientRect;
			
			// Object stuff.
			window->rcClient->objectType = ObjectTypeRectangle;
			window->rcClient->objectClass = ObjectClassGuiObjects;
			
			// Validation.
			window->rcClient->used = 1;
			window->rcClient->magic = 1234;
			
			// janela.
			// O ret�ngulo pertence � essa janela.
			window->rcClient->window = (void *) window; 
			
			// #importante
			// ( Posicionamento e dimencionamento. )
            // + Ajustando o posicionamento da �rea de cliente com base
            // no posicionamento e dimens�es da janela.
			// + Nesse espa�o ser�o inclu�dos elementos adicionais,
			// desenhados e repintados pelo pr�prio aplicativo.
			// + Essa � a �nica parte que deve aparecer quando o aplicativo
			// entrar em modo full screen.
			//
			
			// ## SIMPLE ##
			
			//#bugbug
			//Janela simples n�o deve ter �rea de cliente.

			if ( window->type == WT_SIMPLE )
			{	
				
					//#debug
                 //printf("createw: l={%d} t={%d} w={%d} h={%d}\n", 
	             //    window->left, window->top, window->width, window->height ); 	
				 //refresh_screen();
				 //while(1){}
				
                //left top
                window->rcClient->left = (unsigned long) (window->left);
                window->rcClient->top = (unsigned long) (window->top);
            
                // width e height.
                // width = window widdth - as bordas verticais.
                // height = windows height - as bordas horizontais - a barra de t�tulos.
                window->rcClient->width = (unsigned long) (window->width);
                window->rcClient->height = (unsigned long) (window->height);
				
				//window->rcClient->right = window->rcClient->left + window->rcClient->width;
				//window->rcClient->bottom = window->rcClient->top + window->rcClient->height;
			};			
			
			
			// ## OVERLAPPED WINDOW ##
			
            if ( window->type == WT_OVERLAPPED )
            {
			    //left top
                window->rcClient->left = (unsigned long) (window->left +1);
                window->rcClient->top = (unsigned long) (window->top  +2 +32 +2);
            
                // width e height.
                // width = window widdth - as bordas verticais.
                // height = windows height - as bordas horizontais - a barra de t�tulos.
                window->rcClient->width = (unsigned long) (window->width -1 -1);
                window->rcClient->height = (unsigned long) (window->height -2 -32 -2 -24 -2);
				
				//window->rcClient->right = window->rcClient->left + window->rcClient->width;
				//window->rcClient->bottom = window->rcClient->top + window->rcClient->height;
                
			};
			
			// ## EDITBOX ##

			if( window->type == WT_EDITBOX || 
			    window->type == WT_EDITBOX_MULTIPLE_LINES )
			{	
                //left top
                window->rcClient->left = (unsigned long) (window->left +1);
                window->rcClient->top = (unsigned long) (window->top  +1);
            
                // width e height.
                // width = window widdth - as bordas verticais.
                // height = windows height - as bordas horizontais - a barra de t�tulos.
                window->rcClient->width = (unsigned long) (window->width -1 -1);
                window->rcClient->height = (unsigned long) (window->height -1 -1);
				
				//window->rcClient->right = window->rcClient->left + window->rcClient->width;
				//window->rcClient->bottom = window->rcClient->top + window->rcClient->height;
			};
			
			//right bottom.
			//v�lido para todos os tipos de janela.
  			window->rcClient->right = (unsigned long) (window->rcClient->left + window->rcClient->width);
			window->rcClient->bottom = (unsigned long) (window->rcClient->top + window->rcClient->height);
			
			
            // #bugbug Isso pode dar problemas.		
		    // Deslocamento dentro da �rea de cliente.
            // Zerando o deslocamento.
		    window->rcClient->x = 0;
			window->rcClient->y = 0;
			
		    //
			// * ESSA COR FOI PASSADA VIA ARGUMENTO.
			//
			
			//#debug
			//window->rcClient->bg_color = (unsigned long) COLOR_YELLOW; 
            
			window->rcClient->bg_color = (unsigned long) window->clientrect_bg_color;	      	      
			
			//
			// Draw!
			//
			
			
            drawDataRectangle( (unsigned long) window->rcClient->left, 
		                       (unsigned long) window->rcClient->top, 
						       (unsigned long) window->rcClient->width, 
						       (unsigned long) window->rcClient->height, 
							   (unsigned long) window->rcClient->bg_color );
							   
            // Done.
		};
		//Nothing.
	};
	
	
	//#debug
    //if ( (unsigned long) type == WT_OVERLAPPED )
	//{
	//    printf("CreateWindow: #debug clientarea ok");
	//	refresh_screen();
	//	while(1){}
	//}
	
	
	//@todo: janela com scroll bar padr�o.
	//if( ScrollBar == 1 ){}

	//MenuBar (teste)
    /*   
    if(MenuBar == 1)
	{
	    create_menubar(window);
        initmenuArray( window->barMenu, 2);    //Quantidade.
        create_menubar_item(window->barMenu,"Item1",1);    /Selected
        create_menubar_item(window->barMenu,"Item2",0); 		
	};
	*/
	
	// SCROLL BAR
	//se create window foi usada para criar uma janela filha do tipo scroll bar. 		
	//completaremos 
	if ( (unsigned long) type == WT_SCROLLBAR )
	{

		
        //bg
		drawDataRectangle ( window->left, window->top, 
			window->width, window->height, window->bg_color ); 
						   
	    
		//#importante 
		//#todo
        Parent->scrollbarUsed = 1;
		Parent->scrollbar = window;

		
		//Bot�o de cima da scrollbar vertival
		
		// Criar.
		windowButton4 = CreateWindow ( WT_BUTTON, 1, 1, "^", 
	        1, 1, (window->width -2), 32,									  
		    window, 0, (unsigned long) COLOR_TERMINAL2, (unsigned long) COLOR_TERMINAL2);
        
		// Registrar.
		RegisterWindow (windowButton4);  		
        window->scrollbar_button1 = windowButton4; 
		
		//#test	
	    //limits
	    if ( window->height == 0 || window->height > SavedX ){ 
		    window->height = 2; 
		}

	    //Bot�o do meio da scrollbar vertival
		
		// Criar.
		windowButton5 = CreateWindow( WT_BUTTON, 1, 1, "=", 
	        1, (window->height/2), (window->width -2), 32,									  
		    window, 0, (unsigned long) COLOR_TERMINAL2, (unsigned long) COLOR_TERMINAL2);	
		
		// Registrar.
		RegisterWindow (windowButton5);
		window->scrollbar_button3 = windowButton5;
		
	    //bot�o 2 da barra horizontal.
        //Bot�o de baixo da scrollbar vertival
	    
		// Criar.
		windowButton6 = CreateWindow( WT_BUTTON, 1, 1, "v", 
	        1, (window->height -32 -1), (window->width -2), 32,									  
		    window, 0, (unsigned long) COLOR_TERMINAL2, (unsigned long) COLOR_TERMINAL2);			
	    
		// Registrar.
		RegisterWindow (windowButton6);
		window->scrollbar_button2 = windowButton6;
	};	

	
	//if ( (unsigned long) type == WT_STATUSBAR )
	//{
		//#debug 
		//printf("CreateWindow.WT_STATUSBAR\n");
		
	//	drawDataRectangle ( window->left, window->top, 
	//		window->width -1, window->height, window->bg_color ); 	
						   
		//draw_string ( window->left +8, window->top +8, COLOR_TEXT,  
		//	window->name );  
			
			
		//#importante 
		//#todo
        //Parent->statusbarUsed = 1;
		//Parent->statusbar = window;
	//};
	
	//JANELA DO TIPO BOT�O.
	//#BUGBUG: N�O TEMOS INFORMA��ES SOBRE O TIPO DE BOT�O 
	//QUE DEVEMOS CRIAR. S� SABEMOS QUE A JANELA � DO TIPO BOT�O.
	if ( (unsigned long) type == WT_BUTTON )
	{
	    //as posi��es de left e right s�o da janela do bot�o.	
        //#obs: Essa fun��o retorna uma estrutura de bot�o.	
        //Ent�o uma janela, quando acianoada sua flag que indica que ela � um bot�o,
        //podemos usar a estrutura de bot�o para acessar as caracter�sticas do bot�o.		
		

        window->button = (struct button_d *) draw_button ( Parent, windowname, BS_DEFAULT, 0, 0,		
                                                 window->left, window->top, window->width, window->height, 
                                                 window->bg_color );
		
		window->isButton = 1;	
	}	

    if ( (unsigned long) type == WT_EDITBOX )	
	{
		window->isEditBox = 1;
	}


	//
	// Continua ... ( Pinta detalhes da janela, caso ouver )
	//
	
	//Barras de rolagem ...
	//efeitos ...
	//itens de menus...
	//itens de barras ...
	//carrega �cones ...
	//Continua ...
	
	
	// Message: 
	// @todo: Enviar uma mensagem de CREATE pra janela.
	//        Assim o procedimento cria os elementos
	//        restantes.
		
	
	// @todo: Atualizar a linkedlist da parent window.
	
	
	// Overlapped.
	if (window->type == WT_OVERLAPPED)
	{
		active_window = window->id;
		window->active = 1;
		
		window_with_focus = window->id;
		window->focus = 1;
		
		// ## scrollbar ##
		//Esses valores precisam ser melhor declarados.
        
		//#BUGBUG
		//S� CRIAREMOS SCROLL BAR NUMA OVERLAPPED SE A FLAG INDICAR ISSO;
		//ISSO VALE PRA OUTROS TIPOS DE JANELA N�O SOMENTE ESSE TIPO.
		
		// Criar.
		//window->scrollbar = CreateWindow ( WT_SCROLLBAR, 1, 1, "scrollbar-test", 
	    //                        window->right -40, window->top +2 +40 +2, 
		//						40, (window->height -2 -40 -2 -24 -2),									  
		//			            window, 0, 
		//						(unsigned long) CurrentColorScheme->elements[csiScrollBar], 
		//						(unsigned long) CurrentColorScheme->elements[csiScrollBar]);
		
		// Registrar.
		//RegisterWindow (window->scrollbar);


		
        // ## status bar ##
		//Esses valores precisam ser melhor declarados.
		
		//#BUGBUG
		//S� CRIAREMOS STATUS BAR NUMA OVERLAPPED SE A FLAG INDICAR ISSO;
		//ISSO VALE PRA OUTROS TIPOS DE JANELA N�O SOMENTE ESSE TIPO.		
		
		window->statusbar = CreateWindow ( WT_STATUSBAR, 1, 1, "Status: ...", 
	                              window->left+1, window->bottom-25-1, 
								  window->width, 25,							  
					              window, 0, 
								  COLOR_BLUE, 
								  COLOR_GRAY );
        
		// Registrar.
		RegisterWindow (window->statusbar);
		
		window->statusbarUsed = 1;
		window->statusbar = window->statusbar;
        
        // # icon #
		// Os �cones j� est�o carregados, 
		// vamos apenas decodific�-lo no backbuffer 
		
	    bmpDisplayBMP ( appIconBuffer, window->left +8, window->top +8 );
	};
	
	
	//#debug
    //if ( (unsigned long) type == WT_OVERLAPPED )
	//{
	//    printf("CreateWindow: #debug scroll status and *icon ok");
	//	refresh_screen();
	//	while(1){}
	//}

	
	//Ex: if( sendMessage(.,.,.,.) == 0 )
	//{
	//	//apaga tudo, notifica o erro e retorna null.
	//	return NULL;
	//}
	
	
	// Nesse momento a janela j� est� criada, podemos configurar 
	// algumas globais importantes, como thread, desktop ...
	
	//atualizar parent and child links.
	
	//@TODO: ENVIAR UMA MENSAGEM PARA A JANELA QUE RETORNAR� 
	//O TAMANHO OFICIAL DA JANELA DO CLIENTE.(STRUCT DE RECT)
	
	// #test
	// TEMOS QUE MANDAR UMA MENSAGEM DE 'MSG_CREATE' PRA JANELA,
	// SE ELA N�O RESPONDER CORRETAMENTE, ENT�O TEMOS QUE APAGAR TUDO O QUE
	// INICIALIZAMOS E NOTIFICAR O ERRO E RETORNAR NULL.

	//ENVIAMOS A MENSAGEM CREATE PRA JANELA. SEI LA, AVISANDO QUE CRIAMOS 
	//A A JANELA DO CLIENTE , ?? A JANELA DO CLIENTE � UMA JANELA NORMAL??
	//OU S� UM RET�NGULO??
	
	//NOTIFICAR TODO MUNDO SOBRE O EVENTO DA CRIA��O DA JANELA.
	//ALGUEM PODERIA ESTAR ESPERANDO POR ISSO...
	//DEVEMOS NOTIFICAR SOMENTE DEPOIS DE ENVIARMOS A MENSAGEM MSG_CREATE ...
	//E ANTES DE QUALQUER OPERA��O DE MOVIMENTA��O OU REDIMENSIONAMENTO DE JANELA ..
	//EXISTEM OBSERVADORES FAZENDO CONTAGEM DESSAS COISAS.

    //� HORA DE MINIMIZAR OU MAXIMIZAR, CONFORME FOI INDICADO POR QUEM
    //CHAMOU A FUN��O PARA CRIAR A JANELA.

    //SE � UMA JANELA FILHA, DEVEMOS ENVIAR UMA MENSAGEM PRA JANELA PAI,
    //AVISANDO QUE A JANELA FILHA FOI CRIADA ... POIS ELE PODE ESTAR ESPERANDO 
    //POR ISSO. ... devemos travar a threadmae dessa janela antes de enviar a mensagem e 
    //depois destravar.

	// @todo: ShowWindow.

    //destravar a thread dessa janela e a thread mae antes de sair.
	
	
	
	// # zorder support #
	
	// Deve ter uma fun��o que fa�a isso,
	// da� chamamos s� a fun��o.
	
	// Op��es:
	// + z-order dos elementos gr�ficos dentro da janela m�e.
	// + z-order global da overlapped windows.
	
    //z-order global de overlapped windows.
	//@todo: Colocar essa defini��o no in�cio da fun��o.
    int z;
	z = (int) z_order_get_free_slot();	
    
	if( z >= 0 && z < ZORDER_COUNT_MAX )
	{
	    zorderList[z] = (unsigned long) window;
	};
	
	//@todo: z-order de elementos gr�ficos dentro da janela m�e.
 
// done.		
done:

    //Testando se a rotina modificou algum desses.
    //window->left =  saveLeft;
	//window->top  =  saveTop;
	
    //SetFocus(window);
	
	//if ( window->type == WT_BUTTON || 
    //     window->type == WT_STATUSBAR )
	//{
	//    RegisterWindow (window);	
	//}
	
	//#teste
	//Associando a janela criada a trhead atual, que chamou essa rotina.
	//o problema � na hora da inicializa��o.
	
		window->InputThread = (struct thread_d *) threadList[current_thread];
		
		if( (void *) window->InputThread != NULL )
        {
			if ( window->used != 1 || window->magic != 1234 )
			{
			    window->InputThread = NULL;	
			}
		}	
	    
	
	return (void *) window;
};

//
// End.
//
