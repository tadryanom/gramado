/*
 * File: include\execve\dd\sm\disk\volume.h
 *
 * Descri��o:
 *     Gerenciador de volumes.
 *     Header para o Volume Manager.
 *     Ger�ncia de volumes.
 *
 *
 * History:
 *     2015 - Created by Fred Nora.
 *     2018 - Some new structures.
 */

 
#define VOLUME_COUNT_MAX 1024



/*
 *****************************************************
 * volume_type_t:
 *     Enumerando os tipos de volume.
 */
typedef enum {
	
	VOLUME_TYPE_NULL,            
	
	// Parti��o em disco f�sico.
	VOLUME_TYPE_DISK_PARTITION,  

	// Parti��o em disco f�sico.
	VOLUME_TYPE_VIRTUAL_DISK_PARTITION,  

    // Arquivo.
	// Um pequeno arquivo qualquer. N�o tem MBR.
	// Esse foi carregado de algum disco.
	VOLUME_TYPE_FILE,           
	
	// Buffer.
	// Um pequeno buffer qualquer. N�o tem MBR.
	// Obs: Esse n�o foi carregado de disco nenhum.
	// O volume 0, ser� desse tipo. (conductor://)
	VOLUME_TYPE_BUFFER
	
	//...
	
}volume_type_t;


/*
 * volume_d:
 *     Estrutura para acesso r�pido a volumes.
 *     Deve ser simples e com poucos elementos.
 */

struct volume_d
{ 
	object_type_t objectType;
	object_class_t objectClass;
	
	volume_type_t volumeType;
	
	int id;
	
	int used;
	int magic;
	
	//label
	char *name;
	
	//pathname
	char *cmd;
	
	//string usada no path para identificar o disco.
	//isso n�o existe.
    char path_string[32];  
	
	struct disk_d *disk;
	
	struct volume_d *next;
};
struct volume_d *volume_vfs;             // volume 0
struct volume_d *volume_bootpartition;   // volume 1
struct volume_d *volume_systempartition; // volume 2
//#importante:
//Esses s�o os tr�s volumes b�sicos do sistema 
//mesmo que o disco s� tenha um volume, essas 
//estruturas v�o existir.


//
// Lista de volumes.
//

unsigned long volumeList[VOLUME_COUNT_MAX];


void volumeShowCurrentVolumeInfo ();


void volumeShowCurrentVolume();
void *volume_get_volume_handle( int number );
void *volume_get_current_volume_info();
int volumeShowVolumeInfo( int descriptor );
int volume_init();


//
// End.
//

