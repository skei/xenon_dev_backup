 #ifndef engine_h_included
 #define engine_h_included
 // ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

 #include <math.h>

 // --- protos and stuff ---

 class cMaterial;
 class cNormal;
 class cVertex;
 class cPolygon;
 class cKeyframe;
 class cNode;
 class cObject;
 class cLight;
 class cCamera;
 class cScene;

inline void  Vector_Print(sVector *v);
inline void  Vector_Create(sVector *v, float x,float y,float z);
inline void  Vector_Zero(sVector *v);
inline void  Vector_Copy(sVector *vd, sVector *vs);
inline void  Vector_Add(sVector *vd, sVector *v1, sVector *v2);
inline void  Vector_Sub(sVector *vd, sVector *v1, sVector *v2);
inline void  Vector_Mul(sVector *vd, sVector *v1, sVector *v2);
inline void  Vector_Div(sVector *vd, sVector *v1, sVector *v2);
inline void  Vector_Scale(sVector *vd, sVector *v, float scale);
inline void  Vector_Negate(sVector *vd, sVector *vs);
inline int   Vector_Equal(sVector *v1, sVector *v2);
inline float Vector_Length(sVector *v);
inline float Vector_Distance(sVector *v1, sVector *v2);
inline float Vector_Dot(sVector *v1, sVector *v2);
inline void  Vector_Cross(sVector *vd, sVector *v1, sVector *v2);
inline void  Vector_Normalize(sVector *vd, sVector *vs);
inline void  Vector_Trans(sVector *dest, sVector v, float *m);
inline void  Vector_Trans3x3(sVector *dest, sVector v, float *m);
inline void  Vector_Inv_Trans3x3(sVector *dest, sVector v, float *m);

// ÄÄÄÄÄÄÄÄÄÄ

inline void Matrix_Identity(float *m);
inline void Matrix_Translation(float *m, float tx, float ty, float tz);
inline void Matrix_RotationXYZ(float *m, float xang, float yang, float zang);
inline void Matrix_RotationX(float *m, float xang);
inline void Matrix_RotationY(float *m, float yang);
inline void Matrix_RotationZ(float *m, float zang);
inline void Matrix_Mul(float *a, float *b);
inline void Matrix_Copy(float *m1, float *m2);
inline void Matrix_Inverse( float *d, float *s );
inline int  Matrix_Inverse2( float *matrix );
inline void Matrix_AngleAxis ( float *m, float angle, sVector *v );

// ÄÄÄÄÄÄÄÄÄÄ

inline void  Quat_Copy( float *out, float *a );
inline void  Quat_Normalize( float *qd,float *q );
inline void  Quat_Mul(float *qd,float *qL, float *qR);
inline void  Quat_Inverse(float *qd,float *q);
inline void  Quat_ToMatrix(float *q, float *m);
inline void  Quat_FromAngAxis(float *q,float ang, sVector *axis);
inline void  Quat_ToAngAxis(float *q, float *ang, sVector *axis);
inline float Quat_RelAngAxis(float *p, float *q, sVector *axis);
inline float Quat_Length(float *a);
inline float Quat_Dot (float *a, float *b);
inline float Quat_DotUnit (float *a, float *b);
inline void  Quat_Slerp(float *out, float *a, float *b, float spin, float alpha);
inline void  Quat_Slerpl (float *out, float *a, float *b, float spin, float alpha);
inline void  Quat_Negate (float *out, float *a);
inline void  Quat_Exp(float *out,float *a);
inline void  Quat_Log(float *out,float *a);
inline void  Quat_Lndif (float *out, float *a, float *b);


//
// ÄÄÄ DEFINES ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
//

#define    NULL 0
#define      PI 3.1415926535
#define EPSILON 1.0E-6
#define    TRUE 1
#define     YES 1
#define   FALSE 0
#define      NO 0

#define qX 0
#define qY 1
#define qZ 2
#define qW 3

// ÄÄÄÄÄ

 // change to #define DEBUG(x) //x to remove all debug stuff
 //#define DEBUG(x) x
#define DEBUG(x) //x

 // Conversion from degrees to radians
#define DEG2RAD(deg) ((deg)*PI/180.0)
 // Calculate focal distance from field of vision
