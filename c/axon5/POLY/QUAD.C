// Quadratic mapping testing

typedef struct sEdge
{
 int x0,x2;
 float t0,t2;
 float z0,z2;
} sEdge;

// --------------------------------------------------------------------------

void Scan(int x0, int y0, int z0, float t0, int x2, int y2, int z2, float t2)
{

}

// --------------------------------------------------------------------------

void HLine(int x0, int z0, float t0, int x2, int z2, float t2)
{
 int i;
 int xd;
 float td;
 float a,b,c,t1;
 float f0,f1,f2,ff;

 xd = x2-x0;
 td = t2-t0;
 a  = td*(z2+z0)/(z0+z2);
 t1 = (t0+t2)*0.5-a/3;
 b  = a/xd;
 ff = td/xd;
 f1 = ff-b;
 f2 = 2*b/(xd-1);

 f0 = t0;

 for (i=x0;i<x2;i++)
 {
  printf("x = %i, f0 = %f, f1 = %f, f1-ff = %f\n",i,f0,f1,f1-ff);
  f0+=f1;
  f1+=f2;
 }
}

// 様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様

void main(void)
{
 HLine(0,0,8,4,32,12);
}
