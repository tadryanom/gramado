/*
 * File: stdio.c 
 *
 * Descrição:
 *     Rotinas de input/output para User Mode.
 *     Standard C language. C99.
 *
 * Environment:
 *     >>> Ring 3 <<<
 *
 * Credits:
 *     printf: https://www.menie.org/georges/embedded/small_printf_source_code.html
 *     stdio_fntos: Luiz Felipe
 *
 * History:
 *     2015 - Created.
 *     2015~2018 - Revision. 
 */

#include <limits.h> //testando no printf do nelson
//#include <string.h>

#include <stdio.h>
#include <types.h> 
#include <stdarg.h> 

#include <stddef.h>
//#include <stdlib.h> 

#include <ctype.h>

//system calls.
#include <stubs/gramado.h> 



// Testando define.

#define	SYSTEMCALL_READ_LBA    1
#define	SYSTEMCALL_WRITE_LBA   2
#define	SYSTEMCALL_READ_FILE   3
#define	SYSTEMCALL_WRITE_FILE  4

#define VK_RETURN      0x1C 
#define VK_BACKSPACE   0x0E 
#define VK_BACK	       0x0E  
#define VK_TAB         0x0F 
//PRINT_BUF_LEN


//internas.
// estamos falando do posicionamento do cursor dentro da janela 
// e não dentro do terminal. 
void stdioSetCursor( unsigned long x, unsigned long y );
unsigned long stdioGetCursorX(); 
unsigned long stdioGetCursorY(); 

//interna; Mudar o nome
static size_t stdio_strlen (const char *s);


//atoi. # talvez isso possa ir para o topo do 
//arquivo para servir mais funções.
int stdio_atoi (char *s){
	
    int rv=0; 
    char sign = 0;

    /* skip till we find either a digit or '+' or '-' */
    while (*s) 
	{
	    if (*s <= '9' && *s >= '0')
		    break;
	    if (*s == '-' || *s == '+') 
		    break;
	    s++;
    }; 	  

    if (*s == '-')
	    sign=1;

    //     sign = (*s == '-');
    if (*s == '-' || *s == '+') 
	    s++;

    while (*s && *s >= '0' && *s <= '9') 
	{
	    rv = (rv * 10) + (*s - '0');
        s++;
    };

    if (sign) return (-rv);
        else return (rv);
     
    //     return (sign ? -rv : rv);
}



//rotina interna de support.
//isso deve ir para bibliotecas depois.
//não tem protótipo ainda.
// Credits: Luiz Felipe

void stdio_fntos (char *name){
	
    int  i, ns = 0;
    char ext[4];
	
	ext[0] = 0;
	ext[1] = 0;
	ext[2] = 0;
	ext[3] = 0;
	
    //const char ext[4];
	
    //Transforma em maiúscula enquanto não achar um ponto.
	
	while ( *name && *name != '.' )
	{
        if ( *name >= 'a' && *name <= 'z' )
            *name -= 0x20;

        name++;
        ns++;
    };

    // Aqui name[0] é o ponto.
	// Então constrói a extensão.
	
	for ( i=0; i < 3 && name[i+1]; i++ )
	{
		//Transforma uma letra da extensão em maiúscula.
        
		//if (name[i+1] >= 'a' && name[i+1] <= 'z')
        //    name[i+1] -= 0x20;

        //ext[i] = name[i+1];
    
	
	    //#testando
	    //Se não for letra então não colocamos no buffer de extensão;
		if (name[i+1] >= 'a' && name[i+1] <= 'z')
		{
			name[i+1] -= 0x20;
		    ext[i] = name[i+1];
		}
	};

	//Acrescentamos ' ' até completarmos as oito letras do nome.
	
    while (ns < 8)
	{	
        *name++ = ' ';
        ns++;
    };

	//Acrescentamos a extensão
	
    for (i=0; i < 3; i++)
        *name++ = ext[i];

    *name = '\0';
}



/*
 ************************************************
 * fclose:
 *     Close a file.
 *     If work, return 0. */

int fclose (FILE *stream){
 
    if ( (void *) stream == NULL )
        return EOF;
 
    return (int) gramado_system_call ( 232, 
                     (unsigned long) stream, 
                     (unsigned long) stream, 
                     (unsigned long) stream ); 
}


/*
 **************************************************************
 * fopen:
 *     Open a file.
 *     @todo: Abrir onde? saída padrão?
 *     @retorna o que? o endereço do buffer?
 *     Obs: Tem função na api que carrega um arquivo em um buffer.
 *
 * #obs: Deveríamos checar o tamanho do arquivo antes de criar o buffer.
 *
 * #todo: E se o ponteiro para o nome do arquivo for inválido? tem que  
 * falhar e retornar null.
 */

FILE *fopen ( const char *filename, const char *mode ){


    return (FILE *) gramado_system_call ( 246, 
                        (unsigned long) filename, 
                        (unsigned long) mode, 
                        (unsigned long) mode ); 
}



/*
 * scroll:
 *     Scroll the screen in (text mode).
 *     @todo: Mudar para tmScroll(void);
 *     @todo: Criar gmScroll(void) talvez;
 *     @todo: Essa rotina precisa ser revisada e aprimorada.
 */
void scroll (void){
	
	//Início da tela.
	unsigned short *p1 = (unsigned short *) SCREEN_START;
	
	//Início da segunda linha.
    unsigned short *p2 = (unsigned short *) (SCREEN_START + 2 * SCREEN_WIDTH);
    unsigned short i,j;



	//linhas.
	//usa o valor default se passar dos limites.
	if ( g_rows == 0 || g_rows >= SCREEN_MAX_HEIGHT )
	{
		g_rows = SCREEN_HEIGHT;
	}

	//24 vezes.
    for( i=0; i < g_rows -1; i++ )
	{
	    //80 vezes.
		
		//Se a variável nao foi inicializada, usar valor default.
		if(g_columns == 0 || g_columns >= SCREEN_MAX_WIDTH){ 
		    g_columns = SCREEN_WIDTH;
		};
		
        
		//modo texto
	    //if(g_using_gui == 0)
		//{	
		//    for( j=0; j < g_columns; j++ ){
        //        *p1++ = *p2++;
		//    };
		//};
		
		//modo gráfico.
	    if(g_using_gui == 1)	
		{	
		    for( j=0; j < g_columns; j++ )
			{
                //*p1++ = *p2++;
		        //@todo:
			};
		};
		
	};
	
	
	//
	// Limpando a última linha.
	//
	
	//80 vezes.
    
	//modo texto
	//if(g_using_gui == 0){
	//    for( i=0; i < g_columns; i++ ){ *p1++ = 0x07*256 + ' '; };
	//}
	
	//modo gráfico.
	if(g_using_gui == 1)
	{
	    for( i=0; i < g_columns; i++ )
		{ 
	        //*p1++ = 0x07*256 + ' ';
			//@todo:	
		};    
	};
		


    return;
}



