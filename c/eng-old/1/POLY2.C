#include "engine.h"

char * HWritePagePtr;    // (char *) 0xa0000
int HLeftClip;           // 0
int HRightClip;          // 320
int HTopClip;            // 0
int HBotClip;            // 200

// --------------------------------------------------------------------------

int shl10idiv(int x, int y);
#pragma aux shl10idiv = \
 " mov   edx, eax "\
 " shl   eax, 10  "\
 " sar   edx, 22  "\
 " idiv  ebx      "\
 parm [eax] [ebx] \
 modify exact [eax edx] \
 value [eax]


// --------------------------------------------------------------------------

static sVertex *left_array[3], *right_array[3];
static int left_section, right_section;
static int left_section_height, right_section_height;
static int left_u, delta_left_u, left_v, delta_left_v;
static int left_x, delta_left_x, right_x, delta_right_x;
static int vertclip, horzclip;

// --------------------------------------------------------------------------

static int RightSection(void)
{
 sVertex *v1, *v2;
 int height;
 int y1, y2;
 int oheight;

 v1 = right_array[ right_section ];
 v2 = right_array[ right_section-1 ];
 if(!vertclip)
 {
  height = v2->iy - v1->iy;
  if(height == 0) return 0;
  delta_right_x = ((v2->ix - v1->ix) << 16) / height;
  right_x = v1->ix << 16;
 }
 else
 {
  y1 = v1->iy;
  y2 = v2->iy;
  oheight = height = y2-y1;   // Unclipped height
  if((y2 -= HBotClip) > 0) height -= y2;               // Skip pixels at end
  y2 = 0;
  if(y1 < HTopClip)
  y2 = HTopClip - y1;
  if((height -= y2) <= 0) return height;        // Skip pixels at start
  // y2 = pixels skipped at start
  delta_right_x = ((v2->ix - v1->ix) << 16) / oheight;
  right_x = (v1->ix << 16) + delta_right_x * y2;
 }
 right_section_height = height;
 return height;
}

// --------------------------------------------------------------------------

static int LeftSection(void)
{
 sVertex *v1,*v2;
 int height,oheight;
 int y1,y2;


 v1 = left_array[ left_section ];
 v2 = left_array[ left_section-1 ];

 if(!vertclip)
 {
  height = v2->iy - v1->iy;
  if(height == 0) return 0;
  delta_left_x = ((v2->ix - v1->ix) << 16) / height;
  left_x = v1->ix << 16;
  delta_left_u = ((v2->iu - v1->iu) << 10) / height;
  left_u = v1->iu << 10;
  delta_left_v = ((v2->iv - v1->iv) << 10) / height;
  left_v = v1->iv << 10;
 }
 else
 {
  y1 = v1->iy;
  y2 = v2->iy;
  oheight = height = y2-y1;   // Unclipped height

  if((y2 -= HBotClip) > 0) height -= y2;               // Skip pixels at end
  y2 = 0;
  if(y1 < HTopClip) y2 = HTopClip - y1;
  if((height -= y2) <= 0) return height;        // Skip pixels at start
  // y2 = pixels skipped at start
  delta_left_x = ((v2->ix - v1->ix) << 16) / oheight;
  left_x = (v1->ix << 16) + delta_left_x * y2;
  delta_left_u = ((v2->iu - v1->iu) << 10) / oheight;
  left_u = (v1->iu << 10) + delta_left_u * y2;
  delta_left_v = ((v2->iv - v1->iv) << 10) / oheight;
  left_v = (v1->iv << 10) + delta_left_v * y2;
 }
 left_section_height = height;
 return height;
}

// --------------------------------------------------------------------------

