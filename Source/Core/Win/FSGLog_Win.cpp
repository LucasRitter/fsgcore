#ifdef WIN32
    #include <cstdio>
    #include <iostream>

    #include <Windows.h>

    #include "Collections/FSGString.hpp"
    #include "Core/FSGAssert.hpp"
    #include "Core/FSGLog.hpp"
    #include "File/FSGFile.hpp"

u32       isInitialised = false;
u32       toConsole     = false;
u32       toFile        = false;
character fileName[LOGFILE_NAME_MAX_LENGTH];
u64       initTime = 0;

void FSGLogInit(static_string logFileName, u32 logToFile, u32 logToConsole)
{
    // Original implementation used 32bit tick count.
    initTime  = GetTickCount64();
    toConsole = logToConsole;

    if(logToFile && logFileName)
    {
        FSG_ASSERT(strlen(logFileName) > 0, "Logging has no file name defined (ie logFileName = '\\0')");
        String::FormatString(fileName, LOGFILE_NAME_MAX_LENGTH, "%s", logFileName);
        toFile = true;
    }
    else
    {
        toFile      = false;
        fileName[0] = 0;
    }

    isInitialised = true;

    if(toFile)
    {
        auto logFile = new CFile();
        if(logFile->Open(fileName, CFile::EFile_Access::EFile_Access_1))
        {
            logFile->Close();
        }
        else
        {
            toFile = false;
        }
        delete logFile;
    }
}

void FSGDoLog(static_string logText, ...)
{
    if(isInitialised)
    {
        FSG_ASSERT(logText, nullptr);

        character buffer[LOG_TEXT_MAX_LENGTH];

        va_list args;
        va_start(args, logText);
        vsprintf_s(buffer, LOG_TEXT_MAX_LENGTH - 2, logText, args);
        va_end(args);

        if(toConsole)
        {
            std::cout << buffer;
            OutputDebugStringA(buffer);
        }

        if(toFile)
        {
            auto handle = CreateFileA(fileName, 0x40000000, 0, nullptr, 3, 0x80, nullptr);
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