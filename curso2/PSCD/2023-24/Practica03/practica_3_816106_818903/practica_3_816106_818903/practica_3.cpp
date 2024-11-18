//*****************************************************************
// File:   practica_3.cpp
// Author: Eduardo Guerrero (816106), Ismael Martinez (818903)
// Date:   octubre 2023
// Coms:   Parte del práctica 3 de PSCD
//         Compilar mediante
//           make -f Makefile_p3
//*****************************************************************

// Leccion 6 semaforos, ejercicio 8
#include <iostream>
#include <fstream> // Necesario para trabajar con archivos
#include <string>
#include <sstream>
#include <cstdlib> // Necesario para rand y srand
#include <ctime> // Necesario para time
#include <thread>
#include <chrono>
#include <list>
#include <iterator> // Necesario para std::advance
#include <vector>
#include <chrono> // Agrega esta línea para incluir high_resolution_cloc


#include "librerias/Semaphore_V4/Semaphore_V4.hpp"
#include "librerias/MultiBuffer/MultiBuffer.hpp"

using namespace std;
const int N_CONTROLLERS = 10;
double tiempoTotalExitosoT1=0;
double tiempoTotalExitosoT2=0;
double tiempoTotalExitosoT3=0;
//----------------------------------------------------
struct Tarea
{
    string tipoTarea;
    float cargaDeTrabajo;
};

//----------------------------------------------------
string to_string(Tarea t)
{
    return "{" + t.tipoTarea + "," + to_string(t.cargaDeTrabajo) + "}";
}

void sleep(int milisegundos) {
    this_thread::sleep_for(chrono::milliseconds(milisegundos));
}

string to_string(MultiBuffer<Tarea, N_CONTROLLERS> &bT)
{
    const string sep = "   ";
    string res = "[\n";
    for (unsigned i = 0; i < bT.size(); i++)
    {
        res = res + sep + "(" + to_string(bT.isNew(i)) + "):";
        if (bT.isNew(i))
        {
            res += to_string(bT.watch(i)) + "\n";
        }
        else
        {
            res += "???\n";
        }
    }
    res += "]";
    return res;
}

int buscarBufferLibre(MultiBuffer<Tarea, N_CONTROLLERS> mBT){
    int i=0;
    while(i < mBT.size() && mBT.isNew(i) == 1){
        i++;
    }
    return i;
}

int buscarBufferLleno(MultiBuffer<Tarea, N_CONTROLLERS> mBT){
    int i=0;
    while(i < mBT.size() && mBT.isNew(i) == 0){
        i++;
    }
    return i;
}

bool ejecucion(Tarea tareaEnEjecucion){
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
        tiempoTotalExitosoT1 += tiempoEjecucion.count();
    } else if (tareaEnEjecucion.tipoTarea == "t2") {
        tiempoTotalExitosoT2 += tiempoEjecucion.count();
    } else if (tareaEnEjecucion.tipoTarea == "t3") {
        tiempoTotalExitosoT3 += tiempoEjecucion.count();
    }
    return resulatado;
}

void escribirMatriz(vector<vector<double>>& matrizResultados, Tarea tareaController, bool resultado){
    if (tareaController.tipoTarea == "t1"){
        matrizResultados[0][0] ++;
        matrizResultados[0][2] += tiempoTotalExitosoT1;
        if (resultado) matrizResultados[0][1] ++;
    }
    else if (tareaController.tipoTarea == "t2"){
        matrizResultados[1][0] ++;
        matrizResultados[1][2] += tiempoTotalExitosoT2;
        if (resultado) matrizResultados[1][1] ++;
    }
    
    else if (tareaController.tipoTarea == "t3"){
        matrizResultados[2][0] ++;
        matrizResultados[2][2] += tiempoTotalExitosoT1;
        if (resultado) matrizResultados[2][1] ++;
    }
 
}

bool comprobarTf(MultiBuffer<Tarea, N_CONTROLLERS> mBT){
    int contador = 0;
    for (int i = 0; i < mBT.size(); i++){
        if (mBT.watch(i).tipoTarea == "TF") contador ++;
    }
    if (contador == mBT.size()) return true;
    return false;
}

