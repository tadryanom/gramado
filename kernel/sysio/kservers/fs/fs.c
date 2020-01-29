/*
 * File: fs/fs.c 
 *
 * Descri��o:
 *    Arquivo principal do file system manager.
 *    M�dulo em kernel mode, incluido no kernel base. 
 *    
 * IMPORTANTE: 
 * A id�ia principal � que esse m�dulo de o suporte ao acesso a qualquer 
 * sistema de arquivo suportado, esteja ele em user mode ou em kernel mode.
 *
 * Arquivo principal do m�dulo fs do executive do kernel..
 * Gerenciador de arquivos do kernel.
 * Na verdade o Kernel n�o tem atribui��es de carregar arquivos. Mas ele 
 * ofere�e servi�os b�sicos que podem ser chamados pelos aplicativos.
 *
 * Objetivo:
 * Ofere�er opera��es nos sistemas de arquivos suportados pelo kernel.
 * Esse gerenciador chama as fun��es em kernel mode do sistema de arquivos
 * interno e chama as servi�os oferecidos pelos drivers em user mode.
 *
 * Os drivers em user mode podem oferecer acesso � sistemas de arquivos
 * variados.
 *
 * Ao kernel, compete as fun��es b�sicas de opera��es com hardware.
 * O sistema de arquivos suportado pelo kernel � fat16.
 * Continua ...
 *
 * Observa��o: 
 *    Os valores das estruturas do sistema de arquivos podem ser salvos 
 * em um registro.
 *    No registro pode ter um registro das opera��es efetuadas 
 * no sistema de arquivo.
 *   Continua ...
 *   
 * History: 
 *    2015 - Created by Fred Nora.
 *    2016 - Revision.
 *    2019 - fred, pwd and cd support.
 *    ...
 */
 

#include <kernel.h>



//Vari�veis internas.
//int fsStatus;
//int fsError;
//...


// #todo
/*
int fs_count_separators( const char *path);
int fs_count_separators( const char *path){

    size_t count = 0;
    int i;

    
    for (i=0; i < strlen(path); i++)
    {
        if (path[i] == '/')
        {
            count++;
        }
    };


    return (size_t) count;
}
*/


int fsCheckELFFile ( unsigned long address ){

    unsigned char *buffer = (unsigned char *) address;

	// #todo: Error message.
    //if ( (void *) address == NULL )
        //return -1;

    if ( buffer[0] != 0x7F ||
         buffer[1] != 0x45 ||
         buffer[2] != 0x4C ||
         buffer[3] != 0x46 )
    {
        printf ("fsCheckELFFile: Sig \n");
        return 1;
    }

	// Continua...


    return 0;
}




// Checando a validade de arquivos bin�rios no formato PE.
// #bugbug: Isso n�o deve ficar no kernel, pois PE � propriet�rio.
// Nem usaremos PE no futuro.

int fsCheckPEFile ( unsigned long address ){

    unsigned char *buffer = (unsigned char *) address;

	// #todo: Error message.
    //if ( (void *) address == NULL )
        //return -1;

	//i386
    if ( buffer[0] != 0x4C || buffer[1] != 0x01 )
    {
        printf ("fsCheckPEFile: Sig \n");
        return 1;
    }

	//i486

	//Continua...


    return 0;
}


/*
 *********************************
 * fsListFiles:
 *     Lista os arquivos em um diret�rio, dados os �ndices de disco, 
 * volume e diret�rio.
 */

void 
fsListFiles ( int disk_id, 
              int volume_id, 
              int directory_id )
{
	// @todo: Checar mais limites.

    if ( disk_id == -1 || volume_id == -1 || directory_id == -1 )
    {
        goto fail;
    }

    printf ("fsListFiles: disk={%d} vol={%d} dir={%d}\n", 
        disk_id, volume_id, directory_id );

	// Show!
	// Se o diret�rio selecionado � o diret�rio raiz do VFS.

    if ( current_disk == 0 && current_volume == 0 && current_directory == 0 )
    {
        vfsListFiles ();
        goto done;
    }

	// @todo: Suportar outros diret�rios.
	// ...

    goto done;


fail:
    printf ("fail\n");

done:
    refresh_screen ();
    return;
}
 


/*
int fsIsFAT12();
int fsIsFAT12()
{
   //testa o tipo de fat.
    return 0;
};
*/


/*
int fsIsFAT16();
int fsIsFAT16()
{
    //testa o tipo de fat.
    return 0;
};
*/


/*
int fsIsFAT32();
int fsIsFAT32()
{
    //testa o tipo de fat.
    return 0;
};
*/


/*
 **********************
 * fsList
 *     comando 'dir'.
 *     Listando arquivos em um diret�rio dado o nome. 
 */

