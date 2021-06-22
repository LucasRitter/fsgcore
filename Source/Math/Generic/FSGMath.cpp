#include <Math/FSGMath.hpp>

f32 FSGSqrt(const f32& t_x)
{
    return sqrt(static_cast<f64>(t_x));
}

f32 FSGCeil(const f32& t_x)
{
    return ceil(t_x);
}

f32 FSGFloor(const f32& t_x)
{
    return floor(t_x);
}
