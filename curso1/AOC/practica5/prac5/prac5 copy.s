 
				AREA datos, DATA 
VICIntEnable 	EQU   0xFFFFF010			; act. Irqs (solo 1?s)
VICIntEnClear 	EQU   0xFFFFF014			; desact. IRQs (solo 1?s)
VICVectAddr0 	EQU   0xFFFFF100			; vector interr. (VI)
VICVectAddr		EQU   0xFFFFF030			; reg. para EOI

TO_IR			EQU   0xE0004000			; reg. bajar IRQ[4]
RDAT			EQU   0xE0010000			; reg datos tec (UART1)
					  
IOSET			EQU   0xE0028004			; GPIO (activar bits)
IOCLR			EQU   0xE002800C			; GPIO (desactivar bits)
					  
tecl_so			DCD   0						; var para @RSI_tecl_SO
timer_so		DCD   0						; var para @RSI_timer_SO
					  
inicio_pan		EQU   0x40007E00			; inicio pantalla
tam_fila		EQU	  32					; tamaño de la fila de la pantalla
	
MAX_TICS		EQU   10	

reloj  			DCD   0 				;contador de centesimas de segundo 
max  			DCD   8 				;velocidad de movimiento (en centesimas s.) 
cont  			DCD   0 				;instante siguiente movimiento 

ascii1			DCB	  'X'				; ascii del personaje 1
x1				DCD	  0					; posicion x de la cabeza del jugador 1
y1				DCD	  0					; posicion y de la cabeza del jugador 1
x2				DCD	  0					; posicion x de la cabeza del jugador 2
y2				DCD	  0					; posicion y de la cabeza del jugador 2
dir1x  			DCB	  1 				;mov. horizontal jugador1 (-1 izq,0 col fija,1 der) 
dir1y  			DCB   0 				;mov. vertical jugador1 (-1 arriba,0 fila fija,1 abajo) 
dir2x  			DCB   -1 				;mov. horizontal jugador2 (-1 izq.,0 col fija,1 der) 
dir2y  			DCB   0 				;mov. vertical jugador2 (-1 arriba,0 fila fija,1 abajo) 
fin  			DCB   0 				;indicador fin de programa (si vale 1) 
  
				AREA codigo, CODE 
				EXPORT  inicio 			;etiqueta enlace con Startup.s 
