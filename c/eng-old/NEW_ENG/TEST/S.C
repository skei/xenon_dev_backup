#include <math.h>

// Tension    T=+1-->Tight            T=-1--> Round
// Bias       B=+1-->Post Shoot       B=-1--> Pre shoot
// Continuity C=+1-->Inverted corners C=-1--> Box corners

typedef struct sKey
{
 int FrameNumber;
 float x;
 float Cont,Bias,Tens;
 float EaseIn,EaseOut;
 float a,b,c,d;
} sKey;

// ---

typedef struct sKeyFrame
{
 float CurrentPos;
 float Speed;
 int CurrentKey;
 int NumKeys;
 sKey *Key;
} sKeyFrame;

// 컴컴� Kochanek-Bartels 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�

void Spline_Init( sKeyFrame *Kframe )
{
 int a,b,c,d;
 sKey *K;
 int n;

 float FFA,FFB,FFC,FFD;
 float DD;
 float Ft1,Ft2,Ft3,Ft4;
 float Fu1,Fu2,Fu3,Fu4;
 float Fv1,Fv2,Fv3,Fv4;
 float Fw1,Fw2,Fw3,Fw4;
 float FAX,FBX,FCX,FDX;
 float FT,FB,FC;

 for (n=0;n<Kframe->NumKeys;n++)
 {
  a = n-1;
  b = n;
  c = n+1;
  d = n+2;
  if (a<0) a += Kframe->NumKeys;
  if (c>=Kframe->NumKeys) c -= Kframe->NumKeys;
  if (d>=Kframe->NumKeys) d -= Kframe->NumKeys;

  K = &(Kframe->Key[n]);

  FFA=(1-K->Tens)*(1+K->Cont)*(1+K->Bias);
  FFB=(1-K->Tens)*(1-K->Cont)*(1-K->Bias);
  FFC=(1-K->Tens)*(1-K->Cont)*(1+K->Bias);
  FFD=(1-K->Tens)*(1+K->Cont)*(1-K->Bias);

//       n-1      n        n+1       n+2
//  T^3  -A    4+A-B-C   -4+B+C-D     D     /
//  T^2 +2A  -6-2A+2B+C  6-2B-C+D    -D    /
//  T^1  -A     A-B         B         0   /  2
//  T^0   0      2          0         0  /

  DD = 2;  //divisor for K-B

  Ft1 = -FFA/DD;
  Ft2 = (+4+FFA-FFB-FFC)/DD;
  Ft3 = (-4+FFB+FFC-FFD)/DD;
  Ft4 = FFD/DD;

  Fu1 = (+2*FFA)/DD;
  Fu2 = (-6-2*FFA+2*FFB+FFC)/DD;
  Fu3 = (+6-2*FFB-FFC+FFD)/DD;
  Fu4 = -FFD/DD;

  Fv1 = -FFA/DD;
  Fv2 = (FFA-FFB)/DD;
  Fv3 = FFB/DD;
  Fv4 = 0;

  Fw1 = 0;
  Fw2 = +2/DD;
  Fw3 = 0;
  Fw4 = 0;

  FAX =   Ft1 * Kframe->Key[a].x
        + Ft2 * Kframe->Key[b].x
        + Ft3 * Kframe->Key[c].x
        + Ft4 * Kframe->Key[d].x;

  FBX =   Fu1 * Kframe->Key[a].x
        + Fu2 * Kframe->Key[b].x
        + Fu3 * Kframe->Key[c].x
        + Fu4 * Kframe->Key[d].x;

  FCX =   Fv1 * Kframe->Key[a].x
        + Fv2 * Kframe->Key[b].x
        + Fv3 * Kframe->Key[c].x
        + Fv4 * Kframe->Key[d].x;

  FDX =   Fw1 * Kframe->Key[a].x
        + Fw2 * Kframe->Key[b].x
        + Fw3 * Kframe->Key[c].x
        + Fw4 * Kframe->Key[d].x;

  Kframe->Key[n].a = FAX;
  Kframe->Key[n].b = FBX;
  Kframe->Key[n].c = FCX;
  Kframe->Key[n].d = FDX;
 }
}

// 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴

void Spline_Update( sKeyFrame *Kframe )
 {
  int next;

  Kframe->CurrentPos += Kframe->Speed;
  next = Kframe->CurrentKey+1;
  if (next >= Kframe->NumKeys) next -= Kframe->NumKeys;
  if (Kframe->CurrentPos >= (float)Kframe->Key[next].FrameNumber)
  {
   Kframe->CurrentKey++;
  }
  // Wrap if reached the end...
  if (Kframe->CurrentKey == Kframe->NumKeys)
  {
   Kframe->CurrentKey = 0;
   Kframe->CurrentPos = 0;
  }
 }

 // -------------------------------------------------------------------------

float Ease(float u, float a, float b)
 {
  float k;
  float s = a+b;

  if (s==0.0) return(u);
  if (s<1.0)
  {
   a=a/s;
   b=b/s;
  }
  k = 1.0/(2.0-a-b);
  if (u<a) return ((k/a)*u*u);
  else if (u<1.0-b) return (k*(2*u-a));
  else
  {
   u = 1.0-u;
   return(1-0-(k/b)*u*u);
  }
 }

 // -------------------------------------------------------------------------

float Spline_GetFloat( sKeyFrame *Kframe)
 {
  int Curr,Next,Prev;
  float t,T,T2,T3;
  sKey *K,*Kp;
  float k,s;

  Curr = Kframe->CurrentKey;
  Next = Kframe->CurrentKey+1;
  if (Next >= Kframe->NumKeys) Next-= Kframe->NumKeys;

  t = (Kframe->CurrentPos - (float)Kframe->Key[Curr].FrameNumber)
      / (float)(Kframe->Key[Next].FrameNumber - Kframe->Key[Curr].FrameNumber);

  //                               from                      to
  T = Ease(t,Kframe->Key[Curr].EaseOut,Kframe->Key[Next].EaseIn);
  T2 = T * T;
  T3 = T2 * T;
  K = &(Kframe->Key[Curr]);

//  Prev = Curr-1;
//  if (Prev<0) Prev += Kframe->NumKeys;
//  Kp = &(Kframe->Key[Prev]);

  return ((K->a*T3) + (K->b*T2) + (K->c*T) + K->d);

// return ((K->a*T3) + (K->b*T2) + (Kp->c*T) + Kp->d);
//                                   ^          ^

 }

 // -------------------------------------------------------------------------

void main(void)
{
 sKeyFrame KF;
 sKey K[11];

 K[0].FrameNumber = 0;
 K[0].x = 0;
 K[0].Cont = 0.5;
 K[0].Bias = 0.5;
 K[0].Tens = 0.5;
 K[0].EaseIn = 0;
 K[0].EaseOut = 0;

 K[1].FrameNumber = 3;
 K[1].x = -15.839417;
 K[1].Cont = 0.5;
 K[1].Bias = 0.5;
 K[1].Tens = 0.5;
 K[1].EaseIn = 0;
 K[1].EaseOut = 0;

 K[2].FrameNumber = 10;
 K[2].x = -90.701706;
 K[2].Cont = 0.5;
 K[2].Bias = 0.5;
 K[2].Tens = 0.5;
 K[2].EaseIn = 0;
 K[2].EaseOut = 0;

 K[3].FrameNumber = 16;
 K[3].x = -14.222313;
 K[3].Cont = 0.5;
 K[3].Bias = 0.5;
 K[3].Tens = 0.5;
 K[3].EaseIn = 0;
 K[3].EaseOut = 0;

 K[4].FrameNumber = 20;
 K[4].x = 2.109337;
 K[4].Cont = 0.5;
 K[4].Bias = 0.5;
 K[4].Tens = 0.5;
 K[4].EaseIn = 0;
 K[4].EaseOut = 0;

 K[5].FrameNumber = 30;
 K[5].x = -10.639786;
 K[5].Cont = 0.5;
 K[5].Bias = 0.5;
 K[5].Tens = 0.5;
 K[5].EaseIn = 0;
 K[5].EaseOut = 0;

 K[6].FrameNumber = 34;
 K[6].x = 38.044212;
 K[6].Cont = 0.5;
 K[6].Bias = 0.5;
 K[6].Tens = 0.5;
 K[6].EaseIn = 0;
 K[6].EaseOut = 0;

 K[7].FrameNumber = 40;
 K[7].x = 76.479134;
 K[7].Cont = 0.5;
 K[7].Bias = 0.5;
 K[7].Tens = 0.5;
 K[7].EaseIn = 0;
 K[7].EaseOut = 0;

 K[8].FrameNumber = 47;
 K[8].x = 76.966293;
 K[8].Cont = 0.5;
 K[8].Bias = 0.5;
 K[8].Tens = 0.5;
 K[8].EaseIn = 0;
 K[8].EaseOut = 0;

 K[9].FrameNumber = 55;
 K[9].x = 66.279427;
 K[9].Cont = 0.5;
 K[9].Bias = 0.5;
 K[9].Tens = 0.5;
 K[9].EaseIn = 0;
 K[9].EaseOut = 0;

 K[10].FrameNumber = 60;
 K[10].x = 2.109314;
 K[10].Cont = 0.5;
 K[10].Bias = 0.5;
 K[10].Tens = 0.5;
 K[10].EaseIn = 0;
 K[10].EaseOut = 0;

 KF.Key = &K[0];
 KF.CurrentPos = 0;
 KF.Speed = 1;
 KF.CurrentKey = 0;
 KF.NumKeys = 11;

 Spline_Init(&KF);

 while (!kbhit())
 {
  printf("Pos: %5.1f   Value: %f\n",KF.CurrentPos,Spline_GetFloat(&KF));
  Spline_Update(&KF);
 }
}

