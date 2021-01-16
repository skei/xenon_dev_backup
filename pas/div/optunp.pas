Program OptUnp;

Uses Crt;

Const BufferSize = 1024;

var f1,f2:file;
    buffer : array[1..BufferSize] of byte;
    size,offset:longint;
    Fname:string[12];
    c : char;
    Quit : boolean;
    action:char;
    i,j:longint;
    oldoff : longint;

begin
 assign(f1,'optimal.dat');
 reset(f1,1);
 Quit := false;
 repeat
  blockread(f1,offset,sizeof(offset));
  blockread(f1,size,sizeof(size));
  fname := '';d
  for i := 1 to 13 do
  begin
   blockread(f1,c,1);
   if c <> #0 then fname := fname + c;
  end;
  Writeln (fname,' þ size: ',size,' þ Offset: ',offset);
  Writeln ('(S)ave or any other key to Quit');
  action := ReadKey;
  if upcase(action) = 'S' then
  begin
   assign(f2,fname);
   rewrite(f2,1);
   oldoff := filepos(f1);
   seek(f1,offset);
   j := size div 1024;
   for i := 1 to j do
   begin
    blockread(f1,buffer,Buffersize);
    blockwrite(f2,buffer,buffersize);
   end;
   blockread(f1,buffer,size-(j*1024));
   blockwrite(f2,buffer,size-(j*1024));
   close(f2);
   seek(f1,oldoff);
  end
  else Quit := true;
 until quit;
 close(f1);
end.
