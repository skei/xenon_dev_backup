 #include <math.h>
 // #include <stdio.h>
 #include "engine.h"


float Ease (float t, float a, float b)
 {
  float k;
  float s = a+b;

  if (s == 0.0) return t;
  if (s > 1.0)
  {
   a = a/s;
   b = b/s;
  }
  k = 1.0/(2.0-a-b);
  if (t < a) return ((k/a)*t*t);
  else
  {
   if (t < 1.0-b) return (k*(2*t-a));
   else
   {
    t = 1.0-t;
    return (1.0-(k/b)*t*t);
   }
  }
 }

 // ---

void CompElementDeriv (float pp,  float p,   float pn,
                       float *ds, float *dd, float ksm,
                       float ksp, float kdm, float kdp)
 {
  float delm, delp;

  delm = p - pp;
  delp = pn - p;
  *ds  = ksm*delm + ksp*delp;
  *dd  = kdm*delm + kdp*delp;
 }

 // ---

void CompDeriv ( sKeyframe *keyp, sKeyframe *key, sKeyframe *keyn)
 {
  float tm, cm, cp, bm, bp, tmcm, tmcp;
  float ksm, ksp, kdm, kdp, delm, delp;
  float dt, fp, fn, c;

  dt = 0.5 * (keyn->Frame - keyp->Frame);
  fp = (key->Frame - keyp->Frame) / dt;
  fn = (keyn->Frame - key->Frame) / dt;
  c  = fabs (key->Cont);
  fp = fp + c - c * fp;
  fn = fn + c - c * fn;
  cm = 1.0 - key->Cont;
  tm = 0.5 * (1.0 - key->Tens);
  cp = 2.0 - cm;
  bm = 1.0 - key->Bias;
  bp = 2.0 - bm;
  tmcm = tm * cm;
  tmcp = tm * cp;
  ksm = tmcm * bp * fp; ksp = tmcp * bm * fp;
  kdm = tmcp * bp * fn; kdp = tmcm * bm * fn;
  CompElementDeriv (keyp->Val._quat[qW], key->Val._quat[qW], keyn->Val._quat[qW],
                    &key->dsa, &key->dda, ksm, ksp, kdm, kdp);
  CompElementDeriv (keyp->Val._quat[qX], key->Val._quat[qX], keyn->Val._quat[qX],
                    &key->dsb, &key->ddb, ksm, ksp, kdm, kdp);
  CompElementDeriv (keyp->Val._quat[qY], key->Val._quat[qY], keyn->Val._quat[qY],
                    &key->dsc, &key->ddc, ksm, ksp, kdm, kdp);
  CompElementDeriv (keyp->Val._quat[qZ], key->Val._quat[qZ], keyn->Val._quat[qZ],
                    &key->dsd, &key->ddd, ksm, ksp, kdm, kdp);
 }

 // ---

CompDerivFirst ( sKeyframe *key, sKeyframe *keyn, sKeyframe *keynn)
 {
  float f20, f10, v20, v10;

  f20 = keynn->Frame - key->Frame;
  f10 =  keyn->Frame - key->Frame;
  v20 = keynn->Val._quat[qW] - key->Val._quat[qW];
  v10 =  keyn->Val._quat[qW] - key->Val._quat[qW];
  key->dda = (1-key->Tens)*(v20*(0.25-f10/(2*f20))+(v10-v20/2)*3/2+v20/2);
  v20 = keynn->Val._quat[qX] - key->Val._quat[qX];
  v10 =  keyn->Val._quat[qX] - key->Val._quat[qX];
  key->ddb = (1-key->Tens)*(v20*(0.25-f10/(2*f20))+(v10-v20/2)*3/2+v20/2);
  v20 = keynn->Val._quat[qY] - key->Val._quat[qY];
  v10 =  keyn->Val._quat[qY] - key->Val._quat[qY];
  key->ddc = (1-key->Tens)*(v20*(0.25-f10/(2*f20))+(v10-v20/2)*3/2+v20/2);
  v20 = keynn->Val._quat[qZ] - key->Val._quat[qZ];
  v10 =  keyn->Val._quat[qZ] - key->Val._quat[qZ];
  key->ddd = (1-key->Tens)*(v20*(0.25-f10/(2*f20))+(v10-v20/2)*3/2+v20/2);
 }

 // ---

