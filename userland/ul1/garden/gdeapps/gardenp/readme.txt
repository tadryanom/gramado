
Esse � um aplicativo para rodar no sistema operacional gramado.
Roda apenas no ambiente gramado core, no processo init.

� um editor de testes para testar funcionalidades do sistema.
No momento usamos ele para testar o envio de uma string do shell
para o editor atrav�s do kernel via mem�ria compartilhada.
O envio da mensagem funcionou.

crt0.asm cont�m o entry point, chamando main em seguida.


entry point em  
crt0 ()
  


fun��o principal em main.c
gfeMain ();


flags na linha de comando poder�o alterar o funcionamento desse aplicativo,
para que ele sirva de suporte ao desenvovledor.

ex: gfe.bin -ls