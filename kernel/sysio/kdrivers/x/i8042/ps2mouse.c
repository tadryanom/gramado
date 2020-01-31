/*
 * File: i8042/ps2mouse.c
 * 
 *      + ps2 mouse controler support.
 *      + mouse event support.
 * 
 * Environment:
 *     Ring 0. Kernel base persistent code.
 *
 * History:
 *     2018 - Created by Fred Nora.
 */

// #todo
// talvez fazer estruturas para controlar as configura��es de mouse.
 
#include <kernel.h>


//++
// ===================
// Defines
// Credits: serenity OS
#define IRQ_MOUSE 1
#define I8042_BUFFER 0x60
#define I8042_STATUS 0x64
#define I8042_ACK 0xFA
#define I8042_BUFFER_FULL 0x01
#define I8042_WHICH_BUFFER 0x20
#define I8042_MOUSE_BUFFER 0x20
#define I8042_KEYBOARD_BUFFER 0x00
#define PS2MOUSE_SET_RESOLUTION 0xE8
#define PS2MOUSE_STATUS_REQUEST 0xE9
#define PS2MOUSE_REQUEST_SINGLE_PACKET 0xEB
#define PS2MOUSE_GET_DEVICE_ID 0xF2
#define PS2MOUSE_SET_SAMPLE_RATE 0xF3
#define PS2MOUSE_ENABLE_PACKET_STREAMING 0xF4
#define PS2MOUSE_DISABLE_PACKET_STREAMING 0xF5
#define PS2MOUSE_SET_DEFAULTS 0xF6
#define PS2MOUSE_RESEND 0xFE
#define PS2MOUSE_RESET 0xFF
#define PS2MOUSE_INTELLIMOUSE_ID 0x03
//--


// Screen width and height

extern unsigned long SavedX; 
extern unsigned long SavedY; 


//=============================
//mouse control

// habilitar e desabilitar o mouse.
// usada pelos aplicativos.
int ps2_mouse_status;

int ps2_mouse_has_wheel;

//1= pressionado 0=liberado
int ps2_button_pressed;

int ps2_mouse_moving;
int ps2_mouse_drag_status;
//int ps2_mouse_drop_status;

//====================================================================
// update_mouse support



// i8042 mouse status bit.
#define MOUSE_LEFT_BTN    0x01
#define MOUSE_RIGHT_BTN   0x02
#define MOUSE_MIDDLE_BTN  0x04
#define MOUSE_X_SIGN      0x10
#define MOUSE_Y_SIGN      0x20
#define MOUSE_X_OVERFLOW  0x40
#define MOUSE_Y_OVERFLOW  0x80



long mouse_x = 0;
long mouse_y = 0;

char mouse_packet_data = 0;
char mouse_packet_x = 0;
char mouse_packet_y = 0;
char mouse_packet_scroll = 0;
//=====================================================================




//====================================================================
//  mouseHandler support
#define MOUSE_DATA_BIT 1
#define MOUSE_SIG_BIT  2
#define MOUSE_F_BIT  0x20
#define MOUSE_V_BIT  0x08 

// Contador
static int count_mouse = 0;

// Buffer
static char buffer_mouse[3];


//=====================================================================



//
// Prototype
//

// Prot�tipos de fun��es internas.
int MOUSE_BAT_TEST (void);



/*
 ******************************************
 * MOUSE_SEND_MESSAGE:
 *     Send a mouse message. 
 */

// Pegamos os tres char do inout de mouse e 
// transformamos em uma mensagem que ser� enviada para uma thread.

int MOUSE_SEND_MESSAGE (void *buffer) {

    if ( (void *) buffer == NULL )
        return (int) -1;


    unsigned char *chars = (unsigned char *) buffer;

    //char char0 = (char) &chars[0];
    //char char1 = (char) &chars[1];
    //char char2 = (char) &chars[2];
    //char char3 = (char) &chars[3];
    //...


	// #todo
	// Colocar esses dados em uma mesnagem e enviar para thread.

    return (int) -1;
}



/*
 *********************************************
 * mouse_write:
 *     Envia um byte para a porta 0x60.
 */

//mudar o arg para data;

void mouse_write (unsigned char write){

    prepare_for_output();
    out8 (I8042_STATUS, 0xd4);

    prepare_for_output();
    out8 (I8042_BUFFER, write);
}




