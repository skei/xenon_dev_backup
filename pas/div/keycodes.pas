Program KeyCodes;

Uses Dos;

var R:registers;
    Ascii : byte;
    ScanCode : byte;

begin
 repeat
  r.ah := 1;
  intr($16,r);
  if (r.flags AND FZero) = FZero then
  begin
   r.ah := 0;
   intr($16,r);
   Ascii := r.al;
   ScanCode := r.ah;
   Writeln ('Key: ',chr(ascii),' þ ASCII: ',ascii,' þ ScanCode: ',ScanCode);
  end;
 until Ascii = 27;
end.