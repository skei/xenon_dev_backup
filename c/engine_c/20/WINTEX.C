// WinTexture3D : Tor-Helge Skei
// Time to rename it??? :-/

// Working on:
// - Bounding boxes/volumes..
// - Lookat function for camera... (bugfixing)
// - Gouraud shaded texture mapping (for fog/haze/etc.)
// - Better animation (re-calc normals, etc...)
// - Moving lightsources

// Structurize everything A LOT!!! Things are getting messy!
// C/H files:
//   - Defines, types & vars
//   - Math3D   - matrix, vector, quaternion, etc...
//   - Sort     - Bytesort
//   - Interpol - interpolation (linear, spline)
//   - Render   - texture fillers, screen blanking, copying
//   - Engine   - the 3D engine API

#pragma warning (disable: 4244)	// Get rid of the "double to float" conversion warning

#include <math.h>
#include <string.h>

#include <windows.h>
#include <windowsx.h>

#include "defines.h"			 // all #define's
#include "vars.h"					 // global variables.... ugh! :-/
#include "types.h"				 // mainly structures
#include "WinTexture3D.h"	 // main_init, etc... prototypes
#include "matrix.h"				 // matrix functions
#include "quat.h"					 // quaternion functions (not working yet)
#include "texmap.h"				 // the texture mapping routines
#include "bytesort.h"			 // sorting
#include "engine.h"				 // object/camera support routines
#include "spline.h"

// ----------------
// Global variables
// ----------------

extern struct screen_S buffer;
extern BITMAPINFO BitmapInfo;

extern struct objstruc face;
extern struct objstruc sphere;

extern struct objstruc dolph1;
extern struct objstruc dolph2;
extern struct objstruc dolph3;

extern unsigned char PhongMap[];
extern unsigned char PhongPal[];

// extern unsigned gouraudmap[];

// // some test quaternions
// int q[4];
// int q1[4];
// int q2[4];

// additional objects for testing purposees
// struct objstruc *oo[10];

// camera splne data...

spline camera_path;
int camloc;
coord campos;

spline object_spline[MAXCOORDS];
int objloc;
coord objpos;

struct objstruc *anim;


/*****************/
/* Windows stuff */
/*****************/

// Initialize & allocate buffer, init BitMapInfo
BOOL init_buffer(int x_res, int y_res)
{
 x_res = (x_res/2)*2;	// To fix modulo bug
 buffer.screen = (unsigned short *)malloc(x_res*y_res*2);
 bufferpointer = (int)buffer.screen;
 paaaaa = (int *)buffer.screen;
 paaaaa--;
 bufferpointersize = *paaaaa;
 if(buffer.screen != NULL)
 {
  buffer.width = x_res;
  buffer.height = y_res;

  BitmapInfo.bmiHeader.biSize          = sizeof(BitmapInfo.bmiHeader);	// Size of struct
  BitmapInfo.bmiHeader.biWidth         = buffer.width;					// Width of bitmap
  BitmapInfo.bmiHeader.biHeight        = -buffer.height;				// Height of bitmap. Negative means top-down
  BitmapInfo.bmiHeader.biPlanes        = 1;								// Must be 1
  BitmapInfo.bmiHeader.biBitCount      = 16;							// Number of bits per pixel
  BitmapInfo.bmiHeader.biCompression   = BI_RGB;						// Uncompressed
  BitmapInfo.bmiHeader.biSizeImage     = 0;								// 0 for BI_RGB bitmaps
  BitmapInfo.bmiHeader.biXPelsPerMeter = 2000;	// ????					// Pixels per meter	horizontal
  BitmapInfo.bmiHeader.biYPelsPerMeter = 1000;	// ????					// Pixels per meter vertical
  BitmapInfo.bmiHeader.biClrUsed       = 0;								// Number of colors used in bitmap
  BitmapInfo.bmiHeader.biClrImportant  = 0;								// Important colors
  return TRUE;
 }
 else
 {
  MessageBox(NULL, "ERROR: Unable to allocate memory for viewport!", "ERROR", MB_OK);
  return FALSE;
 }
}

