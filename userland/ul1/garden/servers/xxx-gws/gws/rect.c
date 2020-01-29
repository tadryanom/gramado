/*
 * File: rect.c
 *
 * Descri��o:
 *     Rotina de pintura de retangulos.
 *     Faz parte do m�dulo Window Manager do tipo MB.
 *
 * History:
 *    2015 - Created by Fred Nora.
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



void *rectStrCopyMemory32 ( unsigned long *dest, unsigned long *src, int count ) 
{
	// Destination is an Null pointer? Source is an Null pointer? Zero-sized copy? Destination is Source?
	
	if ( (dest == NULL) || 
	     (src == NULL)  || 
		 (count == 0) || 
		 (src == dest) ) 
	{
        // Yes		
		return dest;																
	}
	
	// GCC should optimize this for us :)
	
	int i;
	for ( i=0; i < count; i++ ) 
	{											
		dest[i] = src[i];
	}
	
	return dest;
};




//
//===============================================================
// refresh rect - Fred. P.
//
// @todo:
// Nessa macro podemos usar vari�veis globais e inicializar
// essas vari�vel. E considerarmos valores como g_bpp, g_screen_width. 
//


//#define RGB_PIXEL_OFFSET(x,y) \
//( (3*800*(y)) + (3*(x)) )

//
// #define RGB_PIXEL_OFFSET(x,y) \
// ( (3*screenGetWidth()*(y)) + (3*(x)) )
//
// #define RGB_PIXEL_OFFSET(x,y) \
// ( ( screenGetBPP() * screenGetWidth()*(y)) + ( screenGetBPP() *(x)) )

// #define RGB_PIXEL_OFFSET(x,y) \
// ( ( g_bpp * g_screen_width *(y)) + ( g_bpp *(x)) )

//================================================


//Usada no refresh rectangle.
//#todo: precisamos de uma vari�vel para a lergura 
//da tela e para bytes per pixel.
//#todo: isso deve virar uma fun��o.
//#define BUFFER_PIXEL_OFFSET(x,y) \
//( (3*800*(y)) + (3*(x)) )

/*
 
 ### Usar isso ###
 
unsigned long function_BUFFER_PIXEL_OFFSET ( unsigned long x, unsigned long y );
unsigned long function_BUFFER_PIXEL_OFFSET ( unsigned long x, unsigned long y )
{
    //( (3*800*(y)) + (3*(x)) )
	
    return (unsigned long) ( ( bytes_per_pixel * pixels_per_line *(y)) + ( bytes_per_pixel *(x)) );    	
};
*/

//
// @todo: Criar uma estrutura para o elemento gr�fico. entre os elementos da estrutura
// pode ter os buffer para o char. backbuffer, frontbuffer, dedicatedbuffer.
//
//


/*
 * rectDrawRectangleWindowBuffer:
 *     Pinta um ret�ngulo no buffer da janela.
 *     Serve para pintar janelas que ir�o direto do seu buffer para o LFB da 
 * mem�ria de v�deo, sem passar pelo back buffer. (OVERLAPPED)
 *
 */
/*
void rectDrawRectangleWindowBuffer(struct window_d *window, 
                                   unsigned long x, 
                                   unsigned long y, 
						           unsigned long width, 
						           unsigned long height, 
						           unsigned long color ); 
void rectDrawRectangleWindowBuffer(struct window_d *window, 
                                   unsigned long x, 
                                   unsigned long y, 
						           unsigned long width, 
						           unsigned long height, 
						           unsigned long color )
{
    return;	
}
*/


