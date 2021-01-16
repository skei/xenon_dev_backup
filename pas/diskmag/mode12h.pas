Unit Mode12h;

interface

Procedure GFXMode;
Procedure TXTMode;
Procedure SetPlane(BitP:byte);
Procedure SetPalette(var pal);
Procedure SetStartAdr(a:word);
Procedure DrawChar(x,y:word;ch:char;plane:byte);
Procedure DrawString(x,y:word;s:string;p:byte);


implementation

CONST Chars = ' ABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890.,'+#39+'!/+-()þ°±²Û';
Procedure Font;assembler;
asm
db 00000000b
db 00000000b
db 00000000b
db 00000000b
db 00000000b
db 00000000b
db 00000000b
db 00000000b

db 00000000b
db 01111100b
db 11100110b
db 11100110b
db 11111110b
db 11100110b
db 11100110b
db 11100110b

db 00000000b
db 11111100b
db 11100110b
db 11100110b
db 11111100b
db 11100110b
db 11100110b
db 11111100b

db 00000000b
db 01111100b
db 11100110b
db 11100000b
db 11100000b
db 11100000b
db 11100110b
db 01111100b

db 00000000b
db 11111100b
db 11100110b
db 11100110b
db 11100110b
db 11100110b
db 11100110b
db 11111100b

db 00000000b
db 11111110b
db 11100110b
db 11100000b
db 11111000b
db 11100000b
db 11100110b
db 11111110b

db 00000000b
db 11111110b
db 11100110b
db 11100000b
db 11111000b
db 11100000b
db 11100000b
db 11100000b

db 00000000b
db 01111110b
db 11100110b
db 11100000b
db 11101110b
db 11100110b
db 11100110b
db 01111110b

db 00000000b
db 11100110b
db 11100110b
db 11100110b
db 11111110b
db 11100110b
db 11100110b
db 11100110b

db 00000000b
db 11111110b
db 00111000b
db 00111000b
db 00111000b
db 00111000b
db 00111000b
db 11111110b

db 00000000b
db 01111110b
db 00001110b
db 00001110b
db 00001110b
db 00001110b
db 11001110b
db 01111100b

db 00000000b
db 11100110b
db 11100110b
db 11101100b
db 11111000b
db 11101100b
db 11100110b
db 11100110b

db 00000000b
db 11100000b
db 11100000b
db 11100000b
db 11100000b
db 11100000b
db 11100110b
db 11111110b

db 00000000b
db 11111100b
db 11010110b
db 11010110b
db 11010110b
db 11010110b
db 11010110b
db 11010110b

db 00000000b
db 11111100b
db 11100110b
db 11100110b
db 11100110b
db 11100110b
db 11100110b
db 11100110b

db 00000000b
db 01111100b
db 11100110b
db 11100110b
db 11100110b
db 11100110b
db 11100110b
db 01111100b

db 00000000b
db 11111100b
db 11100110b
db 11100110b
db 11100110b
db 11111100b
db 11100000b
db 11100000b

db 00000000b
db 01111100b
db 11100110b
db 11100110b
db 11100110b
db 11101010b
db 11100100b
db 01111010b

db 00000000b
db 11111100b
db 11100110b
db 11100110b
db 11111100b
db 11100110b
db 11100110b
db 11100110b

db 00000000b
db 01111110b
db 11100110b
db 11100000b
db 01111100b
db 00000110b
db 11100110b
db 11111100b

db 00000000b
db 11111110b
db 00111000b
db 00111000b
db 00111000b
db 00111000b
db 00111000b
db 00111000b

db 00000000b
db 11100110b
db 11100110b
db 11100110b
db 11100110b
db 11100110b
db 11100110b
db 01111100b

db 00000000b
db 11100110b
db 11100110b
db 11100110b
db 11100110b
db 01100110b
db 00110110b
db 00011110b

db 00000000b
db 11010110b
db 11010110b
db 11010110b
db 11010110b
db 11010110b
db 11010110b
db 01111110b

db 00000000b
db 11100110b
db 11100110b
db 01101100b
db 00111000b
db 01101100b
db 11100110b
db 11100110b

db 00000000b
db 11100110b
db 11100110b
db 11100110b
db 01111100b
db 00111000b
db 00111000b
db 00111000b

db 00000000b
db 11111110b
db 11001110b
db 00011100b
db 00111000b
db 01110000b
db 11100110b
db 11111110b

db 00000000b
db 00011000b
db 00111000b
db 01111000b
db 00111000b
db 00111000b
db 00111000b
db 11111110b

db 00000000b
db 01111100b
db 11001110b
db 00011100b
db 00111000b
db 01110000b
db 11100110b
db 11111110b

db 00000000b
db 11111100b
db 11001110b
db 00001110b
db 00111100b
db 00001110b
db 11001110b
db 01111100b

db 00000000b
db 11100110b
db 11100110b
db 11100110b
db 11111110b
db 00000110b
db 00000110b
db 00000110b

