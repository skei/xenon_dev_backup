;DOSSEG
.MODEL MEDIUM,PASCAL
.386
.STACK
;JUMPS
LOCALS

MAXWIDTH   equ 320      ; virtual screen dimensions.
MAXHEIGHT  equ 200
MAXCOLOR   equ 15      ; max colors for plasma.
SHIFTVALUE equ 1        ; granularity shift.
                        
;° DATA °°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
.DATA                   
                        
RandSeed dw ?   ; random generator seed.
Palette Label byte
 i=0
 rept 16
  db i*4,0,0
  i=i+1
 endm
 db 768 dup(0)
              
  

                        
                        
;° CODE °°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
.CODE                   

;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
PROC SetPalette
 push ds
 mov ax,seg palette   
 mov ds,ax            
 mov si,offset palette
 mov dx,3C8h
 xor al,al
 out dx,al
 inc dx 
 mov cx,768
 rep outsb
 pop ds
 ret    
ENDP    
        
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
     
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
; GetPixel - get the pixel color at specified location.
; In:
;   ES    - virtual screen segment.
;   (X,Y) - pixel location.
; Out:
;   AL    - pixel color value.
; Modified:
;   BX, Flags.
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
GetPixel macro X,Y
 mov     bx,Y
 imul    bx,MAXWIDTH
 add     bx,X
 mov     al,es:[bx]
endm

;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
; PutPixel - put the pixel color at specified location.
; In:
;   ES    - virtual screen segment.
;   (X,Y) - pixel location.
;   AL    - pixel value.
; Modified:
;   BX, Flags.
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
PutPixel macro X,Y
 mov     bx,Y
 imul    bx,MAXWIDTH
 add     bx,X
 mov     es:[bx],al
endm
 
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
; Adjust - adjust a new pixel value using two neighboring pixels.
; In:
;  (XA,YA) - first near pixel.
;  (XB,YB) - second near pixel.
;  (X,Y)   - new pixel.
; Out:
;  AX      - new pixel color value.
; Modified:
;  AX, BX, DX, SI, DI, Flags.
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
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
     
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
; SubDivide - main plasma routine that divides a region recursively.
; In:
;   (X1,Y1,X2,Y2) - screen plasma region.
; Modified:
;   AX, BX, CX, DX, Flags.
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
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

;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
; DrawPlasma - Main routine to draw plasma into a virtual screen.
; In:                       
;   (X1,Y1,X2,Y2) - Virtual screen Window location.
;   Seed          - Initial random seed for the plasma generator.
;   ScreenSeg     - Virtual screen segment.
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
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
    
;° EntryPoint from DOS °°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
EntryPoint:             
 mov ax,@data           
 mov ds,ax
 
 mov ax,13h
 int 10h
                 
 Call SetPalette
 Call DrawPlasma,0,0,319,199,1234h,0a000h
 mov ah,0
 int 16h
 mov ax,13h
 int 10h
                        
 mov ax,4c00h           
 int 21h                
End EntryPoint          
