	AREA codigo, CODE
    PRESERVE8
	EXPORT dense_layer_q12_ARM_C
	;EXPORT dense_layer_q12_ARM
	EXTERN dense_layer_q12_C
	
dense_layer_q12_ARM_C
	
	mov IP, SP
	STMDB SP!, {r4-r7,FP,IP,LR}
	sub FP, IP, #4

	LDMIB FP, {r4-r7}
	STMDB SP!, {r4-r7}
	
	BL dense_layer_q12_C

	LDMDA FP, {r4-r7,FP,SP,PC}
		
dense_layer_q12_ARM

	mov IP, SP
	STMDB SP!, {r4-r7,FP,IP,LR}
	sub FP, IP, #4

	LDMIB FP, {r4-r7}
	STMDB SP!, {r4-r7}
	
	BL dense_layer_q12_C

	LDMDA FP, {r4-r7,FP,SP,PC}

	END