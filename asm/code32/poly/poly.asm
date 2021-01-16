.486p
.MODEL FLAT
.STACK     
;JUMPS     
LOCALS     
.CODE      
           
b equ byte ptr
w equ word ptr
d equ dword ptr

;° EQU's °°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
 
ScreenHeight = 200
ScreenWidth  = 320
 
AddValue = 64
ShrValue = 2
CutOff = 0
 
;NumPoints EQU NumSize*NumSize*NumSize	 ; Number of points to rotate
NumCoords EQU 130
NumPolys EQU 256
P_Scaler EQU 2048               ; DIV 2048 = shr 11
Z_Plane_Threshold EQU 130
TableSize EQU 256		; Size of SIN/COS table
 
;° DATA °°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
Screen dd ?
         
 db 640 dup(?)
Buffer db ScreenWidth*ScreenHeight*2 dup(?)
                                    
Align 4  
Mul320 Label dWord                  
i=0                                 
Rept 200                            
 dd i*640
 dd 0
 i=i+1                          
ENDM                            
                                
Align 4                         
LineBuf dd ScreenHeight*2 dup(?)
                           
Align 4       
Topy dd ?     
BotY dd ?     
Align 4       
Color db ?    
Align 4       
x1 dd ?       
y1 dd ?       
x2 dd ?       
y2 dd ?       
x3 dd ?       
y3 dd ?                                               
                                                      
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
; eCX= x1, eDX = y1, eAX = x2, eBX = y2               
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
PROC ScanLeft                                         
 cmp edx,ebx    ; is y1=y2 -> Zero height?            
 jne @@SL1      ; nope... continue                    
 ret            ; yes - quit scanning                 
@@SL1:                                                
 jl @@SL2       ; is y1 < y1 (y1 top) - if yes, continue
 xchg ecx,eax   ; else swap all x,y and color 1-2     
 xchg edx,ebx                                         
@@SL2:                                                
 push ecx     	; save X start                        
 mov ebp,ebx    ; bp = y2                             
 shl ebp,3      ;      *8
 cmp ebp,[BotY] ; Set new BotY if Y2*2 > BotY         
 jl @@SL3
 mov [BotY],ebp                                       
@@SL3:                                                
 sub eax,ecx   	; ax = Width    (x2-x1)               
 sub ebx,edx	; bx = height   (y2-y1)               
 mov ebp,edx    ; bp = y1                             
 shl ebp,3      ; bp -> Ypos in Both buffers    (=y1*8)
 cmp ebp,[TopY] ; Set new TopY?                       
 jg @@SL4
 mov [TopY],ebp                                       
@@SL4:                                                
 shl eax,16     ; eax = width -> 16.16                
 cdq            ; edx:eax = eax                       
 idiv ebx     	; eax = Xslope 16.16                  
 mov edi,eax 	; edi = Xslope                        
                                                        
 pop eax    	; X start (prev. push cx)             
 shl eax,16     ; 16.16 - eax = X                     
                                                      
; edi = Xslope, eax = X                               
; ebx = Height, ebp = pos in buffers                  
 add ebp,offset LineBuf  
@@SL5:                                                
 shld edx,eax,16    	; dx = Xpos                   
 mov [ebp],dx                                         
 add ebp,8               
 add eax,edi 	    	; inc x,xslope                
 dec ebx                                              
 jnz @@SL5                                            
 ret                                                  
ENDP                              
                                                                          
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
; eCX= x1, eDX = y1, eAX = x2, eBX = y2
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
PROC ScanRight                         
 cmp edx,ebx    ; is y1=y2 -> Zero height?
 jne @@SR1      ; nope... continue
 ret            ; yes - quit scanning  
@@SR1:    
 jl @@SR2       ; is y1 < y1 (y1 top) - if yes, continue
 xchg ecx,eax   ; else swap all x,y and color 1-2
 xchg edx,ebx                                      
@@SR2:    
 push ecx 	; save X start
 mov ebp,ebx    ; bp = y2     
 shl ebp,3      ;      *8
 cmp ebp,[BotY] ; Set new BotY if Y2*2 > BotY
 jl @@SR3
 mov [BotY],ebp               
@@SR3:                   
 sub eax,ecx   	; ax = Width    (x2-x1)
 sub ebx,edx	; bx = height   (y2-y1)
 mov ebp,edx    ; bp = y1
 shl ebp,3      ; bp -> Ypos in Both buffers    (=y1*8)
 cmp ebp,[TopY] ; Set new TopY?                       
 jg @@SR4
 mov [TopY],ebp                                       
