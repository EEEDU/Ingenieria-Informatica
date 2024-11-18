//------------------------------------------------------------------------------
// File:   practica_1_V2.cpp
// Author: PSCD-Unizar
// Date:   septiembre 2020
// Coms:   Parte del práctica 1 de PSCD
//         Compilar mediante
//           g++ practica_1_V2.cpp -o practica_1_V2 -std=c++11 -pthread
//------------------------------------------------------------------------------

#include <iostream>
#include <thread>
#include <string>
#include <chrono>

using namespace std;

void saludo(int id, int retardo, int veces) {
    for(int i=1; i<=veces; i++) {
        cout << "Soy " << id << endl;
        // cout << "Soy " + nombre + "\n";
        //el thread que me ejecuta se bloquea durante "retardo" milisegundos
        this_thread::sleep_for(chrono::milliseconds(retardo));
    }
}

int main(int argc, char* argv[]) {
    const int N = 10;
    thread P[N]; //de momento, ningún thread se pone en marcha
    for (int i = 0; i<N; i++){
        int milisegundos = 100 + rand() % 201; // Genera un valor entre 100 y 300
        int repeticiones = 5 + rand() % 11; // Genera un valor entre 5 y 15
        cout << "El proceso " << i << " se reptite " << repeticiones << " con un retardo de " << milisegundos;

        P[i] = thread(&saludo, i, milisegundos, repeticiones); //P[i] se pone en marcha
    }

    for (int i=0; i<N; i++) { 
        P[i].join(); //me bloqueo hasta que "P[i]" termine
    }

    cout << "Fin\n";
    return 0;
}
