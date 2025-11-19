/******************************************************************************
 * Fichero: colecInterdep.hpp
 * Autores: Daniel Ubalde (875396)
 *          Eduardo Guerrero (816106)
 *
 * Asignatura: Estructuras de Datos y Algoritmos
 * Descripcion: Implementacion del TAD generico colecInterdep (colecciones de
 *              elementos interdependientes) mediante lista enlazada simple
 *              ordenada en memoria dinamica.
 ********************************************************************************/

#ifndef COLEC_INTERDEP_HPP
#define COLEC_INTERDEP_HPP

/* INICIO DE LA PARTE PÚBLICA o INTERFAZ  */

/* El tipo I requerira tener una funcion:
 * bool operator== (const I& i1, const I& i2); {devuelva verdad si y solo si los elementos i1 y i2 se consideran iguales}
 * bool operator< (const I& i1, const I& i2); {devuelva verdad si y solo si el elemento i1 es menor estricto que i2}
 */
template <typename I, typename V>
struct colecInterdep;

/*
 * Operacion: crear
 * ----------------
 * Descripcion: Crea una coleccion vacia, sin elementos.
 *
 * Precondicion: Ninguna.
 *
 * Postcondicion: Devuelve una coleccion vacia c tal que:
 *                - esVacia(c) = verdad
 *                - tamano(c) = 0
 *
 * Parametros:
 *   [in,out] a: coleccion que sera inicializada como vacia
 *
 * Complejidad: O(1)
 */
template <typename I, typename V>
void crear(colecInterdep<I, V> &a);

/*
 * Operacion: tamanio
 * -----------------
 * Descripcion: Devuelve el numero de elementos que hay en la coleccion.
 *
 * Precondicion: Ninguna.
 *
 * Postcondicion: Devuelve el numero total de elementos almacenados en c.
 *
 * Parametros:
 *   [in] a: coleccion a consultar
 *
 * Devuelve: numero de elementos en la coleccion
 *
 * Complejidad: O(1)
 */
template <typename I, typename V>
unsigned int tamanio(const colecInterdep<I, V> &a);

/*
 * Operacion: esVacia?
 * -------------------
 * Descripcion: Comprueba si la coleccion esta vacia.
 *
 * Precondicion: Ninguna.
 *
 * Postcondicion: Devuelve verdad si y solo si c no contiene ningun elemento.
 *
 * Parametros:
 *   [in] a: coleccion a consultar
 *
 * Devuelve: verdad si la coleccion esta vacia, falso en caso contrario
 *
 * Complejidad: O(1)
 */
template <typename I, typename V>
bool esVacia(const colecInterdep<I, V> &a);

/*
 * Operacion: existe?
 * ------------------
 * Descripcion: Comprueba si existe algun elemento con el identificador dado.
 *
 * Precondicion: Ninguna.
 *
 * Postcondicion: Devuelve verdad si y solo si en c hay algun elemento con
 *                ident igual a id.
 *
 * Parametros:
 *   [in] id: identificador a buscar
 *   [in] a: coleccion donde buscar
 *
 * Devuelve: verdad si existe un elemento con identificador id, falso si no
 *
 * Complejidad: O(n) en el peor caso, donde n es el numero de elementos
 */
template <typename I, typename V>
bool existe(const I &id, const colecInterdep<I, V> &a);

/*
 * Operacion: existeDependencia?
 * -----------------------------
 * Descripcion: Comprueba si existe un elemento con el identificador dado y
 *              determina si es dependiente o independiente.
 *
 * Precondicion: Ninguna.
 *
 * Postcondicion: Si en a existe un elemento con ident igual a id, devuelve verdad
 *                en caso contrario de no encontrar el elemento en la lista 'a' devuelve falso
 *                Si el elemento es independiente y e_type = falso
 *                Si el elemento es dependiente y e_type = verdad
 *
 * Parametros:
 *   [in] id: identificador a buscar
 *   [in] a: coleccion donde buscar
 *   [in,out] e_type: tipo del elemento (en caso de existir), verdad si es dependiente,
 *                    falso si es independiente. (Si no existe el elemento, su valor no es relevante)
 *
 * Devuelve: verdad si existe el elemento, falso en caso contrario
 *
 * Complejidad: O(n) en el peor caso
 */
template <typename I, typename V>
bool existeDependencia(const I &id, const colecInterdep<I, V> &a, bool &e_type);

