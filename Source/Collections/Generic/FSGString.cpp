#include <cstring>
#include <cstdio>
#include <cstdarg>

#include "Core/FSGAssert.hpp"
#include "Collections/FSGString.hpp"

String::String()
{
    isStatic = false;
    buffer   = nullptr;
    capacity = 0;
    length   = 0;
}

i32 String::GetLength() const
{
    return this->length;
}

string String::GetBuffer() const
{
    return this->buffer;
}

void String::Clear()
{
    FSG_ASSERT(!this->isStatic, "Statically allocated strings cannot be modified at run-time.");

    *this->buffer = 0;
    this->length  = 0;
}

i32 String::Compare(static_string text)
{
    FSG_ASSERT(text != nullptr, "Supplied text pointer for comparison was NULL.");

    return strcmp(this->buffer, text);
}

i32 String::CompareNoCase(static_string text)
{
    FSG_ASSERT(text != nullptr, "Supplied text pointer for invariant comparison was NULL.");

    return _stricmp(this->buffer, text);
}

void String::Delete(u32 index, u32 count)
{
    FSG_ASSERT(!this->isStatic, "Statically allocated strings cannot be modified at run-time.");
    FSG_ASSERT((index + count) <= this->length, "Parameters extend beyond the end of the string.");

    auto currentIndex = index;
    auto newLength    = this->length - count;

    // Todo: Find good names for v10, v11.
    // Todo: Rewrite to use for loop instead of while.
    if(index < newLength)
    {
        auto v10 = newLength - index;
        do
        {
            auto v11 = &this->buffer[currentIndex++];
            *v11     = v11[count];
            --v10;
        } while(v10);
    }

    this->length            = newLength;
    this->buffer[newLength] = 0;
}

i32 String::Find(static_string text, i32 offset) const
{
    FSG_ASSERT(text != nullptr, "Supplied text pointer was NULL.");

    auto stringLength = strlen(text);
    auto remaining    = this->length - stringLength + 1;

    auto foundIndex = -1;

    if(stringLength <= this->length && offset < remaining)
    {
        for(auto i = offset; i < remaining; i++)
        {
            if(*text == this->buffer[i])
            {
                foundIndex       = i;
                auto isDifferent = strcmp(&this->buffer[i], text);
                if(isDifferent)
                {
                    foundIndex = -1;
                }
                else
                {
                    break;
                }
            }
        }
    }

    return foundIndex;
}

i32 String::Find(string text, i32 offset) const
{
    FSG_ASSERT(text != nullptr, "Supplied text pointer was NULL.");

    auto stringLength = strlen(text);
    auto remaining    = this->length - stringLength - 1;

    auto foundIndex = -1;

    if(stringLength <= this->length && offset < remaining)
    {
        for(auto i = offset; i < remaining; i++)
        {
            if(*text == this->buffer[i])
            {
                foundIndex       = i;
                auto isDifferent = strcmp(&this->buffer[i], text);
                if(isDifferent)
                {
                    foundIndex = -1;
                }
                else
                {
                    break;
                }
            }
        }
    }

    return foundIndex;
}

i32 String::ReverseFind(static_string text, i32 offset) const
{
    // Fixme: Implement String::ReverseFind()
    FSG_ASSERT(text != nullptr, "Supplied text pointer was NULL");

    auto searchLength = strlen(text);

    auto indexOfLastBufferCharacter = this->length - 1;
    auto indexOfLastSearchCharacter = searchLength - 1;

    auto result = -1;

    if(indexOfLastSearchCharacter && indexOfLastSearchCharacter < this->length)
    {
        if(offset < indexOfLastSearchCharacter - 1)
        {
            offset = indexOfLastSearchCharacter - 1;
        }

        if(offset < indexOfLastBufferCharacter)
        {
            auto v15 = indexOfLastBufferCharacter - offset;

            do
            {
                if(result != -1)
                {
                    break;
                }

                if(*text == this->buffer[indexOfLastBufferCharacter - offset])
                {
                    result = v15;
                    for(auto i = 1; result != -1; i++)
                    {
                        if(i >= indexOfLastSearchCharacter)
                        {
                            break;
                        }
                        if(this->buffer[i + result] != text[i])
                        {
                            result = -1;
                        }
                    }
                }
                offset++;
                v15++;
            } while(offset < indexOfLastBufferCharacter);
        }
    }

    return result;
}