/*
 ***********************
 * puts:
 */

int puts ( const char *str ){
	
	//provisório ...
	return (int) printf ("%s",str);
}


//#todo
size_t fread(void *ptr, size_t size, size_t n, FILE *fp)
{
    return -1;
}


size_t fwrite(const void *ptr, size_t size, size_t n, FILE *fp)
{
	return -1;
}







/*
 ****************************************************************
 *    Construindo a função printf;
 *****************************************************************
 */


/*
 **********************************************************************
 * prints:
 *
 */
static int prints ( char **out, const char *string, int width, int pad ){
	
    register int pc = 0, padchar = ' ';

    
	// filtro: e se width for menor que 0??
	
	//if(width < 0){
	//	return (int) 1;
	//}
	
	if ( width > 0 ) 
	{
	    register int len = 0;
		register const char *ptr;
		
		for(ptr = string; *ptr; ++ptr) ++len;
		if(len >= width) width = 0;
		else width -= len;
		if(pad & PAD_ZERO) padchar = '0';
	};
	
	if ( !(pad & PAD_RIGHT) ) 
	{
		for ( ; width > 0; --width ){
		    printchar ( out,padchar );
			++pc;
		};
	};
	
	for ( ; *string ; ++string ){
		printchar(out, *string);
		++pc;
	};
	
	for ( ; width > 0; --width ){
		printchar(out,padchar);
		++pc;
	};

	return pc;
};


/*
 ***************************************
 * printi:
 *     used by printf.
 */
static int printi ( char **out, 
                    int i, 
					int b, 
					int sg, 
					int width, 
					int pad, 
					int letbase )
{
	register char *s;
	register int t, neg = 0, pc = 0;
	register unsigned int u = i;
	
	char print_buf[PRINT_BUF_LEN];
	
	if (i == 0){
		print_buf[0] = '0';
		print_buf[1] = '\0';		
		return prints (out, print_buf, width, pad);
	};
	
	if ( sg && b == 10 && i < 0) 
	{
		neg = 1;
		u = -i;
	};

	s = print_buf + PRINT_BUF_LEN-1;
	*s = '\0';

	while (u) 
	{
		t = u % b;
		
		if (t >= 10)
		    t += letbase - '0' - 10;
		    *--s = t + '0';
		    u /= b;
	};

	if (neg) 
	{
		if ( width && (pad & PAD_ZERO) ) 
		{
		    printchar (out, '-');
			++pc;
			--width;
		
		}else{
			*--s = '-';
		};
	};
	
    //Nothing.
//done:

	return pc + prints(out, s, width, pad);
};


/*
 ************************************************
 * print:
 *     Used by printf.
 */
static int print ( char **out, int *varg ){
	
	register int width, pad;
	register int pc = 0;
	register char *format = (char *) (*varg++);
	char scr[2];

	for ( ; *format != 0; format++ ) 
	{
		//switch.
		if ( *format == '%' ) 
		{
			++format;
			width = pad = 0;
			
			if ( *format == '\0' ) 
			    break;
			
			if ( *format == '%' ) 
			    goto out;
			
			if ( *format == '-' ){
				++format;
				pad = PAD_RIGHT;
			};
			
			while ( *format == '0' ){
				++format;
				pad |= PAD_ZERO;
			};
			
			for ( ; *format >= '0' && *format <= '9'; ++format ){
				width *= 10;
				width += *format - '0';
			};
			
			//string
			if ( *format == 's' ){
				
				register char *s = *( (char **) varg++ );
				pc += prints ( out, s?s:"(null)", width, pad);
				continue;
			}
			
			//int
			if ( *format == 'd' ){
				pc += printi ( out, *varg++, 10, 1, width, pad, 'a');
				continue;
			}
			
			//hexa
			if ( *format == 'x' ){
				pc += printi ( out, *varg++, 16, 0, width, pad, 'a');
				continue;
			}
			
			if ( *format == 'X' ){
				pc += printi ( out, *varg++, 16, 0, width, pad, 'A');
				continue;
			}
			
			if ( *format == 'u' ){
				pc += printi ( out, *varg++, 10, 0, width, pad, 'a');
				continue;
			}
			
			//char
			//char are converted to int then pushed on the stack. 
			if ( *format == 'c' ){
				
				scr[0] = *varg++;
				scr[1] = '\0';
				pc += prints ( out, scr, width, pad);
				continue;
			}
			
			//Nothing.
		
		}else{
			
		    out:
			    printchar ( out, *format );
			    ++pc;
		};
		//Nothing.
	};
	
	if (out) 
	    **out = '\0';


    return (int) pc;
}



/* 
 ***********************************************************************
 * printf:
 *     Imprime uma string formatada.
 *     Assuming sizeof(void *) == sizeof(int).
 * Obs: Temos uma chamada ao sistema para cada caractere.
 * 
 * Obs: Se passássemos apenas o ponteiro da string para o kernel pintá-la,
 * não estariamos usando o printf da biblioteca em user mode e sim 
 * a printf do kernel.
 */
 
//padrão não tradicional, mas funciona. 
//int printf ( const char *format, ... ){	
int printf3 ( const char *format, ... ){	
    
	register int *varg = (int *)(&format);
	
	return (int) print ( 0, varg );
};


//
//=============================================================
//
//usada na printf2
void printf_atoi (int value, char* valuestring){
    
  int min_flag;
  char swap, *p;
  min_flag = 0;

  if (0 > value)
  {
    *valuestring++ = '-';
    value = -INT_MAX> value ? min_flag = INT_MAX : -value;
  }

  p = valuestring;

  do
  {
    *p++ = (char)(value % 10) + '0';
    value /= 10;
  } while (value);

  if (min_flag != 0)
  {
    ++*valuestring;
  }
  *p-- = '\0';

  while (p > valuestring)
  {
    swap = *valuestring;
    *valuestring++ = *p;
    *p-- = swap;
  }
}

