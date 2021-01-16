; Mode 12h eksperimentering...
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
; Bitplanes:
;10000000       Pixel til venstre = 1011 = 13...
;00000000       80x480 bytes...
;10000000                      
;10000000                      
                               
DOSSEG                         
.MODEL MEDIUM                  
.386                           
.STACK                         
;JUMPS                         
LOCALS                         
                               
;° DATA °°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
.DATA
          
;° CODE °°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
.CODE

; bp = 4 bits - 1 for † "sl† p†" relevant bit-plan
WritePlane MACRO wp                               
 mov dx,3ceh                                      
 xor al,al                                        
 out dx,al                                        
 inc dx
 mov al,wp
 out dx,al
ENDM

; rp = 00-11 = 0-3 = bit-plan for "read-access"
ReadPlane MACRO rp                             
 mov dx,3ceh                                   
 mov al,4                                      
 out dx,al
 inc dx
 mov al,rp
 out dx,al 
ENDM       
                                               
; CX = x, DX = y, si=color
PROC PutPixel                                    
 mov bp,cx                 
 shr bp,3               ; Del p† 8 = Byte verdi for X
 and edx,0ffffh
 lea di,[edx+edx*4]
 shl di,4               ; * 80               
 add di,bp              ; ES:DI -> byte to change
                                                 
 and cx,7               ; Pixel innen byte
 mov bl,128                   
 shr bl,cl              ; BL = bit to change in byte
;              
 test si,1     
 jz @1         
 ReadPlane 0
 WritePlane 1  
 or [es:di],bl 
@1:            
 test si,2     
 jz @2         
 ReadPlane 1   
 WritePlane 2  
 or [es:di],bl 
@2:            
 test si,4     
 jz @3         
 ReadPlane 2   
 WritePlane 4
 or [es:di],bl
@3:          
 test si,8   
 jz @4       
 ReadPlane 3
 WritePlane 8
 or [es:di],bl
@4:         
 ret        
ENDP        
            
;° EntryPoint from DOS °°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
EntryPoint:
 mov ax,@data
 mov ds,ax
 
 mov ax,12h
 int 10h
              
 mov cx,1     
 mov di,1     
 mov si,15
 Call PutPixel
        
 mov ah,0
 int 16h
 mov ax,3
 int 10h
        
 mov ax,4c00h
 int 21h
End EntryPoint
