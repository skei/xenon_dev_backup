void main(void)
{
 int a;
 volatile int b;

 a = 1;

 if ((a&&1)==1) b=1;
 if ((a&1)==1) b=2;
 if ((a&&1)==2) b=3;
 if ((a&1)==2) b=4;
}
