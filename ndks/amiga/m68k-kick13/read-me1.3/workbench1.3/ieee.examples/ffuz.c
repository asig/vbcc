/***********************************************************************
*
*		    More Fuzzy Numbers on the Amiga
*
*	   This Fuz shows a way to use direct calls to the IEEEDP math
*	functions. This way people without the Manx mi.lib & mi32.lib
*	files can compile this mess (just wait 'till you see the assembler
*	version...) and get some fuzzy idea of how to use the MC68881 on
*	Microbotics' Multi-Function Module.
*	   Fuz works off of three input variable - rot or rotation factor,
*	vx and vy which are the X & Y vectors. The values that give the
*	best results are floating point numbers of course. Use a value of
*	.1 or less for rot, vy generally likes bigger #'s like 599.9893,
*	and vx can be anything including 0 up to the 9 character limit on
*	the input buffer. - when it's started it'll plot the default values
*	I put in, and takes 20 seconds (roughly) to draw. I chose these
*	values because they're a good example of the type of numerical
*	input you should try. It's a toy. Play with it.
*
*		To compile FFuz.c with Manx, you need to link in two extra
*	libraries, midb.lib and midt.lib that I hacked together for use with
*	 Manx. They stand for mathieeedoubbas.lib and mathieeedoubtrans.lib 
*	respectively, which I've gotten damned tired of typing, so if you see
*	 midb or midt somewhere in my code, that's what they mean. I'm glad
*	there's a 30 character filename size limit, 'cause gawd knows what
*	weird library or function names that Dale Luck will come up with,
*	you know? I mean, have you ever looked at the function names in the
*	Layers library? I rest my case.
*		Anyway, here's the Manx makefile for FFuz.c

CFLAGS	=	+fi +L -S
OBJ	=	FFuz.o

ffuz:		$(OBJ)
		ln -o FFuz $(OBJ) -lmidb -lmidt -lmi32 -lc32

*			author: David Milligan, for MicroBotics 
*
* FFuz source code is presented courtesy of MicroBotics, Inc., Richardson, TX.
* FFuz is copyright 1987, MicroBotics, Inc. and David Milligan.
* All rights reserved.
****************************************************************************/

#include <exec/types.h>
#include <intuition/intuition.h>
#include "devices/timer.h"
#include <math.h>
#include <stdio.h>

#define INTUITION	0x00000001
#define GRAPHICS	0x00000002
#define SCREEN		0x00000004
#define WINDOW		0x00000008
#define MIDB		0x00000010
#define MIDT		0x00000020
#define	NW_IDCMP	MOUSEBUTTONS|GADGETDOWN|GADGETUP|CLOSEWINDOW
#define	NW_FLAGS	BORDERLESS|BACKDROP|ACTIVATE|WINDOWCLOSE

#ifndef TRUE
#define TRUE 1
#define FALSE 0
#endif

#define CX	320
#define CY	200
#define XTWEAK	170
#define YTWEAK	140

/*
 * Gadget definitions
 */
#define ROT	1
#define VX	2
#define VY	3
#define WHOA	4
#define GO	5
#define NUMGADS 5
#define BOOLAFLAGS	RELVERIFY|GADGIMMEDIATE
#define STRAFLAGS	RELVERIFY|GADGIMMEDIATE|STRINGCENTER
#define GADL_EDGE	5
#define GADT_EDGE	30

struct  GfxBase       *GfxBase;
struct  IntuitionBase *IntuitionBase;
struct  RastPort      *rp;
struct  ViewPort      *vp;
struct	Window	*w;
struct	Screen	*screen;
struct	IntuiMessage	*message;

UBYTE rotbuf[10] =	".02";	/* This set of values draws a nice display */
UBYTE vxbuf[10] =	"199";
UBYTE vybuf[10] =	"192";
char timebuf[25];

