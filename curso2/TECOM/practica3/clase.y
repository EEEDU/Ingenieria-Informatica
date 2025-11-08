
%{
#include <stdio.h>
extern int yylex();
extern int yyerror();
extern int yyparse();
%}
%token A B
%start s
%%

s : t       {printf("s: t\n");}
	| A s   {printf("s: A s\n");}
	;
t :         {printf("Cadena vacia\n");}
    | t B B {printf("t: t B B\n");}
	;
%%
int yyerror(char* s) {
   printf("\n%s\n", s);
   return 0;
}
int main() {
  yyparse();
}