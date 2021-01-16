#include <stdio.h>

#include "defines.h"
#include "types.h"
#include "vars.h"

// --------------------------------------------------------------------------

 struct
 {
  unsigned short chunk_id;
  unsigned int next_chunk;
 } chunk;

 // --------------------

sObject *Load3DS( char *filename, float VertexMult, float UVmult )
 {
  FILE *fp;
  long filelen;
  unsigned int nextobj;
  unsigned int unknown;

  char objname[20];
  unsigned short vertices,polys,num;
  float x,y,z;
  unsigned short int facea,faceb,facec,faceflags;
  float m11,m12,m13,
        m21,m22,m23,
        m31,m32,m33,
        m41,m42,m43;
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

 // --------------------

  returnO = NULL;
  fp = fopen(filename,"rb");
  filelen = filelength(fileno(fp));
  fread (&chunk,6,1,fp);
  if (chunk.chunk_id != 0x4d4d)
  {
   // printf("Invalid 3DS file!!!\n");
   return NULL;
  }
  else
  {
   while (!feof(fp) && (ftell(fp) < filelen))
   {
    fread (&chunk,6,1,fp);

    switch(chunk.chunk_id)
    {
 // --------------------
     case 0x3d3d : // mesh;
                   break;
     case 0x4000 : // Found object
                   nextobj = ftell(fp) + chunk.next_chunk - 6;
                   // allocate memory for object
                   // O = (sObject *)malloc(sizeof(sObject));
                   if (returnO == NULL)
                   {
                    O = (sObject *)malloc(sizeof(sObject));
                    returnO = O;
                   }
                   else
                   {
                    O->Child = (sObject *)malloc(sizeof(sObject));
                    O = O->Child;
                   }
                   // set some default values
                   O->Flags = OBJECT_active;
                   Object_SetPos(O,0.0001,0.0001,0.0001);
                   Object_SetRot(O,0.0001,0.0001,0.0001);
                   O->Parent = NULL;
                   O->Child = NULL;
                   O->NumVertices = 0;
                   O->NumPolys = 0;
 //                  printf("Creating VUE list. %i frames\n",MaxVUEFrames);
//                   O->VUElist = (sMatrix *)malloc(MaxVUEFrames*sizeof(sMatrix));
                   // read object name

                   c = 0;
                   do
                   {
                    objname[c] = fgetc(fp);
                    c++;
                   } while (objname[c-1] != 0);
                   strcpy (O->ObjectName,objname);
//                   j=0;
//                   for (i=0;i<strlen(objname);i++)
//                   {
//                    if (objname[i] != ' ') O->ObjectName[j++] = objname[i];
//                   }
//                   O->ObjectName[j] = '\0';

                   printf("Object found: '%s'\n",objname,O->ObjectName);
                   break;
     case 0x4100 : // Triangular polygon object
                   break;
     case 0x4110 : // vertices
                   fread(&vertices,2,1,fp);
//                   printf("%i vertices...\n",vertices);
                   O->NumVertices = vertices;
                   O->Vertices = (sVertex *)malloc(vertices*sizeof(sVertex));
                   for (i=0;i<vertices;i++)
                   {
                    fread(&x,4,1,fp);
                    fread(&y,4,1,fp);
                    fread(&z,4,1,fp);
                    O->Vertices[i].Pos.x = x * VertexMult;
                    O->Vertices[i].Pos.y = y * VertexMult;
                    O->Vertices[i].Pos.z = z * VertexMult;
                    O->Vertices[i].u = 0;
                    O->Vertices[i].v = 0;
                    O->Vertices[i].c = 63;
                   }
                   break;
     case 0x4120 : // polygons
//                   printf("polys\n");
                   fread(&polys,2,1,fp);
//                   printf("%i faces...\n",polys);
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
 // Insert this one, and "increase" pointer to child object
 //                  fseek(fp,nextobj,SEEK_SET);       // ?????
                   break;
     case 0x4140 : // U/V mapping
                   O->Flags |= OBJECT_mapped;
                   fread(&num,2,1,fp);
//                   printf("%i U/V mappings...\n",num);
                   for (i=0;i<num;i++)
                   {
                    fread(&u,4,1,fp);
                    fread(&v,4,1,fp);
                    O->Vertices[i].u = u * UVmult;
                    O->Vertices[i].v = v * UVmult;
                   }
                   break;

     case 0x4160 : // matrix
                   fread(&m11,4,1,fp);
                   fread(&m12,4,1,fp);
                   fread(&m13,4,1,fp);
                   fread(&m21,4,1,fp);
                   fread(&m22,4,1,fp);
                   fread(&m23,4,1,fp);
                   fread(&m31,4,1,fp);
                   fread(&m32,4,1,fp);
                   fread(&m33,4,1,fp);
                   fread(&m41,4,1,fp);
                   fread(&m42,4,1,fp);
                   fread(&m43,4,1,fp);
                   break;
     case 0x4600 : // Light
                   fread(&lightx,4,1,fp);
                   fread(&lighty,4,1,fp);
                   fread(&lightz,4,1,fp);
                   break;
     case 0x4700 : // Camera;
                   fread(&camx,4,1,fp);
                   fread(&camy,4,1,fp);
                   fread(&camz,4,1,fp);
                   fread(&tarx,4,1,fp);
                   fread(&tary,4,1,fp);
                   fread(&tarz,4,1,fp);
                   fread(&bank,4,1,fp);
                   fread(&lens,4,1,fp);
                   break;
     case 0xb000 : // Keyframer data
                   break;
     case 0xb002 : // Object framedata
                   break;
     case 0xb003 : // Camera track (pos)
                   break;
     case 0xb004 : // Camera target
                   break;
     case 0xb010 : // object name
                   break;
     case 0xb020 : // translation key
/*
                   for(i=0; i<5; i++) fread(&unknown, 2, 1, fp);
                   fread(&num, 2, 1, fp);
                   // num = number of anim keys
                   KF = &Camera->KeyFramerPos;
                   KF->NumSegments = num;
                   KF->SplineSegments = (sKeySegment *)malloc(num*sizeof(sKeySegment));

                   fread(&unknown, 2, 1, fp);
                   for (i=0;i<num;i++)
                   {
                    fread(&nf, 2, 1, fp);    // frame number
                    fread(&unknown, 2, 1, fp);
                    fread(&flags, 2, 1, fp); // frame flags
                    // Will probably crash if Spline has any flags!!!
                    // any bits set in flags means additionally 1 float
                    for (j=0; j<16; j++)
                    {
                     if (flags & (1 << j))
                     {
                      fread(&temp, 4, 1, fp);
                     }
                    }

                    fread(&x, 4, 1, fp);
                    fread(&y, 4, 1, fp);
                    fread(&z, 4, 1, fp);
                    KF->SplineSegments[i].TimeStamp = nf;
                    KF->SplineSegments[i].Pos.x = x;
                    KF->SplineSegments[i].Pos.y = y;
                    KF->SplineSegments[i].Pos.z = z;
                   }
*/
                   break;
     case 0xb021 : // rotation key
                   break;
     case 0xb022 : // scale key
                   break;
     default : fseek(fp,chunk.next_chunk-6, SEEK_CUR);
 // --------------------
    }
    if (chunk.chunk_id == 0xb001) break;
   }
  }
  // recorse down from returnO...
  O = returnO;
  while (O!=NULL)
  {
   Object_CalcFaceNormals(O);
   Object_CalcVertexNormals(O);
   Object_Center(O);
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
   O = O->Child;
  }
  return returnO;
 }

