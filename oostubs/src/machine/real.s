RMI_IDT equ 8

[GLOBAL enter_real]
[GLOBAL leave_real]

[EXTERN idt_descr]

[SECTION .text]

; extern void enter_real(struct realmode_info *)
enter_real:
	cli

	sidt [rdi+RMI_IDT]

;	mov ax,[rdi]
;	mov ss,ax
;	mov esp,[rdi+2]
;	mov ax,[rdi+6]
;	mov ds,ax
;	mov es,ax
;	mov gs,ax
;	mov fs,ax

	mov rax,cr0
	and eax,0x7FFFFFFE
	mov cr0,rax

	mov ecx,0xC0000080
	rdmsr
	btc eax,8
	wrmsr

	mov ebx,0
	lidt [ebx]

	mov ax,0
	mov es,ax
	mov ax,0x4F00
	sti
	int 0x10

	ret


; extern void leave_real(struct realmode_info *, struct toc *)
leave_real:
	lidt [idt_descr]

	mov ecx,0xC0000080
	rdmsr
	or eax,0x00000100
	wrmsr

	mov rax,cr0
	or eax,0x80000001
	mov cr0,rax

[BITS 32]
	jmp 2 * 0x8 : .longmode_start

.longmode_start:
	ret

