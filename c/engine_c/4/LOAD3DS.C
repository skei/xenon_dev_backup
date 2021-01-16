// mighty.engine version 0.10
// 3d studio mesh loader
// this is just a raped version of jare's 3dsrdr.c

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "engine.h"

//////////////////////////////////////////////////////////////////////////////

void ChunkReader       (FILE *f, int ind, long p);
void ASCIIZReader      (FILE *f, int ind, long p);
void ObjBlockReader    (FILE *f, int ind, long p);
void VertListReader    (FILE *f, int ind, long p);
void FaceListReader    (FILE *f, int ind, long p);
void FaceMatReader     (FILE *f, int ind, long p);
void MapListReader     (FILE *f, int ind, long p);
void TrMatrixReader    (FILE *f, int ind, long p);
void LightReader       (FILE *f, int ind, long p);
void SpotLightReader   (FILE *f, int ind, long p);
void CameraReader      (FILE *f, int ind, long p);
void FramesReader      (FILE *f, int ind, long p);
void PivotPointReader  (FILE *f, int ind, long p);
void TrackPosReader    (FILE *f, int ind, long p);
void TrackRotReader    (FILE *f, int ind, long p);
void TrackScaleReader  (FILE *f, int ind, long p);
void TrackFovReader    (FILE *f, int ind, long p);
void TrackRollReader   (FILE *f, int ind, long p);
void TrackMorphReader  (FILE *f, int ind, long p);
void TrackHideReader   (FILE *f, int ind, long p);
void ObjNumberReader   (FILE *f, int ind, long p);
void SkipReader        (FILE *f, int ind, long p) {}

static int key_chunk;
static int key_obj;
static int key_light;

struct {
  word id;
  void (*func)(FILE *f, int ind, long p);
} ChunkNames[] = {
  {CHUNK_RGBF,         NULL},
  {CHUNK_RGBB,         NULL},
  {CHUNK_PRJ,          NULL},
  {CHUNK_MLI,          NULL},
  {CHUNK_MAIN,         NULL},
  {CHUNK_OBJMESH,      NULL},
  {CHUNK_BKGCOLOR,     NULL},
  {CHUNK_AMBCOLOR,     NULL},
  {CHUNK_OBJBLOCK,     ObjBlockReader},
  {CHUNK_TRIMESH,      NULL},
  {CHUNK_VERTLIST,     VertListReader},
  {CHUNK_VERTFLAGS,    SkipReader},
  {CHUNK_FACELIST,     FaceListReader},
  {CHUNK_MESHCOLOR,    SkipReader},
  {CHUNK_FACEMAT,      FaceMatReader},
  {CHUNK_MAPLIST,      MapListReader},
  {CHUNK_TXTINFO,      SkipReader},
  {CHUNK_SMOOLIST,     NULL},
  {CHUNK_TRMATRIX,     TrMatrixReader},
  {CHUNK_LIGHT,        LightReader},
  {CHUNK_SPOTLIGHT,    SpotLightReader},
  {CHUNK_CAMERA,       CameraReader},
  {CHUNK_HIERARCHY,    NULL},
  {CHUNK_VIEWPORT,     SkipReader},
  {CHUNK_MATERIAL,     NULL},
  {CHUNK_MATNAME,      NULL},
  {CHUNK_AMBIENT,      NULL},
  {CHUNK_DIFFUSE,      NULL},
  {CHUNK_SPECULAR,     NULL},
  {CHUNK_TEXTURE,      NULL},
  {CHUNK_BUMPMAP,      NULL},
  {CHUNK_MAPFILE,      NULL},
  {CHUNK_KEYFRAMER,    NULL},
  {CHUNK_AMBIENTKEY,   NULL},
  {CHUNK_TRACKINFO,    NULL},
  {CHUNK_FRAMES,       FramesReader},
  {CHUNK_TRACKOBJNAME, NULL},
  {CHUNK_TRACKPIVOT,   PivotPointReader},
  {CHUNK_TRACKPOS,     TrackPosReader},
  {CHUNK_TRACKROTATE,  TrackRotReader},
  {CHUNK_TRACKSCALE,   TrackScaleReader},
  {CHUNK_TRACKMORPH,   TrackMorphReader},
  {CHUNK_TRACKHIDE,    TrackHideReader},
  {CHUNK_OBJNUMBER,    ObjNumberReader},
  {CHUNK_TRACKCAMERA,  NULL},
  {CHUNK_TRACKCAMTGT,  NULL},
  {CHUNK_TRACKLIGHT,   NULL},
  {CHUNK_TRACKLIGTGT,  NULL},
  {CHUNK_TRACKSPOTL,   NULL},
  {CHUNK_TRACKFOV,     TrackFovReader},
  {CHUNK_TRACKROLL,    TrackRollReader}};

