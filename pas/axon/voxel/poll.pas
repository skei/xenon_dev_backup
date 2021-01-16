Unit POLL ;         { polled keyboard handler }
                    { does not support F11 or F12 keys } Interface

Const
   EscKey = 1 ;    { key codes }
   aKey = 30 ;     { see TP 6 Programmers guide p 354 }
   sKey = 31 ;
   endKey = 79 ;
   DownKey = 80 ;

Var
   KeyTable : Array[ 1..127 ] of Boolean ; { KeyTable[ x ] is True when key x
is pressed } { and stays True Until key x is released }

Implementation

Uses Dos, KeyIntr ;  { keyboard interrupt support }

Var
   OldInt09 : Pointer ;
   ExitSave : Pointer ;

{$F+} Procedure RestoreInt09 ;
begin
   ExitProc := ExitSave ;
   SetIntVec( $09, OldInt09 ) ;
end ;

{$F+} Procedure NewInt09 ; interrupt ;
Var
   ScanCode : Byte ;
   KeyCode : Byte ;
begin
   STI ;
   ScanCode := ReadScanCode ;
   KeyCode := ScanCode and $7F ;        { strip make/break bit }
   KeyTable[ KeyCode ] := ( ScanCode and $80 ) = 0 ; (* { For non C Programmers
}
   if ( ScanCode and $80 ) = 0  then    { make code -- key pressed }
      KeyTable[ KeyCode ] := True
   else                                 { break code -- key released }
      KeyTable[ KeyCode ] := False ;
*)
   ResetKeyboard ;
   EOI ;
end ;

Var
   N : Byte ;

begin
   ExitSave := ExitProc ;
   ExitProc := addr( RestoreInt09 ) ;

   For N := 1 to 127 do            { no key pressed }
      KeyTable[ N ] := False ;

   GetIntVec( $09, OldInt09 ) ;
   SetIntVec( $09, addr( NewInt09 ) ) ;
end.
