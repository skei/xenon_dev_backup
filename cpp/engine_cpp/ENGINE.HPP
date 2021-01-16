 #ifndef engine_hpp_included
 #define engine_hpp_included
 // 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�

 // change to #define DEBUG(x) //x to remove all debug stuff
 //#define DEBUG(x) x
#define DEBUG(x) //x

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
    float   u,v;
 };

 // ---

class tPolygon
 {
  public:
    tPolygon();
    ~tPolygon();
  //private:
    int a,b,c,flags;
    int material;
    int smoothinggroup;
 };

 // --- keyframer ---

class tKeyNode
 {
  public:
    tKeyNode();
    ~tKeyNode();
  //private:
    int   frame;         // frame
    int   acceleration;  // spline data flags (bit 0=tension,1=continuity,2=bias,3=Ease to,4=ease from
    float tension;       // -1..1, def=0
    float continuity;    // -1..1, def=0
    float bias;          // -1..1, def=0
    float easeto;        //  0..1, def=0
    float easefrom;      //  0..1, def=0
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
    int       flags;    // bits 0-1 (0=single,2=repeat,3=loop), 3=lock X, 4=locy Y, 5=lock Z, 7=unlink X, 8=unlink Y,9=unlink Z
    int       numkeys;
    tKeyNode *keys;     // array of tKeyNode's or derived classes
 };

 // --- objects ---

class tNode
 {
  public:
    tNode();
    ~tNode();
  //private:
    char         name[20];
    int          id;
    int          parentid;
    //class tNode *next,*prev;
    tKeyFrame    kfPos;
    tKeyFrame    kfRot;
    tKeyFrame    kfScale;
    tKeyFrame    kfFov;
    tKeyFrame    kfRoll;
    tKeyFrame    kfColor;
    tKeyFrame    kfMorph;
    tKeyFrame    kfHotspot;
    tKeyFrame    kfFalloff;
    tKeyFrame    kfHide;
 };

 // ---

class tObject : public tNode
 {
  public:
    tObject();
    ~tObject();
  //private:
    bool      hidden;
    bool      nocastshadows;
    bool      matte;
    bool      noreceiveshadows;
    bool      dummy;
    int       numvertices;
    tVertex  *vertices;
    int       numpolygons;
    tPolygon *polygons;
    float     matrix3DS[12];
 };

 // ---

 #define LIGHTtype_omni 1
 #define LIGHTtype_spot 2


class tLight : public tNode
 {
  public:
    tLight();
    ~tLight();
  //private:
    int     type;
    tVector pos;
    tVector spot_target;
    float   spot_hotspot;          // spotlight
    float   spot_falloff;          // spotlight
    bool    spot_rectangular;      // spotlight
    char    spot_spotmap[20];      // spotlight
    float   spot_spotroll;         // spotlight

    float   strength;
    float   maxrange;
 };

 // ---

class tCamera : public tNode
 {
  public:
    tCamera();
    ~tCamera();
  //private:
    tVector pos;
    tVector target;
    float   roll;    // (degrees)
    float   fov;
 };

 // ---

 #define MATflag_texture1   1
 #define MATflag_texture2   2
 #define MATflag_opacity    4
 #define MATflag_reflection 8
 #define MATflag_bump       16

class tMaterial : public tNode
 {
  public:
    tMaterial();
    ~tMaterial();
  //private:
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
    tKeyFrame *ambientKF;

    int     numnodes;
    tNode  *nodes;
    tNode  *lastnode;
    void    addnode(tNode *n);

    class tNode *findnode(char *name);
    class tNode *findnode(int i);

    //int        numobjects;
    //tObject   *objects;
    //int        numlights;
    //tLight    *lights;
    //int        numcameras;
    //tCamera   *cameras;
    //int        nummaterials;
    //tMaterial *materials;
 };

 // 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
 #endif
