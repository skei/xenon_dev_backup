Program SaveVGA;

Var pal : Array[0..255,0..2] of byte;
    screen : array[0..63999] of byte absolute $a000:0;
    s:string;
    f:file;

Procedure GetPalette;
Var
  j : Word;
Begin
  port [$3c7] := 0; {Start from palette index 0}
  For j := 0 To 255 Do Begin {Go thru all the indexes}
    pal[j,0] := port [$3c9];   {Don't do any AND here}
    pal[j,1] := port [$3c9];
    pal[j,2] := port [$3c9];
  End;
End;


Begin
 if pos('.',paramstr(1)) = 0 then s := paramstr(1)+'.PIC' else s:=paramstr(1);
 assign(f,s);
 rewrite(f,1);
 GetPalette;
 BlockWrite(f,pal,768);
 blockwrite(f,screen,64000);
 close(f);
end.
