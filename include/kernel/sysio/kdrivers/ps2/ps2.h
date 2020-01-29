/*
 * File: hal/unblocked/ps2.h
 *
 * Descri��o:
 *     Header para gerenciamento da porta ps/2.
 *     
 * Obs: Encontramos mais informa��es nos headers de teclado e de mouse.
 *
 */

/*
 Wikipedia
 O conector PS/2 � um conector mini-DIN de 6 pinos usado para conectar alguns 
 teclados e mouses a um sistema de computador compat�vel com PC. 
 Seu nome vem da s�rie de computadores pessoais IBM Personal System/2, 
 com o qual ele foi introduzido em 1987. O conector de mouse PS/2 geralmente 
 substitui antigos conectores de "mouses seriais" DE-9 RS-232, 
 enquanto o conector de teclado PS/2 substituiu o conector DIN mais largo de 5 pinos/180� 
 usado no desenho do IBM PC/AT. Os desenhos PS/2 nas interfaces de teclados e 
 mouses s�o eletricamente similares e empregam o mesmo protocolo de comunica��o. 
 Entretanto, a porta de teclado ou mouse de um determinado sistema pode n�o ser 
 intercambi�vel uma vez que os dois dispositivos usam um conjunto de comandos diferentes.
 
A comunica��o � serial, s�ncrona e bidirecional.[1] 
O dispositivo conectado gera o sinal de rel�gio. O hospedeiro controla a comunica��o usando a 
linha de rel�gio. Quando o hospedeiro puxa o rel�gio para baixo, a comunica��o do dispositivo � inibida.
*/

//
// Defini��es.
//

#define PS2_PORT	0x60
#define PS2_CTRL	0x64
#define PS2_ACK		0xFA 
 

//
// End.
//

