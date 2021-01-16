{PreKalkulering av EnvMap-Texture}

Program CalcTex;

var
 r            : integer;
 xy2,t,l,sd : double;
 zc           : longint;
 col,col2     : byte;
 xp,yp,adr    : integer;
 off          : integer;
 screen       : array[0..199,0..319] of byte absolute $a000:0;
 i : integer;
 bugg : byte;


var x,y,z : integer;
    color : byte;
    dummy : array[0..127] of byte;

    f:file;

procedure setpalette(n,r,g,b:byte);
begin
 port[$3c8]:=n;
 port[$3c9]:=r;
 port[$3c9]:=g;
 port[$3c9]:=b;
end;

Procedure CTex;
var leng : real;
    xx,yy,zz :integer;
begin
 for x := 0 to 127 do dummy[x] := 0;
 for y := 0 to 127 do
 begin
  for x := 0 to 127 do
  begin
   z := (sqr(y-64)+sqr(x-64)) SHR 5;
   z := 127-z;
   xx := x-64;
   yy := y-64;
   zz := z-64;
   leng:=sqrt(abs(longint(xx)*longint(xx)+longint(yy)*longint(yy)+longint(zz)*longint(zz)));
   if leng <> 0 then
   begin
    vx := round( ( zz/leng ) * 63 );
    vy := round
   end;
   screen[y,x] := color;
  end;
 end;
end;


Begin
 asm
  mov ax,13h
  int 10h
 end;
 for x := 0 to 63 do setpalette(x,0,x,x);
 CTex;
 readln;
 asm
  mov ax,3h
  int 10h
 end;
end.

{...................................................}




