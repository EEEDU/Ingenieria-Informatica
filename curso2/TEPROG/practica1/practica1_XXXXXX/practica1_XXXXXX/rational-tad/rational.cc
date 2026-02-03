#include "rational.h"

// Auxiliares, privadas

int mcd(int a, int b)
{
	return ( b==0 ? a : mcd(b,a%b) );
}

void reduce(Rational& r)
{
	int d = mcd(r.numerador, r.denominador);
	r.numerador /= d;
	r.denominador /= d;
}

// Inicializadores

	void init(Rational& r)
	{
		r.numerador = 0;
		r.denominador = 1;
	}

	void init(Rational& r,int num, int den)
	{
		r.numerador = num;
		if (den == 0)
		{
			std::cerr << "Error: denominador 0 no permitido\n";
			r.denominador = 1;
		}
		else
		{
			r.denominador = den;
		}
	}

// Entrada/salida

void write(std::ostream& os, const Rational& r)
{
	os << r.numerador << "/" << r.denominador;
}

void read(std::istream& is, Rational& r)
{
	char barra;
    is >> r.numerador >> barra >> r.denominador;

    if (r.denominador == 0)
    {
        std::cerr << "Error: denominador 0 no permitido\n";
        r.denominador = 1;
    }
}

// Operaciones aritmeticas

Rational add(const Rational& r1, const Rational& r2)
{
	Rational result;
	result.numerador = r1.numerador * r2.denominador + r2.numerador * r1.denominador;
	result.denominador = r1.denominador * r2.denominador;
	reduce(result);
	return result;
}

Rational sub(const Rational& r1, const Rational& r2)
{
	Rational result;
	result.numerador = r1.numerador * r2.denominador - r2.numerador * r1.denominador;
	result.denominador = r1.denominador * r2.denominador;
	reduce(result);
	return result;
}

Rational mul(const Rational& r1, const Rational& r2)
{
	Rational result;
	result.numerador = r1.numerador * r2.numerador;
	result.denominador = r1.denominador * r2.denominador;
	reduce(result);
	return result;
}

Rational div(const Rational& r1, const Rational& r2)
{
	Rational result;
	result.numerador = r1.numerador * r2.denominador;
	result.denominador = r1.denominador * r2.numerador;
	reduce(result);
	return result;
}

// Operaciones logicas

bool equal(const Rational& r1, const Rational& r2)
{
	Rational rat1 = r1, rat2 = r2;
	reduce(rat1); reduce(rat2);
	return r1.numerador == r2.numerador && r1.denominador == r2.denominador;
}

bool lesser_than(const Rational& r1, const Rational& r2)
{
	return r1.numerador / r1.denominador < r2.numerador / r2.denominador; 
}

bool greater_than(const Rational& r1, const Rational& r2)
{
	return r1.numerador / r1.denominador > r2.numerador / r2.denominador; 
}
