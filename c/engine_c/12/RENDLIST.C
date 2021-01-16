int project_object(sObject *o, int renderlistpos)
{
   for (i=0;i<o->numpolys;i++)
   {
    c1 = o->polys[i*3];     // Coordinate set #1
    c2 = o->polys[i*3+1];   // Coordinate set #2
    c3 = o->polys[i*3+2];   // Coordinate set #3
// If any of the Z-coordinates are zero or less, skip the entire poly
   if (   ( z[c1] != -1 )
       && ( z[c2] != -1 )
       && ( z[c3] != -1 ))
   {
    x1 = t[c1*2];
    y1 = t[c1*2+1];
    x2 = t[c2*2];
    y2 = t[c2*2+1];
    x3 = t[c3*2];
    y3 = t[c3*2+1];
    if ( (x2-x1)*(y3-y1)-(x3-x1)*(y2-y1) < 0 )
    {
     renderlist[numvisible].x1 = x1;
     renderlist[numvisible].y1 = x1;
     renderlist[numvisible].x2 = x2;
     renderlist[numvisible].y2 = x2;
     renderlist[numvisible].x3 = x3;
     renderlist[numvisible].y4 = x3;
     renderlist[numvisible].texture = o->texture;
     renderlist[numvisible].zcoord = (short int)( z[c1]+z[c2]+z[c3] );

// -----

void add_renderlist(sObject *o)
{
 int listpos = 0;
 int i;
 int c1,c2,c3;

 if (o->acitive = 1)
 {
  if (project_bounding(o) = 1)
  {
   listpos = project_object(o,listpos);
  }
 }
 bytesort...
 for (i=0;i<listpos;i++)
 {
  drawpoly(i);
 }
}






