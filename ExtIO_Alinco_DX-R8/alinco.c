#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "alinco.h"
#include "logging.h"

/*
 *
 */

EAlinco Alinco_init(t_alinco **p)
{
    *p = malloc(sizeof(t_alinco));
    memset(*p, 0, sizeof(t_alinco));

    return ALINCO_OK;
}

EAlinco Alinco_open(t_alinco *p, const char *szDevice)
{
    t_serial    *pSerial = NULL;
    ESerial     serialRet;

    serial_init(&pSerial);
    serialRet = serial_open(pSerial, szDevice, DXR8_BAUD);
    if (serialRet != SERIAL_OK) {

        log_error("Can't open serial port [%s]", szDevice);
        free(pSerial);

        return ALINCO_FAILED;
    }

    p->pSerial = pSerial;

    return ALINCO_OK;
}

EAlinco Alinco_close(t_alinco *p)
{
    if (p->pSerial != NULL) {

        serial_close(p->pSerial);
        free(p->pSerial);
    }
    p->pSerial = NULL;

    return ALINCO_OK;
}

EAlinco Alinco_release(t_alinco *p)
{
    if (p->pSerial != NULL) {

        Alinco_close(p);
    }
    free(p);

    return ALINCO_OK;
}

EAlinco Alinco_setVFO1(t_alinco *p, uint32_t freq)
{
    char buff[SERIAL_BUFFER_SIZE];

    if ((freq < DXR8_MIN_FREQUENCY_HZ) || (freq > DXR8_MAX_FREQUENCY_HZ)) {
        return ALINCO_OUTOFRANGE;
    }

    sprintf(buff, "AL~RW_RXF%08d\r\n", freq);
    log_debug("send freq change query");
    log_debug(buff);
    serial_write(p->pSerial, buff, strlen(buff));
    buff[0] = 0;
    if (p->bWait == 1) {

        log_debug("waiting answer");
        serial_read(p->pSerial, buff, SERIAL_BUFFER_SIZE);
        log_debug(buff);
    }

    return ALINCO_OK;
}

EAlinco Alinco_getVFO1(t_alinco *p, uint32_t *freq)
{
    *freq = 7030000;

    return ALINCO_OK;
}

EAlinco Alinco_setModeSDR(t_alinco *p, bool bSdrMode)
{
    char    buff[SERIAL_BUFFER_SIZE];

    if (bSdrMode == true) {

        // FIXME: Little doubt here, switch to FM ?
        snprintf(buff, SERIAL_BUFFER_SIZE, "AL~RW_RFM05\r\n");
        log_debug(buff);
        serial_write(p->pSerial, buff, strlen(buff));
        if (p->bWait == 1) {
        }
        else {
            Sleep(1);
        }

        snprintf(buff, SERIAL_BUFFER_SIZE, "AL~SW_IQM01\r\n");
        log_debug(buff);
        serial_write(p->pSerial, buff, strlen(buff));
        if (p->bWait == 1) {
        }
        else {
            Sleep(1);
        }

        snprintf(buff, SERIAL_BUFFER_SIZE, "AL~SW_IQF 0000\r\n");
        log_debug(buff);
        serial_write(p->pSerial, buff, strlen(buff));
        if (p->bWait == 1) {
        }
        else {
            Sleep(1);
        }
    }
    else {

        // FIXME: Quit IQ mode
    }

    return ALINCO_OK;
}