void DrawTextureTriangle( sPolygon *P )
{
 char *bitmap;
 sVertex *v1,*v2,*v3,*vv;
 int top;
 int height;
 int owidth;
 int temp;
 int longest;
 int dudx,dvdx;
 int du,dv;
 unsigned short int *destptr,*dest;
 int x1, x2, width, u, v;

 unsigned short int *HWritePagePtr = _VesaBuffer;
 int HLeftClip = 0;
 int HRightClip = 320;
 int HTopClip = 0;
 int HBotClip = 200;

 P->v[0]->iu *= 16;
 P->v[0]->iv *= 16;
 P->v[1]->iu *= 16;
 P->v[1]->iv *= 16;
 P->v[2]->iu *= 16;
 P->v[2]->iv *= 16;

 v1 = P->v[0];
 v2 = P->v[1];
 v3 = P->v[2];

 if(v1->iy > v2->iy) { vv=v1; v1=v2; v2=vv; }
 if(v1->iy > v3->iy) { vv=v1; v1=v3; v3=vv; }
 if(v2->iy > v3->iy) { vv=v2; v2=v3; v3=vv; }
 // Check if we need vertical clipping
 if((top = v1->iy) < HTopClip)
 {
  top = HTopClip;
  vertclip = 1;
 }
 else
 if(v3->iy > HBotClip) vertclip = 1;
  else vertclip = 0;
 height = v3->iy - v1->iy;
 if(height == 0) return;
 temp = ((v2->iy - v1->iy)<<16)/height;
 longest = temp*(v3->ix - v1->ix)+((v1->ix - v2->ix)<<16);
 if(longest == 0) return;
 if(longest < 0)
 {
  right_array[0] = v3;
  right_array[1] = v2;
  right_array[2] = v1;
  right_section  = 2;
  left_array[0]  = v3;
  left_array[1]  = v1;
  left_section   = 1;
  if(LeftSection() <= 0) return;
  if(RightSection() <= 0)
  {
   right_section--;
   if(RightSection() <= 0) return;
  }
  if(longest > -0x1000) longest = -0x1000;
 }
 else
 {
  left_array[0]  = v3;
  left_array[1]  = v2;
  left_array[2]  = v1;
  left_section   = 2;
  right_array[0] = v3;
  right_array[1] = v1;
  right_section  = 1;
  if(RightSection() <= 0) return;
  if(LeftSection() <= 0)
  {
   left_section--;
   if(LeftSection() <= 0) return;
  }
  if(longest < 0x1000) longest = 0x1000;
 }
 // Check if we need horizontal clipping
 if(v1->ix > HRightClip ||
    v1->ix < HLeftClip  ||
    v2->ix > HRightClip ||
    v2->ix < HLeftClip  ||
    v3->ix > HRightClip ||
    v3->ix < HLeftClip)    horzclip = 1;
  else horzclip = 0;
 dudx = shl10idiv(temp*(v3->iu - v1->iu)+((v1->iu - v2->iu)<<16),longest);
 dvdx = shl10idiv(temp*(v3->iv - v1->iv)+((v1->iv - v2->iv)<<16),longest);
 destptr = (unsigned short int *)(top * 320 + HWritePagePtr);
 bitmap = P->Object->Texture1;
 for(;;)
 {
  if(!horzclip)
  {
   x1 = left_x >> 16;
   width = (right_x >> 16) - x1;
   u = left_u;
   v = left_v;
  }
  else
  {
   x1 = left_x >> 16;
   x2 = right_x >> 16;
   owidth = width = x2-x1;
   if((x2 -= HRightClip) > 0) width -= x2;               // Skip pixels at end
   x2 = 0;
   if(x1 < HLeftClip)
   {
    x2 = HLeftClip - x1;
    x1 = HLeftClip;
   }
   if((width -= x2) <= 0) goto linedone;        // Skip pixels at start
   // x2 = pixels skipped at start
   u = (left_u + dudx * x2);
   v = (left_v + dvdx * x2);
  }
  if(width > 0)
  {
   dest = destptr + x1;
   du = dudx;
   dv = dvdx;
   do
   {
    *dest++ = bitmap[ ((v>>8) & 0xff00) + ((u>>16) & 0x00ff) ];
    u += du;
    v += dv;
   }
   while(--width);
  }
linedone:
  destptr += 320;
  if(--left_section_height == 0)
  {
   if(--left_section <= 0) return;
   if(LeftSection() <= 0) return;
  }
  else
  {
   left_x += delta_left_x;
   left_u += delta_left_u;
   left_v += delta_left_v;
  }
  if(--right_section_height == 0)
  {
   if(--right_section <= 0) return;
   if(RightSection() <= 0) return;
  }
  else
  {
   right_x += delta_right_x;
  }
 }
}

// 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴










































































































































































































































































