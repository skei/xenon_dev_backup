 #include <string.h>
 #include <stdlib.h>
 #include <stdio.h>
 #include <math.h>
 #include "engine.h"
 // #include "math3d.h"

// --- chunks/readers definitions, structures -------------------------------

enum Chunks3DS
 {
  CHUNK_RGBF         = 0x0010, CHUNK_RGBB         = 0x0011,
  CHUNK_PRJ          = 0xC23D, CHUNK_MLI          = 0x3DAA,
  CHUNK_MAIN         = 0x4D4D, CHUNK_OBJMESH      = 0x3D3D,
  CHUNK_BKGCOLOR     = 0x1200, CHUNK_AMBCOLOR     = 0x2100,
  CHUNK_OBJBLOCK     = 0x4000, CHUNK_TRIMESH      = 0x4100,
  CHUNK_VERTLIST     = 0x4110, CHUNK_VERTFLAGS    = 0x4111,
  CHUNK_FACELIST     = 0x4120, CHUNK_FACEMAT      = 0x4130,
  CHUNK_MAPLIST      = 0x4140, CHUNK_SMOOLIST     = 0x4150,
  CHUNK_TRMATRIX     = 0x4160, CHUNK_MESHCOLOR    = 0x4165,
  CHUNK_TXTINFO      = 0x4170, CHUNK_LIGHT        = 0x4600,
  CHUNK_SPOTLIGHT    = 0x4610, CHUNK_CAMERA       = 0x4700,
  CHUNK_HIERARCHY    = 0x4F00, CHUNK_VIEWPORT     = 0x7001,
  CHUNK_MATERIAL     = 0xAFFF, CHUNK_MATNAME      = 0xA000,
  CHUNK_AMBIENT      = 0xA010, CHUNK_DIFFUSE      = 0xA020,
  CHUNK_SPECULAR     = 0xA030, CHUNK_TEXTURE      = 0xA200,
  CHUNK_BUMPMAP      = 0xA230, CHUNK_REFLECTION   = 0xA220,
  CHUNK_MAPFILE      = 0xA300, CHUNK_MAPFLAGS     = 0xA351,
  CHUNK_MAPUSCALE    = 0xA354, CHUNK_MAPVSCALE    = 0xA356,
  CHUNK_MAPUOFFSET   = 0xA358, CHUNK_MAPVOFFSET   = 0xA35A,
  CHUNK_KEYFRAMER    = 0xB000, CHUNK_AMBIENTKEY   = 0xB001,
  CHUNK_TRACKINFO    = 0xB002, CHUNK_TRACKOBJNAME = 0xB010,
  CHUNK_TRACKPIVOT   = 0xB013, CHUNK_TRACKPOS     = 0xB020,
  CHUNK_TRACKROTATE  = 0xB021, CHUNK_TRACKSCALE   = 0xB022,
  CHUNK_TRACKMORPH   = 0xB026, CHUNK_TRACKHIDE    = 0xB029,
  CHUNK_OBJNUMBER    = 0xB030, CHUNK_TRACKCAMERA  = 0xB003,
  CHUNK_TRACKFOV     = 0xB023, CHUNK_TRACKROLL    = 0xB024,
  CHUNK_TRACKCAMTGT  = 0xB004, CHUNK_TRACKLIGHT   = 0xB005,
  CHUNK_TRACKLIGTGT  = 0xB006, CHUNK_TRACKSPOTL   = 0xB007,
  CHUNK_TRACKCOLOR   = 0xB025, CHUNK_FRAMES       = 0xB008,
  CHUNK_DUMMYNAME    = 0xB011, CHUNK_MAPROTANGLE  = 0xA35C,
  CHUNK_SHININESS    = 0xA040, CHUNK_SHINSTRENGTH = 0xA041,
  CHUNK_TRANSPARENCY = 0xA050, CHUNK_TRANSFALLOFF = 0xA052,
  CHUNK_REFBLUR      = 0xA053, CHUNK_SELFILLUM    = 0xA084,
  CHUNK_TWOSIDED     = 0xA081, CHUNK_TRANSADD     = 0xA083,
  CHUNK_WIREON       = 0xA085, CHUNK_SOFTEN       = 0xA08C
 };

 #pragma pack(2)

typedef struct          // 3DS chunk structure
 {
  word  id;                      // chunk id
  dword len;                     // chunk length
 } c_HEADER;

 #pragma pack()

typedef struct          // Chunk reader list
 {
  word id;                       // chunk id
  int  sub;                      // has subchunks
  int  (*func) (FILE *f);        // reader function
 } c_CHUNK;

 static int read_NULL         (FILE *f); /* (skip chunk)            */
 static int read_RGBF         (FILE *f); /* RGB float               */
 static int read_RGBB         (FILE *f); /* RGB byte                */
 static int read_ASCIIZ       (FILE *f); /* ASCIIZ string           */
 static int read_TRIMESH      (FILE *f); /* Triangular mesh         */
 static int read_VERTLIST     (FILE *f); /* Vertex list             */
 static int read_FACELIST     (FILE *f); /* Face list               */
 static int read_FACEMAT      (FILE *f); /* Face material           */
 static int read_MAPLIST      (FILE *f); /* Mapping list            */
 static int read_TRMATRIX     (FILE *f); /* Transformation matrix   */
 static int read_LIGHT        (FILE *f); /* Light                   */
 static int read_SPOTLIGHT    (FILE *f); /* Spotlight               */
 static int read_CAMERA       (FILE *f); /* Camera                  */
 static int read_MATERIAL     (FILE *f); /* Material                */
 static int read_MATNAME      (FILE *f); /* Material name           */
 static int read_FRAMES       (FILE *f); /* Number of frames        */
 static int read_OBJNUMBER    (FILE *f); /* Object number           */
 static int read_TRACKOBJNAME (FILE *f); /* Track object name       */
 static int read_DUMMYNAME    (FILE *f); /* Dummy object name       */
 static int read_TRACKPIVOT   (FILE *f); /* Track pivot point       */
 static int read_TRACKPOS     (FILE *f); /* Track position          */
 static int read_TRACKCOLOR   (FILE *f); /* Track color             */
 static int read_TRACKROT     (FILE *f); /* Track rotation          */
 static int read_TRACKSCALE   (FILE *f); /* Track scale             */
 static int read_TRACKFOV     (FILE *f); /* Track fov               */
 static int read_TRACKROLL    (FILE *f); /* Track roll              */
 static int read_TRACKMORPH   (FILE *f); /* Track morph             */
 static int read_TRACKHIDE    (FILE *f); /* Track hide              */
 static int read_MATTYPE      (FILE *f); /* Material: type          */
 static int read_MATTWOSIDED  (FILE *f); /* Material: two sided     */
 static int read_MATSOFTEN    (FILE *f); /* Material: soften        */
 static int read_MATWIRE      (FILE *f); /* Material: wire          */
 static int read_MATTRANSADD  (FILE *f); /* Material: transparency  */
 static int read_MAPFLAGS     (FILE *f); /* Map flags               */
 static int read_MAPFILE      (FILE *f); /* Map file                */
 static int read_MAPUSCALE    (FILE *f); /* Map 1/U scale           */
 static int read_MAPVSCALE    (FILE *f); /* Map 1/V scale           */
 static int read_MAPUOFFSET   (FILE *f); /* Map U offset            */
 static int read_MAPVOFFSET   (FILE *f); /* Map V offset            */
 static int read_MAPROTANGLE  (FILE *f); /* Map rotation angle      */

