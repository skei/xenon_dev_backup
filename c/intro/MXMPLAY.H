 #ifndef __MXMPLAY_H
 #define __MXMPLAY_H

 extern unsigned int _psp;

 #define MXMINTMODEDPMI 0
 #define MXMINTMODEDOS 1
 #define MXMINTMODEEOS 2

 #ifdef __cplusplus
 extern "C"
 {
 #endif

int xmpInit(void *mxmmem, unsigned long port, void *mxmdata, unsigned long maxtimerrate, int getsetintmode);
  #pragma aux xmpInit "*_" parm [esi] [eax] [ebx] [ecx] [edx] value [eax] modify [eax ebx ecx edx esi edi]
void xmpPlay(unsigned int startord);
  #pragma aux xmpPlay "*_" parm [eax] modify [eax ebx ecx edx esi edi]
void xmpStop();
  #pragma aux xmpStop "*_" modify [eax ebx ecx edx esi edi]
void xmpSetVolume(unsigned char vol);
  #pragma aux xmpSetVolume "*_" parm [eax]
unsigned char xmpGetSync();
  #pragma aux xmpGetSync "*_" value [al]
unsigned short xmpGetPos();
  #pragma aux xmpGetPos "*_" value [ax]
long xmpGetTimer();
  #pragma aux xmpGetTimer "*_" value [eax]
void *xmpGetEnvPtrDPMI(unsigned long psp);
  #pragma aux xmpGetEnvPtrDPMI "*_" value [edi] parm [eax] modify [eax ebx ecx edx edi]
void *xmpGetEnvPtrDOS32();
  #pragma aux xmpGetEnvPtrDOS32 "*_" value [edi] modify [eax ebx ecx edx esi edi]
unsigned long xmpGetGUSPort(void *envseg);
  #pragma aux xmpGetGUSPort "*_" value [eax] parm [edi] modify [eax ecx edi]

 #ifdef __cplusplus
 };
 #endif

#endif
