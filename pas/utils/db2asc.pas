{ Convert from DB to ASCII }

Program DB2ASC;

Uses Crt;

Var Infile : file;
    OutFile : text;
    tall : byte;
    Dummy : byte;
    InCounter : Longint;
    OutCOunter : LongInt;

Begin
 InCounter := 0;
 OutCounter := 0;
 ClrScr;
 if paramcount <> 2 then
 begin
  Writeln;
  Writeln ('DB2ASC InFile OutFile');
  Writeln;
  Halt;
 end;
 assign (Infile,Paramstr(1));
 Reset(InFile,1);
 Assign (OutFile,ParamStr(2));
 ReWrite(OutFile);
 while (not EOF(InFile)) do
 begin
  blockread (InFile,Dummy,1);
  inc (InCounter);
  GotoXY(1,1);
  Writeln (InCOunter);
  if chr(Dummy) = '0' then
  begin
   tall := 0;
   BlockRead (InFile,Dummy,1);
   case upcase(chr(Dummy)) of
    '0' : tall := 0*16;
    '1' : tall := 1*16;
    '2' : tall := 2*16;
    '3' : tall := 3*16;
    '4' : tall := 4*16;
    '5' : tall := 5*16;
    '6' : tall := 6*16;
    '7' : tall := 7*16;
    '8' : tall := 8*16;
    '9' : tall := 9*16;
    'A' : tall := 10*16;
    'B' : tall := 11*16;
    'C' : tall := 12*16;
    'D' : tall := 13*16;
    'E' : tall := 14*16;
    'F' : tall := 15*16;
   end;
   BlockRead (InFile,Dummy,1);
   case upcase(chr(Dummy)) of
    '0' : tall := tall+0;
    '1' : tall := tall+1;
    '2' : tall := tall+2;
    '3' : tall := tall+3;
    '4' : tall := tall+4;
    '5' : tall := tall+5;
    '6' : tall := tall+6;
    '7' : tall := tall+7;
    '8' : tall := tall+8;
    '9' : tall := tall+9;
    'A' : tall := tall+10;
    'B' : tall := tall+11;
    'C' : tall := tall+12;
    'D' : tall := tall+13;
    'E' : tall := tall+14;
    'F' : tall := tall+15;
   end;
   Write(OutFile,chr(tall));
   inc (OutCounter);
   GotoXY (2,2);
   Writeln (OutCounter);
  end;
  if keypressed then
  begin
   close(InFile);
   close(OutFile);
   Halt;
  end;
 end;
 Close (InFile);
 Close (OutFile);
end.
