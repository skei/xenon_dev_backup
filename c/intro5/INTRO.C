#include "std.h"        // std stuff... We're not linking in any Watcom libs!
#include "fmath.h"      // direct FPU math stuff
#include "mxmplay.h"    // MXM player
#include "vbe2.h"       // Vesa 2.0 support

#include "data.h"

// #include "syndrom.c"    // The module, in array format

// ±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±

char *mxm_mem;
unsigned short int GusPort;
char c;
char tempstring[256];
unsigned short int *Vesa;
int Music_Playing;

// ±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±

void exit(char);
#pragma aux exit=\
 " mov ah,0x4c",\
 " int 0x21",\
 parm [al] modify [eax];

// --------------------------------------------------------------------------

void QuitMessage(char *s)
{
 if (Music_Playing!=0)
 {
  xmpStop();
  Music_Playing=0;
 }
 if (VesaMode != 0)
 {
  VesaUnInit();
  VesaMode = 0;
  Set_Vesa(3);
 }
 SetMode(3);
 DrawString(s);
 exit(1);
}

// --------------------------------------------------------------------------

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

// ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
// ²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²
// ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

void MAIN(void)   // MAIN, not main, to avoid Watcom linking in _cstart_ etc
{
 int i,j,k;

 VesaMode = 0;
 Music_Playing=0;
 DrawString("\n\rNew intro from Inf.. 19(c)19...\n\r$");

 // ±±±±± Init ±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±

 if (VesaInit(320,200,15) != 1) QuitMessage("Error initializing VBE2 320x200x15\n\r$");
 Music_Init();
 Set_Vesa(VesaMode);
 Vesa = (unsigned short int *)VesaPtr;
 xmpPlay(0);
 Music_Playing=1;

 // ±±±±± Main ±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±

 for (i=0;i<32;i++)
 {
  for (j=0;j<32;j++)
  {
   Vesa[j*320+i] = (j<<5) + i;
  }
 }
 GetKey();

 // ±±±±± Exit ±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±

//Quit_Intro:
 if (mxm_mem != NULL) free(mxm_mem);
 QuitMessage("That's it!\n\r$");
}
