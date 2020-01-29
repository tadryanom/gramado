/*
 * File: ps/mm/x86/memory.c
 *
 * Descri��o:
 *     Arquivo principal do m�dulo /mm do /pc, Memory Manager. 
 *     MB - M�dulos inclu�dos no Kernel Base.
 *
 * Atribui��es:
 *     + Alocar Heap do Kernel.
 *     + Liberar Heap do Kernel.
 *     + Inicializar Heap e Stack do Kernel.
 *     + Gerenciar Page Directory, Page Table ...
 *     ...
 *
 * Obs:
 *     Uma regi�o � uma �rea de 4MB alocada. S�o todas as p�ginas
 * que cabem em uma tabela, 2014 p�ginas de 4KB cada. Isso tamb�m pode
 * ser chamado de pool e ser alocado todo de uma vez. At� mesmo
 * compartilhado entre processos.
 *
 * Sobre o heap de processo:
 *    Uma vari�vel global indica qual � o Heap do processo atual. 
 *    A estrutura de um processo cont�m informa��es sobre o seu Heap.
 *    ... 
 *
 * @todo: Criar: 
 * Criar uma fun��o que aloca mem�ria no heap de um processo. Deve-se usar 
 * o heap do desktop ao qual o processo pertence ou o heap do processo.
 * 
 * Obs: Poss�veis organiza��es de Heap: 
 *     + Heap Size Modifier, (HSM).
 *     + Heap Size Checker, (HSC). 
 *     + Desktop Heap Size Manager, (DHSM). 
 *       Gerencia o Heap alocado para cada desktop. Os programas usam o Heap
 * do seu pr�prio desktop. Alguns desktops podem ter Heaps pequenos.
 * @todo: Criar o arquivo heap.c para as rotinas de ger�ncia de heap.
 * Continua...
 *
 * Obs: Um processo pode criar um heap privado, que ele usar� para aloca��o 
 * din�mica.
 *      
 *
 * In this file:
 * =============
 *     +get_process_heap_pointer
 *     +SetKernelHeap
 *     +heapAllocateMemory
 *     +FreeHeap
 *     +init_heap
 *     +init_stack
 *     +init_mm
 *
 *
 * @todo: Criar: heapGetPointer, heapSePointer.
 *
 *
 * @todo: IMPORTANTE: Devemos ter um heap do kernebase, grande
 * o bastante para alocarmos os recursos gr�ficos grenciados
 * pelo m�dulo /gramado.
 *
 * @todo:
 * IMPORTANTE: Um alocador de mem�ria f�sica precisa ser criado,
 * que considere o tamanho da mem�ria f�sica dispon�vel.
 * Esse alocador deve ser usado para alocarmos uma regi�o bem grande
 * da mem�ria f�sica onde ficar�o os frames de mem�ria f�sica. Os 
 * frames livres ser�o encontrados nessa regi�o e o alocador de 
 * p�ginas, utilizar� esses frames livres para associ�-los �s
 * p�ginas alocadas aos processos.
 *
 * @todo: Vari�veis globais devem controlar o in�cio e o fim da �rea
 * destinada aos frames de mem�ria f�sica. semelhando ao que foi 
 * feito com o heap do kernel base. Faremos isso no in�cio do arquivo mm.h.
 *
 * Hist�rico:
 *     Vers�o 1.0, 2015 - Esse arquivo foi criado por Fred Nora.
 *     Vers�o 1.0, 2016 - Aprimoramento geral das rotinas b�sicas.
 *     ...
 */

 
/*
 @todo:
 +Criar o arquivo heap.c 
 +Criar a fun��o CreateHeap: Se o argumento 'endere�o' for nulo, alocamos mem�ria.
 ...
*/ 
 
 
#include <kernel.h>
 
 
//Vari�veis internas. 

//int mmStatus;

// Heap support.
unsigned long last_valid;         //�ltimo heap pointer v�lido. 
unsigned long last_size;          //�ltimo tamanho alocado.
unsigned long mm_prev_pointer;    //Endere�o da �ntima estrutura alocada.







/*
 Traduz um endere�o virtual em um endere�o f�sico.
unsigned long memoryGetPhysicalAddress( unsigned long virtual_address);
unsigned long memoryGetPhysicalAddress( unsigned long virtual_address){
	//return (unsigned long) ;
}
*/


/*
unsigned long heap_set_new_handler( unsigned long address );
unsigned long heap_set_new_handler( unsigned long address )
{
    unsigned long Old;

    Old = kernel_heap_start;
    
    kernel_heap_start = address;

    return (Old);
};
*/


/*
 ***********************************************************
 * get_process_heap_pointer:
 *     ?? Pega o 'heap pointer' do heap de um processo. ??
 */

