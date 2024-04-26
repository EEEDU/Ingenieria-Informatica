			AREA datos, DATA, READWRITE
N			EQU 5
M			EQU 7
matriz		DCB	"nkongvfkobcfeqpbjufcxxfrewomnkgytvd"
temp		DCB 0

			AREA codigo, CODE, READONLY
			ENTRY
			LDR r2, =temp		; r2 <- @temp
			LDR r3, =N			; r3 <- N
			LDR r4, =M			; r4 <- M
			mul r5, r3, r4		; r5 <- N*M
			mov r7, r5			; i = N*M || n? de iteraciones
			mov r8, #0			; j = 0 || n? de veces que se recorre una columna, cuando sea igual a N hemos acabado y podemos cambiar de columna
			mov r9, #1			; x = 1 || n? de la columna en la que estamos, empezando por la derecha
			
ini_buc_i	LDR r0, =matriz		; r0 <- @matriz
			LDR r1, =matriz		; r1 <- @matriz
			mul r5, r3, r4		; r5 <- N*M
			sub r6, r5, r9		; r6 <- N*M-x || ?ndice del ?ltimo elemento de la columna x
			add r0, r0, r6		; r0 <- @matriz + ?ndice
			add r1, r1, r6		; r1 <- @matriz + ?ndice
			sub r1, r1, #M		; r1 <- r1-M || ?ndice del elemento anterior de la columna x
			
ini_buc_j	ldrb r10, [r0]		; r10 <- matriz[?ndice]
			ldrb r11, [r1]		; r11 <- matriz[?ndice-M]
			cmp r11, r10		; (matriz[?ndice-M]>matriz[?ndice])?
			strhib r10, [r2]	; si s?, r2 <- r10 || temp = matriz[?ndice]
			strhib r11, [r0]	; si s?, r0 <- r11 || matriz[?ndice] = matriz[?ndice-1]
			strhib r10, [r1]	; si s?, r1 <- r10 || matriz[?ndice-1] = temp
			sub r0, r0, #M		; r0 <- r0-M || se actualiza @matriz al elemento anterior de la columna
			sub r1, r1, #M		; r1 <- r1-M || se actualiza @matriz al elemento anterior de la columna
			sub r6, r6, #M		; ?ndice = ?ndice-M || ?ndice del elemento anterior de la columna
			cmp r6, #M			; (?ndice<M)? || cuando ?ndice sea menor que M hemos recorrido una vez la columna
			bge ini_buc_j		; si no, vuelvo a ini_buc_j
			
fin_buc_j	subs r7, r7, #1	    ; i--
			beq fin				; cuando i sea 0, salto a fin
			add r8, r8, #1		; j++
			cmp r8, #N			; (j<N)?
			addge r9, r9, #1	; si no, x++ || cambio de columna
			movge r8, #0		; j = 0 || se reinicia el contador
			b ini_buc_i			; salto a ini_buc_i
			
fin		    b fin
			END