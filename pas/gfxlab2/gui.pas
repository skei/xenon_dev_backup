Unit GUI;

interface

Type
 ItemPtr = ^ItemType;
 ClickProc = Procedure(ip:ItemPtr);
 Itemtype = record
  x1,y1,x2,y2    : word;
  IType          : byte;
  RetCode        : word;
  Status         : word;
  Data           : word;
  Text           : string;
  Click          : ClickProc;
  NextItem       : ItemPtr;
 end;
 dialogtype = record
  x1,y1,x2,y2 : word;
  title       : string;
  flags       : byte;
  savedbg     : pointer;
  FirstItem   : ItemPtr;
 end;


{-----}

Procedure InitDialog(var d:dialogtype;x1,y1,x2,y2:word;t:string;f:byte);
Procedure FreeDialog(var d:dialogtype);

Procedure AddButton(var d:dialogtype;x1,y1,x2,y2:word;t:string;p:ClickProc;id:word);
Procedure AddText(var d:dialogtype;x1,y1:word;t:string;p:ClickProc;id:word);
ProceDure AddInputField(var D:Dialogtype;x1,y1,maxlen:integer;t:string;p:ClickProc;id:word);
Procedure AddColorBar(var d:Dialogtype;x1,y1,x2,y2:word;c:word;p:ClickProc;id:word);
Procedure AddHSlider(var d:dialogtype;x1,y1,x2,y2,value,maks:word;p:ClickProc;id:word);
Procedure AddVSlider(var d:dialogtype;x1,y1,x2,y2,value,maks:word;p:ClickProc;id:word);
ProceDure AddFrame(var D:Dialogtype;x1,y1,x2,y2:word;p:ClickProc;id:word);
Procedure AddSelector(var d:dialogtype;x1,y1,x2,y2:word;s:string;p:ClickProc;id:word);

Procedure NOP(ip:itemptr);

Procedure DrawDialog(var D:DialogType);
Procedure RemoveDialog(var d:dialogtype);
Function HandleDialog(var D:Dialogtype):word;

Function FindItem(D:Dialogtype;id:word):ItemPtr;
Procedure ReDrawItem(d:dialogtype;id:word);
Function GetData(d:dialogtype;id:word):word;
Procedure PutData(var dl:dialogtype;id:word;d:word);
Function GetStatus(d:dialogtype;id:word):word;
Procedure PutStatus(var dl:dialogtype;id:word;d:word);

Function GetText(d:dialogtype;id:word):string;
Procedure PutText(var d:dialogtype;id:word;s:string);
{-----}

implementation

Uses MCGA,GUILow,MOUSE,crt,Misc;

{-----}

Procedure NOP(ip:itemptr);
begin
end;

Procedure ExitError(s:string);
begin
 Writeln ('ERROR! ',s);
 halt(1);
end;

Procedure InitDialog(var d:dialogtype;x1,y1,x2,y2:word;t:string;f:byte);
begin
 d.x1 := x1;
 d.y1 := y1;
 d.x2 := x2;
 d.y2 := y2;
 d.title := t;
 d.flags := f;
 d.savedbg := NIL;
 d.FirstItem := NIL;
end;

{-----}

Procedure FreeDialog(var d:dialogtype);
var  p:ItemPtr;
     p2:ItemPtr;
begin
 p := d.firstitem;
 if p = NIL then exit;
 while p <> NIL do
 begin
  p2 := p;
  p := p2^.NextItem;
  freemem(p2,sizeof(itemtype));
 end;
end;

{-------------------------}

Function NewItem:ItemPtr;
var ip:ItemPtr;
begin
 If MaxAvail < sizeof(Itemtype) then ExitError('not enough memory for Item');
 GetMem(ip,sizeof(itemtype));
 NewItem := ip;
end;

