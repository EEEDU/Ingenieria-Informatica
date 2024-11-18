//------------------------------------------------------------------------------
// File:   practica_1_V1.cpp
// Author: PSCD-Unizar
// Date:   septiembre 2020
// Coms:   Parte del práctica 1 de PSCD
//         Compilar mediante
//           g++ practica_1_V1.cpp -o practica_1_V1 -std=c++11 -pthread
//------------------------------------------------------------------------------

#include <iostream>
#include <thread>
#include <string>
#include <chrono>

using namespace std;
const int N_VECES = 100000;

void inc_1(float& x){
    for(int i=1; i<=N_VECES; i++) {
        x = x+1;
    }
}

void inc_2(float& x){
    for(int i=1; i<=N_VECES; i++) {
        x = x+2;
    }
}

int main(int argc, char* argv[]) {
    thread P,Q;
    float x = 0;
    
    P = thread(&inc_1, ref(x));
    Q = thread(&inc_2, ref(x));

    P.join();
    Q.join();

    cout << "Valor de x: " << x << endl;
    return 0;
}
