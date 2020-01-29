/*
 * File: execve\fs\fs.h 
 *
 * Obs: Dentro do kernel base somente teremos primitivas
 *      Opera��es complexas devem ir para servidores utilit�rios.
 *
 * Descri��o:
 *     Header para o File System Manager. (FSM)
 *     File system header for kernel file management.
 *     Inicialmente suporte ao sistema de arquivos FAT16.
 *     Posteriormente suporte aos sistemas FAT12, FAT32, EXT2.     
 *
 * History:
 *     2014 - Created by Fred Nora.
 *     2015 - Revision.
 */
 

//suporte ao diret�rio alvo que desejamos acessar.
//com base em um pathname passado via argumento.
struct target_dir_d
{
    int used;
    int magic;
	
	//ponteiro para a string do caminho
	//char *pwd_string;  
	
	//file name 8.3 (11 bytes;)
	char name[32];
	
	//onde esse direto'rio esta' carregado.
    unsigned long current_dir_address;
		
	//...
};
struct target_dir_d current_target_dir;
 
 
#define FS_PATHNAME_SEPARATOR "/"
#define FS_PATHNAME_TERMINATOR "\0"

#define FS_STRING_TERMINATOR "\0" 
//
// Volume support
//

//Volume list
#define FS_ROOT_STRING        "root:"
//#define FS_VOLUMELIST_STRING  "root:"  //deletar

//volumes
#define FS_UNKNOWNVOLUME_STRING "unknown-volume"
#define FS_VOLUME0_STRING "volume0"   //vfs
#define FS_VOLUME1_STRING "volume1"   //boot volume
#define FS_VOLUME2_STRING "volume2"   //system volume.

//...

char *current_volume_string;
//int current_volume_id; //DEFINIDO NO KERNEL.H COMO current_volume.



//
// pwd support
//


#define FS_UNKNOWNWORKINGDIRECTORY_ID (-1)
#define FS_VFSWORKINGDIRECTORY_ID     0
#define FS_BOOTWORKINGDIRECTORY_ID    1
#define FS_SYSTEMWORKINGDIRECTORY_ID  2


#define FS_VFSWORKINGDIRECTORY_STRING     "volume1"
#define FS_BOOTWORKINGDIRECTORY_STRING    "volume1"
#define FS_SYSTEMWORKINGDIRECTORY_STRING  "volume2"
#define FS_UNKNOWNWORKINGDIRECTORY_STRING "unknown-directory"

#define WORKINGDIRECTORY_STRING_MAX 32
// a stringo do diret�rio de trabalho.
char current_workingdiretory_string[WORKINGDIRECTORY_STRING_MAX];
int pwd_initialized;

 
//atualiza a string do pwd na estrutura do processo. 
int fs_initialize_process_pwd ( int pid, char *string ); 
int fs_print_process_pwd ( int pid );





/*
 *****************************************
 * fsInitializeWorkingDiretoryString:
 *     Atualiza a string do diret�rio de trabalho.
 * Essa � a string que ser� mostrada antes do prompt.
 * 'pwd'> 
 * ?? isso deve sser todo o pathname do pwd ?? 
 * ex: root:/volume0>
 */
void fsInitializeWorkingDiretoryString(); 

/*
 *****************************************
 * fsUpdateWorkingDiretoryString:
 *     +Atualiza o pathname na estrutura do processo atual.
 *     +Atualiza o pathname na string global.
 */ 
void fsUpdateWorkingDiretoryString ( char *string );

/* 
 ************************************************
 * fs_pathname_backup:
 *     Remove n nomes de diret�rio do pathname do processo indicado no argumento.
 *     Copia o nome para a string global.
 *
 *     Remove the last N directories from PATH.  
 *     Do not leave a blank path.
 *     PATH must contain enough space for MAXPATHLEN characters. 
 *     #obs: O PID costuma ser do processo atual mesmo. 
 *
 * Credits: bash 1.05
 */
void fs_pathname_backup ( int pid, int n ); 		


//essa rotina � chamada por services em services.c
int sys_read_file ( unsigned long name, unsigned long address );


