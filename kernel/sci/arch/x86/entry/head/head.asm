;
; Gramado Header - The kernel entry point for x86 processors.
; (c) Copyright 2005-2017 Fred Nora.
;
; File: x86/entry/head/head.asm 
;
; Descri��o:
;      Parte principal do n�cleo na arquitetura x86. 
; Essa � a parte inicial do n�cleo de 32 bit. 
; Nesse arquivo est� o entry point do Kernel Base.
;
; Endere�o:
;     O Kernel � carregado no endere�o f�sico 0x00100000, com o entry point 
; em 0x00101000. O endere�o l�gico � 0xC0000000.
;     Quem carregou o Kernel e fez as configura��es iniciais foi o Boot Loader.
;
; Heap and Stack:
; ==============
;     O heap do Kernel ser� configurado em  
; kernel_heap_start  = 0xC0100000.(up).
;     A pilha do Kernel ser� configurada em 
; kernel_stack_start = 0xC03FFFF0.(down). 
;
; O Kernel faz algumas inicializa��es.
; No final desse arquivo est�o a GDT, IDT e 
; includes padronizados.
;

;
; Hist�rico:
; Vers�o 1.0, 2005 - Criadas as primeiras rotinas
; Vers�o 1.0, 2006~2014 - Cria��o de outras rotinas b�sicas.
; Vers�o 1.0, 2015 - Organiza��o do conjunto de arquivos que forma programa.
; Vers�o 1.0, 2016 - Revis�o.
; ...
;






;
; Vari�veis importadas.
;

;;see: gdef.h
extern _blSavedLastValidAddress


;Buffers (gdef.h)
extern _g_frontbuffer_pa
;...


;Stacks.
;#bugbug: O endere�o configurado na hora da inicializa��o poder� corromper o 
;processo em user mode, o melhor � deixar configurar na inicializa��o com o mesmo
;endere�o indicado na tss.
extern _kernel_stack_start      ;;usado na inicializa��o @bugbug
extern _kernel_stack_start_pa   ;;indicado na TSS
;...
 
 
 
;Context.
extern _contextSS        ;User Mode.
extern _contextESP       ;User Mode.
extern _contextEFLAGS    ;User Mode.
extern _contextCS        ;User Mode.
extern _contextEIP       ;User Mode.
extern _contextDS
extern _contextES
extern _contextFS
extern _contextGS
extern _contextEAX
extern _contextEBX
extern _contextECX
extern _contextEDX
extern _contextESI
extern _contextEDI
extern _contextEBP
;...

;GUI.
extern _g_useGUI
;...

;Outros.
extern _newtask_EIP
extern _start_new_task_status 
extern _runnable              
extern _start_new_task_address	
extern _dispatch_task
;...


;
; Fun��es importadas.
;

; entrada na rotina de inicializa��o independente da arquitetura.
extern _kernel_main

; entrada na rotina de inicializa��o da arquitetura x86.
extern _x86main

;extern _KeStartIdle         ;Start Idle (entra em User Mode). 
extern _startStartIdle 
extern _save_kernel_args
;extern _kernelServices ;Services


;...

;Tasks in ring 0.
;Threads em ring 0 que rodar�o dentro do kernel base.
extern _task0 
extern _task1 
extern _task2 
;...

;
; Constants.
;


;int SYSTEM      ;modo de usar.
;SYSTEM EQU 200  ;N�MERO DA INTERRUP��O DO SISTEMA.
;LISTA DE SERVI�OS
;SYSTEM_XX EQU ... 
;SYSTEM_XX EQU ...
;SYSTEM_XX EQU ...
;SYSTEM_XX EQU ...
; ...
;
;LISTA DE ERROS
;
; ERROR1 EQU
; ERROR2 EQU
; ERROR3 EQU
; ...



;
; Physical address for Kernel Base and Kernel Entry Point.
;

KRN_BASE       equ 0x00100000    ;Base.
KRN_ENTRYPOINT equ 0x00101000    ;Entry Point.
;KRN_ENTRYPOINT equ (0x00101000+(header size))    ;Entry Point. @todo
;...


;==================================
; head_init:
; IN:  
;    al = 'G' (Graphic Mode).
;    al = 'T' (Text Mode).
;    ebx = LFB.
;
;    ecx = BootBlock pointer.
;    edx = LoaderBlock pointer.
;
; Isso � chamado por boot.asm.
;
    
