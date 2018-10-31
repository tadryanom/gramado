/*
 * File: unb\ldisc.c
 *
 * Descri��o:
 *    Esse ser� o gerenciador de Line Discipline.
 *    Ficar� dentro do kernel base e receber� as entradas 
 * dos dispositivos de caractere e enviar� para as filas apropriadas.
 *    Por enquanto os scancodes de teclado s�o tratados e enviados 
 * para a fila de mensagem da janela apropriada. Principalmente a janela 
 * com o foco de entrada. 
 *
 */


#include <kernel.h>

//#todo #test
//nelson cole ps2 support (interna)
//todo: essas coisas podem ir para ports.c

//IO Delay
//disk1.h tem isso.
//#define io_delay() __asm__ __volatile__("out %%al,$0x80"::)

inline void wait_ns(int count)
{
	count /=100;	 
	while(--count)io_delay();

};

// Esta rotina faz o Auto-teste 0xaa �xito, 0xfc erro
int BAT_TEST ();
int BAT_TEST (){
    
    	uint8_t val;

    	while (1)
		{
        	val = mouse_read();

			//se deu certo.
        	if (val == 0xAA)
			{
				printf("BAT_TEST ok \n");
				return 0;
        	}else{
		 	    
				//se falhou
				if(val == 0xFC) 
				{
					printf("BAT_TEST fail \n");
       			    return -1; 
       		    }				
			} 
				
        	// Reenviar o comando. 
        	// OBS: este comando n�o � colocado em buffer
        	mouse_write(0xFE);       
        };
};


 
//=======================================================
//++ Usadas pelo mouse.
// hardwarelib.inc
//
#define MOUSE_X_SIGN	0x10
#define MOUSE_Y_SIGN	0x20


int saved_mouse_x;
int saved_mouse_y;

//Coordenadas do cursor.
extern int mouse_x;
extern int mouse_y;

//Bytes do controlador.
extern char mouse_packet_data;
extern char mouse_packet_x;
extern char mouse_packet_y;
//extern char mouse_packet_scroll;
 
extern void update_mouse();


//Estado dos bot�es do mouse
int mouse_buttom_1; 
int mouse_buttom_2;
int mouse_buttom_3;

//Estado anterior dos bot�es do mouse.
int old_mouse_buttom_1; 
int old_mouse_buttom_2;
int old_mouse_buttom_3;

//se ouve alguma modifica��o no estado 
//dos bot�es.
int mouse_button_action;

//--
//=========================================================



//?? usado pelo mouse
#define outanyb(p) __asm__ __volatile__( "outb %%al,%0" : : "dN"((p)) : "eax" )


//
// Imported functions.
//

//
// Defini��es para uso interno do m�dulo.
//


// Ports:
// =====
//     The entire range for the keyboard is 60-6F,
//     a total of 16 values (a 16bit range).
//
//  @todo:
//      As portas do controlador ainda est�o subutilizadas.
//      fazer um driver mais completo utilizando melhor o controlador.



//
//Command Listing:
//================
//Command	Descripton
//0xED	Set LEDs
//0xEE	Echo command. Returns 0xEE to port 0x60 as a diagnostic test
//0xF0	Set alternate scan code set
//0xF2	Send 2 byte keyboard ID code as the next two bytes to be read from port 0x60
//0xF3	Set autrepeat delay and repeat rate
//0xF4	Enable keyboard
//0xF5	Reset to power on condition and wait for enable command
//0xF6	Reset to power on condition and begin scanning keyboard
//0xF7	Set all keys to autorepeat (PS/2 only)
//0xF8	Set all keys to send make code and break code (PS/2 only)
//0xF9	Set all keys to generate only make codes
//0xFA	Set all keys to autorepeat and generate make/break codes
//0xFB	Set a single key to autorepeat
//0xFC	Set a single key to generate make and break codes
//0xFD	Set a single key to generate only break codes
//0xFE	Resend last result
//0xFF	Reset keyboard to power on state and start self test


//issso pertence � inicializa��o do teclado. deve ficar no driver de teclado.
/* Keyboard Commands */
#define KBD_CMD_SET_LEDS	    0xED	// Set keyboard leds.
#define KBD_CMD_ECHO     	    0xEE
#define KBD_CMD_GET_ID 	        0xF2	// get keyboard ID.
#define KBD_CMD_SET_RATE	    0xF3	// Set typematic rate.
#define KBD_CMD_ENABLE		    0xF4	// Enable scanning.
#define KBD_CMD_RESET_DISABLE	0xF5	// reset and disable scanning.
#define KBD_CMD_RESET_ENABLE   	0xF6    // reset and enable scanning.
#define KBD_CMD_RESET		    0xFF	// Reset.
//#define RESET  0xFE


/*
enum KYBRD_CTRL_STATS_MASK {
 
	KYBRD_CTRL_STATS_MASK_OUT_BUF	=	1,		//00000001
	KYBRD_CTRL_STATS_MASK_IN_BUF	=	2,		//00000010
	KYBRD_CTRL_STATS_MASK_SYSTEM	=	4,		//00000100
	KYBRD_CTRL_STATS_MASK_CMD_DATA	=	8,		//00001000
	KYBRD_CTRL_STATS_MASK_LOCKED	=	0x10,		//00010000
	KYBRD_CTRL_STATS_MASK_AUX_BUF	=	0x20,		//00100000
	KYBRD_CTRL_STATS_MASK_TIMEOUT	=	0x40,		//01000000
	KYBRD_CTRL_STATS_MASK_PARITY	=	0x80		//10000000
};

//! sets leds
void keyboard_set_leds( int num, int caps, int scroll) 
{ 
	char data = 0;
 
	//! set or clear the bit
	data = (char) (scroll) ? (data | 1) : (data & 1);
	data = (char) (num)    ? (num | 2)  : (num & 2);
	data = (char) (caps)   ? (num | 4)  : (num & 4);
 
	//! send the command -- update keyboard Light Emetting Diods (LEDs)
	kybrd_enc_send_cmd (KYBRD_ENC_CMD_SET_LED);
	kybrd_enc_send_cmd (data);
	
done:
    return;
};

//! send command byte to keyboard encoder
void kybrd_enc_send_cmd (uint8_t cmd) {
 
	//! wait for kkybrd controller input buffer to be clear
	while (1)
		if ( (kybrd_ctrl_read_status () & KYBRD_CTRL_STATS_MASK_IN_BUF) == 0)
			break;
 
	//! send command byte to kybrd encoder
	outportb (KYBRD_ENC_CMD_REG, cmd);
}
//! read status from keyboard controller
uint8_t kybrd_ctrl_read_status () {
 
	return inportb (KYBRD_CTRL_STATS_REG);
}
*/


//
// Vari�veis internas
//
//int keyboardStatus;
//int keyboardError;
//...


//Status
//@todo: Status pode ser (int).
//vari�veis usadas pelo line discipline para controlar o 
//estado das teclas de controle.

unsigned long key_status;
unsigned long escape_status;
unsigned long tab_status;
unsigned long winkey_status;
unsigned long ctrl_status;
unsigned long alt_status;
unsigned long shift_status;
unsigned long capslock_status;
unsigned long numlock_status;
unsigned long scrolllock_status;
//...

//
// ** kernel Winkey shotcuts **
//

/*
 WINKEY+
 ...
 */

//
// Mouse support
//

//bytes do controlador.
char mouse_status;
char delta_x;
char delta_y;

//coordenadas.
int mouse_pos_x;
int mouse_pos_y;


//unsigned char *mousemsg;


//@todo: fazer rotina de get status algumas dessas vari�veis.


//Se h� uma nova mensagem de teclado. 
int kbMsgStatus;



//
// keyboardMessage
//     Estrutura interna para mensagens.
//
struct keyboardMessage 
{
    unsigned char scancode;

    //??
    //@todo: Na verdade todo driver usar� estrutura de janela descrita na API 
    //que o driver use.

    //? hwnd;  
    int message;
    unsigned long long1;
    unsigned long long2;
};


//Pega o status das teclas de modifica��o.

unsigned long keyboardGetKeyState ( unsigned char key ){
	
	unsigned long State = 0;
	
	switch (key){
		
		case VK_LSHIFT: 
		    State = shift_status; 
			break;

	    case VK_LCONTROL:
		    State = ctrl_status;
		    break;

	    case VK_LWIN:
		    State = winkey_status;
		    break;

	    case VK_LMENU:
		    State = alt_status;
		    break;

	    case VK_RWIN:
		    State = winkey_status;
		    break;

	    case VK_RCONTROL:
		    State = ctrl_status;
		    break;
			
	    case VK_RSHIFT:
		    State = shift_status;
		    break;

	    case VK_CAPITAL:
		    State = capslock_status;
		    break;

	    case VK_NUMLOCK:
		    State = numlock_status;
		    break;
			
		case VK_SCROLL:
            State = scrolllock_status;
            break;			
			
		//...
	};

	//Nothing.

    return (unsigned long) State;		
};


/*
 * keyboardEnable:
 *     Enable keyboard.
 */
void keyboardEnable (){
	
	//Wait for bit 1 of status reg to be zero.
    
	while ( (inportb(0x64) & 2) != 0 )
	{
		//Nothing.
	};
	
	//Send code for setting Enable command.
    outportb(0x60,0xF4);
    //sleep(100);
};


/*
 * keyboardDisable:
 *     Disable keyboard.
 */
