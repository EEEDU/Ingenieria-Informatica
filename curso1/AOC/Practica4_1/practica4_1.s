		AREA datos,DATA,READWRITE	; area de datos
N		EQU	10
N_mem	EQU N*4
T 		DCD 4,8,5,9,1,6,2,3,7,0
stotal	DCD 0
desb	DCB 0


    	AREA prog,CODE,READONLY		; area de codigo
    	ENTRY						; primera instruccion a ejecutar
		LDR r0,=T					; r0=@T
		LDR r1,=N					; r1=@N
		ldr r2,[r1]					; r2=N
		PUSH {r0}					; apilar r0
		PUSH {r1}					; apilar r1
		bl ordena					; llama a suma
		add sp,sp,#8				; libera parametros
fin		b fin
		
ordena	PUSH {lr,r11}				; apilar @ret y r11
		mov fp,sp					; r11=fp=frame pointer
		PUSH {r0-r8}				; apilar registros utilizados
		ldr r0,[fp,#12]				; r0=@T (ref)
		ldr r1,[fp,#8]				; r1=N (val)
		sub r1,r1,#1				; r1=r1-1
		add r2,r0,r1,lsl #2			; r2=@T+(N-1)*4
		ldr r3,[r2]					; r3=Mem[@T+N-1*4]
		mov r1,r1,lsr #1			; r1=r1/2
		add r4,r0,r1,lsl #2			; r4=@T+(N-1)/2*4
		ldr r5,[r4]					; r5=[r4] = pivote
		ldr r6,[r0]					; r6=[r0]
		mov r7,r0					; r7=r0
		mov r8,r2					; r8=r2
		
sltizq	cmp r6,r5					; if lista[izq]>=pivote
		bhs	sltder					; no salta izquierda
		add r0,r0,#4				; r0=r0+4 izq++
		ldr r6,[r0]					; r6=[r0]
		cmp r6,r5					; if lista[izq]<pivote
		blo sltizq					; salta izquierda
sltder	cmp r3,r5					; if lista[der]<=pivote
		bls noslt					; no salta derecha
		sub r2,r2,#4				; r2=r2-4
		ldr r3,[r2]					; r3=[r2]
		cmp r3,r5					; if lista[der]>pivote
		bhi	sltder					; salta derecha
noslt	cmp r0,r2					; if izq>der
		bhi nocam					; no cambia
		cmp r0,r4					; if r0=l4
		mov r4,r2					; r4=r2
		cmp r2,r4					; if r2=r4
		mov r4,r0					; r4=r0
		str r6,[r2]					; Mem[r2]=r6
		str r3,[r0]					; Mem[r0]=r3
		add r0,r0,#4				; r0=r0+4 izq++
		ldr r6,[r0]					; r6=[r0]
		sub r2,r2,#4				; r2=r2-4 der--
		ldr r3,[r2]					; r3=[r2]
nocam	cmp r0,r2					; if izq<=der
		bls sltizq					; salta a izq
		
		cmp r7,r2					; if limite_izq >= der
		bhs nOrdIz					; no ordenar izq
		PUSH {r7}					; apilamos limite izq
		sub r1,r2,r7				; r1=@T+(cantidadIzq-1)*4 - @T
		mov r1,r1,lsr#2				; r1=r1/4
		add r1,r1,#1				; r1=r1=cantidadIzq
		PUSH {r1}					; apilar r1
		bl ordena					; llamada recusiva a SBR ordena
		add sp,sp,#8				; liberar parametros
nOrdIz  cmp r8,r0					; if limite_der <= izq
		bls finish					; finish sbr
		PUSH {r2}					; NO SE SI ESTA BIEN
		sub r1,r2,r7				; r1=@T+(cantidadIzq-1)*4 - @T
		mov r1,r1,lsr#2				; r1=r1/4
		add r1,r1,#1				; r1=r1=cantidadIzq
		sub r3,r8,r7				; r1=@T+(cantidadIzq-1)*4 - @T
		mov r3,r3,asr#2				; r1=r1/4
		add r3,r3,#2				; r1=r1=cantidadIzq
		sub r3,r3,r1				; r3=r3-r1
		PUSH {r3}
		bl ordena
		add sp,sp,#8
		; if (limite_izq < der) { quickSort(lista,limite_izq,der); }
		; if (limite_der > izq) { quickSort(lista,izq,limite_der); } 
		
finish	POP {r0-r8}					; recuperar registros utilizados
		POP {r11}					; recuperar r11 (ya no hay fp) 
		POP {pc}					; retorno a PP		

		END