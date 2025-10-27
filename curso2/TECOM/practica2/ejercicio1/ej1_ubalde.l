Práctica 2 Ejercicio 1
Daniel Ubalde 875396, Eduardo Guerrero 816106

%{
  #include <stdio.h>
  char* p1;
  char* p2;
%}

%x LINE_COMMENT
%x BLOCK_COMMENT
%x UPS_DETECTED_LINE_COMMENT
%x UPS_DETECTED_BLOCK_COMMENT
%x P1
%X P2
%x END_FUNCTION

%%

^[ ]*"//" { BEGIN(LINE_COMMENT); ECHO; }
^[ ]*"/*" { BEGIN(BLOCK_COMMENT); ECHO; }

  /*
    Dado el orden de prioridades de FLEX si se encuentra "cups" o "upssdawd" o
    "dasdupsdasda", solo se mostrará por pantalla pero si solo encuentra "ups"
    ambas expresiones habrán encontrado el mismo patrón con igualdad
    de caracteres, entonces se procesará la regla por el orden de declaración.
  */

<LINE_COMMENT,BLOCK_COMMENT>ups {
  ECHO;
  if (YYSTATE == LINE_COMMENT) {
    BEGIN(UPS_DETECTED_LINE_COMMENT);
  } else {
    BEGIN(UPS_DETECTED_BLOCK_COMMENT);
  }
}

<LINE_COMMENT,BLOCK_COMMENT>[a-zA-Z0-9]+ {
  ECHO;
}

<LINE_COMMENT,UPS_DETECTED_LINE_COMMENT>\n {
  if (YYSTATE == UPS_DETECTED_LINE_COMMENT) {
    printf("NO ACTUALIZADO");
  }
  ECHO;
  BEGIN(INITIAL);
}
<BLOCK_COMMENT,UPS_DETECTED_BLOCK_COMMENT>"*/" {
  if (YYSTATE == UPS_DETECTED_BLOCK_COMMENT) {
    printf("NO ACTUALIZADO");
  }
  ECHO;
  BEGIN(INITIAL);
}


  /* Función ups sin parámetros */
ups[ ]*"("[ ]*")" {
  ECHO;
}

  /* Función ups que tiene al menos 1 parámetro */
ups[ ]*"(" {
  ECHO;
  BEGIN(P1);
}

  /* Otra función distinta de ups() */
[a-zA-Z0-9]+[ ]*"(" { ECHO; }

<P1,P2>[^,")"]+ {
  if (YYSTATE == P1) {
    p1 = strdup(yytext);
  } else {
    p2 = strdup(yytext);
  }
}

  /* La función tiene 2 parámetros */
<P1>"," {
  BEGIN(P2);
}

  /* La función tiene 1 parámetro */
<P1>")" {
  printf("%s", p1);
  ECHO;
  BEGIN(INITIAL);
}

  /* Mas de 2 parámetros */
<P2>"," {
  printf("%s,%s", p1, p2);
  ECHO;
  BEGIN(INITIAL);
}

  /* He llegado al final */
<P2>")" {
  printf("%s,%s", p2, p1);
  ECHO;
  BEGIN(INITIAL);
}



%%
