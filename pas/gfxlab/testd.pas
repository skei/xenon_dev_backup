Program TestD;

uses GUI,GUILow,Dialogs,Misc;

var s:string;

begin
 InitGUI;
 s := GetFileName(30,30,'Load file','*.*');
 CloseGUI;
end.