unsigned long get_process_heap_pointer ( int pid ){

    struct process_d *P;

    unsigned long heapLimit;


	//@todo: Limite m�ximo.

    if (pid < 0)
    {
		printf ("get_process_heap_pointer: pid fail\n");
		goto fail;
    }


    P = (void *) processList[pid];

    if ( (void *) P == NULL )
    {
		printf ("get_process_heap_pointer: struct fail\n");
		goto fail;
    }


	//Obs podemos checar 'used' e 'magic'.
	
	// Limits:
	//     Cada processo tem uma �rea onde ele pode alocar mem�ria, os 
    // processos usam seu pr�prio heap ou o heap do desktop ao qual pertencem.
    // Os limites devem ser respeitados.	
	
	heapLimit = (unsigned long) (P->Heap + P->HeapSize);
	
	//Se for menor que o in�cio ou maior que o limite.
    if ( P->HeapPointer < P->Heap || P->HeapPointer >= heapLimit )
    {
		printf ("get_process_heap_pointer: heapLimit\n");
		goto fail;
    }


    //Retorna o heap pointer do processo. 
    return (unsigned long) P->HeapPointer;

fail:

    return (unsigned long) 0;    
}




// #todo
/*
int 
memory_use_this_heap ( struct heap_d *heap );
int 
memory_use_this_heap ( struct heap_d *heap )
{
    if ( (void *) heap == NULL )
    {
        panic ("memory_use_this_heap: struct");
    }else{

        if ( heap->used != 1 || heap->magic != 1234 )
        {
            panic ("memory_use_this_heap: validation");
        }

        // #todo
    };
}
*/




/*
 ****************************************************
 * memory_create_new_head:
 *     Cria um novo heap dado um endere�o virtual v�lido.
 *     #importante
 *     S� podemos usar isso depois que configurarmos manualmente
 *     o heap do kernel.
 */

struct heap_d *memory_create_new_head ( unsigned long start_va, 
                                        unsigned long size )
{
    struct heap_d *h;  


    unsigned long __start;
    unsigned long __end; 
    unsigned long __available;
    
    // Slot support.
    int i;
    int __slot = -1;


    // #todo
    // Tem outros limites que precisam ser respeitados.

    // No inpicio da mem�ria virtual.
    if ( start_va == 0 )
    {
        panic ("memory_create_new_head: Invalid address! (1)");
    }

    // Em cima da im�gem do app.
    if ( start_va >= 0x00400000 && start_va <= 0x007F0000 )
    {
        panic ("memory_create_new_head: Invalid address! (2)");
    }
    
    // Em cima da im�gem do kernel.
    if ( start_va >= 0xC0000000 && start_va <= 0xC0100000 )
    {
        panic ("memory_create_new_head: Invalid address! (3)");
    }
   
    
    // Ajuste para o m�nimo.
    if ( size == 0 )
        size = 32;


    // N�o pode ser maior que 4MB.
    // Por enquanto.
    //if ( size >= (1024*1024*4) )
    if ( size >= (1024*1024*2) )
    {
        panic ("memory_create_new_head: Invalid size");
    }
 
    __start = start_va;
    __end = (__start + size);  
    __available = size;


    //
    // Get slot;
    //
    
    struct heap_d *tmp;

    for ( i=0; i<HEAP_COUNT_MAX; i++ )
    {
        tmp = ( struct heap_d * ) heapList[i];
        
        // Ok.
        if ( (void *) tmp == NULL )
        {
            __slot = i;
            goto ok;
        }
    }

    panic ("memory_create_new_head: Overflow");

ok:

    if ( __slot <= 0 || __slot >= HEAP_COUNT_MAX )
    {
        panic ("memory_create_new_head: __slot");
    }
    
    
    //
    // Struct
    //
    
    // Podemos alocar mem�ria para a estrutura de heap
    // porque j� temos p heap do kernel que foi
    // criado usando vari�veis globais.
    // As vari�veis globais servem para o heap atual.

    h = (void *) kmalloc ( size );

    if ( (void *) h == NULL )
    {
        panic ("memory_create_new_head: struct");
    }else{

        //#todo
        //h->objectType = 0;
        //h->objectClass = 0;
        
        h->id = __slot;

        h->used = 1;
        h->magic = 1234;

        h->HeapStart = (unsigned long) __start;
        h->HeapEnd   = (unsigned long) __end;

        h->HeapPointer   = (unsigned long) h->HeapStart;
        h->AvailableHeap = (unsigned long) __available;


        // Register.
        heapList[__slot] = (unsigned long) h;
        
        return ( struct heap_d *) h;
    };

    // Fail.
    return NULL;
}


// Destr�i um heap se as flags permitirem.
void 
memory_destroy_heap (struct heap_d *heap )
{

    int __slot = -1;
    
    if ( (void *) heap == NULL )
    {
        return;
    }else{

        // Condi��o que permite destruir.
        if ( heap->used != 216 || heap->magic != 4321 )
        {
            if ( heap->id >= 0 || heap->id < HEAP_COUNT_MAX )
            {
                __slot = heap->id;
                
                // Limpa a lista
                heapList[__slot] = (unsigned long) 0;
                
                //#todo
                // Limpar a estrutura.
                
                // Deleta a estrutura
                heap = NULL;
            }
        }
    };
}



