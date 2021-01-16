#include <stdio.h>
#include <math.h>
#include "engine.h"

//
// ±±±±± Global variables ±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
//

kfScene *Engine_Scene;     // The current active scene
int      Engine_Flags;

//
// ±±±±± Internal support functions ±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
//

 // frees keys list.
void     Engine_FreeKey(kfKey *key)
 {
  kfKey *next;

  for (;key;key=next)
  {
   next = key->Next;
   free (key);
  }
 }

 // -------------------------------------------------------------------------

 // calculates face/vertex normals.
void     Engine_CalcNormals (void)
 {
  sObject *obj;
  sNode *node;
  sVector a,b,normal;
  sVertex *vi;
  int i,j,num;

  for (node = Engine_Scene->World; node; node = node->Next)
  {
   if (node->Type == NodeTypeObject)
   {
    obj = (sObject *)node->Object;

    // face normals

    for (i = 0; i < obj->NumPolygons; i++)
    {
     vec_sub (&(obj->Polygons[i].v[0]->Pos),
              &(obj->Polygons[i].v[1]->Pos), &a);
     vec_sub (&(obj->Polygons[i].v[0]->Pos),
              &(obj->Polygons[i].v[2]->Pos), &b);
     vec_cross (&a, &b, &normal);
     vec_normalize (&normal, &(obj->Polygons[i].Normal));
    }

    // vertex normals

    for (i = 0; i < obj->NumVertices; i++)
    {
     vi = &(obj->Vertices[i]);
     num = 0;
     vec_zero (&normal);
     for (j = 0; j < obj->NumPolygons; j++)
     {
      if (   (obj->Polygons[j].v[0] == vi)
          || (obj->Polygons[j].v[1] == vi)
          || (obj->Polygons[j].v[2] == vi))
      {
       vec_add (&normal, &(obj->Polygons[j].Normal), &normal);
       num++;
      }
     }
     if (num > 0) vec_scale (&normal, 1.0 / (float)num, &normal);
     vec_normalize (&normal, &(vi->Normal));
    }

   }
  }
 }

 // -------------------------------------------------------------------------

 // transform all objects.
void     Engine_DoTransform (void)
 {
  sNode *node;
  sObject *obj;
  sVector  vec;
  int i, j;

  for (node = Engine_Scene->World; node; node = node->Next)
  {
   if (node->Type == NodeTypeObject)
   {
    obj = (sObject *)node->Object;
    // vec_sub (&obj->Pos, &obj->Pivot, &vec);      // ???
    // mat_mulvec (obj->Matrix, &vec, &obj->rPos);
    // check if bouning sphere within view frustum.. (distance to planes)
    for (i = 0; i < obj->NumVertices; i++)
    {
     vec_sub (&obj->Vertices[i].Pos, &obj->Pivot, &vec);
     mat_mulvec (obj->Matrix, &vec, &obj->Vertices[i].rPos);

     // mat_mulvec (obj->Matrix, &obj->Vertices[i].Normal,
     //                          &obj->Vertices[i].rNormal);
    }
   }
  }
 }

 // -------------------------------------------------------------------------

 // non-case sensitive string compare.
int      strucmp (char *s1, char *s2)
 {
  int diff = 0;

  do
  {
   diff += (toupper(*s1) - toupper(*s2));
  } while (*s1++ && *s2++);
  return diff;
 }

//
// ±±±±± Engine / API ±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
//

void     Engine_Init(int flags)
 {
  Engine_Scene = NULL;
  Engine_Flags = flags;
 }

 // -------------------------------------------------------------------------

void     Engine_Done(void)
 {
  Engine_Scene = NULL;
  Engine_Flags = 0;
 }

 // -------------------------------------------------------------------------

 // return number of frames.
void     Engine_GetFrames (float *start, float *end)
 {
  *start = Engine_Scene->Start;
  *end = Engine_Scene->End;
 }

 // -------------------------------------------------------------------------

 // set current frame number.
void     Engine_SetFrame (float frame)
 {
  if (Engine_Scene->Start <= frame && Engine_Scene->End >= frame) Engine_Scene->Current = frame;
 }

 // -------------------------------------------------------------------------

 // return current frame number.
