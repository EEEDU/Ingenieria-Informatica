
%{

    #include <stdio.h>

%}


%%

@unizar.es      {printf("@gmail.com");}

%%

int main() {

    int error = yylex();
    
    return error;
    
}
