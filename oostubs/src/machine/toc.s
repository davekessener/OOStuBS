; $Id: toc.asm 9719 2018-12-05 09:57:29Z df $

;******************************************************************************
;* Betriebssysteme                                                            *
;*----------------------------------------------------------------------------*
;*                                                                            *
;*                               T O C . A S M                                *
;*                                                                            *
;*----------------------------------------------------------------------------*
;*
;******************************************************************************

%include "machine/toc.inc"

%macro load_from_toc 1
	mov	rbx,[%1+rbx_offset]
	mov	r12,[%1+r12_offset]
	mov	r13,[%1+r13_offset]
	mov	r14,[%1+r14_offset]
	mov	r15,[%1+r15_offset]
	mov	rbp,[%1+rbp_offset]
	mov	rsp,[%1+rsp_offset]
%endmacro

%macro save_to_toc 1
	mov	[%1+rbx_offset],rbx
	mov	[%1+r12_offset],r12
	mov	[%1+r13_offset],r13
	mov	[%1+r14_offset],r14
	mov	[%1+r15_offset],r15
	mov	[%1+rbp_offset],rbp
	mov	[%1+rsp_offset],rsp
%endmacro

; EXPORTIERTE FUNKTIONEN

[GLOBAL toc_switch]
[GLOBAL toc_go]

; IMPLEMENTIERUNG DER FUNKTIONEN

[SECTION .text]

; TOC_GO : Startet den ersten Prozess ueberhaupt.
;
; C Prototyp: void toc_go (struct toc* regs);

toc_go:
	load_from_toc rdi
	ret

; TOC_SWITCH : Prozessumschaltung. Der aktuelle Registersatz wird
;              gesichert und der Registersatz des neuen "thread of control"
;              wird in den Prozessor eingelesen.
;
; C Prototyp: void toc_switch (struct toc* regs_now,
;                              struct toc* reg_then);

toc_switch:
	save_to_toc rdi
	load_from_toc rsi
;	mov rdi,rsi
	mov rdi,[rsp+8]
	ret

