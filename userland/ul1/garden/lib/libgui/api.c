/*
 * File: api.c 
 *
 * Description:
 *     Application Programming Interface - (API).
 *
 *     Arquivo principal da API 32bit.
 *     Call Kernel via interrupt.
 *     Estamos em User Mode e usaremos os serviços do Kernel através de 
 * chamadas via interrupção.
 *     +As interrupções de serviços do KERNEL vão de 48 à 199.
 *     +A interrupções principal é a 200.
 *     +As interrupções de chamadas especiais vão de 201 até 215. 
 *     +As interrupções de serviços da GUI vão de 216 à 219.
 *
 * Attention: 
 *    Atenção ao passar argumentos via memória, pois utilizamos 
 * endereços lógicos.
 *
 *  @todo: logon, segurança, autenticação, bancodedados de autenticação.
 *         Heap support:
 *  @todo: Get PID, TID.       
 *
 * Obs:
 *     O kernel base deve proteger suas estruturas. Os processos devem 
 * proteger suas estruturas uns dos outros. É natural então que seja
 * passado somente o índice de uma lista de ponteiros de estruturas.
 * Quem tiver em mão o índice, solicita algum elemento da estrutura
 * para quem tem permissão de manipular a estrutura. 
 *     Passar o ponteiro de uma estrutura pode ser um problema de segurança,
 * além do transtorno de converter endereço lógico e físico. Mas existem
 * heaps de memória compartilhada, onde devem ficar algumas estruturas.
 * 
 * Obs:
 * Podemos passar todos os argumentos de uma estrutura para kernelmode
 * onde o kernel criou um estrutura semelhante e protegida. O kernel nos
 * devolve um índice que só pode ser manipulado pelo processo que pediu
 * para o kernel criar a estrutura. Assim essa estrutura fica protegida
 * dentro do heap do kernel. 
 *
 * Obs: Muitos dos recursos oferecidos aqui por essa api, que é ligada 
 *      às aplicações em tempo de compilação, serão oferecidos
 *      por uma api em user mode na forma de server, ou biblioteca
 *      de link dinâmico. (.so, .dll ...).
 *
 * In this file:
 * ============
 *     +sustem_call(....); - Interrupção do systema. Número 200.
 *     +system() - Interpreta um comando via argumento. 
 *     + Muitas outras ...
 *
 *
 * History: 
 *     2014 - Created by Fred Nora.
 *     2015 - New services.
 *     2016 - New services.
 *     2018 - Revision.
 *     ...
 */



#include <types.h>        
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <heap.h>   

#include "include/api.h"     


// buttons
struct window_d *first_responder;
struct window_d *messagebox_button1;
struct window_d *messagebox_button2;
struct window_d *dialogbox_button1;
struct window_d *dialogbox_button2;


//
// Protótipos de funções internas.
//


unsigned long 
mbProcedure ( struct window_d *window, 
              int msg, 
              unsigned long long1, 
              unsigned long long2 );


unsigned long 
dbProcedure ( struct window_d *window, 
              int msg, 
              unsigned long long1, 
              unsigned long long2 );


/*
 ***********************************************************************
 * system_call:  
 *    Interrupção de sistema, número 0x80, chama vários serviços do Kernel com 
 * a mesma interrupção. Essa é a chamada mais simples.
 *
 * Argumentos:
 *    eax = arg1, O número do serviço.
 *    ebx = arg2. 
 *    ecx = arg3.
 *    edx = arg4.
 *
 * 2015 - Created by Fred Nora.
 * 2016 ~ 2019 - New functions.
 * ...
 */

void *system_call ( unsigned long ax, 
                    unsigned long bx, 
                    unsigned long cx, 
                    unsigned long dx )
{

	//##BugBug: Aqui 0 retorno não pode ser inteiro.
	//Temos que pegar unsigned long?? void*. ??
	//unsigned long RET = 0;

    int RET = 0;

    asm volatile ("int %1 \n"
                  : "=a"(RET)
                  : "i"(IA32_SYSCALL_VECTOR), "a"(ax), "b"(bx), "c"(cx), "d"(dx) );

    return (void *) RET;
}


/*
 ***********************************
 * gde_system: 
 *    Interpreta um comando e envia uma systemcall para o kernel.
 *    Obs: Assim como system, isso deve chamar aplicativos. 
 * ?? Talvez deva chamar o próprio shell ??     
 */

int gde_system (const char *command){
    
    // @todo: Checar se comando é válido, se os primeiros caracteres
	//        são espaço. Ou talvez somente compare, sem tratar o argumento.

	//@todo:
	// Criar rotina para pular os caracteres em branco no início do comando.
	
	//@todo: version, ...
	
	//OBS: ESSES SÃO OS COMANDOS DO SISTEMA, USADOS POR TODOS OS PROGRAMAS
	//     QUE INCLUIREM A API. 
	
	//test - Exibe uma string somente para teste.
	if ( gde_strncmp( (char *) command, "test", 4 ) == 0 )
	{
	    printf("gde_system: Testing commands..\n");
		goto exit;
	}
  
	//ls - List files in a folder.
	if ( gde_strncmp( (char *) command, "ls", 2 ) == 0 )
	{
	    printf("gde_system: @todo: ls..\n");
		goto exit;
	} 
	
	//makeboot - Cria arquivos e diretórios principais.
	if ( gde_strncmp( (char *) command, "makeboot", 8 ) == 0 )
	{
	    printf("gde_system: @todo: makeboot..\n");
		
		//ret_value = fs_makeboot();
		//if(ret_value != 0){
		//    printf("shell: makeboot fail!");
		//};
		
        goto exit;
    }
	
	//format.
	if ( gde_strncmp( (char *) command, "format", 6 ) == 0 )
	{
	    printf("gde_system: @todo: format..\n");
		//fs_format(); 
        goto exit;
    }
	
	//debug.
	if ( gde_strncmp( (char *) command, "debug", 5 ) == 0 )
	{
	    printf("gde_system: @todo: debug..\n");
		//debug();
        goto exit;
    }
	
    //dir.
	if ( gde_strncmp( (char *) command, "dir", 3 ) == 0 )
	{
	    printf("gde_system: @todo: dir..\n");
		//fs_show_dir(0); 
        goto exit;
    }

	//newfile.
	if ( gde_strncmp( (char *) command, "newfile", 7 ) == 0 )
	{
	    printf("gde_system: ~newfile - Create empty file.\n");
		//fs_create_file( "novo    txt", 0);
        goto exit;
    }
	
	//newdir.
	if ( gde_strncmp( (char *) command, "newdir", 7 ) == 0 )
	{
	    printf("gde_system: ~newdir - Create empty folder.\n");
		//fs_create_dir( "novo    dir", 0);
        goto exit;
    }
	
    //mbr - Testa mbr.
    if ( gde_strncmp( (char *) command, "mbr", 3 ) == 0 )
	{
	    printf("gde_system: ~mbr\n");
		//testa_mbr();
		goto exit;
    }; 
	
    //root - Testa diretório /root.
    if ( gde_strncmp( (char *) command, "root", 4 ) == 0 )
	{
	    printf("gde_system: ~/root\n");
		//testa_root();
		goto exit;
    }; 

	//start.
    if ( gde_strncmp( (char *) command, "start", 5 ) == 0 )
	{
	    printf("gde_system: ~start\n");
		goto exit;
    }; 
	
	//help.
	//?? O que mostrar aqui ??
    if ( gde_strncmp( (char *) command, "help", 4 ) == 0 )
	{
		printf("gde_system: help stuff\n");
		//printf(help_string);
		//print_help();
		goto exit;
    };
	
	//cls.
    if ( gde_strncmp( (char *) command, "cls", 3 ) == 0 )
	{
	    printf("gde_system: cls\n");
		//black
	    //api_clear_screen(0);
        goto exit;
	};
	
	//save.
	if ( gde_strncmp( (char *) command, "save", 4 ) == 0 )
	{
	    printf("gde_system: ~save\n");
        goto exit;
    };
	
	//install.
	//muda um arquivo da area de transferencia para 
	//o sistema de arquivos...
	if ( gde_strncmp( (char *) command, "install", 7 ) == 0 )
	{
	    printf("gde_system: ~install\n");
		//fs_install();
        goto exit;
    };
	
	
	//boot - Inicia o sistema.
	if ( gde_strncmp( (char *) command, "boot", 4 ) == 0 )
	{
	    printf("gde_system: ~boot\n");
		//boot();
        goto exit;
    };

	//service
	if ( gde_strncmp( (char *) command, "service", 7 ) == 0 )
	{
	    printf("gde_system: ~service - rotina de servicos do kernel base\n");
		//test_services();
        goto exit;
    };

	//slots - slots de processos ou threads.
	if ( gde_strncmp( (char *) command, "slots", 5 ) == 0 )
	{
	    printf("gde_system: ~slots - mostra slots \n");
		//mostra_slots();
        goto exit;
    };
	
	
    //
    // Continua ...
    //
	
	//exit - Exit the current program
    if ( gde_strncmp( (char *) command, "exit", 4 ) == 0 )
	{
		printf("gde_system: exit\n");
		gde_exit (0);
		goto fail;
    };
		
    //reboot.
	if ( gde_strncmp( (char *) command, "reboot", 6 ) == 0 )
	{
		printf ("gde_system: reboot\n");
		gde_reboot ();
		goto fail;
    };

	//shutdown.
    if ( gde_strncmp( (char *) command, "shutdown", 8 ) == 0 )
	{
		printf("gde_system: shutdown\n");
		gde_shutdown();
        goto fail;
    };
	
	//@todo: exec
	
    //:default
	printf ("gde_system: Unknown command!\n");
	
	//
	// o que devemos fazer aqui é pegar o nome digitado e comparar
	// com o nome dos arquivos do diretório do sistema. se encontrado,
	// devemos carregar e executar.
	//
	
// Fail. Palavra não reservada.	
fail:
    printf ("gde_system: FAIL\n");
    return (int) 1;

// #todo: 
// Esse exit como vari'avel local precisa mudar de nome	
// para não confundir com a função exit de sair do processo.
// uma opção é usar 'done:'. 
	
exit:
	
    return 0;
}


//
// ================ ## int 201 ~ int 215 ## =================
//

//#obs:
// Essas interrupções ainda não foram configuradas.
// cada uma delas servirá para um grupo distinto de serviços.
// A int 0x80 ainda é a interrupção principal do sistema.


/*
 * system1:
 *     int 201 */

int system1 ( unsigned long ax, 
              unsigned long bx, 
			  unsigned long cx, 
			  unsigned long dx )
{
    int ret_val;
	
    asm volatile (" int %1 \n"
		          : "=a"(ret_val)	
		          : "i"(201), "a"(ax), "b"(bx), "c"(cx), "d"(dx) );
				  
	return (int) ret_val;
}


/*
 * system2:
 *     int 202 */

