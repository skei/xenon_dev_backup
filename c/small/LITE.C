 #include <math.h>
 #include <stdlib.h>  // debug - rand()
 #include <stdio.h>   // debug - file stuff
 #include <string.h>

 // Lite-engine..
 // Restriksjoner:
 // - Polygoner med en eller flere vertices som passerer Z=1, skippes
 // - Objekterer roteres rundt egen akse, og plasseres i WorldSpace


 // Todo
 // - Fjern limits & hardcoding av ting.... !!!

 #include "lite.h"

 // ---

 typedef unsigned int BOOL;

 #define NULL 0
 #define MinX        0
 #define MaxX        319
 #define MinY        0
 #define MaxY        199
 #define DEBUG_POLYS 2000
 #define ZSORTMULT   65535
 #define QUOTE       '\"'
 #define TRUE        1
 #define FALSE       0
 #define vertmult    1
 #define texmult     255

 // ---

 int       Numbers[256];
 char     *dummy;

//
// 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
//

void  Matrix_Identity(float *m)
 {
  m[ 0] = 1;  m[ 1] = 0;  m[ 2] = 0;  m[ 3] = 0;
  m[ 4] = 0;  m[ 5] = 1;  m[ 6] = 0;  m[ 7] = 0;
  m[ 8] = 0;  m[ 9] = 0;  m[10] = 1;  m[11] = 0;
  m[12] = 0;  m[13] = 0;  m[14] = 0;  m[15] = 1;
 }

 // ---

void  Matrix_Translation(float *m, float tx, float ty, float tz)
 {
  m[ 0] = 1;  m[ 1] = 0;  m[ 2] = 0;  m[ 3] = tx;
  m[ 4] = 0;  m[ 5] = 1;  m[ 6] = 0;  m[ 7] = ty;
  m[ 8] = 0;  m[ 9] = 0;  m[10] = 1;  m[11] = tz;
  m[12] = 0;  m[13] = 0;  m[14] = 0;  m[15] = 1;
 }

 // ---

void  Matrix_Rotation(float *m, float xang, float yang, float zang)
 {
  //      c2*c3             c2*s3          s2      tx
  // -s1*s2*c3-c1*s3   -s1*s2*s3+c1*c3    s1*c2    ty
  // -c1*s2*c3+s1*s3   -c1*s2*s3-s1*c3    c1*c2    tz
  //        0                 0            0        1
  float s1,s2,s3,c1,c2,c3;

  s1 = sin(xang);
  s2 = sin(yang);
  s3 = sin(zang);
  c1 = cos(xang);
  c2 = cos(yang);
  c3 = cos(zang);

  m[ 0] = c2*c3;
  m[ 1] = c2*s3;
  m[ 2] = s2;
  m[ 3] = 0;

  m[ 4] = -s1*s2*c3 - c1*s3;
  m[ 5] = -s1*s2*s3 + c1*c3;
  m[ 6] = s1*c2;
  m[ 7] = 0;

  m[ 8] = -c1*s2*c3 + s1*s3;
  m[ 9] = -c1*s2*s3 - s1*c3;
  m[10] = c1*c2;
  m[11] = 0;

  m[12] = 0;
  m[13] = 0;
  m[14] = 0;
  m[15] = 1;
 }

 // ---

 // a = a*b
void  Matrix_Mult(float *a, float *b)
 {
  float ab[16];
  int i,j,k;

  for (i=0;i<4;i++)
  {
   for (j=0;j<4;j++)
   {
    ab[j*4+i] = 0.0;
    for (k=0;k<4;k++) ab[j*4+i] += a[k*4+i] * b[j*4+k];
   }
  }
  for (i=0;i<16;i++) a[i] = ab[i];
 }

// ---

 // add two vectors together
void  Vector_Add(tVector *dest, tVector *v1, tVector *v2)
 {
  dest->x = v1->x + v2->x;
  dest->y = v1->y + v2->y;
  dest->z = v1->z + v2->z;
 }

 // ---

 // subtract vector v2 from vector v1
