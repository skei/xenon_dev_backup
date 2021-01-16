Program Invert;

var f1,f2:file;
    b:byte;
    w:word;

begin
 assign(f1,paramstr(1));
 assign(f2,paramstr(2));
 reset(f1,1);
 rewrite(f2,1);
 for w := 0 to 65535 do
 begin
  blockread(f1,b,1);
  b := 255-b;
  blockwrite(f2,b,1);
 end;
 close(f1);
 close(f2);
end.
