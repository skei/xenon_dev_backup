#define S 65536.0
#define MAGIC (((S*S*16)+(S*.5))*S)
#define MAGICfixed (MAGIC/65536.0)

// --------------------------------------------------------------------------

#pragma inline float2int;
static long float2int( float d )
{
 double dtemp = MAGIC+d;
 return (*(long *)&dtemp)-0x80000000;
}

// --------------------------------------------------------------------------

#pragma inline float2fixed;
static long float2fixed( float d )
{
 double dtemp = MAGICfixed+d;
 return (*(long *)&dtemp)-0x80000000;
}

// --------------------------------------------------------------------------

#pragma inline CEIL;
static int CEIL(float f)
{
 int i,j;

 i = float2fixed(f);
 j = i >> 16;
 if ((i&0xffff) != 0) j+=1;
 return j;
}

// --------------------------------------------------------------------------

void main(void)
{
 float f;
 int i;

/*
 f = 1.0;
 printf("%f = %i\n",f,CEIL(f));
 f = 1.1;
 printf("%f = %i\n",f,CEIL(f));
 f = 1.9;
 printf("%f = %i\n",f,CEIL(f));
 f = -1.0;
 printf("%f = %i\n",f,CEIL(f));
 f = -1.1;
 printf("%f = %i\n",f,CEIL(f));
 f = -1.9;
 printf("%f = %i\n",f,CEIL(f));
 f = -2.1;
 printf("%f = %i\n",f,CEIL(f));
*/

 f = -127.3;
 printf("float2int(%f) = %i\n",f,float2int(f));
 printf("CEIL(%f) = %i\n",f,CEIL(f));
 f = -1.5;
 printf("float2fixed(%f) = %i\n",f,float2fixed(f));



}