void keyboardDisable (){
	
	//Wait for bit 1 of status reg to be zero.
    
	while ( (inportb(0x64) & 2) != 0 )
	{
		//Nothing.
	};
	
	//Send code for setting disable command.
    outportb(0x60,0xF5);
    //sleep(100);
};


/*
 * keyboard_set_leds:
 *     Set keyboard flags.
 *     ED = Set led.
 */
void keyboard_set_leds (char flag){
	
	//@todo: filtro.

	//Wait for bit 1 of status reg to be zero.
    while( (inportb(0x64) & 2) != 0 ){
		//Nothing.
	};
	//Send code for setting the flag.
    outportb(0x60,0xED);            
    sleep(100);

	//Wait for bit 1 of status reg to be zero.
	while( (inportb(0x64) & 2) != 0 ){
	    //Nothing.	
	};
    //Send flag. 
	outportb(0x60,flag);
	sleep(100);
	
	//@todo mudar o status.
    //switch(flag)
    //{
		
	//}	
};


 
/*
void keyboard();
void keyboard()
{
	//@todo: Create global.
	if(gKeyboardType == 1){
		abnt2_keyboard_handler();
	}
	//...
	return;
}
*/


/*
 ***************
 * LINE DISCIPLINE
 * Funciona como um filtro.
 * Obs: Essa � a rotina principal desse arquivo, 
 * todo o resto poder� encontrar um lugar melhor.
 *
 */
 
int LINE_DISCIPLINE ( unsigned char SC, int type ){
	
    //
    // Step 0 - Declara��es de vari�veis.
    //

    //Vari�veis para tecla digitada.
    unsigned char scancode;
    unsigned char key;         //Tecla (uma parte do scancode).  
    unsigned long mensagem;    //arg2.	
    unsigned long ch;          //arg3 - (O caractere convertido para ascii).
    unsigned long status;      //arg4.  

    // Text mode support.
	// Tela para debug em RING 0.
    // unsigned char *screen = (unsigned char *) 0x000B8000;   
    //unsigned char *screen = (unsigned char *) SCREEN_START;    //Virtual.   
    //...
	
	//Window.
    //struct window_d *wFocus;	


    //
    // Step1 - Pegar o scancode.
    //

    //O driver pegou o scancode e passou para a disciplina de linha 
    //atrav�s de par�metro.	
	scancode = SC;
	

	//Obs: Observe que daqui pra frente todas as rotinas poderiam estar
	//     em user mode.


	//Debug stuff.
    //Show the scancode if the flag is enabled.	
	if (scStatus == 1){
	    printf("{%d,%x}\n",scancode,scancode);
	};
	
	
    //
    // Step 2 - Tratar as mensagens.
    //

    //Se a tecla for (liberada).
	//D� '0' se o bit de paridade for '0'.
    if( (scancode & LDISC_KEY_RELEASED) == 0 )
	{
	    key = scancode;
		key &= LDISC_KEY_MASK;    //Desativando o bit de paridade caso esteja ligado.

		//Configurando se � do sistema ou n�o.
		//@todo: Aqui podemos chamar uma rotina interna que fa�a essa checagem.
		
		switch (key)
		{
			//Os primeiros 'case' � quando libera tecla do sistema.
			//O case 'default' � pra quando libera tecla que n�o � do sistema.
			
			//left Shift liberado.
			case VK_LSHIFT:
				shift_status = 0;
				mensagem = MSG_SYSKEYUP;
			    break;

			//Left Control liberado.			
			case VK_LCONTROL:
				ctrl_status = 0;
				mensagem = MSG_SYSKEYUP;
				break;

			//left Winkey liberada.
			case VK_LWIN:
			    winkey_status = 0;
                mensagem = MSG_SYSKEYUP;
				break;

			//Left Alt liberado.
            case VK_LMENU:
				alt_status = 0;
				mensagem = MSG_SYSKEYUP;
			    break;				
			
			//right winkey liberada.
			case VK_RWIN:
			    winkey_status = 0;
                mensagem = MSG_SYSKEYUP;
				break;

			//control menu.
			case VK_CONTROL_MENU:
			    //controlmenu_status = 0; //@todo
			    mensagem = MSG_SYSKEYUP;
			    break;

            //right control liberada.
			case VK_RCONTROL:
				ctrl_status = 0;
				mensagem = MSG_SYSKEYUP;
				break;
				
			//right Shift liberado.
			case VK_RSHIFT:
				shift_status = 0;
				mensagem = MSG_SYSKEYUP;
			    break;

			//Fun��es liberadas.
            case VK_F1:
            case VK_F2:
            case VK_F3:
            case VK_F4:
            case VK_F5:
            case VK_F6:
            case VK_F7:
            case VK_F8:
            case VK_F9:
            case VK_F10:
            case VK_F11:
            case VK_F12:
			    mensagem = MSG_SYSKEYUP;
			    break;


			//...
				
			//A tecla liberada N�O � do sistema.
			default:
			    mensagem = MSG_KEYUP;
				break;
		};

		//Selecionando o char para os casos de tecla liberada.

        //Analiza: Se for do sistema usa o mapa de caracteres apropriado. 
   		if(mensagem == MSG_SYSKEYUP)
		{
			//Normal.
			ch = map_abnt2[key];
		};

		//Analiza: Se for tecla normal, pega o mapa de caracteres apropriado.
		if(mensagem == MSG_KEYUP)
		{
		    //checar os mudificadores para mai�scula.
		    //if(shift_status == 1 || capslock_status == 1)
			//{	
			//    ch = shift_abnt2[key];	
			//	goto done;
			//};
			
			//min�scula
			//Nenhuma tecla de modifica��o ligada.
			ch = map_abnt2[key];
			goto done;
			
            //Nothing.
		};
        //Nothing.
		goto done;
	};
	
	// * Tecla (pressionada) ...........	
	if( (scancode & LDISC_KEY_RELEASED) != 0 )
	{ 
		key = scancode;
		key &= LDISC_KEY_MASK; //Desativando o bit de paridade caso esteja ligado.

		//O �ltimo bit � zero para key press.
		//Checando se � a tecla pressionada � o sistema ou n�o.
		//@todo: Aqui podemos chamar uma rotina interna que fa�a essa checagem.
		
		switch (key)
		{
			//back space ser� tratado como tecla normal
			
			//@todo: tab,
            //?? tab ser� tratado como tecla normal por enquanto.
			
			//caps lock keydown
			case VK_CAPITAL:
			    //muda o status do capslock n�o importa o anterior.
				if (capslock_status == 0)
				{ 
				    capslock_status = 1; 
					mensagem = MSG_SYSKEYDOWN; 
					break; 
				};
				if (capslock_status == 1)
				{ 
				    capslock_status = 0; 
					mensagem = MSG_SYSKEYDOWN; 
					break; 
				};
				break; 

			//Left shift pressionada.
			case VK_LSHIFT:
			//case KEY_SHIFT:
				shift_status = 1;
				mensagem = MSG_SYSKEYDOWN;
			    break;

			//left control pressionada.
			case VK_LCONTROL:
			//case KEY_CTRL:
				ctrl_status = 1;
				mensagem = MSG_SYSKEYDOWN;
				break;

			//Left Winkey pressionada.
			case VK_LWIN:
			    winkey_status = 1;
				mensagem = MSG_SYSKEYDOWN;
				break;

            //left Alt pressionada.
            case VK_LMENU:
				alt_status = 1;
				mensagem = MSG_SYSKEYDOWN;
			    break;

			//@todo alt gr.	

			//Right Winkey pressionada.
			case VK_RWIN:
			    winkey_status = 1;
				mensagem = MSG_SYSKEYDOWN;
				break;
			
            //@todo: Control menu.
            
			//Right control pressionada.
			case VK_RCONTROL:
				ctrl_status = 1;
				mensagem = MSG_SYSKEYDOWN;
				break;

			//Right shift pressionada.
			case VK_RSHIFT:
				shift_status = 1;
				mensagem = MSG_SYSKEYDOWN;
			    break;


            case VK_F1:
            case VK_F2:
            case VK_F3:
            case VK_F4:
            case VK_F5:
            case VK_F6:
            case VK_F7:
            case VK_F8:
            case VK_F9:
            case VK_F10:
            case VK_F11:
            case VK_F12:
			    mensagem = MSG_SYSKEYDOWN;
			    break;


			//Num Lock.	
		    case VK_NUMLOCK:
			    //muda o status do numlock n�o importa o anterior.
				if(numlock_status == 0){
		            numlock_status = 1;
					mensagem = MSG_SYSKEYDOWN;
					break;
				};
				if(numlock_status == 1){ 
				    numlock_status = 0;
                    mensagem = MSG_SYSKEYDOWN; 					
					break; 
				};
			    break;
				
			//Scroll Lock.	
		    case VK_SCROLL:
			    //muda o status do numlock n�o importa o anterior.
				if(scrolllock_status == 0){
		            scrolllock_status = 1;
					mensagem = MSG_SYSKEYDOWN;
					break;
				};
				if(scrolllock_status == 1){ 
				    scrolllock_status = 0;
                    mensagem = MSG_SYSKEYDOWN; 					
					break; 
				};
			    break;

            //...

			//A tecla pressionada n�o � do sistema.
			default:
			    //printf("keyboard debug: default: MSG_KEYDOWN\n");
			    mensagem = MSG_KEYDOWN;
				break;
		};

		//uma tecla do sistema foi pressionada.
		if (mensagem == MSG_SYSKEYDOWN)
		{
			ch = map_abnt2[key];
            goto done;
		};

		//uma tecla normal foi pressionada.
		//mensagem de digita��o.		
		//Se o shift ou o capslock estiverem acionados, ela vira mai�scula.
		//#bugbug: Como o shift  n�o funciona ainda, ent�o vamos usar 
		//s� o capslock como tecla de modifica��o da digita��o por enquanto.
		if (mensagem == MSG_KEYDOWN)
		{
            //mai�sculas.			
			if (capslock_status == 1)
			{
			    ch = shift_abnt2[key];
			    goto done;
			}

			//min�sculas.
			if (capslock_status == 0)
			{
		        ch = map_abnt2[key];
			    goto done;
			};
			
            //Nothing.
		};
		
		//Nothing.
		goto done;
	};//fim do else

    //Nothing.
	
	// Para finalizar, vamos enviar a mensagem para fila certa.
done:

	/*
	 * Debug:
     *     No caso de modo texto.
	 *
	 * Coloca o scancode na tela
     * set_up_cursor(0,4);
	 * printf("       "); 	
     * set_up_cursor(0,4);
	 * printf("%d ",(unsigned char) (key & 0xff) );
     * Coloca o caractere na tela
	 * screen[76] = (char) ch;
	 * screen[77] = (char) 0x09;  //azul no preto
	 */


	//Debug:
	//Canto direito da primeira linha.
	//screen[76] = (char) ch;
	//screen[77] = (char) 0x09;    //Azul no preto.

	//
	// Control + Alt + Del.
	//

		//Op��es:
		//@todo: Chamar a interface do sistema para reboot.
		//@todo: Op��o chamar utilit�rio para gerenciador de tarefas.
		//@todo: Abre um desktop para opera��es com usu�rio, senha, logoff, gerenciador de tarefas.

		//Chamando o m�dulo /sm diretamente.
		//mas n�o � o driver de teclado que deve chamar o reboot.
		//o driver de teclado deve enviar o comando para o console, /sm,
		//e o console chama a rotina de reboot do teclado.
		//Uma mensagem de reboot pode ser enviada para o procedimento do sistema.
		//Pois o teclado envia mensagens e n�o trata as mensagens.
	
	    //Um driver n�o deve chamar rotinas de interface. como as rotians de servi�o.	
		//A inten��o � que essa mensagem chegue no procedimento do sistema.
		//Porem o sistema tambem deve saber quem est� enviando esse pedido.@todo.
		//@todo: podemos O reboot pode ser feito atrav�s de um utilit�rio em user mode.
			
	
		//Uma op��o aqui, � enviar para o aplicativo uma mensagem de reboot.
		//como o aplicativo n�o trata esse tipo de mensagem ele apenas reecaminha 
		//para o procedimentod e janelas do sistema.
	
	if ( (ctrl_status == 1) && 
	     (alt_status == 1) && 
		 (ch == KEY_DELETE) )
	{
		services ( SYS_REBOOT, 0, 0, 0 );
		
		//system_procedure ...
		// @todo: Chamar o aplicativo REBOOT.BIN.
	};

	
	//
	// Nesse momento temos duas op��es:
	// Devemos saber se a janela com o foco de entrada � um terminal ou n�o ...
	// se ela for um terminal chamaremos o porcedimento de janelas de terminal 
	// se ela n�o for um terminal chamaremos o procedimento de janela de edit box. 
	// que � o procedimento de janela do sistema.
	// *IMPORTANTE: ENQUANTO O PROCEDIMENTO DE JANELA DO SISTEMA TIVER ATIVO,
	// MUITOS COMANDOS N�O V�O FUNCIONAR ATE QUE SAIAMOS DO MODO TERMINAL.
	//
	//
	
		//
		// *importante:
		// Passamos a mensagem de teclado para o procedimento de janela do sistema.
		// que dever� passar chamar o procedimento de janela da janela com o focod eentrada.
		//
		
		//
		// *importante:
		// Quem � o 'first responder' para evento de teclado.
		// A janela com o foco de entrada � o first responder para 
		// eventos de teclado, mas n�o para todo tipo de envento.		
		//
	
	
  
	// #importante
    // +Pegamos a janela com o foco de entrada, pois ela 
    // ser� um elemento da mensagem.	
	// Mas enviaremos a mensagem para a fila da thread atual.
	
	struct window_d *w;  
	w = (void *) windowList[window_with_focus];
	
	struct thread_d *t;	
	//t = (void *) threadList[current_thread];
	

	
	
	// ## thread ##
	
	if ( (void *) w == NULL )
	{
		printf("LINE_DISCIPLINE: w");
		die();
		
	}else{
		
		
		if ( w->used != 1 || w->magic != 1234 )
		{
			printf("LINE_DISCIPLINE: w magic");
			die();
		}		

		//#importante:
		//Pegamos a trhead de input associada com a janela que tem o foco de entrada.
		
		t = (void *) w->InputThread;
		if ( (void *) t == NULL )
		{
		    printf("LINE_DISCIPLINE: t fail");
		    die();			
		}
		
		if ( t->used != 1 || t->magic != 1234 )
		{
			printf("LINE_DISCIPLINE: thread w magic fail");
			die();
		}        
			
		//#importante:
		//??
		
		//a janela com o foco de entrada deve receber input de teclado.
		//ent�o a mensagem vai para a thrad associada com a janela com o foco de 
		//entrada.
		//#importante: a rotina que seta o foco dever� fazer essa associa��o,
		//o aplicativo chama a rotina de setar o foco em uma janela, 
		//o foco ser� setado nessa janela e a thread atual ser� associada 
		//a essa janela que est� recebendo o foco.
		
		//??
		//ja o input de mouse deve ir para a thread de qualquer janela.
		
		t->window = w;
		t->msg = (int) mensagem;
		t->long1 = ch;
		t->long2 = ch;
		
		t->newmessageFlag = 1;
		
		
		//#importante:
		//Chamando o porcedimento de janela para que n�o fiquemos sem 
		//mensagem alguma, mas o certo � chamar o procedimento do sistema 
		//s� depois que o aplicativo consumir a mensagem, e o aplicativo decide 
		//se vai chamar o procedimento do sistema ou n�o.
		//inclusive o procedimento do sistema poder� ficar em user mode na API.
		//aqui poder� ficar um segundo procedimento do sistema, bem reduzido,
		//apenas para emerg�ncia do desenvolvedor.
		
		// sm\sys\proc.c
		
	    system_procedure (  w, (int) mensagem, (unsigned long) ch, 
	        (unsigned long) ch );
	};	
 
	//?? porque -1.
    return (int) -1;
};


