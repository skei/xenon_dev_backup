 #ifndef engine_hpp_included
 #define engine_hpp_included
 // 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�

 #include "math3d.hpp"


class cCamera
 {
  public:
   cCamera() {};
   ~cCamera() {};
   void CreateMatrix_Free(void);
   void CreateMatrix_UVN(void);
   void CreateMatrix_3DS(void);
  protected:
   float mFov;            // Field of view
   vector mPos;           // Position in world space
   vector mRot;           // Rotation (for free camera)
   vector mUpVector;      // Up vector (for UVN camera)
   vector mTarget;        // camera target
   float mRoll;           // Camera roll
   matrix mM;             // Camera matrix
   float mPerspX,mPerspY;
   float mSizeX,mSizeY;
   float mAspectRatio;
 };

class cObject
 {
  public:
  protected:
 }

class cScene
 {
  public:
   cScene() {};
   ~cScene() {};
   void Render(void);
  protected:
   int mCameraType;
   cCamera *mCamera;
 };

 // 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
 #endif
