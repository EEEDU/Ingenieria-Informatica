#include <iostream>
#include <string>



template <typename T>
class Caja {
    T valor;

public:
    Caja(T v) : valor(v) {}

    T get() const { return valor; }

    // Necesita que T tenga operator<
    bool esMenorQue(T v) const {
        return valor < v;
    }
};



class Persona {
    std::string nombre;
    int edad;

public:
    Persona(std::string n, int e) : nombre(n), edad(e) {}

    // Comparar por edad
    bool operator<(const Persona& otra) const {
        return edad < otra.edad;
    }
};



class Coche {
    std::string modelo;
    int precio;

public:
    Coche(std::string m, int p) : modelo(m), precio(p) {}

    // Comparar por precio
    bool operator<(const Coche& otro) const {
        return precio < otro.precio;
    }
};

int main() {

    // ---- Con int
    Caja<int> a(3);
    Caja<int> b(5);
    std::cout << a.esMenorQue(b.get()) << "\n";   

    // ---- Con Persona
    Caja<Persona> p1(Persona("persona1", 12));
    Caja<Persona> p2(Persona("persona2", 34));
    std::cout << p1.esMenorQue(p2.get()) << "\n"; 

    // ---- Con Coche
    Caja<Coche> c1(Coche("modelo1", 3400));
    Caja<Coche> c2(Coche("modelo2", 200));
    std::cout << c1.esMenorQue(c2.get()) << "\n"; 

    return 0;
}