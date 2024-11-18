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

// Estructura de datos compartidos
struct DatosCompartidos {
    int (*D)[N_COL];       // Matriz de datos
    // int NIP;               // ID del estudiante
    int iSilla;
    int nipSilla[2];
    bool tienePareja[N_EST];
    int pareja[N_EST];
    int fila[N_EST];
    int numMax[N_EST];
    bool parejaTerminada[N_EST];
    int estudianteTerminado;
};

// Estructura de datos para la sincronización
struct Sincronizacion {
    Semaphore& testigo;
    Semaphore& sSillaVacia;
    Semaphore& sSillaOcupada;
    Semaphore& sFinEstudiantes;
    int& dSillaVacia;
    int& dSillaOcupada;
    int& dFinEstudiantes;
    int* dTienePareja;
    Semaphore** sTienePareja;
    int* dParejaTerminada;
    Semaphore** sParejaTerminada;
};

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

// Pre : <fila > es un ´ı ndice de fila de <D>
// Post : devuelve el m´a ximo de la fila <fila >
int maxFila(int D[N_FIL][N_COL], int fila)
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
int sumaFila(int D[N_FIL][N_COL], int fila)
{
    int sum = 0;
    for (int i = 0; i < N_COL; i++)
    {
        sum += D[fila][i];
    }
    return sum;
}

void pasarTestigo(Sincronizacion& sync, DatosCompartidos& datos)
{
    int estudiante = 0;
    bool pasadoTestigo = false;

    while ((estudiante<N_EST) && !pasadoTestigo)
    {   
        cout << "Estudiante " << estudiante << " tiene pareja " << datos.tienePareja[estudiante] << " con " << sync.dTienePareja[estudiante] << " procesos bloqueados" << endl;;
        if (datos.tienePareja[estudiante] && sync.dTienePareja[estudiante] > 0)
        {
            cout << "-------------------Estudiante " << estudiante << " ha entrado" << endl;
            sync.dTienePareja[estudiante]--;
            sync.sTienePareja[estudiante] -> signal();
            pasadoTestigo = true;
        }
        else
        {
            estudiante++;
        }
    }
    estudiante = 0;
    while ((estudiante<N_EST) && !pasadoTestigo)
    {
        if (datos.parejaTerminada[estudiante] && sync.dParejaTerminada[estudiante] > 0)
        {
            sync.dParejaTerminada[estudiante]--;
            sync.sParejaTerminada[estudiante] -> signal();
            pasadoTestigo = true;
        }
        else
        {
            estudiante++;
        }
    }

    if ((datos.iSilla >= 2) && sync.dSillaOcupada > 0 && !pasadoTestigo)
    {
        sync.dSillaOcupada--;
        sync.sSillaOcupada.signal();
        pasadoTestigo = true;
    }

    else if ((datos.iSilla < 2) && sync.dSillaVacia > 0 && !pasadoTestigo)
    {
        sync.dSillaVacia--;
        sync.sSillaVacia.signal();
        pasadoTestigo = true;
    }

    else if ((datos.estudianteTerminado >= N_EST) && sync.dFinEstudiantes > 0 && !pasadoTestigo)
    {
        sync.dFinEstudiantes--;
        sync.sFinEstudiantes.signal();
        pasadoTestigo = true;
    } 

    else
    {
        sync.testigo.signal();
    }
}

