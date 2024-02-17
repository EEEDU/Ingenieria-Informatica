#include <iostream>
#include <ctime>

using namespace std;

int main(int argc, char* argv[]){
	int repeticiones = atoi(argv[1]);
	for (int j = 0; j < repeticiones; j++){

        	clock_t tiempoInicio = clock();
		cout << clock() << "\n";
		cout << time(nullptr) << "\n";
		cout << "Pulsa enter...\n",
		cin.get();

        	for (int i = 0; i < 1000000000; i++);
		
        	clock_t tiempoFinal = clock();
        	cout << clock() << "\n";
		cout << time(nullptr) << "\n";
		cout << "El tiempo que tarda es de " << (double)(tiempoFinal - tiempoInicio) / CLOCKS_PER_SEC << " segundos" << endl;
	}
        return 0;
}
