#include <stdio.h>

// --------------------------------------------------------------------------

#ifndef TRUE
 #define TRUE 1
#endif

#ifndef FALSE
 #define FALSE 0
#endif

#define NEW(c) (malloc((c)))
#define OUTB(c) (*outpix++ = (unsigned char)(c))

#define MAX_CODES 4096

// --------------------------------------------------------------------------

FILE *FileOpen(const char *fname)
{
 return fopen(fname, "rb");
}

// --------------------------------------------------------------------------

void FileClose(FILE *f)
{
 fclose(f);
}

// --------------------------------------------------------------------------

long FileSize(const char *fname)
{
 FILE *f;
 long l;

 f = fopen(fname, "rb");
 if (f == NULL) return 0;
 fseek(f, 0, SEEK_END);
 l = ftell(f);
 fclose(f);
 return l;
}

// --------------------------------------------------------------------------

/*
 Load_Gif( char* file, char far *outpix, char far* palette,
         int* width, int* height );

 Routine to load a 256 color .GIF file into a memory buffer.  *Only* 256
 color images are supported here!  Sorry, no routines to SAVE .GIFs...
 Memory required is allocated on the fly and no checks are in place.  If you
 don't have enough memory it will likely crash.  It's easy to add the checks
 yourself, just put one after each call to malloc().  If you supply a pointer
 to a palette, it will be filled in.  If you supply a pointer to a width
 and/or height variable, it will be filled in as well.

 This code is hereby placed in the public domain.  Have fun!
*/

// --------------------------------------------------------------------------

int getw(FILE *f)
{
 int k = (unsigned)fgetc(f);
 k += 256*fgetc(f);
 return k;
}

// --------------------------------------------------------------------------

FILE *fp;
int curr_size;
int clear;
int ending;
int newcodes;
int top_slot;
int slot;
int navail_bytes = 0;
int nbits_left = 0;
unsigned char b1;
unsigned char byte_buff[257];
unsigned char *pbytes;
unsigned char *stack;
unsigned char *suffix;
unsigned short *prefix;

// --------------------------------------------------------------------------

unsigned long code_mask[13] =
{
 0L,
 0x0001L, 0x0003L,
 0x0007L, 0x000FL,
 0x001FL, 0x003FL,
 0x007FL, 0x00FFL,
 0x01FFL, 0x03FFL,
 0x07FFL, 0x0FFFL
};

// --------------------------------------------------------------------------

int get_next_code(void)
{
 register int  i;
 unsigned long ret;

 if( ! nbits_left )
 {
  if( navail_bytes <= 0 )
  {
   pbytes = byte_buff;
   navail_bytes = getc( fp );
   if( navail_bytes )
    for( i = 0; i < navail_bytes; ++i )
     byte_buff[i] = (unsigned char)getc( fp );
  }
  b1 = *pbytes++;
  nbits_left = 8;
  --navail_bytes;
 }
 ret = b1 >> (8 - nbits_left);
 while( curr_size > nbits_left )
 {
  if( navail_bytes <= 0 )
  {
   pbytes = byte_buff;
   navail_bytes = getc( fp );
   if( navail_bytes )
    for( i = 0; i < navail_bytes; ++i )
     byte_buff[i] = (unsigned char)getc( fp );
  }
  b1 = *pbytes++;
  ret |= b1 << nbits_left;
  nbits_left += 8;
  --navail_bytes;
 }
 nbits_left -= curr_size;
 return( (int) (ret & code_mask[curr_size]) );
}

// --------------------------------------------------------------------------

int Load_GIF(const char* file, unsigned char *outpix, unsigned char * pal, int* width, int* height)
{
 unsigned char* sp;
 int            code, fc, oc;
 int            i;
 unsigned char  size;
 int            c;
 unsigned char  buf[1028];

 fp = FileOpen(file);
 if( !fp ) return(0);
 fread( buf, 1, 6, fp );
 if( strncmp( (char *)buf, "GIF", 3 ) )
 {
  FileClose( fp );
  return( 0 );
 }
 fread( buf, 1, 7, fp );
 if (pal != NULL) memset(pal, 0, 768);
 for( i = 0; i < 3*(2 << (buf[4] & 7)); i++)
 {
  c = (unsigned char)getc( fp );
  if (pal != NULL) pal[i] = c >> 2;
 }
 fread( buf, 1, 5, fp );
 i = getw( fp );
 if( width ) *width = i;
 i = getw( fp );
 if( height ) *height = i;
 if(outpix == NULL)
 {
  FileClose(fp);
  return( 1 );
 }
 fread( buf, 1, 1, fp );
 size = (unsigned char)getc( fp );
 if( size < 2 || 9 < size )
 {
  FileClose( fp );
  return( 0 );
 }
 stack  = (unsigned char*)NEW( MAX_CODES + 1 );
 suffix = (unsigned char*)NEW( MAX_CODES + 1 );
 prefix = (unsigned short*)NEW( sizeof(*prefix) * (MAX_CODES + 1) );
 curr_size = size + 1;
 top_slot = 1 << curr_size;
 clear = 1 << size;
 ending = clear + 1;
 slot = newcodes = ending + 1;
 navail_bytes = nbits_left = 0;
 oc = fc = 0;
 sp = stack;
 while( (c = get_next_code()) != ending )
 {
  if( c == clear )
  {
   curr_size = size + 1;
   slot = newcodes;
   top_slot = 1 << curr_size;
   while( (c = get_next_code()) == clear );
   if( c == ending ) break;
   if( c >= slot ) c = 0;
   oc = fc = c;
   OUTB(c);
  }
  else
  {
   code = c;
   if( code >= slot )
   {
    code = oc;
    *sp++ = (unsigned char)fc;
   }
   while( code >= newcodes )
   {
    *sp++ = suffix[code];
    code = prefix[code];
   }
   *sp++ = (unsigned char)code;
   if( slot < top_slot )
   {
    fc = code;
    suffix[slot]   = (unsigned char)fc;
    prefix[slot++] = (unsigned short)oc;
    oc = c;
   }
   if( slot >= top_slot && curr_size < 12 )
   {
    top_slot <<= 1;
    ++curr_size;
   }
   while( sp > stack )
   {
    --sp;
    OUTB(*sp);
   }
  }
 }
 free( stack );
 free( suffix );
 free( prefix );
 FileClose( fp );
 return( 1 );
}

// 께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께

void SetMode(unsigned short int);
#pragma aux SetMode =\
 "int 10h",\
 parm [ax] modify [ax];

#pragma aux setrgb=\
 "mov dx,3c8h",\
 "out dx,al",\
 "inc dx",\
 "mov al,ah",\
 "out dx,al",\
 "mov al,bl",\
 "out dx,al",\
 "mov al,bh",\
 "out dx,al",\
 parm [al] [ah] [bl] [bh] modify [edx];


void main(void)
{
 char *buf;
 char *pal;
 int width,height;
 int i;

 Load_GIF("redhair.gif",NULL,NULL,&width,&height);
 buf = (char *)malloc(width*height);
 pal = (char *)malloc(768);
 Load_GIF("redhair.gif", buf, pal,&width,&height);

 SetMode(0x13);
 for(i=0;i<256;i++) setrgb(i,pal[i*3],pal[i*3+1],pal[i*3+2]);

 memcpy((char *)0xA0000,buf,64000);

 getch();

 SetMode(3);

 free(buf);
 free(pal);

}
