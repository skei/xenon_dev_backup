;DOSSEG
.486p ;.386
.MODEL FLAT ;.MODEL MEDIUM
.STACK
;JUMPS
LOCALS
.CODE

;° DATA °°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
Screen dd ?
           
;° CODE °°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
                      
Tabell label byte
 db 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15
               
PROC DrawPoint        
 mov bx,15
 mov al,[Tabell+bx]
 mov edi,[Screen]
 mov byte ptr [edi+330],al
 ret
ENDP 
    
;° EntryPoint from DOS °°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
EntryPoint:
; mov ax,@data
; mov ds,ax
 mov eax,0EE02h
 int 31h
 mov edi,0A0000h
 sub edi,ebx   
 mov [Screen],edi
               
 mov ax,13h    
 int 10h       
 Call DrawPoint
        
 mov ah,0
 int 16h
 mov ax,3
 int 10h

 mov ax,4c00h
 int 21h
End EntryPoint
