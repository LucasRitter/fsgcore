#pragma once

#include "Primitives.hpp"

/**
 * @deprecated Use FSG_ASSERT macro instead. This is the underlying function it calls.
 * Asserts that a given @b t_truthy value is truthy, otherwise traps and logs.
 * @param t_truthy The evaluated expression.
 * @param t_function The function where this was called.
 * @param t_fileName The file name where this was called.
 * @param t_line The line where this was called.
 * @param t_expression The expression.
 * @param t_message The message when the evaluated expression is falsy.
 * @param ...
 * @note Custom implementation. Differs from original fsgcore library.
 */
void FSGAssert(
  bool t_truthy, static_string t_function, static_string t_fileName, i32 t_line, static_string t_expression, static_string t_message, ...);

#define FSG_ASSERT(EXPRESSION, MESSAGE, ...) FSGAssert(EXPRESSION, __FUNCTION__, __FILE__, __LINE__, #EXPRESSION, MESSAGE, __VA_ARGS__);