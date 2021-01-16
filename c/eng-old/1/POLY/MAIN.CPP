
/*
 *      Simple test program
 *
 *      This source is part of the fatmap2.txt document by
 *      Mats Byggmastar, mri@penti.sit.fi
 *      17.4.1997 Jakobstad, Finland
 *
 *      Companies with self respect are encouraged to contact me if
 *      any of this code is to be used as part of a commercial product.
 */


#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <time.h>
#include "misc.h"

#define SEED        12
#define SECONDS     3

#define XLIMIT      320
#define YLIMIT      200

#define CLIMIT      256

#define ULIMIT      256
#define VLIMIT      256

char * WritePagePtr = (char *) 0xa0000;
int    WritePageWidth = 320;


void setmode(int mode);
#pragma aux setmode = "int 10h" parm [eax]

inline int backface(vertexuvfloat * vtx)
{
    return (vtx[0].x - vtx[1].x) * (vtx[2].y - vtx[1].y) -
           (vtx[2].x - vtx[1].x) * (vtx[0].y - vtx[1].y) < 0.0;
}

void readbuffer(char * buf, int bytes)
{
    // Fill cache by reading each byte
    volatile char p=0;
    for(int n=0; n<bytes; n++)
        p += buf[n];
}


int main(int, char *[])
{
    vertexuvfloat vtx[3];
    int color=1;

    // Make two 64k aligned texture maps

    static char bigbuffer[256*256*3];
    char * mapa = (char *)(((int)(bigbuffer+0xffff)) & ~0xffff); 
    char * mapb = mapa + 256*256;

    for(int y=0; y<256; y++) {
        for(int x=0; x<256; x++) {
            mapa[x+y*256] = x^y;            // Make a simple XOR pattern
            // swap bits for tiled mapper
            int dst = ((x<<8) & 0xf800) + (x & 0x0007) + ((y<<3) & 0x07f8);
            mapb[dst] = x^y;
        }
    }

    TopClip   =  20.0;
    BotClip   = 180.0;
    LeftClip  =  10.0;
    RightClip = 310.0;

    setmode(0x13);
    printf("Press any key to continue\n");

    printf("flat ");
    fflush(stdout);

    srand(SEED);
    while(!kbhit()) {
        vtx[0].x = rand() % XLIMIT;
        vtx[0].y = rand() % YLIMIT;
        rand();
        rand();
        vtx[1].x = rand() % XLIMIT;
        vtx[1].y = rand() % YLIMIT;
        rand();
        rand();
        vtx[2].x = rand() % XLIMIT;
        vtx[2].y = rand() % YLIMIT;
        rand();
        rand();
		if(!backface(vtx))
            DrawFlatTriangle(vtx, color += 129);
	}
	getch();
	getch();

    printf("gouraud ");
    fflush(stdout);

    srand(SEED);
    while(!kbhit()) {
        vtx[0].x = rand() % XLIMIT;
        vtx[0].y = rand() % YLIMIT;
        vtx[0].u = rand() % CLIMIT;
        rand();
        vtx[1].x = rand() % XLIMIT;
        vtx[1].y = rand() % YLIMIT;
        vtx[1].u = rand() % CLIMIT;
        rand();
        vtx[2].x = rand() % XLIMIT;
        vtx[2].y = rand() % YLIMIT;
        vtx[2].u = rand() % CLIMIT;
        rand();

        if(!backface(vtx))
            DrawGouraudTriangle(vtx);
    }
    getch();
    getch();

    printf("texture ");
    fflush(stdout);

    srand(SEED);
    while(!kbhit()) {
        vtx[0].x = rand() % XLIMIT;
        vtx[0].y = rand() % YLIMIT;
        vtx[0].u = rand() % ULIMIT;
        vtx[0].v = rand() % VLIMIT;
        vtx[1].x = rand() % XLIMIT;
        vtx[1].y = rand() % YLIMIT;
        vtx[1].u = rand() % ULIMIT;
        vtx[1].v = rand() % VLIMIT;
        vtx[2].x = rand() % XLIMIT;
        vtx[2].y = rand() % YLIMIT;
        vtx[2].u = rand() % ULIMIT;
        vtx[2].v = rand() % VLIMIT;
        if(!backface(vtx))
            DrawTextureTriangle(vtx, mapa);
    }
    getch();
    getch();

    printf("arbitrary ");
    fflush(stdout);

    srand(SEED);
    while(!kbhit()) {
        vtx[0].x = rand() % XLIMIT;
        vtx[0].y = rand() % YLIMIT;
        vtx[0].u = rand() % ULIMIT;
        vtx[0].v = rand() % VLIMIT;
        vtx[1].x = rand() % XLIMIT;
        vtx[1].y = rand() % YLIMIT;
        vtx[1].u = rand() % ULIMIT;
        vtx[1].v = rand() % VLIMIT;
        vtx[2].x = rand() % XLIMIT;
        vtx[2].y = rand() % YLIMIT;
        vtx[2].u = rand() % ULIMIT;
        vtx[2].v = rand() % VLIMIT;
		if(!backface(vtx))
            DrawArbitraryTextureTriangle(vtx, mapa, 256);
	}
	getch();
	getch();

    printf("tiled ");
    fflush(stdout);

    srand(SEED);
    while(!kbhit()) {
        vtx[0].x = rand() % XLIMIT;
        vtx[0].y = rand() % YLIMIT;
        vtx[0].u = rand() % ULIMIT;
        vtx[0].v = rand() % VLIMIT;
        vtx[1].x = rand() % XLIMIT;
        vtx[1].y = rand() % YLIMIT;
        vtx[1].u = rand() % ULIMIT;
        vtx[1].v = rand() % VLIMIT;
        vtx[2].x = rand() % XLIMIT;
        vtx[2].y = rand() % YLIMIT;
        vtx[2].u = rand() % ULIMIT;
        vtx[2].v = rand() % VLIMIT;
		if(!backface(vtx))
			DrawTiledTextureTriangle(vtx, mapb);
	}
	getch();
	getch();

 

    printf("\nTiming... wait %d seconds!\n", SECONDS*5);

    time_t t, ct;
    long flat = 0;
    long gouraud=0;
    long texture=0;
    long arbitrarytexture=0;
    long tiledtexture=0;
    static char screen[320*200];

    WritePagePtr = screen;      // Draw to system RAM

    srand(SEED);
    readbuffer(screen, 320*200);
    t = time(0);
    while(t == (ct = time(0)));     // Wait for the next second to start
    t = ct + SECONDS;
	while(time(0) < t) {
        vtx[0].x = rand() % XLIMIT;
        vtx[0].y = rand() % YLIMIT;
        rand();
        rand();
        vtx[1].x = rand() % XLIMIT;
        vtx[1].y = rand() % YLIMIT;
        rand();
        rand();
        vtx[2].x = rand() % XLIMIT;
        vtx[2].y = rand() % YLIMIT;
        rand();
        rand();
		if(!backface(vtx)) {
            DrawFlatTriangle(vtx, color += 129);
			flat++;
		}
	}

    srand(SEED);
    readbuffer(screen, 320*200);
	t = time(0);
    while(t == (ct = time(0)));     // Wait for the next second to start
    t = ct + SECONDS;
	while(time(0) < t) {
        vtx[0].x = rand() % XLIMIT;
        vtx[0].y = rand() % YLIMIT;
        vtx[0].u = rand() % CLIMIT;
        rand();
        vtx[1].x = rand() % XLIMIT;
        vtx[1].y = rand() % YLIMIT;
        vtx[1].u = rand() % CLIMIT;
        rand();
        vtx[2].x = rand() % XLIMIT;
        vtx[2].y = rand() % YLIMIT;
        vtx[2].u = rand() % CLIMIT;
        rand();
		if(!backface(vtx)) {
            DrawGouraudTriangle(vtx);
            gouraud++;
		}
	}


    srand(SEED);
    readbuffer(screen, 320*200);
    readbuffer(mapa, 256*256);
	t = time(0);
    while(t == (ct = time(0)));     // Wait for the next second to start
    t = ct + SECONDS;
	while(time(0) < t) {
        vtx[0].x = rand() % XLIMIT;
        vtx[0].y = rand() % YLIMIT;
        vtx[0].u = rand() % ULIMIT;
        vtx[0].v = rand() % VLIMIT;
        vtx[1].x = rand() % XLIMIT;
        vtx[1].y = rand() % YLIMIT;
        vtx[1].u = rand() % ULIMIT;
        vtx[1].v = rand() % VLIMIT;
        vtx[2].x = rand() % XLIMIT;
        vtx[2].y = rand() % YLIMIT;
        vtx[2].u = rand() % ULIMIT;
        vtx[2].v = rand() % VLIMIT;
		if(!backface(vtx)) {
			DrawTextureTriangle(vtx, mapa);
			texture++;
		}
	}

    srand(SEED);
    readbuffer(screen, 320*200);
    readbuffer(mapa, 256*256);
	t = time(0);
    while(t == (ct = time(0)));     // Wait for the next second to start
    t = ct + SECONDS;
	while(time(0) < t) {
        vtx[0].x = rand() % XLIMIT;
        vtx[0].y = rand() % YLIMIT;
        vtx[0].u = rand() % ULIMIT;
        vtx[0].v = rand() % VLIMIT;
        vtx[1].x = rand() % XLIMIT;
        vtx[1].y = rand() % YLIMIT;
        vtx[1].u = rand() % ULIMIT;
        vtx[1].v = rand() % VLIMIT;
        vtx[2].x = rand() % XLIMIT;
        vtx[2].y = rand() % YLIMIT;
        vtx[2].u = rand() % ULIMIT;
        vtx[2].v = rand() % VLIMIT;
		if(!backface(vtx)) {
            DrawArbitraryTextureTriangle(vtx, mapa, 256);
            arbitrarytexture++;
		}
	}

    srand(SEED);
    readbuffer(screen, 320*200);
    readbuffer(mapb, 256*256);
	t = time(0);
    while(t == (ct = time(0)));     // Wait for the next second to start
    t = ct + SECONDS;
	while(time(0) < t) {
        vtx[0].x = rand() % XLIMIT;
        vtx[0].y = rand() % YLIMIT;
        vtx[0].u = rand() % ULIMIT;
        vtx[0].v = rand() % VLIMIT;
        vtx[1].x = rand() % XLIMIT;
        vtx[1].y = rand() % YLIMIT;
        vtx[1].u = rand() % ULIMIT;
        vtx[1].v = rand() % VLIMIT;
        vtx[2].x = rand() % XLIMIT;
        vtx[2].y = rand() % YLIMIT;
        vtx[2].u = rand() % ULIMIT;
        vtx[2].v = rand() % VLIMIT;
		if(!backface(vtx)) {
			DrawTiledTextureTriangle(vtx, mapb);
			tiledtexture++;
		}
	}

	setmode(3);

    printf("Flat polys/sec              %ld\n", flat/SECONDS);
    printf("Gouraud polys/sec           %ld\n", gouraud/SECONDS);
    printf("Texture polys/sec           %ld\n", texture/SECONDS);
    printf("Arbitrary texture polys/sec %ld\n", arbitrarytexture/SECONDS);
    printf("Tiled texture polys/sec     %ld\n", tiledtexture/SECONDS);

	return 0;
}
