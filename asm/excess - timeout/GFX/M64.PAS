Program m64x256;

const Diffuse = 50;

var f:file;
 palette : array[0..767] of byte;
 newpal : array[0..16383] of byte;
 x,y:byte;
 rr,gg,bb : integer;
 np : byte;
 span : byte;

var sqrparam : longint;
    sqrreturn : word;

var r,g,b:byte;
    closestcolorreturn:byte;
var i:integer;
    closest:byte;
    r1,g1,b1:byte;
    tc : word;
    t : word;


Procedure SqrRoot; assembler;
asm
 les bx,sqrparam
 mov dx,es
 mov ax,0
 mov cx,-1
@@1:
 add cx,2
 inc ax
 sub bx,cx
 sbb dx,0
 jnb @@1
 mov [sqrreturn],ax
end;

Procedure ClosestColor;
begin
 closest := 0;
 tc := 65535;
 for i := 0 to 255 do
 begin
  r1 := palette[i*3];
  g1 := palette[i*3+1];
  b1 := palette[i*3+2];
  sqrparam :=((r-r1)*(r-r1) + (g-g1)*(g-g1) + (b-b1)*(b-b1));
  SqrRoot;
  t := sqrreturn;
  if t < tc then
  begin
   closest := i;
   tc := t;
  end;
 end;
 ClosestColorreturn := closest;
end;

begin
 assign (f,paramstr(1));
 reset(f,1);
 blockread(f,palette,768);
 close(f);
 asm
  mov ax,13h
  int 10h
 end;
{Highlight}
 span := 63-diffuse+1;
 for x := diffuse to 63 do
 begin
  for y := 0 to 255 do
  begin
   r := (palette[y*3] * x) div (63-diffuse+1);
   g := (palette[y*3+1] * x) div (63-diffuse+1);
   b := (palette[y*3+2] * x) div (63-diffuse+1);
   closestcolor;
   np := closestcolorreturn;
   newpal[x*256+y] := np;
   mem[$a000:(y div 2)*320+x] := np;
  end;
 end;

 for x := diffuse-1 downto 0 do
 begin
  for y := 0 to 255 do
  begin
   r := (palette[y*3] * x) div diffuse;
   g := (palette[y*3+1] * x) div diffuse;
   b := (palette[y*3+2] * x) div diffuse;
   closestcolor;
   np := closestcolorreturn;
   newpal[x*256+y] := np;
  end;
 end;
 readln;
 asm
  mov ax,3h
  int 10h
 end;
 assign(f,paramstr(2));
 rewrite(f,1);
 blockwrite(f,newpal,sizeof(newpal));
 close(f);
end.


