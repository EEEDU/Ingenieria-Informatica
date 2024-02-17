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

int size = 100;

void media(int vector[])
{
    int acum = 0;
    for (int i = 0; i < size; i++)
    {
        acum += vector[i];

        this_thread::sleep_for(chrono::milliseconds(1000));
    }
    cout << "media :" << acum / size << endl;
    this_thread::sleep_for(chrono::milliseconds(1000));
}

void maximo(int vector[], int size, int retardo)
{
    int max = 0;
    for (int i = 0; i < size; i++)
    {
        if (max < vector[i])
            max = vector[i];

        this_thread::sleep_for(chrono::milliseconds(retardo));
    }
    cout << "maximo :" << max << endl;
}

void minimo(int vector[], int size, int retardo)
{
    int min = 0;
    for (int i = 0; i < size; i++)
    {
        if (min > vector[i])
            min = vector[i];

        this_thread::sleep_for(chrono::milliseconds(retardo));
    }
    cout << "minimo :" << min << endl;
}

int main(int argc, char *argv[])
{
    int vector[size];
    for (int i = 0; i < size; i++)
    {
        vector[i] = rand() % 10;
        cout << vector[i] << ", ";
    }

    thread th_media(&media, vector[size]);
    // thread th_maximo(&maximo, vector[size], size, 100);
    // thread th_minimo(&minimo, vector[size], size, 100);

    // th_media.join();
    // th_maximo.join();
    // th_minimo.join();

    cout << "Fin\n";
    return 0;
}
