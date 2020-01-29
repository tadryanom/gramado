/*
 * File: kgws/button.c
 *
 * Descri��o:
 *     Rotinas de cria��o e gerenciamento de bot�es. 
 *     Faz parte do m�dulo Window Manager do tipo MB.
 *
 * Refletindo:
 *    Talvez seja certo criar op��es de rotinas de cria��o de bot�es.
 * Por exemplo: draw_3d_button e draw_flat_button 
 * seria muito confuso uma rotina s� que acione o tipo via argumento.
 *
 *  #todo: precisamos de uma fun��o de redraw button 
 *         para o caso de redraw da janela do tipo bot�o.
 *
 * History:
 *     2015 - Created by Fred Nora.
 *     2018 - redraw_button created. #todo  
 */


#include <kernel.h>


// Atualizando as caracter�sticas do bot�o antes de repint�-lo.

void 
update_button ( struct button_d *button,
                unsigned char *string,
                int style,
                int state,
                int type,
                unsigned long x, 
                unsigned long y, 
                unsigned long width, 
                unsigned long height, 
                unsigned long color )
{
	
	struct window_d *window;
	
    if ( (void *) button == NULL )
    {
		//printf ("update_button\n");
	    return;
		
	}else{
		
		if ( button->used != 1 || button->magic != 1234 )
		{
			//printf ("update_button validation\n");
		    return;
		}
		
		//tratar isso no futuro.
		window = button->window;
		
		button->string = (unsigned char *) string;
		
		button->style = (int) style;
		button->state = (int) state;
		button->type = (int) type;
		
		button->x = (unsigned long) x;
		button->y = (unsigned long) y;
		button->width = (unsigned long) width; 
		button->height = (unsigned long) height;

		button->color = (unsigned long) color;
	};
	
    //
	//  ## State ##
	//

	// #todo: 
	// Usar esquema padr�o de cores.
	
    //button states:
    //0. NULL.
	//1. Default 
    //2. Focus
    //3. Expanded/Toggled/Selected
    //4. Disabled
    //5. Hover and Active

    switch (state)
    {
        case BS_NULL:
            break;

        //N�o pressionado.
		case BS_DEFAULT:
			button->selected = 0;
            button->border1 = COLOR_BUTTONHIGHLIGHT3;
			button->border2 = COLOR_BUTTONSHADOW3;
			break;

		case BS_FOCUS:
            button->border1 = COLOR_BLUE;
			button->border2 = COLOR_BLUE;
			break;
 
         // Pressionado.
        case BS_PRESS:
			button->selected = 1;
			button->border1 = COLOR_BUTTONSHADOW3;
			button->border2 = COLOR_BUTTONHIGHLIGHT3;
            break;
			
		case BS_HOVER:
            button->color = (button->color + 20);
			break;
        
		case BS_DISABLED:
			//b->selected = 0;
            button->border1 = COLOR_GRAY;
			button->border2 = COLOR_GRAY;
            button->color = COLOR_GRAY; 
			break;

        case BS_PROGRESS:
            break;

		// Valor inv�lido.
		default:  
		    return;
		    //return NULL; 
            break;    
    };  
}



/*
 *****************************************
 * draw_button:
 *     Desenha um bot�o dado um tipo.
 *     
 * @todo: Medir o tamanho da string da tag.
 * @todo: O bot�o precisa de uma etiqueta, que pode ser uma string ou uma imagem.
 * Talvez uma estrutura de bot�o seja necess�rio, para armazenas coisas como 
 * deslocamento dentro da janela e endere�o da imagem carregada como etiqueta.
 *
 * #importante: 
 * Retorna o ponteiro pra estrutura do bot�o.
 */


//#todo
//Precisamos usar o esquema de cores.