static c_CHUNK world_chunks[] =       // World definition chunks
 {
  {CHUNK_RGBF,         0, read_RGBF},
  {CHUNK_RGBB,         0, read_RGBB},
  {CHUNK_PRJ,          1, read_NULL},
  {CHUNK_MLI,          1, read_NULL},
  {CHUNK_MAIN,         1, read_NULL},
  {CHUNK_OBJMESH,      1, read_NULL},
  {CHUNK_BKGCOLOR,     1, read_NULL},
  {CHUNK_AMBCOLOR,     1, read_NULL},
  {CHUNK_OBJBLOCK,     1, read_ASCIIZ},
  {CHUNK_TRIMESH,      1, read_TRIMESH},
  {CHUNK_VERTLIST,     0, read_VERTLIST},
  {CHUNK_VERTFLAGS,    0, read_NULL},
  {CHUNK_FACELIST,     1, read_FACELIST},
  {CHUNK_MESHCOLOR,    0, read_NULL},
  {CHUNK_FACEMAT,      0, read_FACEMAT},
  {CHUNK_MAPLIST,      0, read_MAPLIST},
  {CHUNK_TXTINFO,      0, read_NULL},
  {CHUNK_SMOOLIST,     0, read_NULL},
  {CHUNK_TRMATRIX,     0, read_TRMATRIX},
  {CHUNK_LIGHT,        1, read_LIGHT},
  {CHUNK_SPOTLIGHT,    0, read_SPOTLIGHT},
  {CHUNK_CAMERA,       0, read_CAMERA},
  {CHUNK_HIERARCHY,    1, read_NULL},
  {CHUNK_VIEWPORT,     0, read_NULL},
  {CHUNK_MATERIAL,     1, read_MATERIAL},
  {CHUNK_MATNAME,      0, read_MATNAME},
  {CHUNK_AMBIENT,      1, read_NULL},
  {CHUNK_DIFFUSE,      1, read_NULL},
  {CHUNK_SPECULAR,     1, read_NULL},
  {CHUNK_TEXTURE,      1, read_NULL},
  {CHUNK_BUMPMAP,      1, read_NULL},
  {CHUNK_REFLECTION,   1, read_NULL},
  {CHUNK_MAPFILE,      0, read_MAPFILE},
  {CHUNK_MAPFLAGS,     0, read_MAPFLAGS},
  {CHUNK_MAPUSCALE,    0, read_MAPUSCALE},
  {CHUNK_MAPVSCALE,    0, read_MAPVSCALE},
  {CHUNK_MAPUOFFSET,   0, read_MAPUOFFSET},
  {CHUNK_MAPVOFFSET,   0, read_MAPVOFFSET},
  {CHUNK_MAPROTANGLE,  0, read_MAPROTANGLE},
  {CHUNK_SHININESS,    1, read_NULL},
  {CHUNK_SHINSTRENGTH, 1, read_NULL},
  {CHUNK_TRANSPARENCY, 1, read_NULL},
  {CHUNK_TRANSFALLOFF, 1, read_NULL},
  {CHUNK_REFBLUR,      1, read_NULL},
  {CHUNK_SELFILLUM,    1, read_NULL},
  {CHUNK_TWOSIDED,     0, read_MATTWOSIDED},
  {CHUNK_TRANSADD,     0, read_MATTRANSADD},
  {CHUNK_WIREON,       0, read_MATWIRE},
  {CHUNK_SOFTEN,       0, read_MATSOFTEN}
 };

static c_CHUNK key_chunks[] =         // Keyframer chunks
 {
  {CHUNK_MAIN,         1, read_NULL},
  {CHUNK_KEYFRAMER,    1, read_NULL},
  {CHUNK_AMBIENTKEY,   1, read_NULL},
  {CHUNK_TRACKINFO,    1, read_NULL},
  {CHUNK_FRAMES,       0, read_FRAMES},
  {CHUNK_TRACKOBJNAME, 0, read_TRACKOBJNAME},
  {CHUNK_DUMMYNAME,    0, read_DUMMYNAME},
  {CHUNK_TRACKPIVOT,   0, read_TRACKPIVOT},
  {CHUNK_TRACKPOS,     0, read_TRACKPOS},
  {CHUNK_TRACKCOLOR,   0, read_TRACKCOLOR},
  {CHUNK_TRACKROTATE,  0, read_TRACKROT},
  {CHUNK_TRACKSCALE,   0, read_TRACKSCALE},
  {CHUNK_TRACKMORPH,   0, read_TRACKMORPH},
  {CHUNK_TRACKHIDE,    0, read_TRACKHIDE},
  {CHUNK_OBJNUMBER,    0, read_OBJNUMBER},
  {CHUNK_TRACKCAMERA,  1, read_NULL},
  {CHUNK_TRACKCAMTGT,  1, read_NULL},
  {CHUNK_TRACKLIGHT,   1, read_NULL},
  {CHUNK_TRACKLIGTGT,  1, read_NULL},
  {CHUNK_TRACKSPOTL,   1, read_NULL},
  {CHUNK_TRACKFOV,     0, read_TRACKFOV},
  {CHUNK_TRACKROLL,    0, read_TRACKROLL}
 };

 static int   c_chunk_last;       /* parent chunk      */
 static int   c_chunk_prev;       /* previous chunk    */
 static int   c_chunk_curr;       /* current chunk     */
 static int   c_id;               /* current id        */
 static char  c_string[64];       /* current name      */
 static void *c_node;             /* current node      */

// --- misc. routines -------------------------------------------------------

 // swap y/z in vector.
