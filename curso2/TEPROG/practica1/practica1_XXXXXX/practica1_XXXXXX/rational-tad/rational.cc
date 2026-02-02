#include "rational.h"

// Auxiliares, privadas

int mcd(int a, int b)
{
	return ( b==0 ? a : mcd(b,a%b) );
}

void reduce(Rational& r)
{
	...
}

// Inicializadores

void init(Rational& r)
{
	...
}

void init(Rational& r,int num, int den)
{
	...
}

// Entrada/salida

void write(std::ostream& os, const Rational& r)
{
	...
}

void read(std::istream& is, Rational& r)
{
	...
}

// Operaciones aritmeticas

Rational add(const Rational& r1, const Rational& r2)
{
	...
}

Rational sub(const Rational& r1, const Rational& r2)
{
	...
}

Rational mul(const Rational& r1, const Rational& r2)
{
	...
}

Rational div(const Rational& r1, const Rational& r2)
{
	...
}

// Operaciones logicas

bool equal(const Rational& r1, const Rational& r2)
{
	...
}

bool lesser_than(const Rational& r1, const Rational& r2)
{
	...
}

bool greater_than(const Rational& r1, const Rational& r2)
{
	...
}