void *draw_button ( unsigned char *string,
                    int style,
                    int state,
                    int type, 
                    unsigned long x, 
                    unsigned long y, 
                    unsigned long width, 
                    unsigned long height, 
                    unsigned long color )
{

    int Focus;    //(precisa de borda)
    int Selected;
    unsigned long border1;
    unsigned long border2;

    struct button_d *b;


	// Validade da janela onde o bot�o est�.
	// #todo: cheacr used, magic.
	
	//if ( (void *) window == NULL )
	//{
	//    return NULL;
	//}
	
	// Alocando mem�ria para a estrutura do bot�o.
	// Inicializando a estrutura.

    b = (void *) kmalloc ( sizeof(struct button_d) );

    if ( (void *) b == NULL )
    {
		return NULL;
    }else{

		// Object.
        b->objectType = ObjectTypeButton;
        b->objectClass = ObjectClassGuiObjects;

        b->used = 1;
        b->magic = 1234;

        // button states:
        // 1. Default
        // 2. Focus
        // 3. Expanded/Toggled/Selected
        // 4. Disabled
        // 5. Hover and Active

		b->state = (int) state;

		//b->window = (void *) window; 
		b->string = string; 

        b->x = x;
        b->y = y;
        b->width = width;
        b->height = height;
 
        b->color = color; 
 
		b->Next = NULL; 
		//...
    };


	//Devemos colocar o ponteiro na lista encadeada de bot�es 
	//dentro da estrutura da janela.
	//if( (void*) window->buttonList == NULL ){
	//	window->buttonList = (void*)b;
	//}


	//
	//  ## State ##
	//

	// #todo: 
	// Usar esquema padr�o de cores.
	
    //button states:
    //0. NULL.
	//1. Default 
    //2. Focus
    //3. Expanded/Toggled/Selected
    //4. Disabled
    //5. Hover and Active

    switch (state)
    {
        case BS_NULL:
            break;

        //N�o pressionado.
		case BS_DEFAULT:
		    Selected = 0;
			b->selected = 0;
		    border1 = COLOR_BUTTONHIGHLIGHT3;
			border2 = COLOR_BUTTONSHADOW3;
			b->border1 = COLOR_BUTTONHIGHLIGHT3;
			b->border2 = COLOR_BUTTONSHADOW3;
			break;

		case BS_FOCUS:
            border1 = COLOR_BLUE;
			border2 = COLOR_BLUE;
            b->border1 = COLOR_BLUE;
			b->border2 = COLOR_BLUE;
			break;
 
         // Pressionado.
        case BS_PRESS:
		    Selected = 1;
			b->selected = 1;
		    border1 = COLOR_BUTTONHIGHLIGHT3;
			border2 = COLOR_BUTTONSHADOW3;
			b->border1 = COLOR_BUTTONSHADOW3;
			b->border2 = COLOR_BUTTONHIGHLIGHT3;
            break;
			
		case BS_HOVER:
            b->color = (b->color + 20);
			break;
        
		case BS_DISABLED:
		    //Selected = 0;
			//b->selected = 0;
		    border1 = COLOR_GRAY;
			border2 = COLOR_GRAY;
            b->border1 = COLOR_GRAY;
			b->border2 = COLOR_GRAY;
            b->color = COLOR_GRAY; 
			break;

        case BS_PROGRESS:
            break;

		// Valor inv�lido.
		default:  
		    return NULL; 
            break;    
    };  
    
    


//
// Do draw the button.
//
	
//do_draw_button:

	// Usaremos ret�ngulos para desenharmos o bot�o.
   
    // Temos a inten��o de usar estilos diferentes, como flat design,
	// por exemplo.
	
	//
	// ## bg ##
	//
	
	drawDataRectangle ( x, y, width, height, color );
    
	//
	// ## 4 bordas ##
	//

	// #todo
	// As cores das bordas deve estar no esquema de cores.
	
	//board1, borda de cima e esquerda.
	drawDataRectangle ( x, y, width, 1, border1 );
	drawDataRectangle ( x, y, 1, height, border1 );

	//board2, borda direita e baixo.
	drawDataRectangle ( x +width -1, y, 1, height, border2 );
	drawDataRectangle ( x, y +height -1, width, 1, border2 );

	
    //
    // Do draw label.
    //
	
	// Se vamos usar uma imagem ao inv�s de uma string.
	//if(useImage == 1{ ... goto done;}
	
    //usado para calcular o tamanho de uma string.
     size_t tmp_size = (size_t) strlen ( (const char *) string );
	
	//if ( tmp_size > (width/8) )
	//{
	//    printf("fail");
	//}
	
	//(a largura do bot�o menos a largura da string)/2
    unsigned long offset = ( ( (unsigned long) width - ( (unsigned long) tmp_size * (unsigned long) gcharWidth) ) / 2 );

	//button label
    if (Selected == 1)
    {
        draw_string ( x +offset,y +8, COLOR_WHITE, string );

    }else{

		//b->width
		//b->height

		// (window->left +x) left 
		// (largura do bot�o, menos a largura da string)/2

        draw_string ( x +offset, y +8, COLOR_TERMINALTEXT, string );

    };

	//Retornando o ponteiro para a estrutura do bot�o.
    return (void *) b;  
}


