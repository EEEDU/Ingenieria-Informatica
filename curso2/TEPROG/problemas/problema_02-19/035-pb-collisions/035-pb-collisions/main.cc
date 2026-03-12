#include "shape.h"
#include "collide.h"
#include <iostream>

using namespace std;

int main()
{
	Circle c1({0,0},1);
	Circle c2({2,2},2);

	cout << boolalpha;

	cout << collide(c1,c2) << endl;

	list<Shape*> ls;
	ls.push_back(new Rectangle({1,1},1,1));
	ls.push_back(new Circle({3,3},2));

	cout << collide_any(c1,ls) << endl;

	list<Circle*> lc;
	lc.push_back(&c2);
	lc.push_back(new Circle({3,3},2));

	cout << collide_any(c1,lc) << endl;

	Rectangle r1({0,0},1,1);

	cout << collide_any(r1,lc) << endl;

	return 0;
}