/*
 * KdGetWindowPointer:
 *     Retorna o ponteiro da estrutura de janela que pertence a thread.
 *     Dado o id de uma thread, retorna o ponteiro de estrutura da janela 
 * � qual a thread pertence.
 */
void *KdGetWindowPointer (int tid){
	
	struct thread_d *t;

	//@todo: filtrar argumento. 
	
	if ( tid < 0 )
        return NULL;
        
		
	// Structure.
	t = (void *) threadList[tid];

	if ( (void *) t == NULL )
	{
        return NULL;        
	};
	
	return (void *) t->window;
};


/*
 * KbGetMessage:
 *     Pega a mensagem na fila de mensagens na estrutura da thread
 * com foco de entrada.
 *
 * Na estrutura da thread com foco de entrada tem uma fila de mensagens.
 * Pegar a mensagem.
 * 
 * Para falha, retorna -1.
 *
 * @todo: bugbug: A mensagem deve estar na fila do processo, na
 *                estrutura do proceso. (Talvez n�o na thread e nem na janela.)
 */
int KbGetMessage (int tid){
	
	int ret_val;
	struct thread_d *t;
	
	// Structure.
	t = (void*) threadList[tid];

	if ( (void *) t != NULL )
	{
        ret_val = (int) t->msg;
	}else{
	    ret_val = (int) -1;    //Fail.
	};

// Done.
//done:

	WindowProcedure->msgStatus = 0;    //Muda o status.
	return (int) ret_val;              //Retorna a mensagem.
};


/*
 * KbGetLongParam1:
 *    Pega o parametro "long1" do procedimento de janela de uma thread.
 */
unsigned long KbGetLongParam1 (int tid){
   	
	struct thread_d *t;
	
	// Structure.
	t = (void *) threadList[tid];

	if ( (void *) t == NULL)
	{
        return (unsigned long) 0;    //@todo: fail;
	};

// Done.
//done:
    return (unsigned long) t->long1;
};

/*
 * KbGetLongParam2:
 *     Pega o parametro "long2" do procedimento de janela de uma thread.
 */
unsigned long KbGetLongParam2 (int tid){
	
	struct thread_d *t;
	
	// Structure.
	t = (void *) threadList[tid];

	if ( (void *) t == NULL)
	{
        return (unsigned long) 0;    //@todo: fail;
	};

// Done.
//done:
    return (unsigned long) t->long2;
};


/*
 * reboot: 
 *     @todo: essa rotina poder� ter seu pr�prio arquivo.
 *     Reboot system via keyboard port.
 *     ?? #bugbug Por que o reboot est� aqui ??
 *
 * *IMPORTANTE: a interface fechou o que tinha qe fechar,
 * hal chamou essa hotina para efetuar a parte de hardware reboot apenas.
 * @todo: Atribui��es.
 *
 * Atribui��es: 
 *     + Desabilitar as interrup��es.
 *     + Salvar registros.
 *     + Salvar programas abertos e n�o salvos.
 *     + Fechar todas tarefas antes.
 *     + Efetuar o tipo de reboot especificado.
 *    + Outras ...
 */