static void vec_swap (sVector *a)
 {
  float tmp;

  #ifdef ENGINE_SWAP_YZ
   tmp  = a->y;
   a->y = a->z;
   a->z = tmp;
  #endif
 }

 // -------------------------------------------------------------------------

 // swap y/z in vector.
static void qt_swap (sQuat *a)
 {
  float tmp;

  #ifdef ENGINE_SWAP_YZ
   tmp  = a->y;
   a->y = a->z;
   a->z = tmp;
  #endif
 }

 // -------------------------------------------------------------------------

 // swap y/z in matrix.
static void mat_swap (sMatrix a)
 {
  int   i;
  float tmp;

  #ifdef ENGINE_SWAP_YZ
   for (i = 0; i < 3; i++)
   {
    // swap columns
    tmp = a[i][qY];
    a[i][qY] = a[i][qZ];
    a[i][qZ] = tmp;
   }
   for (i = 0; i < 4; i++)
   {
    // swap rows
    tmp = a[qY][i];
    a[qY][i] = a[qZ][i];
    a[qZ][i] = tmp;
   }
  #endif
 }

 // -------------------------------------------------------------------------

 // copies asciiz string with memory allocation.
static char *strcopy (char *src)
 {
  char *temp;

  if ((temp = (char *)malloc (strlen (src)+1)) == NULL) return NULL;
  strcpy (temp, src);
  return temp;
 }

 // -------------------------------------------------------------------------

 // add_key: add a key to track.
static int add_key (kfKey **track, kfKey *key)
 {
  kfKey *curr;

  if (track == NULL || key == NULL) return ERROR;
  key->Next = NULL;
  if (*track == NULL)
  {
   key->Prev = NULL;
   *track = key;
  }
  else
  {
   curr = *track;
   while (curr->Next) curr = curr->Next;
   key->Prev = curr;
   curr->Next = key;
  }
  return OK;
 }

// --- chunk readers (world) ------------------------------------------------

 // read_NULL: "dummy" chunk reader.
static int read_NULL (FILE *f)
 {
  if (f) {}           // to skip the warning
  return OK;
 }

 // -------------------------------------------------------------------------

 // read_RGBF: RGB Float reader.
static int read_RGBF (FILE *f)
 {
  sMaterial *mat = (sMaterial *)c_node;
  sRGB      *rgb = NULL;
  float       c[3];

  switch (c_chunk_last)
  {
   case CHUNK_LIGHT:        rgb = &(((sLight *)c_node)->Color); break;
   case CHUNK_AMBIENT:      rgb = &(mat->Ambient); break;
   case CHUNK_DIFFUSE:      rgb = &(mat->Diffuse); break;
   case CHUNK_SPECULAR:     rgb = &(mat->Specular); break;
   case CHUNK_SHININESS:    rgb = &(mat->Shininess); break;
   case CHUNK_SHINSTRENGTH: rgb = &(mat->ShinStrength); break;
   case CHUNK_TRANSPARENCY: rgb = &(mat->Transparency); break;
   case CHUNK_TRANSFALLOFF: rgb = &(mat->TransFalloff); break;
   case CHUNK_REFBLUR:      rgb = &(mat->RefBlur); break;
   case CHUNK_SELFILLUM:    rgb = &(mat->SelfIllum);
  }
  if (fread (c, sizeof (c), 1, f) != 1) return ERROR;
  if (rgb)
  {
   rgb->r = c[0];
   rgb->g = c[1];
   rgb->b = c[2];
  }
  return OK;
 }

 // -------------------------------------------------------------------------

 // RGB Byte reader.
static int read_RGBB (FILE *f)
 {
  sMaterial *mat = (sMaterial *)c_node;
  sRGB      *rgb = NULL;
  byte        c[3];

  switch (c_chunk_last)
  {
   case CHUNK_LIGHT:        rgb = &(((sLight *)c_node)->Color); break;
   case CHUNK_AMBIENT:      rgb = &(mat->Ambient); break;
   case CHUNK_DIFFUSE:      rgb = &(mat->Diffuse); break;
   case CHUNK_SPECULAR:     rgb = &(mat->Specular); break;
   case CHUNK_SHININESS:    rgb = &(mat->Shininess); break;
   case CHUNK_SHINSTRENGTH: rgb = &(mat->ShinStrength); break;
   case CHUNK_TRANSPARENCY: rgb = &(mat->Transparency); break;
   case CHUNK_TRANSFALLOFF: rgb = &(mat->TransFalloff); break;
   case CHUNK_REFBLUR:      rgb = &(mat->RefBlur); break;
   case CHUNK_SELFILLUM:    rgb = &(mat->SelfIllum);
  }
  if (fread (c, sizeof (c), 1, f) != 1) return ERROR;
  if (rgb)
  {
   rgb->r = (float)c[0] / 255.0;
   rgb->g = (float)c[1] / 255.0;
   rgb->b = (float)c[2] / 255.0;
  }
  return OK;
 }

 // -------------------------------------------------------------------------

 // ASCIIZ string reader.
static int read_ASCIIZ (FILE *f)
 {
  char *s = c_string;
  int   c;

  while ((c = fgetc (f)) != EOF && c != '\0') *s++ = (char)c;
  if (c == EOF) return ERROR;
  *s = '\0';
  return OK;
 }

 // -------------------------------------------------------------------------

 // Triangular mesh reader.
static int read_TRIMESH (FILE *f)
 {
  sObject *obj;

  if (f) {} /* to skip the warning */
  if ((obj = (sObject *)malloc (sizeof (sObject))) == NULL) return ERROR;
  if ((obj->Name = strcopy (c_string)) == NULL) return ERROR;
  obj->Id = c_id++;
  obj->Parent = -1;
  obj->Flags = 0;
  c_node = obj;
  Engine_AddWorld (NodeTypeObject, obj);
  return OK;
 }

 // -------------------------------------------------------------------------

 // Vertex list reader.
static int read_VERTLIST (FILE *f)
 {
  sObject *obj;
  sVertex *v;
  float     c[3];
  word      nv;
  int i;

  obj = (sObject *)c_node;
  if (fread (&nv, sizeof (nv), 1, f) != 1) return ERROR;
  if ((v = (sVertex *)malloc (nv * sizeof (sVertex))) == NULL) return ERROR;
  obj->Vertices = v;
  obj->NumVertices = nv;
  for (i=0;i<nv;i++)
  {
   if (fread (c, sizeof (c), 1, f) != 1) return ERROR;
   vec_make (c[0], c[1], c[2], &(obj->Vertices[i].Pos));
   vec_swap (&(obj->Vertices[i].Pos));
  }
  return OK;
 }

 // -------------------------------------------------------------------------

 // Face list reader.
