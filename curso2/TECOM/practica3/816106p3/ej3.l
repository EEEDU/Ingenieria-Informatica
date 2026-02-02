/* ej3.l programa en Flex del ejercicio 2.1
 * Daniel Ubalde: 875396, Eduardo Guerrero: 816106
 */
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
