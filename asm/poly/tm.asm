; Generell TextureMapping Rutine
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
; BenchMark = 46.34 Sek = 21.580 polys/sek

;DOSSEG
.MODEL MEDIUM
.386
.STACK                    
;JUMPS                    
LOCALS                    
                          
b EQU byte ptr            
w EQU word ptr            
d EQU dword ptr           
                          
SEGMENT Buffer
 db 65536 dup(?)
ENDS 


SEGMENT texture
 include tex1.db
ENDS 

;° DATA °°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
DATA Segment Para Public Use16
                          
EnvMapSize = 32
Palette Label byte
i=0            
rept EnvmapSize
 db 0,i,i      
 i=i+2         
endm
rept 256-EnvMapSize
 db 63,0,0
endm 

MaxCoords = 512                 
ALIGN 4
TriCoords dw MaxCoords dup(?)
TexCoords DW MaxCoords dup(?)
                              
ENDS DATA                              
;° CODE °°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
CODE Segment Para Public Use16
Assume CS:CODE,DS:DATA
                              
PROC CalcTex                       
 cld        
 mov ax,SEG Texture
 mov es,ax         
 xor di,di         
; F›rst blank buffer, s† vi er sikker :)
 mov eax,0         
 mov cx,16384
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
 cmp cx,32
 jb @@1
 mov cx,0
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
             
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

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

ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

ALIGN 16
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
Align 16
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
ALIGN 16
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
ALIGN 16
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
 jz QuitPoly
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
ALIGN 16
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
 jz QuitPoly
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
Align 16 
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
ALIGN 16    
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
ALIGN 16             
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
 jz QuitPoly
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
ALIGN 16    
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
 jz QuitPoly
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
ALIGN 16
Outer1:                         
 mov edx,12345678h      ; TextureIndexes
TextureIndex1 = $-4                           
 mov di,1234h           ; Ypos
Ypos3201 = $-2                  
 add di,si              ; Left of Line
 mov cx,bp                          
 sub cx,si              ; LineLength
 jz SkipLine1
 js SkipLine1
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ            
Align 16                            
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
SkipLine1:
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
ALIGN 16
Outer2: 
 mov edx,12345678h  ;esi ; EDX = TextureIndex
TextureIndex2 = $-4
 mov di,1234h   ; shld edi,ecx,16   ; DI = Ypos
Ypos3202 = $-2
 add di,si
 mov cx,bp   
 sub cx,si          ; CX = LineLength
 jz SkipLine2
 js SkipLine2
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
ALIGN 16  
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
SkipLine2:
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
 inc ah     
 jnz Outer2            
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
QuitPoly:               
 pop ds 
QuitPoly_noDS: 
 ret    
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
ALIGN 4                       
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
ALIGN 16    
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
 add di,si    
 sub cx,si
@@c12:
 cmp cx,0
 jz cSkipLine1
 js cSkipLine1
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ            
Align 16                            
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
ALIGN 16    
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
 add di,si
 sub cx,si
@@c22:
 cmp cx,0
 jz cSkipLine2
 js cSkipLine2
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
ALIGN 16      
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
PROC Testing
 mov ax,13h
 int 10h
 Call SetPalette
 Call CalcTex   
 mov w [TriCoords+2 ],130
 mov w [TriCoords   ],150
 mov w [TriCoords+6 ],100
 mov w [TriCoords+4 ],100
 mov w [TriCoords+10],150
 mov w [TriCoords+8 ],100
                
 mov w [TexCoords+2 ],10  
 mov w [TexCoords   ],10  
 mov w [TexCoords+6 ],10  
 mov w [TexCoords+4 ],127
 mov w [TexCoords+10],127
 mov w [TexCoords+8 ],127
                
TestingLoop:    
 mov ax,0a000h  
 mov es,ax      
 xor di,di      
 mov cx,64000   
 cld            
 mov al,31      
 rep stosb      
 mov si,0                
 mov di,4                
 mov bp,8
 mov ax,SEG Texture     
 mov fs,ax  
 Call TmPoly
 mov ah,0
 int 16h

 cmp al,'q'
 jne @@1
 dec [TriCoords+2] 
@@1:             
 cmp al,'w'      
 jne @@2         
 inc [TriCoords+2] 
@@2:             
 cmp al,'e'      
 jne @@3 
 dec [TriCoords]
@@3:     
 cmp al,'r'      
 jne @@4 
 inc [TriCoords]
@@4:     
         
 cmp al,'a'
 jne @@5  
 dec [TriCoords+6]
@@5:             
 cmp al,'s'      
 jne @@6         
 inc [TriCoords+6]
@@6:             
 cmp al,'d'      
 jne @@7         
 dec [TriCoords+4] 
@@7:             
 cmp al,'f'      
 jne @@8         
 inc [TriCoords+4] 
@@8:             
                 
 cmp al,'z'
 jne @@9         
 dec [TriCoords+10]
@@9:              
 cmp al,'x'       
 jne @@10         
 inc [TriCoords+10]
@@10:             
 cmp al,'c'       
 jne @@11         
 dec [TriCoords+8]
@@11:            
 cmp al,'v'      
 jne @@12        
 inc [TriCoords+8] 
@@12:    
         



 cmp al,27
 jne TestingLoop
 mov ax,3
 int 10h
 ret
ENDP    
                
PROC Visible    
 mov ax,13h     
 int 10h        
 Call SetPalette
 mov ax,0a000h  
 mov es,ax      
 xor di,di      
 mov cx,64000   
 cld            
 mov al,31      
 rep stosb      
 Call CalcTex   
                
 mov w [TriCoords+2 ],10
 mov w [TriCoords   ],10  
 mov w [TriCoords+6 ],310
 mov w [TriCoords+4 ],10
 mov w [TriCoords+10],160
 mov w [TriCoords+8 ],260

 mov w [TexCoords+2 ],10  
 mov w [TexCoords   ],10  
 mov w [TexCoords+6 ],10  
 mov w [TexCoords+4 ],127
 mov w [TexCoords+10],127
 mov w [TexCoords+8 ],127
 mov si,0                
 mov di,4                
 mov bp,8                
 mov ax,0a000h         
 mov es,ax             
 mov ax,SEG Texture
 mov fs,ax
 Call TmPoly           

 mov ah,0
 int 16h
 mov ax,3
 int 10h
 ret
ENDP           
               
PROC BenchMark 
 mov w [TriCoords+2 ],100
 mov w [TriCoords   ],100
 mov w [TriCoords+6 ],100
 mov w [TriCoords+4 ],110
 mov w [TriCoords+10],110 
 mov w [TriCoords+8 ],110
 mov w [TexCoords+2 ],0
 mov w [TexCoords   ],0  
 mov w [TexCoords+6 ],0  
 mov w [TexCoords+4 ],127
 mov w [TexCoords+10],127
 mov w [TexCoords+8 ],127
               
 mov ax,SEG Buffer
 mov es,ax             
 mov ax,SEG Texture
 mov fs,ax     
 mov ecx,1000000
TestLoop:      
 push ecx      
 mov si,0                
 mov di,4                
 mov bp,8              
 Call TmPoly
 pop ecx
 dec ecx
 jnz TestLoop
 ret
ENDP 
 
 
 
;° EntryPoint from DOS °°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
EntryPoint:    
 mov ax,SEG DATA
 mov ds,ax   
             
 Call Testing
; Call Visible
; Call BenchMark
         
 mov ax,4c00h
 int 21h 
ENDS CODE
End EntryPoint


