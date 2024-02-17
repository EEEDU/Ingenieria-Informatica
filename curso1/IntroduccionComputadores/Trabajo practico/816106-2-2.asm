.data 76
vector: .dw .ascii16 'EDUARDO GUERRERO.'; char16_t vector[]={0x0045, 0x0044, 0x0055, 0x0041, 0x0052, 0x0044, 0x004f, 0x0020, 0x0047, 0x0055, 0x0045, 0x0052, 0x0052, 0x0045, 0x0052, 0x004f, 0x002e};
punto: .ascii16 '.'         ; char16_t punto=0x002E;
letra: .rw 1                ; char letra=0x0000;
cero: .dw 0                 ; int cero=0;
const: .dw 80               ; int const=80; 
.data 127                   ; @ memoria para imprimir en tty
tty_out: .rw 1              ; char16_t tty_out=0x0001;
.code
mov cero, tty_out           ; borrar_pantalla();
mov vector, letra           ; letra = vector[0];
WHILE: cmp punto, letra     ; while (letra != punto) {
beq halt                    ;
mov letra, tty_out          ; imprimir(letra);
add const, INST             ;
INST: mov vector, letra     ; letra = vector[i];
cmp cero, cero              ; 
beq while                   ; }
HALT: cmp cero, cero        ; halt();
beq HALT                    ;
.end