//usada na printf2
void printf_i2hex (uint32_t val, char* dest, int len)
{
	char* cp;
	char x;
	
	uint32_t n;
	n = val;
	
	cp = &dest[len];
	
	while (cp > dest)
	{
		x = n & 0xF;
		n >>= 4;
		*--cp = x + ((x > 9) ? 'A' - 10 : '0');
	}
	
    dest[len+1]='\0';
}



//#test 
//tentando implementar a printf do nelson cole.
//padrão tradicional, incompleta, não funciona ainda,
//estamo implementando

int printf2 ( const char *format, ...)
{

    char *ap;
    va_start (ap,format);


	int index = 0;
	uint8_t u;	
	int d;
	char c, *s;
	
	char buffer[256];

	while ( format[index] )
	{
		switch (format[index])
		{
		    case '%':
			    ++index;
			    switch (format[index])
			    {
			
                    case 'C':
			        case 'c':
				        c = (int8_t) va_arg (ap, int32_t);
				        putchar(c);
				        break;
     
                    case 'S':
			        case 's':
				        s = va_arg (ap, int8_t*);
				        //puts(s);
				        printf2(s);
						break;

			        case 'd':
			        case 'i':
				        d = va_arg (ap, int32_t);
				        //atoi(d, buffer);
				        printf_atoi(d, buffer);
						//puts(buffer);
						printf2(buffer);
				        break;

			        case 'U':
			        case 'u':
				        u = va_arg (ap, uint32_t);
				        //atoi(u, buffer);
				        printf_atoi(u, buffer);
						//puts(buffer);
				        printf2(buffer);
						break;

			        case 'X':
			        case 'x':
				        d = va_arg (ap, int32_t);
				        //i2hex(d, buffer,8);
						//printf_i2hex(d, buffer,8);
				        //puts(buffer);
				        printf("%x",d);
						break;
			
			        default:
				        putchar('%');
				        putchar('%');
				        break;
				
			    }
			    break;

		    default:
			    putchar ( format[index] );
			    break;
		};
		
		++index;
    };


    return 0;
}


//
//=============================================================
//


/*
 *****************************************************************
 * sprintf:
 *     Emprime uma string formatada em 'out'.
 *     Escreve em um buffer de caracteres.
 * @todo: ??
 * Em caso de sucesso, a função retorna o número de caracteres 
 * da string formatada. Em caso de erro, um valor negativo é retornado.
 */
 
//opção 
//int sprintf(char *str, const char *format, ...) ?? 

int sprintf ( char *out, const char *format, ... ){
	
    register int *varg = (int *)(&format);


    return (int) print( &out, varg );
}


static void printchar ( char **str, int c ){

    if (str) 
    {
		**str = c;
		
		++(*str);


    }else (void) putchar(c);
}



/*
 ***********************************************
 * putchar:
 *     Put a char in the screen.
 *     Obs: Isso funciona bem. 
 *
 *     #importante: O kernel pinta o char no backbuffer e
 *     em seguida efetua um refresh do char para mostra-lo na tela.
 *     #bugbug: Não precisamos desse retorno. Pode ser void ? libc?
 */
 
int putchar (int ch){
    
	// Opções:
	// +Chamar a função 'putchar' oferecida pelo kernel 
	// e utilizar o cursor gerenciado pelo kernel.
	// +chamar a função 'outbyte' com cursor gerenciado em user mode.
	
	// *importante:
	// Queremos que o kernel gerencie as mensagens de digitação,
	// então devemos usar a opção que permite o kernel usar seu próprio cursor.
	
    
	//#atenção
	//Estamos deixando o kernel gerenciar as mensagens de digitação 
	//usando seu próprio cursor.
    //put char.
	
	//stdio_system_call ( 65, (unsigned long) ch, (unsigned long) ch, 
	//	(unsigned long) ch );
	
	//gramado_system_call ( 65, (unsigned long) ch, (unsigned long) ch, 
		//(unsigned long) ch );

    //#todo: IN: ch, virtual,console id.
    gramado_system_call ( 65, 
        (unsigned long) ch,   // ch
        (unsigned long) 0,    // console id. 0.
        (unsigned long) 0 );


    return (int) ch;    
}


/*
 **********
 * outbyte:
 * @todo: Colocar no buffer de arquivo.
 * #obs: essa função chamará _outbyte.
 *
 * #importante: Não me lebro se o kernel efetua o refresh do char 
 * nesse caso.
 */
 
void outbyte (int c){
	
    static char prev = 0;
        
	// spaces.	
    if( c <  ' ' && 
	    c != '\r' && 
		c != '\n' && 
		c != '\t' && 
		c != '\b' )
	{
        return;
    };
	
	//#testando.
	//nada para imprimir.
	//?? onde fica o tratamento de backspace.??
	//if( c == '\b' )
    //{
	//	return;
	//}		
	
	// carriege return 
	// Volta ao início da linha.
	if ( c == '\r' )
	{	
        g_cursor_x = 0;
        prev = c;
        return;    
    };        
      
	//Próxima linha e não início da linha.   
    if ( c == '\n' && prev != '\r' ) 
    {
        g_cursor_y++;      
        g_cursor_x = 0;    
        prev = c;
        return;
    };
       
    //Próxima linha e início da linha. 	   
    if ( c == '\n' && prev == '\r' ) 
    {
        g_cursor_y++;  
        prev = c;
        return;
	};

    //Tab.
	//@todo: Criar a var -> 'g_tab_size'.
    if ( c == '\t' )  
    {
        g_cursor_x += (4);    
        prev = c;
        return;         
    };
        
    //Space 
	//#todo:
	// ?? talvez devêssemos imprimir o caractere espaço. ??    
	if ( c == ' ' )  
    {
        g_cursor_x++; 
        prev = c;
        return;         
    };
        
    //Delete. 
    if ( c == 8 )  
    {
        g_cursor_x--;         
        prev = c;
        return;         
    };
        
    // Filtra as dimensões da janela onde esta pintando.
//checkLimits:	
	
	//
	// Colunas.
	//
	
	//Definindo a largura que usaremos.
	//A largura precisa ser maior que '0' e menor que o limite máximo.
	//Obs: @todo: Essa rotina de definição pode ir para o momento da inicialização
	//da biblioteca. Ela contunua aqui porque está funcionando como um filtro.
	//
    if ( g_columns == 0 || g_columns >= SCREEN_MAX_WIDTH ){
		g_columns = COLUMNS;
	}
	
	//O cursor não pode ultrapassar a largura definida.
	if ( g_cursor_x > g_columns )
	{
        g_cursor_x = 0;
        g_cursor_y++;  
    }else{
		//Se não alcançamos o limite, apenas incrementa o x.
        g_cursor_x++;    
    };
    
	// Linhas.
	
	//Definindo a altura que usaremos.
	//A altura precisa ser maior que '0' e menor que o limite máximo.
	if ( g_rows == 0 || g_rows >= SCREEN_MAX_HEIGHT ){
		g_rows = ROWS;
	}
	
    //O cursor não pode ultrapassar a altura definida.	
	//se ultrapassar, chamaremos o scroll.
	//Obs: O scroll ainda não está implementado.
	//O scroll será feito depois que implementarmos o array de ponteiros
	//para estruturas de linha.
	
	if ( g_cursor_y > g_rows )
	{ 
	    scroll();
        g_cursor_x = 0;             //O cursor deve fica na primeira coluna.
		g_cursor_y = (g_rows-1);    //O cursor deve ficar na última linha.
    };

    // Imprime os caracteres normais.

	_outbyte (c);

	//Atualisa o prev.
    prev = c;     	
};


