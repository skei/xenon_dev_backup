; Turbo Profiler resultater:
; TmTri = 31%   
; CopyBuffer = 19%
; Rotate = 11%  
; CheckVisible = 7%
; SortSetup = 6%
; S, det er nok mer  hente her og der...

; Fjern mest mulig av index (*2 *4 etc) fra alle rutiner - ihvertfall
; sortering og projektering...
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
; Environment Mapping - Axon - 21.06.95 -> 10.07.95 (optimering)
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
                    
DOSSEG        
.MODEL MEDIUM 
.386          
.STACK        
;JUMPS        
LOCALS        
                 
;° EQU's °°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
                                                                       
b EQU byte ptr
w EQU word ptr
d EQU dword ptr 
              
divd = 14       ; 16-bits fixed point divider                          
                     
;° Segments °°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
                      
Segment BufferSeg
Label buffe
 db 64000 dup(0)
ends       
           
Segment TextureSeg
Label Textur  
 include tex1.db
; db 65536 dup(0)
ends          
                             
Segment treDsegment          
 nc1 dw 416             ; Antall koordinater
 np1 dw 832             ; antall polygons
 include e:\axon\asc\sjakk1.inc
ENDS                                                       
                
;° DATA °°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
SEGMENT DATA PARA USE16
                      
treDseg dw ?        
                    
Palette label byte
 include tex1pal.db
 ;i = 0
;rept 32      
; db i*2,0,0  
; i=i+1 
;endm   
;rept 256
; db 0,0,0
;endm   
                 
MaxPolys = 2624
MaxCoords = 1424
                 
AddToXOff dw ?   
AddToYOff dw ?   
AddtoZoff dw ?   
NumCounter dw ?  
                 
UseBlur dw ?     
saved_di dw ?                
                   
Texture dw ?
Buffer dw ?   

ALIGN 4
Xoff dd 160      ; Variables for 3D->2D transformation
ALIGN 4
Yoff dd 100
ALIGN 4
zoff dd 700
                
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
; 3D variables  
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
ALIGN 4                     
NumCoords dw ?
ALIGN 4
NumPolys dw ?    
; OFFSETS into 3D segment...
Coords dw ?                 
VertexNormals dw ?          
Polygon dw ?     
                            
Rotated label word    
 dw MaxCoords*4 dup(?)
RotatedVN label word 
 dw Maxcoords*4 dup(?)
; Sine  Cosine tables. 1024 entries. Each ranging from -16k to 16k
                 
include sinus.dw 
; Roterings-Vinkler   
VX dw ?                   
VY dw ?       
VZ dw ?             
VxAdd dw ?                  
VyAdd dw ?    
VzAdd dw ?    
Temp dd ?               ; Temporary variable for Rotate routine             
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
; Texture Mapping variables
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
TriCoords dd MaxCoords dup (?)  ; y/x
TexCoords dd MaxCoords*2 dup(?) ;Trenger vel ikke *2 ????????
                                 
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
; Variabler til Sortering...     
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
face_z dw MaxPolys DUP (?)   
faceind dw MaxPolys DUP (?)      
                                 
ENDS DATA   
                          
;° CODE°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
SEGMENT CODE PARA USE16
                                 

ASSUME CS:CODE,DS:DATA

setborder2 MACRO color 
 push    ax            
 push    dx            
 cli                   
 mov     dx,3dah       
 in      al,dx         
 mov     dx,3c0h       
 mov     al,11h+32     
 out     dx,al         
 mov     al,color      
 out     dx,al         
 sti                   
 pop     dx            
 pop     ax               
ENDM                         
setborder MACRO color       
 setborder2 color      
ENDM                   
                       
PROC CalcTex                       
 cld        
 mov ax,[Texture]
 mov es,ax  
 xor di,di  
; Frst blank buffer, s vi er sikker :)
 mov eax,0  
 mov cx,16000
 rep stosd  
 xor di,di  
@@fory_loop:
 mov si,0   
@@forx_loop:
 mov ax,si     
 sub ax,64                
 imul ax,ax 
 mov bx,di  
 sub bx,64  
 imul bx,bx 
 add ax,bx  
 shr ax,7   
 mov cx,31                  
 sub cx,ax  
 cmp cx,5   
; cmp cx,32
; jb @@1      
; mov cx,0
@@1:        
 mov bx,di  
 shl bx,8   
 add bx,si  
 mov [es:bx],cl
 inc si    
 cmp si,127 
 jb @@forx_loop           
 mov si,0
 inc di
 cmp di,127
 jb @@fory_loop
 ret
ENDP 
           
PROC DoObject1              
 mov ax,treDsegment    
 mov gs,ax             
 mov [treDseg],ax      
 mov ax,OFFSET Coords1 
 mov [Coords],ax       
 mov ax,OFFSET VertexNormals1
 mov [VerTexNormals],ax   
 mov ax,OFFSET Polygon1   
 mov [Polygon],ax         
 mov ax,[gs:nc1]          
 mov [NumCoords],ax       
 mov ax,[gs:np1]                                           
 mov [NumPolys],ax        
                             
 mov [vx],256                
 mov [vy],0                  
 mov [vz],0                  
 mov [vxadd],7               
 mov [vyadd],8               
 mov [vzadd],9               
 mov [Xoff],160              
 mov [yoff],100              
 mov [zoff],500
 mov [AddToXoff],0  
 mov [AddToYOff],0  
 mov [AddToZoff],0  
 mov [NumCounter],250
 mov [UseBlur],0    
 Call EnvMap              
 ret                      
ENDP                      
                          
PROC WaitVR   
 mov dx,3DAh  
@@1:               
 in al,dx          
 test al,8         
 jnz @@1             
@@2:                
 in al,dx     
 test al,8     
 jz @@2       
 ret          
ENDP          
           
PROC ClearScreen
 mov ax,0a000h
 mov es,ax 
 xor di,di    
 mov cx,16000                                              
 mov eax,0 
 rep stosd 
 ret       
ENDP       
               
PROC SetPalette         
 push ds                
 mov dx,3c8h         
 xor al,al              
 out dx,al     
 inc dx        
 mov cx,768    
 mov ax,SEG Palette
 mov ds,ax     
 mov si,OFFSET Palette
 rep outsb     
 pop ds        
 ret                
ENDP                   
                       
PROC ShellSort
 Xor esi,esi   
 xor edi,edi   
 mov bp,[CS:NumToDraw]
 dec bp        
ShellLoop:
 cmp bp,1      
 jl QuitShell
 shr bp,1         
RepeatLoop:
 mov cx,1                
 mov di,0  
 mov dx,[CS:NumToDraw]   
 dec dx    
 sub dx,bp               
ForLoop:
 mov si,di               
 add si,bp
 mov ax,[DS:face_z+edi*2]
 cmp ax,[DS:face_z+esi*2] 
 jle DontSwap          
 xchg ax,[DS:face_z+esi*2]
 mov [DS:face_z+edi*2],ax  
 mov ax,[DS:faceInd+edi*2]
 xchg ax,[DS:faceInd+esi*2]
 mov [DS:faceInd+edi*2],ax  
 mov cx,0            
