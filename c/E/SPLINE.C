 #include <math.h>
 #include "engine.h"

 // ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

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

 // ÄÄÄ

 // CompDeriv: compute derivative for key "key".
void CompDeriv ( sKey *keyp, sKey *key, sKey *keyn)
 {
  float tm, cm, cp, bm, bp, tmcm, tmcp;
  float ksm, ksp, kdm, kdp;
  float dt, fp, fn, c;

  dt = 0.5 * (keyn->Frame - keyp->Frame);
  fp = ( key->Frame - keyp->Frame) / dt;
  fn = (keyn->Frame -  key->Frame) / dt;
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

 // ÄÄÄ

 // CompDerivFirst: compute derivative for first key "key".
void CompDerivFirst (sKey *key, sKey *keyn, sKey *keynn)
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

 // ÄÄÄ

 // CompDerivLast: compute derivative for last "key".
void CompDerivLast ( sKey *keypp, sKey *keyp, sKey *key)
 {
  float f20, f10, v20, v10;

  f20 = key->Frame - keypp->Frame;
  f10 = key->Frame -  keyp->Frame;
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

 // ÄÄÄ

 // CompDerivLoopFirst: compute derivative for first key "key".
void CompDerivLoopFirst (sKey *keyp, sKey *key, sKey *keyn, float lf)
 {
  float tm, cm, cp, bm, bp, tmcm, tmcp;
  float ksm, ksp, kdm, kdp;
  float dt, fp, fn, c;

  dt = 0.5 * (keyn->Frame - keyp->Frame + lf);
  fp = (key->Frame - keyp->Frame + lf) / dt;
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

 // ÄÄÄ

 // CompDerivLoopLast: compute derivative for last key "key".
void CompDerivLoopLast (sKey *keyp, sKey *key, sKey *keyn, float lf)
 {
  float tm, cm, cp, bm, bp, tmcm, tmcp;
  float ksm, ksp, kdm, kdp;
  float dt, fp, fn, c;

  dt = 0.5 * (keyn->Frame - keyp->Frame + lf);
  fp = (key->Frame - keyp->Frame) / dt;
  fn = (keyn->Frame - key->Frame + lf) / dt;
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

 // ÄÄÄ

 // CompDerivTwo: compute derivative for keys "key" (2-key spline).
void CompDerivTwo (sKey *key)
 {
  sKey *keyn = key->Next;

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

 // ÄÄÄ

 // CompAB: Compute derivatives for rotation keys.
static void CompAB (sKey *prev, sKey *cur, sKey *next)
 {
  float qprev[4], qnext[4], q[4];
  float qp[4], qm[4], qa[4], qb[4], qae[4], qbe[4];
  float QA[4], QB[4], QC[4];        // key quaternions
  float QAA[4], QAB[4], QAC[4];     // key angle/axis representation
  float  tm, cm, cp, bm, bp, tmcm, tmcp, ksm, ksp, kdm, kdp;
  float  dt, fp, fn, c;
  int  i;

  Quat_Copy(&QAB,&cur->Val._quat);
  Quat_Copy(&QB, &cur->qa);
  if (prev)
  {
   Quat_Copy(&QAA,&prev->Val._quat);
   Quat_Copy(&QA, &prev->qa);
  }
  if (next)
  {
   Quat_Copy(&QAC,&next->Val._quat);
   Quat_Copy(&QC, &next->qa);
  }
  if (prev != NULL)
  {
   if (fabs (QAB[qW] - QAA[qW]) > 2*PI - EPSILON)
   {
    Quat_Copy(&q,&QAB);
    q[qW] = 0.0;
    Quat_Log(&qm,&q);
   }
   else
   {
    Quat_Copy(&qprev,&QA);
    if (Quat_DotUnit(&qprev,&QB) < 0.0) Quat_Negate(&qprev, &qprev);
    Quat_Lndif (&qm,&qprev,&QB);
   }
  }
  if (next != NULL)
  {
   if (fabs (QAC[qW] - QAB[qW]) > 2*PI - EPSILON)
   {
    Quat_Copy(&q,&QAC);
    q[qW] = 0.0;
    Quat_Log(&qp,&q);
   }
   else
   {
    Quat_Copy(&qnext,&QC);
    if (Quat_DotUnit(&qnext, &QB) < 0.0) Quat_Negate(&qnext, &qnext);
    Quat_Lndif(&qp,&QB, &qnext);
   }
  }
  if (prev == NULL) Quat_Copy(&qm,&qp);
  if (next == NULL) Quat_Copy(&qp,&qm);
  fp = fn = 1.0;
  cm = 1.0 - cur->Cont;
  if (prev && next)
  {
   dt = 0.5 * (next->Frame - prev->Frame);
   fp = ( cur->Frame - prev->Frame) / dt;
   fn = (next->Frame -  cur->Frame) / dt;
   c = fabs( cur->Cont );
   fp = fp + c - c * fp;
   fn = fn + c - c * fn;
  }
  tm = 0.5 * (1.0 - cur->Tens);
  cp = 2.0 - cm;
  bm = 1.0 - cur->Bias;
  bp = 2.0 - bm;
  tmcm = tm * cm;
  tmcp = tm * cp;
  ksm  = 1.0 - tmcm * bp * fp;
  ksp  = -tmcp * bm * fp;
  kdm  = tmcp * bp * fn;
  kdp  = tmcm * bm * fn - 1.0;
  qa[qX] = 0.5 * (kdm * qm[qX] + kdp * qp[qX]);
  qb[qX] = 0.5 * (ksm * qm[qX] + ksp * qp[qX]);
  qa[qY] = 0.5 * (kdm * qm[qY] + kdp * qp[qY]);
  qb[qY] = 0.5 * (ksm * qm[qY] + ksp * qp[qY]);
  qa[qZ] = 0.5 * (kdm * qm[qZ] + kdp * qp[qZ]);
  qb[qZ] = 0.5 * (ksm * qm[qZ] + ksp * qp[qZ]);
  qa[qW] = 0.5 * (kdm * qm[qW] + kdp * qp[qW]);
  qb[qW] = 0.5 * (ksm * qm[qW] + ksp * qp[qW]);
  Quat_Exp (&qae,&qa);
  Quat_Exp (&qbe,&qb);
  Quat_Mul (&cur->ds,&QB, &qae);
  Quat_Mul (&cur->dd,&QB, &qbe);
 }

 // ÄÄÄ

 // spline_ease: remap parameter between two keys to apply eases.
float spline_ease (float t, float a, float b)
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

 // ÄÄÄ

 // spline_init: compute spline derivations.
int Spline_Init ( sTrack *track)
 {
  sKey *curr;
  sKey *keys = track->Keys;
  sKey *last = track->LastKey;

  if (!keys) return -1;//clax_err_nullptr;
  if (!keys->Next) return -2;//clax_err_spline;

  if (keys->Next->Next)
  { // 3 or more keys
   for (curr = keys->Next; curr->Next; curr = curr->Next)
   {
    CompDeriv (curr->Prev, curr, curr->Next);
   }
   if (track->Flags & TRACKflag_loop)
   {
    CompDerivLoopFirst (last->Prev, keys, keys->Next, track->LastKey->Frame/*Frames*/);
    CompDerivLoopLast (last->Prev, last, keys->Next, track->LastKey->Frame/*Frames*/);
   }
   else
   {
    CompDerivFirst (keys, keys->Next, keys->Next->Next);
    CompDerivLast (curr->Prev->Prev, curr->Prev, curr);
   }
  }
  else CompDerivTwo (keys); // 2 keys
  return 0;//clax_err_ok;
 }

 // ÄÄÄ

 // spline_initrot: compute spline derivations for rotation track.
int Spline_InitRot (sTrack *track)
 {
  sKey *curr;
  sKey *keys = track->Keys;
  sKey *last = track->LastKey;

  if (!keys) return -1;//clax_err_nullptr;
  if (!keys->Next) return -2;//clax_err_spline;

  if (keys->Next->Next)
  { // 3 or more keys
   for (curr = keys->Next; curr->Next; curr = curr->Next)
   {
    CompAB (curr->Prev, curr, curr->Next);
   }
 //    if (track->flags & clax_track_loop) {
 //      CompAB (last->Prev, keys, keys->Next);
 //      CompAB (keys->Prev, last, keys->Next);
 //    } else {
   CompAB (NULL, keys, keys->Next);
   CompAB (keys, last, NULL);
 //    }
  }
  else
  {
   CompAB (NULL, keys, keys->Next);
   CompAB (keys, last, NULL);
  }
  return 0;//clax_err_ok;
 }

 // ÄÄÄ

 // spline_getkey_float: get spline interpolated float for frame "frame".
int Spline_GetKey_Float (sTrack *track, float frame, float *out)
 {
  sKey *keys;
  float  t, t2, t3;
  float  h[4];

  if (frame < 0.0) return -1;//clax_err_badframe;
  if (!track || !track->Keys) return -2;//clax_err_nullptr;
  if (frame < track->LastKey->Frame) keys = track->Keys;
  else keys = track->LastKey;
  while (keys->Next && frame > keys->Next->Frame) keys = keys->Next;
  track->LastKey = keys;
  if (!keys->Next || frame < keys->Frame)
  { // frame is above last key
   *out = keys->Val._float;
   return 0;//clax_err_ok;
  }
  t = (frame - keys->Frame) / (keys->Next->Frame - keys->Frame);
  t = spline_ease (t, keys->EaseFrom, keys->Next->EaseTo);
  t2 = t * t;
  t3 = t2 * t;
  h[0] =  2 * t3 - 3 * t2 + 1;
  h[1] = -2 * t3 + 3 * t2;
  h[2] = t3 - 2 * t2 + t;
  h[3] = t3 - t2;
  *out = (h[0]*keys->Val._float) + (h[1]*keys->Next->Val._float) +
         (h[2]*keys->dda) +        (h[3]*keys->Next->dsa);
  return 0;//clax_err_ok;
 }

 // ÄÄÄ

 // spline_getkey_vect: get spline interpolated vector for frame "frame".
int Spline_GetKey_Vect ( sTrack *track, float frame, sVector *out)
 {
  sKey *keys;
  float  t, t2, t3;
  float  h[4];

  if (frame < 0.0) return -1;//clax_err_badframe;
  if (!track || !track->Keys) return -2;//clax_err_nullptr;
  if (frame < track->LastKey->Frame) keys = track->Keys;
  else keys = track->LastKey;
  while (keys->Next && frame > keys->Next->Frame) keys = keys->Next;
  track->LastKey = keys;
  if (!keys->Next || frame < keys->Frame)
  { // frame is above last key
   Vector_Copy (&keys->Val._vector, out);
   return 0;//clax_err_ok;
  }
  t = (frame - keys->Frame) / (keys->Next->Frame - keys->Frame);
  t = spline_ease (t, keys->EaseFrom, keys->Next->EaseTo);
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

 // ÄÄÄ

 // spline_getkey_quat: get spline interpolated quaternion for frame "frame".
int Spline_GetKey_Quat (sTrack *track, float frame, float *out)
 {
  sKey  *keys;
  float a[4], b[4], p[4], q[4], q1[4];
  float  t, angle, spin;

  if (frame < 0.0) return -1;//clax_err_badframe;
  if (!track || !track->Keys) return -2;//clax_err_nullptr;
  if (frame < track->LastKey->Frame) keys = track->Keys;
  else keys = track->LastKey;
  while (keys->Next && frame > keys->Next->Frame) keys = keys->Next;
  track->LastKey = keys;
  if (!keys->Next || frame < keys->Frame)
  { // frame is above last key
   Quat_Copy (out,&keys->qa);
   return 0;//clax_err_ok;
  }
  t = (frame - keys->Frame) / (keys->Next->Frame - keys->Frame);
  t = spline_ease (t, keys->EaseFrom, keys->Next->EaseTo);
  Quat_Copy (&a,&keys->qa);
  Quat_Copy (&b,&keys->Next->qa);
  angle = keys->Next->Val._quat[qW] - keys->Val._quat[qW];
  if (angle > 0) spin = floor (angle / (2*PI));
  else spin = ceil (angle / (2*PI));
  Quat_Slerpl (&p,&a,&b,spin,t);
  Quat_Slerpl (&q,&keys->dd, &keys->Next->ds,spin,t);
  t = (((1.0-t)*2.0)*t);
  Quat_Slerpl (&q1,&p,&q,0,t);
  Quat_Copy (out,&q1);
  return 0;//clax_err_ok;
 }