/*
 * Operacion: aniadirIndependiente
 * -------------------------------
 * Descripcion: Aniade un nuevo elemento independiente a la coleccion.
 *
 * Precondicion: no existe?(id, c) - El identificador no debe existir en c.
 *
 * Postcondicion: Si se cumple la precondicion, devuelve una coleccion igual
 *                a la resultante de anadir el elemento independiente
 *                (id, v, -, 0) a la coleccion c, manteniendo el orden.
 *                En caso contrario, devuelve una coleccion igual a c.
 *
 * Parametros:
 *   [in,out] a: coleccion donde anadir el elemento
 *   [in] id: identificador del nuevo elemento
 *   [in] v: valor del nuevo elemento
 *
 * Complejidad: O(n) en el peor caso, donde n es el numero de elementos
 */
template <typename I, typename V>
void aniadirIndependiente(colecInterdep<I, V> &a, const I &id, const V &v);

/*
 * Operacion: aniadirDependiente
 * -----------------------------
 * Descripcion: Anade un nuevo elemento dependiente de otro a la coleccion.
 *
 * Precondicion: no existe?(id, c) y existe?(super, c)
 *               El identificador id no debe existir y super si debe existir.
 *
 * Postcondicion: Si se cumplen las precondiciones, devuelve una coleccion
 *                igual a la resultante de:
 *                - Incrementar en 1 el numero de elementos dependientes del
 *                  elemento con identificador super en c
 *                - Anadir el elemento (id, v, super, 0) a la coleccion c
 *                En cualquier otro caso, devuelve una coleccion igual a c.
 *
 * Parametros:
 *   [in,out] a: coleccion donde anadir el elemento
 *   [in] id: identificador del nuevo elemento
 *   [in] v: valor del nuevo elemento
 *   [in] super: identificador del elemento del cual dependera
 *
 * Complejidad: O(n) en el peor caso
 */
template <typename I, typename V>
void aniadirDependiente(colecInterdep<I, V> &a, const I &id, const V &v, const I &super);

/*
 * Operacion: hacerDependiente
 * ---------------------------
 * Descripcion: Convierte un elemento existente en dependiente de otro.
 *
 * Precondicion: no igual(id, super) y existe?(super, c) y existe?(id, c)
 *
 * Postcondicion: Si se cumplen las precondiciones:
 *                - Si el elemento id era dependiente de superAnt, decrementa
 *                  en 1 el numDepend de superAnt
 *                - Incrementa en 1 el numDepend del elemento super
 *                - El elemento id pasa a ser dependiente de super
 *                En cualquier otro caso, devuelve una coleccion igual a c.
 *
 * Parametros:
 *   [in,out] a: coleccion a modificar
 *   [in] id: identificador del elemento a hacer dependiente
 *   [in] super: identificador del nuevo elemento superior
 *
 * Complejidad: O(n) en el peor caso
 */
template <typename I, typename V>
void hacerDependiente(colecInterdep<I, V> &a, const I &id, const I &super);

/*
 * Operacion: hacerIndependiente
 * -----------------------------
 * Descripcion: Convierte un elemento dependiente en independiente.
 *
 * Precondicion: Ninguna.
 *
 * Postcondicion: Si existeDependiente?(id, c), sea su forma
 *                (id, v, superAnt, NumDep), devuelve una coleccion igual a
 *                la resultante de:
 *                - Decrementar en 1 el numDepend del elemento superAnt
 *                - Sustituir (id, v, superAnt, NumDep) por (id, v, -, NumDep)
 *                En cualquier otro caso, devuelve una coleccion igual a c.
 *
 * Parametros:
 *   [in,out] a: coleccion a modificar
 *   [in] id: identificador del elemento a hacer independiente
 *
 * Complejidad: O(n) en el peor caso
 */
template <typename I, typename V>
void hacerIndependiente(colecInterdep<I, V> &a, const I &id);

/*
 * Operacion: actualizarVal (parcial)
 * ----------------------------------
 * Descripcion: Actualiza el valor de un elemento existente en la coleccion.
 *
 * Precondicion: existe?(id, c) - El elemento debe existir en la coleccion.
 *               OPERACION PARCIAL: no esta definida si no existe?(id, c).
 *
 * Postcondicion: Devuelve una coleccion igual a la resultante de sustituir
 *                el valor v por nuevo en el elemento con identificador id,
 *                manteniendo el resto de informacion del elemento.
 *                Devuelve verdad si la operacion se realizo correctamente.
 *
 * Parametros:
 *   [in,out] a: coleccion a modificar
 *   [in] id: identificador del elemento a actualizar
 *   [in] nuevo: nuevo valor para el elemento
 *
 * Devuelve: verdad si se actualizo el elemento, falso si no existe
 *
 * Complejidad: O(n) en el peor caso
 */
template <typename I, typename V>
bool actualizarVal(colecInterdep<I, V> &a, const I &id, const V &nuevo);

