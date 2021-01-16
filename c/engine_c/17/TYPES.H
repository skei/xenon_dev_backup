#ifndef types_h_included
#define types_h_included

// --------------------
// The camera structure
// --------------------

struct camstruc
{
 int status;
 int xang,yang,zang;
 int xpos,ypos,zpos;
 int m[16];
};

// --------------------
// The object structure
// --------------------

struct objstruc
{
 int status;                        // 0/1 (not)/active
 struct objstruc *next;             // pointer to next object
 struct objstruc *prev;             // pointer to prevois object
 int rottype;                       // rotation type (face/vertex)
 int shadetype;                     // shading type (flat,gouraud,phong,etc)
 int xpos,ypos,zpos;                // Object's center in 3D
 int xang,yang,zang;                // Object's rotation angles
 int m[16];                         // Calculated matrix
 int cc1,cc2,cc3;                   // calculated constants for 7 imul rot
 int bbx1,bby1,bbz1,bbx2,bby2,bbz2; // Bounding box
 int numcoords;                     // Number of Vertexes/Coordinates
 int numpolys;                      // Number of Polys/Faces
 int *coords;                       // Pointer to list of coordinates
 int *polys;                        // Pointer to polygon data
 int *vertex_n;                     // Pointer to vertex normals
 int *face_n;                       // Pointer to face normals
 char *phongmap;                    // Pointer to PhongMap
 int *bump_c;                       // Pointer to bump coordinates
 unsigned short int *bumpmap;       // Pointer to BumpMap
 int *tex_c;                        // Pointer to Texture coordinates
 char *texmap;                      // Pointer to texture map
};

// Structure for ByteSort... Have to change this, so that an unsigned int
// represents both these values.... Polynumber in upper word and zvalue
// in lower word

struct sortstruc
{
 short int polynumber;
 short int zvalue;
};

// Fixed point stuff... Not used yet....

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
