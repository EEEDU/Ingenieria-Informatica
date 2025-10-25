%{
    int contA = 0;
    int contB = 0;
    
    // %x exclusiva -> solo las que esten dentro de la condicion
    // %s inclusiva -> las que esten dentro y las que no tengas condicion
%}

%x DENTRO OTRA

%%

a                   { contA++; }
"/"                 { BEGIN(DENTRO); }
<DENTRO,OTRA>b     { contB++; }
<DENTRO>"/"         { BEGIN(INITIAL); }
.|\n                { /* Ignorar otros caracteres */ }

%%

int main(){
    int error = yylex();
    if (error == 0) {
        printf("Cantidad de a: %d\n", contA);
        printf("Cantidad de b: %d\n", contB);
    }
    return error;
}