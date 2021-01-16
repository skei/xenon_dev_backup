 #include <math.h>
 #include <stdio.h>

// --------------------------------------------------------------------------

#define ScreenWidth  320
#define ScreenHeight 200

// --------------------------------------------------------------------------

void SetMode(unsigned short int);
 #pragma aux SetMode =\
 "int 10h",\
 parm [ax] modify [ax];

void setrgb(char color, char r,char g, char b);
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

// --------------------------------------------------------------------------

float z = 128.0;
float umin = -128.0; // These dictate where in the tmap the coords
float vmin = -128.0; // are, and how large.  If you don't use
float umid =  128.0; // a 256x256 map, you'll have to mask the
float vmid =  128.0; // coords.
int x,y;
float du,dv;

char *sp_map;
char pal[768];

// --------------------------------------------------------------------------

void Init_SphereMap( char *p )
{
 float n;
 float nnn;

 z = ((ScreenWidth/320)*z);
 // calc_coords;
 x = -160;
 y = -100;
 nnn = (x*x)+(y*y)+(z*z);
 du = (sqrt(nnn)*umin)/x;
 dv = (sqrt(nnn)*vmin)/y;
 for (y=-100;y<100;y++)
 {
  for (x=-160;x<160;x++)
  {
   nnn = ((x*x)+(y*y)+(z*z));
   if (nnn==0)    n = 0;   else n = 1/sqrt(nnn);
   *p++ = ((n*x)*du)+umid;
   *p++ = ((n*y)*dv)+vmid;
  }
 }
}

// --------------------------------------------------------------------------

// void DrawSphere( char *map, char *dest, unsigned int uv, char *tex )
void DrawSphere( char *map, char *dest, char uadd, char vadd,char *tex )
{
 int i;
 unsigned short int uv_add;
 unsigned short int *m;
 unsigned char u,v;

 // Denne b›r gj›res om...
 // Man b›r la U og V wrappe istedenfor overflow g†r over fra U til V :-/
 // m = (unsigned short int *)map;
 for (i=0;i<64000;i++)
 {
  v = (*map++)+vadd;
  u = (*map++)+uadd;
  *dest++ = tex[((v<<8)+u)&0xffff];
 }
}

// --------------------------------------------------------------------------

void main(void)
{
 FILE *fp;
 float fu,fv;
 char u,v;
 int i;
 char *bitmap;
 int interpol;

 bitmap = (char *)malloc(65536);
 // fp = fopen("bitmap.pix","rb");
 fp = fopen("map1.pix","rb");
 fread(bitmap,1,65536,fp);
 fclose(fp);
 // fp = fopen("bitmap.pal","rb");
 fp = fopen("map1.pal","rb");
 fread(pal,1,768,fp);
 fclose(fp);

 SetMode(0x13);
 for (i=0;i<256;i++) setrgb(i,pal[i*3],pal[i*3+1],pal[i*3+2]);
 sp_map = (char *)malloc(320*200*2);

 Init_SphereMap(sp_map);

 // fp = fopen("map.raw","rb");
 // fread(sp_map,1,128000,fp);
 // fclose(fp);

 interpol = 0;
 while (!kbhit())
 {
  u = sin(fu)*255;
  v = cos(fv)*255;
  fu+= 0.002;
  fv+= 0.0035;
  // DrawSphere(sp_map,(char *)0xA0000,(v*256+u),bitmap);

  DrawSphere(sp_map,(char *)0xA0000,u,v,bitmap);
//  InterPolate(sp_map,sp_map1,sp_map2);
 }
 getch();
 SetMode(3);
 free(sp_map);
 free(bitmap);
}

