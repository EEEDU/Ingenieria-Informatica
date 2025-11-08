/* ej21.l fichero para la practica 3 de Teoria de la Computacion */
%{
#include "y.tab.h"
#include <stdlib.h>
#include <string.h>


int caracter_hexadecimal_decimal(char caracter) {
    if (caracter >= '0' && caracter <= '9')
        return caracter - '0';
    else if (caracter >= 'A' && caracter <= 'F')
        return caracter - 'A' + 10;
}

int convertir_hexadecimal_decimal(const char *numero) {
    int result = 0;

    for (int i = 0; numero[i] != '\0'; i++) {
        result = result * 16 + caracter_hexadecimal_decimal(numero[i]);
    }
    return result;
}
%}

%s inclusiva


%%
"+"	{return(ADD);}
"-"	{return(SUB);}
"*"	{return(MUL);}
"/"	{return(DIV);}
"("	{return(OP);}
")"	{return(CP);}

[0-9]+	                {yylval = atoi(yytext); return(NUMBER);}
[A-F0-9]+"H"	            {   
                            char numero[128];
                            strncpy(numero, yytext, yyleng - 1);  // quitar la b
                            numero[yyleng - 1] = '\0';  // cerrar la cadena
                            yylval = convertir_hexadecimal_decimal(numero);  // pasar a decimas segun la base
                            return(NUMBER);
                        }
\n	{return(EOL);}
[ \t]	{/* ignorar espacios */}
.	{return(yytext[0]); /* caracter inesperado */}
%%