/*#define USE_COMPILER*/
#ifdef USE_COMPILER
#define	Add(x,y)	((x)+(y))
#define	Mul(x,y)	((x)*(y))
#define	Div(x,y)	((x)/(y))
#define	Sub(x,y)	((x)-(y))
#define Flt(x)	(x)
#define Fix(x) (x)
#define Sin sin
#define Cos cos
#define Abs fabs
double	sin(),cos(),fabs();
#else
#define Add  IEEEDPAdd		/* We re-define the Double-Precision math */
#define Mul IEEEDPMul		/* functions to a tolerable length.	  */
#define Div IEEEDPDiv
double Add();		/* We want to make sure that we get a double value */
double Mul();		/* returned from all the IEEEDP functions.....*/
double Div();
#define Flt IEEEDPFlt
#define Fix IEEEDPFix
double Flt();
		/* Everything EXCEPT IEEDPFix() - this function is */
		/* what turns the IEEEDP values into usable integer */
		/* values. This hung me up for an embarassing amount */
		/* of time whilst beating on the IEEEDP stuff. It must */
		/* be declared as   int IEEEDPFix()   */
int Fix();
#define Sin IEEEDPSin
#define Cos IEEEDPCos
#define Abs IEEEDPAbs
double Sin();
double Cos();
double Abs();
#endif

#ifndef GREENHILLS
long MathIeeeDoubBasBase;
long MathIeeeDoubTransBase;
#endif

double atof();

/*
 *	color values
 *		pen #	  0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15
 */
USHORT red[] =		{ 0, 13,11,12,13,13,14,15, 8, 4, 0, 0, 0, 0, 0, 4 };
USHORT green[] =	{ 0,  2, 3, 4, 5, 7, 9,13,11, 9, 6, 4, 2, 0, 5,10 };
USHORT blue[] =		{ 0,  2, 2, 2, 2, 1, 1, 0, 0, 0, 2, 7,10,14,15,15 };

struct TextAttr Topfont =
	{
	"topaz.font", TOPAZ_SIXTY,
	FSF_EXTENDED | FSF_ITALIC | FSF_BOLD,
	FPF_ROMFONT | FPF_PROPORTIONAL
	};
struct TextAttr Toyfont =
	{
	"topaz.font", TOPAZ_EIGHTY,
	FSF_EXTENDED,
	FPF_ROMFONT | FPF_PROPORTIONAL
	};
/*
 *	border line vectors
 */
USHORT gadvecs2[] =	{ 0,0,58,0,58,10,0,10,0,0 };
USHORT gadvecs3[] =	{ 0,0,84,0,84,12,0,12,0,0 };
USHORT timervec[] =	{0,0,176,0,176,11,0,11,0,0};

struct IntuiText timer_text = { 7,0,JAM2,20,380, &Toyfont, &timebuf, NULL };
struct Border timer_brdr = { 5,377, 1,0,JAM2, 5, timervec, NULL };

struct Border gad_brdr[NUMGADS] = 
{
	{ -2,-3, 15,0,JAM1, 5, gadvecs3, NULL },
	{ -2,-3, 13,0,JAM1, 5, gadvecs3, NULL },
	{ -2,-3, 11,0,JAM1, 5, gadvecs3, NULL },
	{ -2,-1,  5,0,JAM1, 5, gadvecs2, NULL },
	{ -2,-1, 10,0,JAM1, 5, gadvecs2, NULL }
};

struct IntuiText gad_txt[NUMGADS] =
{
	{ 7,0,JAM2,	28,-12, &Toyfont, "rot", NULL },
	{ 7,0,JAM2,	34,-12, &Toyfont, "vx", NULL },
	{ 7,0,JAM2,	34,-12, &Toyfont, "vy", NULL },
	{ 7,4,JAM2,	1,   0, &Topfont, "Whoa!", NULL },
	{ 7,9,JAM2,	1,   0, &Topfont, "Start", NULL }
};

