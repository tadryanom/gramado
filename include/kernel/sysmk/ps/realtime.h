/*
 * Arquivo: realtime.h
 *
 * Descri��o:
 *     Header do m�dolo realtime do kernel.
 *     Rotinas em realtime para multiprocessadores, ou quando est� rodando 
 * apenas uma tarefa, a idle.
 *     As tarefas s�o confinadas em um processador, 
 * destinado para tarefas em real time.
 *
 * @todo: Fazer uma estrutura para gerenciar opera��es em realtime.
 */ 


/*
 * Flag para tornar realtime uma op��o.
 */
int realtimeStatus; 

//lista de processadores dedicados a tarefa de realtime.
unsigned long realtimeprocessorList[32];

//
//fim.
//
