#include <stdio.h>    // file stuf
#include <stdlib.h>   // exit
#include <string.h>   // memcpy
#include <time.h>     // clock

#include <math.h>

#include "engine.h"
#include "vbe2.h"

// --------------------------------------------------------------------------

sObject *oo,*oo2;

char tekst[256];

char palette[768];
char *Tex1,*Tex2;

int CurrentFrame;
int quit;
char c;

FILE *fp;

int timestart;
float fps;
int frames_rendered;

// --------------------------------------------------------------------------

 struct
 {
  unsigned short chunk_id;
  unsigned int next_chunk;
 } CHUNK;

 // --------------------

sObject *Load_3DS( char *filename, float VertexMult, float UVmult,
                   int center_obj, int load_textures)
 {
  FILE *fp;
  long filelen;

  unsigned short int dw;
  unsigned int dd;
  float fl;

  unsigned int nextobj;
  short int parent;

  char objname[20];
  char material[20];
  unsigned short vertices,polys,num;
  float x,y,z,angle;
  unsigned short int facea,faceb,facec,faceflags;
  float ma,mb,mc,md,me,mf,mg,mh,mi,mj,mk,ml;
  float m1[16],m2[16];
  sVector tempvector;
  // light
  float lightx,lighty,lightz;
  // camera
  float camx,camy,camz;
  float tarx,tary,tarz;
  float bank,lens;

  float u,v;

  char c;
  int i,j;

  unsigned short int nf,flags;
  float temp;

  sObject *returnO;
  sObject *O;
  sObject *tempO;
  sKeyFrame *KF;

  sObject *Currenobject;
  sCamera *CurrentCamera;
  sLight  *CurrentLight;

 // --------------------

  tempO = (sObject *)malloc(sizeof(sObject));

  returnO = NULL;
  fp = fopen(filename,"rb");
  filelen = filelength(fileno(fp));
  fread (&CHUNK,6,1,fp);
  if (CHUNK.chunk_id != 0x4d4d)
  {
   // printf("Invalid 3DS file!!!\n");
   fclose(fp);
   return NULL;
  }
  else
  {
   while (!feof(fp) && (ftell(fp) < filelen))
   {
    fread (&CHUNK,6,1,fp);
    switch (CHUNK.chunk_id)
    {
 // --------------------
     case 0x3d3d : // mesh;
                   break;
     case 0x4000 : // Found object
                   // printf("0x4000 CHUNK_OBJBLOCK\n");
                   nextobj = ftell(fp) + CHUNK.next_chunk - 6;

                   // allocate memory for object

                   // O = (sObject *)malloc(sizeof(sObject));
                   if (returnO == NULL)
                   {
                    O = (sObject *)malloc(sizeof(sObject));
                    returnO = O;
                   }
                   else
                   {
                    O->Next = (sObject *)malloc(sizeof(sObject));
                    O->Next->Prev = O;
                    O = O->Next;
                   }

                   // set some default values

                   O->Flags = OBJECT_active;
                   Object_SetPos(O,0,0,0);
                   Object_SetRot(O,0,0,0);
                   O->Next = NULL;
                   O->Parent = NULL;
                   O->NumVertices = 0;
                   O->NumPolys = 0;
                   O->HierarchyNumber = 0x7fffffff;
                   O->ParentNumber    = 0x7ffffffe;
                   O->KeyFramerPos.NumSegments = 0;
                   O->KeyFramerRot.NumSegments = 0;
                   // O->VUElist = (sMatrix *)malloc(MaxVUEFrames*sizeof(sMatrix));

                   // read object name, skip spaces...

                   j = 0;
                   do
                   {
                    c = fgetc(fp);
                    if (c!=' ')
                    {
                     O->ObjectName[j] = c;
                     j++;
                    }
                   } while ((O->ObjectName[j-1] != 0) && (j<20));
                   // printf("Object mesh: %s\n",O->ObjectName);
                   break;
     case 0x4100 : // Triangular polygon object
                   // printf("0x4100 CHUNK_TRIMESH\n");
                   break;
     case 0x4110 : // vertices
                   // printf("0x4110 CHUNK_VERTLIST\n");
                   fread(&vertices,2,1,fp);
                   O->NumVertices = vertices;
                   O->Vertices = (sVertex *)malloc(vertices*sizeof(sVertex));
                   for (i=0;i<vertices;i++)
                   {
                    fread(&x,4,1,fp);
                    fread(&z,4,1,fp);
                    fread(&y,4,1,fp);
                    O->Vertices[i].Pos.x = x * VertexMult;
                    O->Vertices[i].Pos.y = y * VertexMult;
                    O->Vertices[i].Pos.z = z * VertexMult;
                    O->Vertices[i].u = 0;
                    O->Vertices[i].v = 0;
                    O->Vertices[i].c = 63;
                   }
                   break;
     case 0x4120 : // polygons
                   // printf("0x4120 CHUNK_FACELIST\n");
                   fread(&polys,2,1,fp);
                   O->NumPolys = polys;
                   O->Polygons = (sPolygon *)malloc(polys*sizeof(sPolygon));
                   for (i=0;i<polys;i++)
                   {
                    fread(&facea,2,1,fp);
                    fread(&faceb,2,1,fp);
                    fread(&facec,2,1,fp);
                    fread(&faceflags,2,1,fp);
                    // faceflags: 04,02,01 = ab,bc,ca
                    //            08,10 = uwrap,vwrap
                    O->Polygons[i].v[0] = &O->Vertices[facea];
                    O->Polygons[i].v[1] = &O->Vertices[faceb];
                    O->Polygons[i].v[2] = &O->Vertices[facec];
                    O->Polygons[i].Object = O;
                   }
                   break;
//     case 0x4130 : // material.. Hacked.. All polys get same material.
                   // material name = texture file...
//                   i = ftell(fp);
//                   printf("0x4130 CHUNK_FACEMAT\n");
//                   j = 0;
//                   do
//                   {
//                    c = fgetc(fp);
//                    if (c != ' ')
//                    {
//                     O->Texture1FileName[j] = c;
//                     j++;
//                    }
//                   } while ((O->Texture1FileName[j-1] != 0) & (j<20));
//                   printf("Material: %s\n",O->Texture1FileName);
//                   fseek(fp,i, SEEK_SET);
//                   break;
     case 0x4140 : // U/V mapping
                   // printf("0x4140 CHUNK_MAPLIST\n");
                   O->Flags |= OBJECT_mapped;
                   fread(&num,2,1,fp);
                   for (i=0;i<num;i++)
                   {
                    fread(&u,4,1,fp);
                    fread(&v,4,1,fp);
                    O->Vertices[i].u = u * UVmult;
                    O->Vertices[i].v = v * UVmult;
                   }
                   break;


     case 0x4160 : // matrix
                   // printf("0x4160 CHUNK_TRMATRIX\n");
                   fread(&ma,4,1,fp);
                   fread(&mb,4,1,fp);
                   fread(&mc,4,1,fp);
                   fread(&md,4,1,fp);
                   fread(&me,4,1,fp);
                   fread(&mf,4,1,fp);
                   fread(&mg,4,1,fp);
                   fread(&mh,4,1,fp);
                   fread(&mi,4,1,fp);
                   fread(&mj,4,1,fp);
                   fread(&mk,4,1,fp);
                   fread(&ml,4,1,fp);

//        Ú¿
// abc.  acb.  acb.    agdj
// def.  dfe. Úgih. Ä¿ cifl
// ghi.  gih. Àdfe.  | bhek
// jkl.  jlk.  jlk.    ....

                   m1[ 0] =  ma;
                   m1[ 1] =  mg;
                   m1[ 2] =  md;
                   m1[ 3] =  mj;

                   m1[ 4] =  mc;
                   m1[ 5] =  mi;
                   m1[ 6] =  mf;
                   m1[ 7] =  ml;

                   m1[ 8] =  mb;
                   m1[ 9] =  mh;
                   m1[10] =  me;
                   m1[11] =  mk;

                   m1[12] =  0;
                   m1[13] =  0;
                   m1[14] =  0;
                   m1[15] =  1;

                   // the vertices are already processed by the object's
                   // matrix, so we apply the inverse matrix to get'em back
/*
                   Matrix_Inverse(&m2,&m1);
                   for (i=0;i<O->NumVertices;i++)
                   {
                    Vector_Trans(&tempvector,O->Vertices[i].Pos, &m2);
                    O->Vertices[i].Pos = tempvector;
                   }
                   Vector_Trans(&O->Pos,O->Vertices[i].Pos, &m2);
*/
                   break;
     case 0x4600 : // Light
                   // printf("0x4600 CHUNK_LIGHT\n");
                   fread(&lightx,4,1,fp);
                   fread(&lightz,4,1,fp);
                   fread(&lighty,4,1,fp);
                   break;
     case 0x4700 : // Camera;
                   // printf("0x4700 CHUNK_CAMERA\n");
                   fread(&camx,4,1,fp);
                   fread(&camz,4,1,fp);
                   fread(&camy,4,1,fp);
                   fread(&tarx,4,1,fp);
                   fread(&tarz,4,1,fp);
                   fread(&tary,4,1,fp);
                   fread(&bank,4,1,fp);
                   fread(&lens,4,1,fp);
                   World->Camera->Pos.x = camx;
                   World->Camera->Pos.y = camy;
                   World->Camera->Pos.z = camz;
                   World->Camera->Target.x = tarx;
                   World->Camera->Target.y = tary;
                   World->Camera->Target.z = tarz;
                   World->Camera->Roll = bank;
                   World->Camera->FOV = lens;
                   break;
     case 0xb000 : // Keyframer data
                   break;
     case 0xb002 : // Object framedata
                   break;
//     case 0xb003 : // Camera track (pos)
//                   break;
//     case 0xb004 : // Camera target
//                   break;
     case 0xb010 : // object name
                   j = 0;
                   do
                   {
                    c = fgetc(fp);
                    if (c!=' ')
                    {
                     tempO->ObjectName[j] = c;
                     j++;
                    }
                   } while ((tempO->ObjectName[j-1] != 0) && (j<20));
                   // printf("Track info name: %s\n",tempO->ObjectName);
                   O = Object_FindByName(returnO,tempO->ObjectName);
                   // printf("Found object in hierarchy: %s\n",O->ObjectName);

                   fread(&dw,2,1,fp);
                   fread(&dw,2,1,fp);
                   fread(&parent,2,1,fp);
                   O->ParentNumber = parent;
                   // printf("Parent = %i\n",parent);
                     // asciiz name
                     // int flags?, int (2 ints)
                     // hierarchy number of object
                   break;
//     case 0xb020 : // translation key
//                   break;
     case 0xb021 : // rotation key
                   fread(&dw,2,1,fp);
                   fread(&dw,2,1,fp);
                   fread(&dw,2,1,fp);
                   fread(&dw,2,1,fp);
                   fread(&dw,2,1,fp);
                   fread(&num,2,1,fp);    // number of keys
                   fread(&dw,2,1,fp);
                   O->KeyFramerRot.CurrentPos = 0;
                   O->KeyFramerRot.Speed = 1;
                   O->KeyFramerRot.CurrentSegment = 0;
                   O->KeyFramerRot.NumSegments = num;
                   O->KeyFramerRot.BendSize = 1;
                   O->KeyFramerRot.SplineSegments =
                        (sKeySegment *)malloc(num*sizeof(sKeySegment));
                   for (i=0;i<num;i++)
                   {
                    fread(&dw,2,1,fp);
                    O->KeyFramerRot.SplineSegments[i].TimeStamp = dw;
                    fread(&dw,2,1,fp);
                    fread(&dw,2,1,fp);  // flags
                    // ... spline ... (see below)
                    fread(&angle,4,1,fp);
                    fread(&x,4,1,fp);
                    fread(&z,4,1,fp);
                    fread(&y,4,1,fp);
                    O->KeyFramerRot.SplineSegments[i].PosQ.w = angle; // degrees = angle*180/PI
                    O->KeyFramerRot.SplineSegments[i].PosQ.x = x; // degrees = angle*180/PI
                    O->KeyFramerRot.SplineSegments[i].PosQ.y = y; // degrees = angle*180/PI
                    O->KeyFramerRot.SplineSegments[i].PosQ.z = z; // degrees = angle*180/PI
                   }
                   break;
//                   break;
//     case 0xb022 : // scale key
//                   break;
     case 0xb030 : // object number
                   fread(&num,2,1,fp);
                   O->HierarchyNumber = num;      // HACK HACK HACK
                   // printf("object : %s hierarchy number : %i \n",O->ObjectName,num);
                   break;
         default : // printf("%x: Skipped\n",CHUNK.chunk_id);
                   fseek(fp,CHUNK.next_chunk-6, SEEK_CUR);
                   // break;
 // --------------------
    }
    if (CHUNK.chunk_id == 0xb001) break;
   }
   fclose(fp);
   free(tempO);
  }
  // recorse down from returnO...
  O = returnO;
  while (O!=NULL)
  {
   // printf("Object: %s : ",O->ObjectName);
   if (load_textures == TRUE)
   {
    O->Texture1 = (char *)malloc(65536);
    fp = fopen(O->Texture1FileName,"rb");
    fread(O->Texture1,1,65536,fp);
    fclose(fp);
   }
   Object_CalcFaceNormals(O);
   Object_CalcVertexNormals(O);
   if (center_obj == TRUE) Object_Center(O);
   Object_CalcBoundingSphere(O);
   // if object not mapped, apply fake spherical mapping :-/
   if ((O->Flags & OBJECT_mapped) == 0)
   {
    for (i=0;i<O->NumVertices;i++)
    {
     O->Vertices[i].u = O->Vertices[i].Normal.x * (UVmult/2) + (UVmult/2);
     O->Vertices[i].v = O->Vertices[i].Normal.y * (UVmult/2) + (UVmult/2);
    }
   }
   // find parent in Hierarchy
   if (O->ParentNumber != -1)
   {
    tempO = returnO;
    while (tempO!=NULL)
    {
     if (tempO->HierarchyNumber == (O->ParentNumber)+1)
     {
      // printf(" parent: %s",tempO->ObjectName);
      O->Parent = tempO;
      break;
     }
     tempO = tempO->Next;
    }
   }
   // printf("\n");
   O = O->Next;
  }
  return returnO;
 }

