#pragma once

#include "Primitives.hpp"

#define LOGFILE_NAME_MAX_LENGTH 0x200
#define LOG_TEXT_MAX_LENGTH     0x800

extern u32       isInitialised;
extern u32       toConsole;
extern u32       toFile;
extern character fileName[LOGFILE_NAME_MAX_LENGTH];
extern u64       initTime;

void FSGLogInit(static_string logFileName, u32 logToFile, u32 logToConsole);
void FSGDoLog(static_string logText, ...);
void FSGDoLogDirect(static_string logText);
void FSGLogPrefix(static_string category, static_string format, string args);
void FSGLogDestroy();
