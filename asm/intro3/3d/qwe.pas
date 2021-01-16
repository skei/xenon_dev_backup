Program qwe;

var f,f1,f2:file;
    b:byte;

begin
 assign(f,paramstr(1));
 reset(f,1);
 assign(f1,'1.qwe');
 rewrite(f1,1);
 assign(f2,'2.qwe');
 rewrite(f2,1);

 while not eof(f) do
 begin
  blockread(f,b,1);
  blockwrite(f1,b,1);
  blockread(f,b,1);
  blockwrite(f2,b,1);
 end;
 close(f);
 close(f1);
 close(f2);
end.



