
/*
 *      Gouraud filler
 *
 *      This source is part of the fatmap2.txt document by
 *      Mats Byggmastar, mri@penti.sit.fi
 *      17.4.1997 Jakobstad, Finland
 *
 *      Companies with self respect are encouraged to contact me if
 *      any of this code is to be used as part of a commercial product.
 */


#include "misc.h"

static vertexi * max_vtx;                   // Max y vertex (ending vertex)
static vertexi * start_vtx, * end_vtx;      // First and last vertex in array
static vertexi * right_vtx, * left_vtx;     // Current right and left vertex

static long right_height, left_height;
static long right_x, right_dxdy, left_x, left_dxdy;
static long left_i, left_didy;
static long didx_frac, didx_whole;

void inner(void * dst, int width, long i);
#pragma aux inner = \
    "    rol   ebx, 16                                  "\
    "    mov   edx, [didx_frac]                         "\
    "    mov   al, bl                                   "\
    "    mov   ah, byte ptr [didx_whole]                "\
    " next:                                             "\
    "    mov   [edi], al                                "\
    "    add   ebx, edx                                 "\
    "    adc   al, ah                                   "\
    "    inc   edi                                      "\
    "    dec   ecx                                      "\
    "    jnz   next                                     "\
    parm [edi] [ecx] [ebx] modify [eax ebx ecx edx edi]


static void RightSection(void)
{
    // Walk backwards trough the vertex array

    vertexi * v2, * v1 = right_vtx;
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

    vertexi * v2, * v1 = left_vtx;
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
        left_didy = idiv16(v2->i - v1->i, height);
    }
    else {
        // Height is less or equal to one pixel.
        // Calculate slope = width * 1/height
        // using 18:14 bit precision to avoid overflows.

        long inv_height = (0x10000 << 14) / (v2->y - v1->y);
        left_dxdy = imul14(v2->x - v1->x, inv_height);
        left_didy = imul14(v2->i - v1->i, inv_height);
    }

    // Prestep initial values

    long prestep = (ceil(v1->y) << 16) - v1->y;
    left_x = v1->x + imul16(prestep, left_dxdy);
    left_i = v1->i + imul16(prestep, left_didy);
}


void DrawGouraudPoly(vertexi * vtx, int vertices, long didx)
{
    start_vtx = vtx;        // First vertex in array

    // Search trough the vtx array to find min y, max y
    // and the location of these structures.

    vertexi * min_vtx = vtx;
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

    didx_frac  = didx << 16;
    didx_whole = didx >> 16;

    for(;;)
    {
        long x1 = ceil(left_x);
        long width = ceil(right_x) - x1;

        if(width > 0) {

            // Prestep initial color intensity i
    
            long prestep = (x1 << 16) - left_x;
            long i = left_i + imul16(prestep, didx);

            inner(destptr+x1, width, i);
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
            left_i += left_didy;
        }
    }
}
