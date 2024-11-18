#include <Semaphore_V4.hpp>
#include <MultiBuffer.hpp>
#include <fstream>

using namespace std;
const int N_CONTROLLERS = 10;
const int TAM_TAREAS = 1000;
tarea tareaActual;
 vector<Semaphore> controlers(N_CONTROLLERS);

//----------------------------------------------------
struct tarea {
    string tipoTarea;
    float cargaDeTrabajo;
};
struct matrizDeResultados{
    int tareasEjecutadasT1;
    int tareasEjecutadasT2;
    int tareasEjecutadasT3;
    int tareasExitosasT1;
    int tareasExitosasT2;
    int tareasExitosasT3;
    float tiempoTotalExitosoT1;
    float tiempoTotalExitosoT2;
    float tiempoTotalExitosoT3;
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

void imprimirResultados(matrizDeResultados resultados){
	// Imprimir los resultados
	cout << "Resultados de la matriz:" << endl;
	cout << "Tareas ejecutadas T1: " << resultados.tareasEjecutadasT1 << endl;
	cout << "Tareas ejecutadas T2: " << resultados.tareasEjecutadasT2 << endl;
	cout << "Tareas ejecutadas T3: " << resultados.tareasEjecutadasT3 << endl;
	cout << "Tareas exitosas T1: " << resultados.tareasExitosasT1 << endl;
	cout << "Tareas exitosas T2: " << resultados.tareasExitosasT2 << endl;
	cout << "Tareas exitosas T3: " << resultados.tareasExitosasT3 << endl;
	cout << "Tiempo total exitoso T1: " << resultados.tiempoTotalExitosoT1.count() << " segundos" << endl;
	cout << "Tiempo total exitoso T2: " << resultados.tiempoTotalExitosoT2.count() << " segundos" << endl;
	cout << "Tiempo total exitoso T3: " << resultados.tiempoTotalExitosoT3.count() << " segundos" << endl;
}


void ejecutarTarea(tarea& tareaActual, matrizDeResultados& resultados) {
    srand(static_cast<unsigned int>(time(nullptr))); //ini
    float numeroAleatorio = static_cast<float>(rand()) / RAND_MAX;
    // Medir el tiempo de inicio
    auto start_time = high_resolution_clock::now();
    // Realiza la ejecución de la tarea según el tipo
    if (tareaActual.tipoTarea == "t1") {
    	    resultados.tareasEjecutadasT1++;
            if(numeroAleatorio >=tasaDeFallo(tareaActual.tipoTarea)){
            	resultados.tareasExitosasT1++
            }
        resultados.tareasExitosasT1++;
    } else if (tareaActual.tipoTarea == "t2") {
    	    resultados.tareasEjecutadasT2++;
            if(numeroAleatorio >=tasaDeFallo(tareaActual.tipoTarea)){
     		   resultados.tareasExitosasT2++;
     	    }
    } else if (tareaActual.tipoTarea == "t3") {
    	    resultados.tareasEjecutadasT3++;
            if(numeroAleatorio >=tasaDeFallo(tareaActual.tipoTarea)){
        	resultados.tareasExitosasT3++;
        }
    }
     // Medir el tiempo de finalización
    auto end_time = high_resolution_clock::now();

    // Calcular la duración del tiempo de ejecución
    duration<double> tiempoEjecucion = end_time - start_time;
    if (tareaActual.tipoTarea == "t1") {
        resultados.tiempoTotalExitosoT1 += tiempoEjecucion;
    } else if (tareaActual.tipoTarea == "t2") {
        resultados.tiempoTotalExitosoT2 += tiempoEjecucion;
    } else if (tareaActual.tipoTarea == "t3") {
        resultados.tiempoTotalExitosoT3 += tiempoEjecucion;
    }
}

//----------------------------------------------------
void masterTask(... MultiBuffer<tarea,N_CONTROLLERS> mBT) {
    vector<tarea> tareas;
    float i=0.0;
    int id;
    

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
        	testigo.wait();
		if(mBT.numberNews() < mBT.size()){
			testigo.signal();
            		s_buffer.wait();
			mBT.put(nuevaTarea, i);
			id++;
			i++;
		}
		
	   pasarTestigo();
           
	   if(tareaActual.tipoTarea == "TF"){
	   	termina++;
	    	imprimirResultados(resultados);
		testigo.signal();
		termina.wait();
		pasarTestigo();
	    }
	    
    }
	
    fich.close();

    //Tareas pendientes
    /*while (i>0) {
    	if(mBT.isNew(i) == true){
    		tarea tareaActual;
    		testigo.signal();
    		
    	}*/
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
    float tasa;
    
  
        //tomar tarea del buffer correspondiente cuando la haya
        testigo.wait();
        if(id < N_CONTROLLERS){
		tareaActual = mBT.get(i);
		matrizDeResultados.tareasEjecutadas++;
		if (tareaActual.tipoTarea == "TF") {
		    fin = true;  // Marcar la finalización del controlador
		} else {
	   		ejecutarTarea(tareaActual.tipoTarea, matrizDeResultados);
	   		id--;
	   		testigo.signal();
			controlers[id].wait();
		    // Actualiza la matriz de resultados según los resultados de la tarea
		}
		testigo.signal();
		controlers[id].wait();
		
		//completar datos en la matriz de resultados
        }
        pasarTestigo();
    
}
//----------------------------------------------------
int main(int argc, char *argv[]) {
    MultiBuffer<tarea,N_CONTROLLERS> mBT; //multi-buffer de tareas
	...
    return 0;
}