// #include <stdio.h>    // fopen,fread,fclose

 #include "font.inc"

 #define fontwidth 19
 #define fontheight 21
 #define spacewidth 7

 #define widthvalue 254
 #define transvalue 255

 unsigned char *font_offset[256];

 // -----

 // calculates offset for each char
 // p -> string containing characters in order of appearance in font data
 // f -> font data
void Font_CalcPointers(unsigned char *p, unsigned char *f)
 {
  int n;
  int pos;

  for (n=0;n<256;n++) font_offset[n] = f;
  n = 0;
  while (p[n] != 0)
  {
   font_offset[p[n]] = f;
   f = (unsigned char *)( (int)f + (fontwidth*fontheight));
   n++;
  }
 }

 // -----

 // returns width of character
int Font_CharWidth(unsigned char c)
 {
  int w;
  unsigned char *p;


  if (c==' ') return(spacewidth);
  w = 0;
  p = font_offset[c];
  while ((*p != widthvalue) && (w<fontwidth))
  {
   p++;
   w++;
  }
  w++;
  return(w);
 }

 // -----

 // returns width of string
int Font_StringWidth( char *s)
 {
  int w,i;
  unsigned char *p;

  w=0;
  for (i=0;i<strlen(s);i++)
  {
   if (s[i]==' ') w += spacewidth;
   w += Font_CharWidth(s[i]);
  }
  return w;
 }

 // -----

 // draw one char
void Font_DrawChar(char *buffer, int x1, int y1, unsigned char c)
 {
  int x,y;
  unsigned char *p;
  unsigned char *s;
  unsigned char *ss;

  if (c==' ') return;
  s = (unsigned char *)((int)buffer+y1*320+x1);
  p = font_offset[c];
  for (y=0;y<fontheight;y++)
  {
   ss = s;
   for (x=0;x<fontwidth;x++)
   {
    c = p[y*fontwidth+x];
    if ((c != widthvalue) && (c != transvalue)) *s = c;
    s++;
   }
   s = (unsigned char *)((int)ss+320);
  }
 }

 // -----

void Font_DrawString(char *buffer,int x1, int y1, char *s)
 {
  int i;

  i=0;
  while (s[i] != 0)
  {
   Font_DrawChar(buffer,x1,y1,s[i]);
   x1+=Font_CharWidth(s[i]);
   i++;
  }
 }

 // -----

void Font_Setup(void)
 {
  unsigned char s[] = "abcdefghijklmnopqrstuvwxyz@1234567890?!-.,";
  Font_CalcPointers(s,Font);
 }