static int read_FACELIST (FILE *f)
 {
  sObject *obj = (sObject *)c_node;
  sPolygon   *fc;
  word      c[3];
  word      nv, flags;

  if (fread (&nv, sizeof (nv), 1, f) != 1) return ERROR;
  if ((fc = (sPolygon *)malloc (nv * sizeof (sPolygon))) == NULL) return ERROR;
  obj->Polygons = fc;
  obj->NumPolygons = nv;
  while (nv-- > 0)
  {
   if (fread (c, sizeof (c), 1, f) != 1) return ERROR;
   if (fread (&flags, sizeof (flags), 1, f) != 1) return ERROR;
   fc->v[0] = &(obj->Vertices[c[2]]);     // ??? Why turn them around???
   fc->v[1] = &(obj->Vertices[c[1]]);
   fc->v[2] = &(obj->Vertices[c[0]]);
   fc->Flags = 0;
   if (flags & 0x08) fc->Flags |= FaceFlagWrapU;
   if (flags & 0x10) fc->Flags |= FaceFlagWrapV;
   fc++;
  }
  return OK;
 }

 // -------------------------------------------------------------------------

 // Face material reader.
static int read_FACEMAT (FILE *f)
 {
  sPolygon *fc = ((sObject *)c_node)->Polygons;
  sNode *node;
  sMaterial *mat;
  word n, nf;

  if (read_ASCIIZ (f)) return ERROR;
  if (fread (&n, sizeof (n), 1, f) != 1) return ERROR;
  node = Engine_FindByName(c_string);
  if (!node) return ERROR;
  mat = (sMaterial *)node->Object;
  while (n-- > 0)
  {
   if (fread (&nf, sizeof (nf), 1, f) != 1) return ERROR;
   fc[nf].MaterialId = mat->Id;
   fc[nf].Material = mat;
  }
  return OK;
 }

 // -------------------------------------------------------------------------

 // Map list reader.
static int read_MAPLIST (FILE *f)
 {
  sVertex *v = ((sObject *)c_node)->Vertices;
  float c[2];
  word nv;
  sObject *o;

  o = (sObject *)c_node;
  o->Flags |= ObjFlagMapped;

  if (fread (&nv, sizeof (nv), 1, f) != 1) return ERROR;
  while (nv-- > 0)
  {
   if (fread (c, sizeof (c), 1, f) != 1) return ERROR;
   v->u = (c[0]*UVmultiplier);
   v->v = (c[1]*UVmultiplier);
   v++;
  }
  return OK;
 }

 // -------------------------------------------------------------------------

 // Transformation matrix reader.
static int read_TRMATRIX (FILE *f)
 {
  sObject *obj = (sObject *)c_node;
  sVertex *v = obj->Vertices;
  sVector  piv;
  sMatrix  mat;
  float     pivot[3];
  int       i, j;

  mat_identity (mat);
  for (i = 0; i < 3; i++)
  {
   for (j = 0; j < 3; j++)
   {
    if (fread (&mat[i][j], sizeof (float), 1, f) != 1) return ERROR;
   }
  }
  if (fread (pivot, sizeof (pivot), 1, f) != 1) return ERROR;
  vec_make (pivot[0], pivot[1], pivot[2], &piv);
  vec_swap (&piv);
  mat_swap (mat);
  // if (mat_inverse (mat, mat) != OK) return ERROR;
  for (i = 0; i < obj->NumVertices; i++)
  {
   vec_sub (&v->Pos, &piv, &v->Pos);      // pos = pos-pivot
   mat_mulvec (mat, &v->Pos, &v->Pos);    // pos = pos*matrix
   v++;
  }
  return OK;
 }

 // -------------------------------------------------------------------------

 // Light reader.
static int read_LIGHT (FILE *f)
 {
  float   c[3];
  sLight *light;

  if ((light = (sLight *)malloc (sizeof (sLight))) == NULL) return ERROR;
  if (fread (c, sizeof (c), 1, f) != 1) return ERROR;
  if ((light->Name = strcopy (c_string)) == NULL) return ERROR;
  light->Id = c_id++;
  light->Flags = LightFlagOmni;
  vec_make (c[0], c[1], c[2], &light->Pos);
  vec_swap (&light->Pos);
  c_node = light;
  Engine_AddWorld(NodeTypeLight, light);
  return OK;
 }

 // -------------------------------------------------------------------------

 // Spot light reader.
static int read_SPOTLIGHT (FILE *f)
 {
  float   c[5];
  sLight *light = (sLight *)c_node;

  if (fread (c, sizeof (c), 1, f) != 1) return ERROR;
  light->Target.x = c[0];
  light->Target.y = c[1];
  light->Target.z = c[2];
  light->Hotspot = c[3];
  light->Falloff = c[4];
  light->Flags = LightFlagSpot;
  light->Roll = 0;
  vec_swap (&light->Target);
  return OK;
 }

 // -------------------------------------------------------------------------

 // Camera reader.
static int read_CAMERA (FILE *f)
 {
  float    c[8];
  sCamera *cam;

  if ((cam = (sCamera *)malloc (sizeof (sCamera))) == NULL) return ERROR;
  if (fread (c, sizeof (c), 1, f) != 1) return ERROR;
  if ((cam->Name = strcopy (c_string)) == NULL) return ERROR;
  cam->Id = c_id++;
  cam->Roll = c[6];
  cam_lens_fov (c[7], &cam->Fov);
  vec_make (c[0], c[1], c[2], &cam->Pos);
  vec_make (c[3], c[4], c[5], &cam->Target);
  vec_swap (&cam->Pos);
  vec_swap (&cam->Target);
  c_node = cam;
  Engine_AddWorld (NodeTypeCamera, cam);
  return OK;
 }

 // -------------------------------------------------------------------------

 // Material reader.
