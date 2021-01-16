Program Plane;

var f1,f2:file;
    i : integer;
    s : string;
    r : array[0..3] of byte;

begin
 if paramcount <> 2 then
 begin
  writeln ('Usage: PLANE <infile> <outfile w/o ext>');
  Writeln ('Four files will be created, with extensions 1 to 4');
  halt(1);
 end;
 assign(f1,paramstr(1));
 reset(f1,1);
{Plane 1}
 s := paramstr(2)+'.1';
 Writeln ('Plane #1 : ',s);
 assign(f2,s);
 rewrite(f2,1);
 while not eof(f1) do
 begin
  blockread(f1,r,4);
  blockwrite(f2,r[0],1);
 end;
 close(f2);
{Plane 2}
 seek(f1,0);
 s := paramstr(2)+'.2';
 Writeln ('Plane #2 : ',s);
 assign(f2,s);
 rewrite(f2,1);
 while not eof(f1) do
 begin
  blockread(f1,r,4);
  blockwrite(f2,r[1],1);
 end;
 close(f2);
{Plane 3}
 seek(f1,0);
 s := paramstr(2)+'.3';
 Writeln ('Plane #3 : ',s);
 assign(f2,s);
 rewrite(f2,1);
 while not eof(f1) do
 begin
  blockread(f1,r,4);
  blockwrite(f2,r[2],1);
 end;
 close(f2);
{Plane 4}
 seek(f1,0);
 s := paramstr(2)+'.4';
 Writeln ('Plane #4 : ',s);
 assign(f2,s);
 rewrite(f2,1);
 while not eof(f1) do
 begin
  blockread(f1,r,4);
  blockwrite(f2,r[3],1);
 end;
 close(f2);
 close(f1);
end.