/* 
void rectDrawRectangle( struct window_d *window, struct rect_d *rect);
void rectDrawRectangle( struct window_d *window, struct rect_d *rect)
{
	struct window_d *hwndDesktop;
	
    hwndDesktop = guiGetMainWindow();
	
	// Criaremos o ret�ngulo na janela principal caso o ponteiro 
	//passado seja inv�lido.
	
    if( (void*) window == NULL ){
        window = hwndDesktop;
    };

    unsigned long x; 
    unsigned long y; 
	unsigned long width; 
	unsigned long height; 
	unsigned long color;

    x      = window->left + rect->x;	
	y      = window->top + rect->y;
	width  = rect->width;
	height = rect->height;
	color = rect->bg_color;
	
    drawDataRectangle( x, y, width, height, color );
	
}						
*/

 
/* drawDataRectangle:
 *     Draw a rectangle on backbuffer. */

void 
drawDataRectangle ( unsigned long x, 
                    unsigned long y, 
				    unsigned long width, 
				    unsigned long height, 
				    unsigned long color )
{
	
	//#BUGBUG
	//TEMOS UM PROBLEM�O AQUI.
	//Estamos usando um ponteiro sem ao menos
	//termos alocado mem�ria para sua estrutura.
	//>> para contornar, vamos tentar usar sem
	//ponteiro.
	
	//struct rect_d *rect;
	struct rect_d rect;
	
    rect.bg_color = color;
   
    //Dimensions.
	rect.x = 0;        
    rect.y = 0;         
    rect.width = width;   
    rect.height = height;    
   
    //Margins.
    rect.left = x;    
    rect.top = y;
    rect.right = rect.left + rect.width;
    rect.bottom = rect.top + rect.height; 

    //Limits.
	//@todo: Repensar os limites para uma janela.
	// Uma janela poder� ser maior que as dimens�es de um dispositivo.
	// mas n�o poder� ser maior que as dimens�es do backbuffer.
	// Ou seja: O dedicated buffer de uma janela deve ser menor que
	// o backbuffer.
	
	//#todo: Usar vari�vel para largura.
	
    if ( rect.right > SavedX )
	{
        rect.right = SavedX;
	}	

	/* @todo:
    if( rect->bottom > SavedY ){
        rect->bottom = SavedY;
	};
    */	
  	
    // Draw lines on backbuffer.
	
	while (height--){
		
	    my_buffer_horizontal_line ( rect.left, y, rect.right, rect.bg_color );
		y++;
    };    
};
  
  
  
/*
 * getClientAreaRect:
 *     Obt�m o ponteiro da estrutura do ret�ngulo  
 *     da �rea de cliente da janela ativa.
 *     @todo: oferecer esse servi�o para a api.
 */  
void *getClientAreaRect (){
	
    return (void *) rectClientArea;	
};


/*
 * setClientAreaRect:
 *     Inicializa a estrutura do ret�ngulo da �rea de cliente 
 * da janela ativa.
 */
void setClientAreaRect( unsigned long x, 
                        unsigned long y, 
						unsigned long cx, 
						unsigned long cy )
{
    struct rect_d *r;
	
    if ( (void *) rectClientArea == NULL )
	{
	    return;
		
	}else{

        r->x = x;
        r->y = y;
        r->cx = cx;
        r->cy = cy;
		
        rectClientArea = (void *) r;	
	};
};


/*
 ***********************************************************
 * refresh_rectangle:
 *     Copiar um ret�ngulo do backbuffer para o frontbuffer. 
 * 
 *     @todo: Rotina parecida com essa pode ser criada e usada para manipular 
 * regi�es da tela, como �rea de cliente efetuar scroll de buffer em p�ginas 
 * de navegador ou menus .. mas para isso, a c�pia seria dentro do pr�prio 
 * backbuffer ou de um terceiro buffer para o backbuffer. 
 *
 * Hist�rico:
 *     2017 - Criado por Frederico Lamberti Pissarra.
 *     2018 - Fred Nora.
 */	


//#importante
//� bem mais r�pido com m�ltiplos de 4.
 
