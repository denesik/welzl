#include "Welzl.h"

#include <list>
#include <assert.h>
#include "gif-h/gif.h"

#define GIF_SPEED 5

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

  Circle c = RProcess();

  GifEnd(static_cast<GifWriter *>(gw));

  delete static_cast<GifWriter *>(gw);

  return c;
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
      image->StateSave();
      image->DrawCircle(circle.center, circle.radius, 0xFF0000FF);
      GifWriteFrame(static_cast<GifWriter *>(gw), &image->Raw()[0], image->GetSize().width, image->GetSize().height, GIF_SPEED);
      image->Save("img.png");
      image->StateLoad();
    }
  }
  else
  {
    unsigned int point = mPset.back();
    mPset.pop_back();

    unsigned int ic = image->GetPoint(mPoints[point]);
    image->DrawPoint(mPoints[point], 0x000000FF);
    if(circle.radius)
    {
      image->StateSave();
      image->DrawCircle(circle.center, circle.radius, 0xFF0000FF);
      GifWriteFrame(static_cast<GifWriter *>(gw), &image->Raw()[0], image->GetSize().width, image->GetSize().height, GIF_SPEED);
      image->StateLoad();
    }
    else
    {
      GifWriteFrame(static_cast<GifWriter *>(gw), &image->Raw()[0], image->GetSize().width, image->GetSize().height, GIF_SPEED);
    }

    circle = RProcess();

    image->DrawPoint(mPoints[point], 0x0000FFFF);
    if(circle.radius)
    {
      image->StateSave();
      image->DrawCircle(circle.center, circle.radius, 0xFF0000FF);
      GifWriteFrame(static_cast<GifWriter *>(gw), &image->Raw()[0], image->GetSize().width, image->GetSize().height, GIF_SPEED);
      image->StateLoad();
    }
    else
    {
      GifWriteFrame(static_cast<GifWriter *>(gw), &image->Raw()[0], image->GetSize().width, image->GetSize().height, GIF_SPEED);
    }

//     image->DrawPoint(mPoints[point], 0x000000FF);
//     if(circle.radius)
//     {
//       image->StateSave();
//       image->DrawCircle(circle.center, circle.radius, 0xFF0000FF);
//       GifWriteFrame(static_cast<GifWriter *>(gw), &image->Raw()[0], image->GetSize().width, image->GetSize().height, 0);
//       image->StateLoad();
//     }
//     else
//     {
//       GifWriteFrame(static_cast<GifWriter *>(gw), &image->Raw()[0], image->GetSize().width, image->GetSize().height, 0);
//     }

    if(!IsInsideCircle(circle, point))
    {
      unsigned int ic2 = image->GetPoint(mPoints[point]);
      image->DrawPoint(mPoints[point], 0xFF00FFFF);
      if(circle.radius)
      {
        image->StateSave();
        image->DrawCircle(circle.center, circle.radius, 0xFF0000FF);
        GifWriteFrame(static_cast<GifWriter *>(gw), &image->Raw()[0], image->GetSize().width, image->GetSize().height, GIF_SPEED);
        image->StateLoad();
      }
      else
      {
        GifWriteFrame(static_cast<GifWriter *>(gw), &image->Raw()[0], image->GetSize().width, image->GetSize().height, GIF_SPEED);
      }

      mDisk.push_back(point);
      circle = RProcess();
      mDisk.pop_back();
      mPset.push_front(point);

      image->DrawPoint(mPoints[point], ic);
      if(circle.radius)
      {
        image->StateSave();
        image->DrawCircle(circle.center, circle.radius, 0xFF0000FF);
        GifWriteFrame(static_cast<GifWriter *>(gw), &image->Raw()[0], image->GetSize().width, image->GetSize().height, GIF_SPEED);
        image->StateLoad();
      }
      else
      {
        GifWriteFrame(static_cast<GifWriter *>(gw), &image->Raw()[0], image->GetSize().width, image->GetSize().height, GIF_SPEED);
      }
    }
    else
    {
      mPset.push_back(point);
    }

    image->DrawPoint(mPoints[point], ic);
    if(circle.radius)
    {
      image->StateSave();
      image->DrawCircle(circle.center, circle.radius, 0xFF0000FF);
      GifWriteFrame(static_cast<GifWriter *>(gw), &image->Raw()[0], image->GetSize().width, image->GetSize().height, GIF_SPEED);
      image->StateLoad();
    }
    else
    {
      GifWriteFrame(static_cast<GifWriter *>(gw), &image->Raw()[0], image->GetSize().width, image->GetSize().height, GIF_SPEED);
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
