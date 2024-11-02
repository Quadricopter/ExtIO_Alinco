#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "logging.h"
#include "serial.h"

ESerial serial_init(t_serial **pSerial)
{
    *pSerial = malloc(sizeof(t_serial));
    memset(*pSerial, 0, sizeof(t_serial));

    return SERIAL_OK;
}

ESerial serial_open(t_serial *pSerial, const char *szDevice, int speed)
{
    char filename[16];

    /* Append "\\.\" header (physical drive) */
    snprintf(filename, 16, "\\\\.\\%s", szDevice);
    pSerial->hPort = CreateFile(filename,
                                GENERIC_READ | GENERIC_WRITE,
                                0, // Exclusive access
                                NULL,
                                OPEN_EXISTING,
                                0,
                                NULL); // No file attribute
    if (pSerial->hPort == INVALID_HANDLE_VALUE) {

        pSerial->hPort = NULL;
        log_error("CreateFile(%s, ...) failed", filename);

        return SERIAL_OPEN_FAILED;
    }

    DCB dcbSerialParams = { 0 };
    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
    if (!GetCommState(pSerial->hPort, &dcbSerialParams)) {

        //could not get the state of the comport
        serial_close(pSerial);
        log_error("GetCommState() failed");

        return SERIAL_GETSTATE_FAILED;
    }

    dcbSerialParams.BaudRate = speed;
    dcbSerialParams.ByteSize = 8;
    dcbSerialParams.StopBits = ONESTOPBIT;
    dcbSerialParams.Parity = NOPARITY;

    if (!SetCommState(pSerial->hPort, &dcbSerialParams)){

        //analyse error
        serial_close(pSerial);
        log_error("SetCommState() failed");

        return SERIAL_SETSTATE_FAILED;
    }

    COMMTIMEOUTS    timeouts = { 0 };
//    timeouts.ReadIntervalTimeout = 50;
//    timeouts.ReadTotalTimeoutConstant = 50;
//    timeouts.ReadTotalTimeoutMultiplier = 1;
//    timeouts.WriteTotalTimeoutConstant = 50;
//    timeouts.WriteTotalTimeoutMultiplier = 1;

    timeouts.ReadIntervalTimeout = MAXDWORD;
    timeouts.ReadTotalTimeoutConstant = 0;
    timeouts.ReadTotalTimeoutMultiplier = 0;
    timeouts.WriteTotalTimeoutConstant = 0;
    timeouts.WriteTotalTimeoutMultiplier = 0;

    if (!SetCommTimeouts(pSerial->hPort, &timeouts)){

        //handle error
        serial_close(pSerial);
        log_error("SetCommTimeouts() failed");

        return SERIAL_SETTIMEOUT_FAILED;
    }

    return SERIAL_OK;
}

ESerial serial_close(t_serial *pSerial)
{
    if (pSerial) {

        if (pSerial->hPort) {

            CloseHandle(pSerial->hPort);
        }
    }

    return SERIAL_OK;
}

int	serial_read(t_serial *pSerial, char *buff, int maxsize)
{
    DWORD dwNumberOfBytesRead = 0;

    if (ReadFile(pSerial->hPort, buff, maxsize, &dwNumberOfBytesRead, NULL) == FALSE) {

        log_error("ReadFile() failed");
    }

    return dwNumberOfBytesRead;
}

int	serial_write(t_serial *pSerial, const char *buff, size_t len)
{
    DWORD dwBytesRead = 0;

    if(!WriteFile(pSerial->hPort, (char*)buff, strlen(buff), &dwBytesRead, NULL)) {

        log_error("WriteFile() failed");
    }

    return dwBytesRead;
}
