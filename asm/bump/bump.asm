;컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
;Register Usage
;컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
;EAX                     Temp inner
;EBX                     Temp inner      
;ECX     Xint:Xfrac      Yint:Yfrac      Bump texture
;EDX     xint:xfrax      yint:yfrac      Phong texture
;ESI                               
;EDI                     Screen pos
;EBP                     Linelength
;ESP                               
;컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
                                   
                                   
                                   
컴컴컴컴컴컴컴컴컴컴컴컴           
Outer:                             
 mov ecx,12345678h      ; Bump index
BumpIndex = $-4                    
 mov edx,12345678h      ; Phong index
PhongIndex = $-4                   
 shld edi,esi,16                   
                                   
컴컴컴컴컴컴컴컴컴컴컴컴           
; ECX = Bump                       
; EDX = Phong                      
; DI = YPos*320 + Right-X !        
; BP = NEGATIVE line-length        
Inner:                             
 shld ebx,ecx,8         ; BL = Bump X integer
 mov bh,ch              ; BH = Bump Y integer : BX -> Bump Texture     
 add ecx,12345678h      ; Update Bump-indexes    
 mov al,[fs:bx]         ; AL = X adder to Phong index
 mov ah,[gs:bx]         ; AH = Y adder to Phong : AX = Phong-adder 
 shld ebx,edx,8         ; BL = Phong X integer
 mov bh,dh              ; BH = Phong Y integer : BX -> Phong texture 
 add edx,12345678h      ; Update Phong-indexes
 add bx,ax              ; Add Bump adder to Phong texture position
 mov al,[ds:bx]         ; AL = Pixel from [Phong+Bump] :)
 mov [es:di+bp],al      ; Put pixel to screen/buffer
 inc bp                            
 jnz Inner                         
컴컴컴컴컴컴컴컴컴컴컴컴
 