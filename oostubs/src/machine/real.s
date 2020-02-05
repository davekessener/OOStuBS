%include "machine/toc.inc"

[GLOBAL enter_real]
[GLOBAL leave_real]

[SECTION .text]

; extern void enter_real(struct realmode_info *, struct toc *)
enter_real:
	cli

	save_to_toc rsi

	sidt [rdi+8]

	mov ax,[rdi]
	mov ss,ax
	mov esp,[rdi+2]
	mov ax,[rdi+6]
	mov ds,ax
	mov es,ax
	mov gs,ax
	mov fs,ax

	mov rax,cr0
	and eax,0x7FFFFFFE
	mov cr0,rax

	mov ecx,0xC0000080
	rdmsr
	btc eax,8
	wrmsr

	mov ebx,0
	lidt [ebx]

	ret


; extern void leave_real(struct realmode_info *, struct toc *)
leave_real:
[BITS 32]
	lidt [edi+6]

	mov ecx,0xC0000080
	rdmsr
	or eax,0x00000100
	wrmsr

	mov eax,cr0
	or eax,0x80000001
	mov cr0,eax

	jmp 2 * 0x8 : .longmode_start

.longmode_start:
	ret

