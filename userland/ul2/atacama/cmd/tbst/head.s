;
; Gramado Idle - This is a 32bit, user mode, system application.
; It's the idle process to run when the system is with few activity.
; (c) Copyright 2015-2017 Fred Nora.
;
; File: head.s  
;
; Descri��o:
;     � o entry point do programa IDLE.BIN na plataforma x86. 
;     Faz a vez do crt0.s por enquanto.
;
;
; Alguns par�metros: (@todo: rever)
; ================== 
;  4MB � partir de 400000.
;  f�sico = virtual      
;  400000 = 400000
;
;  + O kernel carrega o programa em 0x00400000 com o entry point em 0x0041000.
;  + Quem configurou a pagina��o foi o Boot Loader, mas o kernel refez.
;
;   Stack: (rever)
;   ss  = 0x23
;   esp = 0x00709fff0  ???
;
; Hist�rico:
;     2015 - Created by fred Nora.
;     2016 - Revision.
;

;;
;; codename db 'cali'
;;


;
; Fun��o importadas.
;





segment .head_x86


 
[bits 32]


 


	;
	; Atuando como driver:
	; ====================
	; Para o Kernel utilizar esse processo como driver, ele precisa linkar 
	; esse processo ao processo Kernel. Vamos fazer isso de forma simples 
	; agora como um teste, depois melhoraremos a maneira de fazer. 
	; O que faremos aqui � atender a solicita��o do Kernel de inicializar o 
	; driver. Logo ap�s inicializarmos o driver, enviaremos uma mensagem para 
	; o Kernel na forma de system call, ent�o o Kernel linkar� ao sistema o 
	; driver inicializado.
	;

	;
	; Testar se devemos ou n�o inicializar o driver:
	; ==============================================
	; O Kernel envia uma flag no registrado edx, se a flag for encontrada,
	; significa que devemos inicializar o driver, caso contr�rio, n�o.
	;
	; Status do teste: Ok, funcionou. Podemos usar outros argumentos para 
	; efetuarmos outras opera��es no driver. Como uninitilization.
	;

	
;;============================================
;; _idle_entry_point:
;;     Entry point
;;
;; Argumentos recebidos:
;; ====================
;;     + Temos quatro argumentos na pilha e
;;     + edx comt�m uma flag.
;;     + EAX={N�mero do servi�o solicitado.}
;;

extern _testtest_main

global _idle_entry_point              
_idle_entry_point:
    nop
.checkFlag:	
	
	
	call _testtest_main
	
	;;
	;;    * IDLE LOOP
	;;
	
IdleLoop:
    NOP
    JMP IdleLoop 
 
	
 
;
;End.
;
