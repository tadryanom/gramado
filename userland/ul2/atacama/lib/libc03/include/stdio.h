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


#ifndef __STDIO_H__
#define __STDIO_H__

#include <sys/types.h>
#include <sys/cdefs.h>
#include <stddef.h>
#include <stdarg.h> 



// tty id desse processo.
// Usado na inicializa��o da biblioteca.
// See: stdio.c:
int __libc_tty_id;

void flush2(int count);




//
// # Modes #
//


// #test
//Os caracteres s�o colocados em stdout.
#define LIBC_NORMAL_MODE 1
//Os caracteres s�o pintados na tela.
#define LIBC_DRAW_MODE 2
// ...

//printf using draw mode.
int printf_draw ( const char *fmt, ... );

//setup libc mode
void libc_set_output_mode ( int mode );

	
	
#ifdef	_BSD_SIZE_T_
typedef	_BSD_SIZE_T_	size_t;
#undef	_BSD_SIZE_T_
#endif
#ifdef	_BSD_SSIZE_T_
typedef	_BSD_SSIZE_T_	ssize_t;
#undef	_BSD_SSIZE_T_
#endif

#if defined(_POSIX_C_SOURCE)
#ifndef __VA_LIST_DECLARED
typedef __va_list va_list;
#define __VA_LIST_DECLARED
#endif
#endif




/*bsd-like*/
#define	_IOFBF	0		// setvbuf should set fully buffered 
#define	_IOLBF	1		// setvbuf should set line buffered 
#define	_IONBF	2		// setvbuf should set unbuffered 


//
// bsd style.
//

// #importante
// Esses ser�o os valores de refer�ncia
// para todos os projetos.

#define	__SLBF	0x0001		/* line buffered */
#define	__SNBF	0x0002		/* unbuffered */
#define	__SRD	0x0004		/* OK to read */
#define	__SWR	0x0008		/* OK to write */
	/* RD and WR are never simultaneously asserted */
#define	__SRW	0x0010		/* open for reading & writing */
#define	__SEOF	0x0020		/* found EOF */
#define	__SERR	0x0040		/* found error */
#define	__SMBF	0x0080		/* _buf is from malloc */
#define	__SAPP	0x0100		/* fdopen()ed in append mode */
#define	__SSTR	0x0200		/* this is an sprintf/snprintf string */
#define	__SOPT	0x0400		/* do fseek() optimization */
#define	__SNPT	0x0800		/* do not do fseek() optimization */
#define	__SOFF	0x1000		/* set iff _offset is in fact correct */
#define	__SMOD	0x2000		/* true => fgetln modified _p text */
#define	__SALC	0x4000		/* allocate string space dynamically */





typedef char *stdio_va_list; 

 
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


/*
//bsd-like
//#todo: use this one.
#define	_IOFBF	0		// setvbuf should set fully buffered 
#define	_IOLBF	1		// setvbuf should set line buffered 
#define	_IONBF	2		// setvbuf should set unbuffered 
*/

//#define _IOFBF    0x0000  /* full buffered */
//#define _IOLBF    0x0040  /* line buffered */
//#define _IONBF    0x0004  /* not buffered */


/*
 * The buffer size as used by setbuf such that it is equivalent to
 * (void) setvbuf(fileSetBuffer, caBuffer, _IOFBF, BUFSIZ).
 */
#define BUFSIZ  1024

/* Returned by various functions on end of file condition or error. */
#ifndef EOF
#define EOF (-1)
#endif



/* System V/ANSI C; this is the wrong way to do this, do *not* use these. */
#if defined(_XOPEN_SOURCE) || defined(_NETBSD_SOURCE)
#define	P_tmpdir	"/var/tmp/"
#endif
#define	L_tmpnam	1024	/* XXX must be == PATH_MAX */


//bsd-like
/* Always ensure that this is consistent with <limits.h> */
//#ifndef TMP_MAX
//#define TMP_MAX			308915776	/* Legacy */
//#endif

#define TMP_MAX	32767


