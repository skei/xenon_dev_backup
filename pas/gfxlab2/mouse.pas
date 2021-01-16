Unit Mouse;

interface

{16 words cursor data... 16 words screen mask data}
Type CursorData  = Array [1..32] of Word;

Const ArrowMask : CursorData =
 (32767,16383, 8191, 4095, 2047, 1023,  511,  255,
   1023, 4095,16383,65535,65535,65535,65535,65535,

  32768,49152,40960,36864,34816,33792,33280,33536,
  35840,45056,49152,    0,    0,    0,    0,    0);

Const HourGlassMask : CursorData =
 ($0000,$0000,$0000,$c003,$e007,$f00f,$F81F,$fc3f,
  $fc3F,$F81F,$F00F,$e007,$c003,$0000,$0000,$0000,

  $0000,$7ffe,$0000,$1ff8,$0ff0,$0000,$0000,$0000,
  $0180,$0340,$07e0,$0e78,$1818,$0000,$7ffe,$0000);


Var MouseExist : boolean;
    mousecursoron:boolean;

procedure mouseinit;
procedure mouseon;
procedure mouseoff;
function mousex: word;
function mousey: word;
function mouseleft: boolean;
function mousemiddle: boolean;
function mouseright: boolean;
procedure setmousexy(newx, newy: word);
procedure limitmouse(lox, loy, hix, hiy: word);
Function MouseInArea (x1,y1,x2,y2:integer):boolean;
Function MouseLeftInArea (x1,y1,x2,y2:integer):boolean;
Function MouseRightInArea (x1,y1,x2,y2:integer):boolean;
Procedure SetMouseCursor(CursorMask : CursorData;x,y:integer);

implementation

uses dos;
var r:registers;

{
Mouse functions....
XShift & Yshift = Variables to automatically convert mouse-units to pixels
MouseUnit normally returns Xcoordinates = 0..639, y=0..199,
MCGA -> XShift=1,YShift=0
Must manually do an MOUSEINIT!
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

Procedure SetMouseCursor(CursorMask : CursorData;x,y:integer);
Begin
 r.AX := $0009;
 r.BX := x;
 r.CX := y;
 r.ES := Seg(CursorMask);
 r.DX := Ofs(CursorMask);
 Intr($33,r);
End;


begin
end.