float    Engine_GetFrame (void)
 {
  return Engine_Scene->Current;
 }

 // -------------------------------------------------------------------------

 // sets active scene.
void     Engine_SetActive (kfScene *scene)
 {
  Engine_Scene = scene;
 }

 // -------------------------------------------------------------------------

 // get active scene.
kfScene *Engine_GetActiv(void)
 {
  return Engine_Scene;
 }

 // -------------------------------------------------------------------------

 // find object by name.
sNode   *Engine_FindByName(char *name)
 {
  sNode *node;

  for (node = Engine_Scene->World; node; node = node->Next)
  {
   switch ((node)->Type)
   {
    case NodeTypeCamera :
        if (strcmp (((sCamera *)(node->Object))->Name, name) == 0)
        return node;
        break;
    case NodeTypeObject :
        if (strcmp (((sObject *)(node->Object))->Name, name) == 0)
        return node;
        break;
    case NodeTypeLight :
        if (strcmp (((sLight *)(node->Object))->Name, name) == 0)
        return node;
        break;
    case NodeTypeMaterial :
        if (strcmp (((sMaterial *)(node->Object))->Name, name) == 0)
        return node;
        break;
    case NodeTypeAmbient :
        if (strcmp (((sAmbient *)(node->Object))->Name, name) == 0)
        return node;
        break;
   }
  }
  return NULL;
 }

 // -------------------------------------------------------------------------

 // find object by id.
sNode   *Engine_FindById(int id)
 {
  sNode *node;

  for (node = Engine_Scene->World; node; node = node->Next)
  {
   switch (node->Type)
   {
    case NodeTypeCamera :
        if (((sCamera *)(node->Object))->Id == id)
        return node;
        break;
    case NodeTypeObject :
        if (((sObject *)(node->Object))->Id == id)
        return node;
        break;
    case NodeTypeLight :
        if (((sLight *)(node->Object))->Id == id)
        return node;
        break;
    case NodeTypeMaterial :
        if (((sMaterial *)(node->Object))->Id == id)
        return node;
        break;
    case NodeTypeAmbient :
        if (((sAmbient *)(node->Object))->Id == id)
        return node;
        break;
   }
  }
  return NULL;
 }

 // -------------------------------------------------------------------------

 // finds first node with attribute "attr".
sNode   *Engine_FindFirst (int attr)
 {
  sNode *node;

  node = Engine_Scene->World;
  while (node != NULL)
  {
   if (node->Type & attr) return node;
  }
  return NULL;
 }

 // -------------------------------------------------------------------------

 // finds next node with attribute "attr".
sNode   *Engine_FindNext (int attr, sNode *node)
 {
  while (node != NULL)
  {
   if (node->Type & attr) return node;
  }
  return NULL;
 }

 // -------------------------------------------------------------------------

 // add object to world list.
int      Engine_AddWorld(int type, void *obj)
 {
  sNode *node;

  node = (sNode *)malloc (sizeof(sNode));
  if (node == NULL) return ERROR;
  node->Type = type;
  node->Object = obj;
  node->Next = NULL;
  if (Engine_Scene->World == NULL)
  {
   node->Prev = NULL;
   Engine_Scene->World = node;
   Engine_Scene->Wtail = node;
  }
  else
  {
   node->Prev = Engine_Scene->Wtail;
   Engine_Scene->Wtail->Next = node;
   Engine_Scene->Wtail = node;
  }
  return OK;
 }

 // -------------------------------------------------------------------------

 // add track to keyframer list.
