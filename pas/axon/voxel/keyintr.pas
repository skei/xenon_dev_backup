Unit KeyIntr ;  { support For INT 09 routines }

Interface

Procedure CLI ; Inline( $FA ) ; { disable interrupts }
Procedure STI ; Inline( $FB ) ; { enable interrupts }

{ cannot be used outside an interrupt Procedure }
Procedure JumpInterrupt( p : Pointer ) ;
Inline(
  $5B/$58/                         { POP  BX, AX   AX:BX = p }
  $89/$EC/                         { MOV  SP, BP             }
  $87/$46/$10/                     { XCHG AX, [BP+10H]       }
  $87/$5E/$0E/                     { XCHG BX, [BP+0EH]       }
  $5D/$07/$1F/$5F/$5E/             { POP  BP, ES, DS, DI, SI }
  $5A/$59/                         { POP  DX, CX             }
  $FA/                             { CLI                     }
  $CB ) ;                          { RETF          jmp far p }


Function Control_Pressed : Boolean ;

Procedure EOI ;
{ end of interrupt to 8259 }

Function ReadScanCode : Byte ;
{ read keyboard }

Procedure ResetKeyboard ;
{ prepare For next key }

Procedure StoreKey( Scan, Key : Byte );
{ put key in buffer For INT 16 }


Implementation

Uses
  Crt ;  { Sound, NoSound }

Type
  Address = Record                  { used in Pointer manipulation }
    Offset : Word ;
    Segment : Word ;
  end ;
Const
  BiosDataSegment = $40 ;

Var
  KeyState       : Word Absolute BiosDataSegment:$0017 ;
  KeyBufferHead  : Word Absolute BiosDataSegment:$001A ;
  KeyBufferTail  : Word Absolute BiosDataSegment:$001C ;
  KeyBufferStart : Word Absolute BiosDataSegment:$0080 ;
  KeyBufferend   : Word Absolute BiosDataSegment:$0082 ;


Function Control_Pressed : Boolean ;
begin
  Control_Pressed := ( KeyState and  4 ) = 4 ;
end;

Procedure EOI ;
{ end of interrupt to 8259 interrupt controller }
begin
  CLI ;
  Port[$20] := $20 ;
end ;

Function ReadScanCode : Byte ;
begin
  ReadScanCode := Port[$60] ;
end ;

Procedure ResetKeyboard ;
{ prepare For next key }
Var
  N : Byte ;
begin
  N := Port[$61] ;
  Port[$61] := ( N or $80 ) ;
  Port[$61] := N ;
end ;

Procedure StoreKey( Scan, Key : Byte ) ;
Var
{ put key in buffer that INT 16 reads }
  P : ^Word ;
  N : Word ;
begin
  address(P).segment := BiosDataSegment ;
  N := KeyBufferTail ;
  address(P).offset := N ;
  Inc( N, 2 ) ;                      { advance Pointer two Bytes }
  If( N = KeyBufferend ) then        { end of the circular buffer }
     N := KeyBufferStart ;
  If( N = KeyBufferHead ) then       { buffer full }
  begin
    EOI ;               { EOI must be done before Exit            }
    Sound( 2200 ) ;     {    but before anything that takes a lot }
    Delay( 80 ) ;       {     of time and can be interrupted      }
    NoSound ;
  end
  Else
  begin          { high Byte is scan code, low is ASCII }
    P^ := Scan * $100 + Key ;       { store key in circular buffer }
    KeyBufferTail := N ;            { advance tail Pointer }
    EOI ;
  end ;
end ;

end.
