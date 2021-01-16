Program Split;

Const bufferSize = 1024;

var f1,f2:file;
    i,j,index,count:longint;
    retcode:integer;
    buffer : array[0..BufferSize-1] of byte;

Procedure Error (s:String);
begin
 writeln;
 writeln(s);
 halt(1);
end;


Begin
 if paramcount <> 4 then error ('USAGE: Split <InFile> <OutFile> <Bytes to skip> <bytes to save>');
 assign(f1,paramstr(1));
 reset(f1,1);
 if IOResult <> 0 then error('ERROR: InFile');
 assign(f2,paramstr(2));
 rewrite(f2,1);
 if IOResult <> 0 then error('ERROR: OutFile');
 val(paramstr(3),index,retcode);
 if retcode <> 0 then error('ERROR: Bytes to skip');
 val(paramstr(4),count,retcode);
 if retcode <> 0 then error('ERROR: Bytes to save');

 seek(f1,index);

 j := count div BufferSize;
 for i := 1 to j do
 begin
  blockread(f1,buffer,Buffersize);
  blockwrite(f2,buffer,buffersize);
 end;
 blockread(f1,buffer,count-(j*BufferSize));
 blockwrite(f2,buffer,count-(j*BufferSize));

 close(f1);
 close(f2);
end.


