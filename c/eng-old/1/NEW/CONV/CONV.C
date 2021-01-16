#include <stdio.h>

struct
{
 unsigned short chunk_id;
 unsigned int next_chunk;
} chunk;

void Load3DS( char *filename, sObject *O }
{
 FILE *fp;
 long filelen;

 fp = fopen(filename,"rb");
 filelen = filelength(fileno(fp));


}

