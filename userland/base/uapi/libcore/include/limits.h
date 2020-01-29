/*
 * File: limits.h 
 * 
 * Descri��o:
 *     Descreve alguns limites na linguagem C padr�o 32bit.
 *     @todo: Essa vers�o est� imcompleta.
 */
 
 
#ifndef _LIMITS_H
#define _LIMITS_H 

//
// # char support #
// 
 
//Number of bits in a 'char'.  
#define CHAR_BIT  8

//Minimum and maximum values a 'signed char' can hold.  
#define SCHAR_MIN  (-128)
#define SCHAR_MAX  127    
 
//Maximum value an 'unsigned char' can hold.  (Minimum is 0.).  
#define UCHAR_MAX  255

//
// # short support #
// 

//Minimum and maximum values a 'signed short int' can hold.  
#define SHRT_MIN  (-32768)
#define SHRT_MAX  32767
 
//Maximum value an 'unsigned short int' can hold.  (Minimum is 0.).  
#define USHRT_MAX  65535

//
// # int support #
// 

#define INT_MAX  2147483647
#define UINT_MAX	0xffffffff



#define	ULONG_MAX	0xffffffff	/* max value for an unsigned long */
#define	LONG_MAX	2147483647	/* max value for a long */
#define	LONG_MIN	(-2147483647-1)	/* min value for a long */




/* Minimum sizes required by the POSIX P1003.1 standard (Table 2-3). */

#define _POSIX_ARG_MAX    4096	/* exec() may have 4K worth of args */
#define _POSIX_CHILD_MAX     6	/* a process may have 6 children */
#define _POSIX_LINK_MAX      8	/* a file may have 8 links */
#define _POSIX_MAX_CANON   255	/* size of the canonical input queue */
#define _POSIX_MAX_INPUT   255	/* you can type 255 chars ahead */
#define _POSIX_NAME_MAX DIRSIZ	/* max. file name length */
#define _POSIX_NGROUPS_MAX   0	/* supplementary group IDs are optional */
#define _POSIX_OPEN_MAX     16	/* a process may have 16 files open */
#define _POSIX_PATH_MAX    255	/* a pathname may contain 255 chars */
#define _POSIX_PIPE_BUF    512	/* pipes writes of 512 bytes must be atomic */
#define _POSIX_STREAM_MAX    8	/* at least 8 FILEs can be open at once */
#define _POSIX_TZNAME_MAX    3	/* time zone names can be at least 3 chars */
#define _POSIX_SSIZE_MAX 32767	/* read() must support 32767 byte reads */

/* The number of symbolic links that can be
   traversed in the resolution of a pathname
   in the absence of a loop. */
#define _POSIX_SYMLOOP_MAX   8	


//
// End.
//

#endif /* _LIMITS_H */


