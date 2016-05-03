#ifndef TRIANGLE_H_696E06A8
#define TRIANGLE_H_696E06A8

#include "matrix3x3.h"

class Triangle {
  public:
    Triangle(double x0, double y0, double x1, double y1, double x2, double y2);
    
    bool in_bounds(double x, double y) const;
    void random_point(double& x, double& y) const;
    void random_vertex(double& x, double& y) const;
    
    double getX0() const;
    double getX1() const;
    double getX2() const;
    double getY0() const;
    double getY1() const;
    double getY2() const;
    
  private:
    double x0, x1, x2;
    double y0, y1, y2;
    double minx, miny, maxx, maxy;
    double a1, a2, a3;
    double b1, b2, b3;
    double c1, c2, c3;
};

Matrix3x3 affineTransform(Triangle from, Triangle to);

#endif /* end of include guard: TRIANGLE_H_696E06A8 */
