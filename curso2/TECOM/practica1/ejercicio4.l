
%{

    #include <stdio.h>

%}


%%

[0-9]                       {int numero = atoi(yytext); printf("%d",numero+1);}
\n                          {printf("\n\n");}

%%

int main() {

    int error = yylex();
    
    return error;
    
}
