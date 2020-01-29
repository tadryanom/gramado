/*
 * File: usession.h
 *
 * Descri��o:
 *     Header do gerenciador de user sections.
 *     
 *     WINDOW -> DESKTOP -> WINDOW STATION -> USER SECTION.
 *
 *
 * Obs: Uma estrutura de user session pode armazenas ponteiros para
 *      +Desktop Pool. (Window Station). 
 *      +Object Pool.
 *      +Page Directory Pool. 
 *
 *
 *  Tipos de usu�rios: 
 *     Interactive e non_interactive.
 *
 * Hist�rico:
 *     Vers�o 1.0, 2015 - Esse arquivo foi criado por Fred Nora.
 *     Vers�o 1.0, 2016 - Revis�o do header.
 *     ...
 */

 
 
  

 
 
//user session start
//No caso de termos apenas uma user session, esse ser� o endere�o dela.
unsigned long usersession_start; 
 
 
/*
 * User section structure (u.session)
 */ 

struct usession_d
{
	object_type_t objectType;
	object_class_t objectClass;
	
	//object control
	struct object_d *object;
	
    int initialized;
	
	int usId;    //id da se��o.
	int usUsed;
	int usMagic;
	
	int userID; // *IMPORTANTE , ID do usu�rio da sec��o.
	
	unsigned long BeginTime;
	unsigned long EndTime;
	
	//
	//Lintas encadeadas de ponteiros para pool.
	//ou vetores de ponteiros para pools.
	//
	
	//In�cio da lista encadeada de ponteiros para pools.
	unsigned long *dpHead;     //Desktop Pools.(wstations).
	unsigned long *opHead;     //Object Pools.
	unsigned long *pdpHead;    //Page Directory Pools. (page directory lists) 
	
	
	//Ponteiros para 8 desktop pools. (window stations).
	//pools de desktop.
	unsigned long listDesktopPool[8];
	
	//ponteiros para 8 object pools.
	//pools de objetos.
	unsigned long listObjectPool[8];
	
	//Ponteiros para 8 Page Directory Pools.
	//pools de diret�rio de p�ginas.
	unsigned long listPageDirectoryPool[8];
	
	//struct wstation_d *arrayWindowStation; //ponteiro para um array de estruturas de window station.
	
	//linked list. (a linked list da window station)
	//struct linkedlist_d *linkedlist;
	
    struct usession_d *next;	
};
struct usession_d *CurrentUserSession;

//#tests;
struct usession_d *DefaultUserSession;
struct usession_d *usession0;
//struct usession_d *USession;
//struct usession_d *UserSession;
//..

//
// Um mesmo usu�rio pode usar sessoes diferentes para atividades diferentes.
// -Qual � a user session do usu�rio logado? Sabendo a user session que o usu�rio est�
// podemos determinar quais os pools que que est�o dispon�veis para ele, como pools de desktop
// pools de pagedirectory, pool de objetos...
//

// Lista encadeada de user section.
struct usession_d *usession_Conductor2;
struct usession_d *usession_Conductor;
struct usession_d *usession_rootConductor;

unsigned long usessionList[USER_SESSION_COUNT_MAX];



//
// Prot�tipos das fun��es.
//

void init_user_session(); 
void *CreateUserSession(int userID); 
void *get_current_user_session();
void open_user_session();
void close_user_session();


//
// Fim.
//


