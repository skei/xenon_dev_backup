; 3D Demo... :)
; No design, no excuses. Like it or hate it... Had to release..
; Norway is capable of producing decent 3D demos, too ;)
                                                        
.486p
.MODEL FLAT
.STACK     
;JUMPS     
LOCALS
.CODE

b equ byte ptr
w equ word ptr
d equ dword ptr

;° DATA °°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
Screen dd ?

; I register-kalkuleringen - y*960 herfra... S†, Allokerer vi et 960*600
; buffer, lager by CopyBufferToScreen som henter 320 bytes "strimler"
; til skjerm... Vips - Kjappeste clipping som finnes!!! :)
; Mul960[y] + ScreenBuffer = Start p†  linje i bufferet
i=0                        
rept 200          
 dd (i*960)+320
 i=i+1            
endm              
Mul960 label dword
rept 400  
 dd (i*960)+320
 i=i+1            
endm              
           
MaxCoords = 512
MaxPolys = 1024
                           
nc1 dd 352
np1 dd 671
include cat.inc;cat.inc
                        
Texture label byte      
 include tex_.db;texture.db;kule.db
Palette label byte            
 include tex_pal.db;palette.db;kulepal.db
Bump Label word               
; include bumpmap.db
 dw  65536 dup(0)
Buffer dd ?                   
                              
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
; Texture Mapping variables   
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
Xoff dd 160      ; Variables for 3D->2D transformation
Yoff dd 100                   
zoff dd 900                   
                              
TriCoords dd MaxCoords dup (?)
TexCoords dd MaxCoords dup (?) ;Trenger vel ikke *2 ????????
BumCoords1 dd MaxCoords dup (?)
vx dd ?               
vy dd ?                            
vz dd ?                    
divd = 14
                           
NumCoords dd ?             
NumPolys dd ?              
CoordData dd ?             
NormalData dd ?            
PolygonData dd ?           
                           
ZCoords dw MaxCoords dup(?)

;° CODE °°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
    
; ESI -> Palette
PROC SetPalette         
 cld
 mov dx,3c8h
 xor al,al
 out dx,al       
 inc dx            
 mov cx,768      
@@1:
 mov al,[esi]
 inc esi
 out dx,al
 dec cx
 jnz @@1
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

PROC BumpTri
 mov eax,[buffer]
 mov d [FillerScreen],eax
 
 mov eax,d [TriCoords+esi] 
 mov ebx,d [TriCoords+edi]              
 mov ecx,d [TriCoords+ebp] 
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
 mov [NeedClip],dx   
 cmp dx,-3      ; Alle X'ene er over 320, s†,,QUIT
 je QuitPoly         
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
; Sjekk om vi trenger † clippe Y
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
 mov dx,200          
 cmp dx,ax      
 sbb [NeedClip],0                     
 cmp dx,bx           
 sbb [NeedClip],0    
 cmp dx,cx                      
 sbb [NeedClip],0
; Sort‚r                        
 cmp ax,bx      
 jl @@1                                            
 xchg eax,ebx                   
 xchg esi,edi                     
@@1:                               
 cmp ax,cx                      
 jl @@2                         
 xchg eax,ecx                   
 xchg esi,ebp       
@@2:                    
 cmp bx,cx        
 jl @@3           
 xchg ebx,ecx     
 xchg edi,ebp       
@@3:                                  
; Y-ene i riktig rekkef›lge... N† sjekk X'er
 cmp ax,bx       ;Y1=Y2 ? Is†fall, sjekk om X'ene m† swappes
 jne @@4         
 cmp eax,ebx                    
 jl @@4   
 xchg eax,ebx    
 xchg esi,edi              
@@4:            
 cmp bx,cx       ;Y2=Y3 ? Is†fall, sjekk om X'ene m† swappes
 jne @@5               
 cmp ebx,ecx          
 jl @@5              
 xchg ebx,ecx    
 xchg edi,ebp      
@@5:                  
; 1     1     1     1 2
;  2   2     2 3     3  
;3       3       
; N† burde de v‘re sortert etter Y...
 cmp ax,199                           
 jg QuitPoly
 cmp cx,0        
 jl QuitPoly
 mov d [y1],eax
 mov d [y2],ebx
 mov d [y3],ecx
 mov eax,d [TexCoords+esi]
 mov ebx,d [TexCoords+edi]
 mov ecx,d [TexCoords+ebp]
 mov d [ty1],eax
 mov d [ty2],ebx      
 mov d [ty3],ecx     
 mov eax,d [BumCoords1+esi]
 mov ebx,d [BumCoords1+edi]
 mov ecx,d [BumCoords1+ebp]
 mov d [by1],eax
 mov d [by2],ebx      
 mov d [by3],ecx     
          
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
 movsx ebx,[y2_minus_y1]                 
 imul ebx                                
 shld ebp,eax,16                         
 add bp,[x1]                             
 cmp bp,[x2]            ; bp = x4        
 je QuitPoly                             
 mov [x4],bp            ; *************  
