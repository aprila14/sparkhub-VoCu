#include "misc.h"

#include <time.h>


static const char *level_strings[] = {
    "-", "-", "CRITI", "ERROR", "WARNI", "NOTIC", "INFO ", "DEBUG"
};

static const char *level_colors[] = {
    "", "", "\x1b[35m", "\x1b[31m", "\x1b[33m", "\x1b[33m", "\x1B[32m", "\x1b[36m"
};


void blzlibLogHandler(enum loglevel logLevel, const char* fmt, va_list ap)
{
    char timeStr[20];
    struct tm *sTm;
    time_t now = time(0);
    sTm = gmtime (&now);
    strftime(timeStr, sizeof(timeStr), "%H:%M:%S", sTm);

    printf("%s %s[%s]: ", timeStr, level_colors[logLevel], level_strings[logLevel]);
    vprintf(fmt, ap);
    printf("\x1B[0m \n\r");
    fflush(stdout);
}
