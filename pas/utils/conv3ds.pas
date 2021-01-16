Program Convert;

Const IncludePointNormals = 1;
      IncludeFaceNormals = 1;

Const MaxPoints = 1024;
      MaxFaces  = 2048;
      EnMult    = 255;

Var Points   : Array[0..MaxPoints-1,0..2] of integer;
Var FNormals : array[0..MaxPoints-1,0..2] of longint;
var Faces    : Array[0..MaxFaces-1 ,0..2] of integer;
Var Normals  : Array[0..MaxFaces-1 ,0..2] of longint;

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
    LargestPoint : longint;

var i   : integer;
    a   : longint;


var f   : text;
    st1 : string;
    st2 : string;

{Read from ASC file}

Procedure Remove(num:byte);
begin
 inndata := copy(inndata,num+1,length(inndata)-num);
end;

Function GetString : string;
var d : string;
begin
 d := '';
 repeat
  d := d + inndata[1];
  Remove(1);
 until (inndata[1] = ' ') or (length(inndata)<1);
 getstring := d;
end;

Procedure GetVertices;
begin
 LargestPoint := 0;
 Writeln;
 Writeln ('şşş Points şşş');
 writeln;
 for i := 0 to NumPoints-1 do
 begin
  repeat
   readln(infile,inndata);
  until copy(inndata,1,6) = 'Vertex';
  remove( pos('X:',inndata)+2);
  val(GetString,r1,dum);
  remove(pos('Y:',inndata)+2);
  val(GetString,r2,dum);
  remove(pos('Z:',inndata)+2);
  val(GetString,r3,dum);
  d1 := round(r1);
  d2 := round(r2);
  d3 := round(r3);
  Writeln ('Point: ',i,' : ',d1,',',d2,',',d3);
  if abs(d1) > LargestPoint then LargestPoint := abs(d1);
  Points[i,0] := d1;
  if abs(d2) > LargestPoint then LargestPoint := abs(d2);
  Points[i,1] := d2;
  if abs(d3) > LargestPoint then LargestPoint := abs(d3);
  Points[i,2] := d3;
 end;
end;

Procedure GetFaces;
begin
 Writeln;
 Writeln ('şşş Faces şşş');
 writeln;
 for i := 0 to NumFaces-1 do
 begin
  repeat
   readln(infile,inndata);
  until copy(inndata,1,4) = 'Face';
  remove( pos('A:',inndata)+1);
  val(GetString,d1,dum);
  remove(pos('B:',inndata)+1);
  val(GetString,d2,dum);
  remove(pos('C:',inndata)+1);
  val(GetString,d3,dum);
  Writeln ('Face: ',i,' : ',d1,',',d2,',',d3);
  Faces[i,0] := d1;
  Faces[i,1] := d2;
  Faces[i,2] := d3;
 end;
end;

Procedure Multiply;
begin
 Writeln;
 Writeln ('Largest Point value = ABS(',LargestPoint,')');
 Writeln ('Enter value to MULTIPLY each vertex with (ex. 1.0)');
 ReadLn(MultiPlier);
 for i := 0 to NumPoints-1 do
 begin
  Points[i,0] := Round(Points[i,0]*Multiplier);
  Points[i,1] := Round(Points[i,1]*Multiplier);
  Points[i,2] := Round(Points[i,2]*Multiplier);
 end;
end;

{ Calculate Face & vertexNormals}

procedure calcfakenormals;
var num,i:integer;
    kx,ky,kz,ax,ay,az,bx,by,bz:longint;
    length:real;
begin
 for i:=0 to NumPoints-1 do
 begin
  ax:=0;
  ay:=0;
  az:=0;
  num:=0;
  for a:=0 to numfaces-1 do
  begin
   if faces[a,0]=i then
   begin
    ax:=ax+normals[a,0];
    ay:=ay+normals[a,1];
    az:=az+normals[a,2];
    inc(num);
   end;
   if faces[a,1]=i then
   begin
    ax:=ax+normals[a,0];
    ay:=ay+normals[a,1];
    az:=az+normals[a,2];
    inc(num);
   end;
   if faces[a,2]=i then
   begin
    ax:=ax+normals[a,0];
    ay:=ay+normals[a,1];
    az:=az+normals[a,2];
    inc(num);
   end;
  end;
  ax:=ax div num;
  ay:=ay div num;
  az:=az div num;
  fnormals[i,0]:=ax;
  fnormals[i,1]:=ay;
  fnormals[i,2]:=az;
 end;
end;