DontSwap:            
 inc di              
 cmp di,dx           
 jle ForLoop
 cmp cx,0            
 jz RepeatLoop
 jmp ShellLoop
QuitShell:
 ret                 
ENDP                 
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
PROC BlurCopyBuffer
 mov si,[saved_di] 
 push ds           
 mov ax,[Buffer]
 mov ds,ax         
 mov ax,0a000h     
 mov es,ax     
 mov cx,100        
ALIGN 16
CopyLoop:          
 rept 160          
  mov al,[DS:si]
  mov [ES:si],al
  inc si       
  inc si       
 endm              
 add si,320        
 dec cx            
 jnz CopyLoop      
 pop ds            
               
 cmp [saved_di],0
 jne @@1       
 mov [saved_di],1
 jmp @@4     
@@1:             
 cmp [saved_di],1
 jne @@2         
 mov [saved_di],320
 jmp @@4 
@@2:               
 cmp [saved_di],320
 jne @@3           
 mov [saved_di],321
 jmp @@4 
@@3:               
 cmp [saved_di],321
 jne @@3           
 mov [saved_di],0
@@4:     
 ret               
ENDP               
                   
PROC CopyBuffer    
 push ds          
 mov ax,[Buffer]
 mov ds,ax        
 xor si,si        
 mov ax,0a000h    
 mov es,ax     
 xor di,di     
 mov cx,16000     
ALIGN 16
NCopyLoop:     
 mov eax,[ds:si]
 mov [es:si],eax
 add si,4
 dec cx        
 jnz NCopyLoop 
               
 pop ds        
 ret           
ENDP           
               
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
        
PROC ClearBuffer
 mov ax,[Buffer]
 mov es,ax
 xor di,di  
 xor eax,eax
 mov cx,16000
 rep stosd
 ret    
ENDP      
            
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
PROC TmTri
 mov eax,d [DS:TriCoords+si]
 mov ebx,d [DS:TriCoords+di]
 mov ecx,d [DS:TriCoords+bp]
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
; Sjekk om X'ene er innenfor 320
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
 mov edx,01400000h              
 cmp edx,eax                                
 sbb dx,dx                                  
 cmp edx,ebx                                
 sbb dx,0                                   
 cmp edx,ecx                                
 sbb dx,0
 mov [CS:NeedClip],dx
 cmp dx,-3      ; Alle X'ene er over 320, s,,QUIT
 je QuitPoly_noDS               
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
; Sjekk om vi trenger  clippe Y
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
 mov dx,200         
 cmp dx,ax          
 sbb [CS:NeedClip],0
 cmp dx,bx          
 sbb [CS:NeedClip],0
 cmp dx,cx                      
 sbb [CS:NeedClip],0            
; Sortr                        
 cmp ax,bx                      
 jl @@1                                            
 xchg eax,ebx                   
 xchg si,di                     
@@1:                               
 cmp ax,cx                      
 jl @@2                         
 xchg eax,ecx                   
 xchg si,bp       
@@2:                    
 cmp bx,cx        
 jl @@3           
 xchg ebx,ecx     
 xchg di,bp       
@@3:              
; Y-ene i riktig rekkeflge... N sjekk X'er
 cmp ax,bx       ;Y1=Y2 ? Isfall, sjekk om X'ene m swappes
 jne @@4         
 cmp eax,ebx                    
 jl @@4          
 xchg eax,ebx    
 xchg si,di              
@@4:             
 cmp bx,cx       ;Y2=Y3 ? Isfall, sjekk om X'ene m swappes
 jne @@5               
 cmp ebx,ecx     
 jl @@5              
 xchg ebx,ecx    
 xchg di,bp      
@@5:             
; 1     1     1     1 2
;  2   2     2 3     3  
;3       3       
; N burde de vre sortert etter Y...
 cmp ax,199       
 jg QuitPoly_noDS
 cmp cx,0        
 jl QuitPoly_noDS
 mov d [CS:y1],eax              
 mov d [CS:y2],ebx                            
 mov d [CS:y3],ecx            
 mov eax,d [DS:TexCoords+si]
 mov ebx,d [DS:TexCoords+di]
 mov ecx,d [DS:TexCoords+bp]
 mov d [CS:ty1],eax  
 mov d [CS:ty2],ebx            
 mov d [CS:ty3],ecx  
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
; Heretter brukes bare variabler som ligger i Code-segmentet, s vi
; kopierer CS til DS, og fortsetter som normalt... 
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
 push ds        ; for resetting ver rutine-slutt
 push cs         
 pop ds          
; Utregning av sloper..
 mov [Parts],0                         
; Slope13                              
 mov eax,d [y3] 
 sub eax,d [y1]                 
 xor ebx,ebx                                 
 mov [y3_minus_y1],ax   
 test ax,ax             
 jz QuitPoly            
 mov bx,ax              
 cdq                    
 idiv ebx               
 mov [slope13],eax                                 
; Slope12                      
 mov eax,d [y2]                      
 sub eax,d [y1]
 xor ebx,ebx            
 mov [y2_minus_y1],ax   
 test ax,ax             
 jz @@s1                
 or [Parts],1           
 mov bx,ax      
 cdq           
 idiv ebx      
 mov [slope12],eax              
@@s1:          
; Slope23      
 mov eax,d [y3]
 sub eax,d [y2]
 xor ebx,ebx          
 mov [y3_minus_y2],ax
 test ax,ax      
 jz @@s2                                           
 or [Parts],2
 mov bx,ax                      
 cdq             
 idiv ebx        
 mov [slope23],eax      
@@s2:         
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
 cmp [Parts],3          
 jne Skip4      
; X4 = x1 + S13*(y2-y1)
 mov eax,[Slope13]
 xor ebx,ebx                    
 mov bx,[y2_minus_y1]
 imul ebx 
 shld ebp,eax,16
 add bp,[x1]
 cmp bp,[x2]            ; bp = x4
 je QuitPoly
; Tx4 = tx1 + Txs13*(y2-y1)
 mov ax,[tx3]        
 sub ax,[tx1]
 shl ax,8 
 cwd                            
 idiv [y3_minus_y1]
 imul [y2_minus_y1]
 shld si,ax,8           
 add si,[tx1]           ; si = tx4
; Ty4 = ty1 + Tys13*(y2-y1)       
 mov ax,[ty3]                     
 sub ax,[ty1]                     
 shl ax,8                         
 cwd                              
 idiv [y3_minus_y1]               
 imul [y2_minus_y1]               
 shld di,ax,8                     
 add di,[ty1]           ; di = ty4
Skip4:
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
; Frst sjekker vi om vi skal kjre Clip eller NoClip versjonen...
 cmp [CS:NeedClip],0
 jne DoClip  
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
; Nei - ingen clipping trengs - s vi kjrer rutine uten clipping,
; som er raskere...
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
; Jmp DoClip
NoClip:   
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
; Vi sjekker for hvilket, og utfrer relevant polygon-filler rutine
 mov ax,[y2]                    
 cmp ax,[y1]                    
 je ncFlatTop             ; Y1=y2 - Flat Top
 cmp ax,[y3]            ; Y2=y3 - Flat Bottom
 je ncFlatBottom                               
 mov eax,[slope12]              
 cmp eax,[slope13]              
 je QuitPoly                    
 jl ncFlatRight                   
 Jmp ncFLatLeft                   
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
ALIGN 16
ncFlatBottom:                     
; 1                             
;2 3           
; LeftXAdder = Slope12
 mov eax,[Slope12]                     
 rol eax,16                                            
 mov w [CS:ncIntLeftAdder2],ax
 xor ax,ax                             
 mov d [CS:ncFracLeftAdder2],eax
