; Jepp, tror faktisk ikke det er mulig † optimere selve EnvMap delen s†
; mye mer...
; S†, n† m† vi se p† de andre tingene -> Sortering, rotering, etc...
; Sortering: Radix
; Rotering: 9 IMUL's

; Kombin‚r BackFace-removal og Sort-Setup? Hent inn koordinatene, sjekk
; om synlig, og sett inn i sort-list BARE hvis polygonet er synlig?
;
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
                      
Segment TextureSeg
Label Texture byte
 include tex1.inc
ends          
              
Segment BufferSeg         
Label Buffer byte 
 db 65536 dup(0) 
ends                     
                         
Segment treDsegment
 nc1 dw 182              ; Antall koordinater
 np1 dw 200              ; antall polygons
 include e:\axon\asc\kulering.inc
ENDS                                                       
                
;° DATA °°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
.DATA                 
                      
treDseg dw ?        
                    
Palette label byte
 include intropal.db
;i=0 
;rept 24
; db i,0,0
; i=i+1
;endm   
;rept 8
; db i,0,0
; i=i+3
;endm 
;rept 256-32
; db 0,0,0
;endm
                 
MaxPolys = 1024  
MaxCoords = 1024 
                 
AddToXOff dw ?   
AddToYOff dw ?   
AddtoZoff dw ?   
NumCounter dw ?     
                 
UseBlur dw ?     
saved_di dw ?                
                   
Xoff dd 160      ; Variables for 3D->2D transformation
Yoff dd 100      
zoff dd 700
                
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
; 3D variables  
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
NumCoords dw ?   
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
TriCoords dd MaxCoords dup (?)
TexCoords dd MaxCoords*2 dup(?) ;Trenger vel ikke *2 ????????
                                 
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
; Variabler til Sortering...     
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
face_z dw MaxPolys DUP (?)   
faceind dw MaxPolys DUP (?)      
                                 
;° CODE°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
.CODE                            
                                 
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
 mov [zoff],1000
 mov [AddToXoff],0 
 mov [AddToYOff],0 
 mov [AddToZoff],0 
 mov [NumCounter],150
 mov [UseBlur],0
 Call EnvMap         
 ret                 
ENDP                 
              
PROC WaitVR FAR 
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
 mov bp,[DS:NumPolys]
 dec bp       
ShellLoop:    
 cmp bp,1
 jl QuitShell     
 shr bp,1         
RepeatLoop:       
 mov cx,1         
 mov di,0;1         
 mov dx,[NumPolys]
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
                                             
              
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
PROC BlurCopyBuffer
 mov si,[saved_di]
 push ds           
 mov ax,SEG Buffer 
 mov ds,ax         
 mov ax,0a000h     
 mov es,ax
 mov cx,100        
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
 mov ax,SEG Buffer
 mov ds,ax        
 xor si,si        
 mov ax,0a000h    
 mov es,ax     
 xor di,di     
 mov cx,16000     
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
 mov ax,SEG Buffer
 mov es,ax
 xor di,di  
 xor eax,eax
 mov cx,16000
 rep stosd
 ret    
ENDP      
            
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

;align 16
PROC TMPoly
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
; SI/DI/BP -> Coordinates in TriCoords & TexCoords
; ES = Screen/Buffer
; FS = Texture         
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
 mov eax, d [DS:TriCoords+si]
 mov ebx, d [DS:TriCoords+di]  
 mov ecx, d [DS:TriCoords+bp]  
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
 cmp dx,-3      ; Alle X'ene er over 320, s†,,QUIT
 je QuitPoly_noDS               
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
; Sjekk om vi trenger † clippe Y
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
 mov dx,200         
 cmp dx,ax          
 sbb [CS:NeedClip],0
 cmp dx,bx          
 sbb [CS:NeedClip],0
 cmp dx,cx                      
 sbb [CS:NeedClip],0            
; Sort‚r                        
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
; Y-ene i riktig rekkef›lge... N† sjekk X'er
 cmp ax,bx       ;Y1=Y2 ? Is†fall, sjekk om X'ene m† swappes
 jne @@4         
 cmp eax,ebx                    
 jl @@4          
 xchg eax,ebx                   
 xchg si,di              
@@4:             
 cmp bx,cx       ;Y2=Y3 ? Is†fall, sjekk om X'ene m† swappes
 jne @@5               
 cmp ebx,ecx     
 jl @@5              
 xchg ebx,ecx                 
 xchg di,bp       
@@5:             
; 1     1     1     1 2
;  2   2     2 3     3  
;3       3       
; N† burde de v‘re sortert etter Y...
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
; Heretter brukes bare variabler som ligger i Code-segmentet, s† vi
; kopierer CS til DS, og fortsetter som normalt... 
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
 push ds               
 push cs                           
 pop ds                            
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
; Utregning av sloper..            
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
; Slope13 
 mov eax,d [y3]
 sub eax,d [y1]
 xor ebx,ebx                                 
 mov [y3_minus_y1],ax   
 test ax,ax             
 jz QuitPoly            
 xchg bx,ax   
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
 xchg bx,ax
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
 xchg bx,ax
 cdq                            
 idiv ebx                       
 mov [slope23],eax              
@@s2:                           
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
; F›rst sjekker vi om vi skal kj›re Clip eller NoClip versjonen...
 cmp [CS:NeedClip],0
 jne DoClip
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
; Nei - ingen clipping trengs - s† vi kj›rer rutine uten clipping,
; som er raskere...                      
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
NoClip:                         
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
; Vi sjekker for hvilket, og utf›rer relevant polygon-filler rutine
 mov ax,[y2]
 cmp ax,[y1]  
 je FlatTop             ; Y1=y2 - Flat Top
 cmp ax,[y3]            ; Y2=y3 - Flat Bottom
 je FlatBottom
 mov eax,[slope12]     
 cmp eax,[slope13]     
 je QuitPoly           
 jl FlatRight
 Jmp FlatLeft
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
;align 16
FlatTop:                     
; 1 2
;  3
; TextureIndex = tx1:0 : Ty1:0           
 mov ax,[tx1]                
 shl eax,16                  
 mov ax,[ty1]                   
 shl eax,8                      
 mov d [CS:TextureIndex2],eax
