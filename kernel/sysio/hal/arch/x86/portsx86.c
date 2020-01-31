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


// goal
// in8 in16 in32
// out8 out16 out32



unsigned char in8 (int port){

    unsigned char ret;
 
    asm volatile ("inb %%dx, %%al" : "=a"(ret): "d"(port) );

    return (unsigned char) ret;
}

unsigned short in16 (int port){

    unsigned short ret;

    asm volatile ("inw %%dx, %%ax" : "=a" (ret) : "d" (port) );

    return (unsigned short) ret;
}

unsigned long in32 (int port){

    unsigned long ret;

    asm volatile ("inl %%dx,%%eax" : "=a" (ret) : "d"(port) );

    return (unsigned long) ret;
}



void out8 ( int port, unsigned char data ){

    asm volatile ("outb %%al, %%dx" :: "a" (data), "d" (port) );
}

void out16 (int port, unsigned short data){
    
    asm volatile ("outw %%ax, %%dx" :: "a" (data), "d" (port) );
}

void out32 ( int port, unsigned long data ){

    asm volatile ("outl %%eax, %%dx" :: "a" (data), "d" (port) );
}


//
// --------------
//



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
			return (unsigned long) in8 ((int) port);
			break;
			
		case 16:
			return (unsigned long) in16 ( (int) port);
			break;
			
		case 32:
			return (unsigned long) in32 ( (int) port);
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
			out8 ( (int) port, (unsigned char) value );
			return;
			break;
			
        case 16:
			out16 ( (int) port, (unsigned short) value );
			return;
			break;
			
        case 32:
			out32 ( (int) port, (unsigned long) value );
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

