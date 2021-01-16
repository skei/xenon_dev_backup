Program ReduceColor;

Var f1,f2:file;
    b:byte;

begin
 assign(f1,paramstr(1));
 reset(f1,1);
 assign(f2,paramstr(2));
 rewrite(f2,1);
 while not eof(f1) do
 begin
  blockread(f1,b,1);
  if b > 63 then b := 63;
  blockwrite(f2,b,1);
 end;
 close(f1);
 close(f2);
end.