void ASCIIZReader (FILE *f, int ind, long p)
{
  int c;
  while ((c = fgetc (f)) != EOF && c != '\0');
}

void ObjBlockReader (FILE *f, int ind, long p)
{
  int c;
  ASCIIZReader (f, ind, p);
  ChunkReader (f, ind, p);
}

void VertListReader (FILE *f, int ind, long p)
{
  word  nv;
  float c[3];

  scene->AddObject ();
  if (fread (&nv, sizeof (nv), 1, f) != 1) return;
  while (nv-- > 0)
  {
    if (fread (&c, sizeof (c), 1, f) != 1) return;
    scene->object[scene->numObjects-1]->AddPoint (c[0], c[2], c[1]);
  }
}

void FaceListReader (FILE *f, int ind, long p)
{
  word nv;
  word c[3];
  word flags;

  if (fread (&nv, sizeof (nv), 1, f) != 1) return;
  while (nv-- > 0)
  {
    if (fread (&c, sizeof (c), 1, f) != 1) return;
    if (fread (&flags, sizeof (flags), 1, f) != 1) return;
    scene->object[scene->numObjects-1]->AddPoly (c[0], c[1], c[2]);
  }
  ChunkReader (f, ind, p);
}

void FaceMatReader (FILE *f, int ind, long p)
{
  int  c;
  word n, nf;

  ASCIIZReader (f, ind, p);
  if (fread (&n, sizeof (n), 1, f) != 1) return;
  while (n-- > 0)
  {
    if (fread (&nf, sizeof(nf), 1, f) != 1) return;
    // nf = face #
    // material = asciiz
  }
}

void MapListReader (FILE *f, int ind, long p)
{
  word  nv;
  float c[2];
  int   pn=0;

  if (fread (&nv, sizeof (nv), 1, f) != 1) return;
  while (nv-- > 0)
  {
    if (fread (&c, sizeof (c), 1, f) != 1) return;
    scene->object[scene->numObjects-1]->point[pn]->SetTexCoords (c[0], c[1]);
    pn++;
  }
}

void TrMatrixReader (FILE *f, int ind, long p)
{
  float  rot[9];
  Vector trans;
  float  temp;

  if (fread (&rot, sizeof (rot), 1, f) != 1) return;
  if (fread (&trans, sizeof (trans), 1, f) != 1) return;
  temp = trans.y; trans.y = trans.z; trans.z = temp;
  scene->object [scene->numObjects-1]->SetFlipMatrix (rot);
  scene->object [scene->numObjects-1]->SetPivot (trans);
  // rot = 3x3 rotation matrix
  // trans = translation matrix
}

void LightReader (FILE *f, int ind, long p)
{
  float c[3];

  if (fread (&c, sizeof (c), 1, f) != 1) return;
  scene->AddLight (c[0], c[2], c[1]);
  ChunkReader (f, ind, p);
}

void SpotLightReader (FILE *f, int ind, long p)
{
  float c[5];

  if (fread (&c, sizeof (c), 1, f) != 1) return;
  scene->AddLight (c[0], c[2], c[1]);
}

