/* 
   Daniel Ubalde, NIP:
   Eduardo Guerrero, NIP:
*/


#include <iostream>
using namespace std;

#ifndef COLECCION_HPP
#define COLECCION_HPP

// PREDECLARACION DEL TAD GENERICO agrupacion (inicio INTERFAZ)

template<typename I, typename V> 
struct Coleccion;

// 
template<typename I, typename V>
void crear(Coleccion<I,V>& c);

// 
template<typename I, typename V>
int tamano(const Coleccion<I,V>& c);

// 
template<typename I, typename V>
bool esVacia(const Coleccion<I,V>& c);

// 
template<typename I, typename V>
bool existe(I id, const Coleccion<I,V>& c);

// 
template<typename I, typename V>
bool existeDependiente(I id, const Coleccion<I,V>& c);

// 
template<typename I, typename V>
bool existeIndependiente(I id, const Coleccion<I,V>& c);

// 
template<typename I, typename V>
void anyadirIndependiente(Coleccion<I,V>& c, I id, V v);

// 
template<typename I, typename V>
void anyadirDependiente(Coleccion<I,V>& ccol, I id, V v, I super);

// 
template<typename I, typename V>
void hacerDependiente(Coleccion<I,V>& c, I id, I super);

// 
template<typename I, typename V>
void hacerIndependiente(Coleccion<I,V>& c, I id );

// 
template<typename I, typename V>
bool actualizarVal(Coleccion<I,V>& c, I id, V nuevo);

// 
template<typename I, typename V>
bool obtenerVal(I id, const Coleccion<I,V>& c, V& v_out);

// 
template<typename I, typename V>
bool obtenerSupervisor(I id, const Coleccion<I,V>& c, I& i_out);

// 
template<typename I, typename V>
bool obtenerNumDependientes(I id, const Coleccion<I,V>& c, int& numDep_out);

// 
template<typename I, typename V>
void borrar(I id, Coleccion<I,V>& c);

// 
template<typename I, typename V>
void iniciarIterador(Coleccion<I,V>& c);

// 
template<typename I, typename V>
bool existeSiguiente(const Coleccion<I,V>& c);

// 
template<typename I, typename V>
bool siguienteIdent(const Coleccion<I,V>& c, I& i_out);

// 
template<typename I, typename V>
bool siguienteVal(const Coleccion<I,V>& c, V& v_out);

// 
template<typename I, typename V>
bool siguienteDependiente(const Coleccion<I,V>& c);

// 
template<typename I, typename V>
bool siguienteSuperior(const Coleccion<I,V>& c, I& super_out);

// 
template<typename I, typename V>
bool siguienteNumDependientes(const Coleccion<I,V>& c, int& numDep_out);

// 
template<typename I, typename V>
bool avanza(Coleccion<I,V>& c);


// DECLARACION DEL TAD GENERICO Coleccion

template<typename I, typename V>
struct Coleccion{ 
    friend void crear<I,V>(Coleccion<I,V>& c);
    friend int tamano<I,V>(const Coleccion<I,V>& c);
    friend bool esVacia<I,V>(const Coleccion<I,V>& c);
    friend bool existe<I,V>(I, const Coleccion<I,V>& c);
    friend bool existeDependiente<I,V>(I id, const Coleccion<I,V>& c);
    friend bool existeIndependiente<I,V>(I id, const Coleccion<I,V>& c);
    friend void anyadirIndependiente<I,V>(Coleccion<I,V>& c, I id, V v);
    friend void anyadirDependiente<I,V>(Coleccion<I,V>& c, I id, V v, I super);
    friend void hacerDependiente<I,V>(Coleccion<I,V>& c, I id, I super);
    friend void hacerIndependiente<I,V>(Coleccion<I,V>& c, I id);
    friend bool actualizarVal<I,V>(Coleccion<I,V>& c, I id, V v);
    friend bool obtenerVal<I,V>(I id, const Coleccion<I,V>& c, V& v_out);
    friend bool obtenerSupervisor<I,V>(I id, const Coleccion<I,V>& c, I& super_out);
    friend bool obtenerNumDependientes<I,V>(I id, const Coleccion<I,V>& c, int& numDep_out);
    friend void borrar<I,V>(I id, Coleccion<I,V>& c);
    friend void iniciarIterador<I,V>(Coleccion<I,V>& c);
    friend bool existeSiguiente<I,V>(const Coleccion<I,V>& c);
    friend bool siguienteIdent<I,V>(const Coleccion<I,V>& c, I& id_out);
    friend bool siguienteVal<I,V>(const Coleccion<I,V>& c, V& v_out);
    friend bool siguienteDependiente<I,V>(const Coleccion<I,V>& c);
    friend bool siguienteSuperior<I,V>(const Coleccion<I,V>& c, I& super_out);
    friend bool siguienteNumDependientes<I,V>(const Coleccion<I,V>& c, int& numDep_out);
    friend bool avanza<I,V>(Coleccion<I,V>& c);