void CompDerivLast ( sKeyframe *keypp, sKeyframe *keyp, sKeyframe *key)
 {
  float f20, f10, v20, v10;

  f20 = key->Frame - keypp->Frame;
  f10 =  key->Frame - keyp->Frame;
  v20 = key->Val._quat[qW] - keypp->Val._quat[qW];
  v10 = key->Val._quat[qW] -  keyp->Val._quat[qW];
  key->dsa = (1-key->Tens)*(v20*(0.25-f10/(2*f20))+(v10-v20/2)*3/2+v20/2);
  v20 = key->Val._quat[qX] - keypp->Val._quat[qX];
  v10 = key->Val._quat[qX] -  keyp->Val._quat[qX];
  key->dsb = (1-key->Tens)*(v20*(0.25-f10/(2*f20))+(v10-v20/2)*3/2+v20/2);
  v20 = key->Val._quat[qY] - keypp->Val._quat[qY];
  v10 = key->Val._quat[qY] -  keyp->Val._quat[qY];
  key->dsc = (1-key->Tens)*(v20*(0.25-f10/(2*f20))+(v10-v20/2)*3/2+v20/2);
  v20 = key->Val._quat[qZ] - keypp->Val._quat[qZ];
  v10 = key->Val._quat[qZ] -  keyp->Val._quat[qZ];
  key->dsd = (1-key->Tens)*(v20*(0.25-f10/(2*f20))+(v10-v20/2)*3/2+v20/2);
 }

void CompDerivTwo ( sKeyframe *key)
 {
  sKeyframe *keyn = key->Next;

  key->dsa = 0.0;
  key->dsb = 0.0;
  key->dsc = 0.0;
  key->dsd = 0.0;
  key->dda = (keyn->Val._quat[qW] - key->Val._quat[qW]) * (1 - key->Tens);
  key->ddb = (keyn->Val._quat[qX] - key->Val._quat[qX]) * (1 - key->Tens);
  key->ddc = (keyn->Val._quat[qY] - key->Val._quat[qY]) * (1 - key->Tens);
  key->ddd = (keyn->Val._quat[qZ] - key->Val._quat[qZ]) * (1 - key->Tens);
  keyn->dda = 0.0;
  keyn->ddb = 0.0;
  keyn->ddc = 0.0;
  keyn->ddd = 0.0;
  keyn->dsa = (keyn->Val._quat[qW] - key->Val._quat[qW]) * (1 - keyn->Tens);
  keyn->dsb = (keyn->Val._quat[qX] - key->Val._quat[qX]) * (1 - keyn->Tens);
  keyn->dsc = (keyn->Val._quat[qY] - key->Val._quat[qY]) * (1 - keyn->Tens);
  keyn->dsd = (keyn->Val._quat[qZ] - key->Val._quat[qZ]) * (1 - keyn->Tens);
 }

 // ---

int Spline_Init ( sKeyframe *keys)
 {
  // initialize spline key list.
  int   count;
  sKeyframe *curr;

  if (keys == NULL) return 1;//clax_err_nullptr
  if (keys->Next == NULL) return 2;//clax_err_spline;

  if (keys->Next->Next != NULL)
  { // 3 or more keys
   for (curr = keys->Next; (curr->Next != NULL); curr = curr->Next)
    CompDeriv (curr->Prev, curr, curr->Next);
   CompDerivFirst (keys, keys->Next, keys->Next->Next);
   CompDerivLast (curr->Prev->Prev, curr->Prev, curr);
  }
  else CompDerivTwo (keys); // 2 keys
  return 0;//clax_err_ok;
 }

 // ---

 // get spline interpolated float for frame "frame".
int Spline_GetFloat ( float     *out, sKeyframe *keys, float frame)
 {
  float t, t2, t3;
  float h[4];

  if (frame < 0.0) return 1;//clax_err_badframe;
  if (!keys) return 2;//clax_err_nullptr;

  while (frame > keys->Next->Frame && keys->Next) keys = keys->Next;
  if (!keys->Next || frame < keys->Frame)
  { // frame is above last key
    *out = keys->Val._float;
    return 0;//clax_err_ok;
  }
  t = (frame - keys->Frame) / (keys->Next->Frame - keys->Frame);
  t = Ease (t, keys->EaseFrom, keys->Next->EaseTo);
  t2 = t * t;
  t3 = t2 * t;
  h[0] =  2 * t3 - 3 * t2 + 1;
  h[1] = -2 * t3 + 3 * t2;
  h[2] = t3 - 2 * t2 + t;
  h[3] = t3 - t2;
  *out =   (h[0]*keys->Val._float) + (h[1]*keys->Next->Val._float)
         + (h[2]*keys->dda) + (h[3]*keys->Next->dsa);
  return 0;//clax_err_ok;
 }

 // ---

 // get spline interpolated vector for frame "frame".