void  Vector_Sub(tVector *dest, tVector *v1, tVector *v2)
 {
  dest->x = v1->x - v2->x;
  dest->y = v1->y - v2->y;
  dest->z = v1->z - v2->z;
 }

 // ---

 // Normalize vector
void  Vector_Normalize(tVector *dest, tVector *v)
 {
  float len;
  len = sqrt(v->x*v->x + v->y*v->y + v->z*v->z);
  dest->x = (v->x/len);
  dest->y = (v->y/len);
  dest->z = (v->z/len);
 }

 // ---

 // dot product of two vectors
float Vector_Dot(tVector *v1, tVector *v2)
 {
  return ( v1->x*v2->x + v1->y*v2->y + v1->z*v2->z );
 }

 // ---

 // cross product of two vectors
void  Vector_Cross(tVector *dest, tVector *v1, tVector *v2)
 {
  dest->x = v1->y*v2->z - v1->z*v2->y;
  dest->y = v1->z*v2->x - v1->x*v2->z;
  dest->z = v1->x*v2->y - v1->y*v2->x;
 }

 // ---

 // translate vector by matrix
void  Vector_Trans(tVector *dest, tVector *v, float *m)
 {
  dest->x = v->x*m[ 0] + v->y*m[ 1] + v->z*m[ 2] + m[ 3];
  dest->y = v->x*m[ 4] + v->y*m[ 5] + v->z*m[ 6] + m[ 7];
  dest->z = v->x*m[ 8] + v->y*m[ 9] + v->z*m[10] + m[11];
 }

 // ---

 // translate vector by matrix
void  Vector_Trans3x3(tVector *dest, tVector *v, float *m)
 {
  dest->x = v->x*m[ 0] + v->y*m[ 1] + v->z*m[ 2];
  dest->y = v->x*m[ 4] + v->y*m[ 5] + v->z*m[ 6];
  dest->z = v->x*m[ 8] + v->y*m[ 9] + v->z*m[10];
 }

//
// 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
//

void Camera_CreateMatrix_Free(tCamera *c)
 {
  float mm[16];

  Matrix_Translation(c->matrix,-(c->pos.x),-(c->pos.y),-(c->pos.z));
  Matrix_Rotation(&mm,-(c->rot.x),-(c->rot.y),-(c->rot.z));
  Matrix_Mult(c->matrix,&mm);
 }

 // ---

void Object_CreateMatrix(tObject *o, tCamera *c)
 {
  Matrix_Rotation(o->matrix,o->rot.x,o->rot.y,o->rot.z);
  o->matrix[ 3] += o->pos.x;
  o->matrix[ 7] += o->pos.y;
  o->matrix[11] += o->pos.z;
  Matrix_Mult(o->matrix,c->matrix);
 }

// ---

void Object_CalcFaceNormals( tObject *O )
 {
  tVertex *a,*b,*c;
  float ax,ay,az;
  float bx,by,bz;
  float kx,ky,kz;
  float leng;
  int i;

  for (i=0;i<O->numpolys;i++)
  {
   a = O->polygons[i].v[0];
   b = O->polygons[i].v[1];
   c = O->polygons[i].v[2];

   ax = b->pos.x - a->pos.x;
   ay = b->pos.y - a->pos.y;
   az = b->pos.z - a->pos.z;

   bx = c->pos.x - a->pos.x;
   by = c->pos.y - a->pos.y;
   bz = c->pos.z - a->pos.z;

   kx = (ay*bz)-(az*by);
   ky = (az*bx)-(ax*bz);
   kz = (ax*by)-(ay*bx);

   leng = (sqrt( kx*kx + ky*ky + kz*kz));
   if (leng!=0)
   {
    O->polygons[i].normal.x = (kx/leng);
    O->polygons[i].normal.y = (ky/leng);
    O->polygons[i].normal.z = (kz/leng);
   }
   else
   {
    O->polygons[i].normal.x = 0;
    O->polygons[i].normal.y = 0;
    O->polygons[i].normal.z = 0;
   }
  }
 }

 // ---

