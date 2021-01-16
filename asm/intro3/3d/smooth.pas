Program Smooth;

uses crt;

var f1,f2:file;
    l:longint;
    i:integer;
    b1,b2:shortint;

begin
 assign(f1,'bump.map');
 reset(f1,1);
 assign(f2,'bump2.map');
 rewrite(f2,1);
 l := 0;
 while not eof(f1) do
 begin
  gotoxy (5,5);
  writeln (l);
  inc(l);
  blockread(f1,i,2);
  b1 := (i and $ff00) * 2;
  b2 := (i and $00ff) * 2;
  blockwrite(f2,b1,1);
  blockwrite(f2,b2,1);
 end;
 close(f1);
 close(f2);
end.