/*
 ***********
 * _outbyte:
 *     Just output a byte on the screen.
 *
 *     Obs: A função não se preocupa com o cursor.
 *          Essa rotina está preparada somente par ao modo gráfico.
 *          Talvez usaremos um selecionador de modo.   
 *
 * #obs: 
 * #importante: Não me lebro se o kernel efetua o refresh do char 
 * nesse caso.
 */
  
void _outbyte ( int c ){
	
	//#obs: Tamanho do char constante = 8. 
	//o que queremos é usar uma variável.
	
	//#bugbug
	//essa função não é usada ... NAO funciona.
	//printf usa outra coisa (65).
	


    gramado_system_call ( 7, 
        8*g_cursor_x,  8*g_cursor_y, 
        (unsigned long) c ); 
}



/*
 ************************************************************
 * input:
 *     Coloca os caracteres digitados em uma string. 'prompt[]'
 *     #bugbug: 
 *     Deveríamos considerar o posicionamento dentro do arquivo.
 *     Dentro da stream.
 */

unsigned long input ( unsigned long ch ){
	
	//save cursor position.
	unsigned long tmpX, tmpY;
	
	// Convert.
	char c = (char) ch;    
	
	// Ajust prompt max.
	if ( prompt_max == 0 || prompt_max >= PROMPT_MAX_DEFAULT )
	{
		prompt_max = PROMPT_MAX_DEFAULT;
	}
	
	//Filtra limite.
	//retornar 1??
	if ( prompt_pos > prompt_max ){
		
	    printf ("input: Full buffer!\n");	
	    return (unsigned long) 0;   
	};
 
	// Trata caractere digitado.
	switch (c)
	{
	    //Enter.	
		case VK_RETURN:
		    prompt[prompt_pos] = (char ) '\0';
            goto input_done;			
		    break;

	    // Obs: O tab avança o cursor, mas precisamos 
		// pintar o espaço onde estava o cursor.
		// Por isso talvez seja bom um while de espaços.
		// Mas no buffer fica apenas o '\t'.
		case VK_TAB:
		    prompt[prompt_pos] = (char ) '\t';
			//for( i=0; i<4; i++)
			//{ printf("%c", (char) ' '); }
		    goto input_done;
			break;
			
		case VK_BACKSPACE:
            
			//Se estamos no início da linha.
			if(prompt_pos <= 0)
			{
                prompt_pos = 0; 				
			    prompt[prompt_pos] = (char ) '\0';
				break; 
			};
		    
			//altera o buffer.
			//Apaga o anterior.
			prompt_pos--;                        //volta um no buffer.
			prompt[prompt_pos] = (char ) '\0';   //apaga no buffer.
			
			//altera a tela no modo gráfico com janelas.
			tmpX = stdioGetCursorX(); 
			tmpY = stdioGetCursorY();
			
			//Volta um na tela.
			tmpX--;
			stdioSetCursor(tmpX,tmpY);
			
			//apaga a tela.
			printf ("%c", (char) ' '); 
			
			//altera a tela no modo gráfico com janelas.
			tmpX = stdioGetCursorX(); 
			tmpY = stdioGetCursorY();
			
			//Volta um na tela.
			tmpX--;
			stdioSetCursor(tmpX,tmpY);
			break;
			
		//Continua ...
		
		// Vamos deixar o sistema mostrar o cursor.
		// Coloca carctere normal no buffer.
		// Incrementa posicionamento no buffer.
		default:
		    prompt[prompt_pos] = c;
		    prompt_pos++;
			break;
			
	};
	
//input_more:	
	return (unsigned long) 0;


input_done:
    return VK_RETURN;
}



/*
// Return BUFFER_SIZE. 
int input_file_buffer_size(void);
int input_file_buffer_size(void)
{
  return (BUFFER_SIZE);
}
*/



/*
 *************************************
 * getchar:
 *
 * getchar:
 * pegando diretamente do kernel e não do arquivo.
 * me parece que o tradicional é pegar do arquivo,
 * mas isso acontece em outras situações.
 * @todo:
 * Podemos pegar esse char e colocá-lo stdin 
 * slave (local) de input. 
 */

int getchar (void){


    return (int) gramado_system_call ( 137, 0, 0, 0 ); 
}



/*
 **************************************************
 * stdioInitialize:
 *     Inicializa stdio para usar o fluxo padrão.
 *     O retorno deve ser (int) e falhar caso dê algo errado.
 */
 
 
// Essa libc vai ser usada pelo processo init. 
// Não podemos receber do kernel um arquivo uma stream
// que esteja em ring0. 
// #todo: 
// Então precisamos refazer o fluxo padrão
// para oferecermos ao processo filho streams em ring3.
// O kernel continua podendo ler elas.
 