; Tx4 = tx1 + Txs13*(y2-y1)              
 mov ax,[tx3]                            
 sub ax,[tx1]                            
 shl ax,8
 imul [y2_minus_y1]                      
 idiv [y3_minus_y1]                      
 mov si,[tx1] 
 shl si,8     
 add ax,si           ; si = tx4          
 mov [tx4],ax        ; ************** 8.8
; Ty4 = ty1 + Tys13*(y2-y1)              
 mov ax,[ty3]                            
 sub ax,[ty1]                            
 shl ax,8                                
 imul [y2_minus_y1]                      
 idiv [y3_minus_y1]                      
 mov si,[ty1]   
 shl si,8       
 add ax,si         
 mov [ty4],ax           ; *****************
; Bx4 = bx1 + Bxs13*(y2-y1)
 mov ax,[bx3]
 sub ax,[bx1]
 shl ax,8                                
 imul [y2_minus_y1]                      
 idiv [y3_minus_y1]                      
 mov si,[bx1]
 shl si,8     
 add ax,si           ; si = tx4          
 mov [bx4],ax        ; ************** 8.8
; By4 = by1 + Bys13*(y2-y1)
 mov ax,[by3]
 sub ax,[by1]
 shl ax,8                                
 imul [y2_minus_y1]                      
 idiv [y3_minus_y1]                      
 mov si,[by1]
 shl si,8       
 add ax,si         
 mov [by4],ax           ; *****************
Skip4:                                   
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
 Jmp FlatLeft
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
FlatTop:               
; 1 2                
;  3       
; F›rst konstantene til Code-segmentet...
; LeftXAdder = Slope13
 mov eax,[Slope13]
 rol eax,16                                            
; add ax,320
 mov w [LeftIntAdder],ax
 rol eax,16
 mov w [LeftFracAdder],ax
; RightXAdder = Slope23
 mov eax,[Slope23]     
 rol eax,16            
; add ax,320            
 mov w [RightIntAdder],ax
 rol eax,16             
 mov w [RightFracAdder],ax
; LeftTxAdder = (tx3-tx1)/(y3-y1) 8.8
 mov ax,[tx3]           
 sub ax,[tx1]                                          
 shl ax,8               
 cwd                    
 idiv [y3_minus_y1]                      
 mov w [LeftTxAdder],ax
; LeftTyAdder = (ty3-ty1)/(y3-y1) 8.8
 mov ax,[ty3]           
 sub ax,[ty1]                     
 shl ax,8                                              
 cwd                                
 idiv [y3_minus_y1]     
 mov w [LeftTyAdder],ax 
; LeftBxAdder = (bx3-bx1)/(y3-y1) 8.8
 mov ax,[bx3]
 sub ax,[bx1]
 shl ax,8               
 cwd                    
 idiv [y3_minus_y1]                      
 mov w [LeftBxAdder],ax
; LeftByAdder = (by3-by1)/(y3-y1) 8.8
 mov ax,[by3]          
 sub ax,[by1]
 shl ax,8                                              
 cwd                                
 idiv [y3_minus_y1]     
 mov w [LeftByAdder],ax
             
; TxAdder = (tx2-tx1)/(x2-x1)
 mov ax,[tx2]           
 sub ax,[tx1]           
 shl ax,8              
 cwd                            
 mov bx,[x2]           
 sub bx,[x1]            
 jz QuitPoly                    
 idiv bx                        
 mov w [TxAdder],ax    
; TyAdder = (ty2-ty1)/(x2-x1)
 mov ax,[ty2]                            
 sub ax,[ty1]          
 shl ax,8                       
 cwd                            
 idiv bx                        
 mov w [TyAdder],ax    
; BxAdder = (bx2-bx1)/(x2-x1)
 mov ax,[bx2]
 sub ax,[bx1]
 shl ax,8              
 cwd                            
 mov bx,[x2]           
 sub bx,[x1]            
 jz QuitPoly                    
 idiv bx                        
 mov w [BxAdder],ax
; ByAdder = (by2-by1)/(x2-x1)
 mov ax,[by2]
 sub ax,[by1]
 shl ax,8                       
 cwd                            
 idiv bx                        
 mov w [ByAdder],ax    
; Det var konstantene, n† kommer registrene... phew...
; EAX = TXPos:0 | TYPos:0
 mov ax,[Tx1]
 shl eax,16                              
 mov ax,[Ty1]                            
 shl eax,8                               
; ECX = BXPos:0 | BYPos:0
 mov cx,[Bx1]
 shl ecx,16
 mov cx,[By1]
 shl ecx,8
; EDX = 0 : LeftX               
 movzx edx,[x1]  
; FillerScreen = (y*320) + [Buffer]
 movsx ebx,[y1]  
 mov ebx,[mul960+ebx*4]
 add ebx,[Buffer]
 mov d [FillerScreen],ebx
; EBP = 0 : RightX
 movzx ebp,[x2]
 inc ebp
; ESI = Height : -         
 movzx esi,[y3_minus_y1]
 mov [PolyHeight],esi
          
 JMP Filler                
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
FlatBottom:                                
; 1                    
;2 3                   
; F›rst konstantene til Code-segmentet...
; LeftXAdder = Slope12 
 mov eax,[Slope12]     
 rol eax,16                                            
