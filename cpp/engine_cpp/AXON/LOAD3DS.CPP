 #include <stdio.h>   // file stuff
 #include <io.h>      // filelength
 #include <stdlib.h>  // malloc
 #include <string.h>  // strncpy

 #include "engine.hpp"

 // ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

 typedef struct tChunk
  {
   unsigned short int Id;
   unsigned int       Size;
  } tChunk;

 // --- "global" vars so all sub-functions may access them ---

 char   *BufferPtr;
 tChunk  Chunk;
 int     iBufferPointer;

 tScene  Scene;

 // ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

 // --- some defines to make things easier & cleaner ---

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

 // Convert spaces to underline
 #define ReadAsciiz(a) {         \
  i = 0;                         \
  do                             \
  {                              \
   ReadChar(j);                  \
   if (j == ' ') j = '_';        \
   a[i] = j;                     \
   i++;                          \
  } while ((a[i-1]!=0)&&(i<20)); \
 }

 // ÄÄÄ
void ReadSpline(tKey *key)
 {
  int ii;
  float dat;
  unsigned short int ffflag;

  DEBUG(printf("Spline reader:\n"););
  key->tension    = 0.0;
  key->continuity = 0.0;
  key->bias       = 0.0;
  key->easeto     = 0.0;
  key->easefrom   = 0.0;

  ReadWord(ffflag);
  key->flags = ffflag;
  DEBUG(printf("flags: %6x\n",ffflag););
  for (ii = 0; ii < 16; ii++)
  {
   if ((ffflag & (1 << ii))!=0)
   {
    ReadFloat(dat);
    DEBUG(printf("float #%i: %f\n",ii,dat););
    switch (ii)
    {
     case 0: key->tension    = dat; break;
     case 1: key->continuity = dat; break;
     case 2: key->bias       = dat; break;
     case 3: key->easeto     = dat; break;
     case 4: key->easefrom   = dat;
    }
   }
  }
 }

 // ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

 // Parse previously loaded 3DS file
 // In  : buffer     : ptr to previously loaded 3DS file
 //       BufferSize : size of buffer / length of 3DS file
 //       Scene      : Scene to load