/*
 * HeapAlloc:
 * @todo: Criar essa rotina.
 * Aloca mem�ria dentro de um heap determinado.
 * Esse rotina deve ser oferecida como servi�o e chamada via system call.
void* HeapAlloc(struct heap_d * heap, unsigned long size);
void* HeapAlloc(struct heap_d * heap, unsigned long size)
{
	return (void*) ...;
}
*/


/*
 * GetProcessHeap:
 @todo:
 Retorna um ponteiro para a estrutura do heap de um processo.
 Obs: Oferecer como servi�o do sistema.
void *GetProcessHeap(struct process_d *process);
void *GetProcessHeap(struct process_d *process)
{
	return NULL;
}
*/


/*
 * GetHeap:
 @todo:
 Retorna um ponteiro para o heap do processo atual.
 Obs: Oferecer como servi�o do sistema.
void *GetHeap();
void *GetHeap()
{
	//@todo: Pegar o identificador do processo atual.
	//pegar na estrutura do processo o ponteiro para seu heap.
	return NULL;
}
*/


/*
 ****************************************************************
 * heapAllocateMemory:
 *     Aloca mem�ria no heap do kernel.
 *
 * *IMPORTANTE: 
 *     Aloca BLOCOS de mem�ria dentro do heap do processo Kernel.
 *
 * @todo: 
 *     ?? Ao fim dessa rotina, os valores da estrutura devem ser 
 * armazenas no header, l� onde foi alocado espa�o para o header, 
 * assim tem-se informa��es sobre o header alocado. ??
 *
 *  A estrutura header do heap, � uma estrutura e deve ficar antes 
 * da �rea desejada. Partes={ header,client,footer }.
 *
 * Obs: 
 *     ?? A estrutura usada aqui � salva onde, ou n�o � salva ??
 *
 * IN:  size in bytes
 * OUT: address if success. 0 if fail.
 *
 * History:
 *     2015 - Created by Fred Nora.
 *     2016 - Revision.
 * ...
 */

