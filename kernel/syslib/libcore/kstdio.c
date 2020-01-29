/*
 * File: libcore/kstdio.c 
 *
 *    i/o routines for the base kernel.
 *    ring 0.
 * 
 * Credits:
 *     printf support - Georges Menie's tutorial.
 *     https://www.menie.org/georges/embedded/small_printf_source_code.html
 *
 * History:
 *     2015 - Create by Fred Nora.
 *     2019 - Revision.
 */
  
 
#include <kernel.h>


// Herdadas do Boot Loader.
// De onde vem isso ?? head.s
// #todo: 
// Devemos chamar o m�dulo hal para obtermos esses valores.
// Depois salvamos em vari�veis internas usadas pela gui.

extern unsigned long SavedBootBlock;
extern unsigned long SavedLFB;
extern unsigned long SavedX;
extern unsigned long SavedY;
extern unsigned long SavedBPP; 


// Internas.

 


/*
 *********************************************
 * fclose:
 *     Close a file. 
 */

int fclose (file *stream){

    //if ( (void *) stream == NULL )
       //return EOF;

    if ( (void *) stream == NULL )
    {
        return (int) (-1);

    }else{

        stream->used = 1;
        stream->magic = 1234;

        stream->_p = NULL;
        stream->_cnt = 0;
        stream->_base = NULL;
        stream->_flags = 0;
        stream->_file = 0;
        stream->_charbuf = 0;
        stream->_lbfsize = 0;
        stream->_tmpfname = NULL;
        stream = NULL;

    };

	//...

    return 0;
}


/*
 ******************************************
 * fopen:
 *     Open a file.
 *     
 *     #todo: 
 *     Mas j� temos recursos para abrimos arquivos maiores 
 *     usando essa fun��o. � s� obtermos o tamanho do arquivo 
 *     e alocarmos um buffer do tamanho necess�rio
 */

//#bugbug
//ainda nao usamos o argumento mode.

//#bugbug
//na m�quina real falhou no momento de pegar o tamanho do arquivo.
//debug: vamos colocar verbose nessa rotina e olhar na m�quina real
//se o problema aparece.

//#bugbug
//problemas na string de nome.

// #todo: 
// Precisamos inicializar corretamente a estrutura antes de 
// retornarmos o ponteiro.

file *fopen ( const char *filename, const char *mode ){

    unsigned long fileret;

    int i;
    
	//struct _iobuf *stream;
    file *stream;

	// Buffer para armazenar o arquivo que vamos abrir.
    char *file_buffer;

	// #bugbug: 
	// aqui podemos usar o kmalloc ?
	// Buffer usado para colocar a estrutura, 
	// mas o problema � o free que ainda n�o funciona.

    // 1024 � desperd�cio.
    unsigned char struct_buffer[1024];

    //
    // stream;
    //

	//buffer da estrutura.
    stream = (file *) &struct_buffer[0];


	// #bugbug
	// Estamos com problemas com a string de nome.
	// #debug: vamos exib�-la.

	// #debug
	//printf ("before_read_fntos: %s\n", filename );
	
    read_fntos ( (char *) filename );

	// #debug
	//printf ("after_read_fntos: %s\n", filename );

	// #debug
	//refresh_screen ();
	
	
    //#bugbug
    //na m�quina real falhou no momento de pegar o tamanho do arquivo.
    //debug: vamos colocar verbose nessa rotina e olhar na m�quina real
    //se o problema aparece.
	
	//#test
	//temos que fazer isso de um jeito melhor

    size_t s = (size_t) fsGetFileSize ( (unsigned char *) filename );

    if ( s <= 0 )
    {
        printf ("fopen: file size \n");
        goto fail;

		//refresh_screen();
		//return (file *) 0;
    }


	// #debug
	//printf ("after_fsGetFileSize: %s\n", filename );
	//refresh_screen ();


	//#bugbug
	//Quando pegamos o tamanho do arquivo e for muito grande
	//o kmalloc vai falhar.
	//precisamos exibir o tamanho do arquivo.

	//vamos impor um limite.
	// 128 kb

	//printf ("klibc: fopen limits. size=%d \n", s);
	//refresh_screen ();


	/*
	 *usado para alocar mais p�ginas.
	Ret = (void*) allocPages(2);      //8KB. para imagem pequena.
	if( (void*) Ret == NULL ){
	    printf("Ret fail\n");
        goto fail;
        //return NULL;
	}
    */




	//alocando apenas uma p�gina.
	//4KB
	//Buffer do arquivo.
	//@todo: Deve ser maior. Do tamanho do arquivo.
	//Podemos usar outra rotina de aloca��o de p�gina.
	
	//#todo:
	//j� temos recursos para alocar mem�ria para um buffer maior.
	//obs: Essa aloca��o vai depender do tamanho do arquivo.
	


	//file_buffer = (char *) newPage();
    file_buffer = (char *) kmalloc (s);

    if ( (char *) file_buffer == NULL )
    {
        printf ("fopen: file_buffer \n");
        goto fail;

		//refresh_screen ();
		//return (FILE *) 0;
    }


	//
	// Configurando a stream;
	//

    if ( (void *) stream == NULL )
    {
        printf ("fopen: stream \n");
        goto fail;
    
		//refresh_screen();
		//return (FILE *) 0;

	}else{

        stream->used = 1;
        stream->magic = 1234;

        stream->_base = file_buffer;
        stream->_bf._base = file_buffer;
        stream->_lbfsize = s;    // File size.
        stream->_r = 0;
        stream->_w = 0;
        stream->_p = stream->_base;

		//?? #todo: 
		// precisamos de um id
		// Esse ID � um indice da estrutura de processo.

        stream->_file = 0; 

        stream->_tmpfname = (char *) filename;

        // Quanto falta para acabar o arquivo.
        stream->_cnt = s;


        // #todo
        // Talvez tenhamos que colocar o ponteiro em fileList[i]
         
		//...

    };


	// #bugbug:
	// Aten��o !!
	// Por enquanto esse esquema de pwd mais atrapalha que ajuda.

	// pwd support.
    fsUpdateWorkingDiretoryString ( (char *) filename );

    if ( current_target_dir.current_dir_address == 0 )
    {
        printf ("fopen: current_target_dir.current_dir_address fail \n");

		//reset.
        current_target_dir.current_dir_address = VOLUME1_ROOTDIR_ADDRESS;

        for ( i=0; i< 11; i++ )
        {
            current_target_dir.name[i] = '\0';
        }

        goto fail;
        //return (file *) 0;
    }

	//
	// Loading file.
	//


	//#debug
	//printf ("before_fsLoadFile: %s\n", filename );

    fileret = fsLoadFile ( VOLUME1_FAT_ADDRESS, 
                  current_target_dir.current_dir_address, 
                  (unsigned char *) filename, 
                  (unsigned long) stream->_base );

	//printf ("after_fsLoadFile: %s\n", filename );  

    if ( fileret != 0 )
    {
        stream = NULL;

        printf ("fopen: fsLoadFile fail\n");
        goto fail;

		//refresh_screen ();
		//return (FILE *) 0;
    }

done:
    return (file *) stream;

fail:

    refresh_screen ();
    return (file *) 0;
}

