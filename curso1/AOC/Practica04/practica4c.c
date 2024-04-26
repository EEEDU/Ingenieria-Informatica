#include <stdio.h>  
 
void quickSort(int lista[], int limite_izq, int limite_der) {  
	int izq, der, aux, pivote;
	
	izq = limite_izq;
	der = limite_der;
	pivote = lista[(izq + der)/2];
	
	do{
		while(lista[izq] < pivote) izq++; 
		while(lista[der] > pivote) der--;
		if (izq <= der){
			aux = lista[izq]; lista[izq] = lista[der]; lista[der] = aux;
			izq++; der--;
		}
	}while(izq<=der);
	
	if (limite_izq < der) { quickSort(lista,limite_izq,der); }
	if (limite_der > izq) { quickSort(lista,izq,limite_der); }
}  
 
// Function to print the array  
void printArray(int arr[], int size) { 
   int i;  
   for (i = 0; i < size; i++)  
       printf("%d ", arr[i]);  
   printf("\n");   
}  
 
int main() {  
   int lista[] = { 12, 17, 6, 25, 1, 5 };  
   int n = sizeof(lista) / sizeof(lista[0]);  
		
   quickSort(lista, 0, n - 1);  
		
   printf("Lista ordenada: \n");  
   printArray(lista, n);  
   
		return 0;  
}  