int fsList ( const char *dir_name ){

    printf ("fsList:\n");

    if ( current_target_dir.current_dir_address == 0 )
    {
        printf ("current_target_dir.current_dir_address fail, reseting\n");
        refresh_screen ();
        current_target_dir.current_dir_address = VOLUME1_ROOTDIR_ADDRESS; 
    }

    if ( dir_name == 0 )
    {
        dir_name = current_target_dir.name;

        if ( dir_name == 0 ){
            printf ("current_target_dir.name fail\n");
        }
    }


	// name, dir address, number of entries

    fsFAT16ListFiles ( (const char *) dir_name,         
        (unsigned short *) current_target_dir.current_dir_address, 
        256 );


    return 0;
}


/*
 * fsFAT16ListFiles:
 *     Mostra os nomes dos arquivos de um diret�rio.
 *     Sistema de arquivos fat16.
 *
 * IN:
 *     dir_address = Ponteiro para um endere�o de mem�ria 
 *                   onde foi carregado o diret�rio. 
 */
 
void 
fsFAT16ListFiles ( const char *dir_name, 
                   unsigned short *dir_address, 
                   int number_of_entries )
{
	// #todo: 
	// O n�mero de entradas � maior no diret�rio raiz.(512 ?)

    int i = 0;
    int max = number_of_entries;         // N�mero m�ximo de entradas.

    unsigned long j = 0;  // Deslocamento

    unsigned short *DirBaseAddress = (unsigned short *) dir_address;


    if ( number_of_entries <= 0 )
    {
        return;
    }

    printf ("fsFAT16ListFiles: Listing names in [%s]\n\n", dir_name );

	// Mostra.

    i=0; 
    while (i < max)
    {
		// Diferente de vazio.
		if ( DirBaseAddress[j] != 0 )
		{
			//O problema � a termina��o da string '\0'
			printf ("%s\n", &DirBaseAddress[j] );
        } 
		
		//(32/2) pr�xima entrada! (16 words) 512 vezes!
        j += 16;  
        i++;  
    }; 


	//...
	
	//printf ("Done\n");
	refresh_screen();
}




/*
 * get_file:
 *     Obt�m o ponteiro da estrutura do arquivo, dado seu id.
 *     Com um �ndice da lista de arquivos, a fun��o retorna o ponteiro da
 *     estrutura do arquivo. 
 */
 
// void *get_stream (int Index)

void *get_file (int Index){
	
	//Limits.
	//@todo: max.

    if (Index < 0)
    {
        return NULL;
    }


    return (void *) fileList[Index];
}


/*
 * set_file:
 *     Coloca na lista de arquivos o ponteiro de estrutura, dado seu ID. 
 */
 
//void set_stream ( FILE *stream, int Index ){ 

void set_file ( void *file, int Index ){

    if (Index < 0)
    {
        return;
    }

	// #todo:
	// Limite m�ximo da lista.

	// Structure.

    if ( (void *) file == NULL )
    {
        return;
    }

	// Include pointer in the list.


     fileList[Index] = (unsigned long) file;
}


void fs_test_fat_vector (void)
{
	//Ainda n�o implementada.
}


unsigned long fs_get_fat_entry (unsigned long n)
{
    //Ainda n�o implementada.
    return 0; 
}


void fs_set_fat_entry ( unsigned long n, unsigned long value )
{
    //Ainda n�o implementada.
}



void fs_set_entry ( unsigned long id, unsigned long eid ){
	//suspensa
}


void fs_get_entry ( unsigned long id, unsigned long eid ){
	//suspensa
}


void fs_show_dir_entry ( unsigned long id, unsigned long eid ){
    //Ainda n�o implementada.
}


void fs_show_dir (unsigned long id){
	//Ainda n�o implementada.
}


//confere clusers de um diretorio.
unsigned long fs_check_cluster (unsigned long id){
	//Ainda n�o implementada.
	return 0; 
}


/*
 * fs_check_fat:
 *     Check FAT. 
 */
unsigned long fs_check_fat (void){
	
	//Ainda n�o implementada.
	return 1; 
}


void fs_show_entry ( unsigned long id, unsigned long eid ){
	//Ainda n�o implementada.
}


unsigned long 
fs_get_entry_status ( unsigned long id, 
                      unsigned long eid )
{
	//Ainda n�o implementada.
	return 0; 
}


void 
fs_set_entry_status ( unsigned long id, 
                      unsigned long eid, 
                      unsigned long status )
{
	//Ainda n�o implementada.
}


/*
 ***************************************************
 * fsCheckMbrFile: (Essa rotina pode ser usada pra checar se o volume � fat.)
 *     Checamos um mbr carregado em algum endere�o de mem�ria.
 *
 *     Checa o registro do disco. (MBR). (disco do sistema).
 *
 *     Obs: O MBR desse sistema n�o tem o bpb no inicio do primeiro setor e 
 * sim no final do primeiro setor, antes da tabela de parti��es.
 *
 * Endere�o da mem�ria onde o MBR � carregado na inicializa��o. 0x00020000.   
 *
 * @todo: Talvez essa rotina pudesse fornecer o endere�o do buffer como 
 * argumento.
 * @todo: O mbr do sisco do sistema precisa de uma estruura que 
 * coordene o acesso a ele.
 *
 * #todo: precisa colocar tudo na estrutura que est� em fs.h
 */