void pasarTestigo(MultiBuffer<Tarea, N_CONTROLLERS> mBT, Semaphore &testigo, Semaphore &s_master, Semaphore &s_controller, int &master_bloq, int &controller_bloq)
{
    if (mBT.numberNews() < N_CONTROLLERS && master_bloq > 0){
        master_bloq--;
        s_master.signal();
    }
    else if (mBT.numberNews() > 0 && controller_bloq > 0){
        controller_bloq--;
        s_controller.signal();
    }
    else{
        testigo.signal();
    }
}

void master(MultiBuffer<Tarea, N_CONTROLLERS> &mBT, string archivoTareas, Semaphore &testigo, Semaphore &s_master, Semaphore &s_controller, int &master_bloq, int &controller_bloq, bool &final)
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


        testigo.wait();
        if (mBT.numberNews() >= N_CONTROLLERS)
        {
            master_bloq++;
            testigo.signal();
            s_master.wait();
        }        
        mBT.put(tarea, buscarBufferLibre(mBT));

        pasarTestigo(mBT, testigo, s_master, s_controller, master_bloq, controller_bloq);
        
    }
    while(!tf){
        testigo.wait();
        if (mBT.numberNews() >= N_CONTROLLERS)
        {
            master_bloq++;
            testigo.signal();
            s_master.wait();
        }        
        mBT.put({"TF", 0}, buscarBufferLibre(mBT));

        pasarTestigo(mBT, testigo, s_master, s_controller, master_bloq, controller_bloq);
        tf = comprobarTf(mBT);
    }

    final = true;

    archivo.close();
}

//----------------------------------------------------
// Pre:  0 <= id < N_CONTROLLERS
void controller(MultiBuffer<Tarea, N_CONTROLLERS> &mBT, Semaphore &testigo, Semaphore &s_master, Semaphore &s_controller, int &master_bloq, int &controller_bloq, vector<vector<double>>& matrizResultados, bool &final)
{
    Tarea tareaController;
    while (!final)
    {
        testigo.wait();
        if (mBT.numberNews() <= 0)
        {
            controller_bloq++;
            testigo.signal();
            s_controller.wait();
        }        
        tareaController = mBT.get(buscarBufferLleno(mBT));
        pasarTestigo(mBT, testigo, s_master, s_controller, master_bloq, controller_bloq); 
        escribirMatriz(matrizResultados, tareaController, ejecucion(tareaController));
    }
    for (int i = 0; i < N_CONTROLLERS; i++){
        mBT.put({"TF",0}, i);
    }


}
//----------------------------------------------------
int main(int argc, char *argv[])
{
    string fichero = "tareas.txt";
    Semaphore testigo(1);    
    Semaphore s_master(0);
    Semaphore s_controller(0);
    int master_bloq = 0, controller_bloq = 0;
    MultiBuffer<Tarea, N_CONTROLLERS> mBT; // multi-buffer de tareas
    bool final = false;
    
    vector<vector<double>> matrizResultados;

    matrizResultados.push_back({0, 0, 0});
    matrizResultados.push_back({0, 0, 0});
    matrizResultados.push_back({0, 0, 0});

    srand(static_cast<unsigned int>(time(nullptr)));

    thread master_thread(&master , ref(mBT), fichero, ref(testigo), ref(s_master), ref(s_controller), ref(master_bloq), ref(controller_bloq), ref(final));
    thread controller_thread(&controller, ref(mBT), ref(testigo), ref(s_master), ref(s_controller), ref(master_bloq), ref(controller_bloq), ref(matrizResultados), ref(final));

    master_thread.join();
    controller_thread.join();

    for (int i = 0; i < matrizResultados.size(); i++){

        cout << "[" << endl;
        cout << "   Tipo de tarea t" << i+1 << endl;
        cout << "   Total: " << matrizResultados[i][0] << endl;
        cout << "   Exito: " << matrizResultados[i][1] << endl;
        cout << "   Tiempo: " << matrizResultados[i][2] << endl;
        cout << "]" << endl;
        
    }

    return 0;
}
