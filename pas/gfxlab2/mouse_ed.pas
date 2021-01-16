Program Mouse_ed;

Uses MCGA,Mouse,crt;

Const Bits : array[0..15] of word =
 (32768,16384,8192,4096,2048,1024,512,256,128,64,32,16,8,4,2,1);



Const DefaultMouseShape : CursorData =
 (32767,16383, 8191, 4095, 2047, 1023,  511,  255,
   1023, 4095,16383,65535,65535,65535,65535,65535,

  32768,49152,40960,36864,34816,33792,33280,33536,
  35840,45056,49152,    0,    0,    0,    0,    0);

Var MouseShape : Cursordata;

Var Quit   : boolean;
    mx,my  : integer;
    s      : string;

{-----}



Procedure DrawScreen;
var x,y:integer;
begin
 fillscreen(0);
 for y := 0 to 15 do
 begin
  for x := 0 to 15 do
  begin
   if (MouseShape[y+1]  and bits[x]) <> 0 then drawchar(x*6,y*6,'л',7);
   if (MouseShape[y+17] and bits[x]) <> 0 then drawchar(120+x*6,y*6,'л',7);
  end;
  str(mouseshape[y+1],s);
  DrawString(250,y*6,s,7);
  str(mouseshape[y+17],s);
  DrawString(290,y*6,s,7);

 end;
 Bar(0,150,160,199,7);
end;

Begin
 mouseshape := defaultmouseshape;
 mouseinit;
 if not mouseexist then
 begin
  Writeln ('Ehem.... Install a mouse driver and try again...');
  halt(1);
 end;
 setmode($13);
 DrawScreen;
 SetMouseCursor(MouseSHape,0,0);
 mouseon;
 quit := false;
 repeat

  if mouseleftinarea(0,0,95,95) then
  begin
   repeat until not mouseleft;
   mx := mousex div 6;
   my := mousey div 6;
   MouseShape[my+1] := MouseShape[my+1] xor bits[mx];
   mouseoff;
   if (mouseshape[my+1] and bits[mx]) <> 0 then DrawChar(mx*6,my*6,'л',7)
   else DrawChar(mx*6,my*6,'л',0);
   DrawString(250,my*6,'ллллл',0);
   str(mouseshape[my+1],s);
   DrawString(250,my*6,s,7);
   SetMouseCursor(MouseSHape,0,0);
   mouseon;
  end;

  if mouseleftinarea(120,0,215,95) then
  begin
   repeat until not mouseleft;
   mx := (mousex-120) div 6;
   my := mousey div 6;
   MouseShape[my+17] := MouseShape[my+17] xor bits[mx];
   mouseoff;
   if (mouseshape[my+17] and bits[mx]) <> 0 then DrawChar(120+mx*6,my*6,'л',7)
   else DrawChar(120+mx*6,my*6,'л',0);
   DrawString(290,my*6,'ллллл',0);
   str(mouseshape[my+17],s);
   DrawString(290,my*6,s,7);
   SetMouseCursor(MouseSHape,0,0);
   mouseon;
  end;

  if mouseRight then Quit := true;;
 until quit;
 mouseoff;
 setmode(3);
end.