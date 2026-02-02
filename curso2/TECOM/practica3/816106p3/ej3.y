/* ej3.y programa en Flex del ejercicio 3
 * Daniel Ubalde: 875396, Eduardo Guerrero: 816106
 */
%{
#include <stdio.h>
extern int yylex();
extern int yyerror();
%}
%token X Y Z
%token EOL
%start s
%%

s : 	/* nada */
		| c X s EOL 
		;
b : 	X c Y Z Y 
		| X c 
		;
c : 	X b X 
		| Z 
		;

%%
int yyerror(char* s) {
   printf("\n%s\n", s);
   return 0;
}
int main() {
  yyparse();
}

