/*
 * File: x86cont.h 
 *
 * Context support for x86 arch. 
 *
 * Descri��o:
 *     Header para rotinas de opera��es com contexto de threads e processos.
 *
 * * IMPORTANTE:
 * Obs:
 * @todo:
 * Existe um tipo de page fault causada por recarregar os registradores 
 * incorretamente. Acho que rotinas de checagem de conte�do dos registradores 
 * antes de retornar devem ser mais severas. Os seja, depois de restaurar, 
 * tem que checar. Se n�o ouver falha, executa iretd. Mas se ouver falha, 
 * devemos bloquear a thread, checar novamente, tentar concertar se for algo 
 * simples e fechar a thread se o erro for grave ou persistir. Logo em seguida 
 * escalonar uma outra thread.
 *
 * oct 2016 - Created.
 */


void save_current_context (void);

void restore_current_context (void);

int contextCheckThreadRing0Context (int tid);

int contextCheckThreadRing3Context (int tid); 


//
// End.
//

