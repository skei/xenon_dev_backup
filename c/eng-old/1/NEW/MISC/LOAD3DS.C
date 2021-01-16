#include <stdio.h>

// #include "engine.h"

struct
{
 unsigned short chunk_id;
 unsigned int next_chunk;
} chunk;

void Load3DS( char *filename/*, sObject *O*/ )
{
 FILE *fp;
 long filelen;
 unsigned int nextobj;
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
 int i;
// --------------------------------------------------------------------------
 fp = fopen(filename,"rb");
 filelen = filelength(fileno(fp));
 fread (&chunk,6,1,fp);
 if (chunk.chunk_id == 0x4d4d)
 {
  while (!feof(fp) && (ftell(fp) < filelen))
  {
   fread (&chunk,6,1,fp);

   switch(chunk.chunk_id)
   {
// --------------------------------------------------------------------------
    case 0x3d3d : // mesh;
                  break;
    case 0x4000 : // Found object
                  nextobj = ftell(fp) + chunk.next_chunk - 6;
                  // malloc mem for object, etc...
                  // read object name
                  c = 0;
                  do
                  {
                   objname[c] = fgetc(fp);
                   c++;
                  } while (objname[c-1] != 0);
//                  strcpy (O->ObjectName,objname);
                  printf("Object found: %s\n",objname);
                  break;
    case 0x4100 : // Triangular polygon object
                  break;
    case 0x4110 : // vertices
                  fread(&vertices,2,1,fp);
                  printf("Number of vertices: %i\n",vertices);
//                  O->NumVertices = vertices;
                  for (i=0;i<vertices;i++)
                  {
                   fread(&x,4,1,fp);
                   fread(&y,4,1,fp);
                   fread(&z,4,1,fp);
                  }
                  break;
    case 0x4120 : // polygons
                  fread(&polys,2,1,fp);
                  printf("Number of faces: %i\n",polys);

//                  O->NumPolys = polys;
                  for (i=0;i<polys;i++)
                  {
                   fread(&facea,2,1,fp);
                   fread(&faceb,2,1,fp);
                   fread(&facec,2,1,fp);
                   fread(&faceflags,2,1,fp);
                   // faceflags: 04,02,01 = ab,bc,ca
                   //            08,10 = uwrap,vwrap
                  }
//                  fseek(fp,nextobj,SEEK_SET);       // ?????
                  break;
    case 0x4140 : // U/V mapping
                  fread(&num,2,1,fp);
                  printf("Number of U/V mappings: %i\n",num);
                  for (i=0;i<num;i++)
                  {
                   fread(&u,4,1,fp);
                   fread(&v,1,1,fp);
                  }

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
                  printf("Matrix: %f, %f, %f\n",m11,m12,m13);
                  printf("        %f, %f, %f\n",m21,m22,m23);
                  printf("        %f, %f, %f\n",m31,m32,m33);
                  printf("        %f, %f, %f\n",m41,m42,m43);


//                  fseek(fp,nextobj,SEEK_SET);       // ?????

                  break;
    case 0x4600 : // Light
                  fread(&lightx,4,1,fp);
                  fread(&lighty,4,1,fp);
                  fread(&lightz,4,1,fp);
                  printf("Light: %f, %f, %f\n",lightx,lighty,lightz);
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
    case 0xb010 : // object
                  break;
    case 0xb020 : // translation key
                  break;
    case 0xb021 : // rotation key
                  break;
    case 0xb022 : // scale key
                  break;
    default : fseek(fp,chunk.next_chunk-6, SEEK_CUR);
// --------------------------------------------------------------------------
   }
   if (chunk.chunk_id == 0xb001) break;
  }
 }
 else printf("Invalid 3DS file!!!\n");
}

// 께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께

void main( int argc, const char *argv[] )
{
 if (argc != 2)
 {
  printf("Load3ds <file.3ds>\n");
  exit(1);
 }
 Load3DS(argv[1]);
}
