#include <cstring>
#include <cstdio>

#include <Core/FSGAssert.hpp>
#include <Collections/FSGString.hpp>
#include <cstdarg>

CString::CString() {
    m_static = false;
    m_buffer = nullptr;
    m_capacity = 0;
    m_length = 0;
}

i32 CString::GetLength() const {
    return this->m_length;
}

string CString::GetBuffer() const {
    return this->m_buffer;
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

i32 CString::Find(static_string t_string, i32 t_offset) const {
    FSG_ASSERT(t_string != nullptr, "Supplied string pointer was NULL.");

    auto stringLength = strlen(t_string);
    auto remaining = this->m_length - stringLength + 1;

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

i32 CString::Find(string t_string, i32 t_offset) const {
    FSG_ASSERT(t_string != nullptr, "Supplied string pointer was NULL.");

    auto stringLength = strlen(t_string);
    auto remaining = this->m_length - stringLength - 1;

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

i32 CString::ReverseFind(static_string t_string, i32 t_offset) const {
    // Fixme: Implement CString::ReverseFind()
    FSG_ASSERT(t_string != nullptr, "Supplied string pointer was NULL");

    auto searchLength = strlen(t_string);

    auto indexOfLastBufferCharacter = this->m_length - 1;
    auto indexOfLastSearchCharacter = searchLength - 1;

    auto result = -1;

    if (indexOfLastSearchCharacter && indexOfLastSearchCharacter < this->m_length) {
        if (t_offset < indexOfLastSearchCharacter - 1) {
            t_offset = indexOfLastSearchCharacter - 1;
        }

        if (t_offset < indexOfLastBufferCharacter) {
            auto v15 = indexOfLastBufferCharacter - t_offset;

            do {
                if (result != -1) {
                    break;
                }

                if (*t_string == this->m_buffer[indexOfLastBufferCharacter - t_offset]) {
                    result = v15;
                    for (auto i = 1; result != -1; i++) {
                        if (i >= indexOfLastSearchCharacter) {
                            break;
                        }
                        if (this->m_buffer[i + result] != t_string[i]) {
                            result = -1;
                        }
                    }
                }
                t_offset++;
                v15++;
            } while (t_offset < indexOfLastBufferCharacter);
        }
    }

    return result;
}

i32 CString::ReverseFind(character t_char, i32 t_offset) const {
    // For some reason, this function is also implemented differently
    // to the other functions in the CString class. Outdated?

    if (!this->m_length) {
        return -1;
    }

    FSG_ASSERT(t_offset >= 0 && t_offset < this->m_length, "Start offset is outside the bounds of the string.");

    // Custom implementation starting here.
    // Original was not fun.
    for (auto i = this->m_length - 1; i >= t_offset; i--) {
        if (this->m_buffer[i] == t_char) {
            return i;
        }
    }

    return -1;
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

void CString::Replace(character t_find, character t_replacement) {
    FSG_ASSERT(!this->m_static, "Statically allocated strings cannot be modified at run-time.");

    for (auto i = 0; i < this->m_length; i++) {
        if (this->m_buffer[i] == t_find) {
            this->m_buffer[i] = t_replacement;
        }
    }
}

void CString::ReplaceSingleChar(character t_replacement, u32 t_offset) {
    FSG_ASSERT(!this->m_static, "Statically allocated strings cannot be modified at run-time.");
    FSG_ASSERT(t_offset < this->m_length, "Offset is outside string bounds.");

    this->m_buffer[t_offset] = t_replacement;
}

void CString::Reverse() {
    FSG_ASSERT(!this->m_static, "Statically allocated strings cannot be modified at run-time.");

    auto steps = this->m_length >> 1;
    if (steps) {
        auto i = 0;
        auto j = this->m_length - 1;

        do {
            auto temp = this->m_buffer[i];
            this->m_buffer[i] = m_buffer[j];
            this->m_buffer[j] = temp;

            i++;
            j--;
            steps--;
        } while (steps);
    }
}

void CString::ToUpper() {
    FSG_ASSERT(!this->m_static, "Statically allocated strings cannot be modified at run-time.");

    for (auto i = 0; i < this->m_length; i++) {
        if (this->m_buffer[i] >= 'a' && this->m_buffer[i] <= 'z') {
            this->m_buffer[i] -= 0x20;
        }
    }
}

void CString::ToLower() {
    FSG_ASSERT(!this->m_static, "Statically allocated strings cannot be modified at run-time.");

    for (auto i = 0; i < this->m_length; i++) {
        if (this->m_buffer[i] >= 'A' && this->m_buffer[i] <= 'Z') {
            this->m_buffer[i] += 0x20;
        }
    }
}

void CString::StringCopy(string t_target, u32 t_targetCapacity, static_string t_source) {
    auto sourceLength = strlen(t_source);

    FSG_ASSERT(sourceLength < t_targetCapacity, "Source string is longer than target capacity.");

    strncpy_s(t_target, t_targetCapacity, t_source, t_targetCapacity - 1);
    t_target[t_targetCapacity - 1] = 0;
}

void CString::TrimLeft() {
    FSG_ASSERT(!this->m_static, "Statically allocated strings cannot be modified at run-time.");

    // This is referenced in the assert statements, but omitted in the actual code.
    auto offset = 0;
    auto count = 0;

    for (auto i = offset; i < this->m_length; i++) {
        auto character = this->m_buffer[i];
        if (character == ' ' || character == '\t' || character == '\n' || character == '\r') {
            count++;
        } else {
            break;
        }
    }

    if (count > 0) {
        this->Delete(offset, count);
    }
}

void CString::TrimRight() {
    FSG_ASSERT(!this->m_static, "Statically allocated strings cannot be modified at run-time.");

    // This is referenced in the assert statements, but omitted in the actual code.
    auto offset = 0;
    auto count = 0;

    for (auto i = this->m_length - 1; i > offset; i--) {
        auto character = this->m_buffer[i];
        if (character == ' ' || character == '\t' || character == '\n' || character == '\r') {
            count++;
        } else {
            break;
        }
    }

    if (count > 0) {
        this->Delete(m_length - (count + 1), count);
    }
}

CString::operator static_string() const {
    return this->m_buffer;
}

void CString::FormatString(string t_target, u32 t_targetCapacity, static_string t_source, ...) {
    va_list args;
    va_start(args, t_source);
    vsprintf_s(t_target, t_targetCapacity, t_source, args);
    va_end(args);
}
