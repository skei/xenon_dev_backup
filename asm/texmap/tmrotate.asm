;ƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒ
; Gouraud shaded polygon - Axon/Xenon Development
;ƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒ

DOSSEG
.MODEL MEDIUM
.386
.STACK
JUMPS
LOCALS


;∞ EQU's ∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞

ScreenHeight = 200
ScreenWidth  = 320

;NumPoints EQU NumSize*NumSize*NumSize	 ; Number of points to rotate
NumCoords EQU 8
NumPolys EQU 12
P_Scaler EQU 2048		; DIV 2048 = shr 11
Z_Plane_Threshold EQU 130
TableSize EQU 256		; Size of SIN/COS table

;∞ SEGMENTS ∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞

Segment TextureSeg 
Label Texture byte
 include texture.db
ends 

Segment BufferSeg
Label Buffer Byte
 db ScreenWidth*ScreenHeight dup(?)
ENDS

;∞ DATA ∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞
.DATA

; Coords to rotate -> DW's -32768 to 32767
; in clock-wise direction when viewed from visible side
; + 0 at end for easier access (*8)
label Coords_To_Rotate word
; include coords.inc
 dw -150,-150, 150,0
 dw  150,-150, 150,0
 dw  150, 150, 150,0
 dw -150, 150, 150,0
 dw -150,-150,-150,0
 dw  150,-150,-150,0
 dw  150, 150,-150,0
 dw -150, 150,-150,0
Rotated_Coords dw NumCoords*4 dup(?)
                    
label Polygons word ;- 1st, 2nd and 3rd coordinate
 dw 0,1,2           
 dw 0,2,3           
 dw 1,5,6
 dw 1,6,2
 dw 5,4,7
 dw 5,7,6
 dw 4,0,3
 dw 4,3,7
 dw 0,4,5
 dw 0,5,1
 dw 2,6,7
 dw 2,7,3          

label SineTable word
Include sin.db
; + TableSize/4 for Cosinus i samme tabell -> COS = SIN + 1/4 av tabellen
include sin.db

; temporary variables for the rotate32 procedure
rY1 dw 0
rX2 dw 0
rY3 dw 0
; current coordinates of Coords being rotated
Xo dw 0
Yo dw 0
Zo dw 0
do dw 0 ; dummy
; temporary variables used by rotate32
source dw 0
dest dw 0
; number of Coords left to rotate used by rotate32
Coords_left dw 0   
; current angles of rotation
Z_angle dw 0
Y_angle dw 0
X_angle dw 0
; cosine and sine of each angle
cos_1 dw 0
sin_1 dw 0
cos_2 dw 0
sin_2 dw 0
cos_3 dw 0
sin_3 dw 0
; perspective variables...
s_pos dw -200
m_pos dw 0
; current origon
origon_x dw 0
origon_y dw 0
origon_z dw -800

MaxCoords = 100
                    
TriCoords dd MaxCoords dup (?)
TexCoords dd MaxCoords dup (?)
                    
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


Palette label byte
 include palette.db

;∞ CODE ∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞
.CODE

PROC TmTri          
 mov eax,dword ptr [DS:TriCoords+si]
 mov ebx,dword ptr [DS:TriCoords+di]
 mov ecx,dword ptr [DS:TriCoords+bp]
 mov edx,01400000h
 cmp edx,eax
 sbb dx,dx
 cmp edx,ebx
 sbb dx,0
 cmp edx,ecx
 sbb dx,0
 cmp dx,-3
 je QuitPoly
; SortÇr
 rol eax,16
 rol ebx,16
 rol ecx,16
 cmp eax,ebx 
 jl @@1                                            
 xchg eax,ebx
 xchg si,di
@@1:            
 cmp eax,ecx
 jl @@2   
 xchg eax,ecx
 xchg si,bp
@@2:    
 cmp ebx,ecx
 jl @@3   
 xchg ebx,ecx
 xchg di,bp
@@3:      
 rol eax,16
 rol ebx,16
 rol ecx,16          
