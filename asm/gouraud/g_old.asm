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

AddValue = 64
ShrValue = 2
CutOff = 0

;NumPoints EQU NumSize*NumSize*NumSize	 ; Number of points to rotate
NumCoords EQU 130
NumPolys EQU 256
P_Scaler EQU 2048               ; DIV 2048 = shr 11
Z_Plane_Threshold EQU 130
TableSize EQU 256		; Size of SIN/COS table

;∞ SEGMENTS ∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞

Segment BufferSeg
Label Buffer Byte
 db ScreenWidth*ScreenHeight dup(?)
ENDS

;∞ DATA ∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞
.DATA

include drop.inc

Rotated_Coords dw NumCoords*4 dup(?)
Rotated_Normals dw NumCoords*4 dup(?)
label SineTable word
Include sin.db
; + TableSize/4 for Cosinus i samme tabell -> COS = SIN + 1/4 av tabellen
include sin.db

color db 15

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
s_pos dw -256
m_pos dw 0
; current origon
origon_x dw 0
origon_y dw 0
origon_z dw -400

Palette Label byte
i=0
rept 64
 db i,0,0
 i=i+1
endm
i=0
rept 64
 db 0,i,0
 i=i+1
endm
i=0
rept 64
 db 0,0,i
 i=i+1
endm
i=0
rept 64
 db i,i,i
 i=i+1
endm

Mul320 Label Word
i=0
 Rept 200
  DW i*320
  i=i+1
 ENDM

LBuf dw ScreenHeight dup(?)
RBuf dw ScreenHeight dup(?)
CLBuf dw ScreenHeight dup(?)
CRBuf dw ScreenHeight dup(?)
                           
Topy dw ?
BotY dw ?
         
x1 dw ?  
y1 dw ?  
z1 dw ?  
x2 dw ?  
y2 dw ?  
z2 dw ?  
x3 dw ?  
y3 dw ?  
z3 dw ?  
c1 dw ?  
c2 dw ?  
c3 dw ?  
         
;∞ CODE ∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞
.CODE    
         
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
 imul bx		;x2 * v
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
 MOV AX,[y2]   ; y2
 SUB AX,[y1]   ; y1
 MOV BX,[x3]	; x3
 SUB BX,[x1]	; x1
 IMUL BX
 MOV CX,AX
 MOV AX,[y3]   ; y3
 SUB AX,[y1]   ; y1
 MOV BX,[x2]	; x2
 SUB BX,[x1]	; x1
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
 mov dx,3c8h
 xor al,al
 out dx,al
 inc dx
 mov cx,768
 mov ax,SEG Palette
 mov ds,ax
 mov si,OFFSET Palette
 rep outsb
 ret
ENDP

PROC ClearBuffer
 mov ax,SEG Buffer
 mov es,ax      
 xor di,di
 mov eax,CutOff shl 24 + CutOff shl 16 + CutOff shl 8 + CutOff
; xor eax,eax
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
      
;ƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒ
; CX= x1, DX = y1, AX = x2, BX = y2 (all must be clipped and positive)
; di = c1, si = c2            
;ƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒ
PROC ScanGouraudLeft          
 cmp dx,bx      ; is y1=y2 -> Zero height?
 jne @@SGL1     ; nope... continue
 ret            ; yes - quit scanning
@@SGL1:                                 
 jl @@SGL2      ; is y1 < y1 (y1 top) - if yes, continue
 xchg cx,ax     ; esle swap all x,y and color 1-2
 xchg dx,bx                                      
 xchg di,si                                      
@@SGL2:                          
 push cx	; save X start
 push di	; Save color start
 mov bp,bx      ; bp = y2
 add bp,bp      ;      *2
 cmp bp,[BotY]  ; Set new BotY if Y2*2 > BotY
 jb @@SGL3          
 mov [BotY],bp      
@@SGL3:             
 sub ax,cx	; ax = Width    (x2-x1)
 sub bx,dx	; bx = height   (y2-y1)
 sub si,di	; si = Colorrun (c2-c1)
 mov bp,dx      ; bp = y1
 add bp,bp	; bp -> Ypos in Both buffers    (=y1*2)
 cmp bp,[TopY]  ; Set new TopY?
 ja @@SGL4                        
 mov [TopY],bp                 
@@SGL4:                                
 shl eax,16     ; eax = width -> 16.16
 cdq            ; edx:eax = eax
 movzx ebx,bx   ; ebx=bx -> Height
 idiv ebx     	; eax = Xslope 16.16
 mov edi,eax	; edi = Xslope 
 mov ax,si      ; ax = ColorRun
 shl eax,16     ; eax = 16.16 Colorrun
 cdq            ; fix EDX      
 idiv ebx	; eax = Colorslope 16.16
 mov esi,eax	; esi = ColorSlope      
                                      
 pop cx    	; restore color start (previous push di)
 shl ecx,16     ; ecx = 16.16 color
 pop ax    	; X start (prev. push cx)
 shl eax,16     ; 16.16 - eax = X

 xchg bx,bp     ; Hadde et helvetes strev med Ü finne ut at hvis jeg hadde
                ; bx og bp motsatt (bp = index til bufre og bx = counter),
                ; sÜ funka det ikke... Denne enkle xchg'en fikser det!
                    
