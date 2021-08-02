#include "Math/FSGAngle.hpp"
#include "Math/FSGMatrix.hpp"
#include "Math/FSGVec.hpp"

Matrix::Matrix() = default;

Matrix::Matrix(const Matrix& matrix)
{
    *this = matrix;
}

void Matrix::Invert()
{
    auto det = this->Determinant();

    if(det == 0.0f)
    {
        return;
    }

    float invDet = 1.0f / det;

    auto t00 = Determinant3x3(m[1][1], m[1][2], m[1][3], m[2][1], m[2][2], m[2][3], m[3][1], m[3][2], m[3][3]) * invDet;
    auto t10 = -Determinant3x3(m[1][0], m[1][2], m[1][3], m[2][0], m[2][2], m[2][3], m[3][0], m[3][2], m[3][3]) * invDet;
    auto t20 = Determinant3x3(m[1][0], m[1][1], m[1][3], m[2][0], m[2][1], m[2][3], m[3][0], m[3][1], m[3][3]) * invDet;
    auto t30 = -Determinant3x3(m[1][0], m[1][1], m[1][2], m[2][0], m[2][1], m[2][2], m[3][0], m[3][1], m[3][2]) * invDet;

    auto t01 = -Determinant3x3(m[0][1], m[0][2], m[0][3], m[2][1], m[2][2], m[2][3], m[3][1], m[3][2], m[3][3]) * invDet;
    auto t11 = Determinant3x3(m[0][0], m[0][2], m[0][3], m[2][0], m[2][2], m[2][3], m[3][0], m[3][2], m[3][3]) * invDet;
    auto t21 = -Determinant3x3(m[0][0], m[0][1], m[0][3], m[2][0], m[2][1], m[2][3], m[3][0], m[3][1], m[3][3]) * invDet;
    auto t31 = Determinant3x3(m[0][0], m[0][1], m[0][2], m[2][0], m[2][1], m[2][2], m[3][0], m[3][1], m[3][2]) * invDet;

    auto t02 = Determinant3x3(m[0][1], m[0][2], m[0][3], m[1][1], m[1][2], m[1][3], m[3][1], m[3][2], m[3][3]) * invDet;
    auto t12 = -Determinant3x3(m[0][0], m[0][2], m[0][3], m[1][0], m[1][2], m[1][3], m[3][0], m[3][2], m[3][3]) * invDet;
    auto t22 = Determinant3x3(m[0][0], m[0][1], m[0][3], m[1][0], m[1][1], m[1][3], m[3][0], m[3][1], m[3][3]) * invDet;
    auto t32 = -Determinant3x3(m[0][0], m[0][1], m[0][2], m[1][0], m[1][1], m[1][2], m[3][0], m[3][1], m[3][2]) * invDet;

    auto t03 = -Determinant3x3(m[0][1], m[0][2], m[0][3], m[1][1], m[1][2], m[1][3], m[2][1], m[2][2], m[2][3]) * invDet;
    auto t13 = Determinant3x3(m[0][0], m[0][2], m[0][3], m[1][0], m[1][2], m[1][3], m[2][0], m[2][2], m[2][3]) * invDet;
    auto t23 = -Determinant3x3(m[0][0], m[0][1], m[0][3], m[1][0], m[1][1], m[1][3], m[2][0], m[2][1], m[2][3]) * invDet;
    auto t33 = Determinant3x3(m[0][0], m[0][1], m[0][2], m[1][0], m[1][1], m[1][2], m[2][0], m[2][1], m[2][2]) * invDet;

    m[0][0] = t00;
    m[0][1] = t01;
    m[0][2] = t02;
    m[0][3] = t03;

    m[1][0] = t10;
    m[1][1] = t11;
    m[1][2] = t12;
    m[1][3] = t13;

    m[2][0] = t20;
    m[2][1] = t21;
    m[2][2] = t22;
    m[2][3] = t23;

    m[3][0] = t30;
    m[3][1] = t31;
    m[3][2] = t32;
    m[3][3] = t33;
}

void Matrix::SetRotation(f32 x, f32 y, f32 z)
{
    f32 xSin, xCos;
    f32 ySin, yCos;
    f32 zSin, zCos;

    FSGCosSin(xCos, xSin, x);
    FSGCosSin(yCos, ySin, y);
    FSGCosSin(zCos, zSin, z);

    this->m[0][3] = 0.0f;
    this->m[1][3] = 0.0f;
    this->m[2][3] = 0.0f;

    this->m[0][0] = zCos * yCos;
    this->m[0][1] = zSin * yCos;
    this->m[0][2] = -ySin;

    this->m[1][0] = (ySin * xSin * zCos) - (zSin * xCos);
    this->m[1][1] = (ySin * xSin * zSin) + (zCos * xCos);
    this->m[1][2] = yCos * xSin;

    this->m[2][0] = (ySin * xCos * zCos) + zSin * xSin;
    this->m[2][1] = (ySin * xCos * zSin) - zCos * xSin;
    this->m[2][2] = yCos * xCos;
}

