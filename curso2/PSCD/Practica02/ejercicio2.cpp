//------------------------------------------------------------------------------
// File:   ejercicio21¡.cpp
// Author: Eduardo Guerrero (nip:816106) 
// Date:   octubre 2022
// Coms:   Parte del práctica 2 de PSCD, parte 2
//         Compilar mediante
//           g++ main_p2_e2.cpp -o main_p2_e2 -std=c++11 -pthread
//------------------------------------------------------------------------------

#include <fstream>  // Para ofstream
#include <iostream> // Para cout
#include <thread>
#include <atomic>

using namespace std;

const int N = 512;
const int N_BUSC = 8;
using VectInt = int[N];

/*
 * Carga N los datos del fichero datos.txt en el vector v.
 * Una vez los tenemos se igala a true la variable finCarga
 * Se espera a que terminen N_BUSC los procesos search
 * Se muestra por pantalla el trozo en el que mas veces aparece el valor buscado, y cuantas son estas
*/
void coordinador(VectInt v, bool& finCarga, bool vFin[], int& maxVeces, int& indMin, int& indMax)
{
    ifstream fich("datos.txt");
    if (fich.is_open())
    {
        //se lee el fichero
        for(int i=0; i<N; i++)
        {
            fich >> v[i];
        }

        //indica que ya se ha leido el fichero
        finCarga = true;

        //Espera a que los buscadores finalizen
        for(unsigned int i = 0; i < N_BUSC; i++)
        {
            while(!vFin[i]){}
        }

        cout << "El numero buscado aparece " << maxVeces << " en el trozo " << indMin << "-" << indMax << endl;
    }
    else
    {
        cerr << "No se pudo abrir el archivo \"datos.txt\"." << endl;
    }

}
//Pre: 0 ≤ i ≤ d ≤ N − 1
//Post: veces = Num α ∈ [i, d].value = v[α]
//      si veces > maxVeces entonces (maxVeces = veces indMin = i indMax = d)
void search(const VectInt v, const int i, const int d, const int value, int& maxVeces, int& indMin, int& indMax, atomic_flag& tas)
{
    int veces = 0;

    for(unsigned j = i; j <= d; j++){
        if(v[j] == value){
            veces++;
        }
    }

    //SC
    tas.test_and_set();
    if(veces > maxVeces)
    {
        maxVeces = veces;
        indMin = i;
        indMax = d;
    }
    tas.clear();
}

/*
 * Espera a que el coordinador cargue el vector
 * Una vez esta el vector v listo se llama a la funcion search
 * Cuando salga de la subrutina indica en su posicion del vector que ha terminado
*/
void searchAux(const VectInt v, const int i, const int d, const int value, int& maxVeces, int& indMin, int& indMax, atomic_flag& tas, bool& finCarga, bool vFin[], int numHilo)
{
    while(!finCarga){}

    search(v,i,d,value,maxVeces,indMin,indMax,tas);
    
    vFin[numHilo] = true;
}

int main()
{
    thread P[N_BUSC];
    VectInt v;
    bool finCarga = false;
    bool vFin[N_BUSC];
    int valorLeido;
    int maxVeces = 0;
    int indMin = 0;
    int indMax = 0;
    atomic_flag lock_stream = ATOMIC_FLAG_INIT;

    cout << "Introduzca el valor que desee ver cuantas veces esta en el vector: " << ends;
    cin >> valorLeido;
    cout << endl;

    //inicializa el vector vFin a false
    for(unsigned int i = 0; i < N_BUSC; i++){
        vFin[i] = false;
    }

    thread th_coor(&coordinador,v,ref(finCarga),vFin,ref(maxVeces),ref(indMin),ref(indMax));

    //ponemos en marcha los threads buscadores
    for (int i=0; i<N_BUSC; i++)
    {
        int izq = i*N/8;
        int der = ((i+1)*N/8)-1;

        P[i] = thread(&searchAux,v,izq,der,valorLeido,ref(maxVeces),ref(indMin),ref(indMax),ref(lock_stream),ref(finCarga),vFin,i);
    }

    th_coor.join();

    for (int i=0; i<N_BUSC; i++) {
        P[i].join();
    }
}