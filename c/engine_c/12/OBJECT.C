#include <stdio.h>    // file handling
#include <stdlib.h>   // malloc
#include <string.h>   // memset, memcpy
#include "engine.h"

// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
// ฑ Object handling routines ฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑ
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ

sObject* Object_Init(void)
{
 sObject *newo;
 newo = (sObject *)malloc(sizeof(sObject));
 if (newo!=0)
 {
  memset(newo,0,sizeof(sObject));
 }
 return newo;
}

// -----

sObject* Object_Copy(sObject *o)
{
 sObject *newo;
 newo = (sObject *)malloc(sizeof(sObject));
 if (newo!=0 )
 {
  memcpy(newo,o,sizeof(sObject));
 }
 return newo;
}

// -----

void Object_Set_Rot(sObject *o, int xa, int ya, int za)
{
 o->xang = xa;
 o->yang = ya;
 o->zang = za;
}

// -----

void Object_Set_Pos(sObject *o, int x, int y, int z)
{
 o->xpos = x;
 o->ypos = y;
 o->zpos = z;
}

// -----

void Object_Add_Rot(sObject *o, int xa, int ya, int za)
{
 o->xang += xa;
 o->yang += ya;
 o->zang += za;
}

// -----

void Object_Add_Pos(sObject *o, int x, int y, int z)
{
 o->xpos += x;
 o->ypos += y;
 o->zpos += z;
}

// -----

void Object_Set_Mapping(sObject *o, int t, char *texture)
{
 switch(t)
 {
// Environment mapping
	case 0 : o->rottype = 0;
	         o-> shadetype = 0;
					 o->texmap = texture;
	         break;
// Standard texture mapping
	case 1 : o->rottype = 1;
	         o-> shadetype = 1;
					 o->texmap = texture;
	         break;
 }
}

char* Load_Texture(char *f)
{
 char *r;
 FILE *fp;

 r = malloc(65536);
 fp = fopen (f, "rb");
 fread (r, 1, 65535, fp);
 fclose (fp);
 return r;
}

char* Load_Palette(char *f)
{
 char *r;
 FILE *fp;

 r = malloc(768);
 fp = fopen (f, "rb");
 fread (r, 1, 768, fp);
 fclose (fp);
 return r;
}

