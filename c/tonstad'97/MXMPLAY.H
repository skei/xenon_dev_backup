#ifndef __MXMPLAY_H
#define __MXMPLAY_H

// extern unsigned int _psp;

#define MXMINTMODEDPMI 0
#define MXMINTMODEDOS 1
#define MXMINTMODEEOS 2

#ifdef __cplusplus
extern "C"
{
#endif

int xmpInit(void *mxmmem, unsigned long port, void *mxmdata, unsigned long maxtimerrate, int getsetintmode);
  #pragma aux xmpInit parm [esi] [eax] [ebx] [ecx] [edx] value [eax] modify [eax ebx ecx edx esi edi]
void xmpPlay(unsigned int startord);
  #pragma aux xmpPlay parm [eax] modify [eax ebx ecx edx esi edi]
void xmpStop();
  #pragma aux xmpStop modify [eax ebx ecx edx esi edi]
void xmpSetVolume(unsigned char vol);
  #pragma aux xmpSetVolume parm [eax]
unsigned char xmpGetSync();
  #pragma aux xmpGetSync value [al]
unsigned short xmpGetPos();
  #pragma aux xmpGetPos value [ax]
long xmpGetTimer();
  #pragma aux xmpGetTimer value [eax]
void *xmpGetEnvPtrDPMI(unsigned long psp);
  #pragma aux xmpGetEnvPtrDPMI value [edi] parm [eax] modify [eax ebx ecx edx edi]
void *xmpGetEnvPtrDOS32();
  #pragma aux xmpGetEnvPtrDOS32 value [edi] modify [eax ebx ecx edx esi edi]
unsigned long xmpGetGUSPort(void *envseg);
  #pragma aux xmpGetGUSPort value [eax] parm [edi] modify [eax ecx edi]

#ifdef __cplusplus
};
#endif

#endif
