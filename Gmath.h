#pragma once

#ifndef Gmath_h__
#define Gmath_h__

struct Size
{
  unsigned int width;
  unsigned int height;
  Size()
    : width(0), height(0)
  {}
  Size(unsigned int w, unsigned int h)
    : width(w), height(h)
  {}
};

struct Point
{
  float x;
  float y;
  Point()
    : x(0.0f), y(0.0f)
  {}
  Point(float _x, float _y)
    : x(_x), y(_y)
  {}
  const Point Point::operator+(const Point &p) const 
  {
    return Point(x + p.x, y + p.y);
  }
};

#endif // Gmath_h__
