#pragma once

#ifndef Welzl_h__
#define Welzl_h__

#include "Gmath.h"
#include <vector>
#include <list>

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

  Circle Process(const std::vector<Point> &points);

private:

  std::vector<Point> mPoints;

  std::list<unsigned int> mPset;

  std::vector<unsigned int> mDisk;

  Circle mCircle;

  static unsigned int count;

private:
  bool IsInsideCircle(unsigned int index);

  void RProcess(std::list<unsigned int>::iterator last);

  Circle CreateCircle3();

  Circle CreateCircle2();

};


#endif // Welzl_h__
