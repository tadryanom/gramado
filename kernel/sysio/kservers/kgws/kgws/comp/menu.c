/*
 * File: kgws/kgws/comp/menu.c 
 *
 * Descri��o:
 *     Cria��o e gerenciamento de menus.
 *     Faz parte do m�dulo Window Manager do tipo MB.
 *
 * History:
 *     2015 -  Created by Fred Nora.
 */

 
#include <kernel.h>


/*
 * menuIsMenu:
 *     Testa atrav�s do handle passado, se � uma menu ou n�o.
 *     0 = Yes; 1 = Not; 
 */
/* 
int menuIsMenu(struct menu_d *menu);
int menuIsMenu(struct menu_d *menu)
{
	//@todo:Implementar.
	return 0;
}
*/


/*
 **********************************************************
 * create_menu:
 *     Cria um menu e retorna o ponteiro pra janela do menu.
 *     A largura dos �tens do menu dependem da largura da janela.
 */
 
// IN: A parent window deve ser o bg. 
 
void *create_menu ( struct window_d *pwindow,  // Parent window. 
          unsigned long iCount,                // Contagem de �tems. 
          unsigned long sCount,                // Contagem de espa�adores.
          int type,                            // Horizontal ou vertical
          int style )                          // Estilo.
{
 
    unsigned long y;
    unsigned long width;
    unsigned long height;

    unsigned long iHeight;    //Altura do �tem.
    unsigned long sHeight;    //Altura do espa�ador.
    unsigned long x;

    //Window.
    struct window_d *hWindow;
	
	//Linked list.           
	//struct linkedlist_d *LinkedList;    


    //
	// Parent window 
	//
	
	// #obs:
	// Isso veio via argumento.
    // A parent window deve ser o bg. 

    if ( (void *) pwindow == NULL )
    {
        printf ("create_menu: pwindow\n");
        goto fail;
	}else{
		
		// Altura do �tem.
		// Altura do espa�ador.
		
	    //iHeight = (unsigned long) pwindow->width;
	    //iHeight = (unsigned long) (8*3);  //#rever  
	    //sHeight = (unsigned long) (3);      

		//#aten��o: 
		// N�o pode ser zero.
		// #bugbug: Isso � um teste.
		//if (iHeight == 0 ){ iHeight = 1; };

        // Menu.
		//height = (iHeight*iCount)+(sHeight*sCount);

        // ficar� no in�cio da janela enviada pelo aplicativo como m�e.
        x = 1;
        y = 1;
 
		//x = (unsigned long)  pwindow->left + 2; 
        //y = (unsigned long)  pwindow->top + 36 + 2;
		//x = (unsigned long)  pwindow->left; 
        //y = (unsigned long)  pwindow->top;
	    

        width = (unsigned long) (pwindow->width -4);  
        //a altura ser� configurada abaixo.
        //height = (unsigned long) (pwindow->height -4); 

		// Altura do �tem.
		// Altura do espa�ador.
	    iHeight = (unsigned long) (8*3);  //3 vezes a altura do char.  
	    sHeight = (unsigned long) (3);    //espa�ador magrinho. 
	    
		//...
    };


	//
	// @todo: Checar limites.
	//
	
	
    // ## Create  ##
	
//Create:

	// @todo: 
	// Criar as defini��es de tipos de menu.
	
	switch (style)
	{ 
	    case 0:  
		    //width  = (unsigned long) (pwindow->width);
		    height = (unsigned long) ( (iHeight*iCount) + (sHeight*sCount) ); 
		    goto do_create;
            break;

	    case 1:  
		    //width  = (unsigned long) (pwindow->width);
		    height = (unsigned long) ( (iHeight*iCount) + (sHeight*sCount) ); 
		    goto do_create;
            break;

	    case 2: 
            //width  = (unsigned long) (pwindow->width); 
		    height = (unsigned long) ( (iHeight*iCount) + (sHeight*sCount) ); 
			goto do_create;
            break;
			
		//...	
		
        //Normal tipo control menu.
		default:
		    //width  = (unsigned long) (pwindow->width);
		    height = (unsigned long) ( (iHeight*iCount) + (sHeight*sCount) ); 
		    goto do_create;
            break;
    };


	//
	// Background
	//


	// Efetua a cria��o da janela que conter� itens.


do_create:


    if ( (void *) pwindow != NULL )
    {
        x = pwindow->left + x;
        y = pwindow->top + y;
    }
    
    
    //
    // Draw!
    //
    
    // todo: 
    // Rever o esquema de cores para menu.
    
    // #todo
    // O tamanho dessa janela depende da quantidade de �tens
    // e do tamanho da janela m�e.
    // deve ter o tamanho da soma dos �tens e ser menor que
    //o tamanho da janela m�e.
    
    hWindow = (void *) CreateWindow ( 1, 0, 0, "menu-window",
                           x, y, 
                           width, height,
                           pwindow, 0, 0, COLOR_RED );

    if ( (void *) hWindow == NULL )
    {
        printf ("create_menu: hWindow\n");
        goto fail;

    }else{
        RegisterWindow (hWindow);
    };


    //debug
    //printf ("create_menu: *debug breakpoint");
    //refresh_screen();
    //while(1){}
 
 
	//
	// Menu structure. 
	//
	
	// #obs:
	// Cria uma estrutura para o menu.
	
menuStructure:
	
	menuDefault = (void *) kmalloc( sizeof( struct menu_d ) );  
	
	if ( (void *) menuDefault == NULL )
	{
	    printf("create_menu: menuDefault\n");
        goto fail;
        
	}else{
		
	    //Registra na estrutura de menu a janela que � o menu.
	    menuDefault->menuWindow = (void *) hWindow;

		//Registra na estrutura a janela � qual o menu pertence.(parent window)
		menuDefault->parentWindow = (void *) pwindow;
		
		//horizontal ou vertical
		menuDefault->Type = type;
		
		//estilo de menu, aparencia. detalhes.
		menuDefault->Style = style;
		
		//Posicionamento do pr�ximo �tem a ser criado.
		//no caso o primeiro �tem.
		//vamos usar a janela do menu como referencia.
		// #bugbug: Validade da estrutura hWindow.
		menuDefault->newitemLeft = hWindow->left;
		menuDefault->newitemTop = hWindow->top;
		
		//Zerar a contagem de itens quando cria um menu.
		menuDefault->itemCount = 0;

	    // registra o menu da janela.
	    // #bugbug: Validade da estrutura hWindow.
	    hWindow->defaultMenu = (void *) menuDefault; 
        RegisterMenu (menuDefault);  
		//@todo: menu id ... contagem ...
	};

	
    //
	// Array para items.
	//
	
	//Limits.
	if ( iCount < 1 )
	{
		iCount = 1;
		printf ("create_menu: Itens iCount limits < 1\n");
		//goto fail;
	}
	
	
	//Limits.
	if ( iCount >= MENUITEM_COUNT_MAX )
	{
	    iCount = ( MENUITEM_COUNT_MAX -1 );
		printf("create_menu: Itens iCount limits > max\n");
		//goto fail;
	}
	
	
	/*
	 * O array pode ser inicializado depois.
	 *
	    menuDefault->Items = kmalloc( sizeof(struct menuitem_d) * iCount );
        if( (void*) menuDefault->Items == NULL)
	    {
	        printf("create_menu error: Menu array struct.\n");
		    refresh_screen();
		   while(1){}        
	    };
	*/	
 
    //
    // Linked list. (linked list no menu)	
    //
	
    /*	
    LinkedList = (void*) kmalloc( sizeof(struct linkedlist_d) );
    if( (void*) LinkedList == NULL )
	{
	    printf("create_menu: LinkedList\n");
		refresh_screen();
		while(1){}        
	}
	else
	{  
	    //registra na estrutura da janela.
	    hWindow->linkedlist = (void*) LinkedList;
	};
	*/
	
//
// Done. 
// Retorna a janela (menubar) que � um menu.
//

done:  
    return (void *) hWindow;
    
fail:
    printf ("fail\n");
	refresh_screen ();
	return NULL;
}


