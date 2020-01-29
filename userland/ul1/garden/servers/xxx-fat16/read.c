/*
 * File: fs/read.c 
 *
 * Descri��o:
 *     Rotinas de leitura
 *     As rotinas na biblioteca C devem chamar essas fun��es.
 *
 * History:
 *    2015 - Created by Fred nora.
 *    2016~2018 - Revision.
 *    ...
 */
 
 
/*
 #todo: 
     Usar estruturas para gerenciar parti��o, FAT e Root dir.
 #obs: 
     Cada parti��o deve ter uma estrutura.
 precisamos checar na estrutura o status da FAT e do 
 diret�rio raiz, para n�o carreg�los repetidas vezes.
 */ 
 
 
#include <kernel.h>


/*
 ************************************************
 * read_fntos:
 *     rotina interna de support.
 *     isso deve ir para bibliotecas depois.
 *     n�o tem prot�tipo ainda.
 *     Credits: Luiz Felipe */
 
void read_fntos ( char *name ){
	
    int  i, ns = 0;
    char ext[4];
	
	ext[0] = 0;
	ext[1] = 0;
	ext[2] = 0;
	ext[3] = 0;
		
    //Transforma em mai�scula enquanto n�o achar um ponto.
	
	while ( *name && *name != '.' )
	{
        if ( *name >= 'a' && *name <= 'z' )
            *name -= 0x20;

        name++;
        ns++;
    };

    // Aqui name[0] � o ponto.
	// Ent�o constr�i a extens�o.
	
	for ( i=0; i < 3 && name[i+1]; i++ )
	{
		//Transforma uma letra da extens�o em mai�scula.
        
		//if (name[i+1] >= 'a' && name[i+1] <= 'z')
        //    name[i+1] -= 0x20;

        //ext[i] = name[i+1];
    
	
	    //#testando
	    //Se n�o for letra ent�o n�o colocamos no buffer de extens�o;
		if (name[i+1] >= 'a' && name[i+1] <= 'z')
		{
			name[i+1] -= 0x20;
		    ext[i] = name[i+1];
		}
	};

	//Acrescentamos ' ' at� completarmos as oito letras do nome.
	
    while (ns < 8)
	{	
        *name++ = ' ';
        ns++;
    };

	//Acrescentamos a extens�o
	
    for (i=0; i < 3; i++)
        *name++ = ext[i];

    *name = '\0';
}
	
	
/*
 ***********************
 * fatClustToSect:
 *     Calcula qual � o setor inicial de um dado cluster.
 *     Informa��es para o calculo: 
 *     ~ N�mero do cluster.
 *     ~ N�mero de setores por cluster.
 *     ~ Lba inicial da area de dados.
 */	
 
unsigned long 
fatClustToSect ( unsigned short cluster, 
                 unsigned long spc, 
				 unsigned long first_data_sector )
{
    unsigned long C = (unsigned long) cluster;

	C -= 2;
	
	//@todo: Check limits.
	
	return (unsigned long) (C * spc) + first_data_sector;
}


/*
 *************************************************************
 * fatLoadCluster:
 *     Carrega um cluster.
 *     Argumentos:
 *         setor   ~ Primeiro setor do cluster.
 *         address ~ Endere�o do primeiro setor do cluster.
 *         spc     ~ N�mero de setores por cluster.
 * Come�a do primeiro setor do cluster.
 */
 
void 
fatLoadCluster ( unsigned long sector, 
                 unsigned long address, 
				 unsigned long spc )
{
	unsigned long i;
	
	for ( i=0; i < spc; i++ ){
		
        read_lba ( address, sector + i );
		address = address +512; 
	}
}


/*
 *****************************************************************
 * read_lba:
 *     Carrega um um setor na mem�ria, dado o LBA.
 *     Obs: Talvez essa rotina tenha que ter algum retorno no caso de falhas. */
 
void read_lba ( unsigned long address, unsigned long lba ){
	
 	//taskswitch_lock();
	//scheduler_lock();	          	
	
    switch (fatbits){
		
	    case 32:
	        //Nothing.	    
            return;
			break;		
			
	    case 16:
		    //hdd.c
	        my_read_hd_sector ( address, lba, 0, 0 );	    
            return;
			break;		
			
	    case 12:
	        //Nothing.	    
            return;
			break;

        default:
		    //@todo: 
			// Precisamos fazer alguma coisa se essa vari�vel for 
			// um valor inv�lido.
		    //return;
            break;		
	};	
	
	//scheduler_unlock();
	//taskswitch_unlock();
}


