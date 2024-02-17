//***************************************************************************
// File:	mainLab.cpp
// Author: Programación II. Universidad de Zaragoza
// Date:	April 14, 2021
// Coms:	Programa principal que verifica las funciones de "laberinto.cpp"
//***************************************************************************
#include "laberinto.hpp"
#include <ctime>

using namespace std;

int main(int argc, char *args[])
{
    Laberinto lab;
    bool encontrado = false;
    time_t semilla = time(nullptr);
    srand(semilla);
    if (argc == 2)
    {
        string nombFichero = args[1];
        cout << "Se va a cargar el laberinto del fichero " << nombFichero << endl;
        cout << endl;
        cargarLaberinto(nombFichero, lab);
    }
    if (argc == 4)
    {
        int alto = atoi(args[1]);
        int ancho = atoi(args[2]);
        double densidad = atof(args[3]);
        lab.alto = alto;
        lab.ancho = ancho;
        cout << "Se va a generar un laberinto de " << alto << " de alto, " << ancho
             << " de ancho y " << densidad << " de densidad" << endl;
        cout << endl;
        generarLaberinto(lab, densidad, 0, 0);
    }
    cout << "Búsqueda de un camino para salir del laberinto..." << endl;
    lab.mapa[1][1] = LIBRE;
    lab.mapa[lab.alto - 2][lab.ancho - 2] = LIBRE;
    buscarCamino(lab, encontrado);
    cout << endl;
    if (encontrado)
    {
        mostrarLaberintoR(lab);
        cout << endl;
    }
    else
    {
        cout << "No hay solución" << endl;
    }
    return 0;
}
