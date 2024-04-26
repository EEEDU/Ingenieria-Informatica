		AREA datos,DATA,READWRITE	; area de datos
M 		EQU 3
N		EQU 4
matriz 	DCB "rfjkolabcghx"


		AREA prog,CODE,READONLY		; area de codigo
		ENTRY						; primera instruccion a ejecutar
		
		LDR r0,=matriz				; r0=direccion comienzo tabla
		mov r1,#N					; r1=N
		sub r1,r1,#1				; r1=i=N-1
		eor r2,r2,r2				; r2=j=0
		mov r4, #M ; r4 = M			; r4=M
		mul r5,r1,r4				; r5=M*i
		
buc		add r6,r2,r5				; r6=j+M*i
		sub r7,r1,#1				; r7=i-1
		mul r5,r7,r4				; r5=M*(i-1)
		add r8,r2,r5				; r8=j+M*(i-1)
		
		ldrb r6,[r6,r0]				; r6 = Mem[r6+r0] (almacena en r6 la memoria de r6+r0)
		ldrb r8,[r8,r0]				; r8 = Mem[r8+r0] (almacena en r8 la memoria de r8+r0)
		
		cmp r8,r6					; if r8<=r6
		bls nosus					; b nosus (no sustituye)
		
		
nosus

		add r2,r2,#M
		b buc
		
		
		
		
fin		b fin						; fin del programa
		
		END							; fin ensamblador
		