void stdioInitialize ()
{
    // #obs:
    // malloc já está inicializado.


    // Buffers para as estruturas.
    // ring3.
    unsigned char buffer0[BUFSIZ];
    unsigned char buffer1[BUFSIZ];
    unsigned char buffer2[BUFSIZ];


    unsigned char buffer0_data[BUFSIZ];
    unsigned char buffer1_data[BUFSIZ];
    unsigned char buffer2_data[BUFSIZ];


    int status = 0;
    int i;
    
            
    //FILE *_fp;




	//Os caracteres são pintados na tela.
	//__libc_output_mode = LIBC_DRAW_MODE;

	//Os caracteres são colocados em stdout.
    //__libc_output_mode = LIBC_NORMAL_MODE;


	// Alocando espaço para as estruturas.
	// Mas não usaremos a estrutura em ring3, somente o ponteiro.
	// O ponteiro apontará para um estrutura em ring0.

	stdin  = (FILE *) &buffer0[0];
	stdout = (FILE *) &buffer1[0];
	stderr = (FILE *) &buffer2[0];
	
	
	stdin->_base    = &buffer0_data[0];
	stdout->_base   = &buffer1_data[0];
	stderr->_base   = &buffer2_data[0];

    stdin->_ptr  = stdin->_base;
    stdout->_ptr = stdout->_base;
    stderr->_ptr = stderr->_base;
    
    
    stdin->_cnt  = BUFSIZ;
    stdout->_cnt = BUFSIZ;
    stderr->_cnt = BUFSIZ;

    
    // pegando s stream na lista de arquivos do processo.
    // O ponteiro apontará para um estrutura em ring0.
    // service, pid, fd, 0.
    
    // #bugbug
    // Isso é ruim no caso do processo init,
    // pois os processo filhos herdariam estruturas 
    // que estão em ring0.
    
    // Estruturas em ring3. O kernel não saberá disso.
    // então tem que inicializar os elementos.
    //stdin  = (FILE *) gramado_system_call ( 167, getpid(), 0, 0 );
    //stdout = (FILE *) gramado_system_call ( 167, getpid(), 1, 0 );
    //stderr = (FILE *) gramado_system_call ( 167, getpid(), 2, 0 );
}




/* 
 ***********************************
 * fflush: 
 * Salva o buffer no arquivo associado a ele.
 * e limpa o buffer. 
 * Se for NULL então faz isso para todas stream abertas.
 * retorna 0 se funcionar e retorna EOF se falhar.
 */

int fflush ( FILE *stream ){

    return (int) gramado_system_call ( 233, 
                     (unsigned long) stream, 
                     (unsigned long) stream, 
                     (unsigned long) stream ); 
}


/*
 ********************************
 * fprintf:     
 */

int fprintf ( FILE *stream, const char *format, ... ){

    if ( (void *) stream == NULL )
        return EOF;

    return (int) gramado_system_call ( 234, 
                     (unsigned long) stream, 
                     (unsigned long) format, 
                     (unsigned long) format ); 
}


/*
 ********************************
 * fputs:      
 */
int fputs ( const char *str, FILE *stream ){

    if ( (void *) stream == NULL )
        return EOF;

    return (int) gramado_system_call ( 235, 
                     (unsigned long) str, 
                     (unsigned long) stream, 
                     (unsigned long) stream ); 
}


/*
 *********************************
 * gets:
 *
 * obs: 
 * gets() devolve um ponteiro para string
 */
 
char *gets (char *s){
	
    int ch;
	
    int t = 0;
	char *p;
		
	//printf("gets:\n");
	
    //salva
	p = s; 	
	
	while (1)
	{
        ch = (int) getchar ();
		
        if ( ch != -1 )
		{
            			
			switch (ch) 
		    {
				/* termina a string */
			    case '\n':
				case VK_RETURN: 
                    s[t] =  (char) '\0'; 
                    goto done;
				    break;
            
                case '\b':
				case VK_BACKSPACE:
			        if(t > 0){ 
					    t--;
					};
                    break;
            
			    default:
                    //s[t] = (char) ch;
					//t++;
					break;
            };
			
			printf("%c",ch);
			s[t] = (char) ch;
			t++;			
			
		};
		
		asm ("pause");
    };


done:	
    //s[t] = (char) '\0';
    return (char *) p;
}


/*
 *********************************
 * ungetc:
 */

// #todo

int ungetc ( int c, FILE *stream ){

    if ( (void *) stream == NULL )
        return EOF;


    return EOF;
}


// #todo
long ftell (FILE *stream)
{
    return (long) -1;	
}


// #todo
int fileno ( FILE *stream ){

    if ( (void *) stream == NULL )
        return EOF;

    return EOF;
}


/*
 *********************************
 * fgetc:
 *     #todo: chamar a rotina do kernel para pegar o char no arquivo,
 * pois agora o arquivo 'e gerenciado pelo kernel.
 */

int fgetc ( FILE *stream ){

    if ( (void *) stream == NULL )
        return EOF;

    return (int) gramado_system_call ( 136, 
                     (unsigned long) stream,  
                     (unsigned long) stream,  
                     (unsigned long) stream );
}


/*
 *********************************
 * feof:
 */
int feof ( FILE *stream ){

    if ( (void *) stream == NULL )
        return EOF;
 
    return (int) gramado_system_call ( 193, 
                    (unsigned long) stream,  
                    (unsigned long) stream,  
                    (unsigned long) stream );
}


/*
 *********************************
 * ferror:
 *
 */

int ferror ( FILE *stream ){

    if ( (void *) stream == NULL )
        return EOF;

    return (int) gramado_system_call ( 194, 
                     (unsigned long) stream,  
                     (unsigned long) stream,  
                     (unsigned long) stream );    
}


/*
 **************************************
 * fseek:
 *     offset argument is the position that you want to seek to,
 *     and whence is what that offset is relative to.
 */

int fseek ( FILE *stream, long offset, int whence ){
    
    if ( (void *) stream == NULL )
        return EOF;

    return (int) gramado_system_call ( 195, 
                     (unsigned long) stream, 
                     (unsigned long) offset, 
                     (unsigned long) whence ); 
}


/*
 *****************************************
 * fputc:
 */

int fputc ( int ch, FILE *stream ){

    if ( (void *) stream == NULL )
        return EOF;
 
    return (int) gramado_system_call ( 196, 
                     (unsigned long) ch,  
                     (unsigned long) stream, 
                     (unsigned long) stream );    
}


/*
 ********************************
 * stdioSetCursor:
 *     estamos falando do posicionamento do cursor dentro da janela
 *     e não dentro do terminal.
 */

void 
stdioSetCursor ( unsigned long x, unsigned long y )
{
    gramado_system_call ( 34, x, y, 0 );
}


/*
 * stdioGetCursorX:
 *     Get cursor x. 
 *     estamos falando do posicionamento do cursor dentro da janela
 *     e não dentro do terminal.
 */  
unsigned long stdioGetCursorX (){

    return (unsigned long) gramado_system_call ( 240, 0, 0, 0 );
}


/*
 * stdioGetCursorY:
 *     Get cursor y.
 *     estamos falando do posicionamento do cursor dentro da janela
 *     e não dentro do terminal. 
 */
unsigned long stdioGetCursorY (){

    return (unsigned long) gramado_system_call ( 241, 0, 0, 0 );
}



