#include <cmath>
#include "Math/FSGAngle.hpp"

f32 FSGSin(const f32 &t_x) {
    return sin(static_cast<f64>(t_x));
}

f32 FSGCos(const f32 &t_x) {
    return cos(static_cast<f64>(t_x));
}

f32 FSGTan(const f32 &t_x) {
    return tan(static_cast<f64>(t_x));
}


f32 FSGASin(const f32 &t_x) {
    return asin(static_cast<f64>(t_x));
}

f32 FSGACos(const f32 &t_x) {
    return acos(static_cast<f64>(t_x));
}

f32 FSGATan(const f32 &t_x) {
    return atan(static_cast<f64>(t_x));
}