procedure calcnormals;
var i:integer;kx,ky,kz,ax,ay,az,bx,by,bz:longint;length:real;begin
 for i:=0 to numfaces-1 do
 begin
  ax:=points[faces[i,1],0]-points[faces[i,0],0];
  ay:=points[faces[i,1],1]-points[faces[i,0],1];
  az:=points[faces[i,1],2]-points[faces[i,0],2];
  bx:=points[faces[i,2],0]-points[faces[i,0],0];
  by:=points[faces[i,2],1]-points[faces[i,0],1];
  bz:=points[faces[i,2],2]-points[faces[i,0],2];
  kx:=(ay*bz)-(az*by);ky:=(az*bx)-(ax*bz);kz:=(ax*by)-(ay*bx);
  length:=sqrt(longint(kx)*longint(kx)+longint(ky)*longint(ky)+longint(kz)*longint(kz));
  if length <> 0 then
  begin
   normals[i,0]:=(round(((kx/length)*EnMult)));
   normals[i,1]:=(round(((ky/length)*EnMult)));
   normals[i,2]:=(round(((kz/length)*EnMult)));
  end
  else
  begin
   normals[i,0]:=0;
   normals[i,1]:=0;
   normals[i,2]:=0;
  end;
 end;
end;

Procedure WriteToFile;
begin
 assign(f,ParamStr(1)+'.INC');
 rewrite(f);
 Writeln (f,';Converted by Axon/Xenon Development');
 Str(NumPoints,st1);
 Writeln (f,';NumPoints: '+st1);
 Str(NumFaces,st1);
 Writeln (f,';NumFaces: '+st1);
 Writeln (f,';ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ');
 writeln (f,'; Points');
 Writeln (f,';ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ');
 Writeln (f,'Label Coords Word');
 for i := 0 to NumPoints-1 do
 begin
  str(Points[i,0],st1);
  st2 := 'DW '+st1;
  str(Points[i,1],st1);
  st2 := st2+', '+st1;
  str(Points[i,2],st1);
  st2 := st2+', '+st1;
  st2 := st2+',0';
  Writeln (f,st2);
 end;
 if IncludePointNormals = 1 then
 begin
  Writeln (f,';ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ');
  writeln (f,'; Point Normals');
  Writeln (f,';ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ');
  Writeln(f,'Label VertexNormals word');
  for i := 0 to NumPoints-1 do
  begin
   str(fnormals[i,0] div 4,st1);
   st2 := 'DW '+st1;
   str(fnormals[i,1] div 4,st1);
   st2 := st2+', '+st1;
   str(fnormals[i,2] div 4,st1);
   st2 := st2+', '+st1;
   st2 := st2+',0';
   Writeln (f,st2);
  end;
 end;
 Writeln (f,';ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ');
 writeln (f,'; Faces');
 Writeln (f,';ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ');
 Writeln (f,'Label Polygon Word');
 for i := 0 to NumFaces-1 do
 begin
  str(Faces[i,0],st1);
  st2 := 'DW '+st1;
  str(Faces[i,1],st1);
  st2 := st2+', '+st1;
  str(Faces[i,2],st1);
  st2 := st2+', '+st1;
  st2 := st2+',0';
  Writeln (f,st2);
 end;
 if IncludeFaceNormals = 1 then
 begin
  Writeln (f,';ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ');
  writeln (f,'; Face Normals');
  Writeln (f,';ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ');
  Writeln(f,'Label FaceNormals word');
  for i := 0 to NumFaces-1 do
  begin
   str(Normals[i,0],st1);
   st2 := 'DW '+st1;
   str(Normals[i,1],st1);
   st2 := st2+', '+st1;
   str(Normals[i,2],st1);
   st2 := st2+', '+st1;
   st2 := st2+',0';
   Writeln (f,st2);
  end;
 end;
 close(f);
end;

begin
 if paramcount <> 1 then
 begin
  writeln;
  writeln ('Usage: CONVERT infile <w/o .ASC!>');
  halt;
 end;
 assign(infile,paramstr(1)+'.ASC');
 reset(infile);
 Readln(infile,dummy); {Skip first 5 lines ... :)}
 Readln(infile,dummy); {Next version; Find number of vertices and faces}
 Readln(infile,dummy); {and label names here}
 readln(infile,inndata);
 Remove(pos('Vertices',inndata)+9);
 Val(GetString,NumPoints,dum);
 Writeln ('Number of vertices : ',NumPoints);
 Remove(pos('Faces',inndata)+6);
 Val(GetString,NumFaces,dum);
 Writeln ('Number of faces : ',NumFaces);
 ReadLn(infile,dummy);
 GetVertices;
 ReadLn(infile,dummy);
 GetFaces;
 Close(infile);

 Multiply;

 writeln;
 writeln('Calculating Face-Normals, please wait...');
 calcnormals;
 writeln('Calculating Pseudo/Vertex-Normals, please wait...');
 calcfakenormals;

 WriteToFile;
 Writeln;
 Writeln ('3DStudio -> INC converter by Axon/Xenon Development');
 Writeln ('Summary:');
 Writeln ('Number of Vertices/Coordinates = ',NumPoints);
 Writeln ('Number of Faces/Polygons = ',NumFaces);
 Writeln;
end.