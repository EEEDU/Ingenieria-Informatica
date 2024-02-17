//*****************************************************************
// File: funcionesPilaEnt.hpp
// Author: Programación II. Universidad de Zaragoza
// Date: febrero 2021
// Coms: Fichero de interfaz de funcionesPilaEnt.cpp
// Para la práctica 2
//*****************************************************************
#ifndef FUNCIONES_HPP
#define FUNCIONES_HPP
#include <iostream>
#include <iomanip>
// Tipo PilaEnt y operaciones básicas para el trabajo con pilas de datos
#include "../funciones/pilaEnt.hpp"
// * Notación empleada en las especificaciones:
// p = [] La pila p está vacía, es decir, almacena 0 datos
// p = [D_1, D_2, ..., D_K] la pila p almacena K datos. El más antiguo
// es el dato D_1, el segundo más antiguo el dato D_2, etc., etc.,
// y el más reciente es el dato D_K
// Pre: p = [D_1, D_2, ..., D_K] ∧ K >= 0
// Post: p = [D_1, D_2, ..., D_K] ∧ numDatos(p) = K
int numDatos(PilaEnt &p);
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
void mostrar(PilaEnt &p, const int anchura = 3);