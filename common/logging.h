#ifndef __LOGGING_H__
#define __LOGGING_H__

#include <stdarg.h>

/*
 * Destination filename
 */

#ifndef LOGGING_FILENAME
#define LOGGING_FILENAME  "log.txt"
#endif /*LOGGING_FILENAME*/


#define DEFAULT_LOG_LEVEL   LOG_LEVEL_ERROR
#define LOGGING_EOL  "\n"


/*
 * Helpers
 */

typedef enum {

    LOG_LEVEL_DISABLED = 0,
    LOG_LEVEL_ERROR,
    LOG_LEVEL_WARNING,
    LOG_LEVEL_INFO,
    LOG_LEVEL_DEBUG
} e_log_level;

#define log_error(...)   logging(LOG_LEVEL_ERROR,   __FUNCTION__, __LINE__, __VA_ARGS__)
#define log_warning(...) logging(LOG_LEVEL_WARNING, __FUNCTION__, __LINE__, __VA_ARGS__)
#define log_info(...)    logging(LOG_LEVEL_INFO,    __FUNCTION__, __LINE__, __VA_ARGS__)
#define log_debug(...)   logging(LOG_LEVEL_DEBUG,   __FUNCTION__, __LINE__, __VA_ARGS__)


/*
 * Function
 */

#ifdef __cplusplus
extern "C" {
#endif

void set_log_level(e_log_level level);
void logging(e_log_level level, const char *function, const int line, const char *format, ...);

#ifdef __cplusplus
}
#endif

#endif /*__LOGGING_H__*/