void reboot (){
    
    //@todo: 
	// +criar uma variavel global que especifique o tipo de reboot.
    // +criar um switch para efetuar os tipos de reboot.
	// +criar rota de fuga para reboot abortado.
	// +Identificar o uso da gui antes de apagar a tela.
	//  modo grafico ou modo texto.
	//
	
	//
	// Video.
	//
	
	/*
	sleep(2000);
	//kclear(9);
    set_up_cursor(0,0);	
    set_up_text_color(0x0f, 0x09);
	printf("\n\n REBOOTING ...\n\n");


	//
	// Scheduler stuffs.
	//
	
	sleep(1000);
	printf("locking scheduler ...\n");
	scheduler_lock();
	
	//
	// Tasks.
	//
	
	//@todo: fazer fun��o com while. semelhante ao dead task collector.
	
	sleep(1000);
	printf("killing tasks ...\n");
	//kill_thread(current_task); 
	
	//
	// Final message.
	//
	
	sleep(1000);
	printf("turning off ...\n");
    
	
	refresh_screen();
	
	//
	// Interruo��es.
	//
	
	sleep(7000);
	asm("cli");
	
	*/
	
	
	// @todo: disable();
	
//
// Done.
//

//done:

    hal_reboot ();
	die ();
};


//Get alt Status.
int get_alt_status (){
	
    return (int) alt_status;
};


//Get control status.
int get_ctrl_status (){
	
    return (int) ctrl_status;
};

 
int get_shift_status (){
	
    return (int) shift_status;	
};
 

/*
 * ps2_keyboard_initialize:
 *     +Inicializa a estrutura de io control para o dispositivo teclado.
 *     +Configurar as globais usadas pelo driver de teclado.
 *     +Habilitar a porta de teclado. 
 *     +reset.
 *     +configura leds.
 */
void ps2_keyboard_initialize (){
	
	
	printf ("ps2_keyboard_initialize: initializing ...\n");
	
	// ## Step1 ##
	// Inicializa a estrutura de io control para o dispositivo teclado.
	
	//user.h
	ioControl_keyboard = (struct ioControl_d *) malloc( sizeof(struct ioControl_d) );
	
	if ( (void *) ioControl_keyboard == NULL )
	{
		printf("ps2_keyboard_initialize: ioControl_keyboard fail");
		die();
	}else{
	    
	    ioControl_keyboard->id = 0;
	    ioControl_keyboard->used = 1;
	    ioControl_keyboard->magic = 1234;
	    
		ioControl_keyboard->tid = 0;  //qual threa est� usando o dispositivo.
	    //ioControl_keyboard->
	};
	
    //int Type = 0;

    //
    // @todo: 
	//     Checar se o teclado � do tipo abnt2.   
	//     � necess�rio sondar par�metros de hardware,
	//     como fabricante, modelo para configirar estruturas 
	//     e vari�veis.
	//


/*
    switch(Type)
	{
	    //NULL
		case 0:	
		    break;
			
	    //Americano.
		case 1:	
		    break;

		//pt-ABNT2	
	    case 2:	
		    break;
			
		//...
		
		//Modelo americano
		default:	
		    break;
	}
	
*/

    // ## Step2 ##
    // Configurar as globais usadas pelo driver de teclado.

	//
	// Set abnt2.
	//

	abnt2 = (int) 1;

    //Checar quem est� tentando inicializar o m�dulo.    

	//model.
	
	//handler.
	
	//...

    //Key status.
	key_status = 0;
    escape_status = 0;
    tab_status = 0;
    winkey_status = 0;
    ctrl_status = 0;
    alt_status = 0;
    shift_status = 0;
	capslock_status = 0;
	scrolllock_status = 0;
	numlock_status = 0;
	//...
	
	
	// ## Step3 ##
	// Habilitar a porta de teclado. 

	// 0xAE
	// Enable Keyboard Interface: 
	// Clears Bit 4 of command register enabling keyboard interface.
	// Isso ativa a primeira porta PS/2.
	
	//#bugbug: isso habilita a porta, n�o vamos habilitar a porta por 
	//enquando., deixando a rotina de inicializa��o de ps2 habilitar 
	//as duas portas no final.
	
	kbdc_wait (1);
	outportb ( 0x64, 0xAE );   
	
	
	// ## Step4 ##
	// Reset
	
	kbdc_wait (1);
	outportb ( 0x60, 0xFF );

    // ## Step5 ##
	//Leds.
	//LED_SCROLLLOCK 
	//LED_NUMLOCK 
	//LED_CAPSLOCK  
	
	keyboard_set_leds (LED_NUMLOCK);
	
	//...
	
	//Debug support.
	scStatus = 0;

    g_driver_keyboard_initialized = (int) 1;
	
    printf ("ps2_keyboard_initialize: done\n");		
};


/*
 * Constructor.
int keyboardKeyboard(){
	;
};
*/


/*
 obs: definido acima.
int keyboardInit(){
	;
};
*/


//
// ********************** Mouse ************************
//

//
// Obs: 
// Precisamos de um lugar para as rotinas de mouse. Elas n�o devem ficar aqui.
// @todo: mouse.c 
//



/*
 ***********************************************
 * ps2_mouse_globals_initialize:
 *     +Inicializa a estrutura de io control para dispositivo mouse.
 *     +Inicializamos as globais usadas pelo handler do driver.
 *     +Carregamos o BMP usado pelo driver. 
 */		
int ps2_mouse_globals_initialize (){
	
	printf("ps2_mouse_globals_initialize: initializing ...\n");
	//refresh_screen();
	
    unsigned char response = 0;
    unsigned char deviceId = 0;
    int i; 
	int bruto = 1;  //M�todo.
	int mouse_ret;
	
	
	//
	// ## Step1 ##
	// Inicializa a estrutura de io control para dispositivo mouse.
	//
	
	//user.h
	ioControl_mouse = (struct ioControl_d *) malloc ( sizeof(struct ioControl_d) );
	
	if ( (void *) ioControl_mouse == NULL )
	{
		printf("ps2_mouse_globals_initialize: ioControl_mouse fail");
		die();
		
	} else {
	    
	    ioControl_mouse->id = 0;
	    
		ioControl_mouse->used = 1;
	    ioControl_mouse->magic = 1234;
	    
		//Qual thread est� usando o dispositivo.
		ioControl_mouse->tid = 0;  
	    //ioControl_mouse->
	};


	//
	// ## Step2 ##
	// Inicializamos as globais usadas pelo handler do driver.
	//
	
	//
	// Estamos espa�o para o buffer de mensagens de mouse.
	// mousemsg = ( unsigned char *) malloc(32);

		
	//Inicializando as vari�veis usadas na rotina em Assemly
    //em hwlib.inc
    
	//Coordenadas do cursor.
	g_mousepointer_x = (unsigned long) 0;
    g_mousepointer_y = (unsigned long) 0;	
    
	mouse_x = 0;
    mouse_y = 0;
	
	//mouse_x = 0;
	//mouse_y = 0;
	
	//#bugbug: Essa inicializa��o est� travando o mouse.
	//fazer com cuidado.
	
	//#bugbug. Cuidado com essa inicializa�ao.
	g_mousepointer_width = 16;
	g_mousepointer_height = 16;
	


    //Bytes do controlador.
   // mouse_packet_data = 0;
   // mouse_packet_x = 0;
    //mouse_packet_y = 0;
    //mouse_packet_scroll = 0;
	
	
	//Mostraremos essa mensagem somente no ambiente de debug.
	
#ifdef KERNEL_VERBOSE	
    MessageBox (gui->screen, 1, "ps2_mouse_globals_initialize:","initializing!");
#endif   
	
 	//
	// ## Step3 ##
	// Carregamos o BMP usado pelo driver. 
	//

	// Carregando o bmp do disco para a mem�ria
	// e apresentando pela primeira vez.
	
	// ## test ##
	//susenso. Isso funciona.
    
	mouse_ret = (int) load_mouse_bmp ();	
	if (mouse_ret != 0)
	{
		printf("ps2_mouse_globals_initialize: load_mouse_bmp");
		die();
	}
	
#ifdef KERNEL_VERBOSE		
    MessageBox(gui->screen, 1, "ps2_mouse_globals_initialize:","Mouse initialized!");   
#endif  

    
	printf("ps2_mouse_globals_initialize: done\n");
	
	//initialized = 1;
    //return (kernelDriverRegister(mouseDriver, &defaultMouseDriver));	
	return (int) 0;
};


/*
 * mouse_write:
 *     Envia um byte para a porta 0x60.
 *     (Nelson Cole) 
 */
void mouse_write (unsigned char write){
	
	kbdc_wait(1);
	outportb (0x64,0xD4);
	
	kbdc_wait(1);
	outportb (0x60,write);
};


/*
 * mouse_read:
 *     Pega um byte na porta 0x60.
 *     (Nelson Cole) 
 */
unsigned char mouse_read (){
	
	kbdc_wait(0);
	
	return inportb(0x60);
};



 
// Esta fun��o � usada para a calibragem do IBF e OBF
// Se a entra � 0 = IBF, se entrada � 1 = OBF
void kbdc_wait ( unsigned char type){
	
	int spin = 100000; 

	if(type == 0) {
		while(!(inb(0x64)&1)) { wait_ns(100); if(!(spin--))break; }
       	}

	else if(type == 1) {

                 while((inb(0x64)&2)) { wait_ns(100); if(!(spin--))break; }

      	}else wait_ns(200);

}
 


