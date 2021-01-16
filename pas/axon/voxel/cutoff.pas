Program CutOff;

var f1,f2:file;
    i:longint;  {Index into file}
    s:longint;  {size of block to save}
    rc:word;
    b:byte;
    w:longint;

Begin
 Writeln ('CUTOFF in-file out-fileindex size - (C)1995 Axon - SLOW VERSION! :)');
 assign(f1,paramstr(1));
 assign(f2,paramstr(2));
 reset(f1,1);
 rewrite(f2,1);
 val(paramstr(3),i,rc);
 if rc <> 0 then halt(1);
 val(paramstr(4),s,rc);
 if rc <> 0 then halt(1);
 seek(f1,i);
 for w := 1 to s do
 begin
  blockread(f1,b,1);
  blockwrite(f2,b,1);
 end;
 close(f1);
 close(f2);
end.

