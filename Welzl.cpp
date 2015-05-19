#include "Welzl.h"

#include <list>
#include <assert.h>


Circle Welzl::Process(Image *im, const std::vector<Point> &points)
{
  image = im;
  mPoints = points;

  for(unsigned int i = 0; i < points.size(); ++i)
  {
    mPset.push_back(i);
  }

  return RProcess();
}

Circle Welzl::RProcess()
{
  Circle circle;

  if(mPset.empty() || mDisk.size() == 3)
  {
    if(mDisk.size() == 3)
    {
      circle = CreateCircle();
    }
  }
  else
  {
    unsigned int point = mPset.back();
    mPset.pop_back();

    circle = RProcess();

    if(!IsInsideCircle(circle, point))
    {
      mDisk.push_back(point);
      circle = RProcess();
      mDisk.pop_back();
      mPset.push_front(point);
    }
  }

  return circle;
}

bool Welzl::IsInsideCircle(const Circle &circle, unsigned int index)
{
  if(circle.radius == 0)
  {
    return false;
  }

  const float x = mPoints[index].x;
  const float y = mPoints[index].y;

  float len = sqrt((circle.center.x - x) * (circle.center.x - x) + (circle.center.y - y) * (circle.center.y - y));

  return len < circle.radius;
}

Circle Welzl::CreateCircle()
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

  image->Fill(0xFFFFFFFF);
  image->DrawCircle(circle.center, circle.radius, 0xFF0000FF);
  for(auto it = mPoints.begin(); it != mPoints.end(); ++it)
  {
    image->DrawPoint(*it, 0x00FF00FF);
  }
  image->DrawPoint(mPoints[mDisk[0]], 0x0000FFFF);
  image->DrawPoint(mPoints[mDisk[1]], 0x0000FFFF);
  image->DrawPoint(mPoints[mDisk[2]], 0x0000FFFF);
  image->Save("img.png");

  return circle;
}
