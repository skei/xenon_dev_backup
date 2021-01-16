; Wobbling
          
;DOSSEG   
.MODEL MEDIUM
.386      
.STACK    
;JUMPS    
LOCALS    
          
;° DATA °°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
.DATA     

;° CODE °°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
.CODE

;  for y := 0 to 99 do
;  begin
;  XSinus := Xouter;
;  ysinus := youter;
;   for x := 0 to 127 do
;   begin
;    xpos := x+sine[xsinus];
;    ypos := y+sine[ysinus];
;    c := texture[xpos,ypos];
;    w := c SHL 8 + c;
;    memw[$A000:y*640+x*2] := w;
;    memw[$A000:y*640+320+x*2] := w;
;    xsinus := (xsinus+2);
;    ysinus := (ysinus+3);
;   end;
;  end;




;° EntryPoint from DOS °°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
EntryPoint:
 mov ax,@data
 mov ds,ax

 mov ax,4c00h
 int 21h
End EntryPoint
