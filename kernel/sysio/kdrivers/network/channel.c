/*
 * File: channel.c 
 *
 * Descrição: 
 *     Channel manager. 
 *     Gerenciamento de canais Client/Server para 
 *     troca de mensagens entre processos.
 *
 * Obs: Um canal de comunicação é composto por dois sockets.
 *
 * Isso ficara aqui porque estamos lidando com comunicação cliente servidor.
 * através da rede, essa é a ideia.
 *
 * Versão 1.0, 2015, 2016.
 */


#include <kernel.h>




/*
 ****************************************************
 * CreateChannel:
 *     Cria um canal.
 */

void *CreateChannel ( struct process_d *OwnerProcess,
                      struct thread_d *SenderThread,
                      struct thread_d *ReceiverThread )
{
    struct channel_d *NewChannel;

    NewChannel = (void *) kmalloc ( sizeof(struct channel_d) );

    if( (void *) NewChannel == NULL )
    {
        return NULL;
    }else{

        NewChannel->OwnerProcess = (void *) OwnerProcess;

        NewChannel->SenderThread = (void *) SenderThread;
        NewChannel->ReceiverThread = (void *) ReceiverThread;

        NewChannel->State = 0;
    };


    return (void *) NewChannel;
}


/*
 *******************************************
 * DestroyChannel:
 *     Destroy um canal.
 *     +libera a memória.
 *     +destrói a estrutura.
 *     ...
 */

int DestroyChannel (struct channel_d *channel){

    if ( (void *) channel == NULL )
    {
        return 0;   
    }else{

        //...

    };


	//#todo? ...


    return 0;
}



/*
 * OpenChannel:
 *     Abrir o canal para uso.
 */

int 
OpenChannel ( struct channel_d *channel, 
              struct process_d *OwnerProcess,
              struct thread_d *SenderThread,
              struct thread_d *ReceiverThread )
{
    if ( (void *) channel == NULL )
    {
        return (int) 1;    //Fail.

    }else{

        channel->State = 1;
    };


    return 0;
}



/*
 * CloseChannel:
 *     Libera o canal.
 *     Mas não destroi.
 */

int CloseChannel (struct channel_d *channel){

    if ( (void *) channel == NULL )
    {
        return (int) 1;

    }else{
        channel->State = 0;
    };


    return 0;
}


//
// End.
//

