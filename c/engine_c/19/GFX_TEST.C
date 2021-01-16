#include "engine.h"
#include "gfx.h"

unsigned char *texture;
unsigned char p[768];

void mode(int mode);
 #pragma aux mode = \
  "xor ebx,ebx" \
  "xor ecx,ecx" \
  "xor edx,edx" \
  "int 10h" \
 parm [eax] modify [ebx ecx edx];

void main(void)
{
 Load_GIF("title2.gif",texture,p,NULL,NULL);
 mode(0x13);
 memcpy((char *)0xA0000,texture,64000);
 getch();
 mode(3);
 free(texture);
}