/*
 * kbdc_wait:
 *     Espera por flag de autoriza��o para ler ou escrever.
 *     (Nelson Cole) 
 */
/* 
void kbdc_wait (unsigned char type){
	
	//#todo:
	//Qual que � ler e qual que � escrever ?
	//
	
	if (type==0)
	{
		//#bugbug rever
        while ( !inportb(0x64) & 1 )
		{
			//400ns
			outanyb (0x80);
			outanyb (0x80);
			outanyb (0x80);
			outanyb (0x80);
		};
		
    }else{
		
        while ( inportb(0x64) & 2 )
		{
			//400ns
			outanyb (0x80);
			outanyb (0x80);
			outanyb (0x80);
			outanyb (0x80);
		};
	};
};
*/

/*
//rotina interna de suporta ao mouseHandler 
#define _MOUSE_X_SIGN 0x10
#define _MOUSE_Y_SIGN 0x20

void
update_mouse1()
{

char x = (mouse_packet_data & _MOUSE_X_SIGN);
char y = ( mouse_packet_data & _MOUSE_Y_SIGN );


//do_x:
	    //x 
		//checando o sinal para x.
		//se for diferente de 0 ent�o x � negativo
		if(x != 0)
		{
			//complemento de 2.
			mouse_packet_x = ~mouse_packet_x + 1;
			mouse_x = mouse_x - mouse_packet_x;
		}else{

			mouse_x = mouse_x + mouse_packet_x;
		}

		
//do_y:		
		//y
		//se for diferente de 0 ent�o y � negativo
		if(y != 0)
		{
			//complemento de 2.
			mouse_packet_y = ~mouse_packet_y + 1;
			mouse_y = mouse_y + mouse_packet_y;
		}else{
			mouse_y = mouse_y - mouse_packet_y;
		};		
		
    return;		
};
*/



/*
 **************
 * mouseHandler:
 *     Handler de mouse. 
 *
 * *Importante: 
 *     Se estamos aqui � porque os dados dispon�veis no 
 * controlador 8042 pertencem ao mouse.
 *
 * @todo: Essa rotina n�o pertence ao line discipline.
 * Obs: Temos externs no in�cio desse arquivo.
 * 
 */ 
#define MOUSE_DATA_BIT 1
#define MOUSE_SIG_BIT  2
#define MOUSE_F_BIT  0x20
#define MOUSE_V_BIT  0x08 

//contador
static int count_mouse = 0;

// Buffer.
static char buffer_mouse[3];


int flagRefreshMouseOver;
	
