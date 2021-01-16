Program ShowBump;

var bf:file;
    x,y:integer;
    b:byte;

Procedure SetColor(i,r,g,b:byte);
begin
 port[$3C8]:=i;
 port[$3C9]:=r;
 port[$3C9]:=g;
 port[$3C9]:=b;
end;

begin
 asm
  mov ax,13h
  int 10h
 end;
 for x :=0 to 63 do SetColor(x,x,0,0);
 assign(bf,paramstr(1));
 reset(bf,1);
 for y := 0 to 199 do
 begin
  for x := 0 to 255 do
  begin
   blockread(bf,b,1);
   mem[$a000:y*320+x] := 16+b;
   blockread(bf,b,1);

  end;
 end;
 close(bf);
 readln;
 asm
  mov ax,3h
  int 10h
 end;
end.



