#include /*  .....    */
#include <iostream>
using namespace std;

double total(Cuenta* cuentas[], int n, int t) {
    double sol = 0.0;
    for (int i = 0; i<n; ++i) 
        sol += /*  .....    */
    return sol;
}
void mostrar_taes(Cuenta* cuentas[], int n) {
    cout<<"TAES:\t";
    for (int i = 0; i<n; ++i) 
        cout<<"  "<</*  .....    */<<"\t";
    cout<<endl;
}

void actualizar(Cuenta* cuentas[], int n) {
    for (int i = 0; i<n; ++i) /*  .....    */
}

int main() {


    Cuenta* cuentas[4];
    cuentas[0] = /*  .....    */
    cuentas[1] = /*  .....    */
    cuentas[2] = /*  .....    */
    cuentas[3] = /*  .....    */

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
    /*  .....    */
}