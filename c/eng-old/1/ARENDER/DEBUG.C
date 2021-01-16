#include <stdarg.h>
#include "debug.h"

void DebugPrint ( char* fmt,...)
{
 char buf[256];
 va_list args;
 va_start(args, fmt);
 vsprintf(buf, fmt, args);
 va_end(args);
 // OutputDebugString( buf );
 printf("%s\n",buf);
}