/*
 **************
 * fsLoadFile:
 *    Carrega um arquivo na mem�ria dado o nome e o endere�o.
 *    Apenas FAT16.
 *
 *  @todo: 
 *      Deve-se alocar mem�ria dinamicamente para o root dir e 
 * para a fat caso eles n�o estejam carregados e n�o tenham seus 
 * endere�os salvos.
 * Obs: Pode existir uma estrutura pra dizer se o root dir e fat est�o 
 * carregados na mem�ria e qual o endere�o o tipo o tamanho etc.
 * + O argumento nome poderia ser const char* para compatibilidade 
 * com a libC.
 *
 * #obs: A fat deve estar o tempo todo na mem�ria, mas precisamos carregar 
 * na mem�ria o dret�rio atual para lermos um arquivo.
 * Essa fun��o de leitura n�o precisa carregar a FAT toda vez que for chamada.
 * ela deve apenas checar se a fat est� na mem�ria atrav�s de uma flag.
 * Mas essa fun��o precisa carregar o diret�rio atual na mem�ria para 
 * procurar pelo nome indicado.
 * #bugbug: No momento estamos apenas procurando no diret�rio raiz, 
 * ent�o devemos substituir o ponteiro *root por *current_dir.
 * IN:
 *     Endere�o do diret�rio, 
 *     Nome do arquivo, 
 *     Endere�o do arquivo.
 */

