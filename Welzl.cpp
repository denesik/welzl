#include "Welzl.h"

#include <list>
#include <assert.h>
#include <algorithm>



unsigned int Welzl::count = 0;


Circle Welzl::Process(const std::vector<Point> &points)
{
  mPoints = points;

  std::vector<unsigned int> tmp;
  for(unsigned int i = 0; i < points.size(); ++i)
  {
    tmp.push_back(i);
  }

  std::random_shuffle(tmp.begin(), tmp.end());

  for(unsigned int i = 0; i < points.size(); ++i)
  {
    mPset.push_back(tmp[i]);
  }

  RProcess(mPset.end());

  printf("count: %i\n", count);

  return mCircle;
}

void Welzl::RProcess(std::list<unsigned int>::iterator last)
{
  if(mDisk.size() == 3)
  {
    mCircle = CreateCircle3();
  }
  if(mDisk.size() == 2)
  {
    mCircle = CreateCircle2();
  }

  if(mDisk.size() == 3)
  {
    return;
  }


  for (auto it = mPset.begin(); it != last;)
  {
    unsigned int point = *it;

    ++count;

    if(!IsInsideCircle(point))
    {
      mDisk.push_back(point);
      RProcess(it);
      mDisk.pop_back();

      mPset.splice(mPset.begin(), mPset, it++);
    }
    else
    {
      ++it;
    }
  }

/*  mCircle = circle;*/
}

bool Welzl::IsInsideCircle(unsigned int index)
{
  if(mCircle.radius == 0)
  {
    return false;
  }

  const float x = mPoints[index].x;
  const float y = mPoints[index].y;

  float len = sqrt((mCircle.center.x - x) * (mCircle.center.x - x) + (mCircle.center.y - y) * (mCircle.center.y - y));

  return len < mCircle.radius;
}

Circle Welzl::CreateCircle3()
{
  assert(mDisk.size() == 3);

  Circle circle;


  const float x1 = mPoints[mDisk[0]].x;
  const float y1 = mPoints[mDisk[0]].y;
  const float x2 = mPoints[mDisk[1]].x;
  const float y2 = mPoints[mDisk[1]].y;
  const float x3 = mPoints[mDisk[2]].x;
  const float y3 = mPoints[mDisk[2]].y;

  const float xy3 = x3 * x3 + y3 * y3;
  const float xy1 = x1 * x1 + y1 * y1;
  const float xy2 = x2 * x2 + y2 * y2;

  const float zx = (y1 - y2) * (xy3) + (y2 - y3) * (xy1) + (y3 - y1) * (xy2);
  const float zy = (x1 - x2) * (xy3) + (x2 - x3) * (xy1) + (x3 - x1) * (xy2);
  const float z  = (x1 - x2) * (y3 - y1) - (y1 - y2) * (x3 - x1);

  //assert(z != 0);
  if(z == 0)
  {
    return circle;
  }

  circle.center.x = -(zx) / (2.0f * z);
  circle.center.y = (zy) / (2.0f * z);

  circle.radius = sqrt((circle.center.x - x1) * (circle.center.x - x1) + (circle.center.y - y1) * (circle.center.y - y1));

  return circle;
}

Circle Welzl::CreateCircle2()
{
  assert(mDisk.size() == 2);

  Circle circle;

  const float x1 = mPoints[mDisk[0]].x;
  const float y1 = mPoints[mDisk[0]].y;
  const float x2 = mPoints[mDisk[1]].x;
  const float y2 = mPoints[mDisk[1]].y;

  circle.radius = sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1)) / 2.0f;
  if(circle.radius)
  {
    circle.center.x = (x2 + x1) / 2.0f;
    circle.center.y = (y2 + y1) / 2.0f;
  }

  return circle;
}
