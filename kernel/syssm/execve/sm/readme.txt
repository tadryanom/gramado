

Sobre o diret�rio executive/sm:
==============================
   
    /sm  *  � a interface de administra��o do sistema,

     'System Management'.
     Esse diret�rio comt�m os recursos de gerenciamento de sistema, independente se
est�o em modo texto ou modo gr�fico.


     Os servi�os oferecidos por esses m�dulos do kernel base poder�o ser solicitados
por utilit�rios do sistema em user mode que desejam configurara o sistema. 
Obs: Os utilit�rios utilizar�o rotinas mais complexas e maiores que essas. Essas rotinas aqui ser�o as primitivas usadas por eles.


    Obs: INIT.TXT � o arquivo de configura��o do sistema por enquanto.
         ele fica no diret�rio raiz por enquanto.
         /root/INIT.TXT.


   @todo: Criar pasta /devices.



   **** Importante:
        ===========


   Existe uma fun��o chamada "void *systemGetSystemMetric(int number)" ... que retornar�
  muitas medidas do sistema, usadas para cria��o em dimens�es apropriadas e para an�lises diversas.

a fun��o fica em sm\system\system.c. 