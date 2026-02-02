%{
#include<string.h>
#include "y.tab.h"
%}



%%
					//ejemplo de uso de yylval para tokens a los que se
					//les ha cambiado su valor semantico en th.y
ALGUNA_EXP_REG		{yylval.nombre=strdup(yytext);return(ALGUN_TOKEN);}
%%

