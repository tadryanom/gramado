/*
 * File: heap.h
 *
 * Descri��o:
 *     O prop�sito desse header � dar suporte ao gerenciamento de
 * do heap que ser� usado para a aloca��o de mem�ria para um programa
 * em user mode. 
 *     Um suporte oferecido pela biblioteca libC99.
 *     Obs: O kernel poder� usar tanto o heap do processo quanto o heap 
 * do desktop ao qual o processo pertence.
 * 
 * Hist�rico:
 *     Vers�o 1.0, 2016 - Esse arquivo foi criado por Fred Nora.
 */
 
 
//Contagem de heap.
#define HEAP_COUNT_MAX  256 

unsigned long HEAP_START; 
unsigned long HEAP_END;
unsigned long HEAP_SIZE;
 
//@todo: padronizar 
unsigned long heapCount;            //Conta os heaps da stdlib.

unsigned long heap_start;    //Start.
unsigned long heap_end;      //End.

unsigned long g_heap_pointer;       //Pointer.
unsigned long g_available_heap;     //Available. 
 
 
// 
// Heap pointer:
//     Isso deve apontar para o heap buffer atual. Quando acabar o heap atual
// deve-se chamar o kernel para criar mais heap dentro da working set do processo. 
//
 
void *Heap;    


//
// *** HEAP ***
//

#define HEAP_BUFFER_SIZE (32*1024)  // 32KB, provis�rio.

static unsigned char HeapBuffer[HEAP_BUFFER_SIZE]; 



/*
 * heap_d:
 *     Estrutura para heap.
 *     Cada processo tem seu heap.
 *     Cada heap tem uma lista encadeada de blocos.  
 */  

//typedef struct heap_d heap_descriptor_t;
struct heap_d 
{
	int Id;
    int Used;
    int Magic;
    //int ObjectType; //tipo de objeto ao qual pertence o heap.(process, ...)	
	
	unsigned long HeapStart;             
	unsigned long HeapEnd;
	unsigned long HeapPointer;            
	unsigned long AvailableHeap; 	
	
	//struct mmblock_d *nextblock; //lista linkada de blocos. 
};
struct heap_d *libcHeap;
//heap_descriptor_t *libcHeap;
//...


//Heap list.
//obs:. heapList[0] = The Kernel Heap !!!    
unsigned long heapList[HEAP_COUNT_MAX];     


//
// Prot�tipos.
//

int heapInit();
void heapSetLibcHeap( unsigned long HeapStart, unsigned long HeapSize);


//
// Alloc and Free.
//

unsigned long heapAllocateMemory ( unsigned long size );

unsigned long FreeHeap( unsigned long size );

//
// End.
//

