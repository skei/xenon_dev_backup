#include <string.h>
#include <conio.h>

#include "bitmap.hpp"

void SetMode(unsigned short int mode);
#pragma aux SetMode =\
 "int 10h",\
 parm [ax] modify [ax];

void SetRGB(char c, char r, char g, char b);
#pragma aux SetRGB=\
 "mov dx,3c8h",\
 "out dx,al",\
 "inc dx",\
 "mov al,ah",\
 "out dx,al",\
 "mov al,bl",\
 "out dx,al",\
 "mov al,bh",\
 "out dx,al",\
 parm [al] [ah] [bl] [bh] modify [edx];

// ---

void main(void)
{
 int i;
 char *b;
 c_BITMAP buffer(320,200,8);
 c_BITMAP screen(320,200,8,(void *)0xA0000);

 b = (char *)buffer.GetPtr();
 for (i=0;i<64000;i++)
  b[i] = (i&0xff);

 SetMode(0x13);

 buffer.BlitTo(screen);
 getch();
 buffer.Clear();
 buffer.BlitTo(screen);
 getch();

 SetMode(3);


}
