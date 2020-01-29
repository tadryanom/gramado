

// serial.h


/*
UART (Universal Asynchronous Receiver/Transmitter), 
 A UART transforma em serial a sa�da paralela do barramento do computador, 
 para realizar a transmiss�o via porta serial. 
 
 A maioria dos chips UART possui um buffer embutido entre 16 e 64 kilobytes.
 
 Portas seriais de alta velocidade como a Enhanced Serial Port (ESP) e a 
 Super Enhanced Serial Port (Super ESP) 
 
O conector externo para uma porta serial pode ser tanto de 9 como de 25 pinos.
 Originalmente, a utiliza��o principal de uma porta serial era conectar um modem ao computador. 
 As fun��es dos pinos refletem isso. 



Conector de 9 pinos:
1. Detector de portadora (carrier detect) - determina se o modem est� conectado a uma linha telef�nica em opera��o. 
2. Recebimento de dados (receive data) - o computador recebe as informa��es enviadas pelo modem. 
3. Transmiss�o de dados (transmit data) - o computador envia informa��es ao modem. 
4. Terminal pronto (data terminal ready) - o computador avisa ao modem que est� pronto para se comunicar. 
5. Sinal de terra (signal ground) - pino ligado a terra. 
6. Modem pronto (data set ready) - o modem avisa ao computador que est� pronto para se comunicar. 
7. Solicita��o de envio (request to send) - o computador pergunta ao modem se pode enviar informa��es. 
8. Pronto para enviar (clear to send) - o modem avisa ao computador que as informa��es podem ser enviadas. 
9. Indicador de sinal (ring indicator) - assim que a liga��o � estabelecida, 
o computador reconhece o aviso (enviado pelo modem) que indica que o sinal foi detectado.


controle de fluxo?

*/


/*
  Headless Server viar Serial Port:
  =================================
  
  A headless system is a computer system or device that has been configured to 
  operate without a monitor (the missing "head"), keyboard, and mouse. 
  A headless system is typically controlled via a network connection, 
  although some headless system devices require a serial connection to be made 
  via RS-232 for administration of the device. Headless operation of a server is 
  typically employed to reduce operating costs.
  Obs: RS-232  Serial port.
  
 Headless computers and other remotely controlled hardware devices often operate 
 using network control models.

 */


/*
typedef struct serial_port_d serial_port_t;
struct serial_port_d
{
	//Object.
	object_type_t objectType;
	object_class_t objectClass;
	
	int z;
	
	//...
	
	unsigned long flag;  //f
	unsigne dlong error; //e
	
    struct serial_port_d *next;	
};
serial_port_t *SerialPort;
//...
*/

int serialportError;
//...


//
//End.
//