unsigned long 
fsLoadFile ( unsigned long fat_address,
			 unsigned long dir_address,
             unsigned char *file_name, 
             unsigned long file_address )			 
{
    int Status;		
	int i;
    unsigned short next;

    unsigned long max = 64;    //?? @todo: rever. N�mero m�ximo de entradas.
    unsigned long z = 0;       //Deslocamento do rootdir 
    unsigned long n = 0;       //Deslocamento no nome.
	char NameX[13];	           //??Nome. 

	unsigned short cluster;    //Cluster inicial

	//?? Primeiro setor do cluster.
	unsigned long S;  
	
	int Spc;
	
	// Updating fat address and root address.
	
	unsigned short *fat = (unsigned short *) fat_address;   
	unsigned short *root = (unsigned short *) dir_address;
	
	
	// Lock ??.
	
	//taskswitch_lock();
	//scheduler_lock();	
	
	// Root dir.

//loadRoot:
	
	//Carrega o diret�rio raiz na mem�ria.
	
#ifdef KERNEL_VERBOSE	
	debug_print("fsLoadFile:\n");
#endif	
	
	//#importante
	//N~ao carregaremos mais um diret'orio nesse momento,
	//usaremos o endere�o passado por argumento.
	//esperamos que nesse endere�o tenha um diret'orio carregado.
	//Na inicializa�~ao 'e preciso carregar o diret'orio raiz
	//antes de chamar essa fun�~ao. E para caregar o diret'orio raiz
	//precisa inicializar o sistema de arquivos e o controlador IDE.
	
	//#test
	//funcionou
	//carregando o diret�rio 
	//address, lba, number of sectors.
	
	//load_directory ( dir_address, VOLUME1_ROOTDIR_LBA, 32 );
    //load_directory ( VOLUME1_ROOTDIR_ADDRESS, VOLUME1_ROOTDIR_LBA, 32 );	
	
	//antigo.  
	//funciona
	//carregando o diret�rio raiz.
	//fs_load_rootdirEx ();
	
	//#todo:
	//precisamos na verdade carregarmos o diret�rio corrente.
	
	
	//Checa se � v�lida a estrutura do sistema de arquivos.
	
	if ( (void *) filesystem == NULL )
	{
	    printf("fs-read-fsLoadFile: filesystem\n");
		goto fail;
	
	}else{
		
	    //Setores por cluster.
	    Spc = filesystem->spc;
		
	    if (Spc <= 0)
		{
	        printf("fs-read-fsLoadFile: Spc\n");
		    goto fail;
	    };
	
	    //Max entries ~ N�mero de entradas no rootdir.
		//#bugbug: Devemos ver o n�mero de entradas no diret�rio corrente.
	    max = filesystem->rootdir_entries;	
		
	    if (max <= 0)
		{
	        printf("fs-read-fsLoadFile: max\n");
			goto fail;
	    };
		
	    // More?! 
		// ...
	};
	
	// Continua ... 
	// Pegar mais informa��es sobre o sistema de arquivos.
	// Busca simples pelo arquivo no diret�rio raiz.
	// #todo: 
	// Essa busca pode ser uma rotina mais sofisticada. Uma fun��o auxiliar.
	// Primero caractere da entrada:
	// 0 = entrada vazia.
	// $ = entrada de arquivo deletado.
	// outros ...
	// ATEN��O:
	// Na verdade a vari�vel 'root' � do tipo short.	  
	 
	i = 0; 
		
    size_t size = (size_t) strlen (file_name); 
	
	// Procura o arquivo no diret�rio raiz.	
	
	while ( i < max )
	{
		//Se a entrada n�o for vazia.
		if ( root[z] != 0 )
        {
			// Copia o nome e termina incluindo o char 0.
			memcpy( NameX, &root[z], size );
			NameX[size] = 0;
			
            // Compara 11 caracteres do nome desejado, 
			// com o nome encontrado na entrada atual.
			Status = strncmp ( file_name, NameX, size );
			
            if ( Status == 0 ){ 
			    goto found; 
			}
			// Nothing.
        }; 
		
		//(32/2) pr�xima entrada! (16 words) 512 vezes!
        z += 16;    
        i++;        
    }; 
	
	// Sai do while. O arquivo n�o foi encontrado.
	
    // O arquivo n�o foi encontrado.	
//notFound:
    printf ("fs-read-fsLoadFile: %s not found\n", file_name );  
	
	//#debug
    //printf ("fs-read-fsLoadFile: %s not found\n", NameX );
	//printf("root: %s ",root);	
    goto fail;
	
    // O arquivo foi encontrado.	
found:

    // #debug
    // printf("arquivo encontrado\n");
    // refresh_screen();
	// while(1){}
	
    //Pega o cluster inicial. (word)
	cluster = root[ z+13 ];    //(0x1A/2) = 13.	
	
	
	// Cluster Limits.
	// Checar se 'cluster' est� fora dos limites.
	// +S�o 256 entradas de FAT por setor. 
	// +S�o 64 setores por FAT. 
	// Isso varia de acordo com o tamanho do disco.
	// O n�mero m�ximo do cluster nesse caso � (256*64).
	
	if ( cluster <= 0 || cluster > 0xfff0 ){
		
	    printf("fs-read-fsLoadFile: Cluster limits %x \n", cluster );
		goto fail;
	}	
	
	
//loadFAT:
	
    //Carrega fat na mem�ria.
	
#ifdef KERNEL_VERBOSE		
	printf("loading FAT..\n");
#endif 
	
	//=============================
	//#bugbug: 
	//N�o devemos carregar a FAT na mem�ria toda vez que 
	//formos carregar um arquivo. 
	//Talvez ela deva ficar sempre na mem�ria.
	//precisamos de estruturas para volumes que nos d� esse tipo de informa��o
	
	fs_load_fatEx();
	
    // Carregar o arquivo, cluster por cluster.
    // @todo: Por enquanto, um cluster � igual � um setor, 512 bytes.
 
    //Debug:
#ifdef KERNEL_VERBOSE		
    printf ("Loading clusters..\n");
    //refresh_screen();
#endif

	// Loop: 
	//     Loop de entradas na FAT.
    //     @todo: Esse loop � provis�rio, while pode ser problema.
	
	//
	// Carregar o arquivo.
	//
	
//Loop.	
next_entry:
	
	/*
	while(1)
	{	
	    //Calcula.
		//Primeiro setor do cluster. 
		S = fatClustToSect(cluster, Spc, VOLUME1_DATAAREA_LBA); 
		
		//Carrega 'x' setores come�ando de S.
		fatLoadCluster( S, file_address, Spc);
		
		//Pegar o pr�ximo cluster na FAT.
		//O cluster atual cont�m o n�mero do pr�ximo.
		cluster = fat[cluster];
		
		//Ver se o cluster carregado era o �ltimo.
	    if(cluster == 0xFFFF || cluster == 0xFFF8){ goto done; };
		//Nothing.
    };
	*/
	
	read_lba ( file_address, VOLUME1_DATAAREA_LBA + cluster -2 ); 
	
	//Incrementa o buffer. +512;
	//SECTOR_SIZE;
	file_address = (unsigned long) file_address + 512;    	
	
	
	//Pega o pr�ximo cluster na FAT.
	next = (unsigned short) fat[cluster];		
	
	//Configura o cluster atual.
	cluster = (unsigned short) next;	
	
	//Ver se o cluster carregado era o �ltimo cluster do arquivo.
	if ( cluster == 0xFFFF || cluster == 0xFFF8 ){ 
	    goto done; 
	}

	// ## Loop ## 
	// Vai para pr�xima entrada na FAT.
	
	goto next_entry;
	
	//Nothing.
	
//Falha ao carregar o arquivo.
fail:
    
	printf ("fs-read-fsLoadFile fail: file={%s}\n", file_name );	
    refresh_screen ();
	
	return (unsigned long) 1;
	
done:
    
	//#debug support
	//printf("fsLoadFile: done\n");
	//refresh_screen(); 
    
	return (unsigned long) 0;
}