void Object_CalcVertexNormals( tObject *O )
 {
  int i,a,num;
  tVertex *vi;
  float ax,ay,az;
  float lengd;

  for (i=0;i<O->numvertices;i++)
  {
   vi = &O->vertices[i];
   ax = 0;
   ay = 0;
   az = 0;
   num = 0;
   for (a=0;a<O->numpolys;a++)
   {
    if (    (O->polygons[a].v[0] == vi)
         || (O->polygons[a].v[1] == vi)
         || (O->polygons[a].v[2] == vi) )
    {
     ax += O->polygons[a].normal.x;
     ay += O->polygons[a].normal.y;
     az += O->polygons[a].normal.z;
     num++;
    }
   }
   if ( num != 0 )
   {
    ax = ax / num;
    ay = ay / num;
    az = az / num;
   }
   lengd = sqrt( ax*ax + ay*ay + az*az);
   if ( lengd != 0)
   {
    O->vertices[i].normal.x = (ax/lengd);
    O->vertices[i].normal.y = (ay/lengd);
    O->vertices[i].normal.z = (az/lengd);
   }
   else
   {
    O->vertices[i].normal.x = 0;
    O->vertices[i].normal.y = 0;
    O->vertices[i].normal.z = 0;
   }
  }
 }

 // ---

 // Center object around its average X/Y/Z pos
void Object_Center( tObject *O )
 {
  float xmax,xmin,ymax,ymin,zmax,zmin;
  float xmove, ymove, zmove;
  int i;

  xmax = -32767; xmin = 32767;
  ymax = -32767; ymin = 32767;
  zmax = -32767; zmin = 32767;
  for (i=0;i<O->numvertices;i++)
  {
   if ( O->vertices[i].pos.x > xmax ) xmax = O->vertices[i].pos.x;
   if ( O->vertices[i].pos.x < xmin ) xmin = O->vertices[i].pos.x;
   if ( O->vertices[i].pos.y > ymax ) ymax = O->vertices[i].pos.y;
   if ( O->vertices[i].pos.y < ymin ) ymin = O->vertices[i].pos.y;
   if ( O->vertices[i].pos.z > zmax ) zmax = O->vertices[i].pos.z;
   if ( O->vertices[i].pos.z < zmin ) zmin = O->vertices[i].pos.z;
  }
  xmove = xmin+((xmax-xmin) /2 );
  ymove = ymin+((ymax-ymin) /2 );
  zmove = zmin+((zmax-zmin) /2 );
  for (i=0;i<O->numvertices;i++)
  {
   O->vertices[i].pos.x-=xmove;
   O->vertices[i].pos.y-=ymove;
   O->vertices[i].pos.z-=zmove;
  }
  O->pos.x += xmove;
  O->pos.y += ymove;
  O->pos.z += zmove;
 }

 // ---

void Object_SphericalMapping(tObject *O)
 {
  int i;

  for (i=0;i<O->numvertices;i++)
  {
   O->vertices[i].u = O->vertices[i].normal.x * (texmult/2) + (texmult/2);
   O->vertices[i].v = O->vertices[i].normal.y * (texmult/2) + (texmult/2);
  }
 }

//
// 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
//

void debug_Line(int x0, int y0, int x1, int y1, BYTE color, char *_B)
 {
  int dx,ix,cx,dy,iy,cy,m,i,pos;

  if (x0<x1) { dx=x1-x0; ix=1; } else { dx=x0-x1; ix=-1; }
  if (y0<y1) { dy=y1-y0; iy=1; } else { dy=y0-y1; iy=-1; }
  if (dx>dy) m=dx; else m=dy;
  cx=cy=(m>>1);
  for (i=0;i<m;i++)
  {
   if ((x0>=MinX)&&(x0<=MaxX)&&(y0>=MinY)&&(y0<=MaxY))
   {
    _B[y0*320+x0] = color;
   }
   if ((cx+=dx)>=m) { cx-=m; x0+=ix; }
   if ((cy+=dy)>=m) { cy-=m; y0+=iy; }
  }
 }

 // 컴�

void debug_PolyWireFrame( tPolygon *p, char *Buffer )
 {
  tVertex *v[3];

  v[0] = p->v[0];
  v[1] = p->v[1];
  v[2] = p->v[2];

  debug_Line((int)(v[0]->posR.x),
             (int)(v[0]->posR.y),
             (int)(v[1]->posR.x),
             (int)(v[1]->posR.y),15,Buffer);
  debug_Line((int)(v[0]->posR.x),
             (int)(v[0]->posR.y),
             (int)(v[2]->posR.x),
             (int)(v[2]->posR.y),15,Buffer);
  debug_Line((int)(v[1]->posR.x),
             (int)(v[1]->posR.y),
             (int)(v[2]->posR.x),
             (int)(v[2]->posR.y),15,Buffer);
 }

 // ---


 // ---

void SetRGB(BYTE color, BYTE r,BYTE g, BYTE b);
 #pragma aux SetRGB=\
 "mov dx,3c8h",\
 "out dx,al",\
 "inc dx",\
 "mov al,ah",\
 "out dx,al",\
 "mov al,bl",\
 "out dx,al",\
 "mov al,bh",\
 "out dx,al",\
 parm [al] [ah] [bl] [bh] modify [edx];

 // ---

void WaitVR (void);
 #pragma aux WaitVR =\
 " mov dx,3dah",\
 "wait1:",\
 " in al,dx",\
 " test al,8",\
 " jnz wait1",\
 "wait2:",\
 " in al,dx",\
 " test al,8",\
 " jz wait2",\
 modify [al dx];

// ---

 // returns TRUE if the string s contains the substring v
BOOL     instr(char *s,char *v)
 {
  if (strstr(s,v) != NULL) return TRUE;
  else return FALSE;
 }

 // Returning the value following V, converted to float
double   getvalue ( char *s, char *v )
 {
  char *pos;
  double x;

  pos = strstr(s,v);        // pos -> 1st occurence of V in S
  pos+=strlen(v);              // Skip V
  x = strtod( pos, &dummy );   // x = convert following string to double
  return x;
 }

 // Returning the string (in ""'s) following V
void     getstring( char *s, char *v, char *ret )
 {
  char *pos;
  pos = strstr( s, v );        // pos -> 1st occurence of V in S
  pos+=strlen(v);              // Skip V
  while ( *pos != QUOTE ) pos++;
  while ( *pos == QUOTE ) pos++;
  while ( *pos != QUOTE )
  {
   *ret++ = *pos++;
  }
  *ret = '\0';
 }

 // ---

 // ASC files are defined counter-clockwise..
tObject *LoadASC(char *filename)
 {
  tObject *O;
  FILE  *f;
  int done;
  char buf[256],objname[256];
  int Mapped,NumVert,NumFaces;
  float x,y,z,u,v;
  int   a,b,c;
  int i;

  Mapped = 0;
  f = fopen(filename,"rb");
  O = (tObject *)malloc(sizeof(tObject));
  memset(O,0,sizeof(tObject));
  done = 0;
  // read until 'Tri-mesh'
  while( !instr(buf,"Tri-mesh,")) fgets (buf,255,f);
  NumVert  = getvalue(buf,"Vertices:");
  NumFaces = getvalue(buf,"Faces:");
  O->numvertices = NumVert;
  O->numpolys    = NumFaces;
  O->vertices    = (tVertex *)malloc(NumVert*sizeof(tVertex));
  O->polygons    = (tPolygon *)malloc(NumFaces*sizeof(tPolygon));
  // scan for "Vertex list:", and check for "Mapped" at the same time
  while( !instr(buf,"Vertex list:"))
  {
   fgets (buf,255,f);
   if (instr(buf,"Mapped")) Mapped=1;
  }
  for (i=0;i<NumVert;i++)
  {
   // read next line
   fgets(buf,255,f);
   // skip lines until 'Vertex'
   while (!instr(buf,"Vertex")) fgets(buf,255,f);
   x = getvalue(buf,"X:");
   y = getvalue(buf,"Y:");
   z = getvalue(buf,"Z:");
   O->vertices[i].pos.x = x * vertmult;
   O->vertices[i].pos.y = y * vertmult;
   O->vertices[i].pos.z = z * vertmult;
   O->vertices[i].u = 0;
   O->vertices[i].v = 0;
   if (Mapped == 1)
   {
    u = getvalue(buf,"U:");
    v = getvalue(buf,"V:");
    O->vertices[i].u = u*texmult;
    O->vertices[i].v = v*texmult;
   }
  }
  // scan for "Face list:"
  while( !instr(buf,"Face list:")) fgets(buf,255,f);
  for (i=0;i<NumFaces;i++)
  {
   // read next line
   fgets(buf,255,f);
   // skip lines until 'Face'
   while (!instr(buf,"Face")) fgets(buf,255,f);
   a = (int)getvalue(buf,"A:");
   b = (int)getvalue(buf,"B:");
   c = (int)getvalue(buf,"C:");
   // blir det 0,1,2 eller 2,1,0 ???
   O->polygons[i].v[2] = &(O->vertices[a]);
   O->polygons[i].v[1] = &(O->vertices[b]);
   O->polygons[i].v[0] = &(O->vertices[c]);
  }
  fclose(f);
  return O;
 }

 // ---

