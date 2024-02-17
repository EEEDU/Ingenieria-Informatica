//*****************************************************************
// File: pilaEnt.hpp
// Author: Programación II. Universidad de Zaragoza
// Date: febrero 2021
// Coms: Fichero de interfaz de pilaEnt.cpp, para el manejo
// de pilas de enteros
// Para la práctica 2
//*****************************************************************

#ifndef PILAENT_HPP
#define PILAENT_HPP
// Capacidad máxima de almacenamiento de datos en una pila
const int DIM = 120; // Redefinir su valor en caso necesario
// Un dato definido a partir del tipo PilaEnt representa una pila
// de datos de enteros. Los datos son gestionados según una política LIFO,
// es decir el último en ser apilado será el primero en ser desapilado.
struct PilaEnt {
int pila[DIM];
int altura;
};
// Notación empleada en la especificación de funciones:
// p = [] la pila está vacía, es decir, almacena 0 datos
// p = [d_1, d_2, ..., d_K] la pila almacena K datos. El más antiguo
// es el dato d_1, el segundo más antiguo el dato d_2, etc., etc.,
// y el más reciente es el dato d_K
// Pre: ---
// Post: p = []
void vaciar(PilaEnt &p);
// Pre: p = [d_1, d_2, ..., d_K] ∧ K >= 0 ∧ K < DIM
// Post: p = [d_1, d_2, ..., d_K, nuevo]
void apilar(PilaEnt &p, const int nuevo);
// Pre: p = [d_1, d_2, ...,d_(K-1), d_K] ∧ K > 0
// Post: p = [d_1, d_2, ..., d_(K-1)]
void desapilar(PilaEnt &p);