//void fsCheckMBR(unsigned char* buffer)  //@todo

void fsCheckMbrFile ( unsigned char *buffer ){

	//#todo
	//mudar os argumentos para chamarmos as portas ide.

    int i;
    unsigned char *mbr = (unsigned char *) buffer; 

	//setor 0.
    my_read_hd_sector ( (unsigned long) &mbr[0] , 0, 0 , 0 ); 

	//message:
    //printf ("\n");
    printf ("fsCheckMbrFile: Testing MBR ...\n");

	// @todo:
	// Checar uma estrutura do mbr do disco do sistema,
	// para validar o acesso � ele.	

	// Check signature.
    if ( mbr[0x1FE] != 0x55 || mbr[0x1FF] != 0xAA )
    {
        printf ("fsCheckMbrFile: Sig FAIL \n" );
        goto fail;
    }

	//jmp
    printf ("JMP: [ %x ", mbr[ BS_JmpBoot + 0 ] );
    printf ("%x ", mbr[ BS_JmpBoot + 1 ] );
    printf ("%x ]\n", mbr[ BS_JmpBoot + 2 ] );


	//name
    printf ("OS name: [ ");
    for ( i=0; i<8; i++ )
    {
        printf ("%c", mbr[ BS_OEMName + i ] );
    };
    printf (" ]\n");


	//...


    printf ("Signature: [ %x %x ] \n" , mbr[0x1FE], mbr[0x1FF] );

	// Continua ...
    goto done;

fail:
    printf ("Fail\n");

done:
    printf ("Done\n");
    refresh_screen ();
    return;
}


/*
 * fsCheckVbr:
 *     Checa o registro de boot de um volume.
 *     Vai no endere�o onde est� armazenado o VBR do volume atual
 *     e confere as informa��es sobre o volume.
 */

void fsCheckVbrFile ( unsigned char *buffer ){

    unsigned char *vbr = (unsigned char *) buffer; 

	//#todo: check address validation.

	// #todo:
	// Checar uma estrutura do mbr do disco do sistema, para validar o 
	// acesso � ele.

	// Check signature.

    if ( vbr[0x1FE] != 0x55 || vbr[0x1FF] != 0xAA )
    {
        printf ("fsCheckVbrFile: Sig Fail\n");
        goto fail;
    }


	//
	// Continua ...
	//

    goto done;

fail:
    printf ("fsCheckVbrFile: fail\n");

done:
    printf ("Done\n");
    refresh_screen();
    return;
}


/*
 **************************************************
 * MountShortFileName:
 *     This function parses a directory entry name 
 * which is in the form of "FILE   EXT" and puts it in Buffer 
 * in the form of "FILE.TXT".
 *
 * @todo fsMountShortFileName(...)
 */
 
void 
MountShortFileName ( char *buffer, 
                     struct dir_entry_d *entry )
{
    int i=0;

    // Get the file name.
    while (i < 8)
    {
        if ( entry->FileName[i] == ' ')
        {
            break;
        }

        buffer[i] = entry->FileName[i];
        i++;
    }


    // Get extension.
    if ( (entry->FileName[8] != ' ') )
    {
        buffer[i++] = '.';
        buffer[i++] = (entry->FileName[ 8] == ' ') ? '\0' : entry->FileName[ 8];
        buffer[i++] = (entry->FileName[ 9] == ' ') ? '\0' : entry->FileName[ 9];
        buffer[i++] = (entry->FileName[10] == ' ') ? '\0' : entry->FileName[10];
    }
}


/*
 * set_spc:
 *     Configura spc, 'Sector Per Cluster' em vari�vel global.
 *     ?? #bugbug: De qual disco ?? 
 */
 
void set_spc (int spc)
{
    g_spc = (int) spc;
}


/*
 * get_spc:
 *     Pega spc, Sector Per Cluster.
 *     ?? #bugbug: De qual disco ?? 
 */

int get_spc (void)
{
    return (int) g_spc;
}


/*
 * get_filesystem_type:
 *     Pega o tipo de sistema de arquivos.
 *     ?? #bugbug: De qual volume ??  
 */

int get_filesystem_type (void)
{
    return (int) g_filesystem_type;
}


/*
 * set_filesystem_type:
 *     Configura o tipo de sistema de arquivo.
 *     ?? #bugbug: De qual volume ?? 
 */

void set_filesystem_type (int type)
{
    g_filesystem_type = (int) type;
}


/*
 ***********************************************
 * fs_init_fat:
 *     Inicializa a estrutura usada no sistema de arquivos.
 *     fsInitFat()
 *     ?? #bugbug: De qual volume ?? 
 */

