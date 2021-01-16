#include "defines.h"
#include "types.h"
#include "vars.h"
// #include "bytesort.h"

// ------------------
// Setup for bytesort
// ------------------

int sortsetup(struct objstruc *o)
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
	  sorttable[nvis].zvalue  = (short int)( zcoords[c1]+zcoords[c2]+zcoords[c3] );
	  sorttable[nvis].polynumber = (short int)i;
    nvis++;
   }
	}
 }
 return nvis;
}

// --------
// BYTESORT
// --------

void bytesort(int numvis)
{
 int i;
 short int eax,ebx,edx;

// clear numbers array
 memset( numbers,0,sizeof (numbers));
// Count number of each byte-value
 for ( i=0;i<numvis;i++ )
 {
  numbers[ sorttable[i].zvalue & 255 ]++;
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
  eax = sorttable[i].zvalue & 255;
  edx = numbers[eax];
  sorted[edx].polynumber = sorttable[i].polynumber;
  sorted[edx].zvalue = sorttable[i].zvalue;

  numbers[eax]++;
 }
// Now repeat for High byte
 // clear numbers array
 memset( numbers,0,sizeof (numbers));
// Count number of each byte-value
 for ( i=0;i<numvis;i++ )
 {
	 numbers[ (sorted[i].zvalue & 0xff00) >> 8 ]++;
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
  eax = (sorted[i].zvalue & 0xff00) >> 8;
  edx = numbers[eax];
  sorttable[edx].polynumber = sorted[i].polynumber;
  sorttable[edx].zvalue = sorted[i].zvalue;

  numbers[eax]++;
 }
}