void CameraReader (FILE *f, int ind, long p)
{
  float c[8];

  if (fread (&c, sizeof (c), 1, f) != 1) return;
  scene->AddCamera (c[0], c[2], c[1], c[3], c[5], c[4]);
}

void FramesReader (FILE *f, int ind, long p)
{
  dword c[2];

  if (fread (&c, sizeof (c), 1, f) != 1) return;
  scene->keyframer = new Keyframer (c[1]-c[0]+1);
  // c[0]=start frame, c[1]=end frame
}

void PivotPointReader (FILE *f, int ind, long p)
{
  float pos[3];

  if (fread (&pos, sizeof (pos), 1, f) != 1) return;
  scene->keyframer->object[key_obj-1]->pivot.x = pos[0];
  scene->keyframer->object[key_obj-1]->pivot.y = pos[2];
  scene->keyframer->object[key_obj-1]->pivot.z = pos[1];
  // pivot point = pos[0], pos[2], pos[1]
}

void SplineFlagsReader (FILE *f, int ind, word flags)
{
  int   i;
  float dat;

  for (i = 0; i < 16; i++)
    if (flags & (1 << i)) {
      if (fread (&dat, sizeof (dat), 1, f) != 1) return;
      switch (i) { // dat = value
        case 0:    // tension
        case 1:    // continuity
        case 2:    // bias
        case 3:    // ease to
        case 4: ;  // ease from
      }
    }
}

void TrackPosReader (FILE *f, int ind, long p)
{
  word   n, nf, ns;
  float  pos[3];
  word   unkown;
  word   flags;
  Vector *bptr;
  int    i;

  for (i=0; i<5; i++) if (fread (&unkown, sizeof (unkown), 1, f) != 1) return;
  if (fread (&n, sizeof (n), 1, f) != 1) return;
  ns = n;
  if (fread (&unkown, sizeof (unkown), 1, f) != 1) return;
  InitKeys ();
  while (n-- > 0)
  {
    if (fread (&nf, sizeof (nf), 1, f) != 1) return;
    if (fread (&unkown, sizeof (unkown), 1, f) != 1) return;
    if (fread (&flags, sizeof (flags), 1, f) != 1) return;
    SplineFlagsReader (f, ind, flags);
    if (fread (&pos, sizeof (pos), 1, f) != 1) return;
    if (key_chunk) AddKey (float (nf), pos[0], pos[2], pos[1], 0,0,0,0,0,0);
    // frame number = nf
    // position = pos[0], pos[2], pos[1]
  }
  if (nf < scene->keyframer->numFrames)
    AddKey (float (scene->keyframer->numFrames), pos[0], pos[2], pos[1], 0,
            0,0,0,0,0);
  switch (key_chunk) {
    case CHUNK_TRACKINFO: bptr = scene->keyframer->object[key_obj-1]->position; break;
    case CHUNK_TRACKLIGHT: bptr = scene->keyframer->light[key_light-1]->position; break;
    case CHUNK_TRACKCAMERA: bptr = scene->keyframer->camera->source; break;
    case CHUNK_TRACKCAMTGT: bptr = scene->keyframer->camera->dest; break;
  }
  if ((key_chunk)&&(bptr)) DrawSpline3d (bptr);
}