; add ax,320       
 mov w [LeftIntAdder],ax
 rol eax,16       
 mov w [LeftFracAdder],ax
; RightXAdder = Slope13
 mov eax,[Slope13]
 rol eax,16            
; add ax,320            
 mov w [RightIntAdder],ax
 rol eax,16             
 mov w [RightFracAdder],ax
; LeftTxAdder = (tx2-tx1)/(y2-y1) 8.8
 mov ax,[tx2]     
 sub ax,[tx1]                                          
 shl ax,8                    
 cwd                    
 idiv [y2_minus_y1]
 mov w [LeftTxAdder],ax 
; LeftTyAdder = (ty2-ty1)/(y2-y1) 8.8
 mov ax,[ty2]                                      
 sub ax,[ty1]                     
 shl ax,8                                              
 cwd                                
 idiv [y2_minus_y1]
 mov w [LeftTyAdder],ax      
; LeftBxAdder = (bx2-bx1)/(y2-y1) 8.8
 mov ax,[bx2]          
 sub ax,[bx1]
 shl ax,8                    
 cwd                    
 idiv [y2_minus_y1]
 mov w [LeftBxAdder],ax
; LeftByAdder = (by2-by1)/(y2-y1) 8.8
 mov ax,[by2]
 sub ax,[by1]
 shl ax,8                                              
 cwd                                
 idiv [y2_minus_y1]
 mov w [LeftByAdder],ax
; TxAdder = (tx3-tx2)/(x3-x2)
 mov ax,[tx3]
 sub ax,[tx2]     
 shl ax,8                       
 cwd                            
 mov bx,[x3]      
 sub bx,[x2]      
 jz QuitPoly                    
 idiv bx                        
 mov w [TxAdder],ax
; TyAdder = (ty3-ty2)/(x3-x2)
 mov ax,[ty3]     
 sub ax,[ty2]
 shl ax,8                       
 cwd                                               
 idiv bx                        
 mov w [TyAdder],ax
; BxAdder = (bx3-bx2)/(x3-x2)
 mov ax,[bx3]
 sub ax,[bx2]     
 shl ax,8                       
 cwd                            
 mov bx,[x3]      
 sub bx,[x2]      
 jz QuitPoly                    
 idiv bx                        
 mov w [BxAdder],ax
; ByAdder = (by3-by2)/(x3-x2)
 mov ax,[by3]
 sub ax,[by2]
 shl ax,8                       
 cwd                                               
 idiv bx                        
 mov w [ByAdder],ax
; Det var konstantene, n† kommer registrene... phew...
; EAX = TXPos:0 | TYPos:0
 mov ax,[Tx1]
 shl eax,16                              
 mov ax,[Ty1]                            
 shl eax,8                               
; ECX = BXPos:0 | BYPos:0
 mov cx,[Bx1]
 shl ecx,16
 mov cx,[By1]
 shl ecx,8
; EDX = 0 : LeftX               
 movzx edx,[x1]  
; FillerScreen = (y*320) + [Buffer]
 movsx ebx,[y1]        
 mov ebx,[Mul960+ebx*4]
 add ebx,[Buffer]      
 mov d [FillerScreen],ebx
; EBP = 0 : RightX  
 mov ebp,edx
 inc ebp
; ESI = Height : -         
 movzx esi,[y3_minus_y1]
 mov [PolyHeight],esi
 JMP Filler                
FlatLeft:                  
; 1                    
;  2                   
;3                     
; F›rst konstantene til Code-segmentet...
; LeftXAdder = Slope13 
 mov eax,[Slope13]     
 rol eax,16                                            
; add ax,320      
 mov w [LeftIntAdder],ax
 rol eax,16      
 mov w [LeftFracAdder],ax
; RightXAdder = Slope12
 mov eax,[Slope12]           
 rol eax,16            
; add ax,320            
 mov w [RightIntAdder],ax    
 rol eax,16             
 mov w [RightFracAdder],ax
; LeftTxAdder = (tx3-tx1)/(y3-y1) 8.8
 mov ax,[tx3]
 sub ax,[tx1]
 shl ax,8                    
 cwd                    
 idiv [y3_minus_y1]
 mov w [LeftTxAdder],ax 
; LeftTyAdder = (ty3-ty1)/(y3-y1) 8.8
 mov ax,[ty3]                
 sub ax,[ty1]                     
 shl ax,8                                              
 cwd                                
 idiv [y3_minus_y1]          
 mov w [LeftTyAdder],ax      
; LeftBxAdder = (bx3-bx1)/(y3-y1) 8.8
 mov ax,[bx3]
 sub ax,[bx1]
 shl ax,8                    
 cwd                    
 idiv [y3_minus_y1]
 mov w [LeftBxAdder],ax
; LeftByAdder = (by3-by1)/(y3-y1) 8.8
 mov ax,[by3]
 sub ax,[by1]
 shl ax,8                                              
 cwd                                
 idiv [y3_minus_y1]          
 mov w [LeftByAdder],ax
