/*
 * File: kgws/kgws/comp/grid.c
 *
 *     Grid support.
 *
 * History:
 *     2015 - Created by Fred Nora.
 */


#include <kernel.h>


/*
 ********************************************
 * InitializeGridObject:
 *     Desenha o grid.
 * 
 * IN: Background window, grid structure, number of itens, view.  
 */

int 
InitializeGridObject ( struct window_d *window, 
                       struct grid_d *g, 
                       int n, 
                       int view )
{

    struct window_d *gridWindow;        //bg.
    struct window_d *griditemWindow;    //item atual.

    int i = 0;


    //deslocamento em rela��o � janela m�e.
    unsigned long WindowLeft = 0;
    unsigned long WindowTop = 0;
    unsigned long WindowWidth = 0;
    unsigned long WindowHeight = 0;
    
    //deslocamento em rela��o � janela background do grid,
    //que ainda vamos criar.
    unsigned long x, y, cx, cy;


	//quantidade de itens.
    unsigned long ItensCount = (unsigned long) n; 



	// dimens�es dos bot�es.
	x = 0; //deslocamento em rela��o � margem da janela de background 
	y = 0; //deslocamento em rela��o � margem da janela de background 
	


	if ( (void *) g == NULL )
	{
		printf ("InitializeGridObject: g\n");
		goto fail;
		
	}else{
		
	    if ( g->used != 1 || g->magic != 1234 )
	    {
		    printf ("InitializeGridObject: g validation\n");
		    goto fail;
	    }
		//...
	};



    if ( (void *) window == NULL )
    {
		//fail;
		printf ("InitializeGridObject: window\n");
		goto fail;
		
    }else{
		
        if ( window->used != 1 || window->magic != 1234 )
        {
            printf ("InitializeGridObject: window validation\n");
            goto fail; 
        }

        // Posiconamento do bg.
        // Onde ele ficar� em rela��o � janela m�e passada pelo app.
        
        // #importante
        // O background ser� do tamanho da janela m�e passada pelo app.

        WindowLeft = window->left;
        WindowTop = window->top;
        WindowWidth = (window->width); 
        WindowHeight = (window->height);


		//@todo: filtros.
		
        if ( view == GRID_HORIZONTAL )
        {

		    // #aten��o; Cuidado para n�o dividir por zero.
			cx = (unsigned long) (WindowWidth/ItensCount);    //largura do �tem.
	        cy = (unsigned long) (WindowHeight);              //altura do �tem.
		
	        //horizontal..
		    //muda o posicionamento e o tamanho.
			//mas temos que atuar em uma janela j� registrada.
			//estamos atuando sobre a janela main modificando o tamanho dela 
			//de acordo com o grid criado. Mas n�o � isso que queromos por enquanto.
			
	        //replace_window( gui->main, 0, WindowHeight );        //left, top
	        //resize_window( gui->main, 800, (600-WindowHeight) ); //width, height
        }

       if( view == GRID_VERTICAL )
       {

			cx = (unsigned long) (WindowWidth);                 //largura do �tem.
	        cy = (unsigned long) (WindowHeight/ItensCount);     //altura do �tem.
		
		    //vertical.
		    //muda o posicionamento e o tamanho.
			//mas temos que atuar em uma janela j� registrada.
			//estamos atuando sobre a janela main modificando o tamanho dela 
			//de acordo com o grid criado. Mas n�o � isso que queromos por enquanto.
			
	        //replace_window( gui->main, WindowWidth, 0 );        //left, top
	        //resize_window( gui->main, (800-WindowWidth), 600 ); //width, height
	    }
	    //...
	};


	//
	// Background
	//

	// Background do grid da janela principal.
	// o grid pertence a janela m�e, passada pelo aplicativo. 
	
	// #importante
	// O bg � do tamanho da janela m�e passada pelo app.

    gridWindow = (void *) CreateWindow ( 1, 0, 0, "grid-bg", 
                              WindowLeft, WindowTop, 
                              WindowWidth, WindowHeight, 
                              window, 0, COLOR_BACKGROUND, 0 ); 

    if ( (void *) gridWindow == NULL )
    {
        printf ("InitializeGridObject: gridWindow\n");
        goto fail;
    }else{
        RegisterWindow (gridWindow);
    };


	//
	// Pintando os �tens.
	//
	
	
	// #obs:
	// Nessa hora devemos pegar o nome dos processos e 
	// colocar como etiqueta do �tem.
	
	unsigned char *string; //label.
	string = "$";
	
	//Os processos de usu�rio come�am em 100.
	i = 100;
	ItensCount += 100;
	

    if ( (void *) gridWindow != NULL )
    {
        x = gridWindow->left + x;
        y = gridWindow->top + y;
    }

	
	//draw (horizontal.)
    while (i < ItensCount)
    {
		//grid item 
		//#bugbug tem que usar create window.
	    //griditemWindow = (void*) draw_button( gridWindow, string, 1, x, y, cx, cy, COLOR_BUTTONFACE);   

        griditemWindow = (void *) CreateWindow ( WT_BUTTON, 0, 0, "Item", 
                                      x, y, cx, cy, 
                                      gridWindow, 0, 
                                      COLOR_TERMINAL, COLOR_TERMINAL ); 

        if ( (void *) griditemWindow == NULL )
        {
            printf ("InitializeGridObject: griditemWindow\n");
            goto fail;
        }else{
            RegisterWindow (griditemWindow);
        };


		//@todo: fazer a lista encadeada de bot�es.
		//g->b = (void*) griditemWindow;

		// horizontal.
        if ( view == GRID_HORIZONTAL )
        {
            x = (unsigned long) x + cx + 2; 
        }

		// vertical.
        if ( view == GRID_VERTICAL )
        {
             y = (unsigned long) y + cy;
			//y = (unsigned long) y + cy + 2;   
        }   

        i++;
    };


    refresh_rectangle ( WindowLeft, WindowTop, 
        WindowWidth, WindowHeight );

    return 0;


fail:

    refresh_screen ();
    return (int) 1;
}