/*
 * Operacion: obtenerElemento (parcial)
 * ------------------------------------
 * Descripcion: Obtiene toda la informacion de un elemento de la coleccion.
 *
 * Precondicion: existe?(id, c) - El elemento debe existir.
 *               OPERACION PARCIAL: no esta definida si no existe?(id, c).
 *
 * Postcondicion: Si el elemento existe:
 *                - v_out contiene el valor del elemento
 *                - Si es independiente: s_id_out contiene una cadena vacia
 *                - Si es dependiente: s_id_out contiene el ident del superior
 *                - n_depen contiene el numero de dependientes directos
 *                - Devuelve verdad
 *                Si no existe, devuelve falso.
 *
 * Parametros:
 *   [in] id: identificador del elemento a consultar
 *   [in] a: coleccion donde buscar
 *   [in,out] v_out: valor del elemento
 *   [in,out] s_id_out: identificador del superior (vacio si es independiente)
 *   [in,out] n_depen: numero de elementos que dependen directamente de este
 *
 * Devuelve: verdad si el elemento existe, falso en caso contrario
 *
 * Complejidad: O(n) en el peor caso
 */
template <typename I, typename V>
bool obtenerElemento(const I &id, const colecInterdep<I, V> &a, V &v_out, I &s_id_out, unsigned int &n_depen);

/*
 * Operacion: borrar
 * -----------------
 * Descripcion: Elimina un elemento de la coleccion si no tiene dependientes.
 *
 * Precondicion: Ninguna.
 *
 * Postcondicion: Si existe?(id, c) y obtenerNumDependientes(id, c) = 0:
 *                - Si el elemento era dependiente de super, decrementa en 1
 *                  el numDepend de super
 *                - Elimina el elemento con identificador id de la coleccion
 *                En cualquier otro caso, devuelve una coleccion igual a c.
 *
 * Parametros:
 *   [in] id: identificador del elemento a borrar
 *   [in,out] a: coleccion de donde borrar
 *
 * Complejidad: O(n) en el peor caso
 */
template <typename I, typename V>
void borrar(const I &id, colecInterdep<I, V> &a);

/*
 * OPERACIONES DEL ITERADOR
 */

/*
 * Operacion: iniciarIterador
 * --------------------------
 * Descripcion: Inicializa el iterador para recorrer la coleccion desde el
 *              primer elemento.
 *
 * Precondicion: Ninguna.
 *
 * Postcondicion: El iterador queda situado de forma que el siguiente elemento
 *                a visitar sea el que tiene un ident anterior a los de todos
 *                los demas elementos de la coleccion (situacion de no haber
 *                visitado ningun elemento).
 *
 * Parametros:
 *   [in,out] a: coleccion cuyo iterador se inicializa
 *
 * Complejidad: O(1)
 */
template <typename I, typename V>
void iniciarIterador(colecInterdep<I, V> &a);

/*
 * Operacion: existeSiguiente?
 * ---------------------------
 * Descripcion: Comprueba si quedan elementos por visitar con el iterador.
 *
 * Precondicion: Ninguna.
 *
 * Postcondicion: Devuelve verdad si queda algun elemento por visitar con el
 *                iterador de la coleccion c, devuelve falso si ya se ha
 *                visitado el ultimo elemento.
 *
 * Parametros:
 *   [in] a: coleccion a consultar
 *
 * Devuelve: verdad si quedan elementos por visitar, falso en caso contrario
 *
 * Complejidad: O(1)
 */
template <typename I, typename V>
bool existeSiguiente(const colecInterdep<I, V> &a);

/*
 * Operacion: siguienteIdent (parcial)
 * -----------------------------------
 * Descripcion: Devuelve el identificador del siguiente elemento a visitar.
 *
 * Precondicion: existeSiguiente?(c) - Debe haber elementos por visitar.
 *               OPERACION PARCIAL: no esta definida si no existeSiguiente?(c).
 *
 * Postcondicion: Devuelve el ident del siguiente elemento a visitar con el
 *                iterador de la coleccion c, que sera el elemento no visitado
 *                con ident anterior a los de todos los demas aun no visitados.
 *
 * Parametros:
 *   [in] a: coleccion a consultar
 *   [in,out] id: identificador del elemento actual del iterador
 *
 * Devuelve: verdad si hay elemento actual, falso si no quedan elementos
 *
 * Complejidad: O(1)
 */
template <typename I, typename V>
bool siguienteIdent(const colecInterdep<I, V> &a, I &id);

