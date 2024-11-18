//------------------------------------------------------------------------------
// File:   practica_3.cpp
// Author: 816106
// Date:   octubre 2024
// Coms:   Ejercicio 1 de práctica 3 de PSCD
//         Compilar mediante
//           g++ main_p2_e1.cpp -o main_p2_e1 -std=c++11 -pthread
//------------------------------------------------------------------------------

#include <iostream>
#include <thread>
#include <string>
#include <fstream>
#include <vector>
#include <Semaphore_V4.hpp>

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

    // Verificamos que la cantidad de números sea la correcta
    if (numeros.size() != N_FIL * N_COL) {
        cerr << "Error: se requieren exactamente " << N_FIL * N_COL << " números en el archivo." << endl;
    }


    // Llenamos la matriz con los números leídos
    for (int i = 0; i < N_FIL; ++i) {
        for (int j = 0; j < N_COL; ++j) {
            D[i][j] = numeros[i * N_COL + j];
        }
    }

}

// Pre : <fila > es un ´ı ndice de fila de <D>
// Post : devuelve el m´a ximo de la fila <fila >
int maxFila(int D[N_FIL][N_COL], int fila)
{
    int max = 0;
    return max;
}

// Pre : <fila > es un ´ı ndice de fila de <D>
// Post : devuelve la suma de los els . de la fila <fila >
int sumaFila(int D[N_FIL][N_COL], int fila)
{
    int sum = 0;
    return sum;
}

void pasarTestigo(){}

// ----------------------------------------------------
void Estudiante(int D[N_FIL][N_COL],int nip, int& iSilla, int nipSilla[], bool tienePareja[], int pareja[], int fila[], int numMax[], bool parejaTerminada[], int& estudianteTerminado, Semaphore& testigo, int& dSillaVacia, Semaphore& sSillaVacia, int dTienePareja[], Semaphore sTienePareja[], int dParejaTerminada[], Semaphore sParejaTerminada[])
{
    int filaEstudiante;
    int numMaxEstudiante, sumaEstudiante;
    // ESPERO UNA SILLA LIBRE
    // < await iSilla < 2
    // nipSilla[iSilla] = nip
    // iSilla++ >

    testigo.wait();
    if (iSilla >= 2)
    {
        dSillaVacia++;
        testigo.signal();
        sSillaVacia.wait();
    }
    nipSilla[iSilla] = nip;
    iSilla++;
    pasarTestigo();

    // ESPERAR QUE ME SEA ASIGNADA UNA PARAJA Y UNA FILA
    // < await tienePareja[nip] = true
    // datos = fila >

    testigo.wait();
    if (tienePareja[nip] = false) 
    {
        dTienePareja[nip]++;
        testigo.signal();
        sTienePareja[nip].wait();

    }
    filaEstudiante = fila[nip];
    pasarTestigo();

    if (nip < pareja[nip])
        {
            // calcular max de mi fila
            numMaxEstudiante = maxFila(D, filaEstudiante);

            // HACERSELO LLEGAR A MI PAREJA
            // < numMax[fila[nip]] = numMax >
            testigo.wait();
            numMax[fila[nip]] = numMaxEstudiante;
            pasarTestigo();
            // < parejaTermiada[pareja[nip]] = true>
            testigo.wait();
            parejaTerminada[pareja[nip]] = true;
            pasarTestigo();
        }
    else
    {
        // calcular la suma de mi fila
        sumaEstudiante = sumaFila(D, filaEstudiante);

        // COGER INFO DE MAX
        // < await parejaTermianda[nip] = true
        // numMax = numMax[fila[pareja[nip]]] >
        testigo.wait();
        if (!parejaTerminada[nip]) 
        {
            dParejaTerminada[nip]++;
            testigo.signal();
            sParejaTerminada[nip].wait();
        }
        numMaxEstudiante = numMax[fila[pareja[nip]]];
        pasarTestigo();

        // mostrar resultados (fila,pareja,max de fila,suma de fila)
        // 28 | 35 -54 | 54066 | 27864374
        cout << filaEstudiante << " | " <<  pareja[nip] << "-" << nip << " | " << numMaxEstudiante << " | " << sumaEstudiante;
        

        // COMUNICAR FINAILIZACION
        // < estudianteTerminado += 2 >
        testigo.wait();
        estudianteTerminado += 2;
        pasarTestigo();
    }
}
// ----------------------------------------------------
void Profesor(int& iSilla, int nipSilla[], bool tienePareja[], int pareja[], int fila[], int& estudianteTerminado, Semaphore& testigo, Semaphore& sSillaOcupada, int& dSillaOcupada, Semaphore& sFinEstudiantes, int& dFinEstudiantes)
{
    for (int i = 0; i < N_FIL; i++)
    {
        // ESPERAR QUE HAYA DOS
        // < await iSilla = 2
        // tienePareja[nipSilla[0]] = true
        // tienePareja[nipSilla[1]] = true
        // pareja[nipSilla[0]] = nipSilla[1]
        // pareja[nipSilla[1]] = nipSilla[0]
        // fila[nipSilla[0]] = i
        // fila[nipSilla[1]] = i >

        testigo.wait();
        if (iSilla < 2) 
        {
            dSillaOcupada++;
            testigo.signal();
            sSillaOcupada.wait();
        }
        tienePareja[nipSilla[0]] = true;
        tienePareja[nipSilla[1]] = true;
        pareja[nipSilla[0]] = nipSilla[1];
        pareja[nipSilla[1]] = nipSilla[0];
        fila[nipSilla[0]] = i;
        fila[nipSilla[1]] = i;
        pasarTestigo();
    }
    // ESPERAR QUE TODOS HAYAN TERMIANDO
    // < await estudianteTerminado == N_EST >
    testigo.wait();
    if (estudianteTerminado < N_EST) 
    {
        dFinEstudiantes++;
        testigo.signal();
        sFinEstudiantes.wait();
    }
    pasarTestigo();

}

