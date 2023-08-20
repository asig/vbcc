#ifndef GRAPHICS_GRAPHINT_H
#define GRAPHICS_GRAPHINT_H
/*
**	$Filename: graphics/graphint.h $
**	$Release: 1.3 $
**
**	
**
**	(C) Copyright 1985,1986,1987,1988 Commodore-Amiga, Inc.
**	    All Rights Reserved
*/

#ifndef EXEC_NODES_H
#include <exec/nodes.h>
#endif

/* structure used by AddTOFTask */
struct Isrvstr
{
    struct Node is_Node;
    struct Isrvstr *Iptr;   /* passed to srvr by os */
    int (*code)();
    int (*ccode)();
    int Carg;
};

#endif	/* GRAPHICS_GRAPHINT_H */
