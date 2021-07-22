#pragma once

#include "Primitives.hpp"

/**
 * @param x The angle in radians.
 * @return The sine of the given angle @b x.
 */
f32 FSGSin(const f32& x);

/**
 * @param x The angle in radians.
 * @return The cosine of the given angle @b x.
 */
f32 FSGCos(const f32& x);

/**
 * @param x The angle in radians.
 * @return The tangent of the given angle @b x.
 */
f32 FSGTan(const f32& x);

/**
 * @param x The angle in radians.
 * @return The arc sine of the given angle @b x.
 */
f32 FSGASin(const f32& x);

/**
 * @param x The angle in radians.
 * @return The arc cosine of the given angle @b x.
 */
f32 FSGACos(const f32& x);

/**
 * @param x The angle in radians.
 * @return The arc tangent of the given angle @b x.
 */
f32 FSGATan(const f32& x);

/**
 * @param outCos The output cosine of the given angle @b in.
 * @param outSin The output sine of the given angle @b in.
 * @param in The input angle in radians.
 * @return
 */
void FSGCosSin(f32& outCos, f32& outSin, const f32& in);

// Todo: Implement FSGATan2
f32 FSGATan2(f32, f32);