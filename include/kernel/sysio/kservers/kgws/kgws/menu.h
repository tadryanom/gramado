/*
 * Arquivo: menu.h
 *
 * Descrição:
 *     Header para gerenciamento de menus.
 *
 * Menus:
 *     São três os componentes principais: 
 *     (MenuItem < Menu < MenuBar).
 *
 * Control Menu:
 *     O control menu aparece apenas na janela ativa.
 *     @todo: Sua estrutura de indica em quan janela ele está aparecento. 
 *            Sua estrutura deve indical qual é o seu procedimento.
 *            O control menu deve enviar mensagem para a janela com o resultado
 *            da interação do usuário com o menu ?!
 *
 * Versão 1.0, 2015, 2016.
 */
 
 
 
#define MENU_COUNT_MAX      256 
#define MENUITEM_COUNT_MAX  16



/*
 * MENUITEM:
 *     Estrutura para menuitem.
 */

struct menuitem_d
{
	object_type_t objectType;
	object_class_t objectClass;
	
    int Id;          //Id do menu.	
	
	int Used;        //Se está sendo usado.
	int Magic;       //Se o slot não está corrompido.
	
	int Selected;    //Flag. Selecionado ou não.
	
    unsigned long color_bg;    //cor.
    
    unsigned long x;           //deslocamento x.
    unsigned long y;           //deslocamento y.
    
    unsigned long left;        //margem esquerda. 
    unsigned long top;         //margem superior.
    
    unsigned long right;       //margem direita.  
    unsigned long bottom;      //margem inferior.       
    
    unsigned long width;       //largura.
    unsigned long height;      //altura.
	
	//
	// @todo: Strings.
	//
	unsigned char *string;
	
    int windowId;               //Id da janela que o menu pertence. 	
	struct window_d *window;    //Janela que possui o menu (bg).
    

	//Quando um item abre um submenu.
	//esse é um ponteiro para a estrutura do dubmenu 
	//que o item abre.
	struct menu_d *next;
	
	//todo: pode ter uma estrutura pra retangulo ... tipo, cor ...
	struct menuitem_d *next_menuitem;
	//...

};
struct menuitem_d *menuitemDefault;
//...



/*
 * MENU:
 *     Estrutura para menu.
 * (todo menu tem uma janela que recebe as mensagens,
 *   e a janela tem seu procedimento de janela).
 */

struct menu_d
{
	object_type_t objectType;
	object_class_t objectClass;
	
    int Id;              //Id do menu.
	int selectedItem;    //Id do item selecionado.

	int itemCount;       //Número total de itens. 
	int itemMax;         //Número maximo de itens. (menuitemList[]).
	
	unsigned long newitemLeft;
	unsigned long newitemTop;
	
	int Type;      //Vertical ou horizontal.
    int Style;	   //Enfeites, formatos ... aparência.
	
	//lista de ítens.
	//array de ponteiros para estruturas de menuitem.
	struct menuitem_d *Items; 

	//linked list. ( a linked list do menu).
	struct linkedlist_d *linkedlist;	

	
	//o menu é uma janela.
    int menuId;                    //id da janela que o menu é.
	struct window_d *menuWindow;   //o menu sendo uma janela.
	
	//
	// *IMPORTANTE: O menu está em uma janela. No caso do control menu
	//              ele aparecerá na janela ativa.
	//
	
	//janela mae do menu,(que é uma janela)
    int windowId;                  //id da janela que o menu pertence. 	
	struct window_d *parentWindow; //Janela que possui o menu.
    
	
	
	//background do menu.(suspenso)
	//int backgroundId;
	//struct window_d *background;
	
	//unsigned long menuitemList[16]; //lista de itens do menu.
	
	//...
   
};
struct menu_d *logonMenu;
struct menu_d *guiMenu;
struct menu_d *menubarDefault;
struct menu_d *menuDefault;
//...



/*
 * Lista de Menus.
 */ 

unsigned long menuList[MENU_COUNT_MAX];




/*
 * Contagem de menus.
 */

int menus_count;

  

/*
 * CURRENT MENU:
 *     Estrutura para o menu atual, (ativo).
 */

struct current_menu_d
{
    unsigned long menuId;    //Id do menu ativo, (índice na lista de menus).
    struct menu_d *menu;
	//window...
	//desktop...
	//...
};



//
// Menubar globals.
//

//unsigned long g_menubarWindow;
//unsigned long g_menubarMenu;



//Protótipos.

int init_menus (void);

void set_current_menu (struct menu_d *menu);

void *get_current_menu (void);

int RegisterMenu (struct menu_d *menu);

void initmenuArray (struct menu_d *a, int initialSize);


/*
 * MainMenu:
 *     Cria o menu principal.
 *     Nada mais na tela, apenas um menu centralizado.
 */

int MainMenu(struct window_d * window);   //o main menu é o control menu.

int ControlMenu(struct window_d * window); //control menu


/*
 * MainMenuProcedure:
 *    Main menu. inicialização de ambientes.
 *    //test
 */

unsigned long 
MainMenuProcedure ( struct window_d *window, 
                    int msg, 
					unsigned long long1, 
					unsigned long long2 );


/*
 * create_menu_item:
 *     Menus são verticais.
 */

int create_menu_item (struct menu_d *menu, unsigned char *string, int status);


int select_menubar_item(int n);

int unselect_menubar_item(int n);

int get_menubar_selected_item (void);

int redraw_menubar_item( int n);


void *create_menu ( struct window_d *pwindow,    //Parent window. 
                    unsigned long iCount,        //Contagem de ítems. 
				    unsigned long sCount,        //Contagem de espaçadores.
				    int type,                    //Horizontal ou vertical.
				    int style );                 //Estilo.

				   
				   			   
unsigned long 
MenuBarProcedure ( struct window_d *window, 
                   int msg, 
				   unsigned long long1, 
				   unsigned long long2 );


//
//  End.
//