; TxAdder = (tx2-tx4)/(x2-x4)
 mov ax,[tx2]
 shl ax,8
 sub ax,[tx4]
 cwd                            
 mov bx,[x2]
 sub bx,[x4]
 jz QuitPoly                    
 idiv bx                        
 mov w [TxAdder],ax
; TyAdder = (ty2-ty4)/(x2-x4)
 mov ax,[ty2]               
 shl ax,8                       
 sub ax,[ty4]               
 cwd                                               
 idiv bx                        
 mov w [TyAdder],ax         
; BxAdder = (bx2-bx4)/(x2-x4)
 mov ax,[bx2]
 shl ax,8              
 sub ax,[bx4]
 cwd                            
 mov bx,[x2]
 sub bx,[x4]
 jz QuitPoly                    
 idiv bx                        
 mov w [BxAdder],ax
; ByAdder = (by2-by4)/(x2-x4)
 mov ax,[by2]
 shl ax,8                       
 sub ax,[by4]
 cwd                                               
 idiv bx                        
 mov w [ByAdder],ax
; Det var konstantene, n† kommer registrene... phew...
; EAX = TXPos:0 | TYPos:0   
 mov ax,[Tx1]               
 shl eax,16                              
 mov ax,[Ty1]                            
 shl eax,8                               
; ECX = BXPos:0 | BYPos:0
 mov cx,[Bx1]
 shl ecx,16
 mov cx,[By1]
 shl ecx,8 
; EDX = 0 : LeftX               
 movzx edx,[x1]  
; FillerScreen = (y*320) + [Buffer]
 movsx ebx,[y1]        
 mov ebx,[Mul960+ebx*4]
 add ebx,[Buffer]
 mov d [FillerScreen],ebx
; EBP = 0 : RightX 
 mov ebp,edx
 inc ebp
; ESI = Height : -         
 movzx esi,[y2_minus_y1]
 mov [PolyHeight],esi
 Call Filler
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
; RightXAdder = Slope23
 push eax
; push ecx
 mov eax,[Slope23]
 rol eax,16            
; add ax,320
 mov w [RightIntAdder],ax    
 rol eax,16             
 mov w [RightFracAdder],ax
; pop ecx  
 pop eax             
; ESI = Height : -          
 movzx esi,[y3_minus_y2]
 mov [PolyHeight],esi  
 Jmp Filler            
           
FlatRight:           
; 1                    
;2                   
;  3                 
; F›rst konstantene til Code-segmentet...
; LeftXAdder = Slope12
 mov eax,[Slope12]
 mov w [LeftFracAdder],ax
 rol eax,16              
; add ax,320              
 mov w [LeftIntAdder],ax 
; RightXAdder = Slope13
 mov eax,[Slope13]   
 mov w [RightFracAdder],ax
 rol eax,16               
; add ax,320               
 mov w [RightIntAdder],ax    
; LeftTxAdder = (tx2-tx1)/(y2-y1) 8.8
 mov ax,[tx2]     
 sub ax,[tx1]     
 shl ax,8
 cwd                    
 idiv [y2_minus_y1]
 mov w [LeftTxAdder],ax 
; LeftTyAdder = (ty2-ty1)/(y2-y1) 8.8
 mov ax,[ty2]                
 sub ax,[ty1]                     
 shl ax,8
 cwd                                
 idiv [y2_minus_y1]          
 mov w [LeftTyAdder],ax      
; LeftBxAdder = (bx2-bx1)/(y2-y1) 8.8
 mov ax,[bx2]
 sub ax,[bx1]
 shl ax,8 
 cwd                    
 idiv [y2_minus_y1]
 mov w [LeftBxAdder],ax
; LeftByAdder = (by2-by1)/(y2-y1) 8.8
 mov ax,[by2]
 sub ax,[by1]
 shl ax,8              
 cwd                                
 idiv [y2_minus_y1]          
 mov w [LeftByAdder],ax
; TxAdder = (tx4-tx2)/(x4-x2)
 mov ax,[tx4]
 mov bx,[tx2]
 shl bx,8
 sub ax,bx
 cwd         
 mov bx,[x4] 
 sub bx,[x2] 
 jz QuitPoly                    
 idiv bx                        
 mov w [TxAdder],ax
; TyAdder = (ty4-ty2)/(x4-x2)
 mov ax,[ty4]
 mov cx,[ty2]
 shl cx,8
 sub ax,cx 
 cwd                   
 idiv bx                        
 mov w [TyAdder],ax         
; BxAdder = (bx4-bx2)/(x4-x2)
 mov ax,[bx4]
 mov bx,[bx2]
 shl bx,8 
 sub ax,bx
 cwd         
 mov bx,[x4] 
 sub bx,[x2] 
 jz QuitPoly                    
 idiv bx                        
 mov w [BxAdder],ax
; ByAdder = (by4-by2)/(x4-x2)
 mov ax,[by4]
 mov cx,[by2]
 shl cx,8 
 sub ax,cx 
 cwd      
 idiv bx                        
 mov w [ByAdder],ax
