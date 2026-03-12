#include "shape.h"
#include <algorithm>
#include <cmath>
#include <numbers>
#include <iostream>

using namespace std;
using namespace std::numbers;

Point::Point() : x(0),  y(0)
{
}

Point::Point(float _x,float _y) : x(_x),  y(_y)
{
}

Point::Point(const initializer_list<float>& l)
{
	auto it = begin(l);
	x = *it;
	advance(it,1);
	y = *it;
}

bool Point::compare_x(const Point& a,const Point& b)
{
	return a.x<b.x;
}

bool Point::compare_y(const Point& a,const Point& b)
{
	return a.y<b.y;
}

float distance(const Point& p,const Point& q)
{
	float dx = p.x-q.x;
	float dy = p.y-q.y;

	return sqrt(dx*dx+dy*dy);
}

void Shape::bound()
{
	auto [minxp,maxxp] = minmax_element(begin(pts),end(pts),Point::compare_x);
	min.x = minxp->x;
	max.x = maxxp->x;
	auto [minyp,maxyp] = minmax_element(begin(pts),end(pts),Point::compare_y);
	min.y = minyp->y;
	max.y = maxyp->y;
}

bool Shape::contains(const Point& p) const
{
	cout << __PRETTY_FUNCTION__ << endl;

	return (p.x>=min.x) && (p.x<=max.x) && (p.y>=min.y) && (p.y<=max.y);
}

Circle::Circle(const Point& _c,float _r) : c(_c), r(_r)
{
	static constexpr int N = 8;

	float a;
	for (int i=0; i<N; i++)
	{
		a = i*2*pi/N;
		pts.emplace_back(c.x+r*cos(a),c.y+r*sin(a));
	}
	bound();
}

bool Circle::contains(const Point& p) const
{
	cout << __PRETTY_FUNCTION__ << endl;

	return distance(c,p)<=r;
}

Rectangle::Rectangle(const Point& _c,float _w,float _h) : c(_c), w(_w), h(_h)
{
	pts.emplace_back(c);
	pts.emplace_back(c.x+w,c.y  );
	pts.emplace_back(c.x+w,c.y+h);
	pts.emplace_back(c.x  ,c.y+h);
	bound();
}

Polygon::Polygon(const list<Point>& l)
{
	for (const auto& p : l)
		pts.push_back(p);
	bound();
}

Polygon::Polygon(const initializer_list<Point>& l)
{
	for (const auto& p : l)
		pts.push_back(p);
	bound();
}
