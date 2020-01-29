/*
 * File: pc/ipc/sem.c  
 *
 * Descrri��o:  
 *     Gerenciamento de sem�foros.
 *     Faz parte do Process Manager, parte fundamental do Kernel Base.
 *     Rotinas de Down e Up.
 *     Os processos entram e saem da sua regi�o cr�tica utilizando as 
 * rotinas de Down e Up. 
 * Obs: Quando um processo em user mode solicitar o uso de um sem�foro, ele
 * tem que ter em m�o um ponteiro para uma estrutura v�lida. Ent�o atrav�s de
 * de uma system call ele consegue utilizar os m�todos down e up. 
 *
 * History:
 *      2015 - Created by Fred Nora.
 *      2016 - Revision.
 */		
 

#include <kernel.h>


/*
 **************************************************************
 * init_semaphore:
 *     Inicializa uma estrutura de sem�foro.
 *     Obs: Quem chama essa fun��o deve alocar mem�ria para a 
 * estrutura e passar por argumento uma estrutura v�lida.
 */

int init_semaphore ( struct semaphore_d *s, unsigned int count ){

	if ( (void *) s == NULL )
	{
	    return (int) 1;    
	}
	
	// Atribui algum valor ao sem�foro. 
	// Deve ser 1, para que o primeiro processo � solicitar o sem�foro j� 
	// possa us�-lo.
	
	//1.
	s->count = (unsigned int) count;    
    
	return (int) 0;
}


/*
 ************************************************
 * Down:
 * Quando um processo vai entrar na sua regi�o cr�tica ele executa um Down.
 * Se o sem�foro estiver com zero, significa que
 *     o recurso j� est� bloqueado por outro processo 
 *     ent�o o processo que est� tentando utilizar o recurso
 *     deve esperar, mudando o estado para waiting.
 *		
 * O processo deve esperar.
 *		 Op��es:
 *		 +Retornamos um valor que indique que o processo deve esperar
 *		  em um loop.
 *       +Bloquamos o processo at� que o sem�foro esteja liberado.
 */ 

int Down (struct semaphore_d *s){

	// Enquanto for 0, eu espero. 
	// Quando mudar para 1, eu mudo para 0 e avan�o.
	
    int Flag;
	
	if ( (void *) s ==  NULL)
	{
		return (int) 1;     
	}else{
		
	    Flag = (int) s->count;	
	};
	
	
	switch (Flag)
	{
		//J� estava liberado, retornamos a indica��o de avan�ar.
		//Obs: O sem�fo fica bloquado.
		case 1: 
		    s->count = 0; 
			return (int) 0; 
			break;
			
		//Estava bloqueado, retornamos a indica��o de esperar.
		//Obs: Ele permanece bloqueado.
		case 0: 
		    return (int) 1; 
			break;
			
		//Quanquer valor estranho, retornamos a indica��o de esperar.
		default: 
			break;
	};
	//Nothing
	return (int) 1;
};


/*
 ***************************************************
 * Up:
 *     Quando um processo sai da sua regi�o cr�tica
 *     ele d� um Up no sem�foro, mudando seu valor pra 1.
 *     Isso libera o recurso pra outro processo.
 */ 

int Up (struct semaphore_d *s){
		
	//
	// *** Mudo para 1.
	//

	int Flag;
	
	
	if ( (void *) s == NULL)
	{
		return (int) 1;       
	}else{
		
		 Flag = (int) s->count;	
	};

	switch (Flag)
	{
		//O sem�foro j� est� liberado.
		case 1: 
		    return (int) 0; 
			break;
		
		//Caso ainda n�o esteja.
        case 0: 		
		default:
		    s->count = 1;
			return (int) 0;
		    break;
	};
	
fail:
    return (int) 1;
};


void semaphore_down (struct semaphore_d *s){
	
	Down (s);
}


void semaphore_up (struct semaphore_d *s){
	
	Up (s);
}


void *create_semaphore (void)
{
	//@todo:
    return NULL;
}


void delete_semaphore(struct semaphore_d *s)
{
	//@todo:
	
	//s = NULL;
    return;
};


void open_semaphore(struct semaphore_d *s)
{
	//@todo:	
    return; 
};

 
void close_semaphore(struct semaphore_d *s)
{
	//@todo:	
    return; 
};


//
// End.
//
