// Please keep these 2 lines at the beginning of each cpp module
static const char* LOG_TAG = "NtpClient";
#define LOG_LOCAL_LEVEL ESP_LOG_INFO


#include "ntp_client.h"

#include "commons.h"
#include "sleep.h"

//#include "lwip/apps/sntp.h"
#include "esp_sntp.h"


namespace
{

/**
* @brief A global variable pointing to the "current" NtpClient.
* Unfortuntely we need to have global access to it, to call it from the NTP client callback from LWIP (see sntp_sync_time)
* We have one instance anyway, so it is not a big issue.
*/
NtpClient* g_pNtpClient = nullptr;

}  // local namespace


NtpClient::NtpClient()
:
m_ntpTimeOffsetMs(0)
{

}

void NtpClient::init()
{
    m_accessMutex = mutexCreate();
    g_pNtpClient = this;  // see note near g_pNtpClient
}

void NtpClient::runTask()   // NOLINT - we don't want to make it static
{
    LOG_INFO("NtpClient::runTask...");
    sntp_setoperatingmode(SNTP_OPMODE_POLL); // CONFIG_LWIP_SNTP_UPDATE_DELAY
    sntp_setservername(0, "pool.ntp.org");
    sntp_init();
}

bool NtpClient::waitForNtpTimeSynchronization(int timeoutMs) const
{
    LOG_INFO("Wait for NTP time to be synchronized (but only %d ms)...", static_cast<int>(timeoutMs));

    int64_t timeoutTime = commons::getCurrentTimestampMs() + timeoutMs;

    while (getNtpTimeOffset() == NTP_TIME_OFFSET_NOT_SET)
    {
        LOG_DEBUG("Waiting for NTP synchronization...");
        SLEEP_MS(1000);

        if (commons::getCurrentTimestampMs() > timeoutTime)
        {
            LOG_WARNING("Timeout while waiting for NTP time!");
            return false;
        }
    }

    LOG_DEBUG("waitForNtpTimeSynchronization - NTP time synchronized!");
    return true;
}

bool NtpClient::isTimeSynchronized() const
{
    return (getNtpTimeOffset() != NTP_TIME_OFFSET_NOT_SET);
}

int64_t NtpClient::getNtpTimeOffset() const
{
    LOCK_GUARD(m_accessMutex, accessMutexGuard);
    return m_ntpTimeOffsetMs;
}

int64_t NtpClient::getCurrentUtcTimestampMs() const
{
    int64_t ntpTimeOffsetMs = getNtpTimeOffset();

    if (ntpTimeOffsetMs == NTP_TIME_OFFSET_NOT_SET)
    {
        LOG_ERROR("NTP time requested while not synchronized!");
        return 0;
    }

    int64_t currentUtcTimestampMs = commons::getCurrentTimestampMs() + ntpTimeOffsetMs;
    return currentUtcTimestampMs;
}

int16_t NtpClient::getCurrentMinuteOfWeek(int16_t timezoneOffsetInMinutes) const
{
    int64_t currentUtcTimestampMs = getCurrentUtcTimestampMs();

    int16_t minuteOfWeek = calculateCurrentMinuteOfWeek(timezoneOffsetInMinutes, currentUtcTimestampMs);

    return minuteOfWeek;
}

int16_t NtpClient::calculateCurrentMinuteOfWeek(int16_t timezoneOffsetInMinutes, int64_t currentUtcTimestampMs)
{
    int64_t timezoneOffsetS = static_cast<int64_t>(timezoneOffsetInMinutes) * SEC_IN_MIN;
    int64_t currentUtcTimestampS = currentUtcTimestampMs / MS_IN_S;

    int64_t currentLocalTimeS = timezoneOffsetS + currentUtcTimestampS;

    struct tm timeinfo = {};
    time_t now = currentLocalTimeS;
    gmtime_r(&now, &timeinfo);

    int weekDaysSinceMonday = (timeinfo.tm_wday + 7 - 1) % 7; //
    int16_t minuteOfWeek = (weekDaysSinceMonday * HOURS_IN_DAY + timeinfo.tm_hour) * MIN_IN_HOUR + timeinfo.tm_min;


    LOG_DEBUG(
        "time: %d:%d:%d dayOfWeek=[%d], weekDaysSinceMonday=%d |"
        " %d/%d/%d   ===> "
        "minuteOfWeek = %d",
        timeinfo.tm_hour,
        timeinfo.tm_min,
        timeinfo.tm_sec,
        timeinfo.tm_wday,
        weekDaysSinceMonday,
        timeinfo.tm_mday,
        timeinfo.tm_mon,
        timeinfo.tm_year,
        minuteOfWeek);
    // LOG_DEBUG("UTC:       %s", asctime(gmtime(&now)));
    // LOG_DEBUG("local:     %s", asctime(localtime(&now)));

    return minuteOfWeek;
}

void NtpClient::sntp_sync_time(timeval* tv)
{
    LOG_INFO("sntp_sync_time Time synchronized - %d sec", static_cast<int>(tv->tv_sec));

    // settimeofday(tv, NULL); - we don't want to overwirte our steady clock,
    // instead just save the time difference
    int64_t systemTimestampMs = commons::getCurrentTimestampMs();
    int64_t ntpTimestampMs = static_cast<int64_t>(tv->tv_sec) * MS_IN_S;

    setTimestampOffset(systemTimestampMs, ntpTimestampMs);

    sntp_set_sync_status(SNTP_SYNC_STATUS_COMPLETED);
}

void NtpClient::printCurrentTime(int16_t timezoneOffsetInMinutes) const
{
    int64_t currentUtcTimestampMs = getCurrentUtcTimestampMs();
    int64_t currentUtcTimestampS = currentUtcTimestampMs / MS_IN_S;

    {
        // utc
        time_t now = currentUtcTimestampS;
        LOG_INFO("Current UTC time is: %s", asctime(gmtime(&now)));
    }

    {
        // local
        int64_t timezoneOffsetS = static_cast<int64_t>(timezoneOffsetInMinutes) * SEC_IN_MIN;
        int64_t currentLocalTimeS = timezoneOffsetS + currentUtcTimestampS;
        time_t now = currentLocalTimeS;
        LOG_INFO("Current local time is: %s", asctime(gmtime(&now)));
    }
}

char* NtpClient::getCurrentLocalTimeString(int16_t timezoneOffsetInMinutes) const
{
    int64_t currentUtcTimestampMs = getCurrentUtcTimestampMs();
    int64_t currentUtcTimestampS = currentUtcTimestampMs / MS_IN_S;
    int64_t timezoneOffsetS = static_cast<int64_t>(timezoneOffsetInMinutes) * SEC_IN_MIN;
    int64_t currentLocalTimeS = timezoneOffsetS + currentUtcTimestampS;
    time_t now = currentLocalTimeS;
    return asctime(gmtime(&now));
}

void NtpClient::setTimestampOffset(int64_t systemTimestampMs, int64_t ntpTimestampMs)
{
    LOCK_GUARD(m_accessMutex, accessMutexGuard);
    m_ntpTimeOffsetMs = ntpTimestampMs - systemTimestampMs;
}

/**
 * @brief Overwrites weak function sntp_sync_time from sntp lwip app
 * // void __attribute__((weak)) sntp_sync_time(struct timeval *tv)
 * @param tv Current time received from NTP
 */
void sntp_sync_time(struct timeval* tv)
{
    g_pNtpClient->sntp_sync_time(tv);
}