; RightXAdder = Slope13 
 mov eax,[Slope13]             
 rol eax,16             
 mov w [CS:ncIntRightAdder2],ax
 xor ax,ax   
 mov d [CS:ncFracRightAdder2],eax
; LeftTxAdder = (tx2-tx1)/(y2-y1) 8.8                  
 mov ax,[tx2]
 sub ax,[tx1]                                          
 shl ax,8       
 cwd         
 idiv [y2_minus_y1]                 
 mov w [CS:ncLeftTxAdder2],ax
; LeftTyAdder = (ty2-ty1)/(y2-y1) 8.8
 mov ax,[ty2]                     
 sub ax,[ty1]                     
 shl ax,8                                              
 cwd                                
 idiv [y2_minus_y1]                 
 mov w [CS:ncLeftTyAdder2],ax
; TxAdder = (tx3-tx2)/(x3-x2)     
 mov ax,[tx3]                     
 sub ax,[tx2]                   
 shl ax,8                       
 cwd                            
 mov bx,[x3]                    
 sub bx,[x2]                    
 jz QuitPoly                    
 idiv bx                        
 mov w [cs:ncTxAdder2],ax
; TyAdder = (ty3-ty2)/(x3-x2)   
 mov ax,[ty3]                   
 sub ax,[ty2]                   
 shl ax,8                       
 cwd                            
 idiv bx                        
 mov w [cs:ncTyAdder2],ax
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
; Det var konstantene, n kommer registrene... phew...
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
; EAX = 0 : RightX       
 xor eax,eax                   
 mov ax,[x1]             
 inc ax                      
; EDX = 0 : Ypos
 xor edx,edx    
 mov dx,[y1]    
; ECX = TyPos:0 | TxPos:0
 mov cx,[Ty1]            
 shl ecx,16     
 mov cx,[Tx1]
 shl ecx,8   
; ESI = 0 : LeftX      
 xor esi,esi            
 mov si,[x1]            
; EBP = Height : -      
 mov bp,[y3_minus_y1]
 shl ebp,16             
                         
 JMP ncFiller2                    
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
ALIGN 16
ncFlatTop:                        
;1 2         
; 3          
; LeftXAdder = Slope13
 mov eax,[Slope13]
 rol eax,16                                            
 mov w [CS:ncIntLeftAdder2],ax
 xor ax,ax                             
 mov d [CS:ncFracLeftAdder2],eax
; RightXAdder = Slope23
 mov eax,[Slope23]
 rol eax,16             
 mov w [CS:ncIntRightAdder2],ax
 xor ax,ax      
 mov d [CS:ncFracRightAdder2],eax
; LeftTxAdder = (tx3-tx1)/(y3-y1) 8.8
 mov ax,[tx3]
 sub ax,[tx1]                                          
 shl ax,8       
 cwd            
 idiv [y3_minus_y1]
 mov w [CS:ncLeftTxAdder2],ax
; LeftTyAdder = (ty3-ty1)/(y3-y1) 8.8
 mov ax,[ty3]
 sub ax,[ty1]                     
 shl ax,8                                              
 cwd                                
 idiv [y3_minus_y1]
 mov w [CS:ncLeftTyAdder2],ax
; TxAdder = (tx2-tx1)/(x2-x1)
 mov ax,[tx2]  
 sub ax,[tx1]  
 shl ax,8                       
 cwd                            
 mov bx,[x2]   
 sub bx,[x1]            
 jz QuitPoly                    
 idiv bx                        
 mov w [cs:ncTxAdder2],ax
; TyAdder = (ty2-ty1)/(x2-x1)
 mov ax,[ty2]
 sub ax,[ty1]
 shl ax,8                       
 cwd                            
 idiv bx                        
 mov w [cs:ncTyAdder2],ax
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
; Det var konstantene, n kommer registrene... phew...
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
; EAX = 0 : RightX     
 xor eax,eax                   
 mov ax,[x2] 
 inc ax      
; EDX = - : Ypos
 xor edx,edx 
 mov dx,[y1] 
; ECX = TyPos:0 | TxPos:0
 mov cx,[Ty1]
 shl ecx,16  
 mov cx,[Tx1]
 shl ecx,8   
; ESI = 0 : LeftX       
 xor esi,esi            
 mov si,[x1]            
; EBP = Height : -      
 mov bp,[y3_minus_y1]   
 shl ebp,16             
 JMP ncFiller2                    
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
ALIGN 16
ncFlatLeft:                       
;  1                    
;(4)2                   
; 3                       
; LeftXAdder 1&2 = Slope13
 mov eax,[Slope13]      
 rol eax,16                                            
 mov w [CS:ncIntLeftAdder1],ax
 mov w [CS:ncIntLeftAdder2],ax 
 xor ax,ax                             
 mov d [CS:ncFracLeftAdder1],eax
 mov d [CS:ncFracLeftAdder2],eax 
; RightXAdder 1 = Slope12 
 mov eax,[Slope12]
 rol eax,16               
 mov w [CS:ncIntRightAdder1],ax
 xor ax,ax                
 mov d [CS:ncFracRightAdder1],eax
; RightXAdder 2 = Slope23
 mov eax,[Slope23]      
 rol eax,16               
 mov w [CS:ncIntRightAdder2],ax
 xor ax,ax                
 mov d [CS:ncFracRightAdder2],eax
; LeftTxAdder = (tx3-tx1)/(y3-y1) 8.8
 mov ax,[tx3]     
 sub ax,[tx1]     
 shl ax,8                    
 cwd                       
 idiv [y3_minus_y1]
 mov w [CS:ncLeftTxAdder1],ax
 mov w [CS:ncLeftTxAdder2],ax
; LeftTyAdder = (ty3-ty1)/(y3-y1) 8.8
 mov ax,[ty3]                
 sub ax,[ty1]                
 shl ax,8                                              
 cwd                                
 idiv [y3_minus_y1]     
 mov w [CS:ncLeftTyAdder1],ax
 mov w [CS:ncLeftTyAdder2],ax 
; TxAdder = (tx2-tx4)/(x2-x4)
 mov ax,[tx2]           
 sub ax,si              
 shl ax,8                       
 cwd                            
 mov bx,[x2]            
 sub bx,bp              
 jz QuitPoly                    
 idiv bx                        
 mov w [cs:ncTxAdder1],ax 
 mov w [cs:ncTxAdder2],ax 
; TyAdder = (ty2-ty4)/(x2-x4)
 mov ax,[ty2]           
 sub ax,di                   
 shl ax,8                       
 cwd                            
 idiv bx                        
 mov w [cs:ncTyAdder1],ax 
 mov w [cs:ncTyAdder2],ax 
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
; Det var konstantene, n kommer registrene... phew...
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
; EAX = 0 : RightX      
 xor eax,eax                   
 mov ax,[x1]            
 inc ax                      
