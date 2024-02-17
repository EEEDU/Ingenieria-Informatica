		AREA datos,DATA,READWRITE	; area de datos
tabla	SPACE 2046					; reservar bloque de memoria
	
		AREA prog,CODE,READONLY		; area de codigo
		ENTRY						; primera instruccion a ejecutar
		
		eor r0,r0,r0				; r0=0 (r0=dircciones datos)
		mov r1,r0					; r1=r0=0 (r1=numero de la lista)
		
fin		b fin						; fin del programa
		
		END							; fin ensamblador
		