// ----------------------------------------------------
int main()
{
    int D[N_FIL][N_COL]; // para almacenar los datos
    // int fila = 0;        // cada pareja coger ´a una
    int pareja[N_EST];   // pareja [i] ser´a la pareja asignada

    string nombreArchivo = "datos.txt";

    int iSilla = 0;
    int nipSilla[2] = {0};
    bool tienePareja[N_EST] = {false};
    int fila[N_EST] = {0};
    int numMax[N_EST] = {0};
    bool parejaTerminada[N_EST] = {false};
    int estudianteTerminado = 0;

    Semaphore testigo(1), sSillaVacia(0), sSillaOcupada(0), sFinEstudiantes(0);
    int dSillaVacia = 0, dTienePareja[N_EST] = {0}, dSillaOcupada = 0, dFinEstudiantes = 0, dParejaTerminada[N_EST] = {0};
    Semaphore* sTienePareja[N_EST];
    Semaphore* sParejaTerminada[N_EST];

    thread pEstudiante[N_EST];
    thread pProfesor;

    // cargar " datos .txt " en "D"
    leerDatos(nombreArchivo, D);
    
    pProfesor = thread(&Profesor, ref(iSilla), nipSilla, tienePareja, pareja, fila, ref(estudianteTerminado), ref(testigo), ref(sSillaOcupada), ref(dSillaOcupada), ref(sFinEstudiantes), ref(dFinEstudiantes));
    for (int i=0; i<N_EST; i++) { 
        sTienePareja[i] = new Semaphore(0);
        sParejaTerminada[i] = new Semaphore(0);
        pEstudiante[i] = thread(&Estudiante, D, i, ref(iSilla), nipSilla, tienePareja, pareja, fila, numMax, parejaTerminada, ref(estudianteTerminado), ref(testigo), ref(dSillaVacia), ref(sSillaVacia), dTienePareja, sTienePareja), dParejaTerminada, sParejaTerminada;
    }

    pProfesor.join();
    for (int i = 0; i < N_EST; i++)
    {
        pEstudiante[i].join(); //me bloqueo hasta que "P[i]" termine
    }
    
    cout << " Prueba finalizada \n";
    return 0;
}