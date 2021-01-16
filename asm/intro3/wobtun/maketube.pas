Program MakeTube;

var
 x, y         : integer;
 r            : integer;
 xy2,z,t,l,sd : double;
 zc           : longint;
 col,col2     : byte;
 xp,yp,adr    : integer;
 off          : integer;
 i : integer;
 bugg : byte;
 f:file;
 screen : array[0..199,0..319] of byte absolute $a000:0;
 data : array[0..99,0..159] of word;

Procedure CalcX;
Begin
 r := 256;      {Radius}
 sd := 256;    {Distance between viewer & Screen}
 adr := 0;
 for y := -99 to 100 do
 begin
  for x := -159 to 160 do
  begin
   z := l * sd;
   if ((x <> 0) and (y <> 0)) then
   begin
    t := arctan( y / x );
    t := 256 * t / ( 2 * pi );
    if ((x <= 0) and (y <= 0)) then t := t + 1;
    if ((x > 0) and (y > 0)) then t := t + 1;
    if (x >= 0) then t := t+128;
   end;
   col := round(t);
   if ((y = 0) and (x < 0)) then col := 1;
   screen[(100+y) div 2,(160+x) div 2] := col;
{   mem[$a000:adr] := col;}
   adr := adr + 1;
  end;
 end;
end;

Procedure CalcY;
Begin
 r := 256;      {Radius}
 sd := 256;    {Distance between viewer & Screen}
 adr := 0;
 for y := -99 to 100 do
 begin
  yp := y;
  for x := -159 to 160 do
  begin
   xy2 := abs(( x * x ) + ( y * y )/0.8);
   xy2 := sqrt( xy2 );
   if xy2 <> 0 then l := r/xy2;
   z := l * sd;
   zc := Trunc( z / 4 );
   zc := zc MOD 256;  {% = DIV eller MOD? AND?}
   col := zc;
   screen[(100+y) div 2,(160+x) div 2] := col;
{   mem[$a000:adr] := col;}
   adr := adr + 1;
  end;
 end;
end;

Procedure SaveScreen(fname:string);
begin
 assign(f,fname);
 rewrite(f,1);
 blockwrite(f,data,sizeof(data));
 close(f);
end;

Begin
 asm
  mov ax,13h
  int 10h
 end;

 calcy;
 for y := 0 to 99 do
 begin
  for x := 0 to 159 do
  begin
   data[y,x] := screen[y,x] * 256;
  end;
 end;

 calcx;
 for y := 0 to 99 do
 begin
  for x := 0 to 159 do
  begin
   data[y,x] := data[y,x] + screen[y,x];
  end;
 end;

 readln;
{ SaveScreen('offsets.raw');}
 asm
  mov ax,3
  int 10h
 end;
end.

