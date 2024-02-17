#include <iostream>
#include <ctime>

using namespace std;

int main(int argc, char* argv[]){

	clock_t tiempoInicio = clock();
	
	for (int i = 0; i < 1000000000; i++);

	clock_t tiempoFinal = clock();
	cout << "El tiempo que tarda es de " << (double)(tiempoFinal - tiempoInicio) / CLOCKS_PER_SEC << " segundos" << endl;
	
	return 0;
}
