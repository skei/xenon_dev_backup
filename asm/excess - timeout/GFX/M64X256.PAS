Program m64x256;

var f:file;
    palette : array[0..767] of byte;
    newpal : array[0..16383] of byte;
    x,y:byte;
    qweqwe:byte;

Function SqrRoot(x:longint):word; assembler;
asm
 les bx,X
 mov dx,es
 mov ax,0
 mov cx,-1
@@1:
 add cx,2
 inc ax
 sub bx,cx
 sbb dx,0
 jnb @@1
end;

Procedure GetColor (ColorNo : Byte; Var R,G,B : Byte);
Begin
 Port [$3c7] := ColorNo;
 R := Port [$3c9];
 G := Port [$3c9];
 B := Port [$3c9];
End;


Function ClosestColor(r,g,b:byte):byte;
var i:integer;
    closest:byte;
    r1,g1,b1:byte;
    tc : word;
    t : word;
begin
 closest := 0;
 tc := 65535;
 for i := 0 to 255 do
 begin
  r1 := palette[i*3];
  g1 := palette[i*3+1];
  b1 := palette[i*3+2];
{  GetColor(i,r1,g1,b1);}
  t := sqrRoot((r-r1)*(r-r1) + (g-g1)*(g-g1) + (b-b1)*(b-b1));
  if t < tc then
  begin
   closest := i;
   tc := t;
  end;
 end;
 ClosestColor := closest;
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
 for x := 63 downto 0 do
 begin
  for y := 0 to 255 do
  begin
   qweqwe := closestcolor(palette[y*3],palette[y*3+1],palette[y*3+2]);
   newpal[x*256+y] := qweqwe;
   mem[$a000:y*320+x]:= qweqwe;
  end;
 end;
 asm
  mov ax,3h
  int 10h
 end;

 assign(f,paramstr(2));
 rewrite(f,1);
 blockwrite(f,newpal,sizeof(newpal));
 close(f);
end.