; EDX = - : Ypos
 xor edx,edx    
 mov dx,[y1]    
; ECX = TyPos:0 | TxPos:0
 mov cx,[Ty1]            
 shl ecx,16  
 mov cx,[Tx1]
 shl ecx,8   
; ESI = 0 : LeftX      
 xor esi,esi 
 mov si,[x1]            
; EBP = Height : -      
 mov bp,[y2_minus_y1]   
 shl ebp,16             
 JMP ncFiller1            
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
ALIGN 16
ncFlatRight:                      
; 1                     
;2(4)                   
;  3                       
; LeftXAdder 1 = Slope12
 mov eax,[Slope12]      
 rol eax,16                                            
 mov w [CS:ncIntLeftAdder1],ax
 xor ax,ax                             
 mov d [CS:ncFracLeftAdder1],eax
; LeftXAdder 2 = Slope23
 mov eax,[Slope23]      
 rol eax,16                                            
 mov w [CS:ncIntLeftAdder2],ax
 xor ax,ax                             
 mov d [CS:ncFracLeftAdder2],eax
; RightXAdder = Slope13 
 mov eax,[Slope13]      
 rol eax,16               
 mov w [CS:ncIntRightAdder1],ax
 mov w [CS:ncIntRightAdder2],ax
 xor ax,ax                
 mov d [CS:ncFracRightAdder1],eax
 mov d [CS:ncFracRightAdder2],eax
; LeftTxAdder 1 = (tx2-tx1)/(y2-y1) 8.8
 mov ax,[tx2]           
 sub ax,[tx1]                  
 shl ax,8                    
 cwd                       
 idiv [y2_minus_y1]     
 mov w [CS:ncLeftTxAdder1],ax
; LeftTxAdder 2 = (tx3-tx2)/(y3-y2) 8.8
 mov ax,[tx3]           
 sub ax,[tx2]           
 shl ax,8                    
 cwd                       
 idiv [y3_minus_y2]     
 mov w [CS:ncLeftTxAdder2],ax
; LeftTyAdder 1 = (ty2-ty1)/(y2-y1) 8.8
 mov ax,[ty2]           
 sub ax,[ty1]                
 shl ax,8                                              
 cwd                                
 idiv [y2_minus_y1]     
 mov w [CS:ncLeftTyAdder1],ax
; LeftTyAdder 2 = (ty3-ty2)/(y3-y2) 8.8
 mov ax,[ty3]           
 sub ax,[ty2]           
 shl ax,8                                              
 cwd                                
 idiv [y3_minus_y2]     
 mov w [CS:ncLeftTyAdder2],ax
; TxAdder = (tx4-tx2)/(x4-x2)
 mov ax,si              
 sub ax,[tx2]           
 shl ax,8                       
 cwd                            
 mov bx,bp              
 sub bx,[x2]            
 jz QuitPoly                    
 idiv bx                        
 mov w [cs:ncTxAdder1],ax 
 mov w [cs:ncTxAdder2],ax 
; TyAdder = (ty4-ty2)/(x4-x2)
 mov ax,di      
 sub ax,[ty2]  
 shl ax,8                       
 cwd                            
 idiv bx                        
 mov w [cs:ncTyAdder1],ax
 mov w [cs:ncTyAdder2],ax 
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
; Det var konstantene, n kommer registrene... phew...
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
; EAX = 0 : RightX     
 xor eax,eax                   
 mov ax,[x1] 
 inc ax                      
; EDX = - : Ypos
 xor edx,edx    
 mov dx,[y1]    
; ECX = TyPos:0 | TxPos:0
 mov cx,[Ty1]            
 shl ecx,16     
 mov cx,[Tx1]   
 shl ecx,8      
; ESI = 0 : LeftX      
 xor esi,esi    
 mov si,[x1]            
; EBP = Height : -      
 mov bp,[y2_minus_y1] 
 shl ebp,16             
 JMP ncFiller1                                                           
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
ALIGN 16
DoClip:      
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
; Vi sjekker for hvilket, og utfrer relevant polygon-filler rutine
 mov ax,[y2]                    
 cmp ax,[y1]                    
 je FlatTop             ; Y1=y2 - Flat Top
 cmp ax,[y3]            ; Y2=y3 - Flat Bottom
 je FlatBottom                               
 mov eax,[slope12]              
 cmp eax,[slope13]              
 je QuitPoly                    
 jl FlatRight                   
 Jmp FLatLeft                   
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
ALIGN 16
FlatBottom:                     
; 1                             
;2 3           
; LeftXAdder = Slope12
 mov eax,[Slope12]                     
 rol eax,16                                            
 mov w [CS:IntLeftAdder2],ax
 xor ax,ax                             
 mov d [CS:FracLeftAdder2],eax
; RightXAdder = Slope13 
 mov eax,[Slope13]             
 rol eax,16             
 mov w [CS:IntRightAdder2],ax
 xor ax,ax
 mov d [CS:FracRightAdder2],eax
; LeftTxAdder = (tx2-tx1)/(y2-y1) 8.8                  
 mov ax,[tx2]
 sub ax,[tx1]                                          
 shl ax,8       
 cwd      
 idiv [y2_minus_y1]                 
 mov w [CS:LeftTxAdder2],ax
; LeftTyAdder = (ty2-ty1)/(y2-y1) 8.8
 mov ax,[ty2]                     
 sub ax,[ty1]                     
 shl ax,8                                              
 cwd                                
 idiv [y2_minus_y1]                 
 mov w [CS:LeftTyAdder2],ax
; TxAdder = (tx3-tx2)/(x3-x2)     
 mov ax,[tx3]                     
 sub ax,[tx2]                   
 shl ax,8                       
 cwd                            
 mov bx,[x3]                    
 sub bx,[x2]                    
 jz QuitPoly                    
 idiv bx                        
 mov w [cs:TxAdder2],ax
; TyAdder = (ty3-ty2)/(x3-x2)   
 mov ax,[ty3]                   
 sub ax,[ty2]                   
 shl ax,8                       
 cwd                            
 idiv bx                        
 mov w [cs:TyAdder2],ax
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
; Det var konstantene, n kommer registrene... phew...
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
; EAX = 0 : RightX       
 xor eax,eax                   
 mov ax,[x1]             
 inc ax                      
; EDX = 0 : Ypos
 xor edx,edx    
 mov dx,[y1]    
; ECX = TyPos:0 | TxPos:0
 mov cx,[Ty1]            
 shl ecx,16     
 mov cx,[Tx1]
 shl ecx,8  
; ESI = 0 : LeftX      
 xor esi,esi            
 mov si,[x1]            
; EBP = Height : -      
 mov bp,[y3_minus_y1]
 shl ebp,16             
                         
 JMP Filler2                    
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
ALIGN 16
FlatTop:                        
;1 2    
; 3         
; LeftXAdder = Slope13
 mov eax,[Slope13]
 rol eax,16                                            
 mov w [CS:IntLeftAdder2],ax
 xor ax,ax                             
 mov d [CS:FracLeftAdder2],eax
; RightXAdder = Slope23
 mov eax,[Slope23]
 rol eax,16             
 mov w [CS:IntRightAdder2],ax
 xor ax,ax      
 mov d [CS:FracRightAdder2],eax
