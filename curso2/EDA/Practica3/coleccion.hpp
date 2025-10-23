/* 
   Daniel Ubalde, NIP:
   Eduardo Guerrero, NIP:
*/

#ifndef COLECCION_HPP
#define COLECCION_HPP

// PREDECLARACION DEL TAD GENERICO agrupacion (inicio INTERFAZ)

const int MAX = 40;

template<typename I, typename V> struct coleccion;

// 
template<typename I, typename V>  void crear(coleccion<I,V>& c);

// 
template<typename I, typename V>  int tamano(const coleccion<I,V>& c);

// 
template<typename I, typename V>  bool esVacia(const coleccion<I,V>& c);

// 
template<typename I, typename V>  bool existe(I id, const coleccion<I,V>& c);

// 
template<typename I, typename V>  bool existeDependiente(I id, coleccion<I,V>& c);

// 
template<typename I, typename V>  bool existeIndependiente(I id, coleccion<I,V>& c);

// 
template<typename I, typename V>  void anyadirIndependiente(coleccion<I,V>& c, I id, V v);

// 
template<typename I, typename V>  void anyadirDependiente(coleccion<I,V>& ccol, I id, V v, I super);

// 
template<typename I, typename V>  void hacerDependiente(coleccion<I,V>& c, I id, I super);

// 
template<typename I, typename V>  void hacerIndependiente(coleccion<I,V>& c, I id );


// 
template<typename I, typename V>  void actualizarVal(coleccion<I,V>& c, I id, V nuevo);

// 
template<typename I, typename V>  V obtenerVal(I id, const coleccion<I,V>& c);

// 
template<typename I, typename V>  I obtenerSupervisor(I id, const coleccion<I,V>& c);

// 
template<typename I, typename V>  int obtenerNumDependientes(I id, const coleccion<I,V>& c);

// 
template<typename I, typename V>  void borrar(I id, coleccion<I,V>& c);

// 
template<typename I, typename V>  void iniciarIterador(coleccion<I,V>& c);

// 
template<typename I, typename V>  bool existeSiguiente(const coleccion<I,V>& c);

// 
template<typename I, typename V>  I siguienteIdent(const coleccion<I,V>& c);

// 
template<typename I, typename V>  V siguienteVal(const coleccion<I,V>& c);

// 
template<typename I, typename V>  bool siguienteDependiente(const coleccion<I,V>& c);

// 
template<typename I, typename V>  I siguienteSuperior(const coleccion<I,V>& c);

// 
template<typename I, typename V>  int siguienteNúmDependientes(const coleccion<I,V>& c);

// 
template<typename I, typename V>  void avanza(coleccion<I,V>& c);

// 
template<typename I, typename V>  I siguienteIdent(coleccion<I,V>& c);

// 
template<typename I, typename V>  V siguienteVal(coleccion<I,V>& c);



// DECLARACION DEL TAD GENERICO coleccion

template<typename I, typename V>
struct coleccion{
   friend template<typename I, typename V>  void crear(coleccion<I,V>& c);
   friend template<typename I, typename V>  int tamano(const coleccion<I,V>& c);
   friend template<typename I, typename V>  bool esVacia(const coleccion<I,V>& c);
   friend template<typename I, typename V>  bool existe(I id, const coleccion<I,V>& c);
   friend template<typename I, typename V>  bool existeDependiente(I id, coleccion<I,V>& c);
   friend template<typename I, typename V>  bool existeIndependiente(I id, coleccion<I,V>& c);
   friend template<typename I, typename V>  void anyadirIndependiente(coleccion<I,V>& c, I id, V v);
   friend template<typename I, typename V>  void anyadirDependiente(coleccion<I,V>& ccol, I id, V v, I super);
   friend template<typename I, typename V>  void hacerDependiente(coleccion<I,V>& c, I id, I super);
   friend template<typename I, typename V>  void hacerIndependiente(coleccion<I,V>& c, I id );
   friend template<typename I, typename V>  void actualizarVal(coleccion<I,V>& c, I id, V nuevo);
   friend template<typename I, typename V>  V obtenerVal(I id, const coleccion<I,V>& c);
   friend template<typename I, typename V>  I obtenerSupervisor(I id, const coleccion<I,V>& c);
   friend template<typename I, typename V>  int obtenerNumDependientes(I id, const coleccion<I,V>& c);
   friend template<typename I, typename V>  void borrar(I id, coleccion<I,V>& c);
   friend template<typename I, typename V>  void iniciarIterador(coleccion<I,V>& c);
   friend template<typename I, typename V>  bool existeSiguiente(const coleccion<I,V>& c);
   friend template<typename I, typename V>  I siguienteIdent(const coleccion<I,V>& c);
   friend template<typename I, typename V>  V siguienteVal(const coleccion<I,V>& c);
   friend template<typename I, typename V>  bool siguienteDependiente(const coleccion<I,V>& c);
   friend template<typename I, typename V>  I siguienteSuperior(const coleccion<I,V>& c);
   friend template<typename I, typename V>  int siguienteNúmDependientes(const coleccion<I,V>& c);
   friend template<typename I, typename V>  void avanza(coleccion<I,V>& c);
   friend template<typename I, typename V>  I siguienteIdent(coleccion<I,V>& c);
   friend template<typename I, typename V>  V siguienteVal(coleccion<I,V>& c);

   private:  //declaracion de la representacion interna del tipo
      // TODO: me he quedado por aqui

};

#endif