//Isso pertence � fcntl
int __openat (int dirfd, const char *pathname, int flags){

 
    file *stream;
    // #todo
    // dirfd, flags.

    // #bugbug
    // Improvisando com essa que funciona o carregamento.
    
    stream = (file *) fopen ( (const char *) pathname, "r" );


    if(!stream)
        return -1;

    // #bugbug
    // Esse n�mero esta errado.
    return stream->_file;
}


/*
 **************************************************
 * fread:  
 * 
 */
 
// #importante
// Ler uma certa quantidade de chars de uma stream e coloca-los no buffer.
// Isso vai ser usado pelo terminal por exemplo.
// Assim o terminal pode pegar uma string que esteja no arquivo.
// https://www.tutorialspoint.com/c_standard_library/c_function_fread.htm 


size_t fread (void *ptr, size_t size, size_t n, file *fp){


    //if ( (void *) fp == NULL )
        //return (size_t) -1;


	//tamanho do bloco. char short long ...
	//if ( size <= 0 )
	    //return -1;


    if ( n <= 0 )
    {
        printf ("fread: n \n");
        refresh_screen();
        return (size_t) -1;
    }


    if ( (void *) fp == NULL )
    {
        printf ("fread: fp\n");
        refresh_screen();
        return (size_t) -1;

    }else{

       // if (s->flags & IOSTREAM_READ)
       //switch (s->node->type) arquivo, dispositivo, etc ... 


		//if (stream->offset <= file->size)

		//ai tem algo errado
        if ( fp->_p < fp->_base )
        {
            printf ("fread: position fail\n");
            refresh_screen();
            return (size_t) -1;
        }

        // dst = buffer no app.
        // src = base do buffer da stream + offset de leitura.

        //#todo: 
        // ainda n�o temos uma base do buffer devidamente inicializada. 
		
		// #hack hack: 
		// provis�rio, deletar depois.
		// precisamos inicializar quando usamos fopen
		// suspendendo esse hack pois fiz as modifica��es no fopen
		// pra inicialziar a estrutura.
		// Ent�o isso deve funcionar sem esse ajuste 
		// pelo menos para arquivos abertos com fopen.
		
		//fp->_bf._base = fp->_base; 
		//fp->_r = 0; 

        //...

		//#debug
        printf ("fread: copiando para o buffer\n");
        printf ("_r=%d \n", fp->_r );
        printf ("_bf._base=%x \n", fp->_bf._base );
        refresh_screen ();

        // #todo: 
        // _r limits

        memcpy ( (void *) ptr, 
            (const void *) (fp->_bf._base + fp->_r ), 
            (unsigned long) n );

		 // Update.        
         fp->_r = (fp->_r + n);  
    };


    return 0;
}


/*
 ************************************************
 * fwrite:  
 *     #todo
 */

size_t fwrite (const void *ptr, size_t size, size_t n, file *fp){


   // if ( (void *) fp == NULL )
      // return -1;


	//tamanho do bloco. char short long ...
	//if ( size <= 0 )
	    //return -1;


    if ( n <= 0 )
    {
        printf ("fwrite: n \n");
        refresh_screen ();
        return (size_t) -1;
    }


    if ( (void *) fp == NULL )
    {
        printf ("fwrite: fp\n");
        refresh_screen ();
        return (size_t) -1;

    }else{

       // if (s->flags & IOSTREAM_READ)
       //switch (s->node->type) arquivo, dispositivo, etc ... 

		//#debug
        //printf ("fwrite: copiando do buffer\n");
        //printf ("_w=%d \n", fp->_w );
        //printf ("_bf._base=%x \n", fp->_bf._base );
        //refresh_screen ();

		// #todo: 
		// _w limits

        memcpy ( (void *) (fp->_bf._base + fp->_w ), 
            (const void *) ptr, 
            (unsigned long) n );
     
		// Update.        
        fp->_w = fp->_w + n; 
    };


    return 0;
}


// interna por enquanto.
// Mudar os nomes dos elementos da estrutura.
// para ficarem iguais aos nossos.
int __do_fflush (file *stream)
{
	// #todo
	// Isso parece ser bem legal.

    char *buf;    // Buffer.              
    ssize_t n;    // Byte count to flush. 


    //Not writable. 
    //if (!(stream->_flags & _IOWRITE))
        //return (0);


    // Not buffered.
    // se n�o tem buffer ent�o usamos a _base. 
    if (stream->_flags & _IONBF)
    {
		// #todo usar a _base.

	    // Nothing to flushed. 
	    // na sistua��o de estarmos usando a base.
        if ((n = stream->_p - stream->_base) == 0)
            return (0);

        //reset
        stream->_p = stream->_base;
        //stream->_cnt = (stream->_flags & _IOLBF) ? 0 : stream->_lbfsize - 1;
        
        /*
	    if (write(fileno(stream), stream->_base, n) != n)
	    {
		    stream->flags |= _IOERROR;
		    return (EOF);
	    }
        */

        return (0);
    }
    
    //
    // Com buffer !!  
    //

    /*

    // A estrutura do buffer,
    if ( (void *) stream->_bf == NULL )
    {
        return 0;
    }else{
 
        //if ( (void *) stream._bf->_base == NULL )
        //{ return 0; }
        
        //if ( stream->_p < stream._bf->_base )
            //return 0;
            
        //if ((n = stream->_p - stream._bf->_base) == 0)
            //return (0);

        //reset
        //stream->_p = stream._bf->_base;
        //stream->_cnt = (stream->_flags & _IOLBF) ? 0 : stream->_lbfsize - 1;
        
        //
        // Flush. 
        //
    
        // #todo
        // J� temos a fun��o write(...) ?

        
	    //if (write ( fileno(stream), stream._bf->_base, n) != n )
	    //{
		   // stream->_flags |= _IOERROR;
		   // return (EOF);
	   // }
        
        
        
         return (0);
    };
    
    */


     return (-1);
}