; Ypos320 = Y1*320      
 xor eax,eax            
 mov ax,[y1]            
 lea eax,[eax+eax*4]    
 shl ax,6               
 mov w [CS:Ypos3202],ax
; TxAdder = (tx2-tx1)/(x2-x1)
 mov ax,[tx2]                
 sub ax,[tx1]                
 shl ax,8                    
 cwd                         
 mov bx,[x2]                 
 sub bx,[x1]                 
 idiv bx                     
 mov w [CS:TxAdder2],ax                  
; TyAdder = (ty2-ty1)/(x2-x1)
 mov ax,[ty2]                
 sub ax,[ty1]                   
 shl ax,8                       
 cwd                         
 idiv bx                     
 mov w [CS:TyAdder2],ax     
; LeftTxAdder = (tx3-tx1)/(y3-y1)
 mov ax,[tx3]          
 sub ax,[tx1]                
 shl ax,8                    
 cwd                         
 idiv [y3_minus_y1]    
 mov w [CS:LeftTxAdder2],ax
; LeftTyAdder = (ty3-ty1)/(y3-y1)
 mov ax,[ty3]       
 sub ax,[ty1]       
 shl ax,8                    
 cwd                         
 idiv [y3_minus_y1]                      
 mov w [CS:LeftTyAdder2],ax
; Left frac/int Adder = Slope13
 mov eax,[Slope13]              
 mov w [CS:LeftFracAdder2],ax
 shr eax,16         
 mov w [CS:LeftIntAdder2],ax
; Right frac/int Adder = Slope23
 mov eax,[Slope23]  
 mov w [CS:RightFracAdder2],ax
 shr eax,16         
 mov w [CS:RightIntAdder2],ax
; AH = [y3_minus_y1]
 mov ax,[y3_minus_y1]
 shl ax,8              
; EBP = 0:RightInt  
 xor ebp,ebp        
 mov bp,[x2]        
; ESI = 0:LeftInt
 xor esi,esi
 mov si,[x1]
 Jmp Filler2        
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
;align 16
FlatBottom:                     
;  1
; 2 3
; TextureIndex = tx1:0 : Ty1:0           
 mov ax,[tx1]                
 shl eax,16                    
 mov ax,[ty1]                
 shl eax,8                      
 mov d [CS:TextureIndex2],eax
; Ypos320 = Y1*320      
 xor eax,eax            
 mov ax,[y1]            
 lea eax,[eax+eax*4]    
 shl ax,6               
 mov w [CS:Ypos3202],ax
; TxAdder = (tx3-tx2)/(x3-x2)
 mov ax,[tx3]      
 sub ax,[tx2]      
 shl ax,8                    
 cwd                         
 mov bx,[x3]                    
 sub bx,[x2]       
 idiv bx                     
 mov w [CS:TxAdder2],ax                  
; TyAdder = (ty3-ty2)/(x3-x2)
 mov ax,[ty3]                  
 sub ax,[ty2]
 shl ax,8                       
 cwd                         
 idiv bx                     
 mov w [CS:TyAdder2],ax
; LeftTxAdder = (tx2-tx1)/(y2-y1)
 mov ax,[tx2]
 sub ax,[tx1]                
 shl ax,8                    
 cwd                         
 idiv [y2_minus_y1]
 mov w [CS:LeftTxAdder2],ax
; LeftTyAdder = (ty2-ty1)/(y2-y1)
 mov ax,[ty2]
 sub ax,[ty1]                   
 shl ax,8                    
 cwd                         
 idiv [y2_minus_y1]
 mov w [CS:LeftTyAdder2],ax
; Left frac/int Adder = Slope12
 mov eax,[Slope12]
 mov w [CS:LeftFracAdder2],ax  
 shr eax,16                    
 mov w [CS:LeftIntAdder2],ax   
; Right frac/int Adder = Slope13
 mov eax,[Slope13]     
 mov w [CS:RightFracAdder2],ax
 shr eax,16         
 mov w [CS:RightIntAdder2],ax
; AH = [y3_minus_y1]
 mov ax,[y3_minus_y1]
 shl ax,8           
; EBP = 0:RightInt  
 xor ebp,ebp        
 mov bp,[x1]                    
 inc bp 
; ESI = 0:LeftInt
 xor esi,esi
 mov si,[x1]                           
 Jmp Filler2        
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
;align 16
FlatRight: 
; 1
;2  
;  3
; TextureIndex = tx1:0 : Ty1:0           
 mov ax,[tx1]                
 shl eax,16                    
 mov ax,[ty1]                
 shl eax,8                      
 mov d [CS:TextureIndex1],eax
; Ypos320 = Y1*320
 xor eax,eax            
 mov ax,[y1]            
 lea eax,[eax+eax*4]            
 shl ax,6               
 mov w [CS:Ypos3201],ax
; TxAdder = tx1+(tx3-tx1)/(y3-y1)*(y2-y1) - tx2
;           -----------------------------------
;                Slope13-Slope12 * (y2-y1)         
 mov ax,[tx3]                  
 sub ax,[tx1]                  
 imul [y2_minus_y1]        
 idiv [y3_minus_y1]        
 add ax,[tx1]              
 sub ax,[tx2]          
 cwd
 mov ebx,[Slope13]
 sub ebx,[Slope12]     
 movzx ecx,[y2_minus_y1]
 imul ebx,ecx      
 shr ebx,8       
 idiv bx         
 mov w [CS:TxAdder1],ax
 mov w [CS:TxAdder2],ax         
