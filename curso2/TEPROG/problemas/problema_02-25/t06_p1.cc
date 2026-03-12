
// 1 - Complete so the method accepts List<Circle> and List<Rectangle>

abstract class Shape {
    abstract void draw();
}

class Circle extends Shape {
    void draw() { }
}

class Rectangle extends Shape {
    void draw() { }
}

void drawAll(List<? extends Shape> shapes) {
    for (Shape s : shapes)
        s.draw();
}


// 2 - Fill with a valid type so it compiles

class Box<T> {
    static T latest;
    T data;
}



// 3 - FIll so it works

#include <iostream>

template<typename T>
class Base {
    protected:
        T value;
    public:
        Base(T v) : value(v) {}
};

template<typename T>
class Derived : public Base<T> {
    public:
        Derived(T v) : Base<T>(v) {}

        // Print value
        void print() const {
            std::cout << value << std::endl;
        }
};  



// 4 - Fill so it works

#include <iostream>

template<typename T>
class Holder {
protected:
    T data;
public:
    Holder(T d) : data(d) {}
};

template<typename A, typename B>
class PairHolder : public Holder<A> {
    B second;
public:
    PairHolder(A a, B b)
        : Holder<A>(a), second(b) {}

    void print() const {
        std::cout << data << " , " << second << std::endl;
    }
};