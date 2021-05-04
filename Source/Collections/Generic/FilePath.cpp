#include <Collections/FilePath.hpp>

CFilePath::CFilePath(const CFilePath &t_from) : CFixedString<256>(t_from) {};

CFilePath::CFilePath(static_string t_from) : CFixedString<256>(t_from) {}

CFilePath::~CFilePath() = default;

void CFilePath::GetFilePath(const CString &t_string) {
    // Todo: Switch based on operating system. Windows = '\\', UNIX = '/'.
    auto index = t_string.ReverseFind('\\', 0);
    if (index == -1) {
        this->Clear();
    } else {
        this->Extract(t_string, 0, index + 1);
        this->TrimRight();
    }
}

CFilePath *CFilePath::GetFilePath() {
    auto index = this->ReverseFind('\\', 0);

    if (index == -1) {
        return nullptr;
    }

    auto path = new CFilePath("");
    path->Extract(*this, 0, index + 1);

    return path;
}

void CFilePath::GetFileExtension(const CString &t_string) {
    auto index = t_string.ReverseFind('.', 0);
    auto length = t_string.GetLength() - (index + 1);

    // This check isn't in the original implementation, but is a nice-to-have.
    auto lastDirIndex = t_string.ReverseFind('\\', 0);

    if (index == -1 || length <= 0 || lastDirIndex > index) {
        this->Clear();
    } else {
        this->Extract(t_string, index + 1, length);
    }
}

CFilePath *CFilePath::GetFileExtension() {
    auto index = this->ReverseFind('.', 0);
    auto length = this->GetLength() - (index + 1);

    // This check isn't in the original implementation, but is a nice-to-have.
    auto lastDirIndex = this->ReverseFind('\\', 0);

    if (index == -1 || lastDirIndex > index) {
        return nullptr;
    }

    auto path = new CFilePath("");
    path->Extract(*this, index + 1, length);

    return path;
}

void CFilePath::GetFileName(const CString &t_string) {
    auto index = t_string.ReverseFind('\\', 0);
    if (index == -1) {
        this->Clear();
        CString::StringCopy(this->m_buffer, this->m_capacity, t_string.GetBuffer());
        this->m_length = t_string.GetLength();
    } else {
        if (index + 1 == t_string.GetLength()) {
            this->Clear();
        } else {
            this->Extract(t_string, index + 1, t_string.GetLength() - (index + 1));
        }
    }
}

CFilePath *CFilePath::GetFileName() {
    auto index = this->ReverseFind('\\', 0);
    auto path = new CFilePath("");

    if (index == -1)
        return path;

    path->Extract(*this, index, this->m_length - (index + 1));
    return path;
}

void CFilePath::GetFileNameWithoutExtension(const CString &t_string) {
    auto startIndex = t_string.ReverseFind('\\', 0);
    auto endIndex = t_string.ReverseFind('.', 0);
    auto length = 0;

    if (startIndex != -1) {
        startIndex += 1;

        if (startIndex == t_string.GetLength()) {
            this->Clear();
            return;
        }

        if (endIndex == -1 || endIndex - 1 <= startIndex) {
            length = t_string.GetLength() - startIndex;
        } else {
            length = endIndex - startIndex;
        }
    } else {
        if (endIndex == -1) {
            endIndex = t_string.GetLength();
        }

        startIndex = 0;
        length = endIndex - startIndex;
    }

    this->Extract(t_string, startIndex, length);
}

CFilePath *CFilePath::GetFileNameWithoutExtension() {
    auto startIndex = this->ReverseFind('\\', 0);
    auto endIndex = this->ReverseFind('.', 0);
    auto length = 0;

    auto path = new CFilePath("");

    if (startIndex != -1) {
        startIndex += 1;

        if (startIndex == this->m_length) {
            return path;
        }

        if (endIndex == -1 || endIndex - 1 <= startIndex) {
            length = this->m_length - startIndex;
        } else {
            length = endIndex - startIndex;
        }
    } else {
        if (endIndex == -1) {
            endIndex = this->m_length;
        }

        startIndex = 0;
        length = endIndex - startIndex;
    }

    path->Extract(*this, startIndex, length);
    return path;
}

i32 CFilePath::HasFileExtension() {
    auto separatorIndex = this->ReverseFind('.', 0);
    auto directoryIndex = this->ReverseFind('\\', 0);

    if (separatorIndex == -1 || directoryIndex > separatorIndex) {
        return false;
    }

    return true;
}

void CFilePath::SetFileExtension(static_string t_extension) {
    // Custom implementation. Differs from original.

    auto separatorIndex = this->ReverseFind('.', 0);
    auto directoryIndex = this->ReverseFind('\\', 0);

    auto startIndex = 0;

    if (separatorIndex == -1 || directoryIndex > separatorIndex) {
        startIndex = this->m_length;
    } else {
        startIndex = separatorIndex;
    }

    auto extensionLength = strlen(t_extension);
    FSG_ASSERT(startIndex + extensionLength < this->m_capacity, "ERROR trying to set file extension will cause a buffer overflow!");

    auto deleteLength = this->m_length - startIndex;
    if (deleteLength > 0) {
        this->Delete(startIndex, deleteLength);
    }

    this->Append(".");
    this->Append(t_extension);
}

i32 CFilePath::ValidatePath() {
    for (auto i = 0; i < this->m_length; i++) {
        if (!CFilePath::IsValidCharacter(this->m_buffer[i])) {
            FSG_ASSERT(false, "'%s' contains invalid file characters.", this->m_buffer);
            return false;
        }
    }

    return true;
}

i32 CFilePath::IsValidCharacter(character character) {
    if ((character < 'a' || character > 'z')
        && (character < 'A' || character > 'Z')
        && character != '\\'
        && character != '_'
        && character != '-'
        && character != '.'
        && character != ' '
    ) {
        return false;
    }

    return true;
}
