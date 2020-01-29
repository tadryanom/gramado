/*
 * File: mm/mmglobal.h
 *
 * Aqui terão globais para gerenciamento de memória.
 * listas páginas e frames.
 * é o acesso global a informações gerais sobre o gerenciamento de páginas.
 *
 */


// Macros para conversão de endereços.
#define PAGE_GET_DIR(addr)   ((unsigned)(addr) >> 22)
#define PAGE_GET_TAB(addr)  (((unsigned)(addr) >> 12) & 0x3FF)

/*
// Definições dos campos das páginas.
#define PAGE_PRESENT            0x001
#define PAGE_WRITE              0x002
#define PAGE_USER               0x004
#define PAGE_NOCACHE            0x010
#define PAGE_ACCESSED           0x020
#define PAGE_DIRTY              0x040
#define PAGE_GLOBAL             0x100
#define PAGE_AVAIL              0xE00
#define PAGE_PRESENT_WRITE      (PAGE_PRESENT | PAGE_WRITE)
#define PAGE_PRESENT_WRITE_USER (PAGE_PRESENT | PAGE_WRITE | PAGE_USER)
#define PAGE_KERNEL_PGPROT	(PAGE_PRESENT_WRITE)
*/


/*
 Obs: lfb é um objeto.
      Teremos um lfb por cartão de memória.
	  
typedef struct lfb_d lfb_t;
struct lfb_d
{
	//object ...
	
	int used;
	int magic;
	
	int status;
	unsigned long address; 
};
*/


//
// End.
//