#define FOV2FOC(width,angle) (((width)*cos((angle)/2.0))/(sin((angle)/2.0)*2))

// ÄÄÄ

 // float -> int

#define SSS 65536.0
#define MAGIC (((SSS*SSS*16)+(SSS*.5))*SSS)
#define MAGICfixed (MAGIC/65536.0)

// ÄÄÄ FLAGS ÄÄÄ

#define TRACKflag_loop     1

#define OBJflag_active     1
#define OBJflag_mapped     2

#define POLYflag_material  1

#define LIGHTflag_active   1

#define CAMERAflag_active  1

#define MATflag_texture    1
#define MATflag_opacity    2
#define MATflag_reflection 4
#define MATflag_bump       8

// ÄÄÄ TYPES ÄÄÄ

#define LIGHTtype_omni     1
#define LIGHTtype_spot     2

#define CAMERAtype_free    1
#define CAMERAtype_uvn     2
#define CAMERAtype_3ds     3

#define OBJECTtype_free    1
#define OBJECTtype_3ds     1

// ÄÄÄ 3DS loader errors ÄÄÄ

#define Error3DS_NoError     0
#define Error3DS_InvalidFile 1

//
// ÄÄÄ TYPES ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
//

typedef struct sVector
 {
  float X,Y,Z;
 } sVector;

 // ÄÄÄ

typedef union sKeyData
 {
  int     _int;
  float   _float;
  sVector _vector;
  float   _quat[4];
 } sKeyData;

typedef struct sTrack
 {
  int         Flags;
  int         NumKeys;                       // number of keys
  float       Frames;                        // number of frames
  cKeyframe  *Keys;                          // the track
  cKeyframe  *LastKey;                       // pointer to last used key
 } sTrack;

//
// ÄÄÄ CLASSES ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
//

class cMaterial
 {
  public:
   unsigned int mFlags;
   int          mType;
   char         mName[20];
   char         mFileNameTexture[20];     // texture map
   char         mFileNameOpacity[20];     // alpha map
   char         mFileNameReflection[20];  // env map
   char         mFileNameBump[20];        // bump map
   char        *mTexture;
   char        *mOpacity;
   char        *mReflection;
   char        *mBump;
   int          mColor;
   cMaterial   *mNext;
 };

 // ÄÄÄÄÄ

 // I have a separate normal class, to make it possible to fix the wrapping
 // bugs etc in 3DS, and also to handle smoothing groups..
 //
class cNormal
 {
  public:
   sVector      mPos;
   float        mU;
   float        mV;
 };

 // ÄÄÄÄÄ

class cVertex
 {
  public:
   unsigned int mFlags;
   sVector      mPos;
   sVector      mPosR;        // Rotated
   sVector      mPosP;        // Projected
 };

 // ÄÄÄÄÄ

class cPolygon
 {
  public:
   unsigned int mFlags;
   int          mNumVertices;
   int          A,B,C;
   cVertex     *mVertex[3];         // eller flere for clipping. b†de 2d clip & z-clip
   cNormal     *mVertexNormal[3];   // --"--
   cNormal     *mFaceNormal;
   cMaterial   *mMaterial;
   int          mZ;
   cObject     *mObj;
 };

 // ÄÄÄÄÄ

class cKeyframe
 {
  public:
   float        mFrame;                  // current frame
   float        mTens;
   float        mBias;
   float        mCont;
   float        mEaseTo;
   float        mEaseFrom;
   sKeyData     mVal;                    // the interpolated values
   float        dsa, dsb, dsc, dsd;
   float        dda, ddb, ddc, ddd;
   float        ds[4], dd[4];
   float        qa[4];
   cKeyframe   *mNext;
   cKeyframe   *mPrev;   // just points to [i-1] and [i+1] because of spline routines :-/ fix???
 };

 // ÄÄÄÄÄ

class cNode
 {
  public:
   cNode();

   virtual void Handle(void);

   unsigned int mFlags;
   int          mId;
   char         mName[20];
   float        mM_o2w[16];
   float        mM_o2c[16];
   float        mMatrix3DS[16];
   float        mQ[4];
   cObject     *mNext;
   cObject     *mPrev;
   int          mParentId;
   cObject     *mParent;


 };

 // ÄÄÄÄÄ