static int read_MATERIAL (FILE *f)
 {
  sMaterial *mat;

  if (f) {} /* to skip the warning */
  if ((mat = (sMaterial *)malloc (sizeof (sMaterial))) == NULL) return ERROR;
  mat->Id = c_id++;
  mat->Shading = 0;
  mat->Flags = 0;
  mat->Texture.File = NULL;
  mat->Texture.Uscale = 0.0;
  mat->Texture.Vscale = 0.0;
  mat->Texture.Uoffset = 0.0;
  mat->Texture.Voffset = 0.0;
  mat->Texture.RotAngle = 0.0;
  mat->Bump.File = NULL;
  mat->Bump.Uscale = 0.0;
  mat->Bump.Vscale = 0.0;
  mat->Bump.Uoffset = 0.0;
  mat->Bump.Voffset = 0.0;
  mat->Bump.RotAngle = 0.0;
  mat->Reflection.File = NULL;
  mat->Reflection.Uscale = 0.0;
  mat->Reflection.Vscale = 0.0;
  mat->Reflection.Uoffset = 0.0;
  mat->Reflection.Voffset = 0.0;
  mat->Reflection.RotAngle = 0.0;
  c_node = mat;
  Engine_AddWorld(NodeTypeMaterial, mat);
  return OK;
 }

 // -------------------------------------------------------------------------

 // Material name reader.
static int read_MATNAME (FILE *f)
 {
  sMaterial *mat = (sMaterial *)c_node;
  if (read_ASCIIZ (f)) return ERROR;
  if ((mat->Name = strcopy (c_string)) == NULL) return ERROR;
  return OK;
 }

 // -------------------------------------------------------------------------

static int read_MATTYPE (FILE *f)
 {
  // read_MATTYPE: Material type reader.
  sMaterial *mat = (sMaterial *)c_node;
  word        type;

  if (fread (&type, sizeof (type), 1, f) != 1) return ERROR;
  mat->Shading = type;
  return OK;
 }

 // -------------------------------------------------------------------------

static int read_MATTWOSIDED (FILE *f)
 {
  // read_MATTWOSIDED: Material two sided reader.
  sMaterial *mat = (sMaterial *)c_node;

  if (f) {} /* to skip the warning */
  mat->Flags |= MatFlagTwoSided;
  return OK;
 }

 // -------------------------------------------------------------------------

static int read_MATSOFTEN (FILE *f)
 {
  // read_MATSOFTEN: Material soften reader.
  sMaterial *mat = (sMaterial *)c_node;

  if (f) {} /* to skip the warning */
  mat->Flags |= MatFlagSoften;
  return OK;
 }

 // -------------------------------------------------------------------------

static int read_MATWIRE (FILE *f)
 {
  // Material wireframe reader.
  sMaterial *mat = (sMaterial *)c_node;

  if (f) {} /* to skip the warning */
  mat->Flags |= MatFlagWire;
  return OK;
 }

 // -------------------------------------------------------------------------

static int read_MATTRANSADD (FILE *f)
 {
  // Material transparency add reader.
  sMaterial *mat = (sMaterial *)c_node;

  if (f) {} /* to skip the warning */
  mat->Flags |= MatFlagTransAdd;
  return OK;
 }

 // -------------------------------------------------------------------------

static int read_MAPFILE (FILE *f)
 {
  // MAP file reader.
  sMaterial *mat = (sMaterial *)c_node;
  sMap      *map = NULL;

  if (read_ASCIIZ (f)) return ERROR;
  switch (c_chunk_last)
  {
   case CHUNK_TEXTURE: map = &(mat->Texture); break;
   case CHUNK_BUMPMAP: map = &(mat->Bump); break;
   case CHUNK_REFLECTION: map = &(mat->Reflection);
  }
  if (map) if ((map->File = strcopy (c_string)) == NULL) return ERROR;
 /*
  // Try to load texture file...
  tex = fopen(map->File,"rb");
  filesize = filelength(fileno(tex));
  if (tex)
  {
   map->BitMap = (char *)malloc(filesize);
   fread(map->BitMap,1,filesize,tex);
  }
  fclose(tex);
 */
  return OK;
 }

 // -------------------------------------------------------------------------

static int read_MAPFLAGS (FILE *f)
 {
  // MAP flags reader.
  sMaterial *mat = (sMaterial *)c_node;
  sMap      *map = NULL;
  word        flags;

  if (fread (&flags, sizeof (flags), 1, f) != 1) return ERROR;
  switch (c_chunk_last)
  {
   case CHUNK_TEXTURE: map = &(mat->Texture); break;
   case CHUNK_BUMPMAP: map = &(mat->Bump); break;
   case CHUNK_REFLECTION: map = &(mat->Reflection);
  }
  if (map) map->Flags = flags;
  return OK;
 }

 // -------------------------------------------------------------------------

static int read_MAPUSCALE (FILE *f)
 {
  // MAP U scale reader.
  sMaterial *mat = (sMaterial *)c_node;
  sMap      *map = NULL;
  float       U;

  if (fread (&U, sizeof (U), 1, f) != 1) return ERROR;
  switch (c_chunk_last)
  {
   case CHUNK_TEXTURE: map = &(mat->Texture); break;
   case CHUNK_BUMPMAP: map = &(mat->Bump); break;
   case CHUNK_REFLECTION: map = &(mat->Reflection);
  }
  if (map) map->Uscale = U;
  return OK;
 }

 // -------------------------------------------------------------------------

static int read_MAPVSCALE (FILE *f)
 {
  // MAP U scale reader.
  sMaterial *mat = (sMaterial *)c_node;
  sMap      *map = NULL;
  float       V;

  if (fread (&V, sizeof (V), 1, f) != 1) return ERROR;
  switch (c_chunk_last)
  {
   case CHUNK_TEXTURE: map = &(mat->Texture); break;
   case CHUNK_BUMPMAP: map = &(mat->Bump); break;
   case CHUNK_REFLECTION: map = &(mat->Reflection);
  }
  if (map) map->Vscale = V;
  return OK;
 }

 // -------------------------------------------------------------------------

static int read_MAPUOFFSET (FILE *f)
 {
  // read_MAPUSCALE: MAP U offset reader.
  sMaterial *mat = (sMaterial *)c_node;
  sMap      *map = NULL;
  float       U;

  if (fread (&U, sizeof (U), 1, f) != 1) return ERROR;
  switch (c_chunk_last)
  {
   case CHUNK_TEXTURE: map = &(mat->Texture); break;
   case CHUNK_BUMPMAP: map = &(mat->Bump); break;
   case CHUNK_REFLECTION: map = &(mat->Reflection);
  }
  if (map) map->Uoffset = U;
  return OK;
 }

 // -------------------------------------------------------------------------

static int read_MAPVOFFSET (FILE *f)
 {
  // MAP V offset reader.
  sMaterial *mat = (sMaterial *)c_node;
  sMap      *map = NULL;
  float       V;

  if (fread (&V, sizeof (V), 1, f) != 1) return ERROR;
  switch (c_chunk_last)
  {
   case CHUNK_TEXTURE: map = &(mat->Texture); break;
   case CHUNK_BUMPMAP: map = &(mat->Bump); break;
   case CHUNK_REFLECTION: map = &(mat->Reflection);
  }
  if (map) map->Voffset = V;
  return OK;
 }

 // -------------------------------------------------------------------------