//======================================================================
// scanf support (start)
//======================================================================



int scanf ( const char *fmt, ... ){
	
    va_list ap;
	va_start(ap, fmt);
	
    int d;
    char *s;

	//%c
    int ch;
	
	//usado em %d
	int *i;
	char tmp[80];
	
	
	//o char.
	int c;
	
	char *cp;
	
	char *t;

	int nread;
	int size = (int) stdio_strlen (fmt);
	
	nread = 0;
	

    //if ( (void *) fmt == NULL )
        //return -1;

    while (*fmt)
    {
		c = *fmt;
		if (c == 0)
			return (0); //erro
		
		// pulando os espaços
		if ( isspace(c) ) 
		{
			while ( size > 0 && isspace(*fmt) )
			{
				nread++; 
				size--; 
				fmt++;
			};
			
			continue;
		}
		
        switch (*fmt++) 
		{
			/* string */
            case 's':              
				//pego o ponteiro para  a string, depois é só usar o gets. ??
				s = va_arg(ap, char *);
				
				//usaremos esse ponteiro para colocar uma string digitada.
				if ( (void *) s != NULL )
				{
				    gets(s);					
				}else{
				    printf("scanf s null pointer\n");	
				}
				
				//testar ...
				//printf("string %s\n", s);
                break;
            
			/* int */
			case 'd': 
				//pego o ponteiro para  a string, depois é só usar o gets. ??
				i = va_arg(ap, int *);
				
				//usaremos esse ponteiro para colocar uma string digitada.
				if( (void *) i != NULL )
				{
				    //pego uma string de caracteres, que são números digitados.
					gets(tmp);
					
					//converte essa string em dígito
					i[0] = (int) stdio_atoi (tmp);                     
					
				}else{
				    printf("scanf s null pointer\n");	
				}
				
				//testar ...
				//printf("string %s\n", s);
			
                //d = va_arg(ap, int);
                //printf("int %d\n", d);
                break;
            
			/* char */
			case 'c':    
                t = va_arg(ap, char *);
				if ( (void *) t != NULL )
				{
				
                    while (1)
                    {
			            ch = (int) getchar();
						
						if ( ch != -1 )
						{
						    t[0] = ch;
                            //printf("scanf ch={%c}",ch);
						    break;	
						}			
					};	
					
				}else{
					printf("scanf c null pointer\n");
				}; 				 
                break;
				
			//default:
            //    break; 			
				
        };//switch
    };
   // va_end(ap);


    return 0;
}
//======================================================================
// scanf support (end)
//======================================================================



/*
 * sscanf: 
 * Scan items from a string in accordance with a format.  This is much
 * simpler than the C standard function: it only recognises %d without a
 * field width, and does not treat space in the format string or the
 * input any differently from other characters.  The return value is the
 * number of characters from the input string that were successfully
 * scanned, not the number of format items matched as in standard sscanf.
 * e.mcmanus@opengroup.org, 12 Feb 97
 * Credits: apple open source.
 */

int sscanf ( const char *str, const char *format, ... ){
	
	const char *start = str;
	va_list args;
	
	va_start(args, format);
	for ( ; *format != '\0'; format++) {
		if (*format == '%' && format[1] == 'd') {
			int positive;
			int value;
			int *valp;
			
			if (*str == '-') {
				positive = 0;
				str++;
			} else
				positive = 1;
			if (!isdigit(*str))
				break;
			value = 0;
			do {
				value = (value * 10) - (*str - '0');
				str++;
			} while (isdigit(*str));
			if (positive)
				value = -value;
			valp = va_arg(args, int *);
			*valp = value;
			format++;
		} else if (*format == *str) {
			str++;
		} else
			break;
	}
	va_end(args);


    return str - start;
}





//=============================================================
// printf (start)
//=============================================================
/*-
 * Copyright (c) 1986, 1988, 1991, 1993
 *	The Regents of the University of California.  All rights reserved.
 * (c) UNIX System Laboratories, Inc.
 * All or some portions of this file are derived from material licensed
 * to the University of California by American Telephone and Telegraph
 * Co. or Unix System Laboratories, Inc. and are reproduced herein with
 * the permission of UNIX System Laboratories, Inc.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 4. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 *	@(#)subr_prf.c	8.3 (Berkeley) 1/21/94
 */

//typedef unsigned long size_t;
typedef long ssize_t;
//#ifdef __64BIT__
//typedef unsigned long long uintmax_t;
//typedef long long intmax_t;
//#else
typedef unsigned int uintmax_t;
typedef int intmax_t;
//#endif
typedef unsigned char u_char;
typedef unsigned int u_int;
typedef unsigned long u_long;
typedef unsigned short u_short;
typedef unsigned long long u_quad_t;
typedef long long quad_t;
typedef unsigned long uintptr_t;
//typedef long ptrdiff_t;
//#define NULL ((void*)0)
#define NBBY    8               /* number of bits in a byte */
char const hex2ascii_data[] = "0123456789abcdefghijklmnopqrstuvwxyz";
#define hex2ascii(hex)  (hex2ascii_data[hex])
//#define va_list __builtin_va_list
//#define va_start __builtin_va_start
//#define va_arg __builtin_va_arg
//#define va_end __builtin_va_end

//#obs:
//vamos usar o que está em ctype.h
//#define toupper(c)  ((c) - 0x20 * (((c) >= 'a') && ((c) <= 'z')))

static size_t stdio_strlen (const char *s){
	
	size_t l = 0;
	while (*s++)
		l++;
    return l;
}


/* 
 Max number conversion buffer length: 
 a u_quad_t in base 2, plus NUL byte. 
 */

#define  MAXNBUF  ( sizeof(intmax_t) * NBBY + 1 )


/*
 * ksprintn:
 * Put a NUL-terminated ASCII number (base <= 36) in a buffer in reverse
 * order; return an optional length and a pointer to the last character
 * written in the buffer (i.e., the first character of the string).
 * The buffer pointed to by `nbuf' must have length >= MAXNBUF.
 */
 
static char *ksprintn ( char *nbuf, 
                        uintmax_t num, 
                        int base, 
                        int *lenp, 
                        int upper )
{

    char *p, c;

    p = nbuf;

	//*p = ' ';
    *p = 0;


    do {
		
		c = hex2ascii (num % base);
		
		*++p = upper ? toupper(c) : c;
		
    } while (num /= base);


	if (lenp)
		*lenp = p - nbuf;
    return (p);
}



