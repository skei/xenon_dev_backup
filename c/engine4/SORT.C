// bytesort.c

#include <memory.h>

#include "defines.h"
#include "types.h"
#include "vars.h"

// ------------------------------------------------------
// Setup for bytesort. So that we only sort visible polys
// ------------------------------------------------------

int polysortsetup(struct objstruc *o, int *table)
{
 int nvis;
 int i;
 int c1,c2,c3;
 int x1,y1,x2,y2,x3,y3;

 nvis = 0;

 for ( i=0;i<o->numpolys;i++ )
 {
  c1 = o->polys[i*3];     // Coordinate set #1
  c2 = o->polys[i*3+1];   // Coordinate set #2
  c3 = o->polys[i*3+2];   // Coordinate set #3

// If any of the Z-coordinates are zero or less, skip the entire poly
	if (   ( zcoords[c1] != -1 )
	    && ( zcoords[c2] != -1 )
	    && ( zcoords[c3] != -1 ))
  {
   x1 = tricoords[c1*2];
   y1 = tricoords[c1*2+1];
   x2 = tricoords[c2*2];
   y2 = tricoords[c2*2+1];
   x3 = tricoords[c3*2];
   y3 = tricoords[c3*2+1];
   if ( (x2-x1)*(y3-y1)-(x3-x1)*(y2-y1) < 0 )
   {
		table[nvis] = ((unsigned short int)i<<16) + (short int)( zcoords[c1]+zcoords[c2]+zcoords[c3] );
//		sorttable[nvis].sortvalue  = (short int)( zcoords[c1]+zcoords[c2]+zcoords[c3] );
//	  sorttable[nvis].data = (short int)i;
    nvis++;
   }
	}
 }
 return nvis;
}

// --------
// BYTESORT
// --------
// sorttable[n].zvalue = value to sort
// sorttable[n].polynumber = appended data (might be a pointer or anything)...
//                           in this case, the polygon number
// numvis = number of elements in sorttable
// this version handles 16-bit values only... Easily extended to 32-bit (int)

void bytesort(int numvis, int *table)
{
 int i;
 short int eax,ebx,edx;

// clear numbers array
 memset( numbers,0,sizeof (numbers));
// Count number of each byte-value
 for ( i=0;i<numvis;i++ )
 {
//  numbers[ sorttable[i].sortvalue & 255 ]++;
	numbers[ table[i] & 255 ]++;
 }
// Convert from numbers to offset in Sorted
 eax = 0;
 for ( i=0;i<256;i++ )
 {
  ebx = numbers[i];
  numbers[i] = eax;
  eax+=ebx;
 }
// Sort according to LOW byte
 for ( i=0;i<numvis;i++ )
 {
  eax = table[i] & 255; //sorttable[i].sortvalue & 255;
  edx = numbers[eax];
  sorted[edx] = table[i];
//  sorted[edx].data = sorttable[i].data;
//  sorted[edx].sortvalue = sorttable[i].sortvalue;
  numbers[eax]++;
 }
// Now repeat for High byte
 // clear numbers array
 memset( numbers,0,sizeof (numbers));
// Count number of each byte-value
 for ( i=0;i<numvis;i++ )
 {
//	 numbers[ (sorted[i].sortvalue & 0xff00) >> 8 ]++;
	numbers[ (unsigned int)(sorted[i] & 0xff00) >> 8 ]++;
 }
// Convert from numbers to offset in Sorted
 eax = 0;
 for ( i=0;i<256;i++ )
 {
  ebx = numbers[i];
  numbers[i] = eax;
  eax+=ebx;
 }
// Sort according to HIGH byte
 for ( i=0;i<numvis;i++ )
 {
	eax = (unsigned int)(sorted[i] & 0xff00) >> 8; // (sorted[i].sortvalue & 0xff00) >> 8;
  edx = numbers[eax];
  table[edx] = sorted[i];
//  sorttable[edx].data = sorted[i].data;
//  sorttable[edx].sortvalue = sorted[i].sortvalue;

  numbers[eax]++;
 }
}
