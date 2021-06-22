#include <cmath>

#include "Math/FSGAngle.hpp"

f32 FSGSin(const f32& x)
{
    return sin(static_cast<f64>(x));
}

f32 FSGCos(const f32& x)
{
    return cos(static_cast<f64>(x));
}

f32 FSGTan(const f32& x)
{
    return tan(static_cast<f64>(x));
}

f32 FSGASin(const f32& x)
{
    return asin(static_cast<f64>(x));
}

f32 FSGACos(const f32& x)
{
    return acos(static_cast<f64>(x));
}

f32 FSGATan(const f32& x)
{
    return atan(static_cast<f64>(x));
}
