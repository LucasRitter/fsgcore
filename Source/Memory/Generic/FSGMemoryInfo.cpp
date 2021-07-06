#include "Memory/FSGMemoryInfo.hpp"
#include "Collections/FSGString.hpp"

void GetMemoryInfo(MemoryInfo& info, u32 length)
{
    info.total     = length;
    info.mebibytes = length >> 20;
    info.kibibytes = (length >> 10) - ((length >> 10) & 0x3ffc00);
    info.bytes     = length - (length & 0xfffffc00);

    if(info.mebibytes >= 1)
    {
        String::FormatString(info.formatted, MEM_INFO_FORMATTED_STRING_CAPACITY, "%dMB %dkb %d bytes", info.mebibytes, info.kibibytes, info.bytes);
    }
    else if(info.kibibytes >= 1)
    {
        String::FormatString(info.formatted, MEM_INFO_FORMATTED_STRING_CAPACITY, "%dkb %d bytes", info.kibibytes, info.bytes);
    }
    else
    {
        String::FormatString(info.formatted, MEM_INFO_FORMATTED_STRING_CAPACITY, "%d bytes", info.bytes);
    }
}