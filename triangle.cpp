#include "triangle.h"

Triangle::Triangle(double _x0, double _y0, double _x1, double _y1, double _x2, double _y2)
{
  x0 = _x0;
  y0 = _y0;
  if ((_x1 - _x0)*(_y1 + _y0) + (_x2-_x1)*(_y2+_y1) + (_x0-_x2)*(_y0+_y2) < 0)
  {
    x1 = _x1;
    x2 = _x2;
    y1 = _y1;
    y2 = _y2;
  } else {
    x1 = _x2;
    x2 = _x1;
    y1 = _y2;
    y2 = _y1;
  }

  // Calculate line test constants
  a1 = y1 - y0;
  b1 = x0 - x1;
  c1 = -y0*b1 - x0*a1;

  a2 = y2 - y1;
  b2 = x1 - x2;
  c2 = -y1*b2 - x1*a2;

  a3 = y0 - y2;
  b3 = x2 - x0;
  c3 = -y2*b3 - x2*a3;

  // Extent
  minx = std::min(x0, std::min(x1, x2));
  miny = std::min(y0, std::min(y1, y2));
  maxx = std::max(x0, std::max(x1, x2));
  maxy = std::max(y0, std::max(y1, y2));
}

bool Triangle::in_bounds(double x, double y) const
{
  return ((x*a1+y*b1+c1 <= 0) && (x*a2+y*b2+c2 <= 0) && (x*a3+y*b3+c3 <= 0));
}

void Triangle::random_point(double& x, double& y) const
{
  for (;;)
  {
    double _x = (rand() % (int)(maxx-minx))+minx;
    double _y = (rand() % (int)(maxy-miny))+miny;
    if (in_bounds(_x, _y))
    {
      x = _x;
      y = _y;
      break;
    }
  }
}

void Triangle::random_vertex(double& x, double& y) const
{
  switch (rand()%3)
  {
    case 0:
    {
      x = x0;
      y = y0;
      break;
    }
  
    case 1:
    {
      x = x1;
      y = y1;
      break;
    }
  
    case 2:
    {
      x = x2;
      y = y2;
      break;
    }
  }
}

double Triangle::getX0() const
{
  return x0;
}

double Triangle::getX1() const
{
  return x1;
}

double Triangle::getX2() const
{
  return x2;
}

double Triangle::getY0() const
{
  return y0;
}

double Triangle::getY1() const
{
  return y1;
}

double Triangle::getY2() const
{
  return y2;
}

Matrix3x3 affineTransform(Triangle from, Triangle to)
{
  Matrix3x3 A {{from.getX0(), from.getY0(), 1}, {from.getX1(), from.getY1(), 1}, {from.getX2(), from.getY2(), 1}};
  Matrix3x3 B {{to.getX0(), to.getY0(), 1}, {to.getX1(), to.getY1(), 1}, {to.getX2(), to.getY2(), 1}};
  
  return B * A.inv();
}
