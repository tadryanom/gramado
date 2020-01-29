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


#ifndef _STDLIB_H
#define _STDLIB_H 1


#include <stddef.h>



#ifdef	_BSD_SIZE_T_
typedef	_BSD_SIZE_T_	size_t;
#undef	_BSD_SIZE_T_
#endif

#if defined(_BSD_WCHAR_T_) && !defined(__cplusplus)
typedef	_BSD_WCHAR_T_	wchar_t;
#undef	_BSD_WCHAR_T_
#endif

typedef struct {
	int quot;		/* quotient */
	int rem;		/* remainder */
} div_t;

typedef struct {
	long quot;		/* quotient */
	long rem;		/* remainder */
} ldiv_t;


#if !defined(_ANSI_SOURCE) && \
    (defined(_ISOC99_SOURCE) || (__STDC_VERSION__ - 0) >= 199901L || \
     (__cplusplus - 0) >= 201103L || defined(_NETBSD_SOURCE))
typedef struct {
	/* LONGLONG */
	long long int quot;	/* quotient */
	/* LONGLONG */
	long long int rem;	/* remainder */
} lldiv_t;
#endif

#if defined(_NETBSD_SOURCE)
typedef struct {
	quad_t quot;		/* quotient */
	quad_t rem;		    /* remainder */
} qdiv_t;
#endif


#define	EXIT_FAILURE	1
#define	EXIT_SUCCESS	0


//bsd-like
//#define	RAND_MAX	0x7fffffff

#define RAND_MAX  32767 


//bsd stuff
//extern size_t __mb_cur_max;
//#define	MB_CUR_MAX	__mb_cur_max


void *stdlib_system_call ( unsigned long ax, 
                           unsigned long bx, 
                           unsigned long cx, 
                           unsigned long dx );

void stdlib_die (char *str);

int atoi (const char *str);	


void itoa (int n, char s[]);
		   

char *getenv (const char *name);


/*
 * malloc:
 *    Allocates the requested memory and returns a pointer to it. */

void *malloc (size_t size);

void *calloc (size_t count, size_t size);

void *zmalloc ( size_t size);

void *realloc ( void *start, size_t newsize );


/*
 * xmalloc:
 *     exit se malloc n�o der certo. */

void *xmalloc ( int size);



/*
 * free:
 *     Libera a mem�ria alocada por malloc. */

void free (void *ptr);

//Returns a pseudo-random number in the range of 0 to RAND_MAX.
int rand (void);

//Alimenta a fun��o rand.
void srand (unsigned int seed);


//@todo: talvez essa fun��o esteja declara erradamente em systemcall.
//Obs: Essa rotina existe na API e funciona. Se ela faz parte da lib C
// ent�o ela deve sair de l� vir pra c�.
int system (const char *command);





//=================================


long
strtol(const char *nptr, char **endptr, int base);
	
unsigned long
strtoul( const char *nptr, char **endptr, int base);

double strtod (const char *str, char **endptr);
float strtof(const char *str, char **endptr);
long double strtold(const char *str, char **endptr);
double atof(const char *str);


//=================================


int abs( int j);


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


#endif  /*stdlib.h*/

//
// End.
//
