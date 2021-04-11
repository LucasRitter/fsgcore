#pragma once

#include <cstdlib>
#include <cstring>

#include <Collections/FSGString.hpp>

template <u32 t_size>
class CFixedString: CString {
public:
    /**
     * Creates a new instance of a fixed length string from @b t_string.
     * @param t_string The original string.
     */
    explicit CFixedString(const class CString &t_string) {
        this->m_capacity = t_size;

        FSG_ASSERT(t_string.GetLength() + 1 < t_size, "Input string was too long to fit into data buffer.");

        this->m_length = t_string.GetLength();
        this->m_buffer = reinterpret_cast<string>(malloc(t_size));
        CString::StringCopy(this->m_buffer, t_size, t_string.GetBuffer());
    }

    /**
     * Creates a new instance of a fixed length string from @b t_string.
     * @param t_string The original string.
     */
    CFixedString(const class CFixedString<t_size> &t_string) {
        this->m_capacity = t_size;

        this->m_length = t_string.GetLength();
        this->m_buffer = reinterpret_cast<string>(malloc(t_size));
        CString::StringCopy(this->m_buffer, t_size, t_string.GetBuffer());
    }

    /**
     * Creates a new instance of a fixed length string from @b t_string.
     * @param t_string The original string.
     */
    explicit CFixedString(static_string t_string) {
        this->m_capacity = t_size;

        this->m_length = strlen(t_string);
        this->m_buffer = reinterpret_cast<string>(malloc(t_size));
        CString::StringCopy(this->m_buffer, t_size, t_string);
    }

    // Todo: Write documentation for CFixedString::Append
    void Append(const class CString &t_string) override {
        // Todo: Implement CFixedString::Append
    }

    // Todo: Write documentation for CFixedString::Append
    void Append(static_string t_string) override {
        // Todo: Implement CFixedString::Append
    }

    // Todo: Write documentation for CFixedString::Format
    void Format(static_string t_format, ...) override {
        // Todo: Implement CFixedString::Format
    }

    // Todo: Write documentation for CFixedString::Insert
    void Insert(static_string t_string, u32 t_offset) override {
        // Todo: Implement CFixedString::Insert
    }

    // Todo: Write documentation for CFixedString::Insert
    void Insert(const class CString &t_string, u32 t_offset) override {
        // Todo: Implement CFixedString::Insert
    }

    // Todo: Write documentation for CFixedString::Insert
    void Insert(character, u32 t_offset) override {
        // Todo: Implement CFixedString::Insert
    }
};