static int read_MAPROTANGLE (FILE *f)
 {
  // MAP rotation angle reader.
  sMaterial *mat = (sMaterial *)c_node;
  sMap      *map = NULL;
  float       angle;

  if (fread (&angle, sizeof (angle), 1, f) != 1) return ERROR;
  switch (c_chunk_last)
  {
   case CHUNK_TEXTURE: map = &(mat->Texture); break;
   case CHUNK_BUMPMAP: map = &(mat->Bump); break;
   case CHUNK_REFLECTION: map = &(mat->Reflection);
  }
  if (map) map->RotAngle = angle;
  return OK;
 }

// --- chunk readers (keyframer) --------------------------------------------

static int read_FRAMES (FILE *f)
 {
  // Frames reader.
  dword c[2];

  if (fread (c, sizeof (c), 1, f) != 1) return ERROR;
  Engine_Scene->Start = c[0];
  Engine_Scene->End = c[1];
  return OK;
 }

 // -------------------------------------------------------------------------

static int read_OBJNUMBER (FILE *f)
 {
  // Object number reader. (3DS 4.0+)
  word n;

  if (fread (&n, sizeof (n), 1, f) != 1) return ERROR;
  c_id = n;
  return OK;
 }

 // -------------------------------------------------------------------------

static int read_DUMMYNAME (FILE *f)
 {
  // Dummy object name reader.
  sObject *obj = (sObject *)c_node;

  if (read_ASCIIZ (f)) return ERROR;
  if ((obj->Name = strcopy (c_string)) == NULL) return ERROR;
  return OK;
 }

 // -------------------------------------------------------------------------

static int read_TRACKOBJNAME (FILE *f)
 {
  // Track object name reader.
  sNode    *node;
  sObject  *obj;
  sLight   *light;
  sCamera  *cam;
  sAmbient *amb;
  void      *track;
  word       flags[2];
  sword      parent;

  /* for 3DS 3.0 compatibility */
  if (c_chunk_prev != CHUNK_OBJNUMBER) c_id++;

  if (read_ASCIIZ (f)) return ERROR;
  if (strcmp (c_string, "$AMBIENT$") == 0)
  {
   if ((amb = (sAmbient *)malloc (sizeof (sAmbient))) == NULL) return ERROR;
   if ((amb->Name = strcopy (c_string)) == NULL) return ERROR;
   amb->Id = 1024+c_id;
   Engine_AddWorld (NodeTypeAmbient, amb);
  }
  else
  if (strcmp (c_string, "$$$DUMMY") == 0)
  {
   if ((obj = (sObject *)malloc (sizeof (sObject))) == NULL) return ERROR;
   obj->Id = 1024+c_id;
   obj->Flags |= ObjFlagDummy;
   obj->NumVertices = 0;
   obj->NumPolygons = 0;
   obj->Vertices = NULL;
   obj->Polygons = NULL;
   Engine_AddWorld (NodeTypeObject, obj);
  }
  else
  {
   node = Engine_FindByName (c_string);
   if (!node) return ERROR;
   obj = (sObject *)node->Object;
   cam = (sCamera *)node->Object;
   light = (sLight *)node->Object;
  }
  if (fread (flags, sizeof (flags), 1, f) != 1) return ERROR;
  if (fread (&parent, sizeof (parent), 1, f) != 1) return ERROR;
  if (c_chunk_last == CHUNK_TRACKINFO)
  {
   obj->Parent = parent;
   if (flags[0] & 0x800) obj->Flags |= ObjFlagHidden;
   if ((track = malloc (sizeof (sOBJECT))) == NULL) return ERROR;
   memset (track, 0, sizeof (sOBJECT));
   Engine_AddTrack(TrackTypeObject, c_id, parent, track, obj);
   c_node = obj;
  }
  if (c_chunk_last == CHUNK_TRACKCAMTGT)
  {
   cam->Parent2 = parent;
   if ((track = malloc (sizeof (sCAMERATGT))) == NULL) return ERROR;
   memset (track, 0, sizeof (sCAMERATGT));
   Engine_AddTrack(TrackTypeCameratgt, c_id, parent, track, cam);
  }
  if (c_chunk_last == CHUNK_TRACKCAMERA)
  {
   cam->Parent1 = parent;
   if ((track = malloc (sizeof (sCAMERA))) == NULL) return ERROR;
   memset (track, 0, sizeof (sCAMERA));
   Engine_AddTrack(TrackTypeCamera, c_id, parent, track, cam);
  }
  if (c_chunk_last == CHUNK_TRACKLIGHT)
  {
   light->Parent1 = parent;
   if ((track = malloc (sizeof (sLIGHT))) == NULL) return ERROR;
   memset (track, 0, sizeof (sLIGHT));
   Engine_AddTRack(TrackTypeLight, c_id, parent, track, light);
  }
  if (c_chunk_last == CHUNK_TRACKSPOTL)
  {
   light->Parent1 = parent;
   if ((track = malloc (sizeof (sSPOTLIGHT))) == NULL) return ERROR;
   memset (track, 0, sizeof (sSPOTLIGHT));
   Engine_AddTrack(TrackTypeSpotlight, c_id, parent, track, light);
  }
  if (c_chunk_last == CHUNK_TRACKLIGTGT)
  {
   light->Parent2 = parent;
   if ((track = malloc (sizeof (sLIGHTTGT))) == NULL) return ERROR;
   memset (track, 0, sizeof (sLIGHTTGT));
   Engine_AddTrack (TrackTypeLighttgt, c_id, parent, track, light);
  }
  if (c_chunk_last == CHUNK_AMBIENTKEY)
  {
   if ((track = malloc (sizeof (sAMBIENT))) == NULL) return ERROR;
   memset (track, 0, sizeof (sAMBIENT));
   Engine_AddTrack (TrackTypeAmbient, c_id, parent, track, amb);
  }
  return OK;
 }

 // -------------------------------------------------------------------------

static int read_TRACKPIVOT (FILE *f)
 {
  // Track pivot point reader.
  float    pos[3];
  sNode  *node;
  sObject *obj = (sObject *)c_node;

  if (fread (pos, sizeof (pos), 1, f) != 1) return ERROR;
  vec_make (pos[0], pos[1], pos[2], &obj->Pivot);
  vec_swap (&obj->Pivot);
  return OK;
 }

 // -------------------------------------------------------------------------

