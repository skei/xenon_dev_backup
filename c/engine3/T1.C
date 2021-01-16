 #include <stdio.h>
 #include <io.h>
 #include <stdlib.h>
 #include <string.h>
 #include <conio.h>
 #include "engine.h"
 #include "gfx.h"

 #define ZsortMultiplier 64

void NOP(void);
 #pragma aux NOP =\
  "nop",\
  "nop",\
  "nop",\
  "nop",\
  "nop"

void mode(int mode);
 #pragma aux mode = \
  "xor ebx,ebx" \
  "xor ecx,ecx" \
  "xor edx,edx" \
  "int 10h" \
 parm [eax] modify [ebx ecx edx];

 unsigned char Pal[768];
 float nearestZ;
 char s[256];

 // ±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±

void Engine_SetupMaterials(void)
 {
  sNode *node;
  FILE *f;
  sMaterial *m;

  for (node = Engine_Scene->World; node; node = node->Next)
  {
   if (node->Type == NodeTypeMaterial)
   {
    m = (sMaterial *)node->Object;
    m->TEXTURE = 0;
    m->BUMP = 0;
    m->ENVMAP = 0;

    // --- Texture mapping ---
    if (m->Texture.File != NULL)
    {
     m->TEXTURE = 1;
     printf("þ Loading texture map: %s\n",m->Texture.File);
     f = fopen(m->Texture.File,"rb");
     if (f)
     {
      m->Texture.BitMap = (char *)malloc(65536);
      fread(m->Texture.BitMap,1,65536,f);
      fclose(f);
     }
     else
     {
      printf("  - Error loading %s. Using Default Material\n",m->Texture.File);
      m->Texture.BitMap = Engine_Scene->DefaultTexture;
     }
    }
    // --- Bump mapping ---
    if (m->Bump.File != NULL)
    {
     m->BUMP = 1;
     printf("þ Loading bump map: %s\n",m->Bump.File);
     f = fopen(m->Bump.File,"rb");
     if (f)
     {
      m->Bump.BitMap = (char *)malloc(65536);
      fread(m->Bump.BitMap,1,65536,f);
      fclose(f);
     }
     else
     {
      printf("  - Error loading %s. Using Default Material\n",m->Bump.File);
      m->Bump.BitMap = Engine_Scene->DefaultTexture;
     }
    }

    // --- Environment/Reflection mapping ---
    if (m->Reflection.File != NULL)
    {
     m->ENVMAP = 1;
     printf("þ Loading reflection map: %s\n",m->Reflection.File);
     f = fopen(m->Reflection.File,"rb");
     if (f)
     {
      m->Reflection.BitMap = (char *)malloc(65536);
      fread(m->Reflection.BitMap,1,65536,f);
      fclose(f);
     }
     else
     {
      printf("  - Error loading %s. Using Default Material\n",m->Reflection.File);
      m->Reflection.BitMap = Engine_Scene->DefaultTexture;
     }
    }

    // ---
   }
  }
 }

 // -------------------------------------------------------------------------