; Det var konstantene, n† kommer registrene... phew...
; EAX = TXPos:0 | TYPos:0   
 mov ax,[Tx1]               
 shl eax,16                              
 mov ax,[Ty1]                            
 shl eax,8                               
; ECX = CXPos:0 | CYPos:0
 mov cx,[Bx1]
 shl ecx,16
 mov cx,[By1]
 shl ecx,8        
; EDX = 0 : LeftX               
 movzx edx,[x1]
; FillerScreen = (y*320) + [Buffer]
 movsx ebx,[y1]        
 mov ebx,[Mul960+ebx*4]
 add ebx,[Buffer]
 mov d [FillerScreen],ebx
; EBP = 0 : RightX
 mov ebp,edx
 inc ebp
; ESI = Height : -         
 movzx esi,[y2_minus_y1]
 mov [PolyHeight],esi
 Call Filler 
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
 push eax
 push ecx
 push edx    
; LeftXAdder = Slope23
 mov eax,[Slope23]
 mov w [LeftFracAdder],ax
 rol eax,16                                            
; add ax,320           
 mov w [LeftIntAdder],ax
; LeftTxAdder = (tx3-tx2)/(y3-y2) 8.8
 mov ax,[tx3]
 sub ax,[tx2]
 shl ax,8
 cwd                    
 idiv [y3_minus_y2]    
 mov w [LeftTxAdder],ax 
; LeftTyAdder = (ty3-ty2)/(y3-y2) 8.8
 mov ax,[ty3]                   
 sub ax,[ty2]
 shl ax,8 
 cwd                                
 idiv [y3_minus_y2]
 mov w [LeftTyAdder],ax      
; LeftBxAdder = (bx3-bx2)/(y3-y2) 8.8
 mov ax,[bx3]
 sub ax,[bx2]    
 shl ax,8  
 cwd                    
 idiv [y3_minus_y2]
 mov w [LeftBxAdder],ax
; LeftTyAdder = (ty3-ty2)/(y3-y2) 8.8
 mov ax,[by3]
 sub ax,[by2]
 shl ax,8 
 cwd                                
 idiv [y3_minus_y2]
 mov w [LeftByAdder],ax
             
 pop edx  
 pop ecx  
 pop eax     
; ESI = Height : -          
; ESI = Height : -         
 movzx esi,[y3_minus_y2]
 mov [PolyHeight],esi  
 Jmp Filler            
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
QuitPoly:                
 ret                    
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
y1 dw ?                          
x1 dw ?                                    
y2 dw ?                          
x2 dw ?                          
y3 dw ?                          
x3 dw ?                

x4 dw ?                    

ty1 dw ?                         
tx1 dw ?                         
ty2 dw ?                         
tx2 dw ?                         
ty3 dw ?                         
tx3 dw ?                         
ty4 dw ?
tx4 dw ?   
 
by1 dw ?   
bx1 dw ?
by2 dw ?
bx2 dw ?                                          
by3 dw ?
bx3 dw ?
by4 dw ?
bx4 dw ?               

Slope12 dd ?                     
Slope13 dd ?                     
Slope23 dd ?                     
y2_minus_y1 dw ?                 
y3_minus_y1 dw ?                 
y3_minus_y2 dw ?                 
Parts db ?      ; Bit 1 = HasTop, Bit 2 = HasBottom
NeedClip dw ?
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
Filler:    
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
;EAX *  Xint:XFrac    : YInt:YFrac
;EBX    temp.......................               
;ECX *  BXInt:BXFrac  : BYInt:BYFrac
;EDX *  RightFrac     : LeftInt (+y*320)
;ESI                  : temp
;EDI    Screen-pos...............      
;EBP *  LeftFrac      : RightInt (+y*320)
; [PolyHeight]         
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
Outer:                                   
 push bp                ; Save RightInt  
 sub bp,dx              ; Right - Left = Line-length
; js SkipLine           ; Blir vel aldri Negativ?
 jz SkipLine                             
                                         
 movzx edi,dx
 add edi,12345678h      ; SelfMod code - sett inn verdien direkte her!
FillerScreen = $-4          
 push eax               ; Save Texture index
 push ecx               ; Save bump index
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ                    
Inner:                                      
 xor ebx,ebx
 shld ebx,ecx,8
 mov bh,ch
 mov esi,ebx            ; Bump-Map Index
                                             
 xor ebx,ebx                            
 shld ebx,eax,8                         
 mov bh,ah          ; EBX = Texture Index
 add bx,[Bump+esi*2]
                       
 add eax,12345678h      ; .. 78 56 34 12
TXAdder = $-2                      
TYAdder = $-4          
 add ecx,12345678h      ; .. 78 56 34 12
BXAdder = $-2          
BYAdder = $-4          
 mov bl,[Texture+ebx]
 mov [edi],bl          
 inc edi                           
 dec bp                
 jnz Inner                         
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ 
 pop ecx           
 pop eax     
SkipLine:  
 add d [FillerScreen],960
 add eax,12345678h
