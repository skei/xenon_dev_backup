; TWIRL/TUNNEL           
              
;DOSSEG       
.486p ;.386   
.MODEL FLAT,PASCAL
.STACK
;JUMPS        
LOCALS        
.CODE         
              
MAXWIDTH   equ 256      ; virtual screen dimensions.
MAXHEIGHT  equ 256
MAXCOLOR   equ 60      ; max colors for plasma.
SHIFTVALUE equ 1        ; granularity shift.
                
include e:\demo\support.inc
                         
;° DATA °°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
Screen dd ?
texture dd ?
               
RandSeed dw 1234h       ; random generator seed.
XStart dw ?             
YStart dw ?             
XStop dw ?              
YStop dw ?              
Seed dw 1234h           
                        
xx dw ?                 
yy dw ?                 
                        
label twirlOffsets byte
 include twi.db
label bitmap byte
 include b1.db;bear.db;bitmap.db
 db 16000 dup(0)
label Palette byte
 include palette.db

RotZoom dw ?
AllocErrorMess db 'Error allocating 64K of memory!',13,10,36
            

;° CODE °°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
                  
PROC DrawTwirl    
 mov dx,[RotZoom]
 mov esi,offset twirloffsets
 mov edi,[Screen]        
 mov ecx,100             
Outer1:                                      
 rept 160
  xor ebx,ebx    
  mov bx,[esi]                  ;; AX = GetPixel-offset in texture
  add esi,2        
  add bx,dx                     ;; Add X-offset/Rotation
  add ebx,[texture]
  mov al,[ebx]     
  mov ah,al 
  mov [edi],ax
  mov [edi+320],ax
  add edi,2 
 endm               
 add edi,320
 dec ecx            
 jnz Outer1         
 ret                
ENDP               
                   
Proc DoTwirl       
 mov [rotzoom],0100h
@@1:                
 Call DrawTwirl
 add [rotzoom],0302h
 in al,60h        
 cmp al,1         
 jne @@1           
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
                   
 mov [texture],offset bitmap

 mov ax,13h  
 int 10h     
 mov esi,offset palette    
 mov al,0                    
 mov ecx,768 
 call setpalette
 Call DoTwirl              
QuitIntro:   
 mov ax,3                    
 int 10h                     
 mov ax,4c00h                
 int 21h
End EntryPoint
