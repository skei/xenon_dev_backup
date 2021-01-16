#ifndef debug_h_included
#define debug_h_included
// 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴

#include <stdarg.h>

void TRACE ( char* fmt,...)
{
 #ifdef _DEBUG
  char buf[256];
  va_list args;
  va_start(args, fmt);
  vsprintf(buf, fmt, args);
  va_end(args);
  // OutputDebugString( buf );
  printf("%s\n",buf);
 #endif
}

// 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
#endif