int system2 ( unsigned long ax, 
              unsigned long bx, 
			  unsigned long cx, 
			  unsigned long dx )
{
    int ret_val;
	
    asm volatile (" int %1 \n"
		          : "=a"(ret_val)		
		          : "i"(202), "a"(ax), "b"(bx), "c"(cx), "d"(dx) );
				  
	return (int) ret_val;
}


/*
 * system3:
 *     int 203 */

int system3 ( unsigned long ax, 
              unsigned long bx, 
			  unsigned long cx, 
			  unsigned long dx )
{
    int ret_val;
	
    asm volatile ( "int %1\n"
		          : "=a"(ret_val)	
		          : "i"(203), "a"(ax), "b"(bx), "c"(cx), "d"(dx) );
				  
	return (int) ret_val;
}


/*
 * system4:
 *     int 204 */

int system4 ( unsigned long ax, 
              unsigned long bx, 
			  unsigned long cx, 
			  unsigned long dx )
{
    int ret_val;
	
    asm volatile ("int %1\n"
		          : "=a"(ret_val)		
		          : "i"(204), "a"(ax), "b"(bx), "c"(cx), "d"(dx) );
				  
	return (int) ret_val;
}


/*
 * system5:
 *     int 205 */

int system5 ( unsigned long ax, 
              unsigned long bx, 
			  unsigned long cx, 
			  unsigned long dx )
{
    int ret_val;
	
    asm volatile ("int %1\n"
		          : "=a"(ret_val)		
		          : "i"(205), "a"(ax), "b"(bx), "c"(cx), "d"(dx) );
				  
	return (int) ret_val;
}


/*
 * system6:
 *     int 206 */

int system6 ( unsigned long ax, 
              unsigned long bx, 
			  unsigned long cx, 
			  unsigned long dx )
{
    int ret_val;
	
    asm volatile ("int %1\n"
		          : "=a"(ret_val)		
		          : "i"(206), "a"(ax), "b"(bx), "c"(cx), "d"(dx) );
				  
	return (int) ret_val;
}


/*
 * system7:
 *     int 207 */

int system7 ( unsigned long ax, 
              unsigned long bx, 
			  unsigned long cx, 
			  unsigned long dx )
{
    int ret_val;
	
    asm volatile ("int %1\n"
		          : "=a"(ret_val)		
		          : "i"(207), "a"(ax), "b"(bx), "c"(cx), "d"(dx) );
				  
	return (int) ret_val;
}


/*
 * system8:
 *     int 208 */

int system8 ( unsigned long ax, 
              unsigned long bx, 
			  unsigned long cx, 
			  unsigned long dx )
{
    int ret_val;
	
    asm volatile ("int %1\n"
		          : "=a"(ret_val)		
		          : "i"(208), "a"(ax), "b"(bx), "c"(cx), "d"(dx) );
				  
	return (int) ret_val;
}


/*
 * system9:
 *     int 209 */

int system9 ( unsigned long ax, 
              unsigned long bx, 
			  unsigned long cx, 
			  unsigned long dx )
{
    int ret_val;
	
    asm volatile ("int %1\n"
		          : "=a"(ret_val)		
		          : "i"(209), "a"(ax), "b"(bx), "c"(cx), "d"(dx) );
				  
	return (int) ret_val;
}


/*
 * system10:
 *     int 210 */

int system10 ( unsigned long ax, 
               unsigned long bx, 
			   unsigned long cx, 
			   unsigned long dx )
{
    int ret_val;
	
    asm volatile ("int %1\n"
		          : "=a"(ret_val)		
		          : "i"(210), "a"(ax), "b"(bx), "c"(cx), "d"(dx) );
				  
	return (int) ret_val;
}


/*
 * system11:
 *     int 211 */

int system11 ( unsigned long ax, 
               unsigned long bx, 
			   unsigned long cx, 
			   unsigned long dx )
{
    int ret_val;
	
    asm volatile ("int %1\n"
		          : "=a"(ret_val)		
		          : "i"(211), "a"(ax), "b"(bx), "c"(cx), "d"(dx) );
				  
	return (int) ret_val;
}


/*
 * system12:
 *     int 212 */

int system12 ( unsigned long ax, 
               unsigned long bx, 
			   unsigned long cx, 
			   unsigned long dx )
{
    int ret_val;
	
    asm volatile ("int %1\n"
		          : "=a"(ret_val)		
		          : "i"(212), "a"(ax), "b"(bx), "c"(cx), "d"(dx) );
				  
	return (int) ret_val;
}


/*
 * system13:
 *     int 213 */

int system13 ( unsigned long ax, 
               unsigned long bx, 
			   unsigned long cx, 
			   unsigned long dx )
{
    int ret_val;
	
	asm volatile (" int %1 \n"
		          : "=a"(ret_val)		
		          : "i"(213), "a"(ax), "b"(bx), "c"(cx), "d"(dx) );
				  
	return (int) ret_val;
}


/*
 * system14:
 *     int 214 */

int system14 ( unsigned long ax, 
               unsigned long bx, 
			   unsigned long cx, 
			   unsigned long dx )
{
    int ret_val;
	
    asm volatile (" int %1 \n "
		          : "=a"(ret_val)		
		          : "i"(214), "a"(ax), "b"(bx), "c"(cx), "d"(dx) );
				  
	return (int) ret_val;
}


/*
 * system15:
 *      int 215 */

int system15 ( unsigned long ax, 
               unsigned long bx, 
			   unsigned long cx, 
			   unsigned long dx )
{
	int ret_val;
	
	asm volatile ("int %1\n"
		          : "=a"(ret_val)		
		          : "i"(215), "a"(ax), "b"(bx), "c"(cx), "d"(dx) );
				  
	return (int) ret_val;
}


//=====================================
// #obs
// some other functions.


/*
 **********************************************
 * gde_refresh_buffer:
 * 
 *     int 0x80, serviços de 1 à 9.
 *     Refresh buffer.
 *     Atualiza um buffer dado seu número.  (rever) 
 */

void gde_refresh_buffer (unsigned long n){
	
	// Limits.
    if (n < 1 || n > 9)
    {
        return;
    }


    switch (n){

        //refresh buffer 1,2,3
        
		case 1: 
		    system_call( 1, 0, 0, 0); 
			break;
        case 2: 
		    system_call( 2, 0, 0, 0); 
			break;
        case 3: 
		    system_call( 3, 0, 0, 0); 
			break;
                 
        //refresh screen     
        case 4: 
		    system_call( 4, 0, 0, 0); 
			break;
             
        //refresh title bar    
        case 5: 
		    system_call( 5, 0, 0, 0); 
			break;
                 
        //refresh menu bar     
        case 6: 
		    system_call( 6, 0, 0, 0); 
			break;
                 
        //refresh tollbar    
        case 7: 
		    system_call( 7, 0, 0, 0); 
			break;
                 
        //refresh status bar   
        case 8: 
		    system_call( 8, 0, 0, 0); 
			break;

        //refresh taskbar   
        case 9: 
		    system_call( 9, 0, 0, 0); 
			break;
                 
        //default:
            //Nothing
			//break;
    }
}


/*
 * gde_print_string:
 *     int 0x80, serviço 10.
 *     Print string no buffer.  (rever) 
 */

void 
gde_print_string ( unsigned long x,  
               unsigned long y,  
               unsigned long color, 
               unsigned char *string )
{
    //#todo
}


/*
 **********************
 * gde_vsync:
 *     Int 0x80, serviço 15.
 *     Sincroniza o retraço vertical do monitor.  (rever) 
 */

int gde_vsync (){

    return (int) gramado_system_call ( SYSTEMCALL_VSYNC, 0, 0, 0 );
}


int gws_vsync () {

    return (int) gde_vsync ();
}

/*
 ******************************************************
 * gde_system_procedure:
 *     Chama o procedimento de janela padrão do sistema.
 *     Ele está no kernel.
 */

void *gde_system_procedure ( struct window_d *window,
                             int msg,
                             unsigned long long1,
                             unsigned long long2 )
{

    unsigned long message_buffer[5]; 


    message_buffer[0] = (unsigned long) window; 
    message_buffer[1] = (unsigned long) msg;
    message_buffer[2] = (unsigned long) long1;
    message_buffer[3] = (unsigned long) long2;
    
    
    // Isso evita muitos problemas.
    if ( (void *) window == NULL )
    {
        return NULL;
    }

    return (void *) system_call ( SYSTEMCALL_CALL_SYSTEMPROCEDURE, 
                        (unsigned long) &message_buffer[0], 
                        (unsigned long) &message_buffer[0], 
                        (unsigned long) &message_buffer[0] );
}




/*
 **********************************
 * __gde_set_cursor:
 *
 */

int __gde_set_cursor (unsigned long x, unsigned long y){

    return (int) gramado_system_call ( SYSTEMCALL_SETCURSOR, x, y, 0 );
}


/*
 * __gde_put_char:
 *     Put char.
 */

void 
__gde_put_char ( unsigned long x, 
               unsigned long y, 
               unsigned long color, 
               unsigned char *ch )
{
    //return;    //Nothing for now.
}


/*
 *  gde_load_bitmap_16x16:
 *     Coloca um bitmap no buffer.
 *     interrupção 200, serviço SYSTEMCALL_LOAD_BITMAP_16x16, 
 *     pôe um bitmap no buffer.
 *
 *     ?? @todo: Rever, onde pega e pra onde vai.
 *
 * a - endreço da imagem. 
 * b - x
 * c - y 
 * d - null
 *
 * Obs: 
 *     Lembrar que estamos usando endereços lógicos.  (rever)
 */

void 
gde_load_bitmap_16x16 ( unsigned long img_address, 
                        unsigned long x, 
                        unsigned long y )
{
    system_call ( SYSTEMCALL_LOAD_BITMAP_16x16, img_address, x, y ); 
}


/*
 **********************
 * gde_shutdown:
 *     Shutdown the machine.
 */

void gde_shutdown (){


    //gramado_system_call ( ??, 0, 0, 0 );

    while (1){ asm ("pause"); };
}






/*
 * gde_init_background:
 *     Initialize default background configuration.  (rever) 
 */

void gde_init_background ()
{
    //todo: Implementar.	
    //Não há uma chamada para isso ainda.
}


/*
 ***************************************************
 * gde_message_box:
 *     Message box on gui->screen.
 *     Types=[1~5]
 *     @todo: Devemos considerar o retorno? E se a chamada falhar?
 */
 
int __mb_current_button; 

// #todo: 
// usar get system metrics
	
