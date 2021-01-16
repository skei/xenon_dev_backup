 #include <stdio.h>   // file
 #include <io.h>      // filelength
 #include <string.h>  // strncpy,strncmp
 #include "engine.h"
 // 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴

 // Load 3DS file into memory...
 // In  : filename : name of 3DS file to load
 //       buffer   : ptr to allocate and load file into
 // Out : size of 3DS file/buffer

sWorld *Load3DS( char *filename )
 {
  FILE *fp;
  char *buffer;
  int filesize;
  sWorld *W;

  fp = fopen(filename,"rb");
  filesize = filelength(fileno(fp));
  buffer = (char *)malloc(filesize);
  fread(buffer,1,filesize,fp);
  fclose(fp);
  W = Parse3DS( buffer, filesize );
  free(buffer);
  return W;
 }

 // 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�

 #define ReadChunk {                                   \
  chunk.chunk_id   = *(unsigned short int *)BufferPtr; \
  BufferPtr+=2;                                        \
  chunk.next_chunk = *(unsigned int *)BufferPtr;       \
  BufferPtr+=4;                                        \
  iBufferPointer += 6;                                 \
 }

 // -----

 #define ReadChar(a) {    \
  a = *(char *)BufferPtr; \
  BufferPtr+=1;           \
  iBufferPointer += 1;    \
 }

 // -----

 #define ReadWord(a) {                  \
  a = *(unsigned short int *)BufferPtr; \
  BufferPtr+=2;                         \
  iBufferPointer += 2;                  \
 }

 // -----

 #define ReadDword(a) {           \
  a = *(unsigned int *)BufferPtr; \
  BufferPtr+=4;                   \
  iBufferPointer += 4;            \
 }

 // -----

 #define ReadFloat(a) {    \
  a = *(float *)BufferPtr; \
  BufferPtr+=4;            \
  iBufferPointer += 4;     \
 }


 // -----

 #define ReadAsciiz(a) {         \
  i = 0;                         \
  do                             \
  {                              \
    ReadChar(j);                 \
    if (j!=' ')                  \
   {                             \
    a[i] = j;                    \
    i++;                         \
   }                             \
  } while ((a[i-1]!=0)&&(i<20)); \
 }

 // -----

 #define ReadSpline {      \
  ReadWord(Flag);          \
  for (k = 0; k < 16; k++) \
  {                        \
   if (Flag & (1 << k))    \
   {                       \
    ReadFloat(TempFloat);  \
   }                       \
  }                        \
 }

 // Parse previously loaded 3DS file
 // In  : buffer     : ptr to previously loaded 3DS file
 //       BufferSize : size of buffer / length of 3DS file
 //       World      : World/Scene to load
 // Out : 0=OK, else error code
 //       1=Invalid 3DS file

