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

  Size GetSize() const;

  const std::vector<unsigned char> &Raw();

  void SetRaw(const std::vector<unsigned char> &raw);

  void Save(const std::string &fileName);

  void DrawPoint(const Point &p, unsigned int color);

  void DrawCircle(const Point &center, float radius, unsigned int color);

  void Fill(unsigned int color);

  unsigned int GetPoint(const Point &p);

  void StateSave();

  void StateLoad();

private:
  Size mSize;

  std::vector<unsigned char> mData;

  std::vector<unsigned char> mDataState;

private:

  void Set(const Point &p, unsigned int color);

};



#endif // Image_h__