/*
 **************************************
 * mouse_read:
 *     Pega um byte na porta 0x60.
 */

unsigned char mouse_read (void){

    //kbdc_wait (0);
    //return (unsigned char) in8 (0x60);

    prepare_for_input();
    return (unsigned char) in8 (0x60);
}


/*
 *********************************** 
 * MOUSE_BAT_TEST:
 * 
 */

// Esta rotina faz o Auto-teste. 
// 0xaa �xito, 0xfc erro.
// Created by Fed Nora.

int MOUSE_BAT_TEST (void){

    int val = -1;
    int i = 0;


	// #todo:
	// Cuidado.
	// Diminuir isso se for possivel.
	// Nao funciona na maquina reala sem esse delay.

    for (i=0; i<99000; i++)
    {
        wait_ns (400);
    };


	//while (1)

    for ( i=0; i<999; i++ )
    {
        val = mouse_read ();

        if (val == 0xAA)
        {
            printf ("MOUSE_BAT_TEST OK\n");
            return 0;

        }else if (val == 0xFC){

            printf ("MOUSE_BAT_TEST fail\n");
            return (int) -1; 
        };
    
        // Reenviar o comando. 
        // OBS: este comando n�o � colocado em buffer

        mouse_write (0xFE);       
    };

    // #bugbug:
    // N�o podemos exibir essa mensagem repetidas vezes.
    printf ("MOUSE_BAT_TEST %d times\n", i );
}



/*
 ***********************************************
 * ps2_mouse_globals_initialize:
 *     Inicializando o mouse no controlador 8042.
 *     Carregando o bmp para o curso do mouse.
 * 
 * History:
 *     2018 - Created by Fred Nora.  
 */

int ps2_mouse_globals_initialize (void){

    unsigned char response = 0;
    unsigned char deviceId = 0;
    int i = 0; 
    int bruto = 1;  //M�todo.
    int mouse_ret = 0;


	//printf("ps2_mouse_globals_initialize: inicializando estrutura\n");
	//refresh_screen ();


	//user.h
    ioControl_mouse = (struct ioControl_d *) kmalloc ( sizeof(struct ioControl_d) );

    if ( (void *) ioControl_mouse == NULL )
    {
        panic ("ps2_mouse_globals_initialize: ioControl_mouse fail\n");

    }else{

        ioControl_mouse->id = 0;
        ioControl_mouse->used = 1;
        ioControl_mouse->magic = 1234;

		//Qual thread est� usando o dispositivo.
        ioControl_mouse->tid = 0;  
        //ioControl_mouse->
    };


	//printf("ps2_mouse_globals_initialize: inicializando variaveis\n");
	//refresh_screen ();

    // #importante
    // habilitando o mouse ps2.
    ps2_mouse_status = 1;


	// Estamos espa�o para o buffer de mensagens de mouse.
	// mousemsg = ( unsigned char *) kmalloc(32);


	//Inicializando as vari�veis usadas na rotina em Assemly
	//em hardwarelib.inc

    //#todo:
    //Podemos inicialziar com o mouse no centro da tela.

	//Coordenadas do cursor.
    g_mousepointer_x = (unsigned long) 0;
    g_mousepointer_y = (unsigned long) 0;
    mouse_x = 0;
    mouse_y = 0;


	// #bugbug: 
	// Essa inicializa��o est� travando o mouse.
	// Fazer com cuidado.


	//#bugbug. Cuidado com essa inicializa�ao.
    g_mousepointer_width = 16;
    g_mousepointer_height = 16;


    //Bytes do controlador.
    //mouse_packet_data = 0;
    //mouse_packet_x = 0;
    //mouse_packet_y = 0;
    //mouse_packet_scroll = 0;


	//
	// ## BMP ##
	//


	//printf ("ps2_mouse_globals_initialize: carregando bmp\n");
	//refresh_screen();

	// Carregando o bmp do disco para a mem�ria
	// e apresentando pela primeira vez.

    mouse_ret = (int) load_mouse_bmp ();

    if (mouse_ret != 0)
    {
        panic ("ps2_mouse_globals_initialize: load_mouse_bmp\n");
    }



	//printf("ps2_mouse_globals_initialize: done\n");
	//refresh_screen ();

    //initialized = 1;
    //return (kernelDriverRegister(mouseDriver, &defaultMouseDriver));


    return 0;
}