; NÜ burde de vëre sortert etter Y...
 cmp ax,199
 jg QuitPoly
 cmp cx,0  
 jl QuitPoly
 mov dword ptr [y1],eax            
 mov dword ptr [y2],ebx                            
 mov dword ptr [y3],ecx            
 mov eax,dword ptr [DS:TexCoords+si]
 mov ebx,dword ptr [DS:TexCoords+di]
 mov ecx,dword ptr [DS:TexCoords+bp]
 mov dword ptr [ty1],eax
 mov dword ptr [ty2],ebx            
 mov dword ptr [ty3],ecx            
;ƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒ
 mov [Parts],0
; Slope13     
 mov eax,dword ptr [y3]
 sub eax,dword ptr [y1]
 xor ebx,ebx
 mov [y3_minus_y1],ax
 test ax,ax
 jz QuitPoly
 mov bx,ax            
 cdq                  
 idiv ebx             
 mov [slope13],eax                                 
; Slope12             
 mov eax,dword ptr [y2]
 sub eax,dword ptr [y1]
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
 mov eax,dword ptr [y3]
 sub eax,dword ptr [y2]
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
;ƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒ
; Vi sjekker for hvilket, og utfõrer relevant polygon-filler rutine
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
            
; mov ax,[x2]            ; AX = X2
; cmp ax,[x3] 
; jl FlatRight           ; X2<X3
; jmp FlatLeft           ; X2>X3
ƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒ
FlatBottom: 
; 1            
;2 3           
; LeftXAdder = Slope12
 mov eax,[Slope12]                     
 rol eax,16                                            
 add ax,320          
 mov word ptr [CS:IntLeftXAdder2],ax   
 xor ax,ax                             
 mov dword ptr [CS:FracLeftXAdder2],eax
; LenAdder = (x3-x2)/(y2-y1)
 mov eax,dword ptr [y3]
 sub eax,dword ptr [y2]
 xor ebx,ebx                                           
 mov bx,[y2_minus_y1]                                  
 cdq                                                   
 idiv ebx                                              
 rol eax,16                                            
 mov word ptr [CS:IntLenAdder2],ax                     
 xor ax,ax                                             
 mov dword ptr [CS:FracLenAdder2],eax                  
; LeftTxAdder = (tx2-tx1)/(y2-y1) 8.8                  
 mov ax,[tx2]                                          
 sub ax,[tx1]                                          
 shl ax,8                                              
 cwd                                                   
 idiv [y2_minus_y1]                 
 shl eax,16                         
 mov dword ptr [CS:LeftTxAdder2],eax
; LeftTyAdder = (ty2-ty1)/(y2-y1) 8.8
 mov ax,[ty2]        
 sub ax,[ty1]        
 shl ax,8                                              
 cwd                                
 idiv [y2_minus_y1]                 
 shl eax,16                         
 mov dword ptr [CS:LeftTyAdder2],eax
; TxAdder = (tx3-tx2)/(x3-x2)
 mov ax,[tx3]                
 sub ax,[tx2]                
 shl ax,8                    
 cwd                         
 mov bx,[x3]                 
 sub bx,[x2]
 jz QuitPoly
 idiv bx                     
 mov word ptr [cs:TxAdder2],ax                         
; TyAdder = (ty3-ty2)/(x3-x2)
 mov ax,[ty3]             
 sub ax,[ty2]             
 shl ax,8                    
 cwd                         
 idiv bx                     
 mov word ptr [cs:TyAdder2],ax                         
;ƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒ
; Det var konstantene, nÜ kommer registrene... phew...
;ƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒ
; EAX = Length
 mov eax,1
; EBX = LeftTx(8.8):-
 mov bx,[tx1]
 shl ebx,24          
; EDI = LeftTy(8.8):-
 mov di,[ty1]        
 shl edi,24 
; ESI = LeftX:-
 xor esi,esi                                           
 mov si,[y1]         
 lea si,[esi+esi*4]   
 shl si,6   
 add si,[x1]
; EBP = POlyHeight:-
 mov bp,[y3_minus_y1]
 shl ebp,16                                            
 JMP Filler2
ƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒ
FlatTop:              
;1 2
; 3
; LeftXAdder = Slope13
 mov eax,[Slope13]    
 rol eax,16                                            
 add ax,320           
 mov word ptr [CS:IntLeftXAdder2],ax   
 xor ax,ax                             
 mov dword ptr [CS:FracLeftXAdder2],eax