; edi = Xslope, esi = colorslope 
; eax = X, ecx = color         
; bx = Height, bp = pos in buffers
@@SGL5:                           
 shld edx,eax,16    	; dx = Xpos
 mov [LBuf+bx],dx   
 shld edx,ecx,16                              
 mov [CLBuf+bx],dx                            
 add bx,2           
 add eax,edi	    	; inc x,xslope        
 add ecx,esi	    	; inc color,colorslope
 dec bp                           
 jnz @@SGL5                          
 ret                              
ENDP                              
                                  
;ƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒ
; Same as ScanGouraudLeft, but LBuf and CLBuf has been exchanged with
; RBuf and CRBuf...               
;ƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒ
PROC ScanGouraudRight             
 cmp dx,bx                        
 jne @@SGR1                          
 ret                              
@@SGR1:                              
 jl @@SGR2                           
 xchg cx,ax                       
 xchg dx,bx                       
 xchg di,si                       
@@SGR2:                              
 mov bp,bx              ; bp =  y1
 add bp,bp              ;       * 2
 cmp bp,[BotY]                    
 jb @@SGR3                           
 mov [BotY],bp                    
@@SGR3:                              
 push cx	; save X start    
 push di  	; Save color start
 sub ax,cx	; ax = Width
 sub bx,dx	; bx = height
 sub si,di	; si = Colorrun
 mov bp,dx               
 add bp,bp	; bp -> Ypos in Both buffers
 cmp bp,[TopY]           
 ja @@SGR4                  
 mov [TopY],bp           
@@SGR4:                     
 shl eax,16              
 cdq                     
 movsx ebx,bx            
 idiv ebx	; eax = Xslope 16.16
 mov edi,eax	; edi = Xslope
 mov ax,si               
 shl eax,16                
 cdq            
 idiv ebx	; ax = Colorslope 16.16
 mov esi,eax	; si = ColorSlope      
                                       
 pop cx   	; color start
 shl ecx,16                    
 pop ax     	; X start    
 shl eax,16

 xchg bx,bp                             

; edi = Xslope, esi = colorslope
; eax = X, ecx = color        
; bx = Height, bp = pos in buffers
@@SGR5:         
 shld edx,eax,16	; dx = Xpos
 mov [RBuf+bx],dx
 shld edx,ecx,16
 mov [CRBuf+bx],dx
 add bx,2           
 add eax,edi		; inc x,xslope
 add ecx,esi		; inc color,colorslope
 dec bp         
 jnz @@SGR5        
 ret            
ENDP            
                                                                          
;ƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒ   
; Draws Gouraud shaded polygon                                            
; ABsolutt ikke kjappest mulige versjon... FÜr optimere senere...         
;ƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒ   
;PROC FillGouraud                                                          
; mov bx,[TopY]                                                            
; mov bp,[BotY]                                                            
; mov ax,SEG Buffer
; mov es,ax      
;DrawLoop:       
; Mov di,[Mul320+bx]
; add di,[LBuf+bx]
; mov ax,[CRBuf+bx]  
; sub ax,[CLBuf+bx]      ; ax = antall farger
; mov cx,[RBuf+bx]
; sub cx,[LBuf+bx]       ; cx = lengde pÜ linje
; jz QuitGHLine
; shl ax,8
; cwd                                                                      
; idiv cx      	 	; SKulle gjerne pÜ en eller annen mÜte blitt kvitt
; mov dx,[CLBuf+bx]	; den h**vetes IDIV'en... :(                      
; shl dx,8                                                                 
;@@Loop1:                                                                  
; cmp dh,CutOff 
; jb Below      
; mov [es:di],dh         ; Mulig Ü fÜ denne Inner-Loop'en kjappere?
;Below:         
; inc di        
; add dx,ax                                                                
; dec cx                                                                   
; jnz @@Loop1                                                              
;QuitGHLine:                                                               
; inc bx                                                                   
; inc bx                                                                   
; cmp bx,bp      
; jl DrawLoop    
; ret            
;ENDP            
                
PROC FillPlain  
 mov bx,[TopY]                                                            
 mov bp,[BotY]                                                            
 mov ax,SEG Buffer
 mov es,ax      
DrawLoop:       
 Mov di,[Mul320+bx]
 add di,[LBuf+bx]
 mov cx,[RBuf+bx]
 sub cx,[LBuf+bx]       ; cx = lengde pÜ linje
 jz QuitGHLine  
 mov dl,[color] 