// Free buffer & deallocate memory

void cleanup_buffer()
{
 free(buffer.screen);		 /* Her.... Exception: Access violation */
 buffer.screen=NULL;
 buffer.width=0;
 buffer.height=0;
}

// -----

WORD gethicolor(char a,char b,char c)
{
 WORD r;
 r = (WORD)(((a&0xf8)<<7) + ((b&0xf8)<<2) + (c>>3));
 return r;
}

// -----

void CopyVertexNormals(struct objstruc *o)
{
 int i;
 int *from = o->tex_c;
 for(i=0;i<o->numcoords;i++)
 {
	texcoords[i*2]   = from[i*2];
  texcoords[i*2+1] = from[i*2+1];
 }
}

// ----------------------
// project vertex normals
// ----------------------
void ProjectVertexNormals (struct objstruc *o)
{
 int i;
 int xt,yt;
 for(i=0;i<o->numcoords;i++)
 {
  xt =  rotatedvertexnormals[i*3];
	yt =  rotatedvertexnormals[i*3+1];
  texcoords[i*2]   = xt+lightx;
  texcoords[i*2+1] = yt+lighty;
 }
}


// ----------------------
// project vertex normals to colors
// ----------------------
void ProjectColors (struct objstruc *o)
{
 int i;
 int xt,yt;
 for(i=0;i<o->numcoords;i++)
 {
  xt =  rotatedvertexnormals[i*3];
	yt =  rotatedvertexnormals[i*3+1];
  colorcoords[i] = PhongMap[(((xt+lightx)<<8) + (yt+lighty)&0xff00)] >> 2;
 }
}


// --------------------------------------------------
// project coordinates 3D (rotated) -> 2D (tricoords)
// --------------------------------------------------
void ProjectCoords (struct objstruc *o)
{
 int i;
 // int *from = &rotated[0];
 int xt,yt,zt;
 for(i=0;i<o->numcoords;i++)
 {
  xt =  rotated[i*3];
	yt =  rotated[i*3+1];
	zt =  rotated[i*3+2];
  if (zt <= 0)
	{
   zcoords[i] = -1;
	}
  else
	{
   zcoords[i] = zt;
	 zt += DISTANCE;
   tricoords[i*2]   = ((xt << PROJ_PARAM) / (zt)) + 128;
   tricoords[i*2+1] = ((yt << PROJ_PARAM) / (zt)) + 128;
  }
 }
}

// -----

void DrawTextureObject(struct objstruc *o,int nv, int *table, int *tri, int *tex)
{
 struct screen_S * vp = &buffer;
 int i;
 unsigned int polynum;
 int c1,c2,c3;
// for (i=0;i<nv;i++)
 for (i=nv-1;i>=0;i--)
 {
  polynum = ((unsigned int)(table[i]) >> 16/*.data*/);         // polynum = # of polygon to draw
  c1 = o->polys[polynum*3];
  c2 = o->polys[polynum*3+1];
  c3 = o->polys[polynum*3+2];
  TexMap(vp,c1,c2,c3,tri,tex,PhongMap);
 }
}

void DrawGouraudTextureObject(struct objstruc *o,int nv, int *table, int *tri, int *tex, char *col)
{
 struct screen_S * vp = &buffer;
 int i;
 unsigned int polynum;
 int c1,c2,c3;
// for (i=0;i<nv;i++)
 for (i=nv-1;i>=0;i--)
 {
  polynum = ((unsigned int)(table[i]) >> 16/*.data*/);         // polynum = # of polygon to draw
  c1 = o->polys[polynum*3];
  c2 = o->polys[polynum*3+1];
  c3 = o->polys[polynum*3+2];
  GTexMap(vp,c1,c2,c3,tri,tex,col,PhongMap);
 }
}


