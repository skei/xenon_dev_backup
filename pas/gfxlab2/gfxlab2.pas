Program GfxLab2;

Uses GUI,GUILow,MISc,MCGA,Images,Mouse,Dialogs;

var mm:DialogType;
    n:word;
    MainQuit : boolean;

Procedure LoadFile;
var s,t : string;
    typ:byte;
begin
 s := 'GIF     PCX     BMP     RAW     ~';
 GetFileName(50,50,'Load file','*.*',s,t,typ);
 mouseoff;
 case typ of
  0 : LoadGIF(t);
  1 : LoadPCX(t);
  2 : LoadBMP(t);
  3 : LoadRAW(t);
 end;
 mouseon;
end;

Procedure SaveFile;
var s,t : string;
    typ:byte;
begin
 s := 'RAW     PCX     ~';
 GetFileName(50,50,'Load file','*.*',s,t,typ);
 if fileexists(t) then
 begin
  if YesNoBox(100,100,'WARNING','File exist! Overwrite?') then
  begin
   mouseoff;
   case typ of
    0 : SaveRAW(t);
    1 : SavePCX(t);
   end;
  end;
 end
 else
 begin
  mouseoff;
  if pos('.PCX',t) <> 0 then SavePCX(t) else
  SaveRAW(t);
 end;
 mouseon;
end;


Procedure SetupMenus;
begin
{Main Menu}
 InitDialog(mm,130,70,237,124,'Main Menu',1);
 Addbutton(MM,0,0, 49, 9,'Load',NOP,1);
 Addbutton(MM,0,10,49,19,'Save',NOP,2);
{ AddButton(MM,0,20,49,29,'About',NOP,3);}
 AddButton(mm,0,30,49,39,'Quit',NOP,99);

 AddButton(mm,50,0,99,9,'Palette',NOP,4);
 AddButton(mm,50,10,99,19,'Grab',NOP,4);
 AddButton(mm,50,20,99,29,'tools',NOP,4);

end;

begin
 initGUI;
 SetupMenus;
 repeat
  if MouseRight then
  begin
   mainquit := false;
   DrawDialog(MM);
   n := HandleDialog(MM);
   Removedialog(mm);
   case n of
    1 : begin
         LoadFile;
        end;
    2 : begin
         SaveFile;
        end;
    99 : begin
          MainQuit := true;
         end;
   end;
  end;
 until MainQuit;
 FreeDialog(MM);
 CloseGUI;
end.