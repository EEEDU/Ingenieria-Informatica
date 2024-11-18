//------------------------------------------------------------------------------
// File:   main_p2_e2.cpp
// Author: 816106
// Date:   octubre 2024
// Coms:   Ejercicio 2 de práctica 2 de PSCD
//         Compilar mediante
//           g++ main_p2_e2.cpp -o main_p2_e2 -std=c++11 -pthread
//------------------------------------------------------------------------------

#include <iostream>
#include <thread>
#include <string>
#include <fstream>
#include <vector>
#include <atomic>         // std::atomic_flag

using namespace std;

const int N = 512;
const int N_BUSC = 8; //# de procesos buscadores
using VectInt = int[N]; //"VectInt" es un "alias"
                //para vector de int de dim. N

//-----------------------------------------------------
//Pre: 0 ≤ i ≤ d ≤ N − 1
//Post: result = Num α ∈ [i, d].value = v[α]
void search(const VectInt numeros, const int inicioBusqueda, const int finalBusqueda, const int numeroBuscado, int& maxVeces, int& indMin, int& indMax, atomic_flag& tas){
    int resultado = 0;
    for(int i=inicioBusqueda; i<finalBusqueda; i++) {
        if (numeros[i] == numeroBuscado) {
            resultado++;
        }
    }
    
    while (tas.test_and_set()) {}
    if (resultado > maxVeces) {
        maxVeces = resultado;
        indMin = inicioBusqueda;
        indMax = finalBusqueda;

        cout << maxVeces << indMin << indMax << endl;
    }
    tas.clear();

};

void coordinador(string fichero, VectInt& numeros, bool& datosCargado, int cantidadBuscador[], int& buscadorTerminado, int maxVeces, int indMax, int indMin) {  
    int index = 0; // Índice para controlar la posición en el arreglo

    ifstream archivo(fichero); // Abre el archivo para lectura
    if (!archivo.is_open()) { // Verifica si el archivo se abrió correctamente
        cerr << "No se pudo abrir el archivo" << endl;
        return; // Código de error
    }

    int numero;
    // Leer cada número del archivo y almacenarlo en el arreglo
    while (archivo >> numero && index < N) {
        numeros[index++] = numero;
    }

    cout << endl;

    archivo.close(); // Cierra el archivo

    datosCargado = true;

    while (buscadorTerminado<8){
    }

    cout << "\nValor maxiom: " << maxVeces << " se encuentra entre " << indMin << " y " << indMax << endl;

    return;
}

void buscador(bool& datosCargados, VectInt numeros, int idBuscador, int numeroBuscado, int cantidadBuscador[], int& buscadorTerminado, int&maxVeces, int& indMax, int& indMin , atomic_flag& tas) {

    while(!datosCargados) {
        // this.thread::yield();
    }
    cout << "entrando en buscador " << idBuscador << endl;

    int inicioBuscador = (N/N_BUSC)*idBuscador;
    int finalBuscador =  ((N/N_BUSC)*(idBuscador+1));

    search(numeros, inicioBuscador, finalBuscador, numeroBuscado, maxVeces, indMin, indMax, tas);

    buscadorTerminado++;

    return;
}

int main(int argc, char const *argv[])
{
    VectInt numeros;   
    int buscadorTerminado = 0;
    bool datosCargados = false;
    int cantidadBuscador [N_BUSC];
    int resultado;

    int maxVeces = 0;
    int indMin = 0;
    int indMax = 0;
    atomic_flag tas;

    string fichero = "datos.txt";
    int numeroBuscado = stoi(argv[1]);

    thread pBuscadores[N_BUSC];
    thread pCoordinador;
    
    pCoordinador = thread(&coordinador, fichero, ref(numeros), ref(datosCargados), cantidadBuscador, ref(buscadorTerminado), ref(maxVeces), ref(indMax), ref(indMin));

    for (int i=0; i<N_BUSC; i++) { 
        pBuscadores[i] = thread(&buscador, ref(datosCargados), ref(numeros), i, numeroBuscado, cantidadBuscador, ref(buscadorTerminado), ref(maxVeces), ref(indMax), ref(indMin), ref(tas));
    }

    pCoordinador.join();
    for (int i = 0; i < N_BUSC; i++)
    {
        pBuscadores[i].join(); //me bloqueo hasta que "P[i]" termine
    }
    

    return 0;
}

