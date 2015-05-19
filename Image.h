#pragma once

#ifndef Image_h__
#define Image_h__

#include "Gmath.h"
#include <vector>

class Image
{
public:
  Image(void);
  ~Image(void);

  void Resize(const Size &size);

  void Save(const std::string &fileName);

  void DrawPoint(const Point &p, unsigned int color);

  void DrawCircle(const Point &center, float radius, unsigned int color);

  void Fill(unsigned int color);

private:
  Size mSize;

  std::vector<unsigned char> mData;

private:

  void Set(const Point &p, unsigned int color);

};



#endif // Image_h__