int      Engine_AddTrack (int type, int id, int parent, void *track, void *obj)
 {
  kfNode *node, *pnode;

  node = (kfNode *)malloc (sizeof(kfNode));
  if (node == NULL) return ERROR;
  node->Type = type;
  node->Id = id;
  node->Track = track;
  node->Brother = NULL;
  node->Child = NULL;
  node->Next = NULL;
  node->Object = obj;
  if (Engine_Scene->Keyframer == NULL)
  {
   node->Prev = NULL;
   Engine_Scene->Keyframer = node;
   Engine_Scene->Ktail = node;
  }
  else
  {
   node->Prev = Engine_Scene->Ktail;
   Engine_Scene->Ktail->Next = node;
   Engine_Scene->Ktail = node;
  }
  if (parent != -1)
  {
   for (pnode = Engine_Scene->Keyframer; pnode; pnode = pnode->Next)
   {
    if (pnode->Id == parent)
    {
     if (pnode->Child == NULL) pnode->Child = node;
     else
     {
      node->Brother = pnode->Child;
      pnode->Child = node;
     }
    }
   }
  }
  return OK;
 }

 // -------------------------------------------------------------------------

 // assigns a key list to track.
int      Engine_SetTrack (int type, int id, kfKey *track)
 {
  int     err;
  void   *obj;
  kfNode *node;

  node = Engine_Scene->Keyframer;
  while (node && node->Id != id) node = node->Next;
  if (!node) return ERROR;
  obj = node->Track;

  switch (node->Type)
  {
   case TrackTypeCamera:
      switch (type)
      {
       case KeyTypePos:    ((sCAMERA *)obj)->pos = track; break;
       case KeyTypeFov:    ((sCAMERA *)obj)->fov = track; break;
       case KeyTypeRoll:   ((sCAMERA *)obj)->roll = track;
      }
      break;
    case TrackTypeCameratgt:
      switch (type)
      {
       case KeyTypePos:    ((sCAMERATGT *)obj)->pos = track;
      }
      break;
    case TrackTypeLight:
      switch (type)
      {
       case KeyTypePos:    ((sLIGHT *)obj)->pos = track; break;
       case KeyTypeColor:  ((sLIGHT *)obj)->color = track;
      }
      break;
    case TrackTypeLighttgt:
      switch (type)
      {
       case KeyTypePos:    ((sLIGHTTGT *)obj)->pos = track; break;
      }
    case TrackTypeSpotlight:
      switch (type)
      {
       case KeyTypePos:    ((sSPOTLIGHT *)obj)->pos = track; break;
       case KeyTypeColor:  ((sSPOTLIGHT *)obj)->color = track; break;
       case KeyTypeRoll:   ((sSPOTLIGHT *)obj)->roll = track;
      }
      break;
    case TrackTypeObject:
      switch (type)
      {
       case KeyTypePos:    ((sOBJECT *)obj)->translate = track; break;
       case KeyTypeScale:  ((sOBJECT *)obj)->scale = track; break;
       case KeyTypeRotate: ((sOBJECT *)obj)->rotate = track; break;
       case KeyTypeMorph:  ((sOBJECT *)obj)->morph = track; break;
       case KeyTypeHide:   ((sOBJECT *)obj)->hide = track;
      }
      break;
    case TrackTypeAmbient:
      switch (type)
      {
       case KeyTypeColor:  ((sAMBIENT *)obj)->color = track;
      }
  }
  return OK;
 }

 // -------------------------------------------------------------------------

 // allocates a new scene.
kfScene *Engine_AllocScene(int width, int height, int bpp, int maxpolys)
 {
  kfScene *scene;

  scene = (kfScene *)malloc(sizeof(kfScene));
  if (scene == NULL) return NULL;
  scene->World = NULL;
  scene->Wtail = NULL;
  scene->Keyframer = NULL;
  scene->Ktail = NULL;

  scene->RenderList = (sPolygon **)malloc(maxpolys*sizeof(sPolygon **));
  scene->Sorted = (sPolygon **)malloc(maxpolys*sizeof(sPolygon **));

  scene->Buffer = (char *)malloc(width*height);
  scene->Screen = (char *)0xA0000;

  scene->ScreenWidth = width;
  scene->ScreenHeight = height;
  scene->TopClip   = 0;
  scene->BotClip   = height-1;
  scene->LeftClip  = 0;
  scene->RightClip = width-1;

  return scene;
 }

 // -------------------------------------------------------------------------

 // loads mesh data from 3ds file "filename" into scene "scene".
