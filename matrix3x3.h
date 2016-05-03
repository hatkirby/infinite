#ifndef MATRIX3X3_H_AEC5146F
#define MATRIX3X3_H_AEC5146F

#include "vector3d.h"

class Matrix3x3 {
  public:
    Matrix3x3();
    
    // Initializer should take three vectors representing the columns
    Matrix3x3(std::initializer_list<Vector3D> vals);
    
    void zero(double val = 0.0);
    double det() const;
    double norm() const;
    
    static Matrix3x3 identity();
    static Matrix3x3 crossProduct(const Vector3D& u);

    Vector3D& column(int i);
    const Vector3D& column(int i) const;

    Matrix3x3 T() const;
    Matrix3x3 inv() const;

    double& operator()(int i, int j);
    const double& operator()(int i, int j) const;

    Vector3D& operator[](int i);
    const Vector3D& operator[](int i) const;

    void operator+=(const Matrix3x3& B);

    Matrix3x3 operator-() const;
    Matrix3x3 operator-(const Matrix3x3& B) const;
    Matrix3x3 operator*(double c) const;
    Matrix3x3 operator*(const Matrix3x3& B) const;
    Vector3D operator*(const Vector3D& x) const;
    void operator/=(double x);   

  protected:
    Vector3D entries[3];
};

// returns the outer product of u and v
Matrix3x3 outer(const Vector3D& u, const Vector3D& v);

// returns c*A
Matrix3x3 operator*(double c, const Matrix3x3& A);

// prints entries
std::ostream& operator<<(std::ostream& os, const Matrix3x3& A);

#endif /* end of include guard: MATRIX3X3_H_AEC5146F */
