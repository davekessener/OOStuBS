; $Id: io_port.asm 8485 2017-03-27 11:50:06Z friesel $

;*****************************************************************************
;* Betriebssysteme                                                           *
;*---------------------------------------------------------------------------*
;*                                                                           *
;*                             I O _ P O R T                                 *
;*                                                                           *
;*---------------------------------------------------------------------------*
;* Die hier definierten Funktionen stellen die Maschinebefehle 'in' und      *
;* 'out' fuer die Klasse IO_Port zur Verfuegung.                             *
;*****************************************************************************

; EXPORTIERTE FUNKTIONEN

[GLOBAL out_b]
[GLOBAL out_w]
[GLOBAL out_l]
[GLOBAL in_b]
[GLOBAL in_w]
[GLOBAL in_l]

; IMPLEMENTIERUNG DER FUNKTIONEN

[SECTION .text]
	
; OUTB: Byteweise Ausgabe eines Wertes ueber einen I/O-Port.
;
;       C-Prototyp: void outb (int port, int value);

out_b:
	push   rbp
	mov    rbp, rsp
	mov    rdx, rdi
	mov    rax, rsi
	out    dx, al
	pop    rbp
	ret

; OUTW: Wortweise Ausgabe eines Wertes ueber einen I/O-Port.
;
;       C-Prototyp: void outw (int port, int value);

out_w:
	push   rbp
	mov    rbp, rsp
	mov    rdx, rdi
	mov    rax, rsi
	out    dx, ax
	pop    rbp
	ret

; void outl(u32 port, u32 val)
out_l:
	push rbp
	mov rbp,rsp
	mov rdx,rdi
	mov rax,rsi
	out dx,eax
	pop rbp
	ret

; INB: Byteweises Einlesen eines Wertes ueber einen I/O-Port.
;
;      C-Prototyp: int inb (int port);

in_b:
	push   rbp
	mov    rbp, rsp
	mov    rdx, rdi
	mov    rax, rsi
	in     al, dx
	pop    rbp
	ret

; INW: Wortweises Einlesen eines Wertes ueber einen I/O-Port.
;
;      C-Prototyp: int inw (int port);

in_w:
	push   rbp
	mov    rbp, rsp
	mov    rdx, rdi
	mov    rax, rsi
	in     ax, dx
	pop    rbp
	ret

in_l:
	push rbp
	mov rbp,rsp
	mov rdx,rdi
	mov rax,rsi
	in eax,dx
	pop rbp
	ret