static int read_TFLAGS (FILE *f, word *nf, kfKey *key)
 {
  // Track flags/spline reader.
  word  unknown, flags;
  int   i;
  float dat;

  key->Tens = 0.0;
  key->Cont = 0.0;
  key->Bias = 0.0;
  key->EaseTo = 0.0;
  key->EaseFrom = 0.0;
  if (fread (nf, sizeof (word), 1, f) != 1) return ERROR;
  if (fread (&unknown, sizeof (word), 1, f) != 1) return ERROR;
  if (fread (&flags, sizeof (flags), 1, f) != 1) return ERROR;
  for (i = 0; i < 16; i++)
  {
   if (flags & (1 << i))
   {
    if (fread (&dat, sizeof (dat), 1, f) != 1) return ERROR;
    switch (i)
    {
     case 0: key->Tens = dat; break;
     case 1: key->Cont = dat; break;
     case 2: key->Bias = dat; break;
     case 3: key->EaseTo = dat; break;
     case 4: key->EaseFrom = dat;
    }
   }
  }
  return OK;
 }

 // -------------------------------------------------------------------------

static int read_TRACKPOS (FILE *f)
 {
  // Track position reader.
  kfKey *key, *track = NULL;
  float  pos[3];
  word   unknown[5];
  word   n, nf;

  if (fread (unknown, sizeof (word), 5, f) != 5) return ERROR;
  if (fread (&n, sizeof (n), 1, f) != 1) return ERROR;
  if (fread (unknown, sizeof (word), 1, f) != 1) return ERROR;
  while (n-- > 0)
  {
   if ((key = (kfKey *)malloc (sizeof (kfKey))) == NULL) return ERROR;
   if (read_TFLAGS (f, &nf, key)) return ERROR;
   if (fread (pos, sizeof (pos), 1, f) != 1) return ERROR;
   key->Frame = nf;
   vec_make (pos[0], pos[1], pos[2], &key->Val._vector);
   vec_swap (&key->Val._vector);
   add_key (&track, key);
  }
  spline_init (track);
  Engine_SetTrack(KeyTypePos, c_id, track);
  return OK;
 }

 // -------------------------------------------------------------------------

static int read_TRACKCOLOR (FILE *f)
 {
  // Track color reader.
  kfKey *key, *track = NULL;
  float  pos[3];
  word   unknown[5];
  word   n, nf;

  if (fread (unknown, sizeof (word), 5, f) != 5) return ERROR;
  if (fread (&n, sizeof (n), 1, f) != 1) return ERROR;
  if (fread (unknown, sizeof (word), 1, f) != 1) return ERROR;
  while (n-- > 0)
  {
   if ((key = (kfKey *)malloc (sizeof (kfKey))) == NULL) return ERROR;
   if (read_TFLAGS (f, &nf, key)) return ERROR;
   if (fread (pos, sizeof (pos), 1, f) != 1) return ERROR;
   key->Frame = nf;
   vec_make (pos[0], pos[1], pos[2], &key->Val._vector);
   vec_swap (&key->Val._vector);
   add_key (&track, key);
  }
  spline_init (track);
  Engine_SetTrack (KeyTypeColor, c_id, track);
  return OK;
 }

 // -------------------------------------------------------------------------

static int read_TRACKROT (FILE *f)
 {
  // Track rotation reader.
  kfKey  *key, *track = NULL;
  sQuat  q, old;
  float   pos[4];
  word    unknown[5];
  word    n, nf;

  if (fread (unknown, sizeof (word), 5, f) != 5) return ERROR;
  if (fread (&n, sizeof (n), 1, f) != 1) return ERROR;
  if (fread (unknown, sizeof (word), 1, f) != 1) return ERROR;
  qt_identity (&old);
  while (n-- > 0)
  {
   if ((key = (kfKey *)malloc (sizeof (kfKey))) == NULL) return ERROR;
   if (read_TFLAGS (f, &nf, key)) return ERROR;
   if (fread (pos, sizeof(pos), 1, f) != 1) return ERROR;
   if (pos[0] > PI)
   {
    pos[0] /= 2.0;
    pos[1] /= 2.0;
    pos[2] /= 2.0;
    pos[3] /= 2.0;
   }
   key->Frame = nf;
   qt_fromang (pos[0], pos[1], pos[2], pos[3], &q);
   qt_swap (&q);
   qt_mul (&q, &old, &old);
   qt_copy (&old, &key->Val._quat);
   add_key (&track, key);
  }
  spline_init (track);
  Engine_SetTrack (KeyTypeRotate, c_id, track);
  return OK;
 }

 // -------------------------------------------------------------------------

static int read_TRACKSCALE (FILE *f)
 {
  // Track scale reader.
  kfKey *key, *track = NULL;
  float  pos[3];
  word   unknown[5];
  word   n, nf;

  if (fread (unknown, sizeof(word), 5, f) != 5) return ERROR;
  if (fread (&n, sizeof (n), 1, f) != 1) return ERROR;
  if (fread (unknown, sizeof (word), 1, f) != 1) return ERROR;
  while (n-- > 0)
  {
   if ((key = (kfKey *)malloc (sizeof (kfKey))) == NULL) return ERROR;
   if (read_TFLAGS (f, &nf, key)) return ERROR;
   if (fread (pos, sizeof (pos), 1, f) != 1) return ERROR;
   key->Frame = nf;
   vec_make (pos[0], pos[1], pos[2], &key->Val._vector);
   vec_swap (&key->Val._vector);
   add_key (&track, key);
  }
  spline_init (track);
  Engine_SetTrack (KeyTypeScale, c_id, track);
  return OK;
 }

 // -------------------------------------------------------------------------

static int read_TRACKFOV (FILE *f)
 {
  // Track FOV reader.
  kfKey *key, *track = NULL;
  word  unknown[5];
  word  n, nf;
  float fov;

  if (fread (unknown, sizeof (word), 5, f) != 5) return ERROR;
  if (fread (&n, sizeof (n), 1, f) != 1) return ERROR;
  if (fread (unknown, sizeof (word), 1, f) != 1) return ERROR;
  while (n-- > 0)
  {
   if ((key = (kfKey *)malloc (sizeof (kfKey))) == NULL) return ERROR;
   if (read_TFLAGS (f, &nf, key)) return ERROR;
   if (fread (&fov, sizeof (fov), 1, f) != 1) return ERROR;
   key->Frame = nf;
   key->Val._float = fov;
   add_key (&track, key);
  }
  spline_init (track);
  Engine_SetTrack (KeyTypeFov, c_id, track);
  return OK;
 }

 // -------------------------------------------------------------------------

