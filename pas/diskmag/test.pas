Program Test;

Uses Mode12h;

begin
 GFXMode;
 DrawString(0,0,' ABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890.,'+#39+'!/+-()�����',15);
 readln;
 TXTMode;
end.