/*
 * Operacion: siguienteVal (parcial)
 * ---------------------------------
 * Descripcion: Devuelve el valor del siguiente elemento a visitar.
 *
 * Precondicion: existeSiguiente?(c)
 *               OPERACION PARCIAL: no esta definida si no existeSiguiente?(c).
 *
 * Postcondicion: Devuelve el val del siguiente elemento a visitar con el
 *                iterador de la coleccion c.
 *
 * Parametros:
 *   [in] a: coleccion a consultar
 *   [in,out] val: valor del elemento actual del iterador
 *
 * Devuelve: verdad si hay elemento actual, falso en caso contrario
 *
 * Complejidad: O(1)
 */
template <typename I, typename V>
bool siguienteVal(const colecInterdep<I, V> &a, V &val);

/*
 * Operacion: siguienteDependiente? (parcial)
 * ------------------------------------------
 * Descripcion: Indica si el siguiente elemento a visitar es dependiente.
 *
 * Precondicion: existeSiguiente?(c)
 *               OPERACION PARCIAL: no esta definida si no existeSiguiente?(c).
 *
 * Postcondicion: Si el siguiente elemento a visitar es de la forma
 *                (ident, val, -, numDep) devuelve falso, pero si es de la
 *                forma (ident, val, identSup, numDep) devuelve verdad.
 *
 * Parametros:
 *   [in] a: coleccion a consultar
 *
 * Devuelve: verdad si el elemento actual es dependiente, falso si es
 *           independiente o no hay elemento actual
 *
 * Complejidad: O(1)
 */
template <typename I, typename V>
bool siguienteDependiente(const colecInterdep<I, V> &a);

/*
 * Operacion: siguienteSuperior (parcial)
 * --------------------------------------
 * Descripcion: Devuelve el identificador del superior del siguiente elemento
 *              a visitar.
 *
 * Precondicion: existeSiguiente?(c) y siguienteDependiente?(c)
 *               OPERACION PARCIAL: no esta definida si no se cumplen ambas.
 *
 * Postcondicion: Si el siguiente elemento a visitar es de la forma
 *                (ident, val, identSup, numDep), devuelve su identSup.
 *
 * Parametros:
 *   [in] a: coleccion a consultar
 *   [in,out] id: identificador del elemento superior
 *
 * Devuelve: verdad si el elemento actual es dependiente y se obtuvo el
 *           identificador del superior, falso en caso contrario
 *
 * Complejidad: O(1)
 */
template <typename I, typename V>
bool siguienteSuperior(const colecInterdep<I, V> &a, I &id);

/*
 * Operacion: siguienteNumDependientes (parcial)
 * ---------------------------------------------
 * Descripcion: Devuelve el numero de dependientes del siguiente elemento.
 *
 * Precondicion: existeSiguiente?(c)
 *               OPERACION PARCIAL: no esta definida si no existeSiguiente?(c).
 *
 * Postcondicion: Devuelve el NumDep del siguiente elemento a visitar con el
 *                iterador de la coleccion c.
 *
 * Parametros:
 *   [in] a: coleccion a consultar
 *   [in,out] n_depen: numero de dependientes del elemento actual
 *
 * Devuelve: verdad si hay elemento actual, falso en caso contrario
 *
 * Complejidad: O(1)
 */
template <typename I, typename V>
bool siguienteNumDependientes(const colecInterdep<I, V> &a, unsigned int &n_depen);

/*
 * Operacion: avanza (parcial)
 * ---------------------------
 * Descripcion: Avanza el iterador al siguiente elemento de la coleccion.
 *
 * Precondicion: existeSiguiente?(c)
 *               OPERACION PARCIAL: no esta definida si no existeSiguiente?(c).
 *
 * Postcondicion: El iterador avanza para poder visitar el siguiente elemento
 *                en orden creciente de identificadores.
 *
 * Parametros:
 *   [in,out] a: coleccion cuyo iterador se avanza
 *
 * Devuelve: verdad si se avanzo correctamente, falso si no habia mas elementos
 *
 * Complejidad: O(1)
 */
template <typename I, typename V>
bool avanza(colecInterdep<I, V> &a);

/*
 * FUNCION AUXILIAR PRIVADA
 */