FILE *sys_read_file2 ( unsigned long name, unsigned long address );

				
int
sys_write_file ( char *file_name, 
                 unsigned long file_size,
                 unsigned long size_in_bytes,			
                 char *file_address,
                 char flag );
 
//
// #BUGBUG
// #importante:
// lembrando que o kernel come�a em 0x00100000
// @todo: Podemos encontrar um novo lugar para isso tudo.
// 
 
//
//  ## MBR SUPPORT  ##
//

#define MBR_LBA               0           // mbr


//
//  ## VBR SUPPORT  ##
//
       
#define VOLUME1_VBR_LBA       63
#define VOLUME2_VBR_LBA       32000


//
//  ## FAT1 SUPPORT  ##
//

#define VOLUME1_FAT_LBA       67 
#define VOLUME2_FAT_LBA       33000  


//
//  ## FAT2 SUPPORT  ##
//

// N�o usada !!

//
//  ## ROOT DIR SUPPORT  ##
//

#define VOLUME1_ROOTDIR_LBA     559
#define VOLUME2_ROOTDIR_LBA     34000

 
//
//  ##  DATA AREA SUPPORT  ##
//

#define VOLUME1_DATAAREA_LBA   591 
#define VOLUME2_DATAAREA_LBA   35000

//
// ============ Path support ======================
//

// Diret�rio raiz dos volumes principais.
#define FS_DIR_VFS    'root:/volume0'  // raiz do vfs
#define FS_DIR_BOOT   'root:/volume1'  // raiz do volume de boot
#define FS_DIR_SYSTEM 'root:/volume2'  // raiz do volume do sistema
//...

// Arquivos no volume de boot.
// Obs: O volume de boot s� ter� o diret�rio raiz.
#define FS_DIR_BM      'root:/volume1/BM.BIN'       // boot manager
#define FS_DIR_BL      'root:/volume1/BL.BIN'       // boot loader
#define FS_DIR_KERNEL  'root:/volume1/KERNEL.BIN'   // kernel
#define FS_DIR_INIT    'root:/volume1/INIT.BIN'     // init
#define FS_DIR_SHELL   'root:/volume1/SHELL.BIN'    // shell
#define FS_DIR_TASKMAN 'root:/volume1/TASKMAN.BIN'  // task manager
#define FS_DIR_INITTXT 'root:/volume1/INIT.TXT'     // init txt
//...  


// Diret�rios no volume do sistema. 
#define FS_DIR_BIN          'root:/volume2/bin'          // binary programs.
#define FS_DIR_DEV          'root:/volume2/dev'          // i/o devices.
#define FS_DIR_LIB          'root:/volume2/lib'          // libraries
#define FS_DIR_USER         'root:/volume2/user'         // users
#define FS_DIR_DEFAULTUSER  'root:/volume2/user/default' // default user


#define FS_MAX_NAME_LENGHT (8+3)   // Max name lenght.   8.3
#define PS1                '/'     // Primary path separator.
#define PS2                '\\'    // Alternate path separator.
//...
 
//
// Types. (@todo: Criar defini��es de tipos).
// 

#define FS_TYPE_VFS   1000  // Designativo gen�rico.  
#define FS_TYPE_GVFS  1001  // Gramado VFS.   
//...

#define FS_TYPE_FAT   2000  // Designativo gen�rico.
#define FS_TYPE_FAT12 2001 
#define FS_TYPE_FAT16 2002 
#define FS_TYPE_FAT32 2003 
//...

#define FS_TYPE_MINIXFS  3000  //Minix FS
#define FS_TYPE_EXT      3001
#define FS_TYPE_EXT2     3002
#define FS_TYPE_EXT3     3003
#define FS_TYPE_EXT4     3004
//... 
 
//... 
 


 
#define FAT16_ROOT_ENTRIES  512  //512 entradas no rootdir. 
#define FAT16_ENTRY_SIZE    32   //tamanho em bytes da entrada no root dir. 
 
#define INIT_SECTOR 20 //??

 
/*
 * FAT constants.
 */
 
#define FAT_ERRO_1 0xffff9  //nao ha entradas vazias. 
#define FAT_ERRO_2 0xffff8   
#define FAT_ERRO_3 0xffff7
#define FAT_ERRO_4 0xffff6

