/* ej3.l fichero para la practica 3 de Teoria de la Computacion */
%{
#include "y.tab.h"
%}
%%
"x"	{return(X);}
"y"	{return(Y);}
"z"	{return(Z);}
\n	{return(EOL);}
.	{return(yytext[0]); /* caracter inesperado */}
%%