; LenAdder = (x1-x2)/(y3-y1)
 mov eax,dword ptr [y1]
 sub eax,dword ptr [y2]
 xor ebx,ebx                                           
 mov bx,[y3_minus_y1]   
 cdq                                                   
 idiv ebx             
 rol eax,16                                            
 mov word ptr [CS:IntLenAdder2],ax                     
 xor ax,ax                                             
 mov dword ptr [CS:FracLenAdder2],eax                  
; LeftTxAdder = (tx3-tx1)/(y3-y1) 8.8
 mov ax,[tx3]
 sub ax,[tx1]                                          
 shl ax,8                                              
 cwd                                                   
 idiv [y3_minus_y1]
 shl eax,16                         
 mov dword ptr [CS:LeftTxAdder2],eax
; LeftTyAdder = (ty3-ty1)/(y3-y1) 8.8
 mov ax,[ty3]                
 sub ax,[ty1]                
 shl ax,8                                              
 cwd                                
 idiv [y3_minus_y1]          
 shl eax,16                         
 mov dword ptr [CS:LeftTyAdder2],eax
; TxAdder = (tx2-tx1)/(x2-x1)
 mov ax,[tx2]                
 sub ax,[tx1]                
 shl ax,8                    
 cwd                         
 mov bx,[x2]                 
 sub bx,[x1]
 jz QuitPoly
 idiv bx                     
 mov word ptr [cs:TxAdder2],ax                         
; TyAdder = (ty2-ty1)/(x2-x1)
 mov ax,[ty2]                
 sub ax,[ty1]                
 shl ax,8                    
 cwd                         
 idiv bx                     
 mov word ptr [cs:TyAdder2],ax                         
;ƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒ
; Det var konstantene, nÜ kommer registrene... phew...
;ƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒ
; EAX = Length               
 xor eax,eax
 mov ax,bx
 inc ax   
; EBX = LeftTx(8.8):-        
 mov bx,[tx1]
 shl ebx,24                  
; EDI = LeftTy(8.8):-        
 mov di,[ty1]                
 shl edi,24                  
; ESI = LeftX:-
 xor esi,esi                                           
 mov si,[y1] 
 lea si,[esi+esi*4]
 shl si,6    
 add si,[x1] 
; EBP = POlyHeight:-           
 mov bp,[y3_minus_y1]
 shl ebp,16                                            
 JMP Filler2 
ƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒ
FlatLeft:                      
;  1 
;(4)2                          
;3                             
; LeftXAdder = Slope12            
 mov eax,[Slope13]                
 rol eax,16                                            
 add ax,320                       
 mov word ptr [CS:IntLeftXAdder1],ax
 mov word ptr [CS:IntLeftXAdder2],ax
 xor ax,ax                             
 mov dword ptr [CS:FracLeftXAdder1],eax
 mov dword ptr [CS:FracLeftXAdder2],eax 
; LenAdder 1 = - (x2-x4)/(y2-y1)
 mov eax,dword ptr [y2]
 mov cx,bp                      
 shl ecx,16                     
 sub eax,ecx                    
 xor ebx,ebx                                           
 mov bx,[y2_minus_y1]          
 cdq                                                   
 idiv ebx                                              
 rol eax,16                                            
 mov word ptr [CS:IntLenAdder1],ax
 xor ax,ax                                             
 mov dword ptr [CS:FracLenAdder1],eax
; LenAdder 2 = (x4-x2)/(y3-y2)
 mov ax,bp           
 shl eax,16          
 sub eax,dword ptr [y2]
 xor ebx,ebx         
 mov bx,[y3_minus_y2]
 cdq                                                   
 idiv ebx
 rol eax,16                                            
 mov word ptr [CS:IntLenAdder2],ax
 xor ax,ax                                             
 mov dword ptr [CS:FracLenAdder2],eax
; LeftTxAdder = (tx3-tx1)/(y3-y1) 8.8
 mov ax,[tx3]                  
 sub ax,[tx1]                                          
 shl ax,8                                              
 cwd                                                   
 idiv [y3_minus_y1]          
 shl eax,16                         
 mov dword ptr [CS:LeftTxAdder1],eax
 mov dword ptr [CS:LeftTxAdder2],eax 