#define FAT_STATUS_1 0xffff5  // NAO ha mais entradas cheias.
#define FAT_STATUS_2 0xffff4
#define FAT_STATUS_3 0xffff3
#define FAT_STATUS_4 0xffff2


//primeiro char do nome do arquivo na entrada de diretorio
//no sistema de arquivos fat16.
#define ENTRY_NEVER_USED       0x00
#define ENTRY_FILE_IS_DELETED  0xE5 
#define ENTRY_DIRECTORY        0x2E 
//#define ENTRY_


//
//file atribute
//
#define FILE_ATTRIBUTE_READ_ONLY     0x01 
#define FILE_ATTRIBUTE_HIDDEN        0x02
#define FILE_ATTRIBUTE_SYSTEM        0x04
#define FILE_ATTRIBUTE_VOLUME_LABEL  0x08
#define FILE_ATTRIBUTE_DIRECTORY     0x10
#define FILE_ATTRIBUTE_ARCHIVE       0x20
#define FILE_ATTRIBUTE_UNUSED1       0x40
#define FILE_ATTRIBUTE_UNUSED2       0x80
 

/*
    IMAGE_FILE_MACHINE_I386 (0x14c)
        for Intel 80386 processor or better

    0x014d
        for Intel 80486 processor or better

    0x014e
        for Intel Pentium processor or better

    0x0160
        for R3000 (MIPS) processor, big endian

    IMAGE_FILE_MACHINE_R3000 (0x162)
        for R3000 (MIPS) processor, little endian

    IMAGE_FILE_MACHINE_R4000 (0x166)
        for R4000 (MIPS) processor, little endian

    IMAGE_FILE_MACHINE_R10000 (0x168)
        for R10000 (MIPS) processor, little endian

    IMAGE_FILE_MACHINE_ALPHA (0x184)
        for DEC Alpha AXP processor

    IMAGE_FILE_MACHINE_POWERPC (0x1F0)
        for IBM Power PC, little endian
		
// 1 byte aligned
struct PeHeader {
	uint32_t mMagic; // PE\0\0 or 0x00004550
	uint16_t mMachine;
	uint16_t mNumberOfSections;
	uint32_t mTimeDateStamp;
	uint32_t mPointerToSymbolTable;
	uint32_t mNumberOfSymbols;
	uint16_t mSizeOfOptionalHeader;
	uint16_t mCharacteristics;
}; 
 
// 1 byte aligned
struct Pe32OptionalHeader {
	uint16_t mMagic; // 0x010b - PE32, 0x020b - PE32+ (64 bit)
	uint8_t  mMajorLinkerVersion;
	uint8_t  mMinorLinkerVersion;
	uint32_t mSizeOfCode;
	uint32_t mSizeOfInitializedData;
	uint32_t mSizeOfUninitializedData;
	uint32_t mAddressOfEntryPoint;
	uint32_t mBaseOfCode;
	uint32_t mBaseOfData;
	uint32_t mImageBase;
	uint32_t mSectionAlignment;
	uint32_t mFileAlignment;
	uint16_t mMajorOperatingSystemVersion;
	uint16_t mMinorOperatingSystemVersion;
	uint16_t mMajorImageVersion;
	uint16_t mMinorImageVersion;
	uint16_t mMajorSubsystemVersion;
	uint16_t mMinorSubsystemVersion;
	uint32_t mWin32VersionValue;
	uint32_t mSizeOfImage;
	uint32_t mSizeOfHeaders;
	uint32_t mCheckSum;
	uint16_t mSubsystem;
	uint16_t mDllCharacteristics;
	uint32_t mSizeOfStackReserve;
	uint32_t mSizeOfStackCommit;
	uint32_t mSizeOfHeapReserve;
	uint32_t mSizeOfHeapCommit;
	uint32_t mLoaderFlags;
	uint32_t mNumberOfRvaAndSizes;
}; 
 
struct IMAGE_SECTION_HEADER { // size 40 bytes
	char[8]  mName;
	uint32_t mVirtualSize;
	uint32_t mVirtualAddress;
	uint32_t mSizeOfRawData;
	uint32_t mPointerToRawData;
	uint32_t mPointerToRawData;
	uint32_t mPointerToRealocations;
	uint32_t mPointerToLinenumbers;
	uint16_t mNumberOfRealocations;
	uint16_t mNumberOfLinenumbers;
	uint32_t mCharacteristics;
}; 
 
*/
 
 
 
