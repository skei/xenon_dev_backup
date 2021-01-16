;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
; Protected mode version of Texture Mapping  Axon/XD
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
                                                    
                                                    
.486p                                               
.MODEL FLAT                                         
.STACK                                              
LOCALS                                              
.CODE                                               
                                                    
;° DATA °°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
Screen dd ?                                         
Texture label byte                                  
 include texture.db                                 
                                                    
;° CODE °°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
                                                    
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
; EAX   Polyheight      AL = temp
; EBX   temp                                        
; ECX   XInt:XFrac      YInt:YFrac                  
; EDX   LeftFrac        RightInt (incl y*320)       
; ESI   RightFrac       LeftInt (incl y*320)        
; EDI   ScreenPos                                   
; EBP                   LineLength                  
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
Outer:
 mov ebp,edx
 sub ebp,esi    ; BP = Right-Left = Length          
 sbb ebp,0      ; Eller adc?                        
 push ecx                                           
Inner:                                              
 shld ebx,ecx,8                                     
 mov bh,ch                                          
 add ecx,12345678h      ; .. 78 56 34 12            
TxAdder = $-2                                       
TyAdder = $-4                                       
 mov al,[Texture+bx]                                
 mov [edi],al                                       
 inc edi                                            
 dec bp                                             
 jnz Inner                                          
;                                                   
 pop ecx                                            
 add ecx,12345678h      ; .. 78 56 34 12            
LeftTxAdder = $-2                                   
LeftTyAdder = $-4                                   
 add edx,12340000h      ; .. 00 00 34 12            
LeftFrac = $-2                                      
 adc esi,12345678h      ; .. 78 56 34 12            
LeftInt = $-4                                       
RightFrac = $-2                                     
 adc dx,1234h                                       
RightInt = $-2                                      
 sub eax,10000h                                     
 jnz Outer                                          
                                                    
                                    
;° EntryPoint from DOS °°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
EntryPoint:                         
 mov eax,0EE02h                     
 int 31h              
 mov edi,0A0000h      
 sub edi,ebx          
 mov [Screen],edi     
                      
 mov ax,4c00h
 int 21h
End EntryPoint