void TrackRotReader (FILE *f, int ind, long p)
{
  word  n, nf, ns;
  float pos[4];
  word  unkown;
  word  flags;
  quat  *bptr;
  quat  q, t, old;
  int   i, first;

  for (i = 0; i < 5; i++) if (fread (&unkown, sizeof (unkown), 1, f) != 1) return;
  if (fread (&n, sizeof (n), 1, f) != 1) return;      // numkeys
  if (fread (&unkown, sizeof (unkown), 1, f) != 1) return;
  ns = n;
  first = 0;
  InitKeys ();
  while (n-- > 0)
  {
   if (fread (&nf, sizeof (nf), 1, f) != 1) return;         // frame #
   if (fread (&unkown, sizeof (unkown), 1, f) != 1) return;
   if (fread (&flags, sizeof (flags), 1, f) != 1) return;     // \ Spline
   SplineFlagsReader (f, ind, flags);                         // /
   if (fread (&pos, sizeof (pos), 1, f) != 1) return;   // pos = ang/axis
   qt_convert (pos[0], pos[1], pos[3], pos[2], &q);
   if (first)
   {
    qt_mul (q, old, &t);
    q = t;
   }
   old = q;
   if (key_chunk) AddKey (float (nf), q.x, q.y, q.z, q.w, 0,0,0,0,0);
   first = 1;
   // frame number = nf
   // quaternion = pos[0], pos[1], pos[3], pos[2]
  }
  if (nf < scene->keyframer->numFrames) AddKey (float (scene->keyframer->numFrames), q.x, q.y, q.z, q.w,0,0,0,0,0);
  bptr = scene->keyframer->object[key_obj-1]->rotate;
  if ((key_chunk==CHUNK_TRACKINFO)&&(bptr)) DrawSpline4d (bptr);
 }

void TrackScaleReader (FILE *f, int ind, long p)
{
  word   n, nf;
  float  pos[3];
  word   unkown;
  word   flags;
  Vector *bptr;
  int    i;

  for (i=0; i<5; i++) if (fread (&unkown, sizeof (unkown), 1, f) != 1) return;
  if (fread (&n, sizeof (n), 1, f) != 1) return;
  if (fread (&unkown, sizeof (unkown), 1, f) != 1) return;
  InitKeys ();
  while (n-- > 0)
  {
    if (fread (&nf, sizeof (nf), 1, f) != 1) return;
    if (fread (&unkown, sizeof (unkown), 1, f) != 1) return;
    if (fread (&flags, sizeof (flags), 1, f) != 1) return;
    SplineFlagsReader (f, ind, flags);
    if (fread (&pos, sizeof (pos), 1, f) != 1) return;
    if (key_chunk) AddKey (float (nf), pos[0], pos[2], pos[1], 0,0,0,0,0,0);
    // frame number = nf
    // scaling = pos[0], pos[2], pos[1]
  }
  if (nf < scene->keyframer->numFrames)
    AddKey (float (scene->keyframer->numFrames), pos[0], pos[2], pos[1], 0,
            0,0,0,0,0);
  bptr = scene->keyframer->object[key_obj-1]->scale;
  if ((key_chunk==CHUNK_TRACKINFO)&&(bptr)) DrawSpline3d (bptr);
}

void TrackFovReader (FILE *f, int ind, long p)
{
  word  n, nf;
  float fov;
  word  unkown;
  word  flags;
  int   i;

  for (i=0; i<5; i++) if (fread (&unkown, sizeof (unkown), 1, f) != 1) return;
  if (fread (&n, sizeof (n), 1, f) != 1) return;
  if (fread (&unkown, sizeof (unkown), 1, f) != 1) return;
  while (n-- > 0)
  {
    if (fread (&nf, sizeof (nf), 1, f) != 1) return;
    if (fread (&unkown, sizeof (unkown), 1, f) != 1) return;
    if (fread (&flags, sizeof (flags), 1, f) != 1) return;
    SplineFlagsReader (f, ind, flags);
    if (fread (&fov, sizeof (fov), 1, f) != 1) return;
    // frame number = nf
    // fov = fov
  }
}

void TrackRollReader (FILE *f, int ind, long p)
{
  word   n, nf;
  float  roll;
  word   unkown;
  word   flags;
  Vector *bptr;
  int    i;

  for (i=0; i<5; i++) if (fread (&unkown, sizeof (unkown), 1, f) != 1) return;
  if (fread (&n, sizeof (n), 1, f) != 1) return;
  if (fread (&unkown, sizeof (unkown), 1, f) != 1) return;
  while (n-- > 0)
  {
    if (fread (&nf, sizeof (nf), 1, f) != 1) return;
    if (fread (&unkown, sizeof (unkown), 1, f) != 1) return;
    if (fread (&flags, sizeof (flags), 1, f) != 1) return;
    SplineFlagsReader (f, ind, flags);
    if (fread (&roll, sizeof (roll), 1, f) != 1) return;
    // frame number = nf
    // roll = roll
  }
}