unsigned long heapAllocateMemory ( unsigned long size ){
	
    struct mmblock_d *Current;

    // @todo: Aplicar filtro.
    // Aqui podemos checar se o quantidade de heap dispon�vel
    // est� coerente com o tamanho do heap. Se essa quantidade
    // for muito grande, maior que o heap total, ent�o temos um problema.
 
    // Se n�o h� espa�o dispon�vel no heap, n�o h� muito o que fazer.
    // Uma op��o seria tentar almentar o heap, se isso for poss�vel.

    // Available heap.
	
    if (g_available_heap == 0)
    {
        // #todo: 
		// Tentar crescer o heap para atender o size requisitado.

        //try_grow_heap() ...

        // #todo: 
		// Aqui poderia parar o sistema e mostrar essa mensagem.

        printf ("heapAllocateMemory fail: g_available_heap={0}\n");
        goto fail;
    };


    //
    // Size limits. (Min, max).
    //

    // Se o tamanho desejado for igual a zero.
    // @todo: Aqui podemos converter o size para o tamanho m�nimo.
    // n�o h� problema nisso.
	
    if( size == 0 )
	{
        //size = 1;
        printf("heapAllocateMemory error: size={0}\n");
        refresh_screen();
        
		//?? NULL seria o retorno para esse caso ??
		return (unsigned long) g_heap_pointer;
    };

    // Se o tamanho desejado � maior ou 
	// igual ao espa�o dispon�vel.
    if( size >= g_available_heap )
    {
        //
        // @todo: Tentar crescer o heap para atender o size requisitado.
        //

        //try_grow_heap() ...

        printf ("heapAllocateMemory error: size >= g_available_heap\n");
        goto fail;
    };
    
	
    //Salvando o tamanho desejado.
    last_size = (unsigned long) size;
    
    //
    // Contador de blocos.
    //

try_again:
	
    // #bugbug
    // Mesmo tendo espa�o suficiente no heap, estamos chegando 
	// nesse limite de indices.
    // Obs: 
	// Temos um limite para a quantidade de �ndices na lista de blocos.	

    mmblockCount++;  
  
    if ( mmblockCount >= MMBLOCK_COUNT_MAX )
    {
        panic ("heapAllocateMemory: MMBLOCK_COUNT_MAX");
    }

	// #importante
    // A vari�vel 'Header', no header do bloco, 
	// � o in�cio da estrutura que o define. 'b->Header'. 
	// Ou seja, o endere�o da vari�vel marca o in�cio da
    // estrutura.
    //
    // Pointer Limits:
    // ( N�o vamos querer um heap pointer fora dos limites 
	//   do heap do kernel ).
    // Se o 'g_heap_pointer' atual esta fora dos limites do heap, 
	// ent�o devemos usar o �ltimo v�lido, que provavelmente est� 
	// nos limites. ?? #bugbug: Mas se o �ltimo v�lido est� sendo 
	// usado por uma aloca��o anterior. ?? Temos flags que 
	// indiquem isso ??
    //
    // #importante: 
	// O HEAP POINTER TAMB�M � O IN�CIO DE UMA ESTRUTURA. 
    // NESSA ESTRUTURA PODEMOS SABER SE O HEAP ESTA EM USO OU N�O.
    // ISSO SE APLICA � TENTATIVA DE REUTILIZAR O �LTIMO HEAP 
	// POINTER V�LIDO.

    //Se estiver fora dos limites.
	
    if ( g_heap_pointer < KERNEL_HEAP_START || 
         g_heap_pointer >= KERNEL_HEAP_END )
    {
        // #bugbug: ?? Como saberemos, se o �ltimo v�lido,
        // n�o est� em uso por uma aloca��o anterior. ??

        //Checa os limites o �ltimo last heap pointer v�lido.
        if ( last_valid < KERNEL_HEAP_START || 
             last_valid >= KERNEL_HEAP_END )
        {
            printf ("pc-mm-memory-heapAllocateMemory: last_valid");
            die ();
        };

        // #todo: 
		// Checar a disponibilidade desse �ltimo v�lido.
        // Ele � v�lido, mas n�o sabemos se est� dispon�vel.
		
		//Havendo um last heap pointer v�lido.
		//?? isso n�o faz sentido.
		
		g_heap_pointer = (unsigned long) last_valid + last_size;
		
		goto try_again;
	};
	

    // Agora temos um 'g_heap_pointer' v�lido, salvaremos ele.
    // 'last_valid' N�O � global. Fica nesse arquivo.
    
    last_valid = (unsigned long) g_heap_pointer;
    

	// #importante:
    // Criando um bloco, que � uma estrutura mmblock_d.
    // Estrutura mmblock_d interna.
    // Configurando a estrutura para o bloco atual.
    //
    // Obs: A estutura dever� ficar l� no espa�o reservado 
	// para o header. (Antes da area alocada).
	//
    // Current = (void*) g_heap_pointer;

    
	// ## importante ##
	// O endere�o do ponteiro da estrutura ser� o pointer do heap.
	
    Current = (void *) g_heap_pointer;    

    if ( (void *) Current != NULL )
    {
        // #importante:
		// Obs: Perceba que 'Current' e 'Current->Header' 
		// devem ser iguais. 

        // Identificadores b�sicos:
		// Endere�o onde come�a o header.
		// Tamanho do header. (*TAMANHO DA STRUCT).
		// Id do mmblock. (�ndice na lista)
		// used and magic flags.
		// 0=not free 1=FREE (*SUPER IMPORTANTE)
		
		Current->Header = (unsigned long) g_heap_pointer;  
        Current->headerSize = MMBLOCK_HEADER_SIZE;         
        Current->Id = mmblockCount;                        
        Current->Used = 1;                
        Current->Magic = 1234;            
        Current->Free = 0;                
        //Continua...

        //
        // Mensuradores. (tamanhos) (@todo:)
        //


        // @todo:
        // Tamanho da �rea reservada para o cliente.
        // userareaSize = (request size + unused bytes)
        // Zera unused bytes, j� que n�o foi calculado.

        // User Area base:
        // *Onde come�a a �rea solicitada. 
        // *F�cil. Isso fica logo depois do header.

        // Obseve que 'Current->headerSize' � igual 
		// a 'MMBLOCK_HEADER_SIZE'
        // E que 'Current->headerSize' � o in�cio da estrutura.

        Current->userArea = (unsigned long) Current->Header + Current->headerSize;

        // Footer:
        // >> O footer come�a no 
		// 'endere�o do in�cio da �rea de cliente' + 'o tamanho dela'.
        // >> O footer � o fim dessa aloca��o e in�cio da pr�xima.
        //
		// #bugbug: Penso que aqui dever�amos considerar 
		// 'userareaSize' como tamanho da �rea de cliente, 
		// esse tamanho equivale ao tamanho solicitado mais o 
		// tanto de bytes n�o usados.
        // Obs: Por enquanto o tamanho da �rea de cliente tem 
		// apenas o tamanho do espa�o solicitado.
        
		Current->Footer = (unsigned long) Current->userArea + size;


        // Heap pointer. 
        //     Atualiza o endere�o onde vai ser a pr�xima aloca��o.

        //if ( Current->Footer < KERNEL_HEAP_START){
        //    Current->Used = 0;                //Flag, 'sendo Usado' ou 'livre'.
        //    Current->Magic = 0;            //Magic number. Ver se n�o est� corrompido.
        //	goto try_again;
        //}


        // Obs: 
		// O limite da contagem de blocos foi checado acima.

        //
        // Coloca o ponteiro na lista de blocos.
        //

        mmblockList[mmblockCount] = (unsigned long) Current;

        // Salva o ponteiro do bloco usado como 'pr�vio'.
        // Obs: 'mm_prev_pointer' n�o � global, fica nesse arquivo.

        mm_prev_pointer  = (unsigned long) g_heap_pointer; 

		
        // *****************************************************
        //                **** SUPER IMPORTANTE ****
        // *****************************************************
        // Atualiza o ponteiro. 
		// Deve ser onde termina o �ltimo bloco configurado.
        // Isso significa que o pr�ximo ponteiro onde come�aremos 
		// a pr�xima estrutura fica exatamente onde come�a o footer 
		// dessa estrutura.
        // Obs: O footer est� aqui somente para isso. Para ajudar
        // a localizamarmos o in�cio da pr�xima estrutura.

        g_heap_pointer = (unsigned long) Current->Footer;


        // Available heap:
        // Calcula o valor de heap dispon�vel para as pr�ximas aloca��es.
        // O heap dispon�vel ser� o que t�nhamos dispon�vel menos o que 
        // gastamos agora.
        // O que gastamos agora foi o tamanho do header mais o tamanho da �rea
        // de cliente.

        g_available_heap = (unsigned long) g_available_heap - (Current->Footer - Current->Header);

        
		// ## Finalmente ##
		//
        // Retorna o ponteiro para o in�cio da �rea alocada.
		// Essa �rea alocada chamado de user area.
        // Obs: Esse � o valor que ser� usado pela fun��o kmalloc.
        //
        // *Importante:
        // O que acontece se um aplicativo utilizar al�m da �rea alocada ??
        // O aplicativo invadir� a �rea do footer, onde est� a estrutura do 
        // pr�ximo bloco. Inutilizando as informa��es sobre aquele bloco.
        // *Aviso: Cuidado com isso. @todo: Como corrigir.?? O que fazer??

        return (unsigned long) Current->userArea;
		
        //Nothing.

    }else{

        //Se o ponteiro da estrutura de mmblock for inv�lido.
		
        printf ("heapAllocateMemory fail: struct\n");
		
        goto fail;
    };

	
    // #todo: 
    // Checar novamente aqui o heap dispon�vel. Se esgotou, tentar crescer.
    // Colocar o conte�do da estrutura no lugar destinado para o header.
    // O header conter� informa��es sobre o heap.
	// Se falhamos, retorna 0. Que equivaler� � NULL.


fail:

    refresh_screen ();
    return (unsigned long) 0;
}


