Program Test;

uses crt;

var c:char;
    x1,y1,x2,y2:word;

procedure line (fromx,fromy,tox,toy,color : integer);
var temp,dx,dy,xincbefore,xincafter,yincbefore,yincafter : integer;
    curpoint,accumul : integer;
begin
 xincbefore := 1; xincafter := 0;
 yincbefore := 0; yincafter := 1;
 dx := tox-fromx;
 if dx<0 then
 begin
  dx := abs(dx);
  xincbefore := -1;
 end;
 dy := toy-fromy;
 if dy<0 then
 begin
  dy := abs(dy);
  yincafter := -1;
 end;
 if dx<dy then
 begin
  temp := dx;
  dx := dy;
  dy := temp;
  xincafter := xincbefore; xincbefore := 0;
  yincbefore := yincafter ; yincafter := 0;
 end;
 accumul := dx div 2;
 for curpoint := 1 to dx do
 begin
  mem[$a000:fromy*320+fromx] := color;
  inc(fromx,xincbefore);
  inc(fromy,yincbefore);
  inc(accumul,dy);
  if accumul > dx then
  begin
   dec(accumul,dx);
   inc(fromx,xincafter);
   inc(fromy,yincafter);
  end;
 end;
end;

Begin
 asm
  mov ax,13h
  int 10h
 end;
 randomize;
 repeat
  x1 := random(320);
  y1 := random(200);
  x2 := random(320);
  y2 := random(200);
  line(x1,y1,x2,y2,1);
  x1 := (x1 div 2)*2;
  x2 := (x2 div 2)*2;
  line(x1,y1,x2,y2,2);


  c := readkey;
 until c = #27;
 asm
  mov ax,3h
  int 10h
 end;
end.