/*
 ******************************************************
 * ps2mouse_initialize_device
 * ...
 */

void ps2mouse_initialize_device (void){

    __breaker_ps2mouse_initialized = 0;

    unsigned char status = 0;
    
    
    //
    // Globals first.
    //
    
    // Inicializa vari�veis de gerenciamento do driver.
    ps2_mouse_globals_initialize();


//__enable_second_port :

    //++
    //======================================================
    
    // #obs:
    // A rotina abaixo habilita o segundo dispositivo. O mouse.

    // Dizemos para o controlador entrar no modo leitura.
    // Esperamos para ler e lemos.
    wait_then_write (0x64,I8042_READ);    // I8042_READ = 0x20    
    status = wait_then_read(0x60) | 2;

    // Dizemos para o controlador entrar no modo escrita.
    // Esperamos para escrever e escrevemos.
    // Enable the PS/2 mouse IRQ (12).
    // The keyboard uses IRQ 1 (and is enabled by bit 0 in this register).
    wait_then_write (0x64,I8042_WRITE);   // I8042_WRITE = 0x60
    wait_then_write (0x60,status);   
    
    //======================================================
    //--    



	// 0xFF
	// Espera o dados descer (ACK)
	// Basic Assurance Test (BAT)



//__set_default:

    //++
    //=================================================
    
    // #obs:
    // A rotina abaixo faz o mouse ficar com sua conigura��o padr�o.

    // Set default settings.
    mouse_write (PS2MOUSE_SET_DEFAULTS);
    expect_ack();

    //=================================================
    //--
    
    
    
//__enable_streaming:    

    //++
    //=================================================
    
    // #obs:
    // A rotina abaixo configura o modo streaming do mouse.


    // Enable streaming.
    mouse_write(PS2MOUSE_ENABLE_PACKET_STREAMING);
    expect_ack();
    
    //=================================================
    //--    
    
    
    
//__enable_wheel:    
  
    //++
    //=================================================
  
    // #obs:
    // A rotina abaixo habilita a rodinha, se o dispositivo possui.
    // Credits: Serenity OS.    

    // Pega o device id e faz configura��es de wheel.
    mouse_write(PS2MOUSE_GET_DEVICE_ID);
    expect_ack();
    
    unsigned char device_id = mouse_read(); 
    
    if (device_id != PS2MOUSE_INTELLIMOUSE_ID){

        // Send magical wheel initiation sequence.
        mouse_write(PS2MOUSE_SET_SAMPLE_RATE);
        expect_ack();
        mouse_write(200);
        expect_ack();
        mouse_write(PS2MOUSE_SET_SAMPLE_RATE);
        expect_ack();
        mouse_write(100);
        expect_ack();
        mouse_write(PS2MOUSE_SET_SAMPLE_RATE);
        expect_ack();
        mouse_write(80);
        expect_ack();

        mouse_write(PS2MOUSE_GET_DEVICE_ID);
        expect_ack();
        device_id = mouse_read();
    }

    if (device_id == PS2MOUSE_INTELLIMOUSE_ID) {

        //m_has_wheel = true;
        ps2_mouse_has_wheel = 1;
        kprintf ("mouse_install: Mouse wheel enabled!\n");
        
    } else {
        kprintf ("mouse_install: No mouse wheel detected!\n");
    };

    //=================================================
    //--

    // Wait for nothing!
    kbdc_wait (1);
    kbdc_wait (1);
    kbdc_wait (1);
    kbdc_wait (1);
    
    __breaker_ps2mouse_initialized = 1;
}



/*
 ***************************************************
 * getMouseData:
 *     Essa fun��o � usada pela rotina kernelPS2MouseDriverReadData.
 * Input a value from the keyboard controller's data port, 
 * after checking to make sure that there's some mouse data 
 * there for us.
 */

static unsigned char getMouseData (void){

    unsigned char data = 0;

    while ((data & 0x21) != 0x21)
        data = in8 (0x64);


    data = in8 (0x60);

    return (data);
}


/*
 ******************************************************
 * kernelPS2MouseDriverReadData:  
 *     Pega os bytes e salva em um array.
 *     Exibe um caractere na tela, dado as cordenadas.
 *     This gets called whenever there is a mouse interrupt
 *     @todo: Rever as entradas no array.         
 */