/*
 ********************
 * FreeHeap:
 */

void FreeHeap (void *ptr){

    struct mmblock_d *Header;


    if ( (void *) ptr == NULL )
        return;


    if ( ptr < (void *) KERNEL_HEAP_START || ptr >= (void *) KERNEL_HEAP_END )
    {
        return;
    }


	// Header
	// Encontrando o endere�o do header.
	// O ponteiro passado � o endere�o da �rea de cliente.

    unsigned long UserAreaStart = (unsigned long) ptr; 


    Header = (void *) ( UserAreaStart - MMBLOCK_HEADER_SIZE );

    if ( (void *) Header == NULL )
    {
		return;
		
    }else{

		if ( Header->Used != 1 || Header->Magic != 1234 )	
		{
			return;
		}
		
		//Checa
		if ( mmblockList[mmblockCount] == (unsigned long) Header && 
			 Header->Id == mmblockCount )
		{
			mmblockList[mmblockCount] = 0;
			mmblockCount--;
		}
		
		//Isso invalida a estrutura, para evitar mal uso.
		Header->Used = 0;
		Header->Magic = 0;
		
		g_heap_pointer = (unsigned long) Header;
    };
}


/*
 *********************************************
 * init_heap:
 *     Iniciar a ger�ncia de Heap do kernel. 
 *     @todo: Usar heapInit() ou heapHeap(). memoryInitializeHeapManager().
 *
 * Essa rotina controi a m�o o heap usado pelo processo kernel.
 *     +Ela � chamada apenas uma vez.
 *     +Ela deve ser chamada entes de quelquer outra opera��o 
 * envolvendo o heap do processo kernel.
 * 
 * @todo: Rotinas de automa��o da cria��o de heaps para processos.
 */

//int memoryInitializeHeapManager() 

