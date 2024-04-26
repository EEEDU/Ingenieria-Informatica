		AREA codigo, CODE 
		EXPORT ordena 

ordena 	; SBR que ordena una tabla de enteros de 32 bits. Parametros: 
		; r0 = @ de comienzo de la tabla a ordenar (tabla por referencia)  
		; r1 = Numero de elementos de la tabla (num_elem por valor) 
		PUSH {lr} 
		PUSH {r0}
		PUSH {r1}
		bl qksort
		add sp,sp,#8				; liberar parametros
		POP {pc} 
		
		
qksort	PUSH {lr,r11}				; apilar @ret y r11
		mov fp,sp					; r11=fp=frame pointer
		PUSH {r0-r8}				; apilar registros utilizados
		ldr r0,[fp,#12]				; r0=@T (ref) 				= izq
		ldr r6,[r0]					; r6=[r0] 					= lista[izq]
		mov r7,r0					; r7=r0						= limite_izq
		ldr r1,[fp,#8]				; r1=N (val)
		sub r1,r1,#1				; r1=r1-1 					= N-1
		add r2,r0,r1,lsl #2			; r2=@T+(N-1)*4 			= der
		ldr r3,[r2]					; r3=Mem[@T+N-1*4] 			= lista[der]
		mov r8,r2					; r8=r2						= limite_der
		mov r1,r1,asr #1			; r1=r1/2
		add r4,r0,r1,lsl #2			; r4=@T+(N-1)/2*4 			= pivote
		ldr r5,[r4]					; r5=[r4] 					= lista[pivote]
		
		
sltizq	cmp r6,r5					; if lista[izq]>=pivote
		bge	sltder					; no salta izquierda
		add r0,r0,#4				; r0=r0+4 izq++
		ldr r6,[r0]					; r6=[r0]
		cmp r6,r5					; if lista[izq]<pivote
		blt sltizq					; salta izquierda
		
sltder	cmp r3,r5					; if lista[der]<=pivote
		ble noslt					; no salta derecha
		sub r2,r2,#4				; r2=r2-4
		ldr r3,[r2]					; r3=[r2]
		cmp r3,r5					; if lista[der]>pivote
		bgt	sltder					; salta derecha
		
noslt	cmp r0,r2					; if izq>der
		bgt nocam					; no cambia
		str r6,[r2]					; Mem[r2]=r6
		str r3,[r0]					; Mem[r0]=r3
		cmn r0,r8
		add r0,r0,#4				; r0=r0+4 izq++
		ldr r6,[r0]					; r6=[r0]
		cmp r2,r7
		ble nocam
		sub r2,r2,#4				; r2=r2-4 der--
		ldr r3,[r2]					; r3=[r2]
		
nocam	cmp r0,r2					; if izq<=der
		ble sltizq					; salta a izq
		
		cmp r7,r2					; if limite_izq >= der
		bge nOrdIz					; no ordenar izq
		PUSH {r7}					; apilamos limite izq
		sub r1,r2,r7				; r1=der-limite_izq
		mov r1,r1,asr#2				; r1=r1/4
		add r1,r1,#1				; r1=r1+1
		PUSH {r1}					; apilar r1
		bl qksort					; llamada recusiva a SBR qksort
		add sp,sp,#8				; liberar parametros


nOrdIz  cmp r8,r0					; if limite_der <= izq
		ble finish					; finish sbr
		PUSH {r0}					; apilar izq
		sub r1,r8,r0				; r1=limite_der-izq
		mov r1,r1,asr#2				; r1=r1/4
		add r1,r1,#1				; r1=r1+1
		PUSH {r1}					; apilar r3
		bl qksort					; llamada recursiva a SBR qksort
		add sp,sp,#8				; liberar parametros

		
finish	POP {r0-r8}					; recuperar registros utilizados
		POP {r11}					; recuperar r11 (ya no hay fp) 
		POP {pc}					; retorno a PP		

		END
		