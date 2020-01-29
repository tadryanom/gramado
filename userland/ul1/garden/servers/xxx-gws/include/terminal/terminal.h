/*
 * File: terminal.h
 *
 * Descri��o:
 *     Terminal emulator. (Teclado e Monitor).
 *     Terminal � onde o computador recebe input e envia output.
 *     pode haver terminal virtual como no linux(tty0 ~tty5)
 *     Configura��es do terminal atual.
 *     Pode-se usar a tela inteira como terminal.
 *     Obs: O Shell poder� configurar o terminal atual
 *          para que os caracteres digitados
 *          sejam enviados pelo kernel para o termonal certo.
 *          Inclusive o buffer que o shell usa pode estar dentro do kernel.
 *          Inclusive o shell pode se apropriar do procedimento de shell
 *          existente dentro do kernel base e do procedimento de janela do sistema.
 *
 * Sobre o emulador de terminal: (Terminal Emulator.)
 * =============================
 *     A tela de um terminal pode ser emulada em uma janela, ent�o uma janela
 * vai agir como se fosse uma tela de terminal. Como acontece com aplicativos shell
 * e m�quinas virtuais.
 *     A disciplica de linha envia caracteres para o emulador de terminal.
 *
 *     Obs: � disciplina de linha n�o precisa receber o caractere diretamente
 * do driver de teclado. O m�todo pode ser o seguinte: O driver de teclado envia
 * o caractera para a fila do processo, o processo envia o caractere para o driver
 * de TTY, o driver de TTY envia o caractere para a disciplina de linha e a
 * disciplina de linha envia o caractere para o emulador de terminal.
 *
 * Obs: O driver de TTY recebe do processo pedidos para escrever e ler caracteres no 
 * emulador de terminal.
 *       Para manipular arquivos, os processos podem usar o driver de io.
 * 
 *
 * Vers�o 1.0, 2016.
 */

/*

Attaching to a terminal:
A process can use the AttachTerminal function to attach to a terminal. A process can be attached to one terminal.
A terminal can have many processes attached to it.

The system creates a new terminal when it starts a terminal process,
*/


//test 
//#bugbug
#define DEFAULT_TERMINAL_LEFT 0
#define DEFAULT_TERMINAL_TOP  0
 
//Constantes.
#define TERMINAL_LIN_MAX    100    //N�mero m�ximo de linhas. 
#define TERMINAL_COL_MAX     74    //N�mero m�ximo de colunas.


#define TERMINAL_COUNT_MAX 8

//
// N�mero do terminal atual.
//

//
int terminalLine;       //Troca de linha;(up down)
int terminalOffset;     //Deslocamento dentro da linha; (left right).
int terminalLineMax;    //N�mero m�ximo de linhas suportadas.
int terminalOffsetMax;  //N�mero m�ximo de caracteres por linha.


//char teminalfeedCH;
//int teminalfeedCHStatus;


/*

[ Reflex�o sobre o ret�ngulo onde aparecem os caracteres no terminal virtual: ]

Um terminal virtual ter� uma janela de inst�ncia e o ret�ngulo que 
compreende a �rea de cliente dessa janela. Esse ret�ngulo deve ser o lugar 
onde os caracteres ser�o pintados. 

Mas podemos desvincular esse ret�ngulo da janela de inst�ncia e usarmos 
apenas o ret�ngulo que compreende a �rea de cliente da janela.

Pois bem, no modo full screen do terminal virtual podemos simplesmente 
desabilitar a janela de inst�ncia e usarmos apenas um ret�ngulo do tamanho da tela inteira.

Isso libera a estrutura de janela. Para isso a estrutura de terminal deve 
conter um ponteiro para janela e um ponteiro para ret�ngulo.

Para efeito de teste, podemos escrever no ret�ngulo do terminal, sem
criarmos uma janela completa, daquelas que tem barra de t�tulos e tudo mais.  

Uma fun��o deve ser oferecida para configurar esse ret�ngulo do terminal virtual 
atual.

Ou seja, o lugar natural de imprimir caracteres de terminal � nesse ret�ngulo 
configur�vel.

*/

//Struct para terminal.
struct terminal_d
{
	object_type_t objectType;
	object_class_t objectClass;

	//Slot number.(N�MERO DO TERMINAL)
    int id;    
    
	int used;
    int magic;
	
	//Owner.
	int PID;
	
	int window_id;
	
	
	//@todo: Instance.
	
    int	LinMax;
	int	ColMax;
	int FullScreen;    //flag.
	
	
	// *IMPORTANTE: 
	// Os caracteres ser�o pintados nesse ret�ngulo.
	// @todo: Uma fun��o deve oferecer a oportunidade de configurar esse
	// at� o limite da �rea de cliente da janela de inst�ncia do terminal.

	//informa��es b�sicas sobre o ret�ngulo
    unsigned long left; 
	unsigned long top;
	unsigned long width;
	unsigned long height;
	
    // #importante
	// Nas estruturas de stream tem elementos
	// para manipular os bytes.
	
	// #importante
	// Isso serve para gerenciar estruturas em ring0.
              
    FILE *stdin;	
	FILE *stdout;
	FILE *stderr;
	
	
}TERMINAL[TERMINAL_COUNT_MAX];

//List.
//(Control+F1) Aciona o primeiro terminal
//(Control+F2)
//(Control+F3)
//(Control+F4)
//(Control+F5)
//(Control+F6)
//(Control+F7)
//(Control+F8) 
//unsigned long terminalList[TERMINAL_COUNT_MAX]; //TERMINAL_COUNT_MAX



/*
 * A do terminal que tem a janela ativa e o foco de entrada.
 *
 *
 */
struct window_d *terminalActiveWindow;
struct window_d *terminalWindowWithFocus;
//...

//envia um caractere para a estrutura de terminal 
//de onde um aplicativo poder� pegar.
int terminalFeed(int terminal_id, int ch );
//int init_system_terminal();

//
//fim.
//