i32 String::ReverseFind(character character, i32 offset) const
{
    // For some reason, this function is also implemented differently
    // to the other functions in the String class. Outdated?

    if(!this->length)
    {
        return -1;
    }

    FSG_ASSERT(offset >= 0 && offset < this->length, "Start offset is outside the bounds of the string.");

    // Custom implementation starting here.
    // Original was not fun.
    for(auto i = this->length - 1; i >= offset; i--)
    {
        if(this->buffer[i] == character)
        {
            return i;
        }
    }

    return -1;
}

void String::Remove(char a1)
{
    FSG_ASSERT(!this->isStatic, "Statically allocated strings cannot be modified at run-time.");

    auto index = 0;
    for(auto i = 0; i < this->length; i++)
    {
        auto character = this->buffer[i];
        if(character != '-')
        {
            this->buffer[index++] = character;
        }
    }

    this->buffer[index] = 0;
    this->length        = index;
}

void String::Replace(static_string find, static_string replacement)
{
    auto findStringLength = strlen(find);
    auto toDelete         = findStringLength - 1;

    for(auto i = this->Find(find, 0); i != -1; i = this->Find(find, 0))
    {
        this->Delete(i, toDelete);
        this->Insert(replacement, i);
    }
}

void String::Replace(character find, character replacement)
{
    FSG_ASSERT(!this->isStatic, "Statically allocated strings cannot be modified at run-time.");

    for(auto i = 0; i < this->length; i++)
    {
        if(this->buffer[i] == find)
        {
            this->buffer[i] = replacement;
        }
    }
}

void String::ReplaceSingleChar(character replacement, u32 offset)
{
    FSG_ASSERT(!this->isStatic, "Statically allocated strings cannot be modified at run-time.");
    FSG_ASSERT(offset < this->length, "Offset is outside string bounds.");

    this->buffer[offset] = replacement;
}

void String::Reverse()
{
    FSG_ASSERT(!this->isStatic, "Statically allocated strings cannot be modified at run-time.");

    auto steps = this->length >> 1;
    if(steps)
    {
        auto i = 0;
        auto j = this->length - 1;

        do
        {
            auto temp       = this->buffer[i];
            this->buffer[i] = buffer[j];
            this->buffer[j] = temp;

            i++;
            j--;
            steps--;
        } while(steps);
    }
}

void String::ToUpper()
{
    FSG_ASSERT(!this->isStatic, "Statically allocated strings cannot be modified at run-time.");

    for(auto i = 0; i < this->length; i++)
    {
        if(this->buffer[i] >= 'a' && this->buffer[i] <= 'z')
        {
            this->buffer[i] -= 0x20;
        }
    }
}

void String::ToLower()
{
    FSG_ASSERT(!this->isStatic, "Statically allocated strings cannot be modified at run-time.");

    for(auto i = 0; i < this->length; i++)
    {
        if(this->buffer[i] >= 'A' && this->buffer[i] <= 'Z')
        {
            this->buffer[i] += 0x20;
        }
    }
}

void String::StringCopy(string target, u32 targetCapacity, static_string source)
{
    auto sourceLength = strlen(source);

    FSG_ASSERT(sourceLength < targetCapacity, "Source string is longer than target capacity.");

    strncpy_s(target, targetCapacity, source, targetCapacity - 1);
    target[targetCapacity - 1] = 0;
}

void String::TrimLeft()
{
    FSG_ASSERT(!this->isStatic, "Statically allocated strings cannot be modified at run-time.");

    // This is referenced in the assert statements, but omitted in the actual code.
    auto offset = 0;
    auto count  = 0;

    for(auto i = offset; i < this->length; i++)
    {
        auto character = this->buffer[i];
        if(character == ' ' || character == '\t' || character == '\n' || character == '\r')
        {
            count++;
        }
        else
        {
            break;
        }
    }

    if(count > 0)
    {
        this->Delete(offset, count);
    }
}

void String::TrimRight()
{
    FSG_ASSERT(!this->isStatic, "Statically allocated strings cannot be modified at run-time.");

    // This is referenced in the assert statements, but omitted in the actual code.
    auto offset = 0;
    auto count  = 0;

    for(auto i = this->length - 1; i > offset; i--)
    {
        auto character = this->buffer[i];
        if(character == ' ' || character == '\t' || character == '\n' || character == '\r')
        {
            count++;
        }
        else
        {
            break;
        }
    }

    if(count > 0)
    {
        this->Delete(length - (count + 1), count);
    }
}

String::operator static_string() const
{
    return this->buffer;
}

void String::FormatString(string target, u32 targetCapacity, static_string source, ...)
{
    va_list args;
    va_start(args, source);
    vsprintf_s(target, targetCapacity, source, args);
    va_end(args);
}