/*
 ****************************************
 * fs_load_fatEx:
 *    Carrega a fat na mem�ria.
 *    Sistema de arquivos fat16.
 *    ? qual disco ?
 *    ? qual volume ? 
 *    #obs: Essa rotina poderia carregar a fat do volume atual do 
 * disco atual. � s� checar na estrutura.
 *
 * current disk, current volume, fat status.
 *
 * +se o status da fat para o vulume atual indicar que ela j� est� carregada,
 *  ent�o n�o precisamos carregar novamente.
 */

void fs_load_fatEx (){
	
	unsigned long i;
	unsigned long b = 0;
	
	//#bugbug 
	//Estamos atribuindo um tamanho, mas tem que calcular.
	unsigned long szFat = 128;
	
	
	//#todo:
	//+checar qual � o disco atual.
	//+checar qual � o volume atual.
	//+checar o status da FAT. Talvez ela j� esteja na mem�ria.
	//obs: a estrutura deve informar onde est� a fat do volume,
	//caso ja esteja na mem�ria.
	//obs: padronizaremos alguns endere�os, e alocaremos outros.
	
	//Carregar fat na mem�ria.
	
	for ( i=0; i < szFat; i++ )
	{
		my_read_hd_sector( VOLUME1_FAT_ADDRESS + b, VOLUME1_FAT_LBA + i, 0 , 0 );
		
		//Incrementa buffer.
		b = b+512;    
	};
}



/*
 ***********************************************************
 * load_directory:
 *    Carrega o diret�rio na mem�ria, dados o endere�o, 
 * o lba inicial e o n�mero de setores.
 */
 
void 
load_directory ( unsigned long address, 
                 unsigned long lba, 
				 unsigned long sectors )
{
	unsigned long i;
	unsigned long b = 0;
	
	for ( i=0; i < sectors; i++ )
	{
        my_read_hd_sector ( address + b, lba + i, 0, 0 );
		
		b = b+512;    
	}
};


void fs_load_rootdir (){
	
    load_directory ( VOLUME1_ROOTDIR_ADDRESS, VOLUME1_ROOTDIR_LBA, 32 );	
}


/*
 ********************************************************
 * fs_load_rootdirEx:
 *    Carrega o diret�rio raiz na mem�ria. */

void fs_load_rootdirEx (){
	
	fs_load_rootdir ();
}


/*
 * fs_load_dir:
 *     Carrega um dado diret�rio da lista de arquivos, 
 * dado o �ndice da lista de streams do kernel.
 */
 
void fs_load_dir ( unsigned long id ){
    //#cancelar.
}


