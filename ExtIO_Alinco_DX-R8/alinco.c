#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "alinco.h"
#include "logging.h"

#define SERIAL_BUFFER_SIZE  128
#define DXR8_BAUD           9600

/*
 *
 */

EAlinco Alinco_init(t_alinco **p)
{
    char        szConfigFile[1024];

    *p = calloc(1, sizeof(t_alinco));

    /*
     * Find ini file path
     */

    GetModuleFileName(NULL, szConfigFile, 1024);
    int n = strlen(szConfigFile) - 1;
    while (n) {

        if (szConfigFile[n] == '\\') {

            strcpy(&szConfigFile[n + 1], CONFIG_FILE_NAME);
            break;
        }
        if (n == 0) {

            log_error("Can't open config file");

            return ALINCO_FAILED;
        }
        n--;
    }

    /*
     * Load port config
     */


    (*p)->bInit = GetPrivateProfileInt("CONFIG", "init", 0, szConfigFile);
    (*p)->bWait = GetPrivateProfileInt("CONFIG", "wait", 0, szConfigFile);

    return ALINCO_OK;
}

EAlinco Alinco_open(t_alinco *p, const char *szDevice)
{
    ESerial     serialRet;
    t_serial    *pSerial;
    char        buff[SERIAL_BUFFER_SIZE];


    //    pSerial = new_serial();
    serial_init(&pSerial);
    serialRet = serial_open(pSerial, szDevice, DXR8_BAUD);
    if (serialRet != SERIAL_OK) {

        printf("serial_open failed\n");
        log_debug("Can't open serial port [%s]", szDevice);
        free(pSerial);

        return ALINCO_FAILED;
    }
    printf("pSerial: %p\n", pSerial);

    p->pSerial = pSerial;

    if (p->bInit == 1) {

        snprintf(buff, SERIAL_BUFFER_SIZE, "AL~RW_RFM05\r\n");
        log_debug(buff);
        serial_write(pSerial, buff, strlen(buff));
        if (p->bWait == 1) {
        }
        else {
            Sleep(1);
        }

        snprintf(buff, SERIAL_BUFFER_SIZE, "AL~SW_IQM01\r\n");
        log_debug(buff);
        serial_write(pSerial, buff, strlen(buff));
        if (p->bWait == 1) {
        }
        else {
            Sleep(1);
        }

        snprintf(buff, SERIAL_BUFFER_SIZE, "AL~SW_IQF 0000\r\n");
        log_debug(buff);
        serial_write(pSerial, buff, strlen(buff));
        if (p->bWait == 1) {
        }
        else {
            Sleep(1);
        }
    }

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

EAlinco Alinco_setModeSDR(t_alinco *p, int bSdrMode)
{
    return ALINCO_OK;
}
