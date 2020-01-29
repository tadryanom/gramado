/*
 * File: stdio.h
 *
 * Descri��o:
 *     Parte da biblioteca C para API 32bit.
 *     c99 - ISO/IEC 9899:1999
 *
 * @todo: 
 *     Usar padr�o C e colocar em outro arquivo o que n�o for daqui.
 *
 * Vers�o 1.0, 2016 - Created.
 */

 
#ifndef NULL
#define NULL ((void *)0)
#endif

 

/*  
"flags" bits definitions
*/


/* Flags for the iobuf structure  */
#define _IOREAD  1
#define _IOWRT	 2
#define _IORW	 0x0080

#define	STDIN_FILENO  0
#define	STDOUT_FILENO 1
#define	STDERR_FILENO 2


#define _IOFBF    0x0000  /* full buffered */
#define _IOLBF    0x0040  /* line buffered */
#define _IONBF    0x0004  /* not buffered */

/* Returned by various functions on end of file condition or error. */
#ifndef EOF
#define EOF (-1)
#endif


#ifndef FILENAME_MAX
#define	FILENAME_MAX	(260)
#endif

#define FOPEN_MAX	(32)
#define NUMBER_OF_FILES (32)

#define TMP_MAX	32767


#define _IOMYBUF  0x0008  /* stdio malloc()'d buffer */
#define _IOEOF    0x0010  /* EOF reached on read */
#define _IOERR    0x0020  /* I/O error from system */
#define _IOSTRG   0x0040  /* Strange or no file descriptor */

#ifdef __POSIX__
#define	_IOAPPEND 0x0200
#endif

/*
 * The buffer size as used by setbuf such that it is equivalent to
 * (void) setvbuf(fileSetBuffer, caBuffer, _IOFBF, BUFSIZ).
 */
#define BUFSIZ  1024


/* It moves file pointer position to the beginning of the file. */
#ifndef SEEK_SET
#define SEEK_SET        0       
#endif

/* It moves file pointer position to given location. */
#ifndef SEEK_CUR
#define SEEK_CUR        1       
#endif

/*  It moves file pointer position to the end of file. */
#ifndef SEEK_END
#define SEEK_END        2       
#endif


/*
 * FILE:
 *     Estrutura padr�o para arquivos.    
 *     
 */
typedef struct _iobuf FILE; 
struct _iobuf 
{
	char *_ptr;      // Current position of file pointer (absolute address).
	int   _cnt;      // number of available characters in buffer 
	char *_base;     // Pointer to the base of the file. the buffer
	int   _flag;     // Flags (see FileFlags). the state of the stream
	int   _file;     // UNIX System file descriptor
	int   _charbuf;   
	int   _bufsiz;
	char *_tmpfname;
};

FILE *stdin;
FILE *stdout;
FILE *stderr;

//listando os arquivos da biblioteca.
unsigned long Streams[NUMBER_OF_FILES];



 
//
// Posi��o virtual da mem�ria de v�deo.
//  VA=0x800000 = PA=0x000B8000.
//
#define SCREEN_START 0x800000 
#define ScreenStart SCREEN_START

#define COLUMNS 80
#define SCREEN_WIDTH COLUMNS

#define ROWS 25
#define SCREEN_HEIGHT ROWS

#define SCREEN_WIDTH COLUMNS
#define HEIGHT ROWS

//limite m�ximo. provis�rio.
#define SCREEN_MAX_HEIGHT 256  //linhas.
#define SCREEN_MAX_WIDTH  256  //colunas.

#define REVERSE_ATTRIB 0x70

#define PAD_RIGHT 1
#define PAD_ZERO 2

#define PRINT_BUF_LEN 12

#define KEY_RETURN   13    //@todo: Pertence ao teclado.



//
// Obs: O tipo da vari�vel aqui � provis�rio. (UL).
//

//cursor
unsigned long g_cursor_x;
unsigned long g_cursor_y;

//char.
unsigned long g_char_attrib;

//columns and rows
//@todo: Esse precisa ser inicializado
//Obs: Se essas vari�veis forem 0, 
// as rotinas de stdio usar�o os valores default.
//COLUMNS e ROWS.
unsigned long g_columns;
unsigned long g_rows;

int g_using_gui; //modo gr�fico?







//===========================================

//
// Prompt support.
//

// Obs: Esses buffers s�o usados pela libe como arquivo.
// #importante: 
// Qual o tamanho m�ximo que eu posso usar nesse buffer.
// Qual � o padr�o de tamanho usado para o fluxo padr�o.
//

//Buffer para o buffer usado para input de comandos.
//prompt[] 
#define PROMPT_MAX_DEFAULT 1024




// Normalmente quem cria o fluxo padr�o � a rotina 
// que cria o processo.

// Fluxo padr�o.
char prompt[PROMPT_MAX_DEFAULT];      //stdin
char prompt_out[PROMPT_MAX_DEFAULT];  //stdout 
char prompt_err[PROMPT_MAX_DEFAULT];  //stderr 
   
int prompt_pos;
int prompt_max;
int prompt_status;

//char prompt_text[] = "$> ";

//...

//===========================================
// ## Prot�tipos do padr�o C. ##
//===========================================

 
 
/*
 * File Operations
 */ 
 
FILE *fopen( const char *filename, const char *mode ); 
int fflush( FILE *stream ); 
int fclose(FILE *stream); 
//#define fileno(p)   ((p)->fd)
 
 
/*
 * Normal output.
 */ 
 
int app_print(char *message, unsigned int line, int color);
static int prints(char **out, const char *string, int width, int pad);
static int printi(char **out, int i, int b, int sg, int width, int pad, int letbase);
static int print(char **out, int *varg);
static void printchar(char **str, int c);
void outbyte(int c);
void _outbyte(int c);
 
 
/*
 * Formatted Output
 */ 
 
int fprintf(FILE *stream, const char *format, ...); 
int printf(const char *format, ...); 
int sprintf(char *out, const char *format, ...);
int printf_main(void);    //Isso � para testes.


/*
 * Normal input.
 */
 
//Usado por interpretadores de comando.
//Recebem input e colocam em prompt[] 
unsigned long input(unsigned long ch);


/*
 * Formatted Input
 */


 
/*
 * Character Input and Output Functions
 */
 
int fgetc( FILE *stream );;
#define getc fgetc
int fputc(int ch, FILE *stream);
char *gets(char *s);
int getchar(void);
int putchar(int ch);
//#define getc(p)     fgetc(p)
//#define putc(x, p)  fputc(x, p)
//#define getchar()   getc(stdin)
//#define putchar(x)  putc((x), stdout)


/*
 * Direct Input and Output Functions
 */
 
 
 
/*
 * File Positioning Functions
 */
int fseek(FILE *stream, long offset, int whence);



/*
 * Error Functions
 */
 
int feof( FILE *stream );
int ferror( FILE *stream ); 
//#define feof(p)	  (((p)->_flags & _IOEOF) != 0)
//#define ferror(p)	  (((p)->_flags & _IOERR) != 0)
//#define clearerr(p) ((p)->_flags &= ~(_IOERR|_IOEOF))
//#define feof(p)     (((p)->flag & _EOF) != 0)
//#define ferror(p)   (((p)->flag & _ERR) != 0) 
 
//
// More stuff.
//

void scroll(void);
int app_clear(int color);
int drawBar(int color);  //??



/*Inicializa��o da biblioteca*/
void stdioInitialize();

//
// End.
//
