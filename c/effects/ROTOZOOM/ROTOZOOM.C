#include <stdio.h>
#include <math.h>
#include <conio.h>

int *yTab;

#define PI 3.1415926535

// --------------------------------------------------------------------------

void InitRotoZoomTables(void)
{
 int i;
 unsigned long dU, dV;
 float realangle = -PI;

 yTab = (int *)malloc(200 * 4);
 for (i=0; i < 200; i++) yTab[i] = i * 320;
}

// --------------------------------------------------------------------------

void DrawRotoZoom(char *screen,char *image, int angle, int scale)
{
 register int rowU, rowV, dU, dV;
 int y, x, u, v, tx, ty;
 char c;
 float realangle;

// angle &= 4095;
 realangle = angle*((PI*2)/4096);
 dU = (int)(scale*sin(realangle));
 dV = (int)(scale*cos(realangle));
 rowU = ((-160 * dV) + (100 * dU)) + (160 << 10);
 rowV = ((-160 * dU) - (100 * dV)) + (100 << 10);


 y=200;
 while (y--)
 {
  u = rowU;
  v = rowV;
  x = 320;
  while (x--)
  {
   tx = (u >> 10);
   ty = (v >> 10);
   while (ty>199) ty-=200;
   while (ty<0) ty+=200;
   while (tx>319) tx-=320;
   while (tx<0) tx+=320;
   // check for disabling texture tiling
//   if (ty<=199 && tx<=319)
   {
    c = *(image + (yTab[ty]+tx));
    /*if (c!=0)*/ *screen = c; //else *screen = *screen -4;
   }
   screen++;
   u+=dV;
   v+=dU;
  }
  rowU -= dU;
  rowV += dV;
 }
}

// --------------------------------------------------------------------------

void preparezoom(int x1, int y1, int x2, int y2, unsigned short int *map)
{
 int x,y,xadd,yadd;
 int xx,yy,pos;

 pos = 0;
 x = x1<<8;
 y = y1<<8;
 xadd = ((x2-x1)<<8)/320;
 yadd = ((y2-y1)<<8)/200;
 for(yy=0;yy<200;yy++)
 {
  x = x1 << 8;
  for(xx=0;xx<320;xx++)
  {
   map[pos] = ((y >> 8)*320) + (x >> 8);
   pos++;
   x+=xadd;
  }
  y+=yadd;
 }
}

// -----

void drawzoom(unsigned short int *zoommap, char *bitmap,char *buffer)
{
 int i;
 char c;
 for (i=320;i<63680;i++)
 {

  c =(  bitmap[zoommap[i-321]]
      + bitmap[zoommap[i-320]]
      + bitmap[zoommap[i-319]]
      + bitmap[zoommap[i-1]]
      + bitmap[zoommap[i+1]]
      + bitmap[zoommap[i+319]]
      + bitmap[zoommap[i+320]]
      + bitmap[zoommap[i+321]]) >> 3;

/*

  c =(  bitmap[zoommap[i-320]]
      + bitmap[zoommap[i-1]]
      + bitmap[zoommap[i+1]]
      + bitmap[zoommap[i+320]]) >> 2;
*/

  c -= 3;

  if (c<0) c = 0;
  buffer[i] = c;
 }
}

// --------------------------------------------------------------------------

void Blur(char *dest, char *source)
{
 int i;

 memcpy(dest,source,320);
 memcpy(dest+64000-320,source+64000-320,320);
 for (i=320;i<63680;i++)
 {
/*
  dest[i] =(  source[i-321]
            + source[i-320]
            + source[i-319]
            + source[i-1]
            + source[i+1]
            + source[i+319]
            + source[i+320]
            + source[i+321]) >> 3;
*/
  dest[i] =(  source[i-320]
            + source[i-1]
            + source[i+1]
            + source[i+320]) >> 2;
 }
}

void Mix(char *dest, char *source)
{
 int i;

 for (i=0;i<64000;i++)
 {
  dest[i] = (dest[i] + source[i]) >> 1;
 }
}


// 께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께

void main(void)
{
 FILE *fp;
 char *bitmap, *pal;
 unsigned short int *ZoomMap;
 char *buffer1, *buffer2, *btemp;
 float RotoAngle;
 float scale;
 int angle,angleadder;
 float anglepos,scalepos;
 int i;
 int quit;
 char c;

// ----

 InitRotoZoomTables();

// ----- loading textures & palettes -----

 bitmap = (char *)malloc(65536);
 fp = fopen("tmk_bw.pix","rb");
 fread (bitmap,1,64000,fp);
 fclose(fp);

 pal = (char *)malloc(768);
 fp = fopen("tmk_bw.pal","rb");
 fread (pal,1,768,fp);
 fclose(fp);

// ----- memory alloc etc -----

// ZoomMap = (unsigned short int *)malloc(64000*2);
// preparezoom(7,7,313,193,ZoomMap);
 buffer1 = (char *)malloc(65536);
 memset(buffer1,0,65536);
 buffer2 = (char *)malloc(65536);
 memset(buffer2,0,65536);

 memcpy(buffer1,bitmap,64000);
 memcpy(buffer2,bitmap,64000);
// memcpy((char *)0xA0000,bitmap,64000);


// -----

 SetMode(0x13);
 for (i=0;i<256;i++) setrgb(i,pal[i*3],pal[i*3+1],pal[i*3+2]);

// -----

 RotoAngle = 0;
 scale = 1100;
 angle = 0;
 quit = 0;
 angleadder=3;

 while (quit==0)
 {



  angle+=angleadder;
  scale = 1100 + sin(scalepos)*50;
  DrawRotoZoom(buffer1,buffer2,angle,scale);
//  memcpy(buffer2,buffer1,64000);
//  Mix(buffer2,buffer1);
  Blur(buffer2,buffer1);
  memcpy(buffer2,bitmap,320*3);
  memcpy((char *)(0xA0000),buffer1,64000);

  if (kbhit())
  {
   c=getch();
   if (c==27) quit = 1;
   else if (c=='+') angleadder++;
   else if (c=='-') angleadder--;
   else if (c=='a') scalepos+=0.5;
   else if (c=='z') scalepos-=0.5;
   else angleadder = -angleadder;
   c=0;
  }
//  scalepos += 0.3;
 }

// -----

 getch();

 free(bitmap);
 free(pal);
 free(buffer1);
 free(buffer2);
// free(ZoomMap);

 SetMode(3);
}


void StartRoto(void)
{
 InitRotoZoomTables();
/*
 bitmap = (char *)malloc(65536);
 fp = fopen("tmk_bw.pix","rb");
 fread (bitmap,1,64000,fp);
 fclose(fp);

 pal = (char *)malloc(768);
 fp = fopen("tmk_bw.pal","rb");
 fread (pal,1,768,fp);
 fclose(fp);
*/
 buffer1 = (char *)malloc(65536);
 memset(buffer1,0,65536);
 buffer2 = (char *)malloc(65536);
 memset(buffer2,0,65536);

 memcpy(buffer1,bitmap,64000);
 memcpy(buffer2,bitmap,64000);

}

