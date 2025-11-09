/* ej3.y fichero para la practica 3 de Teoria de la Computacion  */
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
		| c X s EOL { printf("s\n"); }
		;
b : 	X c Y Z Y  { printf("b1\n"); }
		| X c  { printf("b2\n"); }
		;
c : 	X b X  { printf("c1\n"); }
		| Z { printf("c2\n"); }
		;

%%
int yyerror(char* s) {
   printf("\n%s\n", s);
   return 0;
}
int main() {
  yyparse();
}

