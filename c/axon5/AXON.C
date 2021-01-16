#include <stdio.h>
#include <dos.h>

// --------------------------------------------------------------------------

// square root approximation
// with Newton Raphson with iteration: a(n+1)=a(n)-(a(n)^2-b)/2a(n)
int Sqrt(unsigned int a)
{
  unsigned int b=255,i;
  for (i=0;i<7;i++) b=(a/b+b)>>1;
  return(b);
}

// --------------------------------------------------------------------------

unsigned int AsmAbs(int x);
#pragma aux AsmAbs=\
 " cdq",\
 " xor eax,edx",\
 " sub eax,edx",\
 parm [eax] value [eax] modify exact [edx];

// --------------------------------------------------------------------------

void SetMode(unsigned short int vmode)
{
 union REGS reg;

 reg.w.ax = vmode;
 int386(0x10,&reg,&reg);
}

// --------------------------------------------------------------------------

void draw_pixel(int x,int y,char color,char *sptr)
{
  sptr += (320*y)+x;
  *sptr = color;
}

// --------------------------------------------------------------------------

void draw_pixel_clip(int x,int y,char color,char *sptr)
{
 if (((x>=0) && (x<320)) && ((y>=0) && (y<200))) sptr[320*y+x] = color;
}

// --------------------------------------------------------------------------

void set_rgb(char col,char red,char green,char blue)
{
 outportb(0x3c8,col);
 outportb(0x3c9,red);
 outportb(0x3c9,green);
 outportb(0x3c9,blue);
}

// --------------------------------------------------------------------------

void set_palette(char *pal)
{
 int i;

 for (i=0; i<256; i++) set_rgb(i,pal[i*3+0],pal[i*3+1],pal[i*3+2]);
}

// --------------------------------------------------------------------------

// Vaits for Vertical retrace to START
void waitvr (void);
#pragma aux waitvr =\
 " mov dx,3dah",\
 "wait1:",\
 " in al,dx",\
 " test al,8",\
 " jnz wait1",\
 "wait2:",\
 " in al,dx",\
 " test al,8",\
 " jz wait2",\
 modify [al dx];

// --------------------------------------------------------------------------

void wait_key(void)
{
 union REGS reg;

 reg.w.ax = 0;
 int386(0x16,&reg,&reg);
}

// --------------------------------------------------------------------------

void load_pcx(char *filename, char *sptr, char *pal)
{
  FILE   *fp;
  char   *segm,*saveptr;
  fpos_t position;
  unsigned char   verdi,tegn,antall;
  unsigned short int addr;
  int    i;

  if ((fp = fopen(filename,"rb")) == NULL)
    return;

  fseek(fp,0L,SEEK_END);
  fgetpos(fp,&position);
  fseek(fp,128L,SEEK_SET);

  if ((segm = (char *)malloc(position)) == NULL)
  {
    fclose(fp);
    return;
  }
  saveptr = segm;
  fread((char *)segm,position,1,fp);

  addr = 0;
  for (;;)
  {
    tegn = *(segm++);
    if ((tegn&0xC0) == 0xC0)
    {
      antall =  tegn & 0x3F;
      tegn = *(segm++);
      for (i=0; i<antall; i++)
      {
        if (addr > 63999)
          break;
        sptr[addr++] = tegn;
      }
    } else {
      if (addr > 63999)
        break;
      sptr[addr++] = tegn;
    }
    if (addr > 63999)
      break;
  }
  fseek(fp,position-768,SEEK_SET);
  fread(pal,768,1,fp);
  for (i=0; i<768; i++)
    pal[i] = pal[i] >> 2;
  close(fp);

  fclose(fp);
  segm = saveptr;
  free(segm);
}