void Matrix::SetLookAt(const class Vec& pos, const class Vec& target, const class Vec& up)
{
    // Custom implementation.
      auto zAxis = (target - pos);
      zAxis.Normalise3();

      auto xAxis = up.Cross3(zAxis);
      xAxis.Normalise3();

      auto yAxis = zAxis.Cross3(xAxis);

      this->m[0][0] = xAxis.x;
      this->m[0][1] = yAxis.x;
      this->m[0][2] = zAxis.x;
      this->m[0][3] = 0.0f;

      this->m[1][0] = xAxis.y;
      this->m[1][1] = yAxis.y;
      this->m[1][2] = zAxis.y;
      this->m[1][3] = 0.0f;

      this->m[2][0] = xAxis.z;
      this->m[2][1] = yAxis.z;
      this->m[2][2] = zAxis.z;
      this->m[2][3] = 0.0f;

      this->m[3][0] = -xAxis.Dot(pos);
      this->m[3][1] = -yAxis.Dot(pos);
      this->m[3][2] = -zAxis.Dot(pos);
      this->m[3][3] = 1.0f;
}

void Matrix::SetXRotation(f32 rot)
{
    f32 xCos, xSin;
    FSGCosSin(xCos, xSin, rot);

    this->m[0][0] = 1.0f;
    this->m[0][1] = 0.0f;
    this->m[0][2] = 0.0f;
    this->m[0][3] = 0.0f;

    this->m[1][0] = 0.0f;
    this->m[1][1] = xCos;
    this->m[1][2] = xSin;
    this->m[1][3] = 0.0f;

    this->m[2][0] = 0.0f;
    this->m[2][1] = -xSin;
    this->m[2][2] = xCos;
    this->m[2][3] = 0.0f;
}

void Matrix::SetYRotation(f32 rot)
{
    f32 yCos, ySin;
    FSGCosSin(yCos, ySin, rot);

    this->m[0][0] = yCos;
    this->m[0][1] = 0.0f;
    this->m[0][2] = -ySin;
    this->m[0][3] = 0.0f;

    this->m[1][0] = 0.0f;
    this->m[1][1] = 1.0f;
    this->m[1][2] = 0.0f;
    this->m[1][3] = 0.0f;

    this->m[2][0] = ySin;
    this->m[2][1] = 0.0f;
    this->m[2][2] = yCos;
    this->m[2][3] = 0.0f;
}

void Matrix::SetZRotation(f32 rot)
{
    f32 zCos, zSin;
    FSGCosSin(zCos, zSin, rot);

    this->m[0][0] = zCos;
    this->m[0][1] = zSin;
    this->m[0][2] = 0.0f;
    this->m[0][3] = 0.0f;

    this->m[1][0] = -zSin;
    this->m[1][1] = zCos;
    this->m[1][2] = 0.0f;
    this->m[1][3] = 0.0f;

    this->m[2][0] = 0.0f;
    this->m[2][1] = 0.0f;
    this->m[2][2] = 1.0f;
    this->m[2][3] = 0.0f;
}

void Matrix::Mul33(const class Matrix& b)
{
    auto a00 = this->m[0][0];
    auto a01 = this->m[0][1];
    auto a02 = this->m[0][2];
    auto a10 = this->m[1][0];
    auto a11 = this->m[1][1];
    auto a12 = this->m[1][2];
    auto a20 = this->m[2][0];
    auto a21 = this->m[2][1];
    auto a22 = this->m[2][2];

    auto b00 = b.m[0][0];
    auto b01 = b.m[0][1];
    auto b02 = b.m[0][2];
    auto b10 = b.m[1][0];
    auto b11 = b.m[1][1];
    auto b12 = b.m[1][2];
    auto b20 = b.m[2][0];
    auto b21 = b.m[2][1];
    auto b22 = b.m[2][2];

    this->m[0][0] = b00 * a00 + b01 * a10 + b02 * a20;
    this->m[0][1] = b00 * a01 + b01 * a11 + b02 * a21;
    this->m[0][2] = b00 * a02 + b01 * a12 + b02 * a22;

    this->m[1][0] = b10 * a00 + b11 * a10 + b12 * a20;
    this->m[1][1] = b10 * a01 + b11 * a11 + b12 * a21;
    this->m[1][2] = b10 * a02 + b11 * a12 + b12 * a22;

    this->m[2][0] = b20 * a00 + b21 * a10 + b22 * a20;
    this->m[2][1] = b20 * a01 + b21 * a11 + b22 * a21;
    this->m[2][2] = b20 * a02 + b21 * a12 + b22 * a22;

    this->m[0][3] = 0.0f;
    this->m[1][3] = 0.0f;
    this->m[2][3] = 0.0f;
}