head_init:


    ;Debug.
    ;*IMPORTANTE para text mode.
	;
	; OBS: O endere�o virtual da mem�ria de v�deo vga � 0x800000
	;      o mapeamento foi feito pelo Boot Loader.
	;
	;mov byte [0x800000], byte "K"
    ;mov byte [0x800001], byte 9	
    ;mov byte [0x800002], byte "R"
    ;mov byte [0x800003], byte 9	
    ;mov byte [0x800004], byte "N"
    ;mov byte [0x800005], byte 9	
    ;mov byte [0x800006], byte "L"
    ;mov byte [0x800007], byte 9	

	
	
	;mov byte [0xb8000], byte "k"	
    ;mov byte [0xb8000], byte 9	
    ;mov byte [0xb8000], byte "k"	
    ;mov byte [0xb8000], byte 9	


	;Debug
	;Testando GUI.
	;Ok funcionou.
	
	;mov eax, 0xc0c0c0
	;mov ebx, 500
	;mov ecx, 500
	;call _gui_buffer_putpixel

	;mov eax, 0xc0c0c0
	;mov ebx, 502
	;mov ecx, 500
	;call _gui_buffer_putpixel

	;mov eax, 0xc0c0c0
	;mov ebx, 504
	;mov ecx, 500
	;call _gui_buffer_putpixel
	;call _asm_refresh_screen    ;refresh
	;jmp $

    	
	;; Checando se foi carregado de um multiboot compat�vel
	;; cmp eax, MULTIBOOT_BOOTLOADER_MAGIC ;0x36d76289
	;; je mb_ok
    ;;
	;; #todo: 
	;; Mudar os argumentos passados pelo bootloder para estarem  
	;; em conformidade com os padr�es da multiboot specification.
	
	;
	; **** EAX = MAGIC  **** 0x36d76289
	;
	
	;;se estivermos no modo gr�fico.
	cmp al, byte 'G'
    je .useGUI

	;
	; No GUI.
	;

.nogui:
	mov byte [0xb8000], byte "t"
    mov byte [0xb8001], byte 9
    mov byte [0xb8002], byte "m"
    mov byte [0xb8003], byte 9


.nogui_hang:
    hlt
    jmp .nogui_hang

	;
	; Use GUI.
	;

.useGUI:

    ;checar se chegamos aqui com a flag de gui acionada.
    
	cmp al, byte 'G'
	jne .nogui

	mov dword [_g_useGUI], dword 1
	mov dword [_SavedBootMode], dword 1
	
   
   ;mov byte [bl_video_mode], al
   ;mov dword [_g_lbf_pa], ebx         ;Endere�o f�sico do LFB.

   
	;; #todo: 
	;; O PONTEIRO PARA BOOTBLOCK DEVER� FICAR EM EBX E SEGUIR    
	;; OS PADR�ES DA MULTIBOOT SPECIFICATION, PARA ISSO O BOOT BLOCK
	;; CRIADO NO BOOT MANAGER E PASSADO PELO BOOT LOADER DEVE SER 
	;; REORGANIZADO, PARA ADERIR AO PADR�O.
   
    ;BootBlock pointer.
	;Ponteiro para o bootblock passado pelo Boot Manager.
	
    ;    ecx = BootBlock pointer.
    ;    edx = LoaderBlock pointer.

    mov dword [_SavedBootBlock], edx

	; Salvando o endere�o f�sico do frontbuffer.
	; FrontBuffer Address, (LFB)
	
	;lfb.
	xor eax, eax
	mov eax, dword [edx +0]       
	mov dword [_SavedLFB], eax
	mov dword [_g_frontbuffer_pa], eax

    ;X.
	xor eax, eax
    mov ax, word [edx +4]         
    mov dword [_SavedX], eax

    ;Y.
	xor eax, eax
    mov ax, word [edx +8]         
    mov dword [_SavedY], eax

    ;BPP.
	xor eax, eax
    mov al, byte [edx +12]  
    mov dword [_SavedBPP], eax
	
	;; #importante
    ;; last valid ram address.
    ;; see: gdef.h
	xor eax, eax
	mov eax, dword [edx +16]    
    mov dword [_blSavedLastValidAddress], eax


	;; #importante:
	;; Outras informa��es poderiam ser passadas 
	;; atrav�s do boot block.
	;; Informa��es captadas durante as rotinas de boot.
	
	
