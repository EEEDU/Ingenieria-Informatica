//***************************************************************************
// File:    laberinto.cpp
// Author:  Programación II. Universidad de Zaragoza
// Date:	March 26, 2021
// Coms:	Implementación del tipo "Laberinto" para la práctica 3 de la asignatura
//***************************************************************************

#include "laberinto.hpp"
#include <unistd.h> // para "usleep"

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

void buscarCamino(Laberinto &lab, bool &encontrado)
{
    buscarCaminoInm(lab, encontrado, 1, 1);
}

void buscarCaminoInm(Laberinto &lab, bool &encontrado, int fila, int col)
{
    if (lab.mapa[fila][col] != LIBRE)
    {
        encontrado = false;
    }
    else
    {
        lab.mapa[fila][col] = CAMINO;
        if (fila == lab.alto - 2 && col == lab.ancho - 2)
        {
            encontrado = true;
        }
        else
        {
            buscarCaminoInm(lab, encontrado, fila, col + 1);
            lab.mapa[fila][col] = CAMINO;
            if (!encontrado)
            {
                buscarCaminoInm(lab, encontrado, fila + 1, col);
                lab.mapa[fila][col] = CAMINO;
                if (!encontrado)
                {
                    buscarCaminoInm(lab, encontrado, fila, col - 1);
                    lab.mapa[fila][col] = CAMINO;
                    if (!encontrado)
                    {
                        buscarCaminoInm(lab, encontrado, fila - 1, col);
                        lab.mapa[fila][col] = CAMINO;
                        if (!encontrado)
                        {
                            lab.mapa[fila][col] = IMPOSIBLE;
                        }
                    }
                }
            }
        }
    }
}

//*************************************************************************
// Generar el laberinto
//*************************************************************************

// Pre:	"nombFichero" es el nombre de un fichero que contiene un laberinto almacenado, correcto
// Post: "lab" contiene el laberinto del fichero, almacenado de acuerdo a la especificación dada para el tipo
void cargarLaberinto(const string nombFichero, Laberinto &lab)
{
    ifstream f;
    f.open(nombFichero);
    if (f.is_open())
    {
        char c;
        int fila = 0;
        int col = 0;
        f.get(c);
        while (!f.eof())
        {
            lab.mapa[fila][col] = c;
            if ('\n' == c)
            {
                if (fila == 0)
                {
                    lab.ancho = col;
                }
                fila++;
                col = 0;
            }
            else
            {
                col++;
            }
            f.get(c);
        }
        f.close();
        lab.alto = fila;
    }
    else
    {
        cerr << "Error al abrir el archivo " << nombFichero << endl;
    }
}

// Pre: * 3<=lab.alto, lab.ancho, fila, col <=MAX_DIM 58 //	* 0 <= densidad <= 1
// Post: "lab" queda inicializado con un laberinto aleatorio correcto y vacío
void generarLaberinto(Laberinto &lab, const double densidad, const int fila, const int col)
{
    double x = double(rand()) / RAND_MAX;
    if (fila < lab.alto && col <= lab.ancho)
    {
        if (col == lab.ancho)
        {
            generarLaberinto(lab, densidad, fila + 1, 0);
        }
        else
        {
            if (x <= densidad || fila == 0 || fila == lab.alto - 1 || col == lab.ancho - 1 || col == 0)
            {
                lab.mapa[fila][col] = MURO;
                generarLaberinto(lab, densidad, fila, col + 1);
            }
            else if (x > densidad)
            {
                lab.mapa[fila][col] = LIBRE;
                generarLaberinto(lab, densidad, fila, col + 1);
            }
        }
    }
}

//*************************************************************************
// Visualizar el camino encontrado
//*************************************************************************

// Pre:	"lab" es un laberinto correcto
// Post: Se ha mostrado el laberinto por la salida estándar
// Coms: Versión iterativa
void mostrarLaberinto(const Laberinto &lab)
{
    for (int i = 0; i < lab.alto; i++)
    {
        for (int j = 0; j < lab.ancho; j++)
        {
            cout << lab.mapa[i][j];
        }
        // Al acabar una fila, se cambia de línea
        cout << endl;
    }
}

// Pre:	"lab" es un laberinto correcto
// Post: Se ha mostrado el laberinto por la salida estándar 73 // Coms: Versión recursiva
void mostrarLaberintoR(const Laberinto &lab)
{
    mostrarLaberintoRInm(lab, 0, 0);
}

// Pre:	3 ≤ lab.alto, lab.ancho ≤ MAX DIM ∧ 0 ≤ fila ≤ lab.alto − 1 ∧
//	0 ≤ col ≤ lab.ancho − 1 ∧ "lab" es un laberinto correcto 78 // Post: Se ha mostrado el laberinto por la salida estándar
// Coms: Versión recursiva. Inmersión por refuerzo de la precondición 80 void mostrarLaberintoRInm(const Laberinto& lab, int fila, int col);
void mostrarLaberintoRInm(const Laberinto &lab, int fila, int col)
{
    if (fila < lab.alto && col <= lab.ancho)
    {
        if (col == lab.ancho)
        {
            cout << endl;
            mostrarLaberintoRInm(lab, fila + 1, 0);
        }
        else
        {
            cout << lab.mapa[fila][col];
            mostrarLaberintoRInm(lab, fila, col + 1);
        }
    }
}