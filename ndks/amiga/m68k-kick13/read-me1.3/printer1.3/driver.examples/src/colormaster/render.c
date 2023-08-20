/*
	CALCOMP ColorMaster driver.
	David Berezowski - July/87
*/

#include <exec/types.h>
#include <exec/nodes.h>
#include <exec/lists.h>
#include <exec/memory.h>
#include "../printer/prtbase.h"
#include "../printer/printer.h"

#define NUMENDCMD	0	/* # of cmd bytes after binary data */
#define STARTLEN	4	/* # of start cmd bytes */
#define ORGLEN		7	/* # of origin cmd bytes */

#define DEBUG0	0
#define DEBUG1	0
#define DEBUG2	0
#define DEBUG3	0
#define DEBUG4	0
#define DEBUG5	0
#define DEBUG6	0

Render(ct, x, y, status)
long ct, x, y, status;
{
	extern void *AllocMem(), FreeMem();

	extern struct PrinterData *PD;
	extern struct PrinterExtendedData *PED;

	static UWORD RowSize, BufSize, TotalBufSize, dataoffset, YOrg;
	static UWORD NumStartCmd, NumTotalCmd, NumDiscardCmd, Aspect;
	/*
		00-00	\002	enter raster gfx mode
		01-01	\000	required padding after cmd byte
		02-02	\000	enter landscape/portrait raster gfx mode
		03-03	\000	required padding after cmd byte
	*/
	static UBYTE StartCmd[STARTLEN] = "\002\000\000\000";
	/*
		00-01	\033\117	set origin
		02-03	\000\000	y origin
		04-05	\000\000	x origin
		06-06	\000		required padding after cmd byte
	*/
	static UBYTE OrgCmd[ORGLEN] = "\033\117\000\000\000\000\000";
	UBYTE *ptr, *ptrstart;
	int i, err;

	switch(status) {
		case 0 : /* Master Initialization */
			/*
				ct	- pointer to IODRPReq structure.
				x	- width of printed picture in pixels.
				y	- height of printed picture in pixels.
			*/
#if DEBUG0
			kprintf("0: ct=%lx, x=%ld, y=%ld\n", ct, x, y);
#endif DEBUG0
			StartCmd[2] = (Aspect == ASPECT_HORIZ) ? 0x50 : 0x4c;
			if (Aspect != ASPECT_HORIZ) {
				NumDiscardCmd = 0;
			}
			NumStartCmd = 4 + NumDiscardCmd;
			NumTotalCmd = NumStartCmd + NUMENDCMD;
			RowSize = (x + 7) / 8;
			BufSize = RowSize + NumTotalCmd;
			TotalBufSize = BufSize * 2;
			PD->pd_PrintBuf = AllocMem(TotalBufSize, MEMF_PUBLIC);
			if (PD->pd_PrintBuf == NULL) {
				err = PDERR_BUFFERMEMORY; /* no mem */
			}
			else {
				ptr = PD->pd_PrintBuf;
				*ptr++ = 27;
				*ptr = 'K';	/* raster data follows */
				ptr = &PD->pd_PrintBuf[BufSize];
				*ptr++ = 27;
				*ptr = 'K';	/* raster data follows */
				dataoffset = NumStartCmd;
				if (YOrg == 0) { /* if first dump */
					PrinterReady();
					/* enter raster graphics mode */
					err = (*(PD->pd_PWrite))(StartCmd,
						STARTLEN);
					PrinterReady();
				}
				else { /* not first, set origin for succs. */
					/* set origin */
					OrgCmd[2] = YOrg / 256;
					OrgCmd[3] = YOrg & 255;
					PrinterReady();
					err = (*(PD->pd_PWrite))(OrgCmd,
						ORGLEN);
					PrinterReady();
				}
				YOrg += y;
			}
			break;

		case 1 : /* Scale, Dither and Render */
			/*
				ct	- pointer to PrtInfo structure.
				x	- color code.
				y	- row # (0 to Height - 1).
			*/
#if DEBUG1
			kprintf("1: ct=%lx, x=%ld, y=%ld\n", ct, x, y);
#endif DEBUG1
			Transfer(ct, y, &PD->pd_PrintBuf[dataoffset], x);
			err = PDERR_NOERR; /* all ok */
			break;

		case 2 : /* Dump Buffer to Printer */
			/*
				ct	- 0.
				x	- 0.
				y	- # of rows sent (1 to NumRows).
			*/
#if DEBUG2
			kprintf("2: ct=%lx, x=%ld, y=%ld\n", ct, x, y);
#endif DEBUG2
			i = RowSize;
			ptrstart = &PD->pd_PrintBuf[dataoffset - NumStartCmd];
			ptr = ptrstart + NumStartCmd + i - 1;
			while (i > 0 && *ptr == 0) {
				i--;
				ptr--;
			}
			if (i == 0) {
				PrinterReady();
				/* linefeed */
				err = (*(PD->pd_PWrite))("\012\000", 2);
				PrinterReady();
			}
			else {
				ptr = ptrstart + 2;
				/* set printout width */
				*ptr++ = (i + NumDiscardCmd) >> 8;
				*ptr++ = (i + NumDiscardCmd) & 0xff;
				PrinterReady();
				err = (*(PD->pd_PWrite))
					(ptrstart, i + NumTotalCmd);
				PrinterReady();
			}
			if (err == PDERR_NOERR) {
				dataoffset = (dataoffset == NumStartCmd ?
					BufSize : 0) + NumStartCmd;
			}
			break;

		case 3 : /* Clear and Init Buffer */
			/*
				ct	- 0.
				x	- 0.
				y	- 0.
			*/
#if DEBUG3
			kprintf("3: ct=%lx, x=%ld, y=%ld\n", ct, x, y);
#endif DEBUG3
			ptr = &PD->pd_PrintBuf[dataoffset];
			i = RowSize;
			do {
				*ptr++ = 0;
			} while (--i);
			err = PDERR_NOERR; /* all ok */
			break;

		case 4 : /* Close Down */
			/*
				ct	- error code.
				x	- io_Special flag from IODRPReq.
				y	- 0.
			*/
#if DEBUG4
			kprintf("4: ct=%lx, x=%ld, y=%ld\n", ct, x, y);
#endif DEBUG4
			err = PDERR_NOERR; /* assume all ok */
			/* if user did not cancel the print */
			if (ct != PDERR_CANCEL) {
				/* if do not want to unload paper */
				if (x & SPECIAL_NOFORMFEED) {
					if (PD->pd_Preferences.PrintShade ==
						SHADE_COLOR) { /* color */
						PrinterReady();
						/* Advance Color Panel */
						err = (*(PD->pd_PWrite))
							("\014\000", 2);
						PrinterReady();
					}
				}
				else { /* eject paper */
					YOrg = 0;
					PrinterReady();
					/* End of Transmission */
					err = (*(PD->pd_PWrite))("\004\000",
						2);
					PrinterReady();
				}
			}
			(*(PD->pd_PBothReady))();
			if (PD->pd_PrintBuf != NULL) {
				FreeMem(PD->pd_PrintBuf, TotalBufSize);
			}
			break;

		case 5 :  /* Pre-Master Initialization */
			/*
				ct	- 0 or pointer to IODRPReq structure.
				x	- io_Special flag from IODRPReq.
				y	- 0.
			*/
#if DEBUG5
			kprintf("5: ct=%lx, x=%ld, y=%ld\n", ct, x, y);
#endif DEBUG5
			if (ct != 0) { /* if not case 5 open */
				if ((Aspect = PD->pd_Preferences.PrintAspect)
					== ASPECT_VERT) { /* if sideways */
					PD->pd_Preferences.PrintAspect =
						ASPECT_HORIZ; /* force horz */
				}
			}
			else {
				YOrg = 0;
			}
			if (PD->pd_Preferences.PaperSize == W_TRACTOR) {
				/* CalComp_ColorView-5912 */
				/* 11 x 17 inch paper (B/A3 size) */
				PED->ped_MaxXDots = 2048;
				PED->ped_MaxYDots = 3200;
				NumDiscardCmd = 0;
			}
			else {
				/* 8.5 x 11 inch paper (A/A4 size) */
				PED->ped_MaxXDots = 1600;
				PED->ped_MaxYDots = 2000;
				NumDiscardCmd = 8;
			}
			err = PDERR_NOERR; /* all ok */
			break;

		case 6 : /* Switch to Next Color */
			/*
				ct	- 0.
				x	- 0.
				y	- 0.
			*/
#if DEBUG6
			kprintf("6: ct=%lx, x=%ld, y=%ld\n", ct, x, y);
#endif DEBUG6
			PrinterReady();
			/* Advance Color Panel */
			err = (*(PD->pd_PWrite))("\014\000", 2);
			PrinterReady();
			break;
	}
	return(err);
}