/*
 * Operacion: _buscarNodo (privada)
 * --------------------------------
 * Descripcion: Busca un nodo en la lista por su identificador. Funcion auxiliar de uso interno.
 *
 * Precondicion: Ninguna.
 *
 * Postcondicion: Si existe un nodo con ident igual a id:
 *                - n_out apunta a dicho nodo
 *                - Devuelve verdad
 *                Si no existe:
 *                - n_out apunta al nodo anterior a donde deberia estar id
 *                  (o nullptr si la lista esta vacia)
 *                - Devuelve falso
 *
 * Parametros:
 *   [in] a: coleccion donde buscar
 *   [in] id: identificador a buscar
 *   [in,out] n_out: puntero al nodo encontrado o al anterior
 *
 * Devuelve: verdad si se encontro el nodo, falso en caso contrario
 *
 * Complejidad: O(n) en el peor caso
 *
 * NOTA: Esta funcion es privada y solo debe ser usada internamente por las
 *       operaciones del TAD. No forma parte de la interfaz publica.
 */
template <typename I, typename V>

bool _buscarNodo(const colecInterdep<I, V> &a, const I &id, typename colecInterdep<I, V>::Nodo *&n_out);

/* FIN DE LA PARTE PÚBLICA o INTERFAZ  */

template <typename I, typename V>
struct colecInterdep {
  friend void crear<I, V>(colecInterdep<I, V> &a);
  friend unsigned int tamanio<I, V>(const colecInterdep<I, V> &a);
  friend bool esVacia<I, V>(const colecInterdep<I, V> &a);
  friend bool existe<I, V>(const I &id, const colecInterdep<I, V> &a);
  friend bool existeDependencia<I, V>(const I &id, const colecInterdep<I, V> &a, bool &e_type);
  friend void aniadirIndependiente<I, V>(colecInterdep<I, V> &a, const I &id, const V &v);
  friend void aniadirDependiente<I, V>(colecInterdep<I, V> &a, const I &id, const V &v, const I &super);
  friend void hacerDependiente<I, V>(colecInterdep<I, V> &a, const I &id, const I &super);
  friend void hacerIndependiente<I, V>(colecInterdep<I, V> &a, const I &id);
  friend bool actualizarVal<I, V>(colecInterdep<I, V> &a, const I &id, const V &nuevo);
  friend bool obtenerElemento<I, V>(const I &id, const colecInterdep<I, V> &a, V &v_out, I &s_id_out, unsigned int &n_depen);
  friend void borrar<I, V>(const I &id, colecInterdep<I, V> &a);

  friend void iniciarIterador<I, V>(colecInterdep<I, V> &a);
  friend bool existeSiguiente<I, V>(const colecInterdep<I, V> &a);
  friend bool siguienteIdent<I, V>(const colecInterdep<I, V> &a, I &id);
  friend bool siguienteVal<I, V>(const colecInterdep<I, V> &a, V &val);
  friend bool siguienteDependiente<I, V>(const colecInterdep<I, V> &a);
  friend bool siguienteSuperior<I, V>(const colecInterdep<I, V> &a, I &id);
  friend bool siguienteNumDependientes<I, V>(const colecInterdep<I, V> &a, unsigned int &n_depen);
  friend bool avanza<I, V>(colecInterdep<I, V> &a);


  private:
/* En esta implementación la colecInterdep se representara en memoria dinamica como una lista enlazada de Nodos, simple y no circular, con:
  - el puntero primero: apuntando al Nodo que contiene el elemento que esta en la primera posicion de la lista, sera nullptr si la lista esta vacia
  - el campo tamanio: mantendra calculada el numero de elementos en la lista, sera 0 si la lista esta vacia
  - el puntero iter: se utilizara para mantener el estado del iterador, y unicamente sera utilizado por las operaciones iniciarIterador, existeSiguiente, siguienteIdent, siguienteVal, siguienteDependiente, siguienteSuperior, siguienteNúmDependientes y avanza.
  Cada Nodo contendra un elemento de la lista (en su campo val) junto con su identificador unico y apuntara con su puntero sig: al Nodo que contenga el elemento sigueinte, o dicho puntero tendra valor nullptr si es el último elemento de la lista, el puntero identSup apuntara a el nodo del que depende y el natural numDepend que representa el numero de dependencias que tiene ese nodo.

  El coste de los algorimos esta descrito el la cabecera de cada función y va desde O(1) a O(N)
  El coste en memoria de esta implementacion es de O(N).
*/
  struct Nodo {
    I ident;
    V val;
    Nodo *identSup;
    unsigned int numDepend;
    Nodo *sig;
  };

  Nodo *primero;
  unsigned int tamanio;
  Nodo *iter;

  friend bool _buscarNodo<I, V>(const colecInterdep<I, V> &a, const I &id, typename colecInterdep<I, V>::Nodo *&n_out);
};


//==============================================================================
// IMPLEMENTACION DE LAS OPERACIONES
//==============================================================================


/*
 * Implementacion de crear
 */
template <typename I, typename V>
void crear(colecInterdep<I, V> &a) {
  a.primero = nullptr;
  a.tamanio = 0;
}