void mouseHandler (){
	
	//#debug
	printf(".");
    outportb ( 0xa0, 0x20 ); 
    outportb ( 0x20, 0x20 );
    return;	
	
    // #importante:
	// Essa ser� a thread que receber� a mensagem
	struct thread_d *t;
	
	//#bugbug:
	//usaremos isso provis�riamente para evitar acessar estrutura 
	//inv�lida.
	//t = (struct thread_d *) threadList[current_thread];
	//if ( (void *) t == NULL )
	//{
		//printf("mouseHandler: debug current thread fail\n")
	//	return;
	//}
	
	// #importante:
	// Essa ser� a janela afetada por qualquer
	// evento de mouse.
	
    struct window_d *Window;

	// ID de janela.
	
	int wID; 	
	
	
	// Coordenadas do mouse.
    // Obs: Isso pode ser global.
    // O tratador em assembly tem as vari�veis globais do posicionamento.
    
	int posX=0;
    int posY=0;	

    //Char para o cursor provis�rio.
	//#todo: Isso foi subtituido por uma bmp. Podemos deletar.
    //static char mouse_char[] = "T";

	char *_byte;


	// Lendo um char no controlador.
	
	*_byte = (char) mouse_read();
	
	
	// #importante:
	// Contagem de interru��es:
	// Obs: Precisamos esperar 3 interrup��es.
	
	//#bugbug essa vari�vel est� inicializando toda vez que 
	//se chama o handler porque ela � interna. ??
	
	switch ( count_mouse )
	{
		// Essa foi a primeira interrup��o.
		case 0:
		    //Pegamos o primeiro char.
		    buffer_mouse[0] = (char) *_byte;
            if(*_byte & MOUSE_V_BIT)
                count_mouse++;
		    break;
			
		// Essa foi a segunda interrup��o.	
		case 1:
		    //Pegamos o segundo char.
		    buffer_mouse[1] = (char) *_byte;
			count_mouse++;
		    break;
			
		// Essa foi a terceira interrup��o.	
		case 2:
		    //Pegamos o terceiro char.
            buffer_mouse[2] = (char) *_byte;
			count_mouse = 0;
			
			//#importante:
			//A partir de agora j� temos os tr�s chars.
			
			//??
			//Colocando os tr�s chars em vari�veis globais.
            //Isso ficar� assim caso n�o aja overflow ...
			mouse_packet_data = buffer_mouse[0];
	 	    mouse_packet_x = buffer_mouse[1];       
		    mouse_packet_y = buffer_mouse[2];
            
			// #importante:
			// #todo: Isso est� em Assembly, l� em hwlib.inc, 
			// mas queremos que seja feito em C.
			// #obs: Uma rotina interna aqui nesse arquivo est� tentando isso.
			
		    //salvando antes de atualizar.
			saved_mouse_x = mouse_x;
		    saved_mouse_y = mouse_y;			
			
			update_mouse();			
			
		    // #importante:
			// Agora vamos manipular os valores obtidos atrav�s da 
			// fun��o de atualiza��o dos valores.
			
			mouse_x = (mouse_x & 0x00000FFF ); 
		    mouse_y = (mouse_y & 0x00000FFF );
		
		    // Checando limites.
            // #todo: Os valores foram determinados. 
			// Precisamos usar vari�veis.
			
			if( mouse_x < 1 ){ mouse_x = 1; }	
		    if(	mouse_y < 1 ){ mouse_y = 1; }
		    if(	mouse_x > (800-16) ){ mouse_x = (800-16); }
		    if(	mouse_y > (600-16) ){ mouse_y = (600-16); }

		    // # Draw BMP #
		    // Isso est� funcionando bem.
			// #todo: precisamos colocar no backbuffer o ret�ngulo salvo previamente.
            // #todo: Precisamos salvar o conte�do de um ret�ngulo 
			// que est� no backbuffer, para depois pintarmos o mouse.
						
			
			//  copiar para o lfb o antigo ret�ngulo. Para apagar o ponteiro que est� no lfb.
			refresh_rectangle ( saved_mouse_x, saved_mouse_y, 20, 20 );	
			
			bmpDisplayMousePointerBMP ( mouseBMPBuffer, mouse_x, mouse_y );


            //nova t�cnica.
            //+ copia no LFB um ret�ngulo do backbuffer para apagar o ponteiro antigo.
            //+ decodifica o mouse diretamente no LFB.			
			
            break;

        default:
		    count_mouse = 0;
            break;		
	};
	
	
	// #importante 
	// Por outro lado o mouse deve confrontar seu posicionamento com 
	// todas as janelas, para saber se as coordenadas atuais est�o passando 
	// por alguma das janelas. Se estiver, ent�o enviaremos mensagens para essa 
	// janela que o mouse passou por cima. Ela deve ser sinalizada como hover,
	// 
	// #importante:
	// Se houver um click, o elemento com mousehover ser� afetado, e 
	// enviaremos mesagens para ele, se apertarem enter ou application key, 
	// quem recebe a mensagem � o first responder, ou seja, a janela com o 
	// foco de entrada.
    // Se clicarmos com o bot�o da direita, quem recebe a mensagem � 
    // a janela que o mouse est� passando por cima.	
	
	
	//
	//  ## Button ##
	//
	
	// ===
	//Apenas obtendo o estado dos bot�es.
	
	mouse_buttom_1 = 0;
	mouse_buttom_2 = 0;
	mouse_buttom_3 = 0;
	
	if( ( mouse_packet_data & 0x01 ) == 0 )
	{
		//liberada.
		mouse_buttom_1 = 0;
	}else if( ( mouse_packet_data & 0x01 ) != 0 )
	    {
		    //pressionada.
		    //N�o tem como pressionar mais de um bot�o por vez.
	        mouse_buttom_1 = 1;
	        mouse_buttom_2 = 0;
	        mouse_buttom_3 = 0;		  
	    }
			  

	if( ( mouse_packet_data & 0x02 ) == 0 )
	{
	    //liberada.
	    mouse_buttom_2 = 0;
	}else if( ( mouse_packet_data & 0x02 ) != 0 )
	    {
		    //pressionada.
		    //N�o tem como pressionar mais de um bot�o por vez.
	        mouse_buttom_1 = 0;
	        mouse_buttom_2 = 1;
	        mouse_buttom_3 = 0;		  			 
	    }
			  
	if( ( mouse_packet_data & 0x04 ) == 0 )
	{
	    //liberada.
	    mouse_buttom_3 = 0;
	}else if( ( mouse_packet_data & 0x04 ) != 0 )
	    {
	        //pressionada.
		    //N�o tem como pressionar mais de um bot�o por vez.			 
	        mouse_buttom_1 = 0;
	        mouse_buttom_2 = 0;
	        mouse_buttom_3 = 1;		  			
	    }			
	
	
    // ===	
	// Confrontando o estado atual com o estado anterior para saber se ouve 
	// alguma altera��o ou n�o.
	// 1 = ouve altera��o.
	
	if( mouse_buttom_1 != old_mouse_buttom_1 ||
	    mouse_buttom_2 != old_mouse_buttom_2 ||
		mouse_buttom_3 != old_mouse_buttom_3 )
	{
		mouse_button_action = 1;
	}else{
		mouse_button_action = 0;
	};
	
	// #refletindo: 
	// ?? E no caso de apenas considerarmos que o mouse est� se movendo, 
	// mandaremos para janela over. ???
	
	// #refletindo:
	// Obs: A mensagem over pode ser enviada apenas uma vez. 
	// ser� usada para 'capturar' o mouse ... 
	// e depois tem a mensagem para 'descapturar'.
	
	
	printf(".");
	
	
	//=======================================================================
	//=======================================================================
	
/*

	//
	//  ## Scan ##
	//
	
	//===========
	// (capture) - On mouse over. 
	
	// wID = ID da janela.
	// Escaneamos para achar qual janela bate com os valores indicados.
	// Ou seja. Sobre qual janela o mouse est� passando.

	// #IMPORTANTE
	// Se for v�lido e diferente da atual, significa que 
	// estamos dentro de uma janela.
	// -1 significa que ouve algum problema no escaneamento.
	
	wID = (int) windowScan ( mouse_x, mouse_y );	
	
	//se falhamos na sondagem da janela que o mouse est� passando por cima.
	if ( wID == -1 )
	{ 
        
		// Essa flag indica que podemos fazer o refresh da mouse ouver,
		// mas somente uma vez.
		
		if ( flagRefreshMouseOver == 1 )
		{
		    Window = (struct window_d *) windowList[mouseover_window];	
		    
			// #bugbug:
			// Precisamos checar a validade da estrutura antes de usa-la.
			
			if ( (void *) Window != NULL ){
			    refresh_rectangle ( Window->left, Window->top, 20, 20 );
			}
			
			// N�o podemos mais fazer refresh da mouse over.
			flagRefreshMouseOver = 0;
			
			// #importante
			// Inicializa qual ser� a mouse over.
			mouseover_window = 0;
		}
		
		//Nothing.
		
		
	//ok o mouse passou por cima de uma janela.	
    }else{
		
		Window = (struct window_d *) windowList[wID];
		
		if ( (void *) Window == NULL )
		{
			//fail
			printf ("mouseHandler: Window struct\n");
			return;
		}
		
		if ( Window->used != 1 || Window->magic != 1234)
		{
			//fail
			printf ("mouseHandler: Window validation\n");
			return;
		}
			
		// #importante:
		// Nesse momento temos uma janela v�lida, esse � a janela que o mouse,
        // acabou de entrar.	Ent�o devemos pegar a thread associada � essa janela, 
		// dessa forma enviaremos a mensagem para a thread do aplicativo ao qual 
		// a janela pertence.
		
		t = (void *) Window->InputThread;

		if ( (void *) t == NULL )
		{
			//fail
			return;
		}
		
		//#bugbug 
		//#todo:
		// Deveriamos aqui checarmos a validade da estrutura ??
		
		//redraw_window(wScan);
        
        // #importante
		// Se um bot�o foi pressionado ou liberado, ent�o enviaremos uma 
		// mensagem relativa ao estado do bot�o, caso contr�rio, enviaremos 
		// uma mensagem sobre a movimenta��o do mouse.
		
		//#importante
		// Se houve mudan�a em rela��o ao estado anterior.
		if ( mouse_button_action == 1 )
		{
			//Qual bot�o mudou seu estado??
			//Checaremos um por um.
			
			//1
			//se o bot�o 1 est� igual ao estado anterior
			if( mouse_buttom_1 == old_mouse_buttom_1 )
			{
				//...
			
			//Diferente do estado anterior.
			}else{
				
				//down
				if ( mouse_buttom_1 == 1 )
				{                
					
					//clicou
					if ( old_mouse_buttom_1 == 0 ){
						
						//#importante 
						//enviaremos a mensagem para a thread atual.
						
						//if ( (void *) Window != NULL ){
						
                            //t->window = Window;
						   // t->msg = MSG_MOUSEKEYDOWN;
							//t->long1 = 1;
							//t->long2 = 0;
							
							//t->newmessageFlag = 1;
						//}
										    
					    //atualiza o estado anterior.
					    old_mouse_buttom_1 = 1;
					}
					
				}else{
					
					//up
					
					//#importante 
					//enviaremos a mensagem para a thread atual.
						
					//if ( (void *) Window != NULL ){
						
                        //t->window = Window;
					    //t->msg = MSG_MOUSEKEYUP;
						//t->long1 = 1;
						//t->long2 = 0;
							
						//t->newmessageFlag = 1;
					//}						
						
					old_mouse_buttom_1 = 0;	
				}
			}; 
			
			
			//2
			//se o bot�o 2 est�  Igual ao estado anterior
			if ( mouse_buttom_2 == old_mouse_buttom_2 )
			{
				//...
				
			//Diferente do estado anterior.	
			}else{
				
				//down
				if ( mouse_buttom_2 == 1 )
				{
					
					//clicou
					if( old_mouse_buttom_2 == 0 ){
						
						
						//if ( (void *) Window != NULL ){
						
                           // t->window = Window;
						   // t->msg = MSG_MOUSEKEYDOWN;
							//t->long1 = 2;
							//t->long2 = 0;
							
							//t->newmessageFlag = 1;
						//}						
				    
					    //atualiza o estado anterior.
					    old_mouse_buttom_2 = 1;
					}

                }else{
					
					//up
					//if ( (void *) Window != NULL ){
						
                      //  t->window = Window;
					  //  t->msg = MSG_MOUSEKEYUP;
						//t->long1 = 2;
						//t->long2 = 0;
							
						//t->newmessageFlag = 1;
					//}	
						
					old_mouse_buttom_2 = 0;
				}
			}; 
			
			
			//3
			//se o bot�o 3 est�  Igual ao estado anterior
			if ( mouse_buttom_3 == old_mouse_buttom_3 )
			{
				//...
				
				
			//Diferente do estado anterior.	
			}else{
				
				//down
				if ( mouse_buttom_3 == 1 )
				{
					//clicou
					if ( old_mouse_buttom_3 == 0 ){
                        
						
						//if ( (void *) Window != NULL ){
						
                           // t->window = Window;
						  //  t->msg = MSG_MOUSEKEYDOWN;
							//t->long1 = 3;
							//t->long2 = 0;
							
							//t->newmessageFlag = 1;
						//}	 						
				    
					    //atualiza o estado anterior.
					    old_mouse_buttom_3 = 1;
					}

                }else{
					
					//up
					//if ( (void *) Window != NULL ){
						
                       // t->window = Window;
					   // t->msg = MSG_MOUSEKEYUP;
						//t->long1 = 3;
						//t->long2 = 0;
							
						//t->newmessageFlag = 1;
					//}	
						
					old_mouse_buttom_3 = 0;
				}
			}; 
			
			//A��o conclu�da.
			mouse_button_action = 0;
			
			
		    // Se N�O ouve altera��o no estado dos bot�es, ent�o apenas 
		    // enviaremos a mensagem de movimento do mouse e sinalizamos 
		    // qual � a janela que o mouse est� em cima.
			
		}else{
			
			// #importante
			// Lembrando que estamos dentro de uma janela ...
			// por isso a mensagem � over e n�o move.
			
            //Obs: Se a janela for a mesma que capturou o mouse,
			//ent�o n�o precisamos reenviar a mensagem.
			//Mas se o mouse estiver em cima de uma janela diferente da 
			//que ele estava anteriormente, ent�o precisamos enviar uma 
			//mensagem pra essa nova janela.
			
			//#bugbug:
			//estamos acessando a estrutura, mas precisamos antes saber se ela � v�lida.
			
			if ( (void *) Window != NULL )
			{
			
                //;;;
			    if ( Window->id != mouseover_window )
			    {
				
                    if ( mouseover_window != 0 )
				    {
					
					    //if ( (void *) Window != NULL ){
						
                       // t->window = (struct window_d *) windowList[mouseover_window];
					   // t->msg = MSG_MOUSEEXITED;
						//t->long1 = 0;
						//t->long2 = 0;
						
						//t->newmessageFlag = 1;
					    //}	
				    };
				
				
				    //Agora enviamos uma mensagem pra a nova janela que o mouse 
				    //est� passando por cima.
						
                   // t->window = Window;
					//t->msg = MSG_MOUSEOVER;
					//t->long1 = 0;
					//t->long2 = 0;
						
					//t->newmessageFlag = 1;
				
			
			        //ja que entramos em uma nova janela, vamos mostra isso.
				
				    //bot�o.
			        if ( Window->isButton == 1 )
				    {    
			            bmpDisplayCursorBMP ( fileIconBuffer, Window->left, Window->top );	
			        };
				
				    //n�o bot�o.
				    if ( Window->isButton == 0 )
				    {
				        bmpDisplayCursorBMP ( folderIconBuffer, Window->left, Window->top );		
				    };
				
				    //nova mouse over
				    mouseover_window = Window->id;
				
				    //#importante:
				    //flag que ativa o refresh do mouseover somente uma vez.
				    flagRefreshMouseOver = 1;
			
			    }else{ 
			    
				    //nothing ...
				    //n�o precisamos reenviar a mensagem, pois o mouse 
				    //continua na mesma janela que antes.
                
				    //windowSendMessage ( (unsigned long) wScan, 
		            //    (unsigned long) MSG_MOUSEOVER, 
			        //    (unsigned long) 0, 
			        //    (unsigned long) 0 );				
				
			    };			
			    
				//;;;
			};
			
			//A��o conclu�da.
			//Para o caso de um valor incostante na flag.
			mouse_button_action = 0;			
		};
		
		
		//if ( wScan->isButton == 1 )
		//{
			//colocamos ao passarmos por cima do bot�o,
			//#obs: tem que dar refresh do ret�ngulo no backbuffer quando sair
			//substituir essa imagem.
		//    bmpDisplayCursorBMP ( cursorIconBuffer, wScan->left, wScan->top);	
		//}
		
        //if ( wScan->isButton == 0 )
        //{
		    // #debug. (+)
		    // Isso coloca o (+) no frontbuffer.
		//    draw_text ( wScan, 0, 0, COLOR_YELLOW, "+" );
		//    refresh_rectangle ( wScan->left, wScan->top, 8, 8 );
		//}			
		
	};

*/	
	//=======================================================================
	//=======================================================================
	

    // EOI.		
    outportb ( 0xa0, 0x20 ); 
    outportb ( 0x20, 0x20 );
};


