
/*
 *      Misc declarations
 *
 *      This source is part of the fatmap2.txt document by
 *      Mats Byggmastar, mri@penti.sit.fi
 *      17.4.1997 Jakobstad, Finland
 *
 *      Companies with self respect are encouraged to contact me if
 *      any of this code is to be used as part of a commercial product.
 */


#ifndef _MISC_H
#define _MISC_H

/*
 *
 *      Declarations for mappers
 *
 */

extern char * WritePagePtr;
extern int    WritePageWidth;

struct vertex
{
    long x,y;       // Screen position in 16:16 bit fixed point
};

struct vertexi
{
    long x,y;       // Screen position in 16:16 bit fixed point
    long i;         // Color intensity in 16:16 bit fixed point
};

struct vertexuv
{
    long x,y;       // Screen position in 16:16 bit fixed point
    long u,v;       // Texture u,v in 16:16 bit fixed point
};

extern void DrawFlatPoly(vertex * vtx, int vertices, int color);
extern void DrawGouraudPoly(vertexi * vtx, int vertices, long didx);
extern void DrawTexturePoly(vertexuv * vtx, int vertices,
                            char * bitmap, long dudx, long dvdx);
extern void DrawArbitraryTexturePoly(vertexuv * vtx, int vertices,
                                     char * bitmap, int bitmapwidth,
                                     long dudx, long dvdx);
extern void DrawTiledTexturePoly(vertexuv * vtx, int vertices,
                                 char * bitmap, long dudx, long dvdx);


inline long ceil(long x)
{
    x +=  0xffff;
    return (x >> 16);
}


long imul16(long x, long y);        // (x * y) >> 16
#pragma aux imul16 = \
    " imul  edx        "\
    " shrd  eax,edx,16 "\
    parm [eax] [edx] value [eax]


long imul14(long x, long y);        // (x * y) >> 14
#pragma aux imul14 = \
    " imul  edx        "\
    " shrd  eax,edx,14 "\
    parm [eax] [edx] value [eax]


long idiv16(long x, long y);        // (x << 16) / y
#pragma aux idiv16 = \
    " mov   edx,eax    "\
    " sar   edx,16     "\
    " shl   eax,16     "\
    " idiv  ebx        "\
    parm [eax] [ebx] modify exact [eax edx] value [eax]



/*
 *
 *      Declarations for 2d clipper
 *
 */

extern float TopClip;
extern float BotClip;
extern float LeftClip;
extern float RightClip;

#define VISIBLE     0
#define TOP         1
#define BOT         2
#define LEFT        4
#define RIGHT       8

struct vertexclip
{
    float x,y;
    float u,v;
    int visible;
};

extern int ClipUV(vertexclip *** final, vertexclip * vbp);



/*
 *
 *      Declarations for drawing functions
 *
 */

struct vertexuvfloat
{
    float x,y;
    float u,v;
    int visible;
};

extern void DrawFlatTriangle(vertexuvfloat * vtx, int color);
extern void DrawGouraudTriangle(vertexuvfloat * vtx);
extern void DrawTextureTriangle(vertexuvfloat * vtx, char * bitmap);
extern void DrawTiledTextureTriangle(vertexuvfloat * vtx, char * bitmap);
extern void DrawArbitraryTextureTriangle(vertexuvfloat * vtx, char * bitmap,
                                         int bitmapwidth);


#endif
