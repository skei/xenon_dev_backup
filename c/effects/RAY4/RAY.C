

coordType  RaySphereInter (rayType ray, sphereType sphere, float *soln, int *flag)
{
 coordType  X;              /*this will be the intersection pt*/
 coordType  X0;
 coordType  center;         /*center of sphere*/
 float      R;              /*radius*/
 VectorType u;
 VectorType deltaX;         /*X0 - C*/
 float      det;            /* b^2 - mag(deltaX)^2 + R^2*/
 float      a,b,c;
 float      s1,s2;          /*solution*/
 int        tmp;
 float _a;

 *flag = SOLN;
 u = Normalize(ray.dir);
 X0 = ray.pt;
 center = sphere.ctr;
 deltaX = P1minusP2(X0,center);
 R = sphere.radius;
 b = DotProduct(u,deltaX);
 _a = FastSqrt( deltaX.x*deltaX.x + deltaX.y*deltaX.y + deltaX.z*deltaX.z);
 c = _a*_a - R*R;

 det = b*b - c;
 if (det == 0)
 {
  s1 = -b;
  *soln = s1;
  X.x = X0.x + s1*u.x;
  X.y = X0.y + s1*u.y;
  X.z = X0.z + s1*u.z;
 }
 else if (det > 0)
 {
  s1 = (-b + FastSqrt/*sqrt*/(det));
  s2 = (-b - FastSqrt/*sqrt*/(det));
  if ((s1 < s2) && (s1 > 0))
  {
   *soln = s1;
   X.x = X0.x + s1*u.x;
   X.y = X0.y + s1*u.y;
   X.z = X0.z + s1*u.z;
  }
  else
  {
   *soln = s2;
   X.x = X0.x + s2*u.x;
   X.y = X0.y + s2*u.y;
   X.z = X0.z + s2*u.z;
  }
 }
 else *flag = NO_SOLN;
 return (X);
}

