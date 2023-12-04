#ifndef NTP_CLIENT_H
#define NTP_CLIENT_H

#include "defines.h"
#include "mutex.h"

namespace UtcOffset
{
    constexpr int16_t OFFSET_UTC__1 = -60;
    constexpr int16_t OFFSET_UTC__2 = -120;
    constexpr int16_t OFFSET_UTC__3 = -180;
    constexpr int16_t OFFSET_UTC_1 = 60;
    constexpr int16_t OFFSET_UTC_2 = 120;
    constexpr int16_t OFFSET_UTC_3 = 180;
}

class NtpClient
{
public:
    const int64_t NTP_TIME_OFFSET_NOT_SET = -1;

    NtpClient();

    /**
     * @brief init control structure
     */
    void init();

    /**
     * @brief start the NTP synchronization, should be called after WiFi is on
     */
    void runTask();

    /**
     * @brief Wait until time is synchronized
     * @return
     */
    bool waitForNtpTimeSynchronization(int timeoutMs) const;

    /**
     * @brief Whether time was obtained from NTP
     * @return
     */
    bool isTimeSynchronized() const;

    /**
     * @brief Get difference between NTP time and local time since ESP startup
     * @return value in ms
     */
    int64_t getNtpTimeOffset() const;

    /**
     * @brief getCurrentUtcTimestampMs. Make sure that the time is syncronized first (e.g check getNtpTimeOffset() value)
     * @return
     */
    int64_t getCurrentUtcTimestampMs() const;

    /**
     * @brief get minute of the current week (available after NTP synchronization)
     */
    int16_t getCurrentMinuteOfWeek(int16_t timezoneOffsetInMinutes) const;

    static int16_t calculateCurrentMinuteOfWeek(int16_t timezoneOffsetInMinutes, int64_t currentUtcTimestampMs);

    /**
     * @brief Called from a global callback from lwip (sntp_sync_time) - therefore we use the same naming as in LWIP here
     * @param tv Current NTP time
     */
    void sntp_sync_time(struct timeval* tv);

    void printCurrentTime(int16_t timezoneOffsetInMinutes = 0) const;

    char* getCurrentLocalTimeString(int16_t timezoneOffsetInMinutes = 0) const;

#if !TESTING
private:
#endif

    void setTimestampOffset(int64_t systemTimestampMs, int64_t ntpTimestampMs);

    mutex_t m_accessMutex = nullptr;
    int64_t m_ntpTimeOffsetMs;  // Difference between NTP time and system clock
};

#endif // NTP_CLIENT_H
