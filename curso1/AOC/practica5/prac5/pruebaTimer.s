				AREA datos,DATA
VICIntEnable 	EQU 0xFFFFF010			; act. Irqs (solo 1?s)
VICIntEnClear 	EQU 0xFFFFF014			; desact. IRQs (solo 1?s)
VICVectAddr0 	EQU 0xFFFFF100			; vector interr. (VI)
VICVectAddr		EQU 0xFFFFF030			; reg. para EOI

TO_IR			EQU 0xE0004000			; reg. bajar IRQ[4]
 
MAX_TICS		EQU 10					; centesimas de segundo en una decima
 
timer_so		DCD 0					; var para @RSI_timer_SO
contador		DCD 100					; decimas de segundo que faltan
tics			DCB 0					; contador centesimas de segundo

				AREA codigo,CODE
				EXPORT inicio			; forma de enlazar con el startup.s
				; IMPORT srand			; para poder invocar SBR srand
				; IMPORT rand				; para poder invocar SBR rand
inicio			; se recomienda poner punto de parada (breakpoint) en la primera
				; instruccion de c�digo para poder ejecutar todo el Startup de golpe
				LDR r0,=VICVectAddr0	; r0=@VI
				LDR r1,=timer_so		; r1=@tecl_so
				mov r2,#4				; r2=7
				ldr r3,[r0,r2,LSL #2]	; r3=VI[7]=@RSI_tecl_SO
				str r3,[r1]				; tecl_so=@RSI_tecl_SO
				
				LDR r1,=RSI_teclado		; r1=@RSI_teclado mia
				str r1,[r0,r2,LSL #2]	; VI[7]=@RSI_teclado mia
				
				LDR r0,=VICIntEnable	; r0=@VicIntEnable
				mov r1,#0x10			; r1=#2_10000000
				str r1,[r0]				; VICIntEnable[7]=1
				
				LDR r0,=contador		; r0=@terminar
bucle			ldr r1,[r0]				; r1=terminar
				cmp r1,#0				; if terminar = 0
				bne bucle				; salta a bucle
				
				LDR r0,=VICIntEnClear	; r0=@VICIntEnClr
				mov	r1,#0x10; r1=#2_10000000
				str r1,[r0]				; VICIntEnable[7]=0
				
				LDR r0,=VICVectAddr0	; r0=@VI
				LDR r1,=timer_so			
				ldr r1,[r1]				; r1=@RSI_tecl_SO
				mov r2,#4				; r2=7
				str r1,[r0,r2,LSL #2]	; VI[7]=@RSI_tecl_SO
				
fin				b fin					; fin programa principal
				


RSI_teclado								; mi RSI teclado por UART1
				sub lr,lr,#4			; correccion @ret. (segment.)
				PUSH {lr}				; apilar retorno
				mrs r14,spsr			; r14=cpsr prog interr.
				PUSH {r14}				; apilar estado porg interr.
				msr cpsr_c,#2_01010010	; I=0 -> act.IRQs (modo IRQ)
				
				PUSH {r0-r1}			; apilar registros utilizados
				
				LDR r0,=TO_IR			; escribir 1 en TO_IR
				mov r1,#1				; para
				str r1,[r0]				; bajar peticion IRQ[4]
				
				LDR r0,=tics			; r0=@tics
				ldr r1,[r0]				; r1=tics
				add r1,r1,#1			; r1=tics+1
				cmp r1,#MAX_TICS		; if tics != 10	
				strne r1,[r0]			; tics = tics+1
				bne fintimer			; salta si tics!= 0
				
				eor r1,r1,r1			; r1=0
				str r1,[r0]				; tics=0
				LDR r0,=contador		; r0=@contador
				ldr r1,[r0]				; r1=contador
				sub r1,r1,#1			; r1=contador-1
				str r1,[r0]				; contador=contador-1
				

fintimer		POP {r0-r1}				; desapilar registros utilizados
				
				msr cpsr_c,#2_11010010	; I=1 -> desactivar Interr. IRQ
				POP {r14}				; desapilar cpsr prog. interrumpido
				msr spsr_fsxc,r14		; spsr_cpsr porg. intrrumpido
				LDR r14,=VICVectAddr	; EOI r14=@VICVectAddr
				str r14,[r14]			; EOI escribir VICVectAddr
				POP {pc}^				; retorno al programa interrumpido
				
				END
