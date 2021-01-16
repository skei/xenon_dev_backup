Program CText2;

var screen : array[0..199,0..319] of byte absolute $a000:0;
    f:file;
    x,y : integer;
    color : byte;

Begin
 asm
  mov ax,13h
  int 10h
 end;
 for y := 0 to 127 do
 begin
  for x := 0 to 127 do
  begin
   color := round(sin((x/128)*pi) * sin((y/128)*pi) * 63);
   screen[y,x] := color;
  end;
 end;
 readln;

 assign(f,'tex2.raw');
 rewrite(f,1);
 blockwrite(f,screen,sizeof(screen));
 close(f);

 asm
  mov ax,3
  int 10h
 end;
end.