int      Engine_LoadWorld(char *filename, kfScene *scene)
 {
  kfScene *old_scene;
  FILE *f;
  char *s;
  int i, error;

  old_scene = Engine_Scene;
  Engine_Scene->Start = 0;
  Engine_Scene->End = 0;
  Engine_Scene->Current = 0;
  Engine_Scene->World = NULL;
  Engine_Scene->Wtail = NULL;
  Engine_Scene->Keyframer = NULL;
  Engine_Scene->Ktail = NULL;
  if ((f = fopen (filename, "rb")) == NULL) return ERROR;
  Engine_SetActive(scene);
  error = Engine_LoadMesh_3DS(f); // Load 3DS here... return OK or ERROR
  Engine_SetActive(old_scene);
  fclose (f);
  if (error)
  {
   Engine_FreeWorld(scene);
   return ERROR;
  }
  if (Engine_Flags & FlagCalcnorm) Engine_CalcNormals();
  return OK;
 }

 // -------------------------------------------------------------------------

 // loads motion data from 3ds file "filename" into scene "scene".
int      Engine_LoadMotion(char *filename, kfScene *scene)
 {
  kfScene *old_scene;
  sNode *node;
  sObject *obj;
  FILE *f;
  char *s;
  int i, error;

  old_scene = Engine_Scene;
  if ((f = fopen (filename, "rb")) == NULL) return ERROR;
  Engine_SetActive(scene);
  error = Engine_LoadMotion_3DS(f); // Load 3ds animation here... return OK or ERROR
  fclose (f);
  Engine_SetActive(old_scene);
  if (error)
  {
   Engine_FreeMotion(scene);
   return ERROR;
  }
  if (Engine_Flags & FlagTransform)
  {
   Engine_DoTransform();
  }
  return OK;
 }

 // -------------------------------------------------------------------------

 // release all memory used by world.
int      Engine_FreeWorld(kfScene *scene)
 {
  sNode *node, *next;
  sLight *light;
  sCamera *cam;
  sObject *obj;
  sMaterial *mat;
  sAmbient *amb;

  if (!scene) return ERROR;
  for (node = scene->World; node; node = next)
  {
   switch (node->Type)
   {
    case NodeTypeLight:
        light = (sLight *)node->Object;
        free (light->Name);
        break;
    case NodeTypeCamera:
        cam = (sCamera *)node->Object;
        free (cam->Name);
        break;
    case NodeTypeObject:
        obj = (sObject *)node->Object;
        free (obj->Name);
        free (obj->Vertices);
        free (obj->Polygons);
        break;
    case NodeTypeMaterial:
        mat = (sMaterial *)node->Object;
        free (mat->Name);
        free (mat->Texture.File);
        free (mat->Bump.File);
        free (mat->Reflection.File);
        break;
    case NodeTypeAmbient:
        amb = (sAmbient *)node->Object;
        free (amb->Name);
   }
   next = node->Next;
   free (node->Object);
   free (node);
  }
  Engine_Scene->World = NULL;
  Engine_Scene->Wtail = NULL;
  return OK;
 }

 // -------------------------------------------------------------------------

 // release all memory used by keyframer.
int      Engine_FreeMotion(kfScene *scene)
 {
  kfNode *node,*next;
  sCAMERA *cam;
  sCAMERATGT *camtgt;
  sLIGHT *light;
  sLIGHTTGT *litgt;
  sSPOTLIGHT *spotl;
  sOBJECT *obj;
  sAMBIENT *amb;

  if (!scene) return ERROR;
  for (node = scene->Keyframer; node; node = next)
  {
   switch (node->Type)
   {
    case TrackTypeCamera:
        cam = (sCAMERA *)node->Track;
        Engine_FreeKey(cam->pos);
        Engine_FreeKey(cam->fov);
        Engine_FreeKey(cam->roll);
        break;
    case TrackTypeCameratgt:
        camtgt = (sCAMERATGT *)node->Track;
        Engine_FreeKey(camtgt->pos);
        break;
    case TrackTypeLight:
        light = (sLIGHT *)node->Track;
        Engine_FreeKey(light->pos);
        Engine_FreeKey(light->color);
        break;
    case TrackTypeSpotlight:
        spotl = (sSPOTLIGHT *)node->Track;
        Engine_FreeKey(spotl->pos);
        Engine_FreeKey(spotl->color);
        Engine_FreeKey(spotl->roll);
        break;
    case TrackTypeLighttgt:
        litgt = (sLIGHTTGT *)node->Track;
        Engine_FreeKey(litgt->pos);
        break;
    case TrackTypeObject:
        obj = (sOBJECT *)node->Track;
        Engine_FreeKey(obj->translate);
        Engine_FreeKey(obj->scale);
        Engine_FreeKey(obj->rotate);
        Engine_FreeKey(obj->morph);
        Engine_FreeKey(obj->hide);
        break;
    case TrackTypeAmbient:
        amb = (sAMBIENT *)node->Track;
        Engine_FreeKey(amb->color);
    }
    next = node->Next;
    free (node->Track);
    free (node);
  }
  Engine_Scene->Keyframer = NULL;
  Engine_Scene->Ktail = NULL;
  return OK;
 }

 // -------------------------------------------------------------------------

 // loads mesh and keyframer data from 3ds file "filename" into scene "scene".
