 #include "extender.h"
 #include "std.h"
 #include "mxmplay.h"
 #include "data.h"
 #include "vesa.h"
 #include "mpgstill.h"
 #include "routines.h"
 // #include "event.h"

 // "system variables"
 char *mxm_mem;
 unsigned short int GusPort;
 char *VESA;
 void *EnvPtr;
 // sEventList_t EventList[];

 // other global variables
 unsigned char *xrmap,*yrmap,*source,*map1,*map2,*map3;

 // ---

void CreatePlasmaTexture(unsigned char *map)
 {
  int i;
  float scale,turbulence;

  xrmap  = (unsigned char *)malloc(0x10000);  memset(xrmap, 0,0x10000);
  yrmap  = (unsigned char *)malloc(0x10000);  memset(yrmap, 0,0x10000);
  source = (unsigned char *)malloc(0x10000);  memset(source,0,0x10000);

  memset(map,   0,65536);

  xrmap [0] = rand(); createfractalmap(xrmap, 0,0,255,255); for (i=0;i<3;i++) smoothmap(xrmap);
  yrmap [0] = rand(); createfractalmap(yrmap, 0,0,255,255); for (i=0;i<3;i++) smoothmap(yrmap);
  source[0] = rand(); createfractalmap(source,0,0,255,255); for (i=0;i<3;i++) smoothmap(source);

  scale      = rand()&63;
  turbulence = rand()&63;
  marble (source,map,scale, turbulence,xrmap,yrmap);
  for (i=0;i<3;i++) smoothmap(map);
 }

void Init(void)
 {
  float scale,turbulence;

  // --- Create textures ---

  DrawString("þ Calculating textures\n\r$");

  xrmap  = (unsigned char *)malloc(0x10000);   memset(xrmap, 0,65536);
  yrmap  = (unsigned char *)malloc(0x10000);   memset(yrmap, 0,65536);
  source = (unsigned char *)malloc(0x10000);   memset(source,0,65536);
  map1   = (unsigned char *)malloc(0x10000);   memset(map1,  0,65536);
  map2   = (unsigned char *)malloc(0x10000);   memset(map2,  0,65536);
  map3   = (unsigned char *)malloc(0x10000);   memset(map3,  0,65536);
  xrmap [0] = rand(); createfractalmap(xrmap, 0,0,255,255);
  yrmap [0] = rand(); createfractalmap(yrmap, 0,0,255,255);
  source[0] = rand(); createfractalmap(source,0,0,255,255);
  smoothmap(xrmap);   smoothmap(xrmap);   smoothmap(xrmap);
  smoothmap(yrmap);   smoothmap(yrmap);   smoothmap(yrmap);
  smoothmap(source);  smoothmap(source);  smoothmap(source);
  scale      = rand()&63;
  turbulence = rand()&63;
  marble (source,map1,scale, turbulence,xrmap,yrmap);
  smoothmap(map1);  smoothmap(map1);  smoothmap(map1);

  xrmap [0] = rand(); createfractalmap(xrmap, 0,0,255,255);
  yrmap [0] = rand(); createfractalmap(yrmap, 0,0,255,255);
  source[0] = rand(); createfractalmap(source,0,0,255,255);
  smoothmap(xrmap);   smoothmap(xrmap);   smoothmap(xrmap);
  smoothmap(yrmap);   smoothmap(yrmap);   smoothmap(yrmap);
  smoothmap(source);  smoothmap(source);  smoothmap(source);
  scale      = rand()&63;
  turbulence = rand()&63;
  marble (source,map2,scale, turbulence,xrmap,yrmap);
  smoothmap(map2);  smoothmap(map2);  smoothmap(map2);

  xrmap [0] = rand(); createfractalmap(xrmap, 0,0,255,255);
  yrmap [0] = rand(); createfractalmap(yrmap, 0,0,255,255);
  source[0] = rand(); createfractalmap(source,0,0,255,255);
  smoothmap(xrmap);   smoothmap(xrmap);   smoothmap(xrmap);
  smoothmap(yrmap);   smoothmap(yrmap);   smoothmap(yrmap);
  smoothmap(source);  smoothmap(source);  smoothmap(source);
  scale      = rand()&63;
  turbulence = rand()&63;
  marble (source,map3,scale, turbulence,xrmap,yrmap);
  smoothmap(map3);  smoothmap(map3);  smoothmap(map3);

  // --- Init player & VESA ---

  DrawString("þ Initializing XM player\n\r$");

  mxm_mem = (char *)malloc(16384);
  GusPort = xmpGetGUSPort((void *)(EnvAddr-Code32Addr));
  if (!xmpInit(&Module, GusPort, mxm_mem, 65536, MXMINTMODEDPMI)) DrawString("Error initializing player\n\r$");

  DrawString("þ Initializing VBE2.0\n\r$");

  VBE2_Init(640,480,24);
  VESA = (char *)VesaPtr - Code32Addr;
 }

 // ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

void Exit(void)
 {
  free(xrmap);
  free(yrmap);
  free(source);
  free(map1);
  free(map2);
  free(map3);

  VBE2_UnInit();
  xmpStop();
  if (mxm_mem != NULL) free(mxm_mem);
  SetMode(3);
 }

 // ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

void C_Entry(void)
 {
  int x,y;

  DrawString("\n\rInitializing...\n\r$");
  Init();

  // --- Start playing & entre gfx mode ---

  xmpPlay(0);
  VBE2_SetMode(VesaMode);

  // --- START INTRO ---

  //for (x=0;x<256;x++) SetRGB(x,x>>2,x>>2,x>>2);
  for (y=0;y<256;y++)
  {
   for (x=0;x<256;x++)
   {
    VESA[(y*640)*3+(x*3)]   = map1[y*256+x];
    VESA[(y*640)*3+(x*3)+1] = map2[y*256+x];
    VESA[(y*640)*3+(x*3)+2] = map3[y*256+x];
   }
  }
  getch();

  HexagonMap(map1,5,8,0,128,255);
  for (y=0;y<256;y++)
  {
   for (x=0;x<256;x++)
   {
    VESA[(y*640)*3+(x*3)]   = map1[y*256+x];
    VESA[(y*640)*3+(x*3)+1] = 0;
    VESA[(y*640)*3+(x*3)+2] = 0;
   }
  }
  getch();


  // --- END INTRO ---

  Exit();

 }