// #todo: Usar tipo 'int'.

void fs_init_fat (void){
	
	// File system structure.
	
    if ( (void *) filesystem == NULL )
    {
        printf ("fs_init_fat error: filesystem\n");
        return;
    }

	// FAT structure.

    fat = (void *) kmalloc ( sizeof(struct fat_d) );

    if ( (void *) fat == NULL )
    {
        printf ("fs_init_fat error: fat\n");
        return;

    }else{

        // Info.

        fat->address = filesystem->fat_address; 
        fat->type = filesystem->type;
		//Continua ...
    };


	// #bugbug
	// N�o fizemos nada com a estrutura 'fat'
	// tem que passar esse ponteiro para algum lugar.


	// Continua a inicializa��o da fat.
}


/*
 ******************************************************
 * fs_init_structures:
 *     Inicializa a estrutura do sistema de arquivos.
 *     fsInitStructures
 */

// #todo: Usar tipo 'int'.

void fs_init_structures (void){
	
    int Type;
	
    filesystem = (void *) kmalloc ( sizeof(struct filesystem_d) );

    if ( (void *) filesystem == NULL )
    {
        panic ("fs_init_structures: filesystem\n");

    }else{
        storage->fs = filesystem;
        //...
    };


	//Type.
	//#bugbug: 
    //Em qual dico e volume pegamos o tipo de sistema de arquivos.

    Type = (int) get_filesystem_type ();   

    if ( Type == 0 )
    {
        panic ("fs_init_structures error: Type");

    }else{

        //if ( (void *) filesystem == NULL )
            filesystem->type = (int) Type;
    };


    switch (Type)
    {
        case FS_TYPE_FAT16:

			//Rootdir.
			filesystem->rootdir_address = VOLUME1_ROOTDIR_ADDRESS;
	        filesystem->rootdir_lba = VOLUME1_ROOTDIR_LBA;
	        
			//Fat.
			filesystem->fat_address = VOLUME1_FAT_ADDRESS;
	        filesystem->fat_lba = VOLUME1_FAT_LBA;
	        
			//Dataarea.
			//filesystem->dataarea_address = ??;
	        filesystem->dataarea_lba = VOLUME1_DATAAREA_LBA;
	        
			//sectors per cluster.
			filesystem->spc = (int) get_spc(); //vari�vel
	        filesystem->rootdir_entries = FAT16_ROOT_ENTRIES;
	        filesystem->entry_size = FAT16_ENTRY_SIZE;
			
		    break;
			
	    case FS_TYPE_EXT2:
		    //nothing for now.
		    break;
		
		//case FS_TYPE_EXT3: break;
		
        default:
		    //nothing for now.
            break;
    };
}


/*
 ********************************************************
 * fsInit:
 *     Inicializa o file system manager.
 */
 