int init_heap (void){

    int i = 0;


    //Globals.
    kernel_heap_start = (unsigned long) KERNEL_HEAP_START;  
    kernel_heap_end = (unsigned long) KERNEL_HEAP_END;  
	
	//Heap Pointer, Available heap and Counter.
	g_heap_pointer = (unsigned long) kernel_heap_start; 
    g_available_heap = (unsigned long) (kernel_heap_end - kernel_heap_start);  
	heapCount = 0;      
	
	// #importante
	// �ltimo heap pointer v�lido. 
	last_valid = (unsigned long) g_heap_pointer;
	last_size = 0;


	//Check Heap Pointer.
    if ( g_heap_pointer == 0 )
    {
	    printf ("init_heap fail: Heap pointer\n");
		goto fail;
    }


	//Check Heap Pointer overflow.
    if( g_heap_pointer > kernel_heap_end )
    {
        printf ("init_heap fail: Heap Pointer Overflow\n");
		goto fail;
    }


    //Heap Start.
    if ( kernel_heap_start == 0 )
    {
	    printf("init_heap fail: HeapStart={%x}\n", kernel_heap_start );
	    goto fail;
    }


	//Heap End.
    if ( kernel_heap_end == 0 )
    {
	    printf("init_heap fail: HeapEnd={%x}\n", kernel_heap_end );
	    goto fail;
    }


	//Check available heap.
    if ( g_available_heap == 0 )
    {
	    //@todo: Tentar crescer o heap.
		
		printf("init_heap fail: Available heap\n");
		goto fail;
    }


	// Heap list:
	// Inicializa a lista de heaps.

    while ( i < HEAP_COUNT_MAX )
    {
        heapList[i] = (unsigned long) 0;
        i++;
    };


	//KernelHeap = (void*) x??;

	//More?!

// Done.

#ifdef MK_VERBOSE
    printf ("Done\n");
#endif

    return 0;


	// Fail. 
	// Falha ao iniciar o heap do kernel.

fail:
    printf ("init_heap: Fail\n");
    refresh_screen ();


	/*
	// #debug
	printf("* Debug: %x %x %x %x \n", kernel_heap_start, 
	                                  kernel_heap_end,
									  kernel_stack_start,
									  kernel_stack_end);	
	refresh_screen();	
    while(1){}
	*/


    return (int) 1;
}


/*
 **************************************************
 * init_stack:
 *     Iniciar a ger�ncia de Stack do kernel. 
 *     @todo Usar stackInit(). 
 */
 
int init_stack (void){

    //Globals.
	//#bugbug


    kernel_stack_end = (unsigned long) KERNEL_STACK_END; 
    kernel_stack_start = (unsigned long) KERNEL_STACK_START; 


    //End.
    if ( kernel_stack_end == 0 )
    {
	    printf ("init_stack: fail StackEnd={%x}\n", kernel_stack_end );
	    goto fail;
    }


	//Start.
    if ( kernel_stack_start == 0 )
    {
	    printf ("init_stack: fail StackStart={%x}\n", kernel_stack_start );
	    goto fail;
    }


	// Done.

    return 0;

fail:
    return (int) 1;
}


/*
 ******************************************
 * init_mm:
 *   Inicializa o memory manager.
 *    @todo: Usar mmInit().
 */
 
int init_mm (void){

    int Status = 0;
    int i = 0;

	
	// @todo: 
	// Inicializar algumas vari�veis globais.
	// Chamar os construtores para inicializar o b�sico.
	

	// @todo: 
	// Clear BSS.
	// Criar mmClearBSS()
	

	//#importante:
	//Inicializa heap e stack.
	
	Status = (int) init_heap ();
	
	if (Status != 0)
	{
	    printf("init_mm fail: Heap\n");
	    return (int) 1;
	};	
	
	
	Status = (int) init_stack ();
	
	if (Status != 0)
	{
	    printf("init_mm fail: Stack\n");
	    return (int) 1;
	};

	
	// Zerar a lista.
	// Lista de blocos de mem�ria dentro do heap do kernel.
	
	while ( i < MMBLOCK_COUNT_MAX )
	{
        mmblockList[i] = (unsigned long) 0;
		
		i++;
    };
	
	//Primeiro Bloco.
    //current_mmblock = (void *) NULL;
	
	//#importante:
	//#inicializando o �ndice la lista de ponteiros 
	//par estruturas de aloca��o.
	
	mmblockCount = 0;
	
	
	//
	// MEMORY SIZES
	//
	
	// Get memory sizes via RTC. (KB)
	// base, other, extended.
	// RTC s� consegue perceber 64MB de mem�ria.
	
    memorysizeBaseMemory = (unsigned long) rtcGetBaseMemory();  
    memorysizeOtherMemory = (unsigned long) (1024 - memorysizeBaseMemory);

    // #todo
    // New we have a new value from boot.
    // We're gonna use this new value instead the one from cmos.

    unsigned long __total_memory_in_kb = (blSavedLastValidAddress/0x400);

    // extended memory from cmos.
    //memorysizeExtendedMemory = (unsigned long) rtcGetExtendedMemory(); 
    memorysizeExtendedMemory =  (__total_memory_in_kb - memorysizeBaseMemory - memorysizeOtherMemory);

    memorysizeTotal = (unsigned long) ( memorysizeBaseMemory + memorysizeOtherMemory + memorysizeExtendedMemory );


	// #IMPORTANTE 
	// Determinar o tipo de sistema de mem�ria.
	// small   pelo menos 32mb
	// medium  pelo menos 64mb
	// large   pelo menos 128mb
	
	//0MB
	if ( memorysizeTotal >= (0) )
	{
		g_mm_system_type = stNull;
	    
		// #aten��o 
		// Nesse caso devemos prosseguir e testar as outras op��es.		
	}	
	
	
	//32MB
	//(32*1024)
	if ( memorysizeTotal >= SMALLSYSTEM_SIZE_KB )   
	{
		g_mm_system_type = stSmallSystem;
	}
	
	
	//64MB
	//(64*1024)
	if ( memorysizeTotal >= MEDIUMSYSTEM_SIZE_KB )  
	{
		g_mm_system_type = stMediumSystem;
	}	
	
	
	//128MB
	//(128*1024)
	if ( memorysizeTotal >= LARGESYSTEM_SIZE_KB )  
	{
		g_mm_system_type = stLargeSystem;
	}		
	

	
	//inicializando o framepool (paged pool)
	
	initializeFramesAlloc ();
	
	
	// Continua...

    return (int) Status;
}


