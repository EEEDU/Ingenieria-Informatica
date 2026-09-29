Q_SHIFT		EQU 	12
; ==============================================================
;  dense_layer_q12_ARM_C
;  Capa fully-connected en Q12: capa en ensamblador ARM,
;  neurona en C (llama a neuron_q12_C cumpliendo el ATPCS).
;
;  uint32_t dense_layer_q12_ARM_C(
;      const int16_t *input,    R0
;      const int16_t *weights,  R1
;      const int16_t *bias,     R2
;      int16_t *output,         R3
;      uint16_t input_size,     [SP+0]  al entrar
;      uint16_t output_size,    [SP+4]
;      int16_t clamp_min,       [SP+8]
;      int16_t clamp_max);      [SP+12]
;  Devuelve el checksum en R0.
;
;  Uso de registros (los que sobreviven al BL, R4-R10):
;    R4  = input
;    R5  = weights_o (puntero a los pesos de la neurona actual)
;    R6  = puntero a bias[o]
;    R7  = puntero a output[o]
;    R8  = input_size
;    R9  = output_size (contador: neuronas que faltan)
;    R10 = checksum
;
;  Marco de pila: 32 bytes (R4-R10 y LR) + 8 bytes (z para
;  los 2 argumentos de pila de neuron_q12_C) = 40 bytes.
; ==============================================================

        AREA    DENSE_ARM_C, CODE, READONLY   ; (anadido, no estaba en el papel)
		PRESERVE8
		EXPORT  dense_layer_q12_ARM_C
        IMPORT  neuron_q12_C

dense_layer_q12_ARM_C PROC 				; PROC: indica que empieza procedimiento
        ; ---------- Prologo / marco de pila ----------
		MOV 	IP, SP					; guarda puntero pila en IP
        STMDB   SP!, {R4-R10, FP, IP, LR}   ; guarda R4-R10, FP, IP y LR
        SUB 	FP, IP, #4				; enfoca FP al siguiente hueco despues de la pila
		SUB     SP, SP, #4        		; hueco de argumentos para neuron_q12_C (1)

        ; ---------- Parametros que llegan en registros ----------
        MOV     R4, R0                	; R4 = input
        MOV     R5, R1                	; R5 = weights_o (empieza en weights)
        MOV     R6, R2                	; R6 = puntero a bias[o]
        MOV     R7, R3                	; R7 = puntero a output[o]

        ; ---------- Parametros que llegan por la pila ----------
        LDMIB   FP, {R8-R9}           	; menor registro = menor direccion:
										; R8 = input_size, R9 = output_size

        MOV     R10, #0               	; checksum = 0
				
		CMP     R9, #0                	; inicio del bucle: quedan neuronas?
        BEQ     dense_layer_q12_ARM_C_fin   ; no: salir

        ; ---------- Bucle sobre las neuronas ----------