Procedure AssignItem(var d:dialogtype;ip:Itemptr);
var point:ItemPtr;
begin
 point := d.firstitem;
 if point = NIL then
 begin
  d.firstitem := ip;
 end
 else
 begin
  while point^.nextitem <> NIL do point := point^.nextitem;
  point^.nextitem := ip;
 end;
end;

{-------------------------}

Procedure AddButton(var d:dialogtype;x1,y1,x2,y2:word;t:string;p:ClickProc;id:word);
var ip:ItemPtr;
begin
 ip := NewItem;
 ip^.x1 := x1;
 ip^.y1 := y1;
 ip^.x2 := x2;
 ip^.y2 := y2;
 ip^.itype := 1; {Button}
 ip^.retcode := ID;
 ip^.text := t;
 ip^.Nextitem := NIL;
 ip^.Click := p;
 assignitem(d,ip);
end;

Procedure AddText(var d:dialogtype;x1,y1:word;t:string;p:ClickProc;id:word);
var ip:ItemPtr;
begin
 ip := NewItem;
 ip^.x1 := x1;
 ip^.y1 := y1;
 ip^.x2 := x1+length(t)*6-1;
 ip^.y2 := y1+5;
 ip^.itype := 2; {Text}
 ip^.retcode := ID;
 ip^.text := t;
 ip^.Nextitem := NIL;
 ip^.Click := p;
 assignitem(d,ip);
end;

ProceDure AddInputField(var D:Dialogtype;x1,y1,maxlen:integer;t:string;p:ClickProc;id:word);
var ip:ItemPtr;
begin
 ip := NewItem;
 ip^.x1 := x1;
 ip^.y1 := y1;
 ip^.x2 := x1+maxlen*6+8;
 ip^.y2 := y1+8;
 ip^.itype := 3;
 ip^.retcode := id;
 ip^.text := t;
 ip^.data := maxlen;
 ip^.NextItem := NIL;
 ip^.Click := p;
 assignitem(d,ip);
end;

Procedure AddColorBar(var d:Dialogtype;x1,y1,x2,y2:word;c:word;p:ClickProc;id:word);
var ip:ItemPtr;
begin
 ip := NewItem;
 ip^.x1 := x1;
 ip^.y1 := y1;
 ip^.x2 := x2;
 ip^.y2 := y2;
 ip^.itype := 4;
 ip^.retcode := id;
 ip^.data := c;
 ip^.status := 0;
 ip^.NextItem := NIL;
 ip^.Click := p;
 assignitem(d,ip);
end;

Procedure AddHSlider(var d:dialogtype;x1,y1,x2,y2,value,maks:word;p:ClickProc;id:word);
var ip:ItemPtr;
begin
 ip := NewItem;
 ip^.x1 := x1;
 ip^.y1 := y1;
 ip^.x2 := x2;
 ip^.y2 := y2;
 ip^.itype := 5;
 ip^.retcode := id;
 ip^.data := maks;
 ip^.status := value;
 ip^.NextItem := NIL;
 ip^.Click := p;
 assignitem(d,ip);
end;

Procedure AddVSlider(var d:dialogtype;x1,y1,x2,y2,value,maks:word;p:ClickProc;id:word);
var ip:ItemPtr;
begin
 ip := NewItem;
 ip^.x1 := x1;
 ip^.y1 := y1;
 ip^.x2 := x2;
 ip^.y2 := y2;
 ip^.itype := 6;
 ip^.retcode := id;
 ip^.data := maks;
 ip^.status := value;
 ip^.NextItem := NIL;
 ip^.Click := p;
 assignitem(d,ip);
end;

ProceDure AddFrame(var D:Dialogtype;x1,y1,x2,y2:word;p:ClickProc;id:word);
var ip:ItemPtr;
begin
 ip := NewItem;
 ip^.x1 := x1;
 ip^.y1 := y1;
 ip^.x2 := x2;
 ip^.y2 := y2;
 ip^.itype := 7;
 ip^.retcode := id;
 ip^.NextItem := NIL;
 ip^.Click := p;
 assignitem(d,ip);
