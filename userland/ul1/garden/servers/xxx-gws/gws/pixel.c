/*
 * File: gws\gws\pixel.c
 *
 * Descri��o:
 *     Rotinas de pintura de pixel.
 *     Faz parte do m�dulo Window Manager do tipo MB.
 *
 * *IMPORTANTE
 * Obs: Se uma estrutura de janela for passada como argumento
 *      na hora de colocar um pixel ent�o usaremos o buffer dedicado
 * indicado nessa estutura. Caso contr�rio, se n�o for passado
 * um ponteiro pra estrutura de janela, ou se estrutura n�o tem um 
 * ponteiro para buffer dedicado, ent�o usaremos um buffer padr�o,
 * que sempre ser� o buffer dedicado da janela principal, gui->main.
 * que podemos chamar de 'desktop', mas n�o � a estrutura de desktop
 * como a conhecemos. 
 *
 * Uma solu��o elegante seria ter ponteiros para buffers na 
 * estrutura da gui->, assim podemos alocar mem�ria para um buffer
 * gen�rico, e seu ponteiro ser passado para a janela que se interessar 
 * por ela. As estruturas de user session, windos station, e desktop
 * tambem podem ter ponteiros para buffers gen�ricos. Podemos tamb�m
 * ter uma vari�vel global para isso e tamb�m estrututra de buffer.
 *
 *
 * A solu��o mais simples � passar para a rotina de pintura o buffer desejado,
 * n�o a estrutura de buffer e sim apenas o ponteiro para o buffer. se bem que 
 * se fosse passado um ponteiro para um estrutura de buffer seria bem mais seguro.
 *
 * Vers�o 1.0, 2015.
 */

 
#include <kernel.h>


//
// Vari�veis importadas. 
//


//Herdadas do Boot Loader.
// De onde vem isso ?? head.s
// @todo: Devemos chamar o m�dulo hal para obtermos esses valores.
//depois salvamos em vari�veis internas usadas pela gui.
extern unsigned long SavedBootBlock;
extern unsigned long SavedLFB;
extern unsigned long SavedX;
extern unsigned long SavedY;
extern unsigned long SavedBPP; 

//Outras.
// De onde vem isso ??
// @todo: Devemos chamar o m�dulo hal.
extern void gui_buffer_putpixel();


//
// @todo:
// Criar a rotina guiRefreshScreen(); ??
//
//