/*
 * Scaled down version of printf(3).
 *
 * Two additional formats:
 *     The format %b is supported to decode error registers.
 *     Its usage is:
 *
 *	printf("reg=%bn", regval, "*");
 *
 * where  is the output base expressed as a control character, e.g.
 * 10 gives octal; 20 gives hex.  Each arg is a sequence of characters,
 * the first of which gives the bit number to be inspected (origin 1), and
 * the next characters (up to a control character, i.e. a character <= 32),
 * give the name of the register.  Thus:
 *
 *	kvprintf("reg=%bn", 3, "102BITTWO1BITONEn");
 *
 * would produce output:
 *
 *	reg=3
 *
 * XXX:  %D  -- Hexdump, takes pointer and separator string:
 *		("%6D", ptr, ":")   -> XX:XX:XX:XX:XX:XX
 *		("%*D", len, ptr, " " -> XX XX XX XX ...
 */
 
int 
kvprintf ( char const *fmt, 
           void (*func)( int, void* ), 
           void *arg, 
           int radix, 
           va_list ap )
{
    
	//#define PCHAR(c) { int cc=(c); if(func) (*func)(cc,arg); else *d++ = cc; retval++; }
    #define PCHAR(c) { int cc=(c); if(func) (*func)(cc,arg); else *d++ = cc; retval++; }
	
    char nbuf[MAXNBUF];
	char *d;
	const char *p, *percent, *q;
	u_char *up;
	
	int ch, n;
	uintmax_t num;
	int base, lflag, qflag, tmp, width, ladjust, sharpflag, neg, sign, dot;
	int cflag, hflag, jflag, tflag, zflag;
	int dwidth, upper;
	char padc;
	int stop = 0, retval = 0;

	num = 0;
	
	if (!func)
		d = (char *) arg;
	else
		d = NULL;

	
	if (fmt == NULL)
		fmt = "(fmt null)n";

	
	if (radix < 2 || radix > 36)
		radix = 10;

	for (;;) {
		
		padc = ' ';
		width = 0;
		
		while ( ( ch = (u_char) *fmt++ ) != '%' || stop ) 
		{
			//if (ch == '')
			//	return (retval);
			
			if (ch == 0)
			    return (retval);
			
			PCHAR (ch);
		}
		
		percent = fmt - 1;
		
		qflag = 0; 
		lflag = 0; 
		ladjust = 0; 
		sharpflag = 0; 
		neg = 0;
		
		sign = 0; 
		dot = 0; 
		dwidth = 0; 
		upper = 0;
		
		cflag = 0; 
		hflag = 0; 
		jflag = 0; 
		tflag = 0; 
		zflag = 0;
		
        reswitch:  
    
	    switch ( ch = (u_char) *fmt++ ){
			
		case '.':
			dot = 1;
			goto reswitch;
			
		case '#':
			sharpflag = 1;
			goto reswitch;
			
		case '+':
			sign = 1;
			goto reswitch;
			
		case '-':
			ladjust = 1;
			goto reswitch;
			
		case '%':
			PCHAR(ch);
			break;
			
		case '*':
			if (!dot) 
			{
				width = va_arg(ap, int);
				
				if (width < 0)
				{
					ladjust = !ladjust;
					width = -width;
				};
				
			} else {
				
				dwidth = va_arg(ap, int);
			};
			goto reswitch;
			
		case '0':
			if (!dot) 
			{
				padc = '0';
				goto reswitch;
			}
			
		case '1': 
		case '2': 
		case '3': 
		case '4':
		case '5': 
		case '6': 
		case '7': 
		case '8': 
		case '9':
			for ( n = 0;; ++fmt ) 
			{
				n = n * 10 + ch - '0';
				ch = *fmt;
				
				if (ch < '0' || ch > '9')
					break;
			}
			
			if (dot)
				dwidth = n;
			else
				width = n;
			goto reswitch;
			
		case 'b':
			num = (u_int)va_arg(ap, int);
			p = va_arg(ap, char *);
			
			for (q = ksprintn(nbuf, num, *p++, NULL, 0); *q;)
				PCHAR(*q--);

			if (num == 0)
				break;

			for (tmp = 0; *p;) 
			{
				n = *p++;
				
				if (num & (1 << (n - 1))) 
				{
					PCHAR(tmp ? ',' : '<');
					
					for (; (n = *p) > ' '; ++p)
						PCHAR(n);
					tmp = 1;
				} else
					for (; *p > ' '; ++p)
						continue;
			};
			
			if (tmp)
				PCHAR('>');
			break;
			
		case 'c':
			PCHAR(va_arg(ap, int));
			break;
			
		case 'D':
			up = va_arg(ap, u_char *);
			p = va_arg(ap, char *);
			
			if (!width)
				width = 16;
			while(width--) 
			{
				PCHAR(hex2ascii(*up >> 4));
				PCHAR(hex2ascii(*up & 0x0f));
				up++;
				
				if (width)
					for (q=p;*q;q++)
						PCHAR(*q);
			};
			break;
			
		case 'd':
		case 'i':
			base = 10;
			sign = 1;
			goto handle_sign;
			
		case 'h':
			if (hflag) 
			{
				hflag = 0;
				cflag = 1;
			} else
				hflag = 1;
			goto reswitch;
			
		case 'j':
			jflag = 1;
			goto reswitch;
			
		case 'l':
			if (lflag) 
			{
				lflag = 0;
				qflag = 1;
			} else
				lflag = 1;
			goto reswitch;
			
		case 'n':
			if (jflag)
				*(va_arg(ap, intmax_t *)) = retval;
			else if (qflag)
				*(va_arg(ap, quad_t *)) = retval;
			else if (lflag)
				*(va_arg(ap, long *)) = retval;
			else if (zflag)
				*(va_arg(ap, size_t *)) = retval;
			else if (hflag)
				*(va_arg(ap, short *)) = retval;
			else if (cflag)
				*(va_arg(ap, char *)) = retval;
			else
				*(va_arg(ap, int *)) = retval;
			break;
			
		case 'o':
			base = 8;
			goto handle_nosign;
			
		case 'p':
			base = 16;
			sharpflag = (width == 0);
			sign = 0;
			num = (uintptr_t)va_arg(ap, void *);
			goto number;
			
		case 'q':
			qflag = 1;
			goto reswitch;
			
		case 'r':
			base = radix;
			if (sign)
				goto handle_sign;
			goto handle_nosign;
			
		case 's':
			p = va_arg(ap, char *);
			if (p == NULL)
				p = "(null)";
			if (!dot)
				n = stdio_strlen (p);
			else
				for (n = 0; n < dwidth && p[n]; n++)
					continue;

			width -= n;

			if (!ladjust && width > 0)
				while (width--)
					PCHAR(padc);
			while (n--)
				PCHAR(*p++);
			if (ladjust && width > 0)
				while (width--)
					PCHAR(padc);
			break;
			
		case 't':
			tflag = 1;
			goto reswitch;
			
		// ?? case 'U': ??
		case 'u':
			base = 10;
			goto handle_nosign;
			
		case 'X':
			upper = 1;
		case 'x':
			base = 16;
			goto handle_nosign;
			
		case 'y':
			base = 16;
			sign = 1;
			goto handle_sign;
			
		case 'z':
			zflag = 1;
			goto reswitch;
			
        handle_nosign:
			sign = 0;
			if (jflag)
				num = va_arg(ap, uintmax_t);
			else if (qflag)
				num = va_arg(ap, u_quad_t);
			else if (tflag)
				num = va_arg(ap, ptrdiff_t);
			else if (lflag)
				num = va_arg(ap, u_long);
			else if (zflag)
				num = va_arg(ap, size_t);
			else if (hflag)
				num = (u_short)va_arg(ap, int);
			else if (cflag)
				num = (u_char)va_arg(ap, int);
			else
				num = va_arg(ap, u_int);
			goto number;
			
        handle_sign:
			if (jflag)
				num = va_arg(ap, intmax_t);
			else if (qflag)
				num = va_arg(ap, quad_t);
			else if (tflag)
				num = va_arg(ap, ptrdiff_t);
			else if (lflag)
				num = va_arg(ap, long);
			else if (zflag)
				num = va_arg(ap, ssize_t);
			else if (hflag)
				num = (short)va_arg(ap, int);
			else if (cflag)
				num = (char)va_arg(ap, int);
			else
				num = va_arg(ap, int);
			
        number:
			if (sign && (intmax_t)num < 0) {
				neg = 1;
				num = -(intmax_t)num;
			}
			p = ksprintn(nbuf, num, base, &tmp, upper);
			if (sharpflag && num != 0) {
				if (base == 8)
					tmp++;
				else if (base == 16)
					tmp += 2;
			}
			if (neg)
				tmp++;

			if (!ladjust && padc != '0' && width
			    && (width -= tmp) > 0)
				while (width--)
					PCHAR(padc);
			if (neg)
				PCHAR('-');
			if (sharpflag && num != 0) {
				if (base == 8) {
					PCHAR('0');
				} else if (base == 16) {
					PCHAR('0');
					PCHAR('x');
				}
			}
			if (!ladjust && width && (width -= tmp) > 0)
				while (width--)
					PCHAR(padc);

			while (*p)
				PCHAR(*p--);

			if (ladjust && width && (width -= tmp) > 0)
				while (width--)
					PCHAR(padc);

			break;
			
		default:
			
			while (percent < fmt)
				PCHAR(*percent++);
			
			/* Since we ignore an formatting argument it is no
			 * longer safe to obey the remaining formatting
			 * arguments as the arguments will no longer match
			 * the format specs. */
			 
			stop = 1;
			break;
		};
	};
#undef PCHAR
}


