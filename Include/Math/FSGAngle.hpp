#pragma once
#include "Primitives.hpp"

/**
 * @param t_x The angle in radians.
 * @return The sine of the given angle @b t_x.
 */
f32 FSGSin(const f32 &t_x);

/**
 * @param t_x The angle in radians.
 * @return The cosine of the given angle @b t_x.
 */
f32 FSGCos(const f32 &t_x);

/**
 * @param t_x The angle in radians.
 * @return The tangent of the given angle @b t_x.
 */
f32 FSGTan(const f32 &t_x);



/**
 * @param t_x The angle in radians.
 * @return The arc sine of the given angle @b t_x.
 */
f32 FSGASin(const f32 &t_x);

/**
 * @param t_x The angle in radians.
 * @return The arc cosine of the given angle @b t_x.
 */
f32 FSGACos(const f32 &t_x);

/**
 * @param t_x The angle in radians.
 * @return The arc tangent of the given angle @b t_x.
 */
f32 FSGATan(const f32 &t_x);



// Todo: Implement FSGCosSin
f32 FSGCosSin(f32 &, f32 &, const f32 &);

// Todo: Implement FSGATan2
f32 FSGATan2(f32, f32);