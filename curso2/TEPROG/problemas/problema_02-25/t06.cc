#include <iostream>
#include <string>

/*** RESUMEN DE LA SEMANA PASADA ***/

template <typename T>
class Box {
private:
    T value;
public:
    Box(T v) : value(v) {}

    T get() const {
        return value;
    }

    void set(T v) {
        value = v;
    }

    void print() const {
        std::cout << "Value: " << value << std::endl;
    }
};

Box<int> intBox(10);
intBox.print();  

Box<std::string> stringBox("Hello");
stringBox.print(); 





// Generic class with two types

template <typename T, typename U>
class Pair {
private:
    T first;
    U second;

public:
    Pair(T f, U s) : first(f), second(s) {}

    T getFirst() const {
        return first;
    }

    U getSecond() const {
        return second;
    }

    void print() const {
        std::cout << "(" << first << ", " << second << ")" << std::endl;
    }
};


Pair<int, double> p1(3, 4.5);
p1.print();  

Pair<std::string, int> p2("Age", 25);
p2.print(); 




// Generic method

template <typename T>
T maxValue(T a, T b) {
    return (a > b) ? a : b;
}





// Acotaciones en JAVA

static <T extends Number> double suma(T a, T b) {
    return a.doubleValue() + b.doubleValue();
}




// Sobrecarga de métodos

class Complex {
    float real,imag;
    Complex(float r, float i = 0.0):
        real(r), imag(i) { }
};

template<typename T>
T module(const T& x) {
    return (x >= 0 ? x : -x);
}

float module(const Complex& z) {
    return std::hypot(z.real, z.imag);
}

Complex z(3.0f, 4.0f);
cout << module(z);     






// Sobrecarga de operadores

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







/*********************************/


// Herencia y genéricos - ¿Qué es posible?


// Opción 1 - Fijar el tipo base en la herencia (D)

template <typename T>
class B { };

class D : public B<int> { };   // OK: T es explícitamente int

D thing;                      // OK



// Opción 2 - Hacer al hijo genérico también

template <typename T>
class B {};

template <typename T>
class D : public B<T> {};     // OK: Puedo usar T, que además ya está definido

D<int> thing;                 // OK



// Opción 3 - Darle a D su propio parámetro

template <typename T>
class B { };

template <typename U>
class D : public B<U> { };     // OK

D<int> thing;



// Opción 4 - Fijar al padre

template <typename T>
class B {};

template <typename T>
class D : public B<int> {};

D<std::string> thing;



// ESTO NO FUNCIONARIA:

template <typename T>
class B { };

template <typename U>
class D : public B<T> { };   // ❌ ERROR














// Reglas de herencia a datos parámetro

template<typename T>
class Ptr { public: T* addr; };

class A {};
class B : public A {};

A a; Ptr<A> pa;
B b; Ptr<B> pb;

pa.addr = &a;  // A*  <- A*   (OK)
pb.addr = &b;  // B*  <- B*   (OK)
pa.addr = &b;  // A*  <- B*   (upcast) (OK) because B* converts to A*
pb.addr = &a;  // B*  <- A*   (❌ ERROR) --> not allowed

/* RECORDAD

Every B is an A,
    ... but not every A is a B.
    ... if you allowed it, you could try to access B-only members → crash

*/









// Java can also have these behaviors:

interface Pair<K, V> {
    K getKey();
    V getValue();
}

class MyPair<K, V> implements Pair<K, V> {
    private K key;
    private V value;

    public MyPair(K key, V value) {
        this.key = key; this.value = value;
    }

    public K getKey() { return key; }
    public V getValue() { return value; }
}








// Relaciones entre tipos derivados

class String extends Object;

String s = new String("hello"); 
Object o = s;






class String extends Object;

List<String> ls = new ArrayList<String>();

// Si esto fuera posible...
List<Object> lo = ls;

// ..cuidado:
lo.add(new Integer(7));

String s = ls.get(0);



// List<B> is NOT a subtype of List<A> 
//     ... even if B extends A







// WILDCARDS vs. OBJECTS

class Collection<T> {
    ...
}

// CASE 1
void print(Collection<Object> c) {
    for (Object e : c) {
        System.out.println(e);   
    }
}

Collection<Object> co = new ArrayList<>();
print(co);   // OK

Collection<String> cs = new ArrayList<>();
print(cs);   // ❌ ERROR




class Collection<T> {
    ...
}

// CASE 2
void print(Collection<?> c) {               // <?> = <? extends Object>
    for (Object e : c) {
        System.out.println(e);   
    }
}

Collection<String> cs = new ArrayList<>();
print(cs);   // OK

Collection<Integer> ci = new ArrayList<>();
print(ci);   // OK







// Wildcards acotados

abstract class Shape {
    abstract void draw();
}

class Circle extends Shape { ... } 
class Rectangle extends Shape { ... }

void drawAll(List<Shape> shapes) {
    for (Shape s : shapes)
        s.draw();
}





// Incluso si Circle <-- Shape, List<Circle> NO <-- List<Shape> !!





abstract class Shape {
    abstract void draw();
}

class Circle extends Shape { ... } 
class Rectangle extends Shape { ... }

void drawAll(List<?> shapes) {
    for (Shape s : shapes)
        s.draw();
}





// No sabemos si ? tendrá draw() !!





abstract class Shape {
    abstract void draw();
}

class Circle extends Shape { ... } 
class Rectangle extends Shape { ... }

void drawAll(List<? extends Shape> shapes) {
    for (Shape s : shapes)
        s.draw();
}









// En C++

class Shape {
    virtual void draw() const = 0;
};

class Circle : public Shape {
    void draw() const override;
};

void draw_all(const list<Shape∗>& shapes) {
    for (Shape∗ s : shapes) s−>draw(); 
}

// ¿ Que ocurre para list<Circle*> ?











class Shape {
    virtual void draw() const = 0;
};

class Circle : public Shape {
    void draw() const override;
};

template<typename S>
void draw_all(const list<S∗>& shapes) {
    for (S∗ s : shapes) s−>draw();          // Esto obliga en tiempo de compilación
}











// Statics en Java

class Box<T> {
    static int counter;   // OK
    static Object latest; // OK
    static T first;       // ERROR!
}


















// ----------------------------------------------
// ----------------------------------------------
// ----------------------------------------------