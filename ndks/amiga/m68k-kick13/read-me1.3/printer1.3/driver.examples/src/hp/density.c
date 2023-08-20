/*
	Density module for HP_LaserJet
	David Berezowski - May/87
*/

#include <exec/types.h>
#include "../printer/printer.h"
#include "../printer/prtbase.h"

SetDensity(density_code)
ULONG density_code;
{
	extern struct PrinterExtendedData *PED;
	extern char StartCmd[];

	/* SPECIAL_DENSITY     0   1   2    3    4    5    6    7 */
	static int XDPI[8] = {75, 75, 100, 150, 300, 300, 300, 300};
	static char codes[8][3] = {
		"075", "075", "100", "150", "300", "300", "300", "300"
	};

	density_code /= SPECIAL_DENSITY1;
	PED->ped_MaxXDots = XDPI[density_code] * 8; /* 8 inches */
	PED->ped_MaxYDots = XDPI[density_code] * 10; /* 10 inches */
	PED->ped_XDotsInch = PED->ped_YDotsInch = XDPI[density_code];
	StartCmd[8] = codes[density_code][0];
	StartCmd[9] = codes[density_code][1];
	StartCmd[10] = codes[density_code][2];
}
