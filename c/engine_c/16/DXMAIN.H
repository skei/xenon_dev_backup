// dxmain.h

#ifndef dxmain_h_included
#define dxmain_h_included

BOOL Buffer_Init(int x_res, int y_res, char *palette);
void Buffer_CleanUp();
void Buffer_Clear(char c);
WORD gethicolor(char a,char b,char c);

#endif
