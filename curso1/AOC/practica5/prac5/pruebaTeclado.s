				AREA datos,DATA
VICIntEnable 	EQU 0xFFFFF010			; act. Irqs (solo 1?s)
VICIntEnClear 	EQU 0xFFFFF014			; desact. IRQs (solo 1?s)
VICVectAddr0 	EQU 0xFFFFF100			; vector interr. (VI)
VICVectAddr		EQU 0xFFFFF030			; reg. para EOI

RDAT			EQU 0xE0010000			; reg datos tec (UART1)
 
IOSET			EQU 0xE0028004			; GPIO (activar bits)
IOCLR			EQU 0xE002800C			; GPIO (desactivar bits)
 
tecl_so			DCD 0					; var para @RSI_tecl_SO
terminar		DCD 0					; indicador de terminacion

				AREA codigo,CODE
				EXPORT inicio			; forma de enlazar con el startup.s
				; IMPORT srand			; para poder invocar SBR srand
				; IMPORT rand				; para poder invocar SBR rand
inicio			; se recomienda poner punto de parada (breakpoint) en la primera
				; instruccion de c�digo para poder ejecutar todo el Startup de golpe
				LDR r0,=VICVectAddr0	; r0=@VI
				LDR r1,=tecl_so			; r1=@tecl_so
				mov r2,#7				; r2=7
				ldr r3,[r0,r2,LSL #2]	; r3=VI[7]=@RSI_tecl_SO
				str r3,[r1]				; tecl_so=@RSI_tecl_SO
				
				LDR r1,=RSI_teclado		; r1=@RSI_teclado mia
				str r1,[r0,r2,LSL #2]	; VI[7]=@RSI_teclado mia
				
				LDR r0,=VICIntEnable	; r0=@VicIntEnable
				mov r1,#2_10000000		; r1=#2_10000000
				str r1,[r0]				; VICIntEnable[7]=1
				
				LDR r0,=terminar		; r0=@terminar
bucle			ldr r1,[r0]				; r1=terminar
				cmp r1,#0				; if terminar = 0
				beq bucle				; salta a bucle
				
				LDR r0,=VICIntEnClear		; r0=@VICIntEnClr
				mov	r1,#2_10000000		; r1=#2_10000000
				str r1,[r0]				; VICIntEnable[7]=0
				
				LDR r0,=VICVectAddr0	; r0=@VI
				LDR r1,=tecl_so			
				ldr r1,[r1]				; r1=@RSI_tecl_SO
				mov r2,#7				; r2=7
				str r1,[r0,r2,LSL #2]	; VI[7]=@RSI_tecl_SO
				
fin				b fin					; fin programa principal
				


RSI_teclado								; mi RSI teclado por UART1
				sub lr,lr,#4			; correccion @ret. (segment.)
				PUSH {lr}				; apilar retorno
				mrs r14,spsr			; r14=cpsr prog interr.
				PUSH {r14}				; apilar estado porg interr.
				msr cpsr_c,#2_01010010	; I=0 -> act.IRQs (modo IRQ)
				
				PUSH {r0-r2}			; apilar registros utilizados
				
				LDR r1,=RDAT			; r1=@reg. Datos teclado
				ldrb r0,[r1]			; r0=codigo ASCII tecla
				
				bic r0,r0,#2_10000000	; paso a MAYUSCULAS
				cmp r0,#0x5A				; if tecla != 'Q'
				bne sigue				; salta a sigue
				
				LDR r1,=terminar		; termianr=1
				mov r0,#1				; si se ha pulsado
				str r0,[r1]				; tecla 'Q'
				b fintec
				
sigue			sub r0,r0,#65			; r0=numero letra (i)
				cmp	r0, #25				; CONTROL 0<=I<=25
				bhi fintec				; salta si no es una letra
				LDR r1,=IOCLR			; r1=@IOCLR
				mvn r2,#0				; r2=0xFFFFFFFF
				str r2,[r1]				; IOSET=0
				LDR r1,=IOSET			; r1=@IOSET
				mov r2,#1				; r2=1
				mov r2,r2,LSL r0		; bit r2[i]=1
				str r2,[r1]				; bit IOSET[i]=1
				
fintec			POP {r0-r2}				; desapilar registros utilizados
				
				msr cpsr_c,#2_11010010	; I=1 -> desactivar Interr. IRQ
				POP {r14}				; desapilar cpsr prog. interrumpido
				msr spsr_fsxc,r14		; spsr_cpsr porg. intrrumpido
				LDR r14,=VICVectAddr	; EOI r14=@VICVectAddr
				str r14,[r14]			; EOI escribir VICVectAddr
				POP {pc}^				; retorno al programa interrumpido
				
				END
