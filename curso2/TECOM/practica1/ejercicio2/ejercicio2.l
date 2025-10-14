
%{

    #include <stdio.h>

    int total = 0;

%}


%%

@hotmail      {total++;}

%%

int main() {

    int error = yylex();
    if (error == 0) {
        printf("Total de personas que usan hotmail: %d\n", total);
    }
    return error;
    
}