; TyAdder = (ty3-ty2)/(x3-x2)
 mov ax,[ty3]      
 sub ax,[ty1]      
 imul [y2_minus_y1]        
 idiv [y3_minus_y1]        
 add ax,[ty1]      
 sub ax,[ty2]      
 cwd               
 mov ebx,[Slope13] 
 sub ebx,[Slope12] 
 movzx ecx,[y2_minus_y1]
 imul ebx,ecx      
 shr ebx,8         
 idiv bx               
 mov w [CS:TyAdder1],ax
 mov w [CS:TyAdder2],ax
; LeftTxAdder1 = (tx2-tx1)/(y2-y1)
 mov ax,[tx2]      
 sub ax,[tx1]                
 shl ax,8                       
 cwd                         
 idiv [y2_minus_y1]
 mov w [CS:LeftTxAdder1],ax
; LeftTxAdder2 = (tx3-tx2)/(y3-y2)
 mov ax,[tx3]          
 sub ax,[tx2]          
 shl ax,8                    
 cwd                         
 idiv [y3_minus_y2]
 mov w [CS:LeftTxAdder2],ax
; LeftTyAdder1 = (ty2-ty1)/(y2-y1)
 mov ax,[ty2]          
 sub ax,[ty1]          
 shl ax,8                    
 cwd                         
 idiv [y2_minus_y1]    
 mov w [CS:LeftTyAdder1],ax
; LeftTyAdder2 = (ty3-ty2)/(y3-y2)
 mov ax,[ty3]
 sub ax,[ty2]                   
 shl ax,8                    
 cwd                         
 idiv [y3_minus_y2]
 mov w [CS:LeftTyAdder2],ax
; Left frac/int Adder 1 = Slope12
 mov eax,[Slope12]
 mov w [CS:LeftFracAdder1],ax
 shr eax,16                    
 mov w [CS:LeftIntAdder1],ax
; Left frac/int Adder 2 = Slope23
 mov eax,[Slope23]      
 mov w [CS:LeftFracAdder2],ax  
 shr eax,16                    
 mov w [CS:LeftIntAdder2],ax   
; Right frac/int Adder = Slope13
 mov eax,[Slope13]      
 mov w [CS:RightFracAdder1],ax
 mov w [CS:RightFracAdder2],ax
 shr eax,16               
 mov w [CS:RightIntAdder1],ax   
 mov w [CS:RightIntAdder2],ax
; AH = [y2_minus_y1]     
 mov ax,[y2_minus_y1]
 dec ax
 shl ax,8                
; EBP = 0:RightInt       
 xor ebp,ebp             
 mov bp,[x1]             
 inc bp                  
; ESI = 0:LeftInt        
 xor esi,esi             
 mov si,[x1]             
 Jmp Filler1             
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
;align 16
FlatLeft:                
; 1          1 
;2      ->    2
;  3        3  
; TextureIndex = tx1:0 : Ty1:0           
 mov ax,[tx1]                   
 shl eax,16                    
 mov ax,[ty1]                
 shl eax,8                      
 mov d [CS:TextureIndex1],eax
; Ypos320 = Y1*320
 xor eax,eax            
 mov ax,[y1]            
 lea eax,[eax+eax*4]     
 shl ax,6               
 mov w [CS:Ypos3201],ax
; TxAdder = tx1+(tx3-tx1)/(y3-y1)*(y2-y1) - tx2
;           -----------------------------------
;                Slope13-Slope12 * (y2-y1)         
 mov ax,[tx3]                                  
 sub ax,[tx1]                                  
 imul [y2_minus_y1]                            
 idiv [y3_minus_y1]                            
 add ax,[tx1]                                  
 sub ax,[tx2]                                  
 cwd                                           
 mov ebx,[Slope13]                             
 sub ebx,[Slope12]                             
 movzx ecx,[y2_minus_y1]                       
 imul ebx,ecx                                  
 shr ebx,8                                     
 idiv bx                                       
 mov w [CS:TxAdder1],ax                        
 mov w [CS:TxAdder2],ax                        
; TyAdder = (ty3-ty2)/(x3-x2)
 mov ax,[ty3]      
 sub ax,[ty1]      
 imul [y2_minus_y1]        
 idiv [y3_minus_y1]        
 add ax,[ty1]      
 sub ax,[ty2]      
 cwd               
 mov ebx,[Slope13] 
 sub ebx,[Slope12] 
 movzx ecx,[y2_minus_y1]
 imul ebx,ecx                   
 shr ebx,8         
 idiv bx               
 mov w [CS:TyAdder1],ax
 mov w [CS:TyAdder2],ax
; LeftTxAdder = (tx3-tx1)/(y3-y1)
 mov ax,[tx3]
 sub ax,[tx1]                                  
 shl ax,8                                      
 cwd                         
 idiv [y3_minus_y1]
 mov w [CS:LeftTxAdder1],ax
 mov w [CS:LeftTxAdder2],ax
; LeftTyAdder = (ty3-ty1)/(y3-y1)
 mov ax,[ty3]
 sub ax,[ty1]          
 shl ax,8                    
 cwd                         
 idiv [y3_minus_y1]
 mov w [CS:LeftTyAdder1],ax
 mov w [CS:LeftTyAdder2],ax     
; Left frac/int Adder = Slope13
 mov eax,[Slope13]
 mov w [CS:LeftFracAdder1],ax
 mov w [CS:LeftFracAdder2],ax     
 shr eax,16                       
 mov w [CS:LeftIntAdder1],ax      
 mov w [CS:LeftIntAdder2],ax      