end;

Procedure AddSelector(var d:dialogtype;x1,y1,x2,y2:word;s:string;p:ClickProc;id:word);
var ip:ItemPtr;
begin
 ip := NewItem;
 ip^.x1 := x1;
 ip^.y1 := y1;
 ip^.x2 := x2;
 ip^.y2 := y2;
 ip^.itype := 8;
 ip^.retcode := ID;
 ip^.text := s;
 ip^.data := 0;
 ip^.Nextitem := NIL;
 ip^.Click := p;
 assignitem(d,ip);
end;

{-------------------------}

CONST Chars = ' !"#$%&'+#39+'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\]^_Ûþ';
Procedure HandleInputField(x,y:integer;var ip:ItemPtr);
 var cx,cy:integer;
     s:string;
     c:char;
     maxlen : byte;
     quit:boolean;
begin
 FlushKeyboard;
 c := #0;
 cx := x+2;
 cy := y+2;
 s := ip^.text;
 cx := cx + length(s)*6;
 maxlen := ip^.data;
 mouseoff;
 DrawString(cx,cy,'Û',DarkColor);
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
                DrawChar(cx,cy,'Û',DarkColor);
                mouseon;
               end;
              end;
   else       begin
               if pos(upcase(c),Chars)>0 then
               begin
                if length(s)<maxlen then
                begin
                 mouseoff;
                 s:=s+upcase(c);
                 DrawChar(cx,cy,'Û',BarColor);
                 DrawChar(cx,cy,upcase(c),DarkColor);
                 cx := cx + 6;
                 DrawChar(cx,cy,'Û',DarkColor);
                 mouseon;
                end;
               end;
              end;
  end;
  c := #0;
 until Quit;
 mouseoff;
 DrawChar(cx,cy,'Û',BarColor);
 ip^.text := s;
 mouseon;
end;

Procedure HandleHSlide(x1,y1,x2,y2:word;var ip:itemptr);
var xpos:word;
    oldx:word;
begin
 mouseoff;
 Bar(x1,y1,x2,y2,BarColor);
 xpos := mousex;
 if xpos < x1 then xpos := x1;
 if xpos > x2 then xpos := x2;
 ip^.status := (xpos-x1) * ip^.data DIV (x2-x1+1);
 VertLine(xpos,y1,y2,DarkColor);
 oldx := xpos;
 ip^.Click(ip);
 mouseon;
 repeat
  xpos := mousex;
  if xpos < x1 then xpos := x1;
  if xpos > x2 then xpos := x2;
  if xpos <> oldx then
  begin
   mouseoff;
   VertLine(oldx,y1,y2,BarColor);
   ip^.status := (xpos-x1) * ip^.data DIV (x2-x1+1);
   VertLine(xpos,y1,y2,DarkColor);
   oldx := xpos;
   ip^.Click(ip);
   mouseon;
  end;
 until not MouseLeft
end;

Procedure HandleVSlide(x1,y1,x2,y2:word;var ip:itemptr);
var ypos:word;
    oldy:word;
begin
 mouseoff;
 Bar(x1,y1,x2,y2,BarColor);
 ypos := mousey;
 if ypos < y1 then ypos := y1;
 if ypos > y2 then ypos := y2;
 ip^.status := (ypos-y1) * ip^.data DIV (y2-y1+1);
 HorizLine(x1,ypos,x2,DarkColor);
 oldy := ypos;
 ip^.Click(ip);
 mouseon;
 repeat
  ypos := mousey;
  if ypos < y1 then ypos := y1;
  if ypos > y2 then ypos := y2;
  if ypos <> oldy then
  begin
   mouseoff;
   HorizLine(x1,oldy,x2,BarColor);
   ip^.status := (ypos-y1) * ip^.data DIV (y2-y1+1);
   HorizLine(x1,ypos,x2,DarkColor);
   oldy := ypos;
   ip^.Click(ip);
   mouseon;
  end;
 until not MouseLeft
