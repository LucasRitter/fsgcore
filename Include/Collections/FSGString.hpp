#pragma once

#include "Primitives.hpp"

class CString {
protected:
    bool m_static;
    char * m_buffer;
    u32 m_length;

    CString();

public:
    /**
     * Clears the string.
     */
    void Clear();

    /**
     * Checks if a given @b t_string is different.
     * @param t_string The string to check.
     * @return A value indicating if the strings are different.
     */
    i32 Compare(static_string t_string);

    /**
     * Checks if a given @b t_string is different without regard of uppercase or lowercase characters.
     * @param t_string The string to check.
     * @return A value indicating if the strings are different.
     */
    i32 CompareNoCase(static_string t_string);

    /**
     * Deletes a portion of the string at a given @b t_index with a given @b t_length.
     * @param t_index The index where characters will be deleted.
     * @param t_count How many characters will be deleted.
     */
    void Delete(u32 t_index, u32 t_count);

    /**
     * Finds the index where a given @b t_string can be found.
     * @param t_string The string to find.
     * @param t_offset The offset relative to @b m_buffer where the search will start from.
     * @return The position of the given @b t_string. Otherwise @em -1, if @b t_string can't be found.
     */
    i32 Find(static_string t_string, i32 t_offset);

    /**
     * Finds the index where a given @b t_string can be found.
     * @param t_string The string to find.
     * @param t_offset The offset relative to @b m_buffer where the search will start from.
     * @return The position of the given @b t_string. Otherwise @em -1, if @b t_string can't be found.
     */
    i32 Find(string t_string, i32 t_offset);

    // Todo: Write documentation for CString::Remove
    // a1 seems to be completely irrelevant.
    void Remove(char a1);

    /**
     * Finds @b t_find in @b m_buffer and replaces it with @b t_replacement.
     * @param t_find The string to be found.
     * @param t_replacement The string to replace @b t_find with.
     */
    void Replace(static_string t_find, static_string t_replacement);

    // Todo: Write documentation for virtual functions.
    virtual void Append(const class CString &t_string) = 0;
    virtual void Append(static_string t_string) = 0;
    virtual void Format(static_string t_format, ...) = 0;
    virtual void Insert(static_string t_string, u32 t_offset) = 0;
    virtual void Insert(const class CString &t_string, u32 t_offset) = 0;
    virtual void Insert(char, u32 t_offset) = 0;

    // Todo: Implement CString static methods.
    static void StringCopy(string t_target, u32 t_targetCapacity, static_string t_source);
    static void FormatString(string t_target, u32 t_targetCapacity, static_string t_source, ...);
    static void FormatStringLocalised(string t_target, u32 t_targetCapacity, static_string t_source, ...);
};