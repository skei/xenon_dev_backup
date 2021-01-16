
typedef struct test
{
 unsigned int a : 1;
 unsigned int b : 1;
 unsigned int c : 1;
 unsigned int d : 1;
 unsigned int e : 1;
} test;

void main(void)
{
 test flags;

 flags.a = 1;
 flags.c = 1;
 flags.d = 0;
}
