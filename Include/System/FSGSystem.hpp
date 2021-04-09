#pragma once

#include "Primitives.hpp"

namespace FSGSystem {
#pragma region Time and Date

    /**
     * Container for Time and Date.
     * @note This closely resembles the standard tm struct, but with 16 byte values, instead of 32.
     */
    struct SFSGTimeAndDate {
    public:
        i16 m_seconds;
        i16 m_minutes;
        i16 m_hour;
        u16 m_day;
        u16 m_month;
        u16 m_year;

        static int CompareDates(const SFSGTimeAndDate &t_dateA, const SFSGTimeAndDate &t_dateB);

        bool operator<(const SFSGTimeAndDate &t_date) const;
    };

    /**
     * Fills the given @b t_date with the current time and date.
     * @param t_date The structure to fill.
     * @note Custom implementation. Differs from original fsgcore library.
     */
    void FSGGetTimeAndDate(SFSGTimeAndDate &t_date);

    /**
     * Fills the given @b t_buildDate with the build time and date.
     * @param t_buildDate The structure to fill.
     * @note Custom implementation. Differs from original fsgcore library.
     */
    void FSGGetBuildDateAndTime(SFSGTimeAndDate &t_buildDate);

#pragma endregion

#pragma region Game Information

    /**
     * @returns The build date as a @b static_string.
     */
    static_string FSGGetBuildDateAsString();

    /**
     * @returns The build time as a @b static_string.
     */
    static_string FSGGetBuildTimeAsString();

    /**
     * @returns The game name as a @b static_string.
     */
    static_string FSGGetGameName();

    /**
     * @returns The short game name as a @b static_string.
     */
    static_string FSGGetGameNameShort();

    /**
     * @returns The game version as an @b i32.
     */
    i32 FSGGetGameVersion();

#pragma region Platform Specific

#if defined(WIN32) || defined(WIN64) || defined(UNIX) || defined(MACOSX)

    /**
     * @returns The application identifier as a @b u64.
     */
    u64 FSGGetDiscordApplicationId();

#endif

#pragma endregion
#pragma endregion

#pragma region Unused but included utilities

    /**
     * Unused.
     */
    void FSGCameraInitialise();

    /**
     * Unused.
     */
    void FSGCameraDestroy();

    /**
     * Unused.
     */
    void FSGCameraStart();

    /**
     * Unused.
     */
    void FSGCameraStop();

    /**
     * Unused.
     */
    void *FSGCameraGetVideoBuffer(void *t_videoBuffer);

    /**
     * Unused.
     */
    void FSGCameraGetResolution(i32 &t_width, i32 &t_height);

#pragma endregion
}
