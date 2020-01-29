/*
 * File: stdlib.h 
 *
 * Descri��o:
 *     Parte da libC C99 para usermode 32bit.
 * Vers�o: 1.0, 2016 - Created.
 */

 
/*
   O que deve conter:
   
    atof string para float
    atoi	string para integer
    atol	string para long integer
    strtod	string para double
    strtol	string para long int
    strtoul	string para unsigned long int

	Gera��o de seq��ncia pseudo-aleat�ria
    rand	gera um n�mero inteiro pseudo aleat�rio
    srand	seleciona a semente do gerador pseudo aleat�rio
    
	Aloca��o e libera��o de mem�ria
    malloc
    calloc
    realloc	aloca mem�ria do "heap"
    free	libera mem�ria de volta para o "heap"

	Controle de processos
    abort	for�a o t�rmino da execu��o
    atexit	registra uma fun��o "callback" para sa�da do programa
    exit	termina a execu��o do programa
    getenv	obt�m uma vari�vel de ambiente
    system	executa um comando externo

	Ordenamento e procura
    bsearch	procura bin�ria em "array"
    qsort	ordena "array" segundo algoritmo Quick Sort

	Matem�tica
    abs
    labs	valor absoluto
    div
    ldiv	divis�o inteira
   
*/   
  
//A rotina terminou normalmente.                                                                                 
#define	EXIT_SUCCESS  0    

//A rotina terminou de maneira anormal.                                                                       
#define	EXIT_FAILURE  1

#define RAND_MAX  32767 

void *stdlib_system_call( unsigned long ax, 
                   unsigned long bx, 
				   unsigned long cx, 
				   unsigned long dx );

void exit(int status);

/*
 * malloc:
 *    Allocates the requested memory and returns a pointer to it.
 */
void *malloc(size_t size);

//void *calloc(size_t nitems, size_t size);  //@todo
//void *realloc(void *ptr, size_t size);  //@todo

/*
 * free:
 *     Libera a mem�ria alocada por malloc.
 */
void free(void *ptr);
//void free2(void *ptr);

//Returns a pseudo-random number in the range of 0 to RAND_MAX.
int rand(void);

//Alimenta a fun��o rand.
//void srand(unsigned int seed);


//@todo: talvez essa fun��o esteja declara erradamente em systemcall.
//Obs: Essa rotina existe na API e funciona. Se ela faz parte da lib C
// ent�o ela deve sair de l� vir pra c�.
int system(const char *command);



//rt support
//pegando informa��es sobre o heap usado pela biblioteca C99 em user mode.
unsigned long rtGetHeapStart();
unsigned long rtGetHeapEnd();
unsigned long rtGetHeapPointer();
unsigned long rtGetAvailableHeap();
//...


/*
 * libcInitRT:
 *     Inicializa o gerenciamento em user mode de mem�ria virtual
 * para a biblioteca libC99.
 * Obs: *IMPORTANTE: Essa rotina deve ser chamada entes que a biblioteca C 
 * seja usada.
 * Obs: Pode haver uma chamada � ela em crt0.s por exemplo.
 */
int libcInitRT();
//
// End.
//
