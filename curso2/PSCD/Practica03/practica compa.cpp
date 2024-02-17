#include <Semaphore_V4.hpp>
#include <MultiBuffer.hpp>
#include <fstream>

using namespace std;
const int N_CONTROLLERS = 10;
const int TAM_TAREAS = 1000;
tarea tareaActual;

//----------------------------------------------------
struct tarea {
    string tipoTarea;
    float cargaDeTrabajo;
};
struct matrizDeResultados{
	
};

float tasaDeFallo(const string& tipoTarea) {
    
    if (tipoTarea == "t1") {
        return 0.05;
    } else if (tipoTarea == "t2") {
        return 0.07;  
    } else if (tipoTarea == "t3") {
        return 0.10;  
    }

    return 0.0; 
}

//----------------------------------------------------
void masterTask(... MultiBuffer<tarea,N_CONTROLLERS> mBT) {
    vector<tarea> tareas;
    float i=0.0;
    ifstream fich("tareas.txt");
    // Verifica si se pudo abrir el archivo
    if (!fich.is_open()) {
        cerr << "No se pudo abrir el archivo." << endl;
        return 1;
    }

    string linea;
    while (getline(fich, linea)) {
        istringstream ss(linea);  // Utilizamos un stringstream para dividir la línea
        tarea nuevaTarea;
        if (ss >> nuevaTarea.tipoTarea >> nuevaTarea.cargaDeTrabajo) {
		if(mBT.numberNews() < mBT.size()){
			mBT.put(nuevaTarea, i);
			i++;
		}
           
    }

    fich.close();

    //Tareas pendientes
    while (i>0) {
    	if(mBT.isNew(i) == true){
    		pasartestigo();
    		
    		PREGUNTAR SI HAY QUE HACER UNO CON AWAITS 
    	}
        //poner nueva tarea en buffer libre, cuando lo haya
        
    }
    //poner tareas de fin en cada buffer
    //mostrar matriz de resultados cuando se pueda
}

void pasarTestigo(Semaphore& testigo){
	while()
}
//----------------------------------------------------
//Pre:  0 <= id < N_CONTROLLERS
void controllerTask(unsigned int id,...) {
    bool seguir = true;
    while (seguir) {
        //tomar tarea del buffer correspondiente cuando la haya
        tareaActual = mBT.get(i)
        //ejecutarla
        //completar datos en la matriz de resultados
        ...
    }
}
//----------------------------------------------------
int main(int argc, char *argv[]) {
    MultiBuffer<tarea,N_CONTROLLERS> mBT; //multi-buffer de tareas
	...
    return 0;
}