; LeftTyAdder = (ty3-ty1)/(y3-y1) 8.8
 mov ax,[ty3]
 sub ax,[ty1]                  
 shl ax,8                                              
 cwd                                
 idiv [y3_minus_y1]            
 shl eax,16                         
 mov dword ptr [CS:LeftTyAdder1],eax
 mov dword ptr [CS:LeftTyAdder2],eax 
; TxAdder = (tx2-tx4)/(x2-x4)
 mov ax,[tx2]
 sub ax,si
 shl ax,8                      
 cwd                           
 mov bx,[x2]
 sub bx,bp
 idiv bx                       
 mov word ptr [cs:TxAdder1],ax
 mov word ptr [cs:TxAdder2],ax                          
; TyAdder = (ty2-ty4)/(x2-x4)
 mov ax,[ty2]
 sub ax,di
 shl ax,8                    
 cwd                         
 idiv bx                       
 mov word ptr [cs:TyAdder1],ax 
 mov word ptr [cs:TyAdder2],ax                         
;ƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒ
; Det var konstantene, nÜ kommer registrene... phew...
;ƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒ
; EAX = Length
 mov eax,1
; EBX = LeftTx(8.8):-         
 mov bx,[tx1]
 shl ebx,24          
; EDI = LeftTy(8.8):-
 mov di,[ty1]        
 shl edi,24
; ESI = LeftX:-
 xor esi,esi                                           
 mov si,[y1]         
 lea si,[esi+esi*4]   
 shl si,6   
 add si,[x1]
; EBP = POlyHeight:-           
 mov bp,[y2_minus_y1]          
 shl ebp,16                    
 JMP Filler1                   
ƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒ
FlatRight:                                  
; 1                            
;2(4)                          
;   3                          
              
;  1          
;(4)2                          
;3                             
; LeftXAdder 1 = Slope12            
 mov eax,[Slope12]
 rol eax,16                                            
 add ax,320                       
 mov word ptr [CS:IntLeftXAdder1],ax
 xor ax,ax                             
 mov dword ptr [CS:FracLeftXAdder1],eax
; LeftXAdder 2 = Slope12
 mov eax,[Slope23]
 rol eax,16                                            
 add ax,320                       
 mov word ptr [CS:IntLeftXAdder2],ax
 xor ax,ax                             
 mov dword ptr [CS:FracLeftXAdder2],eax
; LenAdder 1 = - (x4-x2)/(y2-y1)
 mov ax,bp
 shl eax,16
 sub eax,dword ptr [y2]
 xor ebx,ebx                                           
 mov bx,[y2_minus_y1]          
 cdq                                                   
 idiv ebx                                              
 rol eax,16                                            
 mov word ptr [CS:IntLenAdder1],ax
 xor ax,ax                                             
 mov dword ptr [CS:FracLenAdder1],eax
; LenAdder 2 = (x2-x4)/(y3-y2)
 mov eax,dword ptr [y2]
 mov cx,bp
 shl ecx,16
 sub eax,ecx
 xor ebx,ebx         
 mov bx,[y3_minus_y2]
 cdq                                                   
 idiv ebx
 rol eax,16                                            
 mov word ptr [CS:IntLenAdder2],ax
 xor ax,ax                                             
 mov dword ptr [CS:FracLenAdder2],eax
; LeftTxAdder 1 = (tx2-tx1)/(y2-y1) 8.8
 mov ax,[tx2]
 sub ax,[tx1]                                          
 shl ax,8                                              
 cwd                                                   
 idiv [y2_minus_y1]
 shl eax,16                         
 mov dword ptr [CS:LeftTxAdder1],eax
; LeftTxAdder 2 = (tx3-tx2)/(y3-y2) 8.8
 mov ax,[tx3]                     
 sub ax,[tx2]                     
 shl ax,8                                              
 cwd                                                   
 idiv [y3_minus_y2]               
 shl eax,16                         
 mov dword ptr [CS:LeftTxAdder2],eax 
