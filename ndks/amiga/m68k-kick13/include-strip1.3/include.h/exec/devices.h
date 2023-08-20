#ifndef	EXEC_DEVICES_H
#define	EXEC_DEVICES_H
#ifndef	EXEC_LIBRARIES_H
#include	"exec/libraries.h"
#endif	!EXEC_LIBRARIES_H
#ifndef	EXEC_PORTS_H
#include	"exec/ports.h"
#endif	!EXEC_PORTS_H
struct	Device	{
struct	Library	dd_Library;
};
struct	Unit	{
struct	MsgPort	unit_MsgPort;
UBYTE	unit_flags;
UBYTE	unit_pad;
UWORD	unit_OpenCnt;
};
#define	UNITF_ACTIVE	(1<<0)
#define	UNITF_INTASK	(1<<1)
#endif
