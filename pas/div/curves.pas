{ I've seen a lot of Spline and B-Spline code floating around.  Most of it
 is very slow even running with a math unit.  This code is quite reasonably
 fast and will draw very accurate 3-point Splines at a rate of about 180
 splines per second on my 386/40 with _no_ math unit.

 Enjoy!}

Program CUrves; {Spline curve demo (C) 1994 by Wil Barath}
Uses Tgr12; {Mode 12h graphics unit by me available from SWAG}

Procedure Spline(x1,y1,x2,y2,x3,y3:Integer);
Var sx,sy,dx1,dx2,dy1,dy2,sdx,sdy,l:LongInt;deep,deep2,ox,oy:Integer;
Begin
  dx1:=(x2-x1);
  dx2:=(x3-x2);
  dy1:=(y2-y1);
  dy2:=(y3-y2);
  Deep:=32;
  deep2:=Deep*Deep;
  sdx:=dx1*Pred(deep*2)-dx2;
  sdy:=dy1*Pred(deep*2)-dy2;
  Inc(dx1,dx1);
  Inc(dx2,dx2);
  Inc(dy1,dy1);
  Inc(dy2,dy2);
  sx:=0;
  sy:=0;
  x2:=x1;
  y2:=y1;
  For l:=0 to Deep do
  Begin
   ox:=x2;
   oy:=y2;
   x2:=x1+sx Div Deep2;
   y2:=y1+sy Div Deep2;
   Line(x2,y2,ox,oy);
   Inc(sx,sdx);
   Inc(sy,sdy);
   Inc(sdx,dx2-dx1);
   Inc(sdy,dy2-dy1);
  end;
  Line(x2,y2,x3,y3);
end;
COnst
  Tail=30;
  m:Word=1;

Begin
  VideoMode($12);
  Spline(10,10,1000,240,10,470);
  ReadLn;
  VideoMode($3);
end.
