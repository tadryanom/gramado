/*
 * File: sm/sys/sm.c  
 *
 *     Server Manager 
 *
 *     Gerenciamento de servidores feito pelo kernel base.
 *     #importante: Um aplicativo gerenciador de servidores 
 * dever� invocar esse m�dulo para pegar informa��es sobre 
 * os servidores do sistema. Pois o kernel base vai gerenciar
 * as estruturas com informa��es sobre eles, deixando os dados 
 * protegidos em na mem�ria em kernel mode.
 *
 * Descri��o:
 *      Gerenciamento de servidores.
 *      Gerencia os processos servidores que est�o em user mode.
 *      @todo: Essas s�o as rotinas de gerenciamento de servidores 
 * oferecida pelo kernel base, por�m o gerenciador de servidores
 * poder� ser um utilit�rio em user mode, que poder� usar as rotinas
 * oferecidads aqui.
 *      O Kernel faz ponte entre Clientes em user mode e servidores
 *      em user mode. O cliente em user mode solicita ao kernel servi�os
 *      oferecidos por servidores que est�o em user mode.
 *
 *      O kernel estabelece essa conex�o cliente/servidor
 *      atrav�z de mensagens. via interrup��o.
 *      a interrup��o � a 200. e � tratada pelo arquivo services.c.
 *
 *      Um forma de trocar mensagens de dados � atrav�z dos canais. 
 *
 * Vers�o 1.0, 2015, 2016. 
 */

/*
A basic set of servers for a general-purpose microkernel includes 
file system servers, 
device driver servers, 
networking servers, 
display servers, 
and user interface device servers.

The necessary servers are started at system startup and provide services, 
such as file, network, and device access, to ordinary application programs.

display server
timer server
int fs_server();
int mm_server();
int xx_server();
*/


#include <kernel.h>


/*
 * init_systemserver:
 *     System server supprt.
 */

int init_systemserver (void)
{
    return 0;  //Ainda n�o implementada.
}


/*
int serverInit();
int serverInit()
{
	return 0;
}
*/


//
// End.
//