/* 
 *********************************************
 * fflush: (ring 0)
 *     ?? Salva o buffer no arquivo associado a ele e limpa o buffer. 
 *     ?? Exibe o conte�do do arquivo na tela do terminal.
 *     Se for NULL ent�o faz isso para todas as stream abertas.
 *     Retorna 0 se funcionar e retorna EOF se falhar.
 */

// The fflush() function is used because standard output is usually buffered 
// and the prompt may not immediately be printed on the output or terminal.

// https://linux.die.net/man/3/fflush
// http://man7.org/linux/man-pages/man3/fflush.3.html

/*
 #importante:
 Now, stdout is, by default, line buffered. This means, 
 the output sent to stdout is not sent to the screen for display 
 (or to the redirected files/streams) until it gets a newline character in it. 
 So, if you want to override the default buffering behaviour, 
 then you can use fflush to clear the buffer (and in the process, 
 send everything to the screen/file/stream).
 
 Ent�o fflush envia pra tela aquilo que est� somente no arquivo
 pois ainda n�o tem um '\n' '\r' no arquivo.
 Isso significa que fprintf n�o pode ativar a rotina de pintura
 enquanto n�o encontrar um '\n'
*/

int fflush ( file *stream ){

    int __ret;
    
    struct process_d *__P;
  
    int i;
    file *__tmp_stream;



	//Flush all streams.
    if (stream == NULL)
    {
        __ret = 0;

        __P = (struct process_d *) processList[current_process];
        
        if ( (void *) __P == NULL )
        {
			//#debug 
            panic ("fflush: __P\n");
            
            return (int) -1;
        }
         
         
        for (i=0; i<32; i++)
        {
            __tmp_stream = (file *) __P->Streams[i];
            
            
            if ( (void *) __tmp_stream != NULL )
            {
                __ret |= __do_fflush (__tmp_stream);
            }
        }


        return (int) __ret;
    }

    // Para apenas uma stream.
    return (int) __do_fflush (stream);
}




/*
 ***************************** 
 * update_standard_stream:
 *     Atualiza o fluxo padr�o para determinado processo
 *     #bugbug: Talvez n�o estamos mais usando isso.
 */

int 
update_standard_stream ( int PID, 
                         file *stream1, 
                         file *stream2, 
                         file *stream3 )
{
    struct process_d *p;

    //#debug
    //printf ("update_standard_stream: PID %d \n", PID);

	//#todo limits
	if ( PID < 0 )
		return -1;

    p = (struct process_d *) processList[PID];	

    if ( (void *) p == NULL )
    {
        panic ("update_standard_stream: p fail\n");
    }


	if ( (void *) stream1 == NULL || 
		 (void *) stream2 == NULL || 
		 (void *) stream3 == NULL )
	{
		panic ("update_standard_stream: args fail\n");
	}
	
	//p->Streams[0] = (unsigned long) stream1;  
	p->Streams[1] = (unsigned long) stream2; 
	p->Streams[2] = (unsigned long) stream3;
	
	//stdin = stream1;
	stdout = stream2;
	stderr = stream3;
	
	//
	// #bugbug
	// J� que foi passado um PID ent�o temos que atualizar 
	// o fluxo padr�o do processo e n�o o atual.
	//
	
	/*
    if ( (void *) CurrentTTY == NULL )
	{	
		panic ("update_standard_stream: CurrentTTY");
		
	}else{
	    
		//CurrentTTY->stdin = stdin;
	    CurrentTTY->stdout = stdout;
	    CurrentTTY->stderr = stderr;
	};
    */

	//#debug
	//printf ("update_standard_stream: done\n");
	//refresh_screen ();


    return 0;
}





/*
 *===========================================================================
 *  ==== Segue aqui o suporte a fun��o 'printf' ====
 *
 * #obs:
 * Em user mode temos uma modelo mais tradiciona de printf,
 * talvez seja bom implementa-lo aqui tamb�m.
 */


/*
 *****************************
 * prints:
 *     Rotina de suporta a printf. 
 */

int 
prints ( char **out, 
         const char *string, 
         int width, 
         int pad )
{
    register int pc = 0, padchar = ' ';

    if (width > 0) 
    {
	    register int len = 0;
		register const char *ptr;
		
		for (ptr = string; *ptr; ++ptr) ++len;
		if (len >= width) width = 0;
		else width -= len;
		if (pad & PAD_ZERO) padchar = '0';
    };


    if( !(pad & PAD_RIGHT) ) 
    {
		for ( ; width > 0; --width)
		{
		    printchar (out,padchar);
			++pc;
		};
    };


    for ( ; *string; ++string )
    {
		printchar (out, *string);
		++pc;
    };


    for ( ; width > 0; --width )
    {
		printchar (out,padchar);
		++pc;
    };


    return (int) pc;
}


/*
 ****************************************
 * printi:
 *     Rotina de suporta a printf.
 */

int 
printi ( char **out, 
         int i, 
         int b, 
         int sg, 
         int width, 
         int pad, 
         int letbase )
{
    char print_buf[PRINT_BUF_LEN];
    register char *s;
    register int t, neg = 0, pc = 0;
    register unsigned int u = i;


    if ( i == 0 ) 
    {
		print_buf[0] = '0';
		print_buf[1] = '\0';
		
		return prints (out, print_buf, width, pad);
    }


    if ( sg && b == 10 && i < 0 )
    {
		neg = 1;
		u = -i;
    }


    s = ( print_buf + ( PRINT_BUF_LEN -1 ) );

    *s = '\0';


    while (u) 
    {
		t = u % b;
		
		if ( t >= 10 )
		    t += letbase - '0' - 10;
		    *--s = t + '0';
		    u /= b;
    };


    if (neg) 
    {
		if ( width && (pad & PAD_ZERO) ) 
		{
		    printchar(out, '-');
			++pc;
			--width;
		}else {
			
			*--s = '-';
		};
    };


	// #obs: 
	// retorna pc + o retorno da fun��o.

    return (int) pc + prints(out, s, width, pad);
}


/*
 ****************************************
 * print:
 *     Rotina de suporta a printf.
 */

