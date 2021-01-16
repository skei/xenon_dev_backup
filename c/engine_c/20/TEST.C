// Test program using the Engine

#include "defines.h"
#include "types.h"
#include "engine.h"
#include "api.h"
#include "bytesort.h"
#include "spline.h"
#include "texmap.h"
#include "math3d.h"

#ifdef VisCVersion
 #include "winmain.h"
#endif
#ifdef WatcomVersion
 #include "dosmain.h"
#endif

// :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// Variables

extern screen_S buffer;

camstruc cam;
char *palette;
char *texture;

spline camera_path;
int camloc;
coord campos;

morphstruc dolphin_anim;
int dolphin_animpos;
coord anim_coords;

objstruc *dolphin;

#include "face4.inc"
#include "inv_sph.inc"

#include "dolph1.inc"
#include "dolph2.inc"
#include "dolph3.inc"

// :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

void main_init(void)
{
 calc_sincos();
 lightx = 64;  lighty = 64;
 palette = Load_Palette("texture.pal");
 texture = Load_Texture("texture.pix");
 Buffer_Init(SCREEN_WIDTH,SCREEN_HEIGHT, palette);

 
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
 cam.Bank = 0;

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
}

void main_process(void)
{
 Buffer_Clear(0);

// Camera motion (spline)
 camloc+=50;
 camloc&=16383;
 campos = get_spline_point(camloc, &camera_path);
 Camera_Set_Pos(&cam,campos.x,campos.y,campos.z);
 Camera_Set_Target(&cam,0,500,1000);
 Camera_Process_Aim(&cam);

// Camera_Set_Pos(&cam,0,0,-500);
// Camera_Add_Rot(&cam,10,20,5);
// Camera_Process_Free(&cam);

// Draw background sphere
 Object_Project(&sphere,&cam);
 Object_Draw(&buffer,&sphere);

// Dolphin animation/morph (spline mesh interpolation) 
 dolphin_animpos+=400;
 Anim_Update(dolphin,dolphin_animpos,&dolphin_anim);
 Object_Project(dolphin,&cam);
 Object_Draw(&buffer,dolphin);

// Dolphin 1
 Object_Project(&dolph1,&cam);
 Object_Draw(&buffer,&dolph1);

// Rotating face 
// check_BoundingBox(&face,&cam);
 Object_Add_Rot(&face,10,15,5);
 Object_Project(&face,&cam);
 Object_Draw(&buffer,&face);

}


void main_exit(void)
{
 Buffer_CleanUp;
 free(palette);
 free(texture);
 free(dolphin);
}