LeftTXAdder = $-2 
LeftTyAdder = $-4 
 add ecx,12345678h
LeftBXAdder = $-2
LeftByAdder = $-4
 pop bp                 
 add edx,12340000h      ; .. 00 00 34 12
RightFracAdder = $-2    
 adc ebp,12345678h      ; .. 78 56 34 12   
RightIntAdder = $-4     
LeftFracAdder = $-2     
 adc dx,1234h            
LeftIntAdder = $-2     
 dec [POlyHeight]
 jnz Outer     
 ret             
ENDP           
PolyHeight dd ?

;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
           
; 4096 entries = angles from 0..4095
; 2.14 Fixed POint = 0..16383 : DOUBLE-WORDS - For easier access!
include sincos.dd
temp dd ?  
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
; X = EBX, Y = ECX, Z = EBP         
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
;PROC Rotate
Rotate MACRO
;; X-rotation                        
;; Y = cos(vx)*y - sin(vx)*z 
;; Z = sin(vx)*y + cos(vx)*z 
 mov esi,[vx]
 shl esi,2
 mov eax,[Cosinus+esi]  ;; eax = fra -16k til 16k
 imul ecx                
 mov edi,eax            ;; edi = cos(vx)*y
 mov eax,[Sinus+esi]     
 imul ebp               ;; eax = sin(vx)*z
 sub edi,eax             
 sar edi,divd                                
 mov [temp],edi         ;; cos(vx)*y - sin(vx)*z
 mov eax,[sinus+esi]     
 imul ecx                
 mov edi,eax            ;; edi = sin(vx)*y
 mov eax,[cosinus+esi]   
 imul ebp               ;; eax = cos(vx)*z
 add edi,eax                             
 sar edi,divd                                     
 mov ecx,[temp]         ;; ecx (y) = cos(vx)*y - sin(vx)*z
 mov ebp,edi            ;; ebp (z) = sin(vx)*y + cos(vx)*z
;; Y-rotation             
;; X =  cos(vy)*x - sin(vy)*z                                             
;; Z = -sin(vy)*x + cos(vy)*z
 mov esi,[vy]           
 shl esi,2                                       
 mov eax,[Cosinus+esi]   
 imul ebx                                
 mov edi,eax            ;; edi = cos(vy)*x
 mov eax,[Sinus+esi]     
 imul ebp               ;; eax = sin(vy)*z
 sub edi,eax                          
 sar edi,divd                                
 mov [temp],edi             
 mov eax,[sinus+esi]     
; neg eax               ;; hmmm... Sikker p† at denne skal v‘re med???
 imul ebx                                                            
 mov edi,eax            ;; edi = -sin(vy)*x                           
 mov eax,[cosinus+esi]   
 imul ebp               ;; eax = cos(vy)*z                            
 add edi,eax                                                         
 sar edi,divd                                                        
 mov ebx,[temp]         ;; ebx (x) =  cos(vy)*x - sin(vy)*z           
 mov ebp,edi            ;; ebp (z) = -sin(vy)*x + cos(vy)*z           
;; Z-rotation                                                         
;; X = cos(vz)*x - sin(vz)*y                                          
;; Y = sin(vz)*x + cos(vz)*y                                          
 mov esi,[vz]                                                         
 shl esi,2                                                           
 mov eax,[Cosinus+esi]
 imul ebx                                                            
 mov edi,eax            ;; edi = cos(vz)*x                            
 mov eax,[Sinus+esi]     
 imul ecx               ;; eax = sin(vz)*y                            
 sub edi,eax                        
 sar edi,divd                                
 mov [temp],edi                          
 mov eax,[sinus+esi]     
 imul ebx                                         
 mov edi,eax            ;; edi = sin(vz)*x
 mov eax,[cosinus+esi]   
 imul ecx               ;; eax = cos(vz)*y
 add edi,eax                        
 sar edi,divd                       
 mov ebx,[temp]         ;; ebx (x) = cos(vz)*x - sin(vz)*y
 mov ecx,edi            ;; ecx (y) = sin(vz)*x + cos(vz)*y
;; ret                      
ENDM ;ENDP                      
                         
; Coords        ; ESI*2         +4
; TriCoords     ; SI            +4
; ZCoords       ; DI            +2
PROC RotateObjCoords      
 xor esi,esi             
 mov edx,[CoordData]
 mov eax,[NumCoords]                           
 shl eax,2           
 mov d [@@Finito],eax
 mov edi,OFFSET ZCoords  ; DI -> Offset
RotLoop:                  
 movsx ebx,w [edx+esi*2]
 movsx ecx,w [edx+esi*2+2]
 movsx ebp,w [edx+esi*2+4]
 push edx              
 push esi              
 push edi                         
 Rotate
; Call Rotate    ; Returns : ebx,ecx,ebp rotated
 pop edi               
 pop esi                                      
 mov [eDI],bp      
 add edi,2         
; Project at once                                           
 add ebp,[Zoff]           
