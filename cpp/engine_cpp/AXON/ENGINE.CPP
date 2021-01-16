
 #include <math.h>      // fabs
 #include "engine.hpp"

tColor::tColor()
 {
  r = 0;
  g = 0;
  b = 0;
 }

tColor::~tColor()
 {
 }

tVector::tVector()
 {
  x = 0;
  y = 0;
  z = 0;
 }

tVector::~tVector()
 {
 }

tVertex::tVertex()
 {
  u = 0;
  v = 0;
 }

tVertex::~tVertex()
 {
 }

tPolygon::tPolygon()
 {
  flags = 0;
  a = 0;
  b = 0;
  c = 0;
  material = 0;
  smoothing = 0;
 }

tPolygon::~tPolygon()
 {
 }

tKey::tKey()
 {
  frame = 0;
  flags = 0;
  tension = 0;
  continuity = 0;
  bias = 0;
  easeto = 0;
  easefrom = 0;
 }

tKey::~tKey()
 {
 }

tKeyFrame::tKeyFrame()
 {
  flags = 0;
  numkeys = 0;
  keys = NULL;
 }

tKeyFrame::~tKeyFrame()
 {
 }

tObject::tObject()
 {
  int i;

  name[0] = 0;
  id = -1;
  parentid = -1;
  hidden = 0;
  nocastshadows = 0;
  matte = 0;
  noreceiveshadows = 0;
  dummy = 0;
  numvertices = 0;
  vertices = NULL;
  numpolygons = 0;
  polygons = NULL;
  for (i=0;i<12;i++) matrix3DS[i] = 0;
  kfPos = NULL;
  kfRot = NULL;
  kfScale = NULL;

 }

tObject::~tObject()
 {
 }

tLight::tLight()
 {
  name[0] = 0;
  id = -1;
  parentid = -1;
  type = 0;
  spot_hotspot = 0;
  spot_falloff = 0;
  spot_rectangular = 0;
  spot_map[0] = 0;
  spot_roll = 0;
  strength = 0;
  maxrange = 0;
  kfPos = NULL;
  kfTarget = NULL;
 }

tLight::~tLight()
 {
 }

tCamera::tCamera()
 {
  name[0] = 0;
  id = -1;
  parentid = -1;
  roll = 0;
  fov = 0;
  kfPos = NULL;
  kfTarget = NULL;
  kfFov = NULL;
  kfRoll = NULL;
 }

tCamera::~tCamera()
 {
 }

tMaterial::tMaterial()
 {
  name[0] = 0;
  id = -1;
  parentid = -1;
  flags = 0;
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

tMaterial::~tMaterial()
 {
 }

tScene::tScene()
 {
  startframe = 0;
  endframe = 0;
  kfAmbient = NULL;
  numobjects = 0;
  objects = NULL;
  numlights = 0;
  lights = NULL;
  numcameras = 0;
  cameras = NULL;
  nummaterials = 0;
  materials = NULL;
 }

tScene::~tScene()
 {
 }
