/*
 * File: builtins.c
 *     Buittin commands for gdeterm.
 *
 * History:
 *     2019 - Created by Fred Nora.
 */

 
#include "gdeterm.h"


void cd_buitins (){
	
	printf ("cd_buitins:\n");
}


//cls
void cls_builtins (){
	
    shellClearScreen ();
}


void copy_builtins()
{
}


void date_builtins()
{
}


void del_builtins()
{
}


void dir_builtins (){
	
	//
	// @todo: get set disk id e directory id.
	//

    printf ("~dir \n");		
        
	//#test 
	//173 Lista arquivos de um diretório, dado o número do disco,
    //o número do volume e o número do diretório,	
	
    system_call (173,0,0,0);		
		
	printf ("done\n");	
}


//echo command.
//imprimir a lista de comandos de uma linha de comandos.

void echo_builtins (char *list[])
{	
    shell_print_tokenList ( list, " ");
	//shell_print_tokenList( list, "/");
}


void exec_builtins (){
	
    printf ("todo: ~exec\n");	
}


void exit_builtins (){
	
	printf ("Exiting gdeterm ...\n");
	exit (0);	
}


int getpid_builtins (){
	
    shellShowPID ();		
	return 0; 
}


int getppid_builtins (){
	
	shellShowPPID ();	
	return 0;
}


int getuid_builtins (){
	
	shellShowUID ();
	
	return 0;
}


int getgid_builtins (){
	
	shellShowGID ();
	return 0;
}


void help_builtins (int arg){	
	
	printf ("\n gdeterm: \n\n");
	
	switch (arg)
	{
		//all
		case 1:
		    printf ("Show all help topics\n");
			shellHelp ();
		    break;
		//min
		case 2:
		    printf ("cls, help, exit..\n");
			break;
		
		default:
			shellHelp ();
			break;
	};
}


void pwd_builtins (){
	
    //printf("~pwd - print working directory\n");
    system_call ( 170, 0, 0, 0 );		
}


//
// End.
//


