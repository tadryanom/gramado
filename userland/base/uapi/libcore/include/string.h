/*
 * File: string.h
 *
 * Descri��o:
 *     Opera��es com strings. Padr�o C.
 */


#include <stddef.h>

#ifdef	_BSD_SIZE_T_
typedef	_BSD_SIZE_T_	size_t;
#undef	_BSD_SIZE_T_
#endif


int memcmp (const void *s1, const void *s2, size_t n);

 

char *strdup(const char *str);
char *strndup(const char *s, size_t n);

char *strrchr(const char *p, int ch);

int strcasecmp (const char *s1, const char*s2);

char *strncpy(char *s1, const char *s2, size_t n);

//@todo: Deve ser const char.
int strcmp(char *s1, char *s2);
int strncmp(char *s1, char *s2, int len);


//#bugbug deletando porque t� errado.
//int str_cmp(unsigned char *str1, unsigned char *str2);

void *memset ( void *ptr, int value, int size );
void *memoryZeroMemory(void* ptr, size_t cnt);
void *memcpy(void *v_dst, const void *v_src, unsigned long c);
//@todo: void *memcpy(void *dst, const void *src, size_t c); 
char *strcpy(char *to, const char *from);
char *strcat(char *to, const char *from);
void bcopy(char *from, char *to, int len);
void  bzero(char *cp, int len);
size_t strlen (const char *s);
size_t strnlen ( const char *s, size_t maxlen );


/* Copyright (c) 2011, 2012 Jonas 'Sortie' Termansen. */
size_t strcspn(const char* str, const char* reject);
size_t strspn(const char* str, const char* accept);
//char* strtok_r(char* str, const char* delim, char** saveptr);
//char* strtok(char* str, const char* delim);


/*apple style*/
char *strtok_r (char *s, const char *delim, char **last);
char *strtok (char *s, const char *delim);


char *strchr (const char *s, int c);


char *strstr (const char *s1, const char *s2);


//
// End.
//

