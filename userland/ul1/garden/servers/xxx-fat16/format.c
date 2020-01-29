/*
 * File: fs\format.c
 *
 * Descri��o:
 *     Sevi�os de formata��o de disco e volumes.
 *     Formata a tabela de parti��es no mbr.
 *     Altera valores no mbr e metafile. 
 *
 * @todo:
 *     Servi�os de formata��o podem ir para um aplicativo.
 * Hist�rico:
 *    Vers�o 1.0, 2015 - Esse arquivo foi criado por Fred Nora.
 *    Vers�o 1.0: 2016 - Revis�o.
 *    ...
 */
 

#include <kernel.h>


//@todo: mudar o nome da fun��o para fs_clear_fat.
void fs_apaga_fat (){
	
    fs_init_fat ();
}


void fs_delete_entry ( unsigned long id, unsigned long eid )
{
    //return;
}



/*
 * fsClearFat:
 *      Zera o buffer e grava zeros no hd.
 */
void fsClearFat (){
	
	//int i;
    //unsigned char *fat_base = (unsigned char *) FAT16_FAT_ADDRESS;
	
	/*
     * @todo: 
	 *     O endere�o da fat deveria estar definido em uma
	 * vari�vel global. Por enquanto esse endere�o � improviso.
	 */    
    
	/*
     * Tamanho da fat.
     *
     * @todo: 
	 *     O tamanho da fat deveria estar definido em uma
	 * vari�vel global. 
	 */	
    //i = (int) (512*4);
	
	//while(i > 0){
    //    fat_base[i] = 0;    
    //    i--;
    //}; 

    //Salva  FAT ! 
	//write_lba(FAT16_FAT_ADDRESS, FAT16_FAT_LBA);

	
    //@todo: Usar while.      
   
done:   
	return;
};

//
// End.
//

