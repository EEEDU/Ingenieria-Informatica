#include "rational.h"

// Auxiliares

int Rational::mcd(int a, int b)
{
	return ( b == 0 ? a : mcd(b,a%b));
}

void Rational::reduce()
{
	int d = mcd(numerador, denominador);
	numerador /= d;
	denominador /= d;
}

// Constructores

Rational::Rational()
{
	numerador = 0;
	denominador = 1;
}

Rational::Rational(int num, int den)
{
	numerador = num;
	if (den == 0)
	{
		std::cerr << "Error: denominador 0 no permitido\n";
		denominador = 1;
	}
	else
	{
		denominador = den;
	}
}

// Operadores aritmeticos

Rational Rational::operator+(const Rational& rat) const
{
	Rational result;
	result.numerador = numerador * rat.denominador + rat.numerador * denominador;
	result.denominador = denominador * rat.denominador;
	result.reduce();
	return result;
}

Rational Rational::operator-(const Rational& rat) const
{
		Rational result;
	result.numerador = numerador * rat.denominador - rat.numerador * denominador;
	result.denominador = denominador * rat.denominador;
	result.reduce();
	return result;
}

Rational operator*(const Rational& rat1, const Rational& rat2)
{
	Rational result;
	result.numerador = rat1.numerador * rat2.numerador;
	result.denominador = rat1.denominador * rat2.denominador;
	result.reduce();
	return result;
}

Rational operator/(const Rational& rat1, const Rational& rat2)
{
	Rational result;
	result.numerador = rat1.numerador * rat2.denominador;
	result.denominador = rat1.denominador * rat2.numerador;
	result.reduce();
	return result;
}

// Operadores logicos

bool Rational::operator==(const Rational& rat) const
{
	return numerador * rat.denominador == rat.numerador * denominador;
}

bool Rational::operator<(const Rational& rat) const
{		
	return numerador * rat.denominador < rat.numerador * denominador;
}

bool Rational::operator>(const Rational& rat) const
{
	return numerador * rat.denominador > rat.numerador * denominador;
}

// Entrada/salida

std::ostream& operator<<(std::ostream& os, const Rational& rat)
{
	os << rat.numerador << "/" << rat.denominador;
	return os;
}

std::istream& operator>>(std::istream& is, Rational& rat)
{
	char barra;
    is >> rat.numerador >> barra >> rat.denominador;

    if (rat.denominador == 0)
    {
        std::cerr << "Error: denominador 0 no permitido\n";
        rat.denominador = 1;
    };
	return is;
}



