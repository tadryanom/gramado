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

extern _idleServices
extern _driverInitialize  ;Envia um sinal pro kernel inicializar esse driver.
extern _driverUninitialize
extern _testtest_main


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
global _idle_entry_point              
_idle_entry_point:
    nop
.checkFlag:	
	
	;cmp edx, dword 0x00001234
    ;je InitializeDriver    	     ;; Initialize driver.	
	
	
	;cmp edx, dword 0x00004321
	;je UninitializeDriver        ;; Uninitialize driver.
	
	;cmp edx, dword 0x12345678    ;; Magic.
	;je services
	
	
	call _testtest_main
	
	;;
	;;    * IDLE LOOP
	;;
	
IdleLoop:
    NOP
    JMP IdleLoop 
	
	
;;====================================================================	
;; InitializeDriver:
;;     Esse processo ser� inicializado como um driver em user mode.
;;   
InitializeDriver:
    call _driverInitialize	
    JMP IdleLoop 
	
	
;;====================================================================	
;; UninitializeDriver:
;;     O processo deixa de atuar como um driver em user mode.
;; 	
UninitializeDriver:
    call _driverUninitialize
    JMP IdleLoop 
	
	
;;======================================================================
;; services:
;;     Chamaremos algum dos servi�os oferecidos, que obviamente s�
;; funcionar�o se o driver estiver inicializado.
;;
services:
    
	;;
	;; IN: EAX={N�mero do servi�o solicitado.}
	;;

	push eax
	
    call _idleServices 
	mov dword [.ret_val], eax
	
	pop eax
	
	;;
	;; aqui podemos fazer alguma coisa com o valor retornado.
	;;
	
    JMP IdleLoop     	
	
.ret_val: dd 0

;
;End.
;