db 00000000b
db 11111110b
db 11100110b
db 11100000b
db 11111100b
db 00000110b
db 11100110b
db 11111100b

db 00000000b
db 01111100b
db 11100110b
db 11100000b
db 11111100b
db 11100110b
db 11100110b
db 01111100b

db 00000000b
db 11111110b
db 11001110b
db 00011100b
db 00111000b
db 00111000b
db 00111000b
db 00111000b

db 00000000b
db 01111100b
db 11100110b
db 11100110b
db 01111100b
db 11100110b
db 11100110b
db 01111100b

db 00000000b
db 01111100b
db 11100110b
db 11100110b
db 01111110b
db 00000110b
db 11100110b
db 01111100b

db 00000000b
db 01111100b
db 11100110b
db 11100110b
db 11100110b
db 11100110b
db 11100110b
db 01111100b

db 00000000b
db 00000000b
db 00000000b
db 00000000b
db 00000000b
db 00000000b
db 00111000b
db 00111000b

db 00000000b
db 00000000b
db 00000000b
db 00000000b
db 00000000b
db 00111000b
db 00111000b
db 01110000b

db 00000000b
db 00111000b
db 00111000b
db 01110000b
db 00000000b
db 00000000b
db 00000000b
db 00000000b

db 00000000b
db 00111000b
db 00111000b
db 00111000b
db 00111000b
db 00111000b
db 00000000b
db 00111000b

db 00000000b
db 00000110b
db 00001110b
db 00011100b
db 00111000b
db 01110000b
db 11100000b
db 11000000b

db 00000000b
db 00000000b
db 00111000b
db 00111000b
db 11111110b
db 00111000b
db 00111000b
db 00000000b

db 00000000b
db 00000000b
db 00000000b
db 00000000b
db 11111110b
db 00000000b
db 00000000b
db 00000000b

db 00000000b
db 00111100b
db 01110000b
db 01110000b
db 01110000b
db 01110000b
db 01110000b
db 00111100b

db 00000000b
db 01111000b
db 00011100b
db 00011100b
db 00011100b
db 00011100b
db 00011100b
db 01111000b

db 00000000b
db 00000000b
db 01111100b
db 01111100b
db 01111100b
db 01111100b
db 01111100b
db 00000000b

db 10001000b
db 00100010b
db 10001000b
db 00100010b
db 10001000b
db 00100010b
db 10001000b
db 00100010b

db 10101010b
db 01010101b
db 10101010b
db 01010101b
db 10101010b
db 01010101b
db 10101010b
db 01010101b

db 01110111b
db 11011101b
db 01110111b
db 11011101b
db 01110111b
db 11011101b
db 01110111b
db 11011101b

db 11111111b
db 11111111b
db 11111111b
db 11111111b
db 11111111b
db 11111111b
db 11111111b
db 11111111b
end;


Procedure DrawChar(x,y:word;ch:char;plane:byte);
var num:word;
begin
 SetPlane(plane);
 num := pos(upcase(ch),chars);
 if num = 0 then exit else num := ofs(font) + ((num-1) shl 3);
 asm
  mov si,num            {DI -> Char}
  push ds
  mov ax,SEG Font
  mov ds,ax             {DS:SI -> Font/Char data}
  mov ax,0a000h
  mov es,ax
  mov ax,y
  mov bx,80
  mul bx
  mov di,ax
  add di,x              {DI -> ScreenPos}
  cld
  movsb
  add di,79
  movsb
  add di,79
  movsb
  add di,79
  movsb
  add di,79
  movsb
  add di,79
  movsb
  add di,79
  movsb
  add di,79
  movsb
  pop ds
 end;
end;

Procedure DrawString(x,y:word;s:string;p:byte);
var i:byte;
begin
 for i := 0 to length(s)-1 do DrawChar(x+i,y,s[i+1],p);
end;

{ BitP = bits 0..3 = Active plane..
{Note: Stored byte is saved to all enabled planes}
Procedure SetPlane(BitP:byte);
begin
 asm
  mov dx,3c4h
  mov ah,BitP
  mov al,2
  out dx,ax
 end;
end;

Procedure SetPalette(var pal);
begin
 asm
  cld
  push ds
  lds si,pal
  mov dx,3c8h
  mov al,0
  out dx,al
  inc dx
  mov cx,24
  rep outsb
  mov dx,3c8h
  mov al,16
  out dx,al
  inc dx
  mov cx,24
  rep outsb
  pop ds
 end;
end;

{SetStartAddr - Sets the VGA Start Address Register.}
Procedure SetStartAdr(a:word);
Begin
 asm
  mov bx,a
  mov dx,3D4h
  mov al,0Dh
  mov ah,bl
  out dx,ax
  dec al
  mov ah,bh
  out dx,ax
 end;
end;

Procedure GFXMode; assembler;
asm
 mov ax,12h
 int 10h
end;

Procedure TXTMode; assembler;
asm
 mov ax,3
 int 10h
end;


end.