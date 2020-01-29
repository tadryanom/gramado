/*
 * File: user/userenv.c 
 *
 * Descri��o:
 *
 * User Environment Manager, (UEM).
 *
 * Cria o ambiente do usu�rio, �rea de trabalho, onde o usu�rio interage 
 * com a m�quina (logo ap�s o logon). 
 *
 * MB - M�dulos inclu�dos no Kernel Base.
 *    
 * Tem usu�rios do tipo interactive e do tipo non_interactive.
 * * O ambiente gr�fico � criado para usu�rio tipo interactive.
 *
 * Rotinas de cria��o, inicializa��o, finaliza��o e configura��o 
 * do ambiente do usu�rio.
 * Inicia programas, troca backgrounds, cor padr�o de janelas ... etc.
 * Criar session, window stations, desktops. 
 * (usando as rotinas que est�o em outros m�dulos.)
 *
 * History:
 *     2015 - Created by Fred Nora.
 *     2016 - Revision.
 */

 
#include <kernel.h>


//User Environment Manager (UEM)

//
// Segue uma lista de opera��es que poder�o ser realizadas para configura��o
// do ambiente gr�fico do usu�rio. Obs: Nesse m�dulo, apenas chamaremos
// algumas fun��es primitivas. Todo o trabalho maior deve ficar para os
// servidores em user mode.
//

#define UEM_NULL 0
#define UEM_CHANGE_BACKGROUND 1
#define UEM_CHANGE_LOGO       2
#define UEM_CHANGE_COLORS     3
//...


//Vari�veis internas.
//int userenvironmentStatus;
//int userenvironmentError;
//...


//Internas.
void config_user (void);


/*
 *********************************************************************
 * startUserEnvironment:
 *     ?? Quem chamaou essa fun��o ??
 *     Talvez ele nem seja usado ...#importante  
 *     os aplicativos podem esta rodando no ambiente de logon. 
 *     O procedimento do logon.c pode chama-l� apertando F1.  
 *     Cria um ambiente com janelas para usu�rio do tipo interativo.
 *     Obs: Isso n�o � realmente o ambiente que o usu�rio vai usar, mas sim 
 * as janelas principais onde o aplicativo 'file manager' (explorer e taskbar) 
 * v�o criar juas janelas.
 * 
 * Mudar o nome para userenvironmentStart(int argc, char* argv[]).
 */

int startUserEnvironment ( int argc, char* argv[] ){
	
    int Status = 0;
	
	//
	// GUI support.
	//

    //Cria as principais janelas que servir�o de base para 
    //a interface gr�fica.
    create_gui ();

    //Inicializa.
    init_gui (); 
	
	//
	// Keyboard support.
	//
	
	ldisc_init_modifier_keys ();
	ldisc_init_lock_keys ();
	
	
	// #importante
	// Obs: Precisamos inicializar todos os elementos 
	// de intera��o que o usu�rio vai precisar.
	
	//
	// ...
	//

    SetProcedure ( (unsigned long) &system_procedure);
  
	return (int) Status;
}


/*
 **************************************************
 * ShowUserInfo:
 *     Mostra informa��es sobre o usu�rio atual.
 */

// #todo
// Mostrar as informa��es do usu�rio e da sess�o.

void 
ShowUserInfo (int user_id)
{

    struct user_info_d *__User;



    if ( user_id < 0 || user_id >= USER_COUNT_MAX )
    {
        return;
    }


    __User = (void *) userList[user_id];

	if ( (void *) __User == NULL )
	{
	    printf ("ShowUserInfo: Error\n");
        return;
	}else{
	    
	    //buffer
	    printf(__User->__username);
	     
		printf (" Id={%d} UserType={%d} \n", 
		    __User->userId, 
		    __User->userType );
	

		printf (" usession={%d} room={%d} desktop={%d} \n", 
		    __User->usessionId, 
		    __User->roomId,
		    __User->desktopId );

	    //...
	};
	
	refresh_screen();
}


/*
 * config_user:
 *     Abre o arquivo de configura��o de usu�rio. 
 *     No arquivo ou metafile, ter�o todas as informa��es
 * sobre o usu�rio e ser�o colocanas nas estruturas.
 */

