#include "bitmap.hpp"

void main(void)
 {
  // ---
  c_BITMAP screen(320,200,8,(void *)0xA0000);
  c_BITMAP buffer(320,200,8);
  // --
  char *b;

  b = (char *)buffer.GetPtr();

  buffer.Clear();
  screen.Clear();
  buffer.BlitTo(screen);
 }
