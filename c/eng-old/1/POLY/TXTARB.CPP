
/*
 *      Arbitrary size texture mapper
 *
 *      This source is part of the fatmap2.txt document by
 *      Mats Byggmastar, mri@penti.sit.fi
 *      17.4.1997 Jakobstad, Finland
 *
 *      Companies with self respect are encouraged to contact me if
 *      any of this code is to be used as part of a commercial product.
 */


#include "misc.h"

static vertexuv * max_vtx;                  // Max y vertex (ending vertex)
static vertexuv * start_vtx, * end_vtx;     // First and last vertex in array
static vertexuv * right_vtx, * left_vtx;    // Current right and left vertex

static long right_height, left_height;
static long right_x, right_dxdy, left_x, left_dxdy;
static long left_u, left_dudy, left_v, left_dvdy;
static long dudxfrac, dvdxfrac, duvdxstep[2];

void inner(void * dst, int width, void * src, long u, long v);
#pragma aux inner = \
    "    push  ebp                                      "\
    "    lea   edi, [edi+ecx]                           "\
    "    shl   eax, 16                                  "\
    "    xor   ecx, -1                                  "\
    "    shl   ebx, 16                                  "\
    "    inc   ecx                                      "\
    "    mov   edx, [dudxfrac]                          "\    
    "    add   ebx, [dvdxfrac]                          "\
    "    sbb   ebp, ebp                                 "\
    " next:                                             "\
    "    add   eax, edx                                 "\
    "    mov   bl, [esi]                                "\
    "    adc   esi, [duvdxstep+4+ebp*4]                 "\
    "    add   ebx, [dvdxfrac]                          "\
    "    sbb   ebp, ebp                                 "\
    "    mov   [edi+ecx], bl                            "\
    "    inc   ecx                                      "\
    "    jnz   next                                     "\
    "    pop   ebp                                      "\
    parm [edi] [ecx] [esi] [eax] [ebx] modify [eax ebx ecx edx esi edi]


static void RightSection(void)
{
    // Walk backwards trough the vertex array

    vertexuv * v2, * v1 = right_vtx;
    if(right_vtx > start_vtx) v2 = right_vtx-1;     
    else                      v2 = end_vtx;         // Wrap to end of array
    right_vtx = v2;

    // v1 = top vertex
    // v2 = bottom vertex 

    // Calculate number of scanlines in this section

    right_height = ceil(v2->y) - ceil(v1->y);
    if(right_height <= 0) return;

    // Guard against possible div overflows

    if(right_height > 1) {
        // OK, no worries, we have a section that is at least
        // one pixel high. Calculate slope as usual.

        long height = v2->y - v1->y;
        right_dxdy  = idiv16(v2->x - v1->x, height);
    }
    else {
        // Height is less or equal to one pixel.
        // Calculate slope = width * 1/height
        // using 18:14 bit precision to avoid overflows.

        long inv_height = (0x10000 << 14) / (v2->y - v1->y);  
        right_dxdy = imul14(v2->x - v1->x, inv_height);
    }

    // Prestep initial values

    long prestep = (ceil(v1->y) << 16) - v1->y;
    right_x = v1->x + imul16(prestep, right_dxdy);
}

static void LeftSection(void)
{
    // Walk forward trough the vertex array

    vertexuv * v2, * v1 = left_vtx;
    if(left_vtx < end_vtx) v2 = left_vtx+1;
    else                   v2 = start_vtx;      // Wrap to start of array
    left_vtx = v2;

    // v1 = top vertex
    // v2 = bottom vertex 

    // Calculate number of scanlines in this section

    left_height = ceil(v2->y) - ceil(v1->y);
    if(left_height <= 0) return;

    // Guard against possible div overflows

    if(left_height > 1) {
        // OK, no worries, we have a section that is at least
        // one pixel high. Calculate slope as usual.

        long height = v2->y - v1->y;
        left_dxdy = idiv16(v2->x - v1->x, height);
        left_dudy = idiv16(v2->u - v1->u, height);
        left_dvdy = idiv16(v2->v - v1->v, height);
    }
    else {
        // Height is less or equal to one pixel.
        // Calculate slope = width * 1/height
        // using 18:14 bit precision to avoid overflows.

        long inv_height = (0x10000 << 14) / (v2->y - v1->y);
        left_dxdy = imul14(v2->x - v1->x, inv_height);
        left_dudy = imul14(v2->u - v1->u, inv_height);
        left_dvdy = imul14(v2->v - v1->v, inv_height);
    }

    // Prestep initial values

    long prestep = (ceil(v1->y) << 16) - v1->y;
    left_x = v1->x + imul16(prestep, left_dxdy);
    left_u = v1->u + imul16(prestep, left_dudy);
    left_v = v1->v + imul16(prestep, left_dvdy);
}


void DrawArbitraryTexturePoly(vertexuv * vtx, int vertices,
                              char * bitmap, int bitmapwidth,
                              long dudx, long dvdx)
{
    start_vtx = vtx;        // First vertex in array

    // Search trough the vtx array to find min y, max y
    // and the location of these structures.

    vertexuv * min_vtx = vtx;
    max_vtx = vtx;

    long min_y = vtx->y;
    long max_y = vtx->y;

    vtx++;

    for(int n=1; n<vertices; n++) {
        if(vtx->y < min_y) {
            min_y = vtx->y;
            min_vtx = vtx;
        }
        else
        if(vtx->y > max_y) {
            max_y = vtx->y;
            max_vtx = vtx;
        }
        vtx++;
    }

    // OK, now we know where in the array we should start and
    // where to end while scanning the edges of the polygon

    left_vtx  = min_vtx;    // Left side starting vertex
    right_vtx = min_vtx;    // Right side starting vertex
    end_vtx   = vtx-1;      // Last vertex in array

    // Search for the first usable right section

    do {
        if(right_vtx == max_vtx) return;
        RightSection();
    } while(right_height <= 0);

    // Search for the first usable left section

    do {
        if(left_vtx == max_vtx) return;
        LeftSection();
    } while(left_height <= 0);

    char * destptr = WritePagePtr + ceil(min_y) * WritePageWidth;

    dudxfrac = dudx << 16;      // Fractional part in high 16 bits
    dvdxfrac = dvdx << 16;      // clears low 16 bits also
    duvdxstep[1] = (dudx >> 16) + (dvdx >> 16) * bitmapwidth;   // U+V step
    duvdxstep[0] = duvdxstep[1] + bitmapwidth;                  // + carry

    for(;;)
    {
        long x1 = ceil(left_x);
        long width = ceil(right_x) - x1;

        if(width > 0) {

            // Prestep initial texture u,v
    
            long prestep = (x1 << 16) - left_x;
            long u = left_u + imul16(prestep, dudx);
            long v = left_v + imul16(prestep, dvdx);

            // Calc initial source pointer

            char * sourceptr = bitmap + (u >> 16) + (v >> 16) * bitmapwidth;

            inner(destptr+x1, width, sourceptr, u, v);
        }

        destptr += WritePageWidth;

        // Scan the right side

        if(--right_height <= 0) {               // End of this section?
            do {
                if(right_vtx == max_vtx) return;
                RightSection();
            } while(right_height <= 0);
        }
        else 
            right_x += right_dxdy;

        // Scan the left side

        if(--left_height <= 0) {                // End of this section?
            do {
                if(left_vtx == max_vtx) return;
                LeftSection();
            } while(left_height <= 0);
        }
        else {
            left_x += left_dxdy;
            left_u += left_dudy;
            left_v += left_dvdy;
        }
    }
}
