

//inttypes.h

// Conversão precisa entre tipos inteiros.

#include <types.h>

/*
#define PRId8 "d"
#define PRId16 "d"
#define PRId32 "d"
#define PRId64 "lld"
#define PRIi8 "d"
#define PRIi16 "d"
#define PRIi32 "d"
#define PRIi64 "lld"
#define PRIu8 "u"
#define PRIu16 "u"
#define PRIu32 "u"
#define PRIu64 "llu"
#define PRIx8 "b"
#define PRIx16 "w"
#define PRIx32 "x"
#define PRIX32 "X"
#define PRIx64 "llx"
#define PRIX64 "llX"
*/



typedef unsigned long uintptr_t;

typedef unsigned int uintmax_t;
typedef int intmax_t;

intmax_t strtoimax (const char* str, char** endptr, int base);
uintmax_t strtoumax (const char* str, char** endptr, int base);