int print ( char **out, int *varg ){

    register int width, pad;
    register int pc = 0;
    register char *format = (char *) (*varg++);
    char scr[2];

    for ( ; *format != 0; ++format ) 
    {

		if ( *format == '%' ) 
		{
			++format;
			width = pad = 0;
			
			if ( *format == '\0' ) 
			    break;
			
			if ( *format == '%' ) 
			    goto out;
			
			if ( *format == '-' ) 
			{
				++format;
				pad = PAD_RIGHT;
			};
			
			while ( *format == '0' ) 
			{
				++format;
				pad |= PAD_ZERO;
			};
			
			for ( ; *format >= '0' && *format <= '9'; ++format )
			{
				width *= 10;
				width += *format - '0';
			};
			
			if ( *format == 's' ) 
			{
				register char *s = *((char **)varg++);
				pc += prints (out, s?s:"(null)", width, pad);
				continue;
			}
			
			if ( *format == 'd' ){
				pc += printi (out, *varg++, 10, 1, width, pad, 'a');
				continue;
			}
			
			if ( *format == 'x' ){
				pc += printi (out, *varg++, 16, 0, width, pad, 'a');
				continue;
			}
			
			if ( *format == 'X' ){
				pc += printi (out, *varg++, 16, 0, width, pad, 'A');
				continue;
			}
			
			if ( *format == 'u' ){
				pc += printi (out, *varg++, 10, 0, width, pad, 'a');
				continue;
			}
			
			if ( *format == 'c' ) 
	        {
				/* char are converted to int then pushed on the stack */
				scr[0] = *varg++;
				scr[1] = '\0';
				pc += prints (out, scr, width, pad);
				continue;
			}
		
		}else{
			
		    out:
			    printchar (out, *format);
			    ++pc;
		};
    };


    if (out) 
        **out = '\0';


    return (int) pc;
}


/*
 ***************************************************
 * printf:
 *     @field 2
 *     The printf function.
 *     Assuming sizeof(void *) == sizeof(int).
 *
 * Em user mode temos uma modelo mais tradiciona de printf,
 * talvez seja bom implementa-lo aqui tambem.
 */

int printf ( const char *format, ... ){

    register int *varg = (int *) (&format);

    return (int) print ( 0, varg );
}



// print() is a helper function for this one.
/*
int vsprintf(char *string, const char *format, va_list ap);
int vsprintf(char *string, const char *format, va_list ap)
{
}
*/




/*
 ******************
 * puts: 
 *     provis�rio ...
 */

int puts ( const char *str )
{
    return (int) printf ("%s",str);
}




/*
 ***********************************************************
 * sprintf:
 *     @field 2
 *     int sprintf ( char * str, const char * format, ... );
 *     Composes a string with the same text that would be 
 * printed if format was used on printf, 
 * but instead of being printed, the content is stored 
 * as a C string in the buffer pointed by str.
 */

int sprintf ( char *str, const char *format, ... ){

    register int *varg = (int *) (&format);

    return (int) print (&str, varg);
}


/*
 ************************************************
 * fprintf:
 *     A estrtutura � gerenciada em ring0.
 *     A libc em ring3 apenas chamar� essa rotina.
 *     servi�o 234.
 */

// fflush � line buffered,
// Ent�o fflush envia pra tela aquilo que est� somente no arquivo
// pois ainda n�o tem um '\n' '\r' no arquivo.
// Isso significa que fprintf n�o pode ativar a rotina de pintura
// enquanto n�o encontrar um '\n'

int fprintf ( file *stream, const char *format, ... ){

    register int *varg = (int *) (&format);

	//
	// Validation.
	//

	//#debug
	//kprintf ("klibc-stdio-fprintf: stream=%x \n",stream);
	//kprintf ("klibc-stdio-fprintf: stdout=%x \n",stdout);


    if ( (void *) stream == NULL )
    {
        panic ("klibc-stdio-fprintf: stream\n");
    }else{

        if ( stream->used != 1 || stream->magic != 1234 )
        {
            panic ("klibc-stdio-fprintf: stream validation\n");
        }
		//...
    };


	//
	// print 
	//

	// #obs:
	// Isso deve ser feito antes e alterarmos o ponteiro _ptr de stdout.
	
//print:
	
	
	// Se a stream for a stdout ent�o vamos ter alertar para pintar.
	//#obs: esquema antigo, n�o usaremos mais isso por enquanto.
	
	/*
	if (stream == stdout)
	{
		//Indicamos que deve pintar.
		//isso ser� ativado pelo fflush
		CurrentTTY->stdout_status = 1;
		
		//se temos um print pendente n�o precisamos mudar o last.
		//se n�o temos um print pendente ent�o precisamos mudar o last.
		if ( CurrentTTY->print_pending == 0 )
		{
			//#bugbug: Na verdade s� podemos ativar essa
			//pend�ncia quando encontrarmos um '\n' '\r'
			//ent�o ent�o o terminal pode mostrar na tela o conte�do
			//de uma linha, normalmente a �ltima do terminal,
			//mudando de linha efetua-se o scroll.
			//>> Se ainda n�o temos um '\n' s� um fflush mostraria
			//o conte�do na tela.
			
			
			//#importante
			//Mudamos isso para printchar
			//ent�o somente depois de encontrar o \n ativaremos essa flag.
			//Isso na teoria.Porque na pr�tica t� imprimindo pra todos os casos. haha
			//CurrentTTY->print_pending = 1;
			
	        // Indicamos de onde a rotina de pintura deve come�ar.
		    CurrentTTY->stdout_last_ptr = stdout->_p;
		}
	}	
	*/




	//
	// Colocando os chars dentro do arquivo.
	//

	// Colocamos no ponteiro e nao na base.
    char *str = (char *) stream->_p;

	// #todo
	// Tem que atualizar o ponteiro com uma strlen.

    size_t len = 0;
    len = (size_t) strlen ( (const char *) format);


    int status = -1;
    status = (int) print (&str, varg);

	// Depois de ter imprimido ent�o atualizamos o ponteiro de entrada 
	// no arquivo.

    stream->_p = stream->_p + len;


    return (int) status;
}


/*
 ********************************
 * fputs:      
 */

int fputs ( const char *str, file *stream ){

    int size = 0;
    

    //if ( (void *) stream == NULL )
      // return EOF;


    if ( (void *) stream == NULL )
    {
		return (int) (-1);

    } else {
		
		size = (int) strlen (str);
		
		if ( size > stream->_cnt )
		{
			return (int) (-1);
		}
		
		stream->_cnt = (int) (stream->_cnt - size);
		
		sprintf ( stream->_p, str );
		
		stream->_p = stream->_p + size;

        return 0;
    };


    return (int) (-1);
}


