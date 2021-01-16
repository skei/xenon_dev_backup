;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
; Textured Tunnel
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

DOSSEG
.MODEL MEDIUM,PASCAL
.STACK
.386
; JUMPS
LOCALS

SEGMENT ODSeg
OffsetData Label byte
 include Offsets.inc
ENDS            
                
SEGMENT TextureSeg
Texture label byte
db 65536 dup(0)
ENDS            
                
MAXWIDTH   equ 256      ; virtual screen dimensions.
MAXHEIGHT  equ 256
MAXCOLOR   equ 15      ; max colors for plasma.
SHIFTVALUE equ 1        ; granularity shift.
                   
;° DATA °°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
.DATA              
                   
RandSeed dw ?   ; random generator seed.
Palette label byte                      
 include palette.db
Sinus label byte
 include sinus.db

Z1 db 0 
R1 db 0
Z2 db 0
R2 db 0


;° CODE °°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
.CODE

;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
; Random - Returns a random number of 16 bits, modified version of the
;  ripped System unit random routine of the Borland Pascal 7.0.
; Out:
;  AX - random value.
; Modified:
;  AX, DX, Flags.
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
Random MACRO
 mov ax,[RandSeed]
 mov dx,8405h
 mul dx
 inc ax
 mov [RandSeed],ax
ENDM
                 
GetPixel macro X,Y
 mov     bx,Y
 imul    bx,MAXWIDTH
 add     bx,X
 mov     al,es:[bx]
endm

PutPixel macro X,Y
 mov     bx,Y
 imul    bx,MAXWIDTH
 add     bx,X
 mov     es:[bx],al
endm

PROC Adjust XA:word,YA:word,X:word,Y:word,XB:word,YB:word
 mov si,[X]                  ; if already painted
 mov di,[Y]                  ; the pixel, use this
 xor ax,ax                   ; one instead of new
 GetPixel si,di                  ; calculation.
 test ax,ax      
 jne AdjExit
 Random                      ; get a random variation
 mov bx,[XB]                 ; dependent of the pixels
 sub bx,[XA]                 ; distance and granularity
 add bx,[YB]                 ; shift factor.
 sub bx,[YA]
 shl bx,SHIFTVALUE
 imul bx
 xor ax,ax                   ; adds the average of the
 GetPixel [XA],[YA]              ; near pixels colors.
 add dx,ax
 GetPixel [XB],[YB]
 add ax,dx
 shr ax,1
 cmp ax,1                    ; check if new color is
 jge ColorUp                 ; in the right range.
 mov ax,1                                      
ColorUp:
 cmp ax,MAXCOLOR
 jle ColorDn
 mov ax,MAXCOLOR
ColorDn:
 PutPixel si,di                  ; paint pixel color.
AdjExit:
 ret 
ENDP 

PROC SubDivide X1:word,Y1:word,X2:word,Y2:word
local   X:word,Y:word
 mov     ax,[X2]                 ; test if this region
 sub     ax,[X1]                 ; needs a sub-division.
 cmp     ax,2
 jge     SubDivCont
 mov     ax,[Y2]
 sub     ax,[Y1]
 cmp     ax,2
 jge     SubDivCont
 jmp     SubDivExit
SubDivCont:
 mov     ax,[X1]                 ; get the center position
 add     ax,[X2]                 ; of the region.
 rcr     ax,1
 mov     [X],ax
 mov     ax,[Y1]
 add     ax,[Y2]
 rcr     ax,1
 mov     [Y],ax
