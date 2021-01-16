.486p
.MODEL FLAT
.STACK     
;JUMPS     
LOCALS
.CODE

;° DATA °°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
Screen dd ?
           
;° CODE °°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°

;Texture 1 = db Phong/EnvMap
;Texture 2 = dw Offset addere til Texture 1
                  
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
; ECX = Xint:XFrac  YInt:YFrac      Bump
; EDX = Xint:Xfrax  YInt:YFrac      Phong
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

 push ecx         
 push edx         
inner:            
 shld ebx,ecx,8     ; BL = Xint
 mov bh,ch          ; BH = Yint : BX = y*256+x
 add bx,bx          ; pga Words i Bump tabell
 mov si,[Bump+bx]   ; SI = Adderes til Phong offset
 shld ebx,edx,8   
 mov bh,dh        
 add bx,si        
 mov bl,[Phong+bx]  ; BL = Color/Pixel
 add ecx,12345678h
 add edx,12345678h
 mov [edi],bl     
 dec bp           
 jnz Inner        
;                 
 pop edx
 pop ecx
 add ecx,12345678h
 add edx,12345678h
 


;° EntryPoint from DOS °°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
EntryPoint:
 mov eax,0EE02h
 int 31h
 mov edi,0A0000h
 sub edi,ebx
 mov [Screen],edi

 mov ax,4c00h
 int 21h
End EntryPoint