int gde_message_box ( int type, char *string1, char *string2 ){

    int Response = 0;
    int running = 1;  //Loop.

    struct window_d *hWnd;    //Window.

	// x and y
	// #todo 
	// centralizado: metade | um terço.
	// #todo: 
	// Pegar a métrica do dispositivo.

    unsigned long x  = (unsigned long) 10;       //deslocamento x
    unsigned long y  = (unsigned long) 300;      //deslocamento y
    unsigned long cx = (unsigned long) (800/2);  //largura 
    unsigned long cy = (unsigned long) (600/3);  //altura

    int Button = 0;

    unsigned long WindowClientAreaColor;
    unsigned long WindowColor;

	//
	// Colors.
	//

    WindowClientAreaColor = xCOLOR_GRAY3;   
    WindowColor = COLOR_TERMINAL2;  


    //seleciona o botão 1. first responder;
    __mb_current_button = 1;
    first_responder = messagebox_button1;

	// Obs: 
	// Por enquanto para todos os tipos de messagebox 
	// estamos usando o mesmo tipo de janela.


	//if (type <0)
		//return -1;


    switch (type)
    {
        case 1:
        case 2:
            return (int) -1;
            break;

        case 3:
            goto do_create_messagebox_3;
            break;

        case 4:
        default:
            return (int) -1;
            break;
    };


//
// Create Window.
//

do_create_messagebox_3:

    // Características.
    Button = 1;
    //...
    

    //++
    gde_enter_critical_section ();
    
    hWnd = (void *) gde_create_window ( WT_OVERLAPPED, 1, 1, string1, 
                        x, y, 
                        cx, cy, 
                        NULL, 0, 
                        WindowClientAreaColor, WindowColor );


    if ( (void *) hWnd == NULL )
    {
        printf ("gde_message_box: hWnd\n");
        return (int) -1;

    }else{

        gde_register_window (hWnd);
        //APISetActiveWindow (hWnd);
        //gde_set_focus (hWnd);
        
        gde_draw_text ( (struct window_d *) hWnd,
            1*(cx/16), 1*(cy/3), 
            COLOR_WHITE, string2 );

        gde_show_window (hWnd);
    };

    gde_exit_critical_section ();
    //--


	//======================================
	// button support
	//
	
	//obs: o procedure vai precisar dos botões então tem que declarar global.
	
	//unsigned long app1Left = ((ScreenWidth/8) * 2);
	//unsigned long app2Left = ((ScreenWidth/8) * 3);
	
	//unsigned long app1Top = ( (ScreenHeight/10) * 8); 
	//unsigned long app2Top = app1Top; 



    //++
	// button 1
    gde_enter_critical_section ();

    messagebox_button1 = (void *) gde_create_window ( WT_BUTTON, 1, 1, "OK", 
                                      (cx/3), ((cy/8)*5), 
                                      80, 24,    
                                      hWnd, 0, 
                                      xCOLOR_GRAY1, xCOLOR_GRAY1 );

    if ( (void *) messagebox_button1 == NULL )
    {
        printf ("OK button fail\n");
        return (int) -1;
    }else{
        gde_register_window (messagebox_button1); 
        gde_show_window (messagebox_button1);
    };

    gde_exit_critical_section ();
    //--


    //++
	// button 2
	gde_enter_critical_section ();

	messagebox_button2 = (void *) gde_create_window ( WT_BUTTON, 1, 1, "CANCEL",     
                                      ((cx/3)*2), ((cy/8)*5), 
                                      80, 24,    
                                      hWnd, 0, 
                                      xCOLOR_GRAY1, xCOLOR_GRAY1 );

    if ( (void *) messagebox_button2 == NULL )
    {
        printf (" CANCEL button fail \n");
        return (int) -1;
    }else{
        gde_register_window (messagebox_button2); 
        gde_show_window (hWnd);
    };
    
    gde_exit_critical_section();
    //--




    // string
    // apiDrawText ( (struct window_d *) hWnd,
    //    1*(cx/16), 1*(cy/3), COLOR_WINDOWTEXT, string1 );	

    // #sustenso:
    // Show window.
    // apiShowWindow (hWnd);


	//
	//  ==== Loop ====
	//



	// loop support
	unsigned long message_buffer[5];

    message_buffer[0] = 0;
    message_buffer[1] = 0;
    message_buffer[3] = 0;
    message_buffer[4] = 0;



Mainloop:

    while (running)
    {

        //++
        gde_enter_critical_section ();
        system_call ( 111, 
            (unsigned long) &message_buffer[0],
            (unsigned long) &message_buffer[0], 
            (unsigned long) &message_buffer[0] );
        gde_exit_critical_section();
        //--


        if ( message_buffer[1] != 0 )
        {
            Response = (int) mbProcedure ( (struct window_d *) message_buffer[0], 
                                 (int) message_buffer[1], 
                                 (unsigned long) message_buffer[2], 
                                 (unsigned long) message_buffer[3] );

            // Temos uma resposta.
            if (Response > 100)
            {
                printf ("Response=%d \n", Response );
                goto exit_messagebox;
            };

            message_buffer[0] = 0;
            message_buffer[1] = 0;
            message_buffer[3] = 0;
            message_buffer[4] = 0;
        };
    };


//
// Exit.
//

exit_messagebox:

    // #debug
    //printf ("Exiting Message Box \n");

    return (int) Response;
}



/*
 **************************************************
 * mbProcedure:
 *     O procedimento padrão de message box. 
 *     #interna
 */

unsigned long 
mbProcedure ( struct window_d *window, 
              int msg, 
              unsigned long long1, 
              unsigned long long2 )
{

    switch (msg)
	{
		// MSG_MOUSEKEYDOWN
		case 30:
		    switch (long1)
		    {
				case 1://botão 1
					if ( window == messagebox_button1 )
					{
                    gramado_system_call ( 9900,   
                          (unsigned long) window, 
                          (unsigned long) window, 
                          (unsigned long) window );
						  return (unsigned long) 0;
						break;
					}	
					if ( window == messagebox_button2 )
					{
                     gramado_system_call ( 9900,   
                          (unsigned long) window, 
                          (unsigned long) window, 
                          (unsigned long) window );
						return (unsigned long) 0;
						break;
					}
				   break;
			};
		    break;
		    
		//mouse keyup    
		case 31:
		    switch (long1)
		    {
				case 1://botão 1
					if ( window == messagebox_button1 )
					{
                    gramado_system_call ( 9901,   
                          (unsigned long) window, 
                          (unsigned long) window, 
                          (unsigned long) window );
						return (unsigned long) 102; //resposta ok
						break;
					}	
					if ( window == messagebox_button2 )
					{
                     gramado_system_call ( 9901,   
                          (unsigned long) window, 
                          (unsigned long) window, 
                          (unsigned long) window );
						return (unsigned long) 101; //resposta no
						break;
					}
				   break;
			};
		    break;

        case MSG_KEYUP:
            switch(long1)
            {
				    
				case VK_RETURN:
				    //goto __release_current_button;
				    return (unsigned long) 0;
				    break;
				   
                default:
				    //printf ("defaul keydown\n");
				    return (unsigned long) 0;
                    break; 
            };
            break;


        case MSG_KEYDOWN:
            switch(long1)
            {
                case VK_ESCAPE:
				    printf ("scape\n");
                    return (unsigned long) 101; //resposta no
				    break;
				    
				 case VK_TAB:
				     //if ( __mb_current_button == 1 )
				     //{
						// __mb_current_button = 2; 
						 //first_responder = messagebox_button2; 
						 //gde_set_focus (first_responder);
						 //return (unsigned long) 0;
						 //break; 
				     //}
				     //if ( __mb_current_button == 2 )
				     //{
						// __mb_current_button = 1; 
						 //first_responder = messagebox_button1;
						 //gde_set_focus (first_responder);
						 //return (unsigned long) 0;
						 //break; 
					 //}
					 return (unsigned long) 0;
				     break;
				    
				case VK_RETURN:
				    //goto __press_current_button;
				    return (unsigned long) 0;
				    break;
				   
                default:
				    //printf ("defaul keydown\n");
				    return (unsigned long) 0;
                    break; 
            };
            break;
            

        case MSG_SYSKEYDOWN: 
            switch(long1)	       
            {	
				//Test.
				case VK_F1:
				    printf ("f1 Yes\n");
                    return (unsigned long) 102; //resposta ok.
					break;
					
                case VK_F2:
				    printf ("f2 No\n");
                    return (unsigned long) 101; //resposta no.
					break;

				default:
				    //printf ("default sys key down\n");
				    return (unsigned long) 0;
				    break;
		    };              
            break;
		
	    case MSG_SYSKEYUP:
		   //printf ("sys key up\n");
		   return (unsigned long) 0; 
           break;
		
        //@todo case command .. button ??
		
		default:
		    //printf ("default message\n");
		    return (unsigned long) 0;
            break;
	};

	printf ("done\n");
	return (unsigned long) 0;
	
	
/*	
__press_current_button:

    // button_down
    // Quando um botão é clicado ou pressionado,
    // ele será repintado com a aparência de botão apertado.
    switch ( __mb_current_button )
    {
		case 1:
        gramado_system_call ( 9900,   
             (unsigned long) window, 
             (unsigned long) window, 
             (unsigned long) window );
		     break;
		     
		case 2:
        gramado_system_call ( 9900,   
             (unsigned long) window, 
             (unsigned long) window, 
             (unsigned long) window );
		     break;
    };
    return (unsigned long) 0;
*/

/*
__release_current_button:

    // button_down
    // Quando um botão é clicado ou pressionado,
    // ele será repintado com a aparência de botão apertado.
    switch ( __mb_current_button )
    {
		case 1:
        gramado_system_call ( 9901,   
             (unsigned long) window, 
             (unsigned long) window, 
             (unsigned long) window );
             return (unsigned long) 102;
		     break;
		     
		case 2:
        gramado_system_call ( 9901,   
             (unsigned long) window, 
             (unsigned long) window, 
             (unsigned long) window );
             return (unsigned long) 101;
		     break;
    };
	return (unsigned long) 0; 
*/
   
}


/*
 ******************************
 * gde_dialog_box:
 *     
 *     Types=[1~5]
 *     @todo: Devemos considerar o retorno? E se a chamada falhar? 
 */

