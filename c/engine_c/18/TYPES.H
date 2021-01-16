#ifndef types_h_included
#define types_h_included

struct screen_S
{						// Screen structure
 int width;
 int height;
 unsigned short * screen;
};

struct camstruc
{
 int status;
 int xang,yang,zang;			
 int xpos,ypos,zpos;
 int targetx,targety,targetz;
 int upx,upy,upz;
 int Bank;
 float m[16];
};

// --------------------
// The object structure
// --------------------

struct objstruc
{
 int status;                    // 0/1 (not)/active
 struct objstruc *next;         // pointer to next object
 struct objstruc *prev;         // pointer to prevois object
 int rottype;                   // rotation type (face/vertex)
 int shadetype;                 // shading type (flat, gouraud, phong, etc)
 int xpos,ypos,zpos;            // Object's center in 3D
 int xang,yang,zang;            // Object's rotation angles
 float m[16];                   // Calculated matrix
 float e[16];                   // Calculated eye matrix
 int bbox[24];                  // Bounding box
 int numcoords;                 // Number of Vertexes/Coordinates
 int numpolys;                  // Number of Polys/Faces
 int *coords;                   // Pointer to list of coordinates
 int *polys;                    // Pointer to polygon data
 int *vertex_n;                 // Pointer to vertex normals
 int *face_n;                   // Pointer to face normals
 char *phongmap;                // Pointer to PhongMap
 int *bump_c;                   // Pointer to bump coordinates
 unsigned short int *bumpmap;   // Pointer to BumpMap
 int *tex_c;                    // Pointer to Texture coordinates
 char *texmap;                  // Pointer to texture map
};

struct sortstruc
{
 short int data;
 short int sortvalue;
};

struct polystruc
{
 int x1,y1,x2,y2,x3,y3;
 short int tyx1,tyx2,tyx3;		// ty*256+tx
};

struct F8_8
{
 char AL;
 char AH;
};

union REG
{
 int D;
 short int AX;
 struct F8_8 B;
};

/* eksempel:
 union REG EAX;
 EAX.D = 32;
 EAX.AX = 1000;
 EAX.D.AL = 15;
*/

#endif