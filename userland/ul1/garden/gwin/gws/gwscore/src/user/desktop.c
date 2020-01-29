/*
 * File: kernel\gws\user\desktop.c
 *
 * Descri��o:
 *     Ger�ncia a cria��o de desktops.
 *     Cada desktop pertence � uma window station. Cada desktop cont�m v�rias 
 * janelas e menus. Cada desktop tem um prop�sito, as vezes gen�rico, as vezes 
 * espec�fico. 
 *     Define como as janelas est�o empilhadas, qual est� em foregroud (Por cima).
 *     Define uma lista encadeada de janelas, trocando a janela ativa e o 
 * foco de entrada. Etc...  
 *     Faz parte do m�dulo Window Manager do tipo MB.
 *
 * Obs:
 *     Os aplicativos poder�o acessar esses servi�os em user mode. Atrav�s de 
 * um subsistema em user mode que gerencia janelas.
 *     O desktop abarca v�rios componentes como janelas especiais, 
 * janelas normais, �reas de mem�ria para usos diversos, permissoes, 
 * caracter�sticas diversas ... etc ...
 *    Obs: *IMPORTANTE: Um desktop tem um heap de mem�ria. A gerente de
 * mem�ria alocar� mem�ria para os processos pertencentes � um desktop
 * espec�fico no pr�prio heap do desktop ao qual o processo pertence. 
 *
 * Hist�rico:
 *     Vers�o 1.0, 2015 - Esse arquivo foi criado por Fred Nora.
 *     Vers�o 1.0, 2016 - Revis�o.
 *     ...
 */

 
#include <kernel.h>


/*
    Buffer creation:
	===============
    
	//Supposing the display is 800x600 with 32 bpp (meaning 32/8 = 4 bytes per pixel).
    uint8_t *BackBuffer = ((uint8_t *) (malloc(800 * 600 * 4)));
	
	//24bpp.
	unsigned char *BackBuffer = ( (unsigned char *) (malloc(800 * 600 * 3))); 
*/


/*
 * RegisterDesktop:
 *     Registrando um desktop numa lista de desktops.
 *     @todo: Mudar para desktopRegisterDesktop(.). 
 */
int RegisterDesktop (struct desktop_d *d){
	
    int Offset = 0;
	
    if ( (void *) d == NULL ){
		
        return (int) 1;  
    };
	
	//
	// #bugbug: 
	// Pode aparacer um loop infinito aqui.
	//
	
	while ( Offset < DESKTOP_COUNT_MAX )
	{
       if ( (void *) desktopList[Offset] == NULL )
	   {
       	    desktopList[Offset] = (unsigned long) d; 
            
			return (int) 0;			
	   };
	   
	   Offset++;
    };		
	
	// Fail.
	
    return (int) 1;
};


/*
 * set_current_desktop:
 *     Seta o desktop atual.
 *     @todo: Mudar para desktopSetCurrentDesktop(.).
 */
void set_current_desktop ( struct desktop_d *desktop ){
	
	if ( (void *) desktop == NULL ){ 
	    return; 
	}
	
	//Configura a vari�vel global.
	
	current_desktop = (int) desktop->desktopId;
};


/*
 * get_current_desktop:
 *     Pega o endere�o da estrutura do desktop atual.
 *     @todo: Mudar para desktopGetCurrentDesktop().
 */
void *get_current_desktop (){
	
    //Check limits.
    
	if ( current_desktop < 0 || 
	     current_desktop >= DESKTOP_COUNT_MAX )
	{
	    return NULL;
	};
    
	return (void *) desktopList[current_desktop];
};


int get_current_desktop_id(){
	return (int) current_desktop;
};


/*
 * init_desktop_list:
 *     Inicializa o array de ponteiros de desktop.
 */
void init_desktop_list (){
	
    int i=0;		
    
	while ( i < DESKTOP_COUNT_MAX )
	{
        desktopList[i] = 0;
		
		++i; 
	};
};


/*
 * CreateDesktop:
 *     Cria um desktop em uma window station.
 *
 */
void *CreateDesktop ( struct wstation_d *ws ){
	
	int i=0;
	
	struct desktop_d *Current;
    struct desktop_d *Empty;	
    //... 
	
	//Check.
    
	if ( (void *) ws == NULL ){ 
	    
		return NULL; 
	};
	
	//
	// @todo: O usu�rio precisa de permiss�o pra criar desktops.
	//
	
	// Struct.
    
	Current = (void *) malloc ( sizeof(struct desktop_d) );
	
	if ( (void *) Current == NULL )
	{
	    printf ("CreateDesktop:");
		die ();
	
	} else {
	    
		//section.
	    Current->wstation = (void *) ws;
		
		//continua...
	};
	
	
	while ( i < DESKTOP_COUNT_MAX )
	{	
        Empty = (void *) desktopList[i]; 			
        
		if ( (void *) Empty == NULL )
		{
		    desktopList[i] = (unsigned long) Current;
		    
			Current->desktopId = i;    		
		    
			return (void *) Current;
		};	
		
		i++;
	};

    return NULL;	
};


/*
 * set_current_menuwindow:
 *     Configura a menu window atual.
 */
void set_current_menuwindow ( struct desktop_d *desktop, 
                              struct window_d *window )
{
    
	//Check.
	
	if ( (void *) desktop == NULL || (void *) window == NULL )
	{ 
        return; 
	};  
    
