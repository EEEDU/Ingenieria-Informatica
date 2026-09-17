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

// Entrada/salida

void Rational::write(std::ostream& os) const
{
	os << numerador << "/" << denominador;
}

void Rational::read(std::istream& is)
{
	char barra;
    is >> numerador >> barra >> denominador;

    if (denominador == 0)
    {
        std::cerr << "Error: denominador 0 no permitido\n";
        denominador = 1;
    };
}

// Operaciones aritmeticas

Rational Rational::add(const Rational& rat) const
{
	Rational result;
	result.numerador = numerador * rat.denominador + rat.numerador * denominador;
	result.denominador = denominador * rat.denominador;
	result.reduce();
	return result;
}

Rational Rational::sub(const Rational& rat) const
{
	Rational result;
	result.numerador = numerador * rat.denominador - rat.numerador * denominador;
	result.denominador = denominador * rat.denominador;
	result.reduce();
	return result;
}

Rational Rational::mul(const Rational& rat) const
{
	Rational result;
	result.numerador = numerador * rat.numerador;
	result.denominador = denominador * rat.denominador;
	result.reduce();
	return result;
}

Rational Rational::div(const Rational& rat) const
{
	Rational result(numerador * rat.denominador, denominador * rat.numerador);
	result.reduce();
	return result;
}

// Operaciones logicas

bool Rational::equal(const Rational& rat) const
{
	Rational r1 = Rational(numerador, denominador), r2 = rat;
	r1.reduce(); r2.reduce();
	return r1.numerador == r2.numerador && r1.denominador == r2.denominador;
}

bool Rational::lesser_than(const Rational& rat) const
{
	return numerador / denominador < rat.numerador / rat.denominador; 
}

bool Rational::greater_than(const Rational& rat) const
{
	return numerador / denominador > rat.numerador / rat.denominador; 
}
