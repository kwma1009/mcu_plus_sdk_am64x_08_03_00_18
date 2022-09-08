/*
* This source file is part of the EtherCAT Slave Stack Code licensed by Beckhoff Automation GmbH & Co KG, 33415 Verl, Germany.
* The corresponding license agreement applies. This hint shall not be removed.
* https://www.beckhoff.com/media/downloads/slave-stack-code/ethercat_ssc_license.pdf
*/

/**
\addtogroup Bootloader Bootloader Sample
@{
*/

/**
\file Bootloader.c
\author EthercatSSC@beckhoff.com
\brief Implementation

\version 5.12

<br>Changes to version - :<br>
V5.12: file created
*/


/*-----------------------------------------------------------------------------------------
------
------    Includes
------
-----------------------------------------------------------------------------------------*/
#include "ecat_def.h"

#if BOOTLOADER_SAMPLE
#include "applInterface.h"


#define _BOOTLOADER_APPLICATION_
#include "bootloaderappl.h"
#undef _BOOTLOADER_APPLICATION_

#if BOOTSTRAPMODE_SUPPORTED
#include"bootmode.h"
#endif


/*--------------------------------------------------------------------------------------
------
------    local types and defines
------
--------------------------------------------------------------------------------------*/
#if FOE_SUPPORTED
#define    MAX_FILE_NAME_SIZE    16

/** \brief  MAX_FILE_SIZE: Maximum file size */
#if FC1100_HW
#define MAX_FILE_SIZE                             0x6400000 /*100MB*/
#else
#define MAX_FILE_SIZE                             0x180
#endif
#endif

/*-----------------------------------------------------------------------------------------
------
------    local variables and constants
------
-----------------------------------------------------------------------------------------*/
#if FOE_SUPPORTED
UINT32           nFileWriteOffset;
CHAR             aFileName[MAX_FILE_NAME_SIZE];
#if MBX_16BIT_ACCESS
UINT16 MBXMEM     aFileData[(MAX_FILE_SIZE >> 1)];
#else
UINT8 MBXMEM      aFileData[MAX_FILE_SIZE];
#endif //#else #ifMBX_16BIT_ACCESS


UINT32 u32FileSize;
#endif //#if FOE_SUPPORTED

/*-----------------------------------------------------------------------------------------
------
------    application specific functions
------
-----------------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------------
------
------    generic functions
------
-----------------------------------------------------------------------------------------*/
#if FOE_SUPPORTED
UINT16 FoE_Read(UINT16 MBXMEM * pName, UINT16 nameSize, UINT32 password, UINT16 maxBlockSize, UINT16 *pData)
{
    UINT16 i = 0;
    UINT16 sizeError = 0;

    CHAR aReadFileName[MAX_FILE_NAME_SIZE];


    if ((nameSize + 1) > MAX_FILE_NAME_SIZE)
    {

        return ECAT_FOE_ERRCODE_DISKFULL;
    }

    /*Read requested file name to endianess conversion if required*/
    MBXSTRCPY(aReadFileName, pName, nameSize);
    aReadFileName[nameSize] = '\0';

    if (u32FileSize == 0)
    {
        /* No file stored*/
        return ECAT_FOE_ERRCODE_NOTFOUND;
    }

    /* for test only the written file name can be read */
    for (i = 0; i < nameSize; i++)
    {
        if (aReadFileName[i] != aFileName[i])
        {
            /* file name not found */
            return ECAT_FOE_ERRCODE_NOTFOUND;
        }
    }

    sizeError = maxBlockSize;

    if (u32FileSize < sizeError)
    {
        sizeError = (UINT16)u32FileSize;
    }

    /*copy the first foe data block*/
    MEMCPY(pData, aFileData, sizeError);

    return sizeError;
}

UINT16 FoE_ReadData(UINT32 offset, UINT16 maxBlockSize, UINT16 *pData)
{
    UINT16 sizeError = 0;

    if (u32FileSize < offset)
    {
        return 0;
    }

    /*get file length to send*/
    sizeError = (UINT16)(u32FileSize - offset);


    if (sizeError > maxBlockSize)
    {
        /*transmit max block size if the file data to be send is greater than the max data block*/
        sizeError = maxBlockSize;
    }

    /*copy the foe data block 2 .. n*/
    MEMCPY(pData, &(((UINT8 *)aFileData)[offset]), sizeError);

    return sizeError;
}