/*
 ***********************************
 * create_menu_item:
 *     Create a menu item. Pinta.
 *     Obs: Os menus s�o verticais por default.
 */

// #todo:
// Precisamos da altura do �tem com base na altura da janela.
// Ou uma janela com base na quantidade de �tens, sempre da mesma altura.

//IN:
// estrtura de menu, string do �tem e status.
 
int 
create_menu_item ( struct menu_d *menu, 
                   unsigned char *string, 
                   int status )
{
    struct window_d *miWindow;

    int Selected = 0;

	//todo: 
	// Rever a cor conforma o esquema padr�o.
	unsigned long Color = COLOR_WINDOW;




	// Check 'menu' structure.
	// Isso vem via argumento.

    if( (void *) menu == NULL )
    {
        printf ("create_menu_item: menu\n");
        goto fail;
    }


	
	// Contador de �tens.
	menu->itemCount++;
	
	
	//Limits.
	if ( menu->itemCount >= MENUITEM_COUNT_MAX )
	{
        printf ("create_menu_item: menu->itemCount\n");
        goto fail;
	}
	


	//
	// # Selected #
	//
	
	Selected = status;
	menu->Items[menu->itemCount].Selected = (int) Selected;


    /*
	 * Validate item
	 *
	if( menu->Items[i].Used != 1)
	{
	    printf("menu item not Used\n");
		return 1;
	};
	*/

		
	// @todo:
	//     Checar o n�mero de itens existentes.
	//     Checar o n�mero m�ximo possivel.
	//     Checar o id do proximo menuitem.
	//     Checar a posi��o x do pr�ximo item.
	//     Continua ...


	// Ajust string.
    const char *s = (const char *) string;    

    // Posicionamento.
	//unsigned long x = (unsigned long) menu->menuWindow->left;
	//unsigned long y = (unsigned long) menu->newitemTop;
	unsigned long x = (unsigned long) 1;
	unsigned long y = (unsigned long) menu->newitemTop;
	
	// inicializando Dimens�es.
	unsigned long width = 0;
	unsigned long height = 0;
	
	menu->newitemLeft = (unsigned long) x;
	menu->newitemTop  = (unsigned long) y;
	
	
	//printf ("create_menu_item: x=%d y=%d ",x,y);
	//refresh_screen();
	//while(1){}
	
	//
	//  ## menu window ##
	//
	
	// #obs:
	// Essa � a janela do menu. 
	// � um elemento da estrutura menu, que foi passada via argumento.
	
	if ( (void *) menu->menuWindow == NULL )
	{
	    printf ("menu window fail\n");
		goto fail;
		
	}else{
		
		//??
		//Isso poderia ser a altura dividido pela quantidade de �tens??
		height = (unsigned long) (8*3);
		
		//...
		
		if(menu->Type == 0)
		{		
		    width  = (unsigned long) menu->menuWindow->width; 
            //height = (unsigned long) menu->menuWindow->height;
	        goto draw_window;
		};
		
	    //@todo: Deve ser dividido pelo n�mero de �tens.
	    if(menu->Type == 1)
		{
	        width  = (unsigned long) (menu->menuWindow->width / 4);
			//height = (unsigned long) menu->menuWindow->height;
            goto draw_window;
	    };
		
		//...
		
	};



	//
	// Draw.
	//
	
draw_window:
	
    // Selected support.
	// @rever as cores padr�o no esquema de cores.
	
	if( Selected == 1 ){ 
	    Color = COLOR_BLUE;
    }else{ 
	    Color = COLOR_BLACK; 
	};
    
    
    
    if ( (void *) menu->menuWindow != NULL )
    {
        x = menu->menuWindow->left + 1;
        y = menu->newitemTop;
    }

    
    //
	// Create Window 
	//
	
    // Desenhando o �tem de menu.

    miWindow = (void *) CreateWindow ( 1, 0, 0, "MenuItem", 
                          x, y, 
                          width, height, 
                          menu->parentWindow, 0, 0, Color );

    if ( (void *) miWindow == NULL )
    {
        printf ("create_menu_item: miWindow\n");
        goto fail;
    }else{
        RegisterWindow (miWindow); 
    };



	// ## String ##

    // Selected support.
	// @rever as cores padr�o no esquema de cores.
	
strings:
  
	if ( Selected == 1 )
	{
        Color = COLOR_WHITE;
    }else{
	    Color = COLOR_GRAY; 
	};

    draw_string ( (x +8), (y +8), Color, string ); 



	// ## next ##
	
ajust_next:
	
	switch( menu->Type )
    {
		// Vertical
		case 0:
	        menu->newitemLeft = (unsigned long) x;
	        menu->newitemTop  = (unsigned long) y + height;
		    break;
			
		// Horizontal 
		case 1:
	        menu->menuWindow->left = (unsigned long) x + width;
	        menu->menuWindow->top  = (unsigned long) y;
		    break;
		
		// default Vertical
		default:
	        menu->newitemLeft = (unsigned long) x;
	        menu->newitemTop  = (unsigned long) y + height;
		    break;
	};


    //goto done;	
		
    // Continua...	


done:     
    return 0;

fail:
    refresh_screen ();
    return (int) 1;
}


