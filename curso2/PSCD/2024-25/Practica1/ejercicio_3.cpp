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

void media(int datos[], int& valMedia, bool& mediaLista) {
    int suma = 0;
    for(int i=1; i<=sizeof(datos); i++) {
        suma += datos[i];
    }
    valMedia = suma/sizeof(datos);
    mediaLista = true;
}

void maxmin(int datos[], int& max, int& min) {
    for(int i=1; i<=sizeof(datos); i++) {
        if (datos[i] > max) { max = datos[i]; }
        if (datos[i] < min) { min = datos[i]; }
    }
}

void desviacion(int datos[], int& valDesviacion, int media, bool& mediaLista) {
    cout << "Hola" << endl;
}

int main(int argc, char* argv[]) {
    const int numDatos = 100;
    bool mediaLista = false;
    thread tMedia, tMaxmin, tDesviacion;

    int datos[numDatos];
    int valMedia, valMax, valMin, valDesviacion;

    // Inicializar los 100 datos
    for (int i = 0; i < 100; i++){
        datos [i] = 1 + rand() % 100; // Genera un valor entre 1 y 100
        cout << datos[i] << ", ";
    }

    tMedia = thread(&media, ref(datos), ref(valMedia), ref(mediaLista));
    tMaxmin = thread(&maxmin, ref(datos), ref(valMax), ref(valMin));
    tDesviacion = thread(&desviacion, ref(datos), ref(valDesviacion), ref(valMedia), ref(mediaLista));


    tMedia.join(); //me bloqueo hasta que "P[i]" termine
    tMaxmin.join(); //me bloqueo hasta que "P[i]" termine
    tDesviacion.join();

    cout << "Media: " << valMedia << endl;
    cout << "Max: " << valMax << endl;
    cout << "Min: " << valMin << endl;

    cout << "Fin\n";
    return 0;
}