#define _IOMYBUF  0x0008  /* stdio malloc()'d buffer */
#define _IOEOF    0x0010  /* EOF reached on read */
#define _IOERR    0x0020  /* I/O error from system */
#define _IOSTRG   0x0040  /* Strange or no file descriptor */

#ifdef __POSIX__
#define	_IOAPPEND 0x0200
#endif


/* Always ensure that these are consistent with <fcntl.h> and <unistd.h>! */

/* It moves file pointer position to the beginning of the file. */
#ifndef SEEK_SET
#define	SEEK_SET	0	/* set file offset to offset */
#endif

/* It moves file pointer position to given location. */
#ifndef SEEK_CUR
#define	SEEK_CUR	1	/* set file offset to current plus offset */
#endif

/*  It moves file pointer position to the end of file. */
#ifndef SEEK_END
#define	SEEK_END	2	/* set file offset to EOF plus offset */
#endif



/*      
 * This is fairly grotesque, but pure ANSI code must not inspect the
 * innards of an fpos_t anyway.  The library internally uses off_t,
 * which we assume is exactly as big as eight chars.
 */

/*
#if (!defined(_ANSI_SOURCE) && !defined(__STRICT_ANSI__)) || defined(_LIBC)
typedef __off_t fpos_t;
#else
typedef struct __sfpos {
	__off_t _pos;
} fpos_t;
#endif
*/

// glibc-style
/* The type of the second argument to `fgetpos' and `fsetpos'.  */
// See: /sys/types.h
typedef __off_t          fpos_t;
typedef __darwin_off_t   fpos_t;
typedef __gramado_off_t  fpos_t;


// apple??
// #define	_FSTDIO			/* Define for new stdio with functions. */




// =======================================================

//
// FILE
//

// #importante
// Todo suporte � arquivos foi para libio/file.h
// Os tipos que eles precisam ainda est� aqui.

#include <libio/file.h>


// =======================================================



//#define L_ctermid 255	/* required by POSIX */
//#define L_cuserid 255	/* required by POSIX */




 //=====================================================
 
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

#define PROMPT_MAX_DEFAULT BUFSIZ //1024




// Normalmente quem cria o fluxo padr�o � a rotina 
// que cria o processo.

// Fluxo padr�o.
//O alocador pode ignorar isso e criar outros buffers.
char prompt[PROMPT_MAX_DEFAULT];      //stdin
char prompt_out[PROMPT_MAX_DEFAULT];  //stdout 
char prompt_err[PROMPT_MAX_DEFAULT];  //stderr 
   
int prompt_pos;
int prompt_max;
int prompt_status;

//char prompt_text[] = "$ ";

//...


/*
 * The __sfoo macros are here so that we can 
 * define function versions in the C library.
 */

//#todo Precisamos de __srget.
//#define	__sgetc(p) (--(p)->_r < 0 ? __srget(p) : (int)(*(p)->_p++))

//#todo Precisamos de __swbuf.
/*
#if defined(__GNUC__) && defined(__STDC__)
static __inline int __sputc(int _c, FILE *_p) {
	if (--_p->_w >= 0 || (_p->_w >= _p->_lbfsize && (char)_c != '\n'))
		return (*_p->_p++ = _c);
	else
		return (__swbuf(_c, _p));
}
#else
//This has been tuned to generate reasonable code on the vax using pcc.
#define	__sputc(c, p) \
	(--(p)->_w < 0 ? \
		(p)->_w >= (p)->_lbfsize ? \
			(*(p)->_p = (c)), *(p)->_p != '\n' ? \
				(int)*(p)->_p++ : \
				__swbuf('\n', p) : \
			__swbuf((int)(c), p) : \
		(*(p)->_p = (c), (int)*(p)->_p++))
#endif
*/