int      Engine_LoadScene(char *filename, kfScene *scene)
 {
  int error;

  error = Engine_LoadWorld(filename, scene);
  if (error != OK) return ERROR;
  error = Engine_LoadMotion(filename, scene);
  if (error != OK) return ERROR;
  return OK;
 }

 // -------------------------------------------------------------------------

int      Engine_FreeScene(kfScene *scene)
 {
  int error;

  error = Engine_FreeWorld(scene);
  if (error != OK) return ERROR;
  error = Engine_FreeMotion(scene);
  if (error != OK) return ERROR;
  if (scene->RenderList != NULL) free(scene->RenderList);
  if (scene->Sorted != NULL) free(scene->Sorted);
  if (scene->Buffer != NULL) free (scene->Buffer);
  return OK;
 }

 // -------------------------------------------------------------------------

 // return float key at frame "frame".
int      Engine_GetKey_Float(kfKey *keys, float frame, float *out)
 {
  float alpha;

  if (frame < 0.0) return ERROR;
  if (!keys) return ERROR;
  if (!keys->Next || frame < keys->Frame)
  {
   // only one key, return first key
   *out = keys->Val._float;
   return OK;
  }
  // more than one key, spline interpolation
  return Spline_GetKey_Float(keys, frame, out);
 }

 // -------------------------------------------------------------------------

 // return vector key at frame "frame".
int      Engine_GetKey_Vect(kfKey *keys, float frame, sVector *out)
 {
  float alpha;

  if (frame < 0.0) return ERROR;
  if (!keys) return ERROR;
  if (!keys->Next || frame < keys->Frame)
  {
    // only one key, return first key
    vec_copy (&keys->Val._vector, out);
    return OK;
  }
  // more than one key, spline interpolation
  return Spline_GetKey_Vect(keys, frame, out);
 }

 // -------------------------------------------------------------------------

 // return rgb key at frame "frame".
int      Engine_GetKey_RGB(kfKey *keys, float frame, sRGB *out)
 {
  sVector *vect = (sVector *)out;
  return Spline_GetKey_Vect (keys, frame, vect);    // or Engine...
 }

 // -------------------------------------------------------------------------

 // return quaternion key at frame "frame".
int      Engine_GetKey_Quat (kfKey *keys, float frame, sQuat *out)
 {
  float alpha;

  if (frame < 0.0) return ERROR;
  if (!keys) return ERROR;
  #ifdef CLAX_QUAT_SLERP
   while (frame > keys->Next->Frame && keys->Next) keys = keys->Next;
   if (!keys->next || frame < keys->Frame)
   {
    // frame is above last key
    qt_copy (&Neys->Val._quat, out);
    return OK;
   }
   alpha = (frame - keys->Frame) / (keys->Next->Frame - keys->Frame);
   qt_slerp (&keys->Val._quat, &keys->Next->Val._quat, alpha, out);
   return OK;
  #else
   if (!keys->Next || frame < keys->Frame)
   {
    qt_copy (&keys->Val._quat, out);
    return OK;
   }
   return Spline_GetKey_Quat(keys, frame, out);
  #endif
 }

 // -------------------------------------------------------------------------

 // update all keyframer data.
