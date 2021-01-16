uses crt,Poll;

type lrgarr = array[0..65534]of byte;

var
 pal : array[0..767] of byte;
 mp,scr,color : ^lrgarr;
 rng : array[0..320]of byte;
 dir,i,x,y : integer;
 f:file;
 w:word;
 Sine : array[0..360] of integer;
 Cosine : Array[0..360] of Integer;
 XSpeed : integer;
 YSpeed : integer;

 mul320 : array[0..199] of word;

Procedure MakeTables;
Var dir : integer;
Begin
 For dir := 0 to 360 do
 Begin
  Sine[dir]   := round(256*sin ( dir/180 * pi));
  Cosine[dir] := round(256*cos ( dir/180 * pi));
 End;
 for dir := 0 to 199 do
 begin
  mul320[dir] := dir*320;
 end;
End;

Procedure ClearScreen(Var S); Assembler;
Asm
 Les di,S
 db 66h; Xor ax,ax
 Mov cx,16000
 db 66h; Rep Stosw
End;

Procedure CopyScreen(Var S, D); Assembler;
Asm
 Push ds
 Les di,D
 Lds si,S
 Mov cx,16000
 db 66h; Rep Movsw
 Pop  ds
end;


procedure draw(xp,yp,dir : integer);
var ixp,iyp,z : integer{byte};
    zobs,iy1,y:integer;
    ix,iy,x,s,csf,snf,mpc,i,j : integer;
    clr : byte;
    ps:word;
    point:pointer;
begin
 fillchar(rng,sizeof(rng),200);
 zobs:=180;{100+mp^[256*yp+xp];}      {Fly-h›yde}
{
 csf:=cosine[dir];
 snf:=sine[dir];
}
 csf:=round(256*cos(dir/180*pi));
 snf:=round(256*sin(dir/180*pi));

 ClearScreen(scr^);

 for iy:=yp to yp+50{55} do
 begin
  iy1:=1+2*(iy-yp);
  s:=4+300 div iy1;

  for ix:=xp+yp-iy to xp-yp+iy do
  begin
{   ixp:=xp+((ix-xp)*csf+(iy-yp)*snf) div 256;}
   asm
    mov ax,[ix]
    sub ax,[xp]
    cwd
    mov bx,[csf]
    imul bx
    mov cx,ax
    mov ax,[iy]
    sub ax,[yp]
    cwd
    mov bx,[snf]
    imul bx
    add ax,cx
    sar ax,8
    add ax,[xp]
    mov [ixp],ax
   end;
{   iyp:=yp+((iy-yp)*csf-(ix-xp)*snf) div 256;}
   asm
    mov ax,[ix]
    sub ax,[xp]
    cwd
    mov bx,[snf]
    imul bx
    mov cx,ax
    mov ax,[iy]
    sub ax,[yp]
    cwd
    mov bx,[csf]
    imul bx
    sub ax,cx
    sar ax,8
    add ax,[yp]
    mov [iyp],ax
   end;

   x:=160+360*(ix-xp) div iy1;
   if (x>=0) and (x+s<=318) then

   begin
    z:=mp^[256*iyp+ixp];
    clr :=color^[256*iyp+ixp];
{
    asm
     mov bh,[iyp]
     mov bl,[ixp]
     push es
     les di,mp
     mov al,[es:di+bx]
     les di,color
     mov cl,[es:di+bx]
     pop es
     mov [z],al
     mov [clr],cl
    end;
}
{    y:=100+(zobs-z)*32 div iy1;}
    asm
     mov ax,[zobs]
     mov bx,[z]
     sub ax,bx
     shl ax,5
     cwd
     mov bx,[iy1]
     idiv bx
     add ax,100
     mov [y],ax
    end;

    if (y<=199) and (y>=0) then
    begin
     for j:=x to x+s do
     begin
      ps := mul320[y];
      inc(ps,j);

      for i:=y to rng[j] do
      begin
       scr^[ps]:=clr;
       inc(ps,320);
      end;

      if y<rng[j] then rng[j]:=y;
     end;
    end;
   end;
  end;
 end;
 CopyScreen(scr^,ptr($a000,0)^);
end;

begin
 Writeln ('Voxel LandScape - Axon/Xenon Development');
 Writeln ('Try the arrow-keys');
 Writeln ('PS: This is PURE Pascal!');
 Delay(3000);
 randomize;
 x:=0;
 y:=0;
 XSpeed := 0;
 YSpeed := 0;
 dir:=0;
 new(mp);
 new(color);
 new(scr);
 fillchar(mp^,65535,0);
 assign(f,'h.raw');
 reset(f,1);
 blockread(f,mp^,65535);
 close(f);
 assign(f,'p.raw');
 reset(f,1);
 blockread(f,pal,768);
 close(f);
 assign(f,'t.raw');
 reset(f,1);
 blockread(f,color^,65535);
 close(f);
 MakeTables;
{ for w := 0 to 65534 do mp^[w] := round(mp^[w]*1);}
 asm
  xor ax,ax
  mov al,$13
  int $10
 end;
 port[$3C8]:=0;
 for i:=0 to 767 do port[$3C9]:=pal[i];
 repeat
  dir:=dir mod 360;
  draw(x,y,dir);

  if keytable[1] then
  begin
   asm
    xor ax,ax
    mov al,$3
    int $10
   end;
   HALT
  end;
  y:=y+round((XSpeed/2.5)*cos(dir/180*pi));
  x:=x+round((XSpeed/2.5)*sin(dir/180*pi));
  inc(dir,YSpeed);
  if keytable[$48] and (xspeed<10) then inc(Xspeed,2);
  if keytable[$50] and (xspeed>-10) then dec(XSpeed,2);
  if keytable[$4B] and (yspeed>-10)then dec(YSpeed,2);
  if keytable[$4d] and (yspeed<10) then inc(YSpeed,2);

  if Xspeed > 0 then dec (Xspeed) else if XSpeed < 0 then inc(XSpeed);
  if yspeed > 0 then dec (yspeed) else if ySpeed < 0 then inc(ySpeed);
 until false;
end.