/*
//Pinta um pixel diretamente na mem�ria de v�deo.
//Obs: A sincroniza��o vertical deve ser feita na hora de pintar
//um caractere, e n�o a cada pixel pintado.
//Por�m se um aplicativo em user mode desejar pintar apenas um pixel,
//� necess�rio fazer a sincrniza��o vertical.
void pixelDirectPutPixel( void* FrontBuffer, unsigned long x, unsigned long y, unsigned long color );
void pixelDirectPutPixel( void* FrontBuffer, unsigned long x, unsigned long y, unsigned long color )
{
    return; //ainda n�o implementada.							  
}
*/


										 
//Pinta um pixel em um buffer de janela.
void 
pixelPutPixelWindowBuffer( void *buffer, 
                           unsigned long x, 
						   unsigned long y, 
						   unsigned long color )
{
	//return; //@todo: Ainda n�o implementada.					  
};

										 
/*
 * *IMPORTANTE
 * Obs: Se uma estrutura de janela for passada como argumento
 *      na hora de colocar um pixel ent�o usaremos o buffer dedicado
 * indicado nessa estutura. Caso contr�rio, se n�o for passado
 * um ponteiro pra estrutura de janela, ou se estrutura n�o tem um 
 * ponteiro para buffer dedicado, ent�o usaremos um buffer padr�o,
 * que sempre ser� o buffer dedicado da janela principal, gui->main.
 * que podemos chamar de 'desktop', mas n�o � a estrutura de desktop
 * como a conhecemos.
*/										 
void 
pixelPutPixelDedicatedWindowBuffer( struct window_d *window, 
                                    unsigned long x, 
									unsigned long y, 
									unsigned long color )
{
	//O buffer ser� selecionado por essa rotina ficar� aqui
	void *buff;
	
	//Se temos uma estrutura de janela, essa estrutura pode
	//conter um ponteiro para um buffer dedicado de janela.
	if ( (void *) window == NULL )
	{
		buff = NULL;
		goto useDefaultBuffer;
		
	}else{
		
		//Se o ponteiro para buffer dedicado indicado na estrutura
		//for inv�lido.
		
		if ( (void *) window->DedicatedBuffer == NULL )
		{
			goto useDefaultBuffer;
		}
		
		//Ok.
		//O ponteiro para o buffer dedicado parece v�lido.
		//@todo: A rotina de cria��o de estrutura de janela
		//tem que alocar din�micamente mem�ria para o buffer corretamente.
		//e n�o pode admitir falha nesse requesito.
		
		buff = (void *) window->DedicatedBuffer;
		goto useDedicatedBuffer;
	};
	
	//Nothing.
	
// Somos obrigados a usar o buffer dedicado da janela principal.	
//@por enquanto isso n�o ser� considerado uma falha.
useDefaultBuffer:

	//@todo: pegar o buffer default.
	buff = (void *) gui->defaultWindowDedicatedBuffer1;
	
	if ( (void *) buff == NULL )
	{
		//@todo: Alocamos mem�ria par a o buffer
		
		//buff = (void*) malloc(SIZE??);
		
		printf("pixelPutPixelDedicatedWindowBuffer:");
		die();
		//refresh_screen();
		//while(1){}
	};
	
	//...pinta asm_putpixel()
//Aqui conseguimos encontrar o buffer dedicado da janela em quest�o.	
//O buffer selecionad foi o buffer dedicado da janela. temos que checar.
useDedicatedBuffer:	

	if ( (void *) buff == NULL )
	{
		//@todo: Alocamos mem�ria par a o buffer
		
		//buff = (void*) malloc(SIZE??);
		
		printf("pixelPutPixelDedicatedWindowBuffer:");
		die();
		
		//refresh_screen();
		//while(1){}
	}
	//pinta. asm_putpixel()
    return;	
};


/*
 ******************************************************
 * my_buffer_put_pixel:
 *     Coloca um pixel no BACKBUFFER. 
 *     Obs: O backbuffer dever� ser o buffer dedicado da janela principal gui->main.     
 *
 * a = cor
 * b = x
 * c = y
 * d = null
 *
 * O lugar dessa rotina � no hal.
 *
 * @todo: Aqui podemos fazer algum tipo de filtro, considerar
 * a m�trica do buffer e chamar a rotina de /hal apropriada.
 * Usando o m�dulo /hal como interface entre
 * a /gui e o hardware.
 */
//void pixelPutPixelBackBuffer( unsigned long ax, unsigned long bx, unsigned long cx, unsigned long dx )
/*
void 
my_buffer_put_pixel( unsigned long ax, 
                     unsigned long bx, 
				     unsigned long cx, 
					 unsigned long dx )
{
    
	//antigo. isso funciona.
	//hal_backbuffer_putpixel ( ax, bx, cx, dx );

	
	//estamos testando essa para deletar a fun��o my_buffer_put_pixel
	backbuffer_putpixel ( ax, bx, cx, dx );
};
*/


//
// ## putpixel: backbuffer e lfb ##
//
// IN: cor, x, y, 0

void 
backbuffer_putpixel( unsigned long ax, 
                     unsigned long bx, 
				     unsigned long cx, 
				     unsigned long dx )
{
    //#importante
	//Esse � o origina. Isso funciona.
	//hal_backbuffer_putpixel ( ax, bx, cx, dx );
	
	//#test
	//tentando um novo m�todo.
	
	unsigned char *where = (unsigned char *) 0xC0800000;
	
	
	unsigned long color = (unsigned long) ax;
	
	char b, g, r, a;
	
	b = (color & 0xFF);	
	g = (color & 0xFF00) >> 8;
	r = (color & 0xFF0000) >> 16;
	a = (color >> 24) + 1;
	
	int x = (int) bx;
	int y = (int) cx;
	
	
	// = 3; 
	//24bpp
	int bytes_count;
	
	switch (SavedBPP)
	{
		case 32:
		    bytes_count = 4;
		    break;
		
		case 24:
		    bytes_count = 3;
			break;
	}
	
	// #importante
	// Pegamos a largura do dispositivo.
	
	int width = (int) SavedX; 
	
	int offset = (int) ( (bytes_count*width*y) + (bytes_count*x) );
	
	where[offset] = b;
	where[offset +1] = g;
	where[offset +2] = r;
	
	//teste
	if ( SavedBPP == 32 )
	{
	    where[offset +3] = a;	
	}
}


