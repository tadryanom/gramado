/*
 * File: kgws/comp/bg.c 
 *
 * Descri��o:
 *     Rotinas de configura��o do background de um desktop.
 *     O background de um desktop � onde fica a imagem de pano de fundo.
 * Tamb�m � conhecido como Backbuffer.
 *
 * Window server.
 *
 * History:
 *     2013 - Created by Fred Nora.
 *     2016 - Revision.
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

 
/*
 * backgroundDraw:
 *     Draw device screen in graphics mode.
 *     Clear the screen in text mode.
 *     #todo get device info.
 */

// #aten��o: rotina sens�vel.

void backgroundDraw (unsigned long color){
	
    unsigned long i=0;
		
    if (VideoBlock.useGui != 1)
	    return;
	
    for ( i=0; i<SavedY; i++ )
	{			
        my_buffer_horizontal_line ( 0, i, SavedX, color );
	}
	
	//#bugbug
	//estamos determinando as dimensoes do char.
	//?? ja' podemos usar a vari�vel. ??
	
	//Cursor.
	TTY[current_vc].cursor_x = 0;
	TTY[current_vc].cursor_y = 0;
	
	// #bugbug
	// Ser� que nesse momento as dimens�es do char j� est�o configuradas ??
	
	//g_cursor_right = (SavedX/8);
	//g_cursor_bottom = (SavedY/8);
}


/* backgroundSetColor:
 *     Set background color in graphics mode. */

void backgroundSetColor (unsigned long color)
{
    //#cancelada	
}


/*
 * backgroundRedraw:
 *     Redraw bg. 
 */

void backgroundRedraw (unsigned long color){
	
    if ( gui->backgroundStatus == 0 )
    {	
        return;
    }
	
	if ( (void *) gui->background != NULL )
	{	
	   backgroundDraw (color);	
	}
}


//Constructor.

void backgroundBackground (void){
	
	TTY[current_vc].cursor_x = 0;
	TTY[current_vc].cursor_y = 0; 
}


//Init.

int backgroundInit (void){
	
	unsigned long *lfb = (unsigned long *) BACKBUFFER_VA;
	int i=0;
		
	// #bugbug:
	// As dimens�es precisam de vari�veis.	
		
	for ( i=0; i< 800*600; i++ )
		lfb[i] = COLOR_BLACK;
	
	// #todo:
	// Ainda n�o implementada.
	
	backgroundBackground ();
	
	//...
	
	return 0; 
}


//
//End.
//

