// Suporte a top bar.



#include "wintest.h"


/*
 * topbarInitializeTopBar:
 *
 */
 
// #obs:
// Ainda refletindo de esse apps deve ou não criar uma topbar.
// A Topbar pode ficar no window manager.
 
void topbarInitializeTopBar (){
	
	int i;
	
	unsigned long OldX, OldY;

	//salva cursor antigo.
	OldX = apiGetCursorX();
    OldY = apiGetCursorY();
	
	//posiciona.#teste
	apiSetCursor( 0, 0 );
	
	for( i=0; i < ((800/8)-1); i++)
	{
		printf(" ");
	};
	
	apiSetCursor( 8, 0 );	
	printf("|TEDITOR");

	apiSetCursor( 44, 0 );	
	printf("|Application for testing system resources");
	
    apiSetCursor (OldX,OldY);
}