//links para arquivos ou diret�rios dentro do mesmo sistema de arquivos. 
typedef struct hardlink_d hardlink_t; 
struct hardlink_d
{
	int used;
	int magic;
	//..
	
};
 
//links para arquivos e diret�rios em volumes espalhados por v�rios discos. 
typedef struct softlink_d softlink_t; 
struct softlink_d
{
	int used;
	int magic;
	//..
};
 
 
/*
 * mbr_d:
 *     Structure for MBR parameters.
 *     #bugbug: Corrigir esses valores...
 *     +Olhar no VHD.
 *     +
 */ 
 
#define BS_JmpBoot			0		/* x86 jump instruction (3-byte) */
#define BS_OEMName			2 //3		/* OEM name (8-byte) */
#define BPB_BytsPerSec		11		/* Sector size [byte] (WORD) */
#define BPB_SecPerClus		13		/* Cluster size [sector] (BYTE) */
#define BPB_RsvdSecCnt		14		/* Size of reserved area [sector] (WORD) */
#define BPB_NumFATs			16		/* Number of FATs (BYTE) */
#define BPB_RootEntCnt		17		/* Size of root directory area for FAT [entry] (WORD) */
#define BPB_TotSec16		19		/* Volume size (16-bit) [sector] (WORD) */
#define BPB_Media			21		/* Media descriptor byte (BYTE) */
#define BPB_FATSz16			22		/* FAT size (16-bit) [sector] (WORD) */
#define BPB_SecPerTrk		24		/* Number of sectors per track for int13h [sector] (WORD) */
#define BPB_NumHeads		26		/* Number of heads for int13h (WORD) */
#define BPB_HiddSec			28		/* Volume offset from top of the drive (DWORD) */
#define BPB_TotSec32		32		/* Volume size (32-bit) [sector] (DWORD) */
#define BS_DrvNum			36		/* Physical drive number for int13h (BYTE) */
#define BS_NTres			37		/* WindowsNT error flag (BYTE) */
#define BS_BootSig			38		/* Extended boot signature (BYTE) */
#define BS_VolID			39		/* Volume serial number (DWORD) */
#define BS_VolLab			42 //43		/* Volume label string (8-byte) */
#define BS_FilSysType		50 //54		/* Filesystem type string (8-byte) */
#define BS_BootCode			62		/* Boot code (448-byte) */
#define BS_55AA				510		/* Signature word (WORD) */
 
#define MBR_Table			446		/* MBR: Offset of partition table in the MBR */ 
 
typedef struct mbr_d mbr_t; 
struct mbr_d
{

/*
	//jmp code (3 bytes)
	
	//os name 
	//Sistem info. 
    char OEM_ID[5];  //"NORA "
    char VERSION[3]; //"1.0"
	
	//bpb 
	  
    //Bpb.	
    unsigned short BytesPerSector;  // dw 512
	char SectorsPerCluster;         // db 1
	short ReservedSectors;          // dw 2
	char TotalFATs;                 // db 2
	short MaxRootEntries;           // dw 512         ;512 entradas de 32bytes=32 setores.
	short TotalSectorsSmall;        // dw 0
	char MediaDescriptor;           // db 0xF0
	short SectorsPerFAT;            // dw 64          ;64.      
	short SectorsPerTrack;          // dw 0           ;pegar via bios.     
	
	short NumHeads;                 // dw 0           ;pegar via bios.        
	long HiddenSectors;             // dd 0x00000000
	long TotalSectorsLarge;         // dd 0x00000000
	
	//;for extended.
	char DriveNumber;               // db 0x80        ;pegar via bios.
	char Flags;                     // db 0x00
	char bootSignature;             // db 0           ;Extended Boot Signature.
	long VolumeID;                  // dd 0x00000001
	char VolumeLabel[11];           // db "FREDNORA8MB"
	char SystemID[8];               // db "FAT16   "		
	
	//PointerTable
	//unsigned short SystemName;
	//unsigned short SystemVersion;
	//unsigned short SystemBPB;
	//unsigned short SystemSignature;
	
	//Partition Table
	//unsigned long p0[3];
	//unsigned long p1[3];
	//unsigned long p2[3];
	//unsigned long p3[3];
	
	//Signature.
	unsigned short Signature;
*/
}; 
mbr_t *mbr; 
//mbr_t *Mbr; 

 
/*
 * vbr_d:
 *     Structure for VBR parameters.
 *
 */  
