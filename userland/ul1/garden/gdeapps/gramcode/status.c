// Suporte a status bar.


#include "gramcode.h"


/* statusInitializeStatusBar: */

void statusInitializeStatusBar (){
	
	int i;
	unsigned long OldX, OldY;

	//salva cursor antigo.
    OldX = gde_get_cursor_x ();
    OldY = gde_get_cursor_y ();


	gde_set_cursor ( 0, (600/8) -1 );
	
	for ( i=0; i < ((800/8)-1); i++ ){
		
		printf(" ");
	};
	
	
	gde_set_cursor ( 8, (600/8) -1 );	
	printf("|# Status Bar offset 8.");

	gde_set_cursor ( 44, (600/8) -1 );	
	printf("|# Status Bar  offset 44.");
	


    gde_set_cursor (OldX,OldY);
}


void update_statuts_bar ( char *string1, char *string2 ){
	
	int i;
	unsigned long OldX, OldY;

	//salva cursor antigo.
	OldX = gde_get_cursor_x ();
    OldY = gde_get_cursor_y ();
	

	gde_set_cursor ( 0, (600/8) -1 );
	
	for ( i=0; i < ((800/8)-1); i++ )
	{
		printf (" ");
	};


	gde_set_cursor( 8, (600/8) -1 );	
	printf(string1);

	gde_set_cursor( 44, (600/8) -1 );	
	printf(string2);


    gde_set_cursor (OldX,OldY);
}



