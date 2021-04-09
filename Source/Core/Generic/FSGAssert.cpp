#include "Core/FSGAssert.hpp"

#include <cstdio>
#include <cstdarg>
#include <cstdlib>

void FSGAssert(bool t_truthy, static_string t_function, static_string t_fileName, i32 t_line, static_string t_expression, static_string t_message, ...) {
    if (t_truthy) {
        return;
    }

    printf_s("ERROR:\n  %s\n  %s\n  %s:%d\n  ", t_expression, t_function, t_fileName, t_line);

    if (t_message == nullptr) {
        return;
    }

    va_list args;
    va_start(args, t_message);
    vprintf_s(t_message, args);
    va_end(args);

    printf("\n");

    std::abort();
}