Unit Images;

interface

Procedure LoadBMP (Name:String);
Procedure LoadPCX (PCXFile:string);
Procedure SavePCX (FN : String);
Procedure LoadGif (N: String);
PROCEDURE LoadPalette(DateiName:String);
PROCEDURE SavePalette(DateiName:String);
PROCEDURE LoadRaw(DateiName:String);
PROCEDURE SaveRaw(DateiName:String);

implementation

Uses MCGA,Misc;

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

var header1: pcxheader_rec;
    width, depth: word;
    bytes: word;
    f: file;
    c: byte;

{GFX file loading & Saving}

Procedure LoadBMP(Name:String);
type Virtual = Array [1..64000] of byte;
VAR PicBuf: ^Virtual;
    Data:File;
    RGB:ARRAY[0..255,1..4] OF Byte;
    header1:Array[1..54] of Byte;
    aAddr :word;
    I:Byte;
BEGIN
 if not fileexists(name) then exit;
 GetMem (PicBuf,64000);
 Assign(Data,Name); Reset(Data,1);
 BlockRead(Data,header1,54);                 { read and ignore :) }
 BlockRead(Data,RGB,1024);                  { pal info }
 FOR I:=0 TO 255 DO SetPal(I,RGB[I,3] div 4,RGB[I,2] div 4,RGB[I,1] div 4);
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
 blockread (f, header1, sizeof (header1));
 if (header1.manufacturer=10) and (header1.version=5)
 and (header1.bits_per_pixel=8) and (header1.colour_planes=1) then
 begin
  seek (f, filesize (f)-769);
  blockread (f, c, 1);
  if (c=12) then
  begin
   blockread (f, palette, 768);
   seek (f, 128);
   width:=header1.xmax-header1.xmin+1;
   depth:=header1.ymax-header1.ymin+1;
   bytes:=header1.bytes_per_line;
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
  GetPal (L,R,G,B);
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
 FOR I:=0 TO 255 DO SetPal(I,RGB[I,1],RGB[I,2],RGB[I,3]);
END;

PROCEDURE SavePalette(DateiName:String);
VAR Datei:File;
    RGB:ARRAY[0..255,1..3] OF Byte;
    I:Byte;
BEGIN
 Assign(Datei,DateiName);
 Rewrite(Datei,1);
 FOR I:=0 TO 255 DO GetPal(I,RGB[I,1],RGB[I,2],RGB[I,3]);
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



begin
end.
