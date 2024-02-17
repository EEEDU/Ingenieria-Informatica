		AREA datos,DATA,READWRITE	; area de datos
tabla	SPACE 2048					; reservar bloque de memoria
numfin	EQU 1024					; numero final de la tabla

		AREA prog,CODE,READONLY		; area de codigo
		ENTRY						; primera instruccion a ejecutar
		
		eor r0,r0,r0				; r0=0 (r0=direcciones datos)
		mov r1,r0					; r1=r0=0 (r1=numero de la lista)
		LDR r2,=tabla				; r2=direccion comienzo tabla

buc		strh r1,[r2,r0]				; Mem[r2+r0]= r1 (almacena r1 en la memoria de r2+r0)
		add r1,r1,#1				; r1 = r1 + 1 (aumenta a en un numero)
		add r0,r0,#2				; r0 = r0 + 2 (r0 apunta a la siguiente media palabra)
		cmp r1,#numfin				; compara r1 con el numero final (1023)
		bne buc						; salta a buc si r1<>numfin

	
fin		b fin						; fin del programa
		
		END							; fin ensamblador
		