void kernelPS2MouseDriverReadData (void)
{
    //#suspensa !
}


/*
 *******************************************************
 * load_mouse_bmp:  
 * 
 *     ## teste ##
 * 
 *     Carregando o arquivo MOUSE.BMP, que � o ponteiro de mouse.
 *     Usar isso na inicializa��o do mouse.
 * 
 *     #bugbug isso pode ir para windowLoadGramadoIcons
 * 
 *     2018 - Created by Fred Nora.
 */

int load_mouse_bmp (void){

    int Status = 1;
    int Index = 0; 
    unsigned long fileret = 0;


#ifdef KERNEL_VERBOSE
    //printf ("load_mouse_bmp:\n");
#endif


    // Alocando duas p�ginas para um BMP pequeno. 8KB.

	mouseBMPBuffer = (void *) allocPages (2);
	//mouseBMPBuffer = (void *) allocPages (10);


    if ( (void *) mouseBMPBuffer == NULL )
    {
        printf ("load_mouse_bmp: mouseBMPBuffer\n");
        goto fail;
    }


	//
	// ## Loading ...  ##
	//

	//read_fntos( (char *) arg1);


	//===================================
	// MOUSE

    fileret = (unsigned long) fsLoadFile ( VOLUME1_FAT_ADDRESS,
                                  VOLUME1_ROOTDIR_ADDRESS, 
                                  "MOUSE   BMP", 
                                  (unsigned long) mouseBMPBuffer );

    if ( fileret != 0 )
    {
        printf ("MOUSE.BMP FAIL\n");
        
		// Escrevendo string na janela.
		//draw_text( gui->main, 10, 500, COLOR_WINDOWTEXT, "MOUSE.BMP FAIL");

        goto fail;
    }


	// Render BMP file on backbuffer.
	//bmpDisplayBMP( mouseBMPBuffer, 20, 20 );
	//refresh_rectangle( 20, 20, 16, 16 );
	//===================================


//refresh_rectangle:

	//Isso funcionou ...
	//refresh_rectangle( 20, 20, 16, 16 );

    Status = (int) 0;
    goto done;


fail:
    printf ("fail\n");
    Status = (int) 1;

done:

#ifdef KERNEL_VERBOSE
    printf ("done\n");
#endif

	//refresh_screen();
    return (int) Status;
}


// events.h
void set_current_mouse_responder (int i)
{
    current_mouse_responder = i;
}


// events.h
int get_current_mouse_responder (void)
{
    return (int) current_mouse_responder;
}






/*
 * =====================================================
 * update_mouse:
 *     Updates the mouse position.
 */

void update_mouse (void){

//======== X ==========
// Testando o sinal de x.
// Do the x pos first.

//pega o delta x
//testa o sinal para x
do_x:

    if ( mouse_packet_data & MOUSE_X_SIGN ) 
    {
        goto x_neg;
    }


//Caso x seja positivo.
x_pos:

    mouse_x += mouse_packet_x;
    goto do_y;


//Caso x seja negativo.
x_neg:

    mouse_x -= ( ~mouse_packet_x + 1 );

    if (mouse_x > 0)
    {
        goto do_y;
    }
    mouse_x = 0;
 
 
//======== Y ==========
// Testando o sinal de x. 
// Do the same for y position.

//Pega o delta y.
//Testa o sinal para y.
do_y:

    if ( mouse_packet_data & MOUSE_Y_SIGN )
    {
        goto y_neg;
    }


//Caso y seja positivo.
y_pos:

    mouse_y -= mouse_packet_y;

    if ( mouse_y > 0 )
    {
        goto quit;
    }

    mouse_y = 0;
    goto quit;


//Caso y seja negativo. 
y_neg:

    mouse_y += ( ~mouse_packet_y + 1 );

// Quit
quit:
    return;
}


