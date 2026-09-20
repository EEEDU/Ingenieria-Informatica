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
        ARM
        EXPORT  dense_layer_q12_ARM_C
        IMPORT  neuron_q12_C

dense_layer_q12_ARM_C
        ; ---------- Prologo / marco de pila ----------
        STMDB   R13!, {R4-R10, R14}   ; guarda R4-R10 y LR (8 registros = 32 bytes)
        SUB     R13, R13, #0xC        ; hueco de argumentos para neuron_q12_C

        ; ---------- Parametros que llegan en registros ----------
        MOV     R4, R0                ; R4 = input
        MOV     R5, R1                ; R5 = weights_o (empieza en weights)
        MOV     R6, R2                ; R6 = puntero a bias[o]
        MOV     R7, R3                ; R7 = puntero a output[o]

        ; ---------- Parametros que llegan por la pila ----------
        ADD     R8, R13, #0x28        ; R8 = direccion de input_size en la pila
        LDMIA   R8, {R9, R8}          ; menor registro = menor direccion:
                                      ; R8 = input_size, R9 = output_size

        ; ---------- Copiar clamp_min y clamp_max al hueco de argumentos ----------
        LDR     R10, [R13, #0x30]     ; R10 = clamp_min (registro temporal)
        STR     R10, [R13, #0]        ; [SP+0] = clamp_min
        LDR     R10, [R13, #0x34]     ; R10 = clamp_max
        STR     R10, [R13, #4]        ; [SP+4] = clamp_max

        MOV     R10, #0               ; checksum = 0

        ; ---------- Bucle sobre las neuronas ----------
bucle
        CMP     R9, #0                ; inicio del bucle: quedan neuronas?
        BEQ     fin                   ; no: salir

        ; Parametros para llamar a la funcion
        MOV     R0, R4                ; R0 = input
        MOV     R1, R5                ; R1 = weights_o
        MOV     R2, R8                ; R2 = input_size
        MOV     R3, R6                ; R3 = bias_q12

        BL      neuron_q12_C          ; llamada a la funcion; resultado y en R0

        STRH    R0, [R7], #2          ; output[o] = y ; R7 += 2 (post-incremento)

        ADD     R10, R10, R10, LSL #5 ; checksum * 33  (= checksum + checksum*32)
        MOV     R0, R0, LSL #16       ; limpia los 16 bits altos de R0 ...
        MOV     R0, R0, LSR #16       ; ... R0 = (uint16_t)y
        ADD     R10, R10, R0          ; checksum = checksum*33 + y

        ; Actualizar punteros
        ADD     R5, R5, R8, LSL #1    ; weights_o += input_size elementos (2 bytes cada uno)
        ADD     R6, R6, R8, LSL #1    ; puntero a bias

        SUBS     R9, R9, #1            ; output_size - 1
        BNE     bucle

        ; ---------- Epilogo ----------
fin
        MOV     R0, R10               ; return checksum
        ADD     R13, R13, #8          ; libera el hueco de argumentos
        LDMIA   R13!, {R4-R10, R14}   ; restaura registros y LR
        BX      R14                   ; vuelve al llamante

        END
