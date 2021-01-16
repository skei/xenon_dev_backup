// Test program using the engine

#include <stdio.h>    // file stuff
#include <time.h>     // clock

#include "engine.h"

// ----- font ---------------------------------------------------------------

#define fontwidth 24
#define fontheight 20
#define spacewidth 7

#define widthvalue 254
#define transvalue 255

char *tekst;        // for FPS calc
int timestart;
float fps;
int frames_rendered;

unsigned char *font_offset[256];
unsigned char *font_data;
unsigned char s[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890,.'!\0";

// --------------------------------------------------------------------------

sObject *Obj1;
sLight Light;

char *palette;
char *tex1;

// 北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

// calculates offset for each char
// p -> string containing characters in order of appearance in font data
// f -> font data
void CalcFontPointers(unsigned char *p, unsigned char *f)
{
 int n;
 int pos;

 for (n=0;n<256;n++) font_offset[n] = f;
 n = 0;
 while (p[n] != 0)
 {
  font_offset[p[n]] = f;
  f = (unsigned char *)( (int)f + (fontwidth*fontheight));
  n++;
 }
}

// draw one char
void DrawChar(int x1, int y1, unsigned char c, char *b, int m)
{
 int x,y;
 char *p;
 unsigned char *s;
 unsigned char *ss;

 if (c==' ') return;
 s = (unsigned char *)((int)b+y1*m+x1);
 p = font_offset[c];
 for (y=0;y<fontheight;y++)
 {
  ss = s;
  for (x=0;x<fontwidth;x++)
  {
   c = p[y*fontwidth+x];
   if ((c != widthvalue) && (c != transvalue)) *s = c;
   s++;
  }
  s = (unsigned char *)((int)ss+m);
 }
}

// -----

// returns width of character
int CharWidth(unsigned char c)
{
 int w;
 unsigned char *p;


 if (c==' ') return(spacewidth);
 w = 0;
 p = font_offset[c];
 while (*p != widthvalue)
 {
  p++;
  w++;
 }
 w++;
 return(w);
}

// -----

void DrawString(int x1, int y1, char *s, char *b, int m)
{
 int i;

 i=0;
 while (s[i] != 0)
 {
  DrawChar(x1,y1,s[i], b, m);
  x1+=CharWidth(s[i]);
  i++;
 }
}

// --------------------------------------------------------------------------

// 北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

void main( int argc, const char * argv[] )
{
 FILE *fp;
 int i;
 int x,y,z;
 char c;
 int size;
 int GroupNum;
 sGroup *G1;

 if (argc == 2)
 {
  size = 65536;
  font_data = (unsigned char *)malloc(size);
  fp = fopen("font.raw","rb");
  fread(font_data,1,size,fp);
  fclose(fp);
  CalcFontPointers(s,font_data);
// --------------------------------------------------------------------------
 Engine_Init(1000);
 palette = Palette_Load("palette.raw");
 tex1 = Texture_Load("texture.raw");
// Calc_ShadeHighLight(palette,_ShadeTable,50,0,0,0);
// Calc_GouraudMap();
 fp = fopen("shade.raw","rb");
 fread(_ShadeTable,1,32768,fp);
 fclose(fp);
 fp = fopen("phong.raw","rb");
 fread(_PhongMap,1,65536,fp);
 fclose(fp);

 Obj1 = Object_Load( argv[1] );
 Object_set_Pos(Obj1,0,0,0);
 G1 = Object_Get_Group(Obj1,0);
 Group_Set_Texture(G1,tex1);
 Group_Set_RenderMethod(G1,RENDER_GouraudTexture);

 World.Objects = Obj1;

 Camera_Set_Pos(&Camera,0,0,-500);
 Camera_Set_Rot(&Camera,0,0,0);

 Light.Flags = 1;
 Light.Pos.x = 0;
 Light.Pos.y = 0;
 Light.Pos.z = 0;
 Light.Rot.x = 0;
 Light.Rot.y = 63;
 Light.Rot.z = 0;
 Light.Next = NULL;

 World.cameratype = 0;
 World.Camera = &Camera;
 World.Lights = &Light;

 SetMode(0x13);
 SetPalette(palette);
// setrgb(0,0,0,0);

 frames_rendered = 0;
 timestart = clock();
 tekst = (char *)malloc(256);

 while (!kbhit())
 {
  Object_Add_Rot(Obj1,4,5,6);
  asm_ClearBuffer();
  Render();
  frames_rendered++;
  fps = (float)(frames_rendered * CLOCKS_PER_SEC) / (clock() - timestart);
  sprintf(tekst,"FPS %i",(int)fps);
  DrawString(4,4,tekst,(char *)(_Buffer+(960*200+320)),960);
  waitvr();
  asm_CopyBuffer();
 }

 free(tekst);

 SetMode(0x3);
 Texture_Free(tex1);
 Palette_Free(palette);
 Object_Free(Obj1);
 Engine_Exit();
// --------------------------------------------------------------------------
 }
 else
 {
  printf("TEST <3DO-file>\n");
 }
}