/*
#if defined(__GNUC__) && defined(__STDC__)
static __inline int 
__sputc (int _c, FILE *_p)
{
    if ( --_p->_w >= 0 || 
         ( _p->_w >= _p->_lbfsize && (char)_c != '\n' ) )
    {
		
		// Coloca no buffer, quando ainda n�o cheio.
		return (*_p->_p++ = _c);
	}else{

        // Quando o buffer estiver cheio;
        return (__swbuf(_c, _p));
    };
}
#endif
*/




/*bsd*/
#define	__sfeof(p)	(((p)->_flags & __SEOF) != 0)
#define	__sferror(p)	(((p)->_flags & __SERR) != 0)
#define	__sclearerr(p)	((void)((p)->_flags &= ~(__SERR|__SEOF)))
#define	__sfileno(p)	((p)->_file)
//define	feof(p)		__sfeof(p)
//#define	ferror(p)	__sferror(p)
//#define	clearerr(p)	__sclearerr(p)


void clearerr(FILE* stream);

//#ifndef _ANSI_SOURCE
//#define	fileno(p)	__sfileno(p)
//#endif

//#ifndef lint
//#define	getc(fp)	__sgetc(fp)
//#define putc(x, fp)	__sputc(x, fp)
//#endif /* lint */

//#define	getchar()	getc(stdin)
//#define	putchar(x)	putc(x, stdout)



/*linux klibc style.*/
//#define getc(f) fgetc(f)


/*glib style; isso pode ser usado no kernel*/
/* Optimizing.  */
/*
#ifdef	__OPTIMIZE__
#define	feof(stream)	((stream)->__eof != 0)
#define	ferror(stream)	((stream)->__error != 0)
#endif 
*/





/*glibc style*/
/* The C standard explicitly says this can
   re-evaluate its arguments, so it does.  */
//#define  __putc(c, stream)  \
//    ((stream)->_w < (stream)->_lbfsize ?     \
//    (int) (unsigned char) (  (stream)->_base[stream->_w++] = (unsigned char) (c)  ) :   \
//   __flshfp ((stream), (unsigned char) (c)))


/*glibc style*/
/* The C standard explicitly says this can be a macro,
   so we always do the optimization for it.  */
//#define	putc(c, stream)	__putc ((c), (stream))
//#define	putc(c, stream)	__serenity_putc ((c), (stream))




//===========================================
// ## Prot�tipos do padr�o C. ##
//===========================================

char *ctermid(char *s);

int fpurge(FILE *stream);

int fsetpos(FILE *stream, const fpos_t *pos);

int fgetpos(FILE *stream, fpos_t *pos);

FILE *fmemopen(void *buf, size_t size, const char *mode);

FILE *open_wmemstream(wchar_t **ptr, size_t *sizeloc);

FILE *open_memstream(char **ptr, size_t *sizeloc);

FILE *freopen(const char *pathname, const char *mode, FILE *stream);

FILE *fdopen(int fd, const char *mode);

FILE *tmpfile(void);

char *tempnam(const char *dir, const char *pfx);
char *tmpnam_r(char *s);
char *tmpnam(char *s);

int vfscanf(FILE *stream, const char *format, va_list ap);
int vsscanf(const char *str, const char *format, va_list ap);
int vscanf(const char *format, va_list ap);

int vsnprintf(char *str, size_t size, const char *format, va_list ap);


//
//  vsprintf and print
//

int Wirzenius_Torvalds_vsprintf(char *buf, const char *fmt, va_list args);
int Torvalds_printf(const char *fmt, ...);


int vdprintf(int fd, const char *format, va_list ap); 
int dprintf(int fd, const char *format, ...);

unsigned int filesize (FILE * fp);
char * fileread (FILE * fp);




// remove - remove a file or directory 
// On success, zero is returned. On error, -1 is returned, and 
// errno is set appropriately. 
// C89, C99, 4.3BSD, POSIX.1-2001. 
int remove (const char *pathname); 


int fileno ( FILE *stream );
 
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
 
int puts(const char *str); 

//#todo: testar.
//Credits: Sombra OS.
void nputs (char *cp, int len);

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







