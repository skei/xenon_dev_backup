Program SmpMaker;

Uses GUI,RawPlay;

Type Table = array[0..255] of shortint;

var Sine : table;
    saw  : table;
    square : table;
    noise : table;

Var MM,HM,SM : DialogType;
    Redraw:boolean;
    i:integer;
    w:word;
    s:string;
    n:byte;
    d:integer;

Type InsType = record
      Waveform : byte;
      volume   : byte;
      Command  : byte;
      adder    : word;
      size     : word;
     end;

Type SampleType = array[0..59999] of shortint;
Var Sample : ^SampleType;
Var Ins : array[1..31] of instype;
var ssize : word;
    f:file;

Procedure SetupTables;
var x : word;
    y : real;
Begin
 y := -pi;
 randomize;
 For x := 0 to 255 DO
 Begin
  Sine[x]:=Round(sin(y)*127);
  Saw[x] :=X-128;
  Noise[x] := random(255)-128;
  if x>128 then square[x] := -127 else square[x] := 127;
{  Square[x] := ((x and 128)*2)-128;}
  y := y + (pi*2 / 256)
 End;
end;

Procedure SetupMenus;
begin
{Main Menu}
 InitDialog(MM,'Sample Creator - (C)1995 - Axon/X.D.',20,50,300,150);
 AddText(MM,10,12,'Instrument',0);
 AddButton(MM,75,10,85,18,'+',80);
 AddInputField (MM,85,10,2,'1',1);
 AddButton(MM,105,10,115,18,'-',81);
 AddText(MM,125,12,'Size',0);
 AddButton(MM,154,10,169,18,'x2',82);
 AddInputField (MM,169,10,5,'256',2);
 AddButton(MM,207,10,222,18,'/2',83);
 AddButton(MM,230,10,262,19,'Make',90);
 AddButton(MM,230,20,262,29,'Play',91);
 AddButton(MM,230,30,262,39,'Show',92);
 AddButton(MM,230,40,262,49,'Load',93);
 AddButton(MM,230,50,262,59,'Save',94);
 AddButton(MM,230,60,262,69,'Help',95);
 AddButton(MM,230,70,262,79,'Quit',99);
 AddText(MM,10,30,'Waveform',0);
 for i := 1 to 4 do AddButton(MM,10,30+i*10,59,39+i*10,'None',i+9);
 AddText(MM,70,30,'Command',0);
 for i := 1 to 4 do AddButton(MM,70,30+i*10,119,39+i*10,'Set',i+19);
 AddText(MM,130,30,'Vol',0);
 for i := 1 to 4 do AddInputField(MM,130,30+i*10,3,'255',i+29);
 AddText(MM,166,30,'Adder',0);
 for i := 1 to 4 do AddInputField(MM,166,30+i*10,5,'256',i+39);
{Help Menu}
 InitDialog(HM,'Help',30,67,280,117);
 AddButton(HM,212,26,242,35,'OK',1);
 AddText(HM,0,0, 'Hmmm... As you can see, this proggy is',0);
 AddText(HM,0,6, 'far from finished. Read the DOC file for',0);
 AddText(HM,0,12,'more info... :)',0);

{ShowSample Menu}
 InitDialog(SM,'Sample',30,25,30+256+7,25+64+15+11);
 AddButton(SM,212,66,242,75,'OK',1);
end;

Procedure SaveInstrument(inn:byte);
var d : byte;
begin
 s := MM.item[inn].text;
 if s = 'None' then ins[inn].Waveform := 0;
 if s = 'Sine' then ins[inn].Waveform := 1;
 if s = 'Square' then ins[inn].Waveform := 2;
 if s = 'Saw' then ins[inn].Waveform := 3;
 if s = 'Random' then ins[inn].Waveform := 4;
end;

Procedure HandleInstrument;
var nn:byte;
begin
 nn := FindID(MM,1);
 val(MM.Item[nn].Text,i,d);
 if (i > 31) or (i < 1) then i := 1;
 str(i,MM.Item[nn].Text);
 RedrawItem(MM,nn);