void 
refresh_rectangle ( unsigned long x, 
                    unsigned long y, 
				    unsigned long width, 
				    unsigned long height )
{    
	void *p = (void *) FRONTBUFFER_ADDRESS;		
	const void *q = (const void*) BACKBUFFER_ADDRESS;

	//#TEST
	register unsigned int i;
	//unsigned int i;
	
	unsigned int line_size, lines;
	unsigned int offset;
	unsigned long Width = (unsigned long) screenGetWidth();
	unsigned long Height = (unsigned long) screenGetHeight();	

	line_size = (unsigned int) width; 
	lines = (unsigned int) height;
	
	
	
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
			
		//...
	}
	
	
	//offset = (unsigned int) BUFFER_PIXEL_OFFSET( x, y );
	offset = (unsigned int) ( (bytes_count*SavedX*(y)) + (bytes_count*(x)) );
	
	p = (void *) (p + offset);    
	q = (const void *) (q + offset);    
	 
    vsync ();	
	
		
	//(line_size * bytes_count) � o n�mero de bytes por linha. 
	
	int count; 
	
	
    //#importante
    //� bem mais r�pido com m�ltiplos de 4.	
	
	//se for divis�vel por 4.
	if ( ((line_size * bytes_count) % 4) == 0 )
	{
        count = ((line_size * bytes_count) / 4);  	

	    for ( i=0; i < lines; i++ )
	    {
		    //copia uma linha ou um pouco mais caso n�o seja divis�vel por 
		    memcpy32 ( p, q, count );
		    
			q += (Width * bytes_count);
	 	    p += (Width * bytes_count);
	    };
	}

	//se n�o for divis�vel por 4.
	if ( ((line_size * bytes_count) % 4) != 0 )
	{
	    for ( i=0; i < lines; i++ )
	    {
		    memcpy ( (void *) p, (const void *) q, (line_size * bytes_count) );
		    q += (Width * bytes_count);
		    p += (Width * bytes_count);
	    };	
	}    
}


// A ideia aqui � efetuar o refresh de um ret�ngulo que esteja em um dado buffer.
void 
refresh_rectangle2 ( unsigned long x, 
                     unsigned long y, 
				     unsigned long width, 
				     unsigned long height,
                     unsigned long buffer1,
                     unsigned long buffer2 )
{    
	void *p = (void *) buffer1;		
	const void *q = (const void *) buffer2;

	//register unsigned int i;
	unsigned int i;
	
	unsigned int line_size, lines;
	unsigned int offset;
	unsigned long Width = (unsigned long) screenGetWidth();
	unsigned long Height = (unsigned long) screenGetHeight();	

	line_size = (unsigned int) width; 
	lines = (unsigned int) height;
	
	
	int bytes_count;// = 3; //24bpp
	
	switch (SavedBPP)
	{
		case 32:
		    bytes_count = 4;
		    break;
		
		case 24:
		    bytes_count = 3;
			break;
	}
	
	
	//offset = (unsigned int) BUFFER_PIXEL_OFFSET( x, y );
	offset = (unsigned int) ( (bytes_count*SavedX*(y)) + (bytes_count*(x)) );	

	
	p = (void *) (p + offset);    
	q = (const void *) (q + offset);    
	 
    vsync ();	
	
	
	//(line_size * 3) � o n�mero de bytes por linha. 
	
	int count; 
	
	//se for divis�vel por 4.
	if( ((line_size * 3) % 4) == 0 )
	{
        count = ((line_size * 3) / 4);  	

	    for ( i=0; i < lines; i++ )
	    {
		    //copia uma linha ou um pouco mais caso n�o seja divis�vel por 
		    memcpy32 ( p, q, count );
		    
			q += (Width * 3);
	 	    p += (Width * 3);
	    };
	}

	//se n�o for divis�vel por 4.
	if( ((line_size * 3) % 4) != 0 )
	{

        //count = (line_size * 3);  		
	
	    for ( i=0; i < lines; i++ )
	    {
		    memcpy ( (void *) p, (const void *) q, (line_size * 3) );
		    q += (Width * 3);
		    p += (Width * 3);
	    };	
	}  		
	
	/*
	
	for ( i=0; i < lines; i++ )
	{
		memcpy( p, q, (line_size * 3) );
		q += (Width * 3);
		p += (Width * 3);
	};	
	*/
};




