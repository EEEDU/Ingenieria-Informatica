/* ej21.l fichero para la practica 3 de Teoria de la Computacion */
%{
#include "y.tab.h"
#include <stdlib.h>
#include <string.h>

int current_base = 10;

int convertir_base_decimal(const char *numero, int base) {
    int result = 0;

    for (int i = 0; numero[i] != '\0'; i++) {
        result = result * base + (numero[i] - '0');
    }
    return result;
}
%}

%%
"+"	{return(ADD);}
"-"	{return(SUB);}
"*"	{return(MUL);}
"/"	{return(DIV);}
"("	{return(OP);}
")"	{return(CP);}
"b="("10"|[2-9])        {current_base = atoi(yytext + 2);
                        yylval = current_base;
                        return(BASE_CHANGE);}
[0-9]+	                {yylval = atoi(yytext); return(NUMBER);}
[0-9]+"b"	            {   
                            char numero[128];
                            strncpy(numero, yytext, yyleng - 1);  // quitar la b
                            numero[yyleng - 1] = '\0';  // cerrar la cadena
                            yylval = convertir_base_decimal(numero, current_base);  // pasar a decimas segun la base
                            return(NUMBER);
                        }
\n	{return(EOL);}
[ \t]	{/* ignorar espacios */}
.	{return(yytext[0]); /* caracter inesperado */}
%%
