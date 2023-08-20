#ifndef	DEVICES_CLIPBOARD_H
#define	DEVICES_CLIPBOARD_H
#ifndef	EXEC_NODES_H
#include	"exec/nodes.h"
#endif
#ifndef	EXEC_LISTS_H
#include	"exec/lists.h"
#endif
#ifndef	EXEC_PORTS_H
#include	"exec/ports.h"
#endif
#define	CBD_POST	(CMD_NONSTD+0)
#define	CBD_CURRENTREADID	(CMD_NONSTD+1)
#define	CBD_CURRENTWRITEID	(CMD_NONSTD+2)
#define	CBERR_OBSOLETEID	1
struct	ClipboardUnitPartial	{
struct	Node	cu_Node;
ULONG	cu_UnitNum;
};
struct	IOClipReq	{
struct	Message	io_Message;
struct	Device	*io_Device;
struct	Unit	*io_Unit;
UWORD	io_Command;
UBYTE	io_Flags;
BYTE	io_Error;
ULONG	io_Actual;
ULONG	io_Length;
STRPTR	io_Data;
ULONG	io_Offset;
LONG	io_ClipID;
};
#define	PRIMARY_CLIP	0
struct	SatisfyMsg	{
struct	Message	sm_Msg;
UWORD	sm_Unit;
LONG	sm_ClipID;
};
#endif