//#testes
//ok isso funcionou.
int vfprintf ( FILE *stream, const char *format, stdio_va_list argptr );
int stdout_printf (const char *format, ...);
int stderr_printf (const char *format, ...);

void perror(const char *str);


int printf(const char *format, ...); 

//tradicional, incompleta; n�o funciona,
//estamos implementando uma printf padr�o tradicional.
int printf2(const char *format, ...); 

int printf3(const char *fmt, ...);


//find next line
char *stdio_nextline ( char *string );

//next line sprintf
int nlsprintf ( char *out, const char *format, ... );

int sprintf(char *out, const char *format, ...);


//#test 
//?? coisa do c++ 
void rewind ( FILE * stream );

/*
 * Normal input.
 */
 
//Usado por interpretadores de comando.
//Recebem input e colocam em prompt[] 
unsigned long input(unsigned long ch);


/*
 * Formatted Input
 */
int scanf ( const char *fmt, ... );


//(since C99)	
//int fscanf( FILE *restrict stream, const char *restrict format, ... );
	
// #bugbug
// Essa fun��es est� implementada em fscanf/fscanf.c
int fscanf(FILE *stream, const char *format, ... ); 

int sscanf(const char *str, const char *format, ...);
 

long ftell (FILE *stream);

/*
 * Character Input and Output Functions
 */

// low level.

int __gramado__getc ( FILE *stream );
int __gramado__putc ( int ch, FILE *stream );

char *__gets(char *str);



int fgetc ( FILE *stream );;
int fputc (int ch, FILE *stream);

int getc(FILE* stream);
int putc(int ch, FILE* stream);



void debug_print (char *string);

int __serenity_fflush ( FILE *stream);
int __serenity_fputc (int ch, FILE *stream);
int __serenity_putc (int ch, FILE *stream);


//#define getchar()   getc(stdin)
//#define putchar(x)  putc((x), stdout)
int getchar (void);
int putchar (int ch);

int ungetc ( int c, FILE *stream );

char *gets (char *s);
char *fgets(char *s, int count, FILE *fp);


// glibc
/* The C standard explicitly says this can be a macro,
   so we always do the optimization for it.  */
//#define	putc(c, stream)	__putc ((c), (stream))

//#importante:
//Podemos usar isso para compatibilidade com as
//rotinas de baixo n�vel do bsd.
//#todo: podemos fazer o mesmo para compatibilidade com outras libs.
//>>> sempre usando as rotinas de baixo n�vel do gde.
//#define __sgetc(p) fgetc(p)
//#define __sputc(x, p) fputc(x, p)



size_t fread (void *ptr, size_t size, size_t n, FILE *fp);
size_t fwrite (const void *ptr, size_t size, size_t n, FILE *fp);




// prompt support.
// coloca no prompt[] em ring3 e exibe com uma rotina em ring0.
int prompt_putchar ( int c, int con_id );
int prompt_put_string ( char *string );
int prompt_strcat (char *string);
int prompt_flush ( int con_id );
void prompt_clean ();



/*
 * Direct Input and Output Functions
 */
 
 
 
/*
 * File Positioning Functions
 */

int fseek (FILE *stream, long offset, int whence);



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

//#bugbug: deletando isso...
//int app_clear(int color);

int drawBar(int color);  //??


//#todo
int snprintf(char *str,size_t count,const char *fmt,...);

int vprintf (const char *fmt, va_list ap);

//#test
//ainda n�o foi testada
int fscanf (FILE *stream, const char *format, ... );

// inicializa o fluxo padr�o para o processo.
int stdio_initialize_standard_streams (void);


// see: https://linux.die.net/man/3/setvbuf
void setbuf(FILE *stream, char *buf);
void setbuffer(FILE *stream, char *buf, size_t size);
void setlinebuf(FILE *stream);
int setvbuf(FILE *stream, char *buf, int mode, size_t size);


int libcStartTerminal (void);

/*Inicializa��o da biblioteca*/
void stdioInitialize ();


#endif    //__STDIO_H__

//
// End.
//