end;

Procedure HandleSelector(x1,y1,x2,y2:word;var ip:ItemPtr);
var s:string;
begin
 mouseoff;
 s := copy(ip^.text,(ip^.data*8)+1,8);
 while s[length(s)] = ' ' do delete(s,length(s),1);
 DrawButton(x1,y1,x2,y2,s,down);
 mouseon;
 repeat until not MouseLeft;
 inc(ip^.data);
 if ip^.text[ip^.data*8+1] = '~' then ip^.data := 0;
 mouseoff;
 s := copy(ip^.text,(ip^.data*8)+1,8);
 while s[length(s)] = ' ' do delete(s,length(s),1);
 DrawButton(x1,y1,x2,y2,s,up);
 mouseon;
end;

{-------------------------}

Procedure DrawItem(x1,y1,x2,y2:word;p:ItemPtr);
var ss:string;
begin
 case p^.itype of
  1 : DrawButton(x1,y1,x2,y2,p^.text,up);
  2 : Draw3DString(x1,y1,p^.text,up);
  3 : DrawInputField(x1,y1,x2,y2,p^.text);
  4 : if p^.status = 0 then DrawColorBar(x1,y1,x2,y2,p^.data,up)
       else DrawColorBar(x1,y1,x2,y2,p^.data,down);
  5 : DrawHSlide(x1,y1,x2,y2,p^.status,p^.data);
  6 : DrawVSlide(x1,y1,x2,y2,p^.status,p^.data);
  7 : Panel(x1,y1,x2,y2,down);
  8 : begin
       ss := copy(p^.text,(p^.data*8)+1,8);
       while ss[length(ss)] = ' ' do delete(ss,length(ss),1);
       DrawButton(x1,y1,x2,y2,ss,up);
      end;
 end;
end;


Procedure DrawDialog(var D:DialogType);
var i,ix1,iy1,ix2,iy2 : integer;
    tx1 : integer;
    dx1,dy1 : word;
    p:ItemPtr;
begin
 Mouseoff;
 getmem(d.savedbg,(d.y2-d.y1+1)*(d.x2-d.x1+1));
 GetImage(d.savedbg,d.x1,d.y1,d.x2-d.x1+1,d.y2-d.y1+1);
 FindGUIColors;
 Panel(d.x1,d.y1,d.x2,d.y2,up);
 Panel(d.x1+2,d.y1+9,d.x2-2,d.y2-2,down);
 Tx1 := (d.x1+(d.x2-d.x1) div 2) - (length(d.title)*3) + 1;
 Draw3DString(tx1,d.y1+2,d.title,up);
 Dx1 := d.x1 + 4;
 Dy1 := d.y1 + 11;
 p := d.firstitem;
 while p <> NIL do
 begin
  ix1 := dx1+p^.x1;
  iy1 := dy1+p^.y1;
  ix2 := dx1+p^.x2;
  iy2 := dy1+p^.y2;
  DrawItem(ix1,iy1,ix2,iy2,p);
  p := p^.nextitem;
 end;
 mouseon;
end;

Procedure RemoveDialog(var d:dialogtype);
begin
 mouseoff;
 PutImage(d.savedbg,d.x1,d.y1,d.x2-d.x1+1,d.y2-d.y1+1);
 freemem(d.savedbg,(d.y2-d.y1+1)*(d.x2-d.x1+1));
 mouseon;
end;

Function HandleDialog(var D:Dialogtype):word;
var quit:boolean;
    rcode:word;
    ix1,iy1,ix2,iy2:integer;
    p:ItemPtr;