// -----------------------------------------------
// "Engine" stuff
// -----------------------------------------------

void project_object(struct objstruc *o, struct camstruc *c)
{
 init_rotation(o->m,o->xang,o->yang,o->zang);
 copy_matrix(o->e,o->m);
 o->e[3*4+0] += o->xpos;
 o->e[3*4+1] += o->ypos;
 o->e[3*4+2] += o->zpos;
 mult_matrix(o->e,c->m);
}

// -----

void draw_object(struct objstruc *o)
{
 process_matrix(o->e,o->coords,rotated,o->numcoords);
 ProjectCoords(o);
 switch(o->rottype)
 {
	case 0 : process_matrix(o->m,o->vertex_n,rotatedvertexnormals,o->numcoords);
           ProjectVertexNormals(o);
					 break;
	case 1 : //CopyVertexNormals(o);
					 break;
	case 2 : process_matrix(o->m,o->vertex_n,rotatedvertexnormals,o->numcoords);
	         ProjectColors(o);
					 break;
 }
 numvisiblepolys = polysortsetup(o,polysort);
 bytesort(numvisiblepolys,polysort);
 switch(o->shadetype)
 {
	case 0 : DrawTextureObject(o,numvisiblepolys,polysort,tricoords,texcoords);
	         break;
	case 1 : DrawTextureObject(o,numvisiblepolys,polysort,tricoords,o->tex_c);
	         break;
	case 2 : DrawGouraudTextureObject(o,numvisiblepolys,polysort,tricoords,texcoords,colorcoords);
	         break;
 }
}

// -----

void process_camera(struct camstruc *c)
{
 float mm[16];
 // init_translation(c->m,-(c->xpos),-(c->ypos),-(c->zpos));
 // create_camera_matrix(c,c->m);
 // init_rotation(mm,-(c->xang),-(c->yang),-(c->zang));
 // mult_matrix(c->m,mm);
 Camera_set_target(c,0,500,1000);
 camera_create_lookat(c,c->m/*mm*/);

}

// -----

void check_BoundingBox(struct objstruc *o,struct camstruc *c)
{
 int i;
 int x,y,z;
 int b[8*3];
 int numvis;

 project_object(o,c);		 // init matrices
 process_matrix(o->e,o->bbox,b,8);
 numvis = 8;
 for (i=0;i<8;i++)
 {
	z = b[i*3+1];
  if (z <= 0)
	{
	 z = 1;
	}
	else
	{
   z += DISTANCE;
	}
  x = ((b[i*3+0] << PROJ_PARAM) / (z)) + 128;
  y = ((b[i*3+1] << PROJ_PARAM) / (z)) + 128;
	if ( ( (unsigned int)x > 319 ) &&
       ( (unsigned int)y > 199 ) &&
			 ( (unsigned int)z > 1000)) numvis--;
 }
 if (numvis != 0)	draw_object(o);
}

// -----

void Init_animation(struct objstruc *o1,struct objstruc *o2,struct objstruc *o3)
{
 int i;
 for (i=0;i<o1->numcoords;i++)
 {
	object_spline[i].p[0].x = o1->coords[i*3];
	object_spline[i].p[0].y = o1->coords[i*3+1];
	object_spline[i].p[0].z = o1->coords[i*3+2];
	object_spline[i].p[1].x = o2->coords[i*3];
	object_spline[i].p[1].y = o2->coords[i*3+1];
	object_spline[i].p[1].z = o2->coords[i*3+2];
	object_spline[i].p[2].x = o3->coords[i*3];
	object_spline[i].p[2].y = o3->coords[i*3+1];
	object_spline[i].p[2].z = o3->coords[i*3+2];
  object_spline[i].num = 3;
  object_spline[i].oClosed = 1;
  object_spline[i].bendsize = 1;
 }
}

