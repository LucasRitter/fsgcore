#pragma once

#include "Primitives.hpp"

/**
 * @deprecated Use FSG_ASSERT macro instead. This is the underlying function it calls.
 * Asserts that a given @b truthy value is truthy, otherwise traps and logs.
 * @param truthy The evaluated expression.
 * @param function The function where this was called.
 * @param fileName The file name where this was called.
 * @param line The line where this was called.
 * @param expression The expression.
 * @param message The message when the evaluated expression is falsy.
 * @param ...
 * @note Custom implementation. Differs from original fsgcore library.
 */
void FSGAssert(
  bool truthy, static_string function, static_string fileName, i32 line, static_string expression, static_string message, ...);

#define FSG_ASSERT(EXPRESSION, MESSAGE, ...) FSGAssert(EXPRESSION, __FUNCTION__, __FILE__, __LINE__, #EXPRESSION, MESSAGE, __VA_ARGS__);