typedef struct vbr_d vbr_t; 
struct vbr_d
{
	//copiar mbr, � parecido;
}; 
vbr_t *vbr; 
//vbr_t *Vbr; 
 
 

 
 
/*
 * fat_d:
 *     Fat 16.
 *
 */
 
typedef struct fat_d fat_t; 
struct fat_d
{
    unsigned long address;
	int type;
	//...
};
fat_t *fat;


/*
typedef struct findfile_d findfile_t;
struct findfile_d
{
	//char reserved[21];
	//char attrib;
	//unsigned short wr_time;
	//unsigned short wr_date;
	unsigned long size;
	char name[13];	
};
findfile_t *Findfile;
*/



/*
 * file_d:
 *     Estrutura para arquivo.
 */ 
typedef struct file_d file_t;
struct file_d
{
    //@todo: incluir informa��es extras sobre o arquivo.
	
	/*
	 * Endre�o onde o arquivo foi carregado.
	 *
	 */
	unsigned long address;
	
    /*
	 * Um ponteiro pra estrutura da entrada atual.
	 */
	struct dir_entry_d *current;
};
//file_t *File;
//file_t *CurrentFile;
//...







/*
 **************************************
 * dir_d:
 *     Estrutura para diret�rio de arquivo.
 *     #importante: Entrada de diret�rio baseada em streams. 
 */
typedef struct dir_d dir_t;
struct dir_d
{
	//
	// @todo: Criar e organizar os elementos dessa estrutura.
	//
	
	int id;
	
	int used;
	int magic;
	
	// Parte principal da estrutura
	FILE *stream;
	
	
    // @todo: Incluir informa��es extras sobre o diret�rio,
	//        como se est� ou n�o carregado na mem�ria, endere�o,
	//        privil�gios ...
	
	//struct user_info_d *OwnerUser;	
	//struct process_d *OwnerProcess;
	
	
	// n�mero de bytes em uma entrada.
	int entry_size_in_bytes;
	
	//numero total de bytes no diret�rio.
	int totalentries_size_in_bytes;
	
    /*
     * N�metro m�ximo de arquivos em um diret�rio.
	 */
    int fileMax;

    /*
	 * N�mero total de arquivos presentes no diret�rio.
	 */   
    int fileTotal;
	
	/*
	 * Endre�o onde o arquivo foi carregado.
	 *
	 */
	unsigned long address;
   
   /*
    * flag, se esta ou nao na memoria.
	*/
    int inMemory;
   
    /*
	 * Um ponteiro pra estrutura da entrada atual.
	 */
	struct dir_entry_d *current;
};

// \DEV
// \SECURITY
// \TOOLS
// \USERS

 

/*
 * filesystem_d:
 *     Informa��es sobre o sistema de arquivos usado pelo sistema.
 */
typedef struct filesystem_d filesystem_t; 
struct filesystem_d
{
    int used;
    int magic;
	
    int type;
	
	unsigned long rootdir_address; //endere�o do rootdir
    unsigned long rootdir_lba;     //lba
	
	unsigned long fat_address;     //endere�o da fat  
	unsigned long fat_lba;      //lba
	
    unsigned long dataarea_address; //endere�o do inicio da area de dados.
    unsigned long dataarea_lba;   //lba
	
	int spc; //Sectors per cluster.
    int rootdir_entries; //numero de entradas no rootdir.
	int entry_size;      //tamanho da entrada em bytes.
	
