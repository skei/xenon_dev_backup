{PreKalkulering av EnvMap-Texture}

Program CalcTex;

var x,y : integer;
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

Begin
 for x := 0 to 127 do dummy[x] := 0;
 asm
  mov ax,13h
  int 10h
 end;
 assign(f,'tex.raw');
 rewrite(f,1);
 for x := 0 to 31 do setpalette(x,0,x*2,x*2);

 for y := 0 to 127 do
 begin
  for x := 0 to 127 do
  begin
   color := (sqr(y-64)+sqr(x-64)) SHR 7;
   color := 31-color;
   if color > 31 then color := 0;
   if y < 200 then mem[$A000:y*320+x] := color;
   blockwrite(f,color,1);
  end;
  blockwrite(f,dummy,128);
 end;
 close(f);
 readln;
 asm
  mov ax,3h
  int 10h
 end;
end.

