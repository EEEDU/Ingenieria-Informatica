//-----------------------------------------------------------------------------
// File:   ControlMatriz.cpp
// Author: Eduardo Guerrero (nip:816106) && Ismael Martinez (nip:818903)
// Date:   noviembre 2023
// Coms:   Parte del práctica 4 de PSCD
//          monitor encargado de la conciurrencia de la matriz
//------------------------------------------------------------------------------

#include "ControlMatriz.hpp"
#include <iostream>

ControlMatriz::ControlMatriz()
{
    for (int i = 0; i < 3; i++)
    {
        matrizResultados.push_back({0, 0, 0});
    }

    tiempoTotalExitosoT1 = 0;
    tiempoTotalExitosoT2 = 0;      
    tiempoTotalExitosoT3 = 0;      
}

void sleep(int milisegundos) {
    this_thread::sleep_for(chrono::milliseconds(milisegundos));
}


bool ejecucion(Tarea tareaEnEjecucion, vector<vector<double>> &matrizResultados){
    bool resulatado = false;

    int numero_aleatorio = rand() % (100);
    float numeroAleatorio = static_cast<float>(rand()) / RAND_MAX;
    // Medir el tiempo de inicio
    auto start_time = std::chrono::high_resolution_clock::now();

    if (tareaEnEjecucion.tipoTarea == "t1" && numero_aleatorio < 5){
        sleep(static_cast<int>(tareaEnEjecucion.cargaDeTrabajo));
        resulatado = true;
    }
    else if(tareaEnEjecucion.tipoTarea == "t2" && numero_aleatorio < 7){
        sleep(static_cast<int>(tareaEnEjecucion.cargaDeTrabajo));
        resulatado = true;
    }
    else if(tareaEnEjecucion.tipoTarea == "t3" && numero_aleatorio < 10){
        sleep(static_cast<int>(tareaEnEjecucion.cargaDeTrabajo));
        resulatado = true;
    }

    
     // Medir el tiempo de finalización
   auto end_time = std::chrono::high_resolution_clock::now();


    // Calcular la duración del tiempo de ejecución
    std::chrono::duration<double> tiempoEjecucion = end_time - start_time;
    if (tareaEnEjecucion.tipoTarea == "t1") {
        matrizResultados[0][2] += tiempoEjecucion.count();
    } else if (tareaEnEjecucion.tipoTarea == "t2") {
        matrizResultados[1][2] += tiempoEjecucion.count();
    } else if (tareaEnEjecucion .tipoTarea == "t3") {
        matrizResultados[2][2] += tiempoEjecucion.count();
    }
    return resulatado;
}

void escribirMatriz(vector<vector<double>> &matrizResultados, Tarea tareaController, bool resultado){
    if (tareaController.tipoTarea == "t1"){
        matrizResultados[0][0] ++;
        if (resultado) matrizResultados[0][1] ++;
    }
    else if (tareaController.tipoTarea == "t2"){
        matrizResultados[1][0] ++;
        if (resultado) matrizResultados[1][1] ++;
    }
    
    else if (tareaController.tipoTarea == "t3"){
        matrizResultados[2][0] ++;
        if (resultado) matrizResultados[2][1] ++;
    }

}

void ControlMatriz::addResult(Tarea tarea)
{
    escribirMatriz(matrizResultados, tarea, ejecucion(tarea, matrizResultados));
}


void ControlMatriz::writeMatriz()
{
    for (int i = 0; i < matrizResultados.size(); i++){

        cout << "[" << endl;
        cout << "   Tipo de tarea t" << i+1 << endl;
        cout << "   Total: " << matrizResultados[i][0] << endl;
        cout << "   Exito: " << matrizResultados[i][1] << endl;
        cout << "   Tiempo: " << matrizResultados[i][2] << endl;
        cout << "]" << endl;
        
    }
}