;.no_gui:

    mov dword [_g_useGUI], dword 0

    ; Salva os argumentos passados pelo boot loader via registradores.
    
	mov dword [_kArg1], eax    ;Video mode.
	mov dword [_kArg2], ebx    ;LFB. (Linear Frame Buffer)
	mov dword [_kArg3], ecx    ;??.
	mov dword [_kArg4], edx    ;??.


	;
	; Salva os argumentos do kernel em estrutura.
	;

	;call _save_kernel_args
	
	
    ;; *step
	;; Interrupt enabling: 
	;; Come�aremos configurando suporte a interrup��es.

    ; Desabilita para seguran�a.
    
	cli
	
	;; Ordem das tabelas: 
	;; gdt, idt, ldt, tss(tr).
	
	; GDT.
	
	lgdt [_GDT_register] 


	; IDT.
	
	call setup_idt        ;Aponta tudo para uma isr s�. 'unhandled_int'.
	call setup_faults     ;Configura vetores de faults e exceptions.
	call setup_vectors    ;Configura outros vetores.
	lidt [_IDT_register] 


    ;; LDT.
    ;; Clear LDT

    xor eax, eax    
    lldt ax



	;
	; TR. (tss)
	;


    ; ??
    ; Flush TSS:
    ;     Load the index of our TSS structure.
	;     The index is 0x28, as it is the 5th selector and 
	; each is 8 bytes long, but we set the bottom two bits (making 0x2B)
    ; so that it has an RPL of 3, not zero.
    ;     Load 0x2B into the task state register.
	
	;; #importante
	;; Coloca o endere�o da TSS na entrada da GDT
	
	;;isso ja foi configurado.(103)
	;;mov word [gdt6], tss0_end - tss0 - 1  
	
	mov eax, dword tss0
	
	mov [gdt6 + 2], ax
	shr eax, 16
	mov [gdt6 + 4], al
	mov [gdt6 + 7], ah

	;carrega TR.
	mov ax, word 0x2B     ;28+3. 
	ltr ax  

    ; Me parece que nesse momento precisamos de um jmp far, 
    ; semelhante ao que usamos na comuta��o do modo real para o 
    ; modo protegido.	
	
	jmp 8:dummyJmpAfterLTR
    nop	
dummyJmpAfterLTR:
	
	;; Selecting the 'Processor Interrup Mode'.
	;; * PIC MODE *
		
    ;; Todos os componentes APIC s�o ignorados e o sistema opera
    ;; no modo single-thread usando LINT0.