int gde_dialog_box ( int type, char *string1, char *string2 ){
    
    // Antes nós chamávamos o kernel, agora tentaremos implantar na api.
	
	//system_call ( SYSTEMCALL_MESSAGE_BOX, (unsigned long) type, 
	//	(unsigned long) string1, (unsigned long) string2 );
	

	//#debug
	//printf ("Testing dialog Box type=%d \n", type);

    int Response = 0;
    int running = 1;

    //
    // Draw !
    //
	
	struct window_d *hWnd;    //Window.
	struct window_d *pWnd;    //Parent.
	struct window_d *bWnd;    //Button.
	
	
	//#todo: usar get system metrics
	
	// x and y
	// @todo centralizado: metade | um terço.
	// @todo: Pegar a métrica do dispositivo.
	unsigned long x  = (unsigned long) 10;       //deslocamento x
	unsigned long y  = (unsigned long) 300;      //deslocamento y
    unsigned long cx = (unsigned long) (800/2);  //largura   
    unsigned long cy = (unsigned long) (600/3);  //altura

	int Button = 0;	
	
	unsigned long WindowClientAreaColor;
	unsigned long WindowColor;
	
	
	WindowClientAreaColor = COLOR_YELLOW;
	WindowColor = COLOR_PINK;	
	
	//Obs: Por enquanto para todos os tipos de messagebox 
	// estamos usando o mesmo tipo de janela.
	switch (type)
	{	
	    // Com botão, considera o título.
	    case 1:
		    gde_begin_paint ();
		    Button = 1;
			//janela tipo simples.
	        hWnd = (void*) gde_create_window (  WT_SIMPLE, 1, 1, string1, 
			                x, y, cx, cy, NULL, 0, 
							WindowClientAreaColor, WindowColor); 
			if ( (void *) hWnd == NULL ){
				printf("hWnd fail\n");
			};
			gde_end_paint ();
            
			gde_register_window (hWnd);
            gde_set_active_window (hWnd);
            gde_set_focus (hWnd);
		    break;
			
		// Sem botão, considera o título.	
	    case 2:
		    Button = 0;
	        hWnd = (void*) gde_create_window ( WT_POPUP, 1, 1, string1, 
			                x, y, cx, cy, NULL, 0, 
							WindowClientAreaColor, WindowColor); 
	        break;
			
		// Com botão, Título de alerta.	
	    case 3:
		    //janela de aplicativo.
	        Button = 1;
			hWnd = (void*) gde_create_window ( WT_OVERLAPPED, 1, 1, "Alert", 
			                x, y, cx, cy, NULL, 0, 
							WindowClientAreaColor, WindowColor); 
	        break;
			
		//Com botão, título de mensagem do sistema.	
	    case 4:
		    Button = 1;
	        hWnd = (void*) gde_create_window ( WT_OVERLAPPED, 1, 1, "System Message", 
			                x, y, cx, cy, NULL, 0, 
							WindowClientAreaColor, WindowColor); 
	        break;
			
		//Tipo negligenciado. Usamos o formato padrão.	
		default:
		    Button = 1;
	        hWnd = (void*) gde_create_window ( WT_OVERLAPPED, 1, 1, "Error", 
			                x, y, cx, cy, NULL, 0, 
							WindowClientAreaColor, WindowColor); 
		    break;
	};
	
	
	//
	// button
	//
	
	//obs: o procedure vai precisar dos botões então tem que declarar global.
	
	//unsigned long app1Left = ((ScreenWidth/8) * 2);
	//unsigned long app2Left = ((ScreenWidth/8) * 3);
	
	//unsigned long app1Top = ( (ScreenHeight/10) * 8); 
	//unsigned long app2Top = app1Top; 



	// button 1 	
	dialogbox_button1 = (void *) gde_create_window ( WT_BUTTON, 1, 1, "OK",     
                                (cx/3), ((cy/8)*5), 80, 24,    
                                hWnd, 0, xCOLOR_GRAY1, xCOLOR_GRAY1 );
								
	if ( (void *) dialogbox_button1 == NULL )
	{
	    printf("button fail \n");
	}else{
        gde_register_window (dialogbox_button1);   	
	}

     
	// button 2
	dialogbox_button2 = (void *) gde_create_window ( WT_BUTTON, 1, 1, "CANCEL",     
                                ((cx/3)*2), ((cy/8)*5), 80, 24,    
                                hWnd, 0, xCOLOR_GRAY1, xCOLOR_GRAY1 );
								
	if ( (void *) dialogbox_button2 == NULL )
	{
	    printf("button fail \n");
	}else{
        gde_register_window (dialogbox_button2);   	
	}
	
	
	//
	// string
	//
	
    //apiDrawText ( (struct window_d *) hWnd,
    //    1*(cx/16), 1*(cy/3), COLOR_WINDOWTEXT, string1 );	
	
    gde_draw_text ( (struct window_d *) hWnd,
        1*(cx/16), 1*(cy/3), COLOR_WHITE, string1 );	
        

    gde_show_window (hWnd);
	
	//==================================
	// loop support;
	//
	
	unsigned long message_buffer[5];

    message_buffer[0] = 0;
    message_buffer[1] = 0;
    message_buffer[3] = 0;
    message_buffer[4] = 0;

Mainloop:
	
	while (running)
	{
		gde_enter_critical_section ();
		system_call ( 111, (unsigned long)&message_buffer[0],
			(unsigned long)&message_buffer[0], 
			(unsigned long)&message_buffer[0] );
        gde_exit_critical_section();
			
		if ( message_buffer[1] != 0 )
		{
	        
			Response = (int) dbProcedure ( (struct window_d *) message_buffer[0], 
		                        (int) message_buffer[1], 
		                        (unsigned long) message_buffer[2], 
		                        (unsigned long) message_buffer[3] );

			if (Response > 100)
			{
				printf ("Response=%d \n", Response );
				goto exit_dialogbox;
			}

            message_buffer[0] = 0;
            message_buffer[1] = 0;
            message_buffer[3] = 0;
            message_buffer[4] = 0;
        };
    };


//
// Exit.
//

exit_dialogbox:

	//#debug
	printf ("Exiting dialog Box \n");

    return (int) Response;
}



/*
 **************************************************
 * dbProcedure:
 *     O procedimento padrão de dialog box. */

unsigned long 
dbProcedure ( struct window_d *window, 
              int msg, 
              unsigned long long1, 
              unsigned long long2 )
{
    switch (msg)
	{
        case MSG_KEYDOWN:
            switch(long1)
            {
                case VK_ESCAPE:	
				    printf ("scape\n");
                    return (unsigned long) 101;  
				    break;
				   
                default:
				    //printf ("defaul keydown\n");
				    return (unsigned long) 0;
                    break; 
            };
            break;
	
        case MSG_SYSKEYDOWN:                 
            switch(long1)	       
            {	
				//Test.
				case VK_F1:
				    printf ("f1\n");
                    return (unsigned long) 102;
					break;
					
                //case VK_F2:
				    //Nothing.
				//	break;
									
				default:
				    //printf ("default sys key down\n");
				    return (unsigned long) 0;
				    break;
		    };              
            break;
		
	    case MSG_SYSKEYUP:
		   //printf ("sys key up\n");
		   return (unsigned long) 0; 
           break;
		
        //@todo case command .. button ??
		
		default:
		    //printf ("default message\n");
		    return (unsigned long) 0;
            break;		
	};
	
	//Refresh screen. 
	//?? deletar.
	//if(VideoBlock.useGui == 1){
	//    refresh_screen();
	//};

	printf ("done\n");
	return (unsigned long) 0;
}


/*
 **************************************************
 * call_kernel:
 *     (FAST CALLS)
 *     @todo: Change name to apiCallKernel(...) 
 *
 *     Chama os serviços de Kernel. São usadas várias interrupções, 
 * uma para cada número de serviço. O Kernel pode oferecer vários 
 * serviços relativos à ele mesmo e à gerencia do sistema, como requests
 * por exemplo.
 *
 * Obs: Deve haver alguma restrição para o uso dessa chamada.
 * Quem deve usar ela são os drivers e servers. 
 * Os recursos acessados por essa chamada são mais críticos e 
 * sistemicamente importantes.  (rever)
 *
 * #todo: esse retorno deve ser unsigned long
 */

int call_kernel ( unsigned long int_number, 
                  unsigned long service_number, 
                  unsigned long arg1, 
                  unsigned long arg2, 
                  unsigned long arg3, 
                  unsigned long arg4,
                  unsigned long arg5, 
                  unsigned long arg6, 
                  unsigned long arg7, 
                  unsigned long arg8,
                  unsigned long arg9, 
                  unsigned long arg10, 
                  unsigned long arg11, 
                  unsigned long arg12 )
{
	int ret_val;


    switch (int_number){ 
                      
        //48 - Vários serviços de Kernel Requests.
        case KERNEL:
            asm volatile ( "int %1 \n"
		                  : "=a"(ret_val)		
		                  : "i"(KERNEL), "a"(arg1), "b"(arg2), "c"(arg3), "d"(arg4) );
	                       break;
    
        //49
        case KERNEL49:
            asm volatile ( "int %1 \n"
		                  : "=a"(ret_val)		
		                  : "i"(KERNEL49), "a"(arg1), "b"(arg2), "c"(arg3), "d"(arg4) );
	                       break;
             
        //50
        case KERNEL50:
            asm volatile ( "int %1 \n"
		                  : "=a"(ret_val)		
		                  : "i"(KERNEL50), "a"(arg1), "b"(arg2), "c"(arg3), "d"(arg4) );
	                       break;
        
        //51     
        case KERNEL51:
            asm volatile ( "int %1 \n"
		                  : "=a"(ret_val)		
		                  : "i"(KERNEL51), "a"(arg1), "b"(arg2), "c"(arg3), "d"(arg4) );
	                       break;
             
        //52
        case KERNEL52:
            asm volatile ( "int %1 \n"
		                  : "=a"(ret_val)		
		                  : "i"(KERNEL52), "a"(arg1), "b"(arg2), "c"(arg3), "d"(arg4) );
	                       break;

        //53
        case KERNEL53:
            asm volatile ( "int %1 \n"
		                  : "=a"(ret_val)		
		                  : "i"(KERNEL53), "a"(arg1), "b"(arg2), "c"(arg3), "d"(arg4) );
	                       break;
        
        //54     
        case KERNEL54:
            asm volatile ( "int %1 \n"
		                  : "=a"(ret_val)		
		                  : "i"(KERNEL54), "a"(arg1), "b"(arg2), "c"(arg3), "d"(arg4) );
	                       break;

        
        case KERNEL55:
            asm volatile ( "int %1 \n"
		                  : "=a"(ret_val)		
		                  : "i"(KERNEL55), "a"(arg1), "b"(arg2), "c"(arg3), "d"(arg4) );
	                       break;
             
        case KERNEL56:
            asm volatile ( "int %1 \n"
		                  : "=a"(ret_val)		
		                  : "i"(KERNEL56), "a"(arg1), "b"(arg2), "c"(arg3), "d"(arg4) );
	                       break;
             
        case KERNEL57:
            asm volatile ( "int %1 \n"
		                  : "=a"(ret_val)		
		                  : "i"(KERNEL57), "a"(arg1), "b"(arg2), "c"(arg3), "d"(arg4) );
	                       break;
             
        //
		// ...
		//
		
		case KERNEL199:
            asm volatile ( "int %1 \n"
		                  : "=a"(ret_val)		
		                  : "i"(KERNEL199), "a"(arg1), "b"(arg2), "c"(arg3), "d"(arg4) );
	                       break;
        default:
            //Nothing.
            break;
    };


    return (int) ret_val;
}



