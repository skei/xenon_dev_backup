.486p
.MODEL FLAT
.STACK     
LOCALS
.CODE

;° DATA °°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
Screen dd ?
Texture label byte
 include texture2.db
Sinus label byte
 include sin.dw
Cosinus label
 include cos.dw
                    
;° CODE °°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
                    
PROC DrawWobble  
; EAX = temp      
; EBX = Xint:XFrac      YInt:YFrac (inner)
; ECX = inner-adder
; EDX = outer-adder                                  
; ESI = Texture index
; EDI = Screen-pos 
; EBP = Counter
 mov edi,[Screen]
 mov edx,100
Outer:      
 push ebx   
i=0            
rept 160       
 xor eax,eax   
 shld eax,ebx,8
 add eax,eax             
 movsx eax,[sinus+eax]     ; al = sinus(sinusx)
 add esi,eax
 mov ah,al                                 
 mov [edi+i*2],ax
 mov edi+320+i*2],ax
 i=i+2              
 add ebx,ecx            ; inc sinusx,esi
endm                
 pop ebx
 add ebx,edx   
 add edi,640   
 dec ebp
 jnz Outer
 ret
ENDP 
               
                                        
                                        

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
