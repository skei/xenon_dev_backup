;DOSSEG
.MODEL MEDIUM
.386
.STACK
;JUMPS
LOCALS

;° DATA °°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
.DATA
              
;° CODE °°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
.CODE
          
; Register Usage                       
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
; EAX   Same as ECX, but for Outer (Left)
; EBX   Temp - Texture position (inner)
; ECX   XInt:XFrac      YInt:YFrac
; EDX   RightFrac       RightInt
; ESI   Xint:Xfrac      YInt:Yfrac (adder inner)
; EDI                   Screen-pos                  
; EBP   LeftFrac        LeftInt / LineLength (inner)
; ESP                                 
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
Outer:                                
 push bp                ; Save LeftInt
 mov di,bp                  ; DI = Left                           
 sub bp,dx                  ; BP = Left-Right = -Length           
 jns short SkipLine         ; Hvis ikke negativ, skip it...       
 sub di,bp                  ; DI = DI - BP = Left + Length = Right
 mov ecx,eax
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
Inner:                                
 mov bl,ch                            
 shld ebx,ecx,8         ; ecx -> Xint:XFrac : YInt:YFrac
 mov bl,[bx]                                            
 add ecx,esi                                            
 mov [es:di+bp],bl                                      
 inc bp                                                 
 jnz Inner                                              
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ                      
 pop bp                                  
 add eax,12345678h ; .. 78 56 34 12      
TXAdder = $-2                            
TYAdder = $-4                            
 add ebp,12345678h ; .. 78 56 34 12      
LeftFrac = $-2                           
LeftInt = $-4                            
 adc edx,12345678h                       
RightFrac = $-2                          
RightInt = $-4                           
 adc ebp,0                                  
 dec [PolyHeight]                        
 jnz Outer                               
                                         
                            
;° EntryPoint from DOS °°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
EntryPoint:
 mov ax,@data
 mov ds,ax                      
      
 mov ax,4c00h
 int 21h
End EntryPoint
