[GLOBAL lsb]
lsb:
	push ebp
	mov ebp, esp
	bsf eax, [esp+8]
	jne fin
	mov eax, -1
fin:
	mov esp, ebp
	pop ebp
	ret