; Right frac/int Adder 1 = Slope12
 mov eax,[Slope12]
 mov w [CS:RightFracAdder1],ax
 shr eax,16               
 mov w [CS:RightIntAdder1],ax
; Right frac/int Adder 2 = Slope23
 mov eax,[Slope23]
 mov w [CS:RightFracAdder2],ax
 shr eax,16               
 mov w [CS:RightIntAdder2],ax
; AH = [y2_minus_y1]     
 mov ax,[y2_minus_y1]    
 dec ax                         
 shl ax,8                
; EBP = 0:RightInt       
 xor ebp,ebp             
 mov bp,[x1]             
 inc bp                  
; ESI = 0:LeftInt        
 xor esi,esi                      
 mov si,[x1]                      
 Jmp Filler1                      
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
DoClip:
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
; Vi sjekker for hvilket, og utf›rer relevant polygon-filler rutine
 mov ax,[y2]
 cmp ax,[y1]  
 je cFlatTop             ; Y1=y2 - Flat Top
 cmp ax,[y3]            ; Y2=y3 - Flat Bottom
 je cFlatBottom
 mov eax,[slope12]     
 cmp eax,[slope13]     
 je QuitPoly
 jl cFlatRight
 Jmp cFlatLeft
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
;align 16
cFlatTop:                     
; 1 2  
;  3   
; TextureIndex = tx1:0 : Ty1:0           
 mov ax,[tx1]                
 shl eax,16                  
 mov ax,[ty1]                   
 shl eax,8                      
 mov d [CS:cTextureIndex2],eax
; Ypos320 = Y1
 mov ax,[y1]            
 mov w [CS:cYpos3202],ax
; TxAdder = (tx2-tx1)/(x2-x1)
 mov ax,[tx2]                
 sub ax,[tx1]                
 shl ax,8                    
 cwd                         
 mov bx,[x2]                 
 sub bx,[x1]                 
 idiv bx                     
 mov w [CS:cTxAdder2],ax
; TyAdder = (ty2-ty1)/(x2-x1)
 mov ax,[ty2]                
 sub ax,[ty1]                   
 shl ax,8                       
 cwd                         
 idiv bx                     
 mov w [CS:cTyAdder2],ax     
; LeftTxAdder = (tx3-tx1)/(y3-y1)
 mov ax,[tx3]          
 sub ax,[tx1]                
 shl ax,8                    
 cwd                         
 idiv [y3_minus_y1]    
 mov w [CS:cLeftTxAdder2],ax
; LeftTyAdder = (ty3-ty1)/(y3-y1)
 mov ax,[ty3]       
 sub ax,[ty1]       
 shl ax,8                    
 cwd                         
 idiv [y3_minus_y1]                      
 mov w [CS:cLeftTyAdder2],ax
; Left frac/int Adder = Slope13
 mov eax,[Slope13]              
 mov w [CS:cLeftFracAdder2],ax
 shr eax,16         
 mov w [CS:cLeftIntAdder2],ax
; Right frac/int Adder = Slope23
 mov eax,[Slope23]  
 mov w [CS:cRightFracAdder2],ax
 shr eax,16         
 mov w [CS:cRightIntAdder2],ax
; AH = [y3_minus_y1]
 mov ax,[y3_minus_y1]
 shl ax,8              
; EBP = 0:RightInt  
 xor ebp,ebp        
 mov bp,[x2]        
; ESI = 0:LeftInt
 xor esi,esi
 mov si,[x1]
 Jmp cFiller2        
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
;align 16    
cFlatBottom:                     
;  1   
; 2 3  
; TextureIndex = tx1:0 : Ty1:0           
 mov ax,[tx1]                
 shl eax,16                    
 mov ax,[ty1]                
 shl eax,8                      
 mov d [CS:cTextureIndex2],eax
; Ypos320 = Y1      
 mov ax,[y1]            
 mov w [CS:cYpos3202],ax
; TxAdder = (tx3-tx2)/(x3-x2)
 mov ax,[tx3]           
 sub ax,[tx2]           
 shl ax,8                    
 cwd                         
 mov bx,[x3]                    
 sub bx,[x2]            
 idiv bx                     
 mov w [CS:cTxAdder2],ax                  
; TyAdder = (ty3-ty2)/(x3-x2)
 mov ax,[ty3]                  
 sub ax,[ty2]           
 shl ax,8                       
 cwd                         
 idiv bx                     
 mov w [CS:cTyAdder2],ax
; LeftTxAdder = (tx2-tx1)/(y2-y1)
 mov ax,[tx2]           
 sub ax,[tx1]                
 shl ax,8                    
 cwd                         
 idiv [y2_minus_y1]     
 mov w [CS:cLeftTxAdder2],ax
; LeftTyAdder = (ty2-ty1)/(y2-y1)
 mov ax,[ty2]           
 sub ax,[ty1]                   
 shl ax,8                    
 cwd                         
 idiv [y2_minus_y1]     
 mov w [CS:cLeftTyAdder2],ax
; Left frac/int Adder = Slope12
 mov eax,[Slope12]      
 mov w [CS:cLeftFracAdder2],ax  
 shr eax,16                    
 mov w [CS:cLeftIntAdder2],ax   
; Right frac/int Adder = Slope13
 mov eax,[Slope13]      
 mov w [CS:cRightFracAdder2],ax
 shr eax,16             
 mov w [CS:cRightIntAdder2],ax
; AH = [y3_minus_y1]    
 mov ax,[y3_minus_y1]
 shl ax,8               
; EBP = 0:RightInt      
 xor ebp,ebp            
 mov bp,[x1]                    
 inc bp                 