int fsInit (void){


//#ifdef EXECVE_VERBOSE
   // printf ("fsInit: Initializing..\n");
//#endif 


    debug_print ("fsInit:\n");

	// Type - Configura o tipo de sistema de arquivos usado. 
	// No caso, (fat16).
	//
	// @todo: Deve-se checar o volume ativo e ver qual sistema de arquivos est�
	//        sendo usado, ent�o depois definir configurar o tipo.
	//        O sistema operacional pode salvar o tipo usado. Nesse caso 
	//        apenas checar se ouve altera��es nas configura��es de sistema de arquivos.
	//        O registro de configura��es de disco pode ser armazenado em arquivos de metadados.

    set_filesystem_type (FS_TYPE_FAT16);


	// SPC 
	// Configura o n�mero de setores por cluster.
	// Nesse caso, s�o (512 bytes por setor, um setor por cluster).

	set_spc (1);


	// ## initialize currents ##


	//selecionando disco, volume e diret�rio.
	//estamos resetando tudo e selecionando o diret�rio raiz 
	//do vfs ... mas na verdade o diret�rio selecionado 
	//deveria ser o diret�rio onde ficam a maioria dos aplicativos.
	//para que o usu�rio possa chamar o maior n�mero de apps usando 
	//apenas comandos simples.
	//#bugbug: isso deveria se passado pelo boot ??	


	//#bugbug: 
	//Deixaremos cada m�dulo inicializar sua vari�vel.
	//Mas aqui podemos zerar esses valores.
	
	//current_disk = 0;
	//current_volume = 0;   
	//current_directory = 0;

	// Structures and fat.

//#ifdef EXECVE_VERBOSE
	//printf("fsInit: Structures..\n");
//#endif

    fs_init_structures ();


//#ifdef EXECVE_VERBOSE
	//printf("fsInit: FAT..\n");
//#endif

    fs_init_fat ();


	//
	// ==================== ## fileList ## =========================
	//

	// Agora inicialzamos as stream 4 e 5.
	// As anteriores foram inicializadas em stdio,
	// pois s�o o fluxo padr�o.
	
	
	//
    //  ## volume 1 root dir  ##
	//
	
	
	//foi definido em stdio.h
	//FILE *volume1_rootdir;

    volume1_rootdir = (FILE *) kmalloc ( sizeof(FILE) );

    if ( (void *) volume1_rootdir == NULL )
    {
        panic ("fsInit: volume1_rootdir \n");

    } else {

        volume1_rootdir->used = 1;
        volume1_rootdir->magic = 1234;

        volume1_rootdir->_base = (unsigned char *) VOLUME1_ROOTDIR_ADDRESS;
        volume1_rootdir->_p = (unsigned char *) VOLUME1_ROOTDIR_ADDRESS;
        volume1_rootdir->_cnt = (32 * 512) ;
        volume1_rootdir->_file = 0; //?
        volume1_rootdir->_tmpfname = "volume1-stream";

        fileList[__KERNEL_STREAM_VOL1_ROOTDIR] = (unsigned long) volume1_rootdir;

        //#bugbug: Validade da estrutura.
        storage->stream = volume1_rootdir; 
    };


	//
	//  ## volume 2 root dir  ##
	//
	
	
	//foi definido em stdio.h
	//FILE *volume2_rootdir;

    volume2_rootdir = (FILE *) kmalloc ( sizeof(FILE) );

    if ( (void *) volume2_rootdir == NULL )
    {
        panic ("fsInit: volume2_rootdir\n");

    }else{

        volume2_rootdir->used = 1;
        volume2_rootdir->magic = 1234;

        volume2_rootdir->_base = (unsigned char *) VOLUME2_ROOTDIR_ADDRESS;
        volume2_rootdir->_p = (unsigned char *) VOLUME2_ROOTDIR_ADDRESS;
        volume2_rootdir->_cnt = (32 * 512) ;
        volume2_rootdir->_file = 0; //?
        volume2_rootdir->_tmpfname = "volume2-stream";

        fileList[__KERNEL_STREAM_VOL2_ROOTDIR] = (unsigned long) volume2_rootdir;
    };


	//
	// ## Inicializando os pipes usados em execve ## 
	//

	
	//gramado core init execve 
	
	//aloca mem�ria para a estrutura.
    pipe_gramadocore_init_execve = (FILE *) kmalloc ( sizeof(FILE) );
	
    if ( (void *) pipe_gramadocore_init_execve == NULL )
    {
        panic ("fsInit: pipe_gramadocore_init_execve\n");

    }else{

        //aloca mem�ria para o buffer.
        unsigned long pipe0base = (unsigned long) kmalloc (512);

        if ( (void *) pipe0base == NULL )
        {
            panic ("fsInit: pipe0base\n");
        }

        pipe_gramadocore_init_execve->used = 1;
        pipe_gramadocore_init_execve->magic = 1234;

        pipe_gramadocore_init_execve->_base = (unsigned char *) pipe0base;
        pipe_gramadocore_init_execve->_p = (unsigned char *) pipe0base;
        pipe_gramadocore_init_execve->_cnt  = 512;
        pipe_gramadocore_init_execve->_file = 0; //??
        pipe_gramadocore_init_execve->_tmpfname = "pipe0";
        
        
        // #todo
        //fileList[ ? ] = (unsigned long) pipe_gramadocore_init_execve;

	    //0
        Pipes[0] = (unsigned long) pipe_gramadocore_init_execve;
    };


	//
	// ## PWD ##
	//
	
	//inicializa p pwd support.
    fsInitializeWorkingDiretoryString ();
	
	//
	// ## target dir struct ##
	//
	
	//inicializa a estrutura de suporte ao target dir.
    fsInitTargetDir ();

	//
	// @todo: Continua ...
	//


//done:

//#ifdef EXECVE_VERBOSE
    //printf("Done\n");
//#endif 

    return 0;
}


/*
 *****************************************
 * fsInitializeWorkingDiretoryString:
 *     Atualiza a string do diret�rio de trabalho.
 * Essa � a string que ser� mostrada antes do prompt.
 * 'pwd'> 
 * ?? isso deve sser todo o pathname do pwd ?? 
 * ex: root:/volume0>
 */
 
