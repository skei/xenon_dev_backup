Unit MCGA;

interface

FUNCTION IsVGA : Boolean;
procedure setmode(m : byte);
function getmode : byte;
Procedure DrawChar(x,y:word;ch:char;color:byte);
Procedure DrawString(x,y:word;s:string;c:byte);
PROCEDURE WaitRetrace;
procedure line (fromx,fromy,tox,toy,color : integer);
Procedure HorizLine (x1,y,x2:integer;c:byte);
Procedure VertLine (x,y1,y2:integer;c:byte);
Procedure Bar (x1,y1,x2,y2:word;color:byte);
procedure Xorline (fromx,fromy,tox,toy : integer);
PROCEDURE fillscreen (col:BYTE);
Procedure PutPixel(x,y:word;col:byte);
Function GetPixel(x,y:word) : byte;
Procedure XorPixel(x,y:word);
PROCEDURE getimage(p:POINTER;xp,yp:WORD;xs,ys:word);
PROCEDURE putimage(p:POINTER;xp,yp:WORD;xs,ys:word);
Procedure SetPal(ColorNo,R,G,B : Byte);
Procedure GetPal (ColorNo : Byte; Var R,G,B : Byte);

implementation

var screen : array[0..199,0..319] of byte absolute $a000:0;

Procedure Font;assembler;
asm
db 00000000b
db 00000000b
db 00000000b
db 00000000b
db 00000000b

db 00100000b
db 00100000b
db 00100000b
db 00000000b
db 00100000b

db 01010000b
db 01010000b
db 00000000b
db 00000000b
db 00000000b

db 01010000b
db 11111000b
db 01010000b
db 11111000b
db 01010000b

db 00100000b
db 01110000b
db 01100000b
db 00110000b
db 01110000b

db 11001000b
db 11010000b
db 00100000b
db 01011000b
db 10011000b

db 01100000b
db 01101000b
db 01110000b
db 10010000b
db 01101000b

db 00100000b
db 00100000b
db 00000000b
db 00000000b
db 00000000b

db 00010000b
db 00100000b
db 00100000b
db 00100000b
db 00010000b

db 01000000b
db 00100000b
db 00100000b
db 00100000b
db 01000000b

db 10101000b
db 01110000b
db 00100000b
db 01110000b
db 10101000b

db 00100000b
db 00100000b
db 11111000b
db 00100000b
db 00100000b

db 00000000b
db 00000000b
db 00000000b
db 00100000b
db 01000000b

db 00000000b
db 00000000b
db 11111000b
db 00000000b
db 00000000b

db 00000000b
db 00000000b
db 00000000b
db 00000000b
db 00100000b

db 00001000b
db 00010000b
db 00100000b
db 01000000b
db 10000000b

db 01110000b
db 10001000b
db 10001000b
db 10001000b
db 01110000b

db 00010000b
db 00110000b
db 00010000b
db 00010000b
db 00111000b

db 01110000b
db 00001000b
db 01110000b
db 10000000b
db 11111000b

db 11110000b
db 00001000b
db 01110000b
db 00001000b
db 11110000b

db 00010000b
db 10010000b
db 11110000b
db 00010000b
db 00010000b

db 11110000b
db 10000000b
db 11110000b
db 00001000b
db 11110000b

db 01110000b
db 10000000b
db 11110000b
db 10001000b
db 01110000b

db 01111000b
db 00001000b
db 00010000b
db 00100000b
db 00100000b

db 01110000b
db 10001000b
db 01110000b
db 10001000b
db 01110000b

db 01110000b
db 10001000b
db 01111000b
db 00001000b
db 01110000b

db 00000000b
db 00100000b
db 00000000b
db 00100000b
db 00000000b

db 00000000b
db 00100000b
db 00000000b
db 00100000b
db 01000000b

db 00010000b
db 00100000b
db 01000000b
db 00100000b
db 00010000b

db 00000000b
db 11111000b
db 00000000b
db 11111000b
db 00000000b

db 01000000b
db 00100000b
db 00010000b
db 00100000b
db 01000000b

db 01110000b
db 00001000b
db 00110000b
db 00000000b
db 00100000b

db 01110000b
db 10111000b
db 10111000b
db 10000000b
db 01110000b

db 01110000b
db 10001000b
db 11111000b
db 10001000b
db 10001000b

db 11110000b
db 10001000b
db 11110000b
db 10001000b
db 11110000b

db 01110000b
db 10000000b
db 10000000b
db 10000000b
db 01110000b

db 11110000b
db 10001000b
db 10001000b
db 10001000b
db 11110000b

db 11111000b
db 10000000b
db 11110000b
db 10000000b
db 11111000b

db 11111000b
db 10000000b
db 11110000b
db 10000000b
db 10000000b

