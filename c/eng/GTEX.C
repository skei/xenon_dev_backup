

typedef struct
{
 int x,y;
 int u,v;
 int c;
} sVertex;

int ScanBuffer[200*5];

unsigned char *Texture;
unsigned char *ShadeTable;
unsigned char *Buffer;

// -----

void GTfiller(int y1, int y2, int uadd, int vadd, int cadd)
{
 int height,width;
 int x,u,v,c;
 int i,j;
 int pos;

 pos = y1*5;
 for (i=0;i<(y2-y1);i++)
 {
  x = ScanBuffer[pos];
  width = ScanBuffer[pos+1] - x;
  u = ScanBuffer[pos+2];
  v = ScanBuffer[pos+3];
  c = ScanBuffer[pos+4];
  for (j=0;j<width;j++)
  {
   Buffer[x] = ShadeTable[((c >> 8)&0x3f)+(Texture[(v & 0xff00)+((u >> 8)&0xff)])];
   x++;
   u+=uadd;
   v+=vadd;
   c+=cadd;
  }
  pos+=5;
 }
}

// -----

