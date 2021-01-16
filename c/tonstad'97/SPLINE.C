
 // alpha interpolation parameter (0 to 1)
 // a, b  start and end unit quaternions
 // q     output interpolated quaternion
 // spin  number of extra spin rotations
void Slerp(float alpha, sKey *a, sKey *b, sQuaternion *q, int spin)
 {
  float beta;          // complementary interp parameter
  float theta;         // angle between A and B
  float sin_t, cos_t;  // sine, cosine of theta
  float phi;           // theta plus spins
  int bflip;           // use negation of B?

  // cosine theta = dot product of A and B
	cos_t = a->x*b->x + a->y*b->y + a->z*b->z + a->w*b->w;

  // if B is on opposite hemisphere from A, use -B instead
  if (cos_t < 0.0)
  {
   cos_t = -cos_t;
   bflip = TRUE;
  }
  else bflip = FALSE;

  // if B is (within precision limits) the same as A,
  // just linear interpolate between A and B.
  // Can't do spins, since we don't know what direction to spin.
  if (1.0 - cos_t < EPSILON) beta = 1.0 - alpha;
  else
  {        // normal case
   theta = acos(cos_t);
   phi = theta + spin * PI;
   sin_t = sin(theta);
   beta = sin(theta - alpha*phi) / sin_t;
   alpha = sin(alpha*phi) / sin_t;
 	}
  if (bflip) alpha = -alpha;

  // interpolate
 	q->x = beta*a->x + alpha*b->x;
 	q->y = beta*a->y + alpha*b->y;
 	q->z = beta*a->z + alpha*b->z;
 	q->w = beta*a->w + alpha*b->w;
 }

 // -------------------------------------------------------------------------

void Spline_Init( sKeyFrame *K )
 {
  int i;
  int a,b,c,d;

  if (K->NumKeys == 0) return;
  K->CurrentPos = 0;
  K->Speed = 1;
  for (i=0;i<K->NumKeys;i++)
  {
   a = i-1;
   b = i;
   c = i+1;
   d = i+2;

   if (K->KF_Track_Loop == 1)
   {
    if (a <  0)          a += K->NumKeys;
    if (c >= K->NumKeys) c -= K->NumKeys;
    if (d >= K->NumKeys) d -= K->NumKeys;
   }
   else
   {
    if (a <  0)          a = 0;
    if (c >= K->NumKeys) c = K->NumKeys-1;
    if (d >= K->NumKeys) d = K->NumKeys-1;
   }

   if (K->KF_w == 1)
   {
    K->Key[i].ApW =    -(K->Key[a].w)
                    + 3*(K->Key[b].w)
                    - 3*(K->Key[c].w)
                    +   (K->Key[d].w);
    K->Key[i].BpW =   2*(K->Key[a].w)
                    - 5*(K->Key[b].w)
                    + 4*(K->Key[c].w)
                    -   (K->Key[d].w);
    K->Key[i].CpW = -   (K->Key[a].w)
                    +   (K->Key[c].w);
    K->Key[i].DpW =   2*(K->Key[b].w);
   }

   if (K->KF_x == 1)
   {
    K->Key[i].ApX =    -(K->Key[a].x)
                    + 3*(K->Key[b].x)
                    - 3*(K->Key[c].x)
                    +   (K->Key[d].x);
    K->Key[i].BpX =   2*(K->Key[a].x)
                    - 5*(K->Key[b].x)
                    + 4*(K->Key[c].x)
                    -   (K->Key[d].x);
    K->Key[i].CpX = -   (K->Key[a].x)
                    +   (K->Key[c].x);
    K->Key[i].DpX =   2*(K->Key[b].x);
   }

   if (K->KF_y == 1)
   {
    K->Key[i].ApY =    -(K->Key[a].y)
                    + 3*(K->Key[b].y)
                    - 3*(K->Key[c].y)
                    +   (K->Key[d].y);
    K->Key[i].BpY =   2*(K->Key[a].y)
                    - 5*(K->Key[b].y)
                    + 4*(K->Key[c].y)
                    -   (K->Key[d].y);
    K->Key[i].CpY = -   (K->Key[a].y)
                    +   (K->Key[c].y);
    K->Key[i].DpY =   2*(K->Key[b].y);
   }

   if (K->KF_z == 1)
   {
    K->Key[i].ApZ =    -(K->Key[a].z)
                    + 3*(K->Key[b].z)
                    - 3*(K->Key[c].z)
                    +   (K->Key[d].z);
    K->Key[i].BpZ =   2*(K->Key[a].z)
                    - 5*(K->Key[b].z)
                    + 4*(K->Key[c].z)
                    -   (K->Key[d].z);
    K->Key[i].CpZ = -   (K->Key[a].z)
                    +   (K->Key[c].z);
    K->Key[i].DpZ =   2*(K->Key[b].z);
   }
  }
 }

 // -------------------------------------------------------------------------