void Update_animation(int objloc)
{
 int i;
 coord objpos;
 objloc&=16383;
 for (i=0;i<anim->numcoords;i++)
 {
	objpos = get_spline_point(objloc, &object_spline[i]);
  anim->coords[i*3]   = objpos.x;
  anim->coords[i*3+1] = objpos.y;
  anim->coords[i*3+2] = objpos.z;
	// Re-calculate Face & Vertex normals here :-/
 }
}



// --------------------------------------------------------
// Calculates tables
// --------------------------------------------------------

void CalcTables(void)
{
 int i,j;
 int color,shade;
 int r,g,b;
 float realangle = -(float)PI;

 for (i=0;i<768;i++) PhongPal[i] <<=2;
 for (i=0;i<256;i++) colortable[i] = gethicolor(PhongPal  [i*3],PhongPal  [i*3+1],PhongPal  [i*3+2]);


 for (i = 0; i < TABLESIZE; i++)
 {
  sine[i] =  sin(realangle);
  cosine[i] = cos(realangle);
  realangle = realangle + (float)(((PI*2)/TABLESIZE));
 }

 for (shade=0;shade<64;shade++)
 {
	for (color=0;color<256;color++)
	{
	 r = (PhongPal[color*3  ]*shade)/64;
	 g = (PhongPal[color*3+1]*shade)/64;
	 b = (PhongPal[color*3+2]*shade)/64;
	 ShadeMap[shade*256+color] = gethicolor(r,g,b);
	}
 }

}

// -----

void ClearBuffer(char c)
{
 struct screen_S * vp = &buffer;
 memset(vp->screen,c,(SCREEN_WIDTH*SCREEN_HEIGHT*2));
}

/*********************/
/* Main_*** rutinene */
/*********************/

// -----

void main_init(void)
{
 int i;
 init_buffer(SCREEN_WIDTH,SCREEN_HEIGHT);
 CalcTables();
 lightx = 64;  lighty = 64;

 cam.Bank = 0;
 cam.upx = 0;
 cam.upy = -1000;
 cam.upz = 0;

 Object_set_rot(&face,3072,2048,2048);
 Object_set_pos(&face,0,0,800);
 Object_set_mapping(&face,0);

 Object_set_rot(&sphere,0,0,0);
 Object_set_pos(&sphere,0,0,0);
 Object_set_mapping(&sphere,0);

 Camera_set_rot(&cam,0,0,0);
 Camera_set_pos(&cam,0,0,0);

// Init camera path spline

 camera_path.num = 4;
 camera_path.oClosed = 1;
 camera_path.bendsize = 1;

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

 anim = Object_copy(&dolph1);
 Object_set_rot(anim,3072,2048,2048);
 Object_set_pos(anim,0,500,1000);
 Object_set_mapping(anim,0);

 Init_animation(&dolph1,&dolph2,&dolph3);

}

// -----

void main_process(void)
{
 camloc+=50;
 camloc&=16383;
 campos = get_spline_point(camloc, &camera_path);
 Camera_set_pos(&cam,campos.x,campos.y,campos.z);
// Camera_set_pos(&cam,0,0,-500);
// Camera_add_rot(&cam,10,20,5);

 ClearBuffer(0);
 process_camera(&cam);

 project_object(&sphere,&cam);
 draw_object(&sphere);

 objloc+=400;
 Update_animation(objloc);

 project_object(anim,&cam);
 draw_object(anim);

 project_object(&dolph1,&cam);
 draw_object(&dolph1);

// check_BoundingBox(&face,&cam);
 project_object(&face,&cam);
 draw_object(&face);

}

// ----

void main_exit(void)
{
 cleanup_buffer();
}

/*
 KEYS:

 z/x move camera left/right
 q/a move camera up/down
 w/s move camera in/out (along Z)

 Z/X move object left/right
 Q/A move object up/down
 W/S move object in/out (along Z)

 4/6 rotate camera left/right
 8/2 rotate camera up/down
 7/1 rotate camera around Z
*/
