/*
 * File: gws/button.c
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


					
void updateButton ( struct button_d *button,
                    struct window_d *window,
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
	
    if ( (void *) button == NULL )
    {
	    return;	
	} else {
		
		if ( button->used != 1 || button->magic != 1234 )
		{
		    return;	
		}
		
		//tratar isso no futuro.
		//button->window = (struct window_d *) window;
		
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

};


/*
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
 *
 */

void *draw_button ( struct window_d *window,
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
	int Focus; //(precisa de borda)
	int Selected;
	unsigned long border1;
    unsigned long border2;
	
	struct button_d *b;
	
	//validade da janela onde o bot�o est�.
	if ( (void *) window == NULL )
	{
	    return NULL;
	}
	
	// Alocando mem�ria para a estrutura do bot�o.
	// Inicializando a estrutura.
	
	b = (void *) malloc ( sizeof(struct button_d) );
    
	if ( (void *) b == NULL )
	{
		return NULL;
		
	}else{
		
		// Object.
		b->objectType = ObjectTypeButton;
		b->objectClass = ObjectClassGuiObjects;
		
	    b->used = 1;	
	    b->magic = 1234;
		
        //button states:
        //1. Default
        //2. Focus
        //3. Expanded/Toggled/Selected
        //4. Disabled
        //5. Hover and Active	
	
		b->state = (int) state;
		
		b->window = (void *) window; 
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
	
	//Todo: Usar esquema padr�o de cores.
		
	//
	//  ## State ##
	//
	
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
		    border1 = COLOR_BUTTONHIGHLIGHT2;
			border2 = COLOR_BUTTONSHADOW2;
			b->border1 = COLOR_BUTTONHIGHLIGHT2;
			b->border2 = COLOR_BUTTONSHADOW2;
			//b->color = color; 	
            //goto do_draw_button;			
			break;
			
		case BS_FOCUS:
		    border1 = COLOR_BLUE;
			border2 = COLOR_BLUE;
            b->border1 = COLOR_BLUE;
			b->border2 = COLOR_BLUE;		
            //b->color = color; 	
			break;	
                                                       
         // Pressionado.
        case BS_PRESS:
		    Selected = 1;
			b->selected = 1;
		    border1 = COLOR_BUTTONHIGHLIGHT2;
			border2 = COLOR_BUTTONSHADOW2;
            b->border1 = COLOR_BUTTONHIGHLIGHT2;
			b->border2 = COLOR_BUTTONSHADOW2;
			//b->color = color; 	
			//goto do_draw_button;			
            break;
			
		case BS_HOVER:
            b->color = (b->color + 20); 	
			break;
        
		case BS_DISABLED:
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
   
    // Temos a inten��o de uar estilos diferentes, como flat design,
	// por exemplo.
	
	//bg
	drawDataRectangle ( window->left +x, window->top +y, 
	    width, height, color );
    
	//board1, borda de cima e esquerda.
	drawDataRectangle ( window->left +x, window->top +y, 
	    width, 1, border1 );
		
	drawDataRectangle ( window->left +x, window->top +y, 
	    1, height, border1 );

	//board2, borda direita e baixo.
	drawDataRectangle ( window->left +x +width -1, window->top +y, 
		1, height, border2 );
					   
	drawDataRectangle ( window->left +x, window->top +y +height -1, 
		width, 1, border2 );

//
// Do draw label.
//
					   
//do_draw_label:

	
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
	    draw_string ( window->left +x +offset, window->top +y +8, 
			COLOR_WHITE, string );	
			
    }else{
		
		//b->width
		//b->height
		
		// (window->left +x) left 
		// (largura do bot�o, menos a largura da string)/2

	    draw_string ( window->left +x +offset, window->top +y +8, 
			COLOR_TERMINALTEXT, string );	
			
	};
	
	//
	// @todo: Algo mais?
	//
	
// Done! 
//done:
    
	//Retornando o ponteiro para a estrutura do bot�o.
    return (void *) b;          
};






/*
int buttonInit()
{}
*/


//
// End.
//

