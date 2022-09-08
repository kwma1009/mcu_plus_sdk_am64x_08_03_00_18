/*
* This source file is part of the EtherCAT Slave Stack Code licensed by Beckhoff Automation GmbH & Co KG, 33415 Verl, Germany.
* The corresponding license agreement applies. This hint shall not be removed.
* https://www.beckhoff.com/media/downloads/slave-stack-code/ethercat_ssc_license.pdf
*/

/**
\addtogroup FC1100HW FC1100 Hardware Access
@{
*/

/**
\file    fc1100hw.c
\author EthercatSSC@beckhoff.com
\brief Implementation
Hardware access implementation for FC1100 hardware access on Win32 platforms

\version 5.13

<br>Changes to version V5.10:<br>
V5.13 HW1: add fc11xx access changes<br>
V5.13 HW2: change timer implementation for fc11xx on WinOS<br>
<br>Changes to version V5.0:<br>
V5.01 HW1: Invalid ESC access function was used<br>
<br>Changes to version V4.42:<br>
V5.0 ESC4: Save SM disable/Enable. Operation may be pending due to frame handling.
*/

/*-----------------------------------------------------------------------------------------
------
------    Includes
------
-----------------------------------------------------------------------------------------*/
#ifdef _WIN32
#include <stdio.h>
#include <tchar.h>
#include "fc11xxAccess.h"
#endif

#include "ecat_def.h"
#if FC1100_HW


#define _FC1100_
#include "fc1100hw.h"
#undef _FC1100_
#include "ecatappl.h"

/*-----------------------------------------------------------------------------------------
------
------    Local Variables
------
-----------------------------------------------------------------------------------------*/

UINT16 * pFc1100Device;

TSYNCMAN    TmpSyncMan;

/*ECATCHANGE_START(V5.13) HW2*/
LARGE_INTEGER g_CurentCount, g_LastCount, g_diff;
/*ECATCHANGE_END(V5.13) HW2*/



/*-----------------------------------------------------------------------------------------
------
------    Local Functions
------
-----------------------------------------------------------------------------------------*/



UINT16* GetFc1100Pointer(void)
{
    pFc1100Device = (UINT16*)MapESCPointer(u16FcInstance);




    return pFc1100Device;
}

void FreeFc1100Handle(void)
{
    UnmapESCPointer(u16FcInstance);
    pFc1100Device = NULL;
 }

BOOL WINAPI ConsoleHandler(DWORD CEvent)
{
    switch(CEvent)
    {
    case CTRL_CLOSE_EVENT:
            bRunApplication = FALSE;
        break;
    }
    return TRUE;
}

/*-----------------------------------------------------------------------------------------
------
------    Global Functions
------
-----------------------------------------------------------------------------------------*/

#ifdef _DEBUG
/////////////////////////////////////////////////////////////////////////////////////////
/**
\brief    Writes messages to the debugger output
*////////////////////////////////////////////////////////////////////////////////////////
void DebugMessage(const char* pszFormat, ...)
{
    int nBuf;
    char szBuffer[512];

    va_list args;
    va_start(args, pszFormat);



    nBuf = _vsnprintf_s(szBuffer, sizeof(szBuffer), _TRUNCATE, pszFormat, args);

    OutputDebugString(szBuffer);

    va_end(args);
}
#endif 

/////////////////////////////////////////////////////////////////////////////////////////
/**
\return     0 if initialization was successful

 \brief    This function initialize the EtherCAT Slave Interface.
*////////////////////////////////////////////////////////////////////////////////////////

UINT16 HW_Init(void)
{
    /* the memory interface to the ESC, the ESC-interrupt and the ECAT-timer for the
       watchdog monitoring should be initialized here microcontroller specific*/

/*ECATCHANGE_START(V5.13) HW1*/
    /*unmap memory */
    FreeFc1100Handle();
/*ECATCHANGE_END(V5.13) HW1*/

    pEsc = (MEM_ADDR ESCMEM *) GetFc1100Pointer();
    if(pEsc == NULL)
    {
        return 1;
    }


    /* initialize the AL_Event Mask Register */
#if ESC_32BIT_ACCESS
    {
    UINT32 ResetValue = 0;
    HW_EscWriteDWord(ResetValue, ESC_AL_EVENTMASK_OFFSET);
    }
#else
    {
    UINT16 ResetValue = 0;
    HW_EscWriteWord(ResetValue, ESC_AL_EVENTMASK_OFFSET);
    }
#endif 

    return 0;
}

void HW_Release(void)
{
    FreeFc1100Handle();
}

#if BOOTSTRAPMODE_SUPPORTED
/////////////////////////////////////////////////////////////////////////////////////////
/**

 \brief    This function resets the hardware
*////////////////////////////////////////////////////////////////////////////////////////

void     HW_RestartTarget(void)
{
}
#endif /* BOOTSTRAPMODE_SUPPORTED */

#if ESC_EEPROM_EMULATION
/////////////////////////////////////////////////////////////////////////////////////////
/**
\return     0 if reload was successful

 \brief    This function is called when the master has request an EEPROM reload during EEPROM emulation

*////////////////////////////////////////////////////////////////////////////////////////
UINT16 HW_EepromReload (void)
{
    return 0;
}
#endif



UINT32 HW_GetTimer(void)
{
 
    /*ECATCHANGE_START(V5.13) HW2*/
    QueryPerformanceCounter(&g_CurentCount);
    g_diff.QuadPart = g_CurentCount.QuadPart - g_LastCount.QuadPart;
    return (UINT32)g_diff.QuadPart;
    /*ECATCHANGE_END(V5.13) HW2*/
}

void HW_ClearTimer(void)
{
    /*ECATCHANGE_START(V5.13) HW2*/
    QueryPerformanceCounter(&g_LastCount);
    /*ECATCHANGE_END(V5.13) HW2*/

    
}


#if UC_SET_ECAT_LED
void HW_SetLed(BOOL RunLed,BOOL ErrLed)
{
}
#endif //#if UC_SET_ECAT_LED

#endif //#if FC1100_HW

/** @} */

