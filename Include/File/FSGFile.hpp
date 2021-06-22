#pragma once

#include "Primitives.hpp"

#pragma region Callback Definitions
typedef i32             FileExistsCb(static_string path, void*);
typedef u64             FileSizeCb(class CFile*, void*);

class CFile
{
    public:
    enum EFile_Access
    {
        EFile_Access_LowLevel,
        EFile_Access_1,
        EFile_Access_2,
        EFile_Access_MAX,
    };

    enum EFile_Device
    {
        EFile_Device_0
    };

    enum EFile_Source
    {
        EFile_Source_0
    };

    protected:
    struct _sCallbacks
    {
    };

    public:
    CFile();
    CFile(EFile_Device device);
    ~CFile();

    i32  Open(static_string path, EFile_Access access);
    void Close();

    i32 Exists(static_string path);
    u32 GetSize() const;

    u64  Read(void* buffer, u64 length);
    void Write();
    void ReadAhead();
    void StepBufferBytes(u8 bytes);

    u8* GetFileBuffer();

    static void InstallCallbacks(EFile_Source source);

    protected:
    i32 InternalOpen(class CFilePath& path, EFile_Access access);
};