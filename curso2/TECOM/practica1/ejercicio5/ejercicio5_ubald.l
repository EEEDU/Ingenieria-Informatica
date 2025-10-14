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
[0-9]+:[0-9]+ {
  if (totalCaracteresValorMasLargo < yyleng) {
    totalCaracteresValorMasLargo = yyleng;
  }
  totalDuplasNumericas++;
  totalValores++;
  totalCaracteres += yyleng;
}
[0-9]+ {
  totalValoresSinDigito += yyleng;
  totalCaracteres++;
}
(\ *([a-zA-Z])+\ *)* {

  if (totalCaracteresValorMasLargo < yyleng) {
    totalCaracteresValorMasLargo = yyleng;
  }
  totalValoresSinDigito++;
  totalValores++;
  totalCaracteres += yyleng;
}
^\ *$ {
  totalLineasVacias++;
  totalCaracteres += yyleng;
}
\n { totalLineas++; }
. { totalCaracteres++; }
%%

int main() {
  int err = yylex();

  if (err == 0) {
    printf("C: %d\nL: %d\nLV: %d\nV: %d\nM: %d\nD: %d\nN: %d\n",
    totalCaracteres,
    totalLineas,
    totalLineasVacias,
    totalValores,
    totalCaracteresValorMasLargo,
    totalDuplasNumericas,
    totalValoresSinDigito);
  }
  return err;
}