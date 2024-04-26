		AREA datos, DATA
n		DCD 4
fact 	DCD 0
		
		
		AREA codigo,CODE
		ENTRY
		sub sp,sp,#4			; espacio para resultados
		LDR r1,=n				; r1=@n
		ldr r0,[r1]				; r0=n
		PUSH {r0}				; apilar n
		bl facto				; llamar facto
		add sp,sp,#4			; liberar parametros 
		POP {r0}				; desapilar resultado r0=n!
		LDR r1,=fact			; r1=@fact
		str r0,[r1]				; fact=r0
 
fin 	b fin

		; SBR recursiva FACTO
facto 	PUSH {lr,r11}			; apilar @ret y r11
		mov fp,sp				; r11=fp=frame pointer
		PUSH {r0,r1}			; apilar registros
		ldr r0,[fp,#8]			; coger parametros r0=n
		cmp r0,#1				; if r0=n=0
		beq fins				; saltar a fins
		sub r1,r0,#1			; r1=r0-1
		sub sp,sp,#4			; espacio para resultados
		PUSH {r1}				; apilar n-1
		bl facto				; llamada recusiva a SBR facto
		add sp,sp,#4			; liberar parametros
		POP {r1}				; desapilar resultado r1=n-1!
		mul r0,r1,r0			; r0=n*(n-1)!=n!
fins	str r0,[fp,#12]			; almacenar en resultado r0=n!
		POP {r0-r1,r11,pc}		; desapilar 
		END