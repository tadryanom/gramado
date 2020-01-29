;
; File: x86/head/hw.inc 
;
; Descri��o:
;     Interrup��es de hardware.
;     Irqs e interrup��es reservadas (faults,etc).
;     Ordem: Primeiro as irqs, depois as outras.
;
; Hist�rico:
;     Vers�o: 1.0, 2015 - Esse arquivo foi criado por Fred Nora.
;     Vers�o: 1.0, 2016 - Revis�o.
;


;; See:
;; https://wiki.osdev.org/8259_PIC#Spurious_IRQs
;;


;; from interwebs: "
;; There are several reasons for the interrupt to disappear. 
;; In my experience the most common reason is software sending an 
;; EOI at the wrong time. Other reasons include noise on IRQ lines
;; (or the INTR line). "

;; For a spurious IRQ, there is no real IRQ and the PIC chip's 
;; ISR (In Service Register) flag for the corresponding IRQ will 
;; not be set. 
;; This means that the interrupt handler must not send an EOI back to 
;; the PIC to reset the ISR flag. 

;; Also note that some operating systems (e.g. Linux) 
;; keep track of the number of spurious IRQs that have occurred 
;; (e.g. by incrementing a counter when a spurious IRQ occurs). 
;; This can be useful for detecting problems in software 
;; (e.g. sending EOIs at the wrong time) and detecting problems in
;; hardware (e.g. line noise). 



;
; Fun��es importadas.
;


;;;;
; Usadas pela _irq0.
;Driver em user mode.
;extern _KeTimer
;extern _KeTaskSwitch
;Driver no Kernel Base.
extern _KiTimer        
;extern _timer 
extern _KiTaskSwitch   
;extern _task_switch

;;;;
; Usada pela _irq1
extern _KiKeyboard

;;;
;Usada pela _irq8
extern _KiRtcIrq
;extern _rtc_irq

;;usada pela irq12.
extern _mouse_handler


;
; _KiPciHandler (PCI)
;     Todas as interrup��es geradas pelos dispositivos PCI
; usar�o o mesmo isr (handler). Cabera � rotina do handler identificar
; qual dispositivo sinalizou que efetuou uma interrup��o. Ent�o direcionar 
; para a rotina de servi�o apropriada.
; Obs: 4 slots podem compartilhar a mesma interro��o.
; 4 interrup��es, podem atender no m�ximo 16 slots, ou 16 dispositivos.
; Precisamos ser capazes de atender � 256 dispositivos. (256/4=64interrp��es)
;
;

;Cada um desses pode atender at� 4 dispositivos.
extern _KiPciHandler1
extern _KiPciHandler2
extern _KiPciHandler3
extern _KiPciHandler4
;;...

;;...


;; @todo:
;;?? Em que arquivo est� essa fun��o ??
extern _current_process_pagedirectory_address
;;...


;;=====================================================
;;  ## TIMER ##
;;=====================================================
;; _irq0: 
;;
;; IRQ 0. 
;; Handler da interrup��o do timer.
;;

;; #importante
;; Na TSS tem a pilha de ring 0 (SS: ESP), temos que salvar. 
;; precisamos da pilha antes de chamarmos as rotinas em C.
;; #test: Vamos fazer um teste usando a pilha na sua posi��o inicial.

global _irq0
_irq0:

    cli
	
	;
	;stack
	pop dword [_contextEIP]         ; eip (DOUBLE).
	pop dword [_contextCS]          ; cs  (DOUBLE).
	pop dword [_contextEFLAGS]      ; eflags (DOUBLE).
	pop dword [_contextESP]         ; esp - user mode (DOUBLE).
	pop dword [_contextSS]          ; ss  - user mode (DOUBLE).

	;
	;registers 
	mov dword [_contextEDX], edx    ; edx.
	mov dword [_contextECX], ecx    ; ecx.
	mov dword [_contextEBX], ebx    ; ebx.
	mov dword [_contextEAX], eax    ; eax.
	
	;
	;registers 
	mov dword [_contextEBP], ebp    ; ebp.
	mov dword [_contextEDI], edi    ; edi.
	mov dword [_contextESI], esi    ; esi.
	
	;
	;segments
    xor eax, eax
    mov ax, gs
    mov word [_contextGS], ax	
    mov ax, fs
    mov word [_contextFS], ax	
    mov ax, es
    mov word [_contextES], ax	
    mov ax, ds
    mov word [_contextDS], ax	


	; @todo:
	; Continuar salvamento de contexto dos registradores x86. 
	; Outros registradores. Ex: media, float point, debug.

	; Preparando os registradores, para funcionarem em kernel mode.
	;
	; Obs: 
	; Os registradores fs e gs podem ser configurados com seletor nulo '0',
	; ou ignorados para economizar instru��o.