//
// 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
//

void Scene_Sort( tScene *S)
 {
  int i;
  int a,b,d;

  // clear numbers array
  memset( Numbers,0,sizeof (Numbers));
  // Count number of each byte-value
  for ( i=0;i<S->visiblepolys;i++ )
  {
   Numbers[ S->renderlist[i]->z & 255 ]++;
  }
  // Convert from numbers to offset in list to sort
  a = 0;
  for ( i=0;i<256;i++ )
  {
   b = Numbers[i];
   Numbers[i] = a;
   a+=b;
  }
  // Sort according to LOW byte
  for ( i=0;i<S->visiblepolys;i++ )
  {
   a = S->renderlist[i]->z & 255; //sorttable[i].sortvalue & 255;
   d = Numbers[a];
   S->sorted[d] = S->renderlist[i];
   Numbers[a]++;
  }
  // -
  // Now, do the same, but with high byte...
  // -
  // clear numbers array
  memset( Numbers,0,sizeof (Numbers));
  // Count number of each byte-value
  for ( i=0;i<S->visiblepolys;i++ )
  {
   Numbers[ (S->sorted[i]->z & 0xff00) >> 8 ]++;
  }
  // Convert from numbers to offset in list to sort
  a = 0;
  for ( i=0;i<256;i++ )
  {
   b = Numbers[i];
   Numbers[i] = a;
   a+=b;
  }
  // Sort according to HIGH byte
  for ( i=0;i<S->visiblepolys;i++ )
  {
   a = (S->sorted[i]->z & 0xff00) >> 8; //sorttable[i].sortvalue & 255;
   d = Numbers[a];
   S->renderlist[d] = S->sorted[i];
   Numbers[a]++;
  }
 }

 // ---