sWorld *Parse3DS( char *buffer, int BufferSize )
 {

  struct
  {
   unsigned short int chunk_id;
   unsigned int       next_chunk;
  } chunk;

  // -----

  sWorld *World;

  char *BufferPtr;
  int iBufferPointer;

  // ----- temp -----

  int i,j,k;
  float X,Y,Z,W;
  float U,V;
  short int A,B,C,Flag;
  short int num;
  float ma,mb,mc,md,me,mf,mg,mh,mi,mj,mk,ml;

  unsigned short int TempWord;
  unsigned int TempDword;
  float TempFloat;
  char TempString[20];
  float q[4],q1[4],q2[4];
  float m1[16],m2[16];
  sVector TempVector;
  sVector TempVector2;
  sObject *TempObject;
  sObject *LastObject;        // for $$$DUMMY insertion :-/
  sObject *oo1,*oo2;
  sMaterial *TempMaterial;

  // --------------------

  sObject   *CurrentObject;   // Ptr to object being processed
  sLight    *CurrentLight;    //
  sMaterial *CurrentMaterial; //
  sCamera   *CurrentCamera;   //
  char CurrentName[20];       // ObjBlock name (TriMesh, Light, Camera)
  int CurrentType;            // 1 = Object, 6 = Light, 7 = Camera;
  int CurrentObjectKF;        // Object number (TrackObjNumber)
  sObject *Oo;
  sCamera *Cc;

  // --------------------

  // ----- Initialize variables, etc... -----

  World = (sWorld *)GetMem(sizeof(sWorld));

  iBufferPointer = 0;
  BufferPtr = buffer;

  CurrentLight = NULL;
  CurrentMaterial = NULL;
  CurrentCamera = NULL;
  CurrentObject = NULL;
  LastObject = NULL;

  // ----- Start reading chunks -----
  ReadChunk;
  if (chunk.chunk_id != 0x4d4d) return NULL;
  else
  // ----- Valid 3DS file -----
  {
   while (iBufferPointer < BufferSize)
   {
    ReadChunk;
    switch(chunk.chunk_id)
    {
     case 0x3d3d : // ObjMesh
                   break;
      case 0x4000 : // ObjBlock (ObjMesh/ObjBlock)
                    ReadAsciiz(CurrentName);
                    break;

 // --- TriMesh ---

             case 0x4100 : // TriMesh (ObjMesh/ObjBlock/TriMesh)
                     CurrentType = 1;
                     World->NumObjects++;
                     if (CurrentObject == NULL)
                     { // World->Objects
                      CurrentObject = NewObject(1);
                      memset(CurrentObject,0,sizeof(sObject));
                      World->Objects = CurrentObject;
                     }
                     else
                     {
                      CurrentObject->Next = NewObject(1);
                      memset(CurrentObject->Next,0,sizeof(sObject));
                      CurrentObject->Next->Prev = CurrentObject;
                      CurrentObject = CurrentObject->Next;
                     }
                     CurrentObject->OBJECT_active = 1;
                     strncpy(CurrentObject->Name,CurrentName,20);
                     CurrentObject->HierarchyNumber = -1;
                     CurrentObject->ParentNumber    = -1;
                     LastObject = CurrentObject;
                     break;
       case 0x4110 : // VertList (ObjMesh/ObjBlock/TriMesh/VertList)
                     ReadWord(num);
                     CurrentObject->NumVertices = num;
                     CurrentObject->Vertices = NewVertex(num);
                     for (i=0;i<num;i++)
                     {
                      ReadFloat(X);
                      ReadFloat(Z);
                      ReadFloat(Y);
                      CurrentObject->Vertices[i].Pos.x = -X;
                      CurrentObject->Vertices[i].Pos.y = -Y;
                      CurrentObject->Vertices[i].Pos.z = -Z;
                     }
                     break;
       case 0x4120 : // FaceList (ObjMesh/ObjBlock/TriMesh/FaceList)
                     ReadWord(num);
                     CurrentObject->NumPolygons = num;
                     CurrentObject->Polygons = NewPolygon(num);
                     for (i=0;i<num;i++)
                     {
                      ReadWord(A);
                      ReadWord(B);
                      ReadWord(C);
                      ReadWord(Flag);
                      // 01 CA, 02 BC, 04 AB
                      // 08 U-wrap, 10 V-wrap
                      //CurrentObject->Polygons[i].Flags = Flag;  // ???
                      CurrentObject->Polygons[i].v[0] = &(CurrentObject->Vertices[A]);
                      CurrentObject->Polygons[i].v[1] = &(CurrentObject->Vertices[B]);
                      CurrentObject->Polygons[i].v[2] = &(CurrentObject->Vertices[C]);
                     }
                     break;
       case 0x4130 : // FaceMat (ObjMesh/ObjBlock/TriMesh/FaceMat)
                     ReadAsciiz(TempString);
                     TempMaterial = Material_FindByName(World,&TempString);
                     ReadWord(num);
                     for (i=0;i<num;i++)
                     {
                      ReadWord(TempWord);
                      CurrentObject->Polygons[TempWord].POLYGON_material = 1;
                      CurrentObject->Polygons[TempWord].Material = TempMaterial;
                     }
                     break;
       case 0x4140 : // MapList (ObjMesh/ObjBlock/TriMesh/MapList)
                     CurrentObject->OBJECT_mapped = 1;
                     ReadWord(num);
                     for (i=0;i<num;i++)
                     {
                      ReadFloat(U);
                      ReadFloat(V);
                      CurrentObject->Vertices[i].u = U;
                      CurrentObject->Vertices[i].v = V;
                     }
                     break;
       case 0x4150 : // Smoothing (ObjMesh/ObjBlock/TriMesh/Smoothing)
                     for (i=0;i<CurrentObject->NumPolygons;i++)
                     {
                      ReadDword(TempDword);
                     }
                     break;
       case 0x4160 : // Matrix (ObjMesh/ObjBlock/TriMesh/matrix)
                     ReadFloat(ma); // 3x3 matrix
                     ReadFloat(mb);
                     ReadFloat(mc);

                     ReadFloat(md);
                     ReadFloat(me);
                     ReadFloat(mf);

                     ReadFloat(mg);
                     ReadFloat(mh);
                     ReadFloat(mi);


                     ReadFloat(mj); // vector
                     ReadFloat(mk);
                     ReadFloat(ml);
/*

abc0   acb0   acb0   agdj
def0   dfe0   gih0   cifl
ghi0   gih0   dfe0   bhek
jkl1   jlk1   jlk1   0001

*/
                     CurrentObject->m_3ds[ 0] =  -ma;
                     CurrentObject->m_3ds[ 1] =  -mg;
                     CurrentObject->m_3ds[ 2] =  -md;
                     CurrentObject->m_3ds[ 3] =  -mj;

                     CurrentObject->m_3ds[ 4] =  -mc;
                     CurrentObject->m_3ds[ 5] =  -mi;
                     CurrentObject->m_3ds[ 6] =  -mf;
                     CurrentObject->m_3ds[ 7] =  -ml;

                     CurrentObject->m_3ds[ 8] =  -mb;
                     CurrentObject->m_3ds[ 9] =  -mh;
                     CurrentObject->m_3ds[10] =  -me;
                     CurrentObject->m_3ds[11] =  -mk;

                     CurrentObject->m_3ds[12] =  0;
                     CurrentObject->m_3ds[13] =  0;
                     CurrentObject->m_3ds[14] =  0;
                     CurrentObject->m_3ds[15] =  1;

                     break;

 // --- Light ---

      case 0x4600 : // Light (ObjBlock/Light)
                    CurrentType = 6;
                    World->NumLights++;
                    if (CurrentLight == NULL)
                    { // World->Lights
                     CurrentLight = NewLight(1);
                     World->Lights = CurrentLight;
                    }
                    else
                    {
                     CurrentLight->Next = NewLight(1);
                     CurrentLight = CurrentLight->Next;
                    }
                    CurrentLight->LIGHT_active = 1;
                    strncpy(CurrentLight->Name,CurrentName,20);
                    CurrentLight->Strength = 1;                 // ???
                    CurrentLight->MaxRange = 256;               // ???
                    CurrentCamera->HierarchyNumber = -1;
                    CurrentCamera->ParentNumber    = -1;
                    ReadFloat(X);
                    ReadFloat(Z);
                    ReadFloat(Y);
                    CurrentLight->Pos.x = -X;
                    CurrentLight->Pos.y = -Y;
                    CurrentLight->Pos.z = -Z;
                    CurrentLight->Next = NULL;
                    break;
       case 0x4610 : // SpotLight (ObjBlock/Light/SpotLight)
                     ReadFloat(X);  // TargetPosX
                     ReadFloat(Z);  // TargetPosY
                     ReadFloat(Y);  // TargetPosZ
                     ReadFloat(TempFloat);  // HotSpot;
                     ReadFloat(TempFloat);  // FallOff;
                     break;

 // --- Camera ---

      case 0x4700 : // Camera (ObjBlock/Camera)
                    CurrentType = 7;
                    World->NumCameras++;
                    if (CurrentCamera == NULL)
                    { // World->Lights
                     CurrentCamera = NewCamera(1);
                     World->Cameras = CurrentCamera;
                    }
                    else
                    {
                     CurrentCamera->Next = NewCamera(1);
                     memset(CurrentCamera->Next,0,sizeof(sCamera));
                     CurrentCamera = CurrentCamera->Next;
                    }
                    CurrentCamera->CAMERA_active = 1;
                    strncpy(CurrentCamera->Name,CurrentName,20);
                    ReadFloat(X);
                    ReadFloat(Z);
                    ReadFloat(Y);
                    CurrentCamera->Pos.x = -X;
                    CurrentCamera->Pos.y = -Y;
                    CurrentCamera->Pos.z = -Z;
                    ReadFloat(X);
                    ReadFloat(Z);
                    ReadFloat(Y);
                    CurrentCamera->Target.x = -X;
                    CurrentCamera->Target.y = -Y;
                    CurrentCamera->Target.z = -Z;
                    ReadFloat(TempFloat);
                    CurrentCamera->Bank = TempFloat;
                    ReadFloat(TempFloat);
                    CurrentCamera->FOV = TempFloat;
                    CurrentCamera->Next = NULL;
                    break;
 // ???  case 0x4f00 : // Hierarchy (ObjBlock/Hierarchy)
 //                    break;

 // --- Material ---

     case 0xafff : // Material
                   break;
      case 0xa000 : // MaterialName (Material/MaterialName)
                    World->NumMaterials++;
                    if (CurrentMaterial == NULL)
                    { // World->Materials
                     CurrentMaterial = NewMaterial(1);
                     World->Materials = CurrentMaterial;
                    }
                    else
                    {
                     CurrentMaterial->Next = NewMaterial(1);
                     CurrentMaterial = CurrentMaterial->Next;
                    }
                    ReadAsciiz(CurrentMaterial->Name);
                    break;
      case 0xa200 : // TextureMap (Material/TextureMap)
                    CurrentMaterial->MATERIAL_texture = 1;
                    break;
      case 0xa300 : // MapFile (Material/TextureMap/MapFile)
                    ReadAsciiz(CurrentMaterial->FileName);
                    break;

 // --- KeyFramer ---

     case 0xb000 : // KeyFramer
                   World->WORLD_anim = 1;
                   break;
      case 0xb002 : // TrackInfo (KeyFramer/TrackInfo)
                    CurrentType = 1; // object
                    break;
      case 0xb003 : // CameraTrackInfo (KeyFramer/CameraTrackInfo)
                    CurrentType = 7; // Camera pos
                    break;
      case 0xb004 : // CameraTarget (KeyFramer/CameraTarget)
                    CurrentType = 71; // camera target
                    break;
      case 0xb008 : // Hierarchy info
                    ReadDword(TempDword);
                    World->StartFrame = TempDword;
                    ReadDword(TempDword);
                    World->EndFrame = TempDword;
                    break;
       case 0xb030 : // ObjNumber (KeyFramer/TrackInfo/TrackObjNumber)
                     ReadWord(CurrentObjectKF);
                     break;
       case 0xb010 : // TrackObjName (KeyFramer/TrackInfo/TrackObjName)
                     if (CurrentType == 1) // Object
                     {
                      ReadAsciiz(TempString);
                      if (strncmp("$$$DUMMY",TempString,20)==0)
                      {
                       World->NumObjects++;
                       LastObject->Next = NewObject(1);
                       LastObject->Next->Prev = LastObject;
                       LastObject = LastObject->Next;
                       CurrentObject = LastObject;
                       CurrentObject->OBJECT_dummy = 1;
                       strncpy(CurrentObject->Name,TempString,20);
                       CurrentObject->HierarchyNumber = -1;
                       CurrentObject->ParentNumber    = -1;
                      }
                      else
                      {
                       CurrentObject = Object_FindByName(World,&TempString);
                      }
                      ReadWord(TempWord);
                      ReadWord(TempWord);
                      ReadWord(num);
                      CurrentObject->ParentNumber = (int)num;  // Hierarchynumber of parent
                      CurrentObject->HierarchyNumber = (int)CurrentObjectKF;
                     }
                     else
                     if (CurrentType == 7) // Camera
                     {
                      ReadAsciiz(TempString);
                      CurrentCamera = Camera_FindByName(World,&TempString);
                      ReadWord(TempWord);
                      ReadWord(TempWord);
                      ReadWord(num);
                      CurrentCamera->ParentNumber = num;  // Hierarchynumber of parent
                      CurrentCamera->HierarchyNumber = CurrentObjectKF;
                     }
                     else
                     if (CurrentType == 71) // Camera
                     {
                      ReadAsciiz(TempString);
                      CurrentCamera = Camera_FindByName(World,&TempString);
                      ReadWord(TempWord);
                      ReadWord(TempWord);
                      ReadWord(num);
                      CurrentCamera->ParentNumber = num;  // Hierarchynumber of parent
                      CurrentCamera->HierarchyNumber = CurrentObjectKF;
                     }

                     break;
       case 0xb013 : // TrackPivot (KeyFramer/TrackInfo/TrackPivot)
                     ReadFloat(X);
                     ReadFloat(Z);
                     ReadFloat(Y);
                     if (CurrentType == 1)
                     {
                      CurrentObject->Pivot.x = -X;
                      CurrentObject->Pivot.y = -Y;
                      CurrentObject->Pivot.z = -Z;
                     }
                     else
                     if (CurrentType == 7)
                     {
                      CurrentCamera->Pivot.x = -X;
                      CurrentCamera->Pivot.y = -Y;
                      CurrentCamera->Pivot.z = -Z;
                     }
                     break;
       case 0xb020 : // Pos (KeyFramer/TrackInfo/Pos)
                     if (CurrentType == 1)
                     {
                      for (i=0;i<5;i++) ReadWord(TempWord);
                      ReadWord(num);
                      ReadWord(TempWord);
                      CurrentObject->PosKeyFrame.KEY_active = 1;
                      CurrentObject->PosKeyFrame.KEY_defined = 1;
                      CurrentObject->PosKeyFrame.KEY_vector = 1;
                      CurrentObject->PosKeyFrame.CurrentPos = 0;
                      CurrentObject->PosKeyFrame.Speed = 0;
                      CurrentObject->PosKeyFrame.CurrentKey = 0;
                      CurrentObject->PosKeyFrame.NumKeys = num;
                      CurrentObject->PosKeyFrame.BendSize = 1;   // ?
                      CurrentObject->PosKeyFrame.VectorKeys = NewVectorKey(num);
                      CurrentObject->PosKeyFrame.FrameNumber = NewInt(num);
                      for (i=0;i<num;i++)
                      {
                       ReadWord(TempWord);
                       CurrentObject->PosKeyFrame.FrameNumber[i] = TempWord;
                       ReadWord(TempWord);
                       ReadSpline;
                       ReadFloat(X);
                       ReadFloat(Z);
                       ReadFloat(Y);
                       CurrentObject->PosKeyFrame.VectorKeys[i].Pos.x = -X;
                       CurrentObject->PosKeyFrame.VectorKeys[i].Pos.y = -Y;
                       CurrentObject->PosKeyFrame.VectorKeys[i].Pos.z = -Z;
                      }
                     }
                     else
                     if (CurrentType == 7) // Camera
                     {
                      for (i=0;i<5;i++) ReadWord(TempWord);
                      ReadWord(num);
                      ReadWord(TempWord);
                      CurrentCamera->PosKeyFrame.KEY_active = 1;
                      CurrentCamera->PosKeyFrame.KEY_defined = 1;
                      CurrentCamera->PosKeyFrame.KEY_vector = 1;
                      CurrentCamera->PosKeyFrame.CurrentPos = 0;
                      CurrentCamera->PosKeyFrame.Speed = 0;
                      CurrentCamera->PosKeyFrame.CurrentKey = 0;
                      CurrentCamera->PosKeyFrame.NumKeys = num;
                      CurrentCamera->PosKeyFrame.BendSize = 1;   // ?
                      CurrentCamera->PosKeyFrame.VectorKeys = NewVectorKey(num);
                      CurrentCamera->PosKeyFrame.FrameNumber = NewInt(num);

                      for (i=0;i<num;i++)
                      {
                       ReadWord(TempWord);
                       CurrentCamera->PosKeyFrame.FrameNumber[i] = TempWord;
                       ReadWord(TempWord);
                       ReadSpline;
                       ReadFloat(X);
                       ReadFloat(Z);
                       ReadFloat(Y);
                       CurrentCamera->PosKeyFrame.VectorKeys[i].Pos.x = -X;
                       CurrentCamera->PosKeyFrame.VectorKeys[i].Pos.y = -Y;
                       CurrentCamera->PosKeyFrame.VectorKeys[i].Pos.z = -Z;
                      }
                     }
                     else
                     if (CurrentType == 71) // Camera target
                     {
                      for (i=0;i<5;i++) ReadWord(TempWord);
                      ReadWord(num);
                      ReadWord(TempWord);
                      CurrentCamera->TarKeyFrame.KEY_active = 1;
                      CurrentCamera->TarKeyFrame.KEY_defined = 1;
                      CurrentCamera->TarKeyFrame.KEY_vector = 1;
                      CurrentCamera->TarKeyFrame.CurrentPos = 0;
                      CurrentCamera->TarKeyFrame.Speed = 0;
                      CurrentCamera->TarKeyFrame.CurrentKey = 0;
                      CurrentCamera->TarKeyFrame.NumKeys = num;
                      CurrentCamera->TarKeyFrame.BendSize = 1;   // ?
                      CurrentCamera->TarKeyFrame.VectorKeys = NewVectorKey(num);
                      CurrentCamera->TarKeyFrame.FrameNumber = NewInt(num);
                      for (i=0;i<num;i++)
                      {
                       ReadWord(TempWord);
                       CurrentCamera->TarKeyFrame.FrameNumber[i] = TempWord;
                       ReadWord(TempWord);
                       ReadSpline;
                       ReadFloat(X);
                       ReadFloat(Z);
                       ReadFloat(Y);
                       CurrentCamera->TarKeyFrame.VectorKeys[i].Pos.x = -X;
                       CurrentCamera->TarKeyFrame.VectorKeys[i].Pos.y = -Y;
                       CurrentCamera->TarKeyFrame.VectorKeys[i].Pos.z = -Z;
                      }
                     }
                     break;
       case 0xb021 : // Rot (KeyFramer/TrackInfo/Rot)
                     for (i=0;i<5;i++) ReadWord(TempWord);
                     ReadWord(num);
                     ReadWord(TempWord);
                     CurrentObject->RotKeyFrame.KEY_active = 1;
                     CurrentObject->RotKeyFrame.KEY_defined = 1;
                     CurrentObject->RotKeyFrame.KEY_float = 1;
                     CurrentObject->RotKeyFrame.KEY_vector = 1;
                     CurrentObject->RotKeyFrame.CurrentPos = 0;
                     CurrentObject->RotKeyFrame.Speed = 0;
                     CurrentObject->RotKeyFrame.CurrentKey = 0;
                     CurrentObject->RotKeyFrame.NumKeys = num;
                     CurrentObject->RotKeyFrame.BendSize = 1;   // ?
                     CurrentObject->RotKeyFrame.VectorKeys = NewVectorKey(num);
                     CurrentObject->RotKeyFrame.FloatKeys = NewFloatKey(num);
                     CurrentObject->RotKeyFrame.FrameNumber = NewInt(num);

                     for (i=0;i<num;i++)
                     {
                      ReadWord(TempWord);
                      CurrentObject->RotKeyFrame.FrameNumber[i] = TempWord;
                      ReadWord(TempWord);
                      ReadSpline;
                      ReadFloat(W);
                      ReadFloat(X);
                      ReadFloat(Z);
                      ReadFloat(Y);
                      TempVector.x = -X;
                      TempVector.y = -Y;
                      TempVector.z = -Z;
                      Quat_FromAngAxis(W,&TempVector,&q);
                      Quat_Normalize(&q);
                      // Q = Q * Parent.Q
                      if (i!=0)
                      {
                       q1[qX] = CurrentObject->RotKeyFrame.VectorKeys[i-1].Pos.x;
                       q1[qY] = CurrentObject->RotKeyFrame.VectorKeys[i-1].Pos.y;
                       q1[qZ] = CurrentObject->RotKeyFrame.VectorKeys[i-1].Pos.z;
                       q1[qW] = CurrentObject->RotKeyFrame.FloatKeys[i-1].Value;
                       Quat_Mult(&q,&q1);
                       Quat_Normalize(&q);
                      }
                      CurrentObject->RotKeyFrame.VectorKeys[i].Pos.x = q[qX];
                      CurrentObject->RotKeyFrame.VectorKeys[i].Pos.y = q[qY];
                      CurrentObject->RotKeyFrame.VectorKeys[i].Pos.z = q[qZ];
                      CurrentObject->RotKeyFrame.FloatKeys[i].Value  = q[qW];
                     }
                     break;
       case 0xb023 : // FOV (KeyFramer/CameraTrackInfo/FOV)
                     for (i=0;i<5;i++) ReadWord(TempWord);
                     ReadWord(num);
                     ReadWord(TempWord);
                     CurrentCamera->FOVKeyFrame.KEY_active = 1;
                     CurrentCamera->FOVKeyFrame.KEY_defined = 1;
                     CurrentCamera->FOVKeyFrame.KEY_float = 1;
                     CurrentCamera->FOVKeyFrame.CurrentPos = 0;
                     CurrentCamera->FOVKeyFrame.Speed = 0;
                     CurrentCamera->FOVKeyFrame.CurrentKey = 0;
                     CurrentCamera->FOVKeyFrame.NumKeys = num;
                     CurrentCamera->FOVKeyFrame.BendSize = 1;   // ?
                     CurrentCamera->FOVKeyFrame.FloatKeys = NewFloatKey(num);
                     CurrentCamera->FOVKeyFrame.FrameNumber = NewInt(num);
                     for (i=0;i<num;i++)
                     {
                      ReadWord(TempWord);
                      CurrentCamera->FOVKeyFrame.FrameNumber[i] = TempWord;
                      ReadWord(TempWord);
                      ReadSpline;
                      ReadFloat(TempFloat);
                      CurrentCamera->FOVKeyFrame.FloatKeys[i].Value = TempFloat;
                     }
                     break;
       case 0xb024 : // Bank (KeyFramer/CameraTrackInfo/Bank)
                     for (i=0;i<5;i++) ReadWord(TempWord);
                     ReadWord(num);
                     ReadWord(TempWord);
                     CurrentCamera->BankKeyFrame.KEY_active = 1;
                     CurrentCamera->BankKeyFrame.KEY_defined = 1;
                     CurrentCamera->BankKeyFrame.KEY_float = 1;
                     CurrentCamera->BankKeyFrame.CurrentPos = 0;
                     CurrentCamera->BankKeyFrame.Speed = 0;
                     CurrentCamera->BankKeyFrame.CurrentKey = 0;
                     CurrentCamera->BankKeyFrame.NumKeys = num;
                     CurrentCamera->BankKeyFrame.BendSize = 1;   // ?
                     CurrentCamera->BankKeyFrame.FloatKeys = NewFloatKey(num);
                     CurrentCamera->BankKeyFrame.FrameNumber = NewInt(num);
                     for (i=0;i<num;i++)
                     {
                      ReadWord(TempWord);
                      CurrentCamera->BankKeyFrame.FrameNumber[i] = TempWord;
                      ReadWord(TempWord);
                      ReadSpline;
                      ReadFloat(TempFloat);
                      CurrentCamera->BankKeyFrame.FloatKeys[i].Value = TempFloat;
                     }
                     break;
         default : iBufferPointer += (chunk.next_chunk-6); // fseek(fp,chunk.next_chunk-6, SEEK_CUR);
                   BufferPtr += (chunk.next_chunk-6);
    }
   }
  }
  // --- Set up hierarchy ---------------------------------------------------
  oo1 = World->Objects;
  while (oo1 != NULL)
  {
   num = oo1->ParentNumber;
   if (num != -1)
   {
    oo2 = World->Objects;
    for (;;)
    {
     if (oo2==NULL)
     {
      DEBUG(printf("harra-baillj #2! Didn't find object # %i in ObjectList\n",num););
      exit(1);
     }
     if (num == oo2->HierarchyNumber) goto exitloop;
     oo2=oo2->Next;
    }
    exitloop:
    oo1->Parent = oo2;
   }
   oo1 = oo1->Next;
  }
  // --- Init camera splines ------------------------------------------------
  Cc = World->Cameras;
  while (Cc != NULL)
  {
   // flushall();
   if (Cc->PosKeyFrame.KEY_defined==1) Spline_Init(&(Cc->PosKeyFrame));
   if (Cc->TarKeyFrame.KEY_defined==1) Spline_Init(&(Cc->TarKeyFrame));
   if (Cc->FOVKeyFrame.KEY_defined==1) Spline_Init(&(Cc->FOVKeyFrame));
   if (Cc->BankKeyFrame.KEY_defined==1) Spline_Init(&(Cc->BankKeyFrame));
   Cc = Cc->Next;
  }
  // --- Init object splines ------------------------------------------------
  Oo = World->Objects;
  while (Oo != NULL)
  {
   if (Oo->PosKeyFrame.KEY_defined==1) Spline_Init(&(Oo->PosKeyFrame));
   if (Oo->RotKeyFrame.KEY_defined==1) Spline_Init(&(Oo->RotKeyFrame));
   Oo = Oo->Next;
  }
  // --- Transform object from world -> object -> pivot ---------------------
  Oo = World->Objects;
  while (Oo != NULL)
  {
   Matrix_Identity(&m1);
   Matrix_Inverse(&m1,Oo->m_3ds);
   for (i=0;i<Oo->NumVertices;i++)
   {
    Vector_Trans(&TempVector,Oo->Vertices[i].Pos,&m1);
    Oo->Vertices[i].Pos.x = TempVector.x - Oo->Pivot.x;
    Oo->Vertices[i].Pos.y = TempVector.y - Oo->Pivot.y;
    Oo->Vertices[i].Pos.z = TempVector.z - Oo->Pivot.z;
   }
   Oo = Oo->Next;
  }
  // ------------------------------------------------------------------------
  return World;
 }