;.setupPICMODE:

    ; PIC.
    ; Early PIC initialization.

    cli

    ;xor eax, eax
    mov al, 00010001b    ;begin PIC1 initialization.
    out 0x20, al
    IODELAY
    mov al, 00010001b    ;begin PIC2 initialization.
    out 0xA0, al
    IODELAY
    mov al, 0x20         ;IRQ 0-7: interrupts 20h-27h.
    out 0x21, al
    IODELAY
    mov al, 0x28        ;IRQ 8-15: interrupts 28h-2Fh.
    out 0xA1, al
    IODELAY
    mov al, 4
    out 0x21, al
    IODELAY
    mov al, 2
    out 0xA1, al
    IODELAY
    ;mov al, 00010001b    ;11 sfnm 80x86 support.
    mov al, 00000001b     ;01 80x86 support.
    out 0x21, al
    IODELAY
    out 0xA1, al
    IODELAY

    ;Mask all interrupts.
    cli
    mov  al, 255
    out  0xa1, al
    IODELAY
    out  0x21, al
    IODELAY


	;; Com todas as interrup��es mascaradas, � hora de 
	;; configurarmos os timers.
	
	;; Configurando os timers do sistema.
	;; Nossa op��o agora � o PIT.
	;; @todo: Para o RTC podemos fazer uma pequena inicializa��o agora.
	;; pois temos um m�dulo mais completo em C.

	
	; TIMER.
	; PIT 8253 e 8254 = (1234DD) 1193181.6666 / 100 = 11930. ; 1.19MHz.
	; APIC timer      = 3,579,545 / 100 = 35796  3.5 MHz.
	; 11931    ; (1193181.6666 / 100 = 11930) timer frequency 100 HZ
	
	; PIT
	; Early PIT initialization.
	
    ;xor eax, eax
    mov al, byte 0x36
    mov dx, word 0x43
    out dx, al
    IODELAY
    mov eax, dword 11931
    mov dx, word 0x40
    out dx, al
    IODELAY
    mov al, ah
    out dx, al
    IODELAY


	;; #todo:
	;; Init RTC.

	;; step 
    ;; #todo: 
    ;; memory caching control.	
	
	;;step 
	;processor Discovery and initialization	
	;apenas o b�sico para o boot manager.
	
		
	;;
	;; Fazendo alguma inicializa��o de dispositivos de I/O suportados.
	;;

	;step
	;i/o devices
	;( embedded controller EC, super io SIO, legacy free systems, 
	; miscellaneous io devices)
	
	
	;todo: Aqui  � um bom lugar para isso.
	;teclado ide, lfb ...
	

    ;;
    ;; Desmascarando intrrup��es.
    ;;

    ; Unmask only the timer interrupt.
    ;mov dx, word 0x21
    ;in  al, dx
    ;IODELAY
    ;and al, byte 0xfe
    ;out dx, byte al
    ;IODELAY
    
    ; Unmask all interrupts.
    mov al, 0
    out 0xa1, al
    IODELAY
    out 0x21, al
    IODELAY

	; #IMPORTANTE.
	; Desbilita as interrup��es. 
	cli


	;;
	;; Configurando alguns registradores.
	;;
	
	; Debug.
	; Debug: Disable break points.
	
    xor eax, eax
    ;mov dr2, eax
    mov dr7, eax



	;Segmentos de dados em ring0.
	
    mov ax, word 0x10   
    mov ds, ax
    mov es, ax
    ;; ...
    
 
	;;
	;; STACK
	;;

	; Stack
	; (atualiza o ponteiro para a vari�vel global).
	
	; (o mesmo endere�o indicado na TSS ??)

    mov eax, 0x003FFFF0 
    mov esp, eax 
    
    ; Salva.
    mov dword [_kernel_stack_start_pa], 0x003FFFF0   
    mov dword [_kernel_stack_start],    0x003FFFF0 


	; Muda o status do kernel.
	; #todo: 
	; Porque mudou para um se KeMain() muda para 0?
	; +deletar isso. 

    mov dword [_KernelStatus], dword 1
	
	
    ;Debug:
	; Essa mensagem aparece em modot texto.
    ;mov byte [0x800008], byte "M"    ;flag.	
    ;mov byte [0x800009], byte 9

	;Debug
	;#A
	;mov eax, 0xA0A0A0
	;mov ebx, 440
	;mov ecx, 440
	;call _gui_buffer_putpixel

	;call _asm_refresh_screen    
	;jmp $
	
	
   
	; Chama o c�digo em C e checa o retorno.
	; Se n�o terminou de forma normal, halt system.
	
	
	;; #todo
	;; Para simplificar a passagem de C para Assembly, n�o
	;; enviaremos argumentos para a fun��o main.
	;; os argumentos recebidos pelo kernel no entrypoint em head
	;; dever�o ser passador pelo assebly para vari�veis em C.
	

    ;;  kernel/main.c 
    call _kernel_main


    ;; #bugbug
    ;; Estamos no modo gr�fico
    ;; N�o conseguiremos exibir uma mensagem.

.hang:
    cli
    hlt
    jmp __die   ;; headlib.asm
    jmp .hang


	;
	; ====================================
	;


;----------------------------------------
; _SavedBootBlock:
;     Argumentos passados pelo Boot Manager.
;     Sobre video, ...
;
;
global _SavedBootBlock
_SavedBootBlock:
    dd 0
	
global _SavedLFB
_SavedLFB:
    dd 0

global _SavedX
_SavedX:
    dd 0

global _SavedY
_SavedY:
    dd 0

global _SavedBPP
_SavedBPP:
    dd 0	

global _SavedLastValidAddress
_SavedLastValidAddress:
    dd 0



;;
;; 1 = GUI ; 0 = Text Mode
;;	
global _SavedBootMode
_SavedBootMode:
    dd 0


;
; Argumentos recebidos do bootloader pelo kernel.
;	
		
global _kArg1	
_kArg1: 
    dd 0

global _kArg2
_kArg2: 
    dd 0

global _kArg3
_kArg3: 
    dd 0

global _kArg4
_kArg4: 
    dd 0


;
; segments -------------
; 
 
 	
segment .text 

;
; OBS:.  A GDT e a IDT est�o conveniente em segmento de c�digo.
;

;;
;; ============================ GDT =====================================
;;

;;
;; TYPES:
;; The TYPE values for these are 
;;
;;  >> 0101 for a task gate,          5
;;  >> D110 for a interrupt gate,     E
;;  >> D111 for a trap gate,          F
;;
;; where D is 1 for 32 bit gate and 0 for a 16 bit gate. 
;;