; LeftTxAdder = (tx3-tx1)/(y3-y1) 8.8
 mov ax,[tx3]
 sub ax,[tx1]                                          
 shl ax,8       
 cwd            
 idiv [y3_minus_y1]
 mov w [CS:LeftTxAdder2],ax
; LeftTyAdder = (ty3-ty1)/(y3-y1) 8.8
 mov ax,[ty3]
 sub ax,[ty1]                     
 shl ax,8                                              
 cwd                                
 idiv [y3_minus_y1]
 mov w [CS:LeftTyAdder2],ax
; TxAdder = (tx2-tx1)/(x2-x1)
 mov ax,[tx2]  
 sub ax,[tx1]  
 shl ax,8                       
 cwd                            
 mov bx,[x2]   
 sub bx,[x1]            
 jz QuitPoly                    
 idiv bx                        
 mov w [cs:TxAdder2],ax
; TyAdder = (ty2-ty1)/(x2-x1)
 mov ax,[ty2]
 sub ax,[ty1]
 shl ax,8                       
 cwd                            
 idiv bx                        
 mov w [cs:TyAdder2],ax
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
; Det var konstantene, n kommer registrene... phew...
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
; EAX = 0 : RightX     
 xor eax,eax                   
 mov ax,[x2]
 inc ax
; EDX = - : Ypos
 xor edx,edx
 mov dx,[y1]
; ECX = TyPos:0 | TxPos:0
 mov cx,[Ty1]
 shl ecx,16
 mov cx,[Tx1]
 shl ecx,8  
; ESI = 0 : LeftX       
 xor esi,esi            
 mov si,[x1]            
; EBP = Height : -      
 mov bp,[y3_minus_y1]   
 shl ebp,16             
 JMP Filler2                    
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
ALIGN 16
FlatLeft:                       
;  1                    
;(4)2                   
; 3                       
; LeftXAdder 1&2 = Slope13
 mov eax,[Slope13]      
 rol eax,16                                            
 mov w [CS:IntLeftAdder1],ax
 mov w [CS:IntLeftAdder2],ax 
 xor ax,ax                             
 mov d [CS:FracLeftAdder1],eax
 mov d [CS:FracLeftAdder2],eax 
; RightXAdder 1 = Slope12 
 mov eax,[Slope12]
 rol eax,16               
 mov w [CS:IntRightAdder1],ax
 xor ax,ax                
 mov d [CS:FracRightAdder1],eax
; RightXAdder 2 = Slope23
 mov eax,[Slope23]      
 rol eax,16               
 mov w [CS:IntRightAdder2],ax
 xor ax,ax                
 mov d [CS:FracRightAdder2],eax
; LeftTxAdder = (tx3-tx1)/(y3-y1) 8.8
 mov ax,[tx3]     
 sub ax,[tx1]     
 shl ax,8                    
 cwd                       
 idiv [y3_minus_y1]
 mov w [CS:LeftTxAdder1],ax
 mov w [CS:LeftTxAdder2],ax
; LeftTyAdder = (ty3-ty1)/(y3-y1) 8.8
 mov ax,[ty3]                
 sub ax,[ty1]                
 shl ax,8                                              
 cwd                                
 idiv [y3_minus_y1]     
 mov w [CS:LeftTyAdder1],ax
 mov w [CS:LeftTyAdder2],ax 
; TxAdder = (tx2-tx4)/(x2-x4)
 mov ax,[tx2]           
 sub ax,si              
 shl ax,8                       
 cwd                            
 mov bx,[x2]            
 sub bx,bp              
 jz QuitPoly                    
 idiv bx                        
 mov w [cs:TxAdder1],ax 
 mov w [cs:TxAdder2],ax 
; TyAdder = (ty2-ty4)/(x2-x4)
 mov ax,[ty2]           
 sub ax,di                   
 shl ax,8                       
 cwd                            
 idiv bx                        
 mov w [cs:TyAdder1],ax 
 mov w [cs:TyAdder2],ax 
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
; Det var konstantene, n kommer registrene... phew...
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
; EAX = 0 : RightX      
 xor eax,eax                   
 mov ax,[x1]            
 inc ax                      
; EDX = - : Ypos
 xor edx,edx    
 mov dx,[y1]    
; ECX = TyPos:0 | TxPos:0
 mov cx,[Ty1]            
 shl ecx,16
 mov cx,[Tx1]
 shl ecx,8  
; ESI = 0 : LeftX      
 xor esi,esi
 mov si,[x1]            
; EBP = Height : -      
 mov bp,[y2_minus_y1]   
 shl ebp,16             
 JMP Filler1            
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
ALIGN 16
FlatRight:                      
; 1                     
;2(4)                   
;  3                       
; LeftXAdder 1 = Slope12
 mov eax,[Slope12]      
 rol eax,16                                            
 mov w [CS:IntLeftAdder1],ax
 xor ax,ax                             
 mov d [CS:FracLeftAdder1],eax
; LeftXAdder 2 = Slope23
 mov eax,[Slope23]      
 rol eax,16                                            
 mov w [CS:IntLeftAdder2],ax
 xor ax,ax                             
 mov d [CS:FracLeftAdder2],eax
; RightXAdder = Slope13 
 mov eax,[Slope13]      
 rol eax,16               
 mov w [CS:IntRightAdder1],ax
 mov w [CS:IntRightAdder2],ax
 xor ax,ax                
 mov d [CS:FracRightAdder1],eax
 mov d [CS:FracRightAdder2],eax
; LeftTxAdder 1 = (tx2-tx1)/(y2-y1) 8.8
 mov ax,[tx2]           
 sub ax,[tx1]                  
 shl ax,8                    
 cwd                       
 idiv [y2_minus_y1]     
 mov w [CS:LeftTxAdder1],ax
; LeftTxAdder 2 = (tx3-tx2)/(y3-y2) 8.8
 mov ax,[tx3]           
 sub ax,[tx2]           
 shl ax,8                    
 cwd                       
 idiv [y3_minus_y2]     
 mov w [CS:LeftTxAdder2],ax
; LeftTyAdder 1 = (ty2-ty1)/(y2-y1) 8.8
 mov ax,[ty2]           
 sub ax,[ty1]                
 shl ax,8                                              
 cwd                                
 idiv [y2_minus_y1]     
 mov w [CS:LeftTyAdder1],ax
; LeftTyAdder 2 = (ty3-ty2)/(y3-y2) 8.8
 mov ax,[ty3]           
 sub ax,[ty2]           
 shl ax,8                                              
 cwd                                
 idiv [y3_minus_y2]     
 mov w [CS:LeftTyAdder2],ax
; TxAdder = (tx4-tx2)/(x4-x2)
 mov ax,si              
 sub ax,[tx2]           
 shl ax,8                       
 cwd                            
 mov bx,bp              
 sub bx,[x2]            
 jz QuitPoly                    
 idiv bx                        
 mov w [cs:TxAdder1],ax 
 mov w [cs:TxAdder2],ax 
