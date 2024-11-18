//*****************************************************************
// File:   practica_4.cpp
// Author: Eduardo Guerrero (816106), Ismael Martinez (818903)
// Date:   noviembre 2023
// Coms:   Parte del práctica 4 de PSCD
//         Compilar mediante
//           make -f Makefile_p4
//*****************************************************************

#include <iostream>
#include <fstream> // Necesario para trabajar con archivos
#include <string>
#include <sstream>
#include <cstdlib> // Necesario para rand y srand
#include <ctime> // Necesario para time
#include <chrono>
#include <iterator> // Necesario para std::advance
#include <chrono> // Agrega esta línea para incluir high_resolution_cloc

#include "ControlBuffer.hpp"
#include "ControlMatriz.hpp"

#include "librerias/MultiBuffer/MultiBuffer.hpp"

using namespace std;



//----------------------------------------------------
void master(string archivoTareas, ControlBuffer &controlBuffer, ControlMatriz &controlMatriz, bool &final)
{
    Tarea tarea;
    Tarea tareaFinal = {"TF",0};
    ifstream archivo(archivoTareas);
    string linea;
    bool tf = false;
    // Lee y muestra cada línea del archivo
    while (getline(archivo, linea))
    {
        // Extrea la tarea del archivo
        size_t posicion = linea.find(",");
        tarea.tipoTarea = linea.substr(0, posicion);
        try
        {
            tarea.cargaDeTrabajo = stof(linea.substr(posicion + 1));
        }
        catch (const std::invalid_argument &e)
        {
            cerr << "Error: No se pudo convertir la cadena a un número de punto flotante." << std::endl;
        }

        // Añadir tarea en el multibuffer
        controlBuffer.addTask(tarea);

    }
    while(!tf){
        // Añadir tareaFinal en el multibuffer
        controlBuffer.addTask(tareaFinal);

        tf = controlBuffer.checkTf();
    }

    final = true;
    controlMatriz.writeMatriz();

    archivo.close();
}

//----------------------------------------------------
// Pre:  0 <= id < N_CONTROLLERS
void controller(ControlBuffer &controlBuffer, ControlMatriz &controlMatriz, bool &final)
{
    Tarea tareaController;
    while (!final)
    {
        // Coger tarea del buffer
        tareaController = controlBuffer.getTask();
        controlMatriz.addResult(tareaController);

    }

}
//----------------------------------------------------
int main(int argc, char *argv[])
{
    ControlBuffer controlBuffer;
    ControlMatriz controlMatriz;
    
    string fichero = "tareas.txt";
    bool final = false;

    srand(static_cast<unsigned int>(time(nullptr)));

    thread master_thread(&master, fichero, ref(controlBuffer), ref(controlMatriz), ref(final));
    thread controller_thread(&controller, ref(controlBuffer), ref(controlMatriz), ref(final));

    master_thread.join();
    controller_thread.join();

    return 0;
}
