[GLOBAL enter_v8086]
[GLOBAL leave_v8086]

[SECTION .text]

;enter_v8086:
;	mov	ebp,esp
;	push dword [ebp+4]
;	push dword [ebp+8]
;	pushfd
;	or dword [esp],(1<<17)
;	push dword [ebp+12]
;	push dword [ebp+16]
;	iret

leave_v8086:
	mov ax,0x4F00
	int 0x10;
	ret