; ESI = 0:LeftInt       
 xor esi,esi            
 mov si,[x1]                           
 Jmp cFiller2
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
;align 16             
cFlatRight:          
; 1                  
;2          
;  3  
; TextureIndex = tx1:0 : Ty1:0           
 mov ax,[tx1]                
 shl eax,16                    
 mov ax,[ty1]                
 shl eax,8                      
 mov d [CS:cTextureIndex1],eax
; Ypos320 = Y1
 mov ax,[y1]            
 mov w [CS:cYpos3201],ax
; TxAdder = tx1+(tx3-tx1)/(y3-y1)*(y2-y1) - tx2
;           -----------------------------------
;                Slope13-Slope12 * (y2-y1)         
 mov ax,[tx3]                  
 sub ax,[tx1]                  
 imul [y2_minus_y1]        
 idiv [y3_minus_y1]        
 add ax,[tx1]              
 sub ax,[tx2]          
 cwd     
 mov ebx,[Slope13]
 sub ebx,[Slope12]     
 movzx ecx,[y2_minus_y1]
 imul ebx,ecx      
 shr ebx,8       
 idiv bx         
 mov w [CS:cTxAdder1],ax
 mov w [CS:cTxAdder2],ax         
; TyAdder = (ty3-ty2)/(x3-x2)
 mov ax,[ty3]      
 sub ax,[ty1]      
 imul [y2_minus_y1]        
 idiv [y3_minus_y1]        
 add ax,[ty1]      
 sub ax,[ty2]      
 cwd               
 mov ebx,[Slope13] 
 sub ebx,[Slope12] 
 movzx ecx,[y2_minus_y1]
 imul ebx,ecx      
 shr ebx,8         
 idiv bx               
 mov w [CS:cTyAdder1],ax
 mov w [CS:cTyAdder2],ax
; LeftTxAdder1 = (tx2-tx1)/(y2-y1)
 mov ax,[tx2]      
 sub ax,[tx1]                
 shl ax,8                       
 cwd                         
 idiv [y2_minus_y1]
 mov w [CS:cLeftTxAdder1],ax
; LeftTxAdder2 = (tx3-tx2)/(y3-y2)
 mov ax,[tx3]          
 sub ax,[tx2]          
 shl ax,8                    
 cwd                         
 idiv [y3_minus_y2]
 mov w [CS:cLeftTxAdder2],ax
; LeftTyAdder1 = (ty2-ty1)/(y2-y1)
 mov ax,[ty2]          
 sub ax,[ty1]          
 shl ax,8                    
 cwd                         
 idiv [y2_minus_y1]    
 mov w [CS:cLeftTyAdder1],ax
; LeftTyAdder2 = (ty3-ty2)/(y3-y2)
 mov ax,[ty3]
 sub ax,[ty2]                   
 shl ax,8                    
 cwd                         
 idiv [y3_minus_y2]
 mov w [CS:cLeftTyAdder2],ax
; Left frac/int Adder 1 = Slope12
 mov eax,[Slope12]
 mov w [CS:cLeftFracAdder1],ax
 shr eax,16                    
 mov w [CS:cLeftIntAdder1],ax
; Left frac/int Adder 2 = Slope23
 mov eax,[Slope23]      
 mov w [CS:cLeftFracAdder2],ax  
 shr eax,16                    
 mov w [CS:cLeftIntAdder2],ax   
; Right frac/int Adder = Slope13
 mov eax,[Slope13]      
 mov w [CS:cRightFracAdder1],ax
 mov w [CS:cRightFracAdder2],ax
 shr eax,16               
 mov w [CS:cRightIntAdder1],ax   
 mov w [CS:cRightIntAdder2],ax
; AH = [y2_minus_y1]     
 mov ax,[y2_minus_y1]
 dec ax     
 shl ax,8                
; EBP = 0:RightInt       
 xor ebp,ebp             
 mov bp,[x1]             
 inc bp                  
; ESI = 0:LeftInt        
 xor esi,esi             
 mov si,[x1]             
 Jmp cFiller1             
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
;align 16    
cFlatLeft:                
; 1          1 
;2      ->    2
;  3        3  
; TextureIndex = tx1:0 : Ty1:0           
 mov ax,[tx1]                   
 shl eax,16                    
 mov ax,[ty1]                
 shl eax,8                      
 mov d [CS:cTextureIndex1],eax
; Ypos320 = Y1
 mov ax,[y1]            
 mov w [CS:cYpos3201],ax
; TxAdder = tx1+(tx3-tx1)/(y3-y1)*(y2-y1) - tx2
;           -----------------------------------
;                Slope13-Slope12 * (y2-y1)         
 mov ax,[tx3]                                  
 sub ax,[tx1]                                  
 imul [y2_minus_y1]                            
 idiv [y3_minus_y1]                            
 add ax,[tx1]                                  
 sub ax,[tx2]                                  
 cwd                                           
 mov ebx,[Slope13]                             
 sub ebx,[Slope12]                             
 movzx ecx,[y2_minus_y1]                       
 imul ebx,ecx                                  
 shr ebx,8                                     
 idiv bx                                       
 mov w [CS:cTxAdder1],ax                        
 mov w [CS:cTxAdder2],ax                        
; TyAdder = (ty3-ty2)/(x3-x2)
 mov ax,[ty3]      
 sub ax,[ty1]      
 imul [y2_minus_y1]        
 idiv [y3_minus_y1]        
 add ax,[ty1]      
 sub ax,[ty2]      
 cwd               
 mov ebx,[Slope13] 
 sub ebx,[Slope12] 
 movzx ecx,[y2_minus_y1]
 imul ebx,ecx                   
 shr ebx,8         
 idiv bx               
 mov w [CS:cTyAdder1],ax
 mov w [CS:cTyAdder2],ax
