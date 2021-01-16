 #include <math.h>
 #include "engine.hpp"

//
// ÄÄÄ cNode ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
//

cNode::cNode()
 {
  mNext   = NULL;
  mChild  = NULL;
  mParent = NULL;
 }

 // ---

cNode::~cNode()
 {
 }

 // ---

void cNode::Render(void)
 {
  if (mNext != NULL) mNext->Render();
  if (mChild != NULL) mChild->Render();
 }

//
// ÄÄÄ cObject ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
//

cObject::cObject()
 {
 }

 // ---

cObject::~cObject()
 {
 }

 // ---

void cObject::Render(void)
 {
 }

//
// ÄÄÄ cLight ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
//

cLight::cLight()
 {
 }

 // ---

cLight::~cLight()
 {
 }

 // ---

void cLight::Render(void)
 {
 }

//
// ÄÄÄ cCamera ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
//

cCamera::cCamera()
 {
 }

 // ---

cCamera::~cCamera()
 {
 }

 // ---

void cCamera::Render(void)
 {
 }

//
// ÄÄÄ cWorld ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
//

cWorld::cWorld()
 {
  mNodes = NULL;
 }

 // ---

cWorld::~cWorld()
 {
 }

 // ---

void cWorld::Render(void)
 {
  mNodes->Render();
 }

  // Set up camera matrix
  // set up camera view frustum planes
  // for all objects: calc o2w matrix
  // for all objects:
  // {
  //  if parent != NULL inherit matrix from parent's o2w ( = o2w)
  //  multiply w/camera matrix ( = o2c )
  // }
  // for all objects:
  // {
  //  if active, continue
  //  bounding sphere test (A)
  //  for all vertices:
  //  {
  //   transform o2c
  //   project 3D->2D
  //  }
  //  for all polygons:
  //  {
  //   backface cull
  //   check if z-clipping needed (B)
  //   calc z for sorting
  //   insert in global renderlist
  //  }
  // }
  // sort renderlist
  // draw all polys. (C)
  //

 /*

  (A) Check distance from center of bounding sphere (object pos, or pivot?)
      against all view frustum planes...  mark object INSIDE, OUTSIDE or
      NEED_CLIP if OUTSIDE, skip entire object - since it's not visible on
      screen Mark with a bit for each clip plane... So that we know which side
      of the screen and eventual z-clip we need to test for later on...

  (B) If bounding sphere intersects near Z plane, Check Z for all three
      vertices, if all beyond screen plane, skip poly If one or two, mark as
      Z-clipping necessary

  (C) 1. Calc constant gradients
      2. If z-clipping necessary, clip against view plane
      3. if NEED_CLIP - usual 2D clipping
      4. Draw poly, according to shading/mapping flags & material, etc...

 */