// IN: cor, x, y, 0
void 
lfb_putpixel( unsigned long ax, 
              unsigned long bx, 
		      unsigned long cx, 
		      unsigned long dx )
{
    
	//hal_lfb_putpixel ( ax, bx, cx, dx );
	
	//#test
	//tentando um novo m�todo.
	
	unsigned char *where = (unsigned char *) 0xC0400000;
	
	
	unsigned long color = (unsigned long) ax;
	
	char b, g, r, a;
	
	b = (color & 0xFF);	
	g = (color & 0xFF00) >> 8;
	r = (color & 0xFF0000) >> 16;
	a = (color >> 24) + 1;
	
	int x = (int) bx;
	int y = (int) cx;

	// = 3; 
	//24bpp
	int bytes_count;
	
	switch (SavedBPP)
	{
		case 32:
		    bytes_count = 4;
		    break;
		
		case 24:
		    bytes_count = 3;
			break;
	}	
	
	//#importante
	//Pegamos a largura salva.
	int width = (int) SavedX; 
	
	int offset = (int) ( (bytes_count*width*y) + (bytes_count*x) );
	
	where[offset] = b;
	where[offset +1] = g;
	where[offset +2] = r;

	//teste
	if ( SavedBPP == 32 )
	{
	    where[offset +3] = a;	
	}
}


//#importante:
//pega um pixel no BACKBUFFER
//tem que usar vari�vel pra bytes per pixel e screen width. 
//A ideia � poder pegar os p�xel de um ret�ngulo e salv� los
//para depois devolver ao backbuffer.

unsigned long get_pixel ( unsigned long x,  unsigned long y ){
	
	//SALVA A COR
	unsigned long COLOR;
	
	unsigned char *rgba = (unsigned char *) &COLOR;
	
	unsigned char *backbuffer = (unsigned char *) BACKBUFFER_ADDRESS;

	// = 3; 
	//24bpp
	int bytes_count;

	switch (SavedBPP)
	{
		case 32:
		    bytes_count = 4;
		    break;
		
		case 24:
		    bytes_count = 3;
			break;
	}	

	// #importante
	// Usando largura salva.
	
	int width = (int) SavedX; 
		
	unsigned long pos = (unsigned long) (y* bytes_count * width)+(x * bytes_count);	
	
	COLOR = *( unsigned long * ) &backbuffer[pos];
	
    return (unsigned long) COLOR;	
}


//copia um pixel do backbuffer para o frontbuffer
//tem que usar vari�vel pra bytes per pixel e screen width.
//#todo: TESTAR ESSA FUN��O

void refresh_pixel ( unsigned long x,  unsigned long y ){
	
	//SALVA A COR
	unsigned long COLOR;
	
	unsigned char *rgba = (unsigned char *) &COLOR;
	
    unsigned char *frontbuffer = (unsigned char *) FRONTBUFFER_ADDRESS;	
	
	
	// = 3; 
	//24bpp
	int bytes_count;
	
	switch (SavedBPP)
	{
		case 32:
		    bytes_count = 4;
		    break;
		
		case 24:
		    bytes_count = 3;
			break;
	}	

	//#importante
	//Usando largura salva.
	
	int width = (int) SavedX; 	

	unsigned long pos = (unsigned long) (y* bytes_count * width)+(x * bytes_count);	
	
	//pego o pixel no backbuffer
	COLOR = get_pixel ( x, y );
	
	*( unsigned long * ) &frontbuffer[pos] = COLOR;
}


//
// End.
//