dense_layer_q12_ARM_C_bucle

        ; Parametros para llamar a la funcion
        MOV     R0, R4                	; R0 = input
        MOV     R1, R5                	; R1 = weights_o
        MOV     R2, R8              	; R2 = input_size
        LDRSH   R3, [R6]            	; R3 = bias_q12, se cooge el valor, en R6 habia un puntero

		; Meter en la pila parametros de neuron_q12_c
		STR		R10, [FP, #-40]			; Guarda checksum en la pila temporalmente
		LDR     R10, [FP, #12]     		; R10 = clamp_min (que le ha pasado la función antierior)
		LDR     R12, [FP, #16]     		; R12 = clamp_max
		STMDB   SP!, {r10,r12}         	; [SP+0] = clamp_min, [SP+4] = clamp_max (guardar en la pila las varaiables que va a pasar a la func neuron)
		LDR		R10, [FP, #-40]			; Vuelve a meter el valor de checksum a R10
		
        BL      neuron_q12_C          	; llamada a la funcion; resultado y en R0

		ADD		SP, SP, #8			  	; desapilar parametros
        STRH    R0, [R7], #2          	; output[o] = y ; R7 += 2 (post-incremento)

        ADD     R10, R10, R10, LSL #5 	; checksum * 33  (= checksum + checksum*32)
        MOV     R0, R0, LSL #16       	; limpia los 16 bits altos de R0 ...
        MOV     R0, R0, LSR #16       	; ... R0 = (uint16_t)y
        ADD     R10, R10, R0          	; checksum = checksum*33 + y

        ; Actualizar punteros
        ADD     R5, R5, R8, LSL #1    	; weights_o += input_size elementos (2 bytes cada uno)
        ADD     R6, R6, #2            	; puntero a bias

        SUBS    R9, R9, #1            	; output_size - 1
        BNE		dense_layer_q12_ARM_C_bucle

        ; ---------- Epilogo ----------
dense_layer_q12_ARM_C_fin
        MOV     R0, R10               	; return checksum
        LDMDA   FP, {R4-R10, FP, SP, PC}   ; restaura registros y LR
	ENDP
		
		

;  uint32_t dense_layer_q12_ARM(
;      const int16_t *input,    R0
;      const int16_t *weights,  R1
;      const int16_t *bias,     R2
;      int16_t *output,         R3
;      uint16_t input_size,     [SP+0]  al entrar
;      uint16_t output_size,    [SP+4]
;      int16_t clamp_min,       [SP+8]
;      int16_t clamp_max);      [SP+12]

;  Uso de registros:
;     R0  = input (dirección de la matriz de entrada)
;     R1  = weights_o (dirección a los pesos de la neurona actual)
;     R2  = bias[o] (dirección de un elemento de bias[])
;     R3  = output[o] (dirección de un elemento de output[] donde guardar el resultado)
;     R4  = input_size (dato contador: tamaño de la entrada)
;     R5  = output_size (dato contador: neuronas que faltan)
;     R6  = clamp_min (dato limite del resultado)
;	  R7  = clamp_max (dato limite del resultado)
;	  R8  = acc (dato resultado de la multiplicación de als matrices + acc)
;	  R9  = Mul 1 param (dato primer operando de la multiplicación)
;     R10 = Mul 2 param (dato segundo operando de la multiplicación)
;	  R11 = checksum (almacena el dato del chechsum calculado)
dense_layer_q12_ARM PROC
		; ---------- Prologo / marco de pila ----------
		MOV 	IP, SP					; Se guarda SP en IP para posteriormente cargar los parametros de la pila
		STMDB   SP!, {R4-R11, LR}   	; Apilar registros usados para restaurarlos posteriormente
		LDMIA	IP, {r4-r7}				; Carga de parámetros de la pila en trgistros
		
		MOV R11, #0							; checksum

; Asumo que no tiene sentido que input_size o output_size sea 0
		CMP R4, #0
		BEQ dense_layer_q12_ARM_fin
		CMP R5, #0
		BEQ dense_layer_q12_ARM_fin
		MOV IP, R4						; Guardar input_size para poder rearmar bucle 2

dense_layer_q12_ARM_bucle_1
		
		;CMP R4, #0
		;BEQ dense_layer_q12_ARM_bucle_fin_2
		
		LDRSH	R8, [R2], #2			; cargar bias con post incremento y extensi?n de signo
;int32_t acc = ((int32_t)bias_q12) << Q_SHIFT;
		MOV	R8, R8, LSL #Q_SHIFT

		
dense_layer_q12_ARM_bucle_2

		LDRSH R9, [R0], #2				; Carga input[i] con post incremento
		LDRSH R10, [R1], #2				; Carga weight[i] con post incremento
;acc += (int32_t)input[i] * (int32_t)weights[i];		
		MLA R8, R10, R9, R8					; acc += (int32_t)input[i] * (int32_t)weights[i];
		
		SUBS R4, R4, #1					; input_size -1
		BGT dense_layer_q12_ARM_bucle_2

		
dense_layer_q12_ARM_bucle_fin_2

		MOV R4, IP						; Rearmar bucle 2
		SUB R0, IP, LSL #1				; Apuntar de nuevo al primer dato de input, uso de IP para evitar dependencia

;int32_t result_q12 = acc >> Q_SHIFT;
		MOV R8, R8, ASR #Q_SHIFT


;    if (x < lo) return lo;
;    if (x > hi) return hi;
		CMP		R8, R6
		MOVLT	R8, R6
		CMP		R8, R7
		MOVGT	R8, R7
; output[o] = y;
		STRH    R8, [R3], #2			; Guardar el resutado con post incremento
		ADD     R11, R11, R11, LSL #5 ; checksum * 33  (= checksum + checksum*32)
        MOV     R8, R8, LSL #16       ; limpia los 16 bits altos de R0 ...
        MOV     R8, R8, LSR #16       ; ... R0 = (uint16_t)y
        ADD     R11, R11, R8          ; checksum = checksum*33 + y

		SUBS R5, R5, #1					; output_size -1
		BGT dense_layer_q12_ARM_bucle_1
		
				; ---------- Epilogo ----------		
dense_layer_q12_ARM_fin
        MOV     R0, R11               ; return checksum
        LDMIA   SP!, {R4-R11, PC}   ; restaura registros y LR
	ENDP


dense_layer_q12_THB PROC
	
	
	ENDP

	
		END