; LeftTxAdder = (tx3-tx1)/(y3-y1)
 mov ax,[tx3]
 sub ax,[tx1]                                  
 shl ax,8                                      
 cwd                         
 idiv [y3_minus_y1]
 mov w [CS:cLeftTxAdder1],ax
 mov w [CS:cLeftTxAdder2],ax
; LeftTyAdder = (ty3-ty1)/(y3-y1)
 mov ax,[ty3]
 sub ax,[ty1]          
 shl ax,8                    
 cwd                         
 idiv [y3_minus_y1]
 mov w [CS:cLeftTyAdder1],ax
 mov w [CS:cLeftTyAdder2],ax     
; Left frac/int Adder = Slope13
 mov eax,[Slope13]
 mov w [CS:cLeftFracAdder1],ax
 mov w [CS:cLeftFracAdder2],ax     
 shr eax,16                       
 mov w [CS:cLeftIntAdder1],ax      
 mov w [CS:cLeftIntAdder2],ax      
; Right frac/int Adder 1 = Slope12
 mov eax,[Slope12]
 mov w [CS:cRightFracAdder1],ax
 shr eax,16               
 mov w [CS:cRightIntAdder1],ax
; Right frac/int Adder 2 = Slope23
 mov eax,[Slope23]
 mov w [CS:cRightFracAdder2],ax
 shr eax,16               
 mov w [CS:cRightIntAdder2],ax
; AH = [y2_minus_y1]     
 mov ax,[y2_minus_y1]    
 dec ax                         
 shl ax,8                
; EBP = 0:RightInt       
 xor ebp,ebp             
 mov bp,[x1]             
 inc bp                  
; ESI = 0:LeftInt        
 xor esi,esi                      
 mov si,[x1]                      
 Jmp cFiller1                      
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÂÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ           
;EAX                          ³ PolyHeight:Temp                     
;EBX                          ³ Temp                                
;ECX                          ³ LineLength                          
;EDX TxInt:TxFrac (inner)     ³ TyInt:TyFrac (inner)                
;ESI RightFrac                ³ LeftInt                             
;EDI                          ³ ScreenPos (ES:SI)                   
;EBP LeftFrac                 ³ RightInt                            
;ESP                          ³                                     
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÁÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ           
Filler1:                        
 mov bx,fs                      
 mov ds,bx                      
;align 16
Outer1:                         
 mov edx,12345678h      ; TextureIndexes
TextureIndex1 = $-4                           
 mov di,1234h           ; Ypos
Ypos3201 = $-2                  
 add di,si              ; Left of Line
 mov cx,bp                          
 sub cx,si              ; LineLength
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ            
;align 16                            
Inner1:                             
 shld ebx,edx,8     ; BL = Tx       
 mov bh,dh          ; BX = Ty*256+Tx
 mov al,[BX]        ; AL = [DS:BX]
 add edx,12345678h  ; .. 78 56 34 12
TxAdder1 = $-2               
TyAdder1 = $-4               
 mov [es:di],al
 inc di     
 dec cx                
 jnz Inner1 
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
 add d [CS:TextureIndex1],12345678h      ; Left-Tx/Ty-adder
LeftTxAdder1 = $-2
LeftTyAdder1 = $-4
 add ebp,12340000h      ; LeftFrac
LeftFracAdder1 = $-2
 adc esi,12345678h      ; RightFrac/LeftInt ; .. 78 56 34 12
RightFracAdder1 = $-2
LeftIntAdder1 = $-4
 adc bp,1234h           ; RightInt
RightIntAdder1 = $-2
 add w [CS:Ypos3201],320 ; Ypos
 dec ah     
 jnz Outer1 
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
; Overf›r CodeKonstanter
 mov bx, w [CS:TextureIndex1]
 mov w [CS:TextureIndex2],bx
 mov bx, w [CS:Ypos3201]  
 mov w [CS:Ypos3202],bx   
 mov ah,b [CS:y3_minus_y2]
 jmp Filler2           
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
Filler2:
 mov bx,fs
 mov ds,bx
;align 16
Outer2: 
 mov edx,12345678h  ;esi ; EDX = TextureIndex
TextureIndex2 = $-4
 mov di,1234h   ; shld edi,ecx,16   ; DI = Ypos
Ypos3202 = $-2
 add di,si
 mov cx,bp   
 sub cx,si          ; CX = LineLength
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
;align 16
Inner2:                                                     
 shld ebx,edx,8     ; BL = Tx
 mov bh,dh          ; BX = Ty*256+Tx
 mov al,[BX]        ; AL = [DS:BX]
 add edx,12345678h  ; .. 78 56 34 12
TxAdder2 = $-2             
TyAdder2 = $-4           
 mov [es:di],al
 inc di    
 dec cx                  
 jnz Inner2
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
 add d [CS:TextureIndex2],12345678h      ; Left-Tx/Ty-adder
LeftTxAdder2 = $-2      
LeftTyAdder2 = $-4      
 add ebp,12340000h      ; LeftFrac ; ... 00 00 34 12
LeftFracAdder2 = $-2
 adc esi,12345678h      ; RightFrac/LeftInt ; .. 78 56 34 12
RightFracAdder2 = $-2
LeftIntAdder2 = $-4                        
 adc bp,1234h           ; RightInt
RightIntAdder2 = $-2    
 add w [CS:Ypos3202],320 ; Ypos
 dec ah   
 jnz Outer2            
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
QuitPoly:               
 pop ds 
