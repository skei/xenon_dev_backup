// dosmain.c

// needed: main - calling other main_ routines, copy buffer to screen.

#include <conio.h>
#include <string.h>
#include "defines.h"
#include "types.h"

extern void main_init(void);
extern void main_process(void);
extern void main_exit(void);

screen_S buffer;

// Initialize & allocate buffer, init BitMapInfo
void Buffer_Init(int x_res, int y_res, char *palette)
{
 int i;
 x_res = (x_res/2)*2;	// To fix modulo bug
 buffer.screen = (unsigned char *)malloc(x_res*y_res);
 if(buffer.screen != 0)
 {
  buffer.width = x_res;
  buffer.height = y_res;
 }
 else
 {
  printf("ERROR: Unable to allocate memory for viewport!\n");
  exit(1);
 }
}

// Free buffer & deallocate memory

void Buffer_CleanUp()
{
 free(buffer.screen);
 buffer.screen=0;
 buffer.width=0;
 buffer.height=0;
}

// -----

void Buffer_Clear(char c)
{
 screen_S *vp = &buffer;
 memset(vp->screen,c,(SCREEN_WIDTH*SCREEN_HEIGHT));
}

// -----

void CopyBuffer(void)
{
 screen_S *vp = &buffer;
 memcpy((char *)0xA0000,vp->screen,SCREEN_WIDTH*SCREEN_HEIGHT);
}


// 北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

void main(void)
{
 main_init();
 while (!kbhit())
 {
  main_process();
  CopyBuffer();
 }
 main_exit();
}
