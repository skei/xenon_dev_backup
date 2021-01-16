#include <stdio.h>    // file, sprintf
#include <stdlib.h>    // malloc,ext

#include "defines.h"
#include "types.h"
#include "vars.h"

#include "gfx.h"
#include "render.h"
#include "engine.h"

// 컴컴� Defines 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴

// the size of each polygon. How much 1 pixel in the height map represents
// in world coordinates
#define GridSpacing 200
// Number of vertices in LandObject ; NxN
#define LandObjectSize 25

// 컴컴� Variables 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴

float LandOrigoX,LandOrigoY; // Landscape position in world space
char *LandHeightMap;         // the landscape height map

sObject LandObject;
sGroup LandGroup;
sVertex *LandVertices;
sPolygon *LandPolygons;
sCamera Camera;
float PlayerX,PlayerY;
char palette[768];


// 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
// 께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께
// 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴

void InitLand( sObject *O )
{
 sPolygon *P;
 sGroup *G;
 int x,y,i;

 O->Flags = OBJECT_Active;
 Object_SetPos(O,0,0,0);
 Object_SetRot(O,0,0,0);
 O->BSphereRadius = 0;      // ???
 O->NumVertices = LandObjectSize*LandObjectSize;
 O->Groups = &LandGroup;
 O->Vertices = LandVertices;
 O->Child = NULL;
 O->Branch = NULL;

 G = &LandGroup;
 G->Flags = 0;
 G->NumPolys = (LandObjectSize-1)*(LandObjectSize-1)*2 ;
 G->Polygons = LandPolygons;
 G->Next = NULL;

 P = LandPolygons;
 for (y=0;y<(LandObjectSize-1);y++)
 {
  for (x=0;x<(LandObjectSize-1);x++)
  {
   P->v[0] = &(LandVertices[ y     * LandObjectSize + x     ]);
   P->v[1] = &(LandVertices[ y     * LandObjectSize + (x+1) ]);
   P->v[2] = &(LandVertices[ (y+1) * LandObjectSize + x     ]);
   P++;
   P->v[0] = &(LandVertices[ y     * LandObjectSize + (x+1) ]);
   P->v[1] = &(LandVertices[ (y+1) * LandObjectSize + (x+1) ]);
   P->v[2] = &(LandVertices[ (y+1) * LandObjectSize + x     ]);
   P++;
  }
 }
 LandHeightMap = (char *)malloc(65536);
 for (y=0;y<256;y++)
 {
  for (x=0;x<256;x++)
  {
   i=(rand()%2)*127;
   LandHeightMap[y*256+x]=i;
  }
 }
 for (y=0;y<256;y++)
 {
  for (x=0;x<256;x++)
  {
   i =LandHeightMap[ (y-1) * 256 + (x-1) ];
   i+=LandHeightMap[ (y-1) * 256 + (x  ) ];
   i+=LandHeightMap[ (y-1) * 256 + (x+1) ];
   i+=LandHeightMap[ (y  ) * 256 + (x-1) ];
   i+=LandHeightMap[ (y  ) * 256 + (x  ) ];
   i+=LandHeightMap[ (y  ) * 256 + (x+1) ];
   i+=LandHeightMap[ (y+1) * 256 + (x-1) ];
   i+=LandHeightMap[ (y+1) * 256 + (x  ) ];
   i+=LandHeightMap[ (y+1) * 256 + (x+1) ];
   LandHeightMap   [ y*256+x ] = i;
  }
 }
// memset(LandHeightMap,0,65536);
}

// 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴

void CreateLandObject( sObject *O, float PlayerX, float PlayerY )
{
 sVertex *V;
 float GridX, GridY;
 int x,y;

 V = O->Vertices;

 GridX = (PlayerX - LandOrigoX) / GridSpacing;
 GridY = (PlayerY - LandOrigoY) / GridSpacing;

 GridX -= (LandObjectSize/2);
 GridY -= (LandObjectSize/2);

 for (y=GridY;y<GridY+LandObjectSize;y++)
 {
  for (x=GridX;x<GridX+LandObjectSize;x++)
  {
   V->Pos.x =  LandOrigoX + (x*GridSpacing);
   V->Pos.y =  LandOrigoY + (y*GridSpacing);
   V->Pos.z =  LandHeightMap[(y&0xff)*256+(x&0xff)] >> 1;
   V->u = (256/LandObjectSize)*x;
   if (V->u > 256) V->u-= 256;
   if (V->u < 0) V->u+= 256;
   V->v = (256/LandObjectSize)*y;
   if (V->v > 256) V->v-= 256;
   if (V->v < 0) V->c+= 256;

   V++;
  }
 }
}

// 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
// 께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께
// 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴

void main(void)
{
 FILE *fp;
 int NumVertices;
 int NumPolys;
 char tekst[256];
 int i;

 NumVertices = LandObjectSize*LandObjectSize;
 NumPolys = (LandObjectSize-1)*(LandObjectSize-1)*2 ;

 Engine_Init(NumPolys);
 LandVertices = (sVertex *)malloc(NumVertices*sizeof(sVertex));
 LandPolygons = (sPolygon *)malloc(NumPolys*sizeof(sPolygon));

 InitLand(&LandObject);

// fp = fopen("h256.pix","rb");
// fread(LandHeightMap,1,65536,fp);
// fclose(fp);

 World->Camera = &Camera;
 World->Objects = &LandObject;
 Camera.Flags = 0;

 _Buffer = (char *)malloc(64000);
 _Screen = (char *)0xA0000;
 _Texture = (char *)malloc(65536);

 fp = fopen("tex.pix","rb");
 fread(_Texture,1,65536,fp);
 fclose(fp);

 fp = fopen("tex.pal","rb");
 fread(palette,1,768,fp);
 fclose(fp);

 ScreenWidth = 320;
 ScreenHeight = 200;
 MinX = 0; MaxX = 319;
 MinY = 0; MaxY = 199;

 viewdistance = PROJ_VALUE;
 halfscreenwidth_vd = (ScreenWidth/2)/viewdistance;
 halfscreenheight_vd = (ScreenHeight/2)/viewdistance;
 minclip_z = 10;
 maxclip_z = 4000;

 SetMode(0x13);
 for (i=0;i<256;i++) setrgb(i,palette[i*3],palette[i*3+1],palette[i*3+2]);

 Object_SetPos(&LandObject,0,0,0);
 Camera_SetPos(&Camera,0,0,-200);
 Camera_SetRot(&Camera,-1.5,0,0);

 while (!kbhit())
 {
  PlayerX = Camera.Pos.x;
  PlayerY = Camera.Pos.y;
  memset(_Buffer,0,64000);
  CreateLandObject(&LandObject,PlayerX,PlayerY);
  Render();
  waitvr();
  memcpy(_Screen,_Buffer,64000);
  Camera_AddRot(&Camera,0,0,0.01);
//  Camera_AddPos(&Camera,10,5,0);
 }

 getch();
 SetMode(0x3);
 free(_Texture);
 free(_Buffer);
 Engine_Exit();

}