void Parse3DS( char *buffer, int BufferSize )
 {
  // ÄÄÄ variables ÄÄÄ

  int                i,j,k,num;
  float              W,X,Y,Z,U,V;
  int                A,B,C,flags;
  unsigned char      TempChar;
  unsigned short int TempWord;
  unsigned int       TempDword;
  float              TempFloat;
  char               TempString[20];
  float              TempMatrix[16];
  char               curName[20];
  int                curType;
  int                curMaterialType;
  int                curObjectKF; // Object number (TrackObjNumber)
  int                NumKeys;
  tColor            *curColor;
  tVector           *curVector;
  tVertex           *curvertex;
  tObject           *curObject;
  tLight            *curLight;
  tCamera           *curCamera;
  tMaterial         *curMaterial;
  tKey              *curKey;
  tKeyFrame         *curKeyFrame;

  // ÄÄÄ

  iBufferPointer = 0;
  BufferPtr = buffer;
  // Start reading chunks
  ReadChunk;
  if (Chunk.Id != 0x4d4d) return; // NULL;
  else
  {
   // Valid 3DS file
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
                   ReadAsciiz(curName);
                   DEBUG(printf("Object name: %s\n",curName););
                   break;

     case 0x4010 : // ObjMesh/ObjBlock/ObjVisLofter
                   DEBUG(printf("Object visible in 3D lofter\n"););
                   curObject->hidden = 1;
                   break;

     case 0x4012 : // ObjMesh/ObjBlock/NoCastShadow
                   DEBUG(printf("Object doesn't cast (shadow?)\n"););
                   curObject->nocastshadows = 1;
                   // bool = on
                   break;

     case 0x4013 : // ObjMesh/ObjBlock/ObjMatte
                   DEBUG(printf("Object background matte\n"););
                   curObject->matte = 1;
                   // bool = on
                   break;


     case 0x4017 : // ObjMesh/ObjBlock/NoReceiveShadow
                   DEBUG(printf("Object doesn't receive shadows\n"););
                   curObject->noreceiveshadows = 1;
                   // bool = on
                   break;

     // 4014 - bool, fastdraw
     // 4016 - bool, frozen

     // ÄÄÄ TriMesh ÄÄÄ

     case 0x4100 : // ObjMesh/ObjBlock/TriMesh
                   DEBUG(printf("Triangle mesh\n"););
                   curType = 1;
                   curObject = new tObject;
                   // TODO Scene.addnode(curObject);
                   strncpy(curObject->name,curName,20);
                   curObject->id = -1;
                   curObject->parentid = -1;
                   break;

     case 0x4110 : // ObjMesh/ObjBlock/TriMesh/VertList

                   // word     Number of vertices
                   //  Then, for each vertex:
                   // vector   Position

                   ReadWord(num);
                   DEBUG(printf("NumVertices: %i\n",num););
                   curObject->numvertices = num;
                   curObject->vertices = new tVertex[num];
                   for (i=0;i<num;i++)
                   {
                    ReadFloat(X);
                    ReadFloat(Y);
                    ReadFloat(Z);
                    DEBUG(printf("%4i: %5.2f, %5.2f, %5.2f\n",i,X,Y,Z););
                    curObject->vertices[i].pos.x = X;
                    curObject->vertices[i].pos.y = Y;
                    curObject->vertices[i].pos.z = Z;
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
                   curObject->numpolygons = num;
                   curObject->polygons = new tPolygon[num];
                   for (i=0;i<num;i++)
                   {
                    ReadWord(A);
                    ReadWord(B);
                    ReadWord(C);
                    ReadWord(flags);
                    DEBUG(printf("%4i: %4i, %4i, %4i : %4x\n",i,A,B,C,flags););
                    // 01 CA, 02 BC, 04 AB
                    // 08 U-wrap, 10 V-wrap
                    curObject->polygons[i].flags = flags;
                    curObject->polygons[i].a = A;
                    curObject->polygons[i].b = B;
                    curObject->polygons[i].c = C;
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

                   ReadAsciiz(TempString);
                   // TODO: CurMaterial = Material_FindByName(Scene,&TempString);
                   ReadWord(num);
                   // if box assigned material : 0
                   for (i=0;i<num;i++)
                   {
                    ReadWord(TempWord);
                    // curObject->polygons[TempWord].material = CurMaterial;
                   }
                   break;

     case 0x4140 : // ObjMesh/ObjBlock/TriMesh/MapList

                   // word     Number of vertices
                   //  Then, for each vertex
                   // float    U coordinate
                   // float    V coordinate

                   // curObject->Flags |= OBJflag_mapped;
                   ReadWord(num);
                   //for (i=0;i<num;i++) curObject->vertices[i].normal = &(curObject->Normals[i]);
                   for (i=0;i<num;i++)
                   {
                    ReadFloat(U);
                    ReadFloat(V);
                    curObject->vertices[i].u = U;
                    curObject->vertices[i].v = V;
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

                   for (i=0;i<curObject->numpolygons;i++)
                   {
                    ReadDword(TempDword);
                    curObject->polygons[i].smoothing = TempDword;
                   }
                   break;

     case 0x4160 : // ObjMesh/ObjBlock/TriMesh/matrix

                   // 3-by-4 matrix describing object orientation. Used by
                   // the Keyframer to determine object rotation and center.

                   // Local coordinate system (the matrix)
                   // vector   X1
                   // vector   X2
                   // vector   X3
                   // vector   O
                   // X1, X2 and X3 represent the axes, O the origin.

                   ReadFloat(curObject->matrix3DS[ 0]);
                   ReadFloat(curObject->matrix3DS[ 1]);
                   ReadFloat(curObject->matrix3DS[ 2]);

                   ReadFloat(curObject->matrix3DS[ 3]);
                   ReadFloat(curObject->matrix3DS[ 4]);
                   ReadFloat(curObject->matrix3DS[ 5]);

                   ReadFloat(curObject->matrix3DS[ 6]);
                   ReadFloat(curObject->matrix3DS[ 7]);
                   ReadFloat(curObject->matrix3DS[ 8]);

                   ReadFloat(curObject->matrix3DS[ 9]);
                   ReadFloat(curObject->matrix3DS[10]);
                   ReadFloat(curObject->matrix3DS[11]);
                   break;

     case 0x4165 : // Object color in editor (uninteresting??)
                   // byte     Color

                   ReadChar(TempChar);
                   break;

     // 0x4190 = box material, 6 strings (1 to 16 characters plus NULL) for
     // front, back, left, right, top, bottom

     // ÄÄÄ Light ÄÄÄ

     case 0x4600 : // ObjBlock/Light
                   // vector   Position

                   curType = 6;
                   curLight = new tLight;

                   // TODO Scene.addnode(curLight);

                   curLight->type = LIGHTtype_omni;       // default omni
                   strncpy(curLight->name,curName,20);
                   curLight->strength = 1;                  // ???
                   curLight->maxrange = 256;                // ???
                   curCamera->id = -1;
                   // curCamera->ParentId = -1;
                   ReadFloat(X);
                   ReadFloat(Y);
                   ReadFloat(Z);
                   //Vector_Swap(&TempVector);
                   curLight->pos.x = X;
                   curLight->pos.y = Y;
                   curLight->pos.z = Z;
                   break;

     case 0x4610 : // ObjBlock/Light/SpotLight
                   // vector   Target
                   // float    HotSpot
                   // float    FallOff
                   // If this chunk is found, the light is a spot light and
                   // not an omni directional light.

                   curLight->type = LIGHTtype_spot;
                   // Set spotlight flag!
                   ReadFloat(X); // TargetPosX (in global coordinates)
                   ReadFloat(Y); // TargetPosY
                   ReadFloat(Z); // TargetPosZ
                   curLight->spot_target.x = X;
                   curLight->spot_target.y = Y;
                   curLight->spot_target.z = Z;

                   ReadFloat(TempFloat);    // HotSpot cone angle in degrees (1-160)
                   curLight->spot_hotspot = TempFloat;
                   ReadFloat(TempFloat);    // FallOff --"--
                   curLight->spot_falloff = TempFloat;
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
                   // float    Roll angle in degrees (-360..360)

                   ReadFloat(TempFloat);
                   curLight->spot_roll = TempFloat;
                   break;

     // 4620 - (Direct) Light off, boolean
     // 465a - outer range, float
     // 4659 - inner range, float
     // 465b - light intensity multiplier, float
     // 4654 - Illumination exclusion, string (object to exclude)
     // 4625 - light attenuation (boolean), indicates the use of attenuation

     // ÄÄÄ Camera ÄÄÄ

     case 0x4700 : // ObjBlock/Camera
                   // vector   Position
                   // vector   Target
                   // float    Bank (degree)
                   // float    Lens

                   curType = 7;
                   curCamera = new tCamera;
                   // TODO Scene.addnode(curCamera);
                   strncpy(curCamera->name,curName,20);
                   ReadFloat(X);
                   ReadFloat(Y);
                   ReadFloat(Z);
                   curCamera->pos.x = X;
                   curCamera->pos.y = Y;
                   curCamera->pos.z = Z;
                   ReadFloat(X);
                   ReadFloat(Y);
                   ReadFloat(Z);
                   curCamera->target.x = X;
                   curCamera->target.y = Y;
                   curCamera->target.z = Z;
                   ReadFloat(TempFloat);
                   curCamera->roll = TempFloat;     // degrees
                   ReadFloat(TempFloat);
                   curCamera->fov = TempFloat;      // focal length in millimeters
                   break;

     // 4720 - camera ranges (2 floats) 1=near radius of effect, 2=far radius of effect)

     // ÄÄÄ Material ÄÄÄ

     case 0xafff : // Material
                   break;

     case 0xa000 : // Material/MaterialName
                   curMaterial = new tMaterial;
                   // TODO Scene.addnode(curMaterial);
                   curMaterial->flags = 0;
                   ReadAsciiz(curMaterial->name);
                   break;

     // currently only supports one map per material.

     case 0xa200 : // TextureMap (Material/TextureMap)
                   curMaterialType = 1;
                   curMaterial->flags |= MATflag_texture1;
                   break;

     case 0xa210 : // OpacityMap (Material/OpacityMap)
                   curMaterialType = 2;
                   curMaterial->flags |= MATflag_opacity;
                   break;
     case 0xa220 : // ReflectionMap (Material/ReflectionMap)
                   curMaterialType = 3;
                   curMaterial->flags |= MATflag_reflection;
                   break;
     case 0xa230 : // BumpMap (Material/BumpMap)
                   curMaterialType = 4;
                   curMaterial->flags |= MATflag_bump;
                   break;
     case 0xa300 : // MapFile (Material/TextureMap/MapFile)
                   switch(curMaterialType)
                   {
                    case 1 : ReadAsciiz(curMaterial->map_texture1);
                             break;
                    case 2 : ReadAsciiz(curMaterial->map_opacity);
                             break;
                    case 3 : ReadAsciiz(curMaterial->map_reflection);
                             break;
                    case 4 : ReadAsciiz(curMaterial->map_bump);
                             break;
                   }
                   break;

     // ÄÄÄ Keyframer ÄÄÄ

     // b00a
     //   short  = rev. level of keyframe section
     //   string = 12+NULL
     //   long   = anim length in frames

     case 0xb000 : // Keyframer
                   // Scene->SCENE_anim = 1;
                   DEBUG(printf("---------- keyframer ----------\n"););
                   break;

     //case 0xb001 : // Keyframer/AmbientLight
     // Name must always be "$AMBIENT$" and parent index must always be -1.

     case 0xb002 : // KeyFramer/TrackInfo
                   curType = 1; // object
                   break;
     case 0xb003 : // KeyFramer/CameraTrackInfo
                   curType = 7; // Camera pos
                   break;
     case 0xb004 : // KeyFramer/CameraTarget
                   curType = 71; // camera target
                   break;
     case 0xb005 : // KeyFramer/OmniLight
                   curType = 100;
                   break;
     case 0xb006 : // KeyFramer/SpotLightTarget
                   curType = 101;
                   break;
     case 0xb007 : // KeyFramer/SpotLight
                   curType = 102;
                   break;

     case 0xb008 : // Hierarchy info
                   ReadDword(TempDword);
                   Scene.startframe = TempDword;
                   ReadDword(TempDword);
                   Scene.endframe = TempDword;
                   break;

     case 0xb010 : // TrackObjName (KeyFramer/TrackInfo/TrackObjName)
                   // strz     Object Name
                   // word     Flag1
                   //          * Bit 2  (0x4)    Node render object hide
                   //          * Bit 3  (0x8)    Light off in keyframer
                   //          * Bit 11 (0x800)  Hide node in keyframer
                   //          * Bit 12 (0x1000) Fast draw node in keyframer
                   //          * Bit 14 (0x4000) Node tag is not an instance
                   // word     Flag2
                   //          * Bit 1 (0x1)  Show path
                   //          * Bit 2 (0x2)  Object node uses autosmoothing
                   //          * Bit 3 (0x4)  Object node is frozen
                   //          * Bit 5 (0x10) Object motion blur
                   //          * Bit 6 (0x20) Morph materials
                   //          * Bit 7 (0x40) Morph mesh
                   // word     Hierarchy father, link to the parent object
                   //          (-1 for none)

                   ReadAsciiz(TempString);
                   DEBUG(printf("object : %s\n",TempString););


                   ReadAsciiz(TempString);
                   ReadWord(TempWord);   // Flag1
                   ReadWord(TempWord);   // Flag2
                   ReadWord(num);        // hierarchy father

                   // Parent index. An index of 65535 indicates no parent.
                   // OBJECT_NODE_TAGs can be parents of any other node and
                   // CAMERA_NODE_TAGs can be parents of SPOTLIGHT_NODE_TAGs
                   // and LIGHT_NODE_TAGs. The index is computed by the order
                   // the nodes occur in the KFDATA section. This order, however,
                   // can be overridden by the use of NODE_ID tags

                   switch(curType)
                   {
                    // ambient light....

                    case   1: // object

                              //ReadAsciiz(TempString);
                              DEBUG(printf("object : %s\n",TempString););
                              if (strncmp("$$$DUMMY",TempString,20)==0)
                              {
                               // add $$$DUMMY object
                               // TODO Scene.numobjects++;
                               // TODO curObject = Scene.addobject();
                               curObject->dummy = 1;
                               strncpy(curObject->name,TempString,20);
                               curObject->id = -1;
                               curObject->parentid = -1;
                              }
                              else
                              {
                               // TODO curObject = Object_FindByName(Scene,&TempString);
                              }
                              //ReadWord(TempWord);   // Flag1
                              //ReadWord(TempWord);   // Flag2
                              //ReadWord(num);        // hierarchy father
                              curObject->parentid = num;  // Hierarchynumber of parent
                              curObject->id = curObjectKF;
                              break;

                    case   7: // camera
                    case  71: // camera target
                              //ReadAsciiz(TempString);
                              DEBUG(printf("camera : %s\n",TempString););
                              //curCamera = Camera_FindByName(Scene,&TempString);
                              //ReadWord(TempWord);
                              //ReadWord(TempWord);
                              //ReadWord(num);
                              curCamera->parentid = num;  // Hierarchynumber of parent
                              curCamera->id = curObjectKF;
                              break;

                    case 100: // omni light
                    case 101: // spotlight target
                    case 102: // spotlight
                              //ReadAsciiz(TempString);
                              DEBUG(printf("light : %s\n",TempString););
                              //curLight = Light_FindByName(Scene,&TempString);
                              //ReadWord(TempWord);
                              //ReadWord(TempWord);
                              //ReadWord(num);
                              curLight->parentid = num;  // Hierarchynumber of parent
                              curLight->id = curObjectKF;
                              break;
                   }
                   break;

     // 0xb011 - mesh instance name
     // Instance name of a mesh or dummy object. Can be the same or different
     // from the name listed in NODE_HDR. Instance names must be unique.

     // 0xb015, float  - morph smooth, smoothing angle for morphing objects
     //                                (0-180)

     // 0xb014 - bounding box, 2 vectors, min/max bounding point

     case 0xb013 : // TrackPivot (KeyFramer/TrackInfo/TrackPivot)
                   // vector   Pivot point
                   // Rotational pivoting point and scaling center
                   ReadFloat(X);
                   ReadFloat(Y); // Offset from center of BOUNDBOX
                   ReadFloat(Z);
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
                   DEBUG(printf("pos keyframe\n"););
                   ReadWord(flags);
                   ReadWord(flags);
                   for (i=0;i<4;i++) ReadWord(TempWord);
                   ReadDword(TempDword);  // number of keys in track
                   NumKeys = TempDword;
                   DEBUG(printf("NumKeys: %i\n",NumKeys););

                   curKeyFrame = new tKeyFrame;
                   curKeyFrame->numkeys = NumKeys;
                   curKeyFrame->keys = new tKey[NumKeys];

                   for (i=0;i<NumKeys;i++)
                   {
                    DEBUG(printf("key: %i\n",i););
                    ReadDword(TempDword);
                    DEBUG(printf("frame #: %i\n",TempDword););
                    curKeyFrame->keys[i].frame = TempDword;//TempWord;
                    ReadSpline(&(curKeyFrame->keys[i]));
                    ReadFloat(X);
                    ReadFloat(Y);
                    ReadFloat(Z);
                    DEBUG(printf("%4i: %5.2f, %5.2f, %5.2f\n",i,X,Y,Z););
                    curKeyFrame->keys[i].data.f[0] = X;
                    curKeyFrame->keys[i].data.f[1] = Y;
                    curKeyFrame->keys[i].data.f[2] = Z;
                   }
                   // TODO: Spline_Init(curKeyFrame);
                   switch (curType)
                   {
                    case   1: curObject->kfPos = curKeyFrame;
                              break;
                    case   7: curCamera->kfPos = curKeyFrame;
                              break;
                    case  71: curCamera->kfTarget = curKeyFrame;
                              break;
                    case 100: curLight->kfPos = curKeyFrame;
                              break;
                    case 101: curLight->kfTarget = curKeyFrame;
                              break;
                   }
                   break;

     case 0xb021 : // Rot (KeyFramer/TrackInfo/Rot)
                   DEBUG(printf("rot keyframe\n"););
                   ReadWord(flags);
                   ReadWord(flags);
                   for (i=0;i<4;i++) ReadWord(TempWord);
                   ReadDword(TempDword);  // number of keys in track
                   NumKeys = TempDword;
                   DEBUG(printf("NumKeys: %i\n",NumKeys););
                   curKeyFrame = new tKeyFrame;
                   curKeyFrame->numkeys = NumKeys;
                   curKeyFrame->keys = new tKey[NumKeys];
                   for (i=0;i<NumKeys;i++)
                   {
                    DEBUG(printf("key: %i\n",i););
                    ReadDword(TempDword);
                    DEBUG(printf("frame #: %i\n",TempDword););
                    curKeyFrame->keys[i].frame = TempDword;//TempWord;
                    ReadSpline(&(curKeyFrame->keys[i]));
                    ReadFloat(W);
                    ReadFloat(X);
                    ReadFloat(Y);
                    ReadFloat(Z);
                    DEBUG(printf("%4i: %5.2f, %5.2f, %5.2f, %5.2f\n",i,X,Y,Z,W););
                    curKeyFrame->keys[i].data.f[0] = X;
                    curKeyFrame->keys[i].data.f[1] = Y;
                    curKeyFrame->keys[i].data.f[2] = Z;
                    curKeyFrame->keys[i].data.f[3] = W;
                   }
                   // TODO: Spline_Init(curKeyFrame);
                   curObject->kfRot = curKeyFrame;
                   break;

     case 0xb022 : // Scale (KeyFramer/TrackInfo/Scale)
                   DEBUG(printf("scale keyframe\n"););
                   ReadWord(flags);
                   ReadWord(flags);
                   for (i=0;i<4;i++) ReadWord(TempWord);
                   ReadDword(TempDword);  // number of keys in track
                   NumKeys = TempDword;
                   DEBUG(printf("NumKeys: %i\n",NumKeys););

                   curKeyFrame = new tKeyFrame;
                   curKeyFrame->numkeys = NumKeys;
                   curKeyFrame->keys = new tKey[NumKeys];

                   for (i=0;i<NumKeys;i++)
                   {
                    DEBUG(printf("key: %i\n",i););
                    ReadDword(TempDword);
                    DEBUG(printf("frame #: %i\n",TempDword););
                    curKeyFrame->keys[i].frame = TempDword;//TempWord;
                    ReadSpline(&(curKeyFrame->keys[i]));
                    ReadFloat(X);
                    ReadFloat(Y);
                    ReadFloat(Z);
                    DEBUG(printf("%4i: %5.2f, %5.2f, %5.2f\n",i,X,Y,Z););
                    curKeyFrame->keys[i].data.f[0] = X;
                    curKeyFrame->keys[i].data.f[1] = Y;
                    curKeyFrame->keys[i].data.f[2] = Z;
                   }
                   // TODO: Spline_Init(curKeyFrame);
                   curObject->kfScale = curKeyFrame;
                   break;

     case 0xb023 : // Fov (KeyFramer/TrackInfo/Fov)
                   DEBUG(printf("fov keyframe\n"););
                   ReadWord(flags);
                   ReadWord(flags);
                   for (i=0;i<4;i++) ReadWord(TempWord);
                   ReadDword(TempDword);  // number of keys in track
                   NumKeys = TempDword;
                   DEBUG(printf("NumKeys: %i\n",NumKeys););

                   curKeyFrame = new tKeyFrame;
                   curKeyFrame->numkeys = NumKeys;
                   curKeyFrame->keys = new tKey[NumKeys];

                   for (i=0;i<NumKeys;i++)
                   {
                    DEBUG(printf("key: %i\n",i););
                    ReadDword(TempDword);
                    DEBUG(printf("frame #: %i\n",TempDword););
                    curKeyFrame->keys[i].frame = TempDword;//TempWord;
                    ReadSpline(&(curKeyFrame->keys[i]));
                    ReadFloat(TempFloat);
                    DEBUG(printf("%4i: %5.2f\n",i,TempFloat););
                    curKeyFrame->keys[i].data.f[0] = X;
                   }
                   // TODO: Spline_Init(curKeyFrame);
                   curCamera->kfFov = curKeyFrame;
                   break;

     case 0xb024 : // Roll (KeyFramer/TrackInfo/Roll)
                   DEBUG(printf("roll keyframe\n"););
                   ReadWord(flags);
                   ReadWord(flags);
                   for (i=0;i<4;i++) ReadWord(TempWord);
                   ReadDword(TempDword);  // number of keys in track
                   NumKeys = TempDword;
                   DEBUG(printf("NumKeys: %i\n",NumKeys););

                   curKeyFrame = new tKeyFrame;
                   curKeyFrame->numkeys = NumKeys;
                   curKeyFrame->keys = new tKey[NumKeys];

                   for (i=0;i<NumKeys;i++)
                   {
                    DEBUG(printf("key: %i\n",i););
                    ReadDword(TempDword);
                    DEBUG(printf("frame #: %i\n",TempDword););
                    curKeyFrame->keys[i].frame = TempDword;//TempWord;
                    ReadSpline(&(curKeyFrame->keys[i]));
                    ReadFloat(TempFloat);
                    DEBUG(printf("%4i: %5.2f\n",i,TempFloat););
                    curKeyFrame->keys[i].data.f[0] = X;
                   }
                   // TODO: Spline_Init(curKeyFrame);
                   curCamera->kfRoll = curKeyFrame;
                   break;

     // b027 - hotspot, float
     // b028 - falloff, float
     // b029 - hide, toggle...


     case 0xb030 : // KeyFramer/TrackInfo/TrackObjNumber
                   ReadWord(curObjectKF);
                   break;

     // Node ordering information. This chunk is used to override the node
     // order that occurs in the KFDATA section. Node order is used to identify
     // the hierarchical parents referred to by the NODE_HDR chunk. Occurs only
     // once per node tag and is optional.  Node tags without NODE_ID chunks
     // are considered to have a node id of 65535.

     // Node order ID. Valid range is from 0 to 65535. Node ID's do not need to
     // be sequential or continuous. Identical node ids are sorted in the order
     // they occur in the file. AMBIENT_NODE_TAGs must always have a node ID of
     // 65535.

     // ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

     default     : iBufferPointer += (Chunk.Size-6);
                   BufferPtr      += (Chunk.Size-6);

    }
   }
  }

  // --- Set up hierarchy ---------------------------------------------------

  /*

  DEBUG(printf("ş Set up hierarchy\n"););
  oo1 = Scene.objects;
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

  */

  // --- Transform object from world -> object -> pivot ---------------------

  /*

  DEBUG(printf("ş Transform objects to object/pivot space\n"););

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

 }

 // ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

void Load3DS( char *filename )
 {
  FILE *fp;
  char *buffer;
  int filesize;

  fp = fopen(filename,"rb");
  filesize = filelength(fileno(fp));
  buffer = (char *)malloc(filesize);
  fread(buffer,1,filesize,fp);
  fclose(fp);
  Parse3DS( buffer, filesize );
  free(buffer);
 }

 // ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

void main(int argc, char **argv)
 {
  if (argc != 2)
  {
   printf("%s <3DS-file>\n",argv[0]);
   exit(1);
  }
  printf("# Loading & parsing: %s\n\n",argv[1]);
  Load3DS(argv[1]);
 }

