#pragma once

#include "Primitives.hpp"

class Matrix
{
    public:
    f32 m[4][4]{};

    Matrix();
    Matrix(const Matrix &);

    void Invert();

    void SetRotation(f32 x, f32 y, f32 z);
    void SetLookAt(class Vec const& pos, class Vec const& target, class Vec const& up);
    // Todo: Implement SetFromLookDirection
    void SetFromLookDirection(class Vec const& a, class Vec const& b, class Vec const& c);

    void SetXRotation(f32 rot);
    void SetYRotation(f32 rot);
    void SetZRotation(f32 rot);

    void Mul33(class Matrix const& b);
    void Mul44(class Matrix const& b);
    void Transpose44();

    // Todo: Implement GetPositionInWorldSpace
    class CVec GetPositionInWorldSpace() const;

    class Matrix& operator*=(class Matrix const& a);
    // Todo: Implement operator*=(f32)
    class Matrix& operator*=(f32 const& a);

    private:
    f32 Determinant();
    static f32 Determinant3x3(f32 m00, f32 m01, f32 m02, f32 m10, f32 m11, f32 m12, f32 m20, f32 m21, f32 m22);
};