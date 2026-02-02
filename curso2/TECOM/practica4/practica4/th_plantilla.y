%{
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define PALOS 3
#define DIM ... //DIMENSION DE LA MATRIZ DE ADYACENCIA
#define BUFF 4000

//declarar la variable listaTr de tipo ListaTransiciones
//Almacena las transiciones de un solo nodo (nodoOrig) 
//a varios nodos (nodosFin y sus correspondientes etiquetas)
struct ListaTransiciones {
	char* nodoOrig;
	char* nodosFin[DIM];
	char* etiquetas[DIM]; 
	int total;
} listaTr;

//tabla de adyacencia
char* tablaTr[DIM][DIM];

//inicializa una tabla cuadrada DIM x DIM con la cadena vacia
void iniTabla(char* tabla[DIM][DIM]) {
	for (int i = 0; i < DIM; i++) {
		for (int j = 0; j < DIM; j++) {
			tabla[i][j] = "";
		}
	}
}

/*
 * Calcula la multiplicacion simbolica de matrices 
 * cuadradas DIM x DIM: res = t1*t2
 *
 * CUIDADO: res DEBE SER UNA TABLA DISTINTA A t1 y t2
 * Por ejemplo, NO SE DEBE USAR en la forma:
 *           multiplicar(pot, t, pot); //mal
 */
void multiplicar(char* t1[DIM][DIM], char* t2[DIM][DIM], char* res[DIM][DIM]) {
	for (int i = 0; i < DIM; i++) {
		for (int j = 0; j < DIM; j++) {
			res[i][j] = (char*) calloc(BUFF, sizeof(char));
			for (int k = 0; k < DIM; k++) {
				if (strcmp(t1[i][k],"")!=0 && strcmp(t2[k][j],"") != 0) {
					strcat(strcat(res[i][j],t1[i][k]),"-");
					strcat(res[i][j],t2[k][j]);
				}
			}
		}
	}
}


/* 
 *Copia la tabla orig en la tabla copia
*/
void copiar(char* orig[DIM][DIM], char* copia[DIM][DIM]) {
	for (int i = 0; i < DIM; i++) {
		for (int j = 0; j < DIM; j++) {
			copia[i][j] = strdup(orig[i][j]);
		}
	}
}


%}

  //nuevo tipo de dato para yylval
%union{
	char* nombre;
}

%token ID ... //PONER TODOS LOS TOKENS DE LA GRAMATICA, POR EJEMPLO, ID
%start	...		//variable inicial 

%type<nombre> ID ... //lista de tokens y variables que su valor semantico,
                     //recogido mediante yylval, es 'nombre' (ver union anterior).
					 //Para estos tokens, yylval será de tipo char* en lugar de int.

%%


%%

int yyerror(char* s) {
	printf("%s\n");
	return -1;
}

int main() {
	//inicializar lista transiciones
	listaTr.total = 0;

	//inicializar tabla de adyacencia
	iniTabla(tablaTr);

	//nodo inicial
	char* estadoIni = ...;

	//nodo final
	char* estadoFin = ...;
	
	int error = yyparse();

	
	if (error == 0) {
		//matriz para guardar la potencia
		char* pot[DIM][DIM];
		copiar(tablaTr,pot)
		//calcular movimientos de estadoIni a estadoFin
		//calculando las potencias sucesivas de tablaTr
		...
		...


		printf("Nodo inicial  : %s\n", estadoIni);
		//rellenar los ... con los indices adecuados a vuestro codigo
		printf("Movimientos   : %s\n", pot[...][...]);
		printf("Nodo final    : %s\n", estadoFin);
	}

	return error;
}
