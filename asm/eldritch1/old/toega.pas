Program toEGA;

Const Height = 20;
      Width = 40;

var f1,f2 : file;
    x,y,j : word;
    b : byte;
    buffer : array[0..Width-1] of byte;
    screen : array[0..199,0..319] of byte absolute $a000:0;
    y1,y2 : word;
    c:integer;

begin
 asm
  mov ax,13h
  int 10h
 end;
 assign(f1,paramstr(1));
 reset(f1,1);
 blockread(f1,screen,64000);
 seek(f1,0);
 assign(f2,paramstr(2));
 rewrite(f2,1);
 val(paramstr(3),y1,c);
 val(paramstr(4),y2,c);
 for y := y1 to y2 do
 begin
  for x := 0 to Width-1 do
  begin
   b := 0;
   if mem[$a000:y*320+x*8+0] <> 0 then inc(b,128);
   if mem[$a000:y*320+x*8+1] <> 0 then inc(b,64);
   if mem[$a000:y*320+x*8+2] <> 0 then inc(b,32);
   if mem[$a000:y*320+x*8+3] <> 0 then inc(b,16);
   if mem[$a000:y*320+x*8+4] <> 0 then inc(b,8);
   if mem[$a000:y*320+x*8+5] <> 0 then inc(b,4);
   if mem[$a000:y*320+x*8+6] <> 0 then inc(b,2);
   if mem[$a000:y*320+x*8+7] <> 0 then inc(b,1);
   buffer[x] := b;
   for j := 0 to 7 do mem[$a000:y*320+x*8+j] := 15;
  end;
  blockwrite(f2,buffer,sizeof(buffer));
 end;
 close(f1);
 close(f2);
 readln;
 asm
  mov ax,3h
  int 10h
 end;
end.
