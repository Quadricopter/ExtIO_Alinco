#ifndef __ALINCO_H__
#define __ALINCO_H__

#include <stdint.h>
#include <stdbool.h>
#include "serial.h"

/*
 *
 */

typedef struct s_alinco {

    t_serial    *pSerial;
    int         bInit;
    int         bWait;
}   t_alinco;

/*
 *
 */

#define DJX11_MIN_FREQUENCY_HZ       50000
#define DJX11_MAX_FREQUENCY_HZ  1300000000  
#define DXR8_MIN_FREQUENCY_HZ       150000
#define DXR8_MAX_FREQUENCY_HZ     35000000

/*
 *
 */

typedef enum {

    ALINCO_OK,
    ALINCO_SERIAL_OPEN_FAILED,
    ALINCO_SERIAL_CONFIG_FAILED,
    ALINCO_OUTOFRANGE,
    ALINCO_FAILED,
    ALINCO_ERROR_MAX
} EAlinco;

/*
 *
 */

#ifdef __cplusplus
extern "C" {
#endif

EAlinco Alinco_init(t_alinco **pAlinco);
EAlinco Alinco_open(t_alinco *p, const char *szDevice);
EAlinco Alinco_setVFO1(t_alinco *p, uint32_t freq);
EAlinco Alinco_getVFO1(t_alinco *p, uint32_t *freq);
EAlinco Alinco_setModeSDR(t_alinco *p, bool bSdrMode);
EAlinco Alinco_close(t_alinco *p);
EAlinco Alinco_release(t_alinco *p);

#ifdef __cplusplus
}
#endif


#endif /*__ALINCO_H__*/