; get the sum of the neighboring four pixel colors.
 xor     cx,cx
 call    Adjust, [X1],[Y1], [X],[Y1], [X2],[Y1]
 add     cx,ax
 call    Adjust, [X2],[Y1], [X2],[Y], [X2],[Y2]
 add     cx,ax
 call    Adjust, [X1],[Y2], [X],[Y2], [X2],[Y2]
 add     cx,ax
 call    Adjust, [X1],[Y1], [X1],[Y], [X1],[Y2]
 add     cx,ax
 
 mov     si,[X]                  ; test if the center pixel
 mov     di,[Y]                  ; need to be calculated.
 GetPixel si,di
 test    al,al
 jne     SubDivNow
 mov     ax,cx                   ; yes, use the average of
 add     ax,2                    ; the neighboring pixels.
 shr     ax,2                    ; (don't allow color 0)
 PutPixel si,di
; sub-divides the new four regions.
SubDivNow:
 call    SubDivide, [X1],[Y1], [X],[Y]
 call    SubDivide, [X],[Y1], [X2],[Y]
 call    SubDivide, [X],[Y], [X2],[Y2]
 call    SubDivide, [X1],[Y], [X],[Y2]
SubDivExit:
 ret
ENDP

PROC DrawPlasma XStart:word,YStart:word,XStop:word,YStop:word,Seed:word,ScreenSeg:word
 mov     ax,[ScreenSeg]          ; setup virtual screen
 mov     es,ax                   ; segment,
 mov     ax,[Seed]               ; and random calculation
 mov     [RandSeed],ax           ; seed.
 Random                  ; set window corner pixels.
 or      al,1   
 PutPixel [XStart],[YStart]
 Random         
 or      al,1   
 PutPixel [XStop],[YStart]
 Random                     
 or      al,1
 PutPixel [XStart],[YStop]
 Random
 or      al,1
 PutPixel [XStop],[YStop]
 call    SubDivide, [XStart],[YStart], [XStop],[YStop]
 ret
ENDP

;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
PROC SetPalette
 push ds
 mov dx,3c8h
 xor al,al
 out dx,al
 inc dx
 mov cx,768
 mov ax,SEG Palette
 mov ds,ax
 mov si,OFFSET Palette
 rep outsb
 pop ds        
 ret           
ENDP           
               
                    
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
PROC DrawTunnel
; Set-up Segments
 push ds       
 mov ax,SEG Texture
 mov fs,ax
 mov ax,SEG Texture
 mov gs,ax
 mov ax,0a000h        
 mov es,ax     
 mov ax,SEG OffsetData
 mov ds,ax
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
 xor di,di      ; Screen
 xor si,si      ; Offset data
 mov bp,100
DrawLoop:      
 rept 160
  mov bx,[ds:si]
  add bx,cx             ; CX = adder for rot/zoom
  mov al,[fs:BX]
  shl al,4  
  sub bx,cx 
  add bx,dx 
  add al,[gs:bx]
  add si,2      
  mov ah,al                                      
  mov [es:di],ax                                 
;;  mov [es:di+320],ax
  add di,2                                       
 endm                                            
; add di,320
 dec bp                                          
 jnz DrawLoop                                    
 pop ds      
 ret         
ENDP         
          
PROC WaitVR 
 mov dx,3DAh
@@1:               
 in al,dx          
 test al,8         
 jnz @@1             
@@2:                
 in al,dx     
 test al,8      
 jz @@2         
 ret            
ENDP            


ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
;° EntryPoint from DOS °°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
EntryPoint:      
 mov ax,@DATA                           
 mov ds,ax                               
 Call DrawPlasma,0,0,255,255,1234h,SEG Texture
 mov ax,13h
 int 10h
; Double each scanline -> 320x100
 mov dx,3d4h
 mov al,9
 out dx,al  
 inc dx     
 in al,dx   
 and al,0E0h
 add al,3   
            
 out dx,al  
;           
            
 Call SetPalette                 
 mov cx,1040h           ; CL=R1, CH=Z1
 mov dx,2050h           ; DL=R2, DH=Z2
TestLoop:       
 push dx    
 Call WaitVR
 pop dx     
 Call DrawTunnel
            
 xor bh,bh  
            
 add cl,5                       ; Denne inverteres ved takten
 mov bl,[Z1]
 add ch,[Sinus+bx]

 mov bl,[R2]
 add dl,[Sinus+bx]
 add dh,2
            
 add [Z1],1
; add [Z2],2
; add [R1],3
 add [R2],2
             
 Call WobbleTexture

 in al,60h   
 cmp al,1   
 jne TestLoop
 mov ax,3h  
 int 10h    
 mov ax,4c00h
 int 21h    
END EntryPoint
