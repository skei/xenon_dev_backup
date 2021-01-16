Program MakeTabl;

var sine : array[0..8191] of longint;

    f : text;
    i,j : longint;
    m,a,s : longint;
    dummy : integer;
    outstr : string;

function Int2Str(I: Longint): String;
var S: string[11];
begin
 Str(I, S);
 Int2Str := S;
end;

Procedure CalcSinus(m,a,s:longint);
var x : word;
    y : real;
Begin
  y := -pi;
  For x := 0 to s-1 DO
  Begin
   if paramstr(6) = 'S' then Sine[x]:=Round(sin(y)*m)+a;
   if paramstr(6) = 'C' then Sine[x]:=Round(cos(y)*m)+a;
    y := y + (pi*2 / s)
  End;
end;

Begin
 if paramcount <> 6 then
 begin
  Writeln ('MakeTable v0.2 - (c) Axon 1994-1995...');
  writeln ('USAGE: MAKETABL file mul add size B/W S/C');
  Writeln ('Where - file   = Name of sine table file');
  Writeln ('        mul    = value to multiply sine values (-1..1) with');
  Writeln ('        add    = Value to add to each value');
  Writeln ('        size   = Total number of elements in array (max 8k)');
  Writeln ('        B/W/D  = B for byte/shortint, W for word/integer');
  Writeln ('        S/C    = S for Sine table, C for Cosine');
  Writeln;
  Writeln ('Ex: "MAKETABL SINUS.INC 50 100 256 B S" would create a sine table called');
  writeln ('SINUS.INC with 256 DB entries ranging from -150 to 150');
  halt(1);
 end;

 val(paramstr(2),m,dummy);
 val(paramstr(3),a,dummy);
 val(paramstr(4),s,dummy);

 CalcSinus(m,a,s);
 assign (f,paramstr(1));
 rewrite (f);
 Writeln (f,'; Table created with MakeTabl v0.2 - (c) 1994-1995 Axon');
 Writeln (f,'; ',s,' entries - ranging from ',-m+a,' to ',m+a);
 Writeln (f);
 for i := 0 to s-1 do
 begin
  if paramstr(5) = 'B' then Writeln (f,'DB ',int2str(sine[i]));
  if paramstr(5) = 'W' then Writeln (f,'DW ',int2str(sine[i]));
  if paramstr(5) = 'D' then Writeln (f,'DD ',int2str(sine[i]));
 end;
 Writeln;
 Writeln (f,'; End of table');
 close(f);
end.

