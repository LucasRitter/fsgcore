#include "Collections/FilePath.hpp"

FilePath::FilePath(const FilePath& from) : FixedString<256>(from) {}

FilePath::FilePath(static_string from) : FixedString<256>(from) {}

FilePath::~FilePath() = default;

void FilePath::GetFilePath(const String& text)
{
    // Todo: Switch based on operating system. Windows = '\\', UNIX = '/'.
    auto index = text.ReverseFind('\\', 0);
    if(index == -1)
    {
        this->Clear();
    }
    else
    {
        this->Extract(text, 0, index + 1);
        this->TrimRight();
    }
}

FilePath* FilePath::GetFilePath()
{
    auto index = this->ReverseFind('\\', 0);

    if(index == -1)
    {
        return nullptr;
    }

    auto path = new FilePath("");
    path->Extract(*this, 0, index + 1);

    return path;
}

void FilePath::GetFileExtension(const String& text)
{
    auto index  = text.ReverseFind('.', 0);
    auto length = text.GetLength() - (index + 1);

    // This check isn't in the original implementation, but is a nice-to-have.
    auto lastDirIndex = text.ReverseFind('\\', 0);

    if(index == -1 || length <= 0 || lastDirIndex > index)
    {
        this->Clear();
    }
    else
    {
        this->Extract(text, index + 1, length);
    }
}

FilePath* FilePath::GetFileExtension()
{
    auto index  = this->ReverseFind('.', 0);
    auto length = this->GetLength() - (index + 1);

    // This check isn't in the original implementation, but is a nice-to-have.
    auto lastDirIndex = this->ReverseFind('\\', 0);

    if(index == -1 || lastDirIndex > index)
    {
        return nullptr;
    }

    auto path = new FilePath("");
    path->Extract(*this, index + 1, length);

    return path;
}

void FilePath::GetFileName(const String& t_string)
{
    auto index = t_string.ReverseFind('\\', 0);
    if(index == -1)
    {
        this->Clear();
        String::StringCopy(this->buffer, this->capacity, t_string.GetBuffer());
        this->length = t_string.GetLength();
    }
    else
    {
        if(index + 1 == t_string.GetLength())
        {
            this->Clear();
        }
        else
        {
            this->Extract(t_string, index + 1, t_string.GetLength() - (index + 1));
        }
    }
}

FilePath* FilePath::GetFileName()
{
    auto index = this->ReverseFind('\\', 0);
    auto path  = new FilePath("");

    if(index == -1) return path;

    path->Extract(*this, index, this->length - (index + 1));
    return path;
}

void FilePath::GetFileNameWithoutExtension(const String& t_string)
{
    auto startIndex = t_string.ReverseFind('\\', 0);
    auto endIndex   = t_string.ReverseFind('.', 0);
    auto length     = 0;

    if(startIndex != -1)
    {
        startIndex += 1;

        if(startIndex == t_string.GetLength())
        {
            this->Clear();
            return;
        }

        if(endIndex == -1 || endIndex - 1 <= startIndex)
        {
            length = t_string.GetLength() - startIndex;
        }
        else
        {
            length = endIndex - startIndex;
        }
    }
    else
    {
        if(endIndex == -1)
        {
            endIndex = t_string.GetLength();
        }

        startIndex = 0;
        length     = endIndex - startIndex;
    }

    this->Extract(t_string, startIndex, length);
}

FilePath* FilePath::GetFileNameWithoutExtension()
{
    auto startIndex = this->ReverseFind('\\', 0);
    auto endIndex   = this->ReverseFind('.', 0);
    auto length     = 0;

    auto path = new FilePath("");

    if(startIndex != -1)
    {
        startIndex += 1;

        if(startIndex == this->length)
        {
            return path;
        }

        if(endIndex == -1 || endIndex - 1 <= startIndex)
        {
            length = this->length - startIndex;
        }
        else
        {
            length = endIndex - startIndex;
        }
    }
    else
    {
        if(endIndex == -1)
        {
            endIndex = this->length;
        }

        startIndex = 0;
        length     = endIndex - startIndex;
    }

    path->Extract(*this, startIndex, length);
    return path;
}

i32 FilePath::HasFileExtension()
{
    auto separatorIndex = this->ReverseFind('.', 0);
    auto directoryIndex = this->ReverseFind('\\', 0);

    if(separatorIndex == -1 || directoryIndex > separatorIndex)
    {
        return false;
    }

    return true;
}

void FilePath::SetFileExtension(static_string extension)
{
    // Custom implementation. Differs from original.

    auto separatorIndex = this->ReverseFind('.', 0);
    auto directoryIndex = this->ReverseFind('\\', 0);

    auto startIndex = 0;

    if(separatorIndex == -1 || directoryIndex > separatorIndex)
    {
        startIndex = this->length;
    }
    else
    {
        startIndex = separatorIndex;
    }

    auto extensionLength = strlen(extension);
    FSG_ASSERT(startIndex + extensionLength < this->capacity, "ERROR trying to set file extension will cause a buffer overflow!");

    auto deleteLength = this->length - startIndex;
    if(deleteLength > 0)
    {
        this->Delete(startIndex, deleteLength);
    }

    this->Append(".");
    this->Append(extension);
}

i32 FilePath::ValidatePath()
{
    for(auto i = 0; i < this->length; i++)
    {
        if(!FilePath::IsValidCharacter(this->buffer[i]))
        {
            FSG_ASSERT(false, "'%s' contains invalid file characters.", this->buffer);
            return false;
        }
    }

    return true;
}

i32 FilePath::IsValidCharacter(character character)
{
    if((character < 'a' || character > 'z') && (character < 'A' || character > 'Z') && character != '\\' && character != '_' && character != '-' &&
       character != '.' && character != ' ')
    {
        return false;
    }

    return true;
}