db 01111000b
db 10000000b
db 10111000b
db 10001000b
db 01111000b

db 10001000b
db 10001000b
db 11111000b
db 10001000b
db 10001000b

db 11111000b
db 00100000b
db 00100000b
db 00100000b
db 11111000b

db 01111000b
db 00010000b
db 00010000b
db 10010000b
db 01100000b

db 10001000b
db 10010000b
db 11100000b
db 10010000b
db 10001000b

db 10000000b
db 10000000b
db 10000000b
db 10000000b
db 11111000b

db 10001000b
db 11011000b
db 10101000b
db 10001000b
db 10001000b

db 10001000b
db 11001000b
db 10101000b
db 10011000b
db 10001000b

db 01110000b
db 10001000b
db 10001000b
db 10001000b
db 01110000b

db 11110000b
db 10001000b
db 11110000b
db 10000000b
db 10000000b

db 01110000b
db 10001000b
db 10101000b
db 10011000b
db 01111000b

db 11110000b
db 10001000b
db 11110000b
db 10010000b
db 10001000b

db 01110000b
db 10000000b
db 01110000b
db 00001000b
db 01110000b

db 11111000b
db 00100000b
db 00100000b
db 00100000b
db 00100000b

db 10001000b
db 10001000b
db 10001000b
db 10001000b
db 01110000b

db 10001000b
db 10001000b
db 01010000b
db 01010000b
db 00100000b

db 10001000b
db 10001000b
db 10101000b
db 11011000b
db 10001000b

db 10001000b
db 01010000b
db 00100000b
db 01010000b
db 10001000b

db 10001000b
db 10001000b
db 01111000b
db 00001000b
db 01110000b

db 11111000b
db 00010000b
db 00100000b
db 01000000b
db 11111000b

db 01110000b
db 01000000b
db 01000000b
db 01000000b
db 01110000b

db 10000000b
db 01000000b
db 00100000b
db 00010000b
db 00001000b

db 01110000b
db 00010000b
db 00010000b
db 00010000b
db 01110000b

db 00100000b
db 01010000b
db 00000000b
db 00000000b
db 00000000b

db 00000000b
db 00000000b
db 00000000b
db 00000000b
db 11111100b

db 11111000b
db 11111000b
db 11111000b
db 11111000b
db 11111000b

db 00000000b
db 01110000b
db 01110000b
db 01110000b
db 00000000b

db 00100000b
db 01110000b
db 01110000b
db 11111000b
db 11111000b

db 11111000b
db 11111000b
db 01110000b
db 01110000b
db 00100000b

db 00011000b
db 01111000b
db 11111000b
db 01111000b
db 00011000b

db 11000000b
db 11110000b
db 11111000b
db 11110000b
db 11000000b
end;


CONST Chars = ' !"#$%&'+#39+'()*+,-./0123456789:;<=>?@'+
              'ABCDEFGHIJKLMNOPQRSTUVWXYZ[\]^_Ûþ'+#1+#2+#3+#4;

Procedure DrawChar(x,y:word;ch:char;color:byte);
var num:word;
begin
 num := pos(ch,chars);
 if num = 0 then exit else num := ofs(font) + (num-1)*5;
 asm
  mov si,num             {DI -> Char}
  push ds
  mov ax,SEG Font
  mov ds,ax
  mov ax,0a000h
  mov es,ax
  mov ax,y
  mov bx,320
  mul bx
  mov di,ax
  add di,x       {DI -> ScreenPos}
  mov cx,5
  mov al,color
@LineLoop:
  mov bl,[ds:si]
  inc si
  test bl,128
  jz @a
  mov [es:di],al
@a:
  test bl,64
  jz @b
  mov [es:di+1],al
@b:
  test bl,32
  jz @c
  mov [es:di+2],al
@c:
  test bl,16
  jz @d
  mov [es:di+3],al
@d:
  test bl,8
  jz @e
  mov [es:di+4],al
@e:
  add di,320
  dec cx
  jnz @LineLoop
  pop ds
 end;
end;

Procedure DrawString(x,y:word;s:string;c:byte);
var i:integer;
begin
 for i := 1 to length(s) do
 begin
  DrawChar(x+(i-1)*6,y,upcase(s[i]),c);
 end;
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

procedure setmode(m : byte);
begin
 asm
  mov ah, 00h
  mov al, m
  int 10h
 end;
end;

function getmode : byte;
var check_b : byte;
begin
 asm
  mov ah, 0fh
  int 10h
  mov check_b, al
 end;
 if check_b > 127 then getmode:=check_b-128
 else getmode:=check_b;
end;

PROCEDURE WaitRetrace;
begin
 while ((Port[$3DA] AND 8) > 0) do;
 while ((Port[$3DA] AND 8) = 0) do;