// o driver de mouse entrega informa��es sobre o evento atual
// para quem pedir. Como o servidor de janelas atual.
unsigned long ps2_mouse_get_info ( int i )
{
    switch (i)
    {
        case 1:
            return (unsigned long) saved_mouse_x;
            break;

        case 2:
            return (unsigned long) saved_mouse_y;
            break;
            
        case 3:
            return (unsigned long) mouse_x;
            break;
            
        case 4:
            return (unsigned long) mouse_y;
            break;
            
        case 5:
            return (unsigned long) ps2_mouse_moving;
            break;

        case 6:
            return (unsigned long) ps2_mouse_drag_status;
            break;
            
        case 7:
            return (unsigned long) mouse_button_action;
            break;            


        case 8:
            return (unsigned long) mouse_buttom_1;
            break;
            
        case 9:
            return (unsigned long) mouse_buttom_2;
            break;
            
        case 10:
            return (unsigned long) mouse_buttom_3;
            break;



        case 11:
            return (unsigned long) old_mouse_buttom_1;
            break;
            
        case 12:
            return (unsigned long) old_mouse_buttom_2;
            break;
            
        case 13:
            return (unsigned long) old_mouse_buttom_3;
            break;
            
            
        case 14:
            return (unsigned long) ps2_button_pressed;
            break;

          //...
    }	
}



void ps2mouse_change_and_show_pointer_bmp ( int number ){

    switch (number)
    {
		
		case 1:
	       refresh_rectangle ( saved_mouse_x, saved_mouse_y, 20, 20 );
	       bmpDisplayMousePointerBMP ( mouseBMPBuffer, mouse_x, mouse_y ); 		
		   break;
		   
		case 2:
	      refresh_rectangle ( saved_mouse_x, saved_mouse_y, 20, 20 );
	       bmpDisplayMousePointerBMP ( appIconBuffer, mouse_x, mouse_y ); 		
		   break;		
		
		case 3:
	       refresh_rectangle ( saved_mouse_x, saved_mouse_y, 20, 20 );
	       bmpDisplayMousePointerBMP (fileIconBuffer, mouse_x, mouse_y ); 		
		   break;

		case 4:
	      refresh_rectangle ( saved_mouse_x, saved_mouse_y, 20, 20 );
	       bmpDisplayMousePointerBMP (folderIconBuffer, mouse_x, mouse_y ); 		
		   break;

		case 5:
	       refresh_rectangle ( saved_mouse_x, saved_mouse_y, 20, 20 );
	       bmpDisplayMousePointerBMP (terminalIconBuffer, mouse_x, mouse_y ); 		
		   break;
		   
		case 6:
	      refresh_rectangle ( saved_mouse_x, saved_mouse_y, 20, 20 );
	       bmpDisplayMousePointerBMP (cursorIconBuffer, mouse_x, mouse_y ); 		
		   break;
		   
		 //#estamos usando os �cones previamente carregados.  
		   
		 //...  
		   
		default:
	      refresh_rectangle ( saved_mouse_x, saved_mouse_y, 20, 20 );
	       bmpDisplayMousePointerBMP ( mouseBMPBuffer, mouse_x, mouse_y ); 		
		   break;
    }
}