; TyAdder = (ty4-ty2)/(x4-x2)
 mov ax,di      
 sub ax,[ty2]
 shl ax,8                       
 cwd                            
 idiv bx                        
 mov w [cs:TyAdder1],ax
 mov w [cs:TyAdder2],ax 
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
; Det var konstantene, n kommer registrene... phew...
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
; EAX = 0 : RightX     
 xor eax,eax                   
 mov ax,[x1]   
 inc ax                      
; EDX = - : Ypos
 xor edx,edx    
 mov dx,[y1]    
; ECX = TyPos:0 | TxPos:0
 mov cx,[Ty1]            
 shl ecx,16     
 mov cx,[Tx1]   
 shl ecx,8      
; ESI = 0 : LeftX      
 xor esi,esi    
 mov si,[x1]            
; EBP = Height : -      
 mov bp,[y2_minus_y1] 
 shl ebp,16             
 JMP Filler1                                                           
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ  
; Jepp, n kommer selve fillerne.. To stykker, en for ver 1/2-del og  
; en for nedre...                                                      
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ  
ALIGN 16
Filler1:
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
;EAX     LeftX Frac      RightX Int                                    
;EBX                     Temp (inner)
;ECX     TyPos           TxPos
;EDX                     YPos
;ESI     RightX Frac     LeftX Int                                     
;EDI                     ScreenPos (inner)                             
;EBP     PolygonHeight   LineLength (inner)                            
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
Outer1:  
 cmp edx,199
 ja SkipLine1           ; Negativer verdier vil vel fanges opp med denne?
 lea edi,[edx+edx*4]    ; DI = Y*5
 shl di,6               ; DI*64 -> 64*5 = 320
                                             
 mov bp,ax                                                             
 cmp bp,0                                                              
 jle SkipLine1                                                         
 cmp si,319                                                            
 jge SkipLine1                                                         
 cmp bp,319                                      
 jng @@11                                          
 mov bp,319                                      
@@11:                                            
 cmp si,0                                        
 jl @@12                                         
 sub bp,si                                       
 add di,si                                       
@@12:                                            
 push ecx                                        
; ECX = YInt:YFrac      XInt:XFrac               
 add di,bp                                       
 neg bp                                          
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ                         
ALIGN 16                                         
Inner1:                         ; Clocks        Bytes
 shld ebx,ecx,16                ; 2             4
 mov bl,ch                      ; 1             2
 mov bl,[FS:BX]                 ; 1             3
 mov [ES:DI+bp],bl              ; 1+1(+1)       3
 add ecx,12345678h              ; 1+1           7
TxAdder1 = $-4                                     
TyAdder1 = $-2                                   
 inc bp                         ; 1             1
 jnz Inner1                     ; 1/3           2
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ                         
 pop ecx                                         
SkipLine1:                                       
 xor bp,bp                      ;                
 add ecx,12345678h                               
LeftTxAdder1 = $-4                               
LeftTyAdder1 = $-2                               
 add esi,12340000h                               
FracRightAdder1 = $-4                            
 adc ax,1234h                                    
IntRightAdder1 = $-2                             
 add eax,12340000h                               
FracLeftAdder1 = $-4                             
 adc si,1234h                                     
IntLeftAdder1 = $-2                              
 inc dx 
 sub ebp,10000h         
 jnz Outer1             
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
 mov bp,[y3_minus_y2]   
 shl ebp,16       
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
Filler2:                                                               
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
;EAX     LeftX Frac      RightX Int
;EBX     Ypos * 2        Temp (inner)
;ECX                     TxPos  
;EDX                     TyPos  
;ESI     RightX Frac     LeftX Int
;EDI                     ScreenPos (inner)
;EBP     PolygonHeight   LineLength (inner)
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
ALIGN 16
Outer2:                       
 cmp edx,199   
 ja SkipLine2                                    
 lea edi,[edx+edx*4]
 shl di,6 
 mov bp,ax            
 cmp bp,0     
 jle SkipLine2
 cmp si,319       
 jge SkipLine2
 cmp bp,319    
 jle @@21      
 mov bp,319                                                            
@@21:                   
 cmp si,0               
 jl @@22                            
 sub bp,si              
 add di,si              
@@22:                   
 push ecx
 add di,bp
 neg bp        
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ                         
ALIGN 16
Inner2:                                          
 shld ebx,ecx,16                                 
 mov bl,ch                                       
 mov bl,[FS:BX]                                  
 mov [ES:DI+bp],bl
 add ecx,12345678h      
TxAdder2 = $-4          
TyAdder2 = $-2          
 inc bp           
 jnz Inner2             
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
 pop ecx                
SkipLine2:              
 xor bp,bp              
 add ecx,12345678h          
LeftTxAdder2 = $-4
LeftTyAdder2 = $-2
 add esi,12340000h                                
FracRightAdder2 = $-4
 adc ax,1234h     
IntRightAdder2 = $-2   
 add eax,12340000h     
FracLeftAdder2 = $-4   
 adc si,1234h        
IntLeftAdder2 = $-2  
 inc dx    
 sub ebp,10000h      
 jnz Outer2       
QuitPoly:         
 pop ds           
QuitPoly_noDS:    
 ret                                
;ENDP                                
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
y1 dw ?                          
x1 dw ?    
y2 dw ?                          
x2 dw ?                          
y3 dw ?                          
x3 dw ?                          
ty1 dw ?                         
tx1 dw ?                         
ty2 dw ?                         
tx2 dw ?                         
ty3 dw ?                         
tx3 dw ?                         
Slope12 dd ?                     
Slope13 dd ?                     
Slope23 dd ?                     
y2_minus_y1 dw ?                 
y3_minus_y1 dw ?                 
y3_minus_y2 dw ?                 
Parts db ?      ; Bit 1 = HasTop, Bit 2 = HasBottom
NeedClip dw ?
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
; Her kommer NoClip Filleren
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ  
ncFiller1:
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
;EAX     LeftX Frac      RightX Int                                    
;EBX                     Temp (inner)
;ECX     TyPos           TxPos
;EDX                     YPos                          
;ESI     RightX Frac     LeftX Int                                     
;EDI                     ScreenPos (inner)                             
;EBP     PolygonHeight   LineLength (inner)                            
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
 push fs     
 pop ds      
ALIGN 16
ncOuter1:    
 lea edi,[edx+edx*4]
 shl di,6               
 mov bp,si  
 sub bp,ax  
 add di,ax                      
 push ecx                       
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ        
ALIGN 16
ncInner1:                         ; Clocks        Bytes
 shld ebx,ecx,16                ; 2             4        
 mov bl,ch                      ; 1             2
 mov bl,[BX]                    ; 1+1           3
 mov [ES:DI+bp],bl              ; 1+1(+1)       3
 add ecx,12345678h              ; 1+1           7
ncTxAdder1 = $-4                                     
ncTyAdder1 = $-2                                   
 inc bp                         ; 1             1
 jnz ncInner1                     ; 1/3           2
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ                           
 pop ecx  
ncSkipLine1:                          
 xor bp,bp                      ; 
 add ecx,12345678h                
ncLeftTxAdder1 = $-4                
ncLeftTyAdder1 = $-2                
 add esi,12340000h              
ncFracRightAdder1 = $-4             
 adc ax,1234h           
ncIntRightAdder1 = $-2    
 add eax,12340000h      
ncFracLeftAdder1 = $-4    
 adc si,1234h                                     
