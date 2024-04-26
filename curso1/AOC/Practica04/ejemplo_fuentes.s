		AREA datos,DATA,READWRITE	; area de datos
var_s	DCD 5
	
		EXPORT var_s
		EXPORT sbr_s
			
		EXTERN var_c
		EXTERN funcion_c

    	AREA prog,CODE,READONLY		; area de codigo
    	ENTRY						; primera instruccion a ejecutar
sbr_s	;r0=p1
		;r1=p2
		;resultado en r0=p1+p2+var_c+funcion_c(p1,p2)
		PUSH {lr,r1-r3}
		add r3,r0,r1
		bl funcion_c
		add	r0,r0,r3
		ldr r1,=var_c
		ldr r1,[r1]
		add r0,r0,r1
		POP {r1-r3,pc}
		
		END