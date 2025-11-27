#include <iostream>
#include <string>
#include <fstream>
#include <map>
#include <functional>
#include <limits> // Añadir al inicio


#include "colecInterdep.hpp"
#include "evento.hpp"
using namespace std;

void aniadir(colecInterdep<string,Evento>& coleccion, ifstream& entrada);
void cambiar(colecInterdep<string,Evento>& coleccion, ifstream& entrada);
void obtener(colecInterdep<string,Evento>& coleccion, ifstream& entrada);
void existeEvento(colecInterdep<string,Evento>& coleccion, ifstream& entrada);
void hacerEventoIndependiente(colecInterdep<string,Evento>& coleccion, ifstream& entrada);
void hacerEventoDependiente(colecInterdep<string,Evento>& coleccion, ifstream& entrada);
void borrarEvento(colecInterdep<string,Evento>& coleccion, ifstream& entrada);
void listarDependientes(colecInterdep<string,Evento>& coleccion, ifstream& entrada);
void listarTodos(colecInterdep<string,Evento>& coleccion, ifstream& entrada); // no se necesita la entrada

int main() {
    
    // Crear coleccion vacia
    colecInterdep<string, Evento> coleccion;
    crear(coleccion);

    ifstream f_in;
    ofstream f_out;

    // Abriar archivo de entrada
    f_in.open("entrada.txt");

    // Error si no se puede abrir el archivo
    if (!f_in.is_open()){
        cerr << "ERROR: no se ha podido abrir el archivo entrada.txt" << endl;
        return 1;
    }

    string instruccion, salto;
    map<string, function<void(colecInterdep<string,Evento>&, ifstream&)>> operaciones = {
        {"A", aniadir},
        {"C", cambiar},
        {"O", obtener},
        {"E", existeEvento},
        {"I", hacerEventoIndependiente},
        {"D", hacerEventoDependiente},
        {"B", borrarEvento},
        {"LD", listarDependientes},
        {"LT", listarTodos}  
    };

    // Leer instrucciones hasta el final del archivo
    while (f_in >> instruccion)
    {
        getline(f_in, salto); // Consumir el resto de la línea después de la instrucción

        if(operaciones.find(instruccion) != operaciones.end()){
            operaciones[instruccion](coleccion, f_in);
        }
        else {
            cerr << "Instrucción no reconocida: " << instruccion << endl;
        }
    }
    
    f_in.close();

    return 0;
}

void aniadir(colecInterdep  <string,Evento>& coleccion, ifstream& entrada){
    // Recoger datos de entrada
    string nombre, descrip, esDependiente, dependiente; 
    int prioridad;


    getline(entrada, nombre);
    getline(entrada, descrip);
    entrada >> prioridad; // Leer la prioridad que es un int
    entrada.ignore();  // Consumir el salto de línea una sola vez
    getline(entrada, esDependiente);
    getline(entrada, dependiente);
    
    Evento evento;
    crearEvento(descrip, prioridad, evento);

    // Funcionalidad
    int tamanioAntes, tamanioDespues;

    tamanioAntes = tamanio(coleccion);
    if (esDependiente == "DEPendiente"){
         aniadirDependiente(coleccion, nombre, evento, dependiente);
    }
    else if(esDependiente == "INDEPendiente"){
        aniadirIndependiente(coleccion, nombre, evento);
    }
    tamanioDespues = tamanio(coleccion);
    if (tamanioAntes < tamanioDespues) {
        cout << "INTRODUCIDO: ";
    }
    else {
        cout << "NO INTRODUCIDO: ";
    }

    if (esDependiente == "DEPendiente"){
        cout << "[ " << nombre << " -de-> " << dependiente << " ] --- " << descrip << " --- ( " << prioridad << " )" << endl;
    }
    else if(esDependiente == "INDEPendiente"){
        cout << "[ " << nombre << " ] --- " << descrip << " --- ( " << prioridad << " )" << endl;
    }

}

void cambiar(colecInterdep<string,Evento>& coleccion, ifstream& entrada){
    string nombre, descripcion;
    int prioridad;

    getline(entrada, nombre);
    getline(entrada, descripcion);
    entrada >> prioridad; // Leer la prioridad que es un int
    entrada.ignore();

    Evento evento;
    crearEvento(descripcion, prioridad, evento);

    if (actualizarVal(coleccion, nombre, evento)){
        cout << "CAMBIADO: ";
        Evento evento;
        string nombre_super;
        unsigned int numDepend;
        obtenerElemento(nombre, coleccion, evento, nombre_super, numDepend);
        if (nombre_super != ""){    
            cout << "[ " << nombre << " -de-> " << nombre_super << " ;;; " << numDepend + " ] --- " << descripcion << "--- (" << prioridad << ")" << endl;
        }
        else {
            cout << "[ " << nombre << " --- " << numDepend << " ] --- " << descripcion << "--- (" << prioridad << ")" << endl;
        }
    }
    else{
        cout << "NO CAMBIADO: " << nombre << endl;
    }
}

void obtener(colecInterdep<string,Evento>& coleccion, ifstream& entrada){
    string nombre, des, nombre_super;
    int prioridad;
    unsigned int numDepediente;

    getline(entrada, nombre);

    Evento evento;
    crearEvento(des, prioridad, evento);
    if (obtenerElemento(nombre, coleccion, evento, nombre_super, numDepediente)){
        des = descripcion(evento);
        prioridad = suPrioridad(evento);
        cout << "LOCALIZADO: ";
        if (nombre_super != ""){    
            cout << "[ " << nombre << " -de-> " << nombre_super << " ;;; " << numDepediente + " ] --- " << des << "--- (" << prioridad << ")" << endl;
        }
        else {
            cout << "[ " << nombre << " --- " << numDepediente << " ] --- " << des << "--- (" << prioridad << ")" << endl;
        } 
    }
    else {
        cout << "NO LOCALIZADO: " << nombre << endl;
    }

}