/*
 ********************************************************
 * call_gui:
 *     (FAST CALLS)
 *     Chama os serviçoos de GUI que estão
 *     em Kernel mode.
 *
 *     Uma interrupçãoo para cada serviço.
 *
 *     Quando o Kernel híbrido ofereçe
 *     rotinas gráficas há um ganho de desempenho.
 *
 *     Um subsistema de recursos gráficos em user mode
 *     pode usar essa chamada para utilizar recursos 
 *     gráficos do kernel.  (rever)
 *
 *  #todo: esse retorno deve ser unsigned long.
 */

int call_gui ( unsigned long int_number, 
               unsigned long service_number, 
               unsigned long arg1, 
               unsigned long arg2, 
               unsigned long arg3, 
               unsigned long arg4,
               unsigned long arg5, 
               unsigned long arg6, 
               unsigned long arg7, 
               unsigned long arg8,
               unsigned long arg9, 
               unsigned long arg10, 
               unsigned long arg11, 
               unsigned long arg12 )
{
	int ret_val;  
	
	//Vários serviços. 
			
    switch (int_number){      

        //Fast Create Window.		
        case FAST_CREATE_WINDOW:
		//case GUI:
            asm volatile ( " int %1 \n"
		                  : "=a"(ret_val)		
		                  : "i"(FAST_CREATE_WINDOW), "a"(arg1), "b"(arg2), "c"(arg3), "d"(arg4) );
	                       break;
    
        case GUI217:
            asm volatile ( "int %1 \n"
		                  : "=a"(ret_val)		
		                  : "i"(GUI217), "a"(arg1), "b"(arg2), "c"(arg3), "d"(arg4) );
	                       break;
             
        case GUI218:
            asm volatile ( "int %1 \n"
		                  : "=a"(ret_val)		
		                  : "i"(GUI218), "a"(arg1), "b"(arg2), "c"(arg3), "d"(arg4) );
	                       break;
             
        case GUI219:
            asm volatile ( "int %1 \n"
		                  : "=a"(ret_val)		
		                  : "i"(GUI219), "a"(arg1), "b"(arg2), "c"(arg3), "d"(arg4) );
	                       break;
		default:
		    //Nothing.
		    break; 
    };


    return (int) ret_val;
}


/*
 *******************************************************************
 * gde_create_window: 
 *     Cria uma janela com base em uma struct.
 *     Retorna o endereço da estrutura da janela criada. 
 *     Para que possa ser registrada na lista windowList[].
 * 
 *    Não é necessário passar todos os argumentos de uma vez só.
 *	  Podemos realizar 3 ou 4 chamadas para construírmos a janela.
 *	  Essa rotina tem 12 argumentos mas ela poderá realizar 3 chamadas
 *	  ao sistema para passar todos os argumentos.	
 *
 * Cria a janela dependendo do tipo:                              
 * =================================
 * 1 - POP-UP.
 * 2 - EDIT-BOX.
 * 3 - NORMAL ( bg + client area ).
 * 4 - NORMAL_EDIT ( bg + editbox).
 * 5 - MENUBAR.
 * 6 - TOOLBAR.
 * 7 - STATUSBAR.
 * 8 - TASKBAR.
 * 9 - ? 
 * CONTINUA ...
 *
 */

void *gde_create_window ( unsigned long type,        //1, Tipo de janela (popup,normal,...)
                          unsigned long status,      //2, Estado da janela (ativa ou nao)
                          unsigned long view,        //3, (min, max ...)
                          char *windowname,          //4, Título.                          
                          unsigned long x,           //5, Deslocamento em relação às margens do Desktop.                           
                          unsigned long y,           //6, Deslocamento em relação às margens do Desktop.
                          unsigned long width,       //7, Largura da janela.
                          unsigned long height,      //8, Altura da janela.
                          struct window_d *pWindow,  //9, Endereço da estrutura da janela mãe.
                          unsigned long onde,        //10, Ambiente.( Est� no desktop, barra, cliente ...)
                          unsigned long clientcolor, //11, Cor da área de cliente
                          unsigned long color )      //12, Color (bg) (para janela simples).
{ 
	struct window_d *Window;

	// Enviando tudo via argumento.
	// Esse método dá a possibilidade de enviarmos ainda 
	// mais argumentos. 
	// #importante: Isso está funcionado, Vamos fazer assim e 
	// não do jeito antigo.
	
	unsigned long message_buffer[12];
	
	message_buffer[0] = (unsigned long) type;
	message_buffer[1] = (unsigned long) status;
	message_buffer[2] = (unsigned long) view;
	message_buffer[3] = (unsigned long) windowname;
	message_buffer[4] = (unsigned long) x;
	message_buffer[5] = (unsigned long) y;
	message_buffer[6] = (unsigned long) width;
	message_buffer[7] = (unsigned long) height;
	message_buffer[8] = (unsigned long) pWindow;
	message_buffer[9] = (unsigned long) onde;
	message_buffer[10] = (unsigned long) clientcolor;
	message_buffer[11] = (unsigned long) color;
	
    

    Window = (void *) system_call ( 118 , 
                          (unsigned long) &message_buffer[0], 
                          (unsigned long) &message_buffer[0], 
                          (unsigned long) &message_buffer[0] );
    

    if ( (void *) Window == NULL )
        return NULL;  


    // ??
    // #teste
    // Vamos enviar a mensagem MSG_CREATE para o procedimento de janela.
    // Assim ele pode terminar de pintar nessa mesma janela.


    return (void *) Window;    
}


/*
 *****************************
 * gde_register_window:
 *     Register Window. 
 */

int gde_register_window (struct window_d *window){
	
	if ( (void *) window == NULL )
	{
		return 1;
		
	}else{
		
        return (int) system_call ( SYSTEMCALL_REGISTERWINDOW, 
                        (unsigned long) window, 
                        (unsigned long) window, 
                        (unsigned long) window);
    }


    return 2;
}


/*
 *****************************
 * gde_close_window:
 *     Close Window. 
 */

int gde_close_window (struct window_d *window){
	
	if ( (void *) window == NULL )
	{
		return (int) 1;
	}else{
		
        return (int) system_call ( SYSTEMCALL_CLOSEWINDOW, 
                         (unsigned long) window, 
		                 (unsigned long) window, 
		                 (unsigned long) window );
	}


	return 2;
}


/*
 * gde_set_focus:
 *     Set Focus. 
 */

int gde_set_focus (struct window_d *window){


    if ( (void *) window == NULL )
    {
        return (int) 1;
    }else{

        return (int) system_call ( SYSTEMCALL_SETFOCUS, 
                        (unsigned long) window, 
                        (unsigned long) window, 
                        (unsigned long) window );
    };

    return 2;
}


/*
 * gde_get_focus:
 *     Get Focus. 
 */

int gde_get_focus (){
	
    return (int) system_call ( SYSTEMCALL_GETFOCUS, 0, 0, 0 );
}


/*
 ******************************
 * gde_kill_focus:
 *     Kill Focus. 
 */

int gde_kill_focus (struct window_d *window){


    if ( (void *) window == NULL )
    {
        return (int) 1;
    }else{

        return (int) system_call ( SYSTEMCALL_KILLFOCUS, 
                         (unsigned long) window, 
                         (unsigned long) window, 
                         (unsigned long) window );
    };

    return (int) 2;
}


/*
 * gde_set_active_window:
 *     Set Active Window.
 *     @todo: Esse retorno pode ser void??. 
 */

int gde_set_active_window (struct window_d *window){
	
	if ( (void *) window == NULL )
	{
		return (int) 1;
	}else{
	
        return (int) system_call ( SYSTEMCALL_SETACTIVEWINDOW, 
                         (unsigned long) window, 
                         (unsigned long) window, 
                         (unsigned long) window );	
	}

	//a flag 1 indica que deve-se redesenha e efetuar refresh.
	//APIredraw_window ( window, 1 );


    return 2;
}


/*
 * gde_get_active_window:
 *     Get Active Window Id.
 */

int gde_get_active_window (){

    return (int) system_call ( SYSTEMCALL_GETACTIVEWINDOW, 0, 0, 0 );
}


/*
 * gde_show_current_process_info:
 *     Mostra informações sobre o processo atual.
 */

void gde_show_current_process_info (){
	
	// @todo: Essa rotina devira apenas pegar os valores via system call
	//        e imprimir os valores obtidos usando rotinas em user mode.
	
	system_call ( SYSTEMCALL_CURRENTPROCESSINFO, 0, 0, 0 );
}

 
void 
gde_resize_window ( struct window_d *window, 
                   unsigned long x, 
                   unsigned long y )
{
	gramado_system_call ( SYSTEMCALL_RESIZEWINDOW, 
	    (unsigned long) window, 
	    x, 
	    y );
}


/*
 * gde_redraw_window:
 */

void 
gde_redraw_window ( struct window_d *window, 
                    unsigned long flags )
{
    gramado_system_call ( SYSTEMCALL_REDRAWWINDOW, 
        (unsigned long) window, 
        (unsigned long) flags, 
        (unsigned long) flags );
}


void 
gde_replace_window ( struct window_d *window, 
                    unsigned long x, 
                    unsigned long y )
{
	system_call ( SYSTEMCALL_REPLACEWINDOW, (unsigned long) window, x, y );
}


void gde_maximize_window (struct window_d *window){

    gramado_system_call ( SYSTEMCALL_MAXIMIZEWINDOW, 
        (unsigned long) window, 
        (unsigned long) window, 
        (unsigned long) window);
}


void gde_minimize_window (struct window_d *window){

    gramado_system_call ( SYSTEMCALL_MINIMIZEWINDOW, 
        (unsigned long) window, 
        (unsigned long) window, 
        (unsigned long) window);
}



// Envia uma mensagem PAINT para o aplicativo atualizar 
// a área de trabalho.

void gde_update_window (struct window_d *window){

    gramado_system_call ( 279, 
        (unsigned long) window, 
        (unsigned long) window, 
        (unsigned long) window );
}


void *gde_get_foregroung_window (){
	
	system_call ( SYSTEMCALL_GETFOREGROUNDWINDOW, 0, 0, 0 );	
}


void gde_set_foregroung_window (struct window_d *window){
	
	system_call ( SYSTEMCALL_SETFOREGROUNDWINDOW, (unsigned long) window, 
	    (unsigned long) window, (unsigned long) window );
}


/*
 ************************
 * gde_exit:
 *     Torna zombie a thread atual.
 *     Mas o propósito é terminar sair do 
 *     programa, terminando o processo e
 *     liberar os recursos que o processo estava usando.
 */

void gde_exit (int exit_code){

    system_call ( SYSTEMCALL_EXIT, 
        (unsigned long) exit_code, 
        (unsigned long) exit_code, 
        (unsigned long) exit_code );


    while (1){ asm ("pause"); };
}


/*
 *****************************************
 * kill:
 *
 */

void gde_kill (int exit_code)
{
	//#todo
}