static int read_TRACKROLL (FILE *f)
 {
  // Track ROLL reader.
  kfKey *key, *track = NULL;
  word   unknown[5];
  word   n, nf;
  float  roll;

  if (fread (unknown, sizeof (word), 5, f) != 5) return ERROR;
  if (fread (&n, sizeof (n), 1, f) != 1) return ERROR;
  if (fread (unknown, sizeof (word), 1, f) != 1) return ERROR;
  while (n-- > 0)
  {
   if ((key = (kfKey *)malloc (sizeof (kfKey))) == NULL) return ERROR;
   if (read_TFLAGS (f, &nf, key)) return ERROR;
   if (fread(&roll, sizeof(roll), 1, f) != 1) return ERROR;
   key->Frame = nf;
   key->Val._float = roll;
   add_key (&track, key);
  }
  spline_init (track);
  Engine_SetTrack (KeyTypeRoll, c_id, track);
  return OK;
 }

 // -------------------------------------------------------------------------

static int read_TRACKMORPH (FILE *f)
 {
  // Track morph reader.
  kfKey  *key, *track = NULL;
  sNode *node;
  word    unknown[5];
  word    n, nf;

  if (fread (unknown, sizeof (word), 5, f) != 5) return ERROR;
  if (fread (&n, sizeof (n), 1, f) != 1) return ERROR;
  if (fread (unknown, sizeof (word), 1, f) != 1) return ERROR;
  while (n-- > 0)
  {
   if ((key = (kfKey *)malloc (sizeof (kfKey))) == NULL) return ERROR;
   if (read_TFLAGS (f, &nf, key)) return ERROR;
   if (read_ASCIIZ (f)) return ERROR;
   node = Engine_FindByName (c_string);
   if (!node) return ERROR;
   key->Frame = nf;
   key->Val._int = ((sObject *)node->Object)->Id;
   add_key (&track, key);
  }
  Engine_SetTrack (KeyTypeMorph, c_id, track);
  return OK;
 }

 // -------------------------------------------------------------------------

static int read_TRACKHIDE (FILE *f)
 {
  // Track hide reader.
  kfKey *key, *track = NULL;
  word   unknown[5];
  word   n, nf;
  int    hide = 0;

  if (fread (unknown, sizeof (word), 5, f) != 5) return ERROR;
  if (fread (&n, sizeof (n), 1, f) != 1) return ERROR;
  if (fread (unknown, sizeof (word), 1, f) != 1) return ERROR;
  while (n-- > 0)
  {
   if ((key = (kfKey *)malloc (sizeof (kfKey))) == NULL) return ERROR;
   if (fread (&nf, sizeof (nf), 1, f) != 1) return ERROR;
   if (fread (unknown, sizeof (word), 2, f) != 2) return ERROR;
   key->Frame = nf;
   key->Val._int = hide;
   hide ^= 1;
   add_key (&track, key);
  }
  Engine_SetTrack (KeyTypeHide, c_id, track);
  return OK;
 }

// --- chunk readers control ------------------------------------------------

 // Recursive chunk reader (world).
static int ChunkReaderWorld (FILE *f, long p, word parent)
 {
  c_HEADER h;
  long     pc;
  int      n, i, error;

  c_chunk_last = parent;
  while ((pc = ftell (f)) < p)
  {
   if (fread (&h, sizeof (h), 1, f) != 1) return ERROR;
   c_chunk_curr = h.id;
   n = -1;
   for (i = 0; i < sizeof (world_chunks) / sizeof (world_chunks[0]); i++)
   {
    if (h.id == world_chunks[i].id)
    {
     n = i;
     break;
    }
   }
   if (n < 0) fseek (f, pc + h.len, SEEK_SET);
   else
   {
    pc = pc + h.len;
    // printf("Chunk: %x\n",world_chunks[n].id);

    if ((error = world_chunks[n].func (f)) != 0) return error;
    if (world_chunks[n].sub)
    {
     if ((error = ChunkReaderWorld (f, pc, h.id)) != 0) return error;
    }
    fseek (f, pc, SEEK_SET);
    c_chunk_prev = h.id;
   }
   if (ferror (f)) return ERROR;
  }
  return OK;
 }

 // -------------------------------------------------------------------------

static int ChunkReaderKey (FILE *f, long p, word parent)
 {
  // Recursive chunk reader (keyframer).
  c_HEADER h;
  long     pc;
  int      n, i, error;

  c_chunk_last = parent;
  while ((pc = ftell (f)) < p)
  {
   if (fread (&h, sizeof (h), 1, f) != 1) return ERROR;
   c_chunk_curr = h.id;
   n = -1;
   for (i = 0; i < sizeof (key_chunks) / sizeof (key_chunks[0]); i++)
    if (h.id == key_chunks[i].id)
    {
     n = i;
     break;
    }
   if (n < 0) fseek (f, pc + h.len, SEEK_SET);
   else
   {
    pc = pc + h.len;
    if ((error = key_chunks[n].func (f)) != 0) return error;
    if (key_chunks[n].sub)
     if ((error = ChunkReaderKey (f, pc, h.id)) != 0) return error;
    fseek (f, pc, SEEK_SET);
    c_chunk_prev = h.id;
   }
   if (ferror (f)) return ERROR;
  }
  return OK;
 }

// ---

 // loads mesh data from 3ds file "filename" into scene "scene".
int Engine_LoadMesh_3DS (FILE *f)
 {
  byte version;
  long length;

  c_id = 0;
  fseek (f, 0, SEEK_END);
  length = ftell (f);
  fseek (f, 28L, SEEK_SET);
  fread (&version, sizeof (byte), 1, f);
  if (version < 2) return ERROR; // 3DS 3.0+ supported
  fseek (f, 0, SEEK_SET);
  return ChunkReaderWorld (f, length, 0);
 }

 // -------------------------------------------------------------------------

 // loads motion data from 3ds file "filename" into scene "scene".
int Engine_LoadMotion_3DS(FILE *f)
 {
  byte version;
  long length;

  c_id = -1;
  fseek (f, 0, SEEK_END);
  length = ftell (f);
  fseek (f, 28L, SEEK_SET);
  fread (&version, sizeof (byte), 1, f);
  if (version < 2) return ERROR; /* 3DS 3.0+ supported */
  fseek (f, 0, SEEK_SET);
  return ChunkReaderKey (f, length, 0);
 }
