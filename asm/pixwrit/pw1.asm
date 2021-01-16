;DOSSEG
.MODEL MEDIUM
.386
.STACK
;JUMPS
LOCALS
 
NumSteps = 64
MaxPixels = (NumSteps*6)

segment background
 include bg.db
ends 
                
;° DATA °°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
.DATA          
               
label palette byte
 include pal.db

Label Sinus byte
 include sin.db
Label CoSinus byte
 include cos.db 

Label Font byte
 db 256 dup(0)
 include font.db
                   
X_Pos dd MaxPixels dup(0)       ; 16.16 Xpos
Y_Pos dd MaxPixels dup(0)       ;       Y
X_Add dd MaxPixels dup(0)       ; 16.16 Xpos adder
Y_Add dd MaxPixels dup(0)       ;       Y
OldDi dw MaxPixels dup(0)       ; Old DI (for restoring)
Numb dw MaxPixels dup(0)        ; Number of frames left. 0 = InActive
                   
CursorX dw 160            
CursorY dw 100    
                  
c1 db ?           
c2 db ?           
                       
; A-Z    = A..Z        
; 0-9    = [\]^_`abcd  
; !.,'?* = efghij      
Text Label byte        
;    12345678901234567890
 DB ' XENON  DEVELOPMENT '
 DB '      PRESENTS      '
 DB ' YET ANOTHER SHORT  '
 DB '   a_K INTROfffff   '
 DB '      ENTITLED      '
 DB '       WHATii       '
 db '                    '
 db 'CODE            AXON'   
 DB 'MUSIC         MIKE X'
 DB 'GFX          RICHARD'      
 DB 255          
; etc...               
                       
FontX dw 0      ; Hvilken X posisjon i font-data
FontY dw 0      ;         Y
TextPos dw 0    ; Posisjon i TEXT-data for neste tegn
ScreenX dw 0    ; X-posisjon for char p† skjerm
ScreenY dw 0    ; Y
NoMoreText dw 0
                          
;° CODE °°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
.CODE                   
               
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
    
PROC MovePixels
 xor ecx,ecx      ; Start p† Pixel #0
MoveLoop:     
 cmp [Numb+ecx*2],0
 je SkipPixel                                      
 mov di,[OldDi+ecx*2]     
 and byte ptr [es:di],127        ; Fjern ›verste bit = pixel
             
 mov ebx,[Y_Pos+ecx*4]  ; EBX = Ypos 16.16
 add ebx,[Y_Add+ecx*4]  ;       + YAdder
 mov [Y_Pos+ecx*4],ebx  ; Sett ny Ypos
 shr ebx,16             ; Ypos i BX
 lea di,[ebx+ebx*4]     ; Ypos*5
 shl di,6               ; * 64 = (64*5 = 320) = * 320
               
 mov eax,[X_Pos+ecx*4]
 add eax,[X_Add+ecx*4]     
 mov [X_Pos+ecx*4],eax     
 shr eax,16
 add di,ax        
 or byte ptr [es:di],128   
 mov [OldDi+ecx*2],di
 dec [numb+ecx*2]
 jnz SkipPixel
 mov byte ptr [es:di],8
SkipPixel:                
 inc ecx                  
 cmp ecx,MaxPixels        
 jb MoveLoop              
 ret                      
ENDP                      
                          
PROC AddPixels            
; F›rst - finn f›rste ledige posisjon...
 cmp [NoMoreText],1       
 je FinitoAdd             
 xor ecx,ecx              
@@1:                      
 cmp [numb+ecx*2],0       
 je Found
 inc ecx 
 cmp ecx,MaxPixels        
 jb @@1                   
 ret                      
Found:                    
; S†, m† vi finne ut hvilket tegn som skal brukes, og posisjonen i
; Font-Data               
 mov bx,[TextPos]         
 xor eax,eax                        
 mov ah,[Text+bx]       ; Tegn*256
                          
 cmp ah,' '               
 je @@2                   
 cmp ah,255               
 jne @@4         
 mov [NoMoreText],1
 Jmp FinitoAdd   
@@4:             
 sub ah,65       
 add ah,1        
 jmp @@3         
@@2:             
 mov ah,0        
@@3:             
; Deretter, legg til FontY*16+FontX
 mov bx,[FontY]  
 shl bx,4     
 add bx,[FontX]
 add ax,bx     
 cmp [Font+eax],0
 je ItWasBlank   
; initial n_Pos = Cursorn
 mov ax,[CursorY]     
 shl eax,16           
 mov [Y_Pos+ecx*4],eax
 mov ax,[CursorX]         
 shl eax,16           
 mov [X_Pos+ecx*4],eax
; N† har vi funnet ut at det er en pixel som skal tegnes, s† vi m† finne
; ut adderne for denne
; S†, destination = X = ScreenX + FontX
 mov ax,[ScreenX]                      
 add ax,[FontX]                        
 sub ax,[CursorX]                      
 shl eax,16                            
 cdq                                   
 mov ebx,NumSteps                      
 idiv ebx                              
 mov [X_Add+ecx*4],eax                 
 mov ax,[ScreenY]
 add ax,[FontY]
 sub ax,[CursorY]  
 shl eax,16      
 cdq             
 idiv ebx        
 mov [Y_Add+ecx*4],eax
 mov [Numb+ecx*2],NumSteps
;                                      
 inc [FontX]     
 cmp [FontX],16  
 jb FinitoAdd    
 mov [FontX],0   
 inc [FontY]     
 cmp [FontY],16  
 jb FinitoAdd    
 mov [FontY],0   
 inc [TextPos]   
 add [ScreenX],1*16
 cmp [ScreenX],20*16  
 jb FinitoAdd       
 mov [ScreenX],0    
 add [ScreenY],(1*18)
FinitoAdd:          
 ret                
ItWasBlank:      
 inc [FontX]   
 cmp [FontX],16  
 jb Continue   
 mov [FontX],0 
 inc [FontY]   
 cmp [FontY],16
 jb Continue   
 mov [FontY],0 
 inc [TextPos] 
 add [ScreenX],1*16
 cmp [ScreenX],20*16  
 jb Continue    
 mov [ScreenX],0
 add [ScreenY],(1*18)
Continue:          
 jmp Found         
ENDP                                   
                          
PROC SetPalette         
; RET           
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

;° EntryPoint from DOS °°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
EntryPoint:                                                          
 mov ax,@data                                                        
 mov ds,ax  
 mov ax,13h
 int 10h
 Call setpalette
 mov ax,0a000h  
 mov es,ax      
 xor di,di      
 push ds
 mov ax,seg background
 mov ds,ax  
 mov si,offset background
 mov cx,64000
 rep movsb
 pop ds
 mov [c1],0 
 mov [c2],64
                          
TestLoop:      
 Call WaitVR   
 Call MovePixels
rept 3
 xor bx,bx       
 mov bl,[c1]
 xor ax,ax     
 mov al,[Sinus+bx]
 add ax,40     
 mov [CursorX],ax
 xor ax,ax     
 mov al,[CoSinus+bx]
 mov [CursorY],ax
 Call AddPixels
 xor bx,bx       
 mov bl,[c2]   
 xor ax,ax     
 mov al,[Sinus+bx]
 add ax,120
 mov [CursorX],ax
 xor ax,ax     
 mov al,[CoSinus+bx]
 mov [CursorY],ax
 Call AddPixels
 add [c1],1
 add [c2],-1
endm
 in al,60h       
 cmp al,1        
 jne TestLoop    
EndOfWriter:   
 mov ah,0   
 int 16h    
 mov ax,3   
 int 10h    
            
 mov ax,4c00h             
 int 21h
End EntryPoint