; LeftTyAdder 1 = (ty2-ty1)/(y2-y1) 8.8
 mov ax,[ty2]
 sub ax,[ty1]                   
 shl ax,8                                              
 cwd                                
 idiv [y2_minus_y1]
 shl eax,16                         
 mov dword ptr [CS:LeftTyAdder1],eax
; LeftTyAdder 2 = (ty3-ty2)/(y3-y2) 8.8
 mov ax,[ty3]   
 sub ax,[ty2]
 shl ax,8                                              
 cwd                                
 idiv [y3_minus_y2]
 shl eax,16                         
 mov dword ptr [CS:LeftTyAdder2],eax 
; TxAdder = (tx4-tx2)/(x4-x2)
 mov ax,si
 sub ax,[tx2]
 shl ax,8                      
 cwd                           
 mov bx,bp
 sub bx,[x2]
 idiv bx                       
 mov word ptr [cs:TxAdder1],ax
 mov word ptr [cs:TxAdder2],ax                          
; TyAdder = (ty4-ty2)/(x4-x2)
 mov ax,di
 sub ax,[ty2]
 shl ax,8                    
 cwd                         
 idiv bx                       
 mov word ptr [cs:TyAdder1],ax 
 mov word ptr [cs:TyAdder2],ax                         
;ƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒ
; Det var konstantene, nÜ kommer registrene... phew...
;ƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒ
; EAX = Length
 mov eax,1
; EBX = LeftTx(8.8):-         
 mov bx,[tx1]
 shl ebx,24          
; EDI = LeftTy(8.8):-
 mov di,[ty1]        
 shl edi,24
; ESI = LeftX:-
 xor esi,esi                                           
 mov si,[y1]         
 lea si,[esi+esi*4]   
 shl si,6   
 add si,[x1]
; EBP = POlyHeight:-           
 mov bp,[y2_minus_y1]          
 shl ebp,16                    
 JMP Filler1                   
ƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒ
Filler1:         
;ƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒ
; EAX   -               Length Int
; EBX   LeftTx          -
; EDI   LeftTy          -
; ESI   -               LeftX Int               
; EBP   PolyHeight                              
;ƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒ
; REGISTERS                                     
                                                
; EAX   Length Frac     Length Int                            
; EBX   LeftTx          Temp (inner)              
; ECX                   TxPos (inner)                  
; EDX                   TYPos (inner)           
; ESI   LeftX Frac      LeftX Int                          
; EDI   LeftTy          ScreenPos (inner)
; EBP   PolyHeight      LineLen (inner)
;ƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒ
Outer1:                       
 shld ecx,ebx,16
 shld edx,edi,16
 mov bp,ax              ; LineLen
 mov di,si              ; ScreenPos/LeftX
;ƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒ             
Inner1:         
 mov bh,dh              ; BX = TY*256
 mov bl,ch              ;    + TX
 add cx,1234h  
TxAdder1 = $-2     
 add dx,1234h                        
TyAdder1 = $-2                 
 mov bl,[fs:bx] ; penalty - FS                    
 mov [es:di],bl                                        
 inc di                                
 dec bp                                
 jnz Inner1     
;ƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒ             
InnerSkip1:                    
 add eax,12345678h      ; HW.. LowWord = 0 !
FracLenAdder1 = $-4
 adc ax,1234h
IntLenAdder1 = $-2             
 add ebx,12345678h      ; Adder i 8.8 HighWord
LeftTxAdder1 = $-4
 add edi,12345678h
LeftTyAdder1 = $-4      ; Adder i 8.8 HighWord
 add esi,12345678h            
FracLeftXAdder1 = $-4    ; Frac i HW, LW = 0
 adc si,1234h         
IntLeftXAdder1 = $-2
 sub ebp,10000h                            
 jnz Outer1                                       
ƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒ
; Hmmm, er det bare EBP som trengs Ü settes opp her ???
 mov bp,[y3_minus_y2]
 shl ebp,16                    
Filler2:                                    
;ƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒ
; EAX   -               Length Int          
; EBX   LeftTx          -                   
; EDI   LeftTy          -                   
; ESI   -               LeftX Int               
; EBP   PolyHeight                              
;ƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒ
; REGISTERS                                     
                                                