;;.setupKernelModeRegisters:
  
	
	;; #importante
	;; E a pilha em ring 0?? ss
	;; temos que pegar na tss a pilha em ring 0.
	;; Para retornarmos � velha pilha que tinhamos.
	
	;;#bugbug
	;;Temos que pegar a pilha agora, para n�o termos problemas
	;;com as rotinas em C.
	;; #test: Vamos fazer um teste usando a pilha na sua posi��o inicial.
	
	;;VAMOS TESTAR SEM ISSO NA M�QUINA REAL.
	xor eax, eax
	mov ax, word 0x10
	mov ds, ax
	mov es, ax
	mov fs, ax  ;*   
	mov gs, ax  ;* 
	
	mov ss, ax
	mov eax, 0x003FFFF0 
	mov esp, eax 
	

	; Chama as rotinas em C.
	; As rotinas em executar�o servi�os oferecidos pelo kernel
	; ou pelos seus modulos ou drivers.
	; Durante a execu��o dessas rotinas, as interrup��es podem
	; por um instante serem habilitadas novamente, se isso aacontecer
	; n�o queremos que a interrup��o de timer irq0 chame essas rotinas
	; em c novamente. Ent�o desabilitaremos a rechamada dessas fun��es
	; enquanto elas estivere em execu��o e habilitaremos novamnete ao 
	; sairmos delas.


;;.TimerStuff:

	;Chamada ao m�dulo interno.
	;Para essa chamada as rotinas do timer est�o dentro do kernel base.
	;Rotinas de timer. #N�O envolvendo task switch.
    call _KiTimer  


;;.TaskSwitchStuff:
	;Task switch. Troca a tarefa a ser executada.
	;ts.c
    call _KiTaskSwitch 


	;;
	;; Flush TLB.
	;;


    ;Flush TLB.
    jmp dummy_flush
dummy_flush:
	;TLB.
    mov EAX, CR3  
    IODELAY         ;; #test
    nop
    nop
    nop
    nop
    nop
    mov CR3, EAX  


	;----------------------------------------------------------------------
	; ?? Quando chamar a rotina 'request()' ??
	; Obs: AGORA N�O!
	; Nesse momento uma thread foi selecionada, o contexto est� salvo em 
	; vari�veis que j� podem passar para os registradores e efetuar iretd.
	; Isso acontece toda vez que o timer efetua uma interrup��o, ent�o n�o � 
	; esse o momento ideal para atender aos request, (Signal), pois seria 
	; muito constante e atrapalharia o desempenho da rotina de troca de 
	; contexto. Al�m do mais, o prop�sito do request, (signal), � sinalizar
	; a necessidade de uma opera��o, por�m efetua-la somente depois que a
	; thread atual utilize toda a sua cota.
	; *** Importante:
	; O momento ideal ser� quando a thread que est� rodando, usou toda a sua
	; cota. Esse momento ideal acontece na rotina task_switch em taskswitch.c
	;-----------------------------------------------------------------------
	
	;-----------------------------------------------------------------------
	; *IMPORTANTE:
	; Existe um tipo de page fault causada por recarregar os registradores 
	; incorretamente. Por isso as rotinas de checagem de conte�do dos 
	; registradores antes de retornar devem ser mais severas. Ou seja, depois 
	; de restaurar, tem que checar. Se n�o ouver falha, executa iretd. Mas se 
	; ouver falha, devemos bloquear a thread, checar novamente, tentar 
	; concertar se for algo simples e fechar a thread se o erro for grave ou 
	; persistir. Logo apos isso devemos escalonar outra tarefa.
	;------------------------------------------------------------------------
	
	;;====================================================================
	;; ****    IMPORTANTE    ****
	;; Obs: Muitos coment�rios devem ir para a documenta��o. Onde os 
	;; procedimentos poder�o ser explicados detalhadamente.
	;;====================================================================
	
	
	
	;;
	;; * Importante:
	;; Esse � o momento em que restauramos o contexto dos registradores 
	;; de uma thread para podermos efetura um ired e passar o comando para ela.
	;; Obs: A thread est� em user mode.
	;; Obs: Ao final, um sinal de EOI � necess�rio, para sinalizarmos o fim da 
	;; interrup��o de TIMER.
	;;
	
