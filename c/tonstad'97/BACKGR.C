/* CALCULATE BACKGROUND */
void CalcBackground(char *buffer, int addcolor)
{
 unsigned short int from,to;
 int counter1,counter2;
 char data;

 from=0;
 to=0;
 for (counter1=0;counter1<10;counter1++)
 {
  for (counter2=0;counter2<65536;counter2++)
  {
   data = buffer[from+=2];      /* screen */
   if ((data&1)==1) (to+=321); else (to+=320);
   buffer[to]++;                /* screen */
  }
 }
 for (counter1=0;counter1<65536;counter1++)
 {
  buffer[counter1] += addcolor;
 }
}

// --------------------------------------------------------------------------

/* SMOOTH BACKGROUND */
void smooth(char *buffer)
{
 unsigned short int from;
 char data;

 for (from=1;from<318;from++)
 {
  buffer[from] = (buffer[from-1]+buffer[from+1]) >> 1;
 }
 for (from=320;from<64000;from++)
 {
  buffer[from] = (buffer[from-320]+buffer[from-1]+buffer[from+1]+buffer[from+320]) >> 2;
 }
}

