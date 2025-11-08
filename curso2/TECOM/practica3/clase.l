%{
#include "y.tab.h"
%}
%%
a	{return(A);}
b	{return(B);}
[ \t]	{/* ignorar espacios */}
.	{return(yytext[0]); /* caracter inesperado */}
%%
    