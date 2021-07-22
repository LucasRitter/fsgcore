#pragma once

#include "Primitives.hpp"

class Vec
{
    public:
    Vec();
    Vec(const class Vec& vec);

    f32 x;
    f32 y;
    f32 z;
    f32 w;

    f32 Length2() const;
    f32 Length3() const;
    f32 Length4() const;

    bool             operator!=(const class Vec& vec) const;
    class Vec& operator/=(const f32& factor);
    class Vec& operator/(const f32& factor) const;
    class Vec& operator*=(const f32& factor);
    class Vec& operator*(const f32& factor) const;

    class Vec& operator+=(const class Vec& vec);
    class Vec& operator+(const class Vec& vec) const;
    class Vec& operator-=(const class Vec& vec);
    class Vec& operator-(const class Vec& vec) const;

    class Vec operator*(class Matrix const & mat);

    void Mul3(const class Matrix& matrix);
    void Mul4(const class Matrix& matrix);

    f32 Normalise2();
    f32 Normalise3();
    f32 Normalise4();

    f32 NormaliseToLargestComponent2();
    f32 NormaliseToLargestComponent3();
    f32 NormaliseToLargestComponent4();

    [[nodiscard]] f32              Dot(const class Vec& vec) const;
    [[nodiscard]] const class Vec& Cross3(const class Vec& vec) const;

    static i8 PointIntersect3Plane(class Vec& plane0, class Vec& plane1, class Vec& plane2, class Vec& point);

    private:
    [[nodiscard]] f32 FindLargestComponent() const;
};