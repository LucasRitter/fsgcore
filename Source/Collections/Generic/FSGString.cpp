#include <cstring>

#include <Core/FSGAssert.hpp>
#include <Collections/FSGString.hpp>

CString::CString() {
    m_static = false;
    m_buffer = nullptr;
    m_length = 0;
}

void CString::Clear() {
    FSG_ASSERT(!this->m_static, "Statically allocated strings cannot be modified at run-time.");

    *this->m_buffer = 0;
    this->m_length = 0;
}

i32 CString::Compare(static_string t_string) {
    FSG_ASSERT(t_string != nullptr, "Supplied string pointer for comparison was NULL.");

    return strcmp(this->m_buffer, t_string);
}

i32 CString::CompareNoCase(static_string t_string) {
    FSG_ASSERT(t_string != nullptr, "Supplied string pointer for invariant comparison was NULL.");

    return _stricmp(this->m_buffer, t_string);
}

void CString::Delete(u32 t_index, u32 t_count) {
    FSG_ASSERT(!this->m_static, "Statically allocated strings cannot be modified at run-time.");
    FSG_ASSERT((t_index + t_count) <= this->m_length, "Parameters extend beyond the end of the string.");

    auto currentIndex = t_index;
    auto newLength = this->m_length - t_count;

    // Todo: Find good names for v10, v11.
    // Todo: Rewrite to use for loop instead of while.
    if (t_index < newLength) {
        auto v10 = newLength - t_index;
        do {
            auto v11 = &this->m_buffer[currentIndex++];
            *v11 = v11[t_count];
            --v10;
        } while (v10);
    }

    this->m_length = newLength;
    this->m_buffer[newLength] = 0;
}

i32 CString::Find(static_string t_string, i32 t_offset) {
    FSG_ASSERT(t_string != nullptr, "Supplied string pointer was NULL.");

    auto stringLength = strlen(t_string);
    auto remaining = m_length - stringLength + 1;

    auto foundIndex = -1;

    if (stringLength <= this->m_length && t_offset < remaining) {
        for (auto i = t_offset; i < remaining; i++) {
            if (*t_string == this->m_buffer[i]) {
                foundIndex = i;
                auto isDifferent = strcmp(&this->m_buffer[i], t_string);
                if (isDifferent) {
                    foundIndex = -1;
                } else {
                    break;
                }
            }
        }
    }

    return foundIndex;
}

i32 CString::Find(string t_string, i32 t_offset) {
    FSG_ASSERT(t_string != nullptr, "Supplied string pointer was NULL.");

    auto stringLength = strlen(t_string);
    auto remaining = m_length - stringLength - 1;

    auto foundIndex = -1;

    if (stringLength <= this->m_length && t_offset < remaining) {
        for (auto i = t_offset; i < remaining; i++) {
            if (*t_string == this->m_buffer[i]) {
                foundIndex = i;
                auto isDifferent = strcmp(&this->m_buffer[i], t_string);
                if (isDifferent) {
                    foundIndex = -1;
                } else {
                    break;
                }
            }
        }
    }

    return foundIndex;
}

void CString::Remove(char a1) {
    FSG_ASSERT(!this->m_static, "Statically allocated strings cannot be modified at run-time.");

    auto index = 0;
    for (auto i = 0; i < this->m_length; i++) {
        auto character = this->m_buffer[i];
        if (character != '-') {
            this->m_buffer[index++] = character;
        }
    }

    this->m_buffer[index] = 0;
    this->m_length = index;
}

void CString::Replace(static_string t_find, static_string t_replacement) {
    auto findStringLength = strlen(t_find);
    auto toDelete = findStringLength - 1;

    for (auto i = this->Find(t_find, 0); i != -1; i = this->Find(t_find, 0)) {
        this->Delete(i, toDelete);
        this->Insert(t_replacement, i);
    }
}