;;.RestoreThreadContext:

	; @todo:
    ; Outros registradores precisam ser restaurados agora
	; Outros registradores, Ex: media, float point, debug.
	
	;
	;segments
	xor eax, eax
	mov ax, word [_contextDS]
	mov ds, ax
	mov ax, word [_contextES]
	mov es, ax
	mov ax, word [_contextFS]
	mov fs, ax
	mov ax, word [_contextGS]
	mov gs, ax
	
	;
	;registers 
	mov esi, dword [_contextESI]    ;esi.
	mov edi, dword [_contextEDI]    ;edi.
	mov ebp, dword [_contextEBP]    ;ebp.
	;
	mov eax, dword [_contextEAX]    ;eax.
	mov ebx, dword [_contextEBX]    ;ebx.
	mov ecx, dword [_contextECX]    ;ecx.
	mov edx, dword [_contextEDX]    ;edx.

	;
	;stack
	;; #obs: estamos colocando isso na pilha do aplicativo..
	;; assim como todas as opera��es que fizemos em C.
	;; #obs: perceba que esse salvo funciona ...
	;; #o iret da primeira thread n�o funciona porque 
	;; n�o colocamos os valores na pilha do aplicativo como fazemos aqui, e sim
	;; na pilha do kernel.
	
	push dword [_contextSS]        ;ss  - user mode.
	push dword [_contextESP]       ;esp - user mode.
	push dword [_contextEFLAGS]    ;eflags.
	push dword [_contextCS]        ;cs.
	push dword [_contextEIP]       ;eip.

	;
	;EOI - sinal.
	;Sinalizamos apenas o primeiro controlador.
    mov al, 20h
    out 20h, al  

	mov eax, dword [_contextEAX]    ;eax. (Acumulador).

	;( N�o precisa 'sti', pois as flags da pilha habilitam as interrup��es ).
	;sti

;;.Fly:
    ;;
    ;; "Hi, my name is 'iretd', I work so hard for your happiness."
    ;; "So that's why my page has to be always in the TLB."
    ;;
    iretd



;----------------------------------------------------------
; _dispatch_context:
;     Despacha o contexto salvo.
;     Retorna para a tarefa interrompida atrav�s de iret.
;

global _dispatch_context
_dispatch_context:
	
	;segments
	xor eax, eax
	mov ax, word [_contextDS]
	mov ds, ax
	mov ax, word [_contextES]
	mov es, ax
	mov ax, word [_contextFS]
	mov fs, ax
	mov ax, word [_contextGS]
	mov gs, ax
	
	;
	;registers 
	mov esi, dword [_contextESI]    ;esi.
	mov edi, dword [_contextEDI]    ;edi.
	mov ebp, dword [_contextEBP]    ;ebp.
	;
	mov eax, dword [_contextEAX]    ;eax.
	mov ebx, dword [_contextEBX]    ;ebx.
	mov ecx, dword [_contextECX]    ;ecx.
	mov edx, dword [_contextEDX]    ;edx.
	
	;
	;stack
	push dword [_contextSS]        ;ss  - user mode.
	push dword [_contextESP]       ;esp - user mode.
	push dword [_contextEFLAGS]    ;eflags.
	push dword [_contextCS]        ;cs.
	push dword [_contextEIP]       ;eip.

    ;
    ;EOI - sinal
    mov al, 20h
    out 20h, al   

	;(N�o precisa 'sti', pois as flags da pilha habilitam as interrup��es ).
	;sti

    iretd


;-------------------------------------------
; _timer_test:
;     Timer sem multitarefas. 
;     (usado antes de acionar a multitarefa)
;

