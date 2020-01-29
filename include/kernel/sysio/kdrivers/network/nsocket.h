/*
 * File: nsocket.h
 *
 * Descri��o:
 *     Header para gerenciamento de sockets. (n�o � klibc)
 *
 *    "Um soquete � composto por um endere�o de ip,
 *     concatenado com um n�mero de porta, ip_addres:port"
 *
 * History:
 *     2016 -  Created by Fred Nora.
 */


int current_socket;



/*
 **********************
 * socket_d:
 *     Socket strutuct.
 */

struct socket_d
{
    object_type_t objectType;
    object_class_t objectClass;

    unsigned long ip_long;

	//unsigned char ip[4];
    unsigned short port;

	// podemos fazer mais coisa aqui.
	// talvez um arquivo
	//talvez um descritor, pra ficar igual na libc
	//talvez incluir ponteiros para as estruturas em sys/socket.h
	//...
};
struct socket_d *CurrentSocket;
struct socket_d *LocalHostHTTPSocket;
//...



//#todo: 
// Refazer esse limite provi�rio.
#define SOCKET_COUNT_MAX 32

unsigned long socketList[SOCKET_COUNT_MAX];






struct socket_d *create_socket ( unsigned long ip, unsigned short port );

unsigned long getSocketIP ( struct socket_d *socket );
unsigned long getSocketPort ( struct socket_d *socket );

int 
update_socket ( struct socket_d *socket, 
                unsigned long ip, 
                unsigned short port );


//
// End.
//

