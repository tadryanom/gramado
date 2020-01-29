/*
 * File: crts/crt0.c
 *
 * Usado para inicializar a rt na libc03 do atacama.
 * tentando criar um crt0 padrão
 */



#include <types.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>


static char *argv[] = { 
    "-flag1", 
    "-flag2", 
    "-flag3", 
    "-flag4", 
    NULL 
};


/*
// default environment.
// reafazer isso nos padrões.
static char *default_env[] = { 
    "VFSROOT=root:/volume0",           //root dir do vfs
    "BOOTVOLUMEROOT=root:/volume1",    //root dir do volume de boot
    "SYSTEMVOLUMEROOT=root:/volume2",  //root dir do volume do sistema
	NULL 
};
*/

//See: sysdeps/x86/x86start.c
//int x86start ( int argc, char *argv[], char *envp[] );


extern int main ( int argc, char *argv[] );


#define LSH_TOK_DELIM " \t\r\n\a" 
#define SPACE " "
#define TOKENLIST_MAX_DEFAULT 80


//
// # importante
//

// Esses aplicativos rodam no terminal.
// Esses aplicativos escrevem em stdout.
// >>> O terminal precisa conhecer esse stdout para ler.

int crt0 (){


    // Token support.
    char *tokenList[TOKENLIST_MAX_DEFAULT];
    char *token;
    int token_count;
    int index;


    int retval;

	// #importante
	// Linha de comandos passada pelo shell.

    char *shared_memory = (char *) (0xC0800000 -0x100);


/*
#ifdef TEDITOR_VERBOSE
	printf ("\n");
	printf ("crt0: Initializing ...\n");
	//printf("\n");
	//printf(".\n");
	printf ("..\n");
	printf ("# MESSAGE={%s} #\n", shared_memory );
	printf ("..\n");
	//printf(".\n");
	//printf("\n");
	
	//#debug
	//while(1){ asm ("pause"); }
#endif
*/

    // Criando o ambiente.
    // Transferindo os ponteiros do vetor para o ambiente.

    tokenList[0] = strtok ( &shared_memory[0], LSH_TOK_DELIM );

    // Salva a primeira palavra digitada.
    token = (char *) tokenList[0];

    index = 0; 

    while ( token != NULL )
    {
        // Coloca na lista.
        // Salva a primeira palavra digitada.
        tokenList[index] = token;

        //#debug
        //printf("shellCompare: %s \n", tokenList[i] );

        token = strtok ( NULL, LSH_TOK_DELIM );

        // Incrementa o índice da lista
        index++;

        // Salvando a contagem.
        token_count = index;
    }; 

    //Finalizando a lista.
    tokenList[index] = NULL;



/*
#ifdef TEDITOR_VERBOSE
	// #debug 	
	// Mostra a quantidade de argumentos. 

	printf("\n");
	printf("token_count={%d}\n", token_count );

    //Mostra os primeiros argumentos.
    for ( index=0; index < token_count; index++ )
    {
        token = (char *) tokenList[index];
        if ( token == NULL )
        {
            printf ("crt0: for fail!\n")
            goto hang;
        }

        printf ("# argv{%d}={%s} #\n", index, tokenList[index] );
    };
#endif
*/



    // Inicializa a biblioteca.
    libcInitRT ();
    stdioInitialize ();

    // #todo
    // Chamar esse ao invés de chamar os dois acima.
    //See: sysdeps/x86/x86start.c
    //x86start ( ( token_count, tokenList, default_env );


/*
#ifdef TEDITOR_VERBOSE
    //Inicializando o editor propriamente dito.
	printf("Calling main ... \n"); 
#endif
*/

  
  
    // Calling main().


    retval = (int) main ( token_count, tokenList );


    switch (retval)
    {
        case 0:
            //printf ("crt0: main returned 0\n");
            exit (0);
            break;


        case 1:
            //printf ("crt0: main returned 1\n");
            exit (1);
            break;


        //...

        default:
            //printf ("crt0: main returned default\n");
            exit (-1);
            break; 
    };

    //
    // No return!
    //


    //printf ("libc03-crts-crt0: *fail\n");
    exit (-1);

    //printf ("libc03-crts-crt0: *hang\n");
    while (1) { asm ("pause"); };
}


