#pragma once

#include <cmath>
#include "Primitives.hpp"

/**
 * @param x Number.
 * @return The square root of @b x.
 */
f32 FSGSqrt(const f32& x);

/**
 * @param x Number.
 * @return The next integer that's equal or greater than @b x.
 */
f32 FSGCeil(const f32& x);

/**
 * @param x Number.
 * @return The next integer that's equal or less than @b x.
 */
f32 FSGFloor(const f32& x);

/**
 * @param value Number.
 * @param multiple The multiple to round up to.
 * @return The next integer that's a multiple of @b multiple.
 */
u32 FSGNextMultiple(u32 value, u32 multiple);