/*
 ****************************
 * gde_dead_thread_collector:
 * 
 *     Aciona o coletor de threads zumbis.
 *     Procura em todos os slots por threads no estado ZOMBIE e 
 * fecha elas.
 *     @todo: Talvez essa rotina possa ser implemetada em user mode 
 * por um server, ficando à cargo do kernel apenas fazer a realocação 
 * dos recursos de destruição das estruturas. 
 */

// What a cool thing !

void gde_dead_thread_collector (){

    system_call ( SYSTEMCALL_DEAD_THREAD_COLLECTOR, 
        (unsigned long) 0, 
        (unsigned long) 0, 
        (unsigned long) 0 );
}


/*
 ****************************
 * gde_strncmp:
 *     Compara duas strings.
 *     @todo: Isso deve ser oferecido peloa libC e não pela api. 
 */

int gde_strncmp (char *s1, char *s2, int len){


    int n = len;


	//if ( len < 0 )
		//return 3;



	while (n > 0)
	{	
	    n--;
        
		if (*s1 != *s2)
		{
            return (int) 1;
        };

		*s1++;
		*s2++;
	};

	if ( *s1 != '\0' || *s2 != '\0' )
	{	
	    return (int) 2;
	}

	return 0;
}


/*
 *************************************
 * gde_show_backbuffer:
 *     Refresh Screen.
 *     Passa o conteúdo do backbuffer para o lfb. 
 */

void gde_show_backbuffer (){

    // #todo
    // trocar o nome dessa systemcall.
    // refresh screen será associado à refresh all windows.
    
    system_call ( SYSTEMCALL_REFRESHSCREEN, 0, 0, 0 );
}





/*
 *************************
 * gde_reboot:
 *     Reboot the O.S and then reboot the machine. 
 */

	// #todo: 
	// Fazer outras chamadas para
	// as camadas de software de mais alto nível antes
	// de efetuar o reboot de hardware propriamente dito. 

void gde_reboot (){

    gramado_system_call ( SYSTEMCALL_REBOOT, 0, 0, 0 );
    
    // ?? return ??
}


/*
 *******************************
 * gde_set_cursor: 
 *     Set cursor. 
 */

void gde_set_cursor ( unsigned long x, unsigned long y ){
	
    system_call ( SYSTEMCALL_SETCURSOR, x, y, 0 );
}

// #todo
void gde_get_cursor ( unsigned long *x, unsigned long *y )
{
}



/*
 * gde_get_cursor_x:
 *     Get cursor x. 
 */

unsigned long gde_get_cursor_x (){

    return (unsigned long) gramado_system_call ( SYSTEMCALL_GETCURSORX, 
                               0, 0, 0 );
}


/*
 * gde_get_cursor_y:
 *     Get cursor y.
 */

unsigned long gde_get_cursor_y (){

    return (unsigned long) gramado_system_call (SYSTEMCALL_GETCURSORY, 
                               0, 0, 0 );
}


/*
 * gde_get_client_area_rect:
 *     Get client area rect.
 */

void *gde_get_client_area_rect (){
	
    return (void *) system_call ( SYSTEMCALL_GETCLIENTAREARECT, 0, 0, 0 );
}


/*
 * gde_set_client_area_rect:
 *     Enviar uma estrutura de retângulo criada em user mode para o kernel.
 */

void gde_set_client_area_rect (struct rect_d *r){
	
    system_call ( SYSTEMCALL_SETCLIENTAREARECT, 
        (unsigned long) r, (unsigned long) r, (unsigned long) r );
}


/*
 ***********************************
 * gde_create_process:
 *     Create a process.
 */

void *gde_create_process ( unsigned long process_eip, 
                           unsigned long process_priority, 
                           char *name )
{
    return (void *) system_call ( SYSTEMCALL_CREATEPROCESS, 
                        process_eip, 
                        process_priority, 
                        (unsigned long) name );
}


/*
 **************************
 * gde_create_thread:
 *     Create a thread.
 *     #todo: 
 *     Precisamos uma função que envie mais argumentos.
 *     Essa será uma rotina de baixo nível para pthreads.
 */

void *gde_create_thread ( unsigned long init_eip, 
                          unsigned long init_stack, 
                          char *name )
{
    return (void *) system_call ( SYSTEMCALL_CREATETHREAD, 
                        init_eip, 
                        init_stack, 
                        (unsigned long) name );
}


/*
 ****************************************************************
 * gde_start_thread:
 *     Coloca no estado standby para executar pela primeira vez
 */

void gde_start_thread (void *thread){

    system_call ( SYSTEMCALL_STARTTHREAD, 
        (unsigned long) thread, 
        (unsigned long) thread, 
        (unsigned long) thread );
}


/*
 * apiFOpen:
 *     Carrega na memória um arquivo.
 *     Usa um serviço do kernel para carregar um arquivo na memória.
 *     Obs: Devemos passar um endereço válido, previamente alocado. 
 */

// #bugbug
// Cuidado estamos entrando na sessão crítica.
// O aplicativo pode desejar fazer o mesmo.

void *gde_fopen (const char *filename, const char *mode){
	
    void *Ret;	

    // #bugbug
    // Cuidado estamos entrando na sessão crítica.
    // O aplicativo pode desejar fazer o mesmo.

    //#todo: deletar
	gde_enter_critical_section();
	
	Ret = (void *) system_call ( SYSTEMCALL_READ_FILE, 
	                (unsigned long) filename, 
	                (unsigned long) mode, 
	                0 );

	//#todo: deletar
    gde_exit_critical_section (); 
    
    return (void *) Ret;
}


/*
 *************************************************************
 * gde_save_file:
 *     Salva um arquivo no diretório raiz do volume de boot.
 *     Obs: Talvez possamos ter mais argumentos.
 *     #bugbug: Essa rotina seria mais fácil se todas as informações sobre
 * o arquivo fossem gerenciadas pelo kernel. Mas não é o que estamos fazendo agora.
 */
 
//file_size = size in sectors

int
gde_save_file ( char *file_name, 
                unsigned long file_size,
                unsigned long size_in_bytes,
                char *file_address,
                char flag )
{
    int Ret;


	// Enviando tudo via argumento.
	// Esse método dá a possibilidade de enviarmos ainda 
	// mais argumentos. 
	// #importante: Isso está funcionado, Vamos fazer assim e 
	// não do jeito antigo.

	unsigned long message_buffer[12];
	
	message_buffer[0] = (unsigned long) file_name;
	message_buffer[1] = (unsigned long) file_size;
	message_buffer[2] = (unsigned long) size_in_bytes;
	message_buffer[3] = (unsigned long) file_address;
	message_buffer[4] = (unsigned long) flag;
	//message_buffer[5] = (unsigned long) x;
	//message_buffer[6] = (unsigned long) x;
	//message_buffer[7] = (unsigned long) x;
	//message_buffer[8] = (unsigned long) x;
	//message_buffer[9] = (unsigned long) x;
	//message_buffer[10] = (unsigned long) x;
	//message_buffer[11] = (unsigned long) x;
		
	

	gde_enter_critical_section ();
		
	Ret = (int) system_call ( SYSTEMCALL_WRITE_FILE,
                    (unsigned long) &message_buffer[0], 
                    (unsigned long) &message_buffer[0],  
                    (unsigned long) &message_buffer[0] ); 

    gde_exit_critical_section ();

    return (int) Ret;
}


// gde_down:
// Operação down em um semáforo indicado no argumento.

void gde_down (struct semaphore_d *s){
	
	int Status = 1;    //fail.
	
	if( (void*) s == NULL  )
	{
	    //Semáforo inválido, 
        //Bloquear a thread, não adianta fazer a system call.
        //@todo: Chamar rotina que bloqueia a thread.
		
		printf ("gde_down: *fail");
		
		while (1){ asm ("pause"); };
        
		//return;
	};

tryAgain:
    
	//0 = deu certo, entrada liberada na sessão crítica.
    //1 = algo deu errado espere tentando novamente.
	
	Status = (int) system_call ( SYSTEMCALL_SEMAPHORE_DOWN, 
                    (unsigned long) s, (unsigned long) s, (unsigned long) s );

	// Podemos entrar na região crítica.
	
	if (Status == 0)
	{
		return;
	}
	
	// Devemos esperar, monitorando a flag ou bloquando a thread.
	
	if (Status == 1)
	{
		// Opções:
		// + Bloqueamos a thread e quando ela acordar tentaremos novamente.
		// + Fazemos um loop monitorando a flag.
		
		//Opção 1.
		//Exemplo: apiBlockThread(...)
		
		//Opção 2.
		goto tryAgain;
	};
	
fail:

	goto tryAgain;
}


// gde_up:
// Operação up em um semáforo indicado no argumento.

void gde_up (struct semaphore_d *s){
	
	int Status = 1; 
	
	if ( (void *) s == NULL  )
	{
	    //Semáforo inválido, 
        //Bloquear a thread, não adianta fazer a system call.
        //@todo: Chamar rotina que bloqueia a thread.
		
		printf ("gde_up: *fail");
		
		while (1){ asm ("pause"); }
	};

    //0 = deu certo, podemos asir da sessão crítica.
    //1 = algo deu errado, não conseguimos sair da sessão crítica.	
	
tryAgain:


    Status = (int) system_call ( SYSTEMCALL_SEMAPHORE_UP, 
                       (unsigned long) s, 
                       (unsigned long) s, 
                       (unsigned long) s );


	//Ok , podemos sair sa sessão crítica.
	if (Status == 0)
	{
		return;
	}


	//Deu errado a nossa tentativa d sair da sessão crítica.
	if (Status == 1)
	{
		//
		// Opções:
		// + Bloqueamos a thread e quando ela acordar tentaremos novamente.
		// + Fazemos um loop monitorando a flag.
		//
		
		//Opção 1.
		//Exemplo: apiBlockThread(...)
		
		//Opção 2.
		goto tryAgain;
	};	

fail:
	goto tryAgain;
}


//P (Proberen) testar.
void gde_enter_critical_section (){
	
	int S;
	
	while (1)
	{
	    S = (int) system_call ( SYSTEMCALL_GET_KERNELSEMAPHORE, 0, 0, 0 );
	    
		//Se deixou de ser 0 então posso entrar.
		//se ainda for 0, continuo no while.

		if ( S == 1 )
		{
		    goto done;	
		};
		//Nothing.
	};
	
    //Nothing
	
done:
    //Muda para zero para que ninguém entre.
    system_call ( SYSTEMCALL_CLOSE_KERNELSEMAPHORE, 0, 0, 0 );
	return;
}


//V (Verhogen)incrementar.
void gde_exit_critical_section (){
	
	//Hora de sair. Mudo para 1 para que outro possa entrar.
    system_call ( SYSTEMCALL_OPEN_KERNELSEMAPHORE, 0, 0, 0 );
}




// P (Proberen) testar.
void gde_p ()
{
    //gde_enter_critical_section ();         
}

