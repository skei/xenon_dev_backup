Unit Misc;

interface

Uses Dos;

Type STR127 = string[127];

FUNCTION EXECUTE(Name : PathStr ; Tail : STR127) : WORD;
function fileExists(var s : string) : boolean;
Procedure FlushKeyBoard;
Function SqrRoot(x:longint):word;
Procedure ChDrive(n:byte);
Function GetDriveList:String;
function dup(b:byte;c:char):string;

implementation

uses crt;

var reg:registers;

function dup(b:byte;c:char):string;
var i:byte;
    s:string;
begin
 s := '';
 for i := 1 to b do s := s + c;
 dup := s;
end;


Function GetDriveList:string;
Var Count : Integer;
    s:string;
begin
 s := 'AB';
 For Count := 3 to 26 do
 begin
  if DiskSize(Count) > 0 then s := s + (Chr(64+count));
 end;
 GetDriveList := s;
end;

Procedure ChDrive(n:byte);
begin
 if n <> 0 then
 begin
  asm
   mov ah,0Eh
   mov dl,n
   int 21h
  end;
 end;
end;

Function SqrRoot(x:longint):word; assembler;
asm
 les bx,X
 mov dx,es
 mov ax,0
 mov cx,-1
@@1:
 add cx,2
 inc ax
 sub bx,cx
 sbb dx,0
 jnb @@1
end;


PROCEDURE ReallocateMemory(P : POINTER); ASSEMBLER;
ASM
  MOV  AX, PrefixSeg
  MOV  ES, AX
  MOV  BX, WORD PTR P+2
  CMP  WORD PTR P,0
  JE   @OK
  INC  BX
 @OK:
  SUB  BX, AX
  MOV  AH, 4Ah
  INT  21h
  JC   @X
  LES  DI, P
  MOV  WORD PTR HeapEnd,DI
  MOV  WORD PTR HeapEnd+2,ES
 @X:
END;

FUNCTION EXECUTE(Name : PathStr ; Tail : STR127) : WORD; ASSEMBLER;
ASM
  {$IFDEF CPU386}
  DB      66h
  PUSH    WORD PTR HeapEnd
  DB      66h
  PUSH    WORD PTR Name
  DB      66h
  PUSH    WORD PTR Tail
  DB      66h
  PUSH    WORD PTR HeapPtr
  {$ELSE}
  PUSH    WORD PTR HeapEnd+2
  PUSH    WORD PTR HeapEnd
  PUSH    WORD PTR Name+2
  PUSH    WORD PTR Name
  PUSH    WORD PTR Tail+2
  PUSH    WORD PTR Tail
  PUSH    WORD PTR HeapPtr+2
  PUSH    WORD PTR HeapPtr
  {$ENDIF}
  CALL ReallocateMemory
  CALL SwapVectors
  CALL DOS.EXEC
  CALL SwapVectors
  CALL ReallocateMemory
  MOV  AX, DosError
  OR   AX, AX
  JNZ  @OUT
  MOV  AH, 4Dh
  INT  21h
 @OUT:
END;

function fileExists(var s : string) : boolean;
begin
  fileExists := fSearch(s, '') <> '';
end;

Procedure FlushKeyBoard;
begin
 While Keypressed do ReadKey;
end;

begin
end.