void fsInitializeWorkingDiretoryString (void){

    struct volume_d *v;

	// root:/volumeX
    char volume_string[8];   

	volume_string[0] = 'v';
	volume_string[1] = 'o';
	volume_string[2] = 'l';
	volume_string[3] = 'u';
	volume_string[4] = 'm';
	volume_string[5] = 'e';
	volume_string[6] = (char)( '1' + (char) current_volume - (char) 1 );
	volume_string[7] = '\0';


	//'root:'
	//  ## volume list ##
	//primeiro colocamos a string que indica a lista de volumes. 

    sprintf ( current_workingdiretory_string, FS_ROOT_STRING ); 

	//'/'
	// ## separador ##
	strcat ( current_workingdiretory_string, FS_PATHNAME_SEPARATOR );


	//
	//  ## volume root dir ##
	//

    v = (struct volume_d *) volumeList[current_volume];

    if ( (void *) v == NULL )
    {
        panic ("fsInitializeWorkingDiretoryString: v\n");

    }else{

        if ( v->used != 1 || v->magic != 1234 )
        {
            panic ("fsInitializeWorkingDiretoryString: validation\n");
        }

        switch (v->id)
        {
			//case 0:
		        //global usada para string do nome do volume.
			//    current_volume_string = (char *) FS_VOLUME0_STRING;
			//    break;

			//case 1:
		        //global usada para string do nome do volume.
			//   current_volume_string = (char *) FS_VOLUME1_STRING;
			//   break;
			
            default:
		        //fail.
		        //printf("fsInitializeWorkingDiretoryString: default volume #todo\n");
		        //die();
			    current_volume_string = (char *) volume_string; 
                break;
        };


		//path string na estrutura do volume.
        sprintf ( v->path_string, current_volume_string );

	    //'volumeX'
        strcat ( current_workingdiretory_string, v->path_string );
	    //strcat ( current_workingdiretory_string, current_volume_string );
    };


	// ## separador ##
    strcat ( current_workingdiretory_string, FS_PATHNAME_SEPARATOR );

	//More ?...
    pwd_initialized = 1;
}


/*
 ***************************
 * fsInitTargetDir:
 *     Para inicializarmos o sistema ja' com um alvo, no caso o root dir. 
 */

void fsInitTargetDir (void){
	
	current_target_dir.current_dir_address = VOLUME1_ROOTDIR_ADDRESS;
	//current_target_dir.name = ?;
}


/*
 **************************************
 * fs_initialize_process_pwd:
 *     Cada processo deve inicialiar seus dados aqui. 
 */

int fs_initialize_process_pwd ( int pid, char *string ){

    int i;
    struct process_d *p;

    if ( pwd_initialized == 0 )
    {
        panic ("fs_initialize_process_pwd: pwd not initialized\n"); 
    } 


    if (pid<0)
        return 1;


    if ( (void *) string == NULL )
        return 1; 


	// #importante
	// Vamos copiar a string para a estrutura do processo atual.

    p = (struct process_d *) processList[pid];

    if ( (void *) p == NULL )
    {
        panic ("fs_initialize_process_pwd: p\n");

    }else{

        if ( p->used != 1 || p->magic != 1234 )
        {
            panic ("fs_initialize_process_pwd: validation\n");
        }

        for ( i=0; i<32; i++ )
        {
            p->pwd_string[i] = string[i];
        }
    };


    return 0;
}


/*
 * fs_print_process_pwd
 *     Cada processo tem seu pr�prio pwd.
 *     Essa rotina mostra o pathname usado pelo processo. 
 */

int fs_print_process_pwd (int pid){

    struct process_d *p;

    if ( pwd_initialized == 0 )
    {
        panic ("fs_print_process_pwd: pwd not initialized\n"); 

    }


    if (pid<0)
        return 1;


    p = (struct process_d *) processList[pid];

    if ( (void *) p == NULL )
    {
        panic ("fs_print_process_pwd: p\n");

    }else{

        if ( p->used != 1 || p->magic != 1234 )
        {
            panic ("fs_print_process_pwd: validation\n");
        }

        printf ("\n PID=%d %s (%s) \n\n", p->pid, 
            p->pwd_string, current_target_dir.name );

        refresh_screen ();
        return 0;
    };


    //fail.
    return -1;
}


/*
 *****************************************
 * fsUpdateWorkingDiretoryString:
 *     +Atualiza o pathname na estrutura do processo atual.
 *     +Atualiza o pathname na string global. 
 */ 
 
void fsUpdateWorkingDiretoryString ( char *string ){
	
	int i;    
	struct process_d *p;
	
	char *tmp;
	
	tmp = string;

	
	if ( pwd_initialized == 0 )
	{
		printf ("fsUpdateWorkingDiretoryString: pwd not initialized\n"); 
		return;
	}


    if ( (void *) string == NULL )
	    return;  


	p = (struct process_d *) processList[current_process];

    if ( (void *) p == NULL )
    {
        panic ("fsUpdateWorkingDiretoryString: p\n");

    }else{

        if ( p->used != 1 || p->magic != 1234 )
        {
            panic ("fsUpdateWorkingDiretoryString: validation\n");
        }

        if ( (void *) string == NULL )
	    {
			printf ("fsUpdateWorkingDiretoryString: string\n");
		    return;

		} else {


			//#importante
			//Colocamos um novo nome no fim do path;
			//atualiza a string do processo atual.
			
            strcat ( p->pwd_string, string );

	        // ## separador ##
            strcat ( p->pwd_string, FS_PATHNAME_SEPARATOR );

	        //atualiza a string global.
			//usando a string do processo atual.

			for ( i=0; i<32; i++ )
			{
				current_workingdiretory_string[i] = p->pwd_string[i];
			}
			
			//name
			
			for ( i=0; i< 11; i++ )
			{
				current_target_dir.name[i] = *tmp;
				tmp++;
			}
		}
	}
}