QuitPoly_noDS: 
 ret    
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
;ALIGN 4                       
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
NeedClip dw ?         
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÂÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ           
;EAX                          ³ PolyHeight:Temp                     
;EBX                          ³ Temp                                
;ECX                          ³ LineLength                          
;EDX TxInt:TxFrac (inner)     ³ TyInt:TyFrac (inner)                
;ESI RightFrac                ³ LeftInt                             
;EDI                          ³ ScreenPos (ES:SI)                   
;EBP LeftFrac                 ³ RightInt                            
;ESP                          ³                                     
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÁÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ           
cFiller1:
 xor edi,edi
 mov bx,fs  
 mov ds,bx  
;align 16    
cOuter1:
 cmp si,319
 jg cSkipLine1
 cmp bp,0     
 jl cSkipLine1
 mov edx,12345678h      ; TextureIndexes
cTextureIndex1 = $-4
 mov di,1234h           ; Ypos
cYpos3201 = $-2
 cmp di,0             
 jl cSkipLine1
 cmp di,199           
 jg QuitPoly          
 lea edi,[edi+edi*4]  
 shl di,6               
              
 mov cx,319   
 cmp bp,319   
 jg @@c11
 mov cx,bp    
@@c11:
 cmp si,0     
 jl @@c12
 sub cx,si    
 add di,si    
@@c12:
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ            
;align 16                            
cInner1:                             
 shld ebx,edx,8     ; BL = Tx       
 mov bh,dh          ; BX = Ty*256+Tx
 mov al,[BX]        ; AL = [DS:BX]
 add edx,12345678h  ; .. 78 56 34 12
cTxAdder1 = $-2               
cTyAdder1 = $-4               
 mov [es:di],al       
 inc di               
 dec cx                
 jnz cInner1          
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
cSkipLine1:           
 add d [CS:cTextureIndex1],12345678h      ; Left-Tx/Ty-adder
cLeftTxAdder1 = $-2   
cLeftTyAdder1 = $-4   
 add ebp,12340000h      ; LeftFrac
cLeftFracAdder1 = $-2 
 adc esi,12345678h      ; RightFrac/LeftInt ; .. 78 56 34 12
cRightFracAdder1 = $-2
cLeftIntAdder1 = $-4  
 adc bp,1234h           ; RightInt
cRightIntAdder1 = $-2 
 inc w [CS:Ypos3201]    ; Ypos
 dec ah               
 jnz cOuter1          
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
; Overf›r CodeKonstanter
 mov bx, w [CS:cTextureIndex1]
 mov w [CS:cTextureIndex2],bx
 mov bx, w [CS:cYpos3201]  
 mov w [CS:cYpos3202],bx   
 mov ah,b [CS:y3_minus_y2]
 jmp cFiller2           
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
cFiller2:
 xor edi,edi
 mov bx,fs  
 mov ds,bx  
;align 16    
cOuter2:    
 cmp si,319
 jg cSkipLine2
 cmp bp,0
 jl cSkipLine2
 mov edx,12345678h      ; TextureIndexes
cTextureIndex2 = $-4
 mov di,1234h           ; Ypos
cYpos3202 = $-2
 cmp di,0             
 jl cSkipLine2
 cmp di,199           
 jg QuitPoly          
 lea edi,[edi+edi*4]  
 shl di,6               
              
 mov cx,319
 cmp bp,319   
 jg @@c21     
 mov cx,bp    
@@c21:        
 cmp si,0     
 jl @@c22     
 sub cx,si    
 add di,si    
@@c22:        
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
;align 16      
cInner2:                                                     
 shld ebx,edx,8     ; BL = Tx
 mov bh,dh          ; BX = Ty*256+Tx
 mov al,[BX]        ; AL = [DS:BX]
 add edx,12345678h  ; .. 78 56 34 12
cTxAdder2 = $-2             
cTyAdder2 = $-4           
 mov [es:di],al
 inc di    
 dec cx                  
 jnz cInner2
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
cSkipLine2:
 add d [CS:cTextureIndex2],12345678h      ; Left-Tx/Ty-adder
cLeftTxAdder2 = $-2      
cLeftTyAdder2 = $-4      
 add ebp,12340000h      ; LeftFrac ; ... 00 00 34 12
cLeftFracAdder2 = $-2
 adc esi,12345678h      ; RightFrac/LeftInt ; .. 78 56 34 12
cRightFracAdder2 = $-2
cLeftIntAdder2 = $-4                        
 adc bp,1234h           ; RightInt
cRightIntAdder2 = $-2    
 inc w [CS:cYpos3202]   ; Ypos
 dec ah     
 jnz cOuter2            
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
 pop ds  
 ret     
ENDP           


ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

               
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
; neg eax                ; hmmm... Sikker p† at denne skal v‘re med???        
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
 shl ax,3           
 mov w [CS:@@Finito],ax
RotLoop:
 movsx ebx,w [gs:esi+edx]
 movsx ecx,w [gs:esi+edx+2]
 movsx ebp,w [gs:esi+edx+4]
 push edx               
 push esi
 Call Rotate           
 pop esi
 pop edx                
 mov [Rotated+edx],bx
 mov [Rotated+edx+2],cx
 mov [Rotated+edx+4],bp 
 add dx,8              
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
 shl ax,3     
 mov w [CS:@@Finito],ax
RotLoop2:     
 movsx ebx,w [gs:esi+edx]
 movsx ecx,w [gs:esi+edx+2]
 movsx ebp,w [gs:esi+edx+4]
 push edx               
 push esi                
 Call Rotate            
 pop esi                 
 pop edx                 
 mov [RotatedVN+edx],bx
 mov [RotatedVN+edx+2],cx
 mov [RotatedVN+edx+4],bp
 add edx,8
 cmp dx,1234h      
@@Finito = $-2 
 jb RotLoop2           
 ret                                                                    
ENDP                   
                       
                       
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
                       