/*
 * set_current_menu:
 */

void set_current_menu ( struct menu_d *menu ){
	
	if ( (void *) menu == NULL )
	{
	    return;
    }
	
	current_menu = (int) menu->Id;
}


/*
 * get_current_menu:
 */

void *get_current_menu (void){
	
	//#todo: Limits.
	
    if (current_menu < 0)
	{
	    return NULL;
	}
	
    return (void *) menuList[current_menu];
}


/*
 ****************************************
 * RegisterMenu:
 *     Registrando um menu numa lista de menus.
 */

int RegisterMenu ( struct menu_d *menu ){

    struct menu_d *Empty;      


	//Erro de entrada.
    if ( (void *) menu == NULL )
    {
        goto fail;
    }
 

//Loop.
get_next:

	menus_count++;
	
	if( menus_count >= MENU_COUNT_MAX )
	{
	    printf("RegisterMenu: menus_count\n");
		goto fail;
	};
    
	// ## Get empty ##
	
	Empty = (void *) menuList[menus_count]; 
    
	if( (void *) Empty != NULL )
	{
		// Lets try again.
        goto get_next;
    
	}else{
		
	    // Onde est� vazio, registra o argumento.
		// coloca o endere�o da estrutura na lista 
		// de endere�os de estrutura.
		
	    menuList[menus_count] = (unsigned long) menu; 
	};  	
	
	// Nothing.
	
done:
    return 0;

fail:
    return (int) 1; 
}


