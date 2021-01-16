Program Calc1;

const m = 31;
      a = 0;

var sine : array[0..255] of shortint;
    cosine : array[0..255] of shortint;
    i:integer;
    x,x2,w:word;
    xx,xx2:word;


Procedure CalcSinus;
var x : word;
    y : real;
Begin
  y := -pi;
  For x := 0 to 255 DO
  Begin
   Sine[x]:=Round(sin(y)*m)+a;
   coSine[x]:=Round(cos(y)*m)+a;
   y := y + (pi*2 / 255)
  End;
end;


begin
 asm
  mov ax,13h
  int 10h
 end;
 CalcSinus;
 for w := 0 to 127 do
 begin
  x := cosine[w] + 160;
  x2 := 160-cosine[w];
  mem [$a000:w*320+x] := 15;
  mem [$a000:w*320+x2] := 15;

  x := sine[w] + 160;
  x2 := 160-sine[w];
  mem [$a000:w*320+x] := 1;
  mem [$a000:w*320+x2] := 1;

 end;
 readln;
 asm
  mov ax,13h
  int 10h
 end;
end.