Program GfxLab2;

Uses GUI,GUILow,MISc,MCGA,Images,Mouse,Dialogs,PalEdit;

var mm:DialogType;
    tm:DialogType;
    n:word;
    MainQuit : boolean;

{----------}

Procedure DoClip(s:string);
var f:file;
    ox,oy,gx,gy,oox,ooy:word;
    doclipquit : boolean;
    i,j:word;
    buffer:array[0..319] of byte;
    sx,sy:string;
begin
 mouseoff;
 ox := mousex;
 ox := mousey;
 XorLine(ox,0,ox,199);
 XorLine(0,oy,319,oy);
 doclipquit := false;
 repeat
  gx := mousex;
  gy := mousey;
  if (gx <> ox) or (gy <> oy) then
  begin
   XorLine(ox,0,ox,199);
   XorLine(0,oy,319,oy);
   ox := gx;
   oy := gy;
   XorLine(ox,0,ox,199);
   XorLine(0,oy,319,oy);
  end;
 until mouseleft;
 XorLine(ox,0,ox,199);
 XorLine(0,oy,319,oy);
 oox := ox; ooy := oy;
 XorBar(ox,oy,oox,ooy);
 repeat
  gx := mousex;
  gy := mousey;
  if (gx <> oox) or (gy <> ooy) then
  begin
   XorBar(ox,oy,oox,ooy);
   oox := gx; ooy := gy;
   XorBar(ox,oy,oox,ooy);
  end;
 until not mouseleft;
 XorBar(ox,oy,oox,ooy);
{Save selected}
 assign(f,s);
 rewrite(f,1);
 for i := oy to ooy do
 begin
  for j := 0 to oox-ox do
  begin
   buffer[j] := mem[$a000:i*320+(j+ox)];
  end;
  blockwrite(f,buffer,oox-ox+1);
 end;
 close(f);
 str(oox-ox+1,sx);
 str(ooy-oy+1,sy);
 mouseon;
 MessageBox(50,50,sx+','+sy,'Size');
end;

Procedure Clip;
var clipquit:boolean;
    cn:word;
    ss:string;
begin
 InitDialog(tm,50,50,138,110,'Clip',1);
 AddText(tm,0,5,'filename',NOP,0);
 AddInputField(tm,0,15,12,'clip.raw',NOP,1);
 Addbutton(tm,0,30,39,39,'OK',NOP,2);
 Addbutton(tm,40,30,79,39,'Cancel',NOP,99);
 DrawDialog(tM);
 clipquit := false;
 repeat
  cn := HandleDialog(tM);
  case cn of
   2 : begin
        removedialog(tm);
        ss := finditem(tm,1)^.text;
        if fileexists(ss) then
        begin
         if YesNoBox(100,100,'WARNING','File exist! Overwrite?') then DoClip(ss);
        end
        else DoCLip(ss);
        clipquit := true;
        repeat until not mouseleft;
       end;
   99 : begin
         removedialog(tm);
         clipquit := true;
        end;
  end;
 until clipquit;
 freedialog(tm);
end;


Procedure DoGrab(fname:string;sx,sy:word);
var gx,gy,ox,oy : word;
    dograbquit:boolean;
    i,j:word;
    f:file;
    buffer : array[0..255] of byte;
begin
 assign(f,fname);
 rewrite(f,1);
 mouseoff;
 ox := mousex;
 ox := mousey;
 XorBar(ox,oy,ox+sx-1,oy+sy-1);
 dograbquit := false;
 repeat
  gx := mousex;
  gy := mousey;
  if (gx <> ox) or (gy <> oy) then
  begin
   XorBar(ox,oy,ox+sx-1,oy+sy-1);
   ox := gx;
   oy := gy;
   XorBar(ox,oy,ox+sx-1,oy+sy-1);
  end;
  if mouseleft then
  begin
   repeat until not mouseleft;
   XorBar(ox,oy,ox+sx-1,oy+sy-1);
   for i := oy to oy+sy-1 do
   begin
    for j := 0 to sx-1 do
    begin
     buffer[j] := mem[$a000:i*320+(j+ox)];
    end;
    blockwrite(f,buffer,sx);
   end;
   XorBar(ox,oy,ox+sx-1,oy+sy-1);
  end;
 until mouseright;
 XorBar(ox,oy,ox+sx-1,oy+sy-1);
 close(f);
 mouseon;