	//...
};
struct filesystem_d *filesystem;
struct filesystem_d *current_filesystem;
//filesystem_t *filesystem;
//filesystem_t *FileSystem;
//filesystem_t *CurrentFileSystem;
//...





/*
 * file_access_d:
 *     Estrutura para os elementos necess�rios 
 *     para acessar um arquivo.
 *
 */
typedef struct file_access_d file_access_t; 
struct file_access_d
{
    //disk
	struct diskinfo_d *Disk;
    
	//volume
	struct volumeinfo_d *Volume;
	
	//filesystem
	struct filesystem_d *FileSystem;
    
	//directory
	struct dir_d *Directory;
	struct dir_entry_d *current;
	
	//file	
    struct file_d *File;
    struct _iobuf *FileInfo; //FILE (C99)	
	
	//flags
    //int flag;	
};
file_access_t *FileAccess;





//===================================================================
//      ## STORAGE ## 
//===================================================================
//informa��o sobre o sistema de arquivos atual.
//mesmo que nenhum sistema de arquivos esteja configurado � poss�vel
//consultar essa estrutura para saber a condi��o do sistema
//de arquivos atual.
//#info: Esse ser� o n�vel 0. O n�vel masi baixo das estruturas 
//de gerenciamento de armazenamento de massa. Abaixo disso � driver
//de controlador.
//
struct storage_d
{
	
	//
	// # disk #
	//
	
	//disco atual
	struct disk_d *d;
	
	//
	// # volume #
	//	
	
	//volume atual.
	struct volume_d *v;
	
	
	//
	// # file system #
	//
	
	//Ponteiro para o sistema de arquivos atual.
	//Se isso for NULL, ent�o n�o temos sistema de arquivos.
    struct filesystem_d *fs;
	
	
	//virtual file system
	struct vfs_d *vfs;
	
	//Stream 
	//ponteiro para o arquivo atual,
	//que pode ser um arquivo, um diret�rio, um dispositivo ...
	//tudo seguindo defini��es unix-like para esse tipo de estrutura.
	//Na inicializa��o uma das estruturas deve ser apontada aqui.
	
	//#importante 
	//devemos permitir que os programas em user mode tenham acesso 
	//as streams gerenciadas pelo kernel.
	
	//na estrutura de processo devemos indicar 
	//quais s�o as streams gerenciadas pelo kernel que 
	//o processo tem acesso, tamb�m devemos indicar quais s�o 
	//as streams abertas pelo processo.
	
	//stream atual
	FILE *stream;
};
//Essa ser� a esttrutura usada para 
//saber informa��es sobre o sistema de arquivos atual,
//e gerenciar o sistema de arquivos atual.
struct storage_d *storage;




 
 
 
/*
 * Contagem de diret�rios.
 */
int dirCount;  



/*
 * Variables.
 */

 
 
/*
 * Suporte � lista de parti��es.
 * Volumes s�o parti��es, As parti��es podem estar em discos diferentes
 * ou podem ser volumes virtuais.
 * A lista armazena endere�os das estruturas dos volumes.
 *
 */ 
//unsigned long volumeList[256];  //@todo: Diminuir o tamanho do buffer.
 
 
//Id da parti��o atual.(volume atual) 
//int current_volume; 

/*
 * Tipo de sistema de arquivos, fat16, ext2 ...
 */
 
int g_file_system_type; //@todo: DELETAR.
int g_filesystem_type;   //use this one.
int g_spc;               //sectors per cluster.(spc � vari�vel.)



// Se � fat32, 16, 12.
int fatbits;   



//list of clusters. 
unsigned short file_cluster_list[1024]; 




//
// ## Prototypes ##
//


/*
 * Initialization support.
 */
int fsInit();

void 
fsFAT16ListFiles( const char *dir_name, 
                  unsigned short *dir_address, 
				  int number_of_entries );

//MBR support.
void fsCheckMbrFile( unsigned char *buffer );

//VBR support.
void fsCheckVbrFile( unsigned char *buffer );


void fs_init_structures();
void set_filesystem_type(int type);
int get_filesystem_type();
void set_spc(int spc);
int get_spc();	
void fs_save_structure();
void fs_set_structures();
 


