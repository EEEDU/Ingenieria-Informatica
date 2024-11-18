//------------------------------------------------------------------------------
// File:   ejercicio-2.cpp
// Author: Eduardo Guerrero
// Date:   septiembre 2023
// Coms:
//------------------------------------------------------------------------------

#include <iostream>
#include <thread>
#include <string>
#include <chrono>
#include <time.h>

using namespace std;

void saludo(int id, int retardo, int veces)
{
    for (int i = 1; i <= veces; i++)
    {
        // cout << "Soy " << nombre << endl;
        cout << "Soy " << id << "\n";
        id++;
        // el thread que me ejecuta se bloquea durante "retardo" milisegundos
        this_thread::sleep_for(chrono::milliseconds(retardo));
    }
}

int main(int argc, char *argv[])
{
    int randomNumber = 5 + rand() % (16 - 5);

    thread th_1(&saludo, 1, 100, randomNumber); // th_1 se pone en marcha

    th_1.join(); // me bloqueo hasta que "th_1" termine

    cout << "Fin\n";
    return 0;
}
