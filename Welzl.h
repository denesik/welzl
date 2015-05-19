#pragma once

#ifndef Welzl_h__
#define Welzl_h__

#include "Gmath.h"
#include <vector>
#include <list>
#include "Image.h"

struct Circle
{
  Point center;
  float radius;

  Circle()
    : center(), radius(0)
  {}
};

class Welzl
{
public:

  Circle Process(Image *im, const std::vector<Point> &points);

private:

  std::vector<Point> mPoints;

  std::list<unsigned int> mPset;

  std::vector<unsigned int> mDisk;

  Image *image;

private:
  bool IsInsideCircle(const Circle &circle, unsigned int index);

  Circle RProcess();

  Circle CreateCircle();

};


#endif // Welzl_h__