//
// FAT support.
//
 
void fsClearFat();   
void fs_init_fat();
void fs_put_list_on_fat();
void fs_apaga_fat();
void fs_load_fatEx();
unsigned long fs_get_fat_entry(unsigned long n);
void fs_set_fat_entry(unsigned long n, unsigned long value);
unsigned long fs_check_fat();
void fs_test_fat_vector();


// root dir support
void fs_load_rootdir();
void fs_save_rootdir();
void fs_save_entry_on_root(unsigned long eid);
void fs_load_rootdirEx();


//   Carrega o diret�rio na mem�ria, dados o endere�o, 
//o lba inicial e o n�mero de setores.
void 
load_directory ( unsigned long address, 
                 unsigned long lba, 
				 unsigned long sectors );

//
// Read
//

// @todo:
// Mudar o retorno para int.
// Mudar file_name para char*.
// Mudar file_address para char*.
// usar o save file como exemplo.

//#test
//Vamos tentar a op��o de carregarmos um arquivo de um dado diret�rio.
//O endere�o do diret�rio ser� passado via argumento.

unsigned long 
fsLoadFile ( unsigned long fat_address,
			 unsigned long dir_address,
             unsigned char *file_name, 
             unsigned long file_address );

		
//get file size.
unsigned long fsGetFileSize ( unsigned char *file_name );
			
			
//
// Write
//

int
fsSaveFile( char *file_name, 
            unsigned long file_size,
            unsigned long size_in_bytes, 			
		    char* file_address,
            char flag );


						  
int fsSearchFile( unsigned char *file_name);
int fsSearchFileName( unsigned char *name);

int KiSearchFile( unsigned char *file_name, unsigned long address);
void create_system_folders();
void set_file( void *file, int Index);
void *get_file(int Index);

unsigned long fs_create_dir( char *name , unsigned long id);

int fs_create_file( char *name );

void fs_create_entry( char *name, 
                      unsigned long id, 
					  unsigned long eid, 
					  unsigned long cluster, 
					  unsigned long size );
void fs_create_name( char *name, unsigned long id,unsigned long eid);
void fs_show_dir_entry(unsigned long id,unsigned long eid);
void fs_show_dir(unsigned long id);

void fs_set_entry(unsigned long id, unsigned long eid);
void fs_get_entry(unsigned long id, unsigned long eid);
void fs_load_dir(unsigned long id);
void fs_save_dir(unsigned long id);
void fs_set_entry_status(unsigned long id, unsigned long eid, unsigned long status);
unsigned long fs_get_entry_status(unsigned long id, unsigned long eid);
void fs_delete_entry(unsigned long id, unsigned long eid);



void fs_show_entry(unsigned long id, unsigned long eid);																			 

unsigned short 
fs_find_n_empty_entries( int n);

// #importante
// Encontrar uma entrada vazia na fat.
unsigned short 
fs_find_empty_entry( char *fat_address );

unsigned long fs_check_cluster(unsigned long id);
//unsigned long fs_find_not_empty_entry();


unsigned long 
fatClustToSect( unsigned short cluster, 
                unsigned long spc, 
				unsigned long first_data_sector );

void fatLoadCluster( unsigned long sector, 
                     unsigned long address, 
					 unsigned long spc );
					 
void fatWriteCluster( unsigned long sector, 
                      unsigned long address, 
					  unsigned long spc );
					  
void 
MountShortFileName( char *buffer, 
                    struct dir_entry_d *entry );

void fs_check_disk();

void fsCreateVFS();

int fsList( const char *dir_name );

void fsListFiles( int disk_id, 
                  int volume_id, 
				  int directory_id );
				  
				  
				  
//interna
//procura uma entrada vazia no diret�rio 
int 
findEmptyDirectoryEntry( unsigned long dir_address, 
                         int number_of_entries );
				  
				  
//Modificador de string.
void read_fntos(char *name);
				  
int fsCheckELFFile ( unsigned long address );				  
int fsCheckPEFile( unsigned long address );	

int fsLoadFileFromCurrentTargetDir ();
void fsInitTargetDir();
	  
				  
//
// End.
//
