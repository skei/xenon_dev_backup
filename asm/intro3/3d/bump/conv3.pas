Program Convert;

Const MaxArraySize = 8192;

Var IncludePointNormals : boolean;
    IncludeFaceNormals : boolean;
    EnMult : integer;
    LoadMult : real;
    leng:real;

Type BigArray = Array[0..MaxArraySize-1,0..2] of integer;

Var Points : ^BigArray;
var FNormals : ^BigArray;
var Faces : ^BigArray;
var Normals : ^BigArray;

Var infile, outfile : text;
    inndata : string;
    Multiplier : real;
    mult : real;
    r1,r2,r3 : real;
    d1,d2,d3 : integer;
    dum : integer;
    dummy : string;
    NumPoints : integer;
    NumFaces  : integer;
    nv,nf : integer;
    LargestPoint : longint;

var i   : integer;
    a   : longint;

var f   : text;
    st1 : string;
    st2 : string;

{Read from ASC file}

Function GetString : string;
var d : string;
begin
 d := '';
 repeat
  d := d + inndata[1];
  delete(inndata,1,1);
 until (inndata[1] = ' ') or (length(inndata)<1);
 getstring := d;
end;

Procedure GetV(var inndata:string);
begin
 delete(inndata,1,pos('X:',inndata)+2);
 val(GetString,r1,dum);
 delete(inndata,1,pos('Y:',inndata)+2);
 val(GetString,r2,dum);
 delete(inndata,1,pos('Z:',inndata)+2);
 val(GetString,r3,dum);
 d1 := round(r1*LoadMult);
 d2 := round(r2*LoadMult);
 d3 := round(r3*LoadMult);
 Writeln ('Coordinate: ',nv,' : ',d1,',',d2,',',d3);
 if abs(d1) > LargestPoint then LargestPoint := abs(d1);
 Points^[nv,0] := d1;
 if abs(d2) > LargestPoint then LargestPoint := abs(d2);
 Points^[nv,1] := d2;
 if abs(d3) > LargestPoint then LargestPoint := abs(d3);
 Points^[nv,2] := d3;
 inc(nv);
end;

Procedure GetF(var inndata:string);
begin
 delete(inndata,1,pos('A:',inndata)+1);
 val(GetString,d1,dum);
 delete(inndata,1,pos('B:',inndata)+1);
 val(GetString,d2,dum);
 delete(inndata,1,pos('C:',inndata)+1);
 val(GetString,d3,dum);
 Writeln ('Face: ',nf,' : ',d1,',',d2,',',d3);
 Faces^[nf,0] := d1;
 Faces^[nf,1] := d2;
 Faces^[nf,2] := d3;
 inc(nf);
end;

Procedure ReadData;
begin
 assign(infile,paramstr(1)+'.ASC');
 reset(infile);
 LargestPoint := 0;
 nv := 0;
 nf := 0;
 while not eof(infile) do
 begin
  readln(infile,inndata);
  if pos('list',inndata) = 0 then
  begin
   if (copy(inndata,1,6) = 'Vertex') then GetV(inndata);
   if (copy(inndata,1,4) = 'Face') then GetF(inndata);
  end;
 end;
 close(infile);
 NumFaces := nf;
 NumPoints := nv;
end;

Procedure Multiply;
begin
 Writeln;
 Writeln ('Largest Point value = ABS(',LargestPoint,')');
 Writeln ('Enter value to MULTIPLY each vertex with (ex. 1.0)');
 ReadLn(MultiPlier);
 for i := 0 to NumPoints-1 do
 begin
  Points^[i,0] := Round(Points^[i,0]*Multiplier);
  Points^[i,1] := Round(Points^[i,1]*Multiplier);
  Points^[i,2] := Round(Points^[i,2]*Multiplier);
 end;
end;

Procedure Center;
var Xmax,Xmin,Ymax,Ymin,Zmax,Zmin : integer;
var Xmove, YMove, ZMove : integer;
var yn : char;
begin
 Xmax := -32767; Xmin := 32767;
 Ymax := -32767; Ymin := 32767;
 Zmax := -32767; Zmin := 32767;
 for i := 0 to NumPoints-1 do
 begin
  if Points^[i,0] > Xmax then Xmax := Points^[i,0];
  if Points^[i,0] < Xmin then Xmin := Points^[i,0];
  if Points^[i,1] > Ymax then Ymax := Points^[i,1];
  if Points^[i,1] < Ymin then Ymin := Points^[i,1];
  if Points^[i,2] > Zmax then Zmax := Points^[i,2];
  if Points^[i,2] < Zmin then Zmin := Points^[i,2];
 end;
 Writeln;
 Writeln ('Attempt to Center object? (Y/N)');
 Readln(yn);
 if upcase(yn) = 'Y' then
 begin
  Xmove := Xmin+((Xmax-Xmin) div 2); {Trekkes fra alle X}
  Ymove := Ymin+((Ymax-Ymin) div 2);
  Zmove := Zmin+((Zmax-Zmin) div 2);
  for i := 0 to NumPoints-1 do
  begin
   dec(Points^[i,0],Xmove);
   dec(Points^[i,1],Ymove);
   dec(Points^[i,2],Zmove);
  end;
 end;
