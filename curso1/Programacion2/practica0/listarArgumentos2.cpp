#include <iostream>
#include <ctime>

using namespace std;

// Pre: --
// Post: Preseta por la salida estandar un listado numerado
//      de los argumentos dados por la linea de ordenes
int main (int argc, char* argv[]){
        clock_t tiempo = clock();

        for (int i = 0; i < argc; i++) {
                cout << i + 1 << ". " << argv[i] << endl;
        }

	tiempo = clock() - tiempo;
	double segundos = double(tiempo) / CLOCKS_PER_SEC;
	cout << segundos << " segundos" << endl;

        return 0;
}
