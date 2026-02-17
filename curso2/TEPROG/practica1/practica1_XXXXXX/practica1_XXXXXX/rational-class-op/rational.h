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

	// Operadores aritmeticos

	Rational operator+(const Rational& rat) const;
	Rational operator-(const Rational& rat) const;
	friend Rational operator*(const Rational& rat1, const Rational& rat2);
	friend Rational operator/(const Rational& rat1, const Rational& rat2);
	// Operadores logicos

	bool operator==(const Rational& rat) const;
	bool operator<(const Rational& rat) const;
	bool operator>(const Rational& rat) const;

	// Friends, en su caso
	friend std::ostream& operator<<(std::ostream& os, const Rational& rat);
	friend std::istream& operator>>(std::istream& is, Rational& rat);
};

// Operadores aritmeticos

Rational operator*(const Rational& rat1, const Rational& rat2);
Rational operator/(const Rational& rat1, const Rational& rat2);

// Entrada/salida

std::ostream& operator<<(std::ostream& os, const Rational& rat);
std::istream& operator>>(std::istream& is, Rational& rat);
