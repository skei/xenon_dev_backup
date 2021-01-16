
/*
 *      Drawing functions
 *
 *      This source is part of the fatmap2.txt document by
 *      Mats Byggmastar, mri@penti.sit.fi
 *      17.4.1997 Jakobstad, Finland
 *
 *      Companies with self respect are encouraged to contact me if
 *      any of this code is to be used as part of a commercial product.
 */


#include "misc.h"


/*
 *  Watcom float -> long conversion is very slow.     
 *  Therefore we do this with inline FPU assembler.
 *  No fadd trick here though, just simple fistp.
 */

static float Scale16 = 65536.0;

// dst[0] = (long) (src[0] * 65536.0);
// dst[1] = (long) (src[1] * 65536.0);

void fist16two(long * dst, float * src);
#pragma aux fist16two = \
    "   fld    dword [ebx]          "\
    "   fmul   dword [Scale16]      "\
    "   fld    dword [ebx+4]        "\
    "   fmul   dword [Scale16]      "\
    "   fxch   st(1)                "\
    "   fistp  dword [eax]          "\
    "   fistp  dword [eax+4]        "\
    parm [eax] [ebx] modify [8087]


// *dst = (long) src;

void fist(long * dst, double src);
#pragma aux fist = \
    "   fistp  dword [eax]  "\
    parm [eax] [8087] modify [8087]


inline int cliptest(vertexuvfloat * v)
{
    int bits = 0;
    if(v->y < TopClip)   bits |= TOP;
    if(v->y > BotClip)   bits |= BOT;
    if(v->x < LeftClip)  bits |= LEFT;
    if(v->x > RightClip) bits |= RIGHT;
    return bits;
}

void DrawFlatTriangle(vertexuvfloat * vtx, int color)
{
    vertexuvfloat * v0 = vtx+0;
    vertexuvfloat * v1 = vtx+1;
    vertexuvfloat * v2 = vtx+2;

    v0->visible = cliptest(v0);
    v1->visible = cliptest(v1);
    v2->visible = cliptest(v2);

    int andbits = v0->visible & v1->visible & v2->visible;
    if(andbits != 0) return; 

    int orbits  = v0->visible | v1->visible | v2->visible;

    if((orbits & (TOP | BOT | LEFT | RIGHT)) == VISIBLE) {
        static vertex v[3];
        fist16two(&v[0].x, &v0->x);
        fist16two(&v[1].x, &v1->x);
        fist16two(&v[2].x, &v2->x);
        DrawFlatPoly(v, 3, color);
    }
    else {
        static vertexclip vc[12], ** dst;
        static vertex v[12];
        vc[0].x = v0->x;
        vc[0].y = v0->y;
        vc[2].u = 0.0;                      // We don't need u,v
        vc[2].v = 0.0;
        vc[0].visible = v0->visible;
        vc[1].x = v1->x;
        vc[1].y = v1->y;
        vc[2].u = 0.0;
        vc[2].v = 0.0;
        vc[1].visible = v1->visible;
        vc[2].x = v2->x;
        vc[2].y = v2->y;
        vc[2].u = 0.0;
        vc[2].v = 0.0;
        vc[2].visible = v2->visible;
        int max = ClipUV(&dst, vc);
        if(max < 3) return;

        for(int n=0; n<max; n++) {
            fist16two(&v[n].x, &dst[n]->x);
        }
        DrawFlatPoly(v, max, color);
    }
}

