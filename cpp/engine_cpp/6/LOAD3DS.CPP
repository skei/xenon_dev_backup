 #include <stdio.h>   // filestuff, printf
 #include <stdlib.h>  // malloc
 #include <io.h>      // filelength
 #include <string.h>  // strncpy

 #include "engine.hpp"

 // Load 3DS file into memory, and call Parse3DS to actually handle the data
int cScene::Load3DS(char *file)
 {
  FILE *fp;
  char *buffer;
  int filesize;
  int res;

  DEBUG(printf("þ Loading 3DS file: %s\n",file);)
  fp = fopen(file,"rb");
  filesize = filelength(fileno(fp));
  DEBUG(printf("þ Allocating %i bytes (%s filesize)\n",filelength,file););
  buffer = (char *)malloc(filesize);
  fread(buffer,1,filesize,fp);
  fclose(fp);
  res = Parse3DS(buffer, filesize);
  DEBUG(printf("þFreeing temp. memory block for parsing 3DS\n"););
  free(buffer);
  return res;
 }

 // ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

 // "global" vars so all sub-functions may access them

 char *BufferPtr;
 int iBufferPointer;


 // ÄÄÄ some defines to make things easier & cleaner ÄÄÄ

 #define ReadChunk {                             \
  Chunk.Id   = *(unsigned short int *)BufferPtr; \
  BufferPtr+=2;                                  \
  Chunk.Size = *(unsigned int *)BufferPtr;       \
  BufferPtr+=4;                                  \
  iBufferPointer += 6;                           \
 }

 // ÄÄÄ

 #define SkipChunk {                \
  iBufferPointer += (Chunk.Size-6); \
  BufferPtr += (Chunk.Size-6);      \
 }

 // ÄÄÄ

 #define ReadChar(a) {    \
  a = *(char *)BufferPtr; \
  BufferPtr+=1;           \
  iBufferPointer += 1;    \
 }

 // ÄÄÄ

 #define ReadWord(a) {                  \
  a = *(unsigned short int *)BufferPtr; \
  BufferPtr+=2;                         \
  iBufferPointer += 2;                  \
 }

 // ÄÄÄ

 #define ReadDword(a) {           \
  a = *(unsigned int *)BufferPtr; \
  BufferPtr+=4;                   \
  iBufferPointer += 4;            \
 }

 // ÄÄÄ

 #define ReadFloat(a) {    \
  a = *(float *)BufferPtr; \
  BufferPtr+=4;            \
  iBufferPointer += 4;     \
 }

 // ÄÄÄ

 // Skip spaces.. :-/
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

 // ÄÄÄ Support ÄÄÄ

 // swap y/z in vector.
 void Vector_Swap (cVector *a)
 {
  float tmp;
  tmp  = a->y;
  a->y = a->z;
  a->z = tmp;
 }

 // ÄÄÄ

 // swap y/z in quaternion
 void Quat_Swap (float *a)
 {
  float tmp;
  tmp   = a[qY];
  a[qY] = a[qZ];
  a[qZ] = tmp;
 }

 // ÄÄÄ

 // swap y/z in matrix.
 void Matrix_Swap(float *a)
 {
  int   i;
  float tmp;

  for (i=0; i<3; i++)     // swap columns
  {
   tmp        = a[(i*4)+1];
   a[(i*4)+1] = a[(i*4)+2];
   a[(i*4)+2] = tmp;
  }
  for (i=0; i<4; i++)     // swap rows
  {
   tmp        = a[(1*4)+i];
   a[(1*4)+i] = a[(2*4)+i];
   a[(2*4)+i] = tmp;
  }
 }

 void ReadSpline(cKeyframe *key)
 {
  int ii;
  float dat;
  unsigned short int ffflag;

  DEBUG(printf("Spline reader:\n"););
  key->mTens     = 0.0;
  key->mCont     = 0.0;
  key->mBias     = 0.0;
  key->mEaseTo   = 0.0;
  key->mEaseFrom = 0.0;

  ReadWord(ffflag);
  DEBUG(printf("flags: %6x\n",ffflag););
  for (ii = 0; ii < 16; ii++)
  {
   if ((ffflag & (1 << ii))!=0)
   {
    ReadFloat(dat);
    DEBUG(printf("float #%i: %f\n",ii,dat););
    switch (ii)
    {
     case 0: key->mTens     = dat; break;
     case 1: key->mCont     = dat; break;
     case 2: key->mBias     = dat; break;
     case 3: key->mEaseTo   = dat; break;
     case 4: key->mEaseFrom = dat;
    }
   }
  }
 }