/*
 * Implementacion de tamanio
 */
template <typename I, typename V>
unsigned int tamanio(const colecInterdep<I, V> &a) {
  return a.tamanio;
}

/*
 * Implementacion de esVacia
 */
template <typename I, typename V>
bool esVacia(const colecInterdep<I, V> &a) {
  return a.primero == nullptr;
}

/*
 * Implementacion de existe
 */
template <typename I, typename V>
bool existe(const I &id, const colecInterdep<I, V> &a) {

  if (esVacia(a)) { return false; }

  typename colecInterdep<I, V>::Nodo *ignore = nullptr;

  return _buscarNodo(a, id, ignore);
}

/*
 * Implementacion de existeDependencia
 */
template <typename I, typename V>
bool existeDependencia(const I &id, const colecInterdep<I, V> &a, bool &e_type) {

  typename colecInterdep<I, V>::Nodo *aux = nullptr;

  if (_buscarNodo(a, id, aux)) {
    if (aux->identSup == nullptr) {
      e_type = false;
    } else {
      e_type = true;
    }
    return true;
  } else {
    // Nodo no encontrado
    return false;
  }
}

/*
 * Implementacion de aniadirIndependiente
 */
template <typename I, typename V>
void aniadirIndependiente(colecInterdep<I, V> &a, const I &id, const V &v) {

  typename colecInterdep<I, V>::Nodo *nuevoNodo = new typename colecInterdep<I, V>::Nodo;

  nuevoNodo->ident = id;
  nuevoNodo->val = v;
  nuevoNodo->identSup = nullptr;
  nuevoNodo->numDepend = 0;
  nuevoNodo->sig = nullptr;

  if (esVacia(a)) {
    // Aniadir primer elemento de la lista
    typename colecInterdep<I, V>::Nodo *nuevoNodo = new typename colecInterdep<I, V>::Nodo;

    nuevoNodo->ident = id;
    nuevoNodo->val = v;
    nuevoNodo->identSup = nullptr;
    nuevoNodo->numDepend = 0;
    nuevoNodo->sig = nullptr;
    a.primero = nuevoNodo;
    a.tamanio += 1;

  } else {

    typename colecInterdep<I, V>::Nodo *aux_act = a.primero;
    typename colecInterdep<I, V>::Nodo *aux_ant = nullptr;

    // Buscar punto de insercion
    while (aux_act != nullptr && aux_act->ident < id) {
      aux_ant = aux_act;
      aux_act = aux_act->sig;
    }

    if (aux_ant == nullptr) {
      // Insertar al inicio
      typename colecInterdep<I, V>::Nodo *nuevoNodo = new typename colecInterdep<I, V>::Nodo;

      nuevoNodo->ident = id;
      nuevoNodo->val = v;
      nuevoNodo->numDepend = 0;
      nuevoNodo->sig = a.primero;
      nuevoNodo->identSup = nullptr;
      a.primero = nuevoNodo;
      a.tamanio += 1;

    } else if (aux_ant->sig == nullptr) {
      // Insertar al final
      typename colecInterdep<I, V>::Nodo *nuevoNodo = new typename colecInterdep<I, V>::Nodo;

      nuevoNodo->ident = id;
      nuevoNodo->val = v;
      nuevoNodo->numDepend = 0;
      nuevoNodo->sig = nullptr;
      nuevoNodo->identSup = nullptr;
      aux_ant->sig = nuevoNodo;
      a.tamanio += 1;

    } else if (aux_ant->sig != nullptr && aux_ant->sig->ident != id) {
      // Insertar ya que no hay duplicados
      typename colecInterdep<I, V>::Nodo *nuevoNodo = new typename colecInterdep<I, V>::Nodo;

      nuevoNodo->ident = id;
      nuevoNodo->val = v;
      nuevoNodo->numDepend = 0;
      nuevoNodo->sig = aux_ant->sig;
      nuevoNodo->identSup = nullptr;
      aux_ant->sig = nuevoNodo;
      a.tamanio += 1;
    }
  }
}

/*
 * Implementacion de aniadirDependiente
 */