void Scene_Render(tScene *S, char *Buffer)
 {
  tObject  *O;
  tCamera  *C;
  tLight   *L;
  tPolygon *P;
  tVertex  *V,*V1,*V2,*V3;
  tVector   temp;
  int       obj,vert,poly;

  S->visiblepolys = 0;
  C = S->camera;
  Camera_CreateMatrix_Free(C);

  for (obj=0;obj<S->numobjects;obj++)
  {
   O = &(S->objects[obj]);
   Object_CreateMatrix(O,C);

   // ----- handle vertices in current object -----

   for (vert=0;vert<O->numvertices;vert++)
   {
    V = &(O->vertices[vert]);
    Vector_Trans(&temp,&(V->pos),O->matrix);
    if (temp.z <= 1)
    {
     V->posR.x = 160;//temp.x;
     V->posR.y = 100;//temp.y;
     V->posR.z = 0;//temp.z;
    }
    else
    {
     V->posR.z = ZSORTMULT/temp.z;
     V->posR.x = ((temp.x * 256) / (temp.z)) + 160;
     V->posR.y = ((temp.y * 256) / (temp.z)) + 100;
    }
   }

   // ----- handle object/polygons -----

   for (poly=0;poly<O->numpolys;poly++)
   {
    P = &(O->polygons[poly]);
    V1 = P->v[0];
    V2 = P->v[1];
    V3 = P->v[2];
    // Check if any Z in front of screen (later: set flag in vertex, mark: z-clip needed
    if ((V1->posR.z != 0) && (V2->posR.z != 0) && (V3->posR.z != 0))
    {
     // check z-component of face-normal here... And skip if not visible
     {
      P->z = (V1->posR.z + V2->posR.z + V3->posR.z);
      S->renderlist[S->visiblepolys] = P;
      S->visiblepolys++;
     }
    }
   }
  }

  // ----- sort & draw all polys -----

  Scene_Sort(S);
  for (poly=0;poly<S->visiblepolys;poly++)
  {
   Poly_Tex(S->renderlist[poly],Buffer);
   // debug_PolyWireFrame( &(O->polygons[poly]),Buffer );
  }
 }

//
// 컴� main 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
//

 // X = towards left
 // Y = downwards
 // Z = inwards

 #include "poly.c"

 // ---

 int       i;
 FILE     *fp;
 tScene    Scene;
 tObject  *Object;
 tLight    Light;
 tCamera   Camera;
 char      Buffer[64000];
 BYTE      pal[768];
 BYTE      tex[65536];

 // ---

void main(int argc, char **argv)
 {
  // ---

  if (argc != 2)
  {
   printf("LITE <file.asc>\n");
   exit(1);
  }

  fp = fopen("texture.pix","rb");
  fread(tex,1,65536,fp);
  fclose(fp);

  fp = fopen("texture.pal","rb");
  fread(pal,1,768,fp);
  fclose(fp);

  Object = LoadASC(argv[1]);

  printf("\nASC file  : %s\n",argv[1]);
  printf("# vertices: %i\n",Object->numvertices);
  printf("# polygons: %i\n",Object->numpolys);
  printf("\n... press any key ...\n");
  getch();

  Object_CalcFaceNormals(Object);
  Object_CalcvertexNormals(Object);
  Object_Center(Object);
  Object_SphericalMapping(Object);

  for (i=0;i<Object->numpolys;i++) Object->polygons[i].texture = tex;

  Scene.numobjects   = 1;
  Scene.objects      = Object;
  Scene.numlights    = 1;
  Scene.lights       = &Light;
  Scene.camera       = &Camera;
  Scene.visiblepolys = 0;
  Scene.renderlist   = (tPolygon **)malloc(Object->numpolys*sizeof(tPolygon **));
  Scene.sorted       = (tPolygon **)malloc(Object->numpolys*sizeof(tPolygon **));

  Camera.pos.x = 0;
  Camera.pos.y = 0;
  Camera.pos.z = -64;
  Camera.rot.x = 0;
  Camera.rot.y = 0;
  Camera.rot.z = 0;

  Light.pos.x     = 0;
  Light.pos.y     = 0;
  Light.pos.z     = -512;
  Light.intensity = 1;

  _asm
  {
   mov eax,0x13
   int 0x10
  }
  for (i=0;i<256;i++) SetRGB(i,pal[i*3],pal[(i*3)+1],pal[(i*3)+2]);

  Object->pos.z = 512;

  while (!kbhit())
  {
   Object->rot.x += 0.008;
   Object->rot.y += 0.006;
   Object->rot.z += 0.004;
   memset(Buffer,0,64000);
   Scene_Render(&Scene,Buffer);
   WaitVR();
   memcpy((char *)0xA0000,Buffer,64000);
  }
  _asm
  {
   mov eax,0x3
   int 0x10
  }
  free(Object->vertices);
  free(Object->polygons);
  free(Object);
  free(Scene.renderlist);
  free(Scene.sorted);
 }