global _timer_test
_timer_test:
    cli
    mov al, 20h
    out 20h, al  
    sti
    iretd



;--------------------------------------- 
; timer_interrupt:
;    Timer interrupt handler.
;    Provis�rio.
;

timer_interrupt:
    jmp unhandled_irq
    jmp $



;;; ????????

_currentTask:
    dd 0
_nextTask:
    dd 0
_stackPointers: 
    times 512 dd 0    ;;@todo: Isso � �til ?





;========================================
; _irq1:
;     IRQ 1 - #teclado.
;
global _irq1  
_irq1:
 
	;; INTERRUP��ES DESABILITADAS. 
	;; (O TIMER N�O INTERROMPE.)
	
	;;; #bugbug
	;;; salvando registradores.
	;;; e se o handler bagun�ar outros registradores.??
	
	cli

	;; precisaremos para o eoi.
	push dword eax
	
    pushad    ;;tudo.
    push ds
    push es
    push fs
    push gs



	;
	;preparando os registradores, para funcionarem em kernel mode.
	xor eax, eax
	mov ax, word 0x10    ;Kernel mode segment.	
	mov ds, ax
	mov es, ax
	mov fs, ax  ;Usar seletor 0.
	mov gs, ax  ;Usar seletor 0.
	
		
	;mov byte [0x800000], byte "t"	
    ;mov byte [0x800001], byte 9	
	
	;@todo: A op��o � chamar m�dulo externo, em um servidor ou driver.
	;call _KeKeyboard
	
	;Chamando m�dulo dentro do kernel base.
	call _KiKeyboard
	

    pop gs
    pop fs
    pop es
    pop ds
    popad


    ; send EOI to XT keyboard

    ;in      al, 061h
    ;mov     ah, al
    ;or      al, 080h
    ;out     061h, al
    ;mov     al, ah
    ;out     061h, al	

    xor eax, eax 

    ;; #bugbug: AL SUJO!!
    MOV AL, 020h
    ;OUT 0A0h, AL
    OUT 020h, AL
    
    ;; o eoi sujou isso.
    pop eax

    sti
    iretd

	
;------------
; _irq2 - IRQ 2 � cascaded signals from IRQs 8�15 
; (any devices configured to use IRQ 2 will actually be using IRQ 9)


;=======================================================
; IRQ 3 - serial port controller for serial port 2 
; (shared with serial port 4, if present)

extern _serial2_handler
extern _serial4_handler

global _irq3
_irq3:

    cli
	pushad
	
	;call _second_serial_port_Handler
	mov al, 0x20
    ;out 0xA0, al  
    out 0x20, al

	
	popad
	sti
    iretd	

	
;====================================================
; IRQ 4 - serial port controller for serial port 1 
;(shared with serial port 3, if present)

extern _serial1_handler
extern _serial3_handler

global _irq4
_irq4:

    cli
	pushad
	
	;call _first_serial_port_Handler
	mov al, 0x20
    ;out 0xA0, al  
    out 0x20, al	
	
    popad
	sti
    iretd	


;--------------
; IRQ 5 - parallel port 2 and 3  or  sound card
;_irq5:
;    iretd



;--------------
; IRQ 6 � floppy disk controller
;_irq6:
;    iretd





;===================================================
;IRQ 7 parallel port 1. It is used for printers 
; or for any parallel port 
;if a printer is not present. It can also be potentially 
; be shared with a secondary sound card with careful 
; management of the port.

;; The correct way to handle an IRQ 7 is to first check the master PIC 
;; chip's ISR to see if the IRQ is a spurious IRQ or a real IRQ. 
;; If it is a real IRQ then it is treated the same as any other real 
;; IRQ. If it is a spurious IRQ then you ignore it 
;; (and do not send the EOI). 



global _irq7
_irq7:
    cli
	pushad


    ;call _first_parallel_port_Handler

    ;; ++
    ;; ================================================
    ;; #test
    ;; testando o tratamento de interrup��o esp�ria.

    xor eax, eax
    mov  al, 03h    ; PIC.OCW3 set function to read ISR (In Service Register)
    out  23h, al    ; write to PIC.OCW3 master
    in   al, 20h    ; read ISR master.
    test al, 80h         ; if the in-service register does not have IR7 bit set
    jz short __RETURN_Spurious   ; this would be a spurious interrupt.
    
    ;; ================================================
    ;; --

