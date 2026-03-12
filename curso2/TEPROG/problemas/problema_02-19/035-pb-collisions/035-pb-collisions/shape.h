#pragma once

#include <list>
#include <initializer_list>

using namespace std;

class Point
{
public:
	float x,y;
public:
	Point();
	Point(float _x,float _y);
	Point(const initializer_list<float>& l);

	static bool compare_x(const Point& a,const Point& b);
	static bool compare_y(const Point& a,const Point& b);
};

float distance(const Point& p,const Point& q);

class Shape
{
protected:
	list<Point> pts;
	Point	min,max;

	void bound();

public:
	const list<Point>& points() const { return pts; };

	virtual bool contains(const Point& p) const;
};

class Circle : public Shape
{
private:
	Point c;
	float r;
public:
	Circle(const Point& _c,float _r);
	const Point& center() const { return c; }
	float radius() const { return r; }
	bool contains(const Point& p) const;
};

class Rectangle : public Shape
{
private:
	Point c;
	float w,h;
public:
	Rectangle(const Point& _c,float _w,float _h);
};

class Polygon : public Shape
{
private:
public:
	Polygon(const list<Point>& l);
	Polygon(const initializer_list<Point>& l);
};