/*
 ******************************************
 * initmenuArray:
 *     Cria um array de itens para um menu.
 */
 
//#todo
//isso precisa ser int. 
 
void initmenuArray ( struct menu_d *a, int initialSize ){
	
	int i;
			
	//limits.
	if ( initialSize <= 0 )
	{
		initialSize = 1; 
	}

	//Limits.
	//a->itemMax
	if (initialSize > 8)
	{
		initialSize = 8; 
	}
	
	//Struct.
    if ( (void *) a == NULL )
	{
		printf ("initmenuArray: a\n");
	    goto fail;
	}else{
		
		a->itemCount = initialSize;
		a->itemMax = 10;
	};
	
	
	
	// #importante:
	// Aloca espa�o para o array de estruturas.
	// Cont�m o espa�o de v�rias estruturas para �tem.
	
ArraySupport:
 	
	// Obs: 
	// Multiplicamos para encontrar a quantidade a ser alocada.
    // ?? #bugbug: precisamos rever isso.
    
    a->Items = (void *) kmalloc ( sizeof( struct menuitem_d ) * initialSize );	
	
	if ( (void *) a->Items == NULL )
	{
	   printf ("initmenuArray: a->Items\n");	
	   goto fail;
	};
	

	// Inicializa todos os �tens do menu. Lembrando que o �tem '0' � o �tem 
	// que aciona o control menu para a janela.
	
    for( i=0; i < initialSize; i++ )
    {
        a->Items[i].Used = 1;
		a->Items[i].Magic = 1234;
		a->Items[i].Selected = 0;
		
		//a->next = NULL;             //Next menu (cascade)
		//a->next_menuitem = NULL;    //Next menu item.
		//...
    };
	
	//
	// Control Menu:
	// ============
	//
	// O �tem '0' sempre seleciona o control menu para a
	// janela atual, mas ele nunca aparece.
	// ele � representado pelo �cone na parte superior dad janela.
    //	
	a->Items[0].Used = 1;
	a->Items[0].Magic = 1234;
	a->Items[0].Selected = 0;
	
	//goto done;
	
	// Continua ...
    	

	
done:
	return;
	
fail:
    printf("fail\n");
    return;
}