UINT16 FoE_WriteData(UINT16 MBXMEM * pData, UINT16 Size, BOOL bDataFollowing)
{
#if BOOTSTRAPMODE_SUPPORTED
    if (bBootMode)
    {
        return BL_Data(pData, Size);
    }
    else
#endif
        if ((nFileWriteOffset + Size) > MAX_FILE_SIZE)
        {
            return ECAT_FOE_ERRCODE_DISKFULL;
        }

    if (Size)
    {
#if MBX_16BIT_ACCESS
        MBXMEMCPY(&aFileData[(nFileWriteOffset >> 1)], pData, Size);
#else
        MBXMEMCPY(&aFileData[nFileWriteOffset], pData, Size);
#endif

    }

    if (bDataFollowing)
    {
        /* FoE-Data services will follow */
        nFileWriteOffset += Size;
        
    }
    else
    {
        /* last part of the file is written */
        u32FileSize = nFileWriteOffset + Size;
        nFileWriteOffset = 0;
    }

    return 0;
}


UINT16 FoE_Write(UINT16 MBXMEM * pName, UINT16 nameSize, UINT32 password)
{
    if (nameSize < MAX_FILE_NAME_SIZE)
    {
        /* for test every file name can be written */
        MBXSTRCPY(aFileName, pName, nameSize);
        MBXSTRCPY(aFileName + nameSize, "\0", 1); //string termination


        nFileWriteOffset = 0;
        u32FileSize = 0;
        return 0;
    }
    else
    {
        return ECAT_FOE_ERRCODE_DISKFULL;
    }

}
#endif //#if FOE_SUPPORTED

/////////////////////////////////////////////////////////////////////////////////////////
/**
 \brief    The function is called when an error state was acknowledged by the master

*////////////////////////////////////////////////////////////////////////////////////////

void    APPL_AckErrorInd(UINT16 stateTrans)
{

}

/////////////////////////////////////////////////////////////////////////////////////////
/**
 \return    AL Status Code (see ecatslv.h ALSTATUSCODE_....)

 \brief    The function is called in the state transition from INIT to BOOT/PREOP when
             all general settings were checked to start the mailbox handler. This function
             informs the application about the state transition, the application can refuse
             the state transition when returning an AL Status error code.
            The return code NOERROR_INWORK can be used, if the application cannot confirm
            the state transition immediately, in that case this function will be called cyclically
            until a value unequal NOERROR_INWORK is returned

*////////////////////////////////////////////////////////////////////////////////////////

UINT16 APPL_StartMailboxHandler(void)
{
    if (bBootMode)
    {
        /* Init to BOOT requested*/
        return ALSTATUSCODE_NOERROR;
    }
    else
    {
        /* Init to PreOP requested*/
        /* return an error because the PreOP state shall be acknowledged by the main slave application, the reload shall be finished before the PreOP state is acknowledged*/
        return ALSTATUSCODE_NOVALIDFIRMWARE;
    }
    
}

/////////////////////////////////////////////////////////////////////////////////////////
/**
 \return     0, NOERROR_INWORK

 \brief    The function is called in the state transition from BOOT to INIT
             to stop the mailbox handler. This functions informs the application
             about the state transition, the application cannot refuse
             the state transition.

*////////////////////////////////////////////////////////////////////////////////////////

UINT16 APPL_StopMailboxHandler(void)
{
    return ALSTATUSCODE_NOERROR;
}

/////////////////////////////////////////////////////////////////////////////////////////
/**
\brief    This function will called from the mainloop
*////////////////////////////////////////////////////////////////////////////////////////
void APPL_Application(void)
{
}

#if EXPLICIT_DEVICE_ID
/////////////////////////////////////////////////////////////////////////////////////////
/**
 \return    The Explicit Device ID of the EtherCAT slave

 \brief     Calculate the Explicit Device ID
*////////////////////////////////////////////////////////////////////////////////////////
UINT16 APPL_GetDeviceID()
{
    return 0x5;
}
#endif

#if USE_DEFAULT_MAIN
/////////////////////////////////////////////////////////////////////////////////////////
/**

 \brief    This is the main function

*////////////////////////////////////////////////////////////////////////////////////////
#if _PIC24
int main(void)
#elif _WIN32
int main(int argc, char* argv[])
#else
void main(void)
#endif
{
    /* initialize the Hardware and the EtherCAT Slave Controller */
#if FC1100_HW
#if _WIN32
    u16FcInstance = 0;

    if (argc > 1)
    {
        u16FcInstance = atoi(argv[1]);
    }
#endif
    if(HW_Init())
    {
        HW_Release();
        return;
    }
#else
    HW_Init();
#endif
    MainInit();

#if FOE_SUPPORTED
    pAPPL_FoeRead = FoE_Read;
    pAPPL_FoeWriteData = FoE_WriteData;
    pAPPL_FoeReadData = FoE_ReadData;
    pAPPL_FoeWrite = FoE_Write;
    u32FileSize = 0;
#endif

    bRunApplication = TRUE;
    do
    {
        MainLoop();
    } while (bRunApplication == TRUE);

    HW_Release();
#if _PIC24
    return 0;
#endif
}
#endif //#if USE_DEFAULT_MAIN

#endif //#if BOOTLOADER_APPLICATION

/** @} */

