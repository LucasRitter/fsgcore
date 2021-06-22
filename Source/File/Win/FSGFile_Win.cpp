#ifdef WIN32

    #include <Windows.h>

    #include "Collections/FilePath.hpp"
    #include "File/FSGFile.hpp"

CFile::CFile()
{
    // Fixme: Implement the real CFile::Open function
}

CFile::~CFile()
{
    // Fixme: Implement the real CFile destructor
}

i32 CFile::Open(static_string path, EFile_Access access)
{
    // Fixme: Implement the real CFile::Open function
    return 1;
}

void CFile::Close()
{
    // Fixme: Implement the real CFile::Close function
}

#endif