void Engine_ProjectObject(sObject *o, sCamera *c)
 {
  int i;
  sVector vec;
  float inv_z;
  sPolygon *poly;
  sVertex *v,*v1,*v2,*v3;
  sMatrix m,m2;

  // check if bouning sphere within view frustum.. (distance to planes)

  // --- make final transformation matrix:
  vec_negate(&o->Pivot, &vec);
  mat_settrans(&vec,&m);
  mat_mul(&o->Matrix,&m,&m);
  mat_mul(&c->M,&m,&m);

  // --- vertices ---
  for (i=0;i<o->NumVertices;i++)
  {
   v = &(o->Vertices[i]);

   // --- project all vertices

   // Clear render vertex flags
   v->Visible = 0;
   v->EnvMapDone = 0;

   mat_mulvec(&m,&v->Pos,&vec);

   if (vec.z <= 1)      // hither z clipping plane
   {
    v->pPos.z = -1;
   }
   else
   {
    v->pPos.z = vec.z;
    inv_z = 1/vec.z;
    v->pPos.x = 160+(vec.x*c->PerspX)*inv_z/*/vec.z*/;
    v->pPos.y = 100-(vec.y*c->PerspY)*inv_z/*/vec.z*/;
   }



  }
  // --- polygons ---
  poly = o->Polygons;
  for (i=0 ; i<o->NumPolygons ; i++)
  {
   v1 = poly->v[0];
   v2 = poly->v[1];
   v3 = poly->v[2];
   // if any of the Z-coordinates are zero or less, skip the entire poly
   // CHANGE THIS!!!! Clip poly if any of the vertices crosses the
   // camera/view plane!!!

   if (    (v1->pPos.z != -1)
        && (v2->pPos.z != -1)
        && (v3->pPos.z != -1))
   {

    // check z-component of face-normal
    if (  ((v2->pPos.x - v1->pPos.x) * (v3->pPos.y - v1->pPos.y))
        - ((v3->pPos.x - v1->pPos.x) * (v2->pPos.y - v1->pPos.y)) >= 0)
    {
     // mark all vertices as visible (for light etc)
     v1->Visible = 1;
     v2->Visible = 1;
     v3->Visible = 1;

     // --- EnvMap ---

     if (poly->Material->ENVMAP == 1)
     {

      if (v1->EnvMapDone==0)
      {
       mat_mulvec3 (&m,&v1->Normal,&v1->rNormal);
       v1->u = (v1->rNormal.x * EnvMapMultiplier) + (EnvMapMultiplier);
       v1->v = (v1->rNormal.y * EnvMapMultiplier) + (EnvMapMultiplier);
       v1->EnvMapDone = 1;
      }
      if (v2->EnvMapDone==0)
      {
       mat_mulvec3 (&m,&v2->Normal,&v2->rNormal);
       v2->u = (v2->rNormal.x * EnvMapMultiplier) + (EnvMapMultiplier);
       v2->v = (v2->rNormal.y * EnvMapMultiplier) + (EnvMapMultiplier);
       v2->EnvMapDone = 1;
      }
      if (v3->EnvMapDone==0)
      {
       mat_mulvec3 (&m,&v3->Normal,&v3->rNormal);
       v3->u = (v3->rNormal.x * EnvMapMultiplier) + (EnvMapMultiplier);
       v3->v = (v3->rNormal.y * EnvMapMultiplier) + (EnvMapMultiplier);
       v3->EnvMapDone = 1;
      }
     }

     // ---

     poly->SortZ = (v1->pPos.z + v2->pPos.z + v3->pPos.z);
     Engine_Scene->RenderList[ Engine_Scene->VisiblePolys ] = poly;
     Engine_Scene->VisiblePolys++;
    }
   }
   poly++;
  }
 }

 // --------------------------------------------------------------------------
 // ByteSort the "world"-object
 // --------------------------------------------------------------------------
