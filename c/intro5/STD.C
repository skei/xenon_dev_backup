#include "std.h"

// --------------------------------------------------------------------------

short int RandSeed;

// --------------------------------------------------------------------------

void Int2Hex(unsigned int h, char *S)
 {
  const char HexTable[16] = "0123456789ABCDEF";

  S[0] = HexTable[(h>>28)&15];
  S[1] = HexTable[(h>>24)&15];
  S[2] = HexTable[(h>>20)&15];
  S[3] = HexTable[(h>>16)&15];
  S[4] = HexTable[(h>>12)&15];
  S[5] = HexTable[(h>> 8)&15];
  S[6] = HexTable[(h>> 4)&15];
  S[7] = HexTable[(h    )&15];
 }

 // -------------------------------------------------------------------------

void Word2Hex(unsigned short int h, char *S)
 {
  const char HexTable[16] = "0123456789ABCDEF";

  S[0] = HexTable[(h>>12)&15];
  S[1] = HexTable[(h>> 8)&15];
  S[2] = HexTable[(h>> 4)&15];
  S[3] = HexTable[(h    )&15];
 }

 // -------------------------------------------------------------------------

short int Random(void)
 {
  short int r;

  r = (RandSeed * 0x8405)+1;
  RandSeed = r;
  return r;
 }


