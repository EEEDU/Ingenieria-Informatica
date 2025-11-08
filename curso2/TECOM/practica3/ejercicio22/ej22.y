/* calcMejor.y fichero para la practica 3 de Teoria de la Computacion  */
%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
extern int yylex();
extern int yyerror();
int current_base = 10;
int convertir_decimal_base(int numero, int base) {
	char resultado[64], temp[64];
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
        resto = numero % base;
        temp[i++] = '0' + resto;
        numero /= base;

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
	return atoi(resultado);
}
%}
%token NUMBER EOL CP OP
%start calclist
%token ADD SUB
%token MUL DIV
%token BASE_CHANGE END_LINE END_LINE_BASE
%%

calclist : /* nada */
	| calclist exp END_LINE EOL { printf("=%d\n", $2); }
	| calclist exp END_LINE_BASE EOL { printf("=%d\n", convertir_decimal_base($2, current_base)); }
	| calclist BASE_CHANGE EOL  { 	current_base = $2;
								 	printf("cambio a base %d\n", $2); 
							   	}
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

