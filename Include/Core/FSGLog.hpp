#pragma once

#include "Primitives.hpp"

#define LOGFILE_NAME_MAX_LENGTH 0x200
#define LOG_TEXT_MAX_LENGTH     0x800

extern u32  gLogInitialised;
extern u32  gLogToConsole;
extern u32  gLogToFile;
extern char gLogFileName[LOGFILE_NAME_MAX_LENGTH];
extern u64  gLogInitTime;

void FSGLogInit(static_string logFileName, u32 logToFile, u32 logToConsole);
void FSGDoLog(static_string logText, ...);
void FSGDoLogDirect(static_string logText);
void FSGLogPrefix(static_string category, static_string format, string args);
void FSGLogDestroy();