end;

Procedure Handlewave(n:byte);
var nn:byte;
begin
 nn := FindID(MM,n);
 s := MM.Item[nn].Text;
 if s = 'None' then MM.Item[nn].Text := 'Sine';
 if s = 'Sine' then MM.Item[nn].Text := 'Square';
 if s = 'Square' then MM.Item[nn].Text := 'Saw';
 if s = 'Saw' then MM.Item[nn].Text := 'Random';
 if s = 'Random' then MM.Item[nn].Text := 'None';
 ReDrawItem(MM,nn);
end;

Procedure HandleCommand(n:byte);
var nn:byte;
begin
 nn := FindID(MM,n);
 s := MM.Item[nn].Text;
 if s = 'Set' then MM.Item[nn].Text := 'Mix';
 if s = 'Mix' then MM.Item[nn].Text := 'Add';
 if s = 'Add' then MM.Item[nn].Text := 'Sub';
 if s = 'Sub' then MM.Item[nn].Text := 'Max';
 if s = 'Max' then MM.Item[nn].Text := 'Min';
 if s = 'Min' then MM.Item[nn].Text := 'Set';
 ReDrawItem(MM,nn);
end;

Procedure HandleVolume(n:byte);
var nn:byte;
begin
 nn := findid(MM,n);
 val(MM.Item[nn].Text,i,d);
 if (i > 255) or (i < 1) then i := 0;
 str(i,MM.Item[nn].Text);
 RedrawItem(MM,nn);
end;

Procedure HandleAdderInt(n:byte);
var nn:byte;
begin
 nn := findid(MM,n);
 val(MM.Item[nn].Text,w,d);
 if (w > 65535) or (w < 1) then w := 0;
 str(w,MM.Item[nn].Text);
 RedrawItem(MM,nn);
end;

Procedure HandleInsInc(n:byte);
var nn:byte;
begin
 nn := FindID(MM,1);
 val(MM.Item[nn].Text,i,d);
 if i<31 then inc(i);
 str(i,MM.Item[nn].Text);
 ReDrawItem(MM,nn);
end;

Procedure HandleInsDec(n:byte);
var nn:byte;
begin
 nn := FindID(MM,1);
 val(MM.Item[nn].Text,i,d);
 if i>1 then dec(i);
 str(i,MM.Item[nn].Text);
 ReDrawItem(MM,nn);
end;

Procedure HandleSizeDouble(n:byte);
var nn:byte;
begin
 nn := FindID(MM,2);
 val(MM.Item[nn].Text,w,d);
 if w<32767 then w := w*2 else w := 60000;
 str(w,MM.Item[nn].Text);
 ReDrawItem(MM,nn);
end;

Procedure HandleSizeHalve(n:byte);
var nn:byte;
begin
 nn := FindID(MM,2);
 val(MM.Item[nn].Text,w,d);
 if w>2 then w := w div 2 else w := 0;
 str(w,MM.Item[nn].Text);
 ReDrawItem(MM,nn);
end;

Procedure HandleSize(n:byte);
var nn:byte;
begin
 nn := findid(MM,n);
 val(MM.Item[nn].Text,w,d);
 if (w > 65535) or (w < 1) then w := 256;
 str(w,MM.Item[nn].Text);
 RedrawItem(MM,nn);
end;

Procedure HandleShow;
var nn:byte;
    size:word;
    value : byte;
    nx,ny : word;
begin
 DrawDialog(SM);
 mouseoff;
 DrawLineH(SM.x1+4,SM.x2-4,SM.y1+11+32,8);
 val(MM.Item[FindID(MM,2)].text,size,d);
 for w := 0 to 255 do
 begin
  value := (sample^[w*(Size div 256)]) div 4;   {-31 to 31}
  nx := SM.X1+4+w;
  ny := SM.Y1+11+32+value;
  PutPixel (nx,ny,15);
 end;
 Draw3DString(SM.X1+4,SM.Y2-10,'SIZE: '+MM.Item[FindID(MM,2)].text,true);
 mouseon;
 Repeat
 until Handledialog(SM) = 1;
 removedialog(SM);
