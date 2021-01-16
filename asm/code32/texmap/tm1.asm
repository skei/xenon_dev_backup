;DOSSEG
.486p ;.386
.MODEL FLAT ;.MODEL MEDIUM
.STACK
;JUMPS
LOCALS
.CODE
             
;° EQU's °°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
                                                                       
b EQU byte ptr
w EQU word ptr
d EQU dword ptr 

MaxPolys = 1024
MaxCoords = 1024

;° DATA °°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
Screen dd ?  
Height dd ?           
Texture label byte
; include texture.db
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
; Texture Mapping variables
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
TriCoords dd MaxCoords dup (?)
TexCoords dd MaxCoords*2 dup(?) ;Trenger vel ikke *2 ????????
             
;° CODE °°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
             

PROC TmTri
 mov eax,[TriCoords+si]
 mov ebx,[TriCoords+di]
 mov ecx,[TriCoords+bp]
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
 jg QuitPoly
 cmp cx,0        
 jl QuitPoly
 mov d [y1],eax
 mov d [y2],ebx
 mov d [y3],ecx
 mov eax,[TexCoords+si]
 mov ebx,[TexCoords+di]
 mov ecx,[TexCoords+bp]
 mov d [ty1],eax
 mov d [ty2],ebx
 mov d [ty3],ecx
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
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
 imul [y3_minus_y1]
 idiv [y2_minus_y1]
 shld si,ax,8           
 add si,[tx1]           ; si = tx4
; Ty4 = ty1 + Tys13*(y2-y1)       
 mov ax,[ty3]                     
 sub ax,[ty1]                     
 shl ax,8                         
 cwd                              
 imul [y3_minus_y1]
 idiv [y2_minus_y1]
 shld di,ax,8                     
 add di,[ty1]           ; di = ty4
Skip4:   
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
 cmp [y2_minus_y1],0
 je SkipTop
; Left = Slope12
; Right = Slope13
; if left > right then swap
; CALL DoHalf
 mov eax,[Slope12]
 mov ebx,[Slope13]
 cmp eax,ebx
 jl @@DontSwap1
 xchg eax,ebx 
@@DontSwap1:
 mov w [LeftFracAdder],ax
 shr eax,16
 add ax,320
 mov w [LeftIntAdder],ax
 mov w [RightFracAdder],bx
 shr ebx,16
 add bx,320
 mov w [RightIntAdder],bx
                       
SkipTop:               
 cmp [y3_minus_y1],0
 je QuitPoly;SkipBottom
; left = slope13   
; right = slope23  
; if left < right then swap   (a)
; JMP DoHalf       
;SkipBottom:
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
NoClipFiller:        
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
;EAX    Xint:XFrac      YInt:YFrac
;EBX    temp.......................
;ECX                               
;EDX    RightFrac       LeftInt           
;ESI
;EDI    Screen-pos...............  
;EBP    LeftFrac        RightInt   
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
Outer:                             
 push bp
 sub bp,dx              ; Right-Left = Line-length
; js SkipLine           ; Blir vel aldri Negativ?
 jz SkipLine                       
                                         
 movzx edi,dx
 add edi,12345678h      ; SelfMod code - sett inn verdien direkte her!
ScreenOffset = $-4                       
 push eax               
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
Inner:                  
 shld ebx,eax,8         ; BL = XInt
 mov bh,ah              ; BH = YInt : BX = Y*256+x
 add eax,12345678h      ; .. 78 56 34 12  
TXAdder = $-2
TYAdder = $-4 
 mov bl,[Texture+bx]    ; Jepp, dette funka jo d›nn i Pmode ogs†!
 mov [edi],bl           ; Men ikke mov [es:di+bp],bl
 inc edi                
 dec bp                 
 jnz Inner              
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
 pop eax                
 add eax,12345678h      
SkipLine:               
 pop bp                 
 add edx,12340000h      ; .. 00 00 34 12
RightFracAdder = $-2    
 adc ebp,12345678h      ; .. 78 56 34 12
RightIntAdder = $-4     
LeftFracAdder = $-2     
 adc dx,1234h           
LeftIntAdder = $-2 
 dec [Height]           ; BRUK REGISTER!!!
 jnz Outer
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
ENDP           
          
          
;° EntryPoint from DOS °°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
EntryPoint:
; mov ax,@data
; mov ds,ax
 mov eax,0EE02h
 int 31h
 mov edi,0A0000h
 sub edi,ebx
 mov [Screen],edi
    
 mov ax,4c00h
 int 21h
End EntryPoint
