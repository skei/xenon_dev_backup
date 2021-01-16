Unit GUI;

Interface

Const UP = true;
      DOWN = False;

Type ItemType = RECORD
      active      : boolean;
      Itype       : byte;
      ID          : byte;
      x1,y1,x2,y2 : integer;
      Data        : integer;
      text        : String;
     end;

Type DialogType = RECORD
      title       : string;
      x1,y1,x2,y2 : integer;
      NumItems    : byte;
      Item        : Array[1..32] of ItemType;
      P           : pointer;
      PSize       : word;
     end;

Var LightColor  : byte;
    ShadowColor : byte;
    BarColor    : byte;
    TextColor   : byte;

Var Dx1,Dy1 : integer;
var saved : pointer;

Procedure InitGUI;
Procedure CloseGUI;
Procedure SetGUIColors;
Procedure RestoreGUIColors;
Function ClosestColor(r,g,b:byte):byte;
Procedure FindGUIColors;
{----------------------------------------------------------------------------}
{Low level}
{----------------------------------------------------------------------------}
Procedure DrawChar (x,y:integer;c:char;clr:byte);
Procedure DrawString(x,y:integer;s:string;clr:byte);
Procedure Panel(x1,y1,x2,y2:integer;up:boolean);
{----------------------------------------------------------------------------}
{Draw Items}
{----------------------------------------------------------------------------}
Procedure Draw3DString(x,y:integer;s:string;clr:byte;up:boolean);
Procedure DrawButton(x1,y1,x2,y2:integer;title:string;upp:boolean);
{----------------------------------------------------------------------------}
{Dialog}
{----------------------------------------------------------------------------}
Procedure InitDialog(var D:DialogType;title:string;x1,y1,x2,y2:integer);
Procedure DrawDialog(var D:DialogType);
Procedure RemoveDialog(D:DialogType);
Function HandleDialog(var D:Dialogtype):byte;

Procedure AddButton (var D:DialogType;x1,y1,x2,y2:integer;t:string;id:byte);
Procedure AddInputField(var D:Dialogtype;x1,y1,maxlen:integer;s:string;id:byte);
Procedure AddText(var D:DialogType;x1,y1:integer;t:string;id:byte);
{----------------------------------------------------------------------------}

Implementation
Uses MCGA,Mouse,Crt;

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

{----------------------------------------------------------------------------}

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

{----------------------------------------------------------------------------}
{ Low level item drawing }
{----------------------------------------------------------------------------}

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

{----------------------------------------------------------------------------}
{ Draw Items }
{----------------------------------------------------------------------------}

Procedure Draw3DString(x,y:integer;s:string;clr:byte;up:boolean);
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
 Draw3DString(tx1,ty1,title,ShadowColor,upp);
end;

Procedure DrawInputField(x1,y1,x2,y2:integer;S:string);
begin
 Panel(x1,y1,x2,y2,down);
 DrawString(x1+1,y1+2,s,ShadowColor);
end;

s{----------------------------------------------------------------------------}
{ Dialog }
{----------------------------------------------------------------------------}

Procedure HandleInputField(x,y:integer;var I:Itemtype);
 var cx,cy:integer;
     s:string;
     c:char;
     maxlen : byte;
     quit:boolean;
begin
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
 for i := 1 to 32 do D.Item[i].active := false;
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
 Draw3DString(tx1,d.y1+2,d.title,LightColor,up);
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

Function HandleDialog(var D:Dialogtype):byte;
var quit:boolean;
    rcode:byte;
    ix1,iy1,ix2,iy2:integer;
    s:string;
    i:integer;
begin
 HandleDialog := 0;
 repeat
  if MouseLeftInArea(D.x1,D.y1,D.x2,D.y2) then
  begin
   for i := 1 to D.NumItems do
   begin
    ix1 := d.X1+4 + D.item[i].X1;
    iy1 := d.Y1+11 + D.item[i].Y1;
    ix2 := d.X1+4 + D.item[i].X2;
    iy2 := d.Y1+11 + D.item[i].Y2;
    if D.Item[i].active then
    begin
     if MouseInArea(ix1,iy1,ix2,iy2) then
     begin
      rcode := D.item[i].ID;
      quit := true;
      case D.item[i].itype of
       1 : Begin
            mouseoff;
            DrawButton(ix1,iy1,ix2,iy2,d.item[i].text,down);
            mouseon;
            repeat until not MouseLeft;
            mouseoff;
            DrawButton(ix1,iy1,ix2,iy2,d.item[i].text,up);
            mouseon;
           end;
       2 : begin {InputField}
            HandleInputField(ix1,iy1,D.item[i]);
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
 if D.NumItems > 32 then exit else
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
 if D.NumItems > 31 then exit else
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
 if D.NumItems > 32 then exit else
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







{----------------------------------------------------------------------------}

begin
 LightColor := 0;
 ShadowColor := 1;
 BarColor := 2;
 TextColor := 3;
end.

