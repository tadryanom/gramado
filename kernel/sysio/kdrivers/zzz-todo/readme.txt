
 Sobre o diret�rio \dd\drivers:
 ==========================

   #importante
   Ainda n�o estamos falando de drivers carreg�veis.
   Os drivers carrg�veis ficar�o em /gramado/drivers.
   
   Os drivers aqui no kernel base s�o simples.
   Oferecem rotinas prim�rias para os dispositivos
   mais tradicionais.
   
   Cada driver tamb�m oferece acesso ao driver 
   correspondente em user mode. se esse existir.

   O handler de interrup��o que estiver aqui pode ser mudado
   pra dentro do driver em user mode se esse existir.   

   * � necess�rio para fims de portabilidade algum tipo de abstra��o e
     encapsulamento.

    manter-se o mais local poss�vel.

   fim.