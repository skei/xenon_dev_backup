 #ifndef engine_hpp_included
 #define engine_hpp_included
 // ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

 #include <math.h>
 #include "list.hpp"

 class cVector;
 class cMatrix;
 class cKeyframe;
 class cBitmap;
 class cMaterial;
 class cNormal;
 class cVertex;
 class cPolygon;
 class cNode;
 class cObject;

 struct sTrack;

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
// ÄÄÄ Structures ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
//

typedef union sKeyData
 {
  int     _int;
  float   _float;
  float   _vector[3];
  float   _quat[4];
 } sKeyData;

 // ---

typedef struct sTrack
 {
  int         Flags;
  int         NumKeys;                       // number of keys
  float       Frames;                        // number of frames
  cKeyframe  *Keys;                          // the track
  cKeyframe  *LastKey;                       // pointer to last used key
 } sTrack;


//
// ÄÄÄ Classes ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
//

class cVector
 {
 public:

  cVector() {};
  cVector(float xx,float yy,float zz) { x=xx; y=yy; z=zz; }
  ~cVector() {};

  inline cVector operator=(cVector v)  { x=v.x; y=v.y; z=v.z; return *this; }

  inline cVector operator+(cVector v)  { return cVector(x+v.x, y+v.y, z+v.z); }
  inline cVector operator-(cVector v)  { return cVector(x-v.x, y-v.y, z-v.z); }
  inline cVector operator*(cVector v)  { return cVector(x*v.x, y*v.y, z*v.z); }
  inline cVector operator/(cVector v)  { return cVector(x/v.x, y/v.y, z/v.z); }

  inline cVector operator-()           { return cVector(-x, -y, -z); }

  inline cVector operator+=(cVector v) { x+=v.x; y+=v.y; z+=v.z; return *this; }
  inline cVector operator-=(cVector v) { x-=v.x; y-=v.y; z-=v.z; return *this; }
  inline cVector operator*=(cVector v) { x*=v.x; y*=v.y; z*=v.z; return *this; }
  inline cVector operator/=(cVector v) { x/=v.x; y/=v.y; z/=v.z; return *this; }

  inline int operator==(cVector v)     { if (x==v.x && y==v.y && z==v.z) return 1; return 0; }

  inline void    Normalize()
  {
   float l = x*x + y*y + z*z;
   if (l!=0)
   {
    float ll = 1/sqrt(l);
    x*=ll;
    y*=ll;
    z*=ll;
   }
  }

  inline float   Dot(cVector v) { return (x*v.x + y*v.y + z*v.z); }

  inline cVector Cross(cVector v) { return cVector(y*v.z-z*v.y, z*v.x-x*v.z, x*v.y-y*v.x); }

 // private:

  float x,y,z;

 };

 //
 // ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
 //