    private:  //declaracion de la representacion interna del tipo
    
    struct Elemento{
        I ident;
        V val;
        Elemento* identSup;
        int numDepend;
        Elemento* sig;
    };

    Elemento* primera;
    int tamano;
    Elemento* iter;
    
};

#endif


template<typename I, typename V>  
void crear(Coleccion<I,V>& c){
    c.primera = nullptr;
    c.tamano = 0;
}

template<typename I, typename V>  
int tamano(const Coleccion<I,V>& c){
    return c.tamano;
}

template<typename I, typename V>  
bool esVacia(const Coleccion<I,V>& c){
    return c.tamano == 0;
}

template<typename I, typename V>  
bool existe(I id, const Coleccion<I,V>& c){
    if (esVacia(c)) {return false;}
    typename Coleccion<I,V>::Elemento* aux = c.primera;
    // Recorrer mientras haya nodos Y el identificador sea menor
    while (aux != nullptr && aux->ident < id) {
        aux = aux->sig;
    }
    // Si encontramos un nodo y su ident es igual a id
    return aux != nullptr && aux->ident == id;
}

template<typename I, typename V>  
bool existeDependiente(I id, const Coleccion<I,V>& c){
    typename Coleccion<I,V>::Elemento* aux = c.primera;
    // Recorrer mientras haya nodos Y el identificador sea menor
    while (aux != nullptr && aux->ident < id) {
        aux = aux->sig;
    }
    // Si encontramos un nodo y su ident es igual a id
    return aux != nullptr && aux->ident == id && aux->identSup != nullptr;
}

template<typename I, typename V>  
bool existeIndependiente(I id, const Coleccion<I,V>& c){
    typename Coleccion<I,V>::Elemento* aux = c.primera;
    // Recorrer mientras haya nodos Y el identificador sea menor
    while (aux != nullptr && aux->ident < id) {
        aux = aux->sig;
    }
    // Si encontramos un nodo y su ident es igual a id
    return aux != nullptr && aux->ident == id && aux->numDepend != nullptr;
}

template<typename I, typename V>  
void anyadirIndependiente(Coleccion<I,V>& c, I id, V v){
    // Si la coleccion esta vacia, se crea el nuevo elemento en la primera
    
    if(esVacia(c)) {
        typename Coleccion<I, V>::Elemento *nuevo = new typename Coleccion<I, V>::Elemento;
        nuevo->ident = id;
        nuevo->val = v;
        nuevo->identSup = nullptr;
        nuevo->numDepend = 0;
        nuevo->sig = nullptr;
        c.primera = nuevo;
        c.tamano = 1;
        return;
    }
    
    typename Coleccion<I,V>::Elemento* aux = c.primera;
    while (aux->sig != nullptr && aux->ident < id) {
        aux = aux->sig;
    }

    // Si ya existe, no hacer nada
    if (aux->ident == id) {
        return;
    }
    
    // Si no existe, se crea uno nuevo despues del aux
    typename Coleccion<I, V>::Elemento *nuevo = new typename Coleccion<I, V>::Elemento;
    nuevo->ident = id;
    nuevo->val = v;
    nuevo->identSup = nullptr;
    nuevo->numDepend = 0;
    nuevo->sig = aux->sig;
    aux->sig = nuevo;
    c.tamano += 1;
}

