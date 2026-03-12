#include "collide.h"
#include <iostream>

using namespace std;

bool collide(const Shape& s,const Shape& t)
{
	cout << __PRETTY_FUNCTION__ << endl;

	bool res = false;
	for (const auto& p : s.points())
		res |= t.contains(p);
	for (const auto& p : t.points())
		res |= s.contains(p);

	return res;
}

bool collide(const Circle& s,const Shape& t)
{
	cout << __PRETTY_FUNCTION__ << endl;

	bool res = false;
	for (const auto& p : t.points())
		res |= s.contains(p);

	return res;
}

bool collide(const Shape& s,const Circle& t)
{
	cout << __PRETTY_FUNCTION__ << endl;

	bool res = false;
	for (const auto& p : s.points())
		res |= t.contains(p);

	return res;
}

bool collide(const Circle& s,const Circle& t)
{
	cout << __PRETTY_FUNCTION__ << endl;

	return distance(s.center(),t.center())<=(s.radius()+t.radius());
}