class cObject : public cNode
 {
  public:
   cObject();

   void CalcFaceNormals(void);
   void CalcVertexNormals(void);
   void Center(void);
   void CalcBoundingSphere(void);
   void CreateMatrix_Free(void);
   void CheckBoundingSphere(void);

   virtual void Handle(void);

   //unsigned int mFlags;
   int          mType;
   sVector      mPos;
   sVector      mRot;
   sVector      mScale;
   sVector      mPivot;
   float        mSphereRadius;
   int          mNumVertices;
   int          mNumPolygons;
   int          mNumNormals;
   cVertex     *mVertices;
   cPolygon    *mPolygons;
   cNormal     *mNormals;
   // - anim -
   sTrack       kfPos;
   sTrack       kfRot;
   sTrack       kfScale;
 };

 // ÄÄÄÄÄ

class cLight  : public cNode
 {
  public:
   virtual void Handle(void);

   //unsigned int mFlags;
   sVector      mPos;
   sVector      mRot;
   sVector      mTarget;
   sVector      mPivot;
   // char        *mSpotMap
   // Char        *mSpotMapFileName
   float        mStrength;
   float        mMaxRange;
   float        mHotSpot;
   float        mFallOff;
   float        mSpotRoll;    // if rectangular ?
   // - anim -
   sTrack       kfPos;
   sTrack       kfTarget;
 };

 // ÄÄÄÄÄ

class cCamera : public cNode
 {
  public:
   void CreateMatrix_Free(void);
   void CreateMatrix_UVN(void);
   void CreateMatrix_3DS(void);
   void SetupFrustum(void);

   virtual void Handle(void);

   //unsigned int mFlags;
   int          mType;
   sVector      mPos;
   sVector      mRot;
   sVector      mTarget;
   sVector      mPivot;
   sVector      mUpVector;
   float        mM[16];
   float        mRoll;
   float        mFov;
   float        mSizeX,mSizeY;      // 3d->2d projection
   float        mAspectRatio;
   float        mPerspX,mPerspY;    // 3d->2d projection
   // - anim -
   sTrack       kfPos;
   sTrack       kfTarget;
   sTrack       kfFov;
   sTrack       kfRoll;
 };

 // ÄÄÄÄÄ

class cScene
 {
  public:
   void   SortPolys(void);
   void   SetupForRendering(void);
   void   DrawAllPolys(void);
   cNode *FindNodeByName(char *name);
   cNode *FindNodeById(int Id);

   int    Load3DS(char *file);
   int    Parse3DS(char *buffer,int BufferSize);

   int          mNumNodes;
   cNode       *mNodes;
   int          mNumMaterials;
   cMaterial   *mMaterials;
   cCamera     *mActiveCamera;
   int          mVisiblePolys;
   int          mVisibleObjects;
   cPolygon   **mRenderList;
   cPolygon   **mSorted;
   float        mAmbientLight;
   float        mViewDistanceX;     // 3D->2D projection
   float        mViewDistanceY;     // for X & Y : AspectRatio fixing
   float        mScreenWidth;       //   width
   float        mScreenHeight;      // & height of screen (320x200 etc)
   // - anim -
   int          mStartFrame;        // Start frame for 3DS animation
   int          mEndFrame;          // End frame for 3DS animation
 };

//
// ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
//

//
// ÄÄÄ math ÄÄÄ
//

 #pragma inline float2int;
static long float2int( float d )
 {
  double dtemp = MAGIC+d;
  return (*(long *)&dtemp)-0x80000000;
 }

 // ÄÄÄÄÄÄÄÄÄÄ

 #pragma inline float2fixed;
static long float2fixed( float d )
 {
  double dtemp = MAGICfixed+d;
  return (*(long *)&dtemp)-0x80000000;
 }

 // ÄÄÄÄÄÄÄÄÄÄ

 #pragma inline CEIL;
static int CEIL(float f)
 {
  int i,j;

  i = float2fixed(f);
  j = i >> 16;
  if ((i&0xffff) != 0) j+=1;
  return j;
 }

// ÄÄÄÄÄÄÄÄÄÄ


 // ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
 #endif
