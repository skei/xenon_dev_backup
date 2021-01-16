// 3DS spline stuff...

 #include <math.h>

typedef struct PosKey
{
 int frame;
 float pos[3];
 float ds[3];
 float dd[3];
 float Cont,Bias,Tens;
 float EaseFrom,EaseTo;
} PosKey;

// -----

// a = EaseFrom of key[n];
// b = EastTo   of key[n+1]
// u = (t-t[n]/(t[n+1]-t[n-1])
float Ease(float u, float a, float b)
{
 float k;
 float s = a+b;
 if (s==0.0) return(u);
 if (s>1.0)
 {
  a = a/s;
  b = b/s;
 }
 k = 1.0/(2.0-a-b);
 if (u<a) return ((k/a)*u*u);
 else if (u<1.0-b) return (k*(2*u-a));
 else
 {
  u = 1.0-u;
  return(1.0-(k/b)*u*u);
 }
}

// -----

void CompElementDeriv( float pp, float p, float pn,
                       float *ds, float *dd,
                       float ksm, float ksp, float kdm, float kdp)
{
 float delm,delp;

 delm = p-pp;
 delp = pn-p;
 *ds = ksm*delm + ksp*delp;
 *dd = kdm*delm + kdp*delp;
}

// -----

void CompDeriv( PosKey *keyp, PosKey *key, PosKey *keyn)
{
 int i;
 // Full TCB computation
 float tm,cm,cp,bm,bp,tmcm,tmcp,ksm,ksp,kdm,kdp,delm,delp,c;
 float dt,fp,fn;

 // fn,fp apply speed correction when continuity is 0.0
 dt = 0.5 * (float)(keyn->frame - keyp->frame);
 fp = ((float)(key->frame - keyp->frame))/dt;
 fn = ((float)(keyn->frame - key->frame))/dt;
 c = fabs(key->Cont);
 fp = fp+c-c*fp;
 fn = fn+c-c*fn;
 cm = 1.0-key->Cont;
 tm = 0.5*(1.0-key->Tens);
 cp = 2.0-cm;
 bm = 1.0-key->Bias;
 bp = 2.0-bm;
 tmcm = tm*cm;   tmcp = tm*cp;
 ksm = tmcm*bp*fp;   ksp = tmcp*bm*fp;
 kdm = tmcp*bp*fn;   kdp = tmcm*bm*fn;

 i = 0;
// for (i=0;i<3;i++)
// {
  CompElementDeriv(keyp->pos[i], key->pos[i], keyn->pos[i],
                   &key->ds[i], &key->dd[i],
                   ksm,ksp,kdm,kdp);
// }
}

// 北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

void main(void)
{
 int n;
 float t;
 float x;
 float FAX,FBX,FCX,FDX;

 PosKey Key[6];

 Key[0].frame = 0;
 Key[0].pos[0] = 300;
 Key[0].Cont = 1;
 Key[0].Bias = 1;
 Key[0].Tens = 1;
 Key[0].EaseFrom = 0;
 Key[0].EaseTo   = 0;

 Key[1].frame = 10;
 Key[1].pos[0] = 300;
 Key[1].Cont = 1;
 Key[1].Bias = 1;
 Key[1].Tens = 1;
 Key[1].EaseFrom = 0;
 Key[1].EaseTo   = 0;

 Key[2].frame = 20;
 Key[2].pos[0] = 400;
 Key[2].Cont = 1;
 Key[2].Bias = 1;
 Key[2].Tens = 1;
 Key[2].EaseFrom = 0;
 Key[2].EaseTo   = 0;

 Key[3].frame = 30;
 Key[3].pos[0] = 500;
 Key[3].Cont = 1;
 Key[3].Bias = 1;
 Key[2].Tens = 1;
 Key[3].EaseFrom = 0;
 Key[3].EaseTo   = 0;

 Key[4].frame = 40;
 Key[4].pos[0] = 500;
 Key[4].Cont = 1;
 Key[4].Bias = 1;
 Key[4].Tens = 1;
 Key[4].EaseFrom = 0;
 Key[4].EaseTo   = 0;

 Key[5].frame = 50;
 Key[5].pos[0] = 500;
 Key[5].Cont = 1;
 Key[5].Bias = 1;
 Key[5].Tens = 1;
 Key[5].EaseFrom = 0;
 Key[5].EaseTo   = 0;

 CompDeriv( &Key[0], &Key[1], &Key[2] );
 CompDeriv( &Key[1], &Key[2], &Key[3] );
 CompDeriv( &Key[2], &Key[3], &Key[4] );
 CompDeriv( &Key[3], &Key[4], &Key[5] );

 for (n=1;n<4;n++)
 {
  FAX = Key[n].ds[0];
  FBX = Key[n].dd[0];
  FCX = Key[n-1].ds[0];
  FDX = Key[n-1].dd[0];
  // These are the coefficients a, b, c, d, in  aT^3 + bT2 + cT + d
  // NOTE: All terms are here, so all types use same code.
  // To conserve calculations, terms with a zero in the basis matrix can be
  // removed
  // FAX = Ft1*IT(1,n-1) + Ft2*IT(1,n) + Ft3*IT(1,n+1) + Ft4*IT(1,n+2)
  // FBX = Fu1*IT(1,n-1) + Fu2*IT(1,n) + Fu3*IT(1,n+1) + Fu4*IT(1,n+2)
  // FCX = Fv1*IT(1,n-1) + Fv2*IT(1,n) + Fv3*IT(1,n+1) + Fv4*IT(1,n+2)
  // FDX = Fw1*IT(1,n-1) + Fw2*IT(1,n) + Fw3*IT(1,n+1) + Fw4*IT(1,n+2)
  for (t=0.0;t<=1.0;t+=0.25)
  {
   x = ((FAX*t + FBX)*t + FCX)*t +FDX;  // interpolated x value (using Horner)
   printf("n:%i  t:%5.2f  x:%5.2f\n",n,t,x);
  }
 }
}
