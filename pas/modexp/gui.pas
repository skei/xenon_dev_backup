Unit GUI;

{----------}
interface
{----------}

Uses Dos;

Const MaxItems = 50;

Type Str127 = string[127];

Type MaskType = Array[0..1,0..15] of word;
     MaskPointer = ^MaskRec;
     MaskRec = Record
                Mask : MaskType;
                x,y  : word;
               end;

CONST
 StandardCursor:MaskRec=(Mask:(
    ($3FFF,$1FFF,$0FFF,$07FF,
     $03FF,$01FF,$00FF,$007F,
     $003F,$001F,$01FF,$10FF,
     $30FF,$F87F,$F87F,$FC3F),
    ($0000,$4000,$6000,$7000,
     $7800,$7C00,$7E00,$7F00,
     $7F80,$7FC0,$7C00,$4600,
     $0600,$0300,$0300,$0180));X:0;Y:0);
 HourGlassCursor:MaskRec=(Mask:(
    ($0000,$0000,$0000,$0000,
     $8001,$C003,$E007,$F00F,
     $E007,$C003,$8001,$0000,
     $0000,$0000,$0000,$FFFF),
    ($0000,$7FFE,$6006,$300C,
     $1818,$0C30,$0660,$03C0,
     $0660,$0C30,$1998,$33CC,
     $67E6,$7FFE,$0000,$0000));X:7;Y:7);

Var MouseExist : boolean;
    MouseCursoron : boolean;


Const UP = true;
      DOWN = False;

Type ItemType = RECORD
      active      : boolean;
      Itype       : byte;
      ID          : byte;
      x1,y1,x2,y2 : integer;
      Data        : word{integer};
      text        : String;
     end;

Type DialogType = RECORD
      title       : string;
      x1,y1,x2,y2 : integer;
      NumItems    : byte;
      Item        : Array[1..MaxItems] of ItemType;
      P           : pointer;
      PSize       : word;
     end;

Var LightColor  : byte;
    ShadowColor : byte;
    BarColor    : byte;
    TextColor   : byte;

Var Dx1,Dy1 : integer;
var saved : pointer;

{Mouse functions}
procedure mouseinit;
procedure mouseon;
procedure mouseoff;
function mousex: word;
function mousey: word;
function mouseleft: boolean;
function mousemiddle: boolean;
function mouseright: boolean;     { Is the right button down? }
procedure setmousexy(newx, newy: word);   { Position mouse cursor. }
procedure limitmouse(lox, loy, hix, hiy: word);   { Restrict mouse movements. }
Function MouseInArea (x1,y1,x2,y2:integer):boolean;
Function MouseLeftInArea (x1,y1,x2,y2:integer):boolean;
Function MouseRightInArea (x1,y1,x2,y2:integer):boolean;
PROCEDURE SetGFXCursor(VAR MaskP:MaskRec);
{Basic VGA functions}
PROCEDURE MCGAOn;
PROCEDURE MCGAOff;
FUNCTION GetPixel(X,Y:Word):Byte;
PROCEDURE PutPixel(X,Y:Word; C:Byte);
PROCEDURE DrawLineH(X1,X2,Y1:Word; C:Byte);
PROCEDURE DrawLineV(X1,Y1,Y2:Word; C:Byte);
PROCEDURE DrawLine(X1,Y1,X2,Y2:Integer; C:Byte);
PROCEDURE SetColor(Nr,R,G,B:Byte);
PROCEDURE GetColor(Nr:Byte; VAR R,G,B:Byte);
PROCEDURE GetImage(X1,Y1,X2,Y2:Integer; VAR P);
PROCEDURE PutImage(X1,Y1:Integer; VAR P);
PROCEDURE PutImagePart(X1,Y1,XS2,YS2:Integer; VAR P);
PROCEDURE FillBlock(X1,Y1,X2,Y2:Integer; C:Byte);
PROCEDURE FillScreen(C:Byte);
{GFX file load/save}
Procedure LoadBMP (Name:String);
Procedure LoadPCX (PCXFile:string);
Procedure SavePCX (FN : String);
Procedure LoadGif (N: String);
PROCEDURE LoadPalette(DateiName:String);
PROCEDURE SavePalette(DateiName:String);
PROCEDURE LoadRaw(DateiName:String);
PROCEDURE SaveRaw(DateiName:String);
{Various}
FUNCTION IsVGA : Boolean;
Procedure FlushKeyBoard;
FUNCTION EXECUTE(Name : PathStr ; Tail : STR127) : WORD;
function fileExists(var s : string) : boolean;
{GUI misc}
Procedure InitGUI;
Procedure CloseGUI;
Procedure SetGUIColors;
Procedure RestoreGUIColors;
Function ClosestColor(r,g,b:byte):byte;
Procedure FindGUIColors;
{Low level GUI functions}
Procedure DrawChar (x,y:integer;c:char;clr:byte);
Procedure DrawString(x,y:integer;s:string;clr:byte);
Procedure Panel(x1,y1,x2,y2:integer;up:boolean);
{Draw Items}
Procedure Draw3DString(x,y:integer;s:string;up:boolean);
Procedure DrawButton(x1,y1,x2,y2:integer;title:string;upp:boolean);
{Dialog}
Procedure InitDialog(var D:DialogType;title:string;x1,y1,x2,y2:integer);
Procedure DrawDialog(var D:DialogType);
Procedure ReDrawDialog(var D:DialogType);
Procedure ReDrawItem(var D:DialogType;i:byte);
Procedure RemoveDialog(D:DialogType);
Function FindID(D:DialogType;n:Byte):byte;
Function HandleDialog(var D:Dialogtype):byte;
{}
Procedure AddButton (var D:DialogType;x1,y1,x2,y2:integer;t:string;id:byte);
Procedure AddInputField(var D:Dialogtype;x1,y1,maxlen:integer;s:string;id:byte);
Procedure AddText(var D:DialogType;x1,y1:integer;t:string;id:byte);

{----------}
IMPLEMENTATION
{----------}

Uses Crt;
Var r:registers;

Var
 Gd,Gm: Integer;
 Datei: File;
 palette: array[0..767] of byte;
 buffer: array[0..1279] of byte;
 prefix,tail: array[0..4095] OF WORD;
 keller: array[0..640] of Word;

type pcxheader_rec=record
     manufacturer: byte;
     version: byte;
     encoding: byte;
     bits_per_pixel: byte;
     xmin, ymin: word;
     xmax, ymax: word;
     hres: word;
     vres: word;
     egapalette: array [0..47] of byte;
     reserved: byte;
     colour_planes: byte;
     bytes_per_line: word;
     palette_type: word;
     filler: array [0..57] of byte;
     end;

var header: pcxheader_rec;
    width, depth: word;
    bytes: word;
    f: file;
    c: byte;


