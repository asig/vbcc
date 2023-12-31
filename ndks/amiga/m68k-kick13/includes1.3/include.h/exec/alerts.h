#ifndef EXEC_ALERTS_H
#define EXEC_ALERTS_H
/*
**	$Filename: exec/alerts.h $
**	$Release: 1.3 $
**
**	
**
**	(C) Copyright 1985,1986,1987,1988 Commodore-Amiga, Inc.
**	    All Rights Reserved
*/

#define SF_ALERTWACK	(1<<1)	/* in ExecBase.SysFlag */


/*********************************************************************
*
*  Format of the alert error number:
*
*    +-+-------------+----------------+--------------------------------+  
*    |D|  SubSysId   |	General Error |	   SubSystem Specific Error    |
*    +-+-------------+----------------+--------------------------------+
*
*		     D:	 DeadEnd alert
*	      SubSysId:	 indicates ROM subsystem number.
*	 General Error:	 roughly indicates what the error was
*	Specific Error:	 indicates more detail
**********************************************************************/

/*********************************************************************
*
*  General Dead-End Alerts
*
*********************************************************************/

/*------ alert types */
#define AT_DeadEnd	0x80000000
#define AT_Recovery	0x00000000

/*------ general purpose alert codes */
#define AG_NoMemory	0x00010000
#define AG_MakeLib	0x00020000
#define AG_OpenLib	0x00030000
#define AG_OpenDev	0x00040000
#define AG_OpenRes	0x00050000
#define AG_IOError	0x00060000
#define AG_NoSignal	0x00070000

/*------ alert objects: */
#define AO_ExecLib	0x00008001
#define AO_GraphicsLib	0x00008002
#define AO_LayersLib	0x00008003
#define AO_Intuition	0x00008004
#define AO_MathLib	0x00008005
#define AO_CListLib	0x00008006
#define AO_DOSLib	0x00008007
#define AO_RAMLib	0x00008008
#define AO_IconLib	0x00008009
#define AO_ExpansionLib 0x0000800A
#define AO_AudioDev	0x00008010
#define AO_ConsoleDev	0x00008011
#define AO_GamePortDev	0x00008012
#define AO_KeyboardDev	0x00008013
#define AO_TrackDiskDev 0x00008014
#define AO_TimerDev	0x00008015
#define AO_CIARsrc	0x00008020
#define AO_DiskRsrc	0x00008021
#define AO_MiscRsrc	0x00008022
#define AO_BootStrap	0x00008030
#define AO_Workbench	0x00008031


/*********************************************************************
*
*   Specific Dead-End Alerts:
*
*********************************************************************/

/*------ exec.library */
#define AN_ExecLib	0x01000000
#define AN_ExcptVect	0x81000001	/* 68000 exception vector checksum */
#define AN_BaseChkSum	0x81000002	/* execbase checksum */
#define AN_LibChkSum	0x81000003	/* library checksum failure */
#define AN_LibMem	0x81000004	/* no memory to make library */
#define AN_MemCorrupt	0x81000005	/* corrupted memory list */
#define AN_IntrMem	0x81000006	/* no memory for interrupt servers */
#define AN_InitAPtr	0x81000007	/* InitStruct() of an APTR source */
#define AN_SemCorrupt	0x81000008	/* a semaphore is in illegal state */
#define AN_FreeTwice	0x81000009	/* freeing memory already freed */
#define AN_BogusExcpt	0x8100000A	/* illegal 68k exception taken */

/*------ graphics.library */
#define AN_GraphicsLib	0x02000000
#define AN_GfxNoMem	0x82010000	/* graphics out of memory */
#define AN_LongFrame	0x82010006	/* long frame, no memory */
#define AN_ShortFrame	0x82010007	/* short frame, no memory */
#define AN_TextTmpRas	0x02010009	/* text, no memory for TmpRas */
#define AN_BltBitMap	0x8201000A	/* BltBitMap, no memory */
#define AN_RegionMemory 0x8201000B	/* regions, memory not available */
#define AN_MakeVPort	0x82010030	/* MakeVPort, no memory */
#define AN_GfxNoLCM	0x82011234	/* emergency memory not available */	

/*------ layers.library */
#define AN_LayersLib	0x03000000
#define AN_LayersNoMem	0x83010000	/* layers out of memory */