@@Loop1:
 mov [es:di],dh         ; Mulig Ü fÜ denne Inner-Loop'en kjappere?
 inc di
 dec cx                                                                   
 jnz @@Loop1                                                              
QuitGHLine:                                                               
 inc bx                                                                   
 inc bx                                                                   
 cmp bx,bp
 jl DrawLoop
 ret     
ENDP
                
;ƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒ
PROC GouraudPoly
 mov [TopY],400 
 mov [BotY],0                      
 mov cx,[x1]   	      	; Linje 1-2
 mov dx,[y1]          
 mov ax,[x2]          
 mov bx,[y2]    
 mov di,[c1]    
 mov si,[c2]    
 cmp dx,bx            
 jb @@1               
 Call ScanGouraudLeft
 jmp @@2              
@@1:                  
 Call ScanGouraudRight
@@2:                  
 mov cx,[x2]	      	; Linje 2-3
 mov dx,[y2]                       
 mov ax,[x3]                       
 mov bx,[y3]                       
 mov di,[c2]                       
 mov si,[c3]                       
 cmp dx,bx                         
 jb @@3                            
 Call ScanGouraudLeft
 jmp @@4                           
@@3:                               
 Call ScanGouraudRight
@@4:                               
 mov cx,[x1]	      	; Linje 1-3
 mov dx,[y1]          
 mov ax,[x3]          
 mov bx,[y3]          
 mov di,[c1]          
 mov si,[c3]          
 cmp dx,bx            
 jb @@5     
 Call ScanGouraudRight
 jmp @@6    
@@5:                  
 Call ScanGouraudLeft
@@6:                  
 Call FillPlain;Gouraud
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
 movzx eax,word ptr [DS:SI+2]	; coord 2
 movzx edi,word ptr [DS:SI+4]	; coord 3
 mov cx,[Rotated_Coords+EBP*8]  
 mov [x1],cx
 mov cx,[Rotated_Coords+eax*8]  
 mov [x2],cx                    
 mov cx,[Rotated_Coords+edi*8]  
 mov [x3],cx                    
 mov cx,[Rotated_Coords+EBP*8+2]
 mov [y1],cx                    
 mov cx,[Rotated_Coords+eax*8+2]
 mov [y2],cx                    
 mov cx,[Rotated_Coords+edi*8+2]
 mov [y3],cx                    
 mov cx,[Rotated_Coords+ebp*8+4]
 mov [z1],cx
 mov cx,[Rotated_Coords+eax*8+4]
 mov [z2],cx
 mov cx,[Rotated_Coords+edi*8+4]
 mov [z3],cx

 ; Lysvektor -> Langs Z aksen... Inn i skjermen ???

 mov bx,[Rotated_Normals+ebp*8+4]
 cmp bx,0
 jg @@Light1
 neg bx        
@@Light1: 
; add bx,256             ; Rotated Normals -> from -255 to 255 -> 0-511
 shr bx,ShrValue        ; div 8 = 0..63
 add bx,AddValue
 mov [c1],bx
 mov bx,[Rotated_Normals+eax*8+4]
 cmp bx,0
 jg @@Light2
 neg bx
@@Light2: 
; add bx,256
 shr bx,ShrValue
 add bx,AddValue
 mov [c2],bx
 mov bx,[Rotated_Normals+edi*8+4]
 cmp bx,0
 jg @@LIght3
 neg bx
@@LIght3: 
; add bx,256
 shr bx,ShrValue
 add bx,AddValue
 mov [c3],bx

 add si,6    
 push si     
 Call CheckVisible
 cmp ax,0
 jle SkipPoly
Call GouraudPoly
SkipPoly:    
 pop si      
 pop cx      
 dec cx      
 jnz PolyFillLoop
 ret         
ENDP         
             
;∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞
Proc GouraudRutine
RutineLoop:
 Add byte ptr [X_Angle],1	; ùk roterings-vinkler
 Add byte ptr [Y_Angle],3
 Add byte ptr [Z_Angle],2
 mov ax,@data	  		; RotÇr 3D koordinater
 mov es,ax                                            
 mov cx,NumCoords                                     
 mov si,OFFSET Coords_to_rotate                       
 mov di,OFFSET Rotated_Coords                         
 Call Rotate_32
 mov cx,NumCoords
 mov si,OFFSET VertexNormals
 mov di,OFFSET Rotated_Normals
 call Rotate_32
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
 Call SetPalette         
 mov [X_Angle],0         
 mov [Y_Angle],0         
 mov [Z_Angle],0         
 Call GouraudRutine      
 mov ax,3                
 int 10h                 
 mov ax,4c00h            
 int 21h                 
END EntryPoint           
