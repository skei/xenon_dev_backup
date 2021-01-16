 #ifndef engine_hpp_included
 #define engine_hpp_included
 // 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�

 // change to #define DEBUG(x) //x to remove all debug stuff
 #define DEBUG(x) x
 //#define DEBUG(x) //x

 #ifndef NULL
  #define NULL 0
 #endif

 // ----------

class tColor
 {
  public:
    tColor();
    ~tColor();
  //private:
    float r,g,b;
 };

 // ---

class tVector
 {
  public:
    tVector();
    ~tVector();
  //private:
    float x,y,z;
 };

 // ---

class tVertex
 {
  public:
    tVertex();
    ~tVertex();
  //private:
    tVector pos;
    float u,v;
 };

 // ---

class tPolygon
 {
  public:
    tPolygon();
    ~tPolygon();
  //private:
    int flags;
    int a,b,c;
    int material;
    int smoothing;
 };

 // --- keyframer ---

class tKey
 {
  public:
    tKey();
    ~tKey();
  //private:
    int   frame;
    int   flags;      // spline data flags (bit 0=tension,1=continuity,2=bias,3=Ease to,4=ease from
    float tension;    // -1..1, def=0
    float continuity; // -1..1, def=0
    float bias;       // -1..1, def=0
    float easeto;     //  0..1, def=0
    float easefrom;   //  0..1, def=0
    // spline interpolation data
    float dsa, dsb, dsc, dsd, dda, ddb, ddc, ddd;
    float ds[4], dd[4];
    float qa[4];
    //
    union data
    {
     float f[4];
     bool  b;
     void *p;  // object morph (convert ascii name to ptr to correct object
    } data;
 };

 // ---

class tKeyFrame
 {
  public:
    tKeyFrame();
    ~tKeyFrame();
  //private:
    int   flags;    // bits 0-1 (0=single,2=repeat,3=loop), 3=lock X, 4=locy Y, 5=lock Z, 7=unlink X, 8=unlink Y,9=unlink Z
    int   numkeys;
    tKey *keys;     // array of tKeyNode's or derived classes
 };

 // ---

class tObject
 {
  public:
    tObject();
    ~tObject();
  //private:
    char       name[20];
    int        id;
    int        parentid;
    int        hidden           : 1;
    int        nocastshadows    : 1;
    int        matte            : 1;
    int        noreceiveshadows : 1;
    int        dummy            : 1;
    int        numvertices;
    tVertex   *vertices;
    int        numpolygons;
    tPolygon  *polygons;
    float      matrix3DS[12];
    tKeyFrame *kfPos;
    tKeyFrame *kfRot;
    tKeyFrame *kfScale;
 };

 // ---

 #define LIGHTtype_omni 1
 #define LIGHTtype_spot 2

 // ---

class tLight
 {
  public:
    tLight();
    ~tLight();
  //private:
    char    name[20];
    int     id;
    int     parentid;
    int     type;
    tVector pos;
    tVector spot_target;
    float   spot_hotspot;     // spotlight
    float   spot_falloff;     // spotlight
    bool    spot_rectangular; // spotlight
    char    spot_map[20];     // spotlight
    float   spot_roll;        // spotlight

    float   strength;
    float   maxrange;

    tKeyFrame *kfPos;
    tKeyFrame *kfTarget;      // spotlight
 };

 // ---

class tCamera
 {
  public:
    tCamera();
    ~tCamera();
  //private:
    char    name[20];
    int     id;
    int     parentid;
    tVector pos;
    tVector target;
    float   roll;    // (degrees)
    float   fov;
    tKeyFrame *kfPos;
    tKeyFrame *kfTarget;
    tKeyFrame *kfFov;
    tKeyFrame *kfRoll;
 };

 // ---

 #define MATflag_texture1   1
 #define MATflag_texture2   2
 #define MATflag_opacity    4
 #define MATflag_reflection 8
 #define MATflag_bump       16

 // ---

class tMaterial
 {
  public:
    tMaterial();
    ~tMaterial();
  //private:
    char    name[20];
    int     id;
    int     parentid;
    int    flags;
    tColor color_ambient;
    tColor color_diffuse;
    tColor color_specular;
    float  color_shininess;
    float  color_shininess_strength;
    char   map_texture1[20];   // texture1
    char   map_texture2[20];   // texture2
    char   map_opacity[20];    // alpha
    char   map_bump[20];       // bump
    char   map_reflection[20]; // envmap
    float  uscale,vscale;
    float  uoffset,voffset;
    float  rotationangle;
 };

 // ---

class tScene
 {
  public:
    tScene();
    ~tScene();
  //private:
    int        startframe,endframe;
    tKeyFrame *kfAmbient;

    int        numobjects;
    tObject   *objects;

    int        numlights;
    tLight    *lights;

    int        numcameras;
    tCamera   *cameras;

    int        nummaterials;
    tMaterial *materials;
 };




 // 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
 #endif
