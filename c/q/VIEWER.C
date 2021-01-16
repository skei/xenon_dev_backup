 #include <stdio.h>    // file stuf
 #include <stdlib.h>   // exit
 #include <string.h>   // memcpy
 #include <time.h>     // clock

 #include "engine.h"
 #include "vbe2.h"

 // --------------------------------------------------------------------------

 // GIF loader

 #ifndef TRUE
  #define TRUE 1
 #endif

 #ifndef FALSE
  #define FALSE 0
 #endif

 #define NEW(c) (malloc((c)))
 #define OUTB(c) (*outpix++ = (unsigned char)(c))

 #define MAX_CODES 4096

FILE *FileOpen(const char *fname)
 {
  return fopen(fname, "rb");
 }

void FileClose(FILE *f)
 {
  fclose(f);
 }

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

int getw(FILE *f)
 {
  int k = (unsigned)fgetc(f);
  k += 256*fgetc(f);
  return k;
 }

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

 // --------------------------------------------------------------------------

 int timestart;
 float fps;
 int frames_rendered;

 char tekst[256];

 char *Tex;
 char *Pal;

 sObject *tempO;
 sObject *O;

 int width,height;
 int i;
 int quit,rotate;
 char c;

 char *dummy;
 float mult;

 /*

 VIEWER test.3ds 1 texture

 */

void main( int argc, const char *argv[] )
 {
  if (argc != 4)
  {
   printf("\nUsage:    VIEWER <3ds-file> <size> <texture> [..]\n");
   printf("where 3ds-file = 3ds file to view\n");
   printf("      size     = size multiplicator\n");
   printf("      texture  = texture to use (256x256 GIF file\n");
   exit(1);
  }
  if (Engine_Init(320,200,15,TRUE,10000) == FALSE)
  {
   printf("Where's your Vesa 2.0 driver???\n");
   exit(1);
  }

  printf("Vesa 2.0 found\n");
  printf("Mode %X: %i,%i,%i\n",VesaMode,320,200,15);
  printf("RedMaskSize  : %i   RedFieldPosition  : %i\n",RedMaskSize,RedFieldPosition);
  printf("GreenMaskSize: %i   GreenFieldPosition: %i\n",GreenMaskSize,GreenFieldPosition);
  printf("BlueMaskSize : %i   BlueFieldPosition : %i\n",BlueMaskSize,BlueFieldPosition);
  printf("Bytes per scanline: %i\n",BytesPerScanLine);
  printf("Address: %X\n",VesaPtr);

  printf("ESC to quit, any other key to continue\n");
  c = getch();
  if (c==27)
  {
   Engine_UnInit(TRUE);
   VesaUnInit();
   exit(1);
  }

  // ------------------------------
  // read the texture
  // ------------------------------

  printf("Loading texture: %s\n",argv[3]);
  Load_GIF(argv[3],NULL,NULL,&width,&height);
  if ((width!=256)||(height!=256))
  {
   Engine_UnInit(TRUE);
   VesaUnInit();
   printf("\nGIF file must be 256x256!!!\n");
   exit(2);
  }
  Tex = (char *)malloc(width*height);
  Pal = (char *)malloc(768);
  Load_GIF(argv[3], Tex, Pal,&width,&height);

  // ------------------------------
  // Calculate shading table(s)
  // ------------------------------

  // VesaCalc_ShadeHighLight(palette,_VesaShadeTable,50,0,0,0);
  VesaCalc_ShadeTable(Pal, _VesaShadeTable);
  Setup_Intensity(10);

  // ------------------------------
  // Load the objects
  // ------------------------------

 mult = strtod(argv[2],&dummy);
 printf("Loading object(s): %s\n",argv[1]);
 printf("Vertex multiplier: %5.2f\n",mult);
 O = Load3DS(argv[1],mult,250);
 if (O==NULL)
 {
  Engine_UnInit(TRUE);
  VesaUnInit();
  printf("Invalid 3DS file, or error during loading!\n");
  exit(3);
 }

 tempO = O;
 while ( tempO!= NULL)
 {
//  Object_SetPos(tempO,0,0,0);
  tempO->Texture1 = Tex;
  tempO->Flags |= OBJECT_envmap;
  tempO = tempO->Child;
 }

// ------------------------------
// Setup some other values
// ------------------------------

 World->Flags = 0;
 World->Objects = O;
 World->Camera = Camera;
 World->NumLights = 0;
 World->Lights = NULL;
 World->AmbientLight = 20.0/63.0;

 Camera->Flags = CAMERA_free;

 Camera_SetPos(Camera,0,0,-1000);
 Camera_SetRot(Camera,0,0,0);
 Camera_SetTarget(Camera,0,0,0);
 Camera->Roll = 0;
 Camera->UpVector.x = 0;
 Camera->UpVector.y = -1;
 Camera->UpVector.z = 0;

// ------------------------------
// Gfx on!
// ------------------------------

 Set_Vesa(VesaMode);

// ------------------------------
// Start the rendering loop
// ------------------------------

 frames_rendered = 0;
 timestart = clock();

 quit = 0;
 rotate = 0;
 while (quit==0)
 {
  memset(_VesaBuffer,0,64000*2);

  Render_Setup( World );
  Render_Draw( World );

  if (rotate==1) Object_AddRot(O, 0.03, 0.04, 0.05 );

  frames_rendered++;
  fps = (float)(frames_rendered * CLOCKS_PER_SEC) / (clock() - timestart);

  sprintf(tekst,"VISIBLE POLYS %i",World->VisiblePolys);
  drawstring(0, 0,tekst,0x3fff,_VesaBuffer);
  sprintf(tekst,"FPS %5.1f",fps);
  drawstring(0, 8,tekst,0x3fff,_VesaBuffer);

  memset(_VesaBuffer+(320*198),0xff,World->VisibleObjects*2);

//  waitvr();

  memcpy (_VesaScreen,_VesaBuffer,64000*2);

  if (kbhit())
  {
   c = getch();
   switch(c)
   {
    case 27 : quit = 1;
              break;
    case 'q' : Camera_AddPos(Camera, -5,  0,  0);
               break;
    case 'w' : Camera_AddPos(Camera, +5,  0,  0);
               break;
    case 'a' : Camera_AddPos(Camera,  0, -5,  0);
               break;
    case 'z' : Camera_AddPos(Camera,  0, +5,  0);
               break;
    case 'x' : Camera_AddPos(Camera,  0,  0, -5);
               break;
    case 's' : Camera_AddPos(Camera,  0,  0, +5);
               break;
    case 'Q' : Camera_AddRot(Camera, -0.02,     0,     0);
               break;
    case 'W' : Camera_AddRot(Camera, +0.02,     0,     0);
               break;
    case 'A' : Camera_AddRot(Camera,     0, -0.02,     0);
               break;
    case 'Z' : Camera_AddRot(Camera,     0, +0.02,     0);
               break;
    case 'X' : Camera_AddRot(Camera,     0,     0, -0.02);
               break;
    case 'S' : Camera_AddRot(Camera,     0,     0, +0.02);
               break;
    case ' ' : if (rotate == 1) rotate=0;
               else if (rotate == 0) rotate=1;
               break;
   }
   c = 0;
  }
 }

// ------------------------------
// Free objects
// ------------------------------

 Object_Free(O);

 free(Tex);

 Set_Vesa(3);
 SetMode(3);

 Engine_UnInit(TRUE);
}
