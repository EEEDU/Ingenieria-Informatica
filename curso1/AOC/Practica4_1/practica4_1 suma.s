		AREA datos,DATA,READWRITE	; area de datos
N		EQU	10
N_mem	EQU N*4
T 		DCD 4,8,5,9,1,6,2,7,3,0
stotal	DCD 0
desb	DCB 0


    	AREA prog,CODE,READONLY		; area de codigo
    	ENTRY						; primera instruccion a ejecutar
		sub sp,sp,#4				; espacio para los resultados
		LDR r0,=T					; r0=@T
		LDR r1,=N					; r1=@N
		ldr r2,[r1]					; r2=N
		PUSH {r0}					; apilar r0
		PUSH {r1}					; apilar r1
		bl suma						; llama a suma
		add sp,sp,#8				; libera parametros
		POP {r0}					; r0=op1+op2
		LDR r2,=stotal				; r2=@ntotatl
		str r0,[r2]					; almacenar suma
		POP {r0}					; r0=ind. desbordamiento de suma
		LDR	r1,=desb				; r1=@ds_sum
		strb r0,[r1]				; ds_sum = ind. desbordamiento desuma 
		add r3,r3,#4				; r3=r3+4
		cmp r3,N_mem				; if r3<=N_mem
fin		b fin
		
suma	PUSH {lr}					; apilar @ret
		PUSH {r11}					; apilar r11
		mov fp,sp					; r11=fp=frame pointer
		PUSH {r0-r5}				; apilar registros utilizados
		ldr r0,[fp,#12]				; r0=@T (ref)
		ldr r1,[fp,#8]				; r1=N (val)
		mov r3,#0					; r3=0
buc 	ldr r5,[r0,r3]				; r5=Mem[r0+r3]
		LDR r2,=stotal				; r2=@stotal
		ldr r4,[r2]					; r4=stotal
		add r5,r5,r4				; r5=r5+r4
		str r5,[r2]					; stotal=r5
		sub r1,r1,#1				; r1=r1-1
		cmp r1,#0					; if r1>0
		bhi buc						; saltar al buc
		
		str r5,[fp,#16]				; [fp+16]=resultado=stotal
		
		POP {r0-r5}					; recuperar registros utilizados
		POP {r11}					; recuperar r11 (ya no hay fp) 
		POP {pc}					; retorno a PP		

		END