class cMatrix
 {
  public:

  cMatrix() {};
  ~cMatrix() {};

  inline cMatrix operator=(cMatrix m)
  {
   M[ 0]=m.M[ 0]; M[ 1]=m.M[ 1]; M[ 2]=m.M[ 2]; M[ 3]=m.M[ 3];
   M[ 4]=m.M[ 4]; M[ 5]=m.M[ 5]; M[ 6]=m.M[ 6]; M[ 7]=m.M[ 7];
   M[ 8]=m.M[ 8]; M[ 9]=m.M[ 9]; M[10]=m.M[10]; M[11]=m.M[11];
   M[12]=m.M[12]; M[13]=m.M[13]; M[14]=m.M[14]; M[15]=m.M[15];
   return *this;
  }

  // ---

  inline void     operator*=(cMatrix m)
  {
   cMatrix ab;
   int i,j,k;
   for (i=0;i<4;i++)
   {
    for (j=0;j<4;j++)
    {
     M[j*4+i] = 0.0;
     for (k=0;k<4;k++)
     {
      M[j*4+i] += M[k*4+i] * m.M[j*4+k];
     }
    }
   }
  }

  // ---

  inline void Identity(void)
  {
   M[ 0]=1; M[ 1]=0; M[ 2]=0; M[ 3]=0;
   M[ 4]=0; M[ 5]=1; M[ 6]=0; M[ 7]=0;
   M[ 8]=0; M[ 9]=0; M[10]=1; M[11]=0;
   M[12]=0; M[13]=0; M[14]=0; M[15]=1;
  }

  // ---

  //   1   0   0   tx
  //   0   1   0   ty
  //   0   0   1   tz
  //   0   0   0   1
  inline void Translation(float tx, float ty, float tz)
  {
   M[ 0] = 1; M[ 1] = 0; M[ 2] = 0; M[ 3] = tx;
   M[ 4] = 0; M[ 5] = 1; M[ 6] = 0; M[ 7] = ty;
   M[ 8] = 0; M[ 9] = 0; M[10] = 1; M[11] = tz;
   M[12] = 0; M[13] = 0; M[14] = 0; M[15] = 1;
  }

  // ---

  inline void RotationXYZ(float xang, float yang, float zang)
  {
   float s1 = sin(xang);
   float s2 = sin(yang);
   float s3 = sin(zang);
   float c1 = cos(xang);
   float c2 = cos(yang);
   float c3 = cos(zang);
   // Ú                                                 ¿
   // ³      c2*c3             c2*s3          s2      tx³
   // ³ -s1*s2*c3-c1*s3   -s1*s2*s3+c1*c3    s1*c2    ty³
   // ³ -c1*s2*c3+s1*s3   -c1*s2*s3-s1*c3    c1*c2    tz³
   // ³        0                 0            0        1³
   // À                                                 Ù
   M[ 0] = c2*c3;
   M[ 1] = c2*s3;
   M[ 2] = s2;
   M[ 3] = 0;
   M[ 4] = -s1*s2*c3 - c1*s3;
   M[ 5] = -s1*s2*s3 + c1*c3;
   M[ 6] = s1*c2;
   M[ 7] = 0;
   M[ 8] = -c1*s2*c3 + s1*s3;
   M[ 9] = -c1*s2*s3 - s1*c3;
   M[10] = c1*c2;
   M[11] = 0;
   M[12] = 0;
   M[13] = 0;
   M[14] = 0;
   M[15] = 1;
  }

  // ---

  // ABC   123   A1+B4+C7  A2+B5+C8  A3+B6+C9
  // DEF * 456 = D1+E4+F7  D2+E5+F8  D3+E6+C9
  // GHI   789   G1+H4+I7  G2+H5+I8  G3+H6+C9

 // private:

  float M[16];

 };

 //
 // ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
 //

class cKeyframe
 {
 public:

  cKeyframe();
  ~cKeyframe();

 // private:

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
  //cKeyframe   *mNext;
  //cKeyframe   *mPrev;   // just points to [i-1] and [i+1] because of spline routines :-/ fix???
 };


class cBitmap
 {
 public:

  cBitmap();
  cBitmap(int w,int h,int b);
  cBitmap(int w,int h,int b, void *ptr);
  ~cBitmap();

  void  Clear(void);
  void  BlitTo(cBitmap &dest);
  void *GetPtr(void);

 // private:

  int   mWidth;
  int   mHeight;
  int   mBpp;
  int   mSize;
  void *mBitmap;

 };

 //
 // ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
 //

class cMaterial
 {
  public:

   cMaterial();
   ~cMaterial();

  // private:

   unsigned int mFlags;
   int          mType;
   char         mName[20];
   char         mFileNameTexture[20];     // texture map
   char         mFileNameOpacity[20];     // alpha map
   char         mFileNameReflection[20];  // env map
   char         mFileNameBump[20];        // bump map
   cBitmap     *mTexture;
   cBitmap     *mOpacity;
   cBitmap     *mReflection;
   cBitmap     *mBump;
   int          mColor;
   // cMaterial   *mNext;
 };

 //
 // ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
 //