end;

procedure line (fromx,fromy,tox,toy,color : integer);
var temp,dx,dy,xincbefore,xincafter,yincbefore,yincafter : integer;
    curpoint,accumul : integer;
begin
 xincbefore := 1; xincafter := 0;
 yincbefore := 0; yincafter := 1;
 dx := tox-fromx;
 if dx<0 then
 begin
  dx := abs(dx);
  xincbefore := -1;
 end;
 dy := toy-fromy;
 if dy<0 then
 begin
  dy := abs(dy);
  yincafter := -1;
 end;
 if dx<dy then
 begin
  temp := dx;
  dx := dy;
  dy := temp;
  xincafter := xincbefore; xincbefore := 0;
  yincbefore := yincafter ; yincafter := 0;
 end;
 accumul := dx div 2;
 for curpoint := 1 to dx do
 begin
  PutPixel(fromx,fromy,color);
  inc(fromx,xincbefore);
  inc(fromy,yincbefore);
  inc(accumul,dy);
  if accumul > dx then
  begin
   dec(accumul,dx);
   inc(fromx,xincafter);
   inc(fromy,yincafter);
  end;
 end;
end;

Procedure HorizLine (x1,y,x2:integer;c:byte);
{var i:integer;}
begin
 fillchar (ptr($a000,y*320+x1)^,x2-x1+1,chr(c));
{ for i := x1 to x2 do mem[$a000:y*320+i] := c;}
end;

Procedure VertLine (x,y1,y2:integer;c:byte);
var i:integer;
begin
 for i := y1 to y2 do mem[$a000:i*320+x] := c;
end;

Procedure Bar (x1,y1,x2,y2:word;color:byte);
var i:integer;
begin
 for i := y1 to y2 do
 begin
  HorizLine(x1,i,x2,color);
 end;
end;


procedure Xorline (fromx,fromy,tox,toy : integer);
var temp,dx,dy,xincbefore,xincafter,yincbefore,yincafter : integer;
    curpoint,accumul : integer;
begin
 xincbefore := 1; xincafter := 0;
 yincbefore := 0; yincafter := 1;
 dx := tox-fromx;
 if dx<0 then
 begin
  dx := abs(dx);
  xincbefore := -1;
 end;
 dy := toy-fromy;
 if dy<0 then
 begin
  dy := abs(dy);
  yincafter := -1;
 end;
 if dx<dy then
 begin
  temp := dx;
  dx := dy;
  dy := temp;
  xincafter := xincbefore; xincbefore := 0;
  yincbefore := yincafter ; yincafter := 0;
 end;
 accumul := dx div 2;
 for curpoint := 1 to dx do
 begin
  XorPixel(fromx,fromy);
  inc(fromx,xincbefore);
  inc(fromy,yincbefore);
  inc(accumul,dy);
  if accumul > dx then
  begin
   dec(accumul,dx);
   inc(fromx,xincafter);
   inc(fromy,yincafter);
  end;
 end;
end;


PROCEDURE fillscreen (col:BYTE);
BEGIN
  FILLCHAR(Screen,63999,CHR(col));
END;


Procedure PutPixel(x,y:word;col:byte);
begin
 Screen[y,x] := col;
end;

Function GetPixel(x,y:word) : byte;
begin
 GetPixel := screen[y,x];
end;

Procedure XorPixel(x,y:word);
begin
 screen[y,x] := 255-screen[y,x];
end;


PROCEDURE getimage(p:POINTER;xp,yp:WORD;xs,ys:word);
VAR s,o : WORD;
BEGIN
 s:=SEG(p^);
 o:=OFS(p^);
 FOR yp:=yp TO PRED(yp+ys) DO
 BEGIN
  MOVE(screen[yp,xp],MEM[s:o],xs);
  INC(o,xs);
 END;
END;

PROCEDURE putimage(p:POINTER;xp,yp:WORD;xs,ys:word);
VAR s,o : WORD;
BEGIN
 s:=SEG(p^);
 o:=OFS(p^);
 FOR yp:=yp TO PRED(yp+ys) DO
 BEGIN
  MOVE(MEM[s:o],screen[yp,xp],xs);
  INC(o,xs);
 END;
END;

Procedure SetPal(ColorNo,R,G,B : Byte);
begin
 port[$3c8] := colorNo;
 port[$3c9] := R;
 port[$3c9] := G;
 port[$3c9] := B;
end;

Procedure GetPal (ColorNo : Byte; Var R,G,B : Byte);
Begin
 Port [$3c7] := ColorNo;
 R := Port [$3c9];
 G := Port [$3c9];
 B := Port [$3c9];
End;

begin
end.