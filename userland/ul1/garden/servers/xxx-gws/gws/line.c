/*
 * File: gws/gws/line.c
 *
 * Descri��o:
 *     Rotinas de pintura de linha.
 *     Faz parte do m�dulo Window Manager do tipo MB.
 *
 * History:
 *     2015 - Created by Fred Nora.
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
 * lineDrawHorizontalLineWindowBuffer:
 *     Pinta uma linha horizontal no buffer da janela.
 *     Serve para pintar janelas que ir�o direto do seu buffer para o LFB da 
 * mem�ria de v�deo, sem passar pelo back buffer. (OVERLAPPED)
 *
 */
/* 
void lineDrawHorizontalLineWindowBuffer(struct window_d *window,
                                        unsigned long x1,
										unsigned long y,
										unsigned long x2,
										unsigned long color );
										
void lineDrawHorizontalLineWindowBuffer(struct window_d *window,
                                        unsigned long x1,
										unsigned long y,
										unsigned long x2,
										unsigned long color )
{
    return;	
}
*/



/* my_buffer_horizontal_line:
 *     Draw a horizontal line on backbuffer. */

void 
my_buffer_horizontal_line( unsigned long x1,
                           unsigned long y, 
						   unsigned long x2,  
						   unsigned long color )
{
	while (x1 < x2){
		
		backbuffer_putpixel ( color, x1, y, 0 );
        x1++;  
    }
};

					  
void 
refresh_horizontal_line ( unsigned long x1,
                          unsigned long y, 
	  				      unsigned long x2 )
{
	
	int bytes_count;
	
	switch (SavedBPP)
	{
		case 32:
		    bytes_count = 4;
		    break;
		
		case 24:
		    bytes_count = 3;
			break;
			
		//#todo 
		//#bugbug	
		
		default:
			break;
	}
	
	// #importante 
	// Usando a largura salva.
	
	int width = (int) SavedX;  	
	
	void *s = (void *) (BACKBUFFER_ADDRESS)  + (y * bytes_count * width) + (x1 * bytes_count);
    void *d = (void *) (FRONTBUFFER_ADDRESS) + (y * bytes_count * width) + (x2 * bytes_count);
	
	// ??
	// N�o pode ser isso. 
	// Tem que ser uma string de tamanho definido.
	// strcpy(d,s);	
	
	//#bugbug
	//Essa '3' precisa ser uma vari�vel
	
	unsigned long size = (unsigned long) ( (x2-x1) * bytes_count );
	//unsigned long size = (unsigned long) ( (x2-x1) * 3 ); 
	
	memcpy ( (void *) d, (const void *) s, size );
}


//
// End.
//