end;

{ Calculate Face & vertexNormals}

procedure calcfakenormals;
var num,i:integer;
    kx,ky,kz,ax,ay,az,bx,by,bz:longint;
    lengd:real;
begin
 for i:=0 to NumPoints-1 do
 begin
  ax:=0;
  ay:=0;
  az:=0;
  num:=0;
  for a:=0 to numfaces-1 do
  begin
   if faces^[a,0]=i then
   begin
    ax:=ax+normals^[a,0];
    ay:=ay+normals^[a,1];
    az:=az+normals^[a,2];
    inc(num);
   end;
   if faces^[a,1]=i then
   begin
    ax:=ax+normals^[a,0];
    ay:=ay+normals^[a,1];
    az:=az+normals^[a,2];
    inc(num);
   end;
   if faces^[a,2]=i then
   begin
    ax:=ax+normals^[a,0];
    ay:=ay+normals^[a,1];
    az:=az+normals^[a,2];
    inc(num);
   end;
  end;
  if num <> 0 then
  begin
   ax:=ax div num;
   ay:=ay div num;
   az:=az div num;
  end;
  lengd:=sqrt(abs(longint(ax)*longint(ax) + longint(ay)*longint(ay) + longint(az)*longint(az)));
  if lengd <> 0 then
  begin
   fnormals^[i,0]:=round( ( ax/lengd ) * EnMult ) ;
   fnormals^[i,1]:=round( ( ay/lengd ) * EnMult ) ;
   fnormals^[i,2]:=round( ( az/lengd ) * EnMult ) ;
  end
  else
  begin
   fnormals^[i,0]:=0;
   fnormals^[i,1]:=0;
   fnormals^[i,2]:=0;
  end;
 end;
end;

procedure calcnormals;
var i:integer;
    kx,ky,kz:longint;
    ax,ay,az:longint;
    bx,by,bz:longint;
begin
 for i:=0 to numfaces-1 do
 begin
  ax:=points^[faces^[i,1],0]-points^[faces^[i,0],0];
  ay:=points^[faces^[i,1],1]-points^[faces^[i,0],1];
  az:=points^[faces^[i,1],2]-points^[faces^[i,0],2];
  bx:=points^[faces^[i,2],0]-points^[faces^[i,0],0];
  by:=points^[faces^[i,2],1]-points^[faces^[i,0],1];
  bz:=points^[faces^[i,2],2]-points^[faces^[i,0],2];
  kx:=(ay*bz)-(az*by);
  ky:=(az*bx)-(ax*bz);
  kz:=(ax*by)-(ay*bx);
  leng:=sqrt(abs(longint(kx)*longint(kx)+longint(ky)*longint(ky)+longint(kz)*longint(kz)));
  if leng <> 0 then
  begin
   normals^[i,0]:=round( ( kx/leng ) * EnMult ) ;
   normals^[i,1]:=round( ( ky/leng ) * EnMult ) ;
   normals^[i,2]:=round( ( kz/leng ) * EnMult ) ;
  end
  else
  begin
   normals^[i,0]:=0;
   normals^[i,1]:=0;
   normals^[i,2]:=0;
  end;
 end;
end;

