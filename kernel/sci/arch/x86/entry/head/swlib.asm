;
; File: x86/head/swlib.inc
;
; Descri��o:
;     Biblioteca para fun��es de interrup��es de software.
;     Oferece alguns servi�os b�sicos em assembly. 
;     Nome opcional para padr�o 8.3 (swlib.s)
;
; Hist�rico:
; Vers�o: 1.0, 2015 - Created.
; Vers�o: 1.0, 2016 - Revis�o.
;


extern _vsync
	
	
 

;--------------------------------
; _um_buffer_putpixel:
;     Coloca um pixel no backbuffer.
;     @todo: Deletar essa fun��o.
;
; a =  cor
; b =  x
; c =  y
; d = ?
;
global _um_buffer_putpixel
_um_buffer_putpixel:
    jmp _gui_buffer_putpixel
    jmp $
	

;;
;; ## Wrappers para putpixel ##
;;

global _swlib_backbuffer_putpixel
_swlib_backbuffer_putpixel:
    jmp _gui_buffer_putpixel 
	
global _swlib_lfb_putpixel
_swlib_lfb_putpixel:
    jmp _gui_buffer_putpixel2 

	
	
;===================================================================
; _gui_buffer_putpixel:
;     Coloca um pixel no backbuffer.
;     O m�dulo /hal deve chamar essa rotina e n�o a /gui. 
; a =  cor
; b =  x
; c =  y
; d = ?
;
global _gui_buffer_putpixel
_gui_buffer_putpixel:

	push  es
    push  ecx   
   
    ;Ajuste provis�rio.
	mov dword [.cor], eax    ;salva cor.
	mov dword [.x],   ebx    ;salva x.
	mov dword [.y],   ecx    ;salva y.		
    mov eax, dword [.x]
	mov ebx, dword [.y]      ;coloca y em ebx.	 	
	mov ecx, dword [.cor]    ;coloca a cor em ecx.
	
	;ecx (cor) AABBGGRR antigo 

	;antigo    bb gg rr aa
	;ecx (cor) aa bb gg rr  
	
	mov byte [.r], cl	 ;a.
	shr ecx, 8
	mov byte [.g], cl	 ;r.
	shr ecx, 8
	mov byte [.b], cl    ;g.
	shr ecx, 8
	mov byte [.a], cl    ;b.

    ;original
	;mov byte [.a], cl	 ;a.
	;shr ecx, 8
	;mov byte [.r], cl	 ;r.
	;shr ecx, 8
	;mov byte [.g], cl    ;g.
	;shr ecx, 8
	;mov byte [.b], cl    ;b.
    
	;segmento ES ... poderia ser 0x10
	push eax
	mov ax, 0x10
	mov es, ax
    pop eax   
	
	;;[BytesPerScanLineMOS] 
	
	;largura * bytes por pixel.
	imul ebx, 800*3        
    
	;eax*3
	mov ecx, dword 3
	mul ecx
	add eax, ebx    ;Adiciona ebx.
	
	mov  edi, eax ;dword [ebx+eax*3]  ;[ebx+eax*4] ; edi = x*4+(y*y multiplier)
    
	;mov eax, ecx       ;[MenColor] ; eax = color
    
	;(base) buffer 1 
	mov ecx, dword 0xC0800000   
    add edi, ecx     ;(deslocamento)
   	
	;a
	;mov al, byte [.a]
	;stosb 	
	;envia r, g e b.
	mov al, byte [.r]
	stosb 
	mov al, byte [.g]
	stosb 
	mov al, byte [.b]
	stosb 
    
	pop ecx
    pop es
    ret		
.cor: dd 0
.x: dd 0
.y: dd 0
.r db 0	
.g db 0
.b db 0	
.a db 0	


