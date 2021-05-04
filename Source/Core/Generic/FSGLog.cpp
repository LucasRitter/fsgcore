#include <cstdio>
#include <Core/FSGLog.hpp>

void FSGLogPrefix(static_string t_category, static_string t_format, string t_args) {
    char logText[LOG_TEXT_MAX_LENGTH];
    vsprintf_s(logText, LOG_TEXT_MAX_LENGTH, t_format, t_args);
    FSGDoLog("[%s] %s", t_category, logText);
}

void FSGLogDestroy() {
    g_logInitialised = false;
}

void FSGDoLogDirect(static_string t_logText) {
    FSGDoLog(t_logText);
}