//inicializando a estrutura de gerenciamento de ret�ngulo salvo.
int initialize_saved_rect (){
	
	//alocando mem�ria para a estrutura de gerenciamento do ret�ngulo.
    SavedRect = (void *) malloc ( sizeof(struct saved_rect_d) );   

    if ( (void *) SavedRect ==  NULL )
    {
	    return (int) 1;	
		
	} else {
     
        // 800x600x3 (resolu��o m�xima) 351+ p�ginas.
		//com isso poderemos salvar uma tela nessa resolu��o.
		SavedRect->buffer_address = (void *) allocPages (360);
	
	    if ( (void *) SavedRect->buffer_address == NULL )
	    {
		    printf ("initialize_saved_rect: buffer fail");
	        die ();	
	    }

        SavedRect->x = 0; 		
	    SavedRect->y = 0;
		SavedRect->width = 0;
		SavedRect->height = 0;
		
		SavedRect->pixels = 0;
		SavedRect->bytes = 0;
		SavedRect->bpp = 0;
		
		SavedRect->full = 0;   //empty
		
		
		//#todo: limpar o buffer ???
		
	    //...
	};		

	//#debug
	printf("gws-initialize_saved_rect: **** done");
	//refresh_screen();
	
	//while (1){
	//	asm("hlt");
	//}
	
    return (int) 0;	
};



//#testando ...
//salvar um ret�ngulo no buffer ser� semelhante ao m�todo de 
//salvar um bmp em um arquivo.
int save_rect ( unsigned long x, 
                unsigned long y, 
				unsigned long width, 
				unsigned long height )
{		
    if ( (void *) SavedRect ==  NULL )
    {
	    return (int) 1;
		
	} else {

	    if ( (void *) SavedRect->buffer_address == NULL )
	    {
		    printf ("save_rect: buffer fail");
	        die ();	
	    }
    };

   // ## transferindo ... ##
   //======================

    //do backbuffer para o buffer de salvamento.
	
	void *p = (void *) SavedRect->buffer_address; //(buffer para salvar)		
	const void *q = (const void *) BACKBUFFER_ADDRESS;

	//register unsigned int i;
	unsigned int i;
	
	unsigned int line_size, lines;
	
	unsigned int offset1;  
	unsigned int offset2;  
	
	unsigned long Width = (unsigned long) screenGetWidth();
	unsigned long Height = (unsigned long) screenGetHeight();	

	line_size = (unsigned int) width; //passado por argumento
	lines = (unsigned int) height;    //passado por argumento
	
	
	
	int bytes_count;// = 3; //24bpp
	
	switch (SavedBPP)
	{
		case 32:
		    bytes_count = 4;
		    break;
		
		case 24:
		    bytes_count = 3;
			break;
	}
	

	offset1 = (unsigned int) ( (bytes_count*SavedX*(y)) + (bytes_count*(x)) );	
	
	//atualizando o offset do backbuffer
	//offset1 = (unsigned int) BUFFER_PIXEL_OFFSET( x, y );
	
	//configurando o offset do buffer de salvamento.
	offset2 = 0;
	
	p = (void *) (p + offset2);    
	q = (const void *) (q + offset1);    
	 
    
	//n�o precisa de sincroniza��o pois n�o estamos enviando para o LFB.
	//vsync ();	
	
	
	//(line_size * 3) � o n�mero de bytes por linha. 
	
	int count; 
	
	//se for divis�vel por 4.
	if( ((line_size * 3) % 4) == 0 )
	{
        count = ((line_size * 3) / 4);  	

	    for ( i=0; i < lines; i++ )
	    {
		    //copia uma linha ou um pouco mais caso n�o seja divis�vel por 
		    memcpy32 ( p, q, count );
		    
			q += (Width * 3);
	 	    p += (Width * 3);
	    };
	}

	//se n�o for divis�vel por 4.
	if( ((line_size * 3) % 4) != 0 )
	{

        //count = (line_size * 3);  		
	
	    for ( i=0; i < lines; i++ )
	    {
		    memcpy ( (void *) p, (const void *) q, (line_size * 3) );
		    q += (Width * 3);
		    p += (Width * 3);
	    };	
	}  			
	
	/*
	for ( i=0; i < lines; i++ )
	{
		memcpy( p, q, (line_size * 3) );
		q += (Width * 3);
		p += (Width * 3);
	};	 
    */


    return (int) 0;		
};