;; Not spurious

    ;; master.
    ;; for irq7
    mov al, 0x20
    out 0x20, al   

__RETURN_Spurious:

    ;; No eoi for irq 7 spurious
    
    popad
    sti
    iretd









;================================================
; _irq8:
;     System CMOS, Realtime clock.
;     IRQ 8 - real-time clock (RTC)
;
global _irq8
_irq8:

    cli
	pushad
		
	call _KiRtcIrq
	
	mov al, 0x20
    out 0xA0, al  
    out 0x20, al

	
	popad
	sti
	iret
	
	
;=========================================================	
;IRQ 9

extern _xxxe1000handler

global _irq9
_irq9:
    cli
	pushad
	
	;jmp _asm_reboot
	;call _xxxe1000handler
	
	;call _acpiHandler
	mov al, 0x20
    out 0xA0, al  
    out 0x20, al

	
	popad
	sti
    iretd	

;========================================================	
;IRQ 10 � The Interrupt is left open for the use 
;of peripherals (open interrupt/available, SCSI or NIC)
;nvidia
global _irq10
_irq10:
    cli
	pushad
	
	;call _KiPciHandler1
	mov al, 0x20
    out 0xA0, al  
    out 0x20, al

	
	popad
	sti
    iretd
	


;;===============================================
;;  interrup��o 41. irq 9;
extern _xxxe1000handler

global _nic_handler
_nic_handler:
    cli
    pushad

    call _xxxe1000handler

    mov al, 0x20
    out 0xA0, al  
    out 0x20, al

    popad
    sti
    iretd



;=======================================
;IRQ 11 � The Interrupt is left open for 
;the use of peripherals (open interrupt/available, SCSI or NIC)
;audio.
global _irq11
_irq11:

    cli
    pushad
	
	;call _KiPciHandler2
    mov al, 0x20
    out 0xA0, al  
    out 0x20, al

    popad
    sti

    iretd
	
	
	
;=======================================
;IRQ 12 mouse on PS/2 connector
global _irq12
_irq12:
    
    cli

	pushad
	push ds
	push es
	push fs
	push gs
	push ss 

	;mouse.c
    call _mouse_handler

    mov al, 0x20
    out 0xA0, al 
    IODELAY 
    out 0x20, al
    IODELAY 
	
	pop ss
	pop gs 
	pop fs 
	pop es 
	pop ds
	popad

    sti

    iretd
	
	
;;============================================================	
; IRQ 13 
; CPU co-processor  or  integrated floating point unit  
; or  inter-processor interrupt (use depends on OS)
;

global _irq13
_irq13:

    cli
    push ax 

    ;call _coprocessorHandler
    mov al, 0x20
    out 0xA0, al  
    out 0x20, al

    pop ax
    sti

    iretd
	
	
;=================================================	
; _irq14:
;     Tratador de interrup��es para unidade master.
;     IRQ 14 - primary ATA channel 
;     ( ATA interface usually serves hard disk drives and CD drives ) 
;     O timer precisa ser desbilitado. ??
;

extern _ata_handler1

global _irq14
_irq14:
   
    cli 
    push eax
    PUSHAD

    call _ata_handler1

    MOV AL,020h
    OUT 0A0h,AL
    IODELAY
    OUT 020h,AL
    IODELAY
    
    POPAD
    pop eax
    sti

    IRETD


;=================================================	
; _irq15:
;     Tratador de interrup��es para unidade slave.
;     IRQ 15 - secondary ATA channel
;     O timer precisa ser desbilitado. ??
;

;; The correct way to handle an IRQ 15 is similar, but a little trickier 
;; due to the interaction between the slave PIC and the master PIC. 
;; First check the slave PIC chip's ISR to see if the IRQ is a spurious 
;; IRQ or a real IRQ. If it is a real IRQ then it is treated the same 
;; as any other real IRQ. If it's a spurious IRQ then don't send the 
;; EOI to the slave PIC; however you will still need to send the EOI 
;; to the master PIC because the master PIC itself won't know that it 
;; was a spurious IRQ from the slave. 


extern _ata_handler2