void config_user (void){
	
    userconfig_Status = 0;	//#todo
}


/*
 ***************
 * CreateUser:
 *     Cria um usu�rio do computador.
 *     #importante: Quando criar um usu�rio tem que 
 * montar uma pasta para ele em /root/user/(name)
 * Se utilizar um usu�rio v�lido � s� pegar os arquivos de 
 * configura��o dentro da pasta.
 * 
 */
 
void *CreateUser ( char *name, int type ){

    struct user_info_d *New;

    int Index = 0;


    New = (void *) kmalloc ( sizeof(struct user_info_d) ); 

    if ( (void *) New == NULL )
    {
        panic ("CreateUser: New");
    } else {

		New->used = 1;
		New->magic = 1234;
		
		//New->path = ?
	    
	    New->userType = type;  

 		//Session.
        //Window Station. (Desktop pool).
        //Desktop.		 
	
	    New->usessionId = current_usersession;           
	    New->roomId = current_room;  //->roomID  
	    New->desktopId = current_desktop;          

	    //Inicializa tokens. (rever)
	    //New->k_token = KERNEL_TOKEN_NULL;
	    //New->e_token = EXECUTIVE_TOKEN_NULL;
	    //New->m_token = MICROKERNEL_TOKEN_NULL;
	    //New->h_token = HAL_TOKEN_NULL;
		
	    //...
	};		
	

	//Procurando uma entrada livre na lista.
	while ( Index < USER_COUNT_MAX )
	{	
        if ( (void *) userList[Index] == NULL )
		{
		    //User Id. 
			New->userId = Index;     
						
		    userList[Index] = (unsigned long) New;

            //printf("CreateUser: Done.\n"); 
	        return (void *) New;
		};
        
		Index++;
	};
	
//Fail: 
//Fim do loop. 
//N�o encontramos uma entrada livre.

fail:
    return NULL;
}


/*
 * SetCurrentUserId:
 *     Configura o ID do usu�rio atual.  
 */

void SetCurrentUserId (int user_id){
	
	if ( user_id < 0 || user_id >= USER_COUNT_MAX )
	{
		printf("user-userenv-SetCurrentUserId:\n");
		return;
	};
	
    current_user = (int) user_id;
}


/*
 * GetCurrentUserId: 
 *     Pega o ID do usu�rio atual.
 */

int GetCurrentUserId (void)
{	
   return (int) current_user;
}


void SetCurrentGroupId (int group_id){
	
	if ( group_id < 0 || group_id >= GROUP_COUNT_MAX )
	{
		printf("SetCurrentGroupId:\n");
		return;
	};
	
    current_group = (int) group_id;
}


/*
 * GetCurrentGroupId: 
 *     Pega o GID do usu�rio atual.
 */

int GetCurrentGroupId (void)
{	
    return (int) current_group;
}


/*
 ***********************************
 * UpdateUserInfo:
 *    Atualiza todas as informa��es de usu�rio.
 */

void 
UpdateUserInfo ( struct user_info_d *user, 
                 int id, 
                 char *name, 
                 int type, 
                 int user_session_id, 
                 int room_id,
                 int desktop_id )
{


	if ( (void *) user == NULL )
	{
        return;
	
	} else {
		
		//Estamos tentando atualizar uma estrutura v�lida.

        if ( user->used != 1 || user->magic != 1234 )
		{
			//fail;
			return;
		}  	
	    
		user->userId = (int) id;                      //Id.     

		user->userType = type;                        //Type.

        user->usessionId = user_session_id;    //Session.
        user->roomId     = room_id;            //room (Window Station).
        user->desktopId  = desktop_id;         //Desktop.
		//...
	};
}