// V (Verhogen) incrementar.
void gde_v ()
{
    //gde_exit_critical_section ();          
}


//
//Inicializa em 1 o semáforo do kernel para que 
//o primeiro possa usar.

void gde_initialize_critical_section (){
	
	system_call ( SYSTEMCALL_OPEN_KERNELSEMAPHORE, 0, 0, 0 );
}


void gde_begin_paint (){
	
	gde_enter_critical_section ();
}


void gde_end_paint (){
	
	gde_exit_critical_section ();
}


/*
 *******************************************
 * gde_put_char:
 *     Imprime um caractere usando o cursor do sistema.
 *     #todo; falar mais sobre isso.
 */

void gde_put_char (int c){
	
	system_call ( SYSTEMCALL_SYS_PUTCHAR, c, c, c );
}



/*
 * gde_def_dialog:
 *     Defered dialog.
 *     Procedimento de janela adiado. 
 *     Usado pelos aplicativos ao fim dos seus procedimentos de janela.
 */

unsigned long 
gde_def_dialog ( struct window_d *window, 
                 int msg, 
                 unsigned long long1, 
                 unsigned long long2 )
{
    return (unsigned long) 1;
}


/*
 **********************************
 * gde_get_system_metrics:
 *     Obtem informações sobre dimensões e posicionamentos. 
 *     #importante
 */
 
unsigned long gde_get_system_metrics (int index){

    return (unsigned long) system_call ( SYSTEMCALL_GETSYSTEMMETRICS, 
                               (unsigned long) index, 
                               (unsigned long) index, 
                               (unsigned long) index );
}




/*
 // ??
//enviamos um ponteiro par ao kernel carregar elementos na estrutura.
void api_receive_message( struct api_receive_message_d *m );
void api_receive_message( struct api_receive_message_d *m )
{
	//Enviamos para o kernel o ponteiro da estrutura que desejamos que ele 
	//coloque os valores.
	//o kernel deve atualizar a flag dizendo que tem alguma mensagems na estrutura.
    system_call( SYSTEMCALL_138, (unsigned long) m, (unsigned long) m, (unsigned long) m );
				 
};
*/



/*
 *******************************
 * gde_dialog:
 *     Diálogo de yes ou no. 
 */

int gde_dialog ( const char *string ){

    int Status = 1; // Yes!
    int ch;

    printf (string);

    while (1)
    {
	    ch = (int) gde_getchar ();
		
		if ( ch != -1 )
	    {
	        switch (ch)
            {
				case VK_RETURN:
				    return (int) Status;
                    break;
					
			    case 'Y':
				case 'y':
				    printf("Yes\n");
			        Status = 1;
				    break;
				   
			    case 'N':
				case 'n':
			        printf("No\n");
					Status = 0;
				    break;
					
				//default:
				//    break;
		    };		   
		};
		
		// ?? rever isso.
		asm ("pause");
	};
	
    return (int) Status;
}


// #todo
// Descrever o método usdo por essa rotina.
int gde_getchar (){

    return (int) gramado_system_call ( 137, 0, 0, 0 );
}


/*
 ************************************
 * gde_display_bmp:
 *     Mostra na tela uma imagem bmp carregada na memória.
 * 
 * IN:
 *     address = endereço base
 *     x       = posicionamento 
 *     y       = posicionamento
 *     @todo: deletar w h 
 *
 *	// @todo: Criar defines para esses deslocamentos.
 */

static int nibble_count_16colors = 0;
 
int 
gde_display_bmp ( char *address, 
                  unsigned long x, 
                  unsigned long y )
{
    int i, j, base, offset;

    unsigned long left, top, bottom;

    unsigned long color, color2;
    unsigned long pal_address;

    unsigned long Width;
    unsigned long Height;

    unsigned long xLimit, yLimit;

    struct bmp_header_d *bh;
    struct bmp_infoheader_d *bi;

	// Endereço base do BMP que foi carregado na memória
    unsigned char *bmp = (unsigned char *) address;
	
	// Variável para salvar rgba.
    unsigned char *c = (unsigned char *) &color;
    unsigned char *c2 = (unsigned char *) &color2;	

    unsigned long *palette = (unsigned long *) (address + 0x36);		
    unsigned char *palette_index = (unsigned char *) &pal_address;	


	// Limits
	
	xLimit = 800;
	yLimit = 600;
	
	
	//@todo: Refazer isso
	if ( x > xLimit || y > yLimit )
	{ 
        return (int) 1; 
	}
	

	// @todo:
	// Testar validade do endereço.
	
	
	if ( address == 0 )
	{
		//goto fail;
	};
	
	
	//
	// struct for Info header
	//
	
	bh = (struct bmp_header_d *) malloc ( sizeof(struct bmp_header_d) );
	
    if ( (void *) bh == NULL )
	{
		//goto fail;
	}	
	

	//
	// Signature.
	//
	
    unsigned short sig;
	
	sig = *( unsigned short * ) &bmp[0];
	
	bh->bmpType = sig;
	
	
	//
	// Size. ( 2 bytes )
	//
	
	unsigned short Size = *( unsigned short* ) &bmp[2];
	
	bh->bmpSize = Size;
	
	
	//
	// struct for Info header
	//
	
	//Windows bmp.
	bi = (struct bmp_infoheader_d *) malloc ( sizeof(struct bmp_infoheader_d) );
	
    if ( (void *) bi == NULL )
    {
		//goto fail;
    }	

	//The size of this header.
	bi->bmpSize = *( unsigned long * ) &bmp[14];
	
	// Width and height.
    Width = *( unsigned long * ) &bmp[18];
    Height = *( unsigned long * ) &bmp[22];	
	
	//@todo: checar validade da altura e da largura encontrada.
	
	// Salvar.
	bi->bmpWidth = (unsigned long) Width;
	bi->bmpHeight = (unsigned long) Height;
	
	
	/* Number of bits per pixel */
	//1, 4, 8, 16, 24 and 32.
	bi->bmpBitCount = *( unsigned short * ) &bmp[28];
	
	// Único suportado ainda.
	if ( bi->bmpBitCount != 24 )
	{
		//fail
	}
	
	
	// 0 = Nenhuma compressão.
	if ( bi->bmpCompression != 0 )
	{
		//fail
	}
	
	
	//
	// Draw !
	//
	
	left = x;    
	top = y; 
	
	
	//bottom = top + height;
	bottom = (top + bi->bmpHeight );

	// Início da área de dados do BMP.
	
	//#importante:
	//a base é diferente para os tipos.
	 

	switch (bi->bmpBitCount){
		
		//case 1:
		//    base = (0x36 + 0x40);
		//    break;
		    
		//case 2:
		//    base = (0x36 + 0x40);
		//    break;
			
		case 4:
		    //4bytes pra cada cor, 16 cores, 64bytes.
		    base = (0x36 + 0x40);
		    break; 
			
		case 8:
		    //4bytes pra cada cor, 256 cores, 1024bytes.
		    base = (0x36 + 0x400);
		    break; 
			
		default:
		    base = 0x36;
			break;
	};	
	

//1 - 1 bpp (Mono)
//4 - 4 bpp (Indexed)
//8 - 8 bpp (Indexed) bbgggrrr
//16565 - 16 bpp (5:6:5, RGB Hi color)
//16    - 16 bpp (5:5:5:1, RGB Hi color)
//160   - 16 bpp (5:5:5:1, RGBA Hi color)
//24 - 24 bpp (True color)
//32 - 32 bpp (True color, RGB)
//320 - 32 bpp (True color, RGBA)	


  
	// ## ABGR8888 ##
	// Little-endian
	// pegando os caracteres
	//
	// 0 = A (MSByte)(left byte) 
	// 1 = B 
	// 2 = G 
	// 3 = R
	//
	// Output long = 0xRRGGBBAA
    //	
	// Exemplo: gramado GUI
    //#define COLOR_RED   0xFF000000 
    //#define COLOR_GREEN 0x00FF0000
    //#define COLOR_BLUE  0x0000FF00

	
	for ( i=0; i < bi->bmpHeight; i++ )	
	{		
		for ( j=0; j < bi->bmpWidth; j++ )	
		{	
	        // 16 cores
            // Um pixel por nibble.
	        if ( bi->bmpBitCount == 4 )
	        {				
				offset = base;
							    
				palette_index[0] = bmp[offset];
												
                //segundo nibble.
				if( nibble_count_16colors == 2222 )
				{
					palette_index[0] = ( palette_index[0] & 0x0F);  
					color = (unsigned long) palette[  palette_index[0]  ];
					
					nibble_count_16colors = 0;
					base = base + 1;
					
				//primeiro nibble.	
				}else{

			        palette_index[0] =  ( (  palette_index[0] >> 4 ) & 0x0F);
					color = (unsigned long) palette[  palette_index[0] ];
				    
					nibble_count_16colors = 2222;
					base = base;
				};
	        };	

			// 256 cores
			// Próximo pixel para 8bpp
	        if( bi->bmpBitCount == 8 )
	        {   
				offset = base;
				color = (unsigned long) palette[  bmp[offset] ];
				
				base = base + 1;     
	        };			
			
			// 16bpp high color BMP
			// Próximo pixel para 16bpp
			// apenas 565 por enquanto.  
	        if ( bi->bmpBitCount == 16 )
	        {

			    //565
                //if(565 )
                //{
				    offset = base;					
					
				    //A
			        c[0] = 0;	

			        //b				
			        c[1] = bmp[offset];
			        c[1] = (c[1] & 0xF8);  // '1111 1000' 0000 0000  
				
				    //g
			        c2[0] = bmp[offset];
			        c2[0] = c2[0] &  0x07;    // '0000 0111' 0000 0000 
			        c2[1] = bmp[offset+1];
			        c2[1] = c2[1] &  0xE0;    //  0000 0000 '1110 0000' 
					c[2] = ( c2[0] | c2[1]  );
					
			        //r
			        c[3] = bmp[offset+1];
			        c[3] = c[3] & 0x1F;     // 0000 0000 '0001 1111' 
		        
				    base = base + 2;    
				//};
	        };
			

			// Próximo pixel para 24bpp
	        if ( bi->bmpBitCount == 24 )
	        {
				offset = base;
			    
			    c[0] = 0; //A
				
				c[1] = bmp[offset];
			    
			    offset = base+1;
			    c[2] = bmp[offset];
			
			    offset = base+2;
			    c[3] = bmp[offset];

		        base = base + 3;    
	        };
			
			
			// Próximo pixel para 32bpp
	        if ( bi->bmpBitCount == 32 )
	        { 
				//A
				//offset = base+3;
			    c[0] = 0;
				
				offset = base;
			    c[1] = bmp[offset];
			
			    offset = base+1;
			    c[2] = bmp[offset];
			
			    offset = base+2;
			    c[3] = bmp[offset];
				
		        base = base + 4;    
	        };
			
			
			system_call ( SYSTEMCALL_BUFFER_PUTPIXEL, (unsigned long) color, 
				(unsigned long) left, (unsigned long) bottom );
			
			// Próximo pixel.
			left++; 
		};
		
		// Vamos para a linha anterior.
		bottom = bottom-1;
		
		// Reiniciamos o x.
		left = x;    
	};	
	
	// ## test palette 
	//int p;
	
	//if(bi->bmpBitCount == 8 )
	//{
	//    printf("\n");
	//    for( p=0; p<16; ++p )
	//    {
	//	   printf("%x\n",palette[p]);
	//    }
	//    printf("\n");
	//};
	
fail:	
    //printf("fail");	
	
done:
	
	//Debug
	//printf("w={%d} h={%d}\n", bi->bmpWidth, bi->bmpHeight );
	
	return 0;
}



