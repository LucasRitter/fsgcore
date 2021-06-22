#include <ctime>

#include "System/FSGSystem.hpp"

#pragma region Time and Date

int FSGSystem::FSGTimeAndDate::CompareDates(const FSGTimeAndDate& dateA, const FSGTimeAndDate& dateB)
{
    if(dateA.year != dateB.year || dateA.month != dateB.month || dateA.day != dateB.day || dateA.hour != dateB.hour ||
       dateA.minutes != dateB.minutes || dateA.seconds != dateB.seconds)
    {
        return 0;
    }

    return 1;
}

bool FSGSystem::FSGTimeAndDate::operator<(const FSGTimeAndDate& date) const
{
    if(this->year < date.year)
    {
        return true;
    }
    if(this->month < date.month)
    {
        return true;
    }
    if(this->day < date.day)
    {
        return true;
    }

    if(this->hour < date.hour)
    {
        return true;
    }
    if(this->minutes < date.minutes)
    {
        return true;
    }
    if(this->seconds < date.seconds)
    {
        return true;
    }

    return false;
}

void FSGSystem::FSGGetTimeAndDate(FSGTimeAndDate& fsgDate)
{
    struct tm date
    {
    };
    auto now = time(nullptr);

    localtime_s(&date, &now);

    fsgDate.year  = static_cast<i16>(date.tm_year);
    fsgDate.month = static_cast<i16>(date.tm_mon);
    fsgDate.day   = static_cast<i16>(date.tm_mday);

    fsgDate.hour    = static_cast<i16>(date.tm_hour);
    fsgDate.minutes = static_cast<i16>(date.tm_min);
    fsgDate.seconds = static_cast<i16>(date.tm_sec);
}

void FSGSystem::FSGGetBuildDateAndTime(FSGTimeAndDate& buildDate)
{
    // Todo: FSGSystem::FSGGetBuildTimeAndDate
}

#pragma endregion

#pragma region Game Information

static_string FSGSystem::FSGGetBuildDateAsString()
{
    return __DATE__;
}

static_string FSGSystem::FSGGetBuildTimeAsString()
{
    return __TIME__;
}

static_string FSGSystem::FSGGetGameName()
{
#ifdef FSG_GAME_NAME
    return FSG_GAME_NAME;
#else
    return "To Be Filled";
#endif
}

static_string FSGSystem::FSGGetGameNameShort()
{
#ifdef FSG_GAME_NAME_SHORT
    return FSG_GAME_NAME_SHORT;
#else
    return "TBF";
#endif
}

i32 FSGSystem::FSGGetGameVersion()
{
#ifdef FSG_GAME_VERSION
    return FSG_GAME_VERSION;
#else
    return 1;
#endif
}

#if defined(WIN32) || defined(WIN64) || defined(UNIX) || defined(MACOSX)

u64 FSGSystem::FSGGetDiscordApplicationId()
{
    #ifdef FSG_DISCORD_APPID
    return FSG_DISCORD_APPID;
    #else
    return 0;
    #endif
}

#endif

#pragma endregion