#include "Welzl.h"
#include "Image.h"
#include <time.h>
#include <algorithm>

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

//   std::sort(points.begin(), points.end(), 
//     [](const Point &p1, const Point &p2) -> bool
//   {
//     if(p1.y == p2.y)
//       return p1.x > p2.x;
//     return p1.y > p2.y;
//   });
//   auto it = std::unique(points.begin(), points.end(), 
//     [](const Point &p1, const Point &p2)
//   {
//     return p1.x == p2.x && p1.y == p2.y;
//   });   
// 
//   points.resize(std::distance(points.begin(), it));

  return points;
}


int main()
{
  unsigned int seed = static_cast<unsigned int>(time(0));
  printf("seed: %i\n", seed);
  //srand(1432190824);
  //srand(static_cast<unsigned int>(9)); // global test
  //srand(static_cast<unsigned int>(13));
  Size size(300, 300);

  Image image;
  image.Resize(size);
  image.Fill(0xFFFFFFFF);

  std::vector<Point> points = Generate(Point(100, 100), Point(200, 200), 200);

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

  //system("pause");

  return 0;
}
