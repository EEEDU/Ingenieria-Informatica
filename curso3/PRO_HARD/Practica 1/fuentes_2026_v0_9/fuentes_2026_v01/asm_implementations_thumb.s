
;  uint32_t dense_layer_q12_THB(
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

        AREA    DENSE_ARM_C, CODE, READONLY   ; (anadido, no estaba en el papel)
		PRESERVE8
		ARM
		EXPORT  dense_layer_q12_THB
        IMPORT  neuron_q12_C

dense_layer_q12_THB PROC 				; PROC: indica que empieza procedimiento
		SUB 	IP, SP, #4				; guardar siguiente anterior al puntero de pila en IP
		STMDB   SP!, {R4-R11, LR}   	; guarda R4-R10 y LR (8 registros = 32 bytes)
		LDMIB	IP, {r4-r7}				; Desapilar el resto de los parametros
		
		ADR 	IP, into_thumb + 1 		; Processor starts in ARM state
		BX 		IP 						; Inline switch to Thumb state
		THUMB 							; Subsequent instructions are Thumb
into_thumb
		MOV 	R6, #0
        MOV     R11, R6             	; R11 = checksum = 0

        CMP     R5, #0              	; ¿output_size == 0?
        BLE     dense_layer_q12_THB_fin
		
dense_layer_q12_THB_bucle_1
        CMP     R4, #0              	; ¿input_size == 0?
        BLE     dense_layer_q12_THB_fin
		
		; Cargar bias (16 bits con signo) y avanzar puntero R2
        LDRH    R6, [R2]            ; R6 = bias_q12 (sin signo de momento)
        ADDS    R2, R2, #2          ; R2 += 2 (avanza puntero de bias)
        
        ; Extender signo y escalar de Q12 a Q24 (bias << 12)
        LSLS    R6, R6, #16         ; Lleva el bit de signo al bit 31
        ASRS    R6, R6, #4          ; Shift aritmético a la derecha por 4 positions
                                    ; (16 - 4 = 12 desplazamientos a la izquierda con signo)
dense_layer_q12_THB_bucle_2
        MOV     R8, R5
        MOV     R9, R6
        MOV     R10, R7

        LDRH	R6, [r0]
		ADD 	R0, R0, #2			; Carga input[i] con post incremento
		LDRH 	R7, [r1]
		ADD 	R1, R1, #2			; Carga weight[i] con post incremento
										; acc += (int32_t)input[i] * (int32_t)weights[i];		

		MLA R5, R7, R6, R5				; acc += (int32_t)input[i] * (int32_t)weights[i];
		
		SUBS 	r4, r4, #1				; input_size -1
		BGT 	dense_layer_q12_THB_bucle_2


dense_layer_q12_THB_bucle_fin_2
        ; Rearmar bucle 2
        MOV 	R4, IP					; Rearmar bucle 2
		SUB 	R0, IP, LSL #1			; Apuntar de nuevo al primer dato de input, uso de IP para evitar dependencia
		ASR 	R5, R5, #Q_SHIFT 	    ;int32_t result_q12 = acc >> Q_SHIFT;

        MOV     R6, R9                  ; recuperar registros
        MOV     R7, R10
        MOV     R9, R8                  ; intercambiar r5 por r8 (en r8 deja acc)
        MOV     R8, R5
        MOV     R5, R9


		CMP     R8, R6              ; Compara acc con clamp_min (R6)
        BGE     thb_check_max       ; Si acc >= clamp_min, pasa a comprobar el máximo
        MOV     R8, R6              ; Si acc < clamp_min -> acc = clamp_min
        B       thb_clamp_done

thb_check_max
        CMP     R8, R7              ; Compara acc con clamp_max (R6)
        BLE     thb_clamp_done      ; Si acc <= clamp_max, está dentro del rango
        MOV     R8, R6              ; Si acc > clamp_max -> acc = clamp_max

thb_clamp_done

        MOV     R9, R6              ;  guardar registros
        MOV     R10, R7
        MOV     R6, R8              ; R6 = acc
        MOV     R7, R11             ; R7 = checksum

        STRH    R6, [R6], #2			; Guardar el resutado con post incremento
		ADD     R7, R7, R7, LSL #5      ; checksum * 33  (= checksum + checksum*32)
        MOV     R6, R6, LSL #16         ; limpia los 16 bits altos de R0 ...
        MOV     R6, R6, LSR #16         ; ... R0 = (uint16_t)y
        ADD     R7, R7, R8    


        CMP		R8, R6					; if (x < clamp_min) return clamp_min
        BGE skip_mov                    ; Salta si no se cumple saltar mov
		MOVLT	R8, R6
		CMP		R8, R7					; if (x > clamp_max) return clamp_max
		MOVGT	R8, R7

        MOV     R8, R6                  ; R8 = acc   
        MOV     R11, R7                 ; R7 = checksum
        MOV     R6, R9                  ; recuperar registros
        MOV     R7, R10

		subs r5, r5, #1					; output_size -1

		bgt dense_layer_q12_ARM_bucle_1	
	
		; ---------- Epilogo ----------		
dense_layer_q12_ARM_fin_1
        MOV     R0, R11               ; return checksum
        LDMIA   SP!, {R4-R11, PC}   ; restaura registros y LR
	
	ENDP	
		
		END
