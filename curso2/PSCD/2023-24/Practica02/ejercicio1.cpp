//------------------------------------------------------------------------------
// File:   ejercicio 1.cpp
// Author: Eduardo Guerrero (nip:816106)
// Date:   Octuber 2023
// Coms:   Parte del práctica 2 de PSCD, parte 1
//         Compilar mediante
//           g++ main_p2_e1.cpp -o main_p2_e1 -std=c++11 -pthread
//------------------------------------------------------------------------------

#include <fstream>  
#include <iostream>
#include <thread>

using namespace std;

const int N = 512;
const int N_BUSC = 8;
using VectInt = int[N];

/*
 * Carga N los datos del fichero datos.txt en el vector v.
 * Una vez los tenemos se igala a true la variable finCarga
 * Se espera a que terminen N_BUSC los procesos search
 * Se muestra por pantalla el numero de veces que aparece el valor buscado
*/
void coordinador(VectInt v, bool& finCarga, bool vFin[], int vResultado[])
{
    int suma = 0;

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
        
        //se suman todos los elementos del vector
        for(unsigned i = 0; i < N_BUSC; i++){
            suma+= vResultado[i];
        }

        cout << "El numero buscado aparece: " << suma << endl;
        

    }
    else
    {
        cerr << "No se pudo abrir el archivo \"datos.txt\"." << endl;
    }

}

//Pre: 0 ≤ i ≤ d ≤ N − 1
//Post: result = Num α ∈ [i, d].value = v[α]
void search(const VectInt v, const int i, const int d, const int value, int& result)
{
    for(unsigned j = i; j <= d; j++){
        if(v[j] == value){
            result++;
        }
    }
}

/*
 * Espera a que el coordinador cargue el vector
 * Una vez esta el vector v listo se llama a la funcion search
 * Cuando salga de la subrutina indica en su posicion del vector que ha terminado
*/
void searchAux(const VectInt v, const int i, const int d, const int value, int& result, bool& finCarga, bool vFin[], int numHilo)
{
    while(!finCarga){}

    search(v,i,d,value,result);
    
    vFin[numHilo] = true;
}

int main()
{
    thread P[N_BUSC];
    VectInt v;
    bool finCarga = false;
    bool vFin[N_BUSC];
    int vResultado[N_BUSC];
    int valorLeido;

    cout << "Introduzca el valor que desee ver cuantas veces esta en el vector: " << ends;
    cin >> valorLeido;
    cout << endl;

    //inicializa el vector vFin a false
    for(unsigned int i = 0; i < N_BUSC; i++){
        vFin[i] = false;
    }

    //inicializa el vector vResultado a 0
    for(unsigned int i = 0; i < N_BUSC; i++){
        vResultado[i] = 0;
    }

    thread th_coor(&coordinador,v,ref(finCarga),vFin,vResultado);

    //ponemos en marcha los threads buscadores
    for (int i=0; i<N_BUSC; i++)
    {
        int izq = i*N/8;
        int der = ((i+1)*N/8)-1;

        P[i] = thread(&searchAux,v,izq,der,valorLeido,ref(vResultado[i]),ref(finCarga),vFin,i);
    }

    th_coor.join();

    for (int i=0; i<N_BUSC; i++) {
        P[i].join();
    }
}