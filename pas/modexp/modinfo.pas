Program ModInfo;

Type SampleInfo = record
      Samplename : array[1..22] of byte;
      Samplelength : word;
      Finetune : byte;
      Volume : byte;
      RepPoint : word;
      RepLength : word;
     end;

Var SongName : array[1..20] of byte;
    SongLength : byte;
    dummy : byte;
    SongPos : array[1..128] of byte;
    Marker : string[4];
    samples : array[1..31] of SampleInfo;
    Pattern : array[1..1024] of byte;

var f:file;
    i,j:integer;

Function swp(a:word):word;
begin
 asm
  mov ax,a
  xchg ah,al
  mov @result,ax
 end;
end;

Begin
 assign(f,paramstr(1));
 reset(f,1);
 blockread(f,songname,sizeof(songname));
 BlockRead(f,Samples,sizeof(samples));
 blockread(f,songlength,sizeof(songlength));
 blockread(f,dummy,sizeof(dummy));
 blockread(f,songpos,sizeof(songpos));
 blockread(f,marker,sizeof(marker));
 for i := 1 to Songlength do Blockread(f,pattern,sizeof(pattern));
 close(f);
 Writeln;
 Write ('MOD-name: ');
 for i := 1 to 20 do write(chr(songname[i]));
 Writeln;
 Writeln ('SongLength: ',songlength);
 Write ('Playing order þ ');
 for i := 1 to Songlength do Write(SongPos[i],':');
 writeln;
 for i := 1 to 31 do
 begin
  Write ('Sample #',i,' : ');
  for j := 1 to 22 do write(chr(samples[i].samplename[j]));
  Writeln ('Samplelength: ',swp(samples[i].samplelength)*2);
 end;

 readln;
end.

