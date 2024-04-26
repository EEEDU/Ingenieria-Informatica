		AREA datos,DATA,READWRITE	; area de datos
N		EQU	10
N_mem	EQU N*4
T 		DCD 2,2,2,2,2,2,2,2,2,2
ntotal	DCD 0
desb	DCB 0


    	AREA prog,CODE,READONLY		; area de codigo
    	ENTRY						; primera instruccion a ejecutar
		mov r3,#0					; r3=0
buc		sub sp,sp,#8				; espacio para los resultados
		LDR r0,=T					; r0=@T
		add r0,r0,r3				; r0=r0+r3
		LDR r1,=ntotal				; r1=@ntotal
		PUSH {r1}					; apilar r2
		PUSH {r0}					; apilar r2
		bl suma						; llama a suma
		add sp,sp,#8				; libera parametros
		POP {r0}					; r0=op1+op2
		LDR r2,=ntotal				; r2=@ntotatl
		str r0,[r2]					; almacenar suma
		POP {r0}					; r0=ind. desbordamiento de suma
		LDR	r1,=desb				; r1=@ds_sum
		strb r0,[r1]				; ds_sum = ind. desbordamiento desuma 
		add r3,r3,#4				; r3=r3+4
		cmp r3,N_mem				; if r3<=N_mem
		blo buc						; b buc (no sustituye)
fin		b fin

		
suma	PUSH {lr}					; apilar @ret
		PUSH {r11}					; apilar r11
		mov fp,sp					; r11=fp=frame pointer
		PUSH {r0-r2}				; apilar registros utilizados
		ldr r0,[fp,#12]				; r0=@op1 (ref)
		ldr r1,[r0]				; r1=op1
		ldr r0,[fp,#8]				; r0=@op2 (ref)
		ldr r2,[r0]					; r2=op2
		; suma
		adds r0,r1,r2				; r0=op1+op2 actualizar flags
		
		movvs r2,#1					; r2=1 si V=1
		movvc r2,#0					; r2=0 si V=0
		str r0,[fp,#16]				; [fp+16]=resultado
		str r2,[fp,#20]				; [fp+20]=indicador desbordamiento
		POP {r0-r2}					; recuperar registros utilizados
		POP {r11}					; recuperar r11 (ya no hay fp) 
		POP {pc}					; retorno a PP		

		END