; EAX   Length Frac     Length Int                            
; EBX   LeftTx          Temp (inner)            
; ECX                   TxPos (inner)
; EDX                   TYPos (inner)           
; ESI   LeftX Frac      LeftX Int                          
; EDI   LeftTy          ScreenPos (inner)
; EBP   PolyHeight      LineLen (inner)                
;ƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒ
Outer2:                                                
 shld ecx,ebx,16                                       
 shld edx,edi,16                                       
 mov bp,ax              ; LineLen                      
 mov di,si              ; ScreenPos/LeftX              
;ƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒ                               
Inner2:                                                
 mov bh,dh              ; BX = TY*256                  
 mov bl,ch              ;    + TX                      
 add cx,1234h                                          
TxAdder2 = $-2                                         
 add dx,1234h                                          
TyAdder2 = $-2                                         
 mov bl,[fs:bx] ; penalty - FS                         
 mov [es:di],bl                                        
 inc di                                                
 dec bp                                                
 jnz Inner2                                            
;ƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒ                               
InnerSkip2:                                            
 add eax,12345678h      ; HW.. LowWord = 0 !
FracLenAdder2 = $-4
 adc ax,1234h
IntLenAdder2 = $-2
 add ebx,12345678h      ; Adder i 8.8 HighWord
LeftTxAdder2 = $-4
 add edi,12345678h
LeftTyAdder2 = $-4      ; Adder i 8.8 HighWord
 add esi,12345678h            
FracLeftXAdder2 = $-4    ; Frac i HW, LW = 0
 adc si,1234h         
IntLeftXAdder2 = $-2
 sub ebp,10000h                            
 jnz Outer2                    
ƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒ
QuitPoly:
 ret                                                   
ENDP 
;ƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒ



;∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞
; CX = Coords to rotate
; DS:SI = Source-Coords -> x,y,z (words)
; ES:DI = Destination -> x,y,[z] -> Z unmodified
;∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞

;rotation routine
PROC Rotate_32
 mov [Coords_left],cx
 mov [source],si
 mov [dest],di
; Sets the angles -> SIN/COS
 mov bx,[Z_angle]
 add bx,bx    
 mov ax,word ptr [SineTable+bx]
 mov [sin_1],ax
 mov ax,word ptr [SineTable+bx+(TableSize/4)*2]
 mov [cos_1], ax
 mov bx,[Y_angle]
 add bx,bx    
 mov ax,word ptr [SineTable+bx]
 mov [sin_2],ax
 mov ax,word ptr [SineTable+bx+(TableSize/4)*2]
 mov [cos_2], ax
 mov bx,[X_angle]
 add bx,bx                     
 mov ax,word ptr [SineTable+bx]
 mov [sin_3],ax                
 mov ax,word ptr [SineTable+bx+(TableSize/4)*2]
 mov [cos_3], ax
rot10:                         
; copy the current Coords to rotate to a work area
 mov si,[source]
 mov di, offset Xo
 movsw	; X     
 movsw	; Y     
 movsw	; Z     
 movsw	; dummy 
 mov [source], si
; do first calculation:
; X1 = (Xo * C1 - Yo * S1) \ 16384
 mov bp,[sin_1] 
 mov bx,[cos_1] 
 mov ax,[yo]    
 imul bp		;Yo*S1
 mov si,ax                     
 mov di,dx      
 mov ax,[xo]    
 imul bx		;Xo*C1
 sub ax,si      
 sbb dx,di      
 sal ax,1       
 rcl dx,1       
 sal ax,1       
 rcl dx,1       
 mov cx,dx      
; now do Y1 = (Xo * S1 + Yo * C1) \ 16384
 mov ax,[xo]    
 imul bp		;Xo*S1
 mov si,ax
 mov di,dx
 mov ax,[yo]
 imul bx		;Yo*C1
 add ax,si   
 adc dx,di
 sal ax,1                      
 rcl dx,1
 sal ax,1
 rcl dx,1
 mov [ry1],dx