ncIntLeftAdder1 = $-2                              
 inc dx         
 sub ebp,10000h         
 jnz ncOuter1
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
 mov bp,[CS:y3_minus_y2]   
 shl ebp,16
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
ncFiller2:
 push fs 
 pop ds  
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
;EAX     LeftX Frac      RightX Int
;EBX     Ypos * 2        Temp (inner)
;ECX                     TxPos  
;EDX                     TyPos  
;ESI     RightX Frac     LeftX Int
;EDI                     ScreenPos (inner)
;EBP     PolygonHeight   LineLength (inner)
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
ALIGN 16
ncOuter2:                       
 lea edi,[edx+edx*4]
 shl di,6 
 mov bp,si 
 sub bp,ax 
 add di,ax 
 push ecx  
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ                         
ALIGN 16
ncInner2:                                          
 shld ebx,ecx,16                    
 mov bl,ch                      
 mov bl,[BX]                       
 mov [ES:DI+bp],bl              
 add ecx,12345678h      
ncTxAdder2 = $-4          
ncTyAdder2 = $-2          
 inc bp           
 jnz ncInner2             
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
 pop ecx                
ncSkipLine2:              
 xor bp,bp              
 add ecx,12345678h          
ncLeftTxAdder2 = $-4
ncLeftTyAdder2 = $-2
 add esi,12340000h                                
ncFracRightAdder2 = $-4
 adc ax,1234h     
ncIntRightAdder2 = $-2   
 add eax,12340000h     
ncFracLeftAdder2 = $-4   
 adc si,1234h        
ncIntLeftAdder2 = $-2  
 inc dx    
 sub ebp,10000h      
 jnz ncOuter2       
 pop ds           
 ret                                
ENDP               
               
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
; X = EBX, Y = ECX, Z = EBP         
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
PROC Rotate                                       
; X-rotation                        
; Y = cos(vx)*y - sin(vx)*z 
; Z = sin(vx)*y + cos(vx)*z 
 mov si,[vx]                                     
 add si,si                                       
 movsx eax,[Cosinus+si]    ; eax = fra -16k til 16k
 imul ecx                                          
 mov edi,eax            ; edi = cos(vx)*y
 movsx eax,[Sinus+si]
 imul ebp               ; eax = sin(vx)*z
 sub edi,eax       
 sar edi,divd                                
 mov [temp],edi         ; cos(vx)*y - sin(vx)*z
 movsx eax,[sinus+si]
 imul ecx            
 mov edi,eax            ; edi = sin(vx)*y
 movsx eax,[cosinus+si]
 imul ebp               ; eax = cos(vx)*z
 add edi,eax                             
 sar edi,divd                                     
 mov ecx,[temp]         ; ecx (y) = cos(vx)*y - sin(vx)*z
 mov ebp,edi            ; ebp (z) = sin(vx)*y + cos(vx)*z
; Y-rotation         
; X =  cos(vy)*x - sin(vy)*z                                             
; Z = -sin(vy)*x + cos(vy)*z
 mov si,[vy]         
 add si,si                                       
 movsx eax,[Cosinus+si] 
 imul ebx                                
 mov edi,eax            ; edi = cos(vy)*x
 movsx eax,[Sinus+si]       
 imul ebp               ; eax = sin(vy)*z
 sub edi,eax                          
 sar edi,divd                                
 mov [temp],edi             
 movsx eax,[sinus+si]       
; neg eax                ; hmmm... Sikker p at denne skal vre med???        
 imul ebx                                                            
 mov edi,eax            ; edi = -sin(vy)*x                           
 movsx eax,[cosinus+si]                                              
 imul ebp               ; eax = cos(vy)*z                            
 add edi,eax                                                         
 sar edi,divd                                                        
 mov ebx,[temp]         ; ebx (x) =  cos(vy)*x - sin(vy)*z           
 mov ebp,edi            ; ebp (z) = -sin(vy)*x + cos(vy)*z           
; Z-rotation                                                         
; X = cos(vz)*x - sin(vz)*y                                          
; Y = sin(vz)*x + cos(vz)*y                                          
 mov si,[vz]                                                         
 add si,si                                                           
 movsx eax,[Cosinus+si]
 imul ebx                                                            
 mov edi,eax            ; edi = cos(vz)*x                            
 movsx eax,[Sinus+si]                                                
 imul ecx               ; eax = sin(vz)*y                            
 sub edi,eax                        
 sar edi,divd                                
 mov [temp],edi                          
 movsx eax,[sinus+si]   
 imul ebx                                         
 mov edi,eax            ; edi = sin(vz)*x
 movsx eax,[cosinus+si] 
 imul ecx               ; eax = cos(vz)*y
 add edi,eax                        
 sar edi,divd                       
 mov ebx,[temp]         ; ebx (x) = cos(vz)*x - sin(vz)*y
 mov ecx,edi            ; ecx (y) = sin(vz)*x + cos(vz)*y
 ret                   
ENDP                   
                                                         
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
             
PROC RotateObjCoords
 mov ax,[treDSeg]    
 mov gs,ax           
 xor esi,esi         
 mov si,[Coords]    
 xor edx,edx        
 mov ax,[NumCoords] 
 shl ax,2
 mov w [CS:@@Finito],ax
ALIGN 16            
RotLoop:            
 lea di,[esi+edx*2]
 movsx ebx,w [gs:di] 
 inc di              
 inc di              
 movsx ecx,w [gs:di]          
 inc di                       
 inc di                       
 movsx ebp,w [gs:di]                    
 push edx                               
 push esi                               
 Call Rotate                            
; ebx=X, ecx=Y, ebp=Z
; x := (x*256)/(Z+Zoff) + Xoff                              
                     
 add ebp,[Zoff]      
     
 mov eax,ebx
 shl eax,8                                    
 cdq                                          
 idiv ebp                                     
 add eax,[Xoff]            ; eax = Projected X
 mov ebx,eax         
                     
 mov eax,ecx
 shl eax,8
 cdq                    
 idiv ebp            
 add eax,[Yoff]            ; eax = Projected X
 mov ecx,eax         
                     
 pop esi                                
 pop edx                                
                                   
 lea di,[TriCoords+edx]            
 mov [di],cx                    ; Y
 inc di                            
 inc di                            
 mov [di],bx                    ; X
 add dx,4            
 cmp dx,1234h        
@@Finito = $-2                
 jb RotLoop          
 ret                                                                    
ENDP                
                   
PROC RotateNormals  
 mov ax,[treDseg]   
 mov gs,ax          
 xor esi,esi                                  
 mov si,[VertexNormals]
 Xor edx,edx            ; Start med coord #0
 mov ax,[NumCoords]
 shl ax,2
 mov w [CS:@@Finito],ax
ALIGN 16   
RotLoop2:                          
 lea di,[esi+edx*2]
 movsx ebx,w [gs:di]               
 inc di 
 inc di 
 movsx ecx,w [gs:di]          
 inc di 
 inc di 
 movsx ebp,w [gs:di]
 push edx               
 push esi                
 Call Rotate
 add bx,63 
 add cx,63 
 pop esi                 
 pop edx                 
 lea di,[TexCoords+edx]
 mov [di],cx                    ; Y
 inc di                            
 inc di                            
 mov [di],bx                    ; X
 add edx,4
 cmp dx,1234h                      
