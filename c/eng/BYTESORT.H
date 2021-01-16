#ifndef bytesort_h_included
#define bytesort_h_included
// 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴

#include "types.h"

// table = table to sort
// t = array of coordinates
// z = array of Z values
int polysortsetup(sObject *o, int *table, int *t, int *z);
// sorttable[n].zvalue = value to sort
// sorttable[n].polynumber = appended data (might be a pointer or anything)...
//                           in this case, the polygon number
// numvis = number of elements in sorttable
// this version handles 16-bit values only... Easily extended to 32-bit (int)
void bytesort(int numvis, int *table, int *sorted);

// 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
#endif
