#ifndef EXEC_TASKS_H
#define EXEC_TASKS_H
/*
**	$Filename: exec/tasks.h $
**	$Release: 1.3 $
**
**	
**
**	(C) Copyright 1985,1986,1987,1988 Commodore-Amiga, Inc.
**	    All Rights Reserved
*/

#ifndef EXEC_NODES_H
#include "exec/nodes.h"
#endif !EXEC_NODES_H

#ifndef EXEC_LISTS_H
#include "exec/lists.h"
#endif !EXEC_LISTS_H


struct Task {
    struct  Node tc_Node;
    UBYTE   tc_Flags;
    UBYTE   tc_State;
    BYTE    tc_IDNestCnt;	    /* intr disabled nesting*/
    BYTE    tc_TDNestCnt;	    /* task disabled nesting*/
    ULONG   tc_SigAlloc;	    /* sigs allocated */
    ULONG   tc_SigWait;		    /* sigs we are waiting for */
    ULONG   tc_SigRecvd;	    /* sigs we have received */
    ULONG   tc_SigExcept;	    /* sigs we will take excepts for */
    UWORD   tc_TrapAlloc;	    /* traps allocated */
    UWORD   tc_TrapAble;	    /* traps enabled */
    APTR    tc_ExceptData;	    /* points to except data */
    APTR    tc_ExceptCode;	    /* points to except code */
    APTR    tc_TrapData;	    /* points to trap code */
    APTR    tc_TrapCode;	    /* points to trap data */
    APTR    tc_SPReg;		    /* stack pointer	    */
    APTR    tc_SPLower;		    /* stack lower bound    */
    APTR    tc_SPUpper;		    /* stack upper bound + 2*/
    VOID    (*tc_Switch)();	    /* task losing CPU	  */
    VOID    (*tc_Launch)();	    /* task getting CPU	 */
    struct  List tc_MemEntry;	    /* allocated memory */
    APTR    tc_UserData;	    /* per task data */
};

/*----- Flag Bits ------------------------------------------*/
#define TB_PROCTIME	0
#define TB_STACKCHK	4
#define TB_EXCEPT	5
#define TB_SWITCH	6
#define TB_LAUNCH	7

#define TF_PROCTIME	(1<<0)
#define TF_STACKCHK	(1<<4)
#define TF_EXCEPT	(1<<5)
#define TF_SWITCH	(1<<6)
#define TF_LAUNCH	(1<<7)

/*----- Task States ----------------------------------------*/
#define TS_INVALID	0
#define TS_ADDED	1
#define TS_RUN		2
#define TS_READY	3
#define TS_WAIT		4
#define TS_EXCEPT	5
#define TS_REMOVED	6

/*----- Predefined Signals -------------------------------------*/

#define SIGB_ABORT	0
#define SIGB_CHILD	1
#define SIGB_BLIT	4
#define SIGB_SINGLE	4
#define SIGB_DOS	8

#define SIGF_ABORT	(1<<0)
#define SIGF_CHILD	(1<<1)
#define SIGF_BLIT	(1<<4)
#define SIGF_SINGLE	(1<<4)
#define SIGF_DOS	(1<<8)

#endif	/* EXEC_TASKS_H */
