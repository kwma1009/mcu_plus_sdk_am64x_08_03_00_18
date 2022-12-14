/*
* This source file is part of the EtherCAT Slave Stack Code licensed by Beckhoff Automation GmbH & Co KG, 33415 Verl, Germany.
* The corresponding license agreement applies. This hint shall not be removed.
* https://www.beckhoff.com/media/downloads/slave-stack-code/ethercat_ssc_license.pdf
*/

/**
\addtogroup TestAppl Tests Application
@{
 */
 
/**
\file Testappl.c
\author EthercatSSC@beckhoff.com
\brief Implementation
This application can be used to generate specific (also non conform) slave behaviour to test the correct master reaction
See the Application Note ET9300 for further information about the test and the application in general

\version 5.13

<br>Changes to version V5.12:<br>
V5.13 ECAT3: reset local Error flag in case of two consecutive pending state response and the the first failes<br>
V5.13 ESM1: local error handling update, ECAT_StateChange triggers only transitions from Op->Any or reject/accept a pending transition<br>
V5.13 TEST1: add test object for Explicit Device ID and to trigger ESM transition<br>
V5.13 TEST2: add EoE Test object 0x2040, Si3 : send ARP on Set EoE IP Parameter<br>
V5.13 TEST5: <br>
<br>Changes to version V5.11:<br>
V5.12 ECAT1: update SM Parameter measurement (based on the system time), enhancement for input only devices and no mailbox support, use only 16Bit pointer in process data length caluclation<br>
V5.12 ECAT2: big endian changes<br>
V5.12 EOE2: prevent static ethernet buffer to be freed<br>
V5.12 EOE3: fix memory leaks in sample ICMP application<br>
V5.12 TEST1: test pending ESM handling (APPL_StartMailboxHandler shall only called once), try to overwrite the error in case of ErrSafeOP<br>
V5.12 TEST2: add pending ESM test,trigger complete ESM transition from ecat main<br>
<br>Changes to version V5.10:<br>
V5.11 DIAG4: change parameter handling in DIAG_CreateNewMessage()<br>
V5.11 ECAT11: create application interface function pointer, add eeprom emulation interface functions<br>
V5.11 ECAT4: enhance SM/Sync monitoring for input/output only slaves<br>
V5.11 TEST2: add test 0x300C object (entry bitlength > 65535)<br>
V5.11 TEST8: create diag message on SDO write access to 0x3002<br>
V5.11 TEST9: "add behaviour 0x2020.7 (SDO requests on 0x3006.0 are set to pending until an FoE read request on ""UnlockSdoResp"" is received or in case that no mbx queue is supported when a new mbx request was received)"<br>
<br>Changes to version V5.01:<br>
V5.10 DIAG1: Define diagmessage textIDs<br>
V5.10 ECAT6: Add "USE_DEFAULT_MAIN" to enable or disable the main function<br>
V5.10 TEST5: test 0x2020.1 change limit from 10 to 16 Byte <br>
             Add test object 0x3009/0x300A (huge array and record objects)<br>
V5.10 TEST6: Add Bit3Array (0x3007) test object<br>
V5.10 TEST7: Add Test behavior control via User RAM (see SSC Application Note for further details)<br>
V5.10 TEST8: Add ESM variables Object 0xA000 and map to process data (0x1AFF)<br>
<br>Changes to version V5.0:<br>
V5.01 SDO6: Update SDO response interface handling. (used if the object access function returns "ABORTIDX_WORKING" and SDO_RES_INTERFACE is active)<br>
V5.01 TEST1: Length of ESC access rights array were wrong<br>
V5.01 TEST5: Add slave configuration object handling (0x8000, if test application is active)<br>
V5.0: file created
 */


/*-----------------------------------------------------------------------------------------
------
------    Includes
------
-----------------------------------------------------------------------------------------*/
#ifdef _WIN32
#include <windows.h>
#endif

#include "ecat_def.h"

#if TEST_APPLICATION



#define _TESTAPPL_
#include "testappl.h"
#undef _TESTAPPL_



#if FOE_SUPPORTED
#include "ecatfoe.h"
#endif


/*--------------------------------------------------------------------------------------
------
------    local Types and Defines
------
--------------------------------------------------------------------------------------*/
//#define TEST_ESC_PDI_ACCESS_RIGHTS      //if a set of ESC registers will be checked if the access rights are correct (this feature is currently only for 8Bit_ESC_ACCESS available)
#if FC1100_HW
//#define PERF_TEST                       //run a performance query for the main loop
#endif

#if !FC1100_HW
#undef DebugMessage
#endif



#ifdef TEST_ESC_PDI_ACCESS_RIGHTS

typedef struct {
    UINT16 EscRegister;
    UINT8 Flags;
} RegisterWriteFlag;

//Writeable registers from PDI (based on ET1100)
RegisterWriteFlag EscPdiWriteAccess[] =
{
    {0x0012, 0xFF},
    {0x0013, 0xFF},
    {0x0130, 0xFF},
    {0x0131, 0xFF},
    {0x0134, 0xFF},
    {0x0135, 0xFF},
    {0x0204, 0xFF},
    {0x0205, 0xFF},
    {0x0206, 0xFF},
    {0x0207, 0xFF},
    /*EEPROM Registers skipped (generally read only)*/
    /*MII Registers skipped (generally read only)*/
    {0x080F, 0x03}, //SM0 PDI Control
    {0x0817, 0x03}, //SM1 PDI Control
    {0x081F, 0x03}, //SM2 PDI Control
    {0x0827, 0x03}, //SM3 PDI Control
    {0x082F, 0x03}, //SM4 PDI Control
    {0x0837, 0x03}, //SM5 PDI Control
    {0x083F, 0x03}, //SM6 PDI Control
    {0x0847, 0x03}, //SM7 PDI Control
    {0x084F, 0x03} //SM8 PDI Control
    /*DC Register skipped (0x900 - 0x9FF)*/
    /*GPIO skipped (0xF10 - 0xF17)*/
    /*UserRAM skipped (0xF80 - 0xFFF)*/
};

#define WR_MASK_LENGTH (SIZEOF(EscPdiWriteAccess) / SIZEOF(RegisterWriteFlag))
#endif

#if FOE_SUPPORTED


#if TEST_APPLICATION_REDUCED_MEMORY
#define MAX_FILE_SIZE                             0x180
#else
#define MAX_FILE_SIZE                             0x6400000 /*100MB*/

#endif


UINT32 u32FileSize;

UINT16 u16Busycnt;
UINT32           nFileWriteOffset;
#if MBX_16BIT_ACCESS
UINT16 MBXMEM     aFoeData[(MAX_FILE_SIZE >> 1)];
#else
UINT8 MBXMEM      aFoeData[MAX_FILE_SIZE];
#endif //#else #ifMBX_16BIT_ACCESS
#endif

#define IS_CTT_TEST_ACTIVE(tid,gid,cid) (((tid) == u16CurrentTestID) && ((gid) == u16CurrentTestGroupID) && ((cid) == u16CurrentTestCaseID))

/*-----------------------------------------------------------------------------------------
------
------    local variables and constants
------
-----------------------------------------------------------------------------------------*/
#if SDO_RES_INTERFACE
UINT8 SDOResult = 0; /**< \brief used to access object via SDO response stack (every first object access will be set to pending)*/
#endif

UINT32 CurrentActiveTest = 0; /**< \brief Contains the current active test. This value is used if the tests are activated via the Process data RAM of the ESC (Last 4 Bytes of the address range), see SSC application note for further details.*/


#if COE_SUPPORTED
extern TSDOINFOENTRYDESC OBJMEM asEntryDesc0x3009[];
#endif

#if EOE_SUPPORTED
/*Create broadcast Ethernet address*/
const    UINT8    BroadcastEthernetAddress[6] = { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff };
#endif

#if ESC_EEPROM_EMULATION
extern UINT16 EepromReadSize;
#endif

#if COE_SUPPORTED
extern TOBJECT OBJMEM * ObjDicList;
#endif

BOOL bStateOverwritten;
/*ECATCHANGE_START(V5.13) TEST1*/
BOOL CallEcatStateChange;
/*ECATCHANGE_END(V5.13) TEST1*/
/*-----------------------------------------------------------------------------------------
------
------    local (test) functions
------
-----------------------------------------------------------------------------------------*/
#if COE_SUPPORTED
void HandleTestControlViaEsm(void);
#endif


#if EOE_SUPPORTED


/////////////////////////////////////////////////////////////////////////////////////////
/**

\brief    This function calculates a checksum (only for an even number of bytes).
\brief Note that if you are going to checksum a checksummed packet that includes the checksum,
\brief you have to compliment the output.

*////////////////////////////////////////////////////////////////////////////////////////

UINT16 EOEAPPL_CalcCheckSum(UINT16 MBXMEM *pWord, UINT16 nLen)
{
    UINT32 crc;
    UINT32 CrcLo;
    UINT32 CrcHi;
    UINT16 RetCrc;

        crc = 0;
    while (nLen > 1)
    {
            crc += SWAPWORD(*pWord);
        pWord++;
        nLen -= 2;
    }
    if (nLen == 1)                          // if nLen odd
    {
            crc += *((UINT8*)pWord);
    }

    CrcLo = LOWORD(crc);
    CrcHi = HIWORD(crc);
    crc = CrcLo + CrcHi;

    CrcHi = HIWORD(crc);
    crc += CrcHi;
    if (crc == 0xFFFF)                     // remove the -0 ambiguity
    {
            crc = 0;
    }

    RetCrc = (UINT16)crc;
    RetCrc = ~RetCrc;
    return(RetCrc);
}