inicio  
				;programar @IRQ4 -> RSI_reloj 
				LDR r0,=VICVectAddr0	; r0=@VI
				LDR r1,=timer_so		; r1=@tecl_so
				mov r2,#4				; r2=7
				ldr r3,[r0,r2,LSL #2]	; r3=VI[7]=@RSI_tecl_SO
				str r3,[r1]				; tecl_so=@RSI_tecl_SO
				
				LDR r1,=RSI_reloj		; r1=@RSI_teclado mia
				str r1,[r0,r2,LSL #2]	; VI[7]=@RSI_teclado mia
				;activar IRQ4
				LDR r0,=VICIntEnable	; r0=@VicIntEnable
				mov r1,#0x10			; r1=#2_10000000
				str r1,[r0]				; VICIntEnable[7]=1
				
				;programar @IRQ7 -> RSI_teclado 
				LDR r0,=VICVectAddr0	; r0=@VI
				LDR r1,=tecl_so			; r1=@tecl_so
				mov r2,#7				; r2=7
				ldr r3,[r0,r2,LSL #2]	; r3=VI[7]=@RSI_tecl_SO
				str r3,[r1]				; tecl_so=@RSI_tecl_SO
				
				LDR r1,=RSI_teclado		; r1=@RSI_teclado mia
				str r1,[r0,r2,LSL #2]	; VI[7]=@RSI_teclado mia
				;activar IRQ7 
				LDR r0,=VICIntEnable	; r0=@VicIntEnable
				mov r1,#2_10000000		; r1=#2_10000000
				str r1,[r0]				; VICIntEnable[7]=1
			 
				;dibujar pantalla inicial 
				LDR r0,=inicio_pan		; r0=@inicio_pan=0x40007E00
				mov r1,#'X'				; r1='X'
				mov r2,#2				; r2=x
				mov r3,#1				; r3=y
				ldr r4,=x1				; r4=@x1
				ldr r5,=y1				; r5=@y1
				str r2,[r4]				; x=r2
				str r3,[r5]				; y=r3
				LDR r6,=tam_fila		; r6=32
				mul r7,r3,r6			; r7=r3*r6=y*32
				add r8,r7,r2			; r8=r3*r6+r2=y*32+x		
				strb r1,[r0,r8]			; inicio_pan+posicion='X'
				
				LDR r0,=inicio_pan		; r0=@inicio_pan=0x40007E00
				mov r1,#'O'				; r1='X'
				mov r2,#20				; r2=x
				mov r3,#5				; r3=y
				ldr r4,=x2				; r4=@x1
				ldr r5,=y2				; r5=@y1
				str r2,[r4]				; x=r2
				str r3,[r5]				; y=r3
				LDR r6,=tam_fila		; r6=32
				mul r7,r3,r6			; r7=r3*r6=y*32
				add r8,r7,r2			; r8=r3*r6+r2=y*32+x		
				strb r1,[r0,r8]			; inicio_pan+posicion='X'
				
				LDR r0,=fin
bucle  			ldr r1,[r0]				; r1=terminar
				cmp r1,#0				; if terminar = 0
				bne fin_juego			; salta a bucle
				
				ldr r2,=cont			; r2=@cont
				ldr r1,[r2]				; r1=cont
				cmp r1,#0				; if cont!=0
				bne bucle				; salta a bucle
				
				ldr r1,=max				; r1=@max
				ldr r3,[r1]				; r3=max
				str r3,[r2]				; cont=max
				
				; dibujar jugador 1
				LDR r5,=inicio_pan		; r0=@inicio_pan=0x40007E00
				
				ldr r1,=dir1x			; r1=@dir1x
				ldrsb r7,[r1]			; r7=dir1x
				ldr r2,=x1				; r2=@x1
				ldr r3,[r2]				; r3=x1
				add r3,r3,r7			; r3=r3+r7 ;x1=x1+dir1x
				and r3,#31				; empieza por el otro lado
				str r3,[r2]				; x1=r3
				
				ldr r1,=dir1y			; r1=@dir1x
				ldrsb r7,[r1]			; r7=dir1x
				ldr r2,=y1				; r2=@y1
				ldr r4,[r2]				; r4=y1
				add r4,r4,r7			; r4=r4+r7 ;y1=y1+dir1y
				and r4,#15				; empieza por el otro lado
				str r4,[r2]				; x1=r4
				
				LDR r6,=tam_fila		; r6=32
				mul r7,r4,r6			; r7=r3*r6=y*32
				add r8,r7,r3			; r8=r3*r6+r2=y*32+x
				mov r1,#'X'				; r1='X'
				strb r1,[r5,r8]			; inicio_pan+posicion='X'
				
				
				; dibujar jugador 2
				LDR r5,=inicio_pan		; r0=@inicio_pan=0x40007E00
				
				ldr r1,=dir2x			; r1=@dir1x
				ldrsb r7,[r1]			; r7=dir1x
				ldr r2,=x2				; r2=@x1
				ldr r3,[r2]				; r3=x1
				add r3,r3,r7			; r3=r3+r7 ;x1=x1+dir1x
				and r3,#31				; empieza por el otro lado	
				str r3,[r2]				; x1=r3
				
				ldr r1,=dir2y			; r1=@dir1x
				ldrsb r7,[r1]			; r7=dir1x
				ldr r2,=y2				; r2=@y1
				ldr r4,[r2]				; r4=y1
				add r4,r4,r7			; r4=r4+r7 ;y1=y1+dir1y
				and r4,#15				; empieza por el otro lado
				str r4,[r2]				; x1=r4
				
				LDR r6,=tam_fila		; r6=32
				mul r7,r4,r6			; r7=r3*r6=y*32
				add r8,r7,r3			; r8=r3*r6+r2=y*32+x
				mov r1,#'O'				; r1='X'
				strb r1,[r5,r8]			; inicio_pan+posicion='X'
				
				b bucle
				
				;mientras fin==0 
				;  para cada elemento movil 
				;    si toca mover elemento 
				;      calcular instante siguiente movimiento 
				;      borrar elemento anterior 
				;      calcular nueva posicion (dirx,diry) elemento 
				;      dibujar nuevo elemento 
				;    fin si 
				;  fin para 
				;  si toca añadir elemento movil 
				;    calcular instante siguiente aparicion 
				;    calcular instante siguiente movimiento 
				;    generar posicion inicial 
				;    dibujar nuevo elemento movil 
				;  fin si 
				;fin mientras 
				
				;desactivar IRQ4
fin_juego		LDR r0,=VICIntEnClear	; r0=@VICIntEnClr
				mov	r1,#0x10			; r1=#2_10000000
				str r1,[r0]				; VICIntEnable[7]=0
				;desactivar RSI_reloj 
				LDR r0,=VICVectAddr0	; r0=@VI
				LDR r1,=timer_so			
				ldr r1,[r1]				; r1=@RSI_tecl_SO
				mov r2,#4				; r2=7
				str r1,[r0,r2,LSL #2]	; VI[7]=@RSI_tecl_SO
				
				;desactivar IRQ7 
				LDR r0,=VICIntEnClear	; r0=@VICIntEnClr
				mov	r1,#2_10000000		; r1=#2_10000000
				str r1,[r0]				; VICIntEnable[7]=0
				;desactivar RSI_teclado
				LDR r0,=VICVectAddr0	; r0=@VI
				LDR r1,=tecl_so			
				ldr r1,[r1]				; r1=@RSI_tecl_SO
				mov r2,#7				; r2=7
				str r1,[r0,r2,LSL #2]	; VI[7]=@RSI_tecl_SO
	
bfin  			b	bfin 
    
RSI_reloj 		;Rutina de servicio a la interrupcion IRQ4 (timer 0) 
				;Cada 0,01 s. llega una peticion de interrupcion

				sub lr,lr,#4			; correccion @ret. (segment.)
				PUSH {lr}				; apilar retorno
				mrs r14,spsr			; r14=cpsr prog interr.
				PUSH {r14}				; apilar estado porg interr.
				msr cpsr_c,#2_01010010	; I=0 -> act.IRQs (modo IRQ)
				
				PUSH {r0-r3}			; apilar registros utilizados
				
				LDR r0,=TO_IR			; escribir 1 en TO_IR
				mov r1,#1				; para
				str r1,[r0]				; bajar peticion IRQ[4]
				
				LDR r0,=reloj			; r0=@reloj
				ldr r1,[r0]				; r1=reloj
				add r1,r1,#1			; r1=reloj+1
				str r1,[r0]				; reloj=reloj+1
				
				LDR r0,=cont			; r0=@cont
				ldr r1,[r0]				; r1=cont
				cmp r1,#0				; if cont!=0
				subne r1,r1,#1			; r1=r1-1
				str r1,[r0]				; cont=r1
				

fintimer		POP {r0-r3}				; desapilar registros utilizados
				
				msr cpsr_c,#2_11010010	; I=1 -> desactivar Interr. IRQ
				POP {r14}				; desapilar cpsr prog. interrumpido
				msr spsr_fsxc,r14		; spsr_cpsr porg. intrrumpido
				LDR r14,=VICVectAddr	; EOI r14=@VICVectAddr
				str r14,[r14]			; EOI escribir VICVectAddr
				POP {pc}^				; retorno al programa interrumpido

RSI_teclado								; mi RSI teclado por UART1
				sub lr,lr,#4			; correccion @ret. (segment.)
				PUSH {lr}				; apilar retorno
				mrs r14,spsr			; r14=cpsr prog interr.
				PUSH {r14}				; apilar estado porg interr.
				msr cpsr_c,#2_01010010	; I=0 -> act.IRQs (modo IRQ)
				
				PUSH {r0-r4}			; apilar registros utilizados
				
				LDR r1,=RDAT			; r1=@reg. Datos teclado
				ldrb r0,[r1]			; r0=codigo ASCII tecla
				
				cmp r0,#'+'				; if codigo ASCII = "+"
				beq suma_vel			; salta a suma_vel
				
				cmp r0,#'-'				; if codigo ASCII = "-"
				beq resta_vel			; salta a resta_vel
				
				bic r0,r0,#2_100000		; paso a MAYUSCULAS
				
				sub r0,r0,#65			; r0=numero letra (i)
				cmp	r0, #25				; CONTROL 0<=I<=25
				bhi fintec				; salta si no es una letra
				
				ldr r1,=dir1x
				ldr r2,=dir1y		
				ldrb r3,[r1]
				ldrb r4,[r2]
				cmp r0,#0				; if tecla == "a"
				beq izq					; jugador 1 a la izq
				cmp r0,#3				; if tecla == "d"
				beq der					; jugador 1 a la izq
				cmp r0,#22				; if tecla == "w"
				beq arr					; jugador 1 a la izq
				cmp r0,#18				; if tecla == "s"
				beq aba					; jugador 1 a la izq
				
				ldr r1,=dir2x
				ldr r2,=dir2y		
				ldrb r3,[r1]
				ldrb r4,[r2]
				cmp r0,#9				; if tecla == "j"
				beq izq					; jugador 2 a la izq
				cmp r0,#11				; if tecla == "l"
				beq der					; jugador 2 a la der
				cmp r0,#8				; if tecla == "i"
				beq arr					; jugador 2 a la der
				cmp r0,#10				; if tecla == "k"
				beq aba					; jugador 2 a la der

				cmp r0,#16				; if tecla != 'Q' or tecla != 'q'
				bne sigue				; salta a sigue
				
				LDR r1,=fin				; termianr=1
				mov r0,#1				; si se ha pulsado
				str r0,[r1]				; tecla 'Q'
				b fintec
				
suma_vel		ldr r0,=max				; r0=@max
				ldr r1,[r0]				; r1=max
				cmp r1,#0				; if r1!=0
				movne r1,r1,asr #1		; r1=r1/2 	
				str r1,[r0]				; max=r1
				b sigue

resta_vel		ldr r0,=max				; r0=@max
				ldr r1,[r0]				; r1=max
				cmp r1,#128				; if r1!=128
				addne r1,r1,r1			; r1=r1+r1
				str r1,[r0]				; max=r1
				b sigue
				
izq				cmp r3,#0x000000FF
				subne r3,r3,#1
				mov r4,#0
				strb r3,[r1]
				strb r4,[r2]
				b sigue
der				cmp r3,#1
				addne r3,r3,#1
				mov r4,#0
				strb r3,[r1]
				strb r4,[r2]
				b sigue
arr				cmp r4,#0x000000FF
				subne r4,r4,#1
				mov r3,#0
				strb r4,[r2]
				strb r3,[r1]
				b sigue
aba				cmp r4,#1
				addne r4,r4,#1
				mov r3,#0
				strb r4,[r2]
				strb r3,[r1]
				b sigue
				
sigue			LDR r1,=IOCLR			; r1=@IOCLR
				mvn r2,#0				; r2=0xFFFFFFFF
				str r2,[r1]				; IOSET=0
				LDR r1,=IOSET			; r1=@IOSET
				mov r2,#1				; r2=1
				mov r2,r2,LSL r0		; bit r2[i]=1
				str r2,[r1]				; bit IOSET[i]=1
				
fintec			POP {r0-r4}				; desapilar registros utilizados
				
				msr cpsr_c,#2_11010010	; I=1 -> desactivar Interr. IRQ
				POP {r14}				; desapilar cpsr prog. interrumpido
				msr spsr_fsxc,r14		; spsr_cpsr porg. intrrumpido
				LDR r14,=VICVectAddr	; EOI r14=@VICVectAddr
				str r14,[r14]			; EOI escribir VICVectAddr
				POP {pc}^				; retorno al programa interrumpido
				
				END