int      Engine_Update(void)
 {
  kfNode *node,*child;
  sCamera *cam;
  sCAMERA *tcam;
  sCAMERATGT *tcamt;
  sLight *light;
  sLIGHT *tlight;
  sSPOTLIGHT *tspotl;
  sLIGHTTGT *tlightt;
  sAmbient *amb;
  sAMBIENT *tamb;
  sObject *obj,*cobj;
  sOBJECT *tobj;
  sMatrix c,d;
  sQuat q;
  float frame;

  if (!Engine_Scene) return ERROR;
  frame = Engine_Scene->Current;
  // update objects
  for (node = Engine_Scene->Keyframer; node; node = node->Next)
  {
   switch (node->Type)
   {
    case TrackTypeCamera:
        cam  = (sCamera *)node->Object;
        tcam = (sCAMERA *)node->Track;
        Engine_GetKey_Vect  (tcam->pos,  frame, &cam->Pos);
        Engine_GetKey_Float (tcam->fov,  frame, &cam->Fov);
        Engine_GetKey_Float (tcam->roll, frame, &cam->Roll);
        break;
    case TrackTypeCameratgt:
        cam   =    (sCamera *)node->Object;
        tcamt = (sCAMERATGT *)node->Track;
        Engine_GetKey_Vect  (tcamt->pos, frame, &cam->Target);
        break;
    case TrackTypeLight:
        light  = (sLight *)node->Object;
        tlight = (sLIGHT *)node->Track;
        Engine_GetKey_Vect (tlight->pos,   frame, &light->Pos);
        Engine_GetKey_RGB  (tlight->color, frame, &light->Color);
        break;
    case TrackTypeSpotlight:
        light  =     (sLight *)node->Object;
        tspotl = (sSPOTLIGHT *)node->Track;
        Engine_GetKey_Vect  (tspotl->pos,   frame, &light->Pos);
        Engine_GetKey_RGB   (tspotl->color, frame, &light->Color);
        Engine_GetKey_Float (tspotl->roll,  frame, &light->Roll);
        break;
    case TrackTypeLighttgt:
        light   =    (sLight *)node->Object;
        tlightt = (sLIGHTTGT *)node->Track;
        Engine_GetKey_Vect (tlightt->pos, frame, &light->Target);
        break;
    case TrackTypeObject:
        obj  = (sObject *)node->Object;
        tobj = (sOBJECT *)node->Track;
        Engine_GetKey_Vect (tobj->translate, frame, &obj->Translate);
        Engine_GetKey_Vect (tobj->scale,     frame, &obj->Scale);
        Engine_GetKey_Quat (tobj->rotate,    frame, &obj->Rotate);
        qt_normalize (&obj->Rotate, &obj->Rotate);
        qt_invmatrix (&obj->Rotate, c);   // obj->RotMatrix.. No camera
        mat_setscale (&obj->Scale, d);
        c[qX][qW] = obj->Translate.x;
        c[qY][qW] = obj->Translate.y;
        c[qZ][qW] = obj->Translate.z;
        mat_mul (c, d, obj->Matrix);
        break;
    case TrackTypeAmbient:
        amb  = (sAmbient *)node->Object;
        tamb = (sAMBIENT *)node->Track;
        Engine_GetKey_RGB (tamb->color, frame, &amb->Color);
   }
  }
  // update hierarchy
  if (Engine_Flags & FlagHierarchy)
  {
   for (node = Engine_Scene->Keyframer; node; node = node->Next)
   {
    if (node->Type == TrackTypeObject)
    {
     for (child = node->Child; child; child = child->Brother)
     {
      if (child->Type == TrackTypeObject)
      {
       obj  =  (sObject *)node->Object;
       cobj = (sObject *)child->Object;
       mat_mul (obj->Matrix, cobj->Matrix, cobj->Matrix);
      }
     }
    }
   }
  }
  if (Engine_Flags & FlagTransform) Engine_DoTransform();
  return OK;
 }

//
// ±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
//

#include "math3d.c"


