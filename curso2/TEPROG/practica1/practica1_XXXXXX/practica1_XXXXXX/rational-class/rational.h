#pragma once

#include <iostream>

// Solucion con atributos privados.

class Rational
{
private:
	// Atributos
	int numerador;
	int denominador;

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

	Rational add(const Rational& rat) const;
	Rational sub(const Rational& rat) const;
	Rational mul(const Rational& rat) const;
	Rational div(const Rational& rat) const;

	// Operaciones logicas

	bool equal(const Rational& rat) const;
	bool lesser_than(const Rational& rat) const;
	bool greater_than(const Rational& rat) const;
};
