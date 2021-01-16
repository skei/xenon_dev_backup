 #ifndef object_hpp_included
 #define object_hpp_included
 // -------------------------------------------------------------------------

 #include "node.hpp"

// 3d-object class.. inherits frm Node...
class c_OBJECT : public c_NODE
 {

 public:

  c_OBJECT();
  ~c_OBJECT();

/*

  void Object_CreateMatrix(sObject *o, sCamera *c)
  {
   Matrix_Rotation(o->m,o->Rot.x,o->Rot.y,o->Rot.z);
   o->m[ 3] += o->Pos.x;
   o->m[ 7] += o->Pos.y;
   o->m[11] += o->Pos.z;
   Matrix_Mult(o->m,c->m);
  }

  // ---

  // Returns 1 if the object's bounding-box is visible in ViewFrustum, else 0
  int Object_CheckBoundingSphere(sObject *o, sCamera *c)
  {
   sVector s1,s2;
   float xcompare,ycompare;
   int r;

   r = 1;

   s1.x = o->Pos.x;
   s1.y = o->Pos.y;
   s1.z = o->Pos.z;

   s2 = Vector_Trans(s1, c->m);

   if (s2.z + o->BSphereRadius < minclip_z) r = 0;
   if (s2.z - o->BSphereRadius > maxclip_z) r = 0;

   xcompare = halfscreenwidth_vd * s2.z;
   if (s2.x + o->BSphereRadius < -xcompare) r = 0;
   if (s2.x - o->BSphereRadius >  xcompare) r = 0;

   ycompare = halfscreenheight_vd * s2.z;
   if (s2.y + o->BSphereRadius < -ycompare) r = 0;
   if (s2.y - o->BSphereRadius >  ycompare) r = 0;

   return r;
  }

  // ---

  // ByteSort the "world"-object
  void SortWorldObject( void )
  {
   int Numbers[256];
   int i;
   short int a,b,d;

   // clear numbers array
   memset( Numbers,0,sizeof (Numbers));
  // Count number of each byte-value
   for ( i=0;i<World.VisiblePolys;i++ )
   {
    Numbers[ World.RenderList[i]->z & 255 ]++;
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
   for ( i=0;i<World.VisiblePolys;i++ )
   {
    a = World.RenderList[i]->z & 255; //sorttable[i].sortvalue & 255;
    d = Numbers[a];
    World.Sorted[d] = World.RenderList[i];
    Numbers[a]++;
   }
  // -
  // Now, do the same, but with high byte...
  // -
   // clear numbers array
   memset( Numbers,0,sizeof (Numbers));
  // Count number of each byte-value
   for ( i=0;i<World.VisiblePolys;i++ )
   {
    Numbers[ (World.Sorted[i]->z & 0xff00) >> 8 ]++;
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
   for ( i=0;i<World.VisiblePolys;i++ )
   {
    a = (World.Sorted[i]->z & 0xff00) >> 8; //sorttable[i].sortvalue & 255;
    d = Numbers[a];
    World.RenderList[d] = World.Sorted[i];
    Numbers[a]++;
   }
  }

*/

 private:

/*
 vertices
 polydata
*/

 unsigned int Flags;
 float BoundingSphereRadius;
 int NumVertices;
 int NumPolygons;

 };

 // -------------------------------------------------------------------------
 #endif
