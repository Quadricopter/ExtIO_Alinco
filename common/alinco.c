#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "alinco.h"
#include "logging.h"

/*
 *
 */

EAlinco Alinco_init(t_alinco **p, e_alinco_model model)
{
    *p = malloc(sizeof(t_alinco));
    memset(*p, 0, sizeof(t_alinco));
    (*p)->model = model;

    return ALINCO_OK;
}

EAlinco Alinco_open(t_alinco *p, const char *szDevice)
{
    t_serial    *pSerial = NULL;
    ESerial     serialRet;
    int         baud = 0;

    switch (p->model) {
    case ALINCO_MODEL_DJX11:
        baud = DJX11_BAUD;
        break;
    case ALINCO_MODEL_DXR8:
        baud = DXR8_BAUD;
        break;
    default:
        log_error("unsupported model: %d", p->model);
    }

    serial_init(&pSerial);
    serialRet = serial_open(pSerial, szDevice, baud);
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

    switch (p->model) {

    case ALINCO_MODEL_DJX11:
        if ((freq < DJX11_MIN_FREQUENCY_HZ) || (freq > DJX11_MAX_FREQUENCY_HZ)) {

            return ALINCO_OUTOFRANGE;
        }
        sprintf(buff, "AL~FW0%010d\r", freq);
        serial_write(p->pSerial, buff, strlen(buff));
        serial_read(p->pSerial, buff, SERIAL_BUFFER_SIZE);
        if (strcmp(buff, "\r\nOK\r\n")) {

            return ALINCO_FAILED;
        }
        break;

    case ALINCO_MODEL_DXR8:
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
        break;
    }

    return ALINCO_OK;
}

EAlinco Alinco_getVFO1(t_alinco *p, uint32_t *freq)
{
    char buff[SERIAL_BUFFER_SIZE];
    unsigned int tmp;

    switch (p->model) {

    case ALINCO_MODEL_DJX11:
        sprintf(buff, "AL~FR0\r");
        serial_write(p->pSerial, buff, strlen(buff));
        serial_read(p->pSerial, buff, SERIAL_BUFFER_SIZE);

        sscanf(buff, "\r\n%d\r\n", &tmp);
        *freq = tmp;
        break;

    case ALINCO_MODEL_DXR8:
    default:
        log_error("Unsupported model type %d", p->model);
        return ALINCO_FAILED;
    }

    return ALINCO_OK;
}

EAlinco Alinco_setModeSDR(t_alinco *p, bool bSdrMode)
{
    char buff[SERIAL_BUFFER_SIZE];

    switch (p->model) {

    case ALINCO_MODEL_DJX11:
        if (bSdrMode == true)
            strcpy(buff, "AL~SDRON\r");
        else
            strcpy(buff, "AL~SDROF\r");

        serial_write(p->pSerial, buff, strlen(buff));
        break;

    case ALINCO_MODEL_DXR8:
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
        break;
    }

    return ALINCO_OK;
}