struct StringInfo gad_info[NUMGADS-2] =
{ 
	{ rotbuf,  NULL, 0, 10, 0, 0,0,0,2,1, 0, NULL, NULL },
	{ vxbuf,   NULL, 0, 10, 0, 0,0,0,2,1, 0, NULL, NULL },
	{ vybuf,   NULL, 0, 10, 0, 0,0,0,2,1, 0, NULL, NULL }
};

struct Gadget toy_gad[NUMGADS] = 
{
	{		/*	rotation string gadget	*/
	NULL,
	GADL_EDGE,GADT_EDGE,
	80,10,
	GADGHCOMP,STRAFLAGS,STRGADGET,
	(APTR)&gad_brdr[0],NULL, &gad_txt[0],
	0, (APTR)&gad_info[0],
	ROT, NULL
	},
	{		/*	x-vector input gadget	*/
	&toy_gad[0],
	GADL_EDGE,GADT_EDGE+25,
	80,10,
	GADGHCOMP,STRAFLAGS,STRGADGET,
	(APTR)&gad_brdr[1],NULL, &gad_txt[1],
	0, (APTR)&gad_info[1],
	VX, NULL
	},
	{		/*	y-vector input gadget	*/
	&toy_gad[1],
	GADL_EDGE,GADT_EDGE+50,
	80,10,
	GADGHCOMP,STRAFLAGS,STRGADGET,
	(APTR)&gad_brdr[2],NULL, &gad_txt[2],
	0, (APTR)&gad_info[2],
	VY, NULL
	},
	{		/*	stop button	*/
	&toy_gad[2],
	GADL_EDGE+11,GADT_EDGE+65,
	58,9,
	GADGHCOMP, BOOLAFLAGS, BOOLGADGET,
	(APTR)&gad_brdr[3],NULL, &gad_txt[3],
	0, NULL,
	WHOA, NULL
	},
	{		/*	start button 	*/
	&toy_gad[3],
	GADL_EDGE+11,GADT_EDGE+80,
	58,9, 
	GADGHCOMP, BOOLAFLAGS, BOOLGADGET, 
	(APTR)&gad_brdr[4],NULL, &gad_txt[4],
	0, NULL, 
	GO, NULL
	}
};

struct NewScreen ns =
	{
	0, 0, 640, 400,
	4, 0, 0,
	HIRES|LACE, CUSTOMSCREEN,
	&Toyfont,NULL, NULL
	};

struct NewWindow nw =
	{
	0, 12, 640, 388, 7, 0,
	NW_IDCMP,
	NW_FLAGS,
	&toy_gad[4], NULL,
	"               MicroBotics' Fuzzy Number Plotter, by Dmil",
	NULL, NULL, 0, 0, 0, 0, CUSTOMSCREEN
	};

struct timeval t1;
struct timeval t2;

extern struct MsgPort *CreatePort();
extern struct IORequest *CreateExtIO();


unsigned int mask = 0;
BOOL idling;
	/* A variable for everything I could think of, and a couple more */
	/* just in case it wasn't confusing enough.	*/
int from_wb,col,cnt,nx,ny,n;
double rot,vx,vy,inc,deginc,degvx,degvy,absvx,absvy,xtweak,ytweak;
double PI,DEG,sintemp,sintempvx,costemp,costempvy,vnx,vny,n1,n2;

/************************************************
 * 		Main Program 			*
 ************************************************/
