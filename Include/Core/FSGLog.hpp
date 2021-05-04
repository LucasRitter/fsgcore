#pragma once

#define LOGFILE_NAME_MAX_LENGTH 0x200
#define LOG_TEXT_MAX_LENGTH 0x800

#include <Primitives.hpp>

extern u32 g_logInitialised;
extern u32 g_logToConsole;
extern u32 g_logToFile;
extern char g_logFileName[LOGFILE_NAME_MAX_LENGTH];
extern u64 g_logInitTime;

void FSGLogInit(static_string t_logFileName, u32 t_logToFile, u32 t_logToConsole);
void FSGDoLog(static_string t_logText, ...);
void FSGDoLogDirect(static_string t_logText);
void FSGLogPrefix(static_string t_category, static_string t_format, string t_args);
void FSGLogDestroy();
