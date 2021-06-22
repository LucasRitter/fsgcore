#pragma once

#include <cstdlib>
#include <cstring>

#include "Collections/FSGString.hpp"
#include "Core/FSGAssert.hpp"

template<u32 t_size>
class CFixedString : public CString
{
    public:
    /**
     * Creates a new instance of a fixed length string from @b t_string.
     * @param t_string The original string.
     */
    explicit CFixedString(const class CString& t_string)
    {
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
    CFixedString(const class CFixedString<t_size>& t_string)
    {
        this->m_capacity = t_size;

        this->m_length = t_string.GetLength();
        this->m_buffer = reinterpret_cast<string>(malloc(t_size));
        CString::StringCopy(this->m_buffer, t_size, t_string.GetBuffer());
    }

    /**
     * Creates a new instance of a fixed length string from @b t_string.
     * @param t_string The original string.
     */
    explicit CFixedString(static_string t_string)
    {
        this->m_capacity = t_size;

        this->m_length = strlen(t_string);
        this->m_buffer = reinterpret_cast<string>(malloc(t_size));
        CString::StringCopy(this->m_buffer, t_size, t_string);
    }

    // Todo: Write documentation for CFixedString::Append
    void Append(const class CString& t_string) override
    {
        auto totalLength = this->m_length + t_string.GetLength() + 1;
        FSG_ASSERT(totalLength <= this->m_capacity, "String being added is too long to fit into available buffer.");
        CString::StringCopy(&this->m_buffer[this->m_length], this->m_capacity, t_string.GetBuffer());
        this->m_length += t_string.GetLength();
    }

    // Todo: Write documentation for CFixedString::Append
    void Append(static_string t_string) override
    {
        auto stringLength = strlen(t_string);
        auto totalLength  = this->m_length + stringLength + 1;
        FSG_ASSERT(totalLength <= this->m_capacity, "String being added is too long to fit into available buffer.");
        CString::StringCopy(&this->m_buffer[this->m_length], this->m_capacity - this->m_length, t_string);
        this->m_length += stringLength;
    }

    // Todo: Write documentation for CFixedString::Extract
    void Extract(const class CString& t_string, i32 t_offset, i32 t_length)
    {
        FSG_ASSERT(t_offset >= 0 && t_offset < t_string.GetLength(), "Extraction point is outside the bounds of the source string.");
        FSG_ASSERT(t_length >= 0, "Negative extraction lengths are not supported.");
        FSG_ASSERT(t_offset + t_length <= t_string.GetLength(), "Extracted string extends beyond the bounds of the source string.");
        FSG_ASSERT(t_length < this->m_capacity, "Extracted string is too large to fit into the target buffer.");

        auto source = (static_string)t_string;
        for(auto i = 0; i < t_length; i++)
        {
            this->m_buffer[i] = source[i + t_offset];
        }
        this->m_buffer[t_length] = 0;
        this->m_length           = t_length;
    }

    // Todo: Write documentation for CFixedString::Format
    void Format(static_string t_format, ...) override
    {
        // Todo: Implement CFixedString::Format
    }

    // Todo: Write documentation for CFixedString::Insert
    void Insert(static_string t_string, u32 t_offset) override
    {
        // Todo: Implement CFixedString::Insert
    }

    // Todo: Write documentation for CFixedString::Insert
    void Insert(const class CString& t_string, u32 t_offset) override
    {
        // Todo: Implement CFixedString::Insert
    }

    // Todo: Write documentation for CFixedString::Insert
    void Insert(character, u32 t_offset) override
    {
        // Todo: Implement CFixedString::Insert
    }
};
