//*****************************************************************
// File: laberinto.hpp
// Author: Programación II. Unviersidad de Zaragoza
// Date: March 29, 2021
// Coms: Definición del tipo "Laberinto" para la práctica 3 de la asignatura
//*****************************************************************

#include <fstream>
#include <iostream>
#include <cstdlib>

using namespace std;

// Dimensión máxima de los laberintos
const int MAX_DIM = 120;

// Posibles valores de las posiciones del laberinto
const char LIBRE = ' ';     // (espacio en blanco), posición libre
const char MURO = '#';      // (almohadilla), posición bloqueada
const char CAMINO = '.';    // (punto), camino
const char IMPOSIBLE = 'I'; // posición ya visitada que no llevó a solución

struct Laberinto
{
    // Altura en char (número de filas) del laberinto
    int alto;

    // Anchura en char (número de columnas) del laberinto
    int ancho;

    // Matriz del laberinto
    // Cada posición puede tomar el valor LIBRE, MURO, CAMINO o IMPOSIBLE
    char mapa[MAX_DIM][MAX_DIM];
};

//*************************************************************************
// Encontrar un camino en el laberinto
//*************************************************************************

// Pre: * "lab" es un laberinto correcto y limpio
// Post: "encontrado" si, y solo si, se cumplen las condiciones siguientes:
// * en "lab" se ha marcado con CAMINO las casillas de un camino
// que une las casillas (1,1) y (lab.alto-2,lab.ancho-2)
// * las casillas visitadas que no llevaban a la salida quedan marcadas
// como IMPOSIBLE
// * el resto de casillas no se han modificado

void buscarCamino(Laberinto &lab, bool &encontrado);

void buscarCaminoInm(Laberinto &lab, bool &encontrado, int fila, int col);

//*************************************************************************
// Generar el laberinto
//*************************************************************************

// Pre:	"nombFichero" es el nombre de un fichero que contiene un laberinto almacenado, correcto
// Post: "lab" contiene el laberinto del fichero, almacenado de acuerdo a la especificación dada para el tipo
void cargarLaberinto(const string nombFichero, Laberinto &lab);

// Pre: * 3<=lab.alto, lab.ancho, fila, col <=MAX_DIM 58 //	* 0 <= densidad <= 1
// Post: "lab" queda inicializado con un laberinto aleatorio correcto y vacío
void generarLaberinto(Laberinto &lab, const double densidad, const int fila, const int col);

//************************************************************************* 
// Visualizar el camino encontrado
//*************************************************************************

// Pre:	"lab" es un laberinto correcto
// Post: Se ha mostrado el laberinto por la salida estándar 
// Coms: Versión iterativa
void mostrarLaberinto(const Laberinto &lab);

// Pre:	"lab" es un laberinto correcto
// Post: Se ha mostrado el laberinto por la salida estándar 73 // Coms: Versión recursiva
void mostrarLaberintoR(const Laberinto &lab);

// Pre:	3 ≤ lab.alto, lab.ancho ≤ MAX DIM ∧ 0 ≤ fila ≤ lab.alto − 1 ∧
//	0 ≤ col ≤ lab.ancho − 1 ∧ "lab" es un laberinto correcto 78 // Post: Se ha mostrado el laberinto por la salida estándar
// Coms: Versión recursiva. Inmersión por refuerzo de la precondición 80 void mostrarLaberintoRInm(const Laberinto& lab, int fila, int col);
void mostrarLaberintoRInm(const Laberinto& lab, int fila, int col);