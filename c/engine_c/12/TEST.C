// Test program using the Engine

#include "engine.h"

// :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// Variables

extern unsigned char *BackgroundBuffer;

sCamera cam;
char *palette;
char *texture;

sSpline camera_path;
int camloc;
sCoord campos;

sMorph dolphin_anim;
int dolphin_animpos;
sCoord anim_coords;

sObject *dolphin;

#include "face4.inc"
#include "inv_sph.inc"

#include "dolph1.inc"
#include "dolph2.inc"
#include "dolph3.inc"

// ±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±

// set video mode
void SetMode(unsigned short int);
#pragma aux SetMode =\
 "int 10h",\
 parm [ax] modify [ax];

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

void main(void)
{
 int i;

 calc_sincos();
 Init_Engine();
 env_lightx = 64;  env_lighty = 64;
 palette = Load_Palette("texture.pal");
 texture = Load_Texture("texture.pix");

 Object_Set_Rot(&face,3072,2048,2048);
 Object_Set_Pos(&face,0,0,800);
 Object_Set_Mapping(&face,0,texture);

 Object_Set_Rot(&sphere,0,0,0);
 Object_Set_Pos(&sphere,0,0,0);
 Object_Set_Mapping(&sphere,0,texture);

 Object_Set_Rot(&dolph1,0,0,0);
 Object_Set_Pos(&dolph1,0,0,0);
 Object_Set_Mapping(&dolph1,0,texture);

 Camera_Set_Rot(&cam,0,0,0);
 Camera_Set_Pos(&cam,0,0,0);
 cam.Bank = 0.0;

// Init camera path spline

 camera_path.num = 4;
 camera_path.oClosed = 1;
 camera_path.bendsize = (float)1.0;

 camera_path.p[0].x = -1000;
 camera_path.p[0].y = 0;
 camera_path.p[0].z = 300;

 camera_path.p[1].x = 400;
 camera_path.p[1].y = 0;
 camera_path.p[1].z = -500;

 camera_path.p[2].x = 1000;
 camera_path.p[2].y = 0;
 camera_path.p[2].z = 200;

 camera_path.p[3].x = 500;
 camera_path.p[3].y = 2000;
 camera_path.p[3].z = 0;

 dolphin = Object_Copy(&dolph1);
 Object_Set_Rot(dolphin,3072,2048,2048);
 Object_Set_Pos(dolphin,0,500,1000);
 Object_Set_Mapping(dolphin,0,texture);

 Anim_Init(&dolph1,&dolph2,&dolph3,&dolphin_anim);


 SetMode(0x13);
 for (i=0;i<256;i++) setrgb(i,palette[i*3],palette[i*3+1],palette[i*3+2]);

// °°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°

 while (!kbhit())
 {
  memset(BackgroundBuffer,0,64000);

// Camera motion (spline)
  camloc+=200;
  camloc&=65535;
  campos = get_spline_point(camloc, &camera_path);
  Camera_Set_Pos(&cam,campos.x,campos.y,campos.z);
  Camera_Set_Target(&cam,0,500,1000);
  Camera_Process_Aim(&cam);

// Draw background sphere
  Object_Project(&sphere,&cam);
  Object_Draw(BackgroundBuffer,&sphere);

// Dolphin animation/morph (spline mesh interpolation)
  dolphin_animpos+=1600;
  Anim_Update(dolphin,dolphin_animpos,&dolphin_anim);
  Object_Project(dolphin,&cam);
  Object_Draw(BackgroundBuffer,dolphin);

// Dolphin 1
  Object_Project(&dolph1,&cam);
  Object_Draw(BackgroundBuffer,&dolph1);

// Rotating face
// check_BoundingBox(&face,&cam);
  Object_Add_Rot(&face,10,15,5);
  Object_Project(&face,&cam);
  Object_Draw(BackgroundBuffer,&face);

  memcpy((char *)0xA0000,BackgroundBuffer,64000);
 }

// °°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°

 free(palette);
 free(texture);
 free(dolphin);
 Exit_Engine();

 SetMode(3);
}
