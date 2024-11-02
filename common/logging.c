#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>
#include "logging.h"


e_log_level g_log_level = DEFAULT_LOG_LEVEL;


void set_log_level(e_log_level level)
{
    g_log_level = level;
}

static const char *get_level_string(e_log_level level)
{
    switch (level) {
    case LOG_LEVEL_ERROR:
        return "ERROR";
        break;
    case LOG_LEVEL_WARNING:
        return "WARNING";
        break;
    case LOG_LEVEL_INFO:
        return "INFO";
        break;
    case LOG_LEVEL_DEBUG:
        return "DEBUG";
        break;
    default:
        return "UNDEF";
    }

    return NULL;
}

void logging(e_log_level level, const char *function, const int line, const char *format, ...)
{
    if ((g_log_level > LOG_LEVEL_DISABLED) &&
        (g_log_level >= level)) {

        FILE    *f = NULL;

        f = fopen(LOGGING_FILENAME, "a");
        if (f) {

            va_list     ap;
            char        time_buff[64];
            time_t      now = 0;
            struct tm   *lt;

            va_start(ap, format);

            /* Timestamp */
            time(&now);
            lt = localtime(&now);
            sprintf(time_buff, "%04d-%02d-%02dT%02d:%02d:%02d",
                    lt->tm_year + 1900, lt->tm_mon + 1, lt->tm_mday,
                    lt->tm_hour, lt->tm_min, lt->tm_sec);
            fprintf(f, "%s ", time_buff);

            /* File, line, level header */
            fprintf(f, "[%s:%d] <%s> ", function, line, get_level_string(level));

            /* Message */
            vfprintf(f, format, ap);
            fprintf(f, LOGGING_EOL);
            fclose(f);

            va_end(ap);
    
        }
    }
}
