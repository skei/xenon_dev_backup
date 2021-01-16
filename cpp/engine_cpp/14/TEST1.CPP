#include <conio.h>

#include "map.hpp"
#include "vector.hpp"
#include "matrix.hpp"

void SetMode(int mode);
#pragma aux SetMode=\
 " int 0x10",\
 parm [eax];

 // ---

void main(void)
{
 int i;
 char *b;

 c_VECTOR v1,v2;
 c_MATRIX m1,m2;
 c_MAP buffer(320,200,8);
 c_MAP screen(320,200,8,(void *)0xA0000);

 SetMode(0x13);

 buffer.Clear();
 buffer.BlitTo(screen);
 getch();
 b = (char *)buffer.GetPtr();
 for (i=0;i<64000;i++)
 {
  *b++ = (i&255);
 }
 buffer.BlitTo(screen);
 getch();

 SetMode(0x3);

}