/*
 *********************************
 * ungetc:
 */

int ungetc ( int c, file *stream ){

    if (c == EOF) 
        return (int) c;


    if ( (void *) stream == NULL )
       return EOF;

	//@todo: flag oef.
	//stream->flags = (stream->flags & ~_IOEOF);
	
	stream->_p--;
	
	stream->_p[0] = (char) c;


    return (int) c;
}


/*
 * ftell: 
 * 
 */

long ftell (file *stream){


    if ( (void *) stream == NULL )
       return EOF;

    return (long) (stream->_p - stream->_base);
}


/*
 * fileno:
 * 
 * 
 */
 
int fileno ( file *stream ){

    if ( (void *) stream == NULL )
       return EOF;

    // fd
    return (int) stream->_file;  
}


/*
 *********************************
 * fgetc:
 *     #precisamos exportar isso como servi�o. (#136)
 */

int fgetc ( file *stream ){

    int ch = 0;


    if ( (void *) stream == NULL )
    {
		// #debug
		printf ("fgetc: stream struct fail\n");
		refresh_screen();
		
		return (int) (-1);

    }else{

		 //(--(p)->_r < 0 ? __srget(p) : (int)(*(p)->_p++))
		
		//#fim.
		//cnt decrementou e chegou a zero.
		//N�o h� mais caracteres dispon�veis entre 
		//stream->_ptr e o tamanho do buffer.
		
		if ( stream->_cnt <= 0 )
		{
			stream->_flags = (stream->_flags | _IOEOF); 
			stream->_cnt = 0;
			
		    //printf ("#debug: fgetc: $\n");
			
			//isso funciona, significa que a estrutura tem ponteiro e base validos.
			//printf("show fgetc:: %s @\n", stream->_base );
		    //refresh_screen();
			
			return (int) (-1);
		};
		
		//#debug
		//nao podemos acessar um ponteiro nulo... no caso endere�o.
		
		if ( stream->_p == 0 )
		{
			printf ("#debug: fgetc: stream struct fail\n");
		    refresh_screen();
			return (int) (-1);
			
		}else{
			
			// #obs: 
			// Tem que ter a op��o de pegarmos usando o posicionamento
			// no buffer. O terminal gosta dessas coisas.
			
		    //Pega o char no posicionamento absoluto do arquivo
		    ch = (int) *stream->_p;
				
            stream->_p++;
            stream->_cnt--;

		    return (int) ch;
		
		}
		//fail
    };


	//#debug
    printf ("fgetc: $$\n");
	refresh_screen();


    return (int) (-1);
}


/*
 *********************************
 * feof:
 */

int feof ( file *stream ){

    int ch = 0;

 
    if ( (void *) stream == NULL )
    {
		return (int) (-1);
		
    } else {

	    ch = fgetc (stream);
		
        if ( ch == EOF )
		{
			return (int) 1;
		
		}else{
			
			return 0;
		};
    };


	//checar se o eof foi atingido.
	// return( (stream->_flag & _IOEOF) );

    return 0;
}


/*
 *********************************
 * ferror:
 *
 */

int ferror ( file *stream ){

    if ( (void *) stream == NULL )
       return EOF;


    return (int) ( ( stream->_flags & _IOERR ) );
}



/*
 **************************************
 * fseek:
 *     offset argument is the position that you want to seek to,
 *     and whence is what that offset is relative to.
 */

int fseek ( file *stream, long offset, int whence ){

    if ( (void *) stream == NULL )
    {
        goto fail;
    }


	// Checar limites do offset.

    switch (whence){

        case SEEK_SET:
		    //printf ("SEEK_SET\n");   
		    stream->_p = (stream->_base + offset); 
			goto done;
			break;
			
        case SEEK_CUR:
		    //printf ("SEEK_CUR\n");
		    stream->_p = (stream->_p + offset);
		    goto done;
			break;

        case SEEK_END:
		    //printf ("SEEK_END stream->_lbfsize=%d \n",stream->_lbfsize);
		    stream->_p = ((stream->_base + stream->_lbfsize) + offset); 
		    goto done;
			break;

        default:
            //printf ("default:\n");
		    goto fail;
			break;
    };


fail:
	printf ("fseek fail\n");
	refresh_screen();
    return (int) (-1);

done:
    //refresh_screen();
    return 0;
}



// bsd stuff
/*
 * Various output routines call wsetup to be sure it is safe to write,
 * because either _flags does not include __SWR, or _buf is NULL.
 * _wsetup returns 0 if OK to write, nonzero otherwise.
 */
/*
int __swsetup(file *fp)
int __swsetup(file *fp)
{
    return -1;
}
*/


/*
 * Write the given character into the (probably full) buffer for
 * the given file.  Flush the buffer out if it is or becomes full,
 * or if c=='\n' and the file is line buffered.
 */

// #importante
// Isso � usado em __sputc no bsd.

/*
int __swbuf (int c, file *fp);
int __swbuf (int c, file *fp)
{
    return -1;
}
*/



/*
 *****************************************
 * fputc:
 */

int fputc ( int ch, file *stream ){

    //if ( (void *) stream == NULL )
       //return EOF;


    if ( (void *) stream == NULL )
    {
        return (int) (-1);

    }else{

		// se tivermos um posicionamento v�lido de escrita no buffer ou
		// se a posi��o de escrita no buffer for maior que o limite
		// do buffer e o char for diferente de fim de linha
		// ent�o usaremos ponteiro absoluto para escrever no arquivo.
		// Isso acontece poque desejamos continuar colocando coisa no arquivo
		// mesmo depois que o buffer se esgota.
		
		// caso contr�rio escreveremos no buffer.
			
			
		// Se ainda n�o esgotamos o buffer,
		// ou se esgotamos o buffer mas o caractere n�o � um 
		// caractere de fim de linha;

		// if ( stream->_w-- >= 0 || 
		//      ( stream->_w >= stream->_lbfsize && (char) ch != '\n' ) )

        // Buffer is not full.
        // Quanto falta para acabar.
        if ( stream->_cnt > 0 && ch != '\n' )
        {    
             stream->_cnt--;

             sprintf ( stream->_p, "%c", ch);
             stream->_p++;
            
             stream->_w++; 
             
             return (int) ch;  
        }
        
        // se o buffer est� cheio.
        
        /*
        //Now writing. 
        if (stream->flags & _IORW)
        {
            stream->flags &= ~_IOREAD;
            stream->flags |= _IOWRITE;
        }
        */        
        
        //File is not writable. 
        //if (!(stream->flags & _IOWRITE))
             //return (EOF);        
        
        
        // ...
    };

    //fail
    return (int) (-1);
}