/*
 *******************************************************
 * CreateGrid:
 *     Create Grid Object,
 *     Inicializa a estrutura do grid da janela principal. 
 * �rea de trabalho.
 * @todo: #define gridObject CreateGrid
 */

void *CreateGrid (void){

    struct grid_d *g;

    g = (void *) kmalloc ( sizeof( struct grid_d) );

    if ( (void *) g == NULL )
    {
		//@todo: Message.
		return NULL;
		
    } else {

		//Object.
		g->objectType = ObjectTypeGrid;
		g->objectClass = ObjectClassGuiObjects;
		
		g->used = 1;
		g->magic = 1234;
		g->window = NULL;
		g->b = NULL;
		g->count = 0;
		//...
		
		g->next = NULL;
    };


    return (void *) g;
}


/*
 ******************************
 * grid:  
 * 
 * ??
 * fun��o para criar o grid gerenciado pelo procedimento de janela do sistema.
 * 
 * IN: window, number of �tens, view mode.
 */
 
// IN: window =  Janela m�e do menu.
//     Essa � a janela que o aplicativo tem ci�ncia.

int 
grid ( struct window_d *window, 
       int n, 
       int view )
{
    struct grid_d *g;

    int Status = 1; 
    
    
    if ( (void *) window == NULL )
    {
        printf ("grid: pointer\n");
        return -1;
    }


	// Cria o objeto grid. 
	// Apenas a estrutura.

    g = (void *) CreateGrid ();

    if ( (void *) g == NULL )
    {
        printf ("grid: fail\n");
        refresh_screen ();
        return -1;

    }else{

		//#bugbug: >>> N�o devemos fazer isso.
		//Ele ser� o grid do kernel.
		
        GRID = (void *) g;

		// Inicializaremos a estrutura e pintaremos o grid.
		// Passaremos a janela m�e.
		
		//#bubug
		//mas nesse momento dever�amos ter o posicionamento desejado.

        Status = (int) InitializeGridObject ( window, g, n, view );
    };


    return (int) Status;
}


//
// End.
//