main(argc)
int argc;
{
	ULONG  class;
	USHORT code;
	register struct Gadget *gad;
	register int gadgid;
	
	col = 1;
	inc = nx = ny = cnt = 0;

	if(argc)		/* started from CLI or Workbench? */
	  from_wb = 0;
	else
	  from_wb = -1;

	std_open();	/* open all the librarys, screens, etc. */

	xtweak = Flt(XTWEAK);
	ytweak = Flt(YTWEAK);

			/* This could've be #defined , but I did that */
			/* in the last program. Tempting fate... */
	PI = 3.14159265358979323846;
	n = 80;
	n1 = Flt(n);
	DEG = Div(PI,n1);

	rot = atof(rotbuf); /* use default values for initial plot */
	vx = atof(vxbuf);
	vy = atof(vybuf);
	SetBPen(rp, 0);
	SetAPen(rp,col);
	Move(rp,CX,CY+YTWEAK);
	idling = FALSE;

	GetSysTime(&t1);

/************************************************
 * 		Main Fuzzy Area			*
 ************************************************/

FOREVER 
  {
	if((message = (struct IntuiMessage *)GetMsg(w->UserPort)) != NULL)
	{
	class = message->Class;
	code  = message->Code;
	gad = message->IAddress;
	ReplyMsg(message);
	   switch (class)
	    {
             case CLOSEWINDOW:
		stage_left();		/* Anyone remember Snagglepuss? */
		break;
	     case GADGETDOWN:
	     case GADGETUP:
		gadgid = gad->GadgetID;
		if(class == GADGETUP)
	  	 switch (gadgid)
		 {
			case WHOA:
			   ny = YTWEAK;
			   GetSysTime(&t2);
			   showetime();
			   break;
			case GO:
			   rot = atof(rotbuf);
			   vx = atof(vxbuf);
			   vy = atof(vybuf);
			   SetRast(rp,0);
			   RefreshWindowFrame(w);
			   col = 1;
			   SetAPen(rp,col);
			   Move(rp,CX,CY+YTWEAK);
			   inc =0;
			   cnt = 0;
			   ny = 0;
			   GetSysTime(&t1);
			   idling = FALSE;
			   break;

			default:
			  break;
			}
	   default:
		break;
	    }

	}
	do_fuzzer();
  }
 stage_left();	/* main exit */
}	/* end of main */


	/*
	 * Here we are, all set to bang out fuzzy numbers & draw them in
	 * loud, garish colors. In interlace too, so that all you out 
	 * there without long persistance monitors (I've got one) get the
	 * added eyestrain of flickering teensy dots.
	 *  I've broken the math functions into individual calls stored
	 * in lots of variables to avoid extremely long lines of code.
	 * You may not like this approach. Bummer. I do.
	 */
do_fuzzer()
{
       if(ny < YTWEAK)
	{
	inc = Add(inc,rot);

	deginc = Mul(inc,DEG);
	sintemp = Sin(deginc);
	costemp = Cos(deginc);
	degvy = Mul(deginc,vy);
	costempvy = Cos(degvy);
	absvy = Abs(costempvy);
	vny = Mul(costemp,absvy);
	if(vx == 0)
	  {
	   nx = Fix(Mul(sintemp,xtweak));
	  } else {	
	 degvx = Mul(deginc,vx);
	 sintempvx = Sin(degvx);
	 absvx = Abs(sintempvx);
	 vnx = Mul(sintemp,absvx);
	nx = Fix(Mul(vnx,xtweak));
	 }
	ny = Fix(Mul(vny,ytweak));
	cnt ++;
	if(cnt >141)
	 {
	  cnt = 0;
	  col++;
	  if(col == 15) col = 1;
	  SetAPen(rp,col);
	 }
	Draw(rp,nx+CX,ny+CY);
	     if(ny >= YTWEAK && !idling)
		{
		GetSysTime(&t2);
		 showetime();
		}
	}
}

