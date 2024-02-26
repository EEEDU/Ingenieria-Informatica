		AREA datos,DATA,READWRITE	; area de datos
tabla	SPACE 2048					; reservar bloque de memoria
numfin	EQU 1024					; numero final de la tabla
									; espacio en el que inicia el bloque 1 (0)
ini2	EQU 512						; espacio en el que inicia el bloque 2
ini3	EQU 1024					; espacio en el que inicia el bloque 3
ini4	EQU 1536					; espacio en el que inicia el bloque 4

		AREA prog,CODE,READONLY		; area de codigo
		ENTRY						; primera instruccion a ejecutar
		
		LDR r0,=tabla				; r0=direccion comienzo tabla
		eor r1,r1,r1				; r1=0 (r1=numero de la lista)
		mov r2,r1					; r2=0 (r2=direcciones datos)


buc		strh r1,[r0,r2]				; Mem[r0+r2]=r1 (almacena r1 en la memoria de r2+r0)
		add r1,r1,#1				; r1=r1+1 (aumenta a en un numero)
		add r2,r2,#2				; r2=r2+2 (r0 apunta a la siguiente media palabra)
		cmp r1,#numfin				; compara r1 con el numero final (1023)
		bne buc						; salta a buc si r1<>numfin
		
		eor r2,r2,r2				; r2 = 0 (@inicio bloque 1)
		mov r3,#ini2				; r3 = 512 (@inicio bloque 2)
		mov r4,#ini3				; r4 = 1024 (@inicio bloque 3)
		mov r5,#ini4				; r5 = 1536 (@inicio bloque 4)

bucmov  ldrh r6,[r0,r2]				; r6=Mem[r0+r2] (carga en r6 el bloque 1)
		ldrh r7,[r0,r3]				; r7=Mem[r0+r3] (carga en r7 el bloque 2)
		ldrh r8,[r0,r4]				; r8=Mem[r0+r4] (carga en r8 el bloque 3)
		ldrh r9,[r0,r5]				; r9=Mem[r0+r5] (carga en r9 el bloque 4)
		strh r6,[r0,r5]				; Mem[r0+r5]=r6 (carga en memoria del bloque 4 r6)
		strh r7,[r0,r2]				; Mem[r0+r2]=r7 (carga en memoria del bloque 1 r6)
		strh r8,[r0,r3]				; Mem[r0+r3]=r8 (carga en memoria del bloque 2 r6)
		strh r9,[r0,r4]				; Mem[r0+r4]=r9 (carga en memoria del bloque 3 r6)
		add r2,r2,#2				; r2=r2+2
		add r3,r3,#2				; r3=r3+2
		add r4,r4,#2				; r4=r4+2
		add r5,r5,#2				; r5=r5+2
		cmp r2,#ini2				; compara r2 con el inicio del bloque 2
		bne bucmov					; salta a bucmov si r2<>ini2

	
fin		b fin						; fin del programa
		
		END							; fin ensamblador
		