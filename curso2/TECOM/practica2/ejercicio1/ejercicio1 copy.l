%{
    #include <stdbool.h>
    #include <stdio.h>
    #include <string.h>
    bool comentario_no_actualizado = false;
    int elementos_funcion = 0;
    char *parametro1 = NULL;  
    char *parametro2 = NULL;  
%}

%x COMENTARIO_LINEA COMENTARIO_MULTI_LINEA FUNCION_ERROR PARAMETRO_1 PARAMETRO_2

%%

"//"                 { BEGIN(COMENTARIO_LINEA); ECHO; }
"/*"                { BEGIN(COMENTARIO_MULTI_LINEA); ECHO; }

<COMENTARIO_LINEA,COMENTARIO_MULTI_LINEA>ups    { comentario_no_actualizado=true; ECHO; }
<COMENTARIO_MULTI_LINEA>"*/"   {
    if (comentario_no_actualizado) {
        printf ("NO ACTUALIZADO");
        comentario_no_actualizado = false;
    } 
    BEGIN(INITIAL); 
    ECHO;
}
<COMENTARIO_LINEA>\n        {
    if (comentario_no_actualizado) {
        printf ("NO ACTUALIZADO");
        comentario_no_actualizado = false;
    } 
    BEGIN(INITIAL); 
    ECHO;
}
<COMENTARIO_LINEA,COMENTARIO_MULTI_LINEA>.|\n {ECHO;}
<COMENTARIO_LINEA,COMENTARIO_MULTI_LINEA>[a-zA-Z0-9]+ {ECHO;}


"ups"[ \t]*"("              { 
    // Si encuentra ups, espacios y ( entra en posible funcion error 
    BEGIN(FUNCION_ERROR);
    ECHO; 
}

<FUNCION_ERROR>[^,)]+       { 
    // Estando en la funcion error, si encuentra cualquier concatenacion de elementos que no sean , o ) son el parametro 1
    parametro1 = strdup(yytext);
    BEGIN(PARAMETRO_1);
}

<PARAMETRO_1>,              {
    // Si dentro del parametro 1 encuentra una coma, pasa al parametro 2
    BEGIN(PARAMETRO_2);
}
<PARAMETRO_1>")"              {
    // Si dentro del parametro 1 encuentra un parentesis, la funcion solo tenia un elemento, restaura todo
    printf("%s", parametro1);
    parametro1 = NULL;
    BEGIN(INITIAL);
    ECHO;

}

<PARAMETRO_2>[^,")"]+         {
    // Dentro del parametro 2, todo lo que encuentra que no sea , o ) es parte del parametro 2
    parametro2 = strdup(yytext);
}
<PARAMETRO_2>,         {
    // Dentro del parametro 2, hay una coma es que hay mas de 2 parametros, restaura todo
    if (parametro2 == NULL) {parametro2="";}
    printf("%s, %s", parametro1, parametro2);
    parametro1 = NULL;
    parametro2 = NULL;
    BEGIN(INITIAL);
    ECHO;
}
<PARAMETRO_2>")"         {
    // Dentro del parametro 2, hay un parentesis es que hay 2 elementos, intercambiarlos
    if (parametro2 == NULL) {parametro2="";}
    printf("%s, %s", parametro2, parametro1);
    parametro1 = NULL;
    parametro2 = NULL;
    BEGIN(INITIAL);
    ECHO;
}

<FUNCION_ERROR>")"      { 
    // funcion sin elementos 
    BEGIN(INITIAL); 
    ECHO; 
}


%%

int main(){
    int error = yylex();
    return error;
}