/*
// Add element to array
void insertArray( Array *a, Student element) 
{
    if( a->used == a->size )
    {
        a->size *= 2;
        a->array = (Student *)realloc(a->array, a->size * sizeof(Student));
    }

    // Copy name
    a->array[a->used].name = (char*) kmalloc(strlen(element.name) + 1);
    strcpy(a->array[a->used].name, element.name);

    // Copy ID
    a->array[a->used].ID=element.ID;

    a->used++;
}
*/


/*
void freeArray(Array *a) 
{
    // Free all name variables of each array element first
    for(int i=0; i<a->used; i++)
    {
        free(a->array[0].name);
        a->array[0].name=NULL;
    }

    // Now free the array 
    free(a->array);
    a->array = NULL;

    a->used = 0;
    a->size = 0;
}
*/


/*
 ***********************************************
 * ControlMenu:
 *     � o menu que gerencia a janela ativa com o 
 * foco de entrada.
 * Aparece no canto superior esquerdo de uma janela ou 
 * uando se clica com o bot�o direito do mouse.
 * menuControlMenu();
 *
 * #bugbug: Me parece que estou perdido nos nomes 
 * dos tipos de menu.
 */
 
int ControlMenu (struct window_d *window)
{
	//@todo: Rever isso aqui.
    return (int) MainMenu (window);	
}


/*
 ****************************************************
 * MainMenu:
 *     Cria o menu principal.
 *     Nada mais na tela, apenas um menu centralizado.
 *     ??
 */
 
// #obs
// A janela passada via argumento � o bg do menu.
// Essa � a janela do tipo menu.
 
int MainMenu (struct window_d *window){

    struct window_d *pWindow;
    struct window_d *cmWindow;
    
    //#debug
    //printf ("MainMenu: *debug initializing ...\n");

	//
	// Parent 
	//
	
	// #importante
	// Essa janela foi passada pelo aplicativo.

    if ( (void *) window == NULL )
    {
		printf ("MainMenu: pointer\n");
		goto fail;
    }


	// A janela com o foco de entrada ser� a janela m�e.
	// Se a estrutura n�o for uma estrutura v�lida, 
	// utilizaremos a janela 'gui->main' por enquanto. 
	// Somente para fins de teste. 
	
	pWindow = (struct window_d *) window;

    if ( (void *) pWindow == NULL )
    {
        pWindow = (void  *) gui->main;

        //#bugbug: 
        // Erro fatal.
        if ( (void *) pWindow == NULL )
        {            
            panic ("mainMenu: no main window");
        }
    }


	
	//Usando a janela screen anteriormente criada.
    //if( (void*) gui->screen == NULL ){
    //    return (int) 1;
    //};	
		
    //limpa a tela
	//kbackground(COLOR_BLACK);	
	
	
	//if ( (void*) gui->ClientArea == NULL )
	//{
	//	gui->ClientArea = (void*) gui->screen;
		
	//	//No Screen.
	//	if( (void*) gui->ClientArea == NULL ){
	//		return (int) 1; //erro.
	//	};		
	//};


	
	//
	//  Menu Window 
	//


	// Isso cria estrutura de menu com 4 �tens e nenhum espa�ador.
	// (O argumento � a janela m�e, selecionada anteriormente )
	//  A janela m�e tem o foco de entrada.
    // OUT: O retorno � a janela do menu.
	// @todo: 
	// A janela do control menu deve aparecer no canto
	// superior esquerdo da janela ativa com e com o foco
    // de entrada, ou pr�xima ao ponteiro do mouse quando
	// clicando o bot�o direito.
	
    cmWindow = (void *) create_menu ( pWindow, 4, 0, 0, 0 );
	
	if ( (void *) cmWindow == NULL )
	{
	    printf ("MainMenu: cmWindow\n");
		goto fail;
		
    }else{

        //Init array
        //apenas estruturas.
        initmenuArray ( cmWindow->defaultMenu, 4);
    
        // #debug
        // Antes de pintarmos os �tens.
        //printf ("MainMenu: *breakpoint\n");
        //refresh_screen();
        //while(1){}

		//
		// Create itens (Draw)
		//

        //#bugbug
        //quando criamos um �tem de menu, a janela bg dessaparece.
        //talvez seja repintada.

		// Cria um menuitem, dado um menu da janela. Agora pinta.
        create_menu_item ( cmWindow->defaultMenu, "Maximize" , 0 );
        create_menu_item ( cmWindow->defaultMenu, "Minimize" , 0 );
        create_menu_item ( cmWindow->defaultMenu, "Close   " , 1 );
        //create_menu_item( cmWindow->defaultMenu, "cmitem3" , 0 );

        //draw_text( gui->screen, 2*(640/3), 2*(480/3) , COLOR_WINDOWTEXT, "MAIN MENU"); 
        //StatusBar(gui->screen, "Status Bar", "MAIN MENU");

        // #debug
        //ok funcionou
        // Depois de pintarmos os �tens.
        //printf ("MainMenu: *breakpoint\n");
        //refresh_screen();
        //while(1){}
    };


//ProcedureSupport:
	//SetProcedure ( (unsigned long) &MainMenuProcedure );	


done: 
    return 0;


fail:
    refresh_screen();
    return -1;
}


