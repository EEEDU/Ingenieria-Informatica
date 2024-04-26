		AREA datos,DATA,READWRITE	; area de datos
op_sum	EQU 0
op_res	EQU 1
si_des	EQU 1
no_des	EQU 0
	
op1		DCQ	0x123456789ABCDEF0
op2		DCQ	0xFEDCBA1234565789

suma	DCQ 0
resta 	DCQ 0
ds_sum	DCB	0
ds_res	DCB 0

    	AREA prog,CODE,READONLY		; area de codigo
    	ENTRY						; primera instruccion a ejecutar
		; LLAMADA SBR SUMA
		sub sp,sp,#12				; espacio para los resultados
		LDR r0, =op1				; r0=@op1
		PUSH {r0}					; apilar op1 (ref)
		LDR r0, =op2				; r0=@op2
		PUSH {r0}					; apilar op2 (ref)		
		mov r0, #op_sum				; r0=0	(operador suma)
		PUSH {r0}					; apilar codigo operaci�n (val)
		bl sumres					; llama a sumres
		add	sp,sp,#12				; liberar parametros
		POP {r0-r1}					; r1:r0 = op1+op2
		LDR	r2,=suma				; r2=@suma
		str r0,[r2],#4				; almacenar suma -
		str r1,[r2]					; almacenar suma +
		POP {r0}					; r0=ind. desboradmiento de suma
		LDR	r1,=ds_sum				; r1=@ds_sum
		strb r0,[r1]				; ds_sum = ind. desbordamiento de suma
		
		; LLAMADA SBR RESTA
		sub sp,sp,#12				; espacio para los resultados
		LDR r0, =op1				; r0=@op1
		PUSH {r0}					; apilar op1 (ref)
		LDR r0, =op2				; r0=@op2
		PUSH {r0}					; apilar op2 (ref)		
		mov r0, #op_res				; r0=0	(operador suma)
		PUSH {r0}					; apilar codigo operaci�n (val)
		bl sumres					; llama a sumres
		add	sp,sp,#12				; liberar parametros
		POP {r0-r1}					; r1:r0 = op1+op2
		LDR	r2,=resta				; r2=@suma
		str r0,[r2],#4				; almacenar suma -
		str r1,[r2]					; almacenar suma +
		POP {r0}					; r0=ind. desboradmiento de suma
		LDR	r1,=ds_sum				; r1=@ds_sum
		strb r0,[r1]				; ds_sum = ind. desbordamiento desuma 
		
fin		b fin

sumres	PUSH {lr}					; apilar @ret
		PUSH {r11}					; apilar r11
		mov fp,sp					; r11=fp=frame pointer
		PUSH {r0-r4}				; apilar registros utilizados
		ldr r0,[fp,#16]				; r0=@op1 (ref)
		ldr r1,[r0],#4				; r1=op1 (-)
		ldr r2,[r0]					; r2=op1 (+)
		ldr r0,[fp,#12]				; r0=@op2 (ref)
		ldr r3,[r0],#4				; r3=op2 (-)
		ldr r4,[r0]					; r4=op2  (+)
		cmp r0,#op_sum				
		bne res						; salta si cod. op. = 1 (resta)
		; suma
		adds r0,r1,r3				; r0=op1+op2(-)	actualizar flags
		adcs r1,r2,r4				; r1=op1+op2+C(+) actualizar flags
		b desb	
		; resta
res		subs r0,r1,r3				; r0=op1-op2(-)	actualizar flags
		sbcs r1,r2,r4				; r1=op1-op2-(1-C)(+) actualizar flags
desb	movvs r2,#1					; r2=1 si V=1
		movvc r2,#0					; r2=0 si V=0
		str r0,[fp,#20]				; [fp+20]=resultado(-)
		str r1,[fp,#24]				; [fp+24]=resultado(+)
		str r2,[fp,#28]				; [fp+28]=indicador desbordamiento
		POP {r0-r4}					; recuperar registros utilizados
		POP {r11}					; recuperar r11 (ya no hay fp) 
		POP {pc}					; retorno a PP
		
		END