global _gdt
_gdt:
;Selector 0 - Null.
NULL_SEL equ $-_gdt
    dd 0
    dd 0
;Selector 8 - Code, kernel mode.  
CODE_SEL equ $-_gdt
	dw 0xFFFF
	dw 0
	db 0 
	db 0x9A   ; present, ring0, code, non-confirming, readble.
	db 0xCF
	db 0	
;Selector 0x10 - Data, kernel mode.
DATA_SEL equ $-_gdt
	dw 0xFFFF
	dw 0
	db 0 
	db 0x92    ; present, ring0, data, expanded up, writeble. (BITS)
	db 0xCF
	db 0
;Selector 18h - Code, user mode.
USER_CODE_SEL equ $-_gdt
	dw 0xFFFF
	dw 0
	db 0 
	db 0xF8   ;;0xFE   ;;5,E,F ;;A  ; 1111b ,ah  [ ( present|ring3|1 )  A = CODE ]
	db 0xCF
	db 0
	
        ;dw     0xffff
        ;dw     0x0000
        ;db     0x00
        ;dw     11011111b *256 +11111010b
        ;db     0x00	
	
;Selector 20h - Data, user mode.
USER_DATA_SEL equ $-_gdt
	dw 0xFFFF
	dw 0
	db 0 
	db 0xF2   ; 1111b ,2h  [ ( present|ring3|1 )  ,  2 = DATA ]
	db 0xCF
	db 0

        ;dw     0xffff
        ;dw     0x0000
        ;db     0x00
        ;dw     11011111b *256 +11110010b
        ;db     0x00
	
;Tem que ter pelo menos uma tss para mudar para user mode, 
;sen�o da falta.
;Selector 28h - Tss.
TSS_DATA_SEL equ $-_gdt	
gdt6:
    dw 104 ;;103
    dw 0
    db 0
    db 0x89 ;;0x89   ;; presente, ring0(onde esta a tss??), s=0(segmento do sistema) /  ;;89h ((Present|Executable|Accessed)) 1001  bit3=32bitcode
    db 0x10
    db 0
;Selector 30h - Ldt.
LDT_TEST_SEL equ $-_gdt
    db 0xff
    db 0x0
    db 0x0
    db 0xd1
    db 0x0
    db 0x82
    db 0x0
    db 0x0

global _end_gdt
_end_gdt:
    dd 0

;
; GDT_register - registro
;
global  _GDT_register
_GDT_register:
	dw (_end_gdt-_gdt)-1
    dd _gdt


;
; idt ----------------------------------------------
;

;
; Usadas nas entradas da idt.
;

sys_interrupt equ 0x8E  
sys_code      equ    8    ;Seletor de c�digo.


;==================================================;
;  Idt.                                            ;
;  Interrupt vectors for intel x86                 ;
;==================================================;

global _idt
_idt:

;0 interrupt 0h, div error.
	dw 0			  
	dw sys_code
	db 0
	db sys_interrupt
	dw 0
	
;1 interrupt 1h, debug exception.
	dw 0                    		    
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

;2 interrupt 2h, non maskable interrupt.
	dw 0
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

;3 interrupt 3h, int3 trap.
	dw 0
	dw sys_code
	db 0
	db sys_interrupt
	dw 0
	
;4 interrupt 4h, into trap.
	dw 0
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

;5 interrupt 5h,  bound trap.
	dw 0
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

;6 interrupt 6h, invalid instruction.
	dw 0
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

;7 interrupt 7h, no coprocessor.
	dw 0
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

;8 interrupt 8h, double fault.
	dw 0
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

;9 interrupt 9h, coprocessor segment overrun 1.
	dw 0
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

;10 interrupt Ah, invalid tss.
	dw 0
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

;11 interrupt Bh, segment not present.
	dw 0
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

;12 interrupt Ch, stack fault.
	dw 0
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

;13 interrupt Dh, general protection fault.
	dw 0
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

;14 interrupt Eh, page fault.
	dw 0
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

;15 interrupt Fh, reserved.
	dw 0
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

;16 interrupt 10h, coprocessor error.
	dw 0
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

;17 interrupt 11h, alignment check.
	dw 0
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

;18 interrupt 12h, machine check. 
	dw 0
	dw sys_code
	db 0
	db sys_interrupt
	dw 0
	
	;;
	;; ## Intel reserveds ##
	;;

;19 interrupt 13h, reserved.
	dw 0		   
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

;20 interrupt 14h, reserved.
	dw 0
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

