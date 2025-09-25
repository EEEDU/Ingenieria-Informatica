
%{

    #include <stdio.h>

    int totalCaracteres = 0;
    int totalLineas = 0;
    int totalLineasVacias = 0;
    int totalValores = 0;
    int totalCaracteresValorMasLargo = 0;
    int totalDuplasNumericas = 0;
    int totalValoresSinDigito = 0;


%}


%%

\n                          {totalLineas++;}
\n?\n                      {totalLineasVacias++;}

%%

int main() {

    int error = yylex();

    if (error == 0) {
        printf("C:%d\n", totalCaracteres);
        printf("L:%d\n", totalLineas);
        printf("LV:%d\n", totalLineasVacias);
        printf("V:%d\n", totalValores);
        printf("M:%d\n", totalCaracteresValorMasLargo);
        printf("D:%d\n", totalDuplasNumericas);
        printf("N:%d\n", totalValoresSinDigito);
    }
    
    return error;
    
}
