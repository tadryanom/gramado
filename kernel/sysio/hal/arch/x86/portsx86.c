/*
 * File: ports.c
 *
 * Descri��o:
 *     Interface de abstra��o para acesso as portas.
 *     A id�ia � que um m�dulo do kernel base possa acessar as portas sem se 
 * preocupar com o tipo de arquitetura. Mas por enquanto estamos apenas 
 * suportando a arquitetura intel.
 *
 *     O acesso as portas tamb�m ser� oferecido na forma de servi�os, para
 * que processos servidores tamb�m possam configurar o hardware diretamente.
 * Logicamente algum tipo de filtro ser� criado, para conferir a permiss�o.
 *
 *     @todo: As rotinas aqui, desviar�o a chamada para para a rotina da 
 * arquitetura atual.
 *
 *    Obs: O HAL deve chamar essas rotinas e n�o os programas.
 *
 *    Obs: Por enquanto a unica arquitetura suportada aqui
 *         � a arquitetura PC x86.
 *
 *   header em /hal/arch/x86/ports.h
 *
 * Vers�o 1.0, 2015, 2016.
 */



#include <kernel.h>


unsigned char inportb (int port){
	
    unsigned char ret;
 
    asm ("inb %%dx, %%al" : "=a"(ret): "d"(port) );
	
	return (unsigned char) ret;
};


void outportb ( int port, int data ){
	
    asm ("outb %%al, %%dx" :: "a" (data), "d" (port) );
};



/*
 * inportl:
 *     L� um dword de uma determina porta. */

unsigned long inportl (unsigned long port){
	
	unsigned long ret;

	asm volatile ("inl %%dx,%%eax" : "=a" (ret) : "d"(port) );
	
	return ret;
}


/*
 * outportl:
 *     Escreve um dword em uma determinada porta. */

void outportl ( unsigned long port, unsigned long value ){
	
	asm volatile ("outl %%eax,%%dx" :: "d" (port), "a" (value) );
}




//inport byte
unsigned char kinportb (int port){
	
    unsigned char ret;
 
    asm ("inb %%dx, %%al" : "=a"(ret): "d"(port) );
	
	return (unsigned char) ret;
};


/*
 * koutportb:
 * Escreve um byte em uma determina porta.
 */
void koutportb ( unsigned int port, unsigned int value ){
    
	asm volatile ("outb %%al, %%dx" :: "d" (port), "a" (value) );
};


//inport byte.
unsigned char inb (int port){
	
    unsigned char ret;
 
    asm ("inb %%dx, %%al" : "=a"(ret): "d"(port) );
	
	return (unsigned char) ret;
};


//outpot byte
void outb ( int port, int data ){
    
	asm ("outb %%al, %%dx" :: "a" (data), "d" (port) );
};





int inport8 (int port){
	
	unsigned char ret;
	
    __asm ("inb %%dx, %%al" : "=a" (ret) : "d" (port) );
	
	return (unsigned char) ret;
};


void outport8 (int port, int data){
    
	__asm ("outb %%al, %%dx" :: "a" (data), "d" (port) );
};


int inport16 (int port){
	
    unsigned short ret;
	
    __asm ("inw %%dx, %%ax" : "=a" (ret) : "d" (port) );
	
	return (unsigned short) ret;
};


void outport16 (int port, int data){
    
	__asm ("outw %%ax, %%dx" :: "a" (data), "d" (port) );
};


int inport32 (int port){
	
	unsigned long ret;
    
	__asm("inl %%dx, %%eax" : "=a" (ret) : "d" (port) );
	
	return (unsigned long) ret;
};


void outport32 ( int port, int data ){ 

    __asm ("outl %%eax, %%dx" :: "a" (data), "d" (port) );
};



// Input.
// Input a value from the keyboard controller's data port, after checking
// to make sure that there's some data there for us.
static unsigned char inPort60 (void){
	
    unsigned char data = 0;

    while (!(data & 0x01))
        kernelProcessorInPort8(0x64, data);

    kernelProcessorInPort8(0x60, data);
	
    return (data);
};


// Output.
// Output a value to the keyboard controller's data port, after checking
// to make sure it's ready for the data.
static void outPort60 (unsigned char value){
	
    unsigned char data;
  
    // Wait for the controller to be ready
    data = 0x02;
    while (data & 0x02)
        kernelProcessorInPort8(0x64, data);
  
    data = value;
    kernelProcessorOutPort8(0x60, data);
 
    return;
};


// Output.
// Output a value to the keyboard controller's command port, after checking
// to make sure it's ready for the command
static void outPort64 (unsigned char value){
	
    unsigned char data;
  
    // Wait for the controller to be ready
    data = 0x02;
	
    while (data & 0x02)
        kernelProcessorInPort8(0x64, data);

    data = value;
    kernelProcessorOutPort8(0x64, data);
	
	return;
};



//ir para portsx86
int kernelProcessorInPort8 ( int port, int data){
	
    asm ("inb %%dx, %%al" : "=a" (data) : "d" (port) );
};

//ir para portsx86
int kernelProcessorOutPort8 (int port, int data){
	
    __asm ("outb %%al, %%dx" : : "a" (data), "d" (port) );
};


//ir para portsx86
int kernelProcessorInPort16 (int port,int data){
	
    __asm ("inw %%dx, %%ax" : "=a" (data) : "d" (port) );
};

//ir para portsx86
int kernelProcessorOutPort16 (int port, int data){
	
    __asm ("outw %%ax, %%dx" : : "a" (data), "d" (port) );
};

//ir para portsx86
int kernelProcessorInPort32 (int port, int data){
	
    __asm ("inl %%dx, %%eax" : "=a" (data) : "d" (port) );
};

//ir para portsx86
int kernelProcessorOutPort32 (int port,int data){ 

    __asm ("outl %%eax, %%dx" : : "a" (data), "d" (port) );
};


void __x86_io_delay (void)
{
    asm ("xorl %%eax, %%eax" ::);
    asm ("outb %%al, $0x80" ::);
    return;
}



void wait_ns (int count)
{
    count /= 100; 

    while (--count)
        io_delay ();
}


unsigned long portsx86_IN ( int bits, unsigned long port ){

    switch (bits)
	{
		case 8:
			return (unsigned long) inportb ((int) port);
			break;
			
		case 16:
			return (unsigned long) inport16 ( (int) port);
			break;
			
		case 32:
			return (unsigned long) inportl ( (unsigned long) port);
			break;
	
		default:
			return 0;
			break;
	}	
}


void portsx86_OUT ( int bits, unsigned long port, unsigned long value ){

    switch (bits)
	{
		case 8:
			outportb ( (int) port, (int) value );
			return;
			break;
			
		case 16:
			outport16 ( (int) port, (int) value );
			return;
			break;
			
		case 32:
			outportl ( (unsigned long) port, (unsigned long) value );
			return;
			break;
	
		default:
			return;
			break;
	}
}




//
//  End.
//

