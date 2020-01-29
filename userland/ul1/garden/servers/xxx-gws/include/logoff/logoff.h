/*
 * File: gramado\logoff\logoff.h
 *
 * Descri��o:
 *     Kernel Mode Logoff Support Routines.
 *     Header de suporte ao logoff.
 *
 * OBS:
 *     O Logoff deve seu pr�prio desktop. 
 *     As rotinas de Logoff podem estar em um processo em User Mode. Ou em um 
 * m�dulo externo em kernel mode e poder�o chamar as rotinas presentes nesse
 * m�dulo. 
 *
 * History:
 *     2015 - Created by Fred Nora.
 *     2016 - revision.
 *     ...
 */
 
 
int init_logoff(int argc, char *argv[]);
int StartLogoff(int argc, char *argv[]);


//
// End.
//