int Spline_GetVector( sVector   *out, sKeyframe *keys, float frame)
 {

  float t, t2, t3;
  float h[4];

  if (frame < 0.0) return 1;//clax_err_badframe;
  if (keys == NULL) return 2;//clax_err_nullptr;
  while (frame > keys->Next->Frame && keys->Next) keys = keys->Next;
  if ((keys->Next == NULL) || (frame < keys->Frame))
  { // frame is above last key
   out->x = keys->Val._vector.x;
   out->y = keys->Val._vector.y;
   out->z = keys->Val._vector.z;
   return 0;//clax_err_ok;
  }
  t = (frame - keys->Frame) / (keys->Next->Frame - keys->Frame);
  t = Ease(t, keys->EaseFrom, keys->Next->EaseTo);
  t2 = t * t;
  t3 = t2 * t;
  h[0] =  2 * t3 - 3 * t2 + 1;
  h[1] = -2 * t3 + 3 * t2;
  h[2] = t3 - 2 * t2 + t;
  h[3] = t3 - t2;
  out->x = (h[0]*keys->Val._vector.x) + (h[1]*keys->Next->Val._vector.x) +
           (h[2]*keys->dda) +         (h[3]*keys->Next->dsa);
  out->y = (h[0]*keys->Val._vector.y) + (h[1]*keys->Next->Val._vector.y) +
           (h[2]*keys->ddb) +         (h[3]*keys->Next->dsb);
  out->z = (h[0]*keys->Val._vector.z) + (h[1]*keys->Next->Val._vector.z) +
           (h[2]*keys->ddc) +         (h[3]*keys->Next->dsc);
  return 0;//clax_err_ok;
 }

 // ---

 // get spline interpolated quaternion for frame "frame".
int Spline_GetQuat  ( float     *out, sKeyframe *keys, float frame)
 {
  float t, t2, t3;
  float h[4];

  if (frame < 0.0) return 1;//clax_err_badframe;
  if (!keys) return 2;//clax_err_nullptr;

  while (frame > keys->Next->Frame && keys->Next) keys = keys->Next;
  if (!keys->Next || frame < keys->Frame)
  { // frame is above last key
   out[qX] = keys->Val._quat[qX];
   out[qY] = keys->Val._quat[qY];
   out[qZ] = keys->Val._quat[qZ];
   out[qW] = keys->Val._quat[qW];
   return 0;//clax_err_ok;
  }
  t = (frame - keys->Frame) / (keys->Next->Frame - keys->Frame);
  t = Ease (t, keys->EaseFrom, keys->Next->EaseTo);
  t2 = t * t;
  t3 = t2 * t;
  h[0] =  2 * t3 - 3 * t2 + 1;
  h[1] = -2 * t3 + 3 * t2;
  h[2] = t3 - 2 * t2 + t;
  h[3] = t3 - t2;
  out[qW] = (h[0]*keys->Val._quat[qW]) + (h[1]*keys->Next->Val._quat[qW]) +
            (h[2]*keys->dda) +         (h[3]*keys->Next->dsa);
  out[qX] = (h[0]*keys->Val._quat[qX]) + (h[1]*keys->Next->Val._quat[qX]) +
            (h[2]*keys->ddb) +         (h[3]*keys->Next->dsb);
  out[qY] = (h[0]*keys->Val._quat[qY]) + (h[1]*keys->Next->Val._quat[qY]) +
            (h[2]*keys->ddc) +         (h[3]*keys->Next->dsc);
  out[qZ] = (h[0]*keys->Val._quat[qZ]) + (h[1]*keys->Next->Val._quat[qZ]) +
            (h[2]*keys->ddd) +         (h[3]*keys->Next->dsd);
  return 0;//clax_err_ok;
 }
