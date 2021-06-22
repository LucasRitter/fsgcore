#ifdef WIN32

    #include <Collections/FSGString.hpp>
    #include <Core/FSGAssert.hpp>
    #include <Core/FSGLog.hpp>
    #include <File/FSGFile.hpp>
    #include <Windows.h>
    #include <cstdio>

u32  g_logInitialised = false;
u32  g_logToConsole   = false;
u32  g_logToFile      = false;
char g_logFileName[LOGFILE_NAME_MAX_LENGTH];
u64  g_logInitTime = 0;

void FSGLogInit(static_string t_logFileName, u32 t_logToFile, u32 t_logToConsole)
{
    // Original implementation used 32bit tick count.
    g_logInitTime  = GetTickCount64();
    g_logToConsole = t_logToConsole;

    if(t_logToFile && t_logFileName)
    {
        FSG_ASSERT(strlen(t_logFileName) > 0, "Logging has no file name defined (ie t_logFileName = '\\0')");
        CString::FormatString(g_logFileName, LOGFILE_NAME_MAX_LENGTH, "%s", t_logFileName);
        g_logToFile = true;
    }
    else
    {
        g_logToFile      = false;
        g_logFileName[0] = 0;
    }

    g_logInitialised = true;

    if(g_logToFile)
    {
        auto logFile = new CFile();
        if(logFile->Open(g_logFileName, CFile::EFile_Access::EFile_Access_1))
        {
            logFile->Close();
        }
        else
        {
            g_logToFile = false;
        }
        delete logFile;
    }
}

void FSGDoLog(static_string t_logText, ...)
{
    if(g_logInitialised)
    {
        FSG_ASSERT(t_logText, nullptr);

        char buffer[LOG_TEXT_MAX_LENGTH];

        va_list args;
        va_start(args, t_logText);
        vsprintf_s(buffer, LOG_TEXT_MAX_LENGTH - 2, t_logText, args);
        va_end(args);

        if(g_logToConsole)
        {
            OutputDebugStringA(buffer);
        }

        if(g_logToFile)
        {
            auto handle = CreateFileA(g_logFileName, 0x40000000, 0, nullptr, 3, 0x80, nullptr);
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