// --------------------------------------------------------------------------

 // Renders the entire scene
void R_Setup(void)
 {
  int i;
  int frame;
  float mm[16];
 // int cc;
  sObject *O;
  sCamera *C;

  C = World->Camera;
  frame = World->FrameNum;
  World->VisiblePolys = 0;
  World->VisibleObjects = 0;

  Camera_CreateMatrix_Aim( C );

  #define aFov (2400/C->FOV)
  C->viewdistance = 160 / ( sin((aFov/360)*PI) / cos((aFov/360)*PI));

  O = World->Objects;
  while (O!=NULL)
  {
   // Create object matrix
   // Object_CreateMatrix_VUE(O,World->Camera,World->FrameNum);
   if (O->Parent != NULL)
   {
    Object_CreateMatrix_Inherit(O,World->Camera,O->Parent);
   }
   else
   {
    Object_CreateMatrix(O,World->Camera);
   }
   O = O->Next;
  }
  Handle_Object( World->Objects, World->Camera/*, World*/ );
  SortWorldObject( World );
 }

 // -------------------------------------------------------------------------

void R_Draw(void)
 {
  int i;

  for (i=World->VisiblePolys-1 ; i>=0 ; i-- )
  {
   GTexPoly(World->RenderList[i]);
  }
 }

