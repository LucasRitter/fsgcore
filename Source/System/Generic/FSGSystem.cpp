#include <ctime>

#include "System/FSGSystem.hpp"

#pragma region Time and Date

int FSGSystem::SFSGTimeAndDate::CompareDates(const FSGSystem::SFSGTimeAndDate &t_dateA,
                                             const FSGSystem::SFSGTimeAndDate &t_dateB) {
    if (t_dateA.m_year != t_dateB.m_year
        || t_dateA.m_month != t_dateB.m_month
        || t_dateA.m_day != t_dateB.m_day
        || t_dateA.m_hour != t_dateB.m_hour
        || t_dateA.m_minutes != t_dateB.m_minutes
        || t_dateA.m_seconds != t_dateB.m_seconds) {
        return 0;
    }

    return 1;
}

bool FSGSystem::SFSGTimeAndDate::operator<(const FSGSystem::SFSGTimeAndDate &t_date) const {
    if (this->m_year < t_date.m_year) {
        return true;
    }
    if (this->m_month < t_date.m_month) {
        return true;
    }
    if (this->m_day < t_date.m_day) {
        return true;
    }

    if (this->m_hour < t_date.m_hour) {
        return true;
    }
    if (this->m_minutes < t_date.m_minutes) {
        return true;
    }
    if (this->m_seconds < t_date.m_seconds) {
        return true;
    }

    return false;
}

void FSGSystem::FSGGetTimeAndDate(FSGSystem::SFSGTimeAndDate &t_date) {
    struct tm date{};
    auto now = time(nullptr);

    localtime_s(&date, &now);

    t_date.m_year = static_cast<u16>(date.tm_year);
    t_date.m_month = static_cast<u16>(date.tm_mon);
    t_date.m_day = static_cast<u16>(date.tm_mday);

    t_date.m_hour = static_cast<u16>(date.tm_hour);
    t_date.m_minutes = static_cast<u16>(date.tm_min);
    t_date.m_seconds = static_cast<u16>(date.tm_sec);
}


void FSGSystem::FSGGetBuildDateAndTime(FSGSystem::SFSGTimeAndDate &t_buildDate) {
    // Todo: FSGSystem::FSGGetBuildTimeAndDate
}

#pragma endregion

#pragma region Game Information

static_string FSGSystem::FSGGetBuildDateAsString() {
    return __DATE__;
}

static_string FSGSystem::FSGGetBuildTimeAsString() {
    return __TIME__;
}

static_string FSGSystem::FSGGetGameName() {
#ifdef FSG_GAME_NAME
    return FSG_GAME_NAME;
#else
    return "To Be Filled";
#endif
}

static_string FSGSystem::FSGGetGameNameShort() {
#ifdef FSG_GAME_NAME_SHORT
    return FSG_GAME_NAME_SHORT;
#else
    return "TBF";
#endif
}

i32 FSGSystem::FSGGetGameVersion() {
#ifdef FSG_GAME_VERSION
    return FSG_GAME_VERSION;
#else
    return 1;
#endif
}

#if defined(WIN32) || defined(WIN64) || defined(UNIX) || defined(MACOSX)

u64 FSGSystem::FSGGetDiscordApplicationId() {
#ifdef FSG_DISCORD_APPID
    return FSG_DISCORD_APPID;
#else
    return 0;
#endif
}

#endif

#pragma endregion