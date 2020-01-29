
//esse arquivo foi criado por compatibilidade 

//typedef jmp_buf sigjmp_buf;


#ifndef _JB_ATTRIBUTES
#define _JB_ATTRIBUTES	/**/
#else
#endif
#ifndef _BSD_JBSLOT_T_
#define	_BSD_JBSLOT_T_	long
#endif

#ifndef ____sigset_t_defined
#define ____sigset_t_defined
#define _SIGSET_NWORDS (1024 / (8 * sizeof (unsigned long int)))
typedef struct
{
  unsigned long int __val[_SIGSET_NWORDS];
} __sigset_t;
#endif

typedef int __jmp_buf[6];

/* Calling environment, plus possibly a saved signal mask.  */
struct __jmp_buf_tag
  {
    /* NOTE: The machine-dependent definitions of `__sigsetjmp'
       assume that a `jmp_buf' begins with a `__jmp_buf' and that
       `__mask_was_saved' follows it.  Do not move these members
       or add others before it.  */
    __jmp_buf __jmpbuf;                /* Calling environment.  */
    int __mask_was_saved;        /* Saved the signal mask?  */
    __sigset_t __saved_mask;        /* Saved signal mask.  */
  };

typedef struct __jmp_buf_tag jmp_buf[1];