/*
 * show_saved_rect:
 *      Pintar no backbuffer o ret�ngulo salvo.
 *      Semelhante ao processo de decodificar um bmp, copiando 
 * do arquivo para o backbuffer.
 * Esses argumentos representam o posicionamento desejado do 
 * ret�ngulo no backbuffer. 
 */

int 
show_saved_rect ( unsigned long x, 
                  unsigned long y, 
				  unsigned long width, 
				  unsigned long height )
{		
    
	
	
	
	// Checando a estrutura que tem informa��es 
	// sobre o ret�ngulo salvo.
	
	if ( (void *) SavedRect ==  NULL )
    {
	    return (int) 1;
		
	} else {

	    if ( (void *) SavedRect->buffer_address == NULL )
	    {
		    printf ("save_rect: buffer fail");
	        die ();	
	    }
    };

	
    //
    // ## Transferindo ... ##
    //
	
	void *p = (void *) BACKBUFFER_ADDRESS;   	
	const void *q = (const void *) SavedRect->buffer_address;         

	//register unsigned int i;
	unsigned int i;
	
	int count; 
	
	int bytes_count;  // = 3; //24bpp	
	
	
	unsigned int offset1;  //offset dentro do buffer de salvamento.
	unsigned int offset2;  //offset dentro do backbuffer
	
	unsigned long Width = (unsigned long) screenGetWidth();
	unsigned long Height = (unsigned long) screenGetHeight();	

	unsigned int line_size, lines;
	
	line_size = (unsigned int) width; //passado por argumento
	lines = (unsigned int) height;    //passado por argumento
	
	
	switch (SavedBPP)
	{
		case 32:
		    bytes_count = 4;
		    break;
		
		case 24:
		    bytes_count = 3;
			break;
	};
	

	offset1 = (unsigned int) ( ( bytes_count * SavedX * (y) ) + ( bytes_count * (x) ) );		
	
	offset2 = 0;
	
	
	p = (void *) (p + offset1);    
	
	q = (const void *) (q + offset2);    
	 
    // #importante:
	// N�o precisa de sincroniza��o pois n�o estamos enviando para o LFB.
	// vsync ();		
	
	//(line_size * 3) � o n�mero de bytes por linha. 
	
	//se for divis�vel por 4.
	if ( ((line_size * 3) % 4) == 0 )
	{
        count = ((line_size * 3) / 4);  	

	    for ( i=0; i < lines; i++ )
	    {
		    //copia uma linha ou um pouco mais caso n�o seja divis�vel por 
		    memcpy32 ( p, q, count );
		    
			q += (Width * 3);
	 	    p += (Width * 3);
	    };
	}

	//se n�o for divis�vel por 4.
	if ( ((line_size * 3) % 4) != 0 )
	{

        //count = (line_size * 3);  		
	
	    for ( i=0; i < lines; i++ )
	    {
		    memcpy ( (void *) p, (const void *) q, (line_size * 3) );
		    q += (Width * 3);
		    p += (Width * 3);
	    };	
	}  			
	
    return (int) 0;	
};


//
// End.
//
