//*****************************************************************
// File: pilaEnt.hpp
// Author: Programación II. Universidad de Zaragoza
// Date: febrero 2021
// Coms: Fichero de implementación de pilaEnt.cpp, para el manejo
// de pilas de enteros
// Para la práctica 2
//*****************************************************************

#include "pilaEnt.hpp"
// Pre: ---
// Post: p = []
void vaciar(PilaEnt &p) {
p.altura = 0;
}
// Pre: p = [d_1, d_2, ..., d_K] ∧ K >= 0 ∧ K < DIM
// Post: p = [d_1, d_2, ..., d_K, nuevo]
void apilar(PilaEnt &p, const int nuevo) {
p.pila[p.altura] = nuevo;
p.altura++;
}
// Pre: p = [d_1, d_2, ...,d_(K-1), d_K] ∧ K > 0
// Post: p = [d_1, d_2, ..., d_(K-1)]
void desapilar(PilaEnt &p) {
p.altura--;
}
// Pre: p = [d_1, d_2, ...,d_(K-1),d_K] ∧ K > 0
// Post: cima(p) = d_K
int cima(const PilaEnt &p) {
return p.pila[p.altura-1];
}
// Pre: ---
// Post: estaVacia(p) = (p = [])
bool estaVacia(const PilaEnt &p) {
return p.altura == 0;
}