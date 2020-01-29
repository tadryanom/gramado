/*
 * File: console.h  (at the console --usually just the monitor) 
 * Console � o termo usado para o dispositivo que envia comandos 
 * de gerenciamento do sistema. ... o m�dulo /sm esta tratando disso.
 *
 * Obs: O termo usado nesse sistema ser� terminal virtual. Est� em (terminal.h).
 *      Um terminal � composto de teclado e monitor.
 *      Talvez o termo console nem ser� usado nesse sistema.
 *
 * [melhor defini��o:]
 * The console is the text output device for system administration messages. 
 * These messages come from the kernel, from the init system and from the system logger.
 * On modern small computers the console is usually the computer's attached monitor and keyboard.
 *
 * Console is a basic computer or monitor and keyboard that is connected to another computer, 
 * server, or a mainframe over a network.
 * It is used to maintain or monitor the status of the network or computer.
 *
 * Console � um termo que generaliza os terminais de computador.
 * Ou seja, qualquer tipo de terminal de computador � chamado de console.
 * Com input e output. Seja um terminal f�sico ou um terminal emulado. 
 *
 * The system console, computer console, root console, operator's console, 
 * or simply console is the text entry and display device 
 * for system administration messages, 
 * particularly those from the BIOS or boot loader, the kernel, 
 * from the init system and from the system logger. 
 * It is a physical device consisting of a keyboard and a screen, 
 * and traditionally is a text terminal, 
 * but may also be a graphical terminal. 
 * System consoles are generalized to (computer terminals), 
 * which are abstracted respectively by virtual consoles and terminal emulators. 
 * Today communication with system consoles is generally done abstractly, 
 * via the standard streams (stdin, stdout, and stderr), 
 * but there may be system-specific interfaces, 
 * for example those used by the system kernel.
 *
 *    " Console � o meio por onde a m�quina interage com o usu�rio, 
 *      recebendo input de comandos ou enviando mensagens em uma tela."
 *
 *   The console provides a way for the kernel and other processes to 
 *   output text-based messages to the user, and 
 *   to receive text-based input from the user.
 *
 *   Muitos dispositivos podem ser usados como console do sistema, 
 *   como: LFB da placa de video, mem�ria VGA em modo texto, 
 *         porta serial (onboard ou usb), e terminal virtual.
 *
 *   Pode-se ter varios terminais virtuais.
 *
 *   Quando o sistema seleciona um console, ele seleciona
 *   por ende ir� receber e enviar mensagens.
 *   Normalmente recebe-se input de teclado e mouse e
 *   envia mensagens na tela de um monitor atrav�z do LFB
 *   ou mem�ria VGA. 
 *   
 *   Uma placa de desenvolvimento usa com frequ�ncia 
 *   a porta serial como console.
 *
 *   Durante o processo de boot usa-se um console
 *   pra apresentar o log do kernel (verbose).
 *
 *   Consoles virtuais permitem armazenar multiplos
 *   buffers de texto, que permitem m�ltiplos
 *   programas de console rodarem simultaneamente em ambiente
 *   multitarefa.
 *   
 *   Cada console virtual pode ter seu pr�prio conjunto de caracteres
 *   e layout de teclado. 
 *   Cada console virtual pode ter uma fonte diferente. ou um modo de video diferente.
 *
 *   Um caractere pode ser enviado para um monitor atrav�z do lfb da placa de video pcie e
 *   um caractere pode ser enviado para um monitor LCD atrav�s de uma porta serial.
 *
 * Exemplos:
 *    +Virtual consoles
 *    +Text mode console
 *    +framebuffer console
 *    +serial port console
 *
 *
 * * Importante: 
 *       Prov� suporte � fluxo padr�o:
 *       stdin, stdout, stderr. 
 *
 *    Console � uma classe e oferece suporte � varias
 *    rotinas de escrita em tela, como writeline... 
 */

 
/* scroll */
#define SCROLL_UP       (1)
#define SCROLL_DOWN     (2)
 

 

//console types. 
typedef enum {
	CONSOLE_NULL,    //negligenciado.
    CONSOLE_SERIAL,
    CONSOLE_USBSERIAL,
    CONSOLE_VIRTUALTERMINAL,
    CONSOLE_VGATEXTMODE,
    CONSOLE_LFB,
    //...	
}console_type_t; 
 
 
 
 
/*
 * console_d:
 *     Estrutura para gerenciamento de consoles.
 *
 */ 
typedef struct console_d console_t;
struct console_d
{
	object_type_t objectType;
	object_class_t objectClass;
	
	//
    // Identificadores:
	//
	
	int id;
	int used;
	int magic;
	char *name; 
	console_type_t type;    //Serial?

	//
	//Caracter�sticas
	//

	int flags;	
    unsigned long col;
    unsigned long row;
    unsigned long color;
    unsigned long memory_address;    //(display memory)
    unsigned long buffer_address;    //(backbuffer)

    //
	// Fluxo de dados.
	//
	
	int input; 
	int output;	
	void *data;

    //
	// Arquivos (drivers, metafiles, stdout)
	//
	
	FILE *_stdin;	
	FILE *_stdout;
	FILE *_stderr;
	//...
	
	//Continua...
	
	struct console_d *next;
};
console_t *Console;
console_t *CurrentConsole;
//...



//
// End.
//

