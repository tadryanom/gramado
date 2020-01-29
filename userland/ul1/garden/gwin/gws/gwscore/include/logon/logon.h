/*
 * File: gramado\logon\logon.h
 *
 * Descri��o:
 *     Kernel Mode Logon Support Routines.
 *     Header de suporte � cria��o do Logon.
 *  
 * OBS:
 *     O Logon est� na WindowStation0 e no Desktop0. 
 *     Existir� um processo em user mode para gerenciamento de Logon e esse 
 * processo poder� chamar rotinas de logon presentes nesse m�dulo.
 *
 * History:
 *     2015 - Created by Fred Nora.
 *     ...
 */
 

//
// Vari�veis globais.
// 
 
//flag, usar ou na� o default user. 
int g_use_default_user; 
 
 

//
// Prot�tipos.
// 
 
//Cria.
void create_logon(); 

//Inicializa.
int init_logon(int argc, char *argv[]); 	


/*
 * SetLogonParameters:
 *     Configura alguns par�metros para a tela de logon.
 *
 */
void SetLogonParameters( int refresh,         // Flag. Colocar ou n�o o backbuffer na tela.
int screen,          // Tela do computador.
int background,      // Background da area de trabalho. 
int logo,            // Logo, Janela para imagem da area de trabalho.
int taskbar,         // Barra de tarefas.(icones para programas)
int main,            // Janela principal.( Desktop, ou Kernel Setup Utility ) 
int menu,            // Control menu da �rea de trabalho.
int infobox,         // Janelinha no canto, para alerta sobre sobre eventos ocorridos no sistema.
int messagebox,      // O message box do kernel.(cada app cria seu messagebox.) 
int debug,           // Janela do debug do kernel.(sonda por problemas)
int navigationbar,   // Janela de navega��o(semelhante a navigation bar dos dispositivos mobile) 
int grid             // Grid da janela principal.
//limites ...
);



/*
 * LogonProcedure:
 *     Procedimento de janela do LOGON.
 */ 
unsigned long LogonProcedure( struct window_d *window, 
                              int msg, 
							  unsigned long long1, 
							  unsigned long long2 );
							  
							  
//
//fim.
//