@@Finito = $-2                     
 jb RotLoop2                       
 ret                                                                    
ENDP                               
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
                   
; ESI = Coord set 1 - uindeksert, etc, bare rene nummeret til coord'ene
; EDi = Coord set 2                                                    
; EBP = Coord set 3                                                    
; +0 = Y, +2 = X                                                      
; Returnerer Face Normal i EAX... negativ=synlig...                   
; Destroys eax,ebx,ecx,edx                                            
PROC CheckVisible          
; (x2-x1)*(y3-y1) - (x3-x1)*(y2-y1)                                   
 mov ax,w [DS:TriCoords+edi*4+2]        ; x2
 sub ax,w [DS:TriCoords+esi*4+2]        ; x1
 movsx eax,ax                               
 mov bx,w [DS:TriCoords+ebp*4]          ; y3
 sub bx,w [DS:TriCoords+esi*4]          ; y1    
 movsx ebx,bx                               
 imul ebx       ; Blir denne IMUL'en noensinne over 16bit???
 mov ecx,eax    ; klarer jeg meg med 16-bits ??? Sjekk senere...
 mov ax,w [DS:TriCoords+ebp*4+2]        ; x3
 sub ax,w [DS:TriCoords+esi*4+2]        ; x1
 movsx eax,ax    
 mov bx,w [DS:TriCoords+edi*4]          ; y2  
 sub bx,w [DS:TriCoords+esi*4]          ; y1
 movsx ebx,bx                  
 imul ebx                      
 sub ecx,eax                   
 mov eax,ecx                   
 ret                                            
ENDP                                            
                 
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
; Teoretisk, s snart den funker 100% :) gr denne gjennom alle polygons,
; sjekker om de er synlige, og setter dem i s fall inn i lista over
; polys som skal frst sorteres, og deretter tegnes...
PROC SortSetup                                                       
 mov [CS:NumToDraw],0
 mov ax,[treDseg]
 mov gs,ax    
 xor edx,edx                                           
 mov dx,[Polygon]               ; EDX ->Polygon-face-data
 xor ecx,ecx                    ; ECX = Current polygon to check
 xor eax,eax                    ; EAX = Number in Face_Z/Ind
SSLoop1:                                                    
 xor esi,esi                                
 mov si,[gs:EDX+ecx*8]          ; Coord set #1
 xor edi,edi                                                
 mov di,[gs:EDX+ecx*8+2]        ; Coord set #2
 xor ebp,ebp                                                
 mov bp,[gs:EDX+ecx*8+4]        ; Coord set #3
 push eax                                                   
 push ecx     
 push edx                                                            
 Call CheckVisible                          
 cmp eax,0                                  
 jl SortThisOne                 ; Negative = Visible
SkipThisOne:                                             
 pop edx                                                 
 pop ecx                                                 
 pop eax                                                 
 inc ecx                ; Neste Polygon                  
 cmp cx,[NumPolys]              ; Checked all?           
 jb SSLoop1                                              
 ret                                                     
SortThisOne:                                             
 pop edx                                                 
 pop ecx                                                 
 pop eax                                                 
; Rotated = Start p Roterte koordinater (words X,Y,Z,0)
; SI = Coords set #1, DI = #2, BP = #3
 mov bx,[Rotated+esi*8+4]       ; Z1
 add bx,[Rotated+edi*8+4]       ; Z2
 add bx,[Rotated+ebp*8+4]       ; Z3
                     
 Mov w [DS:Face_z+eax*2],bx       ; Insert Z's til poly #
 mov w [DS:FaceInd+eax*2],cx      ; INsert Polygon # in Ind
 inc eax                        ; Next pos in lists
 inc [CS:NumToDraw]             ; Inc number of polys to draw/sort
 inc ecx                        ; Next poly to check     
 cmp cx,[NumPolys]                                  
 jb SSLoop1                                         
 ret                                                
ENDP                                                  
               
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
NumToDraw dw ?                                      
                                                    
PROC DoObject                                   
 mov ax,[Buffer]
 mov es,ax                                   
 mov ax,[Texture]
 mov fs,ax                          
 Call RotateObjCoords                                                
 Call RotateNormals
               
 Call SortSetup
 Call ShellSort
                                                         
 xor edx,edx                                             
 mov dx,[CS:NumToDraw]          ; DX = teller nedover fra siste poly
 dec edx                        ; Antall polys-1 (fra n-1 til 0)
 jz FinitoDraw                                                  
 xor ebx,ebx                                                                   
 mov bx,[Polygon]               ; EBX -> Polygon-face data                
DrawObjLoop:                                                    
 xor ecx,ecx 
 mov cx,[DS:FaceInd+edx*2]      ; CX = Polygon #
 xor esi,esi                                             
 xor edi,edi                                                    
 xor ebp,ebp                                                    
; EBX = Start of Polygon-face data list
; ECX = Current polygon #
 mov si,[gs:ebx+ecx*8]         ; SI = Coord set #1
 shl si,2                      ; * 4 = Index in TriCoords & TexCoords
 mov di,[gs:ebx+ecx*8+2]       ; DI = Coord set #2
 shl di,2                                                       
 mov bp,[gs:ebx+ecx*8+4]       ; BP = Coord set #3
 shl bp,2                                                       
 push edx             
 push ebx                                                
 Call TmTri    
 pop ebx       
 pop edx       
 dec edx     
 jns DrawObjLoop
FinitoDraw:  
 ret                  
ENDP                  
                      
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
               
Proc EnvMap FAR
 mov [saved_di],0
ALIGN 16
IntroLoop:       
 Call ClearBuffer
 Call DoObject
 Call WaitVR  
 cmp [UseBlur],1
 jne noblur    
 Call BlurCopyBuffer
 jmp fincopy   
noblur:        
 Call CopyBuffer
fincopy:      
 mov ax,[VxAdd]
 add [vx],ax   
 and [vx],1023 
               
 mov ax,[VyAdd]
 add [vy],ax   
 and [vy],1023 
               
 mov ax,[VzAdd]
 add [vz],ax   
 and [vz],1023 
               
 movsx eax,[AddToXoff]
 add [Xoff],eax
 movsx eax,[AddToYoff]
 add [Yoff],eax
 movsx eax,[AddToZoff]
 add [Zoff],eax
 in al,60h
 cmp al,1
 jne IntroLoop 
; je QuitEnvMap
               
; dec [NumCounter]
; jnz IntroLoop
QuitEnvMap:    
 ret           
ENDP           
               
PROC Error   
 mov ax,4c00h
 int 21h
ENDP 
    
;° EntryPoint from DOS °°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
EntryPoint:                             
 mov ax,DATA
 mov ds,ax
 assume ds:data
 mov ax,SEG Buffe
 mov [Buffer],ax  
 mov ax,SEG Textur
 mov [Texture],ax
 mov ax,13h    
 int 10h       
 Call SetPalette
; Call CalcTex  
 Call DoObject1 
 mov ax,3      
 int 10h       
 mov ax,4c00h  
 int 21h       
               
ENDS CODE      
               
End EntryPoint 