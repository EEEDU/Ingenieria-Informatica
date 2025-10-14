
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

^[ \t]*\n                   {totalLineasVacias++;totalLineas++;}

\n                          {totalLineas++;}

[ ]*[A-Za-z0-9:]+([ ][A-Za-z0-9:]+)*[ ]* {
    totalValores++;

    char valor[1024];
    int i = 0, j = 0;
    while (isspace(yytext[i])) i++;
    int start = i;
    int end = yyleng;
    while (end > 0 && isspace(yytext[end-1])) end--;
    strncpy(valor, yytext + start, end - start);
    valor[end - start] = '\0';

    int len = strlen(valor);

    if (len > totalCaracteresValorMasLargo)
        totalCaracteresValorMasLargo = len;

    int tieneDigito = 0;
    for (i = 0; i < len; i++)
        if (isdigit(valor[i])) tieneDigito = 1;
    if (!tieneDigito) totalValoresSinDigito++;

    int esDupla = 1, puntos = 0;
    for (i = 0; valor[i]; i++) {
        if (isdigit(valor[i])) continue;
        else if (valor[i] == ':' && puntos == 0) puntos++;
        else { esDupla = 0; break; }
    }
    if (esDupla && puntos == 1) totalDuplasNumericas++;

    totalCaracteres += yyleng;
}

[,]              { totalCaracteres++; }
.                { totalCaracteres++; }

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