int cScene::Parse3DS(char *buffer,int BufferSize)
 {
  // ÄÄÄ variables ÄÄÄ

  int                 error;
  char                CurrentName[20]; // ObjBlock name
  int                 i,j,k;
  float               X,Y,Z,W;
  float               U,V;
  short int           A,B,C,Flag;
  short int           num;
  int                 NumKeys;
  unsigned char       TempChar;
  unsigned short int  TempWord;
  unsigned int        TempDword;
  float               TempFloat;
  char                TempString[20];
  cVector             TempVector;

  cNode         *CurrentNode;
  cObject       *CurrentObject;
  cMaterial     *CurrentMaterial;

  struct
  {
   unsigned short int Id;
   unsigned int       Size;
  } Chunk;

  // ÄÄÄ Initialize variables, etc... ÄÄÄ

  iBufferPointer = 0;
  BufferPtr = buffer;

  // ÄÄÄ Start reading chunks ÄÄÄ
  ReadChunk;
  if (Chunk.Id != 0x4d4d) return Error3DS_InvalidFile;// NULL;
  else
  {
   // ÄÄÄ Valid 3DS file ÄÄÄ
   while (iBufferPointer < BufferSize)
   {
    ReadChunk;
    DEBUG(printf("Chunk Id: %6x\n",Chunk.Id););
    switch(Chunk.Id)
    {
     // case 0x0002 : 3DS version (10 bytes)
     case 0x3d3d : // ObjMesh
                   break;

     case 0x4000 : // ObjMesh/ObjBlock
                   ReadAsciiz(CurrentName);
                   DEBUG(printf("Object: %s\n",CurrentName););
                   break;

     case 0x4010 : // ObjMesh/ObjBlock/ObjHidden
                   DEBUG(printf("Object is hidden\n"););
                   // bool = on
                   break;

     case 0x4012 : // ObjMesh/ObjBlock/NoCastShadow
                   DEBUG(printf("Object doesn't cast (shadow?)\n"););
                   // bool = on
                   break;

     case 0x4013 : // ObjMesh/ObjBlock/ObjMatte
                   DEBUG(printf("Object matte\n"););
                   // bool = on
                   break;

     case 0x4017 : // ObjMesh/ObjBlock/NoReceiveShadow
                   DEBUG(printf("Object doesn't receive shadows\n"););
                   // bool = on
                   break;

     // ÄÄÄ TriMesh ÄÄÄ

     case 0x4100 : // ObjMesh/ObjBlock/TriMesh
                   DEBUG(printf("Triangle mesh\n"););
                   // New object, add object to scene
                   CurrentNode = new cObject();
                   CurrentObject = (cObject *)CurrentNode;
                   //AddNode(CurrentNode);
                   strncpy(CurrentNode->mName,CurrentName,20);
                   break;

     case 0x4110 : // ObjMesh/ObjBlock/TriMesh/VertList

                   // word     Number of vertices
                   //  Then, for each vertex:
                   // vector   Position

                   ReadWord(num);
                   DEBUG(printf("NumVertices: %i\n",num););
                   CurrentObject->mNumVertices = num;
                   CurrentObject->mVertices = new cVertex[num];
                   CurrentObject->mNumNormals = num;
                   CurrentObject->mNormals = new cNormal[num];
                   for (i=0;i<num;i++)
                   {
                    ReadFloat(TempVector.x);
                    ReadFloat(TempVector.y);
                    ReadFloat(TempVector.z);
                    Vector_Swap(&TempVector);
                    CurrentObject->mVertices[i].mPos = TempVector;
                    //CurrentObject->mVertices[i].mPos.x = TempVector.x;
                    //CurrentObject->mVertices[i].mPos.y = TempVector.y;
                    //CurrentObject->mVertices[i].mPos.z = TempVector.z;
                    DEBUG(printf("%4i: %5.2f, %5.2f, %5.2f\n",i,TempVector.x,TempVector.y,TempVector.z););
                   }
                   break;

     case 0x4120 : // ObjMesh/ObjBlock/TriMesh/FaceList

                   // word     Number of faces
                   //  Then, for each face:
                   // word     Vertex for corner A (number reference)
                   // word     Vertex for corner B (number reference)
                   // word     Vertex for corner C (number reference)
                   // word     Face flag
                   //           * bit 0 : CA visible
                   //           * bit 1 : BC visible
                   //           * bit 2 : AB visible
                   //           * bit 3 : U wrapping
                   //           * bit 4 : V wrapping
                   // After datas, parse sub-chunks (0x4130, 0x4150).

                   ReadWord(num);
                   DEBUG(printf("NumPolygons: %i\n",num););
                   CurrentObject->mNumPolygons = num;
                   CurrentObject->mPolygons = new cPolygon[num];
                   for (i=0;i<num;i++)
                   {
                    ReadWord(A);
                    ReadWord(B);
                    ReadWord(C);
                    ReadWord(Flag);
                    DEBUG(printf("%4i: %4i, %4i, %4i : %4x\n",i,A,B,C,Flag););
                    CurrentObject->mPolygons[i].A = A;
                    CurrentObject->mPolygons[i].B = B;
                    CurrentObject->mPolygons[i].C = C;
                    CurrentObject->mPolygons[i].mFlags = Flag;
                    // 01 CA, 02 BC, 04 AB
                    // 08 U-wrap, 10 V-wrap
                    // Let polygons have default (1st created) material
                    //  in case there is no material assigned to face
                   }
                   break;

     case 0x4130 : // ObjMesh/ObjBlock/TriMesh/FaceMat

                   // strz     Material name
                   // word     Number of entries
                   //  Then, for each entry:
                   // word     Face assigned to this material (number reference)
                   // I think the faces of one object can have different
                   // materials. Therefore, this chunk can be present more
                   // than once.

                   ReadAsciiz(TempString); // material name
                   CurrentMaterial = FindMaterialByName(TempString);
                   ReadWord(num);          // number of entries for this material
                   for (i=0;i<num;i++)
                   {
                    ReadWord(TempWord); // poly number
                    CurrentObject->mPolygons[TempWord].mMaterial = CurrentMaterial;
                   }
                   break;

     case 0x4140 : // ObjMesh/ObjBlock/TriMesh/MapList

                   // word     Number of vertices
                   //  Then, for each vertex
                   // float    U coordinate
                   // float    V coordinate

                   ReadWord(num); // number of vertices
                   for (i=0;i<num;i++)
                   {
                    ReadFloat(U);
                    ReadFloat(V);
                    CurrentObject->mNormals[i].mU = U;
                    CurrentObject->mNormals[i].mV = V;
                   }
                   break;

     case 0x4150 : // ObjMesh/ObjBlock/TriMesh/Smoothing

                   // For each face in this mesh:
                   // dword    Smoothing group bits
                   // Each bit in the dword is a smoothing group. If a bit
                   // is set, the face belongs to the corresponding group.
                   // Two or more faces belonging to the same group and
                   // sharing the same vertex should be smoothly shaded
                   // over the faces edges connecting them (i.e. they
                   // should share the same "average" light-vector for that
                   // vertex).

                   for (i=0;i<CurrentObject->mNumPolygons;i++)
                   {
                    ReadDword(TempDword);
                    CurrentObject->mPolygons[i].mSmoothingGroup = TempDword;
                   }
                   break;

     case 0x4160 : // ObjMesh/ObjBlock/TriMesh/matrix

                   // Local coordinate system (the matrix)
                   // vector   X1
                   // vector   X2
                   // vector   X3
                   // vector   O
                   // X1, X2 and X3 represent the axes, O the origin.

                   CurrentObject->mMatrix3DS->Identity();

                   ReadFloat(CurrentObject->mMatrix3DS->M[ 0]);
                   ReadFloat(CurrentObject->mMatrix3DS->M[ 4]);
                   ReadFloat(CurrentObject->mMatrix3DS->M[ 8]);

                   ReadFloat(CurrentObject->mMatrix3DS->M[ 1]);
                   ReadFloat(CurrentObject->mMatrix3DS->M[ 5]);
                   ReadFloat(CurrentObject->mMatrix3DS->M[ 9]);

                   ReadFloat(CurrentObject->mMatrix3DS->M[ 2]);
                   ReadFloat(CurrentObject->mMatrix3DS->M[ 6]);
                   ReadFloat(CurrentObject->mMatrix3DS->M[10]);

                   ReadFloat(CurrentObject->mMatrix3DS->M[ 3]);
                   ReadFloat(CurrentObject->mMatrix3DS->M[ 7]);
                   ReadFloat(CurrentObject->mMatrix3DS->M[11]);
                   break;

     case 0x4165 : // Object color in editor (uninteresting??)
                   // byte     Color

                   ReadChar(TempChar);
                   break;

 /*

 // ÄÄÄ Light ÄÄÄ

       case 0x4600 : // ObjBlock/Light
                     // vector   Position

                     // CurrentType = 6;
                     Scene->NumLights++;
                     if (CurrentLight == NULL)
                     { // Scene->Lights
                      CurrentLight = Light_New();
                      Scene->Lights = CurrentLight;
                     }
                     else
                     {
                      CurrentLight->Next = Light_New();
                      CurrentLight = CurrentLight->Next;
                     }
                     CurrentLight->Flags |= LIGHTflag_active;
                     CurrentLight->Type = LIGHTtype_omni;       // default omni
                     strncpy(CurrentLight->Name,CurrentName,20);
                     CurrentLight->Strength = 1;                  // ???
                     CurrentLight->MaxRange = 256;                // ???
                     CurrentCamera->Id = -1;
                     // CurrentCamera->ParentId = -1;
                     ReadFloat(TempVector.x);
                     ReadFloat(TempVector.y);
                     ReadFloat(TempVector.z);
                     Vector_Swap(&TempVector);
                     Vector_Copy(&(CurrentLight->Pos),&TempVector);
                     CurrentLight->Next = NULL;
                     break;

        case 0x4610 : // ObjBlock/Light/SpotLight
                      // vector   Target
                      // float    HotSpot
                      // float    FallOff
                      // If this chunk is found, the light is a spot light and
                      // not an omni directional light.

                     CurrentLight->Type = LIGHTtype_spot;
                     // Set spotlight flag!
                     ReadFloat(TempVector.x); // TargetPosX
                     ReadFloat(TempVector.y); // TargetPosY
                     ReadFloat(TempVector.z); // TargetPosZ
                     Vector_Swap(&TempVector);
                     Vector_Copy(&(CurrentLight->Target),&TempVector);
                     ReadFloat(TempFloat);    // HotSpot;
                     CurrentLight->HotSpot = TempFloat;
                     ReadFloat(TempFloat);    // FallOff;
                     CurrentLight->FallOff = TempFloat;
                     break;

         case 0x4651 : // ObjBlock/Light/SpotLight/Rectangular
                       // Spot is rectangular
                       // bool = on

                       break;

         case 0x4653 : // ObjBlock/Light/SpotLight/SpotMap
                       // Spot map
                       // strz     Filename

                       ReadAsciiz(TempString);
                       break;

         case 0x4656 : // ObjBlock/Light/SpotLight/SpotRoll
                       // Spot roll
                       // float    Roll (degree)

                       ReadFloat(TempFloat);
                       CurrentLight->SpotRoll = TempFloat;
                       break;

 // ÄÄÄ Camera ÄÄÄ

       case 0x4700 : // ObjBlock/Camera
                     // vector   Position
                     // vector   Target
                     // float    Bank (degree)
                     // float    Lens

                     CurrentType = 7;
                     Scene->NumCameras++;
                     if (CurrentCamera == NULL)
                     { // Scene->Lights
                      CurrentCamera = Camera_New();
                      Scene->Cameras = CurrentCamera;
                     }
                     else
                     {
                      CurrentCamera->Next = Camera_New();
                      memset(CurrentCamera->Next,0,sizeof(sCamera));
                      CurrentCamera = CurrentCamera->Next;
                     }
                     CurrentCamera->Flags |= CAMERAflag_active;
                     strncpy(CurrentCamera->Name,CurrentName,20);
                     ReadFloat(TempVector.x);
                     ReadFloat(TempVector.y);
                     ReadFloat(TempVector.z);
                     Vector_Swap(&TempVector);
                     Vector_Copy(&(CurrentCamera->Pos),&TempVector);

                     ReadFloat(TempVector.x);
                     ReadFloat(TempVector.y);
                     ReadFloat(TempVector.z);
                     Vector_Swap(&TempVector);
                     Vector_Copy(&(CurrentCamera->Target),&TempVector);

                     ReadFloat(TempFloat);
                     CurrentCamera->Roll = TempFloat;
                     ReadFloat(TempFloat);
                     CurrentCamera->Fov = TempFloat;
                     CurrentCamera->Next = NULL;
                     break;

 // ÄÄÄ Material ÄÄÄ

      case 0xafff : // Material
                    break;
       case 0xa000 : // Material/MaterialName
                     Scene->NumMaterials++;
                     if (CurrentMaterial == NULL)
                     { // Scene->Materials
                      CurrentMaterial = Material_New();
                      Scene->Materials = CurrentMaterial;
                     }
                     else
                     {
                      CurrentMaterial->Next = Material_New();
                      CurrentMaterial = CurrentMaterial->Next;
                     }
                     ReadAsciiz(CurrentMaterial->Name);
                     break;

       // currently only supports one map per material.

       case 0xa200 : // TextureMap (Material/TextureMap)
                     CurrentMaterialType = 1;
                     CurrentMaterial->Flags |= MATflag_texture;
                     break;
       case 0xa210 : // OpacityMap (Material/OpacityMap)
                     CurrentMaterialType = 2;
                     CurrentMaterial->Flags |= MATflag_opacity;
                     break;
       case 0xa220 : // ReflectionMap (Material/ReflectionMap)
                     CurrentMaterialType = 3;
                     CurrentMaterial->Flags |= MATflag_reflection;
                     break;
       case 0xa230 : // BumpMap (Material/BumpMap)
                     CurrentMaterialType = 4;
                     CurrentMaterial->Flags |= MATflag_bump;
                     break;
       case 0xa300 : // MapFile (Material/TextureMap/MapFile)
                     switch(CurrentMaterialType)
                     {
                      case 1 : ReadAsciiz(CurrentMaterial->FileNameTexture);
                               break;
                      case 2 : ReadAsciiz(CurrentMaterial->FileNameOpacity);
                               break;
                      case 3 : ReadAsciiz(CurrentMaterial->FileNameReflection);
                               break;
                      case 4 : ReadAsciiz(CurrentMaterial->FileNameBump);
                               break;
                     }
                     break;

 // ÄÄÄ Keyframer ÄÄÄ

      case 0xb000 : // Keyframer
 //                   Scene->SCENE_anim = 1;
                      DEBUG(printf("---------- keyframer ----------\n"););

                    break;

     //case 0xb001 : // Keyframer/AmbientLight

       case 0xb002 : // KeyFramer/TrackInfo
                     CurrentType = 1; // object
                     break;
       case 0xb003 : // KeyFramer/CameraTrackInfo
                     CurrentType = 7; // Camera pos
                     break;
       case 0xb004 : // KeyFramer/CameraTarget
                     CurrentType = 71; // camera target
                     break;

       case 0xb005 : // KeyFramer/OmniLight
                     CurrentType = 100;
                     break;
       case 0xb006 : // KeyFramer/SpotLightTarget
                     CurrentType = 101;
                     break;
       case 0xb007 : // KeyFramer/SpotLight
                     CurrentType = 102;
                     break;

       case 0xb008 : // Hierarchy info
                     ReadDword(TempDword);
                     Scene->StartFrame = TempDword;
                     ReadDword(TempDword);
                     Scene->EndFrame = TempDword;
                     break;

        case 0xb010 : // TrackObjName (KeyFramer/TrackInfo/TrackObjName)

                      // strz     Object Name
                      // word     Flag1
                      //          * Bit 11 : Hidden
                      // word     Flag2
                      //          * Bit 0 : Show path
                      //          * Bit 1 : Animate smoothing
                      //          * Bit 4 : Object motion blur
                      //          * Bit 6 : Morph materials
                      // word     Hierarchy father, link to the parent object
                      //          (-1 for none)

                      if (CurrentType == 1) // Object
                      {
                       ReadAsciiz(TempString);
                       DEBUG(printf("object : %s\n",TempString););
                       if (strncmp("$$$DUMMY",TempString,20)==0)
                       {
                        Scene->NumObjects++;
                        LastObject->Next = Object_New();
                        LastObject->Next->Prev = LastObject;
                        LastObject = LastObject->Next;
                        CurrentObject = LastObject;
 //                       CurrentObject->OBJECT_dummy = 1;
                        strncpy(CurrentObject->Name,TempString,20);
                        CurrentObject->Id = -1;
                        CurrentObject->ParentId = -1;
                       }
                       else
                       {
                        CurrentObject = Object_FindByName(Scene,&TempString);
                       }
                       ReadWord(TempWord);   // Flag1
                       // Bit 11 : Hidden
                       ReadWord(TempWord);   // Flag2
                       // Bit 0 : Show path
                       // Bit 1 : Animate smoothing
                       // Bit 4 : Object motion blur
                       // Bit 6 : Morph materials
                       ReadWord(num);        // hierarchy father
                       CurrentObject->ParentId = (int)num;  // Hierarchynumber of parent
                       CurrentObject->Id = (int)CurrentObjectKF;
                      }
                      else
                      if (CurrentType == 7) // Camera
                      {
                       ReadAsciiz(TempString);
                       DEBUG(printf("camera : %s\n",TempString););
                       CurrentCamera = Camera_FindByName(Scene,&TempString);
                       ReadWord(TempWord);
                       ReadWord(TempWord);
                       ReadWord(num);
                       CurrentCamera->ParentId = num;  // Hierarchynumber of parent
                       CurrentCamera->Id = CurrentObjectKF;
                      }
                      else
                      if (CurrentType == 71) // Camera target
                      {
                       ReadAsciiz(TempString);
                       DEBUG(printf("camera target : %s\n",TempString););
                       CurrentCamera = Camera_FindByName(Scene,&TempString);
                       ReadWord(TempWord);
                       ReadWord(TempWord);
                       ReadWord(num);
                       CurrentCamera->ParentId = num;  // Hierarchynumber of parent
                       CurrentCamera->Id = CurrentObjectKF;
                      }
                      if (CurrentType == 100) // Omni light
                      {
                       ReadAsciiz(TempString);
                       DEBUG(printf("Omni light : %s\n",TempString););
                       CurrentLight = Light_FindByName(Scene,&TempString);
                       ReadWord(TempWord);
                       ReadWord(TempWord);
                       ReadWord(num);
                       CurrentLight->ParentId = num;  // Hierarchynumber of parent
                       CurrentLight->Id = CurrentObjectKF;
                      }
                      if (CurrentType == 101) // Spotlight target
                      {
                       ReadAsciiz(TempString);
                       DEBUG(printf("Spotlight target: %s\n",TempString););
                       CurrentLight = Light_FindByName(Scene,&TempString);
                       ReadWord(TempWord);
                       ReadWord(TempWord);
                       ReadWord(num);
                       CurrentLight->ParentId = num;  // Hierarchynumber of parent
                       CurrentLight->Id = CurrentObjectKF;
                      }
                      if (CurrentType == 102) // Spotlight
                      {
                       ReadAsciiz(TempString);
                       DEBUG(printf("Spotlight : %s\n",TempString););
                       CurrentLight = Light_FindByName(Scene,&TempString);
                       ReadWord(TempWord);
                       ReadWord(TempWord);
                       ReadWord(num);
                       CurrentLight->ParentId = num;  // Hierarchynumber of parent
                       CurrentLight->Id = CurrentObjectKF;
                      }
                      break;

        case 0xb013 : // TrackPivot (KeyFramer/TrackInfo/TrackPivot)
                      // vector   Pivot point
                      ReadFloat(TempVector.x);
                      ReadFloat(TempVector.y);
                      ReadFloat(TempVector.z);
                      Vector_Swap(&TempVector);
                      if (CurrentType == 1) Vector_Copy(&(CurrentObject->Pivot),&TempVector);
                      else
                      if (CurrentType == 7) Vector_Copy(&(CurrentCamera->Pivot),&TempVector);
                      // can lights have pivot ???? (spotlight???)
                      break;

                      //        Chunk # 0xB020..0xB029
                      //        word     Flag
                      //                  * Bits 0-1 : 0 = single
                      //                               2 = repeat
                      //                               3 = loop
                      //                  * Bit 3 : lock X
                      //                  * Bit 4 : lock Y
                      //                  * Bit 5 : lock Z
                      //                  * Bit 7 : unlink X
                      //                  * Bit 8 : unlink Y
                      //                  * Bit 9 : unlink Z
                      //        8 byte   Unknown
                      //        dword    Number of keys in this track
                      //
                      //                 Then, for each key:
                      //        dword    Key number (position in track)
                      //        word     Acceleration data present (flag)   Range:
                      //                  * Bit 0 : Tension follows         [-1.0, 1.0]
                      //                  * Bit 1 : Continuity follows      [-1.0, 1.0]
                      //                  * Bit 2 : Bias follows            [-1.0, 1.0]
                      //                  * Bit 3 : Ease to follows         [ 0.0, 1.0]
                      //                  * Bit 4 : Ease from follows       [ 0.0, 1.0]
                      //        n floats Acceleration data
                      //        ?        Track specific data
                      //
                      //        Track specific data is:
                      //         0xB020 : Position track : 1 vector   Position
                      //         0xB021 : Rotation track : 1 float    Angle (rad)
                      //                                   1 vector   Axis
                      //         0xB022 : Scale track    : 3 floats   Size
                      //         0xB023 : FOV track      : 1 float    Angle (degree)
                      //         0xB024 : Roll track     : 1 float    Angle (degree)
                      //         0xB025 : Color track    :
                      //         0xB026 : Morph track    : 1 strz     Object name
                      //         0xB027 : Hotspot track  : 1 float    Angle (degree)
                      //         0xB028 : Falloff track  : 1 float    Angle (degree)
                      //         0xB029 : Hide track     : nothing

          case 0xb020 : // Pos (KeyFramer/TrackInfo/Pos)

                      if (CurrentType == 1) // object
                      {
                       DEBUG(printf("object pos keyframe\n"););
                       ReadWord(Flag);
                       // Bits 0-1 : 0 = single
                       //            2 = repeat
                       //            3 = loop
                       // Bit 3 : lock X
                       // Bit 4 : lock Y
                       // Bit 5 : lock Z
                       // Bit 7 : unlink X
                       // Bit 8 : unlink Y
                       // Bit 9 : unlink Z
                       for (i=0;i<4;i++) ReadWord(TempWord);
                       //ReadWord(num);
                       //ReadWord(TempWord);
                       ReadDword(TempDword);  // number of keys in track
                       NumKeys =TempDword;
                       DEBUG(printf("NumKeys: %i\n",NumKeys););
                       CurrentObject->kfPos.NumKeys = NumKeys;//num;
                       CurrentObject->kfPos.Keys = Key_New(NumKeys);

                       CurrentObject->kfPos.Keys[        0].Prev = NULL;
                       CurrentObject->kfPos.Keys[NumKeys-1].Next = NULL;
                       if (NumKeys > 1)
                       {
                        CurrentObject->kfPos.Keys[        0].Next = &(CurrentObject->kfPos.Keys[        1]);
                        CurrentObject->kfPos.Keys[NumKeys-1].Prev = &(CurrentObject->kfPos.Keys[NumKeys-2]);
                       }
                       for (i=1;i<(NumKeys-1);i++)
                       {
                        CurrentObject->kfPos.Keys[i].Next = &(CurrentObject->kfPos.Keys[i+1]);
                        CurrentObject->kfPos.Keys[i].Prev = &(CurrentObject->kfPos.Keys[i-1]);
                       }

                       for (i=0;i<NumKeys/;i++)
                       {
                        DEBUG(printf("key: %i\n",i););
                        //ReadWord(TempWord);
                        ReadDword(TempDword);
                        DEBUG(printf("frame #: %i\n",TempDword););
                        CurrentObject->kfPos.Keys[i].Frame = TempDword;//TempWord;
                        //ReadWord(TempWord);
                        ReadSpline(&(CurrentObject->kfPos.Keys[i]));
                        ReadFloat(TempVector.x);
                        ReadFloat(TempVector.y);
                        ReadFloat(TempVector.z);
                        Vector_Swap(&TempVector);
                        DEBUG(printf("%4i: %5.2f, %5.2f, %5.2f\n",i,TempVector.x,TempVector.y,TempVector.z););
                        Vector_Copy(&(CurrentObject->kfPos.Keys[i].Val._vector),&TempVector);
                       }
                       Spline_Init(&CurrentObject->kfPos);
                      }
                      else
                      if (CurrentType == 7) // camera
                      {
                       DEBUG(printf("camera pos keyframe\n"););
                       ReadWord(Flag);
                       for (i=0;i<4;i++) ReadWord(TempWord);
                       //ReadWord(num);
                       //ReadWord(TempWord);
                       ReadDword(TempDword);
                       NumKeys = TempDword;
                       CurrentCamera->kfPos.NumKeys = NumKeys;//num;
                       CurrentCamera->kfPos.Keys = Key_New(NumKeys);
                       for (i=0;i<NumKeys/;i++)
                       {
                        //ReadWord(TempWord);
                        ReadDword(TempDword);
                        CurrentCamera->kfPos.Keys[i].Frame = TempDword;//TempWord;
                        //ReadWord(TempWord);
                        ReadSpline(&(CurrentCamera->kfPos.Keys[i]));
                        ReadFloat(TempVector.x);
                        ReadFloat(TempVector.y);
                        ReadFloat(TempVector.z);
                        Vector_Swap(&TempVector);
                        Vector_Copy(&(CurrentCamera->kfPos.Keys[i].Val._vector),&TempVector);
                       }
                       Spline_Init(&CurrentCamera->kfPos);
                      }
                      else
                      if (CurrentType == 71) // camera target
                      {
                       DEBUG(printf("camera target keyframe\n"););

                       ReadWord(Flag);
                       for (i=0;i<4;i++) ReadWord(TempWord);
                       //ReadWord(num);
                       //ReadWord(TempWord);
                       ReadDword(TempDword);
                       NumKeys = TempDword;
                       CurrentCamera->kfTarget.NumKeys = NumKeys;//num;
                       CurrentCamera->kfTarget.Keys = Key_New(NumKeys);
                       for (i=0;i<NumKeys;i++)
                       {
                        //ReadWord(TempWord);
                        ReadDword(TempDword);
                        CurrentCamera->kfTarget.Keys[i].Frame = TempDword;//TempWord;
                        //ReadWord(TempWord);
                        ReadSpline(&(CurrentCamera->kfTarget.Keys[i]));
                        ReadFloat(TempVector.x);
                        ReadFloat(TempVector.y);
                        ReadFloat(TempVector.z);
                        Vector_Swap(&TempVector);
                        Vector_Copy(&(CurrentCamera->kfTarget.Keys[i].Val._vector),&TempVector);
                       }
                       Spline_Init(&CurrentCamera->kfTarget);
                      }
                      else
                      if (CurrentType == 100) // omni light
                      {
                       DEBUG(printf("omnilight pos keyframe\n"););
                       ReadWord(Flag);
                       for (i=0;i<4;i++) ReadWord(TempWord);
                       //ReadWord(num);
                       //ReadWord(TempWord);
                       ReadDword(TempDword);
                       NumKeys = TempDword;
                       CurrentLight->kfPos.NumKeys = NumKeys;//num;
                       CurrentLight->kfPos.Keys = Key_New(NumKeys);
                       for (i=0;i<NumKeys;i++)
                       {
                        //ReadWord(TempWord);
                        ReadDword(TempDword);
                        CurrentLight->kfPos.Keys[i].Frame = TempDword;//TempWord;
                        //ReadWord(TempWord);
                        ReadSpline(&(CurrentLight->kfPos.Keys[i]));
                        ReadFloat(TempVector.x);
                        ReadFloat(TempVector.y);
                        ReadFloat(TempVector.z);
                        Vector_Swap(&TempVector);
                        Vector_Copy(&(CurrentLight->kfPos.Keys[i].Val._vector),&TempVector);
                       }
                       Spline_Init(&CurrentLight->kfPos);
                      }
                      else
                      if (CurrentType == 101) // spotlight target
                      {
                       DEBUG(printf("spotlight target keyframe\n"););

                       ReadWord(Flag);
                       for (i=0;i<4;i++) ReadWord(TempWord);
                       //ReadWord(num);
                       //ReadWord(TempWord);
                       ReadDword(TempDword);
                       NumKeys = TempDword;
                       CurrentLight->kfTarget.NumKeys = NumKeys;//num;
                       CurrentLight->kfTarget.Keys = Key_New(NumKeys);
                       for (i=0;i<NumKeys;i++)
                       {
                        //ReadWord(TempWord);
                        ReadDword(TempDword);
                        CurrentLight->kfTarget.Keys[i].Frame = TempDword;//TempWord;
                        //ReadWord(TempWord);
                        ReadSpline(&(CurrentLight->kfTarget.Keys[i]));
                        ReadFloat(TempVector.x);
                        ReadFloat(TempVector.y);
                        ReadFloat(TempVector.z);
                        Vector_Swap(&TempVector);
                        Vector_Copy(&(CurrentLight->kfTarget.Keys[i].Val._vector),&TempVector);
                       }
                       Spline_Init(&CurrentLight->kfTarget);
                      }
                      else
                      if (CurrentType == 102) // spotlight
                      {
                       DEBUG(printf("spotlight pos\n"););
                       ReadWord(Flag);
                       for (i=0;i<4;i++) ReadWord(TempWord);
                       //ReadWord(num);
                       //ReadWord(TempWord);
                       ReadDword(TempDword);
                       NumKeys = TempDword;
                       CurrentLight->kfPos.NumKeys = NumKeys;//num;
                       CurrentLight->kfPos.Keys = Key_New(NumKeys);
                       for (i=0;i<NumKeys;i++)
                       {
                        //ReadWord(TempWord);
                        ReadDword(TempDword);
                        CurrentLight->kfPos.Keys[i].Frame = TempDword;//TempWord;
                        //ReadWord(TempWord);
                        ReadSpline(&(CurrentLight->kfPos.Keys[i]));
                        ReadFloat(TempVector.x);
                        ReadFloat(TempVector.y);
                        ReadFloat(TempVector.z);
                        Vector_Swap(&TempVector);
                        Vector_Copy(&(CurrentLight->kfPos.Keys[i].Val._vector),&TempVector);
                       }
                       Spline_Init(&CurrentLight->kfPos);
                      }
                      break;

        case 0xb021 : // Rot (KeyFramer/TrackInfo/Rot)

                       ReadWord(Flag);
                       for (i=0;i<4;i++) ReadWord(TempWord);
                       //ReadWord(num);
                       //ReadWord(TempWord);
                       ReadDword(TempDword);
                       NumKeys = TempDword;
                       DEBUG(printf("NumKeys: %i\n",NumKeys););
                       CurrentObject->kfRot.NumKeys = NumKeys;//num;
                       CurrentObject->kfRot.Keys = Key_New(NumKeys);
                       for (i=0;i<NumKeys;i++)
                       {
                        DEBUG(printf("key: %i\n",i););
                        //ReadWord(TempWord);
                        ReadDword(TempDword);
                        DEBUG(printf("frame #: %i\n",TempDword););
                        CurrentObject->kfRot.Keys[i].Frame = TempDword;//TempWord;
                        //ReadWord(TempWord);
                        ReadSpline(&(CurrentObject->kfRot.Keys[i]));
                        ReadFloat(TempQuat[qW]);
                        ReadFloat(TempQuat[qX]);
                        ReadFloat(TempQuat[qY]);
                        ReadFloat(TempQuat[qZ]);
                        Quat_Swap(&TempQuat);
                        DEBUG(printf("%4i: %5.2f, %5.2f, %5.2f : %5.2f\n",i,TempQuat[qX],TempQuat[qY],TempQuat[qZ],TempQuat[qW]););
                        Quat_Copy(&(CurrentObject->kfRot.Keys[i].Val._quat),&TempQuat);
                       }
                       Spline_Init(&CurrentObject->kfRot);
                       break;

       case 0xb022 : // Scale (KeyFramer/TrackInfo/Scale)

                       ReadWord(Flag);
                       for (i=0;i<4;i++) ReadWord(TempWord);
                       //ReadWord(num);
                       //ReadWord(TempWord);
                       ReadDword(TempDword);
                       NumKeys = TempDword;
                       DEBUG(printf("NumKeys: %i\n",NumKeys););
                       CurrentObject->kfScale.NumKeys = NumKeys;//num;
                       CurrentObject->kfScale.Keys = Key_New(NumKeys);
                       for (i=0;i<NumKeys;i++)
                       {
                        DEBUG(printf("key: %i\n",i););
                        //ReadWord(TempWord);
                        ReadDword(TempDword);
                        DEBUG(printf("frame #: %i\n",TempDword););
                        CurrentObject->kfScale.Keys[i].Frame = TempDword;//TempWord;
                        //ReadWord(TempWord);
                        ReadSpline(&(CurrentObject->kfScale.Keys[i]));
                        ReadFloat(TempVector.x);
                        ReadFloat(TempVector.y);
                        ReadFloat(TempVector.z);
                        Vector_Swap(&TempVector);
                        Vector_Copy(&(CurrentObject->kfScale.Keys[i].Val._vector),&TempVector);
                        DEBUG(printf("%4i: %5.2f, %5.2f, %5.2f\n",i,TempVector.x,TempVector.y,TempVector.z););
                       }
                       Spline_Init(&CurrentObject->kfScale);
                       break;

        case 0xb023 : // Fov (KeyFramer/TrackInfo/Fov)

                       ReadWord(Flag);
                       for (i=0;i<4;i++) ReadWord(TempWord);
                       //ReadWord(num);
                       //ReadWord(TempWord);
                       ReadDword(TempDword);
                       NumKeys = TempDword;
                       CurrentCamera->kfFov.NumKeys = NumKeys;//num;
                       CurrentCamera->kfFov.Keys = Key_New(NumKeys);
                       for (i=0;i<NumKeys;i++)
                       {
                        //ReadWord(TempWord);
                        ReadDword(TempDword);
                        CurrentCamera->kfFov.Keys[i].Frame = TempDword;//TempWord;
                        //ReadWord(TempWord);
                        ReadSpline(&(CurrentCamera->kfFov.Keys[i]));
                        ReadFloat(TempFloat);
                        CurrentCamera->kfFov.Keys[i].Val._float = TempFloat;
                       }
                       Spline_Init(&CurrentCamera->kfFov);
                       break;

        case 0xb024 : // Roll (KeyFramer/TrackInfo/Roll)

                       ReadWord(Flag);
                       for (i=0;i<4;i++) ReadWord(TempWord);
                       //ReadWord(num);
                       //ReadWord(TempWord);
                       ReadDword(TempDword);
                       NumKeys = TempDword;
                       CurrentCamera->kfRoll.NumKeys = NumKeys;//num;
                       CurrentCamera->kfRoll.Keys = Key_New(NumKeys);
                       for (i=0;i<NumKeys;i++)
                       {
                        //ReadWord(TempWord);
                        ReadDword(TempDword);
                        CurrentCamera->kfRoll.Keys[i].Frame = TempDword;//TempWord;
                        //ReadWord(TempWord);
                        ReadSpline(&(CurrentCamera->kfRoll.Keys[i]));
                        ReadFloat(TempFloat);
                        CurrentCamera->kfRoll.Keys[i].Val._float = TempFloat;
                       }
                       Spline_Init(&CurrentCamera->kfRoll);
                       break;

        case 0xb030 : // KeyFramer/TrackInfo/TrackObjNumber
                      ReadWord(CurrentObjectKF);
                      break;
 */

     // ÄÄÄ

     default : iBufferPointer += (Chunk.Size-6);
               BufferPtr += (Chunk.Size-6);
    }
   }
  }

 /*

  DEBUG(printf("þ Loading ok.. Now doing some stuff\n"););
  // --- Set up hierarchy ---------------------------------------------------
  DEBUG(printf("þ Set up hierarchy\n"););
  oo1 = mNodes;
  while (oo1 != NULL)
  {
   num = oo1->ParentId;
   if (num != -1)
   {
    oo2 = Scene->Objects;
    for (;;)
    {
     if (oo2==NULL)
     {
      DEBUG(printf("harra-baillj #2! Didn't find object # %i in ObjectList\n",num););
      exit(1);
     }
     if (num == oo2->Id) goto exitloop;
     oo2=oo2->Next;
    }
    exitloop:
    oo1->Parent = oo2;
   }
   oo1 = oo1->Next;
  }
  // --- Transform object from world -> object -> pivot ---------------------
  DEBUG(printf("þ Transform objects to object/pivot space\n"););

  Oo = Scene->Objects;
  while (Oo != NULL)
  {
   Matrix_Identity(&m1);
   Matrix_Inverse(&m1,Oo->Matrix3DS);
   for (i=0;i<Oo->NumVertices;i++)
   {
    Vector_Trans(&TempVector,Oo->Vertices[i].Pos,&m1);
    Vector_Sub(&(Oo->Vertices[i].Pos),&TempVector,&(Oo->Pivot));
    //Oo->Vertices[i].Pos.x = TempVector.x - Oo->Pivot.x;
    //Oo->Vertices[i].Pos.y = TempVector.y - Oo->Pivot.y;
    //Oo->Vertices[i].Pos.z = TempVector.z - Oo->Pivot.z;
   }
   Oo = Oo->Next;
  }
 */
  return error;
 }