class cNormal
 {
 public:

  cNormal();
  ~cNormal();

 // private:

  cVector mPos;
  float   mU;
  float   mV;

 };

 //
 // ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
 //

class cVertex
 {
 public:

  cVertex();
  ~cVertex();

 // private:

  unsigned int mFlags;
  cVector      mPos;
  cVector      mPosR;        // Rotated
  cVector      mPosP;        // Projected

 };

 //
 // ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
 //

class cPolygon
 {
 public:

  cPolygon();
  ~cPolygon();

 // private:

  unsigned int mFlags;
  unsigned int mSmoothingGroup;
  int          mNumVertices;
  int          A,B,C;
  cVertex     *mVertex[3];         // eller flere for clipping. b†de 2d clip & z-clip
  cNormal     *mVertexNormal[3];   // --"--
  cNormal     *mFaceNormal;
  cMaterial   *mMaterial;
  int          mZ;
  cObject     *mObj;
 };

 //
 // ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
 //

class cNode
 {
 public:

  cNode();
  ~cNode();

 // private:

  unsigned int mFlags;
  int          mId;
  char         mName[20];
  cMatrix      mM_o2w;          // object to world
  cMatrix      mM_o2c;          // object to camera
  cMatrix      mMatrix3DS[16];  // the matrix in the 3DS file
  //cNode       *mNext;
  //cNode       *mPrev;
  int          mParentId;
  cNode       *mParent;

 };

 //
 // ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
 //

class cObject : public cNode
 {

 public:

  cObject();
  ~cObject();

 // private:

   cVector      mPos;
   cVector      mRot;
   cVector      mScale;
   cVector      mPivot;
   float        mSphereRadius;
   int          mNumVertices;
   cVertex     *mVertices;
   int          mNumPolygons;
   cPolygon    *mPolygons;
   int          mNumNormals;
   cNormal     *mNormals;
   // - anim -
   //sTrack       kfPos;
   //sTrack       kfRot;
   //sTrack       kfScale;
 };

 //
 // ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
 //

class cLight  : public cNode
 {
 public:

  //virtual void Handle(void);

 // private:

  //unsigned int mFlags;
  cVector      mPos;
  cVector      mRot;
  cVector      mTarget;
  cVector      mPivot;
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

 //
 // ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
 //

class cCamera : public cNode
 {
 public:

  //void CreateMatrix_Free(void);
  //void CreateMatrix_UVN(void);
  //void CreateMatrix_3DS(void);
  //void SetupFrustum(void);

  //virtual void Handle(void);

 // private:

  //unsigned int mFlags;
  int          mType;
  cVector      mPos;
  cVector      mRot;
  cVector      mTarget;
  cVector      mPivot;
  cVector      mUpVector;
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


 //
 // ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
 //

class cScene
 {
 public:

  cScene();
  ~cScene();

  //void   SortPolys(void);
  //void   SetupForRendering(void);
  //void   DrawAllPolys(void);
  cNode *FindNodeByName(char *name);
  cNode *FindNodeById(int Id);

  cMaterial *FindMaterialByName(char *name);

  int    Load3DS(char *file);
  int    Parse3DS(char *buffer,int BufferSize);

 // private:

  List<cNode>       mNodes;
//int               mNumNodes;
//cNode            *mNodes;
  List<cMaterial>   mMaterials;
//int               mNumMaterials;
  //cMaterial      *mMaterials;
  cCamera          *mActiveCamera;
  int               mVisiblePolys;
  int               mVisibleObjects;
  cPolygon        **mRenderList;
  cPolygon        **mSorted;
  float             mAmbientLight;
  float             mViewDistanceX;     // 3D->2D projection
  float             mViewDistanceY;     // for X & Y : AspectRatio fixing
  float             mScreenWidth;       //   width
  float             mScreenHeight;      // & height of screen (320x200 etc)
  // - anim -
  int               mStartFrame;        // Start frame for 3DS animation
  int               mEndFrame;          // End frame for 3DS animation

 };

 // ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
 #endif

