/*
 * Arquivo: mac.h
 *
 * Descri��o:
 *     MAC address info manager.
 *
 * Vers�o 1.0, 2016.
 */



typedef struct mac_address_d mac_address_t; 
struct mac_address_d
{
	object_type_t objectType;
	object_class_t objectClass;
	
	//6 bytes. (3 de uma vari�vel e tres de outra.)
    unsigned long mac_address_high;   //OUI
    unsigned long mac_address_low;    //NIC

};
mac_address_t *mac_address;
//...

//
//fim.
//