;21 interrupt 15h, reserved.
	dw 0
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

;22 interrupt 16h, reserved.
	dw 0
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

;23 interrupt 17h, reserved.
	dw 0
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

;24 interrupt 18h, reserved.
	dw 0
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

;25 interrupt 19h, reserved.
	dw 0
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

;26 interrupt 1Ah, reserved.
	dw 0
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

;27 interrupt 1Bh, reserved.
	dw 0
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

;28 interrupt 1Ch, reserved.
	dw 0
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

;29 interrupt 1Dh, reserved.
	dw 0
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

;30 interrupt 1Eh, reserved.
	dw 0
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

;31 interrupt 1Fh, reserved.
	dw 0
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

  ;; 
  ;;  ##  IRQs ##
  ;;

;32 interrupt 20h, IRQ0, TIMER.
	dw 0
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

;33 interrupt 21h, IRQ1, TECLADO.
	dw 0
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

;34 interrupt 22h, IRQ2.
	dw 0
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

;35 interrupt 23h, IRQ3.
	dw 0
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

;36 interrupt 24h, IRQ4.
	dw 0
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

;37 interrupt 25h, IRQ5.
	dw 0
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

;38 interrupt 26h, IRQ6.
	dw 0
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

;39 interrupt 27h, IRQ7.
	dw 0
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

;40 interrupt 28h, IRQ8. 
	dw 0
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

;41 interrupt 29h, IRQ9. 
	dw 0
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

;42 interrupt 2Ah, IRQ10. 
	dw 0
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

;43 interrupt 2Bh, IRQ11. 
	dw 0
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

;44 interrupt 2Ch, IRQ12. 
	dw 0
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

;45 interrupt 2Dh, IRQ13. 
	dw 0
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

;46 interrupt 2Eh, IRQ 14. 
	dw 0
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

;47 interrupt 2Fh, IRQ15. 
	dw 0
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

;48 interrupt 30h.
	dw 0
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

;49 interrupt 31h.
	dw 0
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

;50 interrupt 32h.
	dw 0
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

;51 interrupt 33h.
	dw 0
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

;52 interrupt 34h.
	dw 0
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

;53 interrupt 35h.
	dw 0
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

;54 interrupt 36h.
	dw 0
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

;55 interrupt 37h.
	dw 0
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

;56 interrupt 38h.
	dw 0
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

;57 interrupt 39h.
	dw 0
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

;58 interrupt 3Ah.
	dw 0
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

;59 interrupt 3Bh.
	dw 0
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

;60 interrupt 3Ch.
	dw 0
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

;61 interrupt 3Dh.
	dw 0
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

;62 interrupt 3Eh.
	dw 0
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

;63 interrupt 3Fh.
	dw 0
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

;64 interrupt 40h.
	dw 0
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

;65 interrupt 41h.
	dw 0
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

;66 interrupt 42h.
	dw 0
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

;67 interrupt 43h.
	dw 0
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

;68 interrupt 44h.
	dw 0
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

;69 interrupt 45h.
	dw 0
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

;70 interrupt 46h.
	dw 0
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

;71 interrupt 47h.
	dw 0
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

;72 interrupt 48h.
	dw 0
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

;73 interrupt 49h.
	dw 0
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

;74 interrupt 4Ah.
	dw 0
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

;75 interrupt 4Bh.
	dw 0
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

;76 interrupt 4Ch.
	dw 0
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

;77 interrupt 4Dh.
	dw 0
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

;78 interrupt 4Eh.
	dw 0
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

;79 interrupt 4Fh.
	dw 0
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

;80 interrupt 50h.
	dw 0
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

;81 interrupt 51h.
	dw 0
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

;82 interrupt 52h.
	dw 0
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

;83 interrupt 53h.
	dw 0
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

;84 interrupt 54h.
	dw 0
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

;85 interrupt 55h.
	dw 0
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

;86 interrupt 56h.
	dw 0
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

;87 interrupt 57h.
	dw 0
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

;88 interrupt 58h.
	dw 0
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

;89 interrupt 59h
	dw 0
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

;90 interrupt 5Ah.
	dw 0
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

;91 interrupt 5Bh.
	dw 0
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

;92 interrupt 5Ch.
	dw 0
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

;93 interrupt 5Dh.
	dw 0
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

;94 interrupt 5Eh.
	dw 0
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

;95 interrupt 5Fh.
	dw 0
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

;96 interrupt 60h. 
	dw 0
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