void DrawGouraudTriangle(vertexuvfloat * vtx)
{
    vertexuvfloat * v0 = vtx+0;
    vertexuvfloat * v1 = vtx+1;
    vertexuvfloat * v2 = vtx+2;

    v0->visible = cliptest(v0);
    v1->visible = cliptest(v1);
    v2->visible = cliptest(v2);

    int andbits = v0->visible & v1->visible & v2->visible;
    if(andbits != 0) return; 

    // We use u to hold the color intensity

    double denom = (v0->x - v2->x) * (v1->y - v2->y) -
                   (v1->x - v2->x) * (v0->y - v2->y);
    if(denom == 0.0) return;
    long didx;
    fist(&didx, ((v0->u - v2->u) * (v1->y - v2->y) -
                 (v1->u - v2->u) * (v0->y - v2->y)) / denom * 65536.0);

    int orbits  = v0->visible | v1->visible | v2->visible;

    if((orbits & (TOP | BOT | LEFT | RIGHT)) == VISIBLE) {
        static vertexi v[3];
        fist16two(&v[0].x, &v0->x);
        fist(&v[0].i, v0->u*65536.0);
        fist16two(&v[1].x, &v1->x);
        fist(&v[1].i, v1->u*65536.0);
        fist16two(&v[2].x, &v2->x);
        fist(&v[2].i, v2->u*65536.0);
        DrawGouraudPoly(v, 3, didx);
    }
    else {
        static vertexclip vc[12], ** dst;
        static vertexi v[12];
        vc[0].x = v0->x;
        vc[0].y = v0->y;
        vc[0].u = v0->u;
        vc[0].v = 0.0;                      // We don't need v
        vc[0].visible = v0->visible;
        vc[1].x = v1->x;
        vc[1].y = v1->y;
        vc[1].u = v1->u;
        vc[1].v = 0.0;
        vc[1].visible = v1->visible;
        vc[2].x = v2->x;
        vc[2].y = v2->y;
        vc[2].u = v2->u;
        vc[2].v = 0.0;
        vc[2].visible = v2->visible;
        int max = ClipUV(&dst, vc);
        if(max < 3) return;

        for(int n=0; n<max; n++) {
            fist16two(&v[n].x, &dst[n]->x);
            fist(&v[n].i, dst[n]->u*65536.0);
        }
        DrawGouraudPoly(v, max, didx);
    }
}


void DrawTextureTriangle(vertexuvfloat * vtx, char * bitmap)
{
    vertexuvfloat * v0 = vtx+0;
    vertexuvfloat * v1 = vtx+1;
    vertexuvfloat * v2 = vtx+2;

    v0->visible = cliptest(v0);
    v1->visible = cliptest(v1);
    v2->visible = cliptest(v2);

    int andbits = v0->visible & v1->visible & v2->visible;
    if(andbits != 0) return; 

    double denom = (v0->x - v2->x) * (v1->y - v2->y) -
                   (v1->x - v2->x) * (v0->y - v2->y);
    if(denom == 0.0) return;
    double id = 1.0/denom * 65536.0;
    long dudx, dvdx;
    fist(&dudx, ((v0->u - v2->u) * (v1->y - v2->y) -
                 (v1->u - v2->u) * (v0->y - v2->y)) * id);
    fist(&dvdx, ((v0->v - v2->v) * (v1->y - v2->y) -
                 (v1->v - v2->v) * (v0->y - v2->y)) * id);

    int orbits  = v0->visible | v1->visible | v2->visible;

    if((orbits & (TOP | BOT | LEFT | RIGHT)) == VISIBLE) {
        static vertexuv v[3];
        fist16two(&v[0].x, &v0->x);
        fist16two(&v[0].u, &v0->u);
        fist16two(&v[1].x, &v1->x);
        fist16two(&v[1].u, &v1->u);
        fist16two(&v[2].x, &v2->x);
        fist16two(&v[2].u, &v2->u);
        DrawTexturePoly(v, 3, bitmap, dudx, dvdx);
    }
    else {
        static vertexclip vc[12], ** dst;
        static vertexuv v[12];
        vc[0].x = v0->x;
        vc[0].y = v0->y;
        vc[0].u = v0->u;
        vc[0].v = v0->v;
        vc[0].visible = v0->visible;
        vc[1].x = v1->x;
        vc[1].y = v1->y;
        vc[1].u = v1->u;
        vc[1].v = v1->v;
        vc[1].visible = v1->visible;
        vc[2].x = v2->x;
        vc[2].y = v2->y;
        vc[2].u = v2->u;
        vc[2].v = v2->v;
        vc[2].visible = v2->visible;
        int max = ClipUV(&dst, vc);
        if(max < 3) return;

        for(int n=0; n<max; n++) {
            fist16two(&v[n].x, &dst[n]->x);
            fist16two(&v[n].u, &dst[n]->u);
        }
        DrawTexturePoly(v, max, bitmap, dudx, dvdx);
    }
}

