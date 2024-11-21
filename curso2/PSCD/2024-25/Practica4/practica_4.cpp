//------------------------------------------------------------------------------
// File:   practica_4.cpp
// Author: 816106
// Date:   noviembre 2024
// Coms:   Ejercicio de práctica 3 de PSCD
//         Compilar mediante Makefile_p4
//------------------------------------------------------------------------------

#include <iostream>
#include <thread>
#include <string>
#include <fstream>
#include <vector>
#include <MonitorSilla.hpp>
#include <MonitorDatos.hpp>
#include <MonitorFinal.hpp>

using namespace std;

// ----------------------------------------------------
const int N_EST = 60;        // # de estudiantes
const int N_FIL = N_EST / 2; // # de filas en la matriz
const int N_COL = 1000;      // # de columnas

// ----------------------------------------------------
// Pre : archivo donde se encuntran los datos
// Post : los datos en formato array[][]
void leerDatos(string nombreArchivo, int D[N_FIL][N_COL])
{
    // Intentamos abrir el archivo de datos
    ifstream archivo(nombreArchivo);
    if (!archivo) {
        cerr << "Error al abrir el archivo datos.txt" << endl;
    }

    // Vector temporal para almacenar los números del archivo
    vector<int> numeros;
    int numero;

    // Leemos todos los números desde el archivo
    while (archivo >> numero) {
        numeros.push_back(numero);
    }

    // Cerramos el archivo
    archivo.close();

    // Llenamos la matriz con los números leídos
    for (int i = 0; i < N_FIL; ++i) {
        for (int j = 0; j < N_COL; ++j) {
            D[i][j] = numeros[i * N_COL + j];
        }
    }

}

// Pre : <fila > es un indice de fila de <D>
// Post : devuelve el maximo de la fila <fila >
int calcularMax(int D[N_FIL][N_COL], int fila)
{
    int max = 0;
    for (int i = 0; i < N_COL; i++)
    {
        if (D[fila][i] > max) { max = D[fila][i]; }
    }
    return max;
}

// Pre : <fila > es un ´ı ndice de fila de <D>
// Post : devuelve la suma de los els . de la fila <fila >
int calcularSuma(int D[N_FIL][N_COL], int fila)
{
    int sum = 0;
    for (int i = 0; i < N_COL; i++)
    {
        sum += D[fila][i];
    }
    return sum;
}

void estudiante(int nip, int D[N_FIL][N_COL], MonitorSilla& mSilla, MonitorDatos& mDatos, MonitorFinal& mFinal)
{
    int numMax, suma;
    // Utilizar el monitor para sentarse en la silla
    mSilla.sentarse(nip);
    // Utilizar el monitor para levantarse de la silla recogiendo informacion <fila, pareja>
    tuple<int, int> datosEstudiante = mSilla.levantarse(nip);
    int filaEstudiante = get<0>(datosEstudiante);
    int parejaEstudiante = get<1>(datosEstudiante);
    
    // Si el estudiante es menor que su pareja, calcula el maximo, sino calcula la suma
    if (nip < parejaEstudiante)
    {
        // Obtener el maximo de la fila estudiante y enviarlo al monitor de datos
        numMax = calcularMax(D, filaEstudiante);
        mDatos.enviarMax(filaEstudiante, numMax);
    }
    else
    {
        // Obtener la suma de la fila estudiante y recoger informacion de la pareja
        suma = calcularSuma(D, filaEstudiante);
        numMax = mDatos.cogerMax(filaEstudiante);
        // mostrar resultados (fila,pareja-nip,max de fila,suma de fila)
        // 28 | 35 -54 | 54066 | 27864374
        cout << filaEstudiante << " | " <<  parejaEstudiante << "-" << nip << " | " << numMax << " | " << suma << endl;
    }
    mFinal.avisarFin();
}

void profesor(int D[N_FIL][N_COL], MonitorSilla& mSilla, MonitorFinal& mFinal)
{
    for (int fila = 0; fila < N_FIL; fila++)
    {
        // Asigna cada fila a una pareja
        mSilla.asignarInfo(fila);
    }        

    // Esperar a que todos los estudiantes hayan terminado
    mFinal.esperarFin();
}

int main()
{
    int D[N_FIL][N_COL]; // para almacenar los datos
    string nombreArchivo = "datos.txt";

    leerDatos(nombreArchivo, D);

    MonitorSilla mSilla(N_EST);
    MonitorDatos mDatos(N_FIL);
    MonitorFinal mFinal(N_EST);

    thread pProfesor = thread(&profesor, ref(D), ref(mSilla), ref(mFinal));
    thread pEstudiante[N_EST];

    for (int i=0; i<N_EST; i++) { 
        pEstudiante[i] = thread(&estudiante, i,  ref(D), ref(mSilla), ref(mDatos), ref(mFinal));
    }

    pProfesor.join();
    for (int i = 0; i < N_EST; i++)
    {
        pEstudiante[i].join(); //me bloqueo hasta que "P[i]" termine
    }
    cout << " Prueba finalizada \n";
    return 0;
}