std_open()
{
	register int i;

	if(!(IntuitionBase = (struct IntuitionBase *)
			OpenLibrary("intuition.library", 0)))
		errexit(" Can't open Intuition");

	mask |= INTUITION;

	if(!(GfxBase = (struct GfxBase *)OpenLibrary("graphics.library", 0)))
		errexit(" Can't open Gfx lib");

	mask |= GRAPHICS;

#ifndef GREENHILLS

	if(!(MathIeeeDoubBasBase = OpenLibrary("mathieeedoubbas.library",0)))
		errexit(" no basic ieee library\n");
#endif

	mask |= MIDB;

#ifndef GREENHILLS
	if(!(MathIeeeDoubTransBase = OpenLibrary("mathieeedoubtrans.library",0)))
		errexit(" no transcendental ieee library\n");
#endif

	mask |= MIDT;

	if(!(screen = (struct Screen *)OpenScreen(&ns))) 
		errexit(" Can't open the screen");

	mask |= SCREEN;

	nw.Screen = screen;

	ShowTitle(screen,FALSE);	/* turn off the screen title bar */
	vp = &screen->ViewPort;
		
/*
 * install some loud garish color
 */
	for(i = 0; i < 16; i++)
		SetRGB4(vp,i,red[i],green[i],blue[i]);

	if(!(w = (struct Window *)OpenWindow(&nw)))
		errexit(" Can't open the window");

	mask |= WINDOW;

	rp = w->RPort;
}

errexit(str)
register UBYTE *str;
{
	if(!from_wb)		/* started from CLI, so it's safe to */
	  printf("%s\n",str);	/* printf the error message	*/

	stage_left();
}

stage_left()
{
/*
 *	use the masked flag bits to close what we opened
 */

	if (mask & WINDOW)
		CloseWindow(w);
	if (mask & SCREEN)
		CloseScreen(screen);
#ifndef GREENHILLS
	if (mask & MIDT)
		CloseLibrary(MathIeeeDoubTransBase);
	if (mask & MIDB)
		CloseLibrary(MathIeeeDoubBasBase);
#endif
	if (mask & GRAPHICS)
		CloseLibrary(GfxBase);
	if (mask & INTUITION)
		CloseLibrary(IntuitionBase);
	exit();
}

showetime()
{
	if(!idling)
	   sprintf(timebuf,"%ld.%ld seconds"
		,t2.tv_secs-t1.tv_secs,t1.tv_micro-t2.tv_micro);
	   DrawBorder(rp,&timer_brdr,0L,0L);
	   PrintIText(rp,&timer_text,0L,0L);
	   idling = TRUE;
}

struct timerequest *PrepareTimer(precision)
SHORT precision;
{

	int error;
	SHORT whichunit;

	struct MsgPort *timerport;
	struct timerequest *timermsg;
	
        timerport = CreatePort(0,0);
        if (timerport == NULL) 
		return(NULL);	/* Error during CreatePort */

	timermsg = (struct timerequest *)CreateExtIO(
					timerport,sizeof(struct timerequest));
	if (timermsg == NULL)
		{
		DeletePort(timerport);
		return(NULL);	/* Error during CreateExtIO */
		}
	
	if(precision)	/* if true, use precision timer  ( under 1 second ) */
		whichunit = UNIT_MICROHZ;
	else
		whichunit = UNIT_VBLANK;

	error = OpenDevice(TIMERNAME, whichunit, timermsg, 0);
	if (error != 0)
		{
		DeleteExtIO(timermsg,sizeof(struct timerequest));
		DeletePort(timerport);
		return(NULL);	/* Error during OpenDevice */
		}
	return(timermsg);
}

int
GetSysTime(tv)
struct timeval *tv;
{
	struct timerequest *tr;
	tr = PrepareTimer(TRUE);	/* MUST use Precise timer for this */
	if(tr == 0) return(-1);		/* non zero return says error */
 
	tr->tr_node.io_Command = TR_GETSYSTIME;
	DoIO( tr );

	tv->tv_secs = tr->tr_time.tv_secs;
	tv->tv_micro = tr->tr_time.tv_micro;

	DeleteTimer(tr);
	return(0);

}

int
DeleteTimer(tr)
struct timerequest *tr;
{
	struct MsgPort *tp;

	tp = tr->tr_node.io_Message.mn_ReplyPort;
	if(tr != 0)
	{
		CloseDevice(tr);
		DeleteExtIO(tr,sizeof(struct timerequest));
	}
	if(tp != 0)
		DeletePort(tp);
	return(0);		
}