/*
 ********************************* 
 * redraw_button: 
 * 
 */
 
// Repintar o bot�o com base nas caracter�sticas 
// encontradas na estrutura.

int redraw_button ( struct button_d *button ){


    struct window_d *w;

	//int Focus; //(precisa de borda)
	//int Selected;
	//unsigned long border1;
	//unsigned long border2;



    if ( (void *) button == NULL )
    {
		return 1;

    }else{

		//pega a janela.
        w = button->window;

		if ( (void *) w == NULL )
		{
            printf ("redraw_button: window\n");
			return 1;
		}else{
		
		    if ( w->used != 1 || w->magic != 1234 )
			{
				return 1;
			}
		}
		
		//...
    };


	//bg
    drawDataRectangle ( button->x, button->y, 
        button->width, button->height, button->color );


	//board1, borda de cima e esquerda.
    drawDataRectangle ( button->x, button->y, 
        button->width, 1, button->border1 );

    drawDataRectangle ( button->x, button->y, 
        1, button->height, button->border1 );

	//board2, borda direita e baixo.
    drawDataRectangle ( button->x + button->width -1, button->y, 
        1, button->height, button->border2 );

    drawDataRectangle (button->x,  button->y + button->height -1, 
        button->width, 1, button->border2 );



    //usado para calcular o tamanho de uma string.
     size_t tmp_size = (size_t) strlen ( (const char *) button->string );

	//if ( tmp_size > (width/8) )
	//{
	//    printf("fail");
	//}

	//(a largura do bot�o menos a largura da string)/2
	unsigned long offset = ( ( (unsigned long) button->width - ( (unsigned long) tmp_size * (unsigned long) gcharWidth) ) / 2 );

	//button label
    if (button->selected == 1)
    {
        //draw_string ( w->left + button->x +offset, w->top + button->y +8, 
            //COLOR_WHITE, button->string );

        draw_string ( button->x +offset, button->y +8, 
            COLOR_WHITE, button->string );

    }else{

		//b->width
		//b->height

		// (window->left +x) left 
		// (largura do bot�o, menos a largura da string)/2

        //draw_string ( w->left + button->x +offset, w->top  + button->y +8, 
            //COLOR_TERMINALTEXT, button->string );

        draw_string ( button->x +offset, button->y +8, 
            COLOR_TERMINALTEXT, button->string );
    };


    return 0;
}


    //quando um bot�o � clicado ou pressionado,
    //ele ser� repintado com a apar�ncia de bot�o apertado.
int button_down ( struct window_d *window ){

    if ( (void *) window == NULL )
    {
		printf ("window\n");
		refresh_screen();
	    return -1;
	}else{

	    if ( window->isButton != 1 )
	    {
			printf ("isbutton?\n");
			refresh_screen();
			return -1;
		}
		
		// window->button aqui fica a estrutura de bot�o
		// caso a janela for um bot�o.
		//window->button

		//printf ("update button struct \n");
		//refresh_screen();
		
        update_button ( (struct button_d *) window->button,
             (unsigned char *) window->button->string,
             (int) window->button->style,
             (int) BS_PRESS,
             (int) window->button->type,
             (unsigned long) window->button->x, 
             (unsigned long) window->button->y, 
             (unsigned long) window->button->width, 
             (unsigned long) window->button->height, 
             (unsigned long) window->button->color );

		//printf ("redraw it x=%d y=%d \n", window->button->x, window->button->y);
		//refresh_screen();
		
		redraw_button ( (struct button_d *) window->button );
		show_window_rect (window);
	};

    return 0;
}


int button_up ( struct window_d *window ){

    if ( (void *) window == NULL )
    {
	    return -1;
	}else{
	    if ( window->isButton != 1 )
	    {
			return -1;
		}
		
		// window->button aqui fica a estrutura de bot�o
		// caso a janela for um bot�o.
		//window->button

        update_button ( (struct button_d *) window->button,
             (unsigned char *) window->button->string,
             (int) window->button->style,
             (int) BS_DEFAULT,
             (int) window->button->type,
             (unsigned long) window->button->x, 
             (unsigned long) window->button->y, 
             (unsigned long) window->button->width, 
             (unsigned long) window->button->height, 
             (unsigned long) window->button->color );

			redraw_button ( (struct button_d *) window->button );
			show_window_rect (window);
	};

    return 0;
}



//
// End.
//