/* 
 ************************************************
 * fs_pathname_backup:
 *     Remove n nomes de diret�rio do pathname do processo indicado no 
 * argumento.
 *     Copia o nome para a string global.
 *     Remove the last N directories from PATH.  
 *     Do not leave a blank path.
 *     PATH must contain enough space for MAXPATHLEN characters. 
 *     #obs: O PID costuma ser do processo atual mesmo. 
 *     Credits: bash 1.05 
 */
 
void fs_pathname_backup ( int pid, int n ){
	
	int i;
	struct process_d *p;
		
	if ( pwd_initialized == 0 )
	{
		printf ("fs_pathname_backup: pwd not initialized\n"); 
		return;
	}  

	if ( pid<0 )
	{
		printf ("fs_pathname_backup: PID\n"); 
		return;
	}
	
	p = (struct process_d *) processList[pid];
	
    if ( (void *) p == NULL )
    {
        panic ("fsUpdateWorkingDiretoryString: p\n");

    }else{

        if ( p->used != 1 || p->magic != 1234 )
        {
            panic ("fsUpdateWorkingDiretoryString: validation\n");
        }

        char *path = (char *) p->pwd_string;

        register char *s = path + strlen( path );
 
        if (*path)
            s--;

        while (n--)
        {
            while (*s == '/')
                s--;

            while (*s != '/')
                s--;

            *++s = '\0';
        };

		//atualizando a string global.
		for ( i=0; i<32; i++ )
		{
			current_workingdiretory_string[i] = p->pwd_string[i];
		}


		// name.
		for ( i=0; i< 11; i++ )
		{
			current_target_dir.name[i] = '\0';
		}
    };
}


/*
 * sys_read_file:
 *     Interface para carregar arquivo ou diret�rio.
 *     Essa rotina � chamada por services em services.c
 *     Essa rotina n�o � mais usara. (backup),
 *     Agora estamos usando sys_read_file2.
 *     #suspensa. #deletar;
 */

int sys_read_file ( unsigned long name, unsigned long address ){

    int Ret = -1;


	//#bugbug
	//Estamos usando o diret�rio raiz.
	//a rotina sys_read_file2 carrega do diret�rio alvo.


    taskswitch_lock ();
    scheduler_lock ();


    Ret = (int) fsLoadFile ( VOLUME1_FAT_ADDRESS,  
                    VOLUME1_ROOTDIR_ADDRESS, 
                    (unsigned char *) name, 
                    (unsigned long) address ); 


    scheduler_unlock ();
    taskswitch_unlock ();

    return (int) Ret;


fail:
    return 1;
}


/*
 **************************
 * sys_read_file2
 *     carrega do diret�rio alvo. 
 */

FILE *sys_read_file2 ( unsigned long name, unsigned long address ){
 
    FILE *stream;
    int i;
    int Ret = -1;
    unsigned long new_address;


    stream = (FILE *) kmalloc ( sizeof(FILE) );

    if ( (void *) stream == NULL )
    {
	     return (FILE *) 0;

    }else{

		 //stream->_base = (char *) address;
		 //stream->_ptr = stream->_base;
		 //stream->_cnt = 0;
		 //stream->_flag = 0;
		 //...
		 
	     //printf("sys_read_file2: struct ok \n");
		 //printf("base=%x \n",stream->_base);
		 //printf("ptr=%x  \n",stream->_ptr);
		
	 };
	

	 //#importante, 
	 //A atualiza�ao do nome � feita aqui.

	//update name.
    fsUpdateWorkingDiretoryString ( (char *) name );


	//#importante
	//temos que respeitar o endere�o passaro pelo usu'ario.
	
	new_address =  address;

    if ( new_address == 0 )
    {
        printf ("sys_read_file2: address fail\n");
		//return -1;

        return (FILE *) 0;
    }


	//#bugbug
	//tenta carregar o diret'orio que tem o endere�o indicado aqui, 
	//se falhar carregue o root por enquanto.

	if ( current_target_dir.current_dir_address == 0 )
	{
	    printf ("sys_read_file2: current_target_dir.current_dir_address fail \n");
		
		//reset.
		current_target_dir.current_dir_address = VOLUME1_ROOTDIR_ADDRESS;
		
		for ( i=0; i< 11; i++ )
		{
			current_target_dir.name[i] = '\0';
		}		
		
		//return -1;
		return (FILE *) 0;
	}


	//#debug
	//printf ("sys_read_file2: dir_name=(%s) dir_addr=(%x) #debug \n",
	//    current_target_dir.name, current_target_dir.current_dir_address );



	size_t s = (size_t) fsGetFileSize ( (unsigned char *) name );


    stream->_base = (char *) new_address;
    stream->_p  = (char *) new_address;
    stream->_cnt = s;

    stream->_file = 0;
    stream->_tmpfname = (char *) name;

	//#debug
    //printf ("sys_read_file2: struct ok \n");
    printf ("base=%x \n", stream->_base);
    printf ("ptr=%x  \n", stream->_p);


    //++
    taskswitch_lock ();
    scheduler_lock ();

    Ret = (int) fsLoadFile ( VOLUME1_FAT_ADDRESS,  
                    current_target_dir.current_dir_address,    //src dir address 
                    (unsigned char *) current_target_dir.name, 
                    (unsigned long) new_address );             //dst dir address

    scheduler_unlock ();
    taskswitch_unlock ();
    //--


	// Veremos se o carregamento funcionou ou n�o.

	if ( Ret == 0)
	{
		printf ("sys_read_file2: done\n");

        current_target_dir.current_dir_address = new_address;
        return (FILE *) stream;

    }else{

		current_target_dir.current_dir_address = 0;
		//fclose(stream);

        return (FILE *) 0;
    };


    return (FILE *) 0;
}