/*
 **********************************************
 * MainMenuProcedure:
 *     Procedimento de janela do 'main menu'.
 *     #test
 */		
 
//#bugbug
//Esse tipo de procedimento deve ficar em ring3 
//assim como acontece com o message box.
 
unsigned long 
MainMenuProcedure( struct window_d *window, 
                   int msg, 
  				   unsigned long long1, 
				   unsigned long long2 ) 
{
    switch (msg)
	{	
        case MSG_KEYDOWN:
            switch (long1)
            {
                case VK_ESCAPE:
                   //KillFocus(window);				
				    SetProcedure( (unsigned long) &system_procedure);
                   	return (unsigned long) 0; 			   
				    break;
				   
                default:
                    break; 
            };
        break;
		
		case MSG_SYSKEYDOWN:                 
            switch (long1)	       
            {   
				//Screen
				case VK_F1:
				    StatusBar( gui->screen, "Status Bar screen", "F1");
                    break;
					
				//Background
                case VK_F2:
				    StatusBar( gui->background, "Status Bar bg", "F2");
                    break;
					
				//Logo
                case VK_F3:
				    StatusBar( gui->logo, "Status Bar logo", "F3");
                    break;

				//TaskBar
                case VK_F4:
				    StatusBar( gui->taskbar, "Status Bar taskbar", "F4");
                    break;
					
				//Main Window
                case VK_F5:
				    StatusBar( gui->main, "Status Bar main", "F5");
                    break;	    

				//Navigation Bar
                case VK_F6:
				    StatusBar( gui->navigationbar, "Status Bar navigationbar", "F6");
                    break;
					
				default:
                    return (unsigned long) 0;	
				    break;
		    };              
        break;
		
	    case MSG_SYSKEYUP:
           //Nothing.		
        break;
		
		default:
		    //Nothing.
		    break;
	};
	
// Done.

done:

    refresh_screen();
	return (unsigned long) 0;
}


/*
 **************************************
 * init_menus:
 *     Inicializa a lista de menus.
 */

int init_menus (void){
	
	int i=0;
	
	for ( i=0; i < MENU_COUNT_MAX; i++ )
	{
	    menuList[i] = (unsigned long) 0;
	}
	
	set_current_menu (NULL);
	
	menus_count = 0;

	// Continua ...
	
	return 0;
}


//
// End.
//

