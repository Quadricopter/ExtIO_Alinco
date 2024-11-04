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
    serialRet = serial_open(pSerial, szDevice, DJX11_BAUD);
    if (serialRet != SERIAL_OK) {

        log_error("serial_open(%s) failed", szDevice);
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
    if (p->pSerial) {

        Alinco_close(p);
    }
    free(p);

    return ALINCO_OK;
}

EAlinco Alinco_setVFO1(t_alinco *p, uint32_t freq)
{
    char buff[SERIAL_BUFFER_SIZE];

    if ((freq < DJX11_MIN_FREQUENCY_HZ) || (freq > DJX11_MAX_FREQUENCY_HZ)) {

        return ALINCO_OUTOFRANGE;
    }
    sprintf(buff, "AL~FW0%010d\r", freq);
    serial_write(p->pSerial, buff, strlen(buff));
    serial_read(p->pSerial, buff, SERIAL_BUFFER_SIZE);
    if (strcmp(buff, "\r\nOK\r\n")) {

        return ALINCO_FAILED;
    }

    return ALINCO_OK;
}

EAlinco Alinco_getVFO1(t_alinco *p, uint32_t *freq)
{
    char buff[SERIAL_BUFFER_SIZE];
    unsigned int tmp;

    sprintf(buff, "AL~FR0\r");
    serial_write(p->pSerial, buff, strlen(buff));
    serial_read(p->pSerial, buff, SERIAL_BUFFER_SIZE);

    sscanf(buff, "\r\n%d\r\n", &tmp);
    *freq = tmp;

    return ALINCO_OK;
}

EAlinco Alinco_setModeSDR(t_alinco *p, bool bSdrMode)
{
    char buff[SERIAL_BUFFER_SIZE];

    if (bSdrMode == true)
        strcpy(buff, "AL~SDRON\r");
    else
        strcpy(buff, "AL~SDROF\r");

    serial_write(p->pSerial, buff, strlen(buff));

    return ALINCO_OK;
}
