/* Nombre, apellidos y NIP del (de los) autor(es).
*/

#ifndef AGRUPACION_HPP
#define AGRUPACION_HPP

// PREDECLARACION DEL TAD GENERICO agrupacion (inicio INTERFAZ)

const int MAX = 40;

/* COMPLETAR CON:
   LA ESPECIFICACION DEL DOMINIO DE VALORES DEL TAD agrupacion
   Y CON LAS CONDICIONES QUE SE LE EXIGEN AL PARAMETRO DE TIPO T.
   ...
*/
template<typename T> struct agrupacion;


/* Inicia agrupacion. Devuelve una agrupación vacía, sin elemento
*/
template<typename T> void iniciar(agrupacion<T>& a);

/* Devuelve una agrupación igual a la resultante de añadir un elemento e en la agrupación a.
*/
template<typename T> bool anyadir(agrupacion<T>& a, const T& e);

/* Devuelve verdad si y sólo si la agrupación a está vacía
*/
template<typename T> bool vacia(const agrupacion<T>& a);

/* Si a no está vacía, devuelve una agrupación igual a la resultante de eliminar de a el último elemento añadido a ella. Si a está vacía, devuelve la agrupación vací.
*/
template<typename T> void borrarUltimo(agrupacion<T>& a);

/* Dada una agrupación a y un elemento e, devuelve verdad si y sólo si en a hay algún elemento igual a e (en el sentido de la función iguales del parámetro formal elem),falso en caso contrario
*/
template<typename T> bool esta(const agrupacion<T>& a, const T& e);

//COMPLETAR OPERACIONES ITERADOR
//....
template<typename T> void iniciarIterador(agrupacion<T>& a);

template<typename T> bool existeSiguiente(agrupacion<T>& a);

template<typename T> void avanza(agrupacion<T>& a);

template<typename T> T siguiente(agrupacion<T>& a);


// FIN predeclaracion del TAD GENERICO agrupacion (Fin INTERFAZ)



// DECLARACION DEL TAD GENERICO agrupacion

template<typename T>
struct agrupacion{
    friend void iniciar<T>(agrupacion<T>& a);
    friend bool anyadir<T>(agrupacion<T>& a, const T& e);
    friend bool vacia<T>(const agrupacion<T>& a);
    friend void borrarUltimo<T>(agrupacion<T>& a);
    friend bool esta<T>(const agrupacion<T>& a, const T& e);
    //COMPLETAR OPERACIONES ITERADOR
    friend void iniciarIterador<T>(agrupacion<T>& a);
    friend bool existeSiguiente<T>(agrupacion<T>& a);
    friend void avanza<T>(agrupacion<T>& a);
    friend T siguiente<T>(agrupacion<T>& a);  
    
    private: //declaracion de la representacion interna del tipo
        // arrya de datos
        T datos[MAX];
        // Total de datos
        int total;
    //COMPLETAR CAMPOS ITERADOR
        // Posición actual del iterador
        int iter;  

    
};


// IMPLEMENTACION DE LAS OPERACIONES DEL TAD GENERICO agrupacion

 // pone el total a 0
template<typename T>
void iniciar(agrupacion<T>& a) {
    a.total = 0;
}

// Si no hay el numero maximo, agrega el elemento al final de los datos y aumente el total
template<typename T>
bool anyadir(agrupacion<T>& a, const T& e) {
    if (a.total >= MAX) {
        return false;  // No hay espacio
    }
    a.datos[a.total] = e;
    a.total++;
    return true;
}

// Si el total es 0 devuelve true, sino false
template<typename T>
bool vacia(const agrupacion<T>& a) {
    return a.total == 0;
}
	
// Si el total no es 0, resta uno al total
template<typename T>
void borrarUltimo(agrupacion<T>& a) {
    if (a.total > 0){
        a.total--;    
    }
}

// Recorre los datos de la agrupacion, si uno coincide devuelve true y sino false.
template<typename T>
bool esta(const agrupacion<T>& a, const T& e) {
    for (int i = 0; i < a.total; i++){
        if (iguales(a.datos[i], e)) { return true; }
    }
    return false;
}

//COMPLETAR IMPLEMENTACION OPERACIONES ITERADOR

template<typename T> 
void iniciarIterador(agrupacion<T>& a){
    a.iter = 0;
}

template<typename T> 
bool existeSiguiente(agrupacion<T>& a){
    return a.total > a.iter;
}

template<typename T> 
void avanza(agrupacion<T>& a){
    a.iter++;
}

template<typename T> 
T siguiente(agrupacion<T>& a){
    return a.datos[a.iter++];
}

#endif //fin de agrupacion.hpp

