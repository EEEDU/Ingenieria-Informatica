#include <iostream>
#include <string>
#include <fstream>
#include <map>
#include <functional>

#include "coleccion.hpp"
#include "evento.hpp"
using namespace std;

void aniadir(Coleccion<string,evento>& coleccion, ifstream& entrada);
void cambiar(Coleccion<string,evento>& coleccion, ifstream& entrada);
void obtener(Coleccion<string,evento>& coleccion, ifstream& entrada);
void existeEvento(Coleccion<string,evento>& coleccion, ifstream& entrada);
void hacerEventoIndependiente(Coleccion<string,evento>& coleccion, ifstream& entrada);
void hacerEventoDependiente(Coleccion<string,evento>& coleccion, ifstream& entrada);
void borrarEvento(Coleccion<string,evento>& coleccion, ifstream& entrada);
void listarDependientes(Coleccion<string,evento>& coleccion, ifstream& entrada);
void listarTodos(Coleccion<string,evento>& coleccion, ifstream& entrada); // no se necesita la entrada

int main() {
    
    // Crear coleccion vacia
    Coleccion<string, evento> coleccion;
    crear(coleccion);

    // Abriar archivo de entrada
    ifstream entrada("entrada.txt");

    // Error si no se puede abrir el archivo
    if (!entrada.is_open()){
        cerr << "ERROR: no se ha podido abrir el archivo entrada.txt" << endl;
        return 1;
    }

    string instruccion;
    map<string, function<void(Coleccion<string,evento>&, ifstream&)>> operaciones = {
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
    while (entrada >> instruccion)
    {
        if(operaciones.find(instruccion) != operaciones.end()){
            operaciones[instruccion](coleccion, entrada);
        }
        else {
            cerr << "Instrucción no reconocida: " << instruccion << endl;
        }
    }
    
    entrada.close();

    return 0;
}

void aniadir(Coleccion<string,evento>& coleccion, ifstream& entrada){
    // Recoger datos de entrada
    string nombre, descrip, esDependiente, dependiente; 
    int prioridad;

    entrada.ignore();  // Consumir el salto de línea una sola vez

    getline(entrada, nombre);
    getline(entrada, descrip);
    entrada >> prioridad; // Leer la prioridad que es un int
    entrada.ignore();
    getline(entrada, esDependiente);
    getline(entrada, dependiente);
    
    evento evento;
    crearEvento(descrip, prioridad, evento);

    // Funcionalidad
    int tamanioAntes, tamanioDespues;

    tamanioAntes = tamano(coleccion);
    if (esDependiente == "DEPendiente"){
        anyadirDependiente(coleccion, nombre, evento, dependiente);
    }
    else if(esDependiente == "INDEPendiente"){
        anyadirIndependiente(coleccion, nombre, evento);
    }
    tamanioDespues = tamano(coleccion);

    if (tamanioAntes < tamanioDespues) {
        cout << "INTRODUCIDO: ";
    }
    else {
        cout << "NO INTRODUCIDO: ";
    }

    if (esDependiente == "DEPendiente"){
        cout << "[ " << nombre << " -de-> " << dependiente << " ] --- " << descrip << " --- ( " << prioridad << endl;
    }
    else if(esDependiente == "INDEPendiente"){
        cout << "[ " << nombre << " ] --- " << descrip << " --- ( " << prioridad << endl;
    }

}

void cambiar(Coleccion<string,evento>& coleccion, ifstream& entrada){
    string nombre, descripcion;
    int prioridad;

    entrada.ignore();  // Consumir el salto de línea una sola vez

    getline(entrada, nombre);
    getline(entrada, descripcion);
    entrada >> prioridad; // Leer la prioridad que es un int
    entrada.ignore();

    evento evento;
    crearEvento(descripcion, prioridad, evento);

    actualizarVal(coleccion, nombre, evento);
}

void obtener(Coleccion<string,evento>& coleccion, ifstream& entrada){
    string nombre, des, supervisor;
    int prioridad, numDepediente;

    entrada.ignore();
    getline(entrada, nombre);

    evento evento;
    crearEvento(des, prioridad, evento);
    obtenerVal(nombre, coleccion, evento);
    obtenerSupervisor(nombre, coleccion,supervisor);
    obtenerNumDependientes(nombre, coleccion, numDepediente);
    des = descripcion(evento);
    prioridad = suPrioridad(evento);

}

void existeEvento(Coleccion<string,evento>& coleccion, ifstream& entrada){
    string nombre;

    entrada.ignore();
    getline(entrada, nombre);

    existe(nombre, coleccion);
}

void hacerEventoIndependiente(Coleccion<string,evento>& coleccion, ifstream& entrada){
    string nombre;

    entrada.ignore();
    getline(entrada, nombre);

    hacerIndependiente(coleccion, nombre);
}

void hacerEventoDependiente(Coleccion<string,evento>& coleccion, ifstream& entrada){
    string nombre, supervisor;

    entrada.ignore();
    getline(entrada, nombre);
    getline(entrada, supervisor);

    hacerDependiente(coleccion, nombre, supervisor);
}

void borrarEvento(Coleccion<string,evento>& coleccion, ifstream& entrada){
    string nombre;

    entrada.ignore();
    getline(entrada, nombre);

    borrar(nombre, coleccion);
}

void listarDependientes(Coleccion<string,evento>& coleccion, ifstream& entrada){
    string nombre;
    int numDependientes;

    entrada.ignore();
    getline(entrada, nombre);

    obtenerNumDependientes(nombre, coleccion, numDependientes);
    
    string supervisor, nombreHijo, descripcionHijo;
    int prioridadHijo;
    evento evento;
    iniciarIterador(coleccion);
    while(numDependientes>0 && existeSiguiente(coleccion)){
        if(siguienteSuperior(coleccion, supervisor) && supervisor == nombre){
            // TODO: listar esto
            siguienteIdent(coleccion, nombreHijo);
            siguienteVal(coleccion, evento);
            descripcionHijo = descripcion(evento);
            prioridadHijo = suPrioridad(evento);
            numDependientes--;
        }
        avanza(coleccion);
    }
    
}
void listarTodos(Coleccion<string,evento>& coleccion, ifstream& entrada){
    entrada.ignore();

    string supervisor, nombreHijo, descripcionHijo;
    int prioridadHijo;
    evento evento;
    iniciarIterador(coleccion);
    while(existeSiguiente(coleccion)){
        // TODO: listar esto
        siguienteIdent(coleccion, nombreHijo);
        siguienteVal(coleccion, evento);
        descripcionHijo = descripcion(evento);
        prioridadHijo = suPrioridad(evento);
        avanza(coleccion);
    }
}