// --------------------------------------------------------------------------

void main(void)
 {
  int i;
  // --- init engine ---

  if (Engine_Init(320,200,15,TRUE,5000,0) == FALSE)
  {
   printf("Error initializing VBE2 - 320x200x15\n");
   exit(1);
  }

  // --- set some world variables ---

  printf("Setting some def. variables\n");

  Camera->Flags = CAMERA_aim;
  Camera_SetRot(Camera,0,0,0);
  Camera_SetPos(Camera,0,0,-800);
  Camera_SetTarget(Camera,0,0,0);
  Camera->Roll = 0;
  Camera->FOV = 65;
  Camera->UpVector.x = 0;
  Camera->UpVector.y = 1;
  Camera->UpVector.z = 0;

  World->Camera = Camera;
  World->Flags = 0;
  World->NumLights = 0;
  World->Lights = NULL;
  World->AmbientLight = 20.0/63.0;

  // --- load 3ds ---

  printf("Loading 3DS\n");

  // filename
  // VertexMult
  // UVmult
  // center_obj (bool)
  // load_textures (bool)
  World->Objects = Load_3DS("birdshow.3ds",1,250,FALSE,FALSE);

  // --- load texture

  printf("Loading texture\n");
  Tex1 = (char *)malloc(65536);
  fp = fopen("a9.pix","rb");
  fread(Tex1,1,65536,fp);
  fclose(fp);
  fp = fopen("a9.pal","rb");
  fread(palette,1,768,fp);
  fclose(fp);

  // -----

  // "recurse" down to set some default variables
  oo = World->Objects;
  while ( oo!= NULL)
  {
   oo->Texture1 = Tex1;
   oo->Flags |= OBJECT_envmap;
//   Object_SetPos(oo,0,0,0);
   oo = oo->Next;
  }

  VesaCalc_ShadeTable(palette, _VesaShadeTable);
  Setup_Intensity(10);
  World->FrameNum = 0;

  // --- debug ---

//  Camera->Flags = CAMERA_aim;
//  Camera_SetRot(Camera,0,0,0);
//  Camera_SetPos(Camera,0,0,-400);
//  Camera_SetTarget(Camera,0,0,0);
//  Camera->Roll = 0;
//  Camera->FOV = 65;

  oo = World->Objects;
  while ( oo!= NULL)
  {
   printf("-----------------------------------------------------------------------------\n");
   printf("Object: %s\n",oo->ObjectName);
   printf("HierarchyNumber: %i  ParentNumber: %i\n",oo->HierarchyNumber,oo->ParentNumber);
   if (oo->Parent == NULL) printf("No parent\n");
   else printf("Parent: %s\n",Object_FindByName(World->Objects,oo->Parent->ObjectName)->ObjectName);
   printf("Rot KeyFrames: %i\n",oo->KeyFramerRot.NumSegments);
   for (i=0;i<oo->KeyFramerRot.NumSegments;i++)
   {
    printf("%i : Quat: x:%5.1f, y:%5.1f, z:%5.1f, angle:%5.1f\n",
            oo->KeyFramerRot.SplineSegments[i].TimeStamp,
            oo->KeyFramerRot.SplineSegments[i].PosQ.x,
            oo->KeyFramerRot.SplineSegments[i].PosQ.y,
            oo->KeyFramerRot.SplineSegments[i].PosQ.z,
            oo->KeyFramerRot.SplineSegments[i].PosQ.w);
   }
   oo = oo->Next;
  }
  getch();
  printf("-----------------------------------------------------------------------------\n");

  // ------------------------------
  // Start the rendering loop
  // ------------------------------

  Set_Vesa(VesaMode);

  frames_rendered = 0;
  timestart = clock();
  CurrentFrame = 0;

  while (!kbhit())
  {
   memset(_VesaBuffer,0,64000*2);

//   oo = Object_FindByName(World->Objects,"body");
//   Object_AddRot(oo,0,0,0.03);
//   Camera_AddPos(World->Camera,0,1,0);
//   Camera->Roll += 0.005;

//   oo = World->Objects;
//   while ( oo!= NULL)
//   {
//    Spline_GetValuesQ( &O1->Pos, &O1->KeyFramerPos );
//    Spline_AddQ( &O1->KeyFramerPos );
//    oo = oo->Next;
//   }

   R_Setup();
   R_Draw();

   sprintf(tekst,"FRAME %i",World->FrameNum);
   drawstring(0, 0,tekst,0x3fff,_VesaBuffer);

   frames_rendered++;
   fps = (float)(frames_rendered * CLOCKS_PER_SEC) / (clock() - timestart);
   sprintf(tekst,"FPS %5.1f",fps);
   drawstring(0, 8,tekst,0x1f,_VesaBuffer);

   memset(_VesaBuffer+(320*198),0xff,World->VisibleObjects*2);
   World->FrameNum++;
   if (World->FrameNum > MaxVUEFrames) World->FrameNum = 0;
   waitvr();
   memcpy (_VesaScreen,_VesaBuffer,64000*2);
  }
  Set_Vesa(3);
  SetMode(3);
  Engine_UnInit(TRUE);
 }

