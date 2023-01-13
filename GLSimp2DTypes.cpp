#include "GLSimp2DTypes.h"

s2d::Point::Point():x(0), y(0) {}

s2d::Point::Point(float _x, float _y): x(_x), y(_y) {}

s2d::Point::Point(Vec2f v): x(v.x), y(v.y){}

void s2d::Point::decX(float a) { x -= a; }
void s2d::Point::decY(float a) { y -= a; }
void s2d::Point::dec(float _x, float _y) { x -= _x; y -= _y; }
void s2d::Point::incX(float a) { x += a; }
void s2d::Point::incY(float a) { y += a; }
void s2d::Point::inc(float _x, float _y) { x += _x; y += _y; }


s2d::Vec2f::Vec2f(): x(0), y(0) { }

s2d::Vec2f::Vec2f(float _x, float _y) : x(_x), y(_y) {}

s2d::Vec2f::Vec2f(Point p) : x(p.x), y(p.y) {}

s2d::Point s2d::operator-(Point a, Point b) { return Point(a.x - b.x, a.y - b.y); }
