#include <math.h>
#include <string.h>   // memset

// --- typedef's ---

typedef unsigned char  bool;
typedef unsigned char  byte;
typedef unsigned short word;
typedef unsigned long  dword;

typedef signed char  sbyte;
typedef signed short sword;
typedef signed long  sdword;

typedef byte TScr[200][320];
typedef byte (*PScr)[320];
typedef struct
{
 int x, y, px, py, vx, vy;
} TCenter;


// --- define's ---

#define LONGX 160
#define LONGY 100
#define vgascr ((PScr)0xA0000UL)

#define beta1 5
#define beta2 25
#define beta3 15

#define VEL 16

// --- variables ---

TScr scr;
int CosTab[2048];
dword tdist1[2*LONGY+1][2*LONGX+1];
dword tdist2[2*LONGY+1][2*LONGX+1];
dword tdist3[2*LONGY+1][2*LONGX+1];
dword tdist4[2*LONGY+1][2*LONGX+1];
dword tdist5[2*LONGY+1][2*LONGX+1];
TCenter g1 = {65, 55}
       ,g2 = {75, 135}
       ,g3 = {145, 60};

// -----

// set video mode
void SetMode(unsigned short int);
#pragma aux SetMode =\
 "int 10h",\
 parm [ax] modify [ax];

// -----

void setrgb (char,char,char,char);
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

// -----

// Vaits for Vertical retrace to START
void waitvr (void);
#pragma aux waitvr =\
 " mov dx,3dah",\
 "wait1:",\
 " in al,dx",\
 " test al,8",\
 " jnz wait1",\
 "wait2:",\
 " in al,dx",\
 " test al,8",\
 " jz wait2",\
 modify [al dx];

// -----

void DumpScr(void)
{
 static int cont;
 int i, j;
 byte *p = vgascr;
 byte *s;
 int w1, w2;

 p += 320 * (cont&1)+ ((cont & 2)>>1);
 w1 = 321 - (cont&2);
 w2 = 319 + (cont&2);
 cont++;
 for(i=0;i<100;i+=2)
 {
  s=scr[i];
  for(j=0;j<160;j++)
  {
   *p++=*s;
   s++;
   p++;
  }
  p+=w1;
  s=scr[i+1];
  for(j=0;j<160;j++)
  {
   *p++=*s;
   s++;
   p++;
  }
  p+=w2;
 }
}

// -----

void InitCenter(TCenter *c)
{
 c->px = (c->px * LONGX) / 100;
 c->py = (c->py * LONGY) / 100;
 c->px = c->x << 8;
 c->py = c->y << 8;
 c->vx = 0;
 c->vy = 0;
}

// -----

void MoveCenter(TCenter  *c)
{
 if (c->py < (LONGY << 8)) c->vy += VEL;
 else if (c->py > (LONGY << 8)) c->vy -= VEL;
 if (c->px < (LONGX << 8)) c->vx += VEL;
 else if (c->px > (LONGX << 8)) c->vx -= VEL;
 c->px += c->vx;
 c->py += c->vy;
 c->x = c->px >> 8;
 c->y = c->py >> 8;
}

// -----

void main(void)
{
 int i,j;
 byte *s;

 for (i=0; i< 2048; i++) CosTab[i] = 256.0*cos(i*3.149648/512.0);
 for (i = 0; i < 2*LONGY+1; i++)
 {
  for (j = 0; j < 2*LONGX+1; j++)
  {
   tdist1[i][j] = (beta1 *(int) sqrt((j-LONGX)*(j-LONGX) +
                  (i-LONGY)*(i-LONGY))) %  1024;
   tdist2[i][j] = (beta2*(int) sqrt((j-LONGX)*(j-LONGX) +
                  (i-LONGY)*(i-LONGY))) %  1024;
   tdist3[i][j] = (beta3 *(int) sqrt((j-LONGX)*(j-LONGX) +
                  (i-LONGY)*(i-LONGY))) %  1024;
  }
 }
 for (i = 0; i < 2*LONGY+1; i++)
 {
  for (j = 0; j < 2*LONGX+1; j++)
  {
   tdist4[i][j] = 1024/(x1+sqrt((j-LONGX)*(j-LONGX) + (i-LONGY)*(i-LONGY)));
  }
 }
 for (i = 0; i < 2*LONGY+1; i++)
 {
  for (j = 0; j < 2*LONGX+1; j++)
  {
   tdist5[i][j] = (int) sqrt((j-LONGX)*(j-LONGX) + (i-LONGY)*(i-LONGY)) % 1024;
  }
 }
// memset(pal3, 0, 768);

 InitCenter(&g1);
 InitCenter(&g2);
 InitCenter(&g3);

 memset(vgascr, 0, 64000);
 SetMode(0x13);
 for (i=0;i<64;i++)
 {
  setrgb(i    , 0,    0,    i);
  setrgb(i+64 , 0,    i,    63-i);
  setrgb(i+128, i,    63-i, 0);
  setrgb(i+192, 63-i, 0,    0);
 }

 while (!kbhit())
 {
  for (i = LONGY/2; i < LONGY+LONGY/2; i++)
  {
   s = scr[i-LONGY/2];
   for (j = LONGX/2; j < LONGX+LONGX/2; j++)
   {
    *s++ = ( ( tdist4[LONGY+i-g1.y][LONGX+j-g1.x]
            -tdist4[LONGY+i-g2.y][LONGX+j-g2.x]
            +tdist4[LONGY+i-g3.y][LONGX+j-g3.x]
           ) / 2 ) + 128;
   }
  }

  waitvr();
  DumpScr();

  MoveCenter(&g1);
  MoveCenter(&g2);
  MoveCenter(&g3);
 }
 SetMode(3);
}



