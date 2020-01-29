
#include <sys/types.h>  
#include <utmp.h>


/*
       The login_tty() function prepares for a login on the terminal fd
       (which may be a real terminal device, or the slave of a
       pseudoterminal as returned by openpty()) by creating a new session,
       making fd the controlling terminal for the calling process, setting
       fd to be the standard input, output, and error streams of the current
       process, and closing fd. 
 */
 
int login_tty(int fd)
{
    return -1;
}





