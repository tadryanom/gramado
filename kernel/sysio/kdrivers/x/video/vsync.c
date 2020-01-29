/*
 * File: vsync.c
 *
 * Descri��o:
 *     Sincronisa o retra�o vertical.
 *
 *     Na hora de enviar os dados para o LFB �
 * necess�rio sincronizar com o retra�o vertical do monitor.
 *
 *     Pertence ao hal.
 *     N�o sei se pertence a x86. ???!!!
 *
 * History:
 *     2015 - Created by Fred Nora.
 */

 
#include <kernel.h>


// Defini��es.
#define VSYNC_INPORT 0x3DA



// Fun��es internas.
char vsync_inb (int port);

 
void vsync (void)
{	
	// M�todo 1
	//================================
	
	// while ( ( vsync_inb (0x3DA) & 8 ) != 8 );
	// while ( ( vsync_inb (0x3DA) & 8 ) == 8 );
	// return;
	
	// M�todo 2
	//================================
	
	// Checar se vsync foi gerado.
    
	unsigned long MaxDelay;
	
	outportb ( 0x3c4, 0 );

    if (  vsync_inb( 0x3c5 ) & 0x2 ) 
	{
        MaxDelay = 100000;
        while ( ( ( vsync_inb (0x3DA) & 8 ) == 8 ) && MaxDelay-- );
		
		MaxDelay = 100000;
		while ( ( ( vsync_inb (0x3DA) & 8 ) == 0  ) && MaxDelay-- );
    }	
}


/*
 * vsync_inb: 
 *     Pega um byte na porta. 
 */

char vsync_inb (int port){
	
    char Value = 0;
	
    Value = (char) inportb (port); 
	
	asm (" nop \n");
	asm (" nop \n");
	asm (" nop \n");
	asm (" nop \n");
	
	return (char) Value;    
}


//
// End.
//