// Input.
// Input a value from the keyboard controller's data port, after checking
// to make sure that there's some data there for us.
static unsigned char inPort60 (void){
	
    unsigned char data = 0;

    while (!(data & 0x01))
        kernelProcessorInPort8(0x64, data);

    kernelProcessorInPort8(0x60, data);
	
    return (data);
};


// Output.
// Output a value to the keyboard controller's data port, after checking
// to make sure it's ready for the data.
static void outPort60 (unsigned char value){
	
    unsigned char data;
  
    // Wait for the controller to be ready
    data = 0x02;
    while (data & 0x02)
        kernelProcessorInPort8(0x64, data);
  
    data = value;
    kernelProcessorOutPort8(0x60, data);
 
    return;
};


// Output.
// Output a value to the keyboard controller's command port, after checking
// to make sure it's ready for the command
static void outPort64 (unsigned char value){
	
    unsigned char data;
  
    // Wait for the controller to be ready
    data = 0x02;
	
    while (data & 0x02)
        kernelProcessorInPort8(0x64, data);

    data = value;
    kernelProcessorOutPort8(0x64, data);
	
	return;
};


/*
 * getMouseData:
 *     Essa fun��o � usada pela rotina kernelPS2MouseDriverReadData.
 * Input a value from the keyboard controller's data port, after checking
 * to make sure that there's some mouse data there for us.
 */
static unsigned char getMouseData (void){
	
    unsigned char data = 0;

    while ((data & 0x21) != 0x21)
        kernelProcessorInPort8(0x64, data);

    kernelProcessorInPort8(0x60, data);
	
	return (data);
};


/*
 * kernelPS2MouseDriverReadData:  
 *     Pega os bytes e salva em um array.
 *     Exibe um caractere na tela, dado as cordenadas.
 *     This gets called whenever there is a mouse interrupt
 *     @todo: Rever as entradas no array.         
 */
void kernelPS2MouseDriverReadData (void){
    //#suspensa !	
};


/*
 ******************************* 
 * ps2_initialize:
 *     Configurando o controlador ps2.
 *
 * +destivamos as portas para os dois dispositivos.
 * +habilitamos a utiliza��o do dispositivo mouse. 
 */
void ps2_initialize(){
	
	unsigned char status;
	
    printf ("ps2_initialize: initializing ...\n");

	//
	// ## Step1: ##
	// Desabilitar as duas portas.
	//
	
	// Desativar a primeira porta PS/2.
  	//kbdc_wait(1);
	//outportb(0x64,0xAD);  
	
	// Desativar a segunda porta PS/2, 
	// hahaha por default ela j� vem desativada, s� para constar
	//kbdc_wait(1);
	//outportb(0x64,0xA7); 	

	//
	// ## Step2: ##
	// Ativar o segundo dispositivo.
	// Pois o segundo dispositivo vem desativado.
	//	
 
	// Activar o segundo despositivo PS/2, modificando o status de 
	// configura��o do controlador PS/2. 
	// Lembrando que o bit 1 � o respons�vel por habilitar, desabilitar o 
	// segundo despositivo PS/2  ( o rato). 
	// S� para constar se vedes aqui fizemos duas coisas lemos ao mesmo tempo 
	// modificamos o byte de configura��o do controlador PS/2 	
	
	 // Defina a leitura do byte actual de configura��o do controlador PS/2.
	kbdc_wait(1);    
	outportb(0x64,0x20);    
	
	kbdc_wait(0);
	status = inportb(0x60)|2;  
	
	// defina, a escrita  de byte de configura��o do controlador PS/2.
	kbdc_wait(1);
	outportb(0x64,0x60);  

	// devolvemos o byte de configura��o modificado.
	kbdc_wait(1);
	outportb(0x60,status);  
 	
    printf ("ps2_initialize: done\n");	
};


/*
 ****************************
 * ps2_mouse_initialize:
 *     Inicializa��o do mouse no controlador ps2.
 *
 * +Reseta mouse.
 * +Restaura defaults do PS/2 mouse.
 * +Habilita o mouse streaming.
 *
 */
void ps2_mouse_initialize (){
	
    // ## Step1 ##	
    // Reseta mouse (reset ? lento!)...
    // esperando 0xFA??
    // Espero pelo byte 0xaa que encerra a sequ?ncia de reset!
  
    kbdc_wait(1);
    mouse_write(0xff);
    while (mouse_read() != 0xFA);

  
    /*

    // printf("\n init_mouse: chamando BAT_TEST\n");
    // refresh_screen();
	
 	// Basic Assurance Test (BAT)
    	if( BAT_TEST() != 0) 
		{	
    		// Aqui! Precisaremos de fazer alguma coisa, em casos de erro
    		printf("\n init_mouse: BAT_TEST Mouse error!");
            refresh_screen();
			
			while (1){
				asm ("cli");
				asm ("hlt");
			}
    	};
	*/

    
	// ## Step2 ##
	// Restaura defaults do PS/2 mouse.
    
	kbdc_wait(1);
    mouse_write(0xf6);
    while (mouse_read() != 0xFA);


	// ## Step3 ##
    // Habilita o mouse streaming
    // Interessante notar que, no modo streaming,
    // 1 byte recebido do PS/2 mouse gerar  uma IRQ...
    // Talvez valha a pena DESABILITAR o modo streaming
    // para colher os 3 dados de uma s� vez na IRQ!
    kbdc_wait(1);
    mouse_write(0xf4);
    while (mouse_read() != 0xfa);         // ACK	

    // TODO: Pode ser interessante diminuir a sensibilidade do mouse
    // aqui!!!	
	
	// O vetor de interrup��o foi configurado em outro momento.
 };

/* 
 * **************
 * P8042_install:
 *     Configurando o controlador PS/2, 
 *     e activar a segunda porta PS/2 (mouse).
 *     (Nelson Cole)
 */
/* 
void P8042_install (){
	
	printf ("P8042_install: initializing ...\n");
	
	unsigned char status;
	
	
    // Desativar dispositivos PS/2 , isto evita que os dispositivos PS/2 
	// envie dados no momento da configura��o.
	
	//asm ("cli");

//desablePorts:
	
	// Desativar a primeira porta PS/2.
  	//kbdc_wait(1);
	//outportb(0x64,0xAD);  
	
	// Desativar a segunda porta PS/2, 
	// hahaha por default ela j� vem desativada, s� para constar
	//kbdc_wait(1);
	//outportb(0x64,0xA7); 

//goAhead:

	// Activar o segundo despositivo PS/2, modificando o status de 
	// configura��o do controlador PS/2. 
	// Lembrando que o bit 1 � o respons�vel por habilitar, desabilitar o 
	// segundo despositivo PS/2  ( o rato). 
	// S� para constar se vedes aqui fizemos duas coisas lemos ao mesmo tempo 
	// modificamos o byte de configura��o do controlador PS/2 	
	
	 // Defina a leitura do byte actual de configura��o do controlador PS/2.
	kbdc_wait(1);    
	outportb(0x64,0x20);    
	
	kbdc_wait(0);
	status = inportb(0x60)|2;  
	
	// defina, a escrita  de byte de configura��o do controlador PS/2.
	kbdc_wait(1);
	outportb(0x64,0x60);  

	// devolvemos o byte de configura��o modificado.
	kbdc_wait(1);
	outportb(0x60,status);  

	// #importante:
	// Agora temos dois dispositivos seriais teclado e mouse (PS/2).

    // Reabilitando portas.
	
//enablePorts:
	
	// Ativar a primeira porta PS/2.
	//kbdc_wait(1);
	//outportb(0x64,0xAE);   

	// Ativar a segunda porta PS/2.
	//kbdc_wait(1);
	//outportb(0x64,0xA8);  

    // Done!
	
	// espera.
	// ?? Pra que isso ??
	//kbdc_wait(1); 


 
	
  // Reseta mouse (reset ? lento!)...
  // esperando 0xFA??
  // Espero pelo byte 0xaa que encerra a sequ?ncia de reset!
  kbdc_wait(1);
  mouse_write(0xff);
  while (mouse_read() != 0xFA);

   // printf("\n init_mouse: chamando BAT_TEST\n");
   // refresh_screen();
  
  // Restaura defaults do PS/2 mouse.
  kbdc_wait(1);
  mouse_write(0xf6);
  while (mouse_read() != 0xFA);


   // TODO: Pode ser interessante diminuir a sensibilidade do mouse
   // aqui!!!

  // Habilita o mouse streaming
  // Interessante notar que, no modo streaming,
  // 1 byte recebido do PS/2 mouse gerar  uma IRQ...
  // Talvez valha a pena DESABILITAR o modo streaming
  // para colher os 3 dados de uma s� vez na IRQ!
  kbdc_wait(1);
  mouse_write(0xf4);
  while (mouse_read() != 0xfa);         // ACK
  
	
	//
	// Aqui podemos tentar outros modos mais completos.
	//
	
//done:

    // Reabilitando as duas portas.
	
	// Ativar a primeira porta PS/2.
	//kbdc_wait(1);
	//outportb(0x64,0xAE);   

	// Ativar a segunda porta PS/2.
	//kbdc_wait(1);
	//outportb(0x64,0xA8); 	
    
	
	printf ("P8042_install: done\n");
	
    // NOTA. 
	// Esta configura��o discarta do teste do controlador PS/2 e de seus dispositivos. 
	// Depois fa�amos a configura��o decente e minuciosa do P8042.
};

*/

