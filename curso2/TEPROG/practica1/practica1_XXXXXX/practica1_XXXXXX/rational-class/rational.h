#pragma once

#include <iostream>

// Solucion con atributos privados.

class Rational
{
private:
	// Atributos

private:
	// Auxiliares

	static int mcd(int a, int b);
	void reduce();

public:
	// Constructores

	Rational();
	Rational(int num, int den);

	// Entrada/salida

	void write(std::ostream& os) const;
	void read(std::istream& is);

	// Operaciones aritmeticas

	... add(...) ...;
	... sub(...) ...;
	... mul(...) ...;
	... div(...) ...;

	// Operaciones logicas

	... equal(...) ...;
	... lesser_than(...) ...;
	... greater_than(...) ...;
};
