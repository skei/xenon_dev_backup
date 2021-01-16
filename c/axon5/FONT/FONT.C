#include <stdio.h>    // fopen,fread,fclose

#define fontwidth 24
#define fontheight 20
#define spacewidth 7

#define widthvalue 254
#define transvalue 255

unsigned char *font_offset[256];
unsigned char *font_data;

// -----

// calculates offset for each char
// p -> string containing characters in order of appearance in font data
// f -> font data
void calc_font_pointers(unsigned char *p, unsigned char *f)
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

// draw one char
void drawchar(int x1, int y1, unsigned char c)
{
 int x,y;
 char *p;
 unsigned char *s;
 unsigned char *ss;

 if (c==' ') return;
 s = (unsigned char *)(0xA0000+y1*320+x1);
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

// returns width of character
int charwidth(unsigned char c)
{
 int w;
 unsigned char *p;


 if (c==' ') return(spacewidth);
 w = 0;
 p = font_offset[c];
 while (*p != widthvalue)
 {
  p++;
  w++;
 }
 w++;
 return(w);
}

// -----

void drawstring(int x1, int y1, char *s)
{
 int i;

 i=0;
 while (s[i] != 0)
 {
  drawchar(x1,y1,s[i]);
  x1+=charwidth(s[i]);
  i++;
 }
}

// -----

// set video mode
void SetMode(unsigned short int);
#pragma aux SetMode =\
 "int 10h",\
 parm [ax] modify [ax];

// -----

void setrgb (char,char,char,char);
#pragma aux setrgb=\
 "mov dx,3c8h",\
 "out dx,al",\
 "inc dx",\
 "mov al,ah",\
 "out dx,al",\
 "mov al,bl",\
 "out dx,al",\
 "mov al,bh",\
 "out dx,al",\
 parm [al] [ah] [bl] [bh] modify [edx];

// -----

void main(void)
{
 FILE *fp;
 int size;
 unsigned char s[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890,.'!\0";

 size = 65536;
 font_data = (unsigned char *)malloc(size);
 fp = fopen("font.raw","rb");
 fread(font_data,1,size,fp);
 fclose(fp);
 calc_font_pointers(s,font_data);
 SetMode(0x13);

 drawstring(10,10,"This is a test.'");
 drawstring(10,30,"yz1234567890,.'!");

 getch();
 SetMode(3);

}

