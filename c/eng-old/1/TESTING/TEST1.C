void main(void)
{
 char *p;

 p = (char *)malloc(0xffffffff);
 printf("P returns : %i\n",(int)p);
}