// #todo: 
// Isso aqui deveria colocar um pacote na fila
// para o window server pegar depois.
void ps2mouse_parse_data_packet (void)
{

	// A partir de agora j� temos os tr�s chars.
	// Colocando os tr�s chars em vari�veis globais.
	// Isso ficar� assim caso n�o haja overflow.

    mouse_packet_data = buffer_mouse[0];    // Primeiro char
    mouse_packet_x    = buffer_mouse[1];    // Segundo char.
    mouse_packet_y    = buffer_mouse[2];    // Terceiro char.
            
    //
    // ==== Posicionamento ====
    //            
  

	// Salvando o antigo antes de atualizar.
	// Para poder apagar daqui a pouco.
	// Atualizando.
    saved_mouse_x = mouse_x;
    saved_mouse_y = mouse_y;
    update_mouse (); 
 
    // Agora vamos manipular os valores obtidos atrav�s da 
    // fun��o de atualiza��o dos valores.
    // A fun��o de atualiza��o atualizou os valores de
    // mouse_x e mouse_y.
    mouse_x = (mouse_x & 0x000003FF );
    mouse_y = (mouse_y & 0x000003FF );

	// #importante:
	// Checando limites.
	// Isso � provis�rio.

    if ( mouse_x < 1 ){ mouse_x = 1; }
    if ( mouse_y < 1 ){ mouse_y = 1; }
    if ( mouse_x > (SavedX-16) ){ mouse_x = (SavedX-16); }
    if ( mouse_y > (SavedY-16) ){ mouse_y = (SavedY-16); }

    // Comparando o novo com o antigo, pra saber se o mouse se moveu.
    // #obs: Pra quem mandaremos a mensagem de moving ??
    if ( saved_mouse_x != mouse_x || saved_mouse_y != mouse_y )
    {
		// flag: o mouse est� se movendo.
		// usaremos isso no keydown.
		// >> na hora de enviarmos uma mensagem de mouse se movendo
		// se o bot�o estiver pressionado ent�o temos um drag (carregar.)
		// um release cancela o carregar.
		
        ps2_mouse_moving = 1;

       // draw

       // Apaga o antigo.
       // + Copia no LFB um ret�ngulo do backbuffer 
       // para apagar o ponteiro antigo.
       refresh_rectangle ( saved_mouse_x, saved_mouse_y, 20, 20 );
                
       // Acende o novo.
       //+ Decodifica o mouse diretamente no LFB.
       // Copiar para o LFB o antigo ret�ngulo  
       // para apagar o ponteiro que est� no LFB.
       bmpDisplayMousePointerBMP ( mouseBMPBuffer, mouse_x, mouse_y );   
                         
    }else{
		
		// N�o redesenhamos quando o evento for um click, sem movimento.
        ps2_mouse_moving = 0;
    }; 


    //
    // ==== Bot�o ====
    //            


	//Apenas obtendo o estado dos bot�es.
    mouse_buttom_1 = 0;
    mouse_buttom_2 = 0;
    mouse_buttom_3 = 0;



	// ## LEFT ##
    if ( ( mouse_packet_data & MOUSE_LEFT_BUTTON ) == 0 )
    {
		//liberada.
        mouse_buttom_1 = 0;
        ps2_button_pressed = 0;
        
        //mudamos sempre que pressionar.
        //todo: mudaremos sempre que pressionar numa title bar.
        //refresh_rectangle ( saved_mouse_x, saved_mouse_y, 20, 20 );
        //bmpDisplayMousePointerBMP ( mouseBMPBuffer, mouse_x, mouse_y ); 

    }else if( ( mouse_packet_data & MOUSE_LEFT_BUTTON ) != 0 )
     {
		    //pressionada.
		    //N�o tem como pressionar mais de um bot�o por vez.
            mouse_buttom_1 = 1;
            mouse_buttom_2 = 0;
            mouse_buttom_3 = 0;
            ps2_button_pressed = 1;
            
            //mudamos sempre que pressionar.
            //todo: mudaremos sempre que pressionar numa title bar.
            //refresh_rectangle ( saved_mouse_x, saved_mouse_y, 20, 20 );
            //bmpDisplayMousePointerBMP ( appIconBuffer, mouse_x, mouse_y ); 
     };



	// ## RIGHT ##
    if ( ( mouse_packet_data & MOUSE_RIGHT_BUTTON ) == 0 )
    {
	    //liberada.
        mouse_buttom_2 = 0;
        ps2_button_pressed = 0;

    }else if( ( mouse_packet_data & MOUSE_RIGHT_BUTTON ) != 0 )
        {
		    //pressionada.
		    //N�o tem como pressionar mais de um bot�o por vez.
            mouse_buttom_1 = 0;
            mouse_buttom_2 = 1;
            mouse_buttom_3 = 0;
            ps2_button_pressed = 1;
        };



	// ## MIDDLE ##
    if ( ( mouse_packet_data & MOUSE_MIDDLE_BUTTON ) == 0 )
    {
	    //liberada.
        mouse_buttom_3 = 0;
        ps2_button_pressed = 0;

    }else if( ( mouse_packet_data & MOUSE_MIDDLE_BUTTON ) != 0 )
        {
	        //pressionada.
	        //N�o tem como pressionar mais de um bot�o por vez.
	        mouse_buttom_1 = 0;
	        mouse_buttom_2 = 0;
	        mouse_buttom_3 = 1;
	        ps2_button_pressed = 1;
        };




	// ===
	// Confrontando o estado atual com o estado anterior para saber se ouve 
	// alguma altera��o ou n�o.
	// 1 = ouve altera��o.

    if ( mouse_buttom_1 != old_mouse_buttom_1 ||
         mouse_buttom_2 != old_mouse_buttom_2 ||
         mouse_buttom_3 != old_mouse_buttom_3 )
    {
        mouse_button_action = 1;

    }else{
        mouse_button_action = 0;
    };
}



