/*
 * File: types.h 
 * 
 * Descrição:
 *     Define tipos de variáveis.
 *     Tipos usados na biblioteca C99 32bit em user mode.
 *
 * Versão 1.0, 2015~2016.
 */

 
#define TRUE 1
#define FALSE 0
#define NULL ((void *)0)


//HANDLE.
typedef void *HANDLE;

//Void.
typedef void VOID; 
 
//Basics.
typedef char  CHAR;
typedef short SHORT;
typedef int INT;          //32bit
typedef long  LONG;       //32bit

//Unsigned Basics.
typedef unsigned char  UCHAR;
typedef unsigned short USHORT;
typedef unsigned int UINT;        //32bit
typedef unsigned long  ULONG;     //32bit

//Ints.  
typedef char   int8_t;
typedef short int16_t;
typedef int   int32_t;

//Unsigned Ints. 
typedef unsigned char   uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int   uint32_t;

//
//End.
//

