typedef unsigned char byte;
typedef unsigned short int word;

byte HMap[256*256];        // Height field

byte CMap[256*256];        // Color map
byte RMap[256*256];
byte GMap[256*256];
byte BMap[256*256];

int lasty[320];            // Last pixel drawn on a given column

int lastc[320];            // Color of last pixel on a column
int lastr[320];            // Red   of last pixel on a column
int lastg[320];            // Green of last pixel on a column
int lastb[320];            // Blue  of last pixel on a column

float  FOV=3.141592654/4;  // half of the xy field of view
float topi=3.141592654*2;  // half of the xy field of view

float sint(float rad);
float cost(float rad);
int Clamp(int x);
void VoxComputeMap(void);

void VoxLine(word *toarea, int x0,int y0,int x1,int y1,int hy,int s, int color);
void VoxView(word *toarea,int x0,int y0,float aa);


