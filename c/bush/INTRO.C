 #include "std.h"        // std stuff... We're not linking in any Watcom libs!
 #include "fmath.h"      // direct FPU math stuff
 #include "mxmplay.h"    // MXM player

 #include "data.h"
 #include "routines.h"
 #include "font.h"

 // ±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±

 char *mxm_mem = NULL;
 unsigned short int GusPort;
 char c;
 char tempstring[256];
 int Music_Playing = 0;

 // ---

 char Palette[768];
 char *BackGround;
 char *Buffer;
 char *BitMap;
 char *Screen;
 char *SphereMap;
 char *SphereMapTexture;
 char *str;

 // - rotozoom -

 float angle,angleadder;
 float scale,scaleadder;

 // ±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±

void exit(char);
 #pragma aux exit=\
  " mov ah,0x4c",\
  " int 0x21",\
  parm [al] modify [eax];

 // -------------------------------------------------------------------------

void QuitMessage(char *s)
 {
  if (Music_Playing!=0)
  {
   xmpStop();
   Music_Playing=0;
  }
  SetMode(3);
  DrawString(s);
  exit(1);
 }

 // -------------------------------------------------------------------------

void Music_Init(void)
 {
  mxm_mem = (char *)malloc(16384);
  if ((int)mxm_mem==0) QuitMessage("Error allocating memory for player\n\r$");
  DrawString("enter middle gus port digit: 2x0h\b\b\b$");
  c = 0;
  while (c==0)
  {
   c = GetKey();
   if ((c<'0') || (c>'9')) c=0;
  }
  tempstring[0] = c;
  tempstring[1] = '\n';
  tempstring[2] = '\r';
  tempstring[3] = '$';
  DrawString(&tempstring);
  GusPort = 0x200 + ((c-48)<<4); // Port = 0x220;   // or 0 for NoSound
  if (!xmpInit(&Module, GusPort, mxm_mem, 65536, MXMINTMODEDPMI))
       QuitMessage("Error initializing player\n\r$");
 }

//
// ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
// ²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²
// ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
//

void MAIN(void)   // MAIN, not main, to avoid Watcom linking in _cstart_ etc
 {
  int i,j,k;
  int x,y;

  mxm_mem = NULL;
  // VesaMode = 0;
  Music_Playing=0;
  DrawString("\n\rNew intro.. Inf.. 19(c)19...\n\r$");

//
// ±±±±± Init ±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
//

// --- memory allocation ---

  Buffer     = (char *)malloc(64000);
  BitMap     = (char *)malloc(65536);
  BackGround = (char *)malloc(65536);

// --- music --

  Music_Init();

// --- misc tables ---

  Font_Setup();
  Init_Mul320Table();

// --- calculation ---

  // - background -

  memset(BackGround,0,65536);
  CalcBackground(BackGround,0);
  Smooth(BackGround);

  // - sphere mapping init -

  SphereMap = (char *)malloc(320*200*2);
  Init_SphereMap(SphereMap);
  // SphereMapTexture = VoxelPic;


  // - rotozoom -

  // copy background to rotozoom buffer
  memcpy(BitMap,BackGround,64000);

  // - last -


// --- palette ---

  for (i=0;i<18;i++)
  {
   Palette[i*3]   = i*3;
   Palette[i*3+1] = i*1;
   Palette[i*3+2] = i*2;
   // SetRGB(i,i*3,i*1,i*2);
  }
  for (i=0;i<16;i++)
  {
   Palette[(i+32)*3]   = i*4;
   Palette[(i+32)*3+1] = i*4;
   Palette[(i+32)*3+2] = i*4;
   // SetRGB(i+32,i*4,i*4,i*4);
  }

// --- final stuff ---

  Screen = (char *)0xA0000;

  SetMode(0x13);
  xmpPlay(0);
  Music_Playing=1;

//
// ±±±±± Main ±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
//

// --- Part 1 --- rotozoom w/writer ---

  angle = 0;
  scale = 512;
  angleadder = 0;
  scaleadder = 0;

  SetPalette(0,255,&Palette);

  // while (Music_GetPos() < 0x0400)
  while (inp(0x60)!=1)
  {
   DrawRotoZoom(Buffer,BitMap,fsin(angle)*1, 400+fsin(scale)*100);
   str = "inf";
   i = Font_StringWidth(str);
   Font_DrawString(Buffer,160-(i/2),90,str);

   WaitVR();
   memcpy(Screen,Buffer,64000);
   angle += 0.003;
   scale += 0.03;

   if (inp(0x60) == 1) goto Quit_Intro;
  }

//
// ±±±±± Exit ±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
//


  free(BackGround);

Quit_Intro:
  if (mxm_mem != NULL) free(mxm_mem);
  QuitMessage("That's it!\n\r$");
 }
