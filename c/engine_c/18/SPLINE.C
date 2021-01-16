#pragma warning (once:4244)			// conversion, possible loss of data
#pragma warning (disable:4035)	// __asm no return value
#pragma warning (disable:4101)	// unreferenced local variable


#include "defines.h"
#include "spline.h"

// -----

// -------------------------------------------------------
// SYNOPSIS:
//      polar get_spline_point(UWORD location, spline * spline);
//
// INPUT:
//      location        Where on the spline to get the point.
//                      $0000 = at the start of the spline
//                      $ffff = at the end of the spline
//      spline          Pointer to the spline
//
// OUTPUT:
//
// FUNCTION :
//
// NOTE:
//      If it's a open spline (oClosed==FALSE), we are at the last point
//      in the spline when location=$10000.
//      If it's a closed spline (oClosed==TRUE), we are the same location
//      when location=0 and when location=$10000.
//
//      The angle wrapping part can probably be done smarter......
//
//--------------------------------------------------------

coord get_spline_point(int location, spline *spl)
{
 coord ret_cor;
 float t,t1,t2,t3,t4;
 static coord v1, v2;
 spline spl2;
 int uwtmp;

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
  t=location*((float)spl2.num/DIVD_NUM);
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
  t=location*((float)(spl2.num-1)/DIVD_NUM);
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

// -----

float get_spline_value(int location, onespline *spl)
{
 float ret_cor;
 float t,t1,t2,t3,t4;
 static float v1, v2;
 onespline spl2;
 int uwtmp;

 int cofs, cofs2, cofs3, cofs4;

// Make new spline out of the old one so we can change some angles (for angle wrapping).
 spl2.num = spl->num;
 spl2.oClosed = spl->oClosed;
 spl2.bendsize = spl->bendsize;

 for(cofs=0; cofs<spl->num; cofs++)
 {
  spl2.value[cofs] = spl->value[cofs];
 }

 if(spl2.oClosed == 1)
 {													// It's a closed spline
  t=location*((float)spl2.num/DIVD_NUM);
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
  t=location*((float)(spl2.num-1)/DIVD_NUM);
  cofs = (int)t;
  t -= cofs;
  cofs4 = cofs - 1;
  cofs2 = cofs + 1;
  cofs3 = cofs + 2;
  if (cofs4<0) cofs4=0;
  if (cofs2>=spl2.num) cofs2=spl2.num-1;
  if (cofs3>=spl2.num) cofs3=spl2.num-1;
 }
 v1 =  (spl2.value[cofs2] - spl2.value[cofs ] ) * spl2.bendsize;
 v2 =  (spl2.value[cofs3] - spl2.value[cofs2] ) * spl2.bendsize + v1;
 v1 += (spl2.value[cofs ] - spl2.value[cofs4] ) * spl2.bendsize;

 t1 = (2*t*t*t-3*t*t+1);
 t2 = (-2*t*t*t+3*t*t);
 t3 = (t*t*t-2*t*t+t);
 t4 = (t*t*t-t*t);

 ret_cor = ( t1*(float)(spl2.value[cofs ])
            +t2*(float)(spl2.value[cofs2])
            +t3*(float)v1
            +t4*(float)v2 );
 return(ret_cor);
}