//
// Segue rotinas de GC.
// ==================================================
//

//limpa a camada /gramado
int gcGRAMADO (void)
{	
	//Ainda n�o implementado.
	return 0;
}


/*
 gcEXECUTIVE:
 
    Limpa a camada /execve.
    
	Esse � o Garbage Collector, o trabalho dele � checar nas listas 
 de ponteiros de estrutura e encontrar estruturas sinalizadas para 
 serem liberadas. 
 
    Quando encontra uma estrutura sinalizada, libera os recurso da 
estrutura.
   
   --------

    Obs:
        #importante: 
        As estruturas poder�o ser deletadas ou n�o.
  
    Obs: 
	    A �rea do cliente na estrutura mmblock dever� se preenchida com zero
        quando a estrutura estiver sinalizada como reutiliz�vel.
        
	Obs:	
		O garbage collector � um servi�o do kernel, e poder� ser 
	    chamado por interrup��o. Pois utilit�rios de ger�ncia de mem�ria 
	    usar�o recursos de ger�ncia de mem�ria oferecidos pelo kernel.
		
    Obs: 
	    A fun��o gc() poder� ser chamada de tempos em tempos, do mesmo modo 
        que o dead thread collector.
		#todo: Resta encontrarmos quais s�o os momentos em que essas rotinas 
		s�o chamadas.
	   
    Obs: 
        #Importante:
		O GC deve efetuar apenas uma opera��o de libera��o, mesmo que 
        haja muita coisa pra fazer, pois n�o queremos que ele use 
		muito tempo de processamento prejudicando os outros processos.
		Pois bem, ele efetua apenas uma opera��o de limpeza e sai sem erro.
		Se n�o encontrar nada pra fazer, tamb�m sai sem erro.
		
		@todo: Cria os 'for's para as outras listas.
		@todo: Criar as rotinas de limpeza para as outras listas.
		 
	
	#### #todo: #### 
	
	#importante:
	+Checar as etruturas de mmblock e liberar as estruturas marcadas como Free=1.
	 ?? Para isso deve haver um array mmblockList[].
	
	+Checar as estruturas de janela e liberar as estiverem marcadas 
	 como used=216 e magic=4321.

	*Importante:
	Essa rotina deve limpar aloca��es de mem�ria e n�o estruturas de 
	objetos de outros tipos.
	+O m�dulo de ger�ncia de recursos gr�ficos que limpe suas listas.
	+O m�dulo de ger�ncia de processos que limpe suas listas.
    +O m�dulo de ger�ncia de threads que limpe suas listas.
 */

