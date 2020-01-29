/*
 * File: unistd.h
 * 2018
 */

/*
 In the C and C++ programming languages, unistd.h is the name 
 of the header file that provides access to the POSIX 
 operating system API. It is defined by the POSIX.1 standard, 
 the base of the Single Unix Specification, and should therefore 
 be available in any conforming (or quasi-conforming) 
 operating system/compiler (all official versions of UNIX, 
 including macOS, Linux, etc.).
 
 On Unix-like systems, the interface defined by unistd.h is 
 typically made up largely of system call wrapper functions 
 such as fork, pipe and I/O primitives (read, write, close, etc.).
 
 Credits: Wikipedia
 */
 
 
/* FreeBSD stuff */
#define	STDIN_FILENO	0	/* standard input file descriptor */
#define	STDOUT_FILENO	1	/* standard output file descriptor */
#define	STDERR_FILENO	2	/* standard error file descriptor */



/* 
 Process IDentifier 
 posix.  
 */
typedef int pid_t;




// Escreve no arquivo uma certa quantidade de caracteres de uma dada string 
int stdio_file_write ( FILE *stream, char *string, int len );


//posix
int close ( int fd );

int sys_dup ( int oldfd );
int sys_dup2 (int oldfd, int newfd);
int sys_dup3 (int oldfd, int newfd, int flags);

//POSIX.1-2001, POSIX.1-2008.
int sys_pipe ( int *pipefd );

//#todo: rotina provisória
//o serviço de ler num pipe deve ser o mesmo de ler num stream dado o fd.
int sys_read_pipe ( int fd, int count );

//#todo: rotina provisória
//o serviço de escrever num pipe deve ser o mesmo de escrever num stream dado o fd.
int sys_write_pipe ( int fd, int count );

//#todo
//https://www.mkssoftware.com/docs/man3/pathconf.3.asp
long fpathconf (int fildes, int name);
long pathconf (const char *pathname, int name);


//See: network.c
int __sethostname (const char *new_hostname);
int __gethostname (char * buffer);