;                         
 xchg eax,ebx             
 shl eax,8                
 cdq                      
 idiv ebp                 
 add eax,[Xoff]            ; eax = Projected X
 xchg ebx,eax             
;                                                           
 xchg eax,ecx             
 shl eax,8                                    
 cdq                      
 idiv ebp                 
 add eax,[Yoff]            ; eax = Projected Y
 xchg ecx,eax                                 
;                                             
 pop edx           
 mov w [TriCoords+eSI],cx
 mov w [TriCoords+eSI+2],bx
 add esi,4          
 cmp esi,12345678h  
@@Finito = $-4
 jb RotLoop              
 ret                                                                    
ENDP                     
                                              
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
; Samme med de to neste
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
                     
PROC RotateNormals   
 mov esi,[NormalData]    
 Xor edx,edx            ; Start med coord #0
 mov eax,[NumCoords] 
 shl eax,2           
 mov d [@@Finito],eax
RotLoop2:                 
 movsx ebx,w [esi+edx*2]
 movsx ecx,w [esi+edx*2+2]
 movsx ebp,w [esi+edx*2+4]
                         
 push bx
 push cx
 add bx,127
 add cx,127
 mov w [BumCoords1+edx],cx
 mov w [BumCoords1+edx+2],bx
 pop cx
 pop bx
                         
 push edx                
 push esi                
 Rotate
; Call Rotate
 pop esi                 
 pop edx                 
                         
 add bx,127              
 add cx,127              
 mov w [TexCoords+edx],cx
 mov w [TexCoords+edx+2],bx
 add edx,4                 
 cmp edx,12345678h       
@@Finito = $-4           
 jb RotLoop2             
 ret                                                                    
ENDP                     
                         
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
; SI = Coord set 1                                
; Di = Coord set 2         
; BP = Coord set 3  
; +0 = Y, +2 = X    
; Returnerer Face Normal i EAX... negativ=synlig...
;PROC CheckVisible           
CheckVisible MACRO
;; (x2-x1)*(y3-y1) - (x3-x1)*(y2-y1)
 mov ax,w [TriCoords+edi*4+2]   ;; x2
 sub ax,w [TriCoords+esi*4+2]   ;; x1
 movsx eax,ax                    
 mov bx,w [TriCoords+ebp*4]     ;; y3
 sub bx,w [TriCoords+esi*4]     ;; y1
 movsx ebx,bx                                             
 imul ebx               ;; Blir denne IMUL'en noensinne over +/-32k ???
 mov ecx,eax            ;; klarer jeg meg med 16-bits ??? Sjekk senere...
 mov ax,w [TriCoords+ebp*4+2]   ;; x3
 sub ax,w [TriCoords+esi*4+2]   ;; x1
 movsx eax,ax                                             
 mov bx,w [TriCoords+edi*4]     ;; y2
 sub bx,w [TriCoords+esi*4]     ;; y1
 movsx ebx,bx                                     
 imul ebx                                         
 sub ecx,eax                     
 mov eax,ecx                     
;; ret                                     
ENDM; ENDP
                                 
PROC ShellSort                   
 Xor esi,esi                     
 xor edi,edi   
 mov ebp,[NumToDraw]
 dec ebp              
ShellLoop:           
 cmp ebp,1            
 jl QuitShell        
 shr ebp,1            
RepeatLoop:          
 mov ecx,1            
 mov edi,0;1          
 mov edx,[NumToDraw]
 dec edx   
 sub edx,ebp
ForLoop:          
 mov esi,edi        
 add esi,ebp    
 mov ax,[face_z+edi*2]
 cmp ax,[face_z+esi*2]
 jle DontSwap         
 xchg ax,[face_z+esi*2]
 mov [face_z+edi*2],ax
 mov ax,[faceInd+edi*2]
 xchg ax,[faceInd+esi*2]
 mov [faceInd+edi*2],ax
 mov ecx,0     
DontSwap:     
 inc edi       
 cmp edi,edx    
 jle ForLoop   
 cmp ecx,0     
 jz RepeatLoop       
 jmp ShellLoop
QuitShell:     
 ret          
ENDP           
                                
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
; Variabler til Sortering...     
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
face_z dw MaxPolys DUP (?)   
faceind dw MaxPolys DUP (?)      
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
PROC SortSetup 
 mov [NumToDraw],0
 mov edx,[PolygonData]
 xor ecx,ecx                    ; ECX = Current polygon to check
 xor eax,eax                    ; EAX = Number in Face_Z/Ind
SSLoop2:              
 movzx esi,w [edx+ecx*8]          ; Coord set #1
 movzx edi,w [edx+ecx*8+2]        ; Coord set #2
 movzx ebp,w [edx+ecx*8+4]        ; Coord set #3
 push eax           
 push ecx           
 push edx                                                            
 CheckVisible
; Call CheckVisible                          
 cmp eax,0        
 jl SortThisOne2                 ; Negative = Visible
SkipThisOne2:    
 pop edx                                                 
 pop ecx                                                 
 pop eax                                                 
 inc ecx                ; Neste Polygon                  
 cmp ecx,[NumPolys]              ; Checked all?           
 jb SSLoop2       
 ret                                                     
