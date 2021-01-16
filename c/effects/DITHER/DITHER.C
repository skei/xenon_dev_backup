void setmode(int);
#pragma aux setmode=\
 "int 10h",\
 parm [eax];

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

void main()
{
 int i,b,c,color,adder,error;
 printf("Veldig enkelt eksempel p† 'Simple Dithering'..\n");
 printf("Error distribueres kun til neste punkt...\n");
 printf("16 farger....     <Any Key>\n");
 getch();

 setmode(0x13);
 for (i=0;i<16;i++) setrgb(i,i*4,i*4,i*4);
// UnDithered interpolation
 color = 0 << 16;
 adder = ((15-0) << 16 ) / (310-10);
 for (i=10;i<310;i++)
 {
  c = (color) >> 16;
  color+=adder;
  *(char *)(0xA0000 + 10*320 + i) = c;
 }
// Dithered interpolation
 color = 0 << 16;
 adder = ((15-0) << 16 ) / (310-10);
 error = 0;
 for (i=10;i<310;i++)
 {
  b = (color+error);
  c = b >> 16;
  error = b - (c << 16);
  color+=adder;
  *(char *)(0xA0000 + 15*320 + i) = c;
 }
 getch();
 setmode(3);
}
