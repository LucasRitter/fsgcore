#include <cstdio>
#include <cstdarg>
#include <cstdlib>

#include "Core/FSGAssert.hpp"

void FSGAssert(bool truthy, static_string function, static_string fileName, i32 line, static_string expression, static_string message, ...)
{
    if(truthy) return;

    printf_s("ERROR:\n  %s\n  %s\n  %s:%d\n  ", expression, function, fileName, line);

    if(message == nullptr) std::abort();

    va_list args;
    va_start(args, message);

    auto buffer = reinterpret_cast<string>(malloc(0x800));
    vsprintf_s(buffer, 0x800, message, args);

    // Temporary fix for an issue with stdout buffering and CLion on Windows.
    setbuf(stdout, nullptr);
    printf("%s\n", buffer);

    va_end(args);

    std::abort();
}