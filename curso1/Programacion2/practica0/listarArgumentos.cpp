#include <iostream>

using namespace std;

// Pre: --
// Post: Preseta por la salida estandar un listado numerado
// 	de los argumentos dados por la linea de ordenes
int main (int argc, char* argv[]){
	time_t tiempoInicial = time(nullptr);

	for (int i = 0; i < argc; i++) {
		cout << i + 1 << ". " << argv[i] << endl;
	}

	time_t tiempoFinal = time(nullptr);
	
	int segundos = int (tiempoFinal - tiempoInicial);
	std::cout << tiempoFinal;
	std::cout << "Tiempo transcurrido: " + segundos;
	
	return 0;
}
