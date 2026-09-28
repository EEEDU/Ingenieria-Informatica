	
;  uint32_t dense_layer_q12_C_THB(
;      const int16_t *input,    R0
;      const int16_t *weights,  R1
;      const int16_t *bias,     R2
;      int16_t *output,         R3
;      uint16_t input_size,     [SP+0]  al entrar
;      uint16_t output_size,    [SP+4]
;      int16_t clamp_min,       [SP+8]
;      int16_t clamp_max);      [SP+12]

;  Uso de registros (los que sobreviven al BL, R4-R10):
;     R0  = input
;     R1  = weights_o (puntero a los pesos de la neurona actual)
;     R2  = puntero a bias[o]
;     R3  = puntero a output[o]
;    R4  = input_size
;    R5  = output_size (contador: neuronas que faltan)
;	 R6 = clamp_min
;	 R7 = clamp_max
;	 R8 = acc
;	 R9 = Mul 1 param
;    R10 = Mul 2 param
;	 R11 = checksum

        AREA    DENSE_ARM_C, CODE16, READONLY   ; (anadido, no estaba en el papel)
		PRESERVE8
		EXPORT  dense_layer_q12_C_THB
        IMPORT  neuron_q12_C

dense_layer_q12_C_THB PROC 				; PROC: indica que empieza procedimiento
		PUSH    {R4-R7, LR}         	; Guardamos R4-R7 y LR en la pila (Thumb usa PUSH/POP)

		; Cargar parametros desde la pila (input_size, output_size, clamp_min, clamp_max)
        ; En el momento de entrar, SP apunta tras los registros guardados
        LDR     R4, [SP, #20]       	; R4 = input_size
        LDR     R5, [SP, #24]       	; R5 = output_size
        LDR     R6, [SP, #28]       	; R6 = clamp_min
        LDR     R7, [SP, #32]       	; R7 = clamp_max

        ; Trasladamos las constantes a registros altos para liberar R6 y R7
		; THUMB no puedo hacer operacioens con registros >R7 pero si que puede usarlos para almacenar
        MOV     R8, R4              	; R8 = copia de input_size
        MOV     R9, R6              	; R9 = clamp_min
        MOV     R10, R7             	; R10 = clamp_max
        
        MOVS    R6, #0
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
        ; Cargar input[i] (16 bits con signo)
        LDRH    R7, [R0]            ; R7 = input[i]
        ADDS    R0, R0, #2          ; input++
        LSLS    R7, R7, #16
        ASRS    R7, R7, #16         ; R7 = (int32_t)input[i] con signo

        ; Cargar weights[i] (16 bits con signo)
        LDRH    R1, [R1]            ; R1 = weights[i]
        ADDS    R1, R1, #2          ; weights++
        LSLS    R1, R1, #16
        ASRS    R1, R1, #16         ; R1 = (int32_t)weights[i] con signo

        ; Multiplicar y acumular
        MULS    R7, R1              ; R7 = input[i] * weights[i] (Thumb obliga R7 = R7 * R1)
        ADDS    R6, R6, R7          ; acc += producto

        SUBS    R4, R4, #1          ; input_size - 1
        BGT     dense_layer_q12_THB_bucle_2
		
		MOV     R4, R8              ; Restaura R4 = input_size
        MOV     R0, R12             ; Restaura R0 al inicio del array input (copiado previamente)

        ASRS    R6, R6, #12         ; acc = acc >> 12 (de Q24 a Q12)
		
		
		CMP     R6, R9              ; Compara acc con clamp_min (R9)
        BGE     thb_check_max       ; Si acc >= clamp_min, pasa a comprobar el máximo
        MOV     R6, R9              ; Si acc < clamp_min -> acc = clamp_min
        B       thb_clamp_done

thb_check_max
        CMP     R6, R10             ; Compara acc con clamp_max (R10)
        BLE     thb_clamp_done      ; Si acc <= clamp_max, está dentro del rango
        MOV     R6, R10             ; Si acc > clamp_max -> acc = clamp_max

thb_clamp_done
        STRH    R6, [R3]            ; Guarda los 16 bits en output[o]
        ADDS    R3, R3, #2          ; output++
		
		
		
		; Preparar (uint16_t)y en R7
        LSLS    R7, R6, #16
        LSRS    R7, R7, #16         ; R7 = (uint16_t)y

        ; checksum * 33
        MOV     R2, R11             ; R2 = checksum actual
        LSLS    R1, R2, #5          ; R1 = checksum * 32
        ADDS    R2, R2, R1          ; R2 = checksum * 33
        ADDS    R2, R2, R7          ; R2 = checksum * 33 + y
        MOV     R11, R2             ; Guarda el nuevo checksum en R11

        SUBS    R5, R5, #1          ; output_size - 1
        BGT     dense_layer_q12_THB_bucle_1
		
dense_layer_q12_THB_fin
        MOV     R0, R11             ; Retorna el checksum en R0
        POP     {R4-R7, PC}         ; Restaura R4-R7 y asigna LR a PC (retorno de función)
    ENDP	
		
		END
