#pragma once

#include <Collections/FSGFixedString.hpp>

class CFilePath: public CFixedString<256> {
public:
    CFilePath(CFilePath const &t_from);
    explicit CFilePath(static_string t_from);

    ~CFilePath();

    void GetFilePath(const CString &t_string);
    CFilePath *GetFilePath();

    void GetFileExtension(CString const &t_string);
    CFilePath *GetFileExtension();

    void GetFileName(const CString &);
    CFilePath *GetFileName();

    void GetFileNameWithoutExtension(const CString &);
    CFilePath *GetFileNameWithoutExtension();

    i32 HasFileExtension();
    void SetFileExtension(static_string t_extension);

    i32 ValidatePath();
};