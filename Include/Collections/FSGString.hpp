#pragma once

#include "Primitives.hpp"

class String
{
    protected:
    bool   isStatic;
    string buffer;
    i32    capacity;
    i32    length;
    String();

    public:
    /**
     * @return The length of the string.
     */
    i32 GetLength() const;

    /**
     * @return The buffer of the string.
     */
    string GetBuffer() const;

    /**
     * Clears the string.
     */
    void Clear();

    /**
     * Checks if a given @b text is different.
     * @param text The text to check.
     * @return A value indicating if the strings are different.
     */
    i32 Compare(static_string text);

    /**
     * Checks if a given @b text is different without regard of uppercase or lowercase characters.
     * @param text The text to check.
     * @return A value indicating if the strings are different.
     */
    i32 CompareNoCase(static_string text);

    /**
     * Deletes a portion of the string at a given @b index with a given @b t_length.
     * @param index The index where characters will be deleted.
     * @param count How many characters will be deleted.
     */
    void Delete(u32 index, u32 count);

    /**
     * Finds the index where a given @b text can be found.
     * @param text The text to find.
     * @param offset The offset relative to @b buffer where the search will start from.
     * @return The position of the given @b text. Otherwise @em -1, if @b text can't be found.
     */
    i32 Find(static_string text, i32 offset) const;

    /**
     * Finds the index where a given @b text can be found.
     * @param text The text to find.
     * @param offset The offset relative to @b buffer where the search will start from.
     * @return The position of the given @b text. Otherwise @em -1, if @b text can't be found.
     */
    i32 Find(string text, i32 offset) const;

    // Todo: Write documentation for String::ReverseFind
    i32 ReverseFind(static_string text, i32 offset) const;

    // Todo: Write documentation for String::ReverseFind
    i32 ReverseFind(character character, i32 offset) const;

    // Todo: Write documentation for String::Remove
    // a1 seems to be completely irrelevant.
    void Remove(char a1);

    /**
     * Finds @b find in @b buffer and replaces it with @b replacement.
     * @param find The string to be found.
     * @param replacement The string to replace @b find with.
     */
    void Replace(static_string find, static_string replacement);

    /**
     * Finds @b find in @b buffer and replaces it with @t_replacement.
     * @param find The @b character to be found.
     * @param replacement The character to replace @b find with.
     */
    void Replace(character find, character replacement);

    // Todo: Write documentation for String::ReplaceSingleChar
    void ReplaceSingleChar(character replacement, u32 offset);

    // Todo: Write documentation for String::Reverse
    void Reverse();

    /**
     * Makes all characters in @b buffer uppercase.
     */
    void ToUpper();

    /**
     * Makes all characters in @b buffer lowercase.
     */
    void ToLower();

    // Todo: Write documentation for String::TrimLeft
    void TrimLeft();

    // Todo: Write documentation for String::TrimRight
    void TrimRight();

    // Todo: Write documentation for String::operator static_string()
    explicit operator static_string() const;

    // Todo: Write documentation for String::operator==()
    i32 operator==(static_string text) const;

    // Todo: Write documentation for String::operator!=()
    i32 operator!=(static_string text) const;

    // Todo: Write documentation for virtual functions.
    virtual void Append(const class String& text)             = 0;
    virtual void Append(static_string text)                   = 0;
    virtual void Format(static_string format, ...)            = 0;
    virtual void Insert(static_string text, u32 offset)       = 0;
    virtual void Insert(const class String& text, u32 format) = 0;
    virtual void Insert(character character, u32 offset)      = 0;

    // Todo: Implement String static methods.
    static void StringCopy(string target, u32 targetCapacity, static_string source);
    static void StringCopyLength(string target, u32 targetCapacity, static_string source, u32 sourceLength);
    static void FormatString(string target, u32 targetCapacity, static_string source, ...);
    static void FormatStringLocalised(string target, u32 targetCapacity, static_string source, ...);

    // Todo: Find proper parameter names.
    static void StringCat(string, u32, static_string);
};