/*
 ********************************************************
 * mouseHandler:
 *     Handler de mouse. 
 *
 * *Importante: 
 *     Se estamos aqui � porque os dados dispon�veis no controlador 8042 
 * pertencem ao mouse.
 *
 * Obs: 
 * Temos externs no in�cio desse arquivo.
 */

void mouseHandler (void){

	// #importante:
	// Essa ser� a thread que receber� a mensagem.
	//struct thread_d *t;


	// #importante:
	// Essa ser� a janela afetada por qualquer evento de mouse.
	// ID de janela.

    //struct window_d *Window;
    //int wID; 
    
    //#test
    //int last_wID;



	// Coordenadas do mouse.
	// Obs: Isso pode ser global.
	// ?? O tratador em assembly tem as vari�veis globais do posicionamento.

    int posX = 0;
    int posY = 0;


	// Lendo um char no controlador.
    char *_byte;


    //#todo: Isso � um teste.
    // O mouse ps2 est� desabilitado por�m recebendo as interup��es
    // Os aplicativos podem reabilit�-lo a qualquer momento.
    // Atribuindo foco por exemplo.
    
    //#bugbug
    //Isso realmente paraliza o mouse. Mas reabilitar n�o est� funcionando.
    //if ( ps2_mouse_status == 0 )
    //{
    //      return;
    // }


	//Char para o cursor provis�rio.
	//#todo: Isso foi subtituido por uma bmp. Podemos deletar.
    //static char mouse_char[] = "T";


	//
	// Read !
	//

	// #obs:
	// Lendo um char no controlador.

    *_byte = (char) mouse_read ();


	// #importante:
	// Contagem de interru��es:
	// Obs: Precisamos esperar 3 interrup��es.


	//#obs: count_mouse � global. Provavelmente nesse arquivo mesmo.

    switch ( count_mouse )
    {

		// > Essa foi a primeira interrup��o.
        case 0:
            //Pegamos o primeiro char.
            buffer_mouse[0] = (char) *_byte;
            if (*_byte & MOUSE_V_BIT)
                count_mouse++;
            break;


		// >> Essa foi a segunda interrup��o.
        case 1:
            //Pegamos o segundo char.
            buffer_mouse[1] = (char) *_byte;
            count_mouse++;
            break;


		// >>> Essa foi a terceira interrup��o. � a �ltima.
        case 2:
            //Pegamos o terceiro char.
            buffer_mouse[2] = (char) *_byte;
            count_mouse = 0;
            
            ps2mouse_parse_data_packet();
            
            //
            // Message.
            //
            
            //#bugbug
            // escaneando janelas.
            // O window server deveria fazer isso.
            // kgws/kgws.c
            kgws_mouse_scan_windows ();
            
            // O driver precisa do old pra configurar a vari�vel de a��o.
            // #todo Talvez precise de outras
            old_mouse_buttom_1 = mouse_buttom_1;
            old_mouse_buttom_2 = mouse_buttom_2;
            old_mouse_buttom_3 = mouse_buttom_3;
            break;


        // Problemas na contagem de interrup��es.
        default:
            count_mouse = 0;
            return;
            break;
    };
}


void expect_ack  (void){

    while ( mouse_read() != 0xFA);
}





void set_ps2_mouse_status(int status)
{
	ps2_mouse_status = status;
}

int get_ps2_mouse_status(void)
{
	return (int) ps2_mouse_status;
}


unsigned long 
ps2_mouse_dialog ( int msg,
                   unsigned long long1,
                   unsigned long long2 )
{
    switch (msg)
    {
		//habilitar
        case 4000:
            printf ("ps2_mouse_dialog: 4000\n");
            refresh_screen();
            ps2_mouse_status = 1;
            break;

        //desabilitar.
        case 4001:
            printf ("ps2_mouse_dialog: 4001\n");
            refresh_screen();
            ps2_mouse_status = 0;
            break;

        //#test
        // reinicializar ??
        case 4002:
            break;
            
        default:
            break;
    };


    return 0;
}


//
// End.
//

