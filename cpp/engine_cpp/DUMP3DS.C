 // 3DS file loader, parser & dumper... ¸ 1999 - Axon
 // Planned to do a full parse of the 3DS file format, and dump to a nice
 // formatted textfile...

 // faen, glemte av at en tNode ikke kan aksessere f.eks tObject data, etc...

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

 // "global" vars so all sub-functions may access them

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

 // ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ


 // Parse previously loaded 3DS file
 // In  : buffer     : ptr to previously loaded 3DS file
 //       BufferSize : size of buffer / length of 3DS file
 //       Scene      : Scene to load
void Parse3DS( char *buffer, int BufferSize )
 {

  // --- variables ---

  int       i,j,k;
  float     X,Y,Z,W;
  float     U,V;
  short int A,B,C,Flag;
  short int num;
  int       NumKeys;

  unsigned char      TempChar;
  unsigned short int TempWord;
  unsigned int       TempDword;
  float              TempFloat;
  char               TempString[20];
  float              TempMatrix[16];

  char       curName[20]; // ObjBlock name (TriMesh, Light, Camera)
  int        curMaterialType;
  int        curType;     // 1 = Object, 6 = Light, 7 = Camera;
  int        curObjectKF; // Object number (TrackObjNumber)
  tObject   *curObject;
  tObject   *LastObject;
  tLight    *curLight;
  tMaterial *curMaterial;
  tCamera   *curCamera;

  tVertex  *curVertex;
  tPolygon *curPolygon;

  tNode    *curNode;

  // ---

  iBufferPointer = 0;
  BufferPtr = buffer;

  // ÄÄÄ Start reading chunks ÄÄÄ

  ReadChunk;
  if (Chunk.Id != 0x4d4d) return;// NULL;
  else
  {

   // ÄÄÄ Valid 3DS file ÄÄÄ

   while (iBufferPointer < BufferSize)
   {
    ReadChunk;
    printf("Chunk Id: %6x\n",Chunk.Id);
    switch(Chunk.Id)
    {
     // case 0x0002 : 3DS version (10 bytes)
     case 0x3d3d : // ObjMesh
                   break;

     case 0x4000 : // ObjMesh/ObjBlock
                   ReadAsciiz(curName);
                   DEBUG(printf("Object name: %s\n",curName););
                   break;

     case 0x4010 : // ObjMesh/ObjBlock/ObjHidden
                   DEBUG(printf("Object is hidden\n"););
                   curObject->hidden = true;
                   break;

     case 0x4012 : // ObjMesh/ObjBlock/NoCastShadow
                   DEBUG(printf("Object doesn't cast (shadow?)\n"););
                   curObject->nocastshadows = true;
                   // bool = on
                   break;

     case 0x4013 : // ObjMesh/ObjBlock/ObjMatte
                   DEBUG(printf("Object matte\n"););
                   curObject->matte = true;
                   // bool = on
                   break;

     case 0x4017 : // ObjMesh/ObjBlock/NoReceiveShadow
                   DEBUG(printf("Object doesn't receive shadows\n"););
                   curObject->noreceiveshadows = true;
                   // bool = on
                   break;

     // ÄÄÄ TriMesh ÄÄÄ

     case 0x4100 : // ObjMesh/ObjBlock/TriMesh
                   DEBUG(printf("Triangle mesh\n"););
                   curType = 1;
                   curObject = new tObject;
                   Scene.addnode(curObject);
                   strncpy(curObject->name,curName,20);
                   curObject->id = -1;
                   curObject->parentid = -1;
                   LastObject = curObject;
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
                    //Vector_Swap(&TempVector);
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
                    ReadWord(Flag);
                    DEBUG(printf("%4i: %4i, %4i, %4i : %4x\n",i,A,B,C,Flag););
                    // 01 CA, 02 BC, 04 AB
                    // 08 U-wrap, 10 V-wrap
                    curObject->polygons[i].flags = Flag;
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
                   // TODO: TempMaterial = Material_FindByName(Scene,&TempString);
                   ReadWord(num);
                   for (i=0;i<num;i++)
                   {
                    ReadWord(TempWord);
                    // curObject->polygons[TempWord].material =
                    // curObject->polygons[TempWord].flags |= POLYflag_material;
                   }
                   break;

     case 0x4140 : // ObjMesh/ObjBlock/TriMesh/MapList

                   // word     Number of vertices
                   //  Then, for each vertex
                   // float    U coordinate
                   // float    V coordinate

                   //curObject->Flags |= OBJflag_mapped;
                   ReadWord(num);
                   //for (i=0;i<num;i++)
                   //{
                   // curObject->vertices[i].normal = &(curObject->Normals[i]);
                   //}
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
                    curObject->polygons[i].smoothinggroup = TempDword;
                   }
                   break;


     case 0x4160 : // ObjMesh/ObjBlock/TriMesh/matrix

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

     // ÄÄÄ Light ÄÄÄ

     case 0x4600 : // ObjBlock/Light
                   // vector   Position

                   curType = 6;
                   curLight = new tLight;

                   Scene.addnode(curLight);

                   //curLight->Flags |= LIGHTflag_active;
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
                   ReadFloat(X); // TargetPosX
                   ReadFloat(Y); // TargetPosY
                   ReadFloat(Z); // TargetPosZ
                   curLight->spot_target.x = X;
                   curLight->spot_target.y = Y;
                   curLight->spot_target.z = Z;

                   ReadFloat(TempFloat);    // HotSpot;
                   curLight->spot_hotspot = TempFloat;
                   ReadFloat(TempFloat);    // FallOff;
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
                   // float    Roll (degree)

                   ReadFloat(TempFloat);
                   curLight->spot_spotroll = TempFloat;
                   break;

         // ÄÄÄ Camera ÄÄÄ

         case 0x4700 : // ObjBlock/Camera
                       // vector   Position
                       // vector   Target
                       // float    Bank (degree)
                       // float    Lens

                       curType = 7;
                       curCamera = new tCamera;
                       Scene.addnode(curCamera);
                       // curCamera->Flags |= CAMERAflag_active;
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
                       curCamera->roll = TempFloat;
                       ReadFloat(TempFloat);
                       curCamera->fov = TempFloat;
                       break;

         // ÄÄÄ Material ÄÄÄ

         case 0xafff : // Material
                       break;

         case 0xa000 : // Material/MaterialName
                       curMaterial = new tMaterial;
                       Scene.addnode(curMaterial);
                       curMaterial->flags = 0;
                       ReadAsciiz(curMaterial->name);
                       break;

         /*

         a010 = ambient color (sub)
         a020 = diffuse color (sub)
         a030 = specular color (sub)
         a040 = shininess (sub)
         a041 = shininess, strength of (sub)
         a050 = transparency (sub)
         a052 = transparency falloff (sub)
         a053 = reflection blur (sub)
         a081 = two-sided (none)
         a083 = transparency ADD (none)
         a085 = wire on (none)
         a088 = face map (none)
         a084 = self illumination
         a100 = material type (short int. 1=flat,2=gouraud,3=phong,4=metal)

         a351 = shortint (ls byte first)
                  bit4 bit0: 00 tile (default) 11 decal  01 both
                  bit 1: mirror
                  bit 2: not used ? (0)
                  bit 3: negative
                  bit 5: summed area map filtering (instead of pyramidal)
                  bit 6: use alpha (toggles RGBluma/alpha. For masks RGB means RGBluma)
                  bit 7: there is a one channel tint (either RGBluma or alpha)
                  bit 8: ignore alpha (take RGBluma even if an alpha exists (?))
                  bit 9: there is a three channel tint (RGB tint)
                  ... not used (0)

         a354 = 1/U scale (float)
         a356 = 1/V scale
         a358 = U offset
         a35a = V offset
         a35c = map rotation angle


         sub-types

         0011 rgb1
         0012 rgb2

         0030 amount of


         */

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

         case 0xb000 : // Keyframer
                       // Scene->SCENE_anim = 1;
                       DEBUG(printf("---------- keyframer ----------\n"););
                       break;

         //case 0xb001 : // Keyframer/AmbientLight

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
                       //          * Bit 11 : Hidden
                       // word     Flag2
                       //          * Bit 0 : Show path
                       //          * Bit 1 : Animate smoothing
                       //          * Bit 4 : Object motion blur
                       //          * Bit 6 : Morph materials
                       // word     Hierarchy father, link to the parent object
                       //          (-1 for none)

                       ReadAsciiz(TempString);
                       DEBUG(printf("object : %s\n",TempString););
                       if (strncmp("$$$DUMMY",TempString,20)==0)
                       {
                        curObject = new tObject;
                        Scene.addnode(curObject);
                        curObject->dummy = true;
                        strncpy(curObject->name,TempString,20);
                        curObject->id = -1;
                        curObject->parentid = -1;
                       }
                       else
                       {
                        curNode = Scene.find(&TempString);
                       }


                       if (curType == 1) // ----- Object -----
                       {
                        ReadAsciiz(TempString);
                        DEBUG(printf("object : %s\n",TempString););
                        if (strncmp("$$$DUMMY",TempString,20)==0)
                        {
                         // add $$$DUMMY object
                         //Scene.numobjects++;
                         // TODO: curObject = Scene.addobject();
                         //curObject->OBJECT_dummy = 1;
                         strncpy(curObject->name,TempString,20);
                         curObject->id = -1;
                         curObject->parentid = -1;
                        }
                        else
                        {
                         //curObject = Object_FindByName(Scene,&TempString);
                        }
                        ReadWord(TempWord);   // Flag1
                        // Bit 11 : Hidden
                        ReadWord(TempWord);   // Flag2
                        // Bit 0 : Show path
                        // Bit 1 : Animate smoothing
                        // Bit 4 : Object motion blur
                        // Bit 6 : Morph materials
                        ReadWord(num);        // hierarchy father
                        curObject->parentid = (int)num;  // Hierarchynumber of parent
                        curObject->id = (int)curObjectKF;
                       }



                       else
                       if (curType == 7) // ----- Camera -----
                       {
                        ReadAsciiz(TempString);
                        DEBUG(printf("camera : %s\n",TempString););
                        //curCamera = Camera_FindByName(Scene,&TempString);
                        ReadWord(TempWord);
                        ReadWord(TempWord);
                        ReadWord(num);
                        curCamera->parentid = num;  // Hierarchynumber of parent
                        curCamera->id = curObjectKF;
                       }
                       else

                       if (curType == 71) // ----- Camera target -----
                       {
                        ReadAsciiz(TempString);
                        DEBUG(printf("camera target : %s\n",TempString););
                        //curCamera = Camera_FindByName(Scene,&TempString);
                        ReadWord(TempWord);
                        ReadWord(TempWord);
                        ReadWord(num);
                        curCamera->parentid = num;  // Hierarchynumber of parent
                        curCamera->id = curObjectKF;
                       }
                       if (curType == 100) // ----- Omni light -----
                       {
                        ReadAsciiz(TempString);
                        DEBUG(printf("Omni light : %s\n",TempString););
                        //curLight = Light_FindByName(Scene,&TempString);
                        ReadWord(TempWord);
                        ReadWord(TempWord);
                        ReadWord(num);
                        curLight->parentid = num;  // Hierarchynumber of parent
                        curLight->id = curObjectKF;
                       }
                       if (curType == 101) // ----- Spotlight target -----
                       {
                        ReadAsciiz(TempString);
                        DEBUG(printf("Spotlight target: %s\n",TempString););
                        //curLight = Light_FindByName(Scene,&TempString);
                        ReadWord(TempWord);
                        ReadWord(TempWord);
                        ReadWord(num);
                        curLight->parentid = num;  // Hierarchynumber of parent
                        curLight->id = curObjectKF;
                       }
                       if (curType == 102) // ----- Spotlight -----
                       {
                        ReadAsciiz(TempString);
                        DEBUG(printf("Spotlight : %s\n",TempString););
                        //curLight = Light_FindByName(Scene,&TempString);
                        ReadWord(TempWord);
                        ReadWord(TempWord);
                        ReadWord(num);
                        curLight->parentid = num;  // Hierarchynumber of parent
                        curLight->id = curObjectKF;
                       }
                       break;

         case 0xb013 : // TrackPivot (KeyFramer/TrackInfo/TrackPivot)
                       // vector   Pivot point
                       ReadFloat(X);
                       ReadFloat(Y);
                       ReadFloat(Z);
                       //Vector_Swap(&TempVector);
                       //TODO:if (curType == 1) Vector_Copy(&(curObject->Pivot),&TempVector);
                       //TODO:else
                       //TODO:if (curType == 7) Vector_Copy(&(curCamera->Pivot),&TempVector);
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
                       ReadWord(Flag);
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
                       ReadDword(TempDword);  // number of keys in track
                       NumKeys = TempDword;
                       DEBUG(printf("NumKeys: %i\n",NumKeys););
                       curObject->kfPos.numkeys = NumKeys;
                       curObject->kfPos.keys = new tKeyNode[NumKeys];
                       for (i=0;i<NumKeys;i++)
                       {
                        DEBUG(printf("key: %i\n",i););
                        ReadDword(TempDword);
                        DEBUG(printf("frame #: %i\n",TempDword););
                        curObject->kfPos.keys[i].frame = TempDword;//TempWord;

                        // TODO: ReadSpline(&(curObject->kfPos.keys[i]));

                        ReadFloat(X);
                        ReadFloat(Y);
                        ReadFloat(Z);
                        DEBUG(printf("%4i: %5.2f, %5.2f, %5.2f\n",i,X,Y,Z););
                        curObject->kfPos.keys[i].data.f[0] = X;
                        curObject->kfPos.keys[i].data.f[1] = Y;
                        curObject->kfPos.keys[i].data.f[2] = Z;
                       }
                       // TODO: Spline_Init(&curObject->kfPos);
                       break;

         // alle videre er lik den over, med andre data som loades (rot=quat, etc)

         /*

         case 0xb021 : // Rot (KeyFramer/TrackInfo/Rot)

                        ReadWord(Flag);
                        for (i=0;i<4;i++) ReadWord(TempWord);
                        //ReadWord(num);
                        //ReadWord(TempWord);
                        ReadDword(TempDword);
                        NumKeys = TempDword;
                        DEBUG(printf("NumKeys: %i\n",NumKeys););
                        curObject->kfRot.NumKeys = NumKeys;//num;
                        curObject->kfRot.Keys = Key_New(NumKeys);
                        for (i=0;i<NumKeys;i++)
                        {
                         DEBUG(printf("key: %i\n",i););
                         //ReadWord(TempWord);
                         ReadDword(TempDword);
                         DEBUG(printf("frame #: %i\n",TempDword););
                         curObject->kfRot.Keys[i].Frame = TempDword;//TempWord;
                         //ReadWord(TempWord);
                         ReadSpline(&(curObject->kfRot.Keys[i]));
                         ReadFloat(TempQuat[qW]);
                         ReadFloat(TempQuat[qX]);
                         ReadFloat(TempQuat[qY]);
                         ReadFloat(TempQuat[qZ]);
                         Quat_Swap(&TempQuat);
                         DEBUG(printf("%4i: %5.2f, %5.2f, %5.2f : %5.2f\n",i,TempQuat[qX],TempQuat[qY],TempQuat[qZ],TempQuat[qW]););
                         Quat_Copy(&(curObject->kfRot.Keys[i].Val._quat),&TempQuat);
                        }
                        Spline_Init(&curObject->kfRot);
                        break;

        case 0xb022 : // Scale (KeyFramer/TrackInfo/Scale)

                        ReadWord(Flag);
                        for (i=0;i<4;i++) ReadWord(TempWord);
                        //ReadWord(num);
                        //ReadWord(TempWord);
                        ReadDword(TempDword);
                        NumKeys = TempDword;
                        DEBUG(printf("NumKeys: %i\n",NumKeys););
                        curObject->kfScale.NumKeys = NumKeys;//num;
                        curObject->kfScale.Keys = Key_New(NumKeys);
                        for (i=0;i<NumKeys;i++)
                        {
                         DEBUG(printf("key: %i\n",i););
                         //ReadWord(TempWord);
                         ReadDword(TempDword);
                         DEBUG(printf("frame #: %i\n",TempDword););
                         curObject->kfScale.Keys[i].Frame = TempDword;//TempWord;
                         //ReadWord(TempWord);
                         ReadSpline(&(curObject->kfScale.Keys[i]));
                         ReadFloat(TempVector.x);
                         ReadFloat(TempVector.y);
                         ReadFloat(TempVector.z);
                         Vector_Swap(&TempVector);
                         Vector_Copy(&(curObject->kfScale.Keys[i].Val._vector),&TempVector);
                         DEBUG(printf("%4i: %5.2f, %5.2f, %5.2f\n",i,TempVector.x,TempVector.y,TempVector.z););
                        }
                        Spline_Init(&curObject->kfScale);
                        break;

         case 0xb023 : // Fov (KeyFramer/TrackInfo/Fov)

                        ReadWord(Flag);
                        for (i=0;i<4;i++) ReadWord(TempWord);
                        //ReadWord(num);
                        //ReadWord(TempWord);
                        ReadDword(TempDword);
                        NumKeys = TempDword;
                        curCamera->kfFov.NumKeys = NumKeys;//num;
                        curCamera->kfFov.Keys = Key_New(NumKeys);
                        for (i=0;i<NumKeys;i++)
                        {
                         //ReadWord(TempWord);
                         ReadDword(TempDword);
                         curCamera->kfFov.Keys[i].Frame = TempDword;//TempWord;
                         //ReadWord(TempWord);
                         ReadSpline(&(curCamera->kfFov.Keys[i]));
                         ReadFloat(TempFloat);
                         curCamera->kfFov.Keys[i].Val._float = TempFloat;
                        }
                        Spline_Init(&curCamera->kfFov);
                        break;

         case 0xb024 : // Roll (KeyFramer/TrackInfo/Roll)

                        ReadWord(Flag);
                        for (i=0;i<4;i++) ReadWord(TempWord);
                        //ReadWord(num);
                        //ReadWord(TempWord);
                        ReadDword(TempDword);
                        NumKeys = TempDword;
                        curCamera->kfRoll.NumKeys = NumKeys;//num;
                        curCamera->kfRoll.Keys = Key_New(NumKeys);
                        for (i=0;i<NumKeys;i++)
                        {
                         //ReadWord(TempWord);
                         ReadDword(TempDword);
                         curCamera->kfRoll.Keys[i].Frame = TempDword;//TempWord;
                         //ReadWord(TempWord);
                         ReadSpline(&(curCamera->kfRoll.Keys[i]));
                         ReadFloat(TempFloat);
                         curCamera->kfRoll.Keys[i].Val._float = TempFloat;
                        }
                        Spline_Init(&curCamera->kfRoll);
                        break;
         */

         case 0xb030 : // KeyFramer/TrackInfo/TrackObjNumber
                       ReadWord(curObjectKF);
                       break;

     // --------

     default     : iBufferPointer += (Chunk.Size-6);
                   BufferPtr += (Chunk.Size-6);

    }
   }
  }
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
  printf("DUMP3DS - (C) 1999 Axon\n");
  if (argc != 2)
  {
   printf("%s <3DS-file>\n",argv[0]);
   exit(1);
  }
  printf("# %s\n\n",argv[1]);
  Load3DS(argv[1]);
 }