template<typename I, typename V>  
void anyadirDependiente(Coleccion<I,V>& c, I id, V v, I super){

    
    if (esVacia(c) || id == super || c.primera->sig == nullptr && (c.primera->ident == id || c.primera->ident != super)) {
        return;
    }

    // Buscar el elemento supervisor para incrementar su contador
    typename Coleccion<I,V>::Elemento* aux = c.primera;
    typename Coleccion<I,V>::Elemento* node = nullptr;
    typename Coleccion<I,V>::Elemento* superNode = nullptr;

    // Buscar nodo y supervisor
    while (aux != nullptr && (aux->ident < id || superNode == nullptr)) {
        if (aux->ident == super) {
            superNode = aux;
        }
        if (aux->ident < id) {
            node = aux;
        }
        aux = aux->sig;
    }
    // Si nodo no super existe, sale de la funcion
    // Si el nodo node existe, sale de la funcion
    if (superNode == nullptr || (node != nullptr && node->sig->ident == id)) {
        return;
    }

    if (node == nullptr){
        // Insertar al final
        typename Coleccion<I, V>::Elemento *nuevoNodo = new typename Coleccion<I, V>::Elemento;
        nuevoNodo->ident = id;
        nuevoNodo->val = v;
        nuevoNodo->identSup = superNode;
        nuevoNodo->numDepend = 0;
        nuevoNodo->sig = c.primera;
        c.primera=nuevoNodo;
        superNode->numDepend++;
        c.tamano++;
    }
    else{
        // Insertar en mitada de la coleccion
        typename Coleccion<I, V>::Elemento *nuevoNodo = new typename Coleccion<I, V>::Elemento;
        nuevoNodo->ident = id;
        nuevoNodo->val = v;
        nuevoNodo->identSup = superNode;
        nuevoNodo->numDepend = 0;
        nuevoNodo->sig = node->sig;
        node->sig = nuevoNodo;
        superNode->numDepend++;
        c.tamano++;

    }
}

template<typename I, typename V>
void hacerDependiente(Coleccion<I,V>& c, I id , I super){
    if (esVacia(c) || id == super){ 
        return; 
    }

    // Buscar el elemento supervisor para incrementar su contador
    typename Coleccion<I,V>::Elemento* aux = c.primera;
    typename Coleccion<I,V>::Elemento* node = nullptr;
    typename Coleccion<I,V>::Elemento* superNode = nullptr;

    // Buscar nodo y supervisor
    while (aux != nullptr && (node == nullptr || superNode == nullptr)) {
        if (aux->ident == super) {
            superNode = aux;
        }
        else if (aux->ident == id) {
            node = aux;
        }
        aux = aux->sig;
    }

    // Si el nodo super no existe, sale de la funcion
    // Si el nodo node no existe, sale de la funcion
    if (superNode == nullptr || node == nullptr) {
        return;
    }

    if (node->identSup != nullptr){
        node->identSup->numDepend--;
    } 
    node->identSup = superNode;
    superNode->numDepend++;
}

template<typename I, typename V>
void hacerIndependiente(Coleccion<I,V>& c, I id){

    if (esVacia(c)){ 
        return; 
    }

    // Buscar el elementos
    typename Coleccion<I,V>::Elemento* aux = c.primera;
    typename Coleccion<I,V>::Elemento* node = nullptr;
    // Buscar nodo
    while (aux != nullptr && node == nullptr ){
        if (aux->ident == id) {
            node = aux;
        }
        aux = aux->sig;
    }

    // Si el nodo node no existe, sale de la funcion
    if (node == nullptr) {
        return;
    }

    if (node->identSup != nullptr){
        node->identSup->numDepend--;
    } 
    node->identSup = nullptr;
}

template<typename I, typename V>  
bool actualizarVal(Coleccion<I,V>& c, I id, V nuevo){
    typename Coleccion<I,V>::Elemento* aux = c.primera;

    while (aux != nullptr && aux->ident <= id) {
        if (aux->ident == id) {
            aux->val = nuevo;
            return true;
        }
        aux = aux->sig;
    }
    return false;
}

