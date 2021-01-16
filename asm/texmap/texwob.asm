DOSSEG
.MODEL MEDIUM
.386
.STACK
;JUMPS
LOCALS

;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
 
b equ byte pre
w EQU word ptr
d EQU dword ptr


Segment TextureSeg
Label Texture byte
; include texture.inc
ends          
              
Segment BufferSeg         
Label Buffer byte 
; db 65536 dup(0) 
ends                     

MaxPolys = 256
MaxCoords = 256

cx1 = 80
cy1 = 50
cx2 = 240
cy2 = 50
cx3 = 160
cy3 = 100
cx4 = 80 
cy4 = 150
cx5 = 240
cy5 = 150

;° DATA °°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
.DATA
            
saved_di dw ?

Palette label byte
 include palette.db
            
; Tabell med 255 verdier fra -20 til 20?
Sinus Label word
; include sinus.db
CoSinus Label word
; include cosinus.db
                 
; Posisjoner i Sinus-tabellen
s1 db ?                      
s2 db ?                      
s3 db ?                      
s4 db ?                      
s5 db ?                      
               
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
; Texture Mapping variables
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
TriCoords dd MaxCoords dup (?)  ; y/x
TexCoords dd MaxCoords*2 dup(?) ;Trenger vel ikke *2 ????????
                             
;° CODE °°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
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
            
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
;Parameters:
; Si,Di,BP : offsets into TriCoords & TexCoords table
; ES = Buffer to draw to
; FS = Texture
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
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
; F›rst sjekker vi om vi skal kj›re Clip eller NoClip versjonen...
 cmp [CS:NeedClip],0
 jne DoClip  
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
; Nei - ingen clipping trengs - s† vi kj›rer rutine uten clipping,
; som er raskere...
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
; Jmp DoClip
NoClip: 
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
; Vi sjekker for hvilket, og utf›rer relevant polygon-filler rutine
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
; Det var konstantene, n† kommer registrene... phew...
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
; Det var konstantene, n† kommer registrene... phew...
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
; Det var konstantene, n† kommer registrene... phew...
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
; Det var konstantene, n† kommer registrene... phew...
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
DoClip:      
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
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
 Jmp FLatLeft                   
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
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
; Det var konstantene, n† kommer registrene... phew...
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
; Det var konstantene, n† kommer registrene... phew...
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
; Det var konstantene, n† kommer registrene... phew...
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
; Det var konstantene, n† kommer registrene... phew...
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
; Jepp, n† kommer selve fillerne.. To stykker, en for ›ver 1/2-del og  
; en for nedre...                                                      
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ  
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
ncOuter1:    
 lea edi,[edx+edx*4]
 shl di,6               
 mov bp,si  
 sub bp,ax  
 add di,ax     
 push ecx   
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
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
ncOuter2:                       
 lea edi,[edx+edx*4]
 shl di,6 
 mov bp,si
 sub bp,ax
 add di,ax
 push ecx
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ                         
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

PROC DrawAll
 mov ax,SEG Buffer
 mov es,ax
 mov si,0
 mov di,4
 mov bp,8
@@1:
 push cx
 Call TmTri
 pop cx
 add si,12
 add di,12
 add bp,12
 dec cx
 jnz @@1
 ret
ENDP 
    
 ret
ENDP 
                    
     
    
;° EntryPoint from DOS °°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
EntryPoint:                  
 mov ax,@data                
 mov ds,ax
 mov ax,0a000h
 mov es,ax
 mov ax,SEG Texture
 mov fs,ax

 mov ax,4c00h
 int 21h
End EntryPoint