/*
 **********************************************************
 * gde_send_message_to_process:
 *     Envia uma mensagem para a thread de controle de um dado processo.
 *     Dado o PID.
 */

// #obs
// Dá pra criar uma função semelhante, que use estrutura ao invés 
// de buffer.

int
gde_send_message_to_process ( int pid, 
                              struct window_d *window, 
                              int message,
                              unsigned long long1,
                              unsigned long long2 )
{

    unsigned long message_buffer[5];


    if ( pid<0 )
        return -1;


    message_buffer[0] = (unsigned long) window;
    message_buffer[1] = (unsigned long) message;
    message_buffer[2] = (unsigned long) long1;
    message_buffer[3] = (unsigned long) long2;
    //...

    return (int) system_call ( 112 , 
                    (unsigned long) &message_buffer[0], 
                    (unsigned long) pid, 
                    (unsigned long) pid );
}


/*
 *************************************
 * gde_send_message_to_thread:
 *     Envia uma mensagem para uma thread.
 */

int 
gde_send_message_to_thread ( int tid, 
                             struct window_d *window, 
                             int message,
                             unsigned long long1,
                             unsigned long long2 )
{

    unsigned long message_buffer[5];

    if (tid < 0)
        return -1;


    message_buffer[0] = (unsigned long) window;
    message_buffer[1] = (unsigned long) message;
    message_buffer[2] = (unsigned long) long1;
    message_buffer[3] = (unsigned long) long2;
    //...


    return (int) system_call ( 117 , 
                     (unsigned long) &message_buffer[0], 
                     (unsigned long) tid, 
                     (unsigned long) tid );
}



/*
 **********
 * gde_send_message:
 *     Envia uma mensagem para a thread atual.
 *     Isso funcionou.
 */

unsigned long 
gde_send_message ( struct window_d *window, 
                 int message,
                 unsigned long long1,
                 unsigned long long2 )
{
	unsigned long message_buffer[5];

	message_buffer[0] = (unsigned long) window;
	message_buffer[1] = (unsigned long) message;
	message_buffer[2] = (unsigned long) long1;
	message_buffer[3] = (unsigned long) long2; 

	return (unsigned long) system_call ( 114 , 
	                           (unsigned long) &message_buffer[0], 
	                           (unsigned long) &message_buffer[0], 
	                           (unsigned long) &message_buffer[0] );
}


int 
gde_draw_text ( struct window_d *window, 
                unsigned long x, 
                unsigned long y, 
                unsigned long color, 
                char *string )
{
	unsigned long msg[8];
	
	msg[0] = (unsigned long) window;
	msg[1] = (unsigned long) x;
	msg[2] = (unsigned long) y;
	msg[3] = (unsigned long) color;
	msg[4] = (unsigned long) string;
	//...
	
	return (int) system_call ( SYSTEMCALL_DRAWTEXT, 
	                (unsigned long) &msg[0], 
	                (unsigned long) &msg[0], 
	                (unsigned long) &msg[0] );
}


// ?
struct window_d *gde_get_ws_screen_window (){
	
    return (struct window_d *) system_call ( 146 , 0, 0, 0 );
}

// ?
struct window_d *gde_get_ws_main_window (){
	
    return (struct window_d *) system_call ( 147 , 0, 0, 0 );
}



/*
 ****************************** 
 * gde_create_timer:
 *     
 *     Create timer.
 */
 
struct timer_d *gde_create_timer ( struct window_d *window, 
                                   unsigned long ms, 
                                   int type )
{
    return (struct timer_d *) gramado_system_call ( 222, 
                                  (unsigned long) window, 
                                  (unsigned long) ms, 
                                  (unsigned long) type );
}



// pega informações varidas sobre o sys time.
unsigned long gde_get_systime_info ( int n ){

    return (unsigned long) gramado_system_call ( 223, 
                               (unsigned long) n, 
                               (unsigned long) n, 
                               (unsigned long) n );
}


/*
 * gde_show_window 
 * 
 * 
 */
 
//mostra uma janela na tela. backbuffer ---> frontbuffer

void gde_show_window (struct window_d *window){

    gramado_system_call ( 24, 
        (unsigned long) window, 
        (unsigned long) window, 
        (unsigned long) window );
}



/* 
 * gde_start_terminal:
 *     
 */

// 'Clona' e executa o noraterm como processo filho. 
// registra o terminal noraterm como terminal atual.
// pega o pid do terminal atual
// manda uma mensagem pedindo para o terminal dizer hello.

// #obs: Isso funcionou.
 
int gde_start_terminal (void){

	int PID;
	
	// 'Clona' e executa o noraterm como processo filho. 
	PID = (int) system_call ( 900, (unsigned long) "noraterm.bin", 0, 0 );
	//PID = (int) system_call ( 901, (unsigned long) "noraterm.bin", 0, 0 );
		
	// Exibe o PID para debug.
	//printf ("PID = %d \n", PID);

    //registra o terminal como terminal atual.
	system_call ( 1003, PID, 0, 0 ); 
		
	//invalida a variável.
	PID = -1;
		
	//pega o pid do terminal atual
	PID = (int) system_call ( 1004, 0, 0, 0 ); 
		
	if ( PID <= 0 )
	{
		printf ("PID fail. We can't send the message\n");
	    return -1;
	}
		
	//manda uma mensagem pedindo para o terminal dizer hello.
	//__SendMessageToProcess ( PID, NULL, MSG_TERMINALCOMMAND, 2001, 2001 );

	return (int) PID;
}


int 
gde_update_statusbar ( struct window_d *window, 
                       unsigned char *string1, 
                       unsigned char *string2 )
{
    return (int) system_call ( 300, 
                     (unsigned long) window, 
                     (unsigned long) string1, 
                     (unsigned long) string2 );
}


// Usada para obter o pid de alguns drivers e servidores
// do sistema. Aqueles servidores que só podem ter um do tipo 
// rodando ao mesmo tempo.
int gde_get_pid (int index){

    if ( index < 0 )
        return (int) -1;

    // #todo
    // Lá no kernel tem que checar os limites.

    return gramado_system_call ( 9999, index, index, index );
}


struct window_d *gde_get_screen_window (void)
{
	return (struct window_d *) gramado_system_call ( 955, 0, 0, 0 );
}


struct window_d *gde_get_background_window (void)
{
	return (struct window_d *) gramado_system_call ( 956, 0, 0, 0 );
}


struct window_d *gde_get_main_window (void)
{
	return (struct window_d *) gramado_system_call ( 957, 0, 0, 0 );
}



//pega o nome do processo dado o pid.
int gde_getprocessname (int pid, char *name, size_t len)
{
	int __len_ret;
	
	if ( len < 0 || len > 64 ) //HOST_NAME_MAX
	{
	    printf ("gde_getprocessname: len\n");
	    return -1;
	}



    //coloca no buffer interno
    __len_ret = (int) gramado_system_call ( 882, 
                        (unsigned long) pid,
                        (unsigned long) name,
                        (unsigned long) name );

	if ( __len_ret < 0 || __len_ret > 64 ) //HOST_NAME_MAX
	{
	    printf ("gde_getprocessname: __len_ret\n");
	    return -1;
	}

	if ( __len_ret > len )
	{
		__len_ret = len;
	}


    return 0;
}


//pega o nome do thread dado o tid.
int gde_getthreadname (int tid, char *name, size_t len)
{
	int __len_ret;
	
	if ( len < 0 || len > 64 ) //HOST_NAME_MAX
	{
	    printf ("gde_getthreadname: len\n");
	    return -1;
	}



    //coloca no buffer interno
    __len_ret = (int) gramado_system_call ( 883, 
                        (unsigned long) tid,
                        (unsigned long) name,
                        (unsigned long) name );

	if ( __len_ret < 0 || __len_ret > 64 ) //HOST_NAME_MAX
	{
	    printf ("gde_getthreadname: __len_ret\n");
	    return -1;
	}

	if ( __len_ret > len )
	{
		__len_ret = len;
	}


    return 0;
}



unsigned long gde_get_process_stats (int pid, int index){

    return (unsigned long) system_call ( 880, 
                               (unsigned long) pid, 
                               (unsigned long) index, 
                               (unsigned long) index );
}

unsigned long gde_get_thread_stats (int tid, int index){

    return (unsigned long) system_call ( 881, 
                               (unsigned long) tid, 
                               (unsigned long) index, 
                               (unsigned long) index );
}



// Envia uma string para a porta serial COM1
void gde_debug_print (char *string)
{
    gramado_system_call ( 289, 
        (unsigned long) string,
        (unsigned long) string,
        (unsigned long) string );
}


//#test: Isso ainda não foi testado.
//#todo testar.
int gde_clone_and_execute ( char *name )
{
    return (int) gramado_system_call ( 900, (unsigned long) name, 0, 0 );
}



// Enviamos para o kernel um buffer que está no app.
// Associando esse buffer ao pid do app.
// Desse modo o driver de rede pode colocar conteúdo no buffer
// do aplicativo que está ouvindo determinada porta.
// Depois mandar uma mensagem para o app dizendo, pode ler seu buffer.
//

// O kernel poderá guardar esse ponteiro para o net_buffer
// na estrutura do processo cujo pid foi passado por essa função.
// Também pode salvar o tamanho do buffer pou ele pode ter
// tamanho padrçao. 4KB.

// IN: pid, ponteiro para o buffer, comprimento do buffer;
// OUT: 0=Ok -1=fail
int gde_setup_net_buffer (int pid, char *buffer, size_t len){

    int len_ret;
    
    len_ret = (int) gramado_system_call ( 550, 
                        (unsigned long) pid,
                        (unsigned long) buffer,
                        (unsigned long) buffer );

     return len_ret;
}



// cria um novo process, uma thread e carrega a imagem.
int 
execute_new_process ( const char *filename, 
                      char *argv[], 
                      char *envp[] )
{
    return (int) gramado_system_call ( 168, 
                     (unsigned long) filename,    // Nome
                     (unsigned long) argv,        // arg (endereço da linha de comando)
                     (unsigned long) envp );      // env
}




//
// End.
//