/*
 ********************************** 
 * fscanf:
 * 
 * 
 * 
 */
 
// (since C99)
// int fscanf( file *restrict stream, const char *restrict format, ... );
// (until C99)

int fscanf (file *stream, const char *format, ... )
{
	
    if ( (void *) stream == NULL )
       return EOF;

    // #obs:
    // Existe um scanf completo em ring3.
    // Talvez n�o precisamos de outro aqui.

    printf ("fscanf: todo \n");
    return (int) -1;
}


/*
int vfprintf ( file *stream, const char *format, stdio_va_list argptr );
int vfprintf ( file *stream, const char *format, stdio_va_list argptr )
{
}
*/


/*
 * Writes format output of a stdarg argument list to a file.
 */

/* 
int vfprintf(file *stream, const char *format, va_list ap);
int vfprintf(file *stream, const char *format, va_list ap)
{
	int n;             // Characters written. 
	char buffer[1024]; // Buffer.             
	
	// Format string. 
	n = vsprintf(buffer, format, ap);
	
	// Write formated string to file. 
	if (n > 0)
		fputs(buffer, stream);
	
	return (n);
}
*/




/*
 * rewind
 * 
 * 
 */

void rewind ( file *stream )
{
    //fseek (stream, 0L, SEEK_SET);

	if ( (void *) stream == NULL )
		return;
		
    stream->_p = stdin->_base;
}


/*
 *************************************************************
 * printchar:
 *     Coloca o caractere na string ou imprime.
 * Essa fun��o chama uma rotina que dever� tratar o caractere e em seguida 
 * envi�-lo para a tela.
 * Essa rotina � chamada pelas fun��es: /print/printi/prints.
 */

void printchar (char **str, int c)
{
	// #importante
	// Se a string existe colocamos nela,
	// caso contr�rio imprimimos no backbuffer.
	// Vamos aproveitar esse momento para ativarmos a
	// pintura no caso dos caraters enviados para uma 
	// stream de output, como stdout.

	// Ativaremos a rotina de mostrar na tela s� no momento em que 
	// encontramos um fim de linha.

    if (str)
    {
		if ( c == '\n' ) 
		{
			CurrentTTY->print_pending = 1;
		}
		
		**str = c;
		
		++(*str);
		
    }else (void) putchar (c);
}


/*
 ********************************************************************
 * putchar:
 *     Put a char on the screen. (libC).
 *     Essa rotina chama uma rotina de tratamento de caractes, somente
 * depois � que o caractere ser� enviado para a tela.
 *     Essa rotina � chamada pelas fun��es: /printchar/input/.
 */

int putchar (int ch){ 
   
   
    // Para virtual consoles.
    // Em tty/console.c
    console_outbyte (ch,current_vc );


    // Para pseudo terminal (pts).
    //Em tty/vt.c
    //vt_outbyte (ch,tty );

    return (int) ch;    
}


/*
 ****************************
 * getchar:
 *    #todo: 
 *    Isso deve er oferecido como servi�o pelo kernel.
 *    The getchar function is equivalent to getc with stdin as 
 *    the value of the stream argument.
 */
/* 
int getchar()
{
	//terminal.h
	
	
	//erro
	if( teminalfeedCHStatus != 1 )
	{
		return (int) -1;
	}
	
	
done:
    teminalfeedCHStatus = 0;
	return (int) teminalfeedCH;
}
*/


/*
 essa fun��o � legal ... habilitar quando der.
 
void stdio_ClearToEndOfLine();
//limpa com caracteres em branco at� antes da posi��o do cursor.
void stdio_ClearToEndOfLine()
{
    unsigned u;
    unsigned long OldX, OldY;
    
    OldX = g_cursor_x;
	OldY = g_cursor_y;
	
	//de onde o cursor est� at� o fim da linha.
	for( u = g_cursor_x; u < g_cursor_right; u++ )
	{
       _outbyte(' ', current_vc);
    }
	
    g_cursor_x = OldX;
	g_cursor_y = OldY;

}
*/


/*
 essa fun��o � legal ... habilitar quando der.
 
void stdio_ClearToStartOfLine();
//limpa com caracteres em branco at� antes da posi��o do cursor.
void stdio_ClearToStartOfLine()
{
    unsigned u;
    unsigned long OldX, OldY;
    
    OldX = g_cursor_x;
	OldY = g_cursor_y;
	
	//In�cio da linha.
    g_cursor_x = 0;
	g_cursor_y = OldY;	
	
	//de onde o cursor est� at� o fim da linha.
	for( u = g_cursor_x; u < g_cursor_right; u++ )
	{
       _outbyte(' ',current_vc);
    }
	
    g_cursor_x = OldX;
	g_cursor_y = OldY;

}
*/



/*
 ******************************************************************
 * input:
 *     Coloca os caracteres digitados em um buffer, (string). 
 * Para depois comparar a string com outra string, que � um comando.
 * 
 *     Devemos nos certificar que input(.) n�o imprima nada.
 *
 * History:
 *     2015 - Created by Fred Nora.
 *     ...
 */

