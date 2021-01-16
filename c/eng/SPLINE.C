#include "types.h"

// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
// ฑ Spline routines ฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑ
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ

// location = pos in spline (0..65535)
sCoord get_spline_point(int location, sSpline *spl)
{
 sCoord ret_cor;
 float t,t1,t2,t3,t4;
 static sCoord v1, v2;
 sSpline spl2;

 int cofs, cofs2, cofs3, cofs4;

// Make new spline out of the old one so we can change some angles (for angle wrapping).
 spl2.num = spl->num;
 spl2.oClosed = spl->oClosed;
 spl2.bendsize = spl->bendsize;

 for(cofs=0; cofs<spl->num; cofs++)
 {
  spl2.p[cofs].x = spl->p[cofs].x;
  spl2.p[cofs].y = spl->p[cofs].y;
  spl2.p[cofs].z = spl->p[cofs].z;
 }

 if(spl2.oClosed == 1)
 {													// It's a closed spline
  t=location*((float)spl2.num/65536);
  cofs = (int)t;
  t -= cofs;
  cofs4 = cofs - 1;
  cofs2 = cofs + 1;
  cofs3 = cofs + 2;
  if (cofs4<0) cofs4+=spl2.num;
  if (cofs2>=spl2.num) cofs2-=spl2.num;
  if (cofs3>=spl2.num) cofs3-=spl2.num;
  if (cofs3>=spl2.num) cofs3-=spl2.num;
 }
 else
 {                  // It's a open spline
  t=location*((float)(spl2.num-1)/65536);
  cofs = (int)t;
  t -= cofs;
  cofs4 = cofs - 1;
  cofs2 = cofs + 1;
  cofs3 = cofs + 2;
  if (cofs4<0) cofs4=0;
  if (cofs2>=spl2.num) cofs2=spl2.num-1;
  if (cofs3>=spl2.num) cofs3=spl2.num-1;
 }
 v1.x = (spl2.p[cofs2].x - spl2.p[cofs ].x ) * spl2.bendsize;
 v1.y = (spl2.p[cofs2].y - spl2.p[cofs ].y ) * spl2.bendsize;
 v1.z = (spl2.p[cofs2].z - spl2.p[cofs ].z ) * spl2.bendsize;

 v2.x = (spl2.p[cofs3].x - spl2.p[cofs2].x ) * spl2.bendsize + v1.x;
 v2.y = (spl2.p[cofs3].y - spl2.p[cofs2].y ) * spl2.bendsize + v1.y;
 v2.z = (spl2.p[cofs3].z - spl2.p[cofs2].z ) * spl2.bendsize + v1.z;


 v1.x += (spl2.p[cofs ].x - spl2.p[cofs4].x ) * spl2.bendsize;
 v1.y += (spl2.p[cofs ].y - spl2.p[cofs4].y ) * spl2.bendsize;
 v1.z += (spl2.p[cofs ].z - spl2.p[cofs4].z ) * spl2.bendsize;

 t1 = (2*t*t*t-3*t*t+1);
 t2 = (-2*t*t*t+3*t*t);
 t3 = (t*t*t-2*t*t+t);
 t4 = (t*t*t-t*t);

 ret_cor.x = ( t1*(float)(spl2.p[cofs ].x)
              +t2*(float)(spl2.p[cofs2].x)
              +t3*(float)v1.x
              +t4*(float)v2.x );
 ret_cor.y = ( t1*(float)(spl2.p[cofs ].y)
              +t2*(float)(spl2.p[cofs2].y)
              +t3*(float)v1.y
              +t4*(float)v2.y );
 ret_cor.z = ( t1*(float)(spl2.p[cofs ].z)
              +t2*(float)(spl2.p[cofs2].z)
              +t3*(float)v1.z
              +t4*(float)v2.z );

 return(ret_cor);
}

