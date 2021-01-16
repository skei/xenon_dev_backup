#ifndef file_h_included
#define file_h_included
// 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴

#include "engine.h"

 // Load 3DS file into memory...
 // In  : filename : name of 3DS file to load
 //       buffer   : ptr to allocate and load file into
 // Out : size of 3DS file/buffer

void Load3DS( char *filename, sWorld *W );

 // Parse previously loaded 3DS file
 // In  : buffer     : ptr to previously loaded 3DS file
 //       BufferSize : size of buffer / length of 3DS file
 //       World      : World/Scene to load
 // Out : 0=OK, else error code
 //       1=Invalid 3DS file

void Parse3DS( char *buffer, int BufferSize, sWorld *World );


// 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
#endif
