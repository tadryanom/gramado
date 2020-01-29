/*
 * File: grid.h
 *
 * Descri��o:
 *    header no grid do kernel base.
 *
 * Vers�o 1.0, 2015.
 */

 
#define GRID_HORIZONTAL 1000
#define GRID_VERTICAL 2000

#define GRID_COUNT_MAX 128

typedef struct grid_d grid_t;
struct grid_d
{
    object_type_t objectType;
	object_class_t objectClass;
	
	int used;
	int magic;
	
	struct window_d *window;         //janela.
	struct button_d *b;              //lista encadeada de bot�es.
    int count;                       //Quantidade de bot�es na lista.
	//

    struct grid_d *next; 	
};
grid_t *GRID;
//...



// 
// Armazena ponteiros de estrutura.
//

//@todo: Esses grids devem ser m�nimos em kernel mode.
unsigned long screenGrid[GRID_COUNT_MAX];   



void *CreateGrid();
int 
InitializeGrid( struct window_d *window, 
                struct grid_d *g, 
                int n, 
				int view );
				
int grid( struct window_d *window, 
          int n, 
		  int view );


//
//fim.
//
