 #ifndef meta_h_included
 #define meta_h_included
 // ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

 //   3---7
 //  /|  /|
 // 2---6 |
 // | | | |
 // | 1-|-5
 // |/  |/
 // 0---4

 //       /--11---
 //      /|      /|
 //     / |     / |
 //    1  3    5  |
 //   /   |   /   |
 //  /    |  /    |
 // /--10---|     7
 // |     | |     |
 // |     | |     |
 // 2    / -|-9--/
 // |   /   |   /
 // |  0    6  4
 // | /     | /
 // |/      |/
 //  ---8---


 // antall grid-elementer i hver retning
 #define GRIDX 20
 #define GRIDY 20
 #define GRIDZ 20
 #define CUBEX (GRIDX-1)
 #define CUBEY (GRIDY-1)
 #define CUBEZ (GRIDZ-1)
 #define NUMCUBES (GRIDX*GRIDY*GRIDZ)

 #define NUMBALLS 5

 #define MAXPOLYS 2000

 // avstanden mellom hver grid-pos i 3d
 #define GRIDSIZE 27
 // verdien Iso'en m† v‘re st›rre enn, for † kalles INNE i blob'en
 #define THRESHOLD 300

 // 3D -> 2D verdier
 #define ZADD 400
 #define XMUL 256
 #define YMUL 256

// ---

typedef struct sVector
 {
  float x,y,z;
 } sVector;

typedef struct sVertex
 {
  unsigned int CalcFlag;
  sVector Pos;
  sVector pPos;           // projected position
  int clip_visible;
  int ceil_y;
  float prestep;
  float nx,ny,nz;         // vertex normal
  float u,v;
 } sVertex;

typedef struct sPolygon
 {
  sVertex *v[3];                // v[6] for clipping?
  char *texture;
  float nx,ny,nz;               // normal
  int Z;
 } sPolygon;


typedef struct cube_t
 {
  // float   *iso[8];
  //unsigned int CalcFlag;      // 12 bits used, set when appropriate edge is calculated
  //                            // set upper bit when cube has been processed!
  sVertex *V[12];
 } cube_t;

typedef struct BALL
 {
  float x,y,z;
  float sx,sy,sz;
  float sx_add,sy_add,sz_add;
  float strength;
 } BALL;


 // ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
 #endif

