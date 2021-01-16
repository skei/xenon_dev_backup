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

// --------------------------------------------------------------------------

//  Inits the soundsystem and prepares the module for playing.
// -----
//  mxmmem       : Pointer to the module. You must load the complete module
//                 from disk and pass the address to this function.
//  gusport      : Port of the GUS or 0 for no sound.
//  mxmdata      : Pointer to 16k of memory for internal use.
//  maxtimerrate : The maximum number of clock ticks (1193046 ticks/sec)
//                 between two irq 0 calls. Never set this to more than 65536.
//                 Set this to 65536 if you want to have a periodically
//                 called routine. Install your interrupt handler after
//                 starting to play and deinstall before stopping to play.
//                 The interval between two subsequent calls to the interrupt
//                 is no longer than this value, but will vary while runtime.
//  getsetintmode: determines which get/set irq method is to be used:
//                 0: dpmi, works with DOS32, PMODE/W and other good dpmi
//                    servers
//                 1: dos, works with DOS4G and PMODE/W
//                 2: eos, works with EOS
//                 use defines MXMINTMODE...
// returnvalue: 0: failed (at the moment only if the module signature is
//                 wrong)
//              1: ok
int xmpInit(void *mxmmem, unsigned long port, void *mxmdata, unsigned long maxtimerrate, int getsetintmode);
  #pragma aux xmpInit parm [esi] [eax] [ebx] [ecx] [edx] value [eax] modify [eax ebx ecx edx esi edi]

// --------------------------------------------------------------------------

//  Enables the timer services and starts to play the module.
// -----
//  startord: start playing at this order
void xmpPlay(unsigned int startord);
  #pragma aux xmpPlay parm [eax] modify [eax ebx ecx edx esi edi]

// --------------------------------------------------------------------------

//  Stop the sound output and disables the timer services.
void xmpStop();
  #pragma aux xmpStop modify [eax ebx ecx edx esi edi]

// --------------------------------------------------------------------------

//  Set the relative volume.
// -----
//  vol: range 0 to 64.
void xmpSetVolume(unsigned char vol);
  #pragma aux xmpSetVolume parm [eax]

// --------------------------------------------------------------------------

//  Returns the currently set syncvalue. You set the syncvalue with the
//  command EFx or Sxx.
// -----
//  returnvalue: the syncvalue
unsigned char xmpGetSync();
  #pragma aux xmpGetSync value [al]

// --------------------------------------------------------------------------

//  Returns the current position in the song.
// -----
//  returnvalue: highbyte : current order
//               lowbyte  : current row
unsigned short xmpGetPos();
  #pragma aux xmpGetPos value [ax]

// --------------------------------------------------------------------------

//  Returns the timer value. This values starts at the first tick of the
//  module and counts up all the time at a rate of 1193046Hz.
// -----
//  returns: the number of timer ticks since the first tick of the module.
long xmpGetTimer();
  #pragma aux xmpGetTimer value [eax]

// --------------------------------------------------------------------------

//  Gets a near pointer to the environment segment from the PSP selector.
// -----
//  psp: PSP selector
//  returns: pointer to the environment or 0 if failed
void *xmpGetEnvPtrDPMI(unsigned long psp);
  #pragma aux xmpGetEnvPtrDPMI value [edi] parm [eax] modify [eax ebx ecx edx edi]

// --------------------------------------------------------------------------

//  Gets a near pointer to the environment segment under DOS32 extender.
// -----
//  returns: pointer to the environment
void *xmpGetEnvPtrDOS32();
  #pragma aux xmpGetEnvPtrDOS32 value [edi] modify [eax ebx ecx edx esi edi]

// --------------------------------------------------------------------------

//  Reads the port of the GUS from the environment, or 0 if none is found.
// -----
//  envptr: pointer to the environment segment. if 0 the function returns 0.
//  returns: gus port
unsigned long xmpGetGUSPort(void *envseg);
  #pragma aux xmpGetGUSPort value [eax] parm [edi] modify [eax ecx edi]

// --------------------------------------------------------------------------

#ifdef __cplusplus
};
#endif

#endif
