//*****************************************************************
// File: funcionesPilaEnt.hpp
// Author: Programación II. Universidad de Zaragoza
// Date: febrero 2021
// Coms: Fichero de implementación de funcionesPilaEnt.cpp
// Para la práctica 2
//*****************************************************************
#include "funcionesPilaEnt.hpp"
#include "../funciones/pilaEnt.hpp"
using namespace std;
// * Notación empleada en las especificaciones:
// p = [] La pila p está vacía, es decir, almacena 0 datos
// p = [D_1, D_2, ..., D_K] la pila p almacena K datos. El más antiguo
// es el dato D_1, el segundo más antiguo el dato D_2, etc., etc.,
// y el más reciente es el dato D_K
// Pre: p = [D_1, D_2, ..., D_K] ∧ K >= 0
// Post: p = [D_1, D_2, ..., D_K] ∧ numDatos(p) = K
int numDatos(PilaEnt &p)
{
    if (estaVacia(p))
        return 0;
    else
    {
        int elem_K = cima(p);
        desapilar(p);
        int cuenta = numDatos(p) + 1;
        apilar(p, elem_K);
        return cuenta;
    }
}
// Pre: p = [D_1, D_2, ..., D_K] ∧ K >= 0 ∧ anchura >= 1
// Post: p = [D_1, D_2, ..., D_K] y presenta por pantalla un listado de
// los datos apilados en la pila comenzando por la cima, D_K, y acabando
// por el del fondo de la pila, D_1. Cada dato lo escribe en una línea,
// empleando anchura caracteres y alineado a la derecha. Cada dato es
// precedido por el carácter '|' y es seguido por los caracteres ' ' y
// '|', tal como se ilustra a continuación. Tras el último dato se
// presenta una linea de la forma "+--...--+", seguida por una línea
// en blanco:
//
// | D_K |
// | ... |
// | D_2 |
// | D_1 |
// +---------+
void mostrar(PilaEnt &p, const int anchura)
{
    if (estaVacia(p))
    {
        char guion = '-';
        cout << "+" << string(anchura, guion) << "+" << endl;
    }
    else
    {
        int elem_K = cima(p);
        cout << "|" << right << setw(anchura) << elem_K << "|" << endl;
        desapilar(p);
        mostrar(p, anchura);
        apilar(p, elem_K);
    }
}
// Pre: p = [D_1, D_2, ..., D_K] ∧ K >= 0 ∧ anchura >= 1
// Post: p = [D_1, D_2, ..., D_K] y presenta por pantalla un listado de los
// datos apilados en la pila comenzando por el del fondo de la pila, D_1,
// y acabando por el de la cima de la pila, D_1. Cada dato lo escribe
// en una línea, empleando anchura caracteres y alineado a la derecha.
// Cada dato es precedido por el carácter '|' y es seguido por los
// caracteres ' ' y '|', tal como se ilustra a continuación. Antes
// del primer dato se presenta una linea de la forma "+--...--+":
//
// +---------+
// | D_1 |
// | D_2 |
// | ... |
// | D_K |
void mostrarInvertida(PilaEnt &p, const int anchura)
{
    if (estaVacia(p))
    {
        char guion = '-';
        cout << "+" << string(anchura, guion) << "+" << endl;
    }
    else
    {
        int elem_K = cima(p);
        desapilar(p);
        mostrarInvertida(p, anchura);
        cout << "|" << right << setw(anchura) << elem_K << "|" << endl;
        apilar(p, elem_K);
    }
}

// Pre: p = [D_1, D_2, ..., D_K] ∧ K >= 0
// Post: p = [D_2, ..., D_K]
void eliminarFondo(PilaEnt &p)
{
    int elem_K = cima(p);
    desapilar(p);
    if (!estaVacia(p))
    {
        eliminarFondo(p);
        apilar(p, elem_K);
    }
}

// Pre: p = [D_K, ..., D_(i+1), D_i, D_(i-1), ..., D_1] ∧
//	K >= 0 ∧ i >= 1 ∧ i <= K
// Post: p = [D_K, ..., D_(i+1), D_(i-1), ..., D_1]
void eliminar(PilaEnt &p, const int i)
{
    if (i == 1)
    {
        desapilar(p);
    }
    else
    {
        int elem_K = cima(p);
        desapilar(p);
        eliminar(p, i - 1);
        apilar(p, elem_K);
    }
}

// Pre: p = [D_1, D_2, ..., D_K] ∧ K >= 0
// Post: p = [nuevo, D_1, D_2, ..., D_K]
void insertarEnFondo(PilaEnt &pila, const int nuevo)
{
    if (estaVacia(pila))
        apilar(pila, nuevo);
    else
    {
        int elem_K = cima(pila);
        desapilar(pila);
        insertarEnFondo(pila, nuevo);
        apilar(pila, elem_K);
    }
}
