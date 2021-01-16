Program Wobble;

Uses crt;

Const Xouteradder = 2;
      Youteradder = 3;
      Xinneradder = 6;
      Yinneradder = 3;
      xoadder = 10;
      yoadder = 5;

var screen : array[0..199,0..319] of byte absolute $a000:0;
    texture : array[0..127,0..127] of byte;
    palette : array[0..767] of byte;
    Sine : array[0..255] of byte;
    Xpos,Ypos : byte;
    xsinus,ysinus : byte;
    Xouter,YOuter,xo,yo : byte;
    x,y,c : byte;
    di,w : word;
    f:file;
    xoaddersin : byte;
    yoaddersin:byte;

Label Yloop,Xloop;

{----- Kalkul‚r Sinus LookUp tabell -----}
Procedure CalcSinus;
var x : word;
    y : real;
Begin
  y := -pi;
  For x := 0 to 255 DO
  Begin
   Sine[x]:=Round(sin(y)*7);
   y := y + (pi*2 / 256)
  End;
end;

{----- MAIN -----}

Begin
 CalcSinus;
 assign(f,'smalbear.raw');
 reset(f,1);
 blockread(f,texture,sizeof(texture));
 close(f);
 assign(f,'smalbear.pal');
 reset(f,1);
 blockread(f,palette,sizeof(palette));
 close(f);
 asm
  mov ax,13h
  int 10h
 end;
 port[$3c8] := 0;
 for w := 0 to 767 do port[$3c9] := palette[w];
 repeat
  di := 0;
  y := 0;
  xouter := xo;
  youter := yo;
 Yloop:
  inc (xouter,xouteradder);
  xsinus := xouter;
  inc (youter,youteradder);
  ySinus := youter;
  x := 0;
 XLoop:
  xpos := x;
  inc (xpos,sine[xsinus]);
  ypos := y;
  inc (ypos,sine[ysinus]);
  c := texture[ypos,xpos];
  w := c SHL 8;
  inc (w,c);
  memw[$a000:di] := w;
  memw[$a000:di+320] := w;

  inc(xsinus,xinneradder);
  inc(ysinus,yinneradder);

  inc(di,2);
  inc (x);
  if x < 128 then Goto XLoop;
  inc(di,320+64);

  inc (y);
  if y < 100 then Goto YLoop;

  inc (xo,sine[xoadder]);
  inc(xoaddersin,2);
  inc (yo,sine[yoadder]);
  inc(xoaddersin,4);

  inc(yo,yoadder);

 Until KeyPressed;

 asm
  mov ax,3
  int 10h
 end;
end.