PROC ProjectObjCoords
 xor ebx,ebx
 mov ax,[NumCoords]
 shl ax,2      
 mov w [CS:@@Finito],ax
ProjectLoop:                      
 movsx eax,[Rotated+ebx+ebx]         ; x
 movsx ebp,[Rotated+ebx+ebx+2]       ; y
 movsx ecx,[Rotated+ebx+ebx+4]       ; z
 push ebx 
 add ecx,[Zoff]      
 shl eax,8                                        
 cdq                    
 idiv ecx               
 add eax,[Xoff]            ; eax = Projected X
 xchg eax,ebp           
 shl eax,8                                 
 cdq                                       
 idiv ecx                                  
 add eax,[Yoff]      
 xchg eax,ebp           ; ebp = Projected Y
 pop ebx                            
 mov w [TriCoords+ebx],bp
 mov w [TriCoords+ebx+2],ax
 add bx,4
 cmp bx,1234h          
@@Finito = $-2         
 jb ProjectLoop                   
 ret                              
ENDP           
                 
PROC ProjectNormals
 xor ebx,ebx
 mov ax,[NumCoords]
 shl ax,2
 mov w [CS:@@Finito],ax
ProjLoop:              
 mov ax,[RotatedVN+ebx+ebx]
 mov bp,[RotatedVN+ebx+ebx+2]
; mov cx,[RotatedVN+ebx*8+4]      
 add ax,63                
 add bp,63                
                          
 mov w [TexCoords+ebx],bp
 mov w [TexCoords+ebx+2],ax
 add bx,4
 cmp bx,1234h 
@@Finito = $-2 
 jb ProjLoop       
 ret                              
ENDP       
                                
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
                   
; SI = Coord set 1                                
; Di = Coord set 2         
; BP = Coord set 3 
; +0 = Y, +2 = X
; Returnerer Face Normal i EAX... negativ=synlig...
PROC CheckVisible           
; (x2-x1)*(y3-y1) - (x3-x1)*(y2-y1)
 mov ax,w [DS:TriCoords+di+2]        ; x2
 sub ax,w [DS:TriCoords+si+2]        ; x1             
 movsx eax,ax                                             
 mov bx,w [DS:TriCoords+bp  ]        ; y3             
 sub bx,w [DS:TriCoords+si  ]        ; y1             
 movsx ebx,bx                                             
 imul ebx       ; Blir denne IMUL'en noensinne over +/-32k ???
 mov ecx,eax    ; klarer jeg meg med 16-bits ??? Sjekk senere...
 mov ax,w [DS:TriCoords+bp+2]        ; x3             
 sub ax,w [DS:TriCoords+si+2]        ; x1             
 movsx eax,ax                                             
 mov bx,w [DS:TriCoords+di  ]        ; y2
 sub bx,w [DS:TriCoords+si  ]        ; y1
 movsx ebx,bx                                     
 imul ebx                                         
 sub ecx,eax     
 mov eax,ecx     
 ret                                     
ENDP                            
                                
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
                 
PROC SortSetup 
 mov ax,[treDseg]
 mov gs,ax
 xor edi,edi     
 mov di,[Polygon]
 mov ecx,0            
SSLoop1:         
 xor esi,esi     
                 
 mov si,[gs:EDI+ecx*8]         ; Coord set #1
 mov bx,[Rotated+esi*8+4]       ; Z1
 mov si,[gs:EDI+ecx*8+2]       ; Coord set #2
 add bx,[Rotated+esi*8+4]       ; Z2
 mov si,[gs:EDI+ecx*8+4]       ; Coord set #3
 add bx,[Rotated+esi*8+4]       ; Z3
                      
 Mov [DS:Face_z+ecx*2],bx
 mov [DS:FaceInd+ecx*2],cx
                     
 inc ecx                  
 cmp cx,[NumPolys]
 jb SSLoop1    
 ret                      
ENDP                      
                           
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
                                
PROC DoObject        
 mov ax,SEG Buffer   
 mov es,ax                                   
 mov ax,SEG Texture                 
 mov fs,ax                          
 Call RotateObjCoords
 Call RotateNormals                 
 Call ProjectObjCoords              
 Call ProjectNormals
                                    
 Call SortSetup
 Call ShellSort
                             
 xor edx,edx   
 mov dx,[NumPolys]
 dec dx      
 xor ebx,ebx 
 mov bx,[Polygon]
DrawObjLoop:                 
 movsx ecx,[FaceInd+edx*2]
 xor esi,esi                                                    
 xor edi,edi                                                    
 xor ebp,ebp                                                    
 mov si,[gs:ebx+ecx*8]         ; SI = Coord set #1
 shl si,2                       ; * 4 = Index in TriCoords      
 mov di,[gs:ebx+ecx*8+2]       ; DI = Coord set #2
 shl di,2                                                       
 mov bp,[gs:ebx+ecx*8+4]       ; BP = Coord set #3
 shl bp,2                                                       
 push edx      
 push ebx
; mov [CS:FargeAdd],0
 Call CheckVisible
 cmp eax,0
 jl Vanlig
; mov [CS:FargeAdd],32
; jge SkipThisPoly
Vanlig:          
 Call TmPoly
SkipThisPoly:       
 pop ebx       
 pop edx       
 dec edx           
 jns DrawObjLoop
 ret                 
ENDP                 
                     
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
               
Proc EnvMap FAR
 mov [saved_di],0
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
               
;° EntryPoint from DOS °°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
EntryPoint:                             
 mov ax,@DATA  
 mov ds,ax     
 mov ax,13h    
 int 10h       
 Call SetPalette
 Call DoObject1 
 mov ax,3      
 int 10h       
 mov ax,4c00h  
 int 21h       
End EntryPoint 