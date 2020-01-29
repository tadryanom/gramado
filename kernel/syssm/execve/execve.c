/*
 * File: execve.c 
 * 
 * Gramado Executive - The main file for the Executive module in the 
 * kernel base. 
 * 
 * (c) Copyright 2015-2019 Fred Nora.
 */
 
 
#include <kernel.h>
//#include "eidata.h"  //Executive Internal Data.


//unsigned char  EcecutiveName[] = "EXECUTIVE LAYER";


/*
 *****************************************************************
 * do_gexecve:
 *
 *     Executa um programa no processo INIT dentro do ambiente 
 * Gramado Core. 
 *
 *     Ou seja, na thread prim�ria do processo INIT.
 *     Ou seja, a aplica��o tem que ser compilada no mesmo endere�o 
 * do INIT.
 *     #obs: Isso funcionou.
 *    
 *     >> Na verdade a thread fica em standby.
 *
 * #bugbug:
 * E se o aplicativo fechar a thread, como usaremos a thread do 
 * processo init? 
 * Talvez a solu��o para isso seria criar outra. 
 * Nos casos em que o aplicativo chama o exit, n�o est� retornando 
 * para o shell. O fato � que quando chama o exit o estado da 
 * thread muda.
 *
 * Obs: � dever dessa rotina colocar a thread em estado standby,
 * onde ela fica selecionada para execuss�o, 
 * pois um exit pode deixar o estado da thread reaproveitada 
 * de um jeito que n�o rode.
 *
 * IN:
 * servi�o, file name, arg, env.
 */

// #todo
// Atualizar o nome do processo na estrutura de processo.

