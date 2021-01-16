Program Test;

Uses GUILow,GUI;

var i : integer;
    ip : ItemPtr;
    n : word;
    x,y:word;
    mm:dialogtype;
    s:string;

Procedure UpdateHInput(IP:Itemptr);
var s:string;
begin
 str(GetStatus(MM,1),s);
 PutText(MM,10,s);
 ReDrawItem(mm,10);
end;

Procedure UpdateHSlide(IP:Itemptr);
var w:word;
    rc:word;
begin
 val(GetText(MM,10),w,rc);
 if rc <> 0 then
 begin
  PutStatus(MM,1,0);
  ReDrawItem(mm,1);
  PutText(MM,10,'0');
  ReDrawItem(mm,10);
 end else
 begin
  PutStatus(MM,1,w);
  ReDrawItem(MM,1);
 end;
end;


Procedure UpdateVInput(IP:Itemptr);
var s:string;
begin
 str(GetStatus(MM,2),s);
 PutText(MM,20,s);
 ReDrawItem(mm,20);
end;

Procedure UpdateVSlide(IP:Itemptr);
var w:word;
    rc:word;
begin
 val(GetText(MM,20),w,rc);
 if rc <> 0 then
 begin
  PutStatus(MM,2,0);
  ReDrawItem(mm,2);
  PutText(MM,20,'0');
  ReDrawItem(mm,20);
 end else
 begin
  PutStatus(MM,2,w);
  ReDrawItem(MM,2);
 end;
end;

Begin
 InitGUI;
 InitDialog(MM,2,2,318,198,'Testing',1);
 AddButton (MM,275,172,308,181,99,'OK',NOP);

 AddHSlider(MM,10,0,99,9,1,0,64,UpdateHinput);
 AddInputField(MM,100,0,3,10,'0',UpdateHSlide);

 AddVSlider(MM,0,10,9,99,2,0,64,UpdateVInput);
 AddInputField(MM,0,100,3,20,'0',UpdateVSlide);

 DrawDialog(MM);
  repeat
   n := HandleDialog(MM);
  until n = 99;
 FreeDialog(MM);
 CloseGUI;
end.