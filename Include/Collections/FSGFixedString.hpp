#pragma once

#include <cstdlib>
#include <cstring>

#include "Collections/FSGString.hpp"
#include "Core/FSGAssert.hpp"

template<u32 t_size>
class FixedString : public String
{
    public:
    /**
     * Creates a new instance of a fixed length string from @b text.
     * @param text The original string.
     */
    explicit FixedString(const class String& text)
    {
        this->capacity = t_size;

        FSG_ASSERT(text.GetLength() + 1 < t_size, "Input string was too long to fit into data buffer.");

        this->length = text.GetLength();
        this->buffer = reinterpret_cast<string>(malloc(t_size));
        String::StringCopy(this->buffer, t_size, text.GetBuffer());
    }

    /**
     * Creates a new instance of a fixed length string from @b text.
     * @param text The original string.
     */
    FixedString(const class FixedString<t_size>& text)
    {
        this->capacity = t_size;

        this->length = text.GetLength();
        this->buffer = reinterpret_cast<string>(malloc(t_size));
        String::StringCopy(this->buffer, t_size, text.GetBuffer());
    }

    /**
     * Creates a new instance of a fixed length string from @b text.
     * @param text The original string.
     */
    explicit FixedString(static_string text)
    {
        this->capacity = t_size;

        this->length = strlen(text);
        this->buffer = reinterpret_cast<string>(malloc(t_size));
        String::StringCopy(this->buffer, t_size, text);
    }

    // Todo: Write documentation for FixedString::Append
    void Append(const class String& text) override
    {
        auto totalLength = this->length + text.GetLength() + 1;
        FSG_ASSERT(totalLength <= this->capacity, "String being added is too long to fit into available buffer.");
        String::StringCopy(&this->buffer[this->length], this->capacity, text.GetBuffer());
        this->length += text.GetLength();
    }

    // Todo: Write documentation for FixedString::Append
    void Append(static_string text) override
    {
        auto stringLength = strlen(text);
        auto totalLength  = this->length + stringLength + 1;
        FSG_ASSERT(totalLength <= this->capacity, "String being added is too long to fit into available buffer.");
        String::StringCopy(&this->buffer[this->length], this->capacity - this->length, text);
        this->length += stringLength;
    }

    // Todo: Write documentation for FixedString::Extract
    void Extract(const class String& text, i32 offset, i32 length)
    {
        FSG_ASSERT(offset >= 0 && offset < text.GetLength(), "Extraction point is outside the bounds of the source string.");
        FSG_ASSERT(length >= 0, "Negative extraction lengths are not supported.");
        FSG_ASSERT(offset + length <= text.GetLength(), "Extracted string extends beyond the bounds of the source string.");
        FSG_ASSERT(length < this->capacity, "Extracted string is too large to fit into the target buffer.");

        auto source = (static_string)text;
        for(auto i = 0; i < length; i++)
        {
            this->buffer[i] = source[i + offset];
        }
        this->buffer[length] = 0;
        this->length         = length;
    }

    // Todo: Write documentation for FixedString::Format
    void Format(static_string format, ...) override
    {
        // Todo: Implement FixedString::Format
    }

    // Todo: Write documentation for FixedString::Insert
    void Insert(static_string text, u32 offset) override
    {
        // Todo: Implement FixedString::Insert
    }

    // Todo: Write documentation for FixedString::Insert
    void Insert(const class String& text, u32 offset) override
    {
        // Todo: Implement FixedString::Insert
    }

    // Todo: Write documentation for FixedString::Insert
    void Insert(character character, u32 offset) override
    {
        // Todo: Implement FixedString::Insert
    }
};