Type FontData = Array[0..4] of string[5];
Const NumChars = 55;
Const Font : Array [0..NumChars] of FontData = (
{ } ('     ','     ','     ','     ','     '),
{a} (' xxx ','x   x','xxxxx','x   x','x   x'),
{b} ('xxxx ','x   x','xxxx ','x   x','xxxx '),
{c} (' xxxx','x    ','x    ','x    ',' xxxx'),
{d} ('xxxx ','x   x','x   x','x   x','xxxx '),
{e} ('xxxxx','x    ','xxx  ','x    ','xxxxx'),
{f} ('xxxxx','x    ','xxx  ','x    ','x    '),
{g} (' xxxx','x    ','x xxx','x   x',' xxxx'),
{h} ('x   x','x   x','xxxxx','x   x','x   x'),
{i} (' xxx ','  x  ','  x  ','  x  ',' xxx '),
{j} ('  xxx','    x','    x','x   x',' xxx '),
{k} ('x   x','x  x ','xxx  ','x  x ','x   x'),
{l} ('x    ','x    ','x    ','x    ','xxxxx'),
{m} ('x   x','xx xx','x x x','x   x','x   x'),
{n} ('x   x','xx  x','x x x','x  xx','x   x'),
{o} (' xxx ','x   x','x   x','x   x',' xxx '),
{p} ('xxxx ','x   x','xxxx ','x    ','x    '),
{q} (' xxx ','x   x','x   x','x  x ',' xx x'),
{r} ('xxxx ','x   x','xxxx ','x  x ','x   x'),
{s} (' xxxx','x    ',' xxx ','    x','xxxx '),
{t} ('xxxxx','  x  ','  x  ','  x  ','  x  '),
{u} ('x   x','x   x','x   x','x   x',' xxx '),
{v} ('x   x','x   x',' x x ',' x x ','  x  '),
{w} ('x   x','x   x','x x x','xx xx','x   x'),
{x} ('x   x',' x x ','  x  ',' x x ','x   x'),
{y} ('x   x','x   x',' xxxx','    x','xxxx '),
{z} ('xxxxx','   x ','  x  ',' x   ','xxxxx'),
{Û} ('xxxxx','xxxxx','xxxxx','xxxxx','xxxxx'),
{,} ('     ','     ','     ','  x  ',' x   '),
{.} ('     ','     ','     ','     ','  x  '),
{!} ('  x  ','  x  ','  x  ','     ','  x  '),
{"} (' x x ',' x x ','     ','     ','     '),
{#} (' x x ','xxxxx',' x x ','xxxxx',' x x '),
{%} ('xx  x','xx x ','  x  ',' x xx','x  xx'),
{&} (' x   ','x x  ',' xx x','x  x ',' xx x'),
{/} ('    x','   x ','  x  ',' x   ','x    '),
{(} ('   x ','  x  ','  x  ','  x  ','   x '),
{)} (' x   ','  x  ','  x  ','  x  ',' x   '),
{=} ('     ',' xxx ','     ',' xxx ','     '),
{?} ('xxxx ','    x','  xx ','     ','  x  '),
{1} ('  x  ',' xx  ','  x  ','  x  ',' xxx '),
{2} ('xxxx ','    x',' xxx ','x    ','xxxxx'),
{3} ('xxxx ','    x',' xxx ','    x','xxxx '),
{4} ('x  x ','x  x ','xxxxx','   x ','   x '),
{5} ('xxxxx','x    ','xxxx ','    x','xxxx '),
{6} (' xxxx','x    ','xxxx ','x   x',' xxx '),
{7} ('xxxxx','    x','   x ','  x  ','  x  '),
{8} (' xxx ','x   x',' xxx ','x   x',' xxx '),
{9} (' xxx ','x   x',' xxxx','    x','xxxx '),
{0} (' xxx ','x   x','x x x','x   x',' xxx '),
{:} ('     ','  x  ','     ','  x  ','     '),
{;} ('     ','  x  ','     ','  x  ',' x   '),
{'} ('  x  ','  x  ','     ','     ','     '),
{+} ('     ','  x  ',' xxx ','  x  ','     '),
{-} ('     ','     ',' xxx ','     ','     '),
{_} ('     ','     ','     ','     ','xxxxx'));


Const ConvFrom : string
 = ' ABCDEFGHIJKLMNOPQRSTUVWXYZÛ,.!"#%&/()=?1234567890:;'+#39+'+-_';
Const ConvTo : string = #0+#1+#2+#3+#4+#5+#6+#7+#8+#9+#10+#11+#12+#13+#14+#15
 +#16+#17+#18+#19+#20+#21+#22+#23+#24+#25+#26+#27+#28+#29+#30+#31+#32+#33+#34
 +#35+#36+#37+#38+#39+#40+#41+#42+#43+#44+#45+#46+#47+#48+#49+#50+#51+#52+#53
 +#54+#55+#56+#57+#58+#59+#60;

Var r1,g1,b1,r2,g2,b2,r3,g3,b3,r4,g4,b4 : byte;

Var OldMode : Byte;

{
----------------------------------------------------------------------------
 Mouse Functions
----------------------------------------------------------------------------
}


procedure mouseinit;
begin
 r.ax := $0000;
 intr($33, r);
 mouseexist := (r.ax = $FFFF);
 mousecursoron := false;
end;

procedure mouseon;
begin
 if mouseexist then
 begin
  r.ax := $0001;
  intr($33, r);
  mousecursoron := true;
 end;
end;

procedure mouseoff;
begin
 if mouseexist then
 begin
  r.ax := $0002;
  intr($33, r);
  mousecursoron := false;
 end;
end;

function mousex: word;
var tempword: word;
begin
 if mouseexist then
 begin
  r.ax := $0003;
  intr($33, r);
  tempword := r.cx;
 end
 else tempword := 0;
 mousex := tempword shr 1;
end;

function mousey: word;
var tempword: word;
begin
 if mouseexist then
 begin
  r.ax := $0003;
  intr($33, r);
  tempword := r.dx;
 end
 else tempword := 0;
 mousey := tempword;
end;

function mouseleft: boolean;
var tempword: word;
begin
 if mouseexist then
 begin
  r.ax := $0003;
  intr($33, r);
  tempword := r.bx;
 end
 else tempword := 0;
 mouseleft := mouseexist and (1 and tempword = 1);
end;

function mousemiddle: boolean;
var tempword: word;
begin
 if mouseexist then
 begin
  r.ax := $0003;
  intr($33, r);
  tempword := r.bx;
 end
 else tempword := 0;
 mousemiddle := mouseexist and (4 and tempword = 4);
end;

function mouseright: boolean;     { Is the right button down? }
var tempword: word;
begin
 if mouseexist then
 begin
  r.ax := $0003;
  intr($33, r);
  tempword := r.bx;
 end
 else tempword := 0;
 mouseright := mouseexist and (2 and tempword = 2);
end;

procedure setmousexy(newx, newy: word);   { Position mouse cursor. }
begin
 r.ax := $0004;
 r.cx := newx shl 1;
 r.dx := newy;
 intr($33, r);
end;

procedure limitmouse(lox, loy, hix, hiy: word);   { Restrict mouse movements. }
begin
 r.ah := $0f;
 intr($10, r);
 r.ax := $0007;
 r.cx := lox shl 1;
 r.dx := hix shl 1;
 intr($33, r);
 r.ax := $0008;
 r.cx := loy;
 r.dx := hiy;
 intr($33, r);
end;

Function MouseInArea (x1,y1,x2,y2:integer):boolean;
begin
 mouseinarea := false;
 if (mousex>=x1) and (mousex<=x2) and (mousey>=y1) and (mousey<=y2)
 then MouseInArea := true;
end;

Function MouseLeftInArea (x1,y1,x2,y2:integer):boolean;
begin
 MouseLeftInArea := false;
 if (mouseinarea(x1,y1,x2,y2)) and (mouseleft) then MouseLeftInArea := true;
end;

Function MouseRightInArea (x1,y1,x2,y2:integer):boolean;
begin
 MouseRightInArea := false;
 if (mouseinarea(x1,y1,x2,y2)) and (mouseright) then MouseRightInArea := true;
end;

PROCEDURE SetGFXCursor(VAR MaskP:MaskRec);
BEGIN
 r.ax := 9;
 r.bx := MaskP.X;
 r.cx := MaskP.Y;
 r.dx := Ofs(MaskP.Mask);
 r.es := Seg(MaskP.Mask);
 intr($33,r);
END;

{
----------------------------------------------------------------------------
 Low level VGA Functions
----------------------------------------------------------------------------
}

PROCEDURE MCGAOn;
BEGIN
 ASM
  mov ah,$f
  int $10
  mov [offset oldmode],al
 END;
 ASM
  mov ax,$13
  int $10
 END;
END;

PROCEDURE MCGAOff;
BEGIN
 ASM
  mov al,[offset oldmode]
  xor ah,ah
  int $10
 END;
END;


PROCEDURE PutPixel(X,Y:Word; C:Byte);
BEGIN
 ASM
  mov ax,$a000
  mov es,ax
  mov bx,x
  mov dx,y
  xchg dh,dl
  mov al,c
  mov di,dx
  shr di,1
  shr di,1
  add di,dx
  add di,bx
  stosb
 END;
END;

FUNCTION GetPixel(X,Y:Word):Byte;
BEGIN
 ASM
  mov ax,$a000
  mov es,ax
  mov bx,x
  mov dx,y
  mov di,dx
  shl di,1
  shl di,1
  add di,dx
  mov cl,6
  shl di,cl
  add di,bx
  mov al,es:[di]
  mov [bp-1],al
 END;
END;

PROCEDURE DrawLineH(X1,X2,Y1:Word; C:Byte);
BEGIN
 ASM
  mov ax,$a000
  mov es,ax
  mov ax,y1
  mov di,ax
  shl di,1
  shl di,1
  add di,ax
  mov cl,6
  shl di,cl
  mov bx,x1
  mov dx,x2
  cmp bx,dx
  jl @1
  xchg bx,dx
@1:
  inc dx
  add di,bx
  mov cx,dx
  sub cx,bx
  shr cx,1
  mov al,c
  mov ah,al
  ror bx,1
  jnb @2
  stosb
  ror dx,1
  jnb @3
  dec cx
@3:
  rol dx,1
@2:
  rep
  stosw
  ror dx,1
  jnb @4
  stosb
@4:
 END;
END;

PROCEDURE DrawLineV(X1,Y1,Y2:Word; C:Byte);
BEGIN
 ASM
  mov ax,x1
  mov bx,y1
  mov dx,y2
  cmp bx,dx
  jl @1
  xchg bx,dx
@1:
  mov di,bx
  shl di,1
  shl di,1
  add di,bx
  mov cl,6
  shl di,cl
  add di,ax
  mov cx,$a000
  mov es,cx
  mov cx,dx
  sub cx,bx
  inc cx
  mov al,c
  mov bx,$13f
@2:
  stosb
  add di,bx
  loop @2
 END;
END;

PROCEDURE DrawLine(X1,Y1,X2,Y2:Integer; C:Byte);
BEGIN
     ASM
        mov al,c
        xor ah,ah
        mov si,ax
        mov ax,x1
        cmp ax,319
        ja @Ende
        mov bx,x2
        cmp bx,319
        ja @Ende
        mov cx,y1
        cmp cx,199
        ja @Ende
        mov dx,y2
        cmp dx,199
        ja @Ende
        cmp ax,bx
        jnz @weiter
        cmp cx,dx
        jnz @vertical
        push ax
        push cx
        push si
        call Putpixel
        jmp @ende
@weiter:cmp cx,dx
        jnz @weiter2
        push ax
        push bx
        push cx
        push si
        call drawlineh
        jmp @ende
@vertical:push ax
        push cx
        push dx
        push si
        call drawlinev
        jmp @ende
@weiter2:cmp cx,dx
        jbe @1
        xchg cx,dx
        xchg ax,bx
@1:     mov di,cx
        shl di,1
        shl di,1
        add di,cx
        push si
        mov si,bx
        mov bx,dx
        sub bx,cx
        mov cl,06
        shl di,cl
        add di,ax
        mov dx,si
        pop si
        sub dx,ax
        mov ax,$a000
        mov es,ax
        mov ax,si
        push bp
        or dx,0
        jge @jmp1
        neg dx
        cmp dx,bx
        jbe @jmp3
        mov cx,dx
        inc cx
        mov si,dx
        shr si,1
        std
        mov bp,320
@1c:    stosb
@1b:    or si,si
        jge @1a
        add di,bp
        add si,dx
        jmp @1b
@1a:    sub si,bx
        loop @1c
        jmp @Ende2
@jmp3:  mov cx,bx
        inc cx
        mov si,bx
        neg si
        sar si,1
        cld
        mov bp,319
@2c:    stosb
@2b:    or si,si
        jl @2a
        sub si,bx
        dec di
        jmp @2b
@2a:    add di,bp
        add si,dx
        loop @2c
        jmp @Ende2
@jmp1:  cmp dx,bx
        jbe @jmp4
        mov cx,dx
        inc cx
        mov si,dx
        shr si,1
        cld
        mov bp,320
@3c:    stosb
@3b:    or si,si
        jge @3a
        add di,bp
        add si,dx
        jmp @3b
@3a:    sub si,bx
        loop @3c
        jmp @Ende2
@jmp4:  mov cx,bx
        inc cx
        mov si,bx
        neg si
        sar si,1
        std
        mov bp,321
@4c:    stosb
@4b:    or si,si
        jl @4a
        sub si,bx
        inc di
        jmp @4b
@4a:    add di,bp
        add si,dx
        loop @4c
@Ende2: pop bp
        cld
@Ende:END;
END;

PROCEDURE SetColor(Nr,R,G,B:Byte);
BEGIN
 Port[$3C8]:=Nr;
 Port[$3C9]:=R;
 Port[$3C9]:=G;
 Port[$3C9]:=B;
END;

PROCEDURE GetColor(Nr:Byte; VAR R,G,B:Byte);
BEGIN
 Port[$3C7]:=Nr;
 R:=Port[$3C9];
 G:=Port[$3C9];
 B:=Port[$3C9];
END;

PROCEDURE GetImage(X1,Y1,X2,Y2:Integer; VAR P);
VAR
 Data:ARRAY[0..64003] OF Byte ABSOLUTE P;
 I,XS,YS:Word;
 P2:Pointer ABSOLUTE P;
BEGIN
 XS:=X2-X1;
 YS:=Y2-Y1;
 Data[0]:=Lo(XS);
 Data[1]:=Hi(XS);
 Data[2]:=Lo(YS);
 Data[3]:=Hi(YS);
 FOR I:=0 TO YS DO Move(Ptr($A000,(Y1+I)*320+X1)^,Data[(XS+1)*I+4],XS+1);
END;

PROCEDURE PutImage(X1,Y1:Integer; VAR P);
VAR
 Data:ARRAY[0..64003] OF Byte ABSOLUTE P;
 Adr,I,XS,YS:Word;
 DataDS,DataSI:Word;
BEGIN
 XS:=Data[0]+Data[1] SHL 8;
 YS:=Data[2]+Data[3] SHL 8;
 Adr:=Word(Y1)*320+X1;
 DataDS:=Seg(Data[4]);
 DataSI:=Ofs(Data[4]);
 ASM
  mov dx,ys
  inc dx
  mov bx,xs
  inc bx
  mov ax,$a000
  mov es,ax
  mov di,adr
  mov si,DataSI
  mov ax,DataDS
  push ds
  mov ds,ax
  cld
@1:
  mov cx,bx
  rep movsb
  add di,320
  sub di,bx
  dec dx
  jnz @1
  pop ds
 END;
END;

PROCEDURE PutImagePart(X1,Y1,XS2,YS2:Integer; VAR P);
VAR
 Data:ARRAY[0..64003] OF Byte ABSOLUTE P;
 Adr,I,XS,YS:Word;
 DataDS,DataSI:Word;
BEGIN
 XS:=Data[0]+Data[1] SHL 8+1;
 YS:=Data[2]+Data[3] SHL 8+1;
 IF (XS2<0) OR (XS2>XS) THEN XS2:=XS;
 IF (YS2<0) OR (YS2>YS) THEN YS2:=YS;
 Adr:=Word(Y1)*320+X1;
 DataDS:=Seg(Data[4]);
 DataSI:=Ofs(Data[4]);
 ASM
  mov dx,ys
  mov bx,xs2
  mov ax,$a000
  mov es,ax
  mov di,adr
  mov si,DataSI
  mov ax,DataDS
  mov cx,xs
  sub cx,xs2
  push ds
  mov ds,ax
  mov ax,cx
  cld
@1:
  mov cx,bx
  rep movsb
  add di,320
  sub di,bx
  add si,ax
  dec dx
  jnz @1
  pop ds
 END;
END;

PROCEDURE FillBlock(X1,Y1,X2,Y2:Integer; C:Byte);
VAR Y:Integer;
BEGIN
 FOR Y:=Y1 TO Y2 DO DrawLineH(X1,X2,Y,C);
END;

PROCEDURE FillScreen(C:Byte);
BEGIN
 ASM
  mov ax,$a000
  mov es,ax
  mov al,c
  mov ah,al
  cld
  xor di,di
  mov cx,32000
  rep stosw
 END;
END;

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


FUNCTION IsVGA : Boolean; Assembler;
ASM
 MOV AH, 12h
 MOV AL, 00h
 MOV BL, 36h
 INT 10h
 MOV AH, 0
 CMP AL, 12h
 JNZ @Nope
 INC AH
 @Nope:
END;

{GFX file loading & Saving}

Procedure LoadBMP(Name:String);
type Virtual = Array [1..64000] of byte;
VAR PicBuf: ^Virtual;
    Data:File;
    RGB:ARRAY[0..255,1..4] OF Byte;
    Header:Array[1..54] of Byte;
    aAddr :word;
    I:Byte;
BEGIN
 if not fileexists(name) then exit;
 GetMem (PicBuf,64000);
 Assign(Data,Name); Reset(Data,1);
 BlockRead(Data,Header,54);                 { read and ignore :) }
 BlockRead(Data,RGB,1024);                  { pal info }
 FOR I:=0 TO 255 DO SetColor(I,RGB[I,3] div 4,RGB[I,2] div 4,RGB[I,1] div 4);
 BlockRead(Data,PicBuf^,64000);
 Close(Data);
 aAddr := seg (PicBuf^);
 asm                    {AMS routine 2.7 times faster than Pascal one!}
  push si             {wibble, wobble ;) }
  push di             {I'm no ASM, programmer so there MUST me room}
  push es             {for optimisation}
  push ds
  mov ax, $a000
  mov es, ax
  mov ds, [aAddr]
  mov di, 0
  mov dx, 63680
  mov cx, 200
@page:
  push cx
  mov cx, 320
  mov si, dx
@line:
  mov bh, byte ptr ds:[si]
  mov es:[di], bh
  inc di
  inc si
  loop @line
  sub dx, 320
  pop cx
  loop @page
  pop ds
  pop es
  pop di
  pop si
 end;
 FreeMem(PicBuf,64000);
END;

procedure Read_PCX_Line(vidoffset: word);
var c, run: byte;
    n: integer;
    w: word;
begin
 n:=0;
 while (n < bytes) do
 begin
  blockread (f, c, 1);
  if ((c and 192)=192) then
  begin run:=c and 63;
   blockread (f, c, 1);
   n:=n+run;
   for w:=0 to run-1 do
   begin
    mem [$a000:vidoffset]:=c;
    inc (vidoffset);
   end;
  end else
  begin
   n:=n+1;
   mem [$a000:vidoffset]:=c;
   inc (vidoffset);
  end;
 end;
end;

procedure Unpack_PCX_File;
var i: integer;
begin
 for i:=0 to 767 do palette [i]:=palette [i] shr 2;
 asm
  mov ax,1012h
  xor bx,bx
  mov cx,256
  mov dx,offset palette
  int 10h
 end;
 for i:=0 to depth-1 do Read_PCX_Line (i*320);
end;

Procedure LoadPCX(PCXFile:string);
begin
 if not fileexists(pcxfile) then exit;
 assign (f, PCXFile);
 reset (f,1);
 blockread (f, header, sizeof (header));
 if (header.manufacturer=10) and (header.version=5)
 and (header.bits_per_pixel=8) and (header.colour_planes=1) then
 begin
  seek (f, filesize (f)-769);
  blockread (f, c, 1);
  if (c=12) then
  begin
   blockread (f, palette, 768);
   seek (f, 128);
   width:=header.xmax-header.xmin+1;
   depth:=header.ymax-header.ymin+1;
   bytes:=header.bytes_per_line;
   Unpack_PCX_File;
  end;
 end;
 close (f);
end;

Procedure SavePCX (FN : String);
Var Fil : File;
    Ln : Byte;
Procedure Write_Header;
Const
 OldPal : Array [1..48] of Byte = (0,0,0,216,152,56,120,116,4,112,108,4,236,
                                   172,76,248,196,128,64,36,36,36,40,20,248,
                                   188,104,212,144,156,60,36,36,116,112,8,
                                   120,116,8,124,120,8,52,48,4,240,196,136);
Var B,L : Byte;
    I : Integer;
Begin
 B := 10;                              {Manufacturer}
 BlockWrite (Fil,B,1);
 B := 5;                               {Version}
 BlockWrite (Fil,B,1);
 B := 1;                               {Encoding}
 BlockWrite (Fil,B,1);
 B := 8;                               {Bytes Per Pixel}
 BlockWrite (Fil,B,1);
 I := 0;                               {Min X}
 BlockWrite (Fil,I,2);
 I := 0;                               {Min Y}
 BlockWrite (Fil,I,2);
 I := 319;                             {Max X}
 BlockWrite (Fil,I,2);
 I := 199;                             {Max Y}
 BlockWrite (Fil,I,2);
 I := 320;                             {Horizontal Resolution}
 BlockWrite (Fil,I,2);
 I := 200;                             {Vertical Resolution}
 BlockWrite (Fil,I,2);                   {Default Palette}
 BlockWrite (Fil,Mem [Seg (OldPal):Ofs (OldPal)],48);
 B := 0;                               {Reserved}
 BlockWrite (Fil,B,1);
 B := 1;                               {Color Planes}
 BlockWrite (Fil,B,1);
 I := 320;                             {Bytes Per Line}
 BlockWrite (Fil,I,2);
 I := 0;                               {Palette Type}
 BlockWrite (Fil,I,2);
 B := 0;
 For L := 1 to 58 Do BlockWrite (Fil,B,1);
End;

Procedure Encode_Line (Ln : Byte);
Var B : Array [1..64] of Byte;
    I,J,T : Word;
    A : Byte;
    P : Array [0..319] of Byte;
Begin
 I := 0;
 J := 0;
 T := 0;
 Move (Mem [$a000:Ln * 320],P,320);
 While T < 320 Do
 Begin
  I := 0;
  While ((P[T+I] = P[T+I+1]) And ((T+I) < 320) And (I < 63)) Do Inc (I);
  If I > 0 Then
  Begin
   A := I Or 192;
   BlockWrite (Fil,A,1);
   BlockWrite (Fil,P [T],1);
   Inc (T,I);
   Inc (J,2);
  End
  Else
  Begin
   If (((P [T]) And 192) = 192) Then
   Begin
    A := 193;
    BlockWrite (Fil,A,1);
    Inc (J);
   End;
   BlockWrite (Fil,P [T],1);
   Inc (T);
   Inc (J);
  End;
 End;
End;

Procedure Write_Palette;
Var L,R,G,B : Byte;
Begin
 L := 12;
 BlockWrite (Fil,L,1);
 For L := 0 to 255 Do
 Begin
  GetColor (L,R,G,B);
  R := R * 4;
  G := G * 4;
  B := B * 4;
  BlockWrite (Fil,R,1);
  BlockWrite (Fil,G,1);
  BlockWrite (Fil,B,1);
 End;
End;

Begin
 Assign (Fil,FN);
 Rewrite (Fil,1);
 Write_Header;
 For Ln := 0 to 199 Do Encode_Line (Ln);
 Write_Palette;
 Close (Fil);
End;

Function GetChar: Char;
Var C: Char;
Begin
 BlockRead(Datei,C,1);
 GetChar:=C;
End;
Function GetByte: Byte;
Var B: Byte;
Begin
 BlockRead(Datei,B,1);
 GetByte:=B;
End;
Function GetWord: Word;
Var W: Word;
Begin
 BlockRead(Datei,W,2);
 Getword:=W;
End;
Procedure AGetBytes(Anz: Word);
Begin
 BlockRead(Datei,Buffer,Anz);
End;

Procedure LoadGif(N: String);
Var
 lokal_farbtafel: Integer;
 mask,restbytes,pp,lbyte,blocklen,code,oldcode,sonderfall,
 incode,freepos,kanz,pass,clearcode,eofcode,maxcode,infobyte,
 globalfarbtafel,backcolor,interlace,bilddef,abslinks,absoben: word;
 bits,restbits,codesize: Byte;
 rot,gruen,blau,by,bpp: Byte;
 z,i,x1,y1,x2,y2: integer;
 bem: string[6];
 farben: integer;
 x,y,xa,ya,dy: word;
begin
 if not fileexists(n) then exit;
 Assign(Datei,N);
 reset(Datei,1);
 if ioresult>0 then exit;
 bem:='';
 for i:=1 to 6 do bem:=bem+getchar;
 if copy(bem,1,3)<>'GIF' then exit;
 x2:=getword;
 y2:=getword;
 infobyte:=getbyte;
 globalfarbtafel:=infobyte and 128;
 bpp:=(infobyte and 7)+1;
 farben:=1 shl bpp;
 backcolor:=getbyte;
 by:=getbyte;
 if globalfarbtafel<>0 then for i:=0 to (3*farben)-1 do palette[i]:=getbyte shr 2;
 bilddef:=getbyte;
 while bilddef=$21 do
 begin
  by:=getbyte; z:=getbyte;
  for i:=1 to z do by:=getbyte;
  by:=getbyte;
  bilddef:=getbyte;
 end;
 if bilddef<>$2c then exit;
 abslinks:=getword;
 absoben:=getword;
 x2:=getword;
 y2:=getword;
 by:=getbyte;
 lokal_farbtafel:=by and 128;
 interlace:=by and 64;
 by:=getbyte;
 x1:=0; y1:=0; xa:=x2; Ya:=Y2;
 if farben<16 then exit;
 if lokal_farbtafel<>0 then for i:=0 to 3*Farben-1 do palette[I]:=getbyte shr 2;
 asm
  mov ax,$1012
  push ds
  pop es
  xor bx,bx
  mov cx,256
  lea dx,palette
  int $10
  mov pass,0
  MOV CL,bpp
  MOV AX,1
  SHL AX,CL
  MOV clearcode,AX
  INC AX
  MOV eofcode,AX
  INC AX
  MOV freepos,AX
  MOV AL,bpp
  MOV AH,0
  INC AX
  MOV codesize,AL
  MOV CX,AX
  MOV AX,1
  SHL AX,CL
  DEC AX
  MOV maxcode,AX
  MOV kanz,0
  MOV dy,8
  MOV restbits,0
  MOV restbytes,0
  MOV x,0
  MOV y,0
@gif0:
  CALL FAR PTR @getgifbyte
  CMP AX,eofcode
  je @ende1
@gif1:
  CMP AX,clearcode
  je @reset1
@gif3:
  MOV AX,code
  MOV incode,AX
  CMP ax,freepos
  jb @gif4
  MOV AX,oldcode
  MOV code,AX
  MOV BX,kanz
  MOV CX,sonderfall
  SHL BX,1
  MOV [OFFSET keller+BX],CX
  INC kanz
@gif4:
  CMP AX,clearcode
  JB @gif6
@gif5:
  MOV BX,code
  SHL BX,1
  PUSH BX
  MOV AX,[Offset tail+BX]
  MOV BX,kanz
  SHL BX,1
  MOV [OFFSET keller+BX],AX
  INC kanz
  POP BX
  MOV AX,[Offset prefix+BX]
  MOV code,AX
  CMP AX,clearcode
  ja @gif5
@gif6:
  MOV BX,kanz
  SHL BX,1
  MOV [Offset keller+BX],AX
  MOV sonderfall,AX
  INC kanz
@gif7:
  MOV AX,[Offset keller+BX]
  CALL FAR PTR @pixel
  CMP BX,0
  JE @gif8
  DEC BX
  DEC BX
  JMP @gif7
@gif8:
  MOV kanz,0
  MOV BX,freepos
  SHL BX,1
  MOV AX,oldcode
  MOV [Offset prefix+BX],AX
  MOV AX,code
  MOV [Offset tail+BX],AX
  MOV AX,incode
  MOV oldcode,AX
  INC freepos
  MOV AX,freepos
  CMP AX,maxcode
  JBE @gif2
  CMP codesize,12
  JAE @gif2
  INC codesize
  MOV CL,codesize
  MOV AX,1
  SHL AX,CL
  DEC AX
  MOV maxcode,AX
@gif2:
  JMP @gif0
@ende1:
  JMP @ende
@reset1:
  MOV AL,bpp
  MOV AH,0
  INC AX
  MOV codesize,AL
  MOV CX,AX
  MOV AX,1
  SHL AX,CL
  DEC AX
  MOV maxcode,AX
  MOV AX,clearcode
  ADD AX,2
  MOV freepos,AX
  CALL FAR PTR @getgifbyte
  MOV sonderfall,AX
  MOV oldcode,AX
  CALL FAR PTR @pixel
  JMP @gif2
@getgifbyte:
  MOV DI,0
  MOV mask,1
  MOV bits,0
@g1:
  MOV AL,bits
  CMP AL,codesize
  JAE @g0
  CMP restbits,0
  JA @g2
  CMP restbytes,0
  JNE @l2
  PUSH DI
  CALL Getbyte
  POP DI
  MOV blocklen,AX
  MOV restbytes,AX
  PUSH DI
  PUSH AX
  CALL AGetbytes
  POP DI
  MOV pp,0
@l2:
  MOV BX,pp
  MOV AL,[BX+Offset Buffer]
  XOR AH,AH
  INC pp
  DEC restbytes
  MOV lbyte,AX
  MOV restbits,8
@g2:
  SHR lbyte,1
  JNC @nocarry
  OR DI,mask
@nocarry:
  INC bits
  DEC restbits
  SHL mask,1
  JMP @g1
@g0:
  MOV bits,0
  MOV code,DI
  MOV AX,DI
  RETF
@pixel:
  PUSH BX
  MOV BX,x
  ADD BX,abslinks
  PUSH BX
  MOV BX,y
  ADD BX,absoben
  PUSH BX
  PUSH AX
  CALL Putpixel
  POP BX
  INC x
  MOV AX,x
  CMP AX,x2
  JB @s0
  MOV x,0
  CMP interlace,0
  JNE @s1
  INC y
  JMP @s0
@s1:
  MOV AX,dy
  ADD y,AX
  MOV AX,y
  CMP AX,y2
  JB @s0
  INC pass
  CMP pass,1
  JNE @s3
  JMP @s2
@s3:
  SHR dy,1
@s2:
  MOV AX,DY
  SHR AX,1
  MOV Y,AX
@s0:
  RETF
@ende:
 End;
 Close(Datei);
End;

PROCEDURE LoadPalette(DateiName:String);
VAR Datei:File;
    RGB:ARRAY[0..255,1..3] OF Byte;
    I:Byte;
BEGIN
 if not fileexists(dateiname) then exit;
 Assign(Datei,DateiName);
 Reset(Datei,1);
 BlockRead(Datei,RGB,768);
 FOR I:=0 TO 255 DO SetColor(I,RGB[I,1],RGB[I,2],RGB[I,3]);
END;

PROCEDURE SavePalette(DateiName:String);
VAR Datei:File;
    RGB:ARRAY[0..255,1..3] OF Byte;
    I:Byte;
BEGIN
 Assign(Datei,DateiName);
 Rewrite(Datei,1);
 FOR I:=0 TO 255 DO GetColor(I,RGB[I,1],RGB[I,2],RGB[I,3]);
 BlockWrite(Datei,RGB,768);
END;

PROCEDURE LoadRaw(DateiName:String);
VAR Datei:File;
BEGIN
 if not fileexists(dateiname) then exit;
 Assign(Datei,DateiName);
 Reset(Datei,1);
 BlockRead(Datei,Ptr($A000,0)^,64000);
 Close(Datei);
END;

PROCEDURE SaveRaw(DateiName:String);
VAR Datei:File;
BEGIN
 Assign(Datei,DateiName);
 Rewrite(Datei,1);
 BlockWrite(Datei,Ptr($A000,0)^,64000);
 Close(Datei);
END;

Procedure FlushKeyBoard;
begin
 While Keypressed do ReadKey;
end;






















{
----------------------------------------------------------------------------
 Misc GUI Functions
----------------------------------------------------------------------------
}

Procedure InitGUI;
begin
 MCGAOn;
 MouseInit;
 If not MouseExist then
 begin
  Writeln ('No mouse-driver found!');
  halt(1);
 end;
 mouseon;
end;

Procedure CloseGUI;
begin
 mouseoff;
 MCGAOff;
end;

Procedure SetGUIColors;
begin
 GetColor(LightColor,r1,g1,b1);
 GetColor(ShadowColor,r2,g2,b2);
 GetColor(BarColor,r3,g3,b3);
 GetColor(TextColor,r4,g4,b4);
 SetColor(LightColor,60,60,60);
 SetColor(ShadowColor,20,20,20);
 SetColor(BarColor,40,40,40);
 SetColor(TextColor,63,63,63);
end;

Procedure FindGUIColors;
begin
 LightColor := ClosestColor(60,60,60);
 ShadowColor := ClosestColor(20,20,20);
 BarColor := ClosestColor(40,40,40);
 TextColor := ClosestColor(63,63,63);
end;

Procedure RestoreGUIColors;
begin
 SetColor(LightColor,r1,g1,b1);
 SetColor(ShadowColor,r2,g2,b2);
 SetColor(BarColor,r3,g3,b3);
 SetColor(TextColor,r4,g4,b4);
end;

{
----------------------------------------------------------------------------
 Low level GUI Functions
----------------------------------------------------------------------------
}

Procedure DrawChar (x,y:integer;c:char;clr:byte);
var Poss : word;
    i,j:integer;
    cr : byte;
begin
 cr := ord(ConvTo[pos(upcase(c),ConvFrom)]);
 for i := 4 downto 0 do
 begin
  for j := 1 to 5 do
  begin
   if Font[cr][i][j] = 'x' then
   begin
    Mem[$a000:(y+i)*320+x+j] := clr;
   end;
  end;
 end;
end;

Procedure DrawString(x,y:integer;s:string;clr:byte);
var k:integer;
begin
 for k := 1 to length(s) do DrawChar(x+(k-1)*6,y,s[k],clr);
end;

Procedure Panel(x1,y1,x2,y2:integer;up:boolean);
Begin
 MouseOff;
 if up then
 begin
  FillBlock(x1,y1,x2,y2,BarColor);
  DrawLineH(x1,x2-1,y1,LightColor);
  DrawLineV(x1,y1,y2-1,LightColor);
  DrawLineH(x1+1,x2,y2,ShadowColor);
  DrawLineV(x2,y1+1,y2,ShadowColor);
 end
 else
 begin
  FillBlock(x1,y1,x2,y2,BarColor);
  DrawLineH(x1,x2-1,y1,ShadowColor);
  DrawLineV(x1,y1,y2-1,ShadowColor);
  DrawLineH(x1+1,x2,y2,LightColor);
  DrawLineV(x2,y1+1,y2,LightColor);
 end;
 MouseOn;
end;

{
----------------------------------------------------------------------------
 Draw Items
----------------------------------------------------------------------------
}

Procedure Draw3DString(x,y:integer;s:string;up:boolean);
var k:integer;
begin
 if up then
 begin
  for k := 1 to length(s) do DrawChar(x+(k-1)*6+1,y+1,s[k],ShadowColor);
  for k := 1 to length(s) do DrawChar(x+(k-1)*6,y,s[k],TextColor);
 end
 else
 begin
  for k := 1 to length(s) do DrawChar(x+(k-1)*6+1,y+1,s[k],TextColor);
  for k := 1 to length(s) do DrawChar(x+(k-1)*6,y,s[k],ShadowColor);
 end;
end;

Procedure DrawButton(x1,y1,x2,y2:integer;title:string;upp:boolean);
var tx1,ty1:integer;
begin
 Panel(x1,y1,x2,y2,upp);
 Tx1 := (x1+(x2-x1) div 2) - (length(title)*3);
 Ty1 := (y1+(y2-y1) div 2) - 2;
 Draw3DString(tx1,ty1,title,upp);
end;

Procedure DrawInputField(x1,y1,x2,y2:integer;S:string);
begin
 Panel(x1,y1,x2,y2,down);
 DrawString(x1+1,y1+2,s,ShadowColor);
end;

{
----------------------------------------------------------------------------
 Dialog
----------------------------------------------------------------------------
}

Procedure HandleInputField(x,y:integer;var I:Itemtype);
 var cx,cy:integer;
     s:string;
     c:char;
     maxlen : byte;
     quit:boolean;
begin
 FlushKeyboard;
 cx := x+1;
 cy := y+2;
 s := i.text;
 cx := cx + length(s)*6;
 maxlen := i.data;
 mouseoff;
 DrawString(cx,cy,'Û',ShadowColor);
 mouseon;
 Quit := false;
 Repeat
  if keypressed then c := readkey;
  case upcase(c) of
   #13      : Quit := true;
   #8       : begin
               if length(s)>0 then
               begin
                mouseoff;
                s:=copy(s,1,length(s)-1);
                DrawChar(cx,cy,'Û',BarColor);
                cx := cx - 6;
                DrawChar(cx,cy,'Û',ShadowColor);
                mouseon;
               end;
              end;
   else       begin
               if pos(upcase(c),ConvFrom)>0 then
               begin
                if length(s)<maxlen then
                begin
                 mouseoff;
                 s:=s+upcase(c);
                 DrawChar(cx,cy,'Û',BarColor);
                 DrawChar(cx,cy,c,ShadowColor);
                 cx := cx + 6;
                 DrawChar(cx,cy,'Û',ShadowColor);
                 mouseon;
                end;
               end;
              end;
  end;
  c := #0;
 until Quit;
 mouseoff;
 DrawChar(cx,cy,'Û',BarColor);
 i.text := s;
 mouseon;
end;

Procedure InitDialog(var D:DialogType;title:string;x1,y1,x2,y2:integer);
var i:integer;
begin
 D.title := title;
 D.x1 := x1;
 D.y1 := y1;
 D.x2 := x2;
 D.y2 := y2;
 D.Numitems := 0;
 for i := 1 to MaxItems do D.Item[i].active := false;
 D.P := NIL;
end;

Procedure RemoveDialog(D:DialogType);
begin
 mouseoff;
 PutImage(d.x1,d.y1,d.p^);
 freemem(d.p,d.psize);
 mouseon;
end;

Procedure DrawDialog(var D:DialogType);
var i,ix1,iy1,ix2,iy2 : integer;
    tx1 : integer;
begin
 Mouseoff;
 d.psize := (d.y2-d.y1)*(d.x2-d.x1)+4;
 getmem(d.p,d.psize);
 GetImage(d.x1,d.y1,d.x2,d.y2,d.p^);
 FindGUIColors;
 Panel(d.x1,d.y1,d.x2,d.y2,up);
 Panel(d.x1+2,d.y1+9,d.x2-2,d.y2-2,down);
 Tx1 := (d.x1+(d.x2-d.x1) div 2) - (length(d.title)*3);
 Draw3DString(tx1,d.y1+2,d.title,up);
 Dx1 := d.x1 + 4;
 Dy1 := d.y1 + 11;
 for i := 1 to d.NumItems do
 begin
  ix1 := dx1+d.item[i].x1;
  iy1 := dy1+d.item[i].y1;
  ix2 := dx1+d.item[i].x2;
  iy2 := dy1+d.item[i].y2;
  case d.item[i].itype of
   1 : DrawButton(ix1,iy1,ix2,iy2,d.item[i].text,up);
   2 : DrawInputField(ix1,iy1,ix2,iy2,d.item[i].text);
   3 : DrawString(ix1,iy1,d.item[i].text,ShadowColor);
  end;
 end;
 mouseon;
end;

Procedure ReDrawDialog(var D:DialogType);
var i,ix1,iy1,ix2,iy2 : integer;
    tx1 : integer;
begin
 Mouseoff;
 FindGUIColors;
 Panel(d.x1,d.y1,d.x2,d.y2,up);
 Panel(d.x1+2,d.y1+9,d.x2-2,d.y2-2,down);
 Tx1 := (d.x1+(d.x2-d.x1) div 2) - (length(d.title)*3);
 Draw3DString(tx1,d.y1+2,d.title,up);
 Dx1 := d.x1 + 4;
 Dy1 := d.y1 + 11;
 for i := 1 to d.NumItems do
 begin
  ix1 := dx1+d.item[i].x1;
  iy1 := dy1+d.item[i].y1;
  ix2 := dx1+d.item[i].x2;
  iy2 := dy1+d.item[i].y2;
  case d.item[i].itype of
   1 : DrawButton(ix1,iy1,ix2,iy2,d.item[i].text,up);
   2 : DrawInputField(ix1,iy1,ix2,iy2,d.item[i].text);
   3 : DrawString(ix1,iy1,d.item[i].text,ShadowColor);
  end;
 end;
 mouseon;
end;

Procedure ReDrawItem(var D:DialogType;i:byte);
var ix1,iy1,ix2,iy2 : integer;
    tx1 : integer;
begin
 Mouseoff;
{ FindGUIColors;}
 Dx1 := d.x1 + 4;
 Dy1 := d.y1 + 11;
 ix1 := dx1+d.item[i].x1;
 iy1 := dy1+d.item[i].y1;
 ix2 := dx1+d.item[i].x2;
 iy2 := dy1+d.item[i].y2;
 case d.item[i].itype of
  1 : DrawButton(ix1,iy1,ix2,iy2,d.item[i].text,up);
  2 : DrawInputField(ix1,iy1,ix2,iy2,d.item[i].text);
  3 : DrawString(ix1,iy1,d.item[i].text,ShadowColor);
 end;
 mouseon;
end;

Function FindID(D:DialogType;n:Byte):byte;
var i,res:byte;
begin
 i := 0;
 repeat
  inc(i);
 until (D.Item[i].ID = n) or (i>MaxItems);
 FindID := i;
end;


Function HandleDialog(var D:Dialogtype):byte;
var quit:boolean;
    rcode:byte;
    ix1,iy1,ix2,iy2:integer;
    s:string;
    hdi:integer;
begin
 rcode := 0;
 repeat
  if MouseLeftInArea(D.x1,D.y1,D.x2,D.y2) then
  begin
   for hdi := 1 to D.NumItems do
   begin
    ix1 := d.X1+4 + D.item[hdi].X1;
    iy1 := d.Y1+11 + D.item[hdi].Y1;
    ix2 := d.X1+4 + D.item[hdi].X2;
    iy2 := d.Y1+11 + D.item[hdi].Y2;
    if D.Item[hdi].active then
    begin
     if MouseLeftInArea(ix1,iy1,ix2,iy2) then
     begin
      rcode := D.item[hdi].ID;
      quit := true;
      case D.item[hdi].itype of
       1 : Begin
            mouseoff;
            DrawButton(ix1,iy1,ix2,iy2,d.item[hdi].text,down);
            mouseon;
            repeat until not MouseLeft;
            mouseoff;
            DrawButton(ix1,iy1,ix2,iy2,d.item[hdi].text,up);
            mouseon;
           end;
       2 : begin {InputField}
            HandleInputField(ix1,iy1,D.item[hdi]);
           end;
       3 : Begin {Text}
            repeat until not MouseLeft;
           end;
      end;
     end;
    end;
   end;
  end;
 until quit = true;
 HandleDialog := rcode;
end;

Procedure AddButton (var D:DialogType;x1,y1,x2,y2:integer;t:string;id:byte);
var i:integer;
begin
 if D.NumItems > MaxItems then exit else
 begin
  i := D.NumItems+1;
  inc(D.NumItems);
 end;
 d.item[i].active := true;
 d.item[i].itype := 1;
 d.item[i].ID := id;
 d.item[i].text := t;
 d.item[i].x1 := x1;
 d.item[i].y1 := y1;
 d.item[i].x2 := x2;
 d.item[i].y2 := y2;
end;

ProceDure AddInputField(var D:Dialogtype;x1,y1,maxlen:integer;s:string;id:byte);
var i:integer;
begin
 if D.NumItems > MaxItems then exit else
 begin
  i := D.NumItems+1;
  inc(D.NumItems);
 end;
 d.item[i].active := true;
 d.item[i].itype := 2;
 d.item[i].ID := id;
 d.item[i].text := s;
 d.item[i].x1 := x1;
 d.item[i].y1 := y1;
 d.item[i].x2 := x1+maxlen*6+8;
 d.item[i].y2 := y1+8;
 d.item[i].data := maxlen;
end;

Procedure AddText(var D:DialogType;x1,y1:integer;t:string;id:byte);
var i:integer;
begin
 if D.NumItems > MaxItems then exit else
 begin
  i := D.NumItems+1;
  inc(D.NumItems);
 end;
 d.item[i].active := true;
 d.item[i].itype := 3;
 d.item[i].ID := id;
 d.item[i].text := t;
 d.item[i].x1 := x1;
 d.item[i].y1 := y1;
 d.item[i].x2 := x1+length(t)*6-1;
 d.item[i].y2 := y1+5;
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

Function ClosestColor(r,g,b:byte):byte;
var i:integer;
    closest:byte;
    r1,g1,b1:byte;
    tc : word;
    t : word;
begin
 closest := 0;
 tc := 65535;
 for i := 0 to 255 do
 begin
  GetColor(i,r1,g1,b1);
  t := sqrRoot((r-r1)*(r-r1) + (g-g1)*(g-g1) + (b-b1)*(b-b1));
  if t < tc then
  begin
   closest := i;
   tc := t;
  end;
 end;
 ClosestColor := closest;
end;



begin
end.