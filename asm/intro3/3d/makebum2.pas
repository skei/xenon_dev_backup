Program MakeBmp2;

Uses vesa;

const dvd = 1;
var i:integer;

function ncol(mc,n,dvd : integer): integer;
var loc : integer;
begin
 loc:=(mc+n-random(2*n)) div dvd; ncol:=loc;
 if loc>250 then ncol:=250; if loc<5 then ncol:=5
end;

procedure plasma(x1,y1,x2,y2 : word);
var xn,yn,dxy,p1,p2,p3,p4 : word;
begin
 if (x2-x1<2) and (y2-y1<2) then EXIT;
 p1 := GetPixel(x1,y1);
 p2 := GetPixel(x1,y2);
 p3 := GetPixel(x2,y1);
 p4 := GetPixel(x2,y2);
 xn:=(x2+x1) shr 1; yn:=(y2+y1) shr 1;
 dxy:=5*(x2-x1+y2-y1) div 3;
 if GetPixel(xn,y1)=0 then PutPixel(xn,y1,ncol(p1+p3,dxy,2));
 if GetPixel(x1,yn)=0 then PutPixel(x1,yn,ncol(p1+p2,dxy,2));
 if GetPixel(x2,yn)=0 then PutPixel(x2,yn,ncol(p3+p4,dxy,2));
 if GetPixel(xn,y2)=0 then PutPixel(xn,y2,ncol(p2+p4,dxy,2));
 PutPixel(xn,yn,ncol(p1+p2+p3+p4,dxy,4));
 plasma(x1,y1,xn,yn); plasma(xn,y1,x2,yn);
 plasma(x1,yn,xn,y2); plasma(xn,yn,x2,y2);
end;

begin
 ChangeMode($100);
 plasma(0,0,256,256);
 readln;
 SetText;
end.
