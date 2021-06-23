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

u32 FSGNextMultiple(u32 value, u32 multiple) {
    if (multiple == 0)
        return value;

    auto remainder = value % multiple;
    if (remainder == 0)
        return value;

    return value + multiple - remainder;
}