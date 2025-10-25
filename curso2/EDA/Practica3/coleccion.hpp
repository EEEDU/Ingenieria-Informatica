/* 
   Daniel Ubalde, NIP:
   Eduardo Guerrero, NIP:
*/

#ifndef COLECCION_HPP
#define COLECCION_HPP

// PREDECLARACION DEL TAD GENERICO agrupacion (inicio INTERFAZ)

template<typename I, typename V> struct Coleccion;

// 
template<typename I, typename V>  void crear(Coleccion<I,V>& c);

// 
template<typename I, typename V>  int tamano(const Coleccion<I,V>& c);

// 
template<typename I, typename V>  bool esVacia(const Coleccion<I,V>& c);

// 
template<typename I, typename V>  bool existe(I id, const Coleccion<I,V>& c);

// 
template<typename I, typename V>  bool existeDependiente(I id, Coleccion<I,V>& c);

// 
template<typename I, typename V>  bool existeIndependiente(I id, Coleccion<I,V>& c);

// 
template<typename I, typename V>  void anyadirIndependiente(Coleccion<I,V>& c, I id, V v);

// 
template<typename I, typename V>  void anyadirDependiente(Coleccion<I,V>& ccol, I id, V v, I super);

// 
template<typename I, typename V>  void hacerDependiente(Coleccion<I,V>& c, I id, I super);

// 
template<typename I, typename V>  void hacerIndependiente(Coleccion<I,V>& c, I id );

// 
template<typename I, typename V>  void actualizarVal(Coleccion<I,V>& c, I id, V nuevo);

// 
template<typename I, typename V>  V obtenerVal(I id, const Coleccion<I,V>& c);

// 
template<typename I, typename V>  I obtenerSupervisor(I id, const Coleccion<I,V>& c);

// 
template<typename I, typename V>  int obtenerNumDependientes(I id, const Coleccion<I,V>& c);

// 
template<typename I, typename V>  void borrar(I id, Coleccion<I,V>& c);

// 
template<typename I, typename V>  void iniciarIterador(Coleccion<I,V>& c);

// 
template<typename I, typename V>  bool existeSiguiente(const Coleccion<I,V>& c);

// 
template<typename I, typename V>  I siguienteIdent(const Coleccion<I,V>& c);

// 
template<typename I, typename V>  V siguienteVal(const Coleccion<I,V>& c);

// 
template<typename I, typename V>  bool siguienteDependiente(const Coleccion<I,V>& c);

// 
template<typename I, typename V>  I siguienteSuperior(const Coleccion<I,V>& c);

// 
template<typename I, typename V>  int siguienteNumDependientes(const Coleccion<I,V>& c);

// 
template<typename I, typename V>  void avanza(Coleccion<I,V>& c);

// 
template<typename I, typename V>  I siguienteIdent(Coleccion<I,V>& c);

// 
template<typename I, typename V>  V siguienteVal(Coleccion<I,V>& c);



// DECLARACION DEL TAD GENERICO Coleccion

template<typename I, typename V>
struct Coleccion{
   friend void crear(Coleccion<I,V>& c);
   friend int tamano(const Coleccion<I,V>& c);
   friend bool esVacia(const Coleccion<I,V>& c);
   friend bool existe(I id, const Coleccion<I,V>& c);
   friend bool existeDependiente(I id, Coleccion<I,V>& c);
   friend bool existeIndependiente(I id, Coleccion<I,V>& c);
   friend void anyadirIndependiente(Coleccion<I,V>& c, I id, V v);
   friend void anyadirDependiente(Coleccion<I,V>& ccol, I id, V v, I super);
   friend void hacerDependiente(Coleccion<I,V>& c, I id, I super);
   friend void hacerIndependiente(Coleccion<I,V>& c, I id );
   friend void actualizarVal(Coleccion<I,V>& c, I id, V nuevo);
   friend V obtenerVal(I id, const Coleccion<I,V>& c);
   friend I obtenerSupervisor(I id, const Coleccion<I,V>& c);
   friend int obtenerNumDependientes(I id, const Coleccion<I,V>& c);
   friend void borrar(I id, Coleccion<I,V>& c);
   friend void iniciarIterador(Coleccion<I,V>& c);
   friend bool existeSiguiente(const Coleccion<I,V>& c);
   friend I siguienteIdent(const Coleccion<I,V>& c);
   friend V siguienteVal(const Coleccion<I,V>& c);
   friend bool siguienteDependiente(const Coleccion<I,V>& c);
   friend I siguienteSuperior(const Coleccion<I,V>& c);
   friend int siguienteNumDependientes(const Coleccion<I,V>& c);
   friend void avanza(Coleccion<I,V>& c);

   private:  //declaracion de la representacion interna del tipo
   
      struct Elemento{
         I ident;
         V val;
         bool esDependiente;
         I identSup;
         int numDepend;
         Elemento* sig;
      };

      Elemento* primera;
      int tamano;
      Elemento* actual;

};

#endif
