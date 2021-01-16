#ifndef textri_h_included
#define textri_h_included

extern void textri (int,int,int);
#pragma aux textri parm [eax] [ebx] [ecx] modify [eax ebx ecx edx esi edi];

#endif
