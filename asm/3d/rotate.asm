; Axon/Xenon Development

; DET FUNKER!!!!! 24.4.95 - klokka 04:04 !!!
; Ikke s† s‘rlig under OS/2, men, f†r se i morgen under ren DOS
; ENDELIG! N skal det bli fart i 3D kodinga!!!!

;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
; ENDELIG - Egne 3D roterings-rutiner! N† skj›nner jeg ihvertfall hva som
; foreg†r, og hvilke resultater/verdier jeg kan forvente, og hvordan jeg
; kan optimere... :)
; Sikkert mye som kan gj›res enn†...??
; SKulle gjerne hatt beskrivelse av metoden for 3D roteringen med 9 IMUL's
; istedetfor 12.... 3 IMUL's spart pr. koordinat blir MANGE cycles ved
; omfattende objekter!
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

DOSSEG       
.MODEL MEDIUM
.386         
.STACK       
JUMPS        
LOCALS       
             
;° EQU's °°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
             
; Hmmm... Hvorfor 14? Vel, for † holde den innenfor WORD size, og...
; 6 bits i (f.eks) AH = 0-63 -> Kurant shading verdi... (Z...)

divd = 14       ; 16-bits fixed point divider                 

Xoff = 160      ; Variables for 3D->2D transformation
Yoff = 100 
Zoff = 2048
             
;° SEGMENTS °°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°

Bufferseg SEGMENT
Label Buffer byte
 db 320*200 dup(?)
ENDS 


;° MACROS °°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°

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

; Venter til 3DA bit 3 = 0 = i skjerm
WaitForScreen MACRO
local @@vr1
 mov dx,3dah                          
@@vr1:                                 
 in al,dx                             
 test al,8                            
 jnz @@vr1
ENDM                                  
                                      
; Venter til 3DA bit 3 = 1 = i retrace                      
WaitForRetrace MACRO
local @@vr2
 mov dx,3dah       
@@vr2:
 in al,dx          
 test al,8
 jz @@vr2
ENDM
    
;° DATA segment °°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
.DATA        
             
NumCoords = 8
NumPolys = 12
              
; Koordinater, X,Y,Z, og 0 for † f† 8 bytes mellomrom
Coords Label word                                    
 dw -300,-300,-300,0
 dw  300,-300,-300,0
 dw  300, 300,-300,0
 dw -300, 300,-300,0
 dw -300,-300, 300,0
 dw  300,-300, 300,0
 dw  300, 300, 300,0
 dw -300, 300, 300,0
                
Rotated label word
 dw NumCoords*4 dup(?) 
                      
Projected label word
 dw NumCoords*2 dup(?)

Polygon label word
 dw 0,1,2
 dw 2,3,0
 dw 1,5,6
 dw 6,2,1
 dw 5,4,7
 dw 7,6,5
 dw 4,0,3
 dw 3,7,4
 dw 0,4,5
 dw 5,1,0
 dw 2,6,7
 dw 7,3,2
        
; Sine  Cosine tables. 256 entries. Each ranging from -16k to 16k
; or, change the divider (divd = 14) <- 16.16 FP from -1.0 to 1.0
Sinus label word                                                
 include sinus.dw
Cosinus label word
 include cosinus.dw 
                                             
; Roterings-Vinkler
VX dw ?      
VY dw ?      
VZ dw ?      
             
Temp dd ?               ; Temporary variable for Rotate routine             
PushedCounter dd ?      ; Faster to mov to/from this than Push/Pop ?
        
;° CODE segment °°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
.CODE                
        
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
                                                         
PROC RotateAll         
 Xor edx,edx            ; Start med coord #0
RotLoop:
 movsx ebx,[Coords+edx*8]
 movsx ecx,[Coords+edx*8+2]
 movsx ebp,[Coords+edx*8+4]
 mov [PushedCounter],edx
 Call Rotate
 mov edx,[PushedCounter]
 mov [Rotated+edx*8],bx
 mov [Rotated+edx*8+2],cx
 mov [Rotated+edx*8+4],bp 
 inc edx                        ; Bedre med DEC sikkert.. Men da blir det
 cmp edx,NumCoords              ; vel mer strev med † hente X,Y og Z ???
 jb RotLoop                                                             
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
 rep movsd
 pop ds
 ret
ENDP 
        
PROC ClearBuffer
 mov ax,SEG Buffer
 mov es,ax      
 xor di,di      
 mov cx,16000   
; mov eax,01010101h
 xor eax,eax       
 rep stosd         
 ret               
ENDP               
                   
; ax=x, bp=y
PROC Plot3D                     
 cmp ax,0                                 
 jl QuitPoly                               
 cmp ax,319                               
 jg QuitPoly                               
 cmp bp,0                                 
 jl QuitPoly                    
 cmp bp,199                     
 jg QuitPoly                    
 xchg ax,bp     ; ax = y, bp = x                     
 mov cx,320                     
 imul cx             
 add ax,bp   
 mov di,ax           
 mov byte ptr [es:di],15
QuitPoly:   
 ret       
ENDP       
           
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
PROC PlotAll         
 mov ax,SEG Buffer   
 mov es,ax           
 xor ebx,ebx         
PlotLoop:                
 mov ax,[Projected+ebx*4]         ; x
 mov bp,[Projected+ebx*4+2]       ; y
 mov [PushedCounter],ebx
 Call Plot3D        
 Mov ebx,[PushedCounter]
 inc ebx            
 cmp ebx,NumCoords  
 jb PlotLoop        
 ret                
ENDP       
          
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
PROC ProjectAll          
 xor ebx,ebx             
ProjectLoop:                
 movsx eax,[Rotated+ebx*8]         ; x
 movsx ebp,[Rotated+ebx*8+2]       ; y
 movsx ecx,[Rotated+ebx*8+4]       ; z
 mov [PushedCounter],ebx
 add ecx,Zoff     
 shl eax,8        
 cdq           
 idiv ecx      
 add eax,160            ; eax = Projected X     
 xchg eax,ebp        
 shl eax,8                                 
 cdq                                       
 idiv ecx                                  
 add eax,100                               
 xchg eax,ebp           ; ebp = Projected Y
 mov ebx,[PushedCounter]
 mov [Projected+ebx*4],ax
 mov [Projected+ebx*4+2],bp
 inc ebx                 
 cmp ebx,NumCoords
 jb ProjectLoop
 ret       
ENDP       


;° EntryPoint from DOS °°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
EntryPoint:
; Div. initialisering
 mov ax,13h          
 int 10h             
 mov ax,@DATA        
 mov ds,ax
 ASSUME DS:@DATA
 cld            
 mov ax,0a000h  
 mov es,ax
 mov [vx],0
 mov [vy],0
 mov [vz],0
TestLoop:
 Call RotateAll
 Call ProjectAll
 Call ClearBuffer
 Call PlotAll    
 WaitForScreen   
 WaitForRetrace
 Call CopyBuffer
 add [vx],4
 and [vx],1023
 add [vy],5
 and [vy],1023
 add [vz],3
 and [vz],1023
 in al,60h
 cmp al,1
 jne TestLoop
                
 mov ax,3h
 int 10h
 mov ax,4c00h
 int 21h
End EntryPoint 