SortThisOne2:                                             
 pop edx                                                 
 pop ecx                                                 
 pop eax                                                 
; Rotated = Start p† Roterte koordinater (words X,Y,Z,0)
; SI = Coords set #1, DI = #2, BP = #3
 mov bx,[ZCoords+esi*2]       ; Z1
 add bx,[ZCoords+edi*2]       ; Z2
 add bx,[ZCoords+ebp*2]       ; Z3
                  
 Mov w [Face_z + eax*2],bx       ; Insert Z's til poly #
 mov w [FaceInd+ eax*2],cx      ; INsert Polygon # in Ind
 inc eax                        ; Next pos in lists
 inc [NumToDraw]             ; Inc number of polys to draw/sort
 inc ecx                        ; Next poly to check     
 cmp ecx,[NumPolys]                                  
 jb SSLoop2         
 ret                                                
ENDP                                                  
                                  
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
NumToDraw dd ?
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
PROC DrawObject                   
 Call RotateObjCoords
 Call RotateNormals               
 Call SortSetup                                                  
 Call ShellSort                                                  
 mov edx,[NumToDraw]          ; DX = teller nedover fra siste poly
 dec edx                        ; Antall polys-1 (fra n-1 til 0)
 js FinitoDraw                                                   
 mov ebx,[PolygonData]               ; EBX -> Polygon-face data                
DrawObjLoop:                                                     
 movzx ecx,w [FaceInd+edx*2]      ; CX = Polygon #
; EBX = Start of Polygon-face data list
; ECX = Current polygon #         
 movzx esi,w [ebx+ecx*8]         ; SI = Coord set #1
 shl esi,2                      ; * 4 = Index in TriCoords & TexCoords
 movzx edi,w [ebx+ecx*8+2]       ; DI = Coord set #2
 shl edi,2                                                       
 movzx ebp,w [ebx+ecx*8+4]       ; BP = Coord set #3
 shl ebp,2                                                       
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
    
PROC CalcTex                       
 cld
; F›rst blank buffer, s† vi er sikker
 mov edi,offset Texture
 xor eax,eax            
 mov ecx,16384
 rep stosd            
 mov di,0   
@@fory_loop:       
 mov si,0          
@@forx_loop:       
 mov ax,si         
 sub ax,128    
 imul ax,ax        
 mov bx,di         
 sub bx,128    
 imul bx,bx        
 add ax,bx     
 shr ax,9
 mov cx,63
 sub cx,ax   
 cmp cx,5    
 cmp cx,64
 jb @@1                
 mov cx,0              
@@1:                   
 mov bx,di 
 shl ebx,8              
 add bx,si 
 movzx ebx,bx  
 mov [Texture+ebx],cl
 inc si       
 cmp si,255;127   
 jb @@forx_loop           
 mov si,0     
 inc di       
 cmp di,255;127   
 jb @@fory_loop
 ret          
ENDP          
              
              
PROC Demo      
                    
 mov eax,OFFSET Coords1
 mov [CoordData],eax
 mov eax,OFFSET VertexNormals1
 mov [NormalData],eax
 mov eax,OFFSET Polygon1
 mov [PolygonData],eax
 mov eax,[nc1] 
 mov [NumCoords],eax
 mov eax,[np1]
 mov [NumPolys],eax
 mov [vx],0   
 mov [vy],0                  
 mov [vz],0                  
 mov [Xoff],160              
 mov [yoff],100
 mov [zoff],700
DoItAgain:    
; Clear Buffer 
 mov edi,[buffer]
 add edi,(960*200)+320
 mov ecx,200
 mov eax,01020304h
@@1:
 rept 80
  stosd
 endm
 add edi,640
 dec ecx
 jnz @@1
 
 call DrawObject       
 Call WaitVR
; Copy buffer to screen
 mov esi,[Buffer]
 add esi,(960*200)+320
 mov edi,[Screen]
 mov ecx,200
@@2:    
 rept 80    
  movsd      
 endm       
 add esi,640
 dec ecx
 jnz @@2
; update rotation angles
 add [vx],20      
 and [vx],4095 
 add [vy],25   
 and [vy],4095 
 add [vz],30
 and [vz],4095
; loop        
 in al,60h    
 cmp al,1     
 jne DoItAgain 
 ret                 
ENDP                   

;° EntryPoint from DOS °°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
EntryPoint:
 mov eax,0EE02h
 int 31h
 mov edi,0A0000h
 sub edi,ebx
 mov [Screen],edi
 cld
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
 mov ax,13h                
 int 10h                   
 ; Allocate 64K for BUFFER 
 mov ax,0ee42h             
 mov edx,90000h
 int 31h                   
 jc QuitDemo               
 mov [Buffer],edx          
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
 mov esi,offset palette    
 Call SetPalette           
 Call Demo                 
QuitDemo:                  
 mov ax,3                  
 int 10h                   
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
; All Allocated memory is automatically freed upon termination, so we don't
; have to free it here.....
 mov ax,4c00h
 int 21h
End EntryPoint
