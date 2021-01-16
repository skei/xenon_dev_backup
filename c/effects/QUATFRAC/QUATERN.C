#include <math.h>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>

// 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴

 int zant  = 450;   //z-resolution. bigger zant -> better resolution
 int zant1 = 15;   //z-resolution. bigger zant -> better resolution

 float xmin=-1.66;
 float xmax=1;
 float ymin=-1;
 float ymax=1;
 float zmin=-1.7;
 float zmax=1.7;
 int iter=4;

 float lightx=-1;
 float lighty=1;
 float lightz=-3;

 float vx=0;
 float vy=0;
 float vz=0;

 float cr=0.50;     //constant real value             0.50
 float ci=0.40;     //constant imaginary(1) value     0.40
 float cj=1.0;      //constant imaginary(2) value     1
 float ck=0.25;     //constant imaginary(3) value     0.05
 float wk=-0.55;    //4th dimension                  -0.55

 int background = 0;

 int maxcolor = 64;

 int sx,sy;
 float dx,dy,dz;
 float origx, origy, origz;
 float rminx, rminy, rminz;
 float dxx, dxy, dxz;
 float dyx, dyy, dyz;
 float dzx, dzy, dzz;
 float dzx1, dzy1, dzz1;
 float tempx, tempy, tempz;
 float cosx,cosy,cosz,sinx,siny,sinz;
 float z_buffer[640][10];
 int buffer_y;

 int color;
 char *screen;

// 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴

 // set video mode
void SetMode(unsigned short int);
 #pragma aux SetMode =\
  "int 10h",\
  parm [ax] modify [ax];

// 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴

void SetRGB (char,char,char,char);
 #pragma aux SetRGB=\
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


// 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴

void rotate3D(float *_x,float *_y,float *_z)
{
 float xx,xy,xz;
 float yx,yz;
 float zx,zy;
 float x,y,z;

 x = *_x;
 y = *_y;
 z = *_z;

 x-=origx;
 y-=origy;
 z-=origz;
 xy=y*cosx-z*sinx;
 xz=y*sinx+z*cosx;
 xx=x;
 x=xx;
 y=xy;
 z=xz;

 yx=x*cosy+z*siny;
 yz=-x*siny+z*cosy;
 x=yx;
 z=yz;

 zx=x*cosz-y*sinz;
 zy=x*sinz+y*cosz;
 x=zx;
 y=zy;
 x+=origx;y+=origy;z+=origz;

 *_x = x;
 *_y = y;
 *_z = z;
}

// 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴

void rotatevalues()
{
 rminx=xmin;
 rminy=ymin;
 rminz=zmin;
 rotate3D(&rminx, &rminy, &rminz);
 tempx=xmax;
 tempy=ymin;
 tempz=zmin;
 rotate3D(&tempx, &tempy, &tempz);
 dxx=(tempx-rminx)/sx;
 dxy=(tempy-rminy)/sx;
 dxz=(tempz-rminz)/sx;
 tempx=xmin;
 tempy=ymax;
 tempz=zmin;
 rotate3D(&tempx, &tempy, &tempz);
 dyx=(tempx-rminx)/sy;
 dyy=(tempy-rminy)/sy;
 dyz=(tempz-rminz)/sy;
 tempx=xmin;
 tempy=ymin;
 tempz=zmax;
 rotate3D(&tempx, &tempy, &tempz);
 dzx=(tempx-rminx)/zant;
 dzy=(tempy-rminy)/zant;
 dzz=(tempz-rminz)/zant;
 dzx1=dzx/zant1;
 dzy1=dzy/zant1;
 dzz1=dzz/zant1;
}

// 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴

float calc_l(float x, float y, float z)
{
  float lengde;
  float temp;
  float w;
  int m;

  w=wk;
  m=0;
  do
  {
   temp=x+x;
   x=x*x-y*y-z*z-w*w+cr;
   y=temp*y+ci;
   z=temp*z+cj;
   w=temp*w+ck;

   m++;
   lengde=x*x+y*y+z*z+w*w;
	} while ((m<iter) && (lengde<2));
 return lengde;
}

// 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴

