Unit Dialogs;

interface

Procedure GetFilename (x1,y1:word;title,mask,fmtstr:string;var fname:string;var ftype:byte);
Procedure MessageBox(x1,y1:word;s:string);
Function YesNoBox(x1,y1:word;title,s:string):boolean;

implementation

Uses GUI,MISC,GuiLow,Mouse,DOS,MCGA;

var files : array[1..255] of string[12];
    d:Dialogtype;
    numfiles:byte;
    num:byte;
    drive : byte;
    curpath : string;

Procedure InitList(mask:string);
var DirInfo: SearchRec;
    ip:ItemPtr;
begin
 fillchar(files,sizeof(files),#0);
 numfiles := 0;
{finner alle DIR's}
 FindFirst('*.*',Directory,DirInfo);
 while DosError = 0 do
 begin
  if dirinfo.attr = Directory then
  begin
   inc(numfiles);
   files[numfiles] := '<'+DirInfo.Name+'>';
  end;
  FindNext(DirInfo);
 end;
{Alle filer}
 FindFirst(mask,Archive,DirInfo);
 while DosError = 0 do
 begin
  inc(numfiles);
  files[numfiles] := DirInfo.Name;
  FindNext(DirInfo);
 end;
 ip:=finditem(d,3);
 ip^.data := NumFiles;
end;

Procedure DrawList(p:ItemPtr);
var i:byte;
    ix1,iy1,ix2,iy2:word;
    ip:itemptr;
begin
 num := GetStatus(d,3);
 inc(num);
 ip:=Finditem(d,2);
 ix1 := d.x1 + 4  + ip^.x1;
 iy1 := d.y1 + 11 + ip^.y1;
 ix2 := d.x1 + 4  + ip^.x2;
 iy2 := d.y1 + 11 + ip^.y2;
 mouseoff;
 bar(ix1+1,iy1+1,ix2-1,iy2-1,BarColor);
 for i := 0 to 8 do
 begin
  if num+i <= numfiles then DrawString(ix1+2,iy1+2+(i*6),files[num+i],DarkColor);
 end;
 mouseon;
end;

Procedure NewMask(p:Itemptr);
var ip:itemptr;
begin
 initlist(p^.text);
 ReDrawItem(d,2);
 DrawList(p);
 ip := FindItem(d,3);
 ip^.status := 0;
 RedrawItem(d,3);
 DrawList(ip);
end;

Procedure SelectFile(p:ItemPtr);
var iy1 : word;
    n : word;
    s:string;
begin
 iy1 := d.y1 + 11 + p^.y1;
 n := mousey-iy1-2;
 s := files[(n div 6)+num];
 if s[1] = '<' then
 begin {New dir}
  delete(s,1,1);
  delete(s,pos('>',s),1);
  chdir(s);
  NewMask(FindItem(d,5));
  bar(d.x1+4,d.y2-11,d.x2-4,d.y2-4,BarColor);
  mouseoff;
  mouseon;
  GetDir(0,s);
  FindItem(d,101)^.text := s;
  ReDrawItem(d,101);
 end else
 begin
  PutText(d,1,s);
  ReDrawItem(d,1);
 end;
end;

Procedure GetFilename (x1,y1:word;title,mask,fmtstr:string;var fname:string;var ftype:byte);
var n : word;
    num : byte;
    s : string;
    i:byte;
    ret : string;
    Quit : boolean;
begin
 GetDir(0,curpath);
 InitDialog(d,x1,y1,x1+200,y1+102,title,1);
 AddInputField(d,0,0,15,'',NOP,1);

 AddFrame(d,0,10,86,66,SelectFile,2);
 AddVSlider(d,88,10,98,66,0,64,DrawList,3);

 AddText(d,125,10,'File mask',NOP,4);
 AddInputField(d,111,20,12,'*.*',NewMask,5);

 AddButton(d,146,37,192,46,'OK',NOP,99);
 AddButton(d,146,47,192,56,'Cancel',NOP,100);

 AddButton(d,100,37,145,46,'CH.DIR',NOP,98);
 AddSelector(d,100,47,145,56,fmtstr,NOP,102);

 AddText(d,0,80,CurPath,NOP,101);

 s := GetDriveList;
 for n := 1 to length(s) do
 begin
  AddButton(d,(n-1)*10,68,(n-1)*10+9,77,s[n],NOP,ord(s[n]));
 end;
 InitList(mask);
 num := 1;

 ret := '';
 DrawDialog(d);
 DrawList(FindItem(d,2));
 quit := false;
 repeat
  n := HandleDialog(d);
  case n of
   65..90 : begin
             chdrive(n-65);
             NewMask(FindItem(d,5));
            end;
   98 : GetDir(0,CurPath);
   99 : Begin
          ret := GetText(d,1);
          quit := true;
         end;
   100 : Begin
          ret := '';
          Quit := true;
         end;
  end;
 until Quit;
 removedialog(d);
 getdir(0,s);
 fname := s+'\'+ret;
 ftype := FindItem(d,102)^.data;
 chdir(curpath);
 freedialog(d);
end;

Procedure MessageBox(x1,y1:word;s:string);
var x2,sx2 : word;
begin
 x2 := x1 + length(s)*6 + 17;
 InitDialog(d,x1,y1,x2,y1+45,'Message',1);
 AddText(d,5,5,s,NOP,1);
 sx2 := (((x2-x1) div 2)) - 14;
 AddButton(d,sx2,20,sx2+21,29,'OK',NOP,99);
 drawdialog(d);
 repeat
 until HandleDialog(d) = 99;
 removedialog(d);
 freedialog(d);
end;

Function YesNoBox(x1,y1:word;title,s:string):boolean;
var x2,sx2 : word;
    n:word;
begin
 x2 := x1 + length(s)*6 + 17;
 InitDialog(d,x1,y1,x2,y1+45,title,1);
 AddText(d,5,5,s,NOP,1);
 sx2 := (((x2-x1) div 2)) - 14;
 AddButton(d,sx2-11,20,sx2+11,29,'Yes',NOP,3);
 AddButton(d,sx2+12,20,sx2+33,29,'No',NOP,4);
 drawdialog(d);
 repeat
  n := HandleDialog(d);
  if n=3 then YesNoBox := true else
  if n=4 then YesNoBox := false;
 until (n=3) or (n=4);
 removedialog(d);
 freedialog(d);
end;


begin
end.