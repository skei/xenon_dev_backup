Program PalEdit;

Uses GUILow,GUI;

var i : integer;
    ip : ItemPtr;
    n : word;
    x,y:word;
    mm:dialogtype;
    s:string;

Begin
 InitGUI;
 InitDialog(MM,2,2,318,198,'Testing',1);
 for y := 0 to 15 do
 begin
  for x := 0 to 15 do AddColorBar(MM,x*8,y*8,x*8+7,y*8+7,y*16+x,y*16+x,NOP);
 end;
 AddColorBar(MM,150,0,169,19,1000,0,NOP);
 Addbutton(MM,0,130,40,139,500,'OK',NOP);
 AddButton(MM,0,140,40,149,501,'Test',NOP);
 AddText(MM,0,160,502,'Dette er tekst',NOP);
 AddInputField(MM,0,170,20,503,'write here!',NOP);
 DrawDialog(MM);
  repeat
   n := HandleDialog(MM);
   if n < 256 then
   begin
    PutData(mm,1000,n);
    ReDrawItem(mm,1000);
   end;
  until n = 500;
 s := GetText(MM,503);
 FreeDialog(MM);
 CloseGUI;
 writeln ('Input field = ',s);
end.