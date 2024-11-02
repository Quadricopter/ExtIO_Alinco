#ifndef __SERIAL_H__
#define __SERIAL_H__

#ifdef _WIN32

#include <windows.h>

typedef struct  {

    HANDLE hPort;
}   t_serial;

#else

#define READ_CACHE_SIZE 128

typedef struct  {

    int     fd;
    char    *szDevice;
}   t_serial;

#endif /*WIN32*/


/*
 *
 */

typedef enum {

    SERIAL_OK = 0,
    SERIAL_OPEN_FAILED,
    SERIAL_CONFIG_FAILED,
    SERIAL_READ_FAILED,
    SERIAL_WRITE_FAILED,
#ifdef _WIN32
    SERIAL_SETSTATE_FAILED,
    SERIAL_GETSTATE_FAILED,
    SERIAL_SETTIMEOUT_FAILED,
#endif
    SERIAL_ERROR_MAX
} ESerial;

/*
**
*/

#ifdef __cplusplus
extern "C" {
#endif

ESerial serial_init(t_serial **pSerial);
ESerial serial_open(t_serial *pSerial, const char *szDevice, int speed);
ESerial serial_close(t_serial *pSerial);
int     serial_read(t_serial *pSerial, char *buff, int maxsize);
int     serial_write(t_serial *pSerial, const char *buff, size_t len);

#ifdef __cplusplus
}
#endif

#endif /* __SERIAL_H__ */
