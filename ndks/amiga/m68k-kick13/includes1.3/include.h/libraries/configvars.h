#ifndef LIBRARIES_CONFIGVARS_H
#define LIBRARIES_CONFIGVARS_H
/*
**	$Filename: libraries/configvars.h $
**	$Release: 1.3 $
**
**	software structures for configuration subsystem 
**
**	(C) Copyright 1986,1987,1988 Commodore-Amiga, Inc.
**	    All Rights Reserved
*/

#ifndef EXEC_TYPES_H
#include "exec/types.h"
#endif	!EXEC_TYPES_H

#ifndef EXEC_NODES_H
#include "exec/nodes.h"
#endif !EXEC_NODES_H

#ifndef LIBRARIES_CONFIGREGS_H
#include "libraries/configregs.h"
#endif !LIBRARIES_CONFIGREGS_H


struct ConfigDev {
    struct Node		cd_Node;
    UBYTE		cd_Flags;
    UBYTE		cd_Pad;
    struct ExpansionRom cd_Rom;		/* image of expansion rom area */
    APTR		cd_BoardAddr;	/* where in memory the board is */
    APTR		cd_BoardSize;	/* size in bytes */
    UWORD		cd_SlotAddr;	/* which slot number */
    UWORD		cd_SlotSize;	/* number of slots the board takes */
    APTR		cd_Driver;	/* pointer to node of driver */
    struct ConfigDev *	cd_NextCD;	/* linked list of drivers to config */
    ULONG		cd_Unused[4];	/* for whatever the driver whats */
};

/* cd_Flags */
#define CDB_SHUTUP	0	/* this board has been shut up */
#define CDB_CONFIGME	1	/* this board needs a driver to claim it */

#define CDF_SHUTUP	0x01
#define CDF_CONFIGME	0x02

/* this structure is used by GetCurrentBinding() and SetCurrentBinding() */
struct CurrentBinding {
    struct ConfigDev *	cb_ConfigDev;		/* first configdev in chain */
    UBYTE *		cb_FileName;		/* file name of driver */
    UBYTE *		cb_ProductString;	/* product # string */
    UBYTE **		cb_ToolTypes;		/* tooltypes from disk object */
};


struct ConfigDev *AllocConfigDev(), *FindConfigDev();

#endif	/* LIBRARIES_CONFIGVARS_H */
