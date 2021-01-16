Program MakeBump;

var w:word;
    f:file;
    i,j,k:integer;
    b : array[0..15] of word;
    b1,b2,b3 : array[0..255] of word;

begin
 fillchar(b,sizeof(b),#0);
 for i := 0 to 255 do
 begin
  b1[i] := 10;
  b2[i] := 20;
  b3[i] := 30;
 end;
 b[0] := 10*256;
 b[1] := 20*256;
 b[2] := 30*256;
 b[3] := 20*256;
 b[4] := 10*256;
 assign(f,'bump.raw');
 rewrite(f,1);
 for i := 0 to 15 do
 begin
  blockwrite(f,b1,sizeof(b1));
  blockwrite(f,b2,sizeof(b2));
  blockwrite(f,b3,sizeof(b3));
  blockwrite(f,b2,sizeof(b2));
  blockwrite(f,b1,sizeof(b1));
  for k := 0 to 10 do for j := 0 to 15 do blockwrite(f,b,sizeof(b));
 end;
 close(f);
end.



