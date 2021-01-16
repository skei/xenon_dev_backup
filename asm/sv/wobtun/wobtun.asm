;� DATA 같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
       
WobTunTexture label byte
 include e:\intro3\wobtun\texture.db
                  
WobTunPal label byte
 i=0
 rept 64
  db 0,0,i
  i=i+1
 endm
 db 768 dup(0)

OffsetData label word
 include e:\intro3\wobtun\tabell.db;offsets.db
                          
WobbleTable label byte    
 db 256 dup(0)            
                          
Sinus label byte          
 include e:\intro3\wobtun\sin.db         ; 0..32 - 256 entries           
                          
; 8.8 fixed point variables
SinusStart dw ?           
SinusAdder dw ?           
; Y/X - Zoom/Rot          
RotZoom    dw ?           
a dw 0                    
b dw 0
ofset dd 0
                   
;� CODE 같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
PROC MakeWobbleTable                         
 mov ax,[SinusStart]            ; 8.8 fixed point start
 mov cx,[SinusAdder]            ; 8.8 adder
 mov edi,offset WobbleTable
 mov edx,256 
@@1:        
 movzx ebx,ah
 mov bl,[Sinus+ebx]       
 mov [edi],bl
 inc edi
 add ax,cx
 dec edx           
 jnz @@1           
 ret                                         
ENDP                                         
                                             
PROC DrawTunnel                              
 mov dx,[RotZoom]
 mov esi,offset OffsetData                   
 mov edi,[Screen]   
 add edi,[ofset]    
 mov ecx,100
Outer1:                                      
 rept 160                                    
  mov bx,[esi]                  ;; AX = GetPixel-offset in texture
  add esi,2                                  
  add bx,dx                     ;; Add X-offset/Rotation
  movzx ebp,bh                  ;; bl = y
  add bh,[WobbleTable+ebp]      ;; Add wobble data to Y
  mov al,[WobTunTexture+bx]     
;;  mov ah,al               
  mov [edi],al;ax
;;  mov [edi+320],ax 
  add edi,2        
 endm              
 add edi,320       
 dec ecx           
 jnz Outer1        
 ret    
ENDP 
            
PROC DoWobTun
 Call MakeWobbleTable  
 Call WaitVR  
 Call DrawTunnel       
                     
 add [RotZoom],0001h
 add [SinusStart],0200h
 add [a],350
 add [b],512
                     
 xor ebx,ebx         
 mov bl,byte ptr [a+1]        
 mov al,[Sinus+ebx]  
                     
 xor ebx,ebx         
 mov bl,byte ptr [b+1]        
 add al,[Sinus+ebx]
                   
 cbw               
 sar ax,2
 add [SinusAdder],ax
; Update Screen-pos'es
 cmp [ofset],0      
 jne @@1            
 mov [ofset],1     
 jmp @@4            
@@1:                 
 cmp [ofset],1     
 jne @@2           
 mov [ofset],320
 jmp @@4         
@@2:             
 cmp [ofset],320
 jne @@3         
 mov [ofset],321                 
 jmp @@4         
@@3:             
 cmp [ofset],321
 jne @@4         
 mov [ofset],0                 
@@4:          
 ret
ENDP    
