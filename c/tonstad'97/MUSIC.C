#include <stdlib.h>
#include "mxmplay.h"

 extern unsigned char Module;
 #pragma aux Module "*"

void *mxm_mem = NULL;

// -----

int Music_Init(void)
{
 mxm_mem = (void *)malloc(16384);
// timer value: 1193046/65535 = 18.2
//              1193046/16383 = 72.8
 if (!xmpInit(&Module,                                // ptr to module
              xmpGetGUSPort(xmpGetEnvPtrDPMI(_psp)),  // gusport
              mxm_mem,                                // ptr to 16k
              16383,                                  // timer stuff (see above)
              1                                       // int/irq mdoe: 0=dpmi, 1=pmode/w,dos4gw, 2=eos
              ))
 {
  return 0;
 }
 else
 {
  return 1;
 }
}

// --------------------------------------------------------------------------

void Music_DeInit(void)
{
 if (mxm_mem != NULL) free(mxm_mem);
}

// --------------------------------------------------------------------------

void Music_Start(void)
{
 xmpPlay(0);    // start at order 0
}

// --------------------------------------------------------------------------

void Music_Stop(void)
{
 xmpStop();
}

// --------------------------------------------------------------------------

// v = volume (0..64)
void Music_SetVolume(unsigned char v)
{
 xmpSetVolume(v);
}

// --------------------------------------------------------------------------

// returns sync value (effect EFx or Sxx)
unsigned char Music_GetSync(void)
{
 return( xmpGetSync() );
}

// --------------------------------------------------------------------------

// returns current playing pos... (highbyte: order, lowbyte: row)
unsigned short int Music_GetPos(void)
{
 return (xmpGetPos());
}

// --------------------------------------------------------------------------

// returns current playing order
unsigned char Music_GetOrder(void)
{
 return (xmpGetPos() >> 8);
}

// --------------------------------------------------------------------------

// returns current playing row
unsigned char Music_GetRow(void)
{
 return (xmpGetPos() & 0xff);
}

// --------------------------------------------------------------------------

// returns number of ticks since module start. (1193046Hz)
int Music_GetTimer(void)
{
 return ( xmpGetTimer() );
}

