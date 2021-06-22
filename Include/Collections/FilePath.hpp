#pragma once

#include "Collections/FSGFixedString.hpp"

class FilePath : public FixedString<256>
{
    public:
    FilePath(FilePath const& from);
    explicit FilePath(static_string from);

    ~FilePath();

    void      GetFilePath(const String& text);
    FilePath* GetFilePath();

    void      GetFileExtension(String const& text);
    FilePath* GetFileExtension();

    void      GetFileName(const String&);
    FilePath* GetFileName();

    void      GetFileNameWithoutExtension(const String&);
    FilePath* GetFileNameWithoutExtension();

    i32  HasFileExtension();
    void SetFileExtension(static_string extension);

    i32 ValidatePath();

    static i32 IsValidCharacter(character character);
};