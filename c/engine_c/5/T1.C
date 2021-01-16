void main(void)
 {
  int i,j;
  float m[4][4];
  float *mptr;

  mptr = &(m[0][0]);
  j = 0;
  for (i=0;i<16;i++)
  {
   *mptr++ = j++;
  }

  printf("%f\n",m[0][2]);

 }


