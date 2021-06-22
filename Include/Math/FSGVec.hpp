#pragma once

#include "Primitives.hpp"

class Vec
{
    public:
    Vec();

    f32 x;
    f32 y;
    f32 z;
    f32 w;

    f32 Length2() const;
    f32 Length3() const;
    f32 Length4() const;

    bool             operator!=(const class Vec& vec);
    const class Vec& operator/=(const f32& factor);
    const class Vec& operator/(const f32& factor);
    const class Vec& operator*=(const f32& factor);
    const class Vec& operator*(const f32& factor);

    void Mul3(const class CMatrix& matrix);
    void Mul4(const class CMatrix& matrix);

    f32 Normalise2();
    f32 Normalise3();
    f32 Normalise4();

    f32 NormaliseToLargestComponent2();
    f32 NormaliseToLargestComponent3();
    f32 NormaliseToLargestComponent4();

    static i8 PointIntersect3Plane(class Vec& a, class Vec& b, class Vec& c, class Vec& d);
};

static const Vec& operator/(const class Vec& vec, const f32& factor);