; second vector
; X2 = (X1 * C2 - Zo * S2) \ 16384 - Mx + Ox
 mov bp,[sin_2]
 mov bx,[cos_2]
 mov ax,[zo]
 imul bp
 mov si,ax
 mov di,dx
 mov ax,bx
 imul cx		;X1*c2
 sub ax,si
 sbb dx,di
 sal ax,1
 rcl dx,1    
 sal ax,1
 rcl dx,1                      
 add dx,[origon_x]
 mov [rx2],dx
; Z2 = (X1 * S2 + Zo * C2) \ 16384
 mov ax,bp
 imul cx		;X1*S2
 mov si,ax
 mov di,dx
 mov ax,[zo]
 imul bx		;Zo*C2
 add ax,si
 adc dx,di
 sal ax,1
 rcl dx,1
 sal ax,1
 rcl dx,1
 mov cx,dx
; third vector
; Y3 = (Y1 * C3 - Z2 * S3) \ 16384 - My + Oy
 mov bp,[sin_3]
 mov bx,[cos_3]                
 mov ax,bp
 imul cx		;Z2*S3
 mov si,ax
 mov di,dx
 mov ax,bx
 imul [ry1]		;Y1*C3
 sub ax,si
 sbb dx,di
 sal ax,1
 rcl dx,1
 sal ax,1
 rcl dx,1
 add dx,[origon_y]
 mov [ry3],dx
; Z4 = (Y1 * S3 + Z2 * C3) \ 16384
 mov ax,bp
 imul [ry1]		;Y1*S3
 mov si,ax   
 mov di,dx
 mov ax,bx                     
 imul cx		;Z2*C3
 add ax,si
 adc dx,di
 sal ax,1
 rcl dx,1
 sal ax,1
 rcl dx,1
 mov di,[dest]
; save the z coordinate in the output table
 mov [di+4],dx
 add dx,[origon_z]
; now do V=(Spos-Z)/(Mpos-Z)
 neg dx
 mov bx,dx
 add bx,[m_pos]
; is the point in view?
 cmp bx,Z_Plane_Threshold
 jg Rotin_view
; nope
 mov ax,-32767                 
 stosw	; X \
 stosw	; Y  - Settes lik -32767 hvis ikke synlig - nërmere enn skjermen
 stosw	; Z /
 stosw		; - dummy
 mov [dest],di
 jmp Rotnext_point
Rotin_view:
; V= (p_scaler*(Spos-Z)) / (Mpos-Z)
 add dx,[s_pos]
 mov ax,dx
 mov cx,P_Scaler
 imul cx
 idiv bx
 neg ax
; x=160+x2+(x2*-v)\p_scaler
; (or really x=160+x2+(-x2*v)\p_scaler )
 mov bx,ax
 mov bp,[rx2]
 mov ax,bp
 imul bx     		;x2 * v
 idiv cx
 add ax,bp
 add ax,ScreenWidth/2
 stosw
; y=100+y2+(y2*-v)\p_scaler
 mov bp,[ry3]
 mov ax,bp
 imul bx
 idiv cx
 add ax,bp
 add ax,ScreenHeight/2
 stosw
; skip by the already stored Z coordinate
 add di,4
; inc di
; inc di
 mov [dest],di
Rotnext_point:
 dec [Coords_left]
 jz Rot20    
 jmp Rot10
Rot20:
 ret
ENDP Rotate_32

PROC CheckVisible
 MOV AX,word ptr [TriCoords+4]   ; y2
 SUB AX,word ptr [TriCoords]     ; y1
 MOV BX,word ptr [TriCoords+10]	; x3
 SUB BX,word ptr [TriCoords+2]   ; x1
 IMUL BX                
 MOV CX,AX              
 MOV AX,word ptr [TriCoords+8]   ; y3
 SUB AX,word ptr [TRiCoords]     ; y1
 MOV BX,word ptr [TriCoords+6]	; x2
 SUB BX,word ptr [TriCoords+2]	; x1
 IMUL BX         
 SUB CX,AX       
 MOV AX,CX       
 NEG AX
 ret             
ENDP             
                 
                 
;∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞
; Venter til Vertical Retrace STARTER!
;∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞

PROC WaitVR
 mov dx,3DAh
WaitStartVR:
 in al,dx
 test al,8
 jnz WaitStartVR
WaitEndVR:
 in al,dx
 test al,8
 jz WaitEndVR
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

