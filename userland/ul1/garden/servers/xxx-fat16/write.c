/*
 * File: fs\write.c 
 *
 * Descri��o: 
 *     Rotinas de grava��o. em disco
 *
 * Obs:
 *    As rotinas na biblioteca C devem chamar essas fun��es.
 * Hist�rico:
 *    Vers�o 1.0, 2015 - Esse arquivo foi criado por Fred Nora.
 *    Vers�o 1.0: 2016 - Revis�o.
 *    ...
 */
 
 
#include <kernel.h>


extern void reset_ide0();


/*
 ************************************
 * write_fntos
 *     Rotina interna de support.
 *     isso deve ir para bibliotecas depois.
 *     n�o tem prot�tipo ainda.
 *     Credits: Luiz Felipe */

void write_fntos (char *name){
	
    int  i, ns = 0;
    char ext[4];
	
	ext[0] = 0;
	ext[1] = 0;
	ext[2] = 0;
	ext[3] = 0;
	
    //const char ext[4];
	
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
 * fatWriteCluster:
 *     Salva um cluster no disco.
 *
 * Argumentos:
 *   setor   ~ Primeiro setor do cluster.
 *   address ~ Endere�o do primeiro setor do cluster.
 *   spc     ~ N�mero de setores por cluster.
 */

//int fatWriteCluster( unsigned long sector, 
//                     unsigned long address, 
//	  				   int spc )
					  
void fatWriteCluster ( unsigned long sector, 
                       unsigned long address, 
					   unsigned long spc )
{
	unsigned long i;

	//Come�a do primeiro setor do cluster.
	for ( i=0; i < spc; i++ )
	{
        write_lba( address, sector + i );
		address = address +512; 
	};
	
	//...
	
	return;
}


/*
 *************************************************
 * write_lba:
 *     Grava um setor no disco dado o lba. */

//int write_lba( unsigned long address, unsigned long lba ) 

void write_lba ( unsigned long address, unsigned long lba ){
    
    //@todo: Check limits.
	
	// ?? onde est� fatbits ??
	
    switch (fatbits)
    {
	    case 32:
			printf("fs-write-write_lba: fat32 not supported\n");
			goto fail;
			break;		
			
	    case 16:
		    //unb/hdd.c
            my_write_hd_sector ( address, lba, 0, 0 );
            goto done;			
			break;		
			
	    case 12:
			printf("fs-write-write_lba: fat12 not supported\n");
			goto fail;	    
			break;

        default:
			printf("fs-write-write_lba: Unknow fat fs\n");
			goto fail;		    
            break;		
	};

    // Nothing.	
	
fail:
    refresh_screen();	
done:
    return;
};


/*
 **************************************************
 * fsSaveFile:
 *     Salva um arquivo.
 *     Por enquanto no diret�rio raiz.
 * #importante
 * @todo: A biblioteca C pode chamar essa fun�ao.
 */
 
//#todo: 
//#test
//N�mero m�ximo de entradas na lista de clusters. Quantas?
// A FAT tem 246 setores, 123 KB
//#define  fat_range_max (1024*4)  
//#define  fat_range_max (1024*8)  
#define  fat_range_max (1024*32)  

//lista de clusters
unsigned short list[fat_range_max];
 
int
fsSaveFile ( char *file_name, 
             unsigned long file_size,
             unsigned long size_in_bytes,			
             char *file_address,
             char flag )  
{
	
	printf ("write-fsSaveFile: \n");
	
	
	//info
	//me parece que todas as informa��es est�o chegando aqui corretas.
	//
	
	//#debug:
	//o nome est� salvando corretamente;
	printf ("name=%s  \n", file_name ); 
	printf ("size=%d  \n", file_size );
	printf ("nbytes=%d  \n",size_in_bytes);
	printf ("address=%x  \n",file_address);
	printf ("flag=%x \n", flag);
	
	int Status = 0;
	
	//#bugbug: 
	//N�o vale esse determinismo. 
	//Temos que saber qual � o disco atual e o volume atual.
	//Precisamos saber se o root dir j� est� na mem�ria e se
	//a fat j� est� na mem�ria.
	
    unsigned short *root = (unsigned short *) VOLUME1_ROOTDIR_ADDRESS;
    unsigned short *fat  = (unsigned short *) VOLUME1_FAT_ADDRESS;

    //unsigned long endereco = file_address;

    unsigned long i = 0; 
    unsigned long j = 0; //Deslocamento na lista, tem que ser zero.
    unsigned long c = 0; //Deslocamento na FAT.
	
    unsigned short first;
    unsigned short next;
    unsigned short sector;
    
	// Buffer para a entrada de diret�rio.
	char Entry[32];	

    //Mensagem.	
	//printf("fsSaveFile:\n"); 
	
	
	//file_size
	//#todo: precisamos implementar um limite para o tamanho do arquivo,
	//principamente nessa fase de teste.
	
	
    //Carrega root e fat.
//rootdir:
    //#debug
	//printf("Loading root..\n"); 
	fs_load_rootdirEx();
	
//fat:
    //#debug
	//printf("loading FAT..\n");
	fs_load_fatEx();
	
	//#debug
	//refresh_screen();
	
    
	// Procurando cluster livre na fat.
	// Nesse momento construimos uma lista de clusters livres.
	// #todo: Essa lista j� devia existir e agora somente 
	// usar�amos.
	
//SearchEmptyEntries:
    
	// #bugbug
	// Obs: Esse limite � improvisado.
	
    while ( i < fat_range_max )
    {
        //procurando cluster livre na fat.
        //@todo:isso poderia ser uma fun��o.		
        if (fat[c] == 0)
        {
            // Encontrado todos os espa�os livres 
			// que o arquivo precisa.
            // Marca o fim.
			
			if (file_size == 0)
			{
                list[j] = (unsigned short) 0xfff8;   

                //#importante:
                //Se der certo, sa�mos do loop.
				
                goto save_file;
            };    
            
            //salva um endere�o livre
            //salvamos um �ndice na fat dentro da lista			
            //incrementa a lista
			
			list[j] = (unsigned short) c;   
            j++;   

			//decrementa o tamanho do arquivo!
            file_size--;                   
        };
        
        c++; //incrementa o deslocamento na fat. 
        i++; //incrementa a quantidade de busca.
    }; 
  
    // Fail
    // Nossa busca por clusters livres dentro da fat n�o deu certo.
    // Provavelmente n�o encontramos uma quantidade sufciente.
	
out_of_range:  

    printf("fsSaveFile: No free cluster \n");
    goto fail;
	
	//#debug
    //refresh_screen();
    //while(1){ asm("hlt"); }
   
// #importante:
// Deu certo. Encontramos na fat todos os clusters que o arquivo precisa.   
// Salva o arquivo.
// O arquivo tem na lista todos os clusters que ele precisa.

save_file:

    //#debug
    //printf("fsSaveFile: save_file: \n"); 
	//refresh_screen();
  
    // In�cio da lista.
    i = 0; 

	// #bugbug
	// Limite m�ximo improvisado.
	
    j = 512*4;    
 
    // Pegamos o primeiro da lista.
    first = list[i];
	
	// #debug
	// printf("first={%x}\n",first);
	
//CreateEntry:
	
	// Name.
	Entry[0] = (char) file_name[0];
	Entry[1] = (char) file_name[1];
	Entry[2] = (char) file_name[2];
	Entry[3] = (char) file_name[3];
	Entry[4] = (char) file_name[4];
	Entry[5] = (char) file_name[5];
	Entry[6] = (char) file_name[6];
	Entry[7] = (char) file_name[7];
	
	// Ext.
	Entry[8] = (char) file_name[8];
	Entry[9] = (char) file_name[9];
	Entry[10] = (char) file_name[10];
	
	// Flag and reserved.
	// 0x20=arquivo.
	Entry[11] = flag;   
	Entry[12] = 0;       
	
	// Creation time. 14 15 16
	Entry[13] = 0x08; 
	Entry[14] = 0x08; 
	Entry[15] = 0xb6;
	
	// Creation date.
	Entry[16] = 0xb6;
	Entry[17] = 0x4c;
	
	// Access date.
	Entry[18] = 0xb8;
	Entry[19] = 0x4c;
	
	// ??
	// First cluster. 
	// 0 para fat12 ou 16
	Entry[20] = 0;
	Entry[21] = 0;
	
	// Modifield time.
	Entry[22] = 0xa8;
	Entry[23] = 0x49;
	
	// Modifield date.
	Entry[24] = 0xb8;
	Entry[25] = 0x4c;
	
	// First cluster. Low word.
	// 0x1A and 0x1B
	Entry[26] = (char) (first);        
	Entry[27] = (char) (first >> 8);    
	
	// File size in bytes.
	// (size_in_bytes)
	// 4 bytes: (28,29,30,31)
	
	Entry[28] = (char) size_in_bytes;   
	
	size_in_bytes = (size_in_bytes >> 8);
	Entry[29] = (char) size_in_bytes;
	
	size_in_bytes = (size_in_bytes >> 8);
	Entry[30] = (char) size_in_bytes;
	
	size_in_bytes = (size_in_bytes >> 8);
	Entry[31] = (char) size_in_bytes;

	
	// #importante:
	// Vamos encontrar uma entrada livre no diret�rio para
	// salvarmos o nome do arquivo.
	
	// Copia o nome para dentro da entrada do diret�rio.
	
	// Obs: As entradas s�o de 32 bytes. Como root[] � um 
	// array de short ent�o faremos um deslocamento de 16 shorts.
	
	// root[]
	// #importante: root[] � um array de short.	

	// IN: 
	// Endere�o do diret�tio e n�mero m�ximo de entradas.
	// #todo: talvez possamos ampliar esse n�mero para o m�ximo 
	// de entradas no diret�rio.
	
	int xxxx_entryindex = (int) findEmptyDirectoryEntry ( VOLUME1_ROOTDIR_ADDRESS, 128 );
	
	if ( xxxx_entryindex == -1 ){
		
		printf ("No empty entry\n");
		goto fail;
	}
	
	int xxxx_entrysize = 16;
	
	
	// xxxx =  Deslocamento dentro do diret�rio.
	// representa o in�cio da entrada que encontramos.
	// Encontramos multiplicando o �ndice da entrada pelo tamanho da entrada.
	
	int xxxx = (int) ( xxxx_entryindex * xxxx_entrysize );
	
	//Copia 32 bytes.
	memcpy ( &root[xxxx], Entry, 32 );

// reset	
// Reiniciamos o controlador antes de usarmos.
	
//resetIDE:

	//#debug
	//printf("fsSaveFile: reset and while\n"); 
    //refresh_screen();
	
	//reset_ide0 ();
	
	// ## Save ##
    // Vamos ao salvamento propriamente dito.
   
	i = 0; 
	
	//#debug 
	//improvisando um endere�o v�lido
	
	unsigned long address = (unsigned long) file_address;
	
//SavingFile:	

	while (1)
    {            
        //Pega um conte�do da lista.
        next = list[i];
		
		//#debug.
		printf("next={%x}\n",next);
                
        //encontrada a assinatura na lista!
        if ( next == 0xfff8)
        {
            next = list[i-1];
            fat[next] = 0xfff8;  
            goto done;         
        
		}else{

            //n�o � assinatura ainda
			
			//grava na fat o endere�o do pr�ximo cluster
            fat[next] = list[i+1];
            
            //#debug 
			//printf("write_lba\n");
            //refresh_screen();
			
			disk_ata_wait_irq ();
			
            //grava - aqui next esta certo!!!			
            //write_lba ( (unsigned long) address, VOLUME1_DATAAREA_LBA + next -2 );
            my_write_hd_sector ( (unsigned long) address, 
			    (unsigned long) ( VOLUME1_DATAAREA_LBA + next -2), 0, 0  );
			
            address += 512; 
        }; 
         
        //pr�ximo valor da lista
        i++;
		 
        //limite #test #bugbug
		//limitando o tamanho do arquivo a 16 entradas.
        if (i > 16){
		    goto fail;       	
		}

        //??		
    };
    
	
fail:	
    printf("# FAIL #\n");
    refresh_screen();
    return (int) 1;

done:  

    //Nesse momento j� salvamos os clusters do arquivo.
	
    //#debug
    printf("fsSaveFile: clusters saved\n");
	
	
    printf("Saving root..\n");	
    refresh_screen();
	
	
	int r;
    int roff = 0;
    int rlbaoff = 0;	
	
	//int ticks = 1000;
	
	
    //
    // #bugbug: 
	// Precisamos saber o tamanho do root ... 
	// Precismos de estrututra de root	
	//	
	
	for ( r=0; r<32; r++ )
	{
	    //reset_ide0 ();
        
		//printf("write_lba n={%d} \n",r);      
        //refresh_screen();		
	    
		//Wait interrupt!
		//Isso funcionou.
		disk_ata_wait_irq ();
		
		//#bugbug: N�o podemos determinar os valores. 
		// Precisamos de estruturas.
					
		my_write_hd_sector ( (unsigned long) VOLUME1_ROOTDIR_ADDRESS + roff, 
		    (unsigned long) ( VOLUME1_ROOTDIR_LBA     + rlbaoff ), 0, 0  );			
				  
        roff = roff + 0x200;
        rlbaoff = rlbaoff + 1;	  		
	
	    //#bugbug
		//?? Estamos esperando antes de gravarmos o pr�ximo.
		//wait irq
	};

    //reset_ide0 ();
    //write_lba ( VOLUME1_ROOTDIR_ADDRESS, VOLUME1_ROOTDIR_LBA);	
	
	//reset_ide0 ();
	//write_lba ( VOLUME1_ROOTDIR_ADDRESS + 0x200, VOLUME1_ROOTDIR_LBA + 1);
    //write_lba( VOLUME1_ROOTDIR_ADDRESS + 0x400, VOLUME1_ROOTDIR_LBA + 2);
    //write_lba( VOLUME1_ROOTDIR_ADDRESS + 0x600, VOLUME1_ROOTDIR_LBA + 3);

	//........
	
    //printf("write_lba\n");      
    //refresh_screen();		
	
    //#bugbug: Devemos salvar ela toda.	
    //salva 4 setores da fat!       
    
    printf("Saving fat..\n");	
    refresh_screen();	

	int f;
    int off = 0;
    int lbaoff = 0;

    //
    // #bugbug: 
	// Precisamos saber o tamanho da fat ... 
	// Precismos de estrututra de fat	
	//
	
	//#obs: 
	// Estamos salvando 32 setores da FAT,
	// mas FAT na verdade tem 246 setores.
    // #test: Vamos salvar todos os setores da FAT como teste.
   	
	
	//for ( f=0; f<32; f++ )
	for ( f=0; f<246; f++ )
	{
	   //#bugbug:
       //pelo jeito a rotina de salvamento precisa de 
       //de um wait ... um sleep  ... eu checar a interrup��o. 	   
       
	   //printf("write_lba n={%d} \n",f);      
       //refresh_screen();		
	
	    disk_ata_wait_irq ();
		
	    //write_lba ( VOLUME1_FAT_ADDRESS + off, 
	    //            VOLUME1_FAT_LBA     + lbaoff );
		
        my_write_hd_sector ( (unsigned long) VOLUME1_FAT_ADDRESS + off, 
		    (unsigned long) ( VOLUME1_FAT_LBA     + lbaoff ), 0, 0  );		
				  
       off = off + 0x200;
       lbaoff = lbaoff + 1;	   
	};
	
	
    //#debug
    printf("fsSaveFile: done hang \n"); 
    refresh_screen();	
    
	return 0;
}


/*
 * fs_save_rootdir:
 *     Salva o diret�rio raiz no disco.
 *     @todo: Identificar par�metros do sistema de arquivos atual. */

void fs_save_rootdir ()
{
	//return;
};


/*
 * fs_save_dir:
 *     Salva um diret�rio.
 */
void fs_save_dir (unsigned long id)
{
	//return;
};


/*
 * fs_save_entry_on_root:
 *     Salva uma entrada do diret�rio raiz, dada
 * a id da estrutura do arquivo.
 * @todo: Checa os parametros do sistema de arquivos e 
 * salva a entrada no diretorio raiz.
 */
void fs_save_entry_on_root(unsigned long eid)
{
    //return;
};


void fs_save_structure()
{   
	//return;
};


//
// End.
//
