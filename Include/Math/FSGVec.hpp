#pragma once

#include "Primitives.hpp"

class CVec
{
    public:
    CVec();

    f32 x;
    f32 y;
    f32 z;
    f32 w;

    f32 Length2() const;
    f32 Length3() const;
    f32 Length4() const;

    bool              operator!=(const class CVec& vec);
    const class CVec& operator/=(const f32& factor);
    const class CVec& operator/(const f32& factor);
    const class CVec& operator*=(const f32& factor);
    const class CVec& operator*(const f32& factor);

    void Mul3(const class CMatrix& matrix);
    void Mul4(const class CMatrix& matrix);

    f32 Normalise2();
    f32 Normalise3();
    f32 Normalise4();

    f32 NormaliseToLargestComponent2();
    f32 NormaliseToLargestComponent3();
    f32 NormaliseToLargestComponent4();

    static i8 PointIntersect3Plane(class CVec& a, class CVec& b, class CVec& c, class CVec& d);
};

static const CVec& operator/(const class CVec& vec, const f32& factor);