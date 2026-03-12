#pragma once

#include "shape.h"
#include <iostream>

using namespace std;

bool collide(const Shape& s,const Shape& t);
bool collide(const Circle& s,const Shape& t);
bool collide(const Shape& s,const Circle& t);
bool collide(const Circle& s,const Circle& t);

template<typename S,typename L>
bool collide_any(const S& s,const L& l)
{
	cout << __PRETTY_FUNCTION__ << endl;

	bool res=false;
	for (const auto& t : l)
		res |= collide(s,*t);

	return res;
}