;===================================================================
; _gui_buffer_putpixel:
;     Coloca um pixel no LFB.
;     O m�dulo /hal deve chamar essa rotina e n�o a /gui. 
; a =  cor
; b =  x
; c =  y
; d = ?
;
global _gui_buffer_putpixel2
_gui_buffer_putpixel2:

	push  es
    push  ecx   
   
    ;Ajuste provis�rio.
	mov dword [.cor], eax    ;salva cor.
	mov dword [.x],   ebx    ;salva x.
	mov dword [.y],   ecx    ;salva y.		
    mov eax, dword [.x]
	mov ebx, dword [.y]      ;coloca y em ebx.	 	
	mov ecx, dword [.cor]    ;coloca a cor em ecx.
	
	;ecx (cor) AABBGGRR antigo 

	;antigo    bb gg rr aa
	;ecx (cor) aa bb gg rr  
	
	mov byte [.r], cl	 ;a.
	shr ecx, 8
	mov byte [.g], cl	 ;r.
	shr ecx, 8
	mov byte [.b], cl    ;g.
	shr ecx, 8
	mov byte [.a], cl    ;b.

    ;original
	;mov byte [.a], cl	 ;a.
	;shr ecx, 8
	;mov byte [.r], cl	 ;r.
	;shr ecx, 8
	;mov byte [.g], cl    ;g.
	;shr ecx, 8
	;mov byte [.b], cl    ;b.
    
	;segmento ES ... poderia ser 0x10
	push eax
	mov ax, 0x10
	mov es, ax
    pop eax   
	
	;;[BytesPerScanLineMOS]       
	
	;;largura * bytes por pixel
	imul ebx, 800*3            
    
	;eax*3
	mov ecx, dword 3
	mul ecx
	add eax, ebx    ;Adiciona ebx.
	
	mov  edi, eax ;dword [ebx+eax*3]  ;[ebx+eax*4] ; edi = x*4+(y*y multiplier)
    
	;mov eax, ecx       ;[MenColor] ; eax = color
    
	;(base) buffer 1 
	;;mov ecx, dword 0xC0800000  ;;backbuffer  
    mov ecx, dword 0xC0400000	 ;;lfb
    add edi, ecx     ;(deslocamento)
   	
	;a
	;mov al, byte [.a]
	;stosb 	
	;envia r, g e b.
	mov al, byte [.r]
	stosb 
	mov al, byte [.g]
	stosb 
	mov al, byte [.b]
	stosb 
    
	pop ecx
    pop es
    ret		
.cor: dd 0
.x: dd 0
.y: dd 0
.r db 0	
.g db 0
.b db 0	
.a db 0	


;---------------------------
; _background:
;     Pinta um pano de fundo no backbuffer.
;     Obs: Essa rotina tamb�m existe em C. /gui.
;
; Input: 
;     EAX = COLOR.
;
global _background
_background:		
.fade_screen: 

    ;Estamos pintando no backbuffer. N�o precisa sincronismo vertical.
	;call _vsync 
	
	pushad  
	;COR
	;mov al, byte 0
	;mov byte [.a], al	 ;a.
	mov al, byte 0x00     
	mov byte [.r], al	 ;r.
	mov al, byte 0x00
	mov byte [.g], al    ;g.
	mov al, byte 0xFE
	mov byte [.b], al    ;b.
	
	;LFB - Endere�o l�gico do LFB, configurado pelo bootloader.
	;mov   edx, DWORD 0xC0400000 
    ;mov   edi, edx
	
	;BUFFER 1
	mov edi, dword 0xC0800000
	mov edi, edx
	
	;contador de linhas
	mov   ebx, dword 600     ;N�mero de linhas.	    
.bg_newline:     	
	;contador de pixel por linha.
    mov   ecx, dword 800*3    
.bg_newpixel:      
	;mov al, byte [.a]
	;stosb 	
	mov al, byte [.r]
	stosb 	
	mov al, byte [.g]
	stosb 	
	mov al, byte [.b]
	stosb 
    
	loop  .bg_newpixel    ;Pr�ximo pixel. 

    dec  ebx         
	cmp ebx, dword 0
	jne .bg_newline    ;Pr�xima linha.
.exit_bg:
    popad	
    RET
.a db 0		
.r db 0	
.g db 0
.b db 0	




;------------------------------------------------
;  _asm_refresh_screen: 
;      Passa o conte�do do buffer1 para a tela.
;      A tela toda.
;
; @todo: Essa rotina poderia se chamar _softwarelibRefreshScreen.
;
global _asm_refresh_screen
_asm_refresh_screen:		

    ;Sincroniza o retra�o vertical.  
	call _vsync  
	
	push esi
	push edi
	push ecx
	push edx
	
	;
    ; Origem  - BackBuffer.
    ; Destino - LFB.
	mov esi, dword 0xC0800000    ;Endere�o l�gico do backbuffer.
	mov edx, dword 0xC0400000    ;Endere�o logico do LFB. Configurado no bootloader.
    mov edi, edx
	
	
    ;;largura * altura	
	;; Estamos movendo de 4 em 4 bytes.
    mov ecx, dword (800*600)      
    rep movsd                
	
	pop edx
	pop ecx
	pop edi
    pop esi   	
    RET	

	
;-------------------
; write_char:
;     Escreve um caractere no modo texto.
;
write_char:
    ;;Nothing for now.
	ret	
	
	
;
; End.
;