unsigned long input ( unsigned long ch ){

    int i;

	// Converte 'unsigned long' em 'char'.
    char c = (char) ch;


    if ( g_inputmode == INPUT_MODE_LINE )
    {
        //Limite.
	    if(prompt_pos >= PROMPT_SIZE)
	    {
	        printf ("input: INPUT_MODE_LINE full buffer!\n");
	        refresh_screen ();
			return (unsigned long) 0; 
	    };
    };


    if (g_inputmode == INPUT_MODE_MULTIPLE_LINES )
    {
		//tem que ter o tamanho de um arquivo.
		if (prompt_pos >= PROMPT_SIZE)
		{
	        printf("input: INPUT_MODE_MULTIPLE_LINES full buffer\n");
	        refresh_screen();
			return (unsigned long) 0; 
		}
    };


	// Trata o caractere digitado. 


    switch (c)
    {

	    //Enter.
		//+se for modo comando devemos finalizar com zero.
		//+se for modo texto, devemos apenas incluir os caracteres \r \n.
		//case 0x1C:
		case VK_RETURN:
            //modo linha 
			if(g_inputmode == INPUT_MODE_LINE )
			{
			    prompt[prompt_pos] = (char )'\0'; //end of line.
			    //@todo: ?? ldiscCompare();
				//o compare est� no aplicativo.
	            for(i=0; i<PROMPT_MAX_DEFAULT;i++)
	            {
		            prompt[i] = (char) '\0';
		            prompt_out[i] = (char) '\0';
		            prompt_err[i] = (char) '\0';
	            };
                prompt_pos = 0;
				goto input_done;
			};
            //modo multiplas linhas 
		    if (g_inputmode == INPUT_MODE_MULTIPLE_LINES )
		    {
			    prompt[prompt_pos] = (char )'\r';
                prompt_pos++;
				prompt[prompt_pos] = (char )'\n';
				prompt_pos++;
			};
		    break;

	    //Backspace.
		case 0x0E:
		
            if ( prompt_pos <= 0 )
            {
			    prompt_pos = 0;
				prompt[prompt_pos] = (char ) '\0';
				break; 
			}
		    
			//Apaga o anterior (no buffer).
			prompt_pos--;
			prompt[prompt_pos] = (char ) '\0';
			break;
			
		//...	
		
        //Para qualquer caractere que n�o sejam os especiais tratados acima.
		default:
		    prompt[prompt_pos] = c;
		    prompt_pos++;          //incrementa fila
			break;
    };


input_more:
    return 0;


input_done:
    return VK_RETURN;
}


/*
 *****************
 * stdioInitialize:
 *     Inicializando stdio pertencente ao kernel base.
 *     Inicializa as estruturas do fluxo padr�o.
 *     Quem chamou essa inicializa��o ?? Em que hora ??
 *
 * #bugbug: Pelo jeito somente depois dessa inicializa��o � que temos mensagens 
 * com printf decentes. Ent�o a inicializa��o do kernel precisa disso.
 * >> precisamos antecipar essa iniciliza��o. Mas ela precisa ser depois da
 * inicializa��o da pagina��o.
 */
 
// Estamos no kernel base em ring 0.
// Queremos que as streams sejam acess�veis para as rotinas
// da libc em ring3. Para a libc alterar os elementos
// da estrutura.
// #bugbug: Talvez seja poss�vel criar essas estruturas
// em mem�ria compartilhada, usado o alocaro apropriado.
// kmalloc com certeza e ring0.

