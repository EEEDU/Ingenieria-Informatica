#include"Cuenta.h"
#include"CuentaCorriente.h"
#include"PlazoFijo.h"
#include"CuentaNomina.h"
#include"CuentaDivisa.h"
#include <iostream>
using namespace std;

double total(Cuenta* cuentas[], int n, int t) 
{
    double sol = 0.0;
    for (int i = 0; i<n; ++i) 
        sol += cuentas[i]->valor(t);
    return sol;
}
void mostrar_taes(Cuenta* cuentas[], int n) {
    cout<<"TAES:\t";
    for (int i = 0; i<n; ++i) 
        cout<<"  "<< cuentas[i]->tae() <<"\t";
    cout<<endl;
}

void actualizar(Cuenta* cuentas[], int n) {
    for (int i = 0; i<n; ++i) 
    {
        cuentas[i]->actualizar();
    }
}

int main() {


    Cuenta* cuentas[4];
    cuentas[0] = new CuentaCorriente(10.0, 2.0);
    cuentas[1] = new PlazoFijo(10.0, 2.0, 3);
    cuentas[2] = new CuentaNomina(10.0, 4.0);
    cuentas[3] = new CuentaDivisa(cuentas[0], 0.5);


    for (int i = 0; i<=6; ++i)
        cout<<i<<" -> "<<total(cuentas,4,i)<<"\t";
    cout<<endl;
    mostrar_taes(cuentas,4); 
    actualizar(cuentas,4);
    
    cout<<"Despues de actualizar"<<endl;
    for (int i = 0; i<=6; ++i)
        cout<<i<<" -> "<<total(cuentas,4,i)<<"\t";
    cout<<endl;
    

    for (int i = 0; i<4; ++i) 
        delete cuentas[i];
}