end;


Procedure GetParameters(num:byte;var Wave,command,volume:byte;var adder:word);
var s:string;
begin
 s := MM.Item[FindID(MM,num+9)].Text;
 if s = 'None' then Wave := 0;
 if s = 'Sine' then Wave := 1;
 if s = 'Square' then Wave := 2;
 if s = 'Saw' then Wave := 3;
 if s = 'Random' then Wave := 4;
 s := MM.Item[FindID(MM,num+19)].Text;
 if s = 'Set' then Command := 0;
 if s = 'Mix' then Command := 1;
 if s = 'Add' then Command := 2;
 if s = 'Sub' then Command := 3;
 if s = 'Max' then Command := 4;
 if s = 'Min' then Command := 5;
 val(MM.Item[FindID(MM,num+29)].text,volume,d);
 val(MM.Item[FindID(MM,num+39)].text,adder,d);
end;

Procedure Make;
var num,wave,command,volume,modbyte : byte;
    adder,size,tablepos : word;
begin
 val(MM.Item[FindID(MM,2)].text,size,d);
 for i := 1 to 4 do
 begin
  tablepos := 0;
  GetParameters(i,Wave,command,volume,adder);
  for w := 0 to size-1 do
  begin
   tablepos := tablepos + adder;
   if wave <> 0 then
   begin
    case wave of
     1   : modbyte := sine[tablepos div 256];
     2   : modbyte := square[tablepos div 256];
     3   : modbyte := saw[tablepos div 256];
     4   : modbyte := noise[tablepos div 256];
     else modbyte := 0;
    end;
    case command of
     0 {Set} : sample^[w] := modbyte;
     1 {Mix} : sample^[w] := (sample^[w] + modbyte) div 2;
     2 {Add} : begin
                sample^[w] := (sample^[w] + modbyte);
                if sample^[w] > 127 then sample^[w] := 127;
                if sample^[w] < -127 then sample^[w] := -127;
               end;
     3 {Sub} : begin
                sample^[w] := (sample^[w] - modbyte);
                if sample^[w] > 127 then sample^[w] := 127;
                if sample^[w] < -127 then sample^[w] := -127;
               end;
     4 {Max} : begin
                if abs(sample^[w]) < abs(modbyte) then sample^[w] := modbyte;
               end;
     5 {Min} : begin
                if abs(sample^[w]) > abs(modbyte) then sample^[w] := modbyte;
               end;
    end;
   end;
  end;
 end;
end;

Begin
 InitGUI;
 SetupMenus;
 SetupTables;
 getmem(Sample,64000);
 fillchar(sample^,64000,0);
 DrawDialog(MM);
 n := 0;
 Repeat
  n := handledialog(MM);
  Redraw := false;
  case n of
   1      : HandleInstrument;
   2      : HandleSize(n);
   10..13 : HandleWave(n);
   20..23 : HandleCommand(n);
   30..33 : HandleVolume(n);
   40..43 : HandleAdderInt(n);
   80     : HandleInsInc(n);
   81     : HandleInsDec(n);
   82     : HandleSizeDouble(n);
   83     : HandleSizeHalve(n);
   90     : Make;
   91     : begin
             {val(MM.Item[FindID(MM,2)].text,ssize,d);
             PlaySound(sample,ssize,32000,1);}
            end;
   92     : HandleShow;
   94     : Begin      {save - RAW fil bare ennuh}
             val(MM.Item[FindID(MM,2)].text,w,d);
             assign(f,'sound.raw');
             rewrite(f,1);
             blockwrite(f,sample^,w);
             close(f);
            end;
   95     : Begin
             DrawDialog(HM);
             Repeat
             until Handledialog(HM) = 1;
             removedialog(HM);
            end;
  end;
 until n = 99;
 RemoveDialog(MM);
 freemem(Sample,64000);
 CloseGUI;
end.