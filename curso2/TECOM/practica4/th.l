%{
#include<string.h>
#include "y.tab.h"

%}	

%x GRAFO ESTADO


%%
	// Entrar y salir del grafo
[a-zA-Z]+[ \t]*"="[ \t]*[a-zA-Z]+[ \t]*"({" 	{ 
		printf ("ENTRA EN GRAFO"); 
		BEGIN(GRAFO);
		yylval.nombre=strdup(yytext);
		return(INICIO);
	}
		
<GRAFO>"})"								{ 
		printf("SALE DE GRAFO"); 
		BEGIN(INITIAL);
	}


	// Entrar en un estado
<GRAFO>"'"[0-9][0-9][0-9]"'"[ \t]*":"[ \t]*"{"		{ 
		printf ("ENTRA EN ESTADO"); 
		BEGIN(ESTADO); 
		yylval.nombre=strdup(yytext);
	}
%%

