/*
 * File: x/i8042ldisc.c
 *
 * Descri��o:
 *    Esse ser� o gerenciador de Line Discipline.
 *    Ficar� dentro do kernel base e receber� as entradas 
 * dos dispositivos de caractere e enviar� para as filas apropriadas.
 *    Por enquanto os scancodes de teclado s�o tratados e enviados 
 * para a fila de mensagem da janela apropriada. Principalmente a janela 
 * com o foco de entrada. 
 *
 */


// #todo
// A disciplina de linhas poderia chamar o procedimento de janelas padr�o
// do gramado x-server ao inv�s de chamar o procedimento de janelas do sistema.


// #importante
// Essas rotinas est�o ligadas as formas de input.
// seus poss�veis lugares s�o: 
// + line discipline
// + x server
// + terminal virtual.

#include <kernel.h>



//ldisc
//inicializando a fila de mensagens do sistema
//com ponteiros para estruturas de mensagens ...
//es estruturas ser�o reutiliz�veis.

void initialize_system_message_queue (void){
	
	struct message_d *m;
	
	int i;
	
	for ( i=0; i<SYSTEM_MESSAGE_QUEUE_MAX; i++ )
	{
		
		m = (void *) kmalloc ( sizeof(struct message_d) );
		
		if ( (void *) m == NULL )
		{
			panic ("ldisc-initialize_system_message_queue:");
		}else{

			m->objectType = ObjectTypeMessage;
			m->objectClass = ObjectClassKernelObjects;
		    m->used = 1;
			m->magic = 1234;
			m->empty = 1;

            system_message_queue[i] = (unsigned long) m;
		}; 
	};


	system_message_write = 0;
	system_message_read = 0;
}




unsigned long 
ldisc_dialog ( struct window_d *window, 
              int msg, 
              unsigned long long1, 
              unsigned long long2 ) 
{
    //# cancelada !!!
    return 0;	
}



//
// End.
//


