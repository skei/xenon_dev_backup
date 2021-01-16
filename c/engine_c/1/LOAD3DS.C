 /*

  Chunks that are currently ignored, but should be included:

  * MASTER_SCALE 0x0100     Scaling of 3D values in 3DS file
  * AMBIENT_LIGHT 0x2100    Ambient light
  ş BIT_MAP 0x1100          Background bitmap
  ş SOLID_BGND 0x1200
  ş V_GRADIENT 0x1300
  ş USE_BIT_MAP 0x1101
  ş USE_SOLID_BGND 0x1201
  ş USE_V_GRADIENT 0x1301
  ş FOG 0x2200
  ş USE_FOG 0x2201

  ---

  FaceMaterials - Assign a default material to all faces in Material_New ???

 */

 #include <stdio.h>   // filestuff, printf - fseek,ftell
 #include <stdlib.h>  // malloc
 #include <string.h>  // strncpy
 #include <math.h>    // not used here?????????
 #include <conio.h>   // getch

 #include "engine.h"
 #include "math3d.h"
 #include "spline2.h"

 #define UVmultiplier 255;

// ÄÄÄ types ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

 #pragma pack(push);
 #pragma pack(1);

typedef struct sChunkHeader
 {
  unsigned short int Id;
  unsigned int       Size;
 } sChunkHeader;

 #pragma pack(pop);

// ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

int Scene_Load3DS(sScene *Scene, char *file)
 {
  FILE *fp;
  char *buffer;
  unsigned int filesize;
  int res;

  DEBUG(printf("ş Loading 3DS file: %s\n",file);)
  fp = fopen(file,"rb");
  fseek(fp,0,SEEK_END);
  filesize = ftell(fp);
  fseek(fp,0,SEEK_SET);
  DEBUG(printf("ş Allocating %i bytes (%s filesize)\n",filesize,file););
  buffer = (char *)malloc(filesize);
  fread(buffer,1,filesize,fp);
  fclose(fp);
  res = Scene_Parse3DS(Scene, buffer, &(buffer[filesize]));
  DEBUG(printf("ş Freeing temp. memory block\n"););
  free(buffer);
  return res;
 }

// ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

char *Read_ChunkHeader(sChunkHeader *Chunk,char *B)
 {
  memcpy(Chunk,B,sizeof(sChunkHeader));
  B+=sizeof(sChunkHeader);
  return B;
 }

 // ---

char *read_byte(char *B, char *d)
 {
  *d = *(char *)B;
  B+=1;
  return B;
 }
 // ---

char *read_word(char *B, unsigned short int *d)
 {
  *d = *(unsigned short int *)B;
  B+=2;
  return B;
 }
 // ---

char *read_dword(char *B, unsigned int *d)
 {
  *d = *(unsigned int *)B;
  B+=4;
  return B;
 }

 // ---

char *read_float(char *B, float *d)
 {
  *d = *(float *)B;
  B+=4;
  return B;
 }

 // ---

char *read_asciiz(char *B, char *d)
 {
  int i;
  char j;

  i = 0;
  do
  {
   B = read_byte(B,&j);
   if (j!=' ')
   {
    d[i] = j;
    i++;
   }
   else
   {
    d[i] = '_';
    i++;
   }
  } while ((d[i-1]!=0)&&(i<20));
  return B;
 }

 // ---

char *Spline_Read(char *B,sKeyframe *K)
 {
  unsigned short int SplineFlag;
  float Tens,Cont,Bias,EaseTo,EaseFrom;

  Tens = 0; Cont = 0; Bias = 0; EaseTo = 0; EaseFrom = 0;
  B = read_word(B,&SplineFlag);
  if ((SplineFlag &  1) != 0) B = read_float(B,&Tens);
  if ((SplineFlag &  2) != 0) B = read_float(B,&Cont);
  if ((SplineFlag &  4) != 0) B = read_float(B,&Bias);
  if ((SplineFlag &  8) != 0) B = read_float(B,&EaseTo);
  if ((SplineFlag & 16) != 0) B = read_float(B,&EaseFrom);
  K->Tens     = Tens;
  K->Cont     = Cont;
  K->Bias     = Bias;
  K->EaseTo   = EaseTo;
  K->EaseFrom = EaseFrom;
  return B;
 }


// ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