// --- OUTPUT ---

/*

Pos:   0.0   Value: 0.000000    *
Pos:   1.0   Value: -3.169207
Pos:   2.0   Value: -9.374629
Pos:   3.0   Value: -15.839417  *
Pos:   4.0   Value: -21.417818
Pos:   5.0   Value: -32.679440
Pos:   6.0   Value: -47.237595
Pos:   7.0   Value: -62.705593
Pos:   8.0   Value: -76.696732
Pos:   9.0   Value: -86.824333
Pos:  10.0   Value: -90.701698  *
Pos:  11.0   Value: -89.760002
Pos:  12.0   Value: -77.693138
Pos:  13.0   Value: -59.303528
Pos:  14.0   Value: -39.393578
Pos:  15.0   Value: -22.765701
Pos:  16.0   Value: -14.222317  *
Pos:  17.0   Value: -5.508801
Pos:  18.0   Value: -0.635405
Pos:  19.0   Value: 1.527449
Pos:  20.0   Value: 2.109339    *
Pos:  21.0   Value: 2.371290
Pos:  22.0   Value: 1.641705
Pos:  23.0   Value: 0.164337
Pos:  24.0   Value: -1.817060
Pos:  25.0   Value: -4.058731
Pos:  26.0   Value: -6.316921
Pos:  27.0   Value: -8.347876
Pos:  28.0   Value: -9.907844
Pos:  29.0   Value: -10.753063
Pos:  30.0   Value: -10.639785  *
Pos:  31.0   Value: -4.379194
Pos:  32.0   Value: 11.144284
Pos:  33.0   Value: 27.946726
Pos:  34.0   Value: 38.044212   *
Pos:  35.0   Value: 44.169872
Pos:  36.0   Value: 51.881115
Pos:  37.0   Value: 60.072803
Pos:  38.0   Value: 67.639793
Pos:  39.0   Value: 73.476952
Pos:  40.0   Value: 76.479134   *
Pos:  41.0   Value: 78.811890
Pos:  42.0   Value: 79.843216
Pos:  43.0   Value: 79.901344
Pos:  44.0   Value: 79.314499
Pos:  45.0   Value: 78.410889
Pos:  46.0   Value: 77.518753
Pos:  47.0   Value: 76.966293   *
Pos:  48.0   Value: 76.661285
Pos:  49.0   Value: 75.899002
Pos:  50.0   Value: 74.760803
Pos:  51.0   Value: 73.328079
Pos:  52.0   Value: 71.682205
Pos:  53.0   Value: 69.904556
Pos:  54.0   Value: 68.076500
Pos:  55.0   Value: 66.279427   *
Pos:  56.0   Value: 58.720596
Pos:  57.0   Value: 43.441406
Pos:  58.0   Value: 25.524626
Pos:  59.0   Value: 10.053004
Pos:  60.0   Value: 2.109315    *

*/