begin
 rcode := 0;
 quit := false;
 repeat
  if MouseLeftInArea(D.x1,D.y1,D.x2,D.y2) then
  begin
   p := d.firstitem;
   while p <> NIL do
   begin
    ix1 := d.X1+4 + p^.X1;
    iy1 := d.Y1+11 + p^.Y1;
    ix2 := d.X1+4 + p^.X2;
    iy2 := d.Y1+11 + p^.Y2;
    if MouseInArea(ix1,iy1,ix2,iy2) then
    begin
     rcode := p^.retcode;
     quit := true;
     case p^.itype of
      1 : Begin {Button}
           mouseoff;
           DrawButton(ix1,iy1,ix2,iy2,p^.text,down);
           mouseon;
           repeat until not MouseLeft;
           mouseoff;
           DrawButton(ix1,iy1,ix2,iy2,p^.text,up);
           mouseon;
          end;
      2 : Begin {Text}
           repeat until not MouseLeft;
          end;
      3 : begin {InputField}
           repeat until not MouseLeft;
           HandleInputField(ix1,iy1,p);
          end;
      4 : begin {colorbar/button}
           mouseoff;
           if p^.status = 0 then
           begin
            DrawColorBar(ix1,iy1,ix2,iy2,p^.data,down);
            p^.status := 1;
           end
           else
           begin
            DrawColorBar(ix1,iy1,ix2,iy2,p^.data,up);
            p^.status := 0;
           end;
           mouseon;
           repeat until not MouseLeft;
          end;
      5 : begin {HSlide}
           HandleHSlide(ix1+2,iy1+2,ix2-2,iy2-2,p);
          end;
      6 : begin {VSlide}
           HandleVSlide(ix1+2,iy1+2,ix2-2,iy2-2,p);
          end;
      7 : begin {Panel}
           repeat until not MouseLeft;
          end;
      8 : HandleSelector(ix1,iy1,ix2,iy2,p);
     end; {case}
     p^.Click(p);
    end; {if mouseinarea.item}
    p := p^.nextitem;
   end; {while}
  end; {if mouseinarea.dialog}
 until quit = true;
 HandleDialog := rcode;
end;

{-----}

Function FindItem(D:Dialogtype;id:word):ItemPtr;
var p,p2:ItemPtr;
    n:word;
    quit:boolean;
begin
 p := d.firstitem;
 if p = NIL then EXIT;
 quit := false;
 repeat
  if p^.retcode = id then p2 := p;
  if p^.nextitem = NIL then quit := true else p := p^.nextitem;
 until quit;
 FindItem := p2;
end;

Procedure ReDrawItem(d:dialogtype;id:word);
var p:ItemPtr;
    dx1,dy1,ix1,iy1,ix2,iy2:word;
begin
 p := Finditem(d,id);
 Dx1 := d.x1 + 4;
 Dy1 := d.y1 + 11;
 ix1 := dx1+p^.x1;
 iy1 := dy1+p^.y1;
 ix2 := dx1+p^.x2;
 iy2 := dy1+p^.y2;
 mouseoff;
 Bar(ix1,iy1,ix2,iy2,BarColor);
 DrawItem(ix1,iy1,ix2,iy2,p);
 mouseon;
end;

Function GetData(d:dialogtype;id:word):word;
var p:ItemPtr;
begin
 p := FindItem(d,id);
 GetData := p^.data;
end;

Procedure PutData(var dl:dialogtype;id:word;d:word);
var p:ItemPtr;
begin
 p := FindItem(dl,id);
 p^.data := d;
end;

Function GetStatus(d:dialogtype;id:word):word;
var p:ItemPtr;
begin
 p := FindItem(d,id);
 GetStatus := p^.status;
end;

Procedure PutStatus(var dl:dialogtype;id:word;d:word);
var p:ItemPtr;
begin
 p := FindItem(dl,id);
 p^.status := d;
end;


Function GetText(d:dialogtype;id:word):string;
var p:ItemPtr;
begin
 p := FindItem(d,id);
 GetText := p^.text;
end;

Procedure PutText(var d:dialogtype;id:word;s:string);
var p:ItemPtr;
begin
 p := FindItem(d,id);
 p^.text := s;
end;


begin
end.