static void xxxputchar ( int c, void *arg ){

    /* add your putchar here */

    putchar ( (int) c );
}


/*
 *===========================================
 * printf:
 *     http://www.pagetable.com/?p=298
 */

	// #todo
	// Talvez usar semáforo aqui.

int printf ( const char *fmt, ... ){

    int __ret;


    va_list ap;
    va_start(ap, fmt);

	//int 
	//kvprintf ( char const *fmt, 
    //       void (*func)( int, void* ), 
	//	   void *arg, 
	//	   int radix, //??10 gives octal; 20 gives hex.
	//	   va_list ap );	
	
    __ret = (int) kvprintf ( fmt, xxxputchar, NULL, 10, ap );


    va_end(ap);


    return __ret;
}
   

//=============================================================
// printf end
//=============================================================



int 
vfprintf ( FILE *stream, 
           const char *format, 
           stdio_va_list argptr )
{

    int __ret;
    int size;


    if ( (void *) stream == NULL ){
        return EOF;
    } else {


		size = (int) stdio_strlen (format);
		
		//Se a string for maior que o tanto de bytes
		//disponíveis no arquivo
		if ( size > stream->_cnt )
		{
			return EOF;
		}
		
		//recalcula o tanto de bytes disponíveis
		stream->_cnt = (int) (stream->_cnt - size);
		
		//coloca no buffer, usando o ponteiro atual.
		//#obs: isso provavelmente funcione, 
		//mas vamos tentar o segundo método.
		//sprintf ( stream->_ptr, format );
		
        __ret = (int) kvprintf ( format, NULL, stream->_ptr, 10, argptr );
		
		//atualiza o ponteiro atual.
		stream->_ptr = stream->_ptr + size;

        return __ret;
    };


    return EOF;
}  



/* #bsd */
int vprintf (const char *fmt, va_list ap)
{
    if ( (void *) stdout == NULL )
        return EOF;


    return vfprintf (stdout, fmt, ap);
}


//printf que escreve no stdout. 
int stdout_printf (const char *format, ...){
	
    int __ret;

    va_list arg;
    va_start (arg, format);


    if ( (void *) stdout == NULL )
        return EOF;

    __ret = vfprintf (stdout, format, arg);

    va_end (arg);


    return __ret;
}


//printf que escreve no stderr. 
int stderr_printf (const char *format, ...)
{
    int __ret;
    
    va_list arg;
    va_start (arg, format);

    if ( (void *) stderr == NULL )
        return EOF;
        
    __ret = vfprintf (stderr, format, arg);
    
    va_end (arg);

    return __ret;
}

 
void perror (const char *str){

    stderr_printf (str);
}
 
 
// #test 
// apota par o início do arquivo.

void rewind ( FILE * stream ){

    if ( (void *) stream == NULL )
        return;


    // Pointer
    stream->_ptr = stdin->_base;

    // Buffer.
    stream->_bufsiz = BUFSIZ; 
    stream->_cnt = stdin->_bufsiz;
}


//#todo
int snprintf ( char *str, size_t count, const char *fmt, ... ){
	
	size_t ret;
	va_list ap;
    
	va_start(ap, fmt);
	
	//#todo 
	//Isso parece fácil
	//ret = vsnprintf(str, count, fmt, ap);
	
	va_end(ap);
	return ret;
}


//
// End.
//

