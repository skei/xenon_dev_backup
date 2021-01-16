 // Replacement stuff for the Watcom std libs

 #include "std.h"

//
// --- variables ---
//

const char HexTable[16]     = "0123456789ABCDEF";
unsigned short int RandSeed = 0x1234;

//
// --- functions
//

unsigned short int rand(void)
 {
  unsigned short int r;

  r = (RandSeed * 0x8405)+1;
  RandSeed = r;
  return (r&32767);
 }

 // ---

void DrawDword(unsigned int h)
 {
  char S[9];
  S[0] = HexTable[(h>>28)&15];
  S[1] = HexTable[(h>>24)&15];
  S[2] = HexTable[(h>>20)&15];
  S[3] = HexTable[(h>>16)&15];
  S[4] = HexTable[(h>>12)&15];
  S[5] = HexTable[(h>> 8)&15];
  S[6] = HexTable[(h>> 4)&15];
  S[7] = HexTable[(h    )&15];
  S[8] = '$';
  DrawString(&S);
 }

 // ---

void DrawWord(unsigned short int h)
 {
  char S[5];
  S[0] = HexTable[(h>>12)&15];
  S[1] = HexTable[(h>> 8)&15];
  S[2] = HexTable[(h>> 4)&15];
  S[3] = HexTable[(h    )&15];
  S[4] = '$';
  DrawString(&S);
 }