;97 interrupt 61h.
	dw 0
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

;98 interrupt 62h.  
	dw 0
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

;99 interrupt 63h.
	dw 0
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

;100 interrupt 64h.
	dw 0
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

;101 interrupt 65h.
	dw 0
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

;102 interrupt 66h.
	dw 0
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

;103 interrupt 67h.
	dw 0
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

;104 interrupt 68h
	dw 0
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

;105 interrupt 69h.
	dw 0
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

;106 interrupt 6Ah.
	dw 0
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

;107 interrupt 6Bh.
	dw 0
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

;108 interrupt 6Ch.
	dw 0
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

;109 interrupt 6Dh.
	dw 0
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

;110 interrupt 6Eh.
	dw 0
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

;111 interrupt 6Fh.
	dw 0
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

;112 interrupt 70h.
	dw 0
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

;113 interrupt 71h.
	dw 0
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

;114 interrupt 72h.
	dw 0
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

;115 interrupt 73h.
	dw 0
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

;116 interrupt 74h.
	dw 0
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

;117 interrupt 75h.
	dw 0
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

;118 interrupt 76h.
	dw 0
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

;119 interrupt 77h.
	dw 0
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

;120 interrupt 78h.
	dw 0
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

;121 interrupt 79h.
	dw 0
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

;122 interrupt 7Ah.
	dw 0
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

;123 interrupt 7Bh.
	dw 0
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

;124 interrupt 7Ch.
	dw 0
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

;125 interrupt 7Dh.
	dw 0
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

;126 interrupt 7Eh.
	dw 0
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

;127 interrupt 7Fh.
	dw 0
	dw sys_code
	db 0
	db sys_interrupt
	dw 0
	
	;;
	;;  ## system call ##
	;;
	
	;;#importante
	;;Essa agora vai ser a system call.
	;;Ela � UNIX-like e usada em muitos sistemas.

;128 interrupt 80h.
	dw 0
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

;129 interrupt 81h.
	dw 0
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

;130 interrupt 82h.
	dw 0
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

;131 interrupt 83h.
	dw 0
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

;132 interrupt 84h.
	dw 0
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

;133 interrupt 85h.
	dw 0
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

;134 interrupt 86h.
	dw 0
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

;135 interrupt 87h.
	dw 0
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

;136 interrupt 88h.
	dw 0
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

;137 interrupt 89h.
	dw 0
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

;138 interrupt 8Ah.
	dw 0
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

;139 interrupt 8Bh.
	dw 0
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

;140 interrupt 8Ch.
	dw 0
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

;141 interrupt 8Dh.
	dw 0
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

;142 interrupt 8Eh.
	dw 0
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

;143 interrupt 8Fh.
	dw 0
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

;144 interrupt 90h.
	dw 0
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

;145 interrupt 91h.
	dw 0
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

;146 interrupt 92h.
	dw 0
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

;147 interrupt 93h.
	dw 0
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

;148 interrupt 94h.
	dw 0
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

;149 interrupt 95h.
	dw 0
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

;150 interrupt 96h.
	dw 0
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

;151 interrupt 97h.
	dw 0
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

;152 interrupt 98h.
	dw 0
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

;153 interrupt 99h.
	dw 0
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

;154 interrupt 9Ah.
	dw 0
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

;155 interrupt 9Bh.
	dw 0
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

;156 interrupt 9Ch.
	dw 0
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

;157 interrupt 9Dh.
	dw 0
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

;158 interrupt 9Eh.
	dw 0
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

;159 interrupt 9Fh.
	dw 0
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

;160 interrupt A0h.
	dw 0
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

;161 interrupt A1h.
	dw 0
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

;162 interrupt A2h.
	dw 0
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

;163 interrupt A3h.
	dw 0
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

;164 interrupt A4h.
	dw 0
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

;165 interrupt A5h.
	dw 0
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

;166 interrupt A6h.
	dw 0
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

;167 interrupt A7h.
	dw 0
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

;168 interrupt A8h.
	dw 0
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

;169 interrupt A9h.
	dw 0    
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

;170 interrupt AAh.
	dw 0 	   
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

;171 interrupt ABh.
	dw 0 	   
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

;172 interrupt ACh.
	dw 0 	   
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

;173 interrupt ADh.
	dw 0
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

;174 interrupt AEh.
	dw 0
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

;175 interrupt AFh.
	dw 0
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

;176 interrupt B0h.
	dw 0
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

;177 interrupt B1h.
	dw 0
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

