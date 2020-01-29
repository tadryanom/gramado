/*
 * File: room.h
 *
 * Descri��o:
 *     Window station. 
 *     Esta��o de janelas. 
 *     (Cont�m desktops).
 *
 *     WINDOW -> DESKTOP -> WINDOW STATION -> USER SECTION.
 *
 * Tipos de window station: 
 *     (interactive, non_interactive).
 *
 * OBS:
 *     Cada PROCESSO est� associado � 
 *     uma window station e s� podem rodar l�.
 *     Uma window station tem v�rios desktops.
 *     Os desktops tem v�rias janelas e menus.
 *
 * Vers�o 1.0, 2015.
 */


 
/*
 * Window Station:
 *
 */ 
//struct wstation_d

struct room_d
{
	object_type_t objectType;
	object_class_t objectClass;
	
	//object control
	struct object_d *object;
	
    int id;
	
	int used;
	int magic;
	
	int userID;
	
    //@todo: fazer uma lista encadeada. de desktops
	unsigned long desktops[32];    //@todo: usar aloca��o dinamica.
	
	//struct wstation_d *next_wstation;
	
	//struct desktop_d *arrayDesktop; //ponteiro para um array de estruturas de desktop.
	
	//linked list. ( a linked list da window station)
	struct linkedlist_d *linkedlist;	
	
	//section
    struct room_d *room;
	
	//next
	struct room_d *next;
};

struct room_d *room0;
//...

unsigned long roomList[ROOM_COUNT_MAX];


/*
 * Contagem de window stations.
 *
 */

//movido para gws.h 
//int windowstations_count;  

void init_room_manager (void);

void init_room_list (void);

int RegisterRoom(struct room_d *room);

void set_current_room(struct room_d *room);

void *get_current_room (void);

void *CreateRoom (struct room_d *room);


//
// End.
//


