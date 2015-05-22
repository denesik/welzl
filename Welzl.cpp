#include "Welzl.h"

#include <list>
#include <assert.h>
#include "gif-h/gif.h"

#define GIF_SPEED 200

Circle Welzl::Process(Image *im, const std::vector<Point> &points)
{
  image = im;
  mPoints = points;

  for(unsigned int i = 0; i < points.size(); ++i)
  {
    mPset.push_back(i);
  }

  gw = new GifWriter;

  GifBegin(static_cast<GifWriter *>(gw), "welzl.gif", image->GetSize().width, image->GetSize().height, 500);
  GifWriteFrame(static_cast<GifWriter *>(gw), &image->Raw()[0], image->GetSize().width, image->GetSize().height, 200);

  Circle c = RProcess(mPset.end(), 0x00FF00FF);

  GifEnd(static_cast<GifWriter *>(gw));

  delete static_cast<GifWriter *>(gw);

  return c;
}

#define DRAW_CIRCLE \
  image->StateSave(); \
  image->DrawCircle(circle.center, circle.radius, 0xFF0000FF); \
  GifWriteFrame(static_cast<GifWriter *>(gw), &image->Raw()[0], image->GetSize().width, image->GetSize().height, GIF_SPEED); \
  image->StateLoad(); 

#define DRAW_CIRCLE_IF \
  if(circle.radius) \
  { \
    DRAW_CIRCLE \
  } \
  else \
  { \
    GifWriteFrame(static_cast<GifWriter *>(gw), &image->Raw()[0], image->GetSize().width, image->GetSize().height, GIF_SPEED); \
  }

Circle Welzl::RProcess()
{
  Circle circle;

  if(mPset.empty() || mDisk.size() == 3)
  {
    if(mDisk.size() == 3)
    {
      circle = CreateCircle3();
    }
    if(mDisk.size() == 2)
    {
      circle = CreateCircle2();
    }
    if(circle.radius)
    {
      DRAW_CIRCLE
    }
  }
  else
  {
    unsigned int point = mPset.back();
    mPset.pop_back();

    unsigned int ic = image->GetPoint(mPoints[point]);
    image->DrawPoint(mPoints[point], 0x000000FF);
    DRAW_CIRCLE_IF

    circle = RProcess();

    image->DrawPoint(mPoints[point], 0x0000FFFF);
    DRAW_CIRCLE_IF

    if(!IsInsideCircle(circle, point))
    {
      unsigned int ic2 = image->GetPoint(mPoints[point]);
      image->DrawPoint(mPoints[point], 0xFF00FFFF);
      DRAW_CIRCLE_IF

      mDisk.push_back(point);
      circle = RProcess();
      mDisk.pop_back();
      mPset.push_front(point);

      image->DrawPoint(mPoints[point], ic);
      DRAW_CIRCLE_IF
    }
    else
    {
      mPset.push_back(point);
    }

    image->DrawPoint(mPoints[point], ic);
    DRAW_CIRCLE_IF
  }

  return circle;
}

// unsigned int RAND_COLOR()
// {
//   unsigned int color = 0;
//   color |= ((0x000000FF & (0xFF)) << 0);
//   color |= ((0x000000FF & (64 + (rand() % 192))) << 8);
//   color |= ((0x000000FF & (64 + (rand() % 192))) << 16);
//   color |= ((0x000000FF & (64 + (rand() % 192))) << 24);
//   return color;
// }

unsigned int RAND_COLOR()
{
  static int i = 0;

  static unsigned int colors[] = 
  {
    0x85DBEFFF,
    0xD7C833FF,
    0x85EFA0FF,
    0x6495EDFF,
    0xDA70D6FF,
    0xDB7093FF,
    0xDAD871FF,
    0x328910FF,
    0x30D5C8FF,
    0xDE3163FF,
    0x3E987FF,
    0x3E8D98FF,
    0x3E5D98FF,
    0x8C3E98FF,
    0x985E3EFF,
    0x98A531FF,
    0x43A531FF,
    0x0000FFFF,
    0x00FF00FF,
    0xFF00FFFF,
    0x00FFFFFF,
    0xFFFF00FF,
  };

  assert(i < 22);

  return colors[i++];
}

unsigned int clamp(unsigned int color, unsigned int m)
{
  return color < m ? 0 : color - m;
}

unsigned int INC_COLOR(unsigned int color)
{
  unsigned int c = 0;
  c |= ((0x000000FF & (0xFF)) << 0);
  c |= ((0x000000FF & clamp((color >> 8) & 0x000000FF, 128)) << 8);
  c |= ((0x000000FF & clamp((color >> 16) & 0x000000FF, 128)) << 16);
  c |= ((0x000000FF & clamp((color >> 24) & 0x000000FF, 128)) << 24);
  return c;
}

Circle Welzl::RProcess(std::list<unsigned int>::iterator last, unsigned int lastColor)
{
  Circle circle;

  if(mDisk.size() == 3)
  {
    circle = CreateCircle3();
  }
  if(mDisk.size() == 2)
  {
    circle = CreateCircle2();
  }

  if(mDisk.size() == 3)
  {
    return circle;
  }

  unsigned int rcolor = RAND_COLOR();
  for(auto it = mPset.begin(); it != last; ++it)
  {
    image->DrawPoint(mPoints[*it], rcolor);
  }
  DRAW_CIRCLE_IF

  for (auto it = mPset.begin(); it != last;)
  {
    unsigned int point = *it;

    image->DrawPoint(mPoints[point], INC_COLOR(rcolor));
    DRAW_CIRCLE_IF

    if(!IsInsideCircle(circle, point))
    {
//      unsigned int ic = image->GetPoint(mPoints[point]);
      image->DrawPoint(mPoints[point], 0x000000FF);
      DRAW_CIRCLE_IF
      
      mDisk.push_back(point);
      circle = RProcess(it, INC_COLOR(rcolor));
      mDisk.pop_back();

      image->DrawPoint(mPoints[point], INC_COLOR(rcolor));
      DRAW_CIRCLE_IF

      mPset.splice(mPset.begin(), mPset, it++);
    }
    else
    {
      ++it;
    }

//     image->DrawPoint(mPoints[point], rcolor);
//     DRAW_CIRCLE_IF
  }

  for(auto it = mPset.begin(); it != last; ++it)
  {
    image->DrawPoint(mPoints[*it], lastColor);
  }
  DRAW_CIRCLE_IF

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
