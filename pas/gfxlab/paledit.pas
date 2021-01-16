Unit PalEdit;

interface
Procedure EditPalette;
implementation

Uses GUI, GUILow,Misc,MCGA,Images,Mouse,Dialogs;

var quit:boolean;
    pe:dialogtype;
    n,x,y:word;
    selectedcolor : byte;
    r,g,b:byte;

Procedure UpdateRed(ip:itemptr);
var value : word;
    s:string;
begin
 value := ip^.status;
 str(value,s);
 finditem(pe,401)^.text := s;
 redrawitem(pe,401);
 GetPal(selectedcolor,r,g,b);
 SetPal(selectedcolor,value,g,b);
end;

Procedure UpdateGreen(ip:itemptr);
var value : word;
    s:string;
begin
 value := ip^.status;
 str(value,s);
 finditem(pe,402)^.text := s;
 redrawitem(pe,402);
 GetPal(selectedcolor,r,g,b);
 SetPal(selectedcolor,r,value,b);
end;

Procedure UpdateBlue(ip:itemptr);
var value : word;
    s:string;
begin
 value := ip^.status;
 str(value,s);
 finditem(pe,403)^.text := s;
 redrawitem(pe,403);
 GetPal(selectedcolor,r,g,b);
 SetPal(selectedcolor,r,g,value);

end;


Procedure Init;
begin
 InitDialog(pe,10,10,310,152,'Edit Palette',1);
 for y := 0 to 15 do for x := 0 to 15 do AddColorBar(pe,x*8,y*8,x*8+7,y*8+7,y*16+x,NOP,y*16+x);
 selectedcolor := 0;
 finditem(pe,0)^.status := 1;
 AddColorBar(pe,140,0,159,19,0,NOP,300);
 AddButton(pe,140,30,179,39,'OK',NOP,999);

 AddHSlider(pe,140,50,205,58,0,65,UpdateRed,301);
 AddInputField(pe,210,50,2,'0',NOP,401);

 AddHSlider(pe,140,60,205,68,0,65,UpdateGreen,302);
 AddInputField(pe,210,60,2,'0',NOP,402);

 AddHSlider(pe,140,70,205,78,0,65,UpdateBlue,303);
 AddInputField(pe,210,70,2,'0',NOP,403);
end;

Procedure SelectColor(clr:byte);
var ip:itemptr;
    s:string;
begin
 finditem(pe,selectedcolor)^.status := 0;
 redrawitem(pe,selectedcolor);
 selectedcolor := clr;
 finditem(pe,selectedcolor)^.status := 1;
 redrawitem(pe,selectedcolor);
 FindItem(pe,300)^.data := clr;
 redrawitem(pe,300);
 GetPal(clr,r,g,b);

 PutStatus(pe,301,r);
 Redrawitem(pe,301);
 str(r,s);
 PutText(pe,401,s);
 redrawitem(pe,401);

 PutStatus(pe,302,g);
 Redrawitem(pe,302);
 str(g,s);
 PutText(pe,402,s);
 Redrawitem(pe,402);

 PutStatus(pe,303,b);
 Redrawitem(pe,303);
 str(b,s);
 PutText(pe,403,s);
 Redrawitem(pe,403);


end;

Procedure EditPalette;
begin
 Init;
 DrawDialog(pe);
 quit := false;
 repeat
  n := HandleDialog(pe);
  case n of
   0..255 : SelectColor(n);
   999 : begin
          Quit := true;
         end;
  end;
 until Quit;
 Removedialog(pe);
 freedialog(pe);
end;

begin
end.
