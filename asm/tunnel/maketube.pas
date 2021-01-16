Program MakeTube;

var
 x, y         : integer;
 r            : integer;
 xy2,z,t,l,sd : double;
 zc           : longint;
 col,col2     : byte;
 xp,yp,adr    : integer;
 off          : integer;
 fx,fy        : file;
 i : integer;
 bugg : byte;

procedure SetMode(n:word);
begin
 asm
  mov ax,n
  int 10h
 end;
end;

procedure SetRGB(color,r,g,b : byte);
begin
 asm
  cli
  mov dx,$3C8
  mov al,color
  out dx,al
  inc dx
  mov al,r
  out dx,al
  mov al,g
  out dx,al
  mov al,b
  out dx,al
  sti
 end;
end;

Procedure CalcX;
begin
 r := 128;      {Radius}
 sd := 256;    {Distance between viewer & Screen}
 adr := 0;
 for y := -99 to 100 do
 begin
  for x := -159 to 160 do
  begin
   xy2 := abs(( x * x ) + ( y * y )/0.8);
   xy2 := sqrt( xy2 );
   if xy2 <> 0 then l := r/xy2;
   z := l * sd;
   col := Trunc(z/4) MOD 256;
   mem[$a000:adr] := col;
  adr := adr + 1;
  end;
 end;
end;

Procedure CalcY;
Begin
 r := 128;
 sd := 256;
 adr := 0;
 for y := -99 to 100 do
 begin
  for x := -159 to 160 do
  begin
   if ((x <> 0) and (y <> 0)) then
   begin
    t := arctan(y/x);
    t := t / ( 2 * pi );
    if ((x <= 0) and (y <= 0)) then t := t + 1;
    if ((x > 0) and (y > 0)) then t := t + 1;
    if (x >= 0) then t := t+128;
   end;
   col := round(t);
   if ((y = 0) and (x < 0)) then col := 1;
   mem[$a000:adr] := col;
   adr := adr + 1;
  end;
 end;
end;

Begin
 SetMode($13);
 CalcX;
 ReadLn;
 CalcY;
 readln;
 SetMode(3);
end.