/*
 ********************************
 * fsLoadFileFromCurrentTargetDir:
 *     Carrega o diret�rio que est� configurado como target dir 
 * em algum lugar qualquer da mem�ria. 
 */

int fsLoadFileFromCurrentTargetDir (void){
	
	int Ret = -1;
	int i;
	
	unsigned long new_address;
	
	//#bugbug
	//Isso 'e um limite para o tamanho do arquivo (apenas dir).
	//precisamos expandir isso.
	//aqui no m'aquimo o arquivo pode ter 4kb.
	//acho ques estamos falando somente de diret'orio aqui.
	
	new_address = (unsigned long) kmalloc (4096);
	
	if ( new_address == 0 )
	{
		return -1;
	}
	
	//#bugbug
	//tenta carregar o diret'orio que tem o endere�o indicado aqui, 
	//se falhar carregue o root por enquanto.
	
	if ( current_target_dir.current_dir_address == 0 )
	{
	    printf("fsLoadCurrentTargetDir current_target_dir.current_dir_address fail \n");

		//reset.
		current_target_dir.current_dir_address = VOLUME1_ROOTDIR_ADDRESS;
		
		for ( i=0; i< 11; i++ )
		{
			current_target_dir.name[i] = '\0';
		}

		return -1;
	}
	
	//#debug
	//printf ("fsLoadFileFromCurrentTargetDir: dir_name=(%s) old_dir_addr=(%x) #debug \n",
	//    current_target_dir.name, current_target_dir.current_dir_address );


    //++
    taskswitch_lock ();
    scheduler_lock ();

    Ret = (int) fsLoadFile ( VOLUME1_FAT_ADDRESS,  
                    current_target_dir.current_dir_address,    //src dir address 
                    (unsigned char *) current_target_dir.name, 
                    (unsigned long) new_address );             //dst dir address

    scheduler_unlock ();
    taskswitch_unlock ();
    //--

    current_target_dir.current_dir_address = new_address;

	//#debug
	//printf ("fsLoadFileFromCurrentTargetDir: dir_name=(%s) new_dir_addr=(%x) #debug \n",
	//   current_target_dir.name, current_target_dir.current_dir_address );


    return (int) Ret;
}



/*
 * sys_write_file:
 *     Interface para salvar arquivo ou diret�rio.
 *     Isso pode ser usado para criar um diret�rio ou copiar um diret�rio. 
 */

	//#todo:
	//vamos fazer igual ao sys_read_file 
	//e criarmos op��es ... se poss�vel.

int
sys_write_file ( char *file_name, 
                 unsigned long file_size,
                 unsigned long size_in_bytes,
                 char *file_address,
                 char flag )  
{
    int Ret = -1;

    //++
    taskswitch_lock ();
    scheduler_lock ();

    Ret = (int) fsSaveFile ( (char *) file_name,    //name
                    (unsigned long) file_size,      //3, //@todo: size in sectors 
                    (unsigned long) size_in_bytes,  //255, //@todo: size in bytes
                    (char *) file_address,          //arg3,//address
                    (char) flag );                  //,arg4 ); //flag

    scheduler_unlock ();
    taskswitch_unlock ();
    //--


    return Ret;
}


//#todo
//credits: Chicago OS.
/*
int fs_count_separations (char *path)
int fs_count_separations (char *path){

    int count = 0;

    int i;
    int __len = 0;


    __len = (int) strlen ( (const char *) path );
    
    for ( i=0; i < __len; i++ )
    {
		if (path[i] == '/')
		{
			count++;
		}
    };


    return (int) count;
}
*/

//
// End.
//