global _irq15
_irq15:

    cli
    push eax
    PUSHAD

    call _ata_handler2
    
    
    ;; #bugbug
    ;; #todo
    ;; Spurious int for irq15.
    ;; It's different from irq7

    MOV AL, 020h
    OUT 0A0h, AL
    IODELAY
    OUT 020h, AL
    IODELAY
    
    POPAD
    pop eax
    sti

    IRETD


;========================================
; unhandled_irq:
;     Interrup��o de hardware gen�rica. 
;++ 

;; #bugbug
;; N�o podemos efetuar EOI para todos 
;; somente para as irqs.

unhandled_irq:

    cli
    push eax

    mov al, 0x20
    out 0xA0, al
    IODELAY  
    out 0x20, al
    IODELAY
    
    pop eax
    sti 

    iretd
;--


;;
;; ================ # FAULTS # ================ 
;;


; Tratamento das faltas.


extern _faults

;
; Obs: Enquanto tratamos uma excess�o ou flaul, n�o desejamos
; que uma interrup��o de timer atrapalhe, ent�o vamos desabilitar
; as interrup��es.
;

;
; int 0 
global _fault_N0
_fault_N0:
	mov dword [save_fault_number], dword 0
    jmp all_faults	

;
; int 1 
global _fault_N1
_fault_N1:
	mov dword [save_fault_number], dword 1
    jmp all_faults	
	
;
; int 2 
global _fault_N2
_fault_N2:
	mov dword [save_fault_number], dword 2
    jmp all_faults	
	
;
; int 3 
global _fault_N3
_fault_N3:
	mov dword [save_fault_number], dword 3
    jmp all_faults	
	
;
; int 4 
global _fault_N4
_fault_N4:
	mov dword [save_fault_number], dword 4
    jmp all_faults	
	

;
; int 5 
global _fault_N5
_fault_N5:
	mov dword [save_fault_number], dword 5
    jmp all_faults	

;
; int 6 - Instru��o inv�lida.
global _fault_INTRUCAO_INVALIDA
_fault_INTRUCAO_INVALIDA:
	mov dword [save_fault_number], dword 6
    jmp all_faults	

;
; int 7
global _fault_N7
_fault_N7:
	mov dword [save_fault_number], dword 7
    jmp all_faults	

;
; int 8 - double fault
global _fault_DOUBLE
_fault_DOUBLE:
	mov dword [save_fault_number], dword 8
    jmp all_faults	
	
;
; int 9 
global _fault_N9
_fault_N9:
	mov dword [save_fault_number], dword 9
    jmp all_faults	
	
;
; int 10 
global _fault_N10
_fault_N10:
	mov dword [save_fault_number], dword 10
    jmp all_faults	
	
;
; int 11
global _fault_N11
_fault_N11:
	mov dword [save_fault_number], dword 11
    jmp all_faults	

;
; int 12 - Falha de pilha (interrup��o 12).
global _fault_STACK
_fault_STACK:
	mov dword [save_fault_number], dword 12
    jmp all_faults	

;
; int 13 - general protection fault (GPF).
global _fault_GP
_fault_GP:   
	mov dword [save_fault_number], dword 13
    jmp all_faults	

;
; int 14 - Page Fault (PF).
global _fault_N14
_fault_N14:
	mov dword [save_fault_number], dword 14
    jmp all_faults	
	
;
; int 15 
global _fault_N15
_fault_N15:
	mov dword [save_fault_number], dword 15
    jmp all_faults	
	
;
; int 16 
global _fault_N16
_fault_N16:
	mov dword [save_fault_number], dword 16
    jmp all_faults	
	
;
; int 17 
global _fault_N17
_fault_N17:
	mov dword [save_fault_number], dword 17
    jmp all_faults	
	
;
; int 18
global _fault_N18
_fault_N18:
	mov dword [save_fault_number], dword 18
    jmp all_faults	
	
;
; int 19
global _fault_N19
_fault_N19:
	mov dword [save_fault_number], dword 19
    jmp all_faults	
	
;
; int 20 
global _fault_N20
_fault_N20:
	mov dword [save_fault_number], dword 20
    jmp all_faults	
	
;
; int 21
global _fault_N21
_fault_N21:
	mov dword [save_fault_number], dword 21
    jmp all_faults	
	
