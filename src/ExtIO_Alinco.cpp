#include <Windows.h>
#include <stdio.h>
#include <string.h>
#include "alinco.h"
#include "logging.h"
#include "ExtIO.h"
#include "LC_ExtIO_Types.h"

#ifndef EXTIO_LIBRARY_NAME
#error "EXTIO_LIBRARY_NAME not defined"
#endif /*EXTIO_LIBRARY_NAME*/

#ifndef CONFIG_FILE_NAME
#error "CONFIG_FILE_NAME not defined"
#endif /*CONFIG_FILE_NAME*/

/*
 *
 */

t_alinco *gl_pAlinco = NULL;

/*
 * Mandatory ExtIO func
 */


extern "C"
bool EXTIO_ENTRY InitHW(char *name, char *model, int &type)
{
    e_alinco_model alinco_model = ALINCO_MODEL_DJX11;

    set_log_level(LOG_LEVEL_DEBUG);
    log_debug("InitHW()");

#if defined(TARGET_MODEL_DJX11)
    alinco_model = ALINCO_MODEL_DJX11;
#elif defined(TARGET_MODEL_DXR8)
    alinco_model = ALINCO_MODEL_DXR8;
#else
#error "TARGET_MODEL_XXXXX not defined"
#endif

    /*
     * Open Alinco device
     */

    int r = Alinco_init(&gl_pAlinco, alinco_model);
    if (r != ALINCO_OK) {

        char szText[128];
        char szCaption[128];

        log_error("Alinco_init(0x%p) failed: %d", gl_pAlinco, r);
        snprintf(szText, 128, "Alinco_init() failed: %d", r);
        snprintf(szCaption, 128, "%s : %s", EXTIO_LIBRARY_NAME, __FUNCTION__);
        MessageBox(NULL, szText, szCaption, MB_OK | MB_ICONWARNING);

        return false;
    }

    /*
     * "Return" name, model and type
     */

    strncpy(name, "Alinco", EXTIO_MAX_NAME_LEN);
    strncpy(model, gl_pAlinco->pSettings->szModel, EXTIO_MAX_MODEL_LEN);
    type = exthwSCdata;

    return true;
}

extern "C"
bool EXTIO_ENTRY OpenHW(void)
{
    char    szModulePath[1024];
    char    szPort[128];
    DWORD   gppsr;

    log_debug("OpenHW()");

    /*
     * Find ini file path
     * Convert "C:\Program (x86)\HDSDR\HDSDR.exe" to "C:\Program (x86)\HDSDR\ExtIO_alinco_djx11.ini"
     */

    GetModuleFileName(NULL, szModulePath, 1024);
    size_t n = strlen(szModulePath) - 1;
    while (n) {

        /* Find last '\' and replace prog name by config file name */
        if (szModulePath[n] == '\\') {

            strcpy(&szModulePath[n + 1], CONFIG_FILE_NAME);
            break;
        }
        if (n == 0) {

            log_error("GetModuleFileName() failed");
            return false;
        }
        n--;
    }
    log_debug("szModulePath=\"%s\"", szModulePath);

    /*
     * Load port config
     */

    gppsr = GetPrivateProfileString("CONFIG", "port", "com1", szPort, 128, szModulePath);
    if (gppsr == 0) {

        char buff[128];

        log_error("GetPrivateProfileString() failed: %d", gppsr);
        snprintf(buff, 128, "GetPrivateProfileString(%s) failed\r\nreturned: %d", szPort, gppsr);
        MessageBox(NULL, buff, EXTIO_LIBRARY_NAME, MB_OK | MB_ICONWARNING);

        return false;
    }
    log_debug("szPort=\"%s\"", szPort);

    gl_pAlinco->bWait = GetPrivateProfileInt("CONFIG", "wait", 0, szModulePath);

    Alinco_open(gl_pAlinco, szPort);
 
    return true;
}

extern "C"
int EXTIO_ENTRY SetHWLO(long extLOfreq)
{
    log_debug("SetHWLO(%ld)", extLOfreq);

    if (extLOfreq < gl_pAlinco->pSettings->vfoMin) {

        log_warning("extLOfreq below MIN: %d < %d", extLOfreq, gl_pAlinco->pSettings->vfoMin);
        Alinco_setVFO1(gl_pAlinco, gl_pAlinco->pSettings->vfoMin);
        return gl_pAlinco->pSettings->vfoMin * -1;
    }
    if (extLOfreq >= gl_pAlinco->pSettings->vfoMax) {

        log_warning("extLOfreq above MAX: %d > %d", extLOfreq, gl_pAlinco->pSettings->vfoMax);
        Alinco_setVFO1(gl_pAlinco, gl_pAlinco->pSettings->vfoMax);
        return gl_pAlinco->pSettings->vfoMax;
    }

    Alinco_setVFO1(gl_pAlinco, extLOfreq);

    return 0;
}

extern "C"
int EXTIO_ENTRY StartHW(long extLOfreq)
{
    log_debug("StartHW(%ld)", extLOfreq);

    Alinco_setModeSDR(gl_pAlinco, true);
    Alinco_setVFO1(gl_pAlinco, extLOfreq);

    // No complex returned by callback
    return 0;
}

extern "C"
void EXTIO_ENTRY StopHW(void)
{
    log_debug("StopHW()");

    Alinco_setModeSDR(gl_pAlinco, false);
}

extern "C"
void EXTIO_ENTRY SetCallback(pfnExtIOCallback funcptr)
{
    log_debug("SetCallback(0x%p)", funcptr);
}

extern "C"
int EXTIO_ENTRY GetStatus(void)
{
    log_debug("GetStatus()");

    return 0;
}


extern "C"
void EXTIO_ENTRY CloseHW(void)
{
    log_debug("CloseHW()");

    Alinco_close(gl_pAlinco);
    Alinco_release(gl_pAlinco);

    gl_pAlinco = NULL;
}
