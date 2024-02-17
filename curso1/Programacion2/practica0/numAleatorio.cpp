#include <iostream>
#include <ctime>

using namespace std;

void numeroRandom(int min, int max){
	int numeroRandom = min + (rand() % (max - min));
        cout << numeroRandom << endl;
}

int main(int argc, char *argv[]){

	time_t semilla = time(nullptr);
	srand(semilla);

	int numMin = stoi( argv[1] );	
	int numMax = stoi( argv[2] );
	
	for (int i = 0; i < stoi(argv[3]); i++){
		numeroRandom(numMin, numMax + 1);
	}
	return 0;
}