void TrackMorphReader (FILE *f, int ind, long p)
{
  word n, nf;
  word unkown;
  word flags;
  int  i;

  for (i=0; i<5; i++) if (fread (&unkown, sizeof (unkown), 1, f) != 1) return;
  if (fread (&n, sizeof (n), 1, f) != 1) return;
  if (fread (&unkown, sizeof (unkown), 1, f) != 1) return;
  while (n-- > 0)
  {
    if (fread (&nf, sizeof (nf), 1, f) != 1) return;
    if (fread (&unkown, sizeof (unkown), 1, f) != 1) return;
    if (fread (&flags, sizeof (flags), 1, f) != 1) return;
    SplineFlagsReader (f, ind, flags);
    ASCIIZReader (f, ind, p);
    // frame number = nf
    // morph object = asciiz
  }
}

void TrackHideReader (FILE *f, int ind, long p)
{
  word n, nf;
  word frame;
  word unkown;
  word flags;
  int i;

  for (i=0; i<5; i++) if (fread (&unkown, sizeof (unkown), 1, f) != 1) return;
  if (fread (&n, sizeof (n), 1, f) != 1) return;
  if (fread (&unkown, sizeof (unkown), 1, f) != 1) return;
  while (n-- > 0)
  {
    if (fread(&frame, sizeof(frame), 1, f) != 1) return;
    for (i=0; i<2; i++) if (fread (&unkown, sizeof (unkown), 1, f) != 1) return;
  }
}

void ObjNumberReader (FILE *f, int ind, long p)
{
  word n;
  if (fread (&n, sizeof (n), 1, f) != 1) return;
  // object number = n
}

int FindChunk (word id)
{
  int i;

  for (i = 0; i < sizeof (ChunkNames)/sizeof (ChunkNames[0]); i++)
    if (id == ChunkNames[i].id) return i;
  return -1;
}

void ChunkReader (FILE *f, int ind, long p)
{
  TChunkHeader h;
  int          n, i;
  byte         d;
  long         pc;

  while (ftell(f) < p) {
    pc = ftell (f);
    if (fread (&h, sizeof (h), 1, f) != 1) return;
    if (h.len == 0) return;
    n = FindChunk (h.id);
    switch (h.id) {
      case CHUNK_TRACKINFO:   key_obj++; key_chunk=h.id; break;
      case CHUNK_TRACKLIGHT:  key_light++; key_chunk=h.id; break;
      case CHUNK_TRACKCAMERA:
      case CHUNK_TRACKCAMTGT: key_chunk=h.id; break;
      case CHUNK_AMBIENTKEY:
      case CHUNK_TRACKLIGTGT:
      case CHUNK_TRACKSPOTL:  key_chunk=0; break;
    }
    if (n < 0) fseek (f, pc + h.len, SEEK_SET);
      else {
        pc = pc + h.len;
        if (ChunkNames[n].func != NULL) ChunkNames[n].func (f, ind + 2, pc);
          else ChunkReader(f, ind + 2, pc);
        fseek (f, pc, SEEK_SET);
      }
    if (ferror (f)) break;
  }
}

int LoadScene (char *filename)
{
  FILE *f;
  long p;
  int  i;

  key_obj = key_light = key_chunk = 0;
  f = fopen (filename, "rb");
  if (f == NULL) return 0;
  fseek (f, 0, SEEK_END);
  p = ftell (f);
  fseek (f, 0, SEEK_SET);
  ChunkReader (f, 0, p);
  for (i=0; i < scene->numObjects; i++)
    scene->object[i]->CalcVertNormals ();
  return 1;
}

