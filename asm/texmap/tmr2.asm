 ;ƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒ
; Gouraud shaded polygon - Axon/Xenon Development
;ƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒ

DOSSEG
.MODEL MEDIUM
.386
.STACK
JUMPS
LOCALS

b equ byte ptr
w equ word ptr
d equ dword ptr

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
 db 65536 dup(?)
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

Palette label byte
 include palette.db

;∞ CODE ∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞
.CODE

;align 16
PROC TMPoly
;ƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒ
; SI/DI/BP -> Coordinates in TriCoords & TexCoords
; ES = Screen/Buffer
; FS = Texture         
;ƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒ
 mov eax, d [DS:TriCoords+si]
 mov ebx, d [DS:TriCoords+di]  
 mov ecx, d [DS:TriCoords+bp]  
;ƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒ
; Sjekk om X'ene er innenfor 320
;ƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒ
 mov edx,01400000h              
 cmp edx,eax                                
 sbb dx,dx                                  
 cmp edx,ebx                                
 sbb dx,0                                   
 cmp edx,ecx                                
 sbb dx,0                    
 mov [CS:NeedClip],dx
 cmp dx,-3      ; Alle X'ene er over 320, sÜ,,QUIT
 je QuitPoly_noDS               
;ƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒ
; Sjekk om vi trenger Ü clippe Y
;ƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒ
 mov dx,200         
 cmp dx,ax          
 sbb [CS:NeedClip],0
 cmp dx,bx          
 sbb [CS:NeedClip],0
 cmp dx,cx                      
 sbb [CS:NeedClip],0            
; SortÇr                        
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
; Y-ene i riktig rekkefõlge... NÜ sjekk X'er
 cmp ax,bx       ;Y1=Y2 ? IsÜfall, sjekk om X'ene mÜ swappes
 jne @@4         
 cmp eax,ebx                    
 jl @@4          
 xchg eax,ebx                   
 xchg si,di              
@@4:             
 cmp bx,cx       ;Y2=Y3 ? IsÜfall, sjekk om X'ene mÜ swappes
 jne @@5               
 cmp ebx,ecx     
 jl @@5              
 xchg ebx,ecx                 
 xchg di,bp       
@@5:             
; 1     1     1     1 2
;  2   2     2 3     3  
;3       3       
; NÜ burde de vëre sortert etter Y...
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
;ƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒ
; Heretter brukes bare variabler som ligger i Code-segmentet, sÜ vi
; kopierer CS til DS, og fortsetter som normalt... 
;ƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒ
 push ds               
 push cs                           
 pop ds                            
;ƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒ
; Utregning av sloper..            
;ƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒ
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
;ƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒ
; Fõrst sjekker vi om vi skal kjõre Clip eller NoClip versjonen...
 cmp [CS:NeedClip],0
 jne DoClip
;ƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒ
; Nei - ingen clipping trengs - sÜ vi kjõrer rutine uten clipping,
; som er raskere...                      
ƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒ
NoClip:                         
ƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒ
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
 Jmp FlatLeft
ƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒ
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
ƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒ
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
ƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒ
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
ƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒ
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
ƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒ
ƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒ
DoClip:
ƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒ
; Vi sjekker for hvilket, og utfõrer relevant polygon-filler rutine
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
ƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒ
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
ƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒ
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
ƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒ
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
ƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒ
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
ƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒ
;ƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒ¬ƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒ           
;EAX                          ≥ PolyHeight:Temp                     
;EBX                          ≥ Temp                                
;ECX                          ≥ LineLength                          
;EDX TxInt:TxFrac (inner)     ≥ TyInt:TyFrac (inner)                
;ESI RightFrac                ≥ LeftInt                             
;EDI                          ≥ ScreenPos (ES:SI)                   
;EBP LeftFrac                 ≥ RightInt                            
;ESP                          ≥                                     
;ƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒ¡ƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒ           
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
 jz SkipLine1
 js SkipLine1
;ƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒ            
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
;ƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒ
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
;ƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒ
; Overfõr CodeKonstanter
 mov bx, w [CS:TextureIndex1]
 mov w [CS:TextureIndex2],bx
 mov bx, w [CS:Ypos3201]  
 mov w [CS:Ypos3202],bx   
 mov ah,b [CS:y3_minus_y2]
 jmp Filler2           
;ƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒ
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
 jz SkipLine2
 js SkipLine2
;ƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒ
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
;ƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒ
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
 dec ah   
 jnz Outer2            
;ƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒ
QuitPoly:               
 pop ds 
QuitPoly_noDS: 
 ret    
ƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒ
;align 4                       
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
ƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒ
;ƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒ¬ƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒ           
;EAX                          ≥ PolyHeight:Temp                     
;EBX                          ≥ Temp                                
;ECX                          ≥ LineLength                          
;EDX TxInt:TxFrac (inner)     ≥ TyInt:TyFrac (inner)                
;ESI RightFrac                ≥ LeftInt                             
;EDI                          ≥ ScreenPos (ES:SI)                   
;EBP LeftFrac                 ≥ RightInt                            
;ESP                          ≥                                     
;ƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒ¡ƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒ           
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
;ƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒ            
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
;ƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒ
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
;ƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒ
; Overfõr CodeKonstanter
 mov bx, w [CS:cTextureIndex1]
 mov w [CS:cTextureIndex2],bx
 mov bx, w [CS:cYpos3201]  
 mov w [CS:cYpos3202],bx   
 mov ah,b [CS:y3_minus_y2]
 jmp cFiller2           
;ƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒ
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
;ƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒ
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
;ƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒ
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
;ƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒ
 pop ds  
 ret     
ENDP           


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
 Call TMPoly
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
 Add byte ptr [X_Angle],0	; ùk roterings-vinkler
 Add byte ptr [Y_Angle],1
 Add byte ptr [Z_Angle],0
 
 mov ax,@data 	  	 	; RotÇr 3D koordinater
 mov es,ax                                            
 mov cx,NumCoords                                     
 mov si,OFFSET Coords_to_rotate                       
 mov di,OFFSET Rotated_Coords                         
 Call Rotate_32                                       
 Call ClearBuffer                                     
 Mov si,OFFSET Polygons 	; GÜ gjennom alle, og tegn alle
 mov cx,1;NumPolys
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
