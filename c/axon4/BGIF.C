
// GIF,PCX,LBM,TGA8 : 8-bit files...


 #include <stdio.h>
 #include <stdlib.h>
 #include <string.h>
 #include <ctype.h>
 #include <sys/stat.h>

 // -------------------------------------------------------------------------

 typedef unsigned char  byte;
 typedef unsigned short word;
 typedef unsigned long  dword;

 typedef signed char  sbyte;
 typedef signed short sword;
 typedef signed long  sdword;

 typedef byte bool;

 #ifndef TRUE
  #define TRUE 1
 #endif
 #ifndef FALSE
  #define FALSE 0
 #endif

 #define NEW(c) (malloc((c)))

 // -------------------------------------------------------------------------

 static FILE *FileOpen(const char *fname)
  {
   return fopen(fname, "rb");
  }

 static void FileClose(FILE *f)
  {
   fclose(f);
  }

 static long FileSize(const char *fname)
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

 // -------------------------------------------------------------------------


 // VGLGif( char* file, char far *outpix, char far* palette,
 //         int* width, int* height );

 // Routine to load a 256 color .GIF file into a memory buffer.  *Only* 256
 // color images are supported here!  Sorry, no routines to SAVE .GIFs...
 // Memory required is allocated on the fly and no checks are in place.  If
 // you don't have enough memory it will likely crash.  It's easy to add the
 // checks yourself, just put one after each call to malloc().  If you supply
 // a pointer to a palette, it will be filled in.  If you supply a pointer to
 // a width and/or height variable, it will be filled in as well.

 static int getw(FILE *f)
  {
   int k = (unsigned)fgetc(f);
   k += 256*fgetc(f);
   return k;
  }

 #define MAX_CODES     4096

 static FILE*          fp;
 static int            curr_size;
 static int            clear;
 static int            ending;
 static int            newcodes;
 static int            top_slot;
 static int            slot;
 static int            navail_bytes = 0;
 static int            nbits_left = 0;
 static unsigned char  b1;
 static unsigned char  byte_buff[257];
 static unsigned char* pbytes;
 static unsigned char* stack;
 static unsigned char* suffix;
 static word         * prefix;

 static unsigned long code_mask[13] =
 {
  0L,
  0x0001L, 0x0003L,
  0x0007L, 0x000FL,
  0x001FL, 0x003FL,
  0x007FL, 0x00FFL,
  0x01FFL, 0x03FFL,
  0x07FFL, 0x0FFFL
 };

 static int get_next_code(void)
 {
  register int  i;
  static unsigned long ret;

  if( ! nbits_left )
  {
   if( navail_bytes <= 0 )
   {
    pbytes = byte_buff;
    navail_bytes = getc( fp );
    if( navail_bytes )
     for( i = 0; i < navail_bytes; ++i )
      byte_buff[i] = (byte)getc( fp );
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
      byte_buff[i] = (byte)getc( fp );
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

 #define OUTB(c) (*outpix++ = (byte)(c))

extern int PIX_LoadGIF(const char* file, unsigned char *outpix, unsigned char * pal, int* width, int* height)
 {
  unsigned char* sp;
  int            code, fc, oc;
  int            i;
  unsigned char  size;
  int            c;
  unsigned char  buf[1028];

  fp = FileOpen(file);
  if( !fp ) return( 0 );
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
   c = (byte)getc( fp );
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
  size = (byte)getc( fp );
  if( size < 2 || 9 < size )
  {
   FileClose( fp );
   return( 0 );
  }
  stack  = (byte*)NEW( MAX_CODES + 1 );
  suffix = (byte*)NEW( MAX_CODES + 1 );
  prefix = (word*)NEW( sizeof(*prefix) * (MAX_CODES + 1) );

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
     *sp++ = (byte)fc;
    }
    while( code >= newcodes )
    {
     *sp++ = suffix[code];
     code = prefix[code];
    }
    *sp++ = (byte)code;
    if( slot < top_slot )
    {
     fc = code;
     suffix[slot]   = (byte)fc;
     prefix[slot++] = (word)oc;
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

 // =======================================================
 // PCX

 typedef struct
 {
  byte id;
  byte version;
  byte rle;
  byte bpp;
  word xstart;
  word ystart;
  word xend;
  word yend;
  word hres;
  word vres;
  byte pal[48];
  byte rsvd1;
  byte nbitp;
  word bytesperline;
  word paltype;
  word hsize;
  word vsize;
  byte rsvd2[54];
 } PCXHeader;

extern int PIX_LoadPCX(const char* file, unsigned char *outpix, unsigned char * pal, int* width, int* height)
 {
  int            i, w, h;
  PCXHeader      hdr;
  long pos;
  int ret = 0;

  fp = FileOpen(file);
  if (fp == NULL) goto bye;
  pos = ftell(fp);
  fread (&hdr, 1, sizeof(hdr), fp);
  if (hdr.id != 0x0A || hdr.version != 5 || hdr.rle != 1
      || (hdr.bpp == 8 && hdr.nbitp != 1)) goto bye;
  w = hdr.xend - hdr.xstart + 1;
  if (width) *width = w;
  h = hdr.yend - hdr.ystart + 1;
  if (height) *height = h;
  if (pal != NULL)
  {
   if (hdr.bpp == 1 && hdr.nbitp == 4)
   {
    memset(pal, 0, 768);
    for (i = 0; i < 48; i++) pal[i] = hdr.pal[i] >> 2;
   }
   else
   if (hdr.bpp == 8 && hdr.nbitp == 1)
   {
    fseek(fp, pos + FileSize(file)-768, SEEK_SET);
    for (i = 0; i < 768; i++) pal[i] = ((unsigned char)getc(fp)) >> 2;
   }
   else goto bye;
  }
  ret = 1;
  if (outpix == NULL) goto bye;
  fseek(fp, pos + sizeof(PCXHeader), SEEK_SET);
  while (h-- > 0)
  {
   unsigned char c;
   unsigned char *outpt;
   int np = 0;

   outpt = outpix;
   memset(outpix, 0, w);
   for (np = 0; np < hdr.nbitp; np++)
   {
    i = 0;
    outpix = outpt;
    do
    {
     c = (byte)getc(fp);
     if ((c & 0xC0) != 0xC0)
     {
      if (hdr.bpp == 1)
      {
       int k;
       for (k = 7; k >= 0; k--) *outpix++ |= ((c >> k) & 1) << np;
       i += 8;
      }
      else
      {
       *outpix++ = c;
       i++;
      }
     }
     else
     {
      unsigned char v;
      v = (byte)getc(fp);
      c &= ~0xC0;
      while (c > 0 && i < w)
      {
       if (hdr.bpp == 1)
       {
        int k;
        for (k = 7; k >= 0; k--) *outpix++ |= ((v >> k) & 1) << np;
        i += 8;
       }
       else
       {
        *outpix++ = v;
        i++;
       }
       c--;
      }
     }
    }
    while (i < w);
   }
  }
 bye:
  if (fp != NULL) FileClose(fp);
  return ret;
 }

 // =======================================================
 // LBM

 typedef unsigned char  LBMUBYTE;
 typedef short          LBMWORD;
 typedef unsigned short LBMUWORD;
 typedef long           LBMLONG;
 typedef char           LBMID[4];

 typedef struct
 {
  LBMID      id;
  LBMLONG    size;
  LBMUBYTE   data[];
 } LBMCHUNK;

    // A BitMapHeader is stored in a BMHD chunk.
 typedef struct
 {
  LBMUWORD w, h;                  /* raster width & height in pixels */
  LBMUWORD  x, y;                 /* position for this image */
  LBMUBYTE nPlanes;               /* # source bitplanes */
  LBMUBYTE masking;               /* masking technique */
  LBMUBYTE compression;           /* compression algoithm */
  LBMUBYTE pad1;                  /* UNUSED.  For consistency, put 0 here.*/
  LBMUWORD transparentColor;      /* transparent "color number" */
  LBMUBYTE xAspect, yAspect;      /* aspect ratio, a rational number x/y */
  LBMUWORD pageWidth, pageHeight; /* source "page" size in pixels */
 } LBMBMHD;

 // RowBytes computes the number of bytes in a row, from the width in pixels.
 #define RowBytes(w)   (((w) + 15) >> 4 << 1)
 #define LBMIDEQ(i1,i2) (*(long*)(i1) == *(long*)(i2))

 static LBMLONG EndianSwapL(LBMLONG l)
 {
  LBMLONG t = ((LBMLONG)((LBMUBYTE *)&l)[0] << 24) +
              ((LBMLONG)((LBMUBYTE *)&l)[1] << 16) +
              ((LBMLONG)((LBMUBYTE *)&l)[2] <<  8) +
              ((LBMLONG)((LBMUBYTE *)&l)[3] <<  0);
  return t;
 }

 static LBMUWORD EndianSwapW(LBMUWORD l)
 {
  LBMUWORD t = ((LBMUWORD)((LBMUBYTE *)&l)[0] << 8) +
               ((LBMUWORD)((LBMUBYTE *)&l)[1] << 0);
  return t;
 }

 static bool ReadChunk(LBMCHUNK *chunk, FILE *f)
 {
  if (fread(chunk, sizeof(*chunk), 1, f) != 1) return FALSE;
  chunk->size = EndianSwapL(chunk->size);
  if (chunk->size & 1) chunk->size++;
  return TRUE;
 }

extern int PIX_LoadLBM(const char* file, unsigned char *outpix, unsigned char * pal, int* width, int* height)
 {
  FILE *fp;
  long pos;
  LBMCHUNK c;
  LBMID    fid;
  LBMLONG    total;
  bool compression;
  int     w = 0, h = 0;

  fp = FileOpen(file);
  if (fp == NULL) return 0;
  pos = ftell(fp);
  if (!ReadChunk(&c, fp) || !LBMIDEQ(c.id, "FORM")
   || c.size < (4+8+sizeof(LBMBMHD)+8+768)) goto error;
  if (fread(&fid, sizeof(fid), 1, fp) != 1 || !LBMIDEQ(fid, "PBM ")) goto error;
  total = c.size - sizeof(fid);
  while (total > 0)
  {
   if (!ReadChunk(&c, fp)) goto error;
   total -= sizeof(c) + c.size;
   if (LBMIDEQ(c.id, "BMHD"))
   {
    LBMBMHD hd;
    if (c.size != sizeof(LBMBMHD) || fread(&hd, sizeof(hd), 1, fp) != 1
        || hd.nPlanes != 8) goto error;
    w = EndianSwapW(hd.w);
    h = EndianSwapW(hd.h);
    if (width != NULL)  *width  = w;
    if (height != NULL) *height = h;
    compression = hd.compression;
    // if (pal == NULL && outpix == NULL)
    // break;
   }
   else
   if (LBMIDEQ(c.id, "CMAP"))
   {
    int i;
    if (c.size > 768) goto error;
    if (pal == NULL)
    {
     fseek(fp, c.size, SEEK_CUR);
     continue;
    }
    if (fread(pal, c.size, 1, fp) != 1) goto error;
    for (i = 0; i < c.size; i++) pal[i] >>= 2;
   }
   else
   if (LBMIDEQ(c.id, "BODY"))
   {
    int i;
    if (outpix == NULL)
    {
     fseek(fp, c.size, SEEK_CUR);
     continue;
    }
    for (i = 0; i < h; i++)
    {
     if (!compression)
     {
      if (fread(outpix, w, 1, fp) != 1) goto error;
      if (w & 1) fgetc(fp);
      outpix += w;
     }
     else
     {
      int d, v;
      d = w;
      if (d & 1) d++;
      while (d > 0)
      {
       v = (signed char)fgetc(fp);
       if (v > 0)
       {
        v++;
        d -= v;
        if (fread(outpix, v, 1, fp) != 1) goto error;
        outpix += v;
       }
       else
       {
        int c;
        v = -v + 1;
        d -= v;
        c = fgetc(fp);
        if (c == EOF) goto error;
        memset(outpix, c, v);
        outpix += v;
       }
      }
     }
    }
   }
   else
   {
    fseek(fp, c.size, SEEK_CUR);
   }
  }
  FileClose(fp);
  return TRUE;
 error:
  FileClose(fp);
  return FALSE;
 }

 // =======================================================
 // TGA

 #pragma pack(1)

 typedef struct
 {
  byte idsize;
  byte cmapt;
  byte imgtype;
  word cmstart;
  word cmlen;
  byte cmdepth;
  word xoffs;
  word yoffs;
  word width;
  word height;
  byte bpp;
  byte imgd;
 } TGAHeader;

 typedef struct
 {
  dword extoffs;
  dword devoffs;
  char sig[18];
 } TGAFooter;

 #pragma pack()

extern int PIX_LoadTGA(const char* file, unsigned char *outpix, unsigned char * pal, int* width, int* height)
 {
  FILE *fp = NULL;
  long pos;
  TGAHeader hdr;
  TGAFooter foot;
  int i;

  fp = FileOpen(file);
  if (fp == NULL) goto error;
  pos = ftell(fp);

  fseek(fp, pos + FileSize(file) - sizeof(foot), SEEK_SET);
  memset(&foot, 0, sizeof(foot));
  fread(&foot, sizeof(foot), 1, fp);
  if (memcmp(foot.sig, "TRUEVISION-XFILE.", sizeof(foot.sig)) != 0) goto error;
  fseek(fp, pos, SEEK_SET);
  memset(&hdr, 0, sizeof(hdr));
  fread(&hdr, sizeof(hdr), 1, fp);
  if (width != NULL) *width = hdr.width;
  if (height != NULL) *height = hdr.height;
  if (outpix == NULL && pal == NULL) goto ok;

  // Now it should read the file itself. But doesn't.
  if (pal != NULL)
  {
   if (hdr.cmapt != 1) goto error;
   memset(pal, 0, 768);
   fseek(fp, pos + sizeof(hdr) + hdr.idsize, SEEK_SET);
   for (i = 0; i < 256 && i < hdr.cmlen; i++)
   {
    word  vw;
    switch (hdr.cmdepth)
    {
     case 15:
     case 16: vw = ((dword)((byte)getc(fp))) + (((dword)((byte)getc(fp))) << 8);
              pal[3*i+0] = (vw & 0x7C00) >> (10-3+2);
              pal[3*i+1] = (vw & 0x03E0) >> (5-3+2);
              pal[3*i+2] = (vw & 0x001F) << (3-0-2);
              break;
     case 24: pal[3*i+2] = getc(fp) >> 2;
              pal[3*i+1] = getc(fp) >> 2;
              pal[3*i+0] = getc(fp) >> 2;
              break;
     case 32: pal[3*i+2] = getc(fp) >> 2;
              pal[3*i+1] = getc(fp) >> 2;
              pal[3*i+0] = getc(fp) >> 2;
              getc(fp);
              break;
    }
   }
  }
  if (outpix != NULL)
  {
   long offs;

   if (hdr.imgtype != 1 && hdr.imgtype != 9) goto error;
   if (hdr.bpp != 8) goto error;
   offs = pos + sizeof(hdr) + hdr.idsize;
   if (hdr.cmapt != 0) offs += (hdr.cmdepth+1)/8*hdr.cmlen;
   fseek(fp, offs, SEEK_SET);
   if (hdr.imgtype == 1)
   {
    // Raw
    for (i = 0; i < hdr.height; i++)
    {
     fread(outpix + (hdr.height - i - 1)*hdr.width, hdr.width, 1, fp);
    }
   }
   else
   {
    // RLE
    long size;
    byte *outp;

    for (i = 0; i < hdr.height; i++)
    {
     size = hdr.width;
     outp = outpix + (hdr.height - i - 1)*hdr.width;
     while (size > 0)
     {
      byte p;
      p = (byte)getc(fp);
      if (p & 0x80)
      {
       byte v;
       v = getc(fp);
       p = p & 0x7F;
       p += 1;
       memset(outp, v, p);
      }
      else
      {
       p += 1;
       fread(outp, p, 1, fp);
      }
      outp += p;
      size -= p;
     }
    }
   }
  }
 ok:
  FileClose(fp);
  return TRUE;
 error:
  if (fp != NULL) FileClose(fp);
  return FALSE;
 }

// =======================================================

extern int PIX_Load(const char* file, unsigned char *outpix, unsigned char * pal, int* width, int* height)
 {
  if (!PIX_LoadGIF(file, outpix, pal, width, height))
   if (!PIX_LoadPCX(file, outpix, pal, width, height))
    if (!PIX_LoadLBM(file, outpix, pal, width, height))
     if (!PIX_LoadTGA(file, outpix, pal, width, height))
      return 0;
  return 1;
 }

// =======================================================

void Error(const char *txt)
 {
  printf("ERROR: %s\n", txt);
  exit(1);
 }

void Usage(void)
 {
  puts("BGIF, Big GIF/PCX/LBM/TGA8 dumper bye Jare. Version 1.7\n"
       "   Usage: BGIF file[.GIF]|[.PCX]|[.LBM]|[.TGA] [/P]");
  exit(1);
 }


void DumpFile(const char* fn, char * buf, unsigned long l)
 {
  FILE *f;

  if ((f = fopen(fn, "wb")) == NULL) Error("Creating file.");
  while (l > 16384L)
  {
   if (fwrite(buf, 16384, 1, f) < 1) Error("Writing file");
   buf = buf + 16384;
   l -= 16384;
  }
  if (fwrite(buf, (unsigned)l, 1, f) < 1) Error("Finalizing file");
  fclose(f);
 }

 char fname[300];
 char gifname[300];
 char pal[768];


void main(int argc, char *argv[])
 {
  char *p, *g;
  int   w = -1, h = -1;
  byte *im;

  if (argc < 2) Usage();
  p = strrchr(argv[1], '.');
  g = strrchr(argv[1], '\\');
  if (!p || (g && g > p))
  {
   struct stat s;
   sprintf(gifname, "%s.GIF", argv[1]);
   if (stat(gifname, &s) < 0) sprintf(gifname, "%s.PCX", argv[1]);
   if (stat(gifname, &s) < 0) sprintf(gifname, "%s.LBM", argv[1]);
   if (stat(gifname, &s) < 0) sprintf(gifname, "%s.TGA", argv[1]);
  }
  else sprintf(gifname, "%s", argv[1]);
  if (p != NULL) *p = '\0';

  printf("Reading %s...\n", gifname);

  if (!PIX_Load(gifname, NULL, NULL, &w, &h)) Error("File not found");

  printf("Width = %d, Height = %d\n", w, h);

  if (argc == 2 || toupper(argv[2][1]) != 'P')
  {
   sprintf(fname, "%s.pix", argv[1]);
   im = NEW(w*h);
  }
  else im = NULL;

  if (!PIX_Load(gifname, im, pal, NULL, NULL)) Error("Reloading pic.");

  sprintf(fname, "%s.pal", argv[1]);
  DumpFile(fname, pal, 768);
  if (im != NULL)
  {
   sprintf(fname, "%s.pix", argv[1]);
   DumpFile(fname, im, w*h);
  }
  exit(0);
 }