Procedure WriteToFile;
begin
 assign(f,ParamStr(1)+'.INC');
 rewrite(f);
 Writeln (f,';3DStudio -> INC converter by Axon/Xenon Development');
 Str(NumPoints,st1);
 Writeln (f,';NumPoints: '+st1);
 Str(NumFaces,st1);
 Writeln (f,';NumFaces: '+st1);
 Writeln (f,';컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴');
 writeln (f,'; Points');
 Writeln (f,';컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴');
 Writeln (f,'Label Coords'+paramstr(6)+' Word');
 for i := 0 to NumPoints-1 do
 begin
  str(Points^[i,0],st1);
  st2 := 'DW '+st1;
  str(Points^[i,1],st1);
  st2 := st2+', '+st1;
  str(Points^[i,2],st1);
  st2 := st2+', '+st1;
  st2 := st2+',0';
  Writeln (f,st2);
 end;
 if IncludePointNormals then
 begin
  Writeln (f,';컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴');
  writeln (f,'; Point Normals');
  Writeln (f,';컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴');
  Writeln(f,'Label VertexNormals'+paramstr(6)+' word');
  for i := 0 to NumPoints-1 do
  begin
   str(fnormals^[i,0],st1);
   st2 := 'DW '+st1;
   str(fnormals^[i,1],st1);
   st2 := st2+', '+st1;
   str(fnormals^[i,2],st1);
   st2 := st2+', '+st1;
   st2 := st2+',0';
   Writeln (f,st2);
  end;
 end;
 Writeln (f,';컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴');
 writeln (f,'; Faces');
 Writeln (f,';컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴');
 Writeln (f,'Label Polygon'+paramstr(6)+' Word');
 for i := 0 to NumFaces-1 do
 begin
  str(Faces^[i,0],st1);
  st2 := 'DW '+st1;
  str(Faces^[i,1],st1);
  st2 := st2+', '+st1;
  str(Faces^[i,2],st1);
  st2 := st2+', '+st1;
  st2 := st2+',0';
  Writeln (f,st2);
 end;
 if IncludeFaceNormals  then
 begin
  Writeln (f,';컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴');
  writeln (f,'; Face Normals');
  Writeln (f,';컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴');
  Writeln(f,'Label FaceNormals'+paramstr(6)+' word');
  for i := 0 to NumFaces-1 do
  begin
   str(Normals^[i,0],st1);
   st2 := 'DW '+st1;
   str(Normals^[i,1],st1);
   st2 := st2+', '+st1;
   str(Normals^[i,2],st1);
   st2 := st2+', '+st1;
   st2 := st2+',0';
   Writeln (f,st2);
  end;
 end;
 close(f);
end;

Procedure ShowMaxNormal;
var largest : longint;
begin
 largest := 0;
 for i := 0 to NumPoints-1 do
 begin
  if abs(fnormals^[i,0]) > Largest then largest := abs(fnormals^[i,0]);
  if abs(fnormals^[i,1]) > Largest then largest := abs(fnormals^[i,1]);
  if abs(fnormals^[i,2]) > Largest then largest := abs(fnormals^[i,2]);
 end;
 Writeln ('Largest ABS(VertexNormal) = ',Largest);
end;


begin
 if paramcount <> 6 then
 begin
  writeln;
  writeln ('Usage: CONVERT infile FN PN En Lm LabelNum');
  Writeln;
  Writeln ('Where: infile = ASC file WITHOUT extension');
  writeln ('       FN = 1 to Include Face-normals');
  writeln ('       PN = 1 to Include Point-normals');
  Writeln ('       En = Normals, length 1 = This size (for FixedPoint, etc)');
  Writeln ('       LD = LoadMultiplier. How much to Multiply Coords with when loading');
  Writeln ('       LabelNum = string to add to all labels');
  halt;
 end;
 if paramstr(2) = '1' then IncludeFaceNormals := true;
 if paramstr(3) = '1' then IncludePointNormals := true;
 Val(paramstr(4),EnMult,dum);
 Val(paramstr(5),LoadMult,dum);

 GetMem(Points,SizeOf(BigArray));
 GetMem(FNormals,SizeOf(BigArray));
 GetMem(Faces,SizeOf(BigArray));
 GetMem(Normals,SizeOf(BigArray));

 ReadData;
 Center;
 Multiply;
 writeln;
 writeln('Calculating ',NumFaces,' Face-Normals, please wait...');
 calcnormals;
 writeln('Calculating ',NumPoints,' Pseudo/Vertex-Normals, please wait...');
 calcfakenormals;
 ShowMaxNormal;
 WriteToFile;

 FreeMem(Points,SizeOf(BigArray));
 FreeMem(FNormals,SizeOf(BigArray));
 FreeMem(Faces,SizeOf(BigArray));
 FreeMem(Normals,SizeOf(BigArray));

 Writeln;
 Writeln ('3DStudio -> INC converter by Axon/Xenon Development');
 Writeln ('Summary:');
 writeln ('File: ',Paramstr(1),'.INC');
 Writeln ('Number of Vertices/Coordinates = ',NumPoints);
 Writeln ('Number of Faces/Polygons = ',NumFaces);
 Writeln;
end.