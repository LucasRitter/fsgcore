#pragma once

#include "Primitives.hpp"

namespace FSGSystem
{
#pragma region Time and Date

    /**
     * Container for Time and Date.
     * @note This closely resembles the standard tm struct, but with 16 byte values, instead of 32.
     */
    struct FSGTimeAndDate
    {
        public:
        i16 seconds;
        i16 minutes;
        i16 hour;
        i16 day;
        i16 month;
        i16 year;

        static int CompareDates(const FSGTimeAndDate& dateA, const FSGTimeAndDate& dateB);

        bool operator<(const FSGTimeAndDate& date) const;
    };

    /**
     * Fills the given @b fsgDate with the current time and fsgDate.
     * @param fsgDate The structure to fill.
     * @note Custom implementation. Differs from original fsgcore library.
     */
    void FSGGetTimeAndDate(FSGTimeAndDate& fsgDate);

    /**
     * Fills the given @b buildDate with the build time and date.
     * @param buildDate The structure to fill.
     * @note Custom implementation. Differs from original fsgcore library.
     */
    void FSGGetBuildDateAndTime(FSGTimeAndDate& buildDate);

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
    void* FSGCameraGetVideoBuffer(void* videoBuffer);

    /**
     * Unused.
     */
    void FSGCameraGetResolution(i32& width, i32& height);

#pragma endregion
}  // namespace FSGSystem
