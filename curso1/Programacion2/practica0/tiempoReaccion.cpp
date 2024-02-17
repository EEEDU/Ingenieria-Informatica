#include <iostream>
#include <cstdlib>

using namespace std;

// Pre: numero de repeticiones
// Post: Indica el tiempo que tardas en dar la tecla enter desde que te muestran un mensaje
int main (int argc, char* argv[]){

	srand(time(NULL));

	clock_t tiempo = clock();

        for (int i = 0; i < argc; i++) {
                int numeroRandom = 1000 + rand() % (4000 - 1000);
		cout << numeroRandom << "\n";
        }

        return -1;
}
