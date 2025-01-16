
using namespace std;

// Función para ejecutar una instancia del programa Alumno
void ejecutarAlumno(int id) {
stringstream comando;
comando << "./Alumno " << id;
FILE* proceso = popen(comando.str().c_str(), "r+");
if (!proceso) {
cerr << "Error al ejecutar Alumno " << id << endl;
return;
}

// Enviar "sentar,<id>"
fprintf(proceso, "sentar,%d\n", id);
fflush(proceso);

char buffer[1024];
while (fgets(buffer, sizeof(buffer), proceso)) {
    string mensaje(buffer);
    mensaje.pop_back(); // Remueve el salto de línea

    if (mensaje.rfind("nipPareja,", 0) == 0) {
        int nipPareja = stoi(mensaje.substr(10));
        string respuesta = (id < nipPareja) ? "sumFila" : "maxFila";
        fprintf(proceso, "%s\n", respuesta.c_str());
        fflush(proceso);
    } else if (mensaje == "END OF SERVICE") {
        break;
    }
}

pclose(proceso);

}

int main() {
vector hilos;

// Crear y lanzar 60 hilos, cada uno ejecutando una instancia de Alumno
for (int i = 1; i <= 60; i++) {
    hilos.emplace_back(ejecutarAlumno, i);
}

// Esperar a que todos los hilos terminen
for (auto& hilo : hilos) {
    hilo.join();
}

cout << "Todos los alumnos han terminado." << endl;
return 0;

}