;178 interrupt B2h.
	dw 0
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

;179 interrupt B3h.
	dw 0
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

;180 interrupt B4h.
	dw 0
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

;181 interrupt B5h.
	dw 0
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

;182 interrupt B6h.
	dw 0
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

;183 interrupt B7h.
	dw 0
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

;184 interrupt B8h.
	dw 0
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

;185 interrupt B9h.
	dw 0
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

;186 interrupt BAh.
	dw 0
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

;187 interrupt BBh.
	dw 0
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

;188 interrupt BCh.
	dw 0
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

;189 interrupt BDh.
	dw 0
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

;190 interrupt BEh.
	dw 0
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

;191 interrupt BFh.
	dw 0
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

;192 interrupt C0h.
	dw 0
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

;193 interrupt C1h.
	dw 0
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

;194 interrupt C2h.
	dw 0
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

;195 interrupt C3h.
	dw 0
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

;196 interrupt C4h.
	dw 0
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

;197 interrupt C5h.
	dw 0
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

;198 interrupt C6h.
	dw 0
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

;199 interrupt C7h.
	dw 0
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

  ;;#obs
  ;;Essa n�o � mais a interrup��o do sistema.
  ;;Agora � a tradicional 128 (0x80)   
	
;200 interrupt C8h, 
	dw 0
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

;201 interrupt C9h.
	dw 0
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

;202 interrupt CAh.
	dw 0
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

;203 interrupt CBh.
	dw 0
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

;204 interrupt CCh.
	dw 0
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

;205 interrupt CDh.
	dw 0
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

;206 interrupt CEh.
	dw 0
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

;207 interrupt CFh.
	dw 0
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

;208 interrupt D0h.
	dw 0
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

;209 interrupt D1h.
	dw 0
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

;210 interrupt D2h.
	dw 0
	dw sys_code
	db 0
	db sys_interrupt
	dw 0
;211 interrupt D3h.
	dw 0
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

;212 interrupt D4h.
	dw 0
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

;213 interrupt D5h.
	dw 0
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

;214 interrupt D6H.
	dw 0
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

;215 interrupt D7h.
	dw 0
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

;216 interrupt D8h.
	dw 0
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

;217 interrupt D9h.
	dw 0
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

;218 interrupt DAh.
	dw 0
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

;219 interrupt DBh.
	dw 0
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

;220 interrupt DCh.
	dw 0
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

;221 interrupt DDh.
	dw 0
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

;222 interrupt DEh.
	dw 0
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

;223 interrupt DFh.
	dw 0
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

;224 interrupt E0h.
	dw 0
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

;225 interrupt E1h.
	dw 0
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

;226 interrupt E2h.
	dw 0
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

;227 interrupt E3h.
	dw 0
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

;228 interrupt E4h.
	dw 0
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

;229 interrupt E5h.
	dw 0
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

;230 interrupt E6h.
	dw 0
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

;231 interrupt E7h.
	dw 0
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

;232 interrupt E8h.
	dw 0
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

;233 interrupt E9h.
	dw 0
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

;234 interrupt EAh.
	dw 0
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

;235 interrupt EBh.
	dw 0
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

;236 interrupt ECh.
	dw 0
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

;237 interrupt EDh.
	dw 0
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

;238 interrupt EEh.
	dw 0
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

;239 interrupt EFh.
	dw 0
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

;240 interrupt F0h.
	dw 0
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

;241 interrupt F1h.
	dw 0
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

;242 interrupt F2h.
	dw 0
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

;243 interrupt F3h.
	dw 0
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

;244 interrupt F4h.
	dw 0
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

;245 interrupt F5h.
	dw 0
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

;246 interrupt F6h.
	dw 0
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

;247 interrupt F7h.
	dw 0 	   
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

;248 interrupt F8h.
	dw 0
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

;249 interrupt F9h.
	dw 0
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

;250 interrupt FAh.
	dw 0
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

;251 interrupt FBh.
	dw 0
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

;252 interrupt FCh.
	dw 0
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

;253 interrupt FDh.
	dw 0
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

;254 interrupt FEh.
	dw 0
	dw sys_code
	db 0
	db sys_interrupt
	dw 0

;255 interrupt FFh.
	dw 0
	dw sys_code
	db 0
	db sys_interrupt
	dw 0
idt_end:
    dd 0



;
; IDT_register - registro.
;

global _IDT_register
_IDT_register:
    dw  (256*8) - (1)
    dd _idt 
	




;
; End
;

