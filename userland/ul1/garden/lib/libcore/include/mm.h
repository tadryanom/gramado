/*
 * File mm.h 
 *
 * Descri��o:
 *     Suporte para libc99 em user mode.
 */
 
//
// MM BLOCK.
// 

#define MMBLOCK_HEADER_SIZE   64    //Tamanho do header do bloco.
#define MMBLOCK_COUNT_MAX    256    //Contagem de mmblock. 


//
// mm block support.
//

unsigned long mmblockCount;         //Conta os blocos de mem�ria dentro de um heap. 
 

/*
 * mmblock_d:
 *     Estrutura para memory blocks.
 *     Temos v�rios blocos de mem�ria espalhados em lugares diferentes 
 *     dentro de um heap.
 * 
 *     @todo: 
 *         Os blocos precisam de alguma organiza��o.
 *         Por enquanto, o total � 256 heaps de tamanhos diferentes.
 *
 *         Os blocos formar�o uma lista encadeada que ser� percorrida 
 *         para se encontrar um bloco vazio ... walk ...
 *
 *         Um heap de processo tem v�rios blocos dentro.
 */ 
typedef struct mmblock_d mmblock_descriptor_t;
struct mmblock_d 
{
    // Identificadores.	
	unsigned long Header;      // Endere�o onde come�a o header do heap.
	unsigned long headerSize;  // Tamanho do header em bytes.
	unsigned long Id;          // Id do header.
	unsigned long Used;        // Flag 'usado' ou 'livre'.
	unsigned long Magic;       // Magic number. Ver se n�o est� corrompido.
	
	unsigned long Free;       //se o bloco esta livre ou n�o
	
	
	// Mensuradores. (sizes) (tamanhos)	
	unsigned long requestSize;  // Tamanho, em bytes, da �rea solicitada.
	unsigned long unusedBytes;  // Quantidade de bytes n�o usados na �rea do cliente.	
	unsigned long userareaSize; // Tamanho da �rea reservada para o cliente. 
	                            // (request size + unused bytes). 
	
	//
    // User area. (Onde come�a a �rea solicitada).
	//
	
	unsigned long userArea;  // In�cio da �rea alocada.
	
	
	//
	// Footer.
	//
	
	unsigned long Footer;  // Endere�o do in�cio do footer.
	
	
	//
	// Process. (Pertence � qual processo??)
	//
	
	int processId;
	
	struct process_d *process;
	
	struct mmblock_d *Next;
	struct mmblock_d *Prev;
	
	//Continua...
	
};
mmblock_descriptor_t *current_mmblock;
//mmblock_descriptor_t *SystemCache_mmblock;
//mmblock_descriptor_t *ProcessCache_mmblock;
//...

 
//Lista de blocos. 
//lista de blocos de mem�ria dentro de um heap.
//@todo: na verdade temos que usar lista encadeada. 
unsigned long mmblockList[MMBLOCK_COUNT_MAX]; 

//
// End.
//

