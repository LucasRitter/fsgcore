#ifdef WIN32
    #include <cstdio>

    #include <Windows.h>

    #include "Collections/FSGString.hpp"
    #include "Core/FSGAssert.hpp"
    #include "Core/FSGLog.hpp"
    #include "File/FSGFile.hpp"

u32  gLogInitialised = false;
u32  gLogToConsole   = false;
u32  gLogToFile      = false;
char gLogFileName[LOGFILE_NAME_MAX_LENGTH];
u64  gLogInitTime = 0;

void FSGLogInit(static_string logFileName, u32 logToFile, u32 logToConsole)
{
    // Original implementation used 32bit tick count.
    gLogInitTime  = GetTickCount64();
    gLogToConsole = logToConsole;

    if(logToFile && logFileName)
    {
        FSG_ASSERT(strlen(logFileName) > 0, "Logging has no file name defined (ie logFileName = '\\0')");
        String::FormatString(gLogFileName, LOGFILE_NAME_MAX_LENGTH, "%s", logFileName);
        gLogToFile = true;
    }
    else
    {
        gLogToFile      = false;
        gLogFileName[0] = 0;
    }

    gLogInitialised = true;

    if(gLogToFile)
    {
        auto logFile = new CFile();
        if(logFile->Open(gLogFileName, CFile::EFile_Access::EFile_Access_1))
        {
            logFile->Close();
        }
        else
        {
            gLogToFile = false;
        }
        delete logFile;
    }
}

void FSGDoLog(static_string logText, ...)
{
    if(gLogInitialised)
    {
        FSG_ASSERT(logText, nullptr);

        char buffer[LOG_TEXT_MAX_LENGTH];

        va_list args;
        va_start(args, logText);
        vsprintf_s(buffer, LOG_TEXT_MAX_LENGTH - 2, logText, args);
        va_end(args);

        if(gLogToConsole)
        {
            OutputDebugStringA(buffer);
        }

        if(gLogToFile)
        {
            auto handle = CreateFileA(gLogFileName, 0x40000000, 0, nullptr, 3, 0x80, nullptr);
            if(handle != INVALID_HANDLE_VALUE)
            {
                SetFilePointer(handle, 0, nullptr, 2);
                auto length = strlen(buffer);
                WriteFile(handle, buffer, length, nullptr, nullptr);
                CloseHandle(handle);
            }
        }
    }
}

#endif