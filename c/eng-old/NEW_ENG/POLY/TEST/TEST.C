void main(void)
{
 int test,qwe;
 int *a[5];
 int *(*b)[];

 test = 3;

 a[2] = &test;
 b = &a;

 qwe = *(*b)[2];
 printf("%i\n",qwe);

}
