.MODEL MEDIUM
.386
.STACK
;JUMPS
LOCALS

PicW = 320
PicH = 200

Segment TexSeg
Label Texture byte
; include tex.inc
ENDS        
            
;° DATA °°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
.DATA       

PixelSize dw ?
LineBuffer db 320 dup(0)
                              
;° CODE °°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
.CODE                         
            
Proc DrawPixelize
 cld      
 xor bx,bx
 xor bp,bp
 mov si,OFFSET Texture
 mov ax,SEG LineBuffer
 mov es,ax            
 mov di,OFFSET LineBuffer
@@1:                      
 mov al,[si+bx]
 mov cx,[PixelSize]
 rep stosb
 add bx,[PixelSize]
 cmp bx,PicW
 jb @@1
; Tegn ut [PixelSize] antall bufre til skjermen
 mov cx,[PixelSize]
 mov ax,0a000h
 mov es,ax
 mov di,bp
@@2:    
i=0                
rept 320              
 mov al,[LineBuffer+i]
 mov [es:di+i],al
endm    
 add di,320
 dec cx 
 jnz @@1
 ret
ENDP 
          
          
          
                               
            
            
          
;° EntryPoint from DOS °°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
EntryPoint:
 mov ax,@data
 mov ds,ax
 
@@1:          
 mov ax,4c00h
 int 21h  
End EntryPoint