float calc_angle(float w,float e,float n,float s,float cx,float cy,float cz)
{
 float lightdx,lightdy,lightdz;
 float lightlength;
 float fx,fy,fz;
 float flength;
 float c;

 lightdx=cx-lightx;
 lightdy=cy-lighty;
 lightdz=cz-lightz;

 lightlength=sqrt(lightdx*lightdx+lightdy*lightdy+lightdz*lightdz);

 fx=/*(0)*(s-n)*/-(e-w)*(dy+dy);
 fy=/*(e-w)*(0)*/-(dx+dx)*(s-n);
 fz=(dx+dx)*(dy+dy)/*-(0)*(0)*/;

 flength=sqrt(fx*fx+fy*fy+fz*fz);
 c=(fx*lightdx+fy*lightdy+fz*lightdz)/(flength*lightlength);
 return c;
}

// 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴

void show_buffer(int y)
{
 float a;
 int i,t;

 for (t=1; t<sx; t++)
 {
  for (i=1; i<=8; i++)
  {
   if ((y+i)<sy)
   {
    a=calc_angle(z_buffer[t-1][i],z_buffer[t+1][i],z_buffer[t][i-1],z_buffer[t][i+1]
      ,t*dx+xmin,(y+i)*dy+ymin,z_buffer[t][i]);
    if ((z_buffer[t][i]>zmax) && (background==0)) color = 0;
    else if (a<0) color = 1;
    else color = 1+(maxcolor-1)*a;
    screen[(y+i)*320 + t] = color;
   }
  }
 }
 for (t=0; t<640; t++)
 {
  z_buffer[t][0]=z_buffer[t][8];
  z_buffer[t][1]=z_buffer[t][9];
 }
 buffer_y=2;
}

// 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴

void main()
{
 int px,py;
 int pz, pz1;
 int i;
 float l;
 float dz1;
 int ve;

 screen = (char *)0xA0000;
 SetMode(0x13);

 for (i=0; i<64; i++) SetRGB(i, i, i, 0);

 sx=160;
 sy=100;
 dx=(xmax-xmin)/sx;
 dy=(ymax-ymin)/sy;
 dz=(zmax-zmin)/zant;
 dz1=dz/zant1;

 origx=(xmin+xmax)/2;
 origy=(ymin+ymax)/2;
 origz=(zmin+zmax)/2;

 ve=0;
// for (ve=0; ve<50; ve++)
 {  //only used when making animations
// vx=0;vy=0;vz=0;
  vx=vx/(180*3.14159265);
  vy=vy/(180*3.14159265);
  vz=vz/(180*3.14159265);

  cosx=cos(vx);
  cosy=cos(vy);
  cosz=cos(vz);
  sinx=sin(vx);
  siny=sin(vy);
  sinz=sin(vz);

  rotatevalues();
  buffer_y=0;
  for (py=0; py<=sy; py++)
  {
   for (px=0; px<=sx; px++)
   {
    tempx=rminx+px*dxx+py*dyx/*+pz*dzx*/;
    tempy=rminy+px*dxy+py*dyy/*+pz*dzy*/;
    tempz=rminz+px*dxz+py*dyz/*+pz*dzz*/;
    pz=0;
    do
    {
     tempx+=dzx;
     tempy+=dzy;
     tempz+=dzz;
     l=calc_l(tempx,tempy,tempz);
     pz++;
    } while ((l>2) && (pz<zant));
    pz1=0;
    do
    {
     pz1++;
     tempx-=dzx1;
     tempy-=dzy1;
     tempz-=dzz1;
     l=calc_l(tempx,tempy,tempz);
    } while (l<2);
    if (pz < zant) z_buffer[px][buffer_y]=zmin+pz*dz-pz1*dz1;
    else z_buffer[px][buffer_y]=zmax+dz;
    color = 15-pz/10;
    screen[py*320+px] = color;
    if (kbhit()) break;
   }
   buffer_y++;
   if (buffer_y==10) show_buffer(py-9);
   if (kbhit()) break;
  }
  if (!kbhit())
  {
   show_buffer(py-buffer_y);
  }
 }  //end of FOR-loop. Only used when making animations
 getch();
 SetMode(3);
}