;
; int 22
global _fault_N22
_fault_N22:
	mov dword [save_fault_number], dword 22
    jmp all_faults	
	
;
; int 23 
global _fault_N23
_fault_N23:
	mov dword [save_fault_number], dword 23
    jmp all_faults	
	
;
; int 24 
global _fault_N24
_fault_N24:
	mov dword [save_fault_number], dword 24
    jmp all_faults	
	
;
; int 25 
global _fault_N25
_fault_N25:
	mov dword [save_fault_number], dword 25
    jmp all_faults	
	
;
; int 26 
global _fault_N26
_fault_N26:
	mov dword [save_fault_number], dword 26
    jmp all_faults	
	
;
; int 27 
global _fault_N27
_fault_N27:
	mov dword [save_fault_number], dword 27
    jmp all_faults	
	
;
; int 28 
global _fault_N28
_fault_N28:
	mov dword [save_fault_number], dword 28
    jmp all_faults	
	
;
; int 29
global _fault_N29
_fault_N29:
	mov dword [save_fault_number], dword 29
    jmp all_faults	
	
;
; int 30
global _fault_N30
_fault_N30:
	mov dword [save_fault_number], dword 30
    jmp all_faults	
	
;
; int 31 
global _fault_N31
_fault_N31:
	mov dword [save_fault_number], dword 31
    jmp all_faults	
	
	
;===============================================
; all_faults:
;     As faltas s�o tratadas em kernel mode, tem que ajustar 
; os registradores para isso.
;     
; #todo: 
; Enviar o n�mero a falta para uma vari�vel global.
; Essa rotina poderia se chamar hwAllFaults.
;

;; Pushing an error code (for some exceptions): 
;; For some specific exceptions such as page faults, 
;; the CPU pushes an error code, which describes the cause of the exception.

all_faults:

	;d,c,b,a
	mov dword [_contextEDX], edx	
	mov dword [_contextECX], ecx	
	mov dword [_contextEBX], ebx	
	mov dword [_contextEAX], eax
		
	;ebp,edi,esi
	mov eax, ebp
	mov dword [_contextEBP], eax
	mov eax, edi
	mov dword [_contextEDI], eax
	mov eax, esi
	mov dword [_contextESI], eax

	;gs,fs,es,ds
	xor eax, eax	
    mov ax, gs
	mov word [_contextGS], ax	
    mov ax, fs
	mov word [_contextFS], ax	
    mov ax, es
	mov word [_contextES], ax	
    mov ax, ds
	mov word [_contextDS], ax	
		
	;(DOUBLE), eip, cs, eflags, esp, ss 
    
    ; #bugbug: 
    ; No caso de der falha no primeiro salto para ring3:
    ; Pegamos esses valores da pilha.
    ; Mas precisamos saber se esses valores refletem
    ; a parte do c�digo que estava executando na hora da falha,
    ; ou se � o valor configurado antes do primeiro salto para ring3.
    
	pop eax
	mov dword [_contextEIP], eax	
	pop eax
	mov dword [_contextCS], eax	
	pop eax
	mov dword [_contextEFLAGS], eax		
	pop eax
	mov dword [_contextESP], eax	
	pop eax
	mov dword [_contextSS], eax		
    
    ;;
    ;; Reajustando os segmentos para rodarmos tranquilos em ring0.
    ;;
    
	; Load the Kernel Data Segment descriptor.
    xor eax, eax
	mov ax, word 0x10   
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    
    ;; ??
    ;; e a stack, ainda n�o t�nhamos colocado uma configura��o de
    ;; stack aqui.
    
    ;; #todo #todo #todo STACK STACK STACK
	
    
    ;;
    ;; O n�mero da falta.
    ;;
    
	; Chama a rotina em C.
    ;Passa o argumento via pilha.	
	push dword [save_fault_number]     
	
	;; Chama c�digo em C. (faults.c)
	
	call _faults 

;hang	
.hang:	
    cli
	hlt
	jmp .hang
	
	;
	; @todo: 
	;     Existe ERROR NUMBER em algumas exce��es ?
	;
	
;Salva aqui o n�mero da fault.	
save_fault_number: 
    dd 0	
	
;
; End.
;
