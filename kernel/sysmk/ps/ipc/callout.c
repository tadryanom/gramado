/*
 * File: callout.c
 *     O Kernel chama uma rotina em user mode.
 */


/*
    O callout chamar� uma rotina em user mode atrav�s de um iret.
    Acessamos s rotinas de um servidor que est� em user mode atrav�s
	de um iret e quando ele terminar ele aciona uma interrup��o, que � um
	callback.
*/
 
 
#include <kernel.h>


/*
void KiCalloutTest(void)
{
	//
	// @todo: reproduzier o spawn para chamar uma rotina de um servidor em user mode.
	// depois ele responde com uma interrup��o do tipo call back.
	//
	
	//iret
	return;
};
*/

//
// End.
//

