#include "Math/FSGMath.hpp"
#include "Core/FSGAssert.hpp"
#include "Math/FSGVec.hpp"

CVec::CVec()
{
    x = 0.0;
    y = 0.0;
    z = 0.0;
    w = 0.0;
}

f32 CVec::Length2() const
{
    return FSGSqrt(x * x + y * y);
}

f32 CVec::Length3() const
{
    return FSGSqrt(x * x + y * y + z * z);
}

f32 CVec::Length4() const
{
    return FSGSqrt(x * x + y * y + z * z + w * w);
}

f32 CVec::Normalise2()
{
    auto length = this->Length2();
    FSG_ASSERT(length != 0, "Passed 0 length vector for normalisation!");

    this->x *= (f32)(1.0 / length);
    this->y *= (f32)(1.0 / length);

    return length;
}

f32 CVec::Normalise3()
{
    auto length = this->Length3();
    FSG_ASSERT(length != 0, "Passed 0 length vector for normalisation!");

    this->x *= (f32)(1.0 / length);
    this->y *= (f32)(1.0 / length);
    this->z *= (f32)(1.0 / length);

    return length;
}

f32 CVec::Normalise4()
{
    auto length = this->Length4();
    FSG_ASSERT(length != 0, "Passed 0 length vector for normalisation!");

    this->x *= (f32)(1.0 / length);
    this->y *= (f32)(1.0 / length);
    this->z *= (f32)(1.0 / length);
    this->w *= (f32)(1.0 / length);

    return length;
}