int gcEXECUTIVE (void){
	
	int i;
	struct mmblock_d *b;  //memory block.
	struct heap_d *h;     //heap.
	//...
	
	//Obs: 
	// Importante: Cada lista cont�m um tipo de estrutura.
	// Importante: Limparemos somente as listas que pertencem ao m�dulo /ram
	
	
	//#importante:
	//mmblock_d � a estrutura usada pelo kmalloc para organizar as aloca��es 
	//dentro de um heap. 
	//Essa estrutura ser� sinalizada com Free=1 e precisa liberar a �rea do cliente.
	
	//mmblockList[]
	//#importante: Nessa lista tem ponteiros para uma estrutura especial,
	//usada pela kmalloc para organizar os blocos de mem�ria que ser�o utilizados 
	//para aloca��o din�mica.

    for ( i=0; i<MMBLOCK_COUNT_MAX; i++ )
    {
	    b = (void *) mmblockList[i];
		
		//ponteiro v�lido.
	    if( (void *) b != NULL )
		{
			//sinalizado para o GC.
			if( b->Used == 216 && b->Magic == 4321 && b->Free == 1 )
			{
				goto clear_mmblock;
			}
		}
    };
	
	
	//heapList[]
	//Limpar a lista de heaps.
	//Existir�o v�rios heaps que poder�o ser usados pelos alocadores.
	//Essa lista tem o pnteiros para heaps.

    for ( i=0; i<HEAP_COUNT_MAX; i++ )
    {
	    h = (void *) heapList[i];
		
		//ponteiro v�lido.
	    if ( (void *) h != NULL )
		{
			//sinalizado para o GC.
			if( h->used == 216 && h->magic == 4321 )
			{
				goto clear_heap;
			}
		}
    };


	//
	// #### #importante ####
	// @todo: Cria os 'for's para as outras listas.
	// ex: session, window station, desktop, window etc ...

	
    goto done;
	
	//
	// Segue opera��es de limpeza em estruturas de tipos diferentes.
	// Devemos limpar e sairmos em seguida.
	//
	
	//Nothing
	
clear_mmblock:	
    
	if ( (void *) b != NULL )
    {
		//Checar se a �rea alocada est� dentro dos limites.
	    //O inicio da �rea mais o tamanho dela tem que coincidir 
		//com o footer.
		if ( (b->userArea + b->userareaSize) != b->Footer )
		{
			//#debug
			//printf("gc fail: User Area Size");
			//goto fail;
			
			return (int) 1;
		
		}else{
			
	        //preenche com zeros a �rea do cliente.
			bzero ( (char *) b->userArea, (int) b->userareaSize );
		
		};
		
        //Nothing.		
	};
	goto done;
	//Nothing.
	
clear_heap:

    if ( (void *) h != NULL )
	{
		// ?? O que fazer aqui ??
		
		//Limparemos mas n�o deletaremos.
		//h->used  = 1;
		//h->magic = 1234;
		
		//#todo: 
		//Por enquanto vamos desabilitar a estrutura cancelada 
		//pelo sistema.
		if ( h->used == 216 && h->magic == 4321 )
		{
			h->used = 0;
			h->magic = 0;
			h = NULL;
		}
		
		//lista encadeada de blocos que formavam o heap.
		//podemos apenas sinalizar todos os mmblocks dessa lista e depois o GC acaba com eles.
		//para isso precisamos de um 'for'.
		//h->mmblockListHead = NULL;		
	};
	goto done;
	//Nothing.

	//
	// #todo: 
	// Criar as rotinas de limpeza para as outras listas.
	//


fail:
    refresh_screen();
    return (int) 1;

done:
    return 0;
}


//limpa a camada /microkernel
int gcMICROKERNEL (void)
{
    return 0;
}


//limpa a camada /hal
int gcHAL (void)
{
    return 0;
}


/*
 ******************************
 * gc:
 *     Garbage Collector.     
 *	   
 * Call all Garbage Collections rotines.
 * Clear all main layers.
 *	
 * +GRAMADO 
 * +EXECUTIVE 
 * +MICROKERNEL 
 * +HAL
 *
 * #importante:
 * Em duas condi��es as estruturas poder�o ser destruidas ou reaproveitadas: 
 * +Quando a estrutura usada pelo kmalloc estiver sinalizada com Free=1 e 
 * +quando as outras estruturas estiverem com a flag igual a used=216 e 
 * magic=4321.
 * obs: Outros tratamentos de flags ser�o introduzidos com o tempo,
 * #todo: criaremos um enum de flags para serem utilizadas.
 *
 * #importante:
 * Para n�o perdermos muito tempo com a rotina de limpeza devemos limpar
 * apenas uma estrutura e sa�rmos.
 * #importante: ?? Quando essa rotina � chamada, pois ela s� deve ser 
 * chamada de tempos em tempos, provavelmente usando a flag 'extra' que 
 * est� em ts.c.
 * #por enquanto ainda n�o chamamos isso, apenas temos o servi�o para 
 * utilit�rios usarem.
 */
 
int gc (void){
	
    int Status;

//clearGramadoLayer:

    Status = (int) gcGRAMADO ();
    if ( Status == 1 )
	{
		//#debug
		//printf("gc: clearGramadoLayer:\n");
		//goto fail;
		
		return (int) 1;
	}
	
//clearExecutiveLayer:

    Status = (int) gcEXECUTIVE ();
    if ( Status == 1 )
	{
		//#debug
		//printf("gc: clearExecutiveLayer:\n");
		//goto fail;
		
		return (int) 1;
	}
	
//clearMicrokernelLayer:

    Status = (int) gcMICROKERNEL ();
    if ( Status == 1 )
	{
		//#debug
		//printf("gc: clearMicrokernelLayer:\n");
		//goto fail;
		
		return (int) 1;
	}
	
//clearHalLayer:

    Status = (int) gcHAL ();
    if ( Status == 1 )
	{
		//#debug
		//printf("gc: clearHalLayer:\n");
		//goto fail;
	    
		return (int) 1;
	}

	// Done.

    return 0;
}


/*
int mmInit()
{
    //...	
}
*/


//
// End.
//