/*
 **********************************************************
 * load_mouse_bmp:  ## teste ##
 *     Carregando o arquivo MOUSE.BMP que � o ponteiro de mouse.
 *     Usar isso na inicializa��o do mouse.
 *     #bugbug isso pode ir para windowLoadGramadoIcons
 */
int load_mouse_bmp (){
	
	int Status = 1;
	int Index;
    struct page_frame_d *pf;
	unsigned long fileret;
	
#ifdef KERNEL_VERBOSE
	printf("load_mouse_bmp:\n");
#endif	
	
	// #Importante
	// Fizemos um teste e funcionou com 500 p�ginas.
	// Foi suficiente para testarmos um pano de fundo.
	// Ret = (void*) allocPageFrames(500);  
	
    // Alocando duas p�ginas para um BMP pequeno. 8KB.	
	mouseBMPBuffer = (void *) allocPageFrames(2);
	//mouseBMPBuffer = (void*) allocPageFrames(10);
	//mouseBMPBuffer = (void*) allocPageFrames(100);  //400KB
	
	if ( (void *) mouseBMPBuffer == NULL ){
		
	    printf("unblocked-ldisc-load_mouse_bmp: mouseBMPBuffer\n");
		goto fail;		
	};
	
	
	// #debug
	//printf("\n");
	//printf("BaseOfList={%x} Showing #32 \n",mouseBMPBuffer);
    //for(Index = 0; Index < 32; Index++)   	
	//{  
    //    pf = (void*) pageframeAllocList[Index]; 
	//	
	//	if( (void*) pf == NULL ){
	//	    printf("null\n");	 
	//	}
	//   if( (void*) pf != NULL ){
	//	    printf("id={%d} used={%d} magic={%d} free={%d} handle={%x} next={%x}\n",
	//          pf->id ,pf->used ,pf->magic ,pf->free ,pf ,pf->next ); 	
	//	}
	//}
	
	//
	// ## Loading ...  ##
	//
	
	//read_fntos( (char *) arg1);
	
	
	//===================================
	// MOUSE
	fileret = (unsigned long) fsLoadFile( "MOUSE   BMP", 
	                            (unsigned long) mouseBMPBuffer );
								  
	if( fileret != 0 )
	{
		printf("MOUSE.BMP FAIL\n");		
		// Escrevendo string na janela.
		//draw_text( gui->main, 10, 500, COLOR_WINDOWTEXT, "MOUSE.BMP FAIL");
		goto fail;	
	};
	
	// Render BMP file on backbuffer.
	//bmpDisplayBMP( mouseBMPBuffer, 20, 20 );
	//refresh_rectangle( 20, 20, 16, 16 );	
    //===================================							
	
	
	//===================================
	//DENNIS
	//fileret = (unsigned long) fsLoadFile( "DENNIS  BMP", 
	//                              (unsigned long) mouseBMPBuffer );
								  
	//if( fileret != 0 )
	//{
	//	printf("DENNIS  BMP FAIL\n");		
		// Escrevendo string na janela.
		//draw_text( gui->main, 10, 500, COLOR_WINDOWTEXT, "MOUSE.BMP FAIL");
	//	goto fail;	
	//};
	
	// Render BMP file on backbuffer.
	//bmpDisplayBMP( mouseBMPBuffer, 0, 0 );
    //refresh_rectangle( 0, 0, 800, 600 );	
	//refresh_rectangle( 20, 40, 400, 400 );
	//===================================
	
	
//refresh_rectangle:

	//Isso funcionou ...
	//refresh_rectangle( 20, 20, 16, 16 );
	Status = (int) 0;
	goto done;
	
fail:
    printf("fail\n");
    Status = (int) 1;		
done:

#ifdef KERNEL_VERBOSE
    printf("done\n");
#endif	
	//refresh_screen();
    return (int) Status;	
};


/*
 ***************
 * ps2:
 *
 *     #ps2 
 *
 *     Essa rotina de inicializa��o do controladro 
 * poder� ter seu pr�prio m�dulo.
 *     Inicializa o controlador ps2.
 *     Inicializa a porta do teclado no controlador.
 *     Inicializa a porta do mouse no controlador.
 *     Obs: *importante: A ordem precisa ser respeitada.
 *     As vezes os dois n�o funcionam ao mesmo tempo se a 
 *     inicializa��o n�o for feita desse jeito. 
 */
void ps2 (){
	
	//dever� ir para ps2.c @todo: criar arquivo.
	
	asm ("cli");
	
	printf("ps2: Initializing ...\n");
	
	
	// ## Step 0 ##
    // +Inicializa a estrutura de io control para dispositivo mouse.
    // +Inicializamos as globais usadas pelo handler do driver.
    // +Carregamos o BMP usado pelo driver. 
	
	ps2_mouse_globals_initialize();
	//refresh_screen();	
	
	
	
	// ## Step 1 ##
	// Desativar a primeira porta PS/2. (teclado)
	// Deixaremos para configur� lo depois,
  	// Fazemos isso pra ele n�o atrapalhar.
	
	kbdc_wait(1);
	outportb(0x64,0xAD);  
	
	
	// ## Step 2 ##
    // Inicializa��o do mouse no controlador ps2.
    // Reseta mouse.
    // +Restaura defaults do PS/2 mouse.
    // +Habilita o mouse streaming.
 
	ps2_mouse_initialize();	
	

    // ## Step 3 ##
    // Configurando o controlador ps2.
    // +destivamos as portas para os dois dispositivos.
    // +habilitamos a utiliza��o do dispositivo mouse. (talvez isso deva se feito depois.)

    ps2_initialize();

	

	// ## Step 4 ##
    //+Inicializa a estrutura de io control para o dispositivo teclado.
    //+Configurar as globais usadas pelo driver de teclado.
    //+Habilitar a porta de teclado. 
    //+reset.
    //+configura leds.
	
	ps2_keyboard_initialize ();  
	//refresh_screen();  	
 
	

    // ## Step 5 ##
	// Habilitando as duas portas.
	
	// Ativar a primeira porta PS/2.
	kbdc_wait(1);
	outportb(0x64,0xAE);   

	// Ativar a segunda porta PS/2.
	kbdc_wait(1);
	outportb(0x64,0xA8); 	
	
	
	
	printf("ps2: done\n");	
	refresh_screen();
	
	//while (1){
	//	asm ("cli");
	//	asm ("hlt");
	//}
};


void set_current_keyboard_responder ( int i ){
	
	current_keyboard_responder = i;
};


int get_current_keyboard_responder (){
	
	return (int) current_keyboard_responder;
};


void set_current_mouse_responder ( int i ){
	
    current_mouse_responder = i;	
};


int get_current_mouse_responder (){
	
    return (int) current_mouse_responder;	
};


//inicializando a fila de mensagens do sistema
//com ponteiros para estruturas de mensagens ...
//es estruturas ser�o reutiliz�veis.
void initialize_system_message_queue (){
	
	struct message_d *m;
	
	int i;
	
	for ( i=0; i<SYSTEM_MESSAGE_QUEUE_MAX; i++ )
	{
		
		m = (void *) malloc ( sizeof(struct message_d) );
		
		if ( (void *) m == NULL )
		{
			printf("unblocked-ldisc-initialize_system_message_queue:");
			die();
		}else{
			
			m->objectType = ObjectTypeMessage;
			m->objectClass = ObjectClassKernelObjects;
		    m->used = 1;
			m->magic = 1234;
			m->empty = 1;
            system_message_queue[i] = (unsigned long) m;		
		}; 
	};
	
	system_message_write = 0;
	system_message_read = 0;
};


// Inicializa o status das teclas de modifica��o.
// s�o usadas em comjunto com outras teclas para criar atalhos.
// modificam temporariamente a fun��o de outra tecla.
void ldisc_init_modifier_keys (){
	
	// Modifier keys.
	
	// Shift.
	shift_status = 0;
	
	// Control.
	ctrl_status = 0;
	
	// Win key.
	winkey_status = 0;
    
	// Alternate.
    alt_status = 0;

	// Alternate Graphic.
    //altgr_status = 0; //@todo
    
	// Function.
	//fn_status = 0;  //@todo
	
	//...
};


// modificam permanentemente a fun��o de outra tecla.
//ativa as teclas extendidas.
void ldisc_init_lock_keys (){
	
    // Capital Lock.	
	capslock_status = 0;
	
	// Scrolling Lock.
	scrolllock_status = 0;
	
	// Number Lock.
	numlock_status = 0;	
};


//
// End.
//