void existeEvento(colecInterdep<string,Evento>& coleccion, ifstream& entrada){
    string nombre;
    bool dependiente;

    getline(entrada, nombre);

    if (existeDependencia(nombre, coleccion, dependiente)){
        if (dependiente){
            cout << "DEPendiente: ";
        }
        else{
            cout << "INDEPendiente: ";
        }
    }
    else{
        cout << "DESCONOCIDO: ";
    }
    cout << nombre << endl;
}

void hacerEventoIndependiente(colecInterdep<string,Evento>& coleccion, ifstream& entrada){
    string nombre;
    bool dependiente;

    getline(entrada, nombre);

    if (existeDependencia(nombre, coleccion, dependiente)){
        if (dependiente){
            hacerIndependiente(coleccion, nombre);
            cout << "INDEPENDIZADO: ";
        }
        else{
            cout << "YA ERA INDepend.: ";
        }
    }
    else{
        cout << "NO INDEPENDIZADO: ";
    }

    cout << nombre << endl;

}

void hacerEventoDependiente(colecInterdep<string,Evento>& coleccion, ifstream& entrada){
    string nombre, supervisor;
    bool dependiente, dependiente_super;

    getline(entrada, nombre);
    getline(entrada, supervisor);

    if (existeDependencia(nombre, coleccion, dependiente) && existeDependencia(supervisor, coleccion, dependiente_super)){
        cout << "INTENTANDO hacer depend.: ";
        hacerDependiente(coleccion, nombre, supervisor);
    }
    else{
        cout << "IMPOSIBLE hacer depend.: ";
    }

    cout << nombre << " -de-> " << supervisor << endl;
}

void borrarEvento(colecInterdep<string,Evento>& coleccion, ifstream& entrada){
    string nombre;
    int tamanioAntes, tamanioDespues;

    getline(entrada, nombre);

    tamanioAntes = tamanio(coleccion);
    borrar(nombre, coleccion);
    tamanioDespues = tamanio(coleccion);

    if (tamanioAntes > tamanioDespues){
        cout << "BORRADO: ";
    }
    else{
        cout << "NO BORRADO: ";
    }
    cout << nombre << endl;
}

void listarDependientes(colecInterdep<string,Evento>& coleccion, ifstream& entrada){
    string nombre, descrip, supervisor, nombreHijo, descripHijo;
    int prioridad, prioridadHijo;
    unsigned int numDependientes, numDependientesHijo;
    Evento evento;

    getline(entrada, nombre);
    
    crearEvento(descrip, prioridad, evento);
    if (obtenerElemento(nombre, coleccion, evento, supervisor, numDependientes)){
        cout << "****DEPENDIENTES: ";
        descrip = descripcion(evento);
        prioridad = suPrioridad(evento);
        if (supervisor != ""){
            cout << "[ " << nombre << " -de-> " << supervisor << " ;;; " << numDependientes << " ] --- " << descrip << " --- ( " << prioridad << " ) ****" << endl;
        }
        else {
            cout << "[ " << nombre << " --- " << numDependientes << " ] --- " << descrip << " --- ( " << prioridad << " ) ****" << endl;
        }

        iniciarIterador(coleccion);
        int p = 1;
        while(p<=numDependientes && existeSiguiente(coleccion)){
            if(siguienteSuperior(coleccion, supervisor) && supervisor == nombre){
                siguienteIdent(coleccion, nombreHijo);
                siguienteVal(coleccion, evento);
                siguienteNumDependientes(coleccion, numDependientesHijo);
                descripHijo = descripcion(evento);
                prioridadHijo = suPrioridad(evento);

                cout << "[ " << p << " -> " << nombreHijo << " -de-> " << supervisor << " ;;; " << numDependientesHijo << " ] --- " << descripHijo << " --- ( " << prioridadHijo << " ) ;;;;" << endl;

                p++;
            }
            avanza(coleccion);
        }
        cout << "****FINAL dependientes -de-> " << nombre << endl;
    }
    else{
        cout << "****DESCONOCIDO" << endl;
    }
}
    
void listarTodos(colecInterdep<string,Evento>& coleccion, ifstream& entrada){
    string supervisor, nombre, descrip;
    int prioridad;
    unsigned int numDependientes;
    Evento evento;
    crearEvento(descrip, prioridad, evento);

    iniciarIterador(coleccion);
    cout << "-----LISTADO: " << endl;
    while(existeSiguiente(coleccion)){
        siguienteIdent(coleccion, nombre);
        siguienteVal(coleccion, evento);
        descrip = descripcion(evento);
        prioridad = suPrioridad(evento);
        siguienteNumDependientes(coleccion, numDependientes);
        
        if (siguienteDependiente(coleccion)){
            siguienteSuperior(coleccion, supervisor);
            cout << "[ " << nombre << " -de-> " << supervisor << " ;;; " << numDependientes << " ] --- " << descrip << " --- ( " << prioridad << " ) ****" << endl;
        }
        else {
            cout << "[ " << nombre << " --- " << numDependientes << " ] --- " << descrip << " --- ( " << prioridad << " ) ****" << endl;
        }
        avanza(coleccion);
    }
    cout << "-----" << endl;
}