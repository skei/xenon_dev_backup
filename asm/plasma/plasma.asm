DOSSEG
.MODEL MEDIUM
.386
.STACK 
;JUMPS 
LOCALS 

;° DATA °°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
.DATA
           
Label COS byte
 include cos.db

; 8.8 fixed point values
xcos1 dw ?              
xcos2 dw ?              
ycos1 dw ?              
ycos2 dw ?
          
;° CODE °°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
.CODE     
          
PROC DrawPlasma
 mov ax,0a000h
 mov es,ax
 xor di,di
; ecx/edx = 8.8:8.8 cos indexes
; esi/ebp = adders
 mov ah,200
Outer:     
 rept 320                      
  xor bl,bl                    
  shld ebx,ecx,8        ; bl = cos index 1
  mov al,[Cos+bx]              
  mov bl,ch                    
  add al,[Cos+bx]              
  xor bl,bl                    
  shld ebx,edx,8        ; bl = cos index 1
  add al,[Cos+bx]
  mov bl,dh                    
  add al,[Cos+bx] 
  stosb           
  add ecx,esi     
 endm      
 add edx,ebp
 dec ah    
 jnz Outer   
 ret
ENDP 
                               
Proc DoPlasma
 mov [xcos1],0
 mov [xcos2],0
 mov [ycos1],0
 mov [ycos2],0
DoLoop:   
 mov cx,[xcos1]
 shl ecx,16
 mov cx,[xcos2]
 mov dx,[ycos1]
 shl edx,16
 mov dx,[ycos2]
 mov esi,01010101h
 mov edi,02010201h
 call DrawPlasma
 add [xcos1],128
 add [xcos2],256
 add [ycos1],256
 add [ycos2],384
 in al,60h 
 cmp al,1  
 jne DoLoop
 ret      
ENDP      
          
;° EntryPoint from DOS °°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
EntryPoint:                    
 mov ax,@data  
 mov ds,ax
 cld

 mov ax,13h
 int 10h
 Call DoPlasma
 mov ax,3
 int 10h
        
 mov ax,4c00h
 int 21h
End EntryPoint
