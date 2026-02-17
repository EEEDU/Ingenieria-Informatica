#include "Cuenta.h"
#include "CuentaCorriente.h"
#include "PlazoFijo.h"
#include <iostream>
using namespace std;

double total(Cuenta* cuentas[], int n, int t) 
{
    double result = 0.0;
    for (int i=0; i<n; i++)
    {
        result += cuentas[i]->valor(t);
   
    }
    return result;
}

void mostrar_taes(Cuenta* cuentas[], int n) {
    cout<<"TAES:\t";
    for (int i = 0; i<n; ++i) 
        cout<<"  "<< cuentas[i]->tae() <<"\t";
    cout<<endl;
}


int main() {

    Cuenta* cuentas[4];
    cuentas[0] = new CuentaCorriente(12.0, 2.0);
    cuentas[1] = new CuentaCorriente(10.0, 1.0);
    cuentas[2] = new PlazoFijo(12.0, 2.0, 2);
    cuentas[3] = new PlazoFijo(10.0, 1.0, 6);
    
    for (int i = 0; i<=6; ++i)
        cout<<i<<" -> "<<total(cuentas,4,i)<<"\t";
    cout<<endl;
        mostrar_taes(cuentas,4); 

    // LIBERAR MEMORIA
    for (int i = 0; i < 4; ++i) {
        delete cuentas[i];
    }
}