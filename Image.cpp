#include "Image.h"

#define _USE_MATH_DEFINES
#include <math.h>

#include <assert.h>
#include "lodepng\lodepng.h"
#include <iostream>


Image::Image(void)
{
}


Image::~Image(void)
{
}

void Image::Resize(const Size &size)
{
  mSize = size;
  mData.resize(mSize.width * mSize.height * 4);
}

void Image::Set(const Point &p, unsigned int color)
{
  if(p.x < 0 || p.x > mSize.width - 1 || 
     p.y < 0 || p.y > mSize.height - 1)
  {
    return;
  }
  //assert(px < mSize.width);
  //assert(py < mSize.height);

  unsigned int px = static_cast<unsigned int>(floor(p.x + 0.5f));
  unsigned int py = static_cast<unsigned int>(floor(p.y + 0.5f));
  unsigned int ty = mSize.height - py - 1;

  mData[4 * mSize.width * ty + 4 * px + 0] = (color >> 24) & 0x000000FF;
  mData[4 * mSize.width * ty + 4 * px + 1] = (color >> 16) & 0x000000FF;
  mData[4 * mSize.width * ty + 4 * px + 2] = (color >> 8) & 0x000000FF;
  mData[4 * mSize.width * ty + 4 * px + 3] = (color >> 0) & 0x000000FF;
}

void Image::Save(const std::string &fileName)
{
  //Encode the image
  unsigned error = lodepng::encode(fileName, mData, mSize.width, mSize.height);

  //if there's an error, display it
  if(error) std::cout << "encoder error " << error << ": "<< lodepng_error_text(error) << std::endl;
}

void Image::DrawPoint(const Point &p, unsigned int color)
{
  Set(p, color);
}

void Image::DrawCircle(const Point &center, float radius, unsigned int color)
{
  float len = static_cast<float>(M_PI) * 2.0f * radius;
  float step = 360.0f / len;

  // Вектор длиной r. Будем поворачивать его на step градусов.
  Point vec(0.0f, radius);

  for(float a = 0; a < 360.0f; a +=step)
  {
    float ar = a * static_cast<float>(M_PI) / 180.0f;
    float sina = sin(ar);
    float cosa = cos(ar);

    Point p(Point(vec.x * cosa - vec.y * sina, vec.x * sina - vec.y * cosa) + center);
    unsigned int pc = GetPoint(p);
    if(pc == 0xFFFFFFFF)
    {
      pc = color;
    }
    DrawPoint(p, pc);
  }

}

void Image::Fill(unsigned int color)
{
  for(unsigned int y = 0; y < mSize.height; ++y)
    for(unsigned int x = 0; x < mSize.width; ++x)
    {
      Set(Point(static_cast<float>(x), static_cast<float>(y)), color);
    }
}

Size Image::GetSize() const
{
  return mSize;
}

const std::vector<unsigned char> & Image::Raw()
{
  return mData;
}

void Image::SetRaw(const std::vector<unsigned char> &raw)
{
  mData = raw;
}

unsigned int Image::GetPoint(const Point &p)
{
  if(p.x < 0 || p.x > mSize.width - 1 || 
    p.y < 0 || p.y > mSize.height - 1)
  {
    return 0xFFFFFFFF;
  }
  //assert(px < mSize.width);
  //assert(py < mSize.height);
  unsigned int px = static_cast<unsigned int>(floor(p.x + 0.5f));
  unsigned int py = static_cast<unsigned int>(floor(p.y + 0.5f));
  unsigned int ty = mSize.height - py - 1;

  unsigned int color = 0;
  color |= ((0x000000FF & mData[4 * mSize.width * ty + 4 * px + 3]) << 0);
  color |= ((0x000000FF & mData[4 * mSize.width * ty + 4 * px + 2]) << 8);
  color |= ((0x000000FF & mData[4 * mSize.width * ty + 4 * px + 1]) << 16);
  color |= ((0x000000FF & mData[4 * mSize.width * ty + 4 * px + 0]) << 24);
  return color;
}

void Image::StateSave()
{
  mDataState = mData;
}

void Image::StateLoad()
{
  mData = mDataState;
}

