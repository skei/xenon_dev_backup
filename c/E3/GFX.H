#ifndef gfx_h_included
#define gfx_h_included
// ��������������������������������������������������������������������������

void SetMode(unsigned short int);
#pragma aux SetMode =\
 "int 10h",\
 parm [ax] modify [ax];

// ��������������������������������������������������������������������������
#endif