@@SR4:                                                
 shl eax,16     ; eax = width -> 16.16                
 cdq            ; edx:eax = eax                       
 idiv ebx     	; eax = Xslope 16.16                  
 mov edi,eax 	; edi = Xslope                        
 pop eax    	; X start (prev. push cx)             
 shl eax,16     ; 16.16 - eax = X                     
; edi = Xslope, eax = X                               
; ebx = Height, ebp = pos in buffers                  
 add ebp,offset LineBuf+4
@@SR5:                                                
 shld edx,eax,16    	; dx = Xpos                   
 mov [ebp],dx                                         
 add ebp,8
 add eax,edi 	    	; inc x,xslope                
 dec ebx                                              
 jnz @@SR5                                            
 ret                                                  
ENDP                                                  
                                                      
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ   
; Draws Plain polygon                                 
; ABsolutt ikke kjappest mulige versjon... F†r optimere senere...
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ   
PROC FillPoly  
 mov ebx,[TopY]                    
 mov ebp,[BotY]                    
DrawLoop:                          
 mov edi,[Mul320+ebx]
 add edi,[LineBuf+ebx]
 add edi,offset Buffer                  
 mov ecx,[LineBuf+ebx+4]
 sub ecx,[LineBuf+ebx]     ; cx = lengde p† linje
 jz QuitHLine          
 mov al,[color]                    
@@Loop1:                           
 mov [edi],al           ; Mulig † f† denne Inner-Loop'en kjappere?
 inc edi                           
 dec ecx                           
 jnz @@Loop1                       
QuitHLine:                         
 add ebx,8   
 cmp ebx,ebp           
 jl DrawLoop           
 ret                   
ENDP                               
                
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
PROC PlainPoly
 mov [TopY],1600
 mov [BotY],0                      
 mov ecx,[x1]   	      	; Linje 1-2
 mov edx,[y1]          
 mov eax,[x2]          
 mov ebx,[y2]    
 cmp edx,ebx            
 jb @@1               
 Call ScanLeft
 jmp @@2              
@@1:                  
 Call ScanRight
@@2:                  
 mov ecx,[x2] 	      	; Linje 2-3
 mov edx,[y2]                       
 mov eax,[x3]                       
 mov ebx,[y3]                       
 cmp edx,ebx                         
 jb @@3                            
 Call ScanLeft
 jmp @@4                           
@@3:                               
 Call ScanRight
@@4:                               
 mov ecx,[x1] 	      	; Linje 1-3
 mov edx,[y1]                      
 mov eax,[x3]                      
 mov ebx,[y3]                      
 cmp edx,ebx                       
 jb @@5                            
 Call ScanRight                    
 jmp @@6                           
@@5:                               
 Call ScanLeft                     
@@6:                               
 Call FillPoly                     
 ret                               
ENDP                               
                                   
PROC CopyBuffer
 mov esi,offset buffer
 mov edi,[Screen]
 mov ecx,200
@@1:        
 i=0        
 rept 80       
  mov eax,[esi+i]
  mov [edi+i],eax
  i=i+4     
 endm
 add esi,640
 add edi,320
 dec ecx
 jnz @@1
 ret
ENDP 
           
PROC BenchMark
 mov [x1],10  
 mov [y1],10  
 mov [x2],20
 mov [y2],20 
 mov [x3],10
 mov [y3],20
 mov [color],15                    
 mov ecx,1000000
BenchLoop:  
 push ecx   
 Call PlainPoly
 pop ecx
 dec ecx    
 jnz BenchLoop
 ret        
ENDP        
  
PROC Visible
 mov ax,13h   
 int 10h      
 mov [x1],10
 mov [y1],10  
 mov [x2],310
 mov [y2],100
 mov [x3],150
 mov [y3],190
 mov [color],15                    
 Call PlainPoly
 Call CopyBuffer
Waitesc:      
 in al,60h    
 cmp al,1     
 jne Waitesc  
 mov ax,3   
 int 10h   
 ret       
ENDP       

            
;° EntryPoint from DOS °°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
EntryPoint:   
 mov eax,0EE02h
 int 31h      
 mov edi,0A0000h
 sub edi,ebx  
 mov [Screen],edi
 Call Visible
; Call BenchMark
 mov ax,4c00h 
 int 21h      
End EntryPoint