template <typename I, typename V>
void aniadirDependiente(colecInterdep<I, V> &a, const I &id, const V &v, const I &super) {

  // Comprobar si es vacia
  // el id no puede ser el mismo que el super
  // duplicados en caso de un elemento
  // ademas el super tiene que ser el unico elemento existente
  if (esVacia(a) || id == super || a.primero->sig == nullptr && (a.primero->ident == id || a.primero->ident != super)) {
    return;
  }

  typename colecInterdep<I, V>::Nodo *aux_act = a.primero;
  typename colecInterdep<I, V>::Nodo *aux_ant = nullptr;
  typename colecInterdep<I, V>::Nodo *super_node = nullptr;

  // Buscar posicion donde insertar
  while (aux_act != nullptr && (aux_act->ident < id || super_node == nullptr)) {
    if (aux_act->ident == super) {
      super_node = aux_act;
    }
    if (aux_act->ident < id) {
      aux_ant = aux_act;
    }
    aux_act = aux_act->sig;
  }

  if (super_node != nullptr) {
    // Verificar que existe super_node
    if (aux_ant == nullptr) {
      // Insertar al inicio
      typename colecInterdep<I, V>::Nodo *nuevoNodo = new typename colecInterdep<I, V>::Nodo;

      nuevoNodo->ident = id;
      nuevoNodo->val = v;
      nuevoNodo->numDepend = 0;
      nuevoNodo->sig = a.primero;
      super_node->numDepend += 1;
      nuevoNodo->identSup = super_node;
      a.primero = nuevoNodo;
      a.tamanio += 1;

    } else if (aux_ant->sig == nullptr) {
      // Insertar al final
      typename colecInterdep<I, V>::Nodo *nuevoNodo = new typename colecInterdep<I, V>::Nodo;

      nuevoNodo->ident = id;
      nuevoNodo->val = v;
      nuevoNodo->numDepend = 0;
      nuevoNodo->sig = nullptr;
      nuevoNodo->identSup = super_node;
      super_node->numDepend += 1;
      aux_ant->sig = nuevoNodo;
      a.tamanio += 1;

    } else if (aux_ant->sig != nullptr && aux_ant->sig->ident != id) {
      // Insertar ya que no hay duplicados
      typename colecInterdep<I, V>::Nodo *nuevoNodo = new typename colecInterdep<I, V>::Nodo;

      nuevoNodo->ident = id;
      nuevoNodo->val = v;
      nuevoNodo->numDepend = 0;
      nuevoNodo->sig = aux_ant->sig;
      nuevoNodo->identSup = super_node;
      super_node->numDepend += 1;
      aux_ant->sig = nuevoNodo;
      a.tamanio += 1;
    }
  }
}

/*
 * Implementacion de hacerDependiente
 */
template <typename I, typename V>
void hacerDependiente(colecInterdep<I, V> &a, const I &id, const I &super) {

  // Comprobar si la lista esta vacia o el super es distinto id
  if (!esVacia(a) && id != super) {

    typename colecInterdep<I, V>::Nodo *id_node = nullptr;
    typename colecInterdep<I, V>::Nodo *super_node = nullptr;


    typename colecInterdep<I, V>::Nodo *aux_act = a.primero;
    // Buscar nodo con id y al padre
    while (aux_act != nullptr && (id_node == nullptr || super_node == nullptr)) {
      if (aux_act->ident == super) {
        super_node = aux_act;
      } else if (aux_act->ident == id) {
        id_node = aux_act;
      }
      aux_act = aux_act->sig;
    }

    // No se ha encontrado el elemento con id o no se ha encontrado el (padre) super
    if (id_node != nullptr && super_node != nullptr) {
      // Ambos nodo encontrados
      if (id_node->identSup != nullptr) {
        // Si ya era dependiente de otro nodo, quitamos la dependencia
        id_node->identSup->numDepend -= 1;
      }
      // Añadir dependencia
      id_node->identSup = super_node;
      super_node->numDepend += 1;
    }
  }
}

/*
 * Implementacion de hacerIndependiente
 */
template <typename I, typename V>
void hacerIndependiente(colecInterdep<I, V> &a, const I &id) {

  typename colecInterdep<I, V>::Nodo *id_node = nullptr;

  if (_buscarNodo(a, id, id_node) && id_node->identSup != nullptr) {

    // Si el nodo es dependiente
    id_node->identSup->numDepend -= 1;
    id_node->identSup = nullptr;
  }
}

/*
 * Implementacion de actualizarVal
 */
template <typename I, typename V>
bool actualizarVal(colecInterdep<I, V> &a, const I &id, const V &nuevo) {

  typename colecInterdep<I, V>::Nodo *aux = nullptr;

  if (_buscarNodo(a, id, aux)) {
    // Si lo encuentra actualiza el el contenido
    aux->val = nuevo;
    return true;
  } else {
    // Nodo no encontrado
    return false;
  }
}

/*
 * Implementacion de obtenerElemento
 */