void Engine_SortWorldObject( void )
 {
  static int Numbers[256];

  int i;
  short int a,b,d;

  // clear numbers array
  memset( Numbers,0,sizeof (Numbers));
 // Count number of each byte-value
  for ( i=0;i<Engine_Scene->VisiblePolys;i++ )
  {
   Numbers[ (Engine_Scene->RenderList[i]->SortZ) & 255 ]++;
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
  for ( i=0;i<Engine_Scene->VisiblePolys;i++ )
  {
   a = Engine_Scene->RenderList[i]->SortZ & 255; //sorttable[i].sortvalue & 255;
   d = Numbers[a];
   Engine_Scene->Sorted[d] = Engine_Scene->RenderList[i];
   Numbers[a]++;
  }
 // -
 // Now, do the same, but with high byte...
 // -
  // clear numbers array
  memset( Numbers,0,sizeof (Numbers));
 // Count number of each byte-value
  for ( i=0;i<Engine_Scene->VisiblePolys;i++ )
  {
   Numbers[ (Engine_Scene->Sorted[i]->SortZ & 0xff00) >> 8 ]++;
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
  for ( i=0;i<Engine_Scene->VisiblePolys;i++ )
  {
   a = (Engine_Scene->Sorted[i]->SortZ & 0xff00) >> 8; //sorttable[i].sortvalue & 255;
   d = Numbers[a];
   Engine_Scene->RenderList[d] = Engine_Scene->Sorted[i];
   Numbers[a]++;
  }
 }

 // -------------------------------------------------------------------------

void Engine_Draw( void )
 {
  int i;

  for (i=Engine_Scene->VisiblePolys-1 ; i>=0 ; i-- )
  {
   Poly_Tex( Engine_Scene->RenderList[i] );
  }
 }

// ±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±

void main(int argc, char *argv[])
 {
  FILE *fp;
  kfScene  *scene;
  sObject *obj;
  sNode   *node;
  sLight  *lit;
  sCamera *cam;
  sVector vec;
  int err;
  int i, x, y;
  float frame_first,frame_last,frame;
  sMaterial *mat;
  int width,height;

  // --- some setup ---

//  printf("þ Loading default TEXTURE\n");

//  scene->DefaultTexture = (char *)malloc(65536);


//  fp = fopen("texture.pal","rb");
//  if (fp)
//  {
//   fread(Pal,1,768,fp);
//   fclose(fp);
//  }
//  else printf("  - Error reading TEXTURE.PAL!\n");

  printf("þ Initializing engine\n");
  Engine_Init (FlagCalcnorm /*| FlagTransform*/ | FlagHierarchy);
  scene = Engine_AllocScene(320,200,8,10000);

  printf("þ Loading default texture TEXTURE.PIX\n");

  Load_GIF("texture.gif",NULL,
                         NULL,
                         &width,
                         &height);
  printf("Width: %i   Height: %i\n",width,height);

  scene->DefaultTexture = (char *)malloc(width*height);

  Load_GIF("texture.gif",(unsigned char *)scene->DefaultTexture,
                        Pal,
                        NULL,
                        NULL);

//  scene->DefaultTexture = (char *)malloc(65536);
//  fp = fopen("texture.pix","rb");
//  if (fp)
//  {
//   fread(scene->DefaultTexture,1,65536,fp);
//   fclose(fp);
//  }
//  else printf("  - Couldn't find TEXTURE.PIX!\n");

  printf("þ Loading 3DS file\n");
  err = Engine_LoadWorld(argv[1]/*"1.3ds"*/,scene);
  err = Engine_LoadMotion(argv[1]/*"1.3ds"*/,scene);
  Engine_SetActive(scene);
  printf("þ Calculating Normals\n");
  Engine_CalcNormals();

  printf("þ Setting up materials\n");
  Engine_SetupMaterials();

  printf("þ Setting up camera\n");
  // --- set active camera to "Camera01" ---
  node = Engine_FindByName ("Camera01");
  if (node) cam = (sCamera *)node->Object;
  cam->SizeX = 320.0;
  cam->SizeY = 200.0;
  cam->Aspectratio = 0.75;

  printf("þ Ready for rendering... Press any key...\n");
  getch();
  Engine_GetFrames(&frame_first, &frame_last);
  frame = frame_first;

  mode(0x13);

  for (i=0;i<256;i++)
  {
   outp(0x3c8,i);
   outp(0x3c9,Pal[i*3  ]);
   outp(0x3c9,Pal[i*3+1]);
   outp(0x3c9,Pal[i*3+2]);
  }

  while (!kbhit())
  {
   if (inp (0x60)==1)
   {
    mode(0x03);
    exit(0);
   }
   frame += 2;//0.5;
   if (frame >= frame_last) frame = frame_first;

   Engine_SetFrame(frame);
   // update splines, get values into objects, hierarchy matrix multiplication
   Engine_Update();
   // set up camera matrix...
   Camera_Update(cam);

   Engine_Scene->VisiblePolys = 0;
   memset(Engine_Scene->Buffer,0,64000);

 /*

   SpotLight_Update(...blahblah...);

   // -- handle shadows

   // project lights... for, all lights, ShadowBuffer in Engine_Scene->...

   for (node = scene->World; node; node=node->Next)
   {
    if (node->Type == NodeTypeObject)
    {
     obj = (sObject *)node->Object;
     Engine_ProjectObject(obj, the current SpotLight);
    }
   }
   Engine_SortWorldObject();
   Engine_ShadowDraw();

   // then, a different texture etc filler, checking sha value in the
   // shadow-buffer...

   // ---

 */

   for (node = scene->World; node; node=node->Next)
   {
    if (node->Type == NodeTypeObject)
    {
     obj = (sObject *)node->Object;
     Engine_ProjectObject(obj,cam);
    }
  /*
    else
    if (node->Type == NodeTypeLight)
    {
     // render light as bigger, blue dots
     lit = (sLight *)node->Object;
     vec_copy (&lit->Pos,&vec);
     // transform light from world to camera...
     vec_sub (&vec, &cam->Pos, &vec);
     mat_mulvec (cam->CameraMat, &vec, &vec);
     // ... and to screen...
     x = 160+vec.x*cam->PerspX/vec.z;
     y = 100-vec.y*cam->PerspY/vec.z;
     // ... draw it as blue 2x2 dot.
     if (x>=0 && x<320 && y>=0 && y<200 && vec.z > 0)
     {
      myscreen[y*320+x] = 9;
      myscreen[y*320+x+1] = 9;
      myscreen[y*320+x+320] = 9;
      myscreen[y*320+x+321] = 9;
     }
    }
  */
   }
   Engine_SortWorldObject();
   Engine_Draw();
   // show screen
   memcpy(Engine_Scene->Screen, Engine_Scene->Buffer, 64000);
  }

 //Quit:

  mode(0x03);
  printf("þ trying to free scene\n");
  Engine_FreeWorld(scene);
  Engine_FreeMotion(scene);
  printf("þ OK!\n");
 }

