#include "rational.h"

// Auxiliares

int Rational::mcd(int a, int b)
{
	return ( b == 0 ? a : mcd(b,a%b));
}

void Rational::reduce()
{
	...
}

// Constructores

Rational::Rational()
{
	...
}

Rational::Rational(int num, int den)
{
	...
}

// Entrada/salida

void Rational::write(std::ostream& os) const
{
	...
}

void Rational::read(std::istream& is)
{
	...
}

// Operaciones aritmeticas

... Rational::add(...) ...
{
	...
}

... Rational::sub(...) ...
{
	...
}

... Rational::mul(...) ...
{
	...
}

... Rational::div(...) ...
{
	...
}

// Operaciones logicas

... Rational::equal(...) ...
{
	...
}

... Rational::lesser_than(...) ...
{
	...
}

... Rational::greater_than(...) ...
{
	...
}
