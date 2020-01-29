// tty driver header.



//#define TTY_DRIVER_MAGIC		0x5402
#define TTY_DRIVER_MAGIC		1234


//credits: linux
/* tty driver types */
#define TTY_DRIVER_TYPE_SYSTEM		0x0001
#define TTY_DRIVER_TYPE_CONSOLE		0x0002
#define TTY_DRIVER_TYPE_SERIAL		0x0003
#define TTY_DRIVER_TYPE_PTY		0x0004
#define TTY_DRIVER_TYPE_SCC		0x0005	/* scc driver */
#define TTY_DRIVER_TYPE_SYSCONS		0x0006


//credits: linux
//tty driver flags
#define TTY_DRIVER_INSTALLED		0x0001
#define TTY_DRIVER_RESET_TERMIOS	0x0002
#define TTY_DRIVER_REAL_RAW		0x0004
#define TTY_DRIVER_DYNAMIC_DEV		0x0008
#define TTY_DRIVER_DEVPTS_MEM		0x0010
#define TTY_DRIVER_HARDWARE_BREAK	0x0020
#define TTY_DRIVER_HAS_GET_ICOUNT	0x0040




struct ttydrv_d {

    int index;
    
    int used;
    int magic;
    
    char name[64];

    // COntagem simplificada;
    int major;
    int minor;

    // linux-like
    short type;       // type of tty driver. 
    short subtype;    // subtype of tty driver. 
    int flags;        // tty driver flags.

     // owner process.
    struct process_d *process;

    // control thread.
    struct process_d *thread;
    
    
    // #todo:
    // termios(s), tty(s)
    // obs: cada tty já tem um termios.
    // obs: ldisc tem duas tty(s).
    
    //ldisc: 'from' and 'to'
    struct ttyldisc_d *ldisc;
};
struct ttydrv_d *CurrentTTYDRV;





