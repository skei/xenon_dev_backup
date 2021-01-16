// bytesort.h

#ifndef bytesort_h_included
#define bytesort_h_included

#include "types.h"

extern int sorted[MAXPOLYS];
extern int zcoords[MAXCOORDS];
extern int numbers[256];

int polysortsetup(objstruc *o, int *table, int *tric);
void bytesort(int numvis, int *table);

#endif