/*
 *******************************
 * init_user_info:
 *     Configurar informa��es sobre o usu�rio default.
 *
 *     #bugbug: Na verdade o que queremos � que seja feita a 
 * inicializa��o do ambiente de usu�rio com as configura��es 
 * do usu�rio atual e n�o o default.
 *
 * #importante:
 *      Esse � o momento certo para inicializar todas as prefer�ncias 
 * salvas no perfil do usu�rio.
 *  
 * Configurar o ambiente do usu�rio significa:
 *     Selecionar suas prefer�ncias de fonte e cores.
 *     Selecionar suas prefer�ncias de estilos de elementos gr�ficos.
 *     etc ...
 *
 * Obs: 
 *     Deve ser poss�vel configurar usu�rios independente se 
 *     a interface gr�fica est� habilitada ou n�o.
 *
 * Obs:
 *     Utiliza as informa��es do usu�rio ativo. 
 *     O usu�rio ativo pode ser trocado.
 *     pode ser criar e deletar usu�rios.
 *
 * #importante: As informa��es de usu�rio ficam 
 * em /root/user/(name)
 * 
 * Obs: Nessa hora o VFS ja deve estar montado
 * ent�o pegaremos informa��es sobre qual � o usu�rio atual.
 * /root/user/password.txt
 * /root/user/username.txt
 * /root/user/config.txt
 * ...
 */
 
void init_user_info (void){
	
	int Id = 0;
    int Index = 0;	
	
	debug_print ("init_user_info:\n");
	
	//Initialize list.
    
	while (Index < USER_COUNT_MAX)
    {	
	    userList[Index] = (unsigned long) 0;
        Index++;
	};
	
    
	//Configurando a estrutura global.
	// Create default user. (default,interactive)

	DefaultUser = (void *) CreateUser (default_user_name, USER_TYPE_INTERACTIVE);
	
	if ( (void *) DefaultUser == NULL )
	{
	    panic ("init_user_info:");
		
	} else {
		
		
		if ( (void *) CurrentTTY == NULL )
		{
			panic ("init_user_info: CurrentTTY");
		}
		
		CurrentTTY->user_info = DefaultUser;
		
		
		//Coloca no in�cio da lista.
		//userList[0] = (unsigned long) SystemUser;    //System.
		userList[1] = (unsigned long) DefaultUser;     //Default.
		userList[2] = (unsigned long) 0;               //0.
		//userList[3] = (unsigned long) 0;             //0.
		//...
		
		//Configura o usu�rio atual.
		Id = (int) DefaultUser->userId;
		SetCurrentUserId(Id);
		CurrentUser = (void *) DefaultUser;
		
		//Configura o grupo atual ao qual o usu�rio pertence.
		SetCurrentGroupId(0);
		
		//#bugbug: falha na m�quina real.
		//interna
		//__setusername ( (char *) USER_DEFAULTNAME);
		
		//...
		
		//g_logged = (int) 1;
	};
	
    // Continua...??!!
}



int __getusername (char *buffer){


    char *login_buffer = (char *) buffer;


	// #cancelando
	// Isso t� falhando na m�quina real.
	// Provavelmente problemas com ponteiro.
	

	//Estrutura default para informa��es sobre o host.
	//host.h

    if ( (void *) CurrentUser== NULL )
    {
        printf ("__getusername: CurrentUser\n");
        return (int) -1;
    }else{

        
        //64 bytes
        strcpy ( login_buffer, (const char *) CurrentUser->__username );
                
        return (int) CurrentUser->userName_len;
    };


    return (int) -1;
}


/*
 ************************************* 
 * __setusername: 
 *  
 * 
 */

// O novo nome est� no buffer passado via argumento.
// Ele tem o limite de 64 bytes.
// Vamos colocar ele na estrutura de usu�rio.

int __setusername ( const char *new_username){

     // Estrutura de usu�rio.

    if ( (void *) CurrentUser == NULL )
    {
        printf ("__setusername: CurrentUser\n");
        return (int) -1;
    }else{

        CurrentUser->userName_len = (size_t) strlen (new_username) + 1;
            
        //64 bytes
        strcpy ( CurrentUser->__username, (const char *) new_username);
        
        return 0;
    };

    return (int) -1;
}



/*
 * init_user_environment_manager:
 *    Inicializa o User Environment Manager. (UEM). 
 */

int init_user_environment_manager ( int argc, char *argv[] ){
	
	//...
	
    //g_module_uem_initialized = 1;
    userenvironmentStatus = 1;
	
	return 0;
}


/*
 * Construtor
void userenvironmentUserEnvironment()
{}
*/


/*
int userenvironmentInit()
{}
*/

//
// End.
//