void DrawArbitraryTextureTriangle(vertexuvfloat * vtx, char * bitmap,
                                  int bitmapwidth)
{
    vertexuvfloat * v0 = vtx+0;
    vertexuvfloat * v1 = vtx+1;
    vertexuvfloat * v2 = vtx+2;

    v0->visible = cliptest(v0);
    v1->visible = cliptest(v1);
    v2->visible = cliptest(v2);

    int andbits = v0->visible & v1->visible & v2->visible;
    if(andbits != 0) return; 

    double denom = (v0->x - v2->x) * (v1->y - v2->y) -
                   (v1->x - v2->x) * (v0->y - v2->y);
    if(denom == 0.0) return;
    double id = 1.0/denom * 65536.0;
    long dudx, dvdx;
    fist(&dudx, ((v0->u - v2->u) * (v1->y - v2->y) -
                 (v1->u - v2->u) * (v0->y - v2->y)) * id);
    fist(&dvdx, ((v0->v - v2->v) * (v1->y - v2->y) -
                 (v1->v - v2->v) * (v0->y - v2->y)) * id);

    int orbits  = v0->visible | v1->visible | v2->visible;

    if((orbits & (TOP | BOT | LEFT | RIGHT)) == VISIBLE) {
        static vertexuv v[3];
        fist16two(&v[0].x, &v0->x);
        fist16two(&v[0].u, &v0->u);
        fist16two(&v[1].x, &v1->x);
        fist16two(&v[1].u, &v1->u);
        fist16two(&v[2].x, &v2->x);
        fist16two(&v[2].u, &v2->u);
        DrawArbitraryTexturePoly(v, 3, bitmap, bitmapwidth, dudx, dvdx);
    }
    else {
        static vertexclip vc[12], ** dst;
        static vertexuv v[12];
        vc[0].x = v0->x;
        vc[0].y = v0->y;
        vc[0].u = v0->u;
        vc[0].v = v0->v;
        vc[0].visible = v0->visible;
        vc[1].x = v1->x;
        vc[1].y = v1->y;
        vc[1].u = v1->u;
        vc[1].v = v1->v;
        vc[1].visible = v1->visible;
        vc[2].x = v2->x;
        vc[2].y = v2->y;
        vc[2].u = v2->u;
        vc[2].v = v2->v;
        vc[2].visible = v2->visible;
        int max = ClipUV(&dst, vc);
        if(max < 3) return;

        for(int n=0; n<max; n++) {
            fist16two(&v[n].x, &dst[n]->x);
            fist16two(&v[n].u, &dst[n]->u);
        }
        DrawArbitraryTexturePoly(v, max, bitmap, bitmapwidth, dudx, dvdx);
    }
}

void DrawTiledTextureTriangle(vertexuvfloat * vtx, char * bitmap)
{
    vertexuvfloat * v0 = vtx+0;
    vertexuvfloat * v1 = vtx+1;
    vertexuvfloat * v2 = vtx+2;

    v0->visible = cliptest(v0);
    v1->visible = cliptest(v1);
    v2->visible = cliptest(v2);

    int andbits = v0->visible & v1->visible & v2->visible;
    if(andbits != 0) return; 

    double denom = (v0->x - v2->x) * (v1->y - v2->y) -
                   (v1->x - v2->x) * (v0->y - v2->y);
    if(denom == 0.0) return;
    double id = 1.0/denom * 65536.0;
    long dudx, dvdx;
    fist(&dudx, ((v0->u - v2->u) * (v1->y - v2->y) -
                 (v1->u - v2->u) * (v0->y - v2->y)) * id);
    fist(&dvdx, ((v0->v - v2->v) * (v1->y - v2->y) -
                 (v1->v - v2->v) * (v0->y - v2->y)) * id);

    int orbits  = v0->visible | v1->visible | v2->visible;

    if((orbits & (TOP | BOT | LEFT | RIGHT)) == VISIBLE) {
        static vertexuv v[3];
        fist16two(&v[0].x, &v0->x);
        fist16two(&v[0].u, &v0->u);
        fist16two(&v[1].x, &v1->x);
        fist16two(&v[1].u, &v1->u);
        fist16two(&v[2].x, &v2->x);
        fist16two(&v[2].u, &v2->u);
        DrawTiledTexturePoly(v, 3, bitmap, dudx, dvdx);
    }
    else {
        static vertexclip vc[12], ** dst;
        static vertexuv v[12];
        vc[0].x = v0->x;
        vc[0].y = v0->y;
        vc[0].u = v0->u;
        vc[0].v = v0->v;
        vc[0].visible = v0->visible;
        vc[1].x = v1->x;
        vc[1].y = v1->y;
        vc[1].u = v1->u;
        vc[1].v = v1->v;
        vc[1].visible = v1->visible;
        vc[2].x = v2->x;
        vc[2].y = v2->y;
        vc[2].u = v2->u;
        vc[2].v = v2->v;
        vc[2].visible = v2->visible;
        int max = ClipUV(&dst, vc);
        if(max < 3) return;

        for(int n=0; n<max; n++) {
            fist16two(&v[n].x, &dst[n]->x);
            fist16two(&v[n].u, &dst[n]->u);
        }
        DrawTiledTexturePoly(v, max, bitmap, dudx, dvdx);
    }
}