	desktop->menuWindow = (void *) window;  
};


/*
 * set_current_foreground:
 *     Configura a foreground window atual.
 */
void set_current_foreground ( struct desktop_d *desktop, 
                              struct window_d *window )
{
    //Check.
	
	if ( (void *) desktop == NULL || (void *) window == NULL )
	{ 
        return; 
	};  
    
	desktop->foregroundWindow = (void *) window;
};


/*
 * set_current_messagewindow:
 *     Configura a message window atual.
 */
void set_current_messagewindow ( struct desktop_d *desktop, 
                                 struct window_d *window )
{
    //Check.
	
	if ( (void *) desktop == NULL || (void *) window == NULL )
	{ 
        return; 
	};  
	
	desktop->messageWindow = (void *) window;
};


/*
 * set_current_traywindow:
 *     Configura a tray window atual.
 */
void set_current_traywindow ( struct desktop_d *desktop, 
                              struct window_d *window )
{
    //Check.
	
	if ( (void *) desktop == NULL || (void *) window == NULL )
	{ 
        return; 
	};  
	
	desktop->trayWindow = (void*) window; 
};


/*
 * set_current_tooltipwindow:
 *     Configura a tooltip window atual.
 */
void set_current_tooltipwindow ( struct desktop_d *desktop, 
                                 struct window_d *window )
{
    //Check.
	
	if ( (void *) desktop == NULL || (void *) window == NULL )
	{ 
        return; 
	};  
	
	desktop->tooltipWindow = (void*) window;
};


/*
 * change_foreground:
 *    Change foreground window for a desktop.
 */
void change_foreground ( struct desktop_d *desktop ){
	
    //Foreground window.
	struct window_d *fw;
	
    //Check Desktop.
	
	if ( (void *) desktop == NULL ){ 
	    
		return; 
	}  
	
	fw = (void *) desktop->foregroundWindow;
	
	if ( (void *) fw == NULL )
	{
	    return;
    };  

	//Use a pr�xima janela indicada na estrutura da atual.
	
	if ( (void *) fw->next == NULL )
	{
		//Deixa a mesma que estava.
   	    desktop->foregroundWindow = (void *) fw;
    }else{
		//Muda para a pr�xima.
		desktop->foregroundWindow = (void *) fw->next;
	    return;		
	};  
};


/*
 * set_current_sysMenu:
 *     Configura o sysMenu atual.
 */
void set_current_sysMenu ( struct desktop_d *desktop, struct menu_d *menu ){
	
    //Check.
	
	if ( (void *) desktop == NULL || (void *) menu == NULL )
	{ 
        return; 
	};
	
	desktop->sysMenu = (void *) menu;
};


/*
 * set_current_dialogsysMenu:
 *     Configura o dialogsysMenu atual.
 */
void set_current_dialogsysMenu ( struct desktop_d *desktop, 
                                 struct menu_d *menu )
{
    
	//Check.
	
	if ( (void *) desktop == NULL || (void *) menu == NULL )
	{ 
        return; 
	};
	
	desktop->dialogsysMenu = (void *) menu; 
};


/*
 * set_current_menuHScroll:
 *     Configura o menuHScroll atual.
 */
void set_current_menuHScroll ( struct desktop_d *desktop, struct menu_d *menu )
{
    //Check.
	
	if ( (void *) desktop == NULL || (void *) menu == NULL )
	{ 
        return; 
	};
	
	desktop->menuHScroll = (void *) menu; 
};


/*
 * set_current_menuVScroll:
 *     Configura o menuVScroll atual.
 */
void set_current_menuVScroll ( struct desktop_d *desktop, 
                               struct menu_d *menu )
{
    //Check.
	
	if ( (void *) desktop == NULL || (void *) menu == NULL )
	{ 
        return; 
	};
	
	desktop->menuVScroll = (void *) menu;
};


/*
 * init_desktop:
 *     Inicializa o desktop 0.
 *     @todo: Mudar para: desktopInit().
 */
void init_desktop (){
	
    //printf("init_desktop: Initializing..\n");
	
	desktops_count = 0;
	
	//inicializa a lista.
	init_desktop_list ();

	//Struct.
	
	desktop0 = (void *) malloc ( sizeof(struct desktop_d) );
	
	if ( (void *) desktop0 == NULL )
	{
	    printf("init_desktop error: desktop0");
		die ();
		
		//refresh_screen();
		//while(1){}	
	};
	
	//
	// Configura o desktop atual.
	//
	
    set_current_menuwindow (desktop0, NULL); 
    set_current_foreground (desktop0, NULL); 
    set_current_messagewindow (desktop0, NULL); 
    set_current_traywindow (desktop0, NULL); 
    set_current_tooltipwindow (desktop0, NULL); 
	
    set_current_sysMenu (desktop0, NULL);
    set_current_dialogsysMenu (desktop0, NULL);
    set_current_menuHScroll (desktop0, NULL);
    set_current_menuVScroll (desktop0, NULL);

	
	//registra na lista
	RegisterDesktop (desktop0); 
		
	//esse desktop pertence � window station 0.
	//desktop0->windowstation = (void*) ws0;
	
	
	//...
		
//done:

	//printf("Done.\n");
	//Set Current.
	set_current_desktop (desktop0);
	
	//#?? Poder�amos ter um retorno de tipo int.
    //return;
};


/*
int desktopInit()
{}
*/


//
// End.
//
