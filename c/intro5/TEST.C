#include "std.h"

void MAIN(void)
{
 REGS r;

 r.w.ax = 0x13;
 intVideo(&r);

 *(char *)0xA0000 = 15;
 *(char *)0xA0001 = 1;

 GetKey();

 SetMode(3);

}