// ----------------------------------------------------
void Estudiante(int nip, DatosCompartidos& datos, Sincronizacion& sync)
{
    // cout << "Soy el estudiante " << nip << endl;
    int filaEstudiante;
    int numMaxEstudiante, sumaEstudiante;
    // ESPERO UNA SILLA LIBRE
    // < await iSilla < 2
    // nipSilla[iSilla] = nip
    // iSilla++ >
    sync.testigo.wait();
    if (datos.iSilla >= 2)
    {
        sync.dSillaVacia++;
        sync.testigo.signal();
        sync.sSillaVacia.wait();
    }
    datos.nipSilla[datos.iSilla] = nip;
    if (datos.iSilla <2 ) {datos.iSilla++;}

    pasarTestigo(sync, datos);


    // ESPERAR QUE ME SEA ASIGNADA UNA PARAJA Y UNA FILA
    // < await tienePareja[nip] = true
    // datos = fila >

    sync.testigo.wait();
    if (datos.tienePareja[nip] == false) 
    {
        sync.dTienePareja[nip]++;
        sync.testigo.signal();
        sync.sTienePareja[nip] -> wait();
    }
    filaEstudiante = datos.fila[nip];
    cout << "Soy " << nip << " mi parajea es " << datos.pareja[nip] << " y mi fila es " << filaEstudiante << endl;
    pasarTestigo(sync, datos);
    

    // if (nip < datos.pareja[nip])
    //     {

    //         // calcular max de mi fila
    //         numMaxEstudiante = maxFila(datos.D, filaEstudiante);

    //         // HACERSELO LLEGAR A MI PAREJA
    //         // < numMax[fila[nip]] = numMax >
    //         sync.testigo.wait();
    //         datos.numMax[datos.fila[nip]] = numMaxEstudiante;
    //         pasarTestigo(sync, datos);
    //         // < parejaTermiada[pareja[nip]] = true>
    //         sync.testigo.wait();
    //         datos.parejaTerminada[datos.pareja[nip]] = true;
    //         // cout << nip << "-" << datos.pareja[nip] << endl;
    //         pasarTestigo(sync, datos);
    //     }
    // else
    // {
    //     // calcular la suma de mi fila
    //     sumaEstudiante = sumaFila(datos.D, filaEstudiante);

    //     // COGER INFO DE MAX
    //     // < await parejaTermianda[nip] = true 
    //     // numMax = numMax[fila[pareja[nip]]] >
    //     sync.testigo.wait();
    //     if (!datos.parejaTerminada[nip]) 
    //     {
    //         sync.dParejaTerminada[nip]++;
    //         sync.testigo.signal();
    //         sync.sParejaTerminada[nip]->wait();
    //     }
    //     numMaxEstudiante = datos.numMax[datos.fila[datos.pareja[nip]]];
    //     // mostrar resultados (fila,pareja-nip,max de fila,suma de fila)
    //     // 28 | 35 -54 | 54066 | 27864374
    //     // cout << nip << "-" << datos.pareja[nip] << endl;
    //     // cout << filaEstudiante << " | " <<  datos.pareja[nip] << "-" << nip << " | " << numMaxEstudiante << " | " << sumaEstudiante << endl;
    //     pasarTestigo(sync, datos);

    //     // mostrar resultados (fila,pareja,max de fila,suma de fila)
    //     // 28 | 35 -54 | 54066 | 27864374
        

    //     // COMUNICAR FINAILIZACION
    //     // < estudianteTerminado += 2 >
    //     sync.testigo.wait();
    //     datos.estudianteTerminado += 2;
    //     pasarTestigo(sync, datos);
    // }
}
// ----------------------------------------------------
void Profesor(DatosCompartidos& datos, Sincronizacion& sync)
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
        // fila[nipSilla[1]] = i 
        // iSilla = 0>

        sync.testigo.wait();
        if (datos.iSilla < 2) 
        {
            sync.dSillaOcupada++;
            sync.testigo.signal();
            sync.sSillaOcupada.wait();
        }
        datos.tienePareja[datos.nipSilla[0]] = true;
        datos.tienePareja[datos.nipSilla[1]] = true;
        datos.pareja[datos.nipSilla[0]] = datos.nipSilla[1];
        datos.pareja[datos.nipSilla[1]] = datos.nipSilla[0];
        datos.fila[datos.nipSilla[0]] = i;
        datos.fila[datos.nipSilla[1]] = i;
        datos.iSilla = 0;
        pasarTestigo(sync, datos);
    }
    // // ESPERAR QUE TODOS HAYAN TERMIANDO
    // // < await estudianteTerminado == N_EST >
    // sync.testigo.wait();
    // if (datos.estudianteTerminado < N_EST) 
    // {
    //     sync.dFinEstudiantes++;
    //     sync.testigo.signal();
    //     sync.sFinEstudiantes.wait();
    // }
    // pasarTestigo(sync, datos);

}

// ----------------------------------------------------
int main()
{
    int D[N_FIL][N_COL]; // para almacenar los datos
    string nombreArchivo = "datos.txt";

    // Inicializar la estructura de datos compartidos
    DatosCompartidos datos = { D,                   // int (*D)[N_COL]
                            //    0,                   // int NIP
                               0,                   // int iSilla
                               {0},                 // int nipSilla[2]
                               {false},             // bool tienePareja[N_EST]
                               {0},                 // int pareja[N_EST]
                               {0},                 // int fila[N_EST]
                               {false},             // bool parejaTerminada[N_EST]
                               0 };                 // int estudianteTerminado

    Semaphore testigo(1), sSillaVacia(0), sSillaOcupada(0), sFinEstudiantes(0);
    int dSillaVacia = 0, dSillaOcupada = 0, dFinEstudiantes = 0;
    int dTienePareja[N_EST] = {0}, dParejaTerminada[N_EST] = {0};
    Semaphore* sTienePareja[N_EST];
    Semaphore* sParejaTerminada[N_EST];
    for (int i = 0; i < N_EST; i++) {
        sTienePareja[i] = new Semaphore(0);
        sParejaTerminada[i] = new Semaphore(0);
    }

    // Inicializar la estructura de sincronización
    Sincronizacion sync = { testigo,                // Semaphore& testigo      
                            sSillaVacia,            // Semaphore& sSillaVacia    
                            sSillaOcupada,          // Semaphore& sSillaOcupada    
                            sFinEstudiantes,        // Semaphore& sFinEstudiantes        
                            dSillaVacia,            // int& dSillaVacia    
                            dSillaOcupada,          // int& dSillaOcupada    
                            dFinEstudiantes,        // int& dFinEstudiantes        
                            dTienePareja,           // int* dTienePareja    
                            sTienePareja,           // Semaphore** sTienePareja    
                            dParejaTerminada,       // int* dParejaTerminada        
                            sParejaTerminada };     // Semaphore** sParejaTerminada     
                            
    // cargar " datos .txt " en "D"
    leerDatos(nombreArchivo, D);
    
    thread pProfesor = thread(&Profesor, ref(datos), ref(sync));
    thread pEstudiante[N_EST];

    for (int i=0; i<N_EST; i++) { 
        pEstudiante[i] = thread(Estudiante, i,  ref(datos), ref(sync));
    }

    pProfesor.join();
    for (int i = 0; i < N_EST; i++)
    {
        pEstudiante[i].join(); //me bloqueo hasta que "P[i]" termine
    }
    for (int i = 0; i < N_EST; i++) {
        cout << datos.fila[i] << endl;
    }
    cout << " Prueba finalizada \n";
    return 0;
}