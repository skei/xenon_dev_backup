

void Init_SphereMap( char *p )
 {
  #define ScreenW  320
  #define ScreenH  200

  #define umin -128.0
  #define vmin -128.0
  #define umid  128.0
  #define vmid  128.0

  float n;
  float nnn;
  int x,y;
  float du,dv;
  float z = 256.0;


  z = (ScreenW/320)*200.0;   // *z
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

void DrawSphere( char *map, char *dest, unsigned char u, unsigned char v, char *tex )
 {
  int i;
  unsigned char *m;

  m = (unsigned char *)map;
  for (i=0;i<64000;i++)
  {
   *dest++ = tex[(((v + *m++)<<8) + (u + *m++))&0xffff];
  }
 }