// TODO: juntar las funciones de obtener
template<typename I, typename V>  
bool obtenerVal(I id, const Coleccion<I,V>& c, V& v_out){
    
    typename Coleccion<I,V>::Elemento* aux = c.primera;
    typename Coleccion<I,V>::Elemento* node = nullptr;

    while (aux != nullptr && aux->ident <= id) {
        if (aux->ident == id) {
            v_out = aux->val;
            return true;
        }
        aux = aux->sig;
    }
    return false;

}

template<typename I, typename V>  
bool obtenerSupervisor(I id, const Coleccion<I,V>& c, I& super_out){
    if (!existeDependiente(id, c)){
        return false;
    }    
    // No se comprueba, se asume que el cliente lo garantiza
    
    typename Coleccion<I,V>::Elemento* aux = c.primera;
    while (aux != nullptr && aux->ident < id) {
        aux = aux->sig;
    }
    if (aux != nullptr && aux->ident == id) {
        super_out = aux->identSup->ident;
    }
    return true;
}

template<typename I, typename V>  
bool obtenerNumDependientes(I id, const Coleccion<I,V>& c, int& numDep_out){
    if (!existeDependiente(id, c)) {
        return false;
    }
    // No se comprueba, se asume que el cliente lo garantiza
    typename Coleccion<I,V>::Elemento* aux = c.primera;
    while (aux != nullptr && aux->ident < id) {
        aux = aux->sig;
    }
    if (aux != nullptr && aux->ident == id) {
        numDep_out = aux->numDepend;
    }
    return true;
}

template<typename I, typename V>  
void borrar(I id, Coleccion<I,V>& c){

    
    typename Coleccion<I,V>::Elemento* aux = c.primera;
    typename Coleccion<I,V>::Elemento* anterior;
    I super;
    while (aux != nullptr && aux->ident < id) {
        anterior = aux;
        aux = aux->sig;
    }
    if (aux != nullptr && aux->ident == id) {
        anterior->sig = aux->sig;    
    }
    if (aux->identSup != nullptr){
        super = aux->identSup;
        aux = c.primera;
        while (aux != nullptr && aux->ident < super) {
            aux = aux->sig;
        }
        if (aux != nullptr && aux->ident == super) {
            aux->numDepend--;    
        }
    }
}

template<typename I, typename V>  
void iniciarIterador(Coleccion<I,V>& c){
    c.iter = c.primera;
}

template<typename I, typename V>  
bool existeSiguiente(Coleccion<I,V>& c){
    return c.iter = nullptr;
}

// TODO: juntar las funciones de siguiente
template<typename I, typename V>  
bool siguienteIdent(Coleccion<I,V>& c, I& id_out){
    if (!existeSiguiente(c)){
        return false;
    }
    id_out = c.iter->ident;
    return true;
}

template<typename I, typename V>  
bool siguienteVal(Coleccion<I,V>& c, V& v_out){
    if (!existeSiguiente(c)){
        return false;
    }
    v_out = c.iter->val;
    return true;
}

template<typename I, typename V>  
bool siguienteDependiente(Coleccion<I,V>& c){
    if (!existeSiguiente(c)){
        return false;
    }
    return c.iter->identSup != nullptr;
}

template<typename I, typename V>  
bool siguienteSuperior(Coleccion<I,V>& c, I& super_out){
    if (!siguienteDependiente(c)){
        return false;
    }
    super_out = c.iter->identSup;
    return true;
}

template<typename I, typename V>  
bool siguienteNumDependientes(Coleccion<I,V>& c, int& numDep_out){
    if (!existeSiguiente(c)){
        return false;
    }
    numDep_out = c.iter->numDepend;
    return true;
}

template<typename I, typename V>  
bool avanza(Coleccion<I,V>& c){
    if (!existeSiguiente(c)){
        return false;
    }
    c.iter = c.iter->sig;
    return true;
}