int 
do_gexecve ( int i,
             const char *arg1, 
             const char *arg2, 
             const char *arg3 )
{
    int Status = 1;    // fail.

	//??
	//Esse � o primeiro argumento.
    int Plane = 0;

    char *s;
    struct thread_d *Thread;

	// Usados gerenciamento de arquivo.

    size_t l;                        //lenght.
    char bin_string[] = ".bin";
     //char bin2_string[] = ".BIN";

	
	//#debug
	//printf("0=%s ",&argv[0]);
	//printf("1=%s ",&argv[1]);


	// #importante
	// Testando carregar um programa para rodar no processo INIT, 
	// usando a thread prim�ria do processo !
	// � o mesmo que consierar que o processo INIT j� seja o clone 
	// de outro v�lido.
		
    //??
	//array de ponteiros.

    unsigned long *p = (unsigned long *) arg2;

	// #importante:
	// Mem�ria compartilhada entre o kernel e o aplicativo.
	// O aplicativo vai ler esse trem 

    unsigned char *shared_memory = (unsigned char *) (0xC0800000 - 0x100);


	// #IMPORTANTE:
	// PRECISAMOS ENVIAR A MENSAGEM SOMENTE DEPOIS QUE O NOVO 
	// PROGRAMA FOR COLOCADO NA MEM�RIA, SEN�O AO COLOCAR O PROGRAMA 
	// NA MEM�RIA A MENSAGEM SER� SOBRESCRITA.
	// #TODO: 
	// CRIAR UM MECANISMO DE TROCA DE MENSAGENS MAIS EFICIENTE,
	// BASEADO NESSE.


	//=================================================
	//  ## CMD ##

	//#importante.
	//antes de tudo vamos testar o comando.
	//se ele n�o existir ent�o nem vamos mexer na estrutura da trhead.
	//se n�o mexermos na estrutura da thread ele continuar� presa 
	// no while do exit da libc.


	//
	// ==== Load file ====
	//
	
	// #bugbug
	// # arg1=name ##
	
	// Devemos ver se a string n�o passa dos limites.

	// Como essa rotina � para executar um arquivo .bin,
	// caso n�o exista uma extens�o .bin e o nome seja menor que 8, 
	// podemos adicionar a extens�o .bin.

    l = (size_t) strlen ( (char *) arg1 );

    if ( l > 11 )
    {
		// #fail 
        printf ("do_gexecve: l fail\n");
		// Obs: 
		// N�o sairemos da fun��o pois isso � um teste ainda.
		// goto fail;
		
    }else{

		// Se n�o existe um ponto entre os oito primeiros chars,
		// ent�o colocamos a extens�o .bin logo ap�s o nome passado.
		// Ele � pelo menos menor que 11, mas deveria ser menor que oito.
		
            if ( arg1[0] != '.' && 
                 arg1[1] != '.' && 
                 arg1[2] != '.' && 
                 arg1[3] != '.' && 
                 arg1[4] != '.' && 
                 arg1[5] != '.' && 
                 arg1[6] != '.' && 
                 arg1[7] != '.' )
            {       
                if ( l > 8 )
                {
                    printf ("do_gexecve: File without ext is too long\n");
					// Obs: 
					// N�o sairemos da fun��o pois isso � um teste ainda.
					// goto fail;
				}

                strcat ( (char *) arg1, (const char *) bin_string );
            };

			// #obs:
			// Se estamos aqui, isso significa existe um ponto 
            // nos primeiros oito bytes.
            // Ainda n�o sabemos se todo o nome do arquivo est� certo,
            // mas ja sabemos que n�o precisamos incluir uma exten��o.
        };



    // #importante
	// Transformando o nome do arquivo em mai�scula pos estamos
	// usando FAT16, que exige isso.

	read_fntos ( (char *) arg1 );
	
	//
	// Load file.
	//
	
	// #importante:
	// Carregaremos o programa no endere�o l�gico 0x400000, usando o
	// diret�rio de p�ginas do processo kernel.
	
	// #todo:
	// Nas rotinas de execu��o, temos que usar o diretorio de p�ginas
	// do processo que est� solicitando o carregamento e execu��o.
	// Ou carregarmos em um endere�o f�sico e mapearmos no
	// diret�rio de p�ginas do processo na posi��o 0x400000.


    Status = (int) fsLoadFile ( VOLUME1_FAT_ADDRESS, 
                       VOLUME1_ROOTDIR_ADDRESS, 
                       (unsigned char *) arg1, 
                       (unsigned long) 0x00400000 );

    if ( Status == 1 )
    {
        printf ("do_gexecve: Couldn't load file\n");
        goto fail;
    }


	// Check ELF signature.
	// OK. O comando existe e o arquivo foi carregado, mas 
	// precisamos saber se a assinatura de ELF � v�lida.

    Status = (int) fsCheckELFFile ( (unsigned long) 0x00400000 );

    if ( Status == 0 )
    {
        goto format_ok;
    }else{

		// #debug
        panic ("do_gexecve: It's not a valid ELF file\n");
        //goto fail;
    };


	//
	// ELF Signature OK
	//


format_ok:

	//#debug
	//tentando receber uma linha de ocmando inteira.
	//printf("\nexecutive_gramado_core_init_execve: testing..\n\n");
	
	//# ISSO DEU CERTO #
	//testando se o shell transferiu toda alinha de comandos para 
	//a mem�ria compartilhada.
	//printf(">>>cmdline2={%s}\n", shared_memory);
	
	
	//#IMPORTANTE:
	//se a linha de comandos est� na mem�ria compartilhada 
	//e o nome do arquivo programa foi passado via endere�o 
	//ent�o temos tudo o que � preciso 
	//para enviarmos a linha de comandos para o aplicativo.
	
	//...
	
	
	//## teste
	//
	//if( ! strcmp( (char*)argv[0], "-f" ) ) 
	//if( strncmp( (char*) &argv[1], "-f", 2 ) == 0 )
	//{
	//	printf("executive_gramado_core_init_execve: FOREGROUND\n");
    //    Plane = FOREGROUND;
    //}else{
	//	printf("executive_gramado_core_init_execve: BACKGROUND\n");
	//	Plane = BACKGROUND;
	//};
	
	//fail.
	//if( (const char *) filename == NULL ){
	//	return 1;
	//}
	
	//
	// ENVIANDO A MENSAGEM
	//
	
	//unsigned longs.
	//for( i=0; i<512; i++ )
	//{
        
		//N�o queremos transferir o primeiro ponteiro 
		//pois ele � o nome do programa e n�o um argumento.
	//	shared_p[i] = p[i+1];
		
		//pipe[i] = src[i];
		//shared_memory[i] = src[i];
	//};
	
	//os ponteiros est�o na mem�ria compartilhada, 
	//mas as strings est�o onde ??
	//provavelmente as strings ainda esteja na mem�ria 
	//do shell, e o aplicativo n�o pode ler as strings que est�o 
	//na mem�ria do shell.
	//obs: o shell poderia copiar toda a linha de comando para 
	//a mem�ria compartilhada.
	//printf(">>>shared_p0={%s}\n"     ,shared_p[0]);
	//printf(">>>shared_p1={%s}\n"     ,shared_p[1]);
	//printf(">>>shared_p2={%s}\n\n"   ,shared_p[2]);
	//printf(">>>shared_p3={%s}\n\n"   ,shared_p[3]);
	
 
	//#debug
	//ok. isso funcionou.
	//printf("Showpipe={%s}\n",pipe);
	//printf("Showsharedmemory={%s}\n",shared_memory);
	
	
	// Pegar o ponteiro da thread prim�ria do processo INIT.
    // o ponteiro vai continuar existindo mesmo que o deadthread collector 
	// tenha destruido a estrutura depois de um app mudar o estado para zombie 
	// por causa de um exit.
	//Ent�o � melhor criarmos uma thread nova. Mas se fizermos isso
	//sem antes o deadthread collector ter destruido a estrutua e liberado 
	//a mem�ria ent�o a antiga estrutura de thread ficar� sem porteiro e 
	//poderemos desalocar a mem�ria usanda, ou pior, se n�o mudarmos o 
	//status ele pode querer destruir a que estamos criando ent�o s� 
	//criaremos se o ponteiro for NULL, significando que o deadthread collector 
	//ja destruiu a estrutura e aproveitou a mem�ria se conseguiu.

	//#importante:
	//Podemos eswperar que essa thread esteja no estado ZOMBIE.
	//Pois um aplicativo pode ter efetuado um exit.
	//se ela estiver no estado ZOMBIE
	
	// #importante
	// Esperamos que tenha limpado InitThread antes de usarmos o ponteiro.
	// Isso � trabalho do exit e do deadthread collector.
	
	// #bugbug
	// Esse ponteiro pode dar problemas.


    if ( i == 216 )
    {
        printf ("do_gexecve: ClonedThread \n");
        Thread = (struct thread_d *) ClonedThread;

	}else{

        printf ("do_gexecve: InitThread \n");
        Thread = (struct thread_d *) InitThread;
    };


    if ( (void *) Thread == NULL )
    {
        panic ("do_gexecve: Thread Fail \n");
		//goto fail;
    }else{

		// #importante:
		// Checar a flag de reaproveitamento.
		// Se a flag do reaproveitamento falhar ent�o o exit no kernel
		// n�o acionou ela para a threa InitThread, que � nossa �nica reaproveit�vel.
		// por enquanto.
		
		//if ( Thread->used != 1 || Thread->magic != 1234 )
		//{
		//	printf("executive_gramado_core_init_execve: Validation fail \n");
		//	die();
			//goto fail;
		//}


		//
		// ## state ##
		//

		// Vamos seguir a sequ�ncia de nacimento de um thread e 
		// cancelaremos, caso n�o for poss�vel carregar o arquivo do programa.
		// INITIALIZED >> STANDBY >> READY >> RUNNING ...

        Thread->state = INITIALIZED;

		// '0'. Significa que o contexto nunca foi salvo, pois o spawn 
		// n�o funciona em thread com o contexto salvo.

        Thread->saved = 0; 

		// ??
		// Plano. bg/fg.

        Thread->plane = Plane;


		//#test
		// Vamos associar ao primeiro tty, mesmo que seja um aplicatibo GUI.
		// Se ele for um aplicativo GUI ele ir� atualizar o foco.
		// Se for um aplicativo de terminal ent�o ter� uma janela 
		// para rodar. Pois o ldisc manda mensagens para a thread de controle 
		// da janela com foco de entrada. Vamos fazer isso manualmente.

        if ( (void *) CurrentTTY != NULL )
        {
            if ( CurrentTTY->used == 1 && CurrentTTY->magic == 1234 )
            {
                current_tty = CurrentTTY->index;

                Thread->tty_id = current_tty;
                
				// #terminal window.
                window_with_focus = CurrentTTY->window->id;
                terminal_window = CurrentTTY->window->id;

				//#importante
				//a thread de controle da janela, para qual
				//ser�o enviadas as mensagens pelo ldisc
                CurrentTTY->window->control = Thread;
            }

        }else{
            Thread->tty_id = -1;
        };


		// Context.
		// #todo: 
		// Isso deve ser uma estrutura de contexto.


		// Stack frame.

        Thread->ss = 0x23; 
        Thread->esp = (unsigned long) 0x0044FFF0; 
        Thread->eflags = 0x3200; 
        Thread->cs = 0x1B; 
        Thread->eip = (unsigned long) 0x00401000; 


		// Segment registers.

        Thread->ds = 0x23; 
        Thread->es = 0x23; 
        Thread->fs = 0x23; 
        Thread->gs = 0x23; 

		// Outros.

        Thread->eax = 0;
        Thread->ebx = 0;
        Thread->ecx = 0;
        Thread->edx = 0;
        Thread->esi = 0;
        Thread->edi = 0;
        Thread->ebp = 0;


		// Next thread.

        Thread->Next = NULL;

		// ?
		// Thread queue.

        queue_insert_data ( queue, 
            (unsigned long) Thread, 
            QUEUE_INITIALIZED );


		// #importante:
		// * MOVEMENT 1 ( Initialized ---> Standby ).
		// Isso permitira que o taskswitch selecione ela pra rodar.

        SelectForExecution (Thread); 

        goto done;
    };

	// fail

fail:

    printf ("do_gexecve: Fail\n");

	// #debug
	// refresh_screen ();

done:

	//#debug
	//printf(">>>shared_p0={%s}\n"     ,shared_p[0]);
	//printf(">>>shared_p1={%s}\n"     ,shared_p[1]);
	//printf(">>>shared_p2={%s}\n\n"   ,shared_p[2]);
	//printf(">>>shared_p3={%s}\n\n"   ,shared_p[3]);
	//refresh_screen();
	//while(1){ asm("hlt"); }


	//#debug
	//printf("done\n");

	// #Obs: 
	// +N�o devemos emitir mensagens no caso de acerto.
	// +refresh_screen s� no caso de erro.

	// #bugbug
	// #obs: 
	// Estamos usando isso s� por enquanto para debug.

    refresh_screen ();

    return (int) Status;
}


