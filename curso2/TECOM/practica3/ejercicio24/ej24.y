/* calcMejor.y fichero para la practica 3 de Teoria de la Computacion  */
%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
extern int yylex();
extern int yyerror();
char* convertir_decimal_hexadecimal(int numero) {
	static char resultado[64];
    char temp[64];
    int i = 0;
	int resto;
    int negativo = 0;

    // Manejo del caso 0
    if (numero == 0) {
        return 0;
    }

    // Tomar valor absoluto si quieres manejar negativos
    if (numero < 0) {
        negativo = 1;
        numero = -numero;
    }

    // Conversión
    while (numero > 0) {
        resto = numero % 16;
        if (resto<10) {
            temp[i++] = '0' + resto;
        }
        else {
            temp[i++] = 'A' + (resto - 10);
        }
        numero /= 16;

    }

    // Si era negativo anadir -
    if (negativo)
        temp[i++] = '-';

    temp[i] = '\0';

    // Invertir la cadena
    int len = strlen(temp);
    for (int j = 0; j < len; j++)
        resultado[j] = temp[len - j - 1];
    resultado[len] = '\0';
	return resultado;
}
%}
%token NUMBER EOL CP OP
%start calclist
%token ADD SUB
%token MUL DIV
%token END_LINE END_LINE_HEXA
%%

calclist : /* nada */
	| calclist exp END_LINE EOL { printf("=%d\n", $2); }
	| calclist exp END_LINE_HEXA EOL { printf("=%s\n", convertir_decimal_hexadecimal($2)); }
	;
exp : 	factor 
	| exp ADD factor { $$ = $1 + $3; }
	| exp SUB factor { $$ = $1 - $3; }	
	;
factor : 	factor MUL factorsimple { $$ = $1 * $3; }
		| factor DIV factorsimple { $$ = $1 / $3; }
		| factorsimple
		;
factorsimple : 	OP exp CP { $$ = $2; }
		| NUMBER 
		;
%%
int yyerror(char* s) {
   printf("\n%s\n", s);
   return 0;
}
int main() {
  yyparse();
}

