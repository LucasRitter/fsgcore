#include "Math/FSGMath.hpp"

f32 FSGSqrt(const f32& x)
{
    return sqrt(static_cast<f64>(x));
}

f32 FSGCeil(const f32& x)
{
    return ceil(x);
}

f32 FSGFloor(const f32& x)
{
    return floor(x);
}
