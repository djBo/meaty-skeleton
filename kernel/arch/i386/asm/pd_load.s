[GLOBAL pd_load]
pd_load:
	push ebp
	mov ebp, esp
	mov eax, [esp+8]
	mov cr3, eax
	mov esp, ebp
	pop ebp
	ret
