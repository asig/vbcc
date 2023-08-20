
#include <exec/types.h>
#include "setjmp.h"

#define	SIGDIV0	5
#define SIGTRAPV        7
#define SIGFPCP_DIVIDEBY0       50
#define SIGFPCP_OVERFLOW        53

jmp_buf div0_buf;

long	MathIeeeDoubBasBase = 0;

SigTrap0()
{
	printf("divide by zero caught\n");
	longjmp(div0_buf, 1);
}

DOUBLE	zero = 0.0;
DOUBLE	one = 1.0;
DOUBLE	result;

main(argc,argv)
{
	printf(" Test Divide by zero handling\n");

	MathIeeeDoubBasBase = OpenLibrary("mathieeedoubbas.library",0);

	if (!MathIeeeDoubBasBase)
	{
		printf(" could not open Mathieeedoubbas.library\n");
		exit(-1);
	}

	/* first set up Intercept routine */

	InterceptTrap(SIGDIV0, SigTrap0);	/* for 68000/68010 */
	InterceptTrap(SIGFPCP_DIVIDEBY0, SigTrap0);	/* 68020 */

	if (setjmp(div0_buf)) goto caught_div0;

	result = one/zero;

	printf("H'mmm, no error? , oh well\n");
		goto exit;

caught_div0:
	printf(" back in main program, and NO guru \n");

exit:
	CloseLibrary(MathIeeeDoubBasBase);
	exit(0);
}