int Scene_Parse3DS(sScene *Scene, char *Buffer, char *BufferEnd)
 {
  char              *ptr;
  sChunkHeader       Chunk;
  char               BYTE;
  short int          SHORTINT;
  unsigned short int WORD,A,B,C,Flag,W,      NumFaces;
  unsigned int       DWORD,DW1,DW2;
  float              FLOAT,X,Y,Z,U,V;
  char               STRING[32],Name[32];
  int                i,j,num;

  sVector TempVector;
  float   TempMatrix[16];
  float   TempQuat[4],oldQuat[4],t[4];

  sObject   *Obj,*Obj2;
  sCamera   *Cam;
  sLight    *Light;
  sMaterial *Mat;
  sKeyframe *K;
  sNode     *Node,*Node2,*Node3;
  int        MatType;
  int        kfType;
  int        kfCurrentHierarchy;
  sRGB       RGBdummy;
  sRGB      *RGB;

  float Tens,Cont,Bias,EaseTo,EaseFrom;

  // ---

  RGB = &RGBdummy;
  ptr = Buffer;
  // ÄÄÄ Start reading chunks ÄÄÄ
  ptr = Read_ChunkHeader(&Chunk,ptr);
  if (Chunk.Id != 0x4d4d) return Error3DS_InvalidFile;
  while (ptr < BufferEnd)
  {
   ptr = Read_ChunkHeader(&Chunk,ptr);
   //printf("Chunk 0x%4x, size %d\n",Chunk.Id,Chunk.Size);
   //getch();
   switch(Chunk.Id)
   {
    case 0x0002 : // 3DS version. dword: version

      ptr = read_dword(ptr,&DWORD);
      break;

    case 0x0010 : // RGB (float)

      ptr = read_float(ptr,&(RGB->r));
      ptr = read_float(ptr,&(RGB->g));
      ptr = read_float(ptr,&(RGB->b));
      // The RGB pointer is setup in any of the chunks that expect a RGB/color
      // chunk coming, and reset here
      // in case there is coming something we don't handle, so that it doesn't
      // overwrite the previous RGB loaded
      RGB = &RGBdummy;
      break;

    case 0x0011 : // RGB (byte)

      ptr = read_byte(ptr,&BYTE);
      RGB->r = (float)BYTE / 255;
      ptr = read_byte(ptr,&BYTE);
      RGB->g = (float)BYTE / 255;
      ptr = read_byte(ptr,&BYTE);
      RGB->b = (float)BYTE / 255;
      RGB = &RGBdummy;
      break;

    case 0x0100 : // MASTER_SCALE

      ptr = read_float(ptr,&FLOAT);
      Scene->MasterScale = FLOAT;
      break;

    case 0x2100 : // Ambient Light

      RGB = &Scene->AmbientLightRGB;
      break;

    case 0x2200 : // Fog

      ptr = read_float(ptr,&FLOAT);
      Scene->NearFogPlaneDistance = FLOAT;
      ptr = read_float(ptr,&FLOAT);
      Scene->NearFogPlaneDensity = FLOAT;
      ptr = read_float(ptr,&FLOAT);
      Scene->FarFogPlaneDistance = FLOAT;
      ptr = read_float(ptr,&FLOAT);
      Scene->FarFogPlaneDensity = FLOAT;
      RGB = &Scene->FogColor;
      break;

    case 0x2201 : // Use fog flag

      Scene->UseFog = ON;
      break;

    case 0x3d3d : // 3D Editor chunk.

      break;

// --- object ---

    case 0x4000 : // Object block

      ptr = read_asciiz(ptr,&Name);
      break;

    case 0x4010 : // Object hidden (boolean)

      Obj->Flags |= OBJflag_hidden;
      break;

    case 0x4100 : // Triangular mesh

      Node = Scene_NewNode(Scene);
      Obj = Object_New();
      Node->Data = Obj;
      Node->Type = NODEtype_object;
      strcpy(Node->Name,Name);
      break;

    case 0x4110 : // Vertices list

      ptr = read_word(ptr,&WORD);
      Obj->NumVertices = WORD;
      Obj->Vertices = Vertex_New(WORD);
      Obj->NumNormals = WORD;
      Obj->Normals = Normal_New(WORD);
      for (i=0;i<WORD;i++)
      {
       // SWAPPED
       ptr = read_float(ptr,&X);
       ptr = read_float(ptr,&Z);
       ptr = read_float(ptr,&Y);
       Obj->Vertices[i].Pos.x = X;
       Obj->Vertices[i].Pos.y = Y;
       Obj->Vertices[i].Pos.z = Z;
      }
      break;

    case 0x4120 : // Faces list

      ptr = read_word(ptr,&WORD);
      Obj->NumPolygons = WORD;
      Obj->Polygons = Polygon_New(WORD);
      for (i=0;i<WORD;i++)
      {
       ptr = read_word(ptr,&A);
       ptr = read_word(ptr,&B);
       ptr = read_word(ptr,&C);
       ptr = read_word(ptr,&Flag);
       Obj->Polygons[i].A = A;
       Obj->Polygons[i].B = B;
       Obj->Polygons[i].C = C;
       Obj->Polygons[i].Flags = Flag;
       Obj->Polygons[i].Vertices[0] = &(Obj->Vertices[A]);
       Obj->Polygons[i].Vertices[1] = &(Obj->Vertices[B]);
       Obj->Polygons[i].Vertices[2] = &(Obj->Vertices[C]);
       // Flag:
       // bit 0,1,2 = CA,BC,AB visible
       // bit 3 = U wrapping
       // bit 4 = V wrapping
      }
      NumFaces = WORD;
      break;

    case 0x4130 : // Face material list

      ptr = read_asciiz(ptr,&STRING);
      Mat = Scene_FindMaterialByName(Scene,STRING);
      ptr = read_word(ptr,&WORD);
      for (i=0;i<WORD;i++)
      {
       ptr = read_word(ptr,&W);
       // DBG_3DS(printf("%i : %i\n",i,W);)
       // if (W >= Obj->NumPolygons) { printf("ERROR!!\n"); exit; }
       Obj->Polygons[W].Material = Mat;
      }
      break;

    case 0x4140 : // Mapping coordinates for each vertex

      ptr = read_word(ptr,&WORD);
      Obj->Flags |= OBJflag_mapped;
      for (i=0;i<WORD;i++)
      {
       ptr = read_float(ptr,&U);
       ptr = read_float(ptr,&V);
       Obj->Normals[i].u = U * UVmultiplier;
       Obj->Normals[i].v = V * UVmultiplier;
      }
      break;

    case 0x4150 : // Smoothing group list

      for (i=0;i<Obj->NumPolygons;i++)
      {
       ptr = read_dword(ptr,&DWORD);
       Obj->Polygons[i].SmoothingGroup = DWORD;
      }
      break;

    case 0x4160 : // Local coordinate system (matrix)

 /*
    is 3DS matrices 147 or 123 ?
                    258    456
                    369    789
    in case of the first one, the swapped matrix should look like this:
            Ú¿
    adgA   agdA    agdA
    behB   bheB   ÚcifC
    cfiC   cifC   ÀbheB
    0001   0001    0001
  else:
    abcA    acbA    acbA
    defB -> dfeB -> gihC
    ghiC    gihC    dfeB
    0001    0001    0001
    But, since we will doing an inverse transform, I don't know...
    Try this out a bit... :-/
 */

 // 012    021    021
 // 345 -> 354 -> 687
 // 678    687    354

      // SWAPPED
      ptr = read_float(ptr,&Obj->Matrix3DS[0]);
      ptr = read_float(ptr,&Obj->Matrix3DS[2]);
      ptr = read_float(ptr,&Obj->Matrix3DS[1]);

      ptr = read_float(ptr,&Obj->Matrix3DS[6]);
      ptr = read_float(ptr,&Obj->Matrix3DS[8]);
      ptr = read_float(ptr,&Obj->Matrix3DS[7]);

      ptr = read_float(ptr,&Obj->Matrix3DS[3]);
      ptr = read_float(ptr,&Obj->Matrix3DS[5]);
      ptr = read_float(ptr,&Obj->Matrix3DS[4]);
      // SWAPPED
      ptr = read_float(ptr,&Obj->Pivot3DS.x);
      ptr = read_float(ptr,&Obj->Pivot3DS.z);
      ptr = read_float(ptr,&Obj->Pivot3DS.y);

      Matrix_Identity(&TempMatrix);

      TempMatrix[ 0] = Obj->Matrix3DS[0];
      TempMatrix[ 1] = Obj->Matrix3DS[1];
      TempMatrix[ 2] = Obj->Matrix3DS[2];

      TempMatrix[ 4] = Obj->Matrix3DS[3];
      TempMatrix[ 5] = Obj->Matrix3DS[4];
      TempMatrix[ 6] = Obj->Matrix3DS[5];

      TempMatrix[ 8] = Obj->Matrix3DS[6];
      TempMatrix[ 9] = Obj->Matrix3DS[7];
      TempMatrix[10] = Obj->Matrix3DS[8];

      TempVector.x = Obj->Pivot3DS.x;
      TempVector.y = Obj->Pivot3DS.y;
      TempVector.z = Obj->Pivot3DS.z;

      // Matrix_InvScale(&TempMatrix,&TempMatrix);

      for (i=0;i<Obj->NumVertices;i++)
      {
       TempVector.x = Obj->Vertices[i].Pos.x - Obj->Pivot3DS.x;
       TempVector.y = Obj->Vertices[i].Pos.y - Obj->Pivot3DS.y;
       TempVector.z = Obj->Vertices[i].Pos.z - Obj->Pivot3DS.z;
       Vector_Trans(&(Obj->Vertices[i].Pos),&TempVector,&TempMatrix);
      }
      break;

    case 0x4165 : // Object color in 3d editor (useless?)

      ptr = read_byte(ptr,&BYTE);
      break;

// --- light ---

    case 0x4600 : // Light

      Node = Scene_NewNode(Scene);
      Light = Light_New();
      Node->Data = Light;
      Node->Type = NODEtype_light;
      strcpy(Node->Name,Name);
      Light->Type = LIGHTtype_omni;
      // SWAPPED
      ptr = read_float(ptr,&X);
      ptr = read_float(ptr,&Z);
      ptr = read_float(ptr,&Y);
      Light->Pos.x = X;
      Light->Pos.y = Y;
      Light->Pos.z = Z;
      break;

    case 0x4610 : // Spotlight

      Light->Type = LIGHTtype_spot;
      // SWAPPED
      ptr = read_float(ptr,&X);
      ptr = read_float(ptr,&Z);
      ptr = read_float(ptr,&Y);
      Light->Target.x = X;
      Light->Target.y = Y;
      Light->Target.z = Z;
      ptr = read_float(ptr,&FLOAT);
      Light->HotSpot = FLOAT;
      ptr = read_float(ptr,&FLOAT);
      Light->FallOff = FLOAT;
      break;

    case 0x4651 : // Spot is rectangular

      break;

    case 0x4653 : // Spot map

      ptr = read_asciiz(ptr,&STRING);
      break;

    case 0x4656 : // Spot roll

      ptr = read_float(ptr,&FLOAT);
      Light->SpotRoll = FLOAT;
      break;

// --- camera ---

    case 0x4700 : // Camera

      Node = Scene_NewNode(Scene);
      Cam = Camera_New();
      Node->Data = Cam;
      Node->Type = NODEtype_camera;
      strcpy(Node->Name,Name);
      // SWAPPED
      ptr = read_float(ptr,&X);
      ptr = read_float(ptr,&Z);
      ptr = read_float(ptr,&Y);
      Cam->Pos.x = X;
      Cam->Pos.y = Y;
      Cam->Pos.z = Z;
      // SWAPPED
      ptr = read_float(ptr,&X);
      ptr = read_float(ptr,&Z);
      ptr = read_float(ptr,&Y);
      Cam->Target.x = X;
      Cam->Target.y = Y;
      Cam->Target.z = Z;
      ptr = read_float(ptr,&FLOAT);
      Cam->Roll = FLOAT;
      ptr = read_float(ptr,&FLOAT);
      Camera_LensToFov(Cam,FLOAT);
      // Cam->Fov = FLOAT;
      break;

// --- material ---

    case 0xA000 : // Material name

      ptr = read_asciiz(ptr,&STRING);
      strcpy(Mat->Name,STRING);
      break;

    case 0xA010 : // Material ambient color

      RGB = &Mat->AmbientColor;
      break;

    case 0xA020 : // Material difuse color

      RGB = &Mat->DiffuseColor;
      break;

    case 0xA030 : // Material specular color

      RGB = &Mat->SpecularColor;
      break;

    case 0xA081 : // TwoSided material

      Mat->Flags |= MATflag_twosided;
      break;

    case 0xA083 : // Additive transparency

      Mat->Flags |= MATflag_addtrans;
      break;

    case 0xA085 : // Wireframe

      Mat->Flags |= MATflag_wireframe;

      break;

    case 0xA100 : // Material shading method

      ptr = read_word(ptr,&WORD);
      Mat->Type = WORD;
      break;

    case 0xA200 : // Texture map 1

      Mat->Flags |= MATflag_texture1;
      MatType = 1;
      break;

    case 0xA210 : // Opacity map

      Mat->Flags |= MATflag_opacity;
      MatType = 2;
      break;

    case 0xA220 : // Reflection map

      Mat->Flags |= MATflag_reflection;
      MatType = 3;
      break;

    case 0xA230 : // Bump map

      Mat->Flags |= MATflag_bump;
      MatType = 4;
      break;

    case 0xA33A : // Texture map 2

      Mat->Flags |= MATflag_texture2;
      MatType = 5;
      break;

    case 0xA300 : // Mapping file name

      ptr = read_asciiz(ptr,&STRING);
      switch(MatType)
      {
       case 1 : strcpy(Mat->FileNameTexture1,  STRING); break;
       case 2 : strcpy(Mat->FileNameOpacity,   STRING); break;
       case 3 : strcpy(Mat->FileNameReflection,STRING); break;
       case 4 : strcpy(Mat->FileNameBump,      STRING); break;
       case 5 : strcpy(Mat->FileNameTexture2,  STRING); break;
      }
      break;

    case 0xAFFF : // Material

      Mat = Material_New();
      Scene_AddMaterial(Scene,Mat);
      break;

// --- keyframer ---

    case 0xB000 : // Keyframer

      break;

    case 0xB001 : // Ambient light information block

      kfType = 1;
      break;

    case 0xB002 : // Mesh/object information block

      kfType = 2;
      break;

    case 0xB003 : // Camera information block

      kfType = 3;
      break;

    case 0xB004 : // Camera target information block

      kfType = 4;
      break;

    case 0xB005 : // Omni light information block

      kfType = 5;
      break;

    case 0xB006 : // Spot light target information block

      kfType = 6;
      break;

    case 0xB007 : // Spot light information block

      kfType = 7;
      break;

    case 0xB008 : // Frames (Start and End)

      ptr = read_dword(ptr,&DW1);
      ptr = read_dword(ptr,&DW2);
      Scene->StartFrame = DW1;
      Scene->EndFrame = DW2;
      break;

    case 0xB010 : // Object name, parameters and hierarchy father

      ptr = read_asciiz(ptr,&STRING);
      // Handle DUMMY object
      if (strncmp("$$$DUMMY",STRING,20)==0)
      {
       Node = Scene_NewNode(Scene);
       Obj = Object_New();
       Node->Data = Obj;
       Node->Type = NODEtype_object;
       strcpy(Node->Name,STRING);
       Obj->Flags |= OBJflag_dummy;
      }
      else
      // handle $AMBIENT object
      if (strncmp("$AMBIENT$",STRING,20)==0)
      {
       Node = Scene_NewNode(Scene);
       Obj = Object_New();
       Node->Data = Obj;
       Node->Type = NODEtype_object;
       strcpy(Node->Name,STRING);
       Obj->Flags |= OBJflag_ambient;
      }
      else
      {
       Node = Scene_FindNodeByName (Scene,STRING);
       switch(kfType)
       {
        case 1 : break;                                // ambient light
        case 2 : Obj   = (sObject *)Node->Data; break; // object
        case 3 : Cam   = (sCamera *)Node->Data; break; // camera
        case 4 : Cam   = (sCamera *)Node->Data; break; // camera target
        case 5 : Light = (sLight  *)Node->Data; break; // omni light
        case 6 : Light = (sLight  *)Node->Data; break; // spot light target
        case 7 : Light = (sLight  *)Node->Data; break; // spot light
       }
      }

      ptr = read_word(ptr,&WORD);
        // bit  2 = node render object hide
        // bit  3 = Light off in keyframer
        // bit 11 = hidden (hide node in keyframer)
        // bit 12 = Fast draw node in keyframer
        // bit 14 = Node tag is not an instance
      ptr = read_word(ptr,&WORD);
        //  from 3DSFTK (autodesk)
        // bit 1 : Display path.
        // bit 2 : Object node uses autosmoothing.
        // bit 3 : Object node is frozen.
        // bit 5 : Object node uses motion blur.
        // bit 6 : Object node has material morphing.
        // bit 7 : Object node has mesh morphing.
        //  from 3dsinfo.txt
        // Bit 0 : Show path
        // Bit 1 : Animate smoothing
        // Bit 4 : Object motion blur
        // Bit 6 : Morph materials
      ptr = read_word(ptr,&WORD);
      SHORTINT = WORD;
      Node->Id = kfCurrentHierarchy;
      Node->ParentId = SHORTINT;
      //Node->Id = kfCurrentHierarchy;
      //Node->ParentId = SHORTINT;
      break;

    case 0xB011 : // Instance name

      ptr = read_asciiz(ptr,&STRING);
      break;

    case 0xB013 : // Object pivot point (Offset from center of BOUNDBOX)

      // SWAPPED
      ptr = read_float(ptr,&X);
      ptr = read_float(ptr,&Z);
      ptr = read_float(ptr,&Y);
      // Can only Mesh objects have a pivot?
      // Obj is set up in 0xB010
      Obj->Pivot.x = X;
      Obj->Pivot.y = Y;
      Obj->Pivot.z = Z;

      // Trekk ifra her, s† slipper vi † gj›re det for hver gang i rendringa,
      // eller burde jeg gj›re det der uansett... ???

      for (i=0;i<Obj->NumVertices;i++)
      {
       Obj->Vertices[i].Pos.x = Obj->Vertices[i].Pos.x - X;
       Obj->Vertices[i].Pos.y = Obj->Vertices[i].Pos.y - Y;
       Obj->Vertices[i].Pos.z = Obj->Vertices[i].Pos.z - Z;
      }
      break;

    case 0xB014 : // Mesh object bounding box

      // SWAPPED
      ptr = read_float(ptr,&X);
      ptr = read_float(ptr,&Z);
      ptr = read_float(ptr,&Y);
      // SWAPPED
      ptr = read_float(ptr,&X);
      ptr = read_float(ptr,&Z);
      ptr = read_float(ptr,&Y);
      break;

    case 0xB015 : // Smoothing angle for morphing objects

      ptr = read_float(ptr,&FLOAT);
      break;

        // TRACK-FLAGS
        // Bits 0-1 : 0 = single
        //            2 = repeat
        //            3 = loop
        // Bit 3 : lock X
        // Bit 4 : lock Y
        // Bit 5 : lock Z
        // Bit 7 : unlink X
        // Bit 8 : unlink Y
        // Bit 9 : unlink Z

    case 0xB020 : // Position track

      // the following are equal for all tracks (except the specific stuff)
      ptr = read_word(ptr,&WORD);
      for (i=0;i<8;i++) ptr = read_byte(ptr,&BYTE); // skip 8 unknown bytes
      ptr = read_dword(ptr,&DWORD);
      num = DWORD;
      for (i=0;i<num;i++)
      {
       ptr = read_dword(ptr,&DWORD);
       K = Keyframe_New();
       K->Frame = DWORD;
       ptr = Spline_Read(ptr,K);
       // --- track specific data ---
       // SWAPPED
       ptr = read_float(ptr,&X);
       ptr = read_float(ptr,&Z);
       ptr = read_float(ptr,&Y);
       K->Val._vector.x = X;
       K->Val._vector.y = Y;
       K->Val._vector.z = Z;
       // ---
       switch(kfType)
       {
        case 2 : Track_AddKeyframe(&Obj->kfPos,K); break;
        case 3 : Track_AddKeyframe(&Cam->kfPos,K); break;
        case 4 : Track_AddKeyframe(&Cam->kfTarget,K); break;
        case 7 :
        case 5 : Track_AddKeyframe(&Light->kfPos,K); break;
        case 6 : Track_AddKeyframe(&Light->kfTarget,K); break;
       }
       // ---------------------------
      }
      switch(kfType)
      {
       case 2 : Spline_Init( Obj->kfPos.Keys ); break;
       case 3 : Spline_Init( Cam->kfPos.Keys ); break;
       case 4 : Spline_Init( Cam->kfTarget.Keys ); break;
       case 7 :
       case 5 : Spline_Init( Light->kfPos.Keys ); break;
       case 6 : Spline_Init( Light->kfTarget.Keys ); break;
      }
      break;

    case 0xB021 : // Rotation track

      Quat_Identity(&oldQuat);
      // the following are equal for all tracks (except the specific stuff)
      ptr = read_word(ptr,&WORD);
      for (i=0;i<8;i++) ptr = read_byte(ptr,&BYTE); // skip 8 unknown bytes
      ptr = read_dword(ptr,&DWORD);
      num = DWORD;
      for (i=0;i<num;i++)
      {
       ptr = read_dword(ptr,&DWORD);
       K = Keyframe_New();
       K->Frame = DWORD;
       ptr = Spline_Read(ptr,K);
       // --- track specific data ---
       ptr = read_float(ptr,&FLOAT);
       // SWAPPED
       ptr = read_float(ptr,&X);
       ptr = read_float(ptr,&Z);
       ptr = read_float(ptr,&Y);
       TempVector.x = X;
       TempVector.y = Y;
       TempVector.z = Z;
       Quat_FromAngAxis(&TempQuat, FLOAT, &TempVector);
       if (i!=0) Quat_Mul(&t,&TempQuat,&oldQuat);

        // domain error in sqrt = qa in KEYframer must not be 0,0,0
       // Quat_Copy(&(K->qa),&oldQuat);
       Quat_Copy(&oldQuat,&TempQuat);
       Quat_Copy(&(K->Val._quat),&TempQuat);
       Track_AddKeyframe(&Obj->kfRot,K);
       // ---------------------------
      }
      Spline_Init( Obj->kfRot.Keys );
      break;

    case 0xB022 : // Scale track

      // the following are equal for all tracks (except the specific stuff)
      ptr = read_word(ptr,&WORD);
      for (i=0;i<8;i++) ptr = read_byte(ptr,&BYTE); // skip 8 unknown bytes
      ptr = read_dword(ptr,&DWORD);
      num = DWORD;
      for (i=0;i<num;i++)
      {
       ptr = read_dword(ptr,&DWORD);
       K = Keyframe_New();
       K->Frame = DWORD;
       ptr = Spline_Read(ptr,K);
       // --- track specific data ---
       // SWAPPED
       ptr = read_float(ptr,&X);
       ptr = read_float(ptr,&Z);
       ptr = read_float(ptr,&Y);
       K->Val._vector.x = X;
       K->Val._vector.y = Y;
       K->Val._vector.z = Z;
       Track_AddKeyframe(&Obj->kfScale,K);
       // ---------------------------
      }
      Spline_Init( Obj->kfScale.Keys );
      break;

    case 0xB023 : // Fov track

      // the following are equal for all tracks (except the specific stuff)
      ptr = read_word(ptr,&WORD);
      for (i=0;i<8;i++) ptr = read_byte(ptr,&BYTE); // skip 8 unknown bytes
      ptr = read_dword(ptr,&DWORD);
      num = DWORD;
      for (i=0;i<num;i++)
      {
       ptr = read_dword(ptr,&DWORD);
       K = Keyframe_New();
       K->Frame = DWORD;
       ptr = Spline_Read(ptr,K);
       // --- track specific data ---
       ptr = read_float(ptr,&FLOAT);
       K->Val._float = FLOAT;
       Track_AddKeyframe(&Cam->kfFov,K);
       // ---------------------------
      }
      Spline_Init( Cam->kfFov.Keys );
      break;

    case 0xB024 : // Roll track

      // the following are equal for all tracks (except the specific stuff)
      ptr = read_word(ptr,&WORD);
      for (i=0;i<8;i++) ptr = read_byte(ptr,&BYTE); // skip 8 unknown bytes
      ptr = read_dword(ptr,&DWORD);
      num = DWORD;
      for (i=0;i<num;i++)
      {
       ptr = read_dword(ptr,&DWORD);
       K = Keyframe_New();
       K->Frame = DWORD;
       ptr = Spline_Read(ptr,K);
       // --- track specific data ---
       ptr = read_float(ptr,&FLOAT);
       K->Val._float = FLOAT;
       Track_AddKeyframe(&Cam->kfRoll,K);
       // ---------------------------
      }
      Spline_Init( Cam->kfRoll.Keys );
      break;

    // 0xB025 = Color track ?????

    case 0xB026 : // Morph track

      // the following are equal for all tracks (except the specific stuff)
      ptr = read_word(ptr,&WORD);
      for (i=0;i<8;i++) ptr = read_byte(ptr,&BYTE); // skip 8 unknown bytes
      ptr = read_dword(ptr,&DWORD);
      num = DWORD;
      for (i=0;i<num;i++)
      {
       ptr = read_dword(ptr,&DWORD);
       K = Keyframe_New();
       K->Frame = DWORD;
       ptr = Spline_Read(ptr,K);
       // --- track specific data ---
       ptr = read_asciiz(ptr,&STRING);
       strcpy(K->Val._string,STRING);
       Track_AddKeyframe(&Obj->kfMorph,K);
       // ---------------------------
      }
      Spline_Init( Obj->kfMorph.Keys );
      break;

    case 0xB027 : // HotSpot track

      // the following are equal for all tracks (except the specific stuff)
      ptr = read_word(ptr,&WORD);
      for (i=0;i<8;i++) ptr = read_byte(ptr,&BYTE); // skip 8 unknown bytes
      ptr = read_dword(ptr,&DWORD);
      num = DWORD;
      for (i=0;i<num;i++)
      {
       ptr = read_dword(ptr,&DWORD);
       ptr = read_word(ptr,&WORD);
       if ((WORD &  1) != 0) ptr = read_float(ptr,&FLOAT);
       if ((WORD &  2) != 0) ptr = read_float(ptr,&FLOAT);
       if ((WORD &  4) != 0) ptr = read_float(ptr,&FLOAT);
       if ((WORD &  8) != 0) ptr = read_float(ptr,&FLOAT);
       if ((WORD & 16) != 0) ptr = read_float(ptr,&FLOAT);
       // --- track specific data ---
       ptr = read_float(ptr,&FLOAT);
       // ---------------------------
      }
      break;

    case 0xB028 : // FallOff track

      // the following are equal for all tracks (except the specific stuff)
      ptr = read_word(ptr,&WORD);
      for (i=0;i<8;i++) ptr = read_byte(ptr,&BYTE); // skip 8 unknown bytes
      ptr = read_dword(ptr,&DWORD);
      num = DWORD;
      for (i=0;i<num;i++)
      {
       ptr = read_dword(ptr,&DWORD);
       ptr = Spline_Read(ptr,K);

       ptr = read_word(ptr,&WORD);
       if ((WORD &  1) != 0) ptr = read_float(ptr,&FLOAT);
       if ((WORD &  2) != 0) ptr = read_float(ptr,&FLOAT);
       if ((WORD &  4) != 0) ptr = read_float(ptr,&FLOAT);
       if ((WORD &  8) != 0) ptr = read_float(ptr,&FLOAT);
       if ((WORD & 16) != 0) ptr = read_float(ptr,&FLOAT);
       // --- track specific data ---
       ptr = read_float(ptr,&FLOAT);
       // ---------------------------
      }
      break;

    case 0xB029 : // Hide track

      // the following are equal for all tracks (except the specific stuff)
      ptr = read_word(ptr,&WORD);
      for (i=0;i<8;i++) ptr = read_byte(ptr,&BYTE); // skip 8 unknown bytes
      ptr = read_dword(ptr,&DWORD);
      num = DWORD;
      for (i=0;i<num;i++)
      {
       ptr = read_dword(ptr,&DWORD);
       K = Keyframe_New();
       K->Frame = DWORD;
       ptr = Spline_Read(ptr,K);
       // --- track specific data ---
       Track_AddKeyframe(&Obj->kfHide,K);
       // ---------------------------
      }
      Spline_Init( Obj->kfHide.Keys );
      break;

    case 0xB030 : // Hierarchy position

      ptr = read_word(ptr,&WORD);
      SHORTINT = WORD;
      kfCurrentHierarchy = SHORTINT;
      break;

// ---

    default     : ptr += (Chunk.Size-6);
   }
  }

  // ------------------------------------------------------------------------

  DEBUG(printf("ş 3DS loaded ok. Now post-processing stuff...\n"););

  // --- Set up hierarchy ---------------------------------------------------

  DEBUG(printf("ş Set up hierarchy, part 1\n"););

  Node = Scene->Nodes;
  while (Node != NULL)
  {
   num = Node->ParentId;
   if (num != -1)
   {
    Node2 = Scene_FindNodeById(Scene,num);
    Node->Parent = Node2;
   }
   Node = Node->Next;
  }

  DEBUG(printf("ş Set up hierarchy, part 2\n"););

  // Set-up child & brother tree

  Node = Scene->Nodes;
  while (Node != NULL)
  {
   // find all nodes with Node as parent
   Node2 = Scene->Nodes;
   while (Node2!=NULL)
   {
    if ((Node2->ParentId != -1) && (Node2->ParentId == Node->Id))
    {
     // Yes, Node2 is child to Node
     if (Node->Child != NULL)
     {
      // Node already has a child, so we have to search the Brother list,
      // until the end, to add Node2 as a brother to another node (which
      // also is a child to Node...)
      Node3 = Node->Child;
      while (Node3->Brother != NULL) Node3=Node3->Brother;
      Node3->Brother = Node2;
     }
     else Node->Child = Node2;
    }
    Node2=Node2->Next;
   }
   Node=Node->Next;
  }
  return Error3DS_NoError;
 }

