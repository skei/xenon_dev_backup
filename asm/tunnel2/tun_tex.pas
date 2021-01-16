Program CalcTunnelTexture;

Var RandSeed : word;
    i,x,y : word;
    c : byte;

function ncol(mc,n,dvd:integer):byte;
begin
 ncol:=((mc+n-random(n)) div dvd) mod 63;
end;

procedure subdivide(x1,y1,x2,y2:word);
var xn,yn,dxy,p1,p2,p3,p4:word;
begin
 if (x2-x1<2) and (y2-y1<2) then exit;
 p1:=mem[$a000:320*y1+x1];
 p2:=mem[$a000:320*y2+x1];
 p3:=mem[$a000:320*y1+x2];
 p4:=mem[$a000:320*y2+x2];
 xn:=(x2+x1) shr 1;
 yn:=(y2+y1) shr 1;
 dxy:=(x2-x1+y2-y1);
 if mem[$a000:320*y1+xn]=0 then mem[$a000:320*y1+xn]:=ncol(p1+p3,dxy,2);
 if mem[$a000:320*yn+x1]=0 then mem[$a000:320*yn+x1]:=ncol(p1+p2,dxy,2);
 if mem[$a000:320*yn+x2]=0 then mem[$a000:320*yn+x2]:=ncol(p3+p4,dxy,2);
 if mem[$a000:320*y2+xn]=0 then mem[$a000:320*y2+xn]:=ncol(p2+p4,dxy,2);
 mem[$a000:320*yn+xn]:=ncol(p1+p2+p3+p4,dxy,4);
 subdivide(x1,y1,xn,yn); subdivide(xn,y1,x2,yn);
 subdivide(x1,yn,xn,y2); subdivide(xn,yn,x2,y2);
end;


procedure setpal(nummer,r,g,b:byte);
begin
 asm
  mov dx, 3c8h
  mov al, Nummer
  out dx, al
  inc dx
  mov al, R
  out dx, al
  mov al, G
  out dx, al
  mov al, B
  out dx, al
 end
end;

Function Random : Word;
begin
 asm
  mov ax,[RandSeed]
  mov dx,8405h
  mul dx
  inc ax
  mov [RandSeed],ax
  mov @result,dx
 end;
end;

Procedure Smooth;
begin
 for x := 320 to 63999-320 do
 begin
  c := (mem[$a000:x+320] + mem[$a000:x-320] + mem[$a000:x+1] + mem[$a000:x-1]) shr 2;
  mem[$a000:x] := c;
 end;
end;

Begin
 asm
  mov ax,13h
  int 10h
 end;
 for i := 0 to 63 do
 begin
  SetPal(i,    i,   i div 2   ,i);
 end;
 subdivide(0,0,319,199);
 smooth;
 readln;
 asm
  mov ax,3
  int 10h
 end;
end.