/*------ intuition.library */
#define AN_Intuition	0x04000000
#define AN_GadgetType	0x84000001	/* unknown gadet type */
#define AN_BadGadget	0x04000001	/* Recovery form of AN_GadgetType */
#define AN_CreatePort	0x84010002	/* create port, no memory */
#define AN_ItemAlloc	0x04010003	/* item plane alloc, no memory */
#define AN_SubAlloc	0x04010004	/* sub alloc, no memory */
#define AN_PlaneAlloc	0x84010005	/* plane alloc, no memory */
#define AN_ItemBoxTop	0x84000006	/* item box top < RelZero */
#define AN_OpenScreen	0x84010007	/* open screen, no memory */
#define AN_OpenScrnRast 0x84010008	/* open screen, raster alloc, no memory */
#define AN_SysScrnType	0x84000009	/* open sys screen, unknown type */
#define AN_AddSWGadget	0x8401000A	/* add SW gadgets, no memory */
#define AN_OpenWindow	0x8401000B	/* open window, no memory */
#define AN_BadState	0x8400000C	/* Bad State Return entering Intuition */
#define AN_BadMessage	0x8400000D	/* Bad Message received by IDCMP */
#define AN_WeirdEcho	0x8400000E	/* Weird echo causing incomprehension */
#define AN_NoConsole	0x8400000F	/* couldn't open the Console Device */


/*------ math.library */
#define AN_MathLib	0x05000000

/*------ clist.library */
#define AN_CListLib	0x06000000

/*------ dos.library */
#define AN_DOSLib	0x07000000
#define AN_StartMem	0x07010001	/* no memory at startup */
#define AN_EndTask	0x07000002	/* EndTask didn't */
#define AN_QPktFail	0x07000003	/* Qpkt failure */
#define AN_AsyncPkt	0x07000004	/* Unexpected packet received */
#define AN_FreeVec	0x07000005	/* Freevec failed */
#define AN_DiskBlkSeq	0x07000006	/* Disk block sequence error */
#define AN_BitMap	0x07000007	/* Bitmap corrupt */
#define AN_KeyFree	0x07000008	/* Key already free */
#define AN_BadChkSum	0x07000009	/* Invalid checksum */
#define AN_DiskError	0x0700000A	/* Disk Error */
#define AN_KeyRange	0x0700000B	/* Key out of range */
#define AN_BadOverlay	0x0700000C	/* Bad overlay */

/*------ ramlib.library */
#define AN_RAMLib	0x08000000
#define AN_BadSegList	0x08000001	/* no overlays in library seglists */

/*------ icon.library */
#define AN_IconLib	0x09000000

/*------ expansion.library */
#define AN_ExpansionLib 0x0A000000
#define AN_BadExpansionFree	0x0A000001

/*------ audio.device */
#define AN_AudioDev	0x10000000

/*------ console.device */
#define AN_ConsoleDev	0x11000000

/*------ gameport.device */
#define AN_GamePortDev	0x12000000

/*------ keyboard.device */
#define AN_KeyboardDev	0x13000000

/*------ trackdisk.device */
#define AN_TrackDiskDev 0x14000000
#define AN_TDCalibSeek	0x14000001	/* calibrate: seek error */
#define AN_TDDelay	0x14000002	/* delay: error on timer wait */

/*------ timer.device */
#define AN_TimerDev	0x15000000
#define AN_TMBadReq	0x15000001	/* bad request */
#define AN_TMBadSupply	0x15000002	/* power supply does not supply ticks */

/*------ cia.resource */
#define AN_CIARsrc	0x20000000

/*------ disk.resource */
#define AN_DiskRsrc	0x21000000
#define AN_DRHasDisk	0x21000001	/* get unit: already has disk */
#define AN_DRIntNoAct	0x21000002	/* interrupt: no active unit */

/*------ misc.resource */
#define AN_MiscRsrc	0x22000000

/*------ bootstrap */
#define AN_BootStrap	0x30000000
#define AN_BootError	0x30000001	/* boot code returned an error */

/*------ Workbench */
#define AN_Workbench	0x31000000

/*------ DiskCopy */
#define AN_DiskCopy	0x32000000

#endif	/* EXEC_ALERTS_H */
