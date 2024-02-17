//*****************************************************************
// File: pruebas02.cpp
// Author: Programación II. Universidad de Zaragoza
// Date: febrero 2021
// Coms: Programa de pruebas del trabajo con pilas
// de datos enteros y del comportamiento de las funciones
// definidas en el fichero "funcionesPilaEnt.hpp"
// Para la práctica 2
//*****************************************************************

#include <iostream>
#include <cstring>
using namespace std;
// Funciones diseñadas de forma recursiva que trabajan
// con pilas que almacenan datos de enteros
#include "funcionesPilaEnt.hpp"
// Pre: ---
// Post: Ilustra el trabajo con pilas de datos enteros
void pruebasPilaEnteros() {
// Define una pila de datos, P, que almacena datos de tipo 'int'
PilaEnt P;
// Vacía la pila P
vaciar(P);
// PRIMOS almacena los números primeros menores que 100
const int PRIMOS[] = {
2, 3, 5, 7, 11, 13, 17, 19, 23, 29,
31, 37, 41, 43, 47, 53, 59, 61, 67, 71,
73, 79, 83, 89, 97
};
// Apila en P los NUM primeros números primos
const int NUM = 15;
for (int i = 1; i <= NUM; ++i) {
apilar(P, PRIMOS[i-1]);
}
// Muestra por pantalla el contenido de P
cout << "Se han apilado los " << numDatos(P)
<< " primeros numeros primos:" << endl << endl;
mostrar(P);
cout << endl;
// Muestra por pantalla los datos de P en orden inverso
cout << "Y esta es la pila invertida:" << endl << endl;
mostrarInvertida(P);
cout << endl;
// Elimina un dato de la pila P y muestra su contenido
cout << endl << "Se ha eliminado su sexto dato:" << endl << endl;
eliminar(P, 6);
mostrar(P);