//pega o tamanho de um arquivo que est� no diret�rio raiz.
//#todo: podemos alterar para pegar de um arquivo que esteja no diret�rio alvo.
unsigned long fsGetFileSize ( unsigned char *file_name ){
	
    int Status;		
	int i;
    unsigned short next;
	
	unsigned long FileSize = 0;
	
    unsigned long max = 64;    //?? @todo: rever. N�mero m�ximo de entradas.
    unsigned long z = 0;       //Deslocamento do rootdir 
    unsigned long n = 0;       //Deslocamento no nome.
	char NameX[13];	           //??Nome. 

	// #importante:
	// Poder�amos usar malloc ou alocador de p�ginas ??	
    // A FAT permanece a mesma para a situa��od e termos apenas uma parti��o.
	//mas se tivermos mai de uma parti��o tamb�m precisamos carregar a FAT 
	//da parti��o atual.
	//unsigned short *fat = (unsigned short *) VOLUME1_FAT_ADDRESS;
	unsigned short cluster;    //Cluster inicial

    //??	
	unsigned long S;  //Primeiro setor do cluster.
	int Spc;

	// #importante:
	// Poder�amos usar malloc ou alocador de p�ginas ??
	// #todo: Devemos carregar o diret�rio atual.
    unsigned short *root = (unsigned short *) VOLUME1_ROOTDIR_ADDRESS;

	// #todo: Devemos carregar o diret�rio atual.
	//unsigned long current_dir_address = (unsigned long) Newpage();
    //#todo: devemos checar se o endere�o � v�lido.
	//unsigned short *current_dir = (unsigned short *) current_dir_address;	
	// #todo: devemos chamar uma fun��o que carregue um diret�rio no endere�o passado 
	//via argumento.
    //...
	
	// Lock ??.
	
	//taskswitch_lock();
	//scheduler_lock();	
		
	// Root dir.

//loadRoot:
	
	//Carrega o diret�rio raiz na mem�ria.
	
#ifdef KERNEL_VERBOSE	
	printf("fsGetFileSize: Loading root..\n"); 
#endif	
	
	//#bugbug
    //pega o tamanho de um arquivo que est� no diret�rio raiz.
    //#todo: podemos alterar para pegar de um arquivo que esteja no diret�rio alvo.	
	
	//carregando o diret�rio raiz.
	load_directory ( VOLUME1_ROOTDIR_ADDRESS, VOLUME1_ROOTDIR_LBA, 32 );	
	//fs_load_rootdirEx ();
	
	//#todo:
	//precisamos na verdade carregarmos o diret�rio corrente.
	
	// Continua ... 
	// Pegar mais informa��es sobre o sistema de arquivos.
	
	//#obs
	//Checa se � v�lida a estrutura do sistema de arquivos.
    //A inten��o � obtermos a quantidade de entradas no diret�rio raiz.
	//#bugbug: Mas isso deveria ser feito para o diret�rio atual.
	
	if ( (void *) filesystem == NULL )
	{
	    printf("fsGetFileSize: filesystem\n");
		goto fail;
	
	}else{
		
	    //Setores por cluster.
	    Spc = filesystem->spc;
		
	    if (Spc <= 0)
		{
	        printf("fsGetFileSize: Spc\n");
		    goto fail;
	    };
	
	    //Max entries ~ N�mero de entradas no rootdir.
		//#bugbug: Devemos ver o n�mero de entradas no diret�rio corrente.
	    max = filesystem->rootdir_entries;	
		
	    if (max <= 0)
		{
	        printf("fsGetFileSize: max\n");
			goto fail;
	    };
		
	    // More?! 
		// ...
	};

	
	//Busca simples pelo arquivo no diret�rio raiz.
	//todo: Essa busca pode ser uma rotina mais sofisticada. 
	//Uma fun��o auxiliar.

	//Primero caractere da entrada:
	//0 = entrada vazia.
	//$ = entrada de arquivo deletado.
	//outros ...
	//ATEN��O:
    //Na verdade a vari�vel 'root' � do tipo short.	 

	i = 0; 
	
	// Procura o arquivo no diret�rio raiz.
	
//search_file:

    size_t size = (size_t) strlen (file_name); 
	
	//Compara.
	while ( i < max )
	{
		//Se a entrada n�o for vazia.
		if ( root[z] != 0 )
        {
			// Copia o nome e termina incluindo o char 0.
			memcpy( NameX, &root[z], size );
			NameX[size] = 0;
			
            // Compara 11 caracteres do nome desejado, 
			// com o nome encontrado na entrada atual.
			Status = strncmp ( file_name, NameX, size );
			
            if ( Status == 0 ){ 
			    goto found; 
			}
			// Nothing.
        }; 
		
		//(32/2) pr�xima entrada! (16 words) 512 vezes!
        z += 16;    
        i++;        
    }; 
	
	// Sai do while. O arquivo n�o foi encontrado.
	
    // O arquivo n�o foi encontrado.	
//notFound:
    printf ("fsGetFileSize: %s not found\n", file_name );  

//Falha ao carregar o arquivo.
fail:
    printf ("fsGetFileSize: file={%s}\n", file_name );	
    refresh_screen ();
	return (unsigned long) 0;
	
found:

    // #debug
    // printf("arquivo encontrado\n");
    // refresh_screen();
	// while(1){}
	
    //Pega o cluster inicial. (word)
	//cluster = root[ z+13 ];    //(0x1A/2) = 13.	
	
	//#debug
	//pegando o tamanho do arquivo.
    // 28 29 30 31

    FileSize = *(unsigned long*) (VOLUME1_ROOTDIR_ADDRESS + (z*2) + 28 );	
	
	//printf ("%d \n" , root[ z+14 ]);
	//printf ("%d \n" , root[ z+15 ]);
	//printf ("FileSize=%d \n" , FileSize);
	
	//#debug
	//refresh_screen();
	//while(1){
	//	asm("hlt");
	//}
	
	return (unsigned long) FileSize;
}


//
// End.
//