template <typename I, typename V>
bool obtenerElemento(const I &id, const colecInterdep<I, V> &a, V &v_out, I &s_id_out, unsigned int &n_depen) {
  typename colecInterdep<I, V>::Nodo *aux = nullptr;

  if (_buscarNodo(a, id, aux)) {
    v_out = aux->val;

    if (aux->identSup != nullptr) {
      s_id_out = aux->identSup->ident;
    } else {
      s_id_out = "";
    }

    n_depen = aux->numDepend;

    return true;
  } else {
    // Nodo no encontrado
    return false;
  }
}

/*
 * Implementacion de borrar
 */
template <typename I, typename V>
void borrar(const I &id, colecInterdep<I, V> &a) {

  if (esVacia(a)) { return; }

  // Si la lista tiene elementos
  typename colecInterdep<I, V>::Nodo *aux_act = a.primero;
  typename colecInterdep<I, V>::Nodo *aux_ant = nullptr;

  // Avanzar hasta llegar al elemento buscado o al final de la lista
  while (aux_act != nullptr && aux_act->ident < id) {
    aux_ant = aux_act;
    aux_act = aux_act->sig;
  }

  if (aux_act != nullptr && aux_act->ident == id && aux_act->numDepend == 0) {
    // Si se ha encontrado el nodo y no tiene dependencias, se borra
    if (aux_act->identSup != nullptr) {
      // Si es dependiente, se quita una dependencia del padre
      aux_act->identSup->numDepend -= 1;
    }
    if (aux_ant == nullptr) {
      // En caso de borrar el primero
      a.primero = a.primero->sig;
    } else {
      // En caso de no ser el primer elemento
      aux_ant->sig = aux_act->sig;
    }
    delete aux_act;
    a.tamanio -= 1;
  }
}

/*
 * Implementacion de iniciarIterador
 */
template <typename I, typename V>
void iniciarIterador(colecInterdep<I, V> &a) {
  a.iter = a.primero;
}

/*
 * Implementacion de existeSiguiente
 */
template <typename I, typename V>
bool existeSiguiente(const colecInterdep<I, V> &a) {
  return a.iter != nullptr;
}

/*
 * Implementacion de siguienteIdent
 */
template <typename I, typename V>
bool siguienteIdent(const colecInterdep<I, V> &a, I &id) {

  if (existeSiguiente(a)) {
    id = a.iter->ident;
    return true;
  } else {
    return false;
  }
}

/*
 * Implementacion de siguienteVal
 */
template <typename I, typename V>
bool siguienteVal(const colecInterdep<I, V> &a, V &val) {

  if (existeSiguiente(a)) {
    val = a.iter->val;
    return true;
  } else {
    return false;
  }
}

/*
 * Implementacion de siguienteDependiente
 */
template <typename I, typename V>
bool siguienteDependiente(const colecInterdep<I, V> &a) {

  if (existeSiguiente(a)) {
    return a.iter->identSup != nullptr;
  } else {
    return false;
  }
}

/*
 * Implementacion de siguienteSuperior
 */
template <typename I, typename V>
bool siguienteSuperior(const colecInterdep<I, V> &a, I &id) {

  if (existeSiguiente(a) && siguienteDependiente(a)) {
    id = a.iter->identSup->ident;
    return true;
  } else {
    return false;
  }
}

/*
 * Implementacion de siguienteNumDependientes
 */
template <typename I, typename V>
bool siguienteNumDependientes(const colecInterdep<I, V> &a, unsigned int &n_depen) {

  if (existeSiguiente(a)) {
    n_depen = a.iter->numDepend;
    return true;
  } else {
    return false;
  }
}

/*
 * Implementacion de avanza
 */
template <typename I, typename V>
bool avanza(colecInterdep<I, V> &a) {

  if (existeSiguiente(a)) {
    a.iter = a.iter->sig;
    return true;
  } else {
    return false;
  }
}

/*
 * Implementacion de _buscarNodo
 */
template <typename I, typename V>
bool _buscarNodo(const colecInterdep<I, V> &a, const I &id, typename colecInterdep<I, V>::Nodo *&n_out) {

  if (esVacia(a)) {
    // Coleccion vacia: no se encuentra el nodo
    n_out = nullptr;
    return false;
  }

  typename colecInterdep<I, V>::Nodo *aux_act = a.primero;
  typename colecInterdep<I, V>::Nodo *aux_ant = nullptr;

  // Buscar nodo
  while (aux_act != nullptr && aux_act->ident < id) {
    aux_ant = aux_act;
    aux_act = aux_act->sig;
  }

  if (aux_act != nullptr && aux_act->ident == id) {
    // Elemento encontrado
    n_out = aux_act;
    return true;
  } else {
    // Elemento no encontrado
    n_out = aux_ant;
    return false;
  }
}

#endif