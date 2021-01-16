 DOSSEG        
.MODEL MEDIUM
.386
.STACK
JUMPS
LOCALS

PicWidth = 256
PicHeight = 128
InterSinAdd = 1

;° GFX °°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°

SEGMENT Buffer
 db 64000 dup(0)
ENDS 
              
SEGMENT GFXSeg
Label Picture byte
 include gfx.db
Label Palette byte
 include pal.db 
ENDS
    
    
;°±²Û²±°±²Û²±°±²Û²±°±²Û²±°±²Û²±°±²Û²±°±²Û²±°±²Û²±°±²Û²±°±²Û²±°±²Û²±°
 .DATA    
;°±²Û²±°±²Û²±°±²Û²±°±²Û²±°±²Û²±°±²Û²±°±²Û²±°±²Û²±°±²Û²±°±²Û²±°±²Û²±°
         
; 256 entries - from -32 to 32
Sinus Label byte  
 include sin.db   
                  
SinPos1 db ?
SinPOs2 db ?
LinjePosisjon DW ?
                 
;° CODE °°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
.CODE          
                 
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
PROC WaitVR      
 mov dx,3DAh     
WaitStartVR:     
 in al,dx
 test al,8
 jz WaitStartVR
WaitEndVR:    
 in al,dx
 test al,8
 jnz WaitEndVR                
 ret 
ENDP

;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
PROC SetPalette
 call WaitVR   
 mov dx,3C8h   
 xor al,al 
 out dx,al 
 inc dx
 mov cx,768    
 rep outsb
 ret                          
ENDP                          
                              
; [SInPos2]+32 = x1   
; 320 - [SInPos2]+32 = x2
; LInjelengde p† 256 skal fylles inn mellom disse, adder i texture =
; 320*256 / length ???
WobbleLine MACRO
local @@1 
 movsx bp,[sinpos2]
 mov al,[DS:Sinus+bp]   ; ax = fra -32 til 32
 cbw                        
 add ax,32              ; ax fra 0 til 64
 add di,ax                          
 mov bp,320                         
 sub bp,ax                           
 sub bp,ax              ; bp = lengde p† linja
 mov ax,320
@@1:        
 mov al,[FS:SI]
 inc si     
 mov [ES:DI]
ENDM           
               
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
PROC DrawDuff                 
 mov ax,SEG Picture           
 mov fs,ax                    
 mov ax,0a000h 
; mov ax,SEG Buffer            
 mov es,ax        
 xor di,di        
 Call WaitVR   
 xor bx,bx                                  
 mov bl,[SinPos1]       ; Sinus-start
 mov cx,PicHeight       ; Number of lines to draw
 xor dx,dx              ; Linje-teller                                       
DuffLoop:                                   
 xor ax,ax                                  
 mov al,[Sinus+bx]
 cbw
 add ax,dx              ; al = linje + sinus
 inc bl                 ; Sinus-index                  
 inc dl                 ; Linje nummer -> 0-200 + sinus
 add ax,[LinjePosisjon]             
 cmp ax,0       
 jl SkipLine    
 cmp ax,199                                           
 jg SkipLine                                          
 movzx esi,ax  
 lea si,[esi+esi*4]
 shl si,6        
 Jmp DontSkip
SkipLine:       
 mov si,0    
DontSkip:
 WobbleLine
 dec cx        
 jnz DuffLoop  
 ret           
ENDP                       
                      
;° EntryPoint from DOS °°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
EntryPoint:    
 mov ax,13h    
 int 10h       
 cld           
 mov ax,SEG Palette
 mov ds,ax     
 mov si,OFFSET Palette
 Call SetPalette
 mov ax,@DATA  
 mov ds,ax     
 mov [SinPos1],0
 mov [LinjePosisjon],0
MainLoop:            
 Call DrawDuff
; Call DrawWobble
 add [SinPos1],5
 add [SinPos2],3
 in al,60h            
 cmp al,1             
 jne MainLoop  
QuitLoop:               
 mov ax,3      
 int 10h            
 mov ax,4c00h  
 int 21h       
End EntryPoint 
               