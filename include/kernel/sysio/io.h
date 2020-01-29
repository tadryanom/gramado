/*
 * File: io.h
 *
 * Descri��o:
 *     Arquivo principal do I/O Manager.
 *     Todo gerenciamento de dispositivos,
 *     toda concess�o de acesso passar� por 
 *     esse gerenciador, que faz parte da
 *     camada de abstra��o de hardware oferecidad pelo
 *     kernel base.
 *
 * Vers�o 1.0, 2016.
 */
 


//Estrutura para gerenciamento de buffer de i/o. 
typedef struct ioBuffer_d ioBuffer_t; 
struct ioBuffer_d
{
	object_type_t objectType;
	object_class_t objectClass;
	
	struct process_d *OwnerProcess;    //processo dono do buffer.
    unsigned long address;    //Address.
    int size;              	  //size in number of bytes.
}; 
ioBuffer_t *lineBuffer;    //Buffer para uma linha de caracteres.
ioBuffer_t *fileBuffer;    //buffer para um arquivo de texto.
ioBuffer_t *CurrentBuffer;
ioBuffer_t *ipcBuffer;    //IPC buffer. (�rea de transfer�ncia.)
//... 
 
 
//
// Prot�tipos.
// 
 

void *ioServices( unsigned long number, 
                  unsigned long arg2, 
				  unsigned long arg3, 
				  unsigned long arg4 );


// #bugbug
// Precisamos rever os argumentos, principalmente o �ltimo.
int 
sys_ioctl ( int fd, unsigned long request, char *arg );


int ioInit (void);

//
// End.
//