void EoeReceive(UINT16 *pFrame, UINT16 frameSize)
{
    switch (SWAPWORD(((ETHERNET_FRAME *)pFrame)->FrameType))
    {
    case ETHERNET_FRAME_TYPE_ARP1_SW:
    {
#if STATIC_ETHERNET_BUFFER
        ETHERNET_FRAME MBXMEM * pSendFrame = (ETHERNET_FRAME MBXMEM *)aEthernetSendBuffer;
#else
        ETHERNET_FRAME MBXMEM * pSendFrame = (ETHERNET_FRAME MBXMEM *) ALLOCMEM(frameSize);
#endif
        ARP_IP_HEADER MBXMEM    * pArpIp = (ARP_IP_HEADER MBXMEM    *) &pSendFrame[1];

        /*Copy Receive Frame to create ARP Reply*/
        MBXMEMCPY(pSendFrame, pFrame, frameSize);
#if MBX_16BIT_ACCESS
        if ((MBXMEMCMP(BroadcastEthernetAddress, pSendFrame->Destination.w, 4) == 0)
            && (pArpIp->lengthHwAddrPortAddr == 0x0406) /*lowbyte: length of MAC address; highbyte length of port address*/
#else
        if ((MBXMEMCMP(BroadcastEthernetAddress, pSendFrame->Destination.b, 4) == 0)
            && (pArpIp->lengthHwAddr == ETHERNET_ADDRESS_LEN)
            && (pArpIp->lengthProtAddr == SIZEOF(UINT32))
#endif
            && (pArpIp->hwAddrSpace == SWAPWORD(ARP_HW_ADDR_SPACE_ETHERNET_SW))
            && (pArpIp->protAddrSpace == SWAPWORD(ETHERNET_FRAME_TYPE_IP_SW))
            && (pArpIp->opcode == SWAPWORD(ARP_OPCODE_REQUEST_SW))
            )
        {
#if MBX_16BIT_ACCESS
            MBXMEMCPY(pSendFrame->Destination.w, pSendFrame->Source.w, 6);
            MBXMEMCPY(pSendFrame->Source.w, &aMacAdd[0], 6);

            MBXMEMCPY(pArpIp->macDest.w, pArpIp->macSource.w, 6);
            MBXMEMCPY(pArpIp->macSource.w, &aMacAdd[0], 6);
#else
            MBXMEMCPY(pSendFrame->Destination.b, pSendFrame->Source.b, 6);
            MBXMEMCPY(pSendFrame->Source.b, &aMacAdd[0], 6);

            MBXMEMCPY(pArpIp->macDest.b, pArpIp->macSource.b, 6);
            MBXMEMCPY(pArpIp->macSource.b, &aMacAdd[0], 6);
#endif
            MBXMEMCPY(pArpIp->ipDest, pArpIp->ipSource, 4);
            MBXMEMCPY(pArpIp->ipSource, aIpAdd, 4);

            pArpIp->opcode = SWAPWORD(ARP_OPCODE_REPLY_SW);

            EOE_SendFrameRequest((UINT16 MBXMEM *) pSendFrame, ARP_IP_HEADER_LEN + ETHERNET_FRAME_LEN);
        }
        else
        {
#if STATIC_ETHERNET_BUFFER
            pSendFrame = NULL;
#else
            if (pSendFrame != NULL)
            {
                FREEMEM(pSendFrame);
                pSendFrame = NULL;
            }
#endif
        }
    }
        break;
    case ETHERNET_FRAME_TYPE_IP_SW:
    {
#if STATIC_ETHERNET_BUFFER
        ETHERNET_IP_ICMP_MAX_FRAME MBXMEM * pIPHeader = (ETHERNET_IP_ICMP_MAX_FRAME MBXMEM *) aEthernetSendBuffer;
#else
        ETHERNET_IP_ICMP_MAX_FRAME MBXMEM * pIPHeader = (ETHERNET_IP_ICMP_MAX_FRAME MBXMEM *) ALLOCMEM(frameSize);
#endif

        /*Copy Receive Frame to create ICMP Reply*/
        MBXMEMCPY(pIPHeader, pFrame, frameSize);

        
#if MBX_16BIT_ACCESS
        if ((((SWAPWORD(pIPHeader->Ip.ttlProtocol) & IP_HEADER_PROTOCOL_MASK)>> 8) == IP_PROTOCOL_ICMP)
            && ((SWAPWORD(pIPHeader->IpData.Icmp.typeCode) & ICMP_TYPE_MASK) == ICMP_TYPE_ECHO)
#else
        if ((pIPHeader->Ip.protocol == IP_PROTOCOL_ICMP)
            && (pIPHeader->IpData.Icmp.type == ICMP_TYPE_ECHO)
#endif
            && (MBXMEMCMP(pIPHeader->Ip.dest, aIpAdd, 4) == 0)
            )
        {
            // ping requested
            UINT16 length;
#if !BIG_ENDIAN_FORMAT
            UINT16 lo = 0;
            UINT16 hi = 0;
#endif
            UINT32 tmp;

#if BIG_ENDIAN_FORMAT
            length = pIPHeader->Ip.length;
#else
            // length is in BigEndian format -> swap bytes
            lo = ((pIPHeader->Ip.length) & 0xff) << 8;
            hi = pIPHeader->Ip.length >> 8;
            length = hi + lo;
#endif
            // swap src and dest ip address
            MEMCPY(&tmp, pIPHeader->Ip.src, 4);
            MEMCPY(pIPHeader->Ip.src, pIPHeader->Ip.dest, 4);
            MEMCPY(pIPHeader->Ip.dest, &tmp, 4);

#if MBX_16BIT_ACCESS
            // set ping reply command
            pIPHeader->IpData.Icmp.typeCode &= ~ICMP_TYPE_MASK;
            pIPHeader->IpData.Icmp.typeCode |= ICMP_TYPE_ECHO_REPLY;

            // swap src and dest mac address
            MBXMEMCPY(pIPHeader->Ether.Destination.w, pIPHeader->Ether.Source.w, 6);
            MBXMEMCPY(pIPHeader->Ether.Source.w, aMacAdd, 6);

#else
            // set ping reply command
            pIPHeader->IpData.Icmp.type = ICMP_TYPE_ECHO_REPLY;

            // swap src and dest mac address
            MBXMEMCPY(pIPHeader->Ether.Destination.b, pIPHeader->Ether.Source.b, 6);
            MBXMEMCPY(pIPHeader->Ether.Source.b, aMacAdd, 6);
#endif

            // calculate ip checksum
            pIPHeader->Ip.cksum = 0;
            pIPHeader->Ip.cksum = SWAPWORD(EOEAPPL_CalcCheckSum((UINT16 MBXMEM *) &pIPHeader->Ip, IP_HEADER_MINIMUM_LEN));
            // calculate icmp checksum
            pIPHeader->IpData.Icmp.checksum = 0;
            /* type cast because of warning was added */
            pIPHeader->IpData.Icmp.checksum = SWAPWORD(EOEAPPL_CalcCheckSum((UINT16 MBXMEM *) &pIPHeader->IpData.Icmp, (UINT16)(length - 20)));
            /* type cast because of warning was added */
            EOE_SendFrameRequest((UINT16 MBXMEM *) pIPHeader, (UINT16)(ETHERNET_FRAME_LEN + length));
        }
        else
        {
            //protocol not supported => free allocated buffer
#if STATIC_ETHERNET_BUFFER
            pIPHeader = (ETHERNET_IP_ICMP_MAX_FRAME MBXMEM *) NULL;
#else
            if (pIPHeader != NULL)
            {
                FREEMEM(pIPHeader);
                pIPHeader = NULL;
            }
#endif
        }
    }
        break;
    }
}

/*ECATCHANGE_START(V5.13) TEST2*/
void EoeSettingIndication(UINT16 *pMac, UINT16 *pIp, UINT16 *pSubNet, UINT16 *pDefaultGateway, UINT16 *pDnsIp)
{
    if (IS_TEST_ACTIVE(SendArpOnSetIPParamter))
    {
        UINT16 result = 0;
#if STATIC_ETHERNET_BUFFER
        ETHERNET_FRAME MBXMEM * pSendFrame = (ETHERNET_FRAME MBXMEM *)aEthernetSendBuffer;
#else
        ETHERNET_FRAME MBXMEM * pSendFrame = (ETHERNET_FRAME MBXMEM *) ALLOCMEM(60);
#endif
        ARP_IP_HEADER MBXMEM    * pArpIp = (ARP_IP_HEADER MBXMEM    *) &pSendFrame[1];

#if MBX_16BIT_ACCESS
        /*set broadcast destination*/
        VARMEMSET(pSendFrame->Destination.w, 0xFF, SIZEOF(pSendFrame->Destination.w));

        /*set source*/
        MBXMEMCPY(pSendFrame->Source.w, aMacAdd, (SIZEOF(aMacAdd)));
#else
        /*set broadcast destination*/
        VARMEMSET(pSendFrame->Destination.b, 0xFF, SIZEOF(pSendFrame->Destination.b));

        /*set source*/
        MBXMEMCPY(pSendFrame->Source.b, aMacAdd, (SIZEOF(aMacAdd)));
#endif

        pSendFrame->FrameType = SWAPWORD(ETHERNET_FRAME_TYPE_ARP1_SW);


        /* set ARP Data*/
        pArpIp->hwAddrSpace = SWAPWORD(ARP_HW_ADDR_SPACE_ETHERNET_SW);

        pArpIp->protAddrSpace = SWAPWORD(ETHERNET_FRAME_TYPE_IP_SW);

#if MBX_16BIT_ACCESS
        pArpIp->lengthHwAddrPortAddr = 0x0406; /*lowbyte: length of MAC address; highbyte length of port address*/
#else
        pArpIp->lengthHwAddr = ETHERNET_ADDRESS_LEN;
        pArpIp->lengthProtAddr = SIZEOF(UINT32);
#endif

        pArpIp->opcode = SWAPWORD(ARP_OPCODE_REQUEST_SW);

#if MBX_16BIT_ACCESS
        MBXMEMCPY(pArpIp->macSource.w, &aMacAdd[0], 6);
#else
        MBXMEMCPY(pArpIp->macSource.b, &aMacAdd[0], 6);
#endif

        MBXMEMCPY(pArpIp->ipSource, aIpAdd, 4);

#if MBX_16BIT_ACCESS
        VARMEMSET(pArpIp->macDest.w, 0x00, SIZEOF(pArpIp->macDest));
#else
        VARMEMSET(pArpIp->macDest.b, 0x00, SIZEOF(pArpIp->macDest));
#endif
        MBXMEMCPY(pArpIp->ipDest, pDefaultGateway, 4);

        result = EOE_SendFrameRequest((UINT16 MBXMEM *) pSendFrame, ARP_IP_HEADER_LEN + ETHERNET_FRAME_LEN);

        if (result == 0)
        {
            INC_TEST_CNT(SendArpOnSetIPParamter);
        }


    }
}
/*ECATCHANGE_END(V5.13) TEST2*/
#endif

#if FOE_SUPPORTED
UINT16 FoEWriteData(UINT16 MBXMEM * pData, UINT16 Size, BOOL bDataFollowing)
{
    if (Size)
    {
        /*test downloads a file with the size of 0x8080 bytes, the file content does not need to be stored*/
        if (IS_CTT_TEST_ACTIVE(0xFFBA, 22, 3) == FALSE)
        {
            if ((nFileWriteOffset + Size) > MAX_FILE_SIZE)
            {
                return 0x8003; //ECAT_FOE_ERRCODE_DISKFULL (0x8003)
            }
            else
            {

#if MBX_16BIT_ACCESS
                MBXMEMCPY(&aFoeData[(nFileWriteOffset >> 1)], pData, Size);
#else
                MBXMEMCPY(&aFoeData[nFileWriteOffset], pData, Size);
#endif
            }
        }
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

UINT16 FoEWrite(UINT16 MBXMEM * pName, UINT16 nameSize, UINT32 password)
{
    if (password != 0x12345678)
    {
        return ECAT_FOE_ERRCODE_NORIGHTS;
    }

    u32FileSize = 0;
    nFileWriteOffset = 0;
    return 0;
}

UINT16 FoERead(UINT16 MBXMEM * pName, UINT16 nameSize, UINT32 password, UINT16 maxBlockSize, UINT16 *pData)
{
    UINT16 sizeError = 0;

    if (password != 0x12345678)
    {
        return ECAT_FOE_ERRCODE_NORIGHTS;
    }

    if ((nameSize + 1) > 20)
    {
        return  ECAT_FOE_ERRCODE_DISKFULL;
    }


    sizeError = maxBlockSize;

    if (u32FileSize < sizeError)
    {
        sizeError = (UINT16)u32FileSize;
    }


    if (IS_TEST_ACTIVE(DelaySdoResponse0x3006))
    {
        UINT16 UnlockName[] = { SWAPWORD(0x6E55),SWAPWORD(0x6F6C),SWAPWORD(0x6b63),SWAPWORD(0x6453),SWAPWORD(0x526F),SWAPWORD(0x7365),SWAPWORD(0x0070) }; /*ascii code for "UnlockSdoResp"*/


        if (HMEMCMP(pName, UnlockName, nameSize) == 0)
        {
            bUnlockSdoRequest = TRUE;
            sizeError = 10;
        }
        else
        {
            bUnlockSdoRequest = FALSE;
        }
    }


    if (IS_TEST_ACTIVE(FoE0_ReadBusy))
    {
        INC_TEST_CNT(FoE0_ReadBusy);


        if (pName != NULL)
        {
            sizeError = (FOE_MAXBUSY - 50); /* Return 50% busy*/
        }
        else
        {
            sizeError = 0;
        }
    }

    if (sizeError <= FOE_MAXDATA)
    {
        /*copy the first foe data block*/
        MEMCPY(pData, aFoeData, sizeError);
    }


    return sizeError;
}

UINT16 FoEReadData(UINT32 offset, UINT16 maxBlockSize, UINT16 *pData)
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

    //to support also 16bit only platfroms an byte FoE fragments are not supported
    if ((offset % 2) > 0)
    {
        return 0x8004; //ECAT_FOE_ERRCODE_ILLEGAL (0x8004)
    }

    /*copy the foe data block 2 .. n*/
    MEMCPY(pData, &(((UINT16 *)aFoeData)[(offset >> 1)]), sizeError);

    return sizeError;
}

#endif


#if DIAGNOSIS_SUPPORTED
/////////////////////////////////////////////////////////////////////////////////////////
/**
 \param    From         Actual state
 \param    To           Requested state

 \brief    Create new diagnosis message to indicate state request
 *////////////////////////////////////////////////////////////////////////////////////////

void Diag_DumpStateRequest(UINT8 From, UINT8 To) 
{
    UINT8 NumParam = 2;
    TDIAGMSGPARAMINFO aParam[2];

    aParam[0].ParamFlags = (DIAG_MSG_PARAM_TYPE_DATA << DIAG_MSG_PARAM_TYPE_OFFSET) | (UINT16) DEFTYPE_UNSIGNED8;
    aParam[0].pParam = (MEM_ADDR *)&From;

    aParam[1].ParamFlags = (DIAG_MSG_PARAM_TYPE_DATA << DIAG_MSG_PARAM_TYPE_OFFSET) | (UINT16) DEFTYPE_UNSIGNED8;
    aParam[1].pParam = (MEM_ADDR *) &To;

    Diag_CreateNewMessage(DIAG_APPL_INFO, DIAG_MSG_TYPE_INFO, DIAG_TEXT_ID_STATE_CHANGE_INFO, NumParam, aParam);
}
#endif //#if DIAGNOSIS_SUPPORTED

#ifdef TEST_ESC_PDI_ACCESS_RIGHTS
/////////////////////////////////////////////////////////////////////////////////////////
/**
  \brief     test write access of ESC registers.
             - read register value 
             - toggle bits and write register
 *////////////////////////////////////////////////////////////////////////////////////////

void ESC_Access_Test() 
{
    UINT16 RegOffset = 0;
    UINT16 MaskArrCnt = 0;
    UINT8 RdData = 0;
    UINT8 NewData = 0;
    UINT8 RdMask = 0xFF;

    for (RegOffset = 0; RegOffset < 0x1000; RegOffset++) 
    {
        /*Reset ReadFlagsMask;*/
        RdMask = 0xFF;

        if (MaskArrCnt < WR_MASK_LENGTH)
        {
            if (EscPdiWriteAccess[MaskArrCnt].EscRegister == RegOffset) 
            {
                RdMask = ~EscPdiWriteAccess[MaskArrCnt].Flags;
                MaskArrCnt++;
            }
        }

        HW_EscReadByte(RdData, RegOffset);
        NewData = ~RdData;

        HW_EscWriteByte(NewData, RegOffset);
        HW_EscReadByte(NewData, RegOffset);

#ifdef DebugMessage
        if ((RdData & RdMask) != (NewData & RdMask)) 
        {
            DebugMessage("Invalid Register Access (reg: 0x%x)", RegOffset);
        }
#endif
    }
}
#endif

void APPL_Main()
{
    bStateOverwritten = FALSE;
#if EOE_SUPPORTED
    if (i8EoeSendPending == 1)
    {
        UINT16 result = EOE_SendFrameRequest(pObj3017_EoeSendData, Obj3017_EoeSendDataLength);
        
        if (result == 0)
        {
            i8EoeSendPending = 0;
        }
    }
#endif
#if COE_SUPPORTED
/*ECATCHANGE_START(V5.13) ECAT3*/
    if (!bMbxRunning && (u16PsTransitionCallsHelper > 0))
    {
        u16PsTransitionCallsHelper = 0;
    }
/*ECATCHANGE_END(V5.13) ECAT3*/


/*ECATCHANGE_START(V5.13) TEST1*/
    /*Reset Explicit Device ID value in INIT*/
    if (!bMbxRunning)
    {
        Obj3019 = 5;
    }


    if (CallEcatStateChange)
    {
#if CONTROLLER_32BIT
        ECAT_StateChange((UINT8)(sObj301A_TriggerEsm.StatusCode & 0xFF), (UINT16)((sObj301A_TriggerEsm.StatusCode & 0xFFFF0000) >> 16));
#else
        ECAT_StateChange((UINT8)sObj301A_TriggerEsm.Status, (UINT16)sObj301A_TriggerEsm.Code);
#endif
        CallEcatStateChange = FALSE;
    }
/*ECATCHANGE_END(V5.13) TEST1*/
#endif //#if COE_SUPPORTED

}
/////////////////////////////////////////////////////////////////////////////////////////
/**
 \return    result of the application initialization and basic Tests

 \brief     Initialize application variables and perform some basics tests
 *////////////////////////////////////////////////////////////////////////////////////////

UINT16 APPL_Init(void) 
{
    UINT16 result = 0;

#if COE_SUPPORTED
    UINT32 cnt = 0;


    for (cnt = 0; cnt < HUGE_OBJ_BYTE_SIZE; cnt++) 
    {
        HugeObj[cnt] = (UINT8) (cnt & 0xFF);
    }

    /*reset test control object*/
    sTestControlObj.u16SubIndex0 = TEST_CONTROL_BITS;
    for (cnt = 0; cnt < TEST_CONTROL_BITS; cnt++) 
    {
        sTestControlObj.aEntries[cnt].TestIndex = 0;
        sTestControlObj.aEntries[cnt].TestSubindex = 0;
        sTestControlObj.aEntries[cnt].pTest = NULL;
    }

#endif //#if COE_SUPPORTED

    HandleCttTestCaseID();

#ifdef TEST_ESC_PDI_ACCESS_RIGHTS
    ESC_Access_Test();
#endif

    /*Reset last index indication*/
    CurrentActiveTest = 0x00000000;
    HW_EscWriteDWord(CurrentActiveTest, 0xF84);

    /* Reset Test id value */
    CurrentActiveTest = 0xFFFFFFFF;
    HW_EscWriteDWord(CurrentActiveTest, 0xF80);

    bUnlockSdoRequest = FALSE;


#if EOE_SUPPORTED
    pAPPL_EoeReceive = EoeReceive;
    /*ECATCHANGE_START(V5.13) TEST2*/
    pAPPL_EoeSettingInd = EoeSettingIndication;
    /*ECATCHANGE_END(V5.13) TEST2*/
#endif

#if FOE_SUPPORTED
    pAPPL_FoeRead = FoERead;
    pAPPL_FoeReadData = FoEReadData;
    pAPPL_FoeWrite = FoEWrite;
    pAPPL_FoeWriteData = FoEWriteData;
#endif


    bPendingESMLock = FALSE;

#if EOE_SUPPORTED
    i8EoeSendPending = -1;
#endif
    pAPPL_MainLoop = APPL_Main;

    /*ECATCHANGE_START(V5.13) TEST1*/
    CallEcatStateChange = FALSE;
    /*ECATCHANGE_END(V5.13) TEST1*/

    return result;
}

/*-----------------------------------------------------------------------------------------
------
------    generic functions
------
-----------------------------------------------------------------------------------------*/
/////////////////////////////////////////////////////////////////////////////////////////
/**
 \brief    The function is called when an error state was acknowledged by the master

 *////////////////////////////////////////////////////////////////////////////////////////

void APPL_AckErrorInd(UINT16 stateTrans) 
{
}

/////////////////////////////////////////////////////////////////////////////////////////
/**
 \return    AL Status Code (see ecatslv.h ALSTATUSCODE_....)

 \brief    The function is called in the state transition from INIT to PREOP when
           all general settings were checked to start the mailbox handler. This function
           informs the application about the state transition, the application can refuse
           the state transition when returning an AL Status error code.
           The return code NOERROR_INWORK can be used, if the application cannot confirm
           the state transition immediately, in that case the application need to be complete 
           the transition by calling ECAT_StateChange.

 *////////////////////////////////////////////////////////////////////////////////////////

UINT16 APPL_StartMailboxHandler(void) 
{

#if DIAGNOSIS_SUPPORTED
    if (IS_TEST_ACTIVE(CoE0_ESM_Diag)) 
    {
        Diag_DumpStateRequest(STATE_INIT, STATE_PREOP);
        INC_TEST_CNT(CoE0_ESM_Diag);
    }
#endif


#if COE_SUPPORTED
    if (IS_TEST_ACTIVE(ESM0_PENDING_TRANSITIONS))
    {
        if (bPendingESMLock)
        {
            //function is called the second time
            return ALSTATUSCODE_NOVALIDFIRMWARE;
        }
        else
        {
            INC_TEST_CNT(ESM0_PENDING_TRANSITIONS);
            bPendingESMLock = TRUE;
            return NOERROR_INWORK;
        }
    }
#endif

   return ALSTATUSCODE_NOERROR;
}

/////////////////////////////////////////////////////////////////////////////////////////
/**
 \return     0, NOERROR_INWORK

 \brief    The function is called in the state transition from PREEOP to INIT
             to stop the mailbox handler. This functions informs the application
             about the state transition, the application cannot refuse
             the state transition.

 *////////////////////////////////////////////////////////////////////////////////////////

UINT16 APPL_StopMailboxHandler(void) 
{
#if DIAGNOSIS_SUPPORTED
    /* Clear Diagnosis History (0x10F3)*/
    {
        UINT8 ResetVal = 0;
        UINT16 Flags = 0;
        Read0x10F3(0x10F3, 5, 2, (UINT16 MBXMEM *)&Flags, FALSE);
        Flags &= (UINT16)~0x10; //set overwritemode
        Write0x10F3(0x10F3, 5, 2, (UINT16 MBXMEM *)&Flags, FALSE);

        //set clear history command
        Write0x10F3(0x10F3,3,1,(UINT16 MBXMEM * )&ResetVal,FALSE);
    }
#endif

#if DIAGNOSIS_SUPPORTED
    if (IS_TEST_ACTIVE(CoE0_ESM_Diag)) 
    {
        Diag_DumpStateRequest(STATE_PREOP, STATE_INIT);
        INC_TEST_CNT(CoE0_ESM_Diag);
    }
#endif



    bPendingESMLock = FALSE;

    u8LocalErrorState = 0;
    u16LocalErrorCode = 0x00;
    bStateOverwritten = FALSE;

    return ALSTATUSCODE_NOERROR;
}

/////////////////////////////////////////////////////////////////////////////////////////
/**
 \param    pIntMask    pointer to the AL Event Mask which will be written to the AL event Mask
                        register (0x204) when this function is succeeded. The event mask can be adapted
                        in this function
 \return    AL Status Code (see ecatslv.h ALSTATUSCODE_....)

 \brief    The function is called in the state transition from PREOP to SAFEOP when
             all general settings were checked to start the input handler. This function
             informs the application about the state transition, the application can refuse
             the state transition when returning an AL Status error code.
            The return code NOERROR_INWORK can be used, if the application cannot confirm
            the state transition immediately, in that case the application need to be complete 
            the transition by calling ECAT_StateChange.
 *////////////////////////////////////////////////////////////////////////////////////////

UINT16 APPL_StartInputHandler(UINT16 *pIntMask) 
{
#if DIAGNOSIS_SUPPORTED
    if (IS_TEST_ACTIVE(CoE0_ESM_Diag)) 
    {
        Diag_DumpStateRequest(STATE_PREOP, STATE_SAFEOP);
        INC_TEST_CNT(CoE0_ESM_Diag);
    }
#endif

#if COE_SUPPORTED
    if (IS_TEST_ACTIVE(ESM0_PENDING_TRANSITIONS))
    {
        if (bPendingESMLock)
        {
            //function is called the second time
            return ALSTATUSCODE_NOVALIDFIRMWARE;
        }
        else
        {
            INC_TEST_CNT(ESM0_PENDING_TRANSITIONS);
            bPendingESMLock = TRUE;
            return NOERROR_INWORK;
        }
    }

/*ECATCHANGE_START(V5.13) ECAT3*/
    if (IS_TEST_ACTIVE(ESM0_PENDING_PS_TRANSITION_WITH_ERROR))
    {
        u16PsTransitionCallsHelper++;
        return NOERROR_INWORK;
    }
/*ECATCHANGE_END(V5.13) ECAT3*/
#endif

    return ALSTATUSCODE_NOERROR;
}

/////////////////////////////////////////////////////////////////////////////////////////
/**
 \return     0, NOERROR_INWORK

 \brief    The function is called in the state transition from SAFEOP to PREEOP
             to stop the input handler. This functions informs the application
             about the state transition, the application cannot refuse
             the state transition.

 *////////////////////////////////////////////////////////////////////////////////////////

UINT16 APPL_StopInputHandler(void) 
{
#if DIAGNOSIS_SUPPORTED
    if (IS_TEST_ACTIVE(CoE0_ESM_Diag)) 
    {
        Diag_DumpStateRequest(STATE_SAFEOP, STATE_PREOP);
        INC_TEST_CNT(CoE0_ESM_Diag);
    }
#endif

    bPendingESMLock = FALSE;

    return ALSTATUSCODE_NOERROR;
}

/////////////////////////////////////////////////////////////////////////////////////////
/**
 \return    AL Status Code (see ecatslv.h ALSTATUSCODE_....)

 \brief    The function is called in the state transition from SAFEOP to OP when
             all general settings were checked to start the output handler. This function
             informs the application about the state transition, the application can refuse
             the state transition when returning an AL Status error code.
           The return code NOERROR_INWORK can be used, if the application cannot confirm
           the state transition immediately, in that case the application need to be complete 
           the transition by calling ECAT_StateChange.
 *////////////////////////////////////////////////////////////////////////////////////////

UINT16 APPL_StartOutputHandler(void) 
{
#if DIAGNOSIS_SUPPORTED
    if (IS_TEST_ACTIVE(CoE0_ESM_Diag)) 
    {
        Diag_DumpStateRequest(STATE_SAFEOP, STATE_OP);
        INC_TEST_CNT(CoE0_ESM_Diag);
    }
#endif

#if COE_SUPPORTED
    if (IS_TEST_ACTIVE(ESM0_PENDING_TRANSITIONS))
    {
        if (bPendingESMLock)
        {
            //function is called the second time
            return ALSTATUSCODE_NOVALIDFIRMWARE;
        }
        else
        {
            INC_TEST_CNT(ESM0_PENDING_TRANSITIONS);
            bPendingESMLock = TRUE;
            return NOERROR_INWORK;
        }
    }
#endif

    return ALSTATUSCODE_NOERROR;

}

/////////////////////////////////////////////////////////////////////////////////////////
/**
 \return     0, NOERROR_INWORK

 \brief    The function is called in the state transition from OP to SAFEOP
             to stop the output handler. This functions informs the application
             about the state transition, the application cannot refuse
             the state transition.

 *////////////////////////////////////////////////////////////////////////////////////////

UINT16 APPL_StopOutputHandler(void)
{
#if DIAGNOSIS_SUPPORTED
    if(IS_TEST_ACTIVE(CoE0_ESM_Diag))
    {
        Diag_DumpStateRequest(STATE_OP, STATE_SAFEOP);
        INC_TEST_CNT(CoE0_ESM_Diag);
    }
#endif

    bPendingESMLock = FALSE;


    return ALSTATUSCODE_NOERROR;
}

/////////////////////////////////////////////////////////////////////////////////////////
/**
\return     0(ALSTATUSCODE_NOERROR), NOERROR_INWORK
\param      pInputSize  pointer to save the input process data length
\param      pOutputSize  pointer to save the output process data length

\brief    This function calculates the process data sizes from the actual SM-PDO-Assign
            and PDO mapping
 *////////////////////////////////////////////////////////////////////////////////////////

UINT16 APPL_GenerateMapping(UINT16* pInputSize, UINT16* pOutputSize)
{

    UINT16 result = ALSTATUSCODE_NOERROR;
#if COE_SUPPORTED
    UINT16 PDOAssignEntryCnt = 0;
    OBJCONST TOBJECT OBJMEM * pPDO = NULL;
    UINT16 PDOSubindex0 = 0;
    UINT32 *pPDOEntry = NULL;
    UINT16 PDOEntryCnt = 0;
    UINT16 InputSize = 0;
    UINT16 OutputSize = 0;

    /*Scan object 0x1C12 RXPDO assign*/
    for (PDOAssignEntryCnt = 0; PDOAssignEntryCnt < sRxPDOassign.u16SubIndex0; PDOAssignEntryCnt++) 
    {
        pPDO = OBJ_GetObjectHandle(sRxPDOassign.aEntries[PDOAssignEntryCnt]);
        if (pPDO != NULL) 
        {
            PDOSubindex0 = *((UINT16 *) pPDO->pVarPtr);
            for (PDOEntryCnt = 0; PDOEntryCnt < PDOSubindex0; PDOEntryCnt++) 
            {
                pPDOEntry = (UINT32 *)((UINT16 *)pPDO->pVarPtr + (OBJ_GetEntryOffset((UINT8)(PDOEntryCnt + 1), pPDO) >> 3) / 2); //goto PDO entry
                // we increment the expected output size depending on the mapped Entry
                OutputSize += (UINT16) ((*pPDOEntry) & 0xFF);
            }
        } 
        else
        {
            /*assigned PDO was not found in object dictionary. return invalid mapping*/
            OutputSize = 0;
            result = ALSTATUSCODE_INVALIDOUTPUTMAPPING;
            break;
        }
    }

    OutputSize = (OutputSize + 7) >> 3;

    if (result == 0) 
    {
        /*Scan Object 0x1C13 TXPDO assign*/
        for (PDOAssignEntryCnt = 0; PDOAssignEntryCnt < sTxPDOassign.u16SubIndex0; PDOAssignEntryCnt++) 
        {
            pPDO = OBJ_GetObjectHandle(sTxPDOassign.aEntries[PDOAssignEntryCnt]);
            if (pPDO != NULL) 
            {
                PDOSubindex0 = *((UINT16 *) pPDO->pVarPtr);
                for (PDOEntryCnt = 0; PDOEntryCnt < PDOSubindex0; PDOEntryCnt++) 
                {
                    pPDOEntry = (UINT32 *) ((UINT16 *) pPDO->pVarPtr + (OBJ_GetEntryOffset((UINT8)(PDOEntryCnt + 1), pPDO) >> 3)/2); //goto PDO entry
                    // we increment the expected output size depending on the mapped Entry
                    InputSize += (UINT16) ((*pPDOEntry) & 0xFF);
                }
            } 
            else
            {
                /*assigned PDO was not found in object dictionary. return invalid mapping*/
                InputSize = 0;
                result = ALSTATUSCODE_INVALIDINPUTMAPPING;
                break;
            }
        }
    }
    InputSize = (InputSize + 7) >> 3;

    *pInputSize = InputSize;
    *pOutputSize = OutputSize;
#else

    *pInputSize = 4;
    *pOutputSize = 4;
#endif// #else #if COE_SUPPORTED
    return result;
}


/////////////////////////////////////////////////////////////////////////////////////////
/**
\param      pData  pointer to input process data
\brief      This function will copies the inputs from the local memory to the ESC memory
            to the hardware
 *////////////////////////////////////////////////////////////////////////////////////////

void APPL_InputMapping(UINT16* pData) 
{

#if COE_SUPPORTED
    UINT16 *pTmpData = pData;
    UINT16 cnt = 0;

/*ECATCHANGE_START(V5.13) TEST5*/
    sObjA001_PdFncCallc.nPdoInputMappingCalls++;
/*ECATCHANGE_END(V5.13) TEST5*/

    for (cnt = 0; cnt < sTxPDOassign.u16SubIndex0; cnt++) 
    {
        switch (sTxPDOassign.aEntries[cnt]) 
        {
            case 0x1A02:
            {
                /*0x6020*/
                MEMCPY(pTmpData, &CounterOverrun, SIZEOF(CounterOverrun));
                if (cnt < (sTxPDOassign.u16SubIndex0 - 1))
                    pTmpData += 1;
            }
                break;
            case 0x1A00:
            {
                /*0x6000*/
                MEMCPY(pTmpData, &InputCounter, SIZEOF(InputCounter));
                if (cnt < (sTxPDOassign.u16SubIndex0 - 1))
                    pTmpData += 2;
            }
                break;

#if DC_SUPPORTED
            case 0x1AFF:
            {
                *pTmpData = SWAPWORD(nEcatStateTrans); /*0xA000.2, UINT16*/
                pTmpData++;

                *pTmpData = SWAPWORD(EsmTimeoutCounter); /*0xA000.2, Int16*/
                pTmpData++;

                *pTmpData = SWAPWORD(i16WaitForPllRunningTimeout); /*0xA000.3*/
                pTmpData++;

                *pTmpData = SWAPWORD(i16WaitForPllRunningCnt); /*0xA000.4*/
                pTmpData++;

                *pTmpData = SWAPWORD(u16LocalErrorCode); /*0xA000.5*/
                pTmpData++;

                *pTmpData = 0;
                *pTmpData |= bApplEsmPending; /*0xA000.6*/
                *pTmpData |= (bEcatWaitForAlControlRes << 1); /*0xA000.7*/
                *pTmpData |= (bEcatOutputUpdateRunning << 2); /*0xA000.8*/
                *pTmpData |= (bEcatInputUpdateRunning << 3); /*0xA000.9*/
#if MAX_PD_OUTPUT_SIZE > 0
                *pTmpData |= (bEcatFirstOutputsReceived << 4); /*0xA000.10*/
#endif
/*ECATCHANGE_START(V5.13) ESM1*/
                {
                    BOOL tmp = STATE_VALID(u8LocalErrorState);
                    *pTmpData |= (tmp << 5); /*0xA000.11*/
                }
/*ECATCHANGE_END(V5.13) ESM1*/
                /*0xA000.12 2Bit Alignment*/

#if MAX_PD_OUTPUT_SIZE > 0
                *pTmpData |= (bWdTrigger << 8); /*0xA000.17*/
#endif
                *pTmpData |= (bDcSyncActive << 9); /*0xA000.18*/
                *pTmpData |= (bEscIntEnabled << 10); /*0xA000.19*/
                *pTmpData |= (bDcRunning << 11); /*0xA000.20*/
                *pTmpData |= (bSmSyncSequenceValid << 13); /*0xA000.21*/
                /*0xA000.22 3Bit Alignment*/
                pTmpData++;

                *pTmpData = SWAPWORD(EcatWdValue); /*0xA000.33, UINT16*/
                pTmpData++;
#if !ESC_SM_WD_SUPPORTED
                *pTmpData = SWAPWORD(EcatWdCounter); /*0xA000.34, UINT16*/
#else
                *pTmpData = 0; /*EcatWdCounter; 0xA000.34 not available*/
#endif
                pTmpData++;

                *pTmpData = SWAPWORD(Sync0WdCounter); /*0xA000.35, UINT16*/
                pTmpData++;

                *pTmpData = SWAPWORD(Sync0WdValue); /*0xA000.36, UINT16*/
                pTmpData++;

                *pTmpData = SWAPWORD(Sync1WdCounter); /*0xA000.37, UINT16*/
                pTmpData++;

                *pTmpData = SWAPWORD(Sync1WdValue); /*0xA000.38 UINT16*/
                pTmpData++;

                *pTmpData = SWAPWORD(LatchInputSync0Value); /*0xA000.39, UINT16*/
                pTmpData++;

#if COE_SUPPORTED 
#if (MAX_PD_OUTPUT_SIZE > 0)
                *pTmpData = SWAPWORD(sSyncManOutPar.u16SmEventMissedCounter); /*0x1C32.11*/
#elif (MAX_PD_INPUT_SIZE > 0)
                *pTmpData = SWAPWORD(sSyncManInPar.u16SmEventMissedCounter); /*0x1C33.11*/
#endif
#else
                *pTmpData = SWAPWORD(u16SmEventMissedCounter);
#endif
                pTmpData++;
            }
                break;
#endif //#if AL_EVENT_ENABLED && DC_SUPPORTED
        }
    }
#else
    MEMCPY(pData, &InputCounter, SIZEOF(InputCounter));
#endif
}

/////////////////////////////////////////////////////////////////////////////////////////
/**
\param      pData  pointer to output process data

\brief    This function will copies the outputs from the ESC memory to the local memory
            to the hardware
 *////////////////////////////////////////////////////////////////////////////////////////

void APPL_OutputMapping(UINT16* pData) 
{
#if COE_SUPPORTED
    UINT16 *pTmpData = pData;
    UINT16 cnt = 0;

    /*ECATCHANGE_START(V5.13) TEST5*/
    sObjA001_PdFncCallc.nPdoOutputMappingCalls++;
    /*ECATCHANGE_END(V5.13) TEST5*/

    for (cnt = 0; cnt < sRxPDOassign.u16SubIndex0; cnt++) 
    {
        switch (sRxPDOassign.aEntries[cnt]) 
        {
            case 0x1603:
            {
                /*Copy data of 0x7030*/
                MEMCPY(&OutputCounter1, pTmpData, SIZEOF(OutputCounter1));
                if (cnt < (sRxPDOassign.u16SubIndex0 - 1))
                    pTmpData += 2;
            }
                break;
            case 0x1601:
            {
                /*copy data of 0x7010*/
                MEMCPY(&OutputCounter, pTmpData, SIZEOF(OutputCounter));
                if (cnt < (sRxPDOassign.u16SubIndex0 - 1))
                    pTmpData += 2;
            }
                break;
        }
    }
#else
    MEMCPY(&OutputCounter, pData, SIZEOF(OutputCounter));
#endif

}

/////////////////////////////////////////////////////////////////////////////////////////
/**
 \brief     Application which is called with every cycle (the cycle depends on the mode of synchronisation)
 *////////////////////////////////////////////////////////////////////////////////////////

void APPL_Application(void) 
{

    UINT16 ActiveMask = 0;
#if COE_SUPPORTED
    UINT16 cnt = 0;

    /*ECATCHANGE_START(V5.13) TEST5*/
    sObjA001_PdFncCallc.nApplApplicationCalls++;
    /*ECATCHANGE_END(V5.13) TEST5*/
#endif
    


    /*update test control*/
#if EL9800_HW && COE_SUPPORTED
    ActiveMask = (UINT16) (SWITCH_8 << 6) | (SWITCH_7 << 5) | (SWITCH_6 << 4) | (SWITCH_5 << 3) | (SWITCH_4 << 2) | (SWITCH_3 << 1) | (SWITCH_2);
    if (SWITCH_1)
#else
    HW_EscReadWord(ActiveMask, 0xF10);
#endif

#if COE_SUPPORTED
        {
            for (cnt = 0; cnt < sTestControlObj.u16SubIndex0; cnt++) 
            {
                ActiveMask = (ActiveMask >> cnt);

                if (sTestControlObj.aEntries[cnt].pTest != NULL) 
                {
                    if (ActiveMask & 0x1)
                        sTestControlObj.aEntries[cnt].pTest->Control |= 0x1;
                    else
                        sTestControlObj.aEntries[cnt].pTest->Control &= ~0x1;
                }
            }
        }
#endif
    if (OutputCounter > 0) 
    {
        InputCounter = OutputCounter + 1;
#if COE_SUPPORTED
        CounterOverrun = 0;
#endif
    }
    else 
    {
        InputCounter++;
#if COE_SUPPORTED
        if (InputCounter == 0xFFFFFFFF)
            CounterOverrun++;
#endif
    }

#if DIAGNOSIS_SUPPORTED
    if (InputCounter > 0 && IS_TEST_ACTIVE(CoE0_Diag_Test)) 
    {
        TDIAGMSGPARAMINFO DiagParameter;
        DiagParameter.pParam = (MEM_ADDR *) & InputCounter;
/* ECATCHANGE_ENDT(V5.10.1) DIAG4*/
        DiagParameter.ParamFlags = (UINT16) DEFTYPE_UNSIGNED32;

        Diag_CreateNewMessage(DIAG_APPL_INFO, DIAG_MSG_TYPE_INFO, DIAG_TEXT_ID_DEC_NUMBER, 1, &DiagParameter);
    }
#endif

#if MAILBOX_QUEUE
    if(sObj300D_S2S.DstSlv != 0)
    {
        if((sObj300D_S2S.Command != sObj300D_S2S.Status) && (sObj300D_S2S.Command == 0x1))
        {
            TINITSDOUPLOADREQMBX MBXMEM *pMbx = (TINITSDOUPLOADREQMBX MBXMEM *) APPL_AllocMailboxBuffer(16); // allocate buffer for the 
            HMEMSET(pMbx,0x00,16); //clear allocated buffer 

            if (pMbx)
            {
                pMbx->MbxHeader.Address = sObj300D_S2S.DstSlv;
                pMbx->MbxHeader.Flags[MBX_OFFS_TYPE] |= (MBX_TYPE_COE << MBX_SHIFT_TYPE);

                pMbx->CoeHeader |= ((UINT16)COESERVICE_SDOREQUEST) << COEHEADER_COESERVICESHIFT;
                pMbx->SdoHeader.Sdo[SDOHEADER_COMMANDOFFSET] |= SDOSERVICE_INITIATEUPLOADREQ;

                pMbx->MbxHeader.Length = EXPEDITED_FRAME_SIZE;
#if MBX_16BIT_ACCESS
                pMbx->SdoHeader.Sdo[SDOHEADER_INDEXHIOFFSET] = 0x10;
                pMbx->SdoHeader.Sdo[SDOHEADER_INDEXLOOFFSET] |= (0x18 << SDOHEADER_INDEXLOSHIFT);
                pMbx->SdoHeader.Sdo[SDOHEADER_SUBINDEXOFFSET] |= (0x2 << SDOHEADER_SUBINDEXSHIFT);

#else
                pMbx->SdoHeader.Sdo[SDOHEADER_INDEXHIOFFSET] = 0x10;
                pMbx->SdoHeader.Sdo[SDOHEADER_INDEXLOOFFSET] = 0x18;
                pMbx->SdoHeader.Sdo[SDOHEADER_SUBINDEXOFFSET] = 0x2;

#endif

                if (MBX_MailboxSendReq((TMBX MBXMEM *)pMbx, COE_SERVICE) != 0)
                {
                    APPL_FreeMailboxBuffer(pMbx);
                }

            } //Mbox buffer was allocated

            sObj300D_S2S.Command = 0;
            sObj300D_S2S.Status = 1; //set status to wait for response

        } // an command is pending
    } //an slave destination address is set
#endif //#if MAILBOX_QUEUE


        /* try to overwrite the current Error */
        if (nAlStatus == 0x14 && (u8LocalErrorState == 0) && IS_CTT_TEST_ACTIVE(0xFFBA,13,2)) //only over write AL Status Code if the overwrite AL Status code test is active
        {
                ECAT_StateChange((STATE_CHANGE | STATE_SAFEOP), ALSTATUSCODE_NOVALIDFIRMWARE);
        }
    



}

#if EXPLICIT_DEVICE_ID
/////////////////////////////////////////////////////////////////////////////////////////
/**
 \return    The Explicit Device ID of the EtherCAT slave

 \brief     Read the Explicit Device ID (from an external ID switch)
 *////////////////////////////////////////////////////////////////////////////////////////

UINT16 APPL_GetDeviceID(void) 
{
    return Obj3019;
}
#endif

/////////////////////////////////////////////////////////////////////////////////////////
/**
 \brief    Update test active states (called on startup and every ALControl Indication (INIT_2_ANY)
 *////////////////////////////////////////////////////////////////////////////////////////

void UpdateActiveTest() 
{
#if COE_SUPPORTED
    HandleTestControlViaEsm();
#endif //#if COE_SUPPORTED
}
#if COE_SUPPORTED
/////////////////////////////////////////////////////////////////////////////////////////
/**
 \param    index              index of the requested object.
 \param    subindex           subindex of the requested object.
 \param    dataSize           received data size of the SDO Download
 \param    pData              Pointer to the buffer where the written data can be copied from
 \param    bCompleteAccess    Indicates if a complete write of all subindices of the
                              object shall be done or not

 \return    result of the write operation (0 (success) or an abort code (ABORTIDX_.... defined in
            sdosrv.h))

 \brief     This function writes the test control object
 *////////////////////////////////////////////////////////////////////////////////////////

UINT8 WriteObject0x2FFF(UINT16 index, UINT8 subindex, UINT32 dataSize, UINT16 MBXMEM * pData, UINT8 bCompleteAccess) 
{
    OBJCONST TOBJECT OBJMEM * pObject = NULL;
    TTESTOBJ OBJMEM * pTestObject = NULL;
    UINT32 NewTest = 0;
    UINT16 NewTestIndex = 0;
    UINT8 NewTestSubIndex = 0;

    /*Complete Access is not supported for Test objects*/
    if (bCompleteAccess)
        return ABORTIDX_UNSUPPORTED_ACCESS;

    /*handle only test control object with this function*/
    if (index != 0x2FFF)
        return ABORTIDX_OBJECT_NOT_EXISTING;

    /*Subindex 0 is read only*/
    if (subindex == 0)
    {
        UINT8 newVal = 0;
        if (dataSize > 1)
        {
            return ABORTIDX_PARAM_LENGTH_ERROR;
        }

        newVal = (UINT8)*pData;

        if(newVal != 0 && newVal != TEST_CONTROL_BITS)
            return ABORTIDX_VALUE_EXCEEDED;

        sTestControlObj.u16SubIndex0 = newVal;

        if (newVal == 0)
        {
            TOBJECT OBJMEM * pDicEntry = ObjDicList;
            TTESTOBJ *pTestObj = NULL;
            UINT16 u16EntryCnt = 0;
            while (pDicEntry != NULL)
            {
                if (IS_TEST_OBJECT(pDicEntry->Index))
                {
                    pTestObj = (TTESTOBJ *)pDicEntry->pVarPtr;
                    for (u16EntryCnt = 0; u16EntryCnt < pTestObj->u16SubIndex0; u16EntryCnt++)
                    {
                        pTestObj->paEntries[u16EntryCnt]->Control = 0;
                    }
                }
                pDicEntry = pDicEntry->pNext;
            }
        }

        
    }
    else
    {
        if (dataSize != 4)
        {
            return ABORTIDX_PARAM_LENGTH_ERROR;
        }

        /*Check Data (it is only valid to write low Byte, the high Byte contains status information)*/
        MBXMEMCPY(&NewTest, pData, 4);
        NewTestIndex = (UINT16)(NewTest >> 16);
        NewTestSubIndex = (UINT8)((NewTest >> 8) & 0xFF);

        pObject = OBJ_GetObjectHandle(NewTestIndex);

        /*new index is not available*/
        if (NewTestIndex != 0 && (pObject == NULL || NewTestIndex < 0x2000 || NewTestIndex > 0x2FFD))
            return ABORTIDX_VALUE_EXCEEDED;

        /*disable current controlled test*/
        if (sTestControlObj.aEntries[(subindex - 1)].pTest != NULL)
        {
            sTestControlObj.aEntries[(subindex - 1)].pTest->Control &= ~0x1;
        }

        if (NewTestIndex == 0)
        {
            /*reset entry*/
            sTestControlObj.aEntries[(subindex - 1)].pTest = NULL;
            sTestControlObj.aEntries[(subindex - 1)].TestIndex = 0;
            sTestControlObj.aEntries[(subindex - 1)].TestSubindex = 0;
        }
        else
        {
            /*update test control entry*/
            pTestObject = (TTESTOBJ OBJMEM *) pObject->pVarPtr;

            if (pTestObject != NULL)
            {
                sTestControlObj.aEntries[(subindex - 1)].pTest = pTestObject->paEntries[NewTestSubIndex]; //Get handle to test
            }
            sTestControlObj.aEntries[(subindex - 1)].TestIndex = NewTestIndex;
            sTestControlObj.aEntries[(subindex - 1)].TestSubindex = NewTestSubIndex;
        }
    }

    return 0;
}

/////////////////////////////////////////////////////////////////////////////////////////
/**
 \param     index               index of the requested object.
 \param     subindex            subindex of the requested object.
 \param     objSize             size of the requested object data, calculated with OBJ_GetObjectLength
 \param     pData               Pointer to the buffer where the data can be copied to
 \param     bCompleteAccess     Indicates if a complete read of all subindices of the
                                object shall be done or not

 \return    result of the read operation (0 (success) or an abort code (ABORTIDX_.... defined in
            sdosrv.h))

 \brief     This function reads the test control object
 *////////////////////////////////////////////////////////////////////////////////////////

UINT8 ReadObject0x2FFF(UINT16 index, UINT8 subindex, UINT32 dataSize, UINT16 MBXMEM * pData, UINT8 bCompleteAccess) 
{
    UINT16 *pTmpData = pData;

    /*Complete Access is not supported for Test objects*/
    if (bCompleteAccess)
        return ABORTIDX_UNSUPPORTED_ACCESS;

    /*handle only test control object with this function*/
    if (index != 0x2FFF)
        return ABORTIDX_OBJECT_NOT_EXISTING;

    if (subindex > TEST_CONTROL_BITS)
        return ABORTIDX_SUBINDEX_NOT_EXISTING;

    if (subindex > 0) 
    {
        *pTmpData = ((UINT16) sTestControlObj.aEntries[(subindex - 1)].TestSubindex) << 8;
        pTmpData++;
        *pTmpData = sTestControlObj.aEntries[(subindex - 1)].TestIndex;
    }
    else
    {
        *pData = sTestControlObj.u16SubIndex0;
    }

    return 0;
}

/////////////////////////////////////////////////////////////////////////////////////////
/**
 \param    index              index of the requested object.
 \param    subindex           subindex of the requested object.
 \param    dataSize           received data size of the SDO Download
 \param    pData              Pointer to the buffer where the written data can be copied from
 \param    bCompleteAccess    Indicates if a complete write of all subindices of the
                              object shall be done or not

 \return    result of the write operation (0 (success) or an abort code (ABORTIDX_.... defined in
            sdosrv.h))

 \brief     This function writes the Bit3 Array object
 *////////////////////////////////////////////////////////////////////////////////////////

UINT8 WriteObject0x3007(UINT16 index, UINT8 subindex, UINT32 dataSize, UINT16 MBXMEM * pData, UINT8 bCompleteAccess) 
{
    UINT8 result = 0;

    if (subindex > sObj3007.subindex0) 
    {
        return ABORTIDX_SUBINDEX_NOT_EXISTING;
    }


    if (subindex == 0) 
    {
        sObj3007.subindex0 = SWAPWORD(*pData);

        if (dataSize > 2)
            pData++;
    }


    if (bCompleteAccess) 
    {
        if (subindex > 1)
            return ABORTIDX_UNSUPPORTED_ACCESS;

        sObj3007.Value = SWAPWORD(*pData);
    }
    else
    {
        if (subindex > 0) 
        {
            UINT16 TmpVal = SWAPWORD(*pData) << ((subindex - 1) * 3);

            UINT16 Mask = 0x3 << ((subindex - 1) * 3); //get BitMask for corresponding subindex

            sObj3007.Value &= ~Mask;
            sObj3007.Value |= TmpVal;
        }
    }

    return result;
}

/////////////////////////////////////////////////////////////////////////////////////////
/**
 \param     index               index of the requested object.
 \param     subindex            subindex of the requested object.
 \param     objSize             size of the requested object data, calculated with OBJ_GetObjectLength
 \param     pData               Pointer to the buffer where the data can be copied to
 \param     bCompleteAccess     Indicates if a complete read of all subindices of the
                                object shall be done or not

 \return    result of the read operation (0 (success) or an abort code (ABORTIDX_.... defined in
            sdosrv.h))

 \brief     This function reads the Bit3 Array object
 *////////////////////////////////////////////////////////////////////////////////////////

UINT8 ReadObject0x3007(UINT16 index, UINT8 subindex, UINT32 dataSize, UINT16 MBXMEM * pData, UINT8 bCompleteAccess) 
{
    UINT8 result = 0;

    if (subindex > sObj3007.subindex0) 
    {
        return ABORTIDX_SUBINDEX_NOT_EXISTING;
    }

    HMEMSET(pData, 0x00, dataSize);

    if (subindex == 0) 
    {
        *pData = SWAPWORD(sObj3007.subindex0);

        if (dataSize > 2)
            pData++;
    }


    if (bCompleteAccess) 
    {
        if (subindex > 1)
            return ABORTIDX_UNSUPPORTED_ACCESS;

        *pData = SWAPWORD(sObj3007.Value);
    }
    else
    {
        if (subindex > 0) 
        {
            UINT16 TmpVal = SWAPWORD(sObj3007.Value) >> ((subindex - 1) * 3);

            *pData = (TmpVal & 0x3);
        }
    }


    return result;
}

/////////////////////////////////////////////////////////////////////////////////////////
/**
 \param     index               index of the requested object.
 \param     subindex            subindex of the requested object.
 \param     objSize             size of the requested object data, calculated with OBJ_GetObjectLength
 \param     pData               Pointer to the buffer where the data can be copied to
 \param     bCompleteAccess     Indicates if a complete read of all subindices of the
                                object shall be done or not

 \return    result of the read operation (0 (success) or an abort code (ABORTIDX_.... defined in
            sdosrv.h))

 \brief     This function reads the slave configuration object
 *////////////////////////////////////////////////////////////////////////////////////////

UINT8 ReadObject0x8000(UINT16 index, UINT8 subindex, UINT32 dataSize, UINT16 MBXMEM * pData, UINT8 bCompleteAccess) 
{
    /*Complete Access is not supported for Test objects*/
    if (bCompleteAccess)
        return ABORTIDX_UNSUPPORTED_ACCESS;

    if (subindex > 0) 
    {
        *pData = (UINT8) aSlaveConfig[(subindex - 1)].bValue;
    }
    else
    {
        *pData = (SIZEOF(aSlaveConfig) / SIZEOF(TSLV_CONFIG));
    }

    return 0;
}

#if DC_SUPPORTED
/////////////////////////////////////////////////////////////////////////////////////////
/**
 \param     index               index of the requested object.
 \param     subindex            subindex of the requested object.
 \param     objSize             size of the requested object data, calculated with OBJ_GetObjectLength
 \param     pData               Pointer to the buffer where the data can be copied to
 \param     bCompleteAccess     Indicates if a complete read of all subindices of the
                                object shall be done or not

 \return    result of the read operation (0 (success) or an abort code (ABORTIDX_.... defined in
            sdosrv.h))

 \brief     This function reads the ESM variables object 0xA000
 *////////////////////////////////////////////////////////////////////////////////////////

UINT8 ReadObject0xA000(UINT16 index, UINT8 subindex, UINT32 dataSize, UINT16 MBXMEM * pData, UINT8 bCompleteAccess) {
    /*Complete Access is not supported for ESM variables object*/
    if (bCompleteAccess)
        return ABORTIDX_UNSUPPORTED_ACCESS;

    if (index != 0xA000)
        return ABORTIDX_UNSUPPORTED_ACCESS;

    if (pData == NULL)
        return ABORTIDX_OUT_OF_MEMORY;


    *pData = 0;

    switch (subindex) {
        case 0:
            *pData = A000Subindex0;
            break;
        case 1: /* nEcatStateTrans (UINT16)*/
            *pData = nEcatStateTrans;
            break;
        case 2: /* EsmTimeoutCounter (Int16)*/
            *pData = EsmTimeoutCounter;
            break;
        case 3: /* i16WaitForPllRunningTimeout*/
            *pData = i16WaitForPllRunningTimeout;
            break;
        case 4: /* i16WaitForPllRunningCnt*/
            *pData = i16WaitForPllRunningCnt;
            break;
        case 5: /* u16LocalErrorCode*/
            *pData = u16LocalErrorCode;
            break;
        case 6: /* bApplEsmPending*/
            *pData = bApplEsmPending;
            break;
        case 7: /* bEcatWaitForAlControlRes*/
            *pData = bEcatWaitForAlControlRes;
            break;
        case 8: /* bEcatOutputUpdateRunning*/
            *pData = bEcatOutputUpdateRunning;
            break;
        case 9: /* bEcatInputUpdateRunning*/
            *pData = bEcatInputUpdateRunning;
            break;
        case 10: /* bEcatFirstOutputsReceived*/
#if MAX_PD_OUTPUT_SIZE > 0
            *pData = bEcatFirstOutputsReceived;
#else
            *pData = 0;
#endif
            break;
        case 11: /* bLocalErrorFlag*/
/*ECATCHANGE_START(V5.13) ESM1*/
        {
            BOOL tmp = STATE_VALID(u8LocalErrorState);
            *pData = tmp;
        }
/*ECATCHANGE_END(V5.13) ESM1*/
            break;
        case 17: /* bWdTrigger*/
#if MAX_PD_OUTPUT_SIZE > 0
            *pData = bWdTrigger;
#else
            *pData = 0;
#endif
            break;
        case 18: /* bDcSyncActive*/
            *pData = bDcSyncActive;
            break;
        case 19: /* bEscIntEnabled*/
            *pData = bEscIntEnabled;
            break;
        case 20: /* bDcRunning*/
            *pData = bDcRunning;
            break;
        case 21: /* bSmSyncSequenceValid*/
            *pData = bSmSyncSequenceValid;
            break;
        case 33: /* EcatWdValue (UINT16)*/
            *pData = EcatWdValue;
            break;
        case 34: /* EcatWdCounter (UINT16)*/
#if !ESC_SM_WD_SUPPORTED
            *pData = EcatWdCounter;
#else
            *pData = 0;
#endif
            break;
        case 35: /* Sync0WdCounter (UINT16)*/
            *pData = Sync0WdCounter;
            break;
        case 36: /* Sync0WdValue (UINT16)*/
            *pData = Sync0WdValue;
            break;
        case 37: /* Sync1WdCounter (UINT16)*/
            *pData = Sync1WdCounter;
            break;
        case 38: /* Sync1WdValue (UINT16)*/
            *pData = Sync1WdValue;
            break;
        case 39: /* LatchInputSync0Value (UINT16)*/
            *pData = LatchInputSync0Value;
            break;
        case 40: /* u16SmSync0Value (UINT16)*/
            *pData = u16SmSync0Value;
            break;
        case 41: /* u16SmSync0Counter (UINT16)*/
            *pData = u16SmSync0Counter;
            break;
        default:
            return ABORTIDX_SUBINDEX_NOT_EXISTING;
    }

    return 0;
}
#endif

/////////////////////////////////////////////////////////////////////////////////////////
/**
 \param    index              index of the requested object.
 \param    subindex           subindex of the requested object.
 \param    dataSize           received data size of the SDO Download
 \param    pData              Pointer to the buffer where the written data can be copied from
 \param    bCompleteAccess    Indicates if a complete write of all subindices of the
                              object shall be done or not

 \return    result of the write operation (0 (success) or an abort code (ABORTIDX_.... defined in
            sdosrv.h))

 \brief     This function writes the test objects
 *////////////////////////////////////////////////////////////////////////////////////////

UINT8 WriteTestObject(UINT16 index, UINT8 subindex, UINT32 dataSize, UINT16 MBXMEM * pData, UINT8 bCompleteAccess) 
{
    OBJCONST TOBJECT OBJMEM * pObject = NULL;
    TTESTOBJ OBJMEM * pTestObject = NULL;
    UINT16 Value = 0x0000;

    /*Complete Access is not supported for Test objects*/
    if (bCompleteAccess)
        return ABORTIDX_UNSUPPORTED_ACCESS;

    /*Subindex 0 is read only*/
    if (subindex == 0)
        return ABORTIDX_READ_ONLY_ENTRY;

    /*check if the data size is valid*/
    if (dataSize != BIT2BYTE(TEST_ENTRY_BIT_LENGTH))
        return ABORTIDX_PARAM_LENGTH_ERROR;


    pObject = OBJ_GetObjectHandle(index);

    if (pObject == NULL)
        return ABORTIDX_OBJECT_NOT_EXISTING;

    pTestObject = (TTESTOBJ OBJMEM *) pObject->pVarPtr;

    if (pTestObject == NULL)
        return ABORTIDX_GENERAL_ERROR;

    /*Check Data (it is only valid to write low Byte, the high Byte contains status information)*/
    Value = SWAPWORD(*pData);
    if (Value & 0xFF00)
        return ABORTIDX_VALUE_EXCEEDED;

    pTestObject->paEntries[(subindex - 1)]->Control = Value;
    pTestObject->paEntries[(subindex - 1)]->Counter = 0;

    return 0;
}

/////////////////////////////////////////////////////////////////////////////////////////
/**
 \param     index               index of the requested object.
 \param     subindex            subindex of the requested object.
 \param     objSize             size of the requested object data, calculated with OBJ_GetObjectLength
 \param     pData               Pointer to the buffer where the data can be copied to
 \param     bCompleteAccess     Indicates if a complete read of all subindices of the
                                object shall be done or not

 \return    result of the read operation (0 (success) or an abort code (ABORTIDX_.... defined in
            sdosrv.h))

 \brief     This function reads the test objects
 *////////////////////////////////////////////////////////////////////////////////////////

UINT8 ReadTestObject(UINT16 index, UINT8 subindex, UINT32 dataSize, UINT16 MBXMEM * pData, UINT8 bCompleteAccess) 
{
    OBJCONST TOBJECT OBJMEM * pObject = NULL;
    TTESTOBJ OBJMEM * pTestObject = NULL;
    UINT16 Value = 0x0000;

    /*Complete Access is not supported for Test objects*/
    if (bCompleteAccess)
        return ABORTIDX_UNSUPPORTED_ACCESS;

    pObject = OBJ_GetObjectHandle(index);

    if (pObject == NULL)
        return ABORTIDX_OBJECT_NOT_EXISTING;

    pTestObject = (TTESTOBJ OBJMEM *) pObject->pVarPtr;

    if (pTestObject == NULL)
        return ABORTIDX_GENERAL_ERROR;

    if (subindex == 0) 
    {
        Value = pTestObject->u16SubIndex0;

        *pData = (UINT8) (Value & 0xFF);
    }
    else
   {
        Value = (pTestObject->paEntries[(subindex - 1)]->Counter << 8) | pTestObject->paEntries[(subindex - 1)]->Control;

        *pData = SWAPWORD(Value);
    }

    return 0;
}

#if SDO_RES_INTERFACE
/////////////////////////////////////////////////////////////////////////////////////////
/**
 \param    index              index of the requested object.
 \param    subindex           subindex of the requested object.
 \param    dataSize           received data size of the SDO Download
 \param    pData              Pointer to the buffer where the written data can be copied from
 \param    bCompleteAccess    Indicates if a complete write of all subindices of the
                              object shall be done or not

 \return    ABORTIDX_WORKING

 \brief     This function stores the values to handle the SDO request afterwards
 *////////////////////////////////////////////////////////////////////////////////////////

UINT8 WriteObject0x3006(UINT16 index, UINT8 subindex, UINT32 dataSize, UINT16 MBXMEM * pData, UINT8 bCompleteAccess) 
{
    if (SDOResult == ABORTIDX_WORKING) 
    {
        if (!IS_TEST_ACTIVE(DelaySdoResponse0x3006) || (bUnlockSdoRequest == TRUE))
        {
            if (HUGE_OBJ_BYTE_SIZE < dataSize)
                return ABORTIDX_PARAM_LENGTH_TOO_LONG;

            MEMCPY(HugeObj, pData, dataSize);

            SDOResult = 0;
            bUnlockSdoRequest = FALSE;
        }
        else
        {
            /*SDO request is still pending*/
            SDOResult = ABORTIDX_WORKING;
        }
    }
    else
    {
        /*Set SDO access to pending*/
        SDOResult = ABORTIDX_WORKING;
    }

    if (SDOResult == ABORTIDX_WORKING)
    {
        bUnlockSdoRequest = FALSE;
    }

    return SDOResult;
}

/////////////////////////////////////////////////////////////////////////////////////////
/**
 \param     index               index of the requested object.
 \param     subindex            subindex of the requested object.
 \param     objSize             size of the requested object data, calculated with OBJ_GetObjectLength
 \param     pData               Pointer to the buffer where the data can be copied to
 \param     bCompleteAccess     Indicates if a complete read of all subindices of the
                                object shall be done or not

 \return    ABORTIDX_WORKING

 \brief     This function stores the values to handle the SDO request afterwards
 *////////////////////////////////////////////////////////////////////////////////////////

UINT8 ReadObject0x3006(UINT16 index, UINT8 subindex, UINT32 dataSize, UINT16 MBXMEM * pData, UINT8 bCompleteAccess) 
{
    if (SDOResult == ABORTIDX_WORKING) 
    {
        if (!IS_TEST_ACTIVE(DelaySdoResponse0x3006) || (bUnlockSdoRequest == TRUE))
        {

            if (HUGE_OBJ_BYTE_SIZE < dataSize)
                return ABORTIDX_PARAM_LENGTH_TOO_LONG;

            MEMCPY(pData, HugeObj, dataSize);

            SDOResult = 0;
            bUnlockSdoRequest = FALSE;
        }
        else
        {
            /*SDO request is still pending*/
            SDOResult = ABORTIDX_WORKING;
        }
    }
    else
    {
        /*Set SDO access to pending*/
        SDOResult = ABORTIDX_WORKING;
    }

    if (SDOResult == ABORTIDX_WORKING)
    {
        bUnlockSdoRequest = FALSE;
    }
    return SDOResult;
}

/////////////////////////////////////////////////////////////////////////////////////////
/**
 \param    index              index of the requested object.
 \param    subindex           subindex of the requested object.
 \param    dataSize           received data size of the SDO Download
 \param    pData              Pointer to the buffer where the written data can be copied from
 \param    bCompleteAccess    Indicates if a complete write of all subindices of the
                              object shall be done or not

 \return    ABORTIDX_WORKING

 \brief     This function stores the values to handle the SDO request afterwards
 *////////////////////////////////////////////////////////////////////////////////////////

UINT8 WriteObject0x3002(UINT16 index, UINT8 subindex, UINT32 dataSize, UINT16 MBXMEM * pData, UINT8 bCompleteAccess) 
{
    if (SDOResult == ABORTIDX_WORKING) 
    {
        u32Obj = 0;

        if (SIZEOF(u32Obj) < dataSize)
            return ABORTIDX_PARAM_LENGTH_TOO_LONG;

        MEMCPY(&u32Obj, pData, dataSize);

        SDOResult = 0;

#if TEST_APPLICATION && DIAGNOSIS_SUPPORTED
        {
            UINT16 AddMessageResult = 0;
            //store written value in a Diagnosis message
            TDIAGMSGPARAMINFO aParam[1];
            aParam[0].ParamFlags = (DIAG_MSG_PARAM_TYPE_DATA << DIAG_MSG_PARAM_TYPE_OFFSET) | (UINT16)DEFTYPE_UNSIGNED32;
            aParam[0].pParam = (MEM_ADDR *)&u32Obj;

            AddMessageResult = Diag_CreateNewMessage((0xE000 + u32Obj), DIAG_MSG_TYPE_ERROR, 0x2 /*counter value (see ESI file)*/, 1, aParam);

            if (AddMessageResult == DIAG_NO_MESSAGE_MEMORY)
            {
                return ABORTIDX_OUT_OF_MEMORY;
            }
        }
#endif

    }
    else
    {
        /*Set SDO access to pending*/
        SDOResult = ABORTIDX_WORKING;
    }
    return SDOResult;
}

/////////////////////////////////////////////////////////////////////////////////////////
/**
 \param     index               index of the requested object.
 \param     subindex            subindex of the requested object.
 \param     objSize             size of the requested object data, calculated with OBJ_GetObjectLength
 \param     pData               Pointer to the buffer where the data can be copied to
 \param     bCompleteAccess     Indicates if a complete read of all subindices of the
                                object shall be done or not

 \return    ABORTIDX_WORKING

 \brief     This function stores the values to handle the SDO request afterwards
 *////////////////////////////////////////////////////////////////////////////////////////

UINT8 ReadObject0x3002(UINT16 index, UINT8 subindex, UINT32 dataSize, UINT16 MBXMEM * pData, UINT8 bCompleteAccess) 
{
    if (SDOResult == ABORTIDX_WORKING) 
    {
        if (SIZEOF(u32Obj) < dataSize)
            return ABORTIDX_PARAM_LENGTH_TOO_LONG;

        MEMCPY(pData, &u32Obj, dataSize);

        SDOResult = 0;
    }
    else
    {
        /*Set SDO access to pending*/
        SDOResult = ABORTIDX_WORKING;
    }
    return SDOResult;
}
#endif //#if SDO_RES_INTERFACE

#if SEGMENTED_SDO_SUPPORTED
/////////////////////////////////////////////////////////////////////////////////////////
/**
 \param    index              index of the requested object.
 \param    subindex           subindex of the requested object.
 \param    dataSize           received data size of the SDO Download
 \param    pData              Pointer to the buffer where the written data can be copied from
 \param    bCompleteAccess    Indicates if a complete write of all subindices of the
                              object shall be done or not

 \return    0 or SDO ABORT CODE

 \brief     This functions handles write access to the huge array object.
            The entry size can be modified by an single WORD access to SI1. All other write accesses will be ignored (no Abort code will be returned)
 *////////////////////////////////////////////////////////////////////////////////////////

UINT8 WriteObject0x3009_0x300A(UINT16 index, UINT8 subindex, UINT32 dataSize, UINT16 MBXMEM * pData, UINT8 bCompleteAccess) 
{
    if (index == 0x3009 || index == 0x300A) 
    {
        if (subindex == 1 && (bCompleteAccess == FALSE)) 
        {
            if (dataSize == 2)
                asEntryDesc0x3009[1].BitLength = *pData;
        }
    }
    return 0;
}

/////////////////////////////////////////////////////////////////////////////////////////
/**
 \param     index               index of the requested object.
 \param     subindex            subindex of the requested object.
 \param     dataSize             size of the requested object data, calculated with OBJ_GetObjectLength
 \param     pData               Pointer to the buffer where the data can be copied to
 \param     bCompleteAccess     Indicates if a complete read of all subindices of the
                                object shall be done or not

 \return    0 or SDO ABORT CODE

 \brief     This function stores the values to handle the SDO request afterwards
 *////////////////////////////////////////////////////////////////////////////////////////

UINT8 ReadObject0x3009_0x300A(UINT16 index, UINT8 subindex, UINT32 dataSize, UINT16 MBXMEM * pData, UINT8 bCompleteAccess) 
{
    if (subindex == 0) 
    {
        *pData = sObj3009.subindex0;

        if (dataSize >= 2) 
        {
            pData++;
            dataSize -= 2;
        }
        else
        {
            dataSize = 0;
        } 
    }

    if (dataSize >= 2 && (subindex == 1 || bCompleteAccess)) 
    {
        //copy current configured bit size at the beginning of the SI1
        *pData = asEntryDesc0x3009[1].BitLength;
        pData++;
        dataSize -= 2;
    }

    if (dataSize > 0) 
    {
        //fill all following bytes with defined value
        HMEMSET(pData, sObj3009.Value, dataSize);
    }

    return 0;
}
#endif //#if SEGMENTED_SDO_SUPPORTED

#if EOE_SUPPORTED
UINT8 WriteEoeSendData(UINT16 index, UINT8 subindex, UINT32 dataSize, UINT16 MBXMEM * pData, UINT8 bCompleteAccess)
{
    if (index != 0x3017)
        return ABORTIDX_UNSUPPORTED_ACCESS;
    
    if (subindex > 0)
        return ABORTIDX_SUBINDEX_NOT_EXISTING;

    if (bCompleteAccess)
        return ABORTIDX_UNSUPPORTED_ACCESS;
    
    //free memory if it was not freed by the stack before
    if (i8EoeSendPending != 0 && pObj3017_EoeSendData != NULL)
        FREEMEM(pObj3017_EoeSendData);

    if (dataSize > 0)
    {
        pObj3017_EoeSendData = ALLOCMEM(dataSize);
        if (pObj3017_EoeSendData == NULL)
            return ABORTIDX_OUT_OF_MEMORY;

        Obj3017_EoeSendDataLength = (UINT16)dataSize;

        HMEMCPY(pObj3017_EoeSendData, pData, dataSize);

        i8EoeSendPending = 2;
    }

    return 0;
}
UINT8 ReadEoeSendData(UINT16 index, UINT8 subindex, UINT32 dataSize, UINT16 MBXMEM * pData, UINT8 bCompleteAccess)
{
    if (index != 0x3017)
        return ABORTIDX_UNSUPPORTED_ACCESS;

    if (subindex > 0)
        return ABORTIDX_SUBINDEX_NOT_EXISTING;

    if (bCompleteAccess)
        return ABORTIDX_UNSUPPORTED_ACCESS;

    if (pObj3017_EoeSendData == NULL)
    {
        HMEMSET(pData, dataSize, 0x00);
    }

    if (dataSize > Obj3017_EoeSendDataLength)
    {
        return ABORTIDX_VALUE_EXCEEDED;
    }
    else if(pObj3017_EoeSendData != NULL)
    {
        HMEMCPY(pData, pObj3017_EoeSendData, dataSize);
    }

    return 0;
}
#endif

/*ECATCHANGE_START(V5.13) TEST1*/
/////////////////////////////////////////////////////////////////////////////////////////
/**
\param    index              index of the requested object.
\param    subindex           subindex of the requested object.
\param    dataSize           received data size of the SDO Download
\param    pData              Pointer to the buffer where the written data can be copied from
\param    bCompleteAccess    Indicates if a complete write of all subindices of the
object shall be done or not

\return    0 or SDO ABORT CODE

\brief     This function handles write access to trigger ESM transition object. If written the function ECAT_StateChange is called in the next main loop cycle
*////////////////////////////////////////////////////////////////////////////////////////
UINT8 WriteTriggerEsmObject(UINT16 index, UINT8 subindex, UINT32 dataSize, UINT16 MBXMEM * pData, UINT8 bCompleteAccess)
{
    if (index != 0x301A)
        return ABORTIDX_UNSUPPORTED_ACCESS;

    if ((subindex != 1) || (bCompleteAccess == FALSE))
        return ABORTIDX_DATA_CANNOT_BE_ACCESSED_BECAUSE_OF_LOCAL_CONTROL;

    if (dataSize != 4)
        return ABORTIDX_PARAM_LENGTH_ERROR;

#if CONTROLLER_32BIT
    HMEMCPY(&sObj301A_TriggerEsm.StatusCode, pData, 4);
#else
    HMEMCPY(&sObj301A_TriggerEsm.Status, pData, 4);
#endif
    CallEcatStateChange = TRUE;
    
    return 0;
}
/*ECATCHANGE_END(V5.13) TEST1*/

#if !TEST_APPLICATION_REDUCED_MEMORY
/////////////////////////////////////////////////////////////////////////////////////////
/**
 \param     index               index of the requested object.
 \param     subindex            subindex of the requested object.
 \param     dataSize             size of the requested object data, calculated with OBJ_GetObjectLength
 \param     pData               Pointer to the buffer where the data can be copied to
 \param     bCompleteAccess     Indicates if a complete read of all subindices of the
                                object shall be done or not

 \return    0 or SDO ABORT CODE

 \brief     This function handles read access to object 0x300C
 *////////////////////////////////////////////////////////////////////////////////////////
UINT8 ReadObject0x300C( UINT16 index, UINT8 subindex, UINT32 dataSize, UINT16 MBXMEM * pData, UINT8 bCompleteAccess )
{
    HMEMSET(pData,0xBA,dataSize);
    return 0;
}
#endif





/////////////////////////////////////////////////////////////////////////////////////////
/**

 \brief    Handles the a new set test object request 

 *////////////////////////////////////////////////////////////////////////////////////////

void HandleTestControlViaEsm()
{
    UINT32 RegValue = 0;
    UINT16 index = 0;
    UINT8 SubIdx = 0;
    UINT16 EscAlControl = 0;

#if ESC_32BIT_ACCESS
    UINT32 tmpVal = 0;
    HW_EscReadDWord(tmpVal, ESC_AL_CONTROL_OFFSET);
    EscAlControl = (UINT16)SWAPDWORD(tmpVal);
#elif ESC_16BIT_ACCESS
    HW_EscReadWord(EscAlControl, ESC_AL_CONTROL_OFFSET);
    EscAlControl = SWAPWORD(EscAlControl);
#else
    HW_EscReadByte(EscAlControl, ESC_AL_CONTROL_OFFSET);
    EscAlControl = SWAPWORD(EscAlControl);
#endif

    if (SET_TEST_REQUEST_PENDING(EscAlControl) == FALSE)
        return;

    HW_EscReadDWord(RegValue, ESC_TESTID_REGISTER);

    if (RegValue != CurrentActiveTest || (RegValue == 0))
    {
        index = (UINT16)((CurrentActiveTest & 0x0000FFFF));
        SubIdx = (UINT8)((CurrentActiveTest & 0x00FF0000) >> 16);

        if (IS_TEST_OBJECT(index))
        {
            /* Disable current Test*/
            OBJCONST TOBJECT OBJMEM *pObject = OBJ_GetObjectHandle(index);

            if (pObject != NULL)
            {
                TTESTOBJ OBJMEM * pTestObject = (TTESTOBJ OBJMEM *) pObject->pVarPtr;

                if (pTestObject != NULL)
                {
                    pTestObject->paEntries[(SubIdx - 1)]->Control = 0;
#ifdef DebugMessage
                    DebugMessage("Test: 0x%X.%X disabled (%d times triggered)\n",pObject->Index, SubIdx,pTestObject->paEntries[(SubIdx - 1)]->Counter);
#endif

                    pTestObject->paEntries[(SubIdx - 1)]->Counter = 0;
                }
            }

        }

        index = (UINT16)((RegValue & 0x0000FFFF));
        SubIdx = (UINT8)((RegValue & 0x00FF0000) >> 16);

        /* Enable New Test*/
        if (IS_TEST_OBJECT(index))
        {
            OBJCONST TOBJECT OBJMEM *pObject = NULL;

            pObject = OBJ_GetObjectHandle(index);

            if (pObject != NULL) {
                TTESTOBJ OBJMEM * pTestObject = (TTESTOBJ OBJMEM *) pObject->pVarPtr;

                if (pTestObject != NULL)
                {
                    pTestObject->paEntries[(SubIdx - 1)]->Control = TESTS_ENABLED;
#ifdef DebugMessage
                    DebugMessage("Test: 0x%X.%X enabled\n",pObject->Index, SubIdx);
#endif
                }
            }
        }
        else
        {
#ifdef DebugMessage
            DebugMessage("Default slave behaviour\n");
#endif
        }

        CurrentActiveTest = RegValue;
    }

    RegValue = (UINT32)SET_TEST_REQUEST;

    HW_EscWriteDWord(RegValue, 0x130);
    
}
#endif //#if COE_SUPPORTED


/////////////////////////////////////////////////////////////////////////////////////////
/**

\brief    This function is called on a Init to 0x2F request and read the CTT Test IDs from ESC_CTT_TESTID_REGISTER

*////////////////////////////////////////////////////////////////////////////////////////

void HandleCttTestCaseID()
{
    UINT32 tmpValue = 0;
    UINT16 EscAlControl = 0;
    UINT32 RegValue = 0;

#if ESC_32BIT_ACCESS
    UINT32 tmpVal = 0;
    HW_EscReadDWord(tmpVal, ESC_AL_CONTROL_OFFSET);
    EscAlControl = (UINT16)SWAPDWORD(tmpVal);
#elif ESC_16BIT_ACCESS
    HW_EscReadWord(EscAlControl, ESC_AL_CONTROL_OFFSET);
    EscAlControl = SWAPWORD(EscAlControl);
#else
    HW_EscReadByte(EscAlControl, ESC_AL_CONTROL_OFFSET);
    EscAlControl = SWAPWORD(EscAlControl);
#endif
    if (SET_CTT_TEST_REQUEST_PENDING(EscAlControl) == FALSE)
        return;

    HW_EscReadDWord(tmpValue, ESC_CTT_TESTID_REGISTER);


    u16CurrentTestID = (UINT16)(tmpValue & 0x0000FFFF);
    u16CurrentTestGroupID = (UINT16)((tmpValue & 0xFFFF0000) >> 16);

    HW_EscReadDWord(tmpValue, ESC_CTT_TESTID_REGISTER + 4);
    u16CurrentTestCaseID = (UINT16)(tmpValue & 0x0000FFFF);

    RegValue = (UINT32)SET_CTT_TEST_REQUEST;

    HW_EscWriteDWord(RegValue, 0x130);
}

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
   UINT16 result = 0;


#ifdef PERF_TEST
    LONGLONG g_Frequency, g_CurentCount, g_LastCount, g_diff, g_sum;
    UINT32 counter = 0;

    g_sum = 0;
#endif    

#if FC1100_HW && _WIN32
    u16FcInstance = 0;

    if (argc > 1)
    {
        u16FcInstance = (UINT16) atoi(argv[1]);
    }
#endif

    result = HW_Init();

    if (result != 0) 
    {
        HW_Release();

#if _PIC24
        return 0;
#else
        return;
#endif
    }

    MainInit();

    APPL_Init();



#ifdef PERF_TEST
#ifdef DebugMessage
    if (QueryPerformanceFrequency((LARGE_INTEGER*) & g_Frequency))
        DebugMessage("frequency %d/s\n", g_Frequency);
#endif

    g_Frequency = g_Frequency / 1000000; //get ticks /us

    QueryPerformanceCounter((LARGE_INTEGER*) & g_CurentCount);
    QueryPerformanceCounter((LARGE_INTEGER*) & g_LastCount);
    g_diff = g_LastCount - g_CurentCount;

#ifdef DebugMessage
    DebugMessage("Querydiff %d\n", g_diff);
#endif
    g_CurentCount = 0;
#endif


    bRunApplication = TRUE;





    do {

        MainLoop();




#ifdef PERF_TEST
        QueryPerformanceCounter((LARGE_INTEGER*) & g_LastCount);
        if (g_CurentCount > 0) 
        {
            g_sum += g_LastCount - g_CurentCount - g_diff;
            counter++;
        }

        g_CurentCount = g_LastCount;



        if (counter > 100000) 
        {
            g_sum = ((g_sum / counter) / g_Frequency);
#ifdef DebugMessage
            DebugMessage("Delay %d\n", g_sum);
#endif
            counter = 0;
            g_sum = 0;
            g_CurentCount = 0;
        }
#endif

    } while (bRunApplication == TRUE);

    HW_Release();
#if _PIC24
    return 0;
#endif
}
#endif //#if USE_DEFAULT_MAIN


#if BACKUP_PARAMETER_SUPPORTED

/*empty implementations to prevent compiler warnings*/
void EE_ResetFlashData(void) 
{}

UINT32 EE_GetChecksum(void) 
{
    return 0;
}

UINT8 EE_IsDefaultDataInitialized(void) 
{
    return 0;
}

void EE_StoreDefaultData(void) 
{}

void EE_LoadDefaultData(void) 
{}

void EE_ReadWordsFromNonVolatileMemory(UINT16 HUGE *pDest, UINT16 srcOffset, UINT16 n) 
{}

UINT32 EE_WriteWordsToNonVolatileMemory(UINT16 destOffset, UINT16 HUGE * pSrc, UINT16 n) 
{
    return 0;
}
#endif

#endif
/** @} */