PROC ClearBuffer
 mov ax,SEG Buffer
 mov es,ax     
 xor di,di   
 xor eax,eax
 mov cx,64000/4
 rep stosd
 ret
ENDP
                
PROC CopyBuffer
 push ds       
 mov ax,0a000h 
 mov es,ax     
 xor di,di     
 mov ax,SEG Buffer
 mov ds,ax     
 xor si,si     
 mov cx,16000  
 rep movsd     
 pop ds        
 ret           
ENDP           
             
;∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞
; DS:SI - Coords (indexes)
; CX - number of polygons
; GÜr gjennom ALLE polygons, scanner linjer og farger, og tegner til
; Buffer
;ƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒ
             
PROC PlotPoly
PolyFillLoop:
 push cx     
 movzx ebp,word ptr [DS:SI]	; coord 1
 movzx eax,word ptr [DS:SI+2]  	; coord 2
 movzx edi,word ptr [DS:SI+4]	; coord 3
 mov cx,[Rotated_Coords+EBP*8]
 mov word ptr [TriCoords+2],cx
 mov cx,[Rotated_Coords+eax*8]
 mov word ptr [TriCoords+6],cx
 mov cx,[Rotated_Coords+edi*8]
 mov word ptr [TriCoords+10],cx
 mov cx,[Rotated_Coords+EBP*8+2]
 mov word ptr [TriCoords],cx
 mov cx,[Rotated_Coords+eax*8+2]
 mov word ptr [TriCoords+4],cx
 mov cx,[Rotated_Coords+edi*8+2]
 mov word ptr [TriCoords+8],cx
               
 mov word ptr [TexCoords+2],0
 mov word ptr [TexCoords+0],0
 mov word ptr [TexCoords+6],127
 mov word ptr [TexCoords+4],127
 mov word ptr [TexCoords+10],0 
 mov word ptr [TexCoords+8],127
              
; mov cx,[Rotated_Coords+ebp*8+4]
; mov [z1],cx              
; mov cx,[Rotated_Coords+eax*8+4]
; mov [z2],cx              
; mov cx,[Rotated_Coords+edi*8+4]
; mov [z3],cx              
; mov [tx1],0              
; mov [ty1],0              
; mov [tx2],63             
; mov [ty2],63             
; mov [tx3],0              
; mov [ty3],63             
 add si,6                 
 push si                  
 Call CheckVisible        
 cmp ax,0                 
 jle SkipPoly
 mov ax,SEG Buffer       
 mov es,ax                
 mov ax,SEG Texture
 mov fs,ax    
 mov si,0     
 mov di,4     
 mov bp,8     
 Call TMTri   
SkipPoly:                 
 pop si                   
 pop cx                   
 dec cx                   
 jnz PolyFillLoop         
 ret                      
ENDP                      
                
;∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞
Proc TMRutine   
RutineLoop:     
 Add byte ptr [X_Angle],1	; ùk roterings-vinkler
 Add byte ptr [Y_Angle],2
 Add byte ptr [Z_Angle],3
 
 mov ax,@data 	  	 	; RotÇr 3D koordinater
 mov es,ax                                            
 mov cx,NumCoords                                     
 mov si,OFFSET Coords_to_rotate                       
 mov di,OFFSET Rotated_Coords                         
 Call Rotate_32                                       
 Call ClearBuffer                                     
 Mov si,OFFSET Polygons 	; GÜ gjennom alle, og tegn alle
 mov cx,NumPolys                                      
 Call PlotPoly                                        
 Call WaitVR                                          
 Call CopyBuffer
 in al,60h                                            
 cmp al,1                                             
 jne RutineLoop   
 ret              
ENDP      
          
          
;∞ EntryPoint from DOS ∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞

EntryPoint:
 cld
 Mov ax,13h
 int 10h
 mov ax,@Data
 mov ds,ax
 assume ds:@data,es:@data
; Call SetPalette         
 mov [X_Angle],0         
 mov [Y_Angle],0
 mov [Z_Angle],0         
 Call TMRutine
 mov ax,3
 int 10h                 
 mov ax,4c00h            
 int 21h                 
END EntryPoint           
