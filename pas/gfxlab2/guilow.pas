Unit GUILow;

interface

Const up   = true;
      down = false;
      yes  = true;
      no   = false;

var BarColor : byte;
    LightColor : byte;
    DarkColor : byte;
    TextColor : byte;

Procedure FindGUIColors;
Procedure InitGUI;
Procedure CloseGUI;

Procedure Panel (x1,y1,x2,y2:word;up:boolean);
Procedure Draw3DString (x1,y1:word;s:String;up:boolean);
Procedure DrawButton(x1,y1,x2,y2:integer;title:string;up:boolean);
Procedure DrawInputField(x1,y1,x2,y2:integer;S:string);
Procedure DrawColorBar(x1,y1,x2,y2:integer;c:byte;up:boolean);
Procedure DrawHSlide(x1,y1,x2,y2,value,maks:word);
Procedure DrawVSlide(x1,y1,x2,y2,value,maks:word);

implementation

Uses MCGA, Mouse, Misc;

Var OldMode : byte;

{-----}

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
  GetPal(i,r1,g1,b1);
  t := sqrRoot((r-r1)*(r-r1) + (g-g1)*(g-g1) + (b-b1)*(b-b1));
  if t < tc then
  begin
   closest := i;
   tc := t;
  end;
 end;
 ClosestColor := closest;
end;

{-----}

Procedure FindGUIColors;
begin
 BarColor := ClosestColor(40,40,40);
 LightCOlor := ClosestColor(60,60,60);
 DarkColor := ClosestColor(20,20,20);
 TextColor := ClosestColor (63,63,0);
end;

{-----}

Procedure InitGUI;
begin
 MouseInit;
 if not mouseexist then
 begin
  Writeln('Ehem.... Install a Mouse driver and try again...');
  halt(1);
 end;
 OldMode := getmode;
 SetMode($13);
 FindGUIColors;
{ FillScreen(BarColor);}
 mouseon;
 SetMouseCursor(ArrowMask,0,0);
end;

{-----}

Procedure CloseGUI;
begin
 mouseoff;
 SetMode(OldMode);
end;

{ ITEMS }

Procedure Panel (x1,y1,x2,y2:word;up:boolean);
var cl,cd : byte;
begin
 MouseOff;
 if up then
 begin
  Bar (x1,y1,x2,y2,BarColor);
  HorizLine(x1,y1,x2-1,LightColor);
  VertLine(x1,y1,y2-1,LightColor);
  HorizLine(x1+1,y2,x2,DarkColor);
  VertLIne (x2,y1+1,y2,DarkColor);
 end
 else
 begin
  Bar (x1,y1,x2,y2,BarColor);
  HorizLine(x1,y1,x2-1,DarkColor);
  VertLine(x1,y1,y2-1,DarkColor);
  HorizLine(x1+1,y2,x2,LightColor);
  VertLIne (x2,y1+1,y2,LightColor);
 end;
 MouseOn;
end;

Procedure Draw3DString (x1,y1:word;s:String;up:boolean);
var cl,cd : byte;
begin
 if up then
 begin
  DrawString(x1+1,y1+1,s,DarkColor);
  DrawString(x1,y1,s,LightColor);
 end
 else
 begin
  DrawString(x1+1,y1+1,s,LightColor);
  DrawString(x1,y1,s,DarkColor);
 end;
end;

Procedure DrawButton(x1,y1,x2,y2:integer;title:string;up:boolean);
var tx1,ty1:integer;
begin
 Panel(x1,y1,x2,y2,up);
 Tx1 := (x1+(x2-x1) div 2) - (length(title)*3) +1 ;
 Ty1 := (y1+(y2-y1) div 2) - 2;
 Draw3DString(tx1,ty1,title,up);
end;

Procedure DrawInputField(x1,y1,x2,y2:integer;S:string);
begin
 Panel(x1,y1,x2,y2,down);
 DrawString(x1+2,y1+2,s,DarkColor);
end;

Procedure DrawColorBar(x1,y1,x2,y2:integer;c:byte;up:boolean);
begin
 MouseOff;
 if up then
 begin
  Bar (x1,y1,x2,y2,c);
  HorizLine(x1,y1,x2-1,LightColor);
  VertLine(x1,y1,y2-1,LightColor);
  HorizLine(x1+1,y2,x2,DarkColor);
  VertLIne (x2,y1+1,y2,DarkColor);
 end
 else
 begin
  Bar (x1,y1,x2,y2,c);
  HorizLine(x1,y1,x2-1,DarkColor);
  VertLine(x1,y1,y2-1,DarkColor);
  HorizLine(x1+1,y2,x2,LightColor);
  VertLIne (x2,y1+1,y2,LightColor);
 end;
 MouseOn;
end;

Procedure DrawHSlide(x1,y1,x2,y2,value,maks:word);
var lengde,p : word;
begin
 Panel (x1,y1,x2,y2,up);
 lengde := x2-x1-3;
 p := (x1+2) + (value * lengde div maks);
 VertLine(p,y1+2,y2-2,DarkColor);
end;

Procedure DrawVSlide(x1,y1,x2,y2,value,maks:word);
var lengde,p : word;
begin
 Panel (x1,y1,x2,y2,up);
 lengde := y2-y1-3;
 p := (y1+2) + (value * lengde div maks);
 HorizLine(x1+2,p,x2-2,DarkColor);
end;

Begin
end.