void Spline_Update( sKeyFrame *K )
 {
  int next;

  if (K->CurrentKey == -1) return;   // don't update non-wrapping splines

  K->CurrentPos += K->Speed;
  next = K->CurrentKey+1;
  if (next >= (K->NumKeys-1))
  { // spline has reached/passed end...
   if (K->KF_Track_Loop == 1)
   {  // wrapping
    K->CurrentPos -= K->FrameNumber[K->NumKeys-1];
    K->CurrentKey = 0;
   }
   else
   {  // non-wrapping
    K->CurrentKey = -1;   // signal end of spline & non-wrapping
   }
  }
  else
  {
   if (K->CurrentPos >= K->FrameNumber[next]) K->CurrentKey++;
  }
 }

 // -------------------------------------------------------------------------

float Spline_GetValue( sKeyFrame *K, sVector *Vector)
 {
  int x1,x2;
  float T,T2,T3;
  float Value;
  sQuaternion q;

 // Vector->x = K->Key[K->NumKeys-1].x;
 // Vector->y = K->Key[K->NumKeys-1].y;
 // Vector->z = K->Key[K->NumKeys-1].z;
 // return K->Key[0].w;

  if (K->CurrentKey == -1)    // spline has reached end, and is non-wrapping
  {
   Vector->x = K->Key[K->NumKeys-1].x;
   Vector->y = K->Key[K->NumKeys-1].y;
   Vector->z = K->Key[K->NumKeys-1].z;
   return K->Key[K->NumKeys-1].w;
  }
  else
  {
   Value = 0;
   x1 = K->CurrentKey;
   x2 = K->CurrentKey+1;
   T = (K->CurrentPos      - K->FrameNumber[x1])
     / (K->FrameNumber[x2] - K->FrameNumber[x1]);
   T2 = T * T;       // Square of t
   T3 = T2 * T;      // Cube of t

 //  if ((K->KF_w == 1)&&(K->KF_x == 1)&&(K->KF_y == 1)&&(K->KF_z == 1))
 //  {
 //   Slerp (T,&(K->Key[x1]),&(K->Key[x2]),&q,0);
 //   Value = q.w;
 //   Vector->x = q.x;
 //   Vector->y = q.y;
 //   Vector->z = q.z;
 //  }
 //  else

   {
    if (K->KF_w == 1)
    {
     Value = (  (K->Key[K->CurrentKey].ApW*T3)
              + (K->Key[K->CurrentKey].BpW*T2)
              + (K->Key[K->CurrentKey].CpW*T )
              +  K->Key[K->CurrentKey].DpW   )/2; // Calc x value /D
    }
    if (K->KF_x == 1)
    {
     Vector->x = (  (K->Key[K->CurrentKey].ApX*T3)
                  + (K->Key[K->CurrentKey].BpX*T2)
                  + (K->Key[K->CurrentKey].CpX*T )
                  +  K->Key[K->CurrentKey].DpX   )/2; // Calc x value /D
    }
    if (K->KF_y == 1)
    {
     Vector->y = (  (K->Key[K->CurrentKey].ApY*T3)
                  + (K->Key[K->CurrentKey].BpY*T2)
                  + (K->Key[K->CurrentKey].CpY*T )
                  +  K->Key[K->CurrentKey].DpY   )/2; // Calc x value /D
    }
    if (K->KF_z == 1)
    {
     Vector->z = (  (K->Key[K->CurrentKey].ApZ*T3)
                  + (K->Key[K->CurrentKey].BpZ*T2)
                  + (K->Key[K->CurrentKey].CpZ*T )
                  +  K->Key[K->CurrentKey].DpZ   )/2; // Calc x value /D
    }
   }
  }
  return Value;
 }

