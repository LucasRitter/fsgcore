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

    if (index == -1 || length <= 0) {
        this->Clear();
    } else {
        this->Extract(t_string, index + 1, length);
    }
}

CFilePath *CFilePath::GetFileExtension() {
    auto index = this->ReverseFind('.', 0);
    auto length = this->GetLength() - (index + 1);

    if (index == -1) {
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
        if (index + 1 == this->m_length) {
            this->Clear();
        } else {
            this->Extract(t_string, index + 1, this->m_length - (index + 1));
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