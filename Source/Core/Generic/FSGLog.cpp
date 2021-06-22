#include <cstdio>

#include "Core/FSGLog.hpp"

void FSGLogPrefix(static_string category, static_string format, string args)
{
    character logText[LOG_TEXT_MAX_LENGTH];
    vsprintf_s(logText, LOG_TEXT_MAX_LENGTH, format, args);
    FSGDoLog("[%s] %s", category, logText);
}

void FSGLogDestroy()
{
    isInitialised = false;
}

void FSGDoLogDirect(static_string logText)
{
    FSGDoLog(logText);
}