void Matrix::Mul44(const class Matrix& b)
{
    auto a00 = this->m[0][0];
    auto a01 = this->m[0][1];
    auto a02 = this->m[0][2];
    auto a03 = this->m[0][3];
    auto a10 = this->m[1][0];
    auto a11 = this->m[1][1];
    auto a12 = this->m[1][2];
    auto a13 = this->m[1][3];
    auto a20 = this->m[2][0];
    auto a21 = this->m[2][1];
    auto a22 = this->m[2][2];
    auto a23 = this->m[2][3];
    auto a30 = this->m[3][0];
    auto a31 = this->m[3][1];
    auto a32 = this->m[3][2];
    auto a33 = this->m[3][3];

    auto b0 = b.m[0][0];
    auto b1 = b.m[0][1];
    auto b2 = b.m[0][2];
    auto b3 = b.m[0][3];

    this->m[0][0] = b0 * a00 + b1 * a10 + b2 * a20 + b3 * a30;
    this->m[0][1] = b0 * a01 + b1 * a11 + b2 * a21 + b3 * a31;
    this->m[0][2] = b0 * a02 + b1 * a12 + b2 * a22 + b3 * a32;
    this->m[0][3] = b0 * a03 + b1 * a13 + b2 * a23 + b3 * a33;

    b0 = b.m[1][0];
    b1 = b.m[1][1];
    b2 = b.m[1][2];
    b3 = b.m[1][3];

    this->m[1][0] = b0 * a00 + b1 * a10 + b2 * a20 + b3 * a30;
    this->m[1][1] = b0 * a01 + b1 * a11 + b2 * a21 + b3 * a31;
    this->m[1][2] = b0 * a02 + b1 * a12 + b2 * a22 + b3 * a32;
    this->m[1][3] = b0 * a03 + b1 * a13 + b2 * a23 + b3 * a33;

    b0 = b.m[2][0];
    b1 = b.m[2][1];
    b2 = b.m[2][2];
    b3 = b.m[2][3];

    this->m[2][0] = b0 * a00 + b1 * a10 + b2 * a20 + b3 * a30;
    this->m[2][1] = b0 * a01 + b1 * a11 + b2 * a21 + b3 * a31;
    this->m[2][2] = b0 * a02 + b1 * a12 + b2 * a22 + b3 * a32;
    this->m[2][3] = b0 * a03 + b1 * a13 + b2 * a23 + b3 * a33;

    b0 = b.m[3][0];
    b1 = b.m[3][1];
    b2 = b.m[3][2];
    b3 = b.m[3][3];

    this->m[3][0] = b0 * a00 + b1 * a10 + b2 * a20 + b3 * a30;
    this->m[3][1] = b0 * a01 + b1 * a11 + b2 * a21 + b3 * a31;
    this->m[3][2] = b0 * a02 + b1 * a12 + b2 * a22 + b3 * a32;
    this->m[3][3] = b0 * a03 + b1 * a13 + b2 * a23 + b3 * a33;
}

void Matrix::Transpose44() {
    auto t01 = this->m[0][1];
    auto t02 = this->m[0][2];
    auto t03 = this->m[0][3];
    auto t12 = this->m[1][2];
    auto t13 = this->m[1][3];
    auto t23 = this->m[2][3];

    this->m[0][1] = this->m[1][0];
    this->m[0][2] = this->m[2][0];
    this->m[0][3] = this->m[3][0];
    this->m[1][0] = t01;
    this->m[1][2] = this->m[2][1];
    this->m[1][3] = this->m[3][1];
    this->m[2][0] = t02;
    this->m[2][1] = t12;
    this->m[2][3] = this->m[3][2];
    this->m[3][0] = t03;
    this->m[3][1] = t13;
    this->m[3][2] = t23;
}

class Matrix& Matrix::operator*=(const class Matrix& a)
{
    this->Mul44(a);
    return *this;
}

f32 Matrix::Determinant()
{
    auto temp1 = (this->m[2][2] * this->m[3][3]) - (this->m[2][3] * this->m[3][2]);
    auto temp2 = (this->m[2][1] * this->m[3][3]) - (this->m[2][3] * this->m[3][1]);
    auto temp3 = (this->m[2][1] * this->m[3][2]) - (this->m[2][2] * this->m[3][1]);
    auto temp4 = (this->m[2][0] * this->m[3][3]) - (this->m[2][3] * this->m[3][0]);
    auto temp5 = (this->m[2][0] * this->m[3][2]) - (this->m[2][2] * this->m[3][0]);
    auto temp6 = (this->m[2][0] * this->m[3][1]) - (this->m[2][1] * this->m[3][0]);

    return ((((m[0][0] * (((m[1][1] * temp1) - (m[1][2] * temp2)) + (m[1][3] * temp3))) -
              (m[0][1] * (((m[1][0] * temp1) - (m[1][2] * temp4)) + (m[1][3] * temp5)))) +
             (m[0][2] * (((m[1][0] * temp2) - (m[1][1] * temp4)) + (m[1][3] * temp6)))) -
            (m[0][3] * (((m[1][0] * temp3) - (m[1][1] * temp5)) + (m[1][2] * temp6))));
}

f32 Matrix::Determinant3x3(f32 m00, f32 m01, f32 m02, f32 m10, f32 m11, f32 m12, f32 m20, f32 m21, f32 m22)
{
    return m00 * (m11 * m22 - m12 * m21) - m01 * (m10 * m22 - m12 * m20) +
           m02 * (m10 * m21 - m11 * m20);
}