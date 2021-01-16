 #ifndef engine_hpp_included
 #define engine_hpp_included
 //컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴

 #define NULL 0
 #define PI 3.1415926535

 class cVector;
 class cMatrix;
 class cQuaternion;
 class cVertex;
 class cMap;
 class cMaterial;
 class cPolygon;

 class cKeyFrameKey;

 class cNode;
 class cObject;
 class cLight;
 class cCamera;

//
// --- typedef ---
//

typedef union kfData
 {
  int     _int;
  float   _float;
  cVector _vector;
  cQuat   _quat;
 } kfData;


 // -------------------------------------------------------------------------
 /*

 PreRender : hierarchymatrix setup
 Render :

 */
 // -------------------------------------------------------------------------

 // this node also acts as the dummy object
class cNode
 {
  public:

   cNode();
   ~cNode();

   void Render(void);

   void SetBrother (cNode *N);
   void SetChild   (cNode *N);
   void SetParent  (cNode *N);

  protected:

   char Name[20];


   cNode * mBrother;   // next object on same hierarchy level
   cNode * mChild;     // child to current object
   cNode * mParent;    // is this needed

 };

 // -------------------------------------------------------------------------

class cObject : public cNode
 {
  public:

   cObject();
   ~cObject();

   void Render(void);

  protected:
 };

 // -------------------------------------------------------------------------

class cLight  : public cNode
 {
  public:

   cLight();
   ~cLight();

   void Render(void);

  protected:
 };

 // -------------------------------------------------------------------------

class cCamera : public cNode
 {
  public:

    cCamera();
   ~cCamera();

   void Render(void);

  protected:
 };

 // ---

class cWorld
 {
  public:

   cWorld();
   ~cWorld();

   void Render(void);

  protected:

   cNode *mNodes;
 };

 //컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
 #endif
