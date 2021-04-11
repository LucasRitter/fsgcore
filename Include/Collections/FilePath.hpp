#pragma once

#include <Collections/FSGFixedString.hpp>

class CFilePath: public CFixedString<256> {
public:
    CFilePath(const CFilePath &t_from);
    explicit CFilePath(static_string t_from);

    void GetFilePath(const class CString &t_string);

    CFilePath *GetFileExtension(CString const &);
    CFilePath *GetFileExtension();

    CFilePath *GetFileName(CString const &);
    CFilePath *GetFileName();

    CFilePath *GetFileNameWithoutExtension(CString const &);
    CFilePath *GetFileNameWithoutExtension();

    i32 HasFileExtension();
    void SetFileExtension(static_string t_extension);

    i32 ValidatePath();
};