end;

{----------}

Procedure Grab;
var gn:word;
    grabquit:boolean;
    xsize,ysize,c : word;
    ipx,ipy:itemptr;
    ss:string;
begin
 InitDialog(tm,50,50,138,130,'Grab',1);
 AddText(tm,0,0,'X-size',NOP,0);
 AddText(tm,44,0,'Y-size',NOP,0);
 AddInputField(tm,4,10,3,'16',NOP,1);
 AddInputField(tm,48,10,3,'16',NOP,2);
 AddText(tm,0,25,'filename',NOP,0);
 AddInputField(tm,0,35,12,'grab.raw',NOP,4);
 Addbutton(tm,0,50,39,59,'OK',NOP,3);
 Addbutton(tm,40,50,79,59,'Cancel',NOP,99);
 DrawDialog(tM);
 grabquit := false;
 repeat
  gn := HandleDialog(tM);
  case gn of
   3 : begin
        removedialog(tm);
        val(Finditem(tm,1)^.text,xsize,c);
        val(Finditem(tm,2)^.text,ysize,c);
        ss := finditem(tm,4)^.text;
        if fileexists(ss) then
        begin
         if YesNoBox(100,100,'WARNING','File exist! Overwrite?') then
         DoGrab(ss,xsize,ysize);
        end
        else DoGrab(ss,xsize,ysize);
        grabquit := true;
        repeat until not mouseright;
       end;
   4 : begin
       end;
   99 : begin
         removedialog(tm);
         grabquit := true;
        end;
  end;
 until grabquit;
 freedialog(tm);
end;

{----------}

Procedure LoadFile;
var s,t : string;
begin
 GetFileName(50,50,'Load file','*.*',t);
 if t <> '' then
 begin
  mouseoff;
  if pos('.GIF',t)>0 then LoadGIF(t) else
  if pos('.PCX',t)>0 then LoadPCX(t) else
  if pos('.BMP',t)>0 then LoadBMP(t) else
  LoadRAW(t);
  mouseon;
 end;
end;

{----------}

Procedure SaveFile;
var s,t : string;
begin
 GetFileName(50,50,'Load file','*.*',t);
 if t <> '' then
 begin
  if fileexists(t) then
  begin
   if YesNoBox(100,100,'WARNING','File exist! Overwrite?') then
   begin
    mouseoff;
    if pos('.PCX',t)>0 then SaveRAW(t) else SavePCX(t);
   end;
  end
  else
  begin
   mouseoff;
   if pos('.PCX',t) <> 0 then SavePCX(t) else SaveRAW(t);
  end;
  mouseon;
 end;
end;

{----------}

Procedure SetupMenus;
begin
{Main Menu}
 InitDialog(mm,130,70,237,124,'Main Menu',1);
 Addbutton(MM,0,0, 49, 9,'Load',NOP,1);
 Addbutton(MM,0,10,49,19,'Save',NOP,2);
 AddButton(MM,0,20,49,29,'About',NOP,50);
 AddButton(mm,0,30,49,39,'Quit',NOP,99);
 AddButton(mm,50,0,99,9,'Grab',NOP,4);
 AddButton(mm,50,10,99,19,'Clip',NOP,6);
 AddButton(mm,50,20,99,29,'Palette',NOP,3);
 AddButton(mm,50,30,99,39,'tools',NOP,5);
end;

Procedure DoAbout;
begin
 MessageBox(50,50,'GfxLab v0.2 alpha - (c)Axon 1995','About');
end;

{----------}
{   MAIN   }
{----------}

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
    1 : LoadFile;
    2 : SaveFile;
    3 : EditPalette; {MessageBox(50,50,'Sorry, not implemented yet','beta');}
    4 : Grab;
    5 : MessageBox(50,50,'Sorry, not implemented yet','beta');
    6 : Clip;
    50 : DoAbout;
    99 : MainQuit := true;
   end;
  end;
 until MainQuit;
 FreeDialog(MM);
 CloseGUI;
end.