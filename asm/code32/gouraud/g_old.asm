.486p
.MODEL FLAT
.STACK     
;JUMPS     
LOCALS     
.CODE      
           
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
           
Label Buffer Byte
 db ScreenWidth*ScreenHeight dup(?)

ALign 4
Mul320 Label DWord
i=0               
 Rept 200         
  DW i*320
  i=i+1   
 ENDM     
          
Align 4
LBuf dw ScreenHeight dup(?)
Align 4
RBuf dw ScreenHeight dup(?)
                           
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
; eCX= x1, eDX = y1, eAX = x2, eBX = y2 (all must be clipped and positive)
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
PROC ScanGouraudLeft          
 cmp edx,ebx    ; is y1=y2 -> Zero height?
 jne @@SGL1     ; nope... continue
 ret            ; yes - quit scanning
@@SGL1:                                 
 jl @@SGL2      ; is y1 < y1 (y1 top) - if yes, continue
 xchg ecx,eax   ; else swap all x,y and color 1-2
 xchg edx,ebx                                      
@@SGL2:                          
 push ecx	; save X start
 mov ebp,ebx    ; bp = y2
 add ebp,ebp    ;      *2
 cmp ebp,[BotY] ; Set new BotY if Y2*2 > BotY
 jb @@SGL3          
 mov [BotY],ebp      
@@SGL3:             
 sub eax,ecx   	; ax = Width    (x2-x1)
 sub ebx,edx	; bx = height   (y2-y1)
 mov ebp,edx    ; bp = y1
 add ebp,ebp	; bp -> Ypos in Both buffers    (=y1*2)
 cmp ebp,[TopY] ; Set new TopY?
 ja @@SGL4                             
 mov [TopY],ebp                         
@@SGL4:                                
 shl eax,16     ; eax = width -> 16.16
 cdq            ; edx:eax = eax
 idiv ebx     	; eax = Xslope 16.16
 mov edi,eax 	; edi = Xslope 
                                      
 pop ecx    	; restore color start (previous push di)
 shl ecx,16     ; ecx = 16.16 color
 pop eax    	; X start (prev. push cx)
 shl eax,16     ; 16.16 - eax = X
                
; edi = Xslope, eax = X
; ebx = Height, ebp = pos in buffers
 add ebp,offset LBuf
@@SGL5:                           
 shld edx,eax,16    	; dx = Xpos
 mov [ebp],dx    
 add ebp,2       
 add eax,edi 	    	; inc x,xslope        
 dec ebx         
 jnz @@SGL5                                   
 ret                                          
ENDP                              
                                                                          
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
; eCX= x1, eDX = y1, eAX = x2, eBX = y2 (all must be clipped and positive)
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
PROC ScanGouraudRight
 cmp edx,ebx    ; is y1=y2 -> Zero height?
 jne @@SGR1     ; nope... continue
 ret            ; yes - quit scanning
@@SGR1:                                 
 jl @@SGR2      ; is y1 < y1 (y1 top) - if yes, continue
 xchg ecx,eax   ; else swap all x,y and color 1-2
 xchg edx,ebx                                      
@@SGR2:                          
 push ecx	; save X start
 mov ebp,ebx    ; bp = y2
 add ebp,ebp    ;      *2
 cmp ebp,[BotY] ; Set new BotY if Y2*2 > BotY
 jb @@SGR3          
 mov [BotY],ebp      
@@SGR3:             
 sub eax,ecx   	; ax = Width    (x2-x1)
 sub ebx,edx	; bx = height   (y2-y1)
 mov ebp,edx    ; bp = y1
 add ebp,ebp	; bp -> Ypos in Both buffers    (=y1*2)
 cmp ebp,[TopY] ; Set new TopY?
 ja @@SGR4                             
 mov [TopY],ebp                         
@@SGR4:                                
 shl eax,16     ; eax = width -> 16.16
 cdq            ; edx:eax = eax
 idiv ebx     	; eax = Xslope 16.16
 mov edi,eax 	; edi = Xslope 
                                      
 pop ecx    	; restore color start (previous push di)
 shl ecx,16     ; ecx = 16.16 color
 pop eax    	; X start (prev. push cx)
 shl eax,16     ; 16.16 - eax = X
                   
; edi = Xslope, eax = X
; ebx = Height, ebp = pos in buffers
 add ebp,offset RBuf
@@SGR5:                           
 shld edx,eax,16    	; dx = Xpos
 mov [ebp],dx
 add ebp,2         
 add eax,edi 	    	; inc x,xslope        
 dec ebx           
 jnz @@SGR5                                   
 ret                                          
ENDP                              
                   
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ   
; Draws Gouraud shaded polygon
; ABsolutt ikke kjappest mulige versjon... F†r optimere senere...
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ   
PROC FillGouraud 
 mov ebx,[TopY]
 mov ebp,[BotY]  
DrawLoop:        
 Mov edi,[Mul320+ebx]
 add di,[LBuf+ebx]
 add edi,[Screen]
 movsx ecx,[RBuf+ebx]
 sub cx,[LBuf+ebx]     ; cx = lengde p† linje
 jz QuitGHLine          
 mov al,[color]         
@@Loop1:                
 mov [edi],al           ; Mulig † f† denne Inner-Loop'en kjappere?
 inc edi
 dec ecx        
 jnz @@Loop1     
QuitGHLine:      
 add ebx,2
 cmp ebx,ebp       
 jl DrawLoop     
 ret             
ENDP      
                
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
PROC GouraudPoly
 mov [TopY],400 
 mov [BotY],0                      
 mov ecx,[x1]   	      	; Linje 1-2
 mov edx,[y1]          
 mov eax,[x2]          
 mov ebx,[y2]    
 cmp edx,ebx            
 jb @@1               
 Call ScanGouraudLeft
 jmp @@2              
@@1:                  
 Call ScanGouraudRight
@@2:                  
 mov ecx,[x2]	      	; Linje 2-3
 mov edx,[y2]                       
 mov eax,[x3]                       
 mov ebx,[y3]                       
 cmp edx,ebx                         
 jb @@3                            
 Call ScanGouraudLeft
 jmp @@4                           
@@3:                               
 Call ScanGouraudRight
@@4:                               
 mov ecx,[x1]	      	; Linje 1-3
 mov edx,[y1]          
 mov eax,[x3]          
 mov ebx,[y3]          
 cmp edx,ebx            
 jb @@5     
 Call ScanGouraudRight
 jmp @@6    
@@5:                  
 Call ScanGouraudLeft
@@6:                  
 Call FillGouraud
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
