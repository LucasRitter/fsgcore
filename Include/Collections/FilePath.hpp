#pragma once

#include "Collections/FSGFixedString.hpp"

class CFilePath : public CFixedString<256>
{
    public:
    CFilePath(CFilePath const& from);
    explicit CFilePath(static_string from);

    ~CFilePath();

    void       GetFilePath(const CString& text);
    CFilePath* GetFilePath();

    void       GetFileExtension(CString const& text);
    CFilePath* GetFileExtension();

    void       GetFileName(const CString&);
    CFilePath* GetFileName();

    void       GetFileNameWithoutExtension(const CString&);
    CFilePath* GetFileNameWithoutExtension();

    i32  HasFileExtension();
    void SetFileExtension(static_string extension);

    i32 ValidatePath();

    static i32 IsValidCharacter(character character);
};