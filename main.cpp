#include "Welzl.h"
#include "Image.h"
#include <time.h>

// Генерация точек в ректангле.
std::vector<Point> Generate(const Point &lb, const Point &rt, unsigned int count)
{
  std::vector<Point> points;
  points.reserve(count);

  Size size(static_cast<unsigned int>(rt.x - lb.x), static_cast<unsigned int>(rt.y - lb.y));

  for(unsigned int i = 0; i < count; ++i)
  {
    points.push_back(Point(lb.x + rand() % size.width, lb.y + rand() % size.height));
  }

  return points;
}


int main()
{
  //srand(static_cast<unsigned int>(time(0)));
  //srand(static_cast<unsigned int>(9)); // global test
  srand(static_cast<unsigned int>(12));
  Size size(100, 100);

  Image image;
  image.Resize(size);
  image.Fill(0xFFFFFFFF);

  std::vector<Point> points = Generate(Point(20, 20), Point(80, 80), 7);

  Welzl w;

  printf("begin\n");

  for(auto it = points.begin(); it != points.end(); ++it)
  {
    image.DrawPoint(*it, 0x00FF00FF);
  }
  image.StateSave();

  Circle c = w.Process(&image, points);

  printf("end\n");

  image.StateLoad();
  image.DrawCircle(c.center, c.radius, 0xFF0000FF);

  image.Save("img.png");

  return 0;
}