int stdioInitialize (void){

    int Status = 0;
    int i;

    // Buffers para as estruturas.
    unsigned char *buffer0;
    unsigned char *buffer1;
    unsigned char *buffer2;

    int cWidth = get_char_width ();
    int cHeight = get_char_height ();


    if ( cWidth == 0 || cHeight == 0 ){
        panic ("klibc-stdioInitialize: Char info");
    }



	// #bugbug:
	//  4KB alocados para cada estrutura. Isso � muito.
	//  Mas ao mesmo tempo estamos economizando o heap 
	//  usado pelo kmalloc.
	//  Podemos alocar 4KB para o buffer. 'prompt'
	
	
	// Alocando uma p�gina para cada buffer.
	// 4KB size.
	// #importante
	// Obs: Essas p�ginas s�o alocadas em user mode.

    // #obs:
    // Esse alocador usou mem�ria compartilhada ?

	//4KB
    buffer0 = (unsigned char *) newPage (); 
    
    if ( (unsigned char *) buffer0 == NULL ){
        panic ("klibc-stdioInitialize: buffer0 \n");
    }


	//4KB
    buffer1 = (unsigned char *) newPage ();

    if ( (unsigned char *) buffer0 == NULL ){
        panic ("klibc-stdioInitialize: buffer1 \n");
    }


	//4KB
    buffer2 = (unsigned char *) newPage ();

    if ( (unsigned char *) buffer0 == NULL ){
        panic ("klibc-stdioInitialize: buffer2 \n");
    }


	// Alocando mem�ria para o fluxo padr�o do 
	// processo kernel.
	// Estamos apenas alocando mem�ria para a estrutura.

    stdin  = (file *) &buffer0[0];
    stdout = (file *) &buffer1[0];
    stderr = (file *) &buffer2[0];
    
    
    //
    // prompt[]
    //

    // Esses prompts s�o usados como arquivos.
    // S�o buffers para as streams.
    //See: include/kernel/stdio.h
    
    for ( i=0; i<PROMPT_SIZE; i++ )
    {
		prompt[i]     = (char) '\0';
		prompt_out[i] = (char) '\0';
		prompt_err[i] = (char) '\0';
    };

    prompt_pos = 0;



    // Configurando a estrutura de stdin. 
	stdin->used = 1;
	stdin->magic = 1234;
	stdin->_base = &prompt[0];    //See: include/kernel/stdio.h
	stdin->_p =  &prompt[0];
	stdin->_bf._base = stdin->_base;
	stdin->_lbfsize = PROMPT_SIZE; //128; //#todo
	stdin->_r = 0;
	stdin->_w = 0;
	stdin->_cnt = PROMPT_SIZE;
	stdin->_file = 0;
	stdin->_tmpfname = "kstdin";
	//...


    // Configurando a estrutura de stdout.
	stdout->used = 1;
	stdout->magic = 1234;
	stdout->_base = &prompt_out[0];  //See: include/kernel/stdio.h
	stdout->_p = &prompt_out[0];
	stdout->_bf._base = stdout->_base;
	stdout->_lbfsize = PROMPT_SIZE; //128; //#todo
	stdout->_r = 0;
	stdout->_w = 0;
	stdout->_cnt = PROMPT_SIZE;
	stdout->_file = 1;
	stdout->_tmpfname = "kstdout";
	//...


    // Configurando a estrutura de stderr.
	stderr->used = 1;
	stderr->magic = 1234;
	stderr->_base = &prompt_err[0];  //See: include/kernel/stdio.h
	stderr->_p =  &prompt_err[0];
	stderr->_bf._base = stderr->_base;
	stderr->_lbfsize = PROMPT_SIZE; //128; //#todo
	stderr->_r = 0;
	stderr->_w = 0;
	stderr->_cnt = PROMPT_SIZE;
	stderr->_file = 2;
	stderr->_tmpfname = "kstderr";
	//...


    // #importante
    // Salvando os ponteiros na lista de arquivos.
    // Essas estruturas est�o em mem�ria compartilhada ??
    // A libc em ring3 poder� acessar os elementos dessa estrutura ?

    fileList[__KERNEL_STREAM_STDIN]  = (unsigned long) stdin;
    fileList[__KERNEL_STREAM_STDOUT] = (unsigned long) stdout;
    fileList[__KERNEL_STREAM_STDERR] = (unsigned long) stderr;

	//Os pr�ximos s�o inicializados em fs.c
	//fileList[3] volume0 root dir (vfs) 
	//fileList[4] volume1 root dir (boot volume)
	//fileList[5] volume2 root dir  (system volume)
	//...





	// Flag para o tipo de input.
	// # Multiplas linhas.

	
	g_inputmode = INPUT_MODE_MULTIPLE_LINES;
	

    //
    // Virtual console.
    //
    
    // Configurando o cursor para todos os consoles.

    // See:
    // tty.h
    // console.h

    for (i=0; i<4; i++)
    {
        TTY[i].cursor_x = 0;   
        TTY[i].cursor_y = 0;     
        TTY[i].cursor_left = 0; 
        TTY[i].cursor_top = 0;  
        TTY[i].cursor_right  = (SavedX/cWidth);    
        TTY[i].cursor_bottom = (SavedY/cHeight);  

        TTY[i].cursor_color = COLOR_TERMINALTEXT;  
    };



	//
	// Keyboard
	//




	//fluxo padrao
	
	//#importante
	//usando o buffer keybuffer coko arquivo.
	//ele esta em gws/user.
	
	
	//
	// ## stdin
	//

    unsigned char *current_stdin_struct_buffer;
    unsigned char *current_stdin_data_buffer;

	current_stdin_struct_buffer = (unsigned char *) newPage ();
	current_stdin_data_buffer = (unsigned char *) newPage ();
	
	current_stdin = (file *) &current_stdin_struct_buffer[0];
	
	current_stdin->used = 1;
	current_stdin->magic = 1234;
	current_stdin->_base = (unsigned char *) &current_stdin_data_buffer[0];
	current_stdin->_bf._base = current_stdin->_base;
	current_stdin->_r = 0;
	current_stdin->_w = 0;
	current_stdin->_p = (unsigned char *) &current_stdin_data_buffer[0];
	current_stdin->_cnt = 128;  //Limitando. na verdade e' 4KB.
	current_stdin->_lbfsize = 128;
	//#todo: Colocar em fileList[i]
	
	//
	// ## stdout
	//
	
	unsigned char *current_stdout_struct_buffer;
	unsigned char *current_stdout_data_buffer;
	
	current_stdout_struct_buffer = (unsigned char *) newPage ();
	current_stdout_data_buffer = (unsigned char *) newPage ();	
	
	current_stdout = (file *) &current_stdout_struct_buffer[0];
	
	current_stdout->used = 1;
	current_stdout->magic = 1234;		
	current_stdout->_base = (unsigned char *) &current_stdout_data_buffer[0];
	current_stdout->_bf._base = current_stdout->_base;
	current_stdout->_r = 0;
	current_stdout->_w = 0;
	current_stdout->_p  = (unsigned char *) &current_stdout_data_buffer[0];
	current_stdout->_cnt = 128;  //Limitando. na verdade e' 4KB.
	current_stdout->_lbfsize = 128;
	//#todo: Colocar em fileList[i]
	
	//
	// ## stderr
	//	
	
	unsigned char *current_stderr_struct_buffer;
	unsigned char *current_stderr_data_buffer;
	
	current_stderr_struct_buffer = (unsigned char *) newPage();
	current_stderr_data_buffer = (unsigned char *) newPage ();
	
	current_stderr = (file *) &current_stderr_struct_buffer[0];
	
	current_stderr->used = 1;
	current_stderr->magic = 1234;
	current_stderr->_base = (unsigned char *) &current_stderr_data_buffer[0];
	current_stderr->_bf._base = current_stderr->_base;
	current_stderr->_r = 0;
	current_stderr->_w = 0;
	current_stderr->_p  = (unsigned char *) &current_stderr_data_buffer[0];
	current_stderr->_cnt = 128;  //Limitando. na verdade e' 4KB.
	current_stderr->_lbfsize = 128;
    //#todo: Colocar em fileList[i]
    
    
	// Done !

    return 0;

fail:
    panic ("stdio-stdioInitialize: fail\n");
}




/*
 * setbuf:
 * 
 */

// see: 
// https://linux.die.net/man/3/setvbuf

void setbuf (file *stream, char *buf){

    if ( (void *) stream == NULL )
    {
		return;
    }else{
	
		//#todo
		//se o buffer � v�lido.
        //if (stream->_bf._base != NULL) 
        //{
            //if (stream->cnt > 0)
                //fflush (stream);
                
            //free (stream->buf);
        //}
        
        // Udate stream.
        stream->_bf._base = buf;
        //stream->_lbfsize = size;        
        // ?? stream->bufmode = mode;

        stream->_p = buf;
        // ??stream->cnt = 0;
        //...
    };
}


/*
 * setbuffer:
 * 
 */
 
void setbuffer (file *stream, char *buf, size_t size){

    if ( (void *) stream == NULL )
    {
		return;
    }else{

		//#todo
		//se o buffer � v�lido.
        //if (stream->_bf._base != NULL) 
        //{
            //if (stream->cnt > 0)
                //fflush (stream);
                
            //free (stream->buf);
        //}
        
        // Udate stream.
        stream->_bf._base = buf;
        stream->_lbfsize = size;        
        // ?? stream->bufmode = mode;

        stream->_p = buf;
        // ??stream->cnt = 0;
        //...
    };
}


/*
 * setlinebuf:
 * 
 */
 
void setlinebuf (file *stream)
{
    if ( (void *) stream == NULL )
       return;
}



/*
 * setvbuf: 
 * 
 * 
 */

int setvbuf (file *stream, char *buf, int mode, size_t size){

    if ( (void *) stream == NULL )
    {
		return -1;
    }else{

		//#todo
		//se o buffer � v�lido.
        //if (stream->_bf._base != NULL) 
        //{
            //if (stream->cnt > 0)
                //fflush (stream);
                
            //free (stream->buf);
        //}
        
        // Udate stream.
        stream->_bf._base = buf;
        stream->_lbfsize = size;        
        // ?? stream->bufmode = mode;

        stream->_p = buf;
        // ??stream->cnt = 0;
        //...
    };

    return 0;
}


//
// End.
//