/*
 *******************************************
 * do_execve:
 *     Efetua o servi�o execve, rodando um novo programa no 
 * processo atual.
 *     T� usando a thread atual e transformando ela em 
 * thread de controle.
 * 
 *     #todo: 
 *     Melhorar a descri��o do objetivo dessa rotina.
 * 
 *     args:  ?(servi�o), name, arg, env.
 */
 
// #Aten��o
// Isso foi chamado depois de um fork()

// #bugbug
// Isso funciona as vezes e em outra falha. 

int 
do_execve ( int i, 
            const char *arg1, 
            const char *arg2, 
            const char *arg3 ) 
{
    int Status = 1;    // fail.

    struct process_d *process;
    struct thread_d *Thread;

	//??
	//Esse � o primeiro argumento.
    int Plane = 0;

    char *s;

	// Usados gerenciamento de arquivo.

    size_t l;                        //lenght.
    char bin_string[] = ".bin";
    //char bin2_string[] = ".BIN";


	//#debug
	//printf("0=%s ",&argv[0]);
	//printf("1=%s ",&argv[1]);


	// #importante
	// Testando carregar um programa para rodar no processo INIT, 
	// usando a thread prim�ria do processo !
	// � o mesmo que consierar que o processo INIT j� seja o clone 
	// de outro v�lido.

	//??
	//array de ponteiros.
    unsigned long *p = (unsigned long *) arg2;

	// #importante:
	// Mem�ria compartilhada entre o kernel e o aplicativo.
	// O aplicativo vai ler esse trem 
    unsigned char *shared_memory = (unsigned char *) (0xC0800000 - 0x100);

	// #IMPORTANTE:
	// PRECISAMOS ENVIAR A MENSAGEM SOMENTE DEPOIS QUE O NOVO PROGRAMA FOR 
	// COLOCADO NA MEM�RIA, SEN�O AO COLOCAR O PROGRAMA NA MEM�RIA A MENSAGEM 
	// SER� SOBRESCRITA.
	// #TODO: CRIAR UM MECANISMO DE TROCA DE MENSAGENS MAIS EFICIENTE,
	// BASEADO NESSE.

	//=================================================
	//  ## CMD ##

	//#importante.
	//antes de tudo vamos testar o comando.
	//se ele n�o existir ent�o nem vamos mexer na estrutura da trhead.
	//se n�o mexermos na estrutura da thread ele continuar� presa no while 
	//do exit da libc.
	
	
	//
	// ## Load file ##
	//
	
	// #bugbug
	// # arg1=name ##
	
	// Devemos ver se a string n�o passa dos limites.

	// Como essa rotina � para executar um arquivo .bin,
	// caso n�o exista uma extens�o .bin e o nome seja menor que 8, 
	// podemos adicionar a extens�o .bin.



    l = (size_t) strlen ( (char *) arg1 ); //ok
    //l = (size_t) strlen ( (char *) &arg1[0] );
    
	printf ("filename %s   \n", &arg1[0] );
	printf ("name size %d  \n", l );

    // O tamanho m�ximo � 12.
    // '8' '.' '3'

    if ( l > 12 )
    {
		printf ("name size %d  \n", l );
        panic ("do_execve: l. The filename is too long ! \n");
        
		// #obs: 
		// N�o sairemos da fun��o pois isso � um teste ainda.
		// goto fail;

    }
    
    // 
    if (l < 9)
    {
		// Se n�o existe um ponto entre os oito primeiros chars,
		// ent�o colocamos a extens�o .bin logo ap�s o nome passado.
		// Ele � pelo menos menor que 11, mas deveria ser menor que oito.

            if ( arg1[0] != '.' && 
                 arg1[1] != '.' && 
                 arg1[2] != '.' && 
                 arg1[3] != '.' && 
                 arg1[4] != '.' && 
                 arg1[5] != '.' && 
                 arg1[6] != '.' && 
                 arg1[7] != '.' )
            {       
                if ( l > 8 )
                {
                    panic ("do_execve: File without ext is too long\n");
                    
					// Obs: 
					// N�o sairemos da fun��o pois isso � um teste ainda.
					// goto fail;
                }

                strcat ( (char *) arg1, (const char *) bin_string );
            };

			// #obs:
			// Se estamos aqui, isso significa existe um ponto 
			// nos primeiros oito bytes.
			// Ainda n�o sabemos se todo o nome do arquivo est� certo,
			// mas ja sabemos que n�o precisamos incluir uma exten��o.
    };


	// #importante
	// Transformando o nome do arquivo em mai�scula, pois estamos 
	// usando FAT16, que exige isso.

    read_fntos ( (char *) arg1 );

	//
	// Load file.
	//

	// #importante:
	// Carregaremos o programa no endere�o l�gico 0x400000, usando o
	// diret�rio de p�ginas do processo kernel.

	// #todo:
	// Nas rotinas de execu��o, temos que usar o diretorio de p�ginas
	// do processo que est� solicitando o carregamento e execu��o.
	// Ou carregarmos em um endere�o f�sico e mapearmos no
	// diret�rio de p�ginas do processo na posi��o 0x400000.

	// #bugbug
	// Esse carregamento � feito usando o diret�rio de p�ginas do kernel,
	// mas o certo � usarmos o diret�rio de p�ginas do processo atual.
	// A quest�o � que sempre carregaremos em endere�os f�sicos diferentes,
	// mas todo diret�rio de p�ginas ter� o mesmo endere�o l�gico.
	// Essa rotina de carregamento tem que usar o endere�o l�gico
	// referente ao endere�o f�sico desejado e esse endere�o l�gico
	// deve pertencer ao diret�rio de p�ginas do kernel.

    process = (struct process_d *) processList[current_process];

    // #todo
    // Precisamos checar a validade da estrutura.

    // #todo
    // Vamos tentar atualizar o nome do processo na estrutura
    // de processo.

    //memcpy (process->name_address, arg1, 11);
    //memcpy (process->name, arg1, 11);
    //process->name = (char *) arg1; 

    Status = (int) fsLoadFile ( VOLUME1_FAT_ADDRESS, 
                       VOLUME1_ROOTDIR_ADDRESS, 
                       (unsigned char *) arg1, 
                       (unsigned long) process->Image );

    if ( Status == 1 )
    {
        printf ("do_execve: Couldn't load file\n");
        goto fail;
    }

	// Check ELF signature.
	// OK. O comando existe e o arquivo foi carregado, mas 
	// precisamos saber se a assinatura de ELF � v�lida.

    Status = (int) fsCheckELFFile ( (unsigned long) process->Image );

    if ( Status == 0 )
    {
        goto format_ok;
    }else{

		// #debug
		panic ("do_execve: It's not a valid ELF file\n");
		//goto fail;
    };


	//
	// ELF Signature OK
	//

format_ok:

	//#debug
	//tentando receber uma linha de ocmando inteira.
	//printf("\nexecutive_gramado_core_init_execve: testing..\n\n");

	// # ISSO DEU CERTO #
	// testando se o shell transferiu toda alinha de comandos para a 
	// mem�ria compartilhada.
	//printf(">>>cmdline2={%s}\n", shared_memory);
	
	//#IMPORTANTE:
	//se a linha de comandos est� na mem�ria compartilhada 
	//e o nome do arquivo programa foi passado via endere�o 
	//ent�o temos tudo o que � preciso 
	//para enviarmos a linha de comandos para o aplicativo.

	//...
	
	
	//## teste
	//
	//if( ! strcmp( (char*)argv[0], "-f" ) ) 
	//if( strncmp( (char*) &argv[1], "-f", 2 ) == 0 )
	//{
	//	printf("executive_gramado_core_init_execve: FOREGROUND\n");
    //    Plane = FOREGROUND;
    //}else{
	//	printf("executive_gramado_core_init_execve: BACKGROUND\n");
	//	Plane = BACKGROUND;
	//};
	
	//fail.
	//if( (const char *) filename == NULL ){
	//	return 1;
	//}
	
	//
	// ENVIANDO A MENSAGEM
	//
	
	//unsigned longs.
	//for( i=0; i<512; i++ )
	//{
        
		//N�o queremos transferir o primeiro ponteiro 
		//pois ele � o nome do programa e n�o um argumento.
	//	shared_p[i] = p[i+1];
		
		//pipe[i] = src[i];
		//shared_memory[i] = src[i];
	//};
	
	//os ponteiros est�o na mem�ria compartilhada, 
	//mas as strings est�o onde ??
	//provavelmente as strings ainda esteja na mem�ria 
	//do shell, e o aplicativo n�o pode ler as strings que est�o 
	//na mem�ria do shell.
	//obs: o shell poderia copiar toda a linha de comando para 
	//a mem�ria compartilhada.
	//printf(">>>shared_p0={%s}\n"     ,shared_p[0]);
	//printf(">>>shared_p1={%s}\n"     ,shared_p[1]);
	//printf(">>>shared_p2={%s}\n\n"   ,shared_p[2]);
	//printf(">>>shared_p3={%s}\n\n"   ,shared_p[3]);
	
 
	//#debug
	//ok. isso funcionou.
	//printf("Showpipe={%s}\n",pipe);
	//printf("Showsharedmemory={%s}\n",shared_memory);
	
	
	// Pegar o ponteiro da thread prim�ria do processo INIT.
    // o ponteiro vai continuar existindo mesmo que o deadthread collector 
	// tenha destruido a estrutura depois de um app mudar o estado para zombie 
	// por causa de um exit.
	//Ent�o � melhor criarmos uma thread nova. Mas se fizermos isso
	//sem antes o deadthread collector ter destruido a estrutua e 
	// liberado a mem�ria 
	//ent�o a antiga estrutura de thread ficar� sem porteiro e poderemos 
	//desalocar a mem�ria 
	//usanda, ou pior, se n�o mudarmos o status ele pode querer destruir a 
	//que estamos criando
	//ent�o s� criaremos se o ponteiro for NULL, significando que o 
	// deadthread collector 
	//ja destruiu a estrutura e aproveitou a mem�ria se conseguiu.

	//#importante:
	//Podemos eswperar que essa thread esteja no estado ZOMBIE.
	//Pois um aplicativo pode ter efetuado um exit.
	//se ela estiver no estado ZOMBIE
	
	// #importante
	// Esperamos que tenha limpado InitThread antes de usarmos o ponteiro.
	// Isso � trabalho do exit e do deadthread collector.
	
	// #bugbug
	// Esse ponteiro ode dar problemas.
	
	/*
	if ( i == 216 )
	{
		printf ("do_execve: ClonedThread \n");
	    Thread = (struct thread_d *) ClonedThread; 
	}else{
		printf ("do_execve: InitThread \n");
	    Thread = (struct thread_d *) InitThread;
	}
    */




	//
	// Current Thread.
	//

	// #obs
	// A thread que chamou essa rotina deve ser a current_thread. 
	// Certo ?
	// +N�o podemos retornar para ela ap�s essa chamada. 
	// +Devemos reinicializar a thread ou fazermos um spawn.
	// #bugbug: Perderemos a pilha salva na chamada.
	// #bugbug: Se retornarmos teremos problema pois a thread foi 
	// alterada.
	// S� nos resta reinicializarmos a thread atual e rodarmos ela.

    Thread = (struct thread_d *) threadList[current_thread];

    if ( (void *) Thread == NULL )
    {
        panic ("do_execve: Thread fail\n");
		//goto fail;
    }else{

		// #importante:
		// N�o podemos voltar para essa trhead.

		// #importante:
		// Checar a flag de reaproveitamento.
		// Se a flag do reaproveitamento falhar ent�o o exit no kernel
		// n�o acionou ela para a threa InitThread, que � nossa �nica reaproveit�vel.
		// por enquanto.
		
		//if ( Thread->used != 1 || Thread->magic != 1234 )
		//{
		//	printf("executive_gramado_core_init_execve: Validation fail \n");
		//	die();
			//goto fail;
		//}


		//
		// ## state ##
		//

		// Vamos seguir a sequ�ncia de nacimento de um thread e 
		// cancelaremos, caso n�o for poss�vel carregar o arquivo do programa.
		// INITIALIZED >> STANDBY >> READY >> RUNNING ...

        Thread->state = INITIALIZED;

		// '0'. Significa que o contexto nunca foi salvo, pois o spawn 
		// n�o funciona em thread com o contexto salvo.

        Thread->saved = 0; 

		// ??
		// Plano. bg/fg.

        Thread->plane = Plane;

		//#test
		// Vamos associar ao primeiro tty, mesmo que seja um aplicatibo GUI.
		// Se ele for um aplicativo GUI ele ir� atualizar o foco.
		// Se for um aplicativo de terminal ent�o ter� uma janela 
		// para rodar. Pois o ldisc manda mensagens para a thread de controle 
		// da janela com foco de entrada. Vamos fazer isso manualmente.

        if ( (void *) CurrentTTY != NULL )
        {
            if ( CurrentTTY->used == 1 && CurrentTTY->magic == 1234 )
            {
                current_tty = CurrentTTY->index;

                Thread->tty_id = current_tty;

				// #terminal window.
                window_with_focus = CurrentTTY->window->id;
                terminal_window = CurrentTTY->window->id;

				//#importante
				//a thread de controle da janela, para qual
				//ser�o enviadas as mensagens pelo ldisc
                CurrentTTY->window->control = Thread;
            }

        }else{
            //Thread->tty_id = 0; //-1
        };

		// Context.
		// #todo: 
		// Isso deve ser uma estrutura de contexto.

		// Stack frame.

        Thread->ss = 0x23; 
        //Thread->esp = (unsigned long) 0x0044FFF0; 
        Thread->esp = (unsigned long) process->Image + 0x4FFF0; 
        Thread->eflags = 0x3200; 
        Thread->cs = 0x1B; 
        //Thread->eip = (unsigned long) 0x00401000; 
        Thread->eip = (unsigned long) process->Image + 0x1000;


		// Segment registers.
        Thread->ds = 0x23; 
        Thread->es = 0x23; 
        Thread->fs = 0x23; 
        Thread->gs = 0x23; 

		// Outros.
        Thread->eax = 0;
        Thread->ebx = 0;
        Thread->ecx = 0;
        Thread->edx = 0;
        Thread->esi = 0;
        Thread->edi = 0;
        Thread->ebp = 0;


		// Next thread.
        Thread->Next = NULL;

		// Thread queue.
        queue_insert_data ( queue, 
            (unsigned long) Thread, 
            QUEUE_INITIALIZED );


		// #importante:
		// * MOVEMENT 1 ( Initialized ---> Standby ).
		// Isso permitira que o taskswitch selecione ela pra rodar.

        SelectForExecution (Thread); 
        

        
        // #debug
        //printf ("do_execve: Spawn thread \n");
        //refresh_screen();
        //while(1){}

        KiSpawnTask ( Thread->tid );

        // No return!
                
        panic ("do_execve: KiSpawnTask returned ");
        //goto done;
    };

	// fail

fail:

    printf ("do_execve: #fail\n");
	// refresh_screen ();

done:

	//#debug
	//printf(">>>shared_p0={%s}\n"     ,shared_p[0]);
	//printf(">>>shared_p1={%s}\n"     ,shared_p[1]);
	//printf(">>>shared_p2={%s}\n\n"   ,shared_p[2]);
	//printf(">>>shared_p3={%s}\n\n"   ,shared_p[3]);
	//refresh_screen();
	//while(1){ asm("hlt"); }

	//#debug
	//printf("done\n");	

	// #Obs: 
	// +N�o devemos emitir mensagens no caso de acerto.
	// +refresh_screen s� no caso de erro.

	// #bugbug
	// #obs: 
	// Estamos usando isso s� por enquanto para debug.

    refresh_screen ();

    return (int) Status;
}



/*
 ************************************************
 * init_executive:
 *
 *     Initialize the kernel executive.
 *     Archtecture (independent) inicialization. 
 */
 
int init_executive (void){

    int Status = 0;


#ifdef KERNEL_VERBOSE
    printf ("EXECUTIVE:\n");
#endif


    debug_print ("init_executive:\n");


	// PCI - Pega informa��es da PCI.
    init_pci ();


	// CLOCK - Pega informa��es de Hora e Data.
    init_clock ();
    get_cmos_info ();

	//...


	// #importante: 
	// S� depois de inicializarmos o ata 'e que podemos carregar 
	// alguma coisa.

    debug_print ("init_executive: diskATADialog\n");
    diskATADialog ( 1, FORCEPIO, FORCEPIO );

	// ??
	// configura a tabela do kernel de fun��es exportadas
	// e tabela de ponteiros para tabelas dos outros programas em 
	// kernel mode.
    // Status = (int) executive_config_exported_functions();

	//Continua ...
    
	// #todo: 
	// Checar a validade de 'Initialization' ??

	//#debug
	//printf("Done!\n");

    Initialization.executive = 1;

    return (int) Status;
}


//
// End.
//

