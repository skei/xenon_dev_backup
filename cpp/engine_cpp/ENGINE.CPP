 #include <string.h>    // strncmp
 #include "engine.hpp"

//
// ÄÄÄ tColor ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
//

tColor::tColor()
 {
  r = 0;
  g = 0;
  b = 0;
 }

 // ---

tColor::~tColor()
 {
 }

//
// ÄÄÄ tVector ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
//

tVector::tVector()
 {
  x = 0;
  y = 0;
  z = 0;
 }

 // ---

tVector::~tVector()
 {
 }

//
// ÄÄÄ tVertex ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
//

tVertex::tVertex()
 {
  u = 0;
  v = 0;
 }

 // ---

tVertex::~tVertex()
 {
 }

//
// ÄÄÄ tPolygon ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
//

tPolygon::tPolygon()
 {
  a = 0;
  b = 0;
  c = 0;
  flags = 0;
  material = 0;
  smoothinggroup = 0;
 }

 // ---

tPolygon::~tPolygon()
 {
 }

//
// ÄÄÄ tKeyNode ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
//

tKeyNode::tKeyNode()
 {
  frame = 0;
  acceleration = 0;
  tension = 0;
  continuity = 0;
  bias = 0;
  easeto = 0;
  easefrom = 0;
  data.f[0] = 0;
  data.f[1] = 0;
  data.f[2] = 0;
  data.f[3] = 0;
 }

 // ---

tKeyNode::~tKeyNode()
 {
 }

//
// ÄÄÄ tKeyFrame ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
//

tKeyFrame::tKeyFrame()
 {
  flags = 0;
  numkeys = 0;
  keys = NULL;
 }

 // ---

tKeyFrame::~tKeyFrame()
 {
  if (keys != NULL) delete[] keys;
 }

//
// ÄÄÄ tNode ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
//

tNode::tNode()
 {
  name[0]  = 0;
  id       = -1;
  parentid = -1;
  next = NULL;
  prev = NULL;
 }

 // ---

tNode::~tNode()
 {
 }

//
// ÄÄÄ tObject ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
//

tObject::tObject()
 {
  int i;

  hidden = false;
  nocastshadows = false;
  matte = false;
  noreceiveshadows = false;
  numvertices = 0;
  vertices = NULL;
  numpolygons = 0;
  polygons = NULL;
  for (i=0;i<12;i++) matrix3DS[i] = 0;
 }

 // ---

tObject::~tObject()
 {
  if (vertices!=NULL) delete[] vertices;
  if (polygons!=NULL) delete[] polygons;
 }

//
// ÄÄÄ tLight ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
//

tLight::tLight()
 {
  type = 0;
  pos.x = 0;
  pos.y = 0;
  pos.z = 0;
  spot_target.x = 0;
  spot_target.y = 0;
  spot_target.z = 0;
  spot_hotspot = 0;
  spot_falloff = 0;
  spot_rectangular = false;
  spot_spotmap[0] = 0;
  spot_spotroll = 0;

  strength = 0;
  maxrange = 0;
 }

 // ---

tLight::~tLight()
 {
 }

//
// ÄÄÄ tCameraÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
//

tCamera::tCamera()
 {
  pos.x = 0;
  pos.y = 0;
  pos.z = 0;
  target.x = 0;
  target.y = 0;
  target.z = 0;
  roll = 0;
  fov = 0;
 }

 // ---

tCamera::~tCamera()
 {
 }

//
// ÄÄÄ tMaterial ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
//

tMaterial::tMaterial()
 {
  flags = 0;
  color_ambient.r = 0;
  color_ambient.g = 0;
  color_ambient.b = 0;
  color_diffuse.r = 0;
  color_diffuse.g = 0;
  color_diffuse.b = 0;
  color_specular.r = 0;
  color_specular.g = 0;
  color_specular.b = 0;
  color_shininess = 0;
  color_shininess_strength = 0;
  map_texture1[0] = 0;
  map_texture2[0] = 0;
  map_opacity[0] = 0;
  map_bump[0] = 0;
  map_reflection[0] = 0;
  uscale = 0;
  vscale = 0;
  uoffset = 0;
  voffset = 0;
  rotationangle = 0;

 }

 // ---

tMaterial::~tMaterial()
 {
 }

//
// ÄÄÄ tScene ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
//

tScene::tScene()
 {
  startframe = 0;
  endframe = 0;
  ambientKF = NULL;
  numnodes = 0;
  nodes = NULL;
  lastnode = NULL;
 }

 // ---

tScene::~tScene()
 {
 }

 // ---

class tNode *tScene::findnode(char *name)
 {
  tNode *n;

  n = nodes;
  while (n!= NULL)
  {
   if ( strncmp(name,n->name,20) == 0 ) return n;
   n=n->next;
  }
  return NULL;

 }

 // ---

class tNode *tScene::findnode(int id)
 {
  tNode *n;

  n = nodes;
  while (n!= NULL)
  {
   if (n->id == id) return n;
   n=n->next;
  }
  return NULL;
 }

 // ---

void tScene::addnode(tNode *n)
 {
  if (nodes==NULL) nodes = n;
  else
  {
   lastnode->next = n;
   n->prev = lastnode;
  }
  lastnode = n;
 }

