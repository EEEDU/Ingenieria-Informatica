/* ej22.l programa en Flex del ejercicio 2.2
 * Daniel Ubalde: 875396, Eduardo Guerrero: 816106
 */
%{
#include "y.tab.h"
%}
%%
"+"	{return(ADD);}
"-"	{return(SUB);}
"*"	{return(MUL);}
"/"	{return(DIV);}
"("	{return(OP);}
")"	{return(CP);}
";" {return(END_LINE);}
";b" {return(END_LINE_BASE);}
"b="("10"|[2-9]) {yylval = atoi(yytext + 2); return(BASE_CHANGE);}
[0-9]+	{yylval = atoi(yytext); return(NUMBER);}
\n	{return(EOL);}
[ \t]	{/* ignorar espacios */}
.	{return(yytext[0]); /* caracter inesperado */}
%%
