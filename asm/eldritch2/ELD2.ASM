; Rotating polygon removed... Didn't look too good...

.MODEL MEDIUM
.386
.STACK 
;JUMPS 
LOCALS 
       
ScreenWidth  = 320
ScreenHeight = 200

Segment LogoSeg para public use16
 include eld.db
ends                               
                                   
;Segment Buffesegr para public use16
;label buffer byte
; db 8000 dup(0)         ; 40*200 = 8000
;ends

Segment PictureSeg                                   
label picture byte
 include logo.db
; db 8000 dup(0)  
ends        
                                     
;° DATA °°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
.DATA               
                                           
; Space, A-Z                                         
                                                     
; 1234567890                                         
; [\]^_`abcd                                         
                                                     
; .,'!/+-()þ:                              
; efghijklmno                              
                                           
Text label byte                            
;   1234567890123456789012345678901234567890
db '              ELDRITCH BBS              '
db '     j^a a\ _b \d ac  l\]oddkdaoddm     '
db '     SYSOPo  AXONiXENON DEVELOPMENT     '
db '                                        '
db '    PROBABLY THE LARGEST CODEiSOURCE    '
db '    AND SCENE COLLECTION IN NORWAYhh    '
db '        lPROVE ME WRONGhhhm eeee        '
db '                                        '
db '  c`dMB OF ONLY SCENE AND CODE RELATED  '
db ' FILES ONLINEeee NEW FILES EVERY DAYhhh '
db 'NO RATIOSf EXCEPT FOR OBVIOUS LAMERSf om'
db '                                        '
DB 750 dup (252),254                        
; Space, A-Z                                         
                                                     
; 1234567890                                         
; [\]^_`abcd                                         
                                                     
; .,'!/+-()þ:                               
; efghijklmno                               
db '   c`dMBf CDkROMf [^e^ MODEM olf OSi\   '
db '        INIQUITY V[ed ALPHA \_R\        '
db '                                        '
db 'THIS SHITTY BBS INTRO WAS CODED IN A FEW'
db ' HOURS BY AXONe NO TIME SPENT ON DESIGN '
db ' OR OPTIMIZATIONSe TAKE IT OR LEAVE ITh '
db '                                        '
db '       OKf  GIVE ELDRITCH A CALLo       '
db '     j^a  a\ _b \d ac l\]oddkdaoddm     '
db 'WONgT GO FOR \^kH UNTIL THE ACTIVITY HAS'
db ' INCREASED A BITf SOeee UNTIL THENeeeee '
db '                                        '
DB 750 dup(252),254                         
; Space, A-Z                                         
                                                     
; 1234567890                                         
; [\]^_`abcd                                         
                                                     
; .,'!/+-()þ:                               
; efghijklmno                               
db ' OKf LETgS LOOK AT A TINY SELECTION OF  '
db ' OUR FILES: lSCENEiCODE RELATED ONESmo  '
db '                                        '
db 'DIRECTORY l`e MAY [cc`m       FILES MBgS'
db 'CODEiee                        a`c  bb  '
db 'SCENEiee                       b`b  \_a '
db 'SOUNDiPLAYERS                   \c  ^   '
db 'SOUNDiTRACKERS                  \c  `   '
db 'DISKMAGS                        c]  ]b  '
db 'ALL DIRS TOTALo               \[b\  _d[ '
db '                                        '
db '       AND GROWING EVERY DAY hhhh       '
DB 750 dup(252),254                         
db 253                                      
                                           
Buffer dw ?     ; Buffer segment           
;P Polygon      8                          
;D Duffling     4                          
;W Writer       2                          
;L Logo         1     
Palette label byte                            
 db 18,18,32    ;                             
 db 38,35,47    ; Logo                        
 db 10,10,20    ;      Writer                 
 db 10,10,20    ; Logo Writer                 
 db 16,16,29    ;             Duffling               
 db 38,35,47    ; Logo        Duffling        
 db 10,10,20    ;      Writer Duffling        
 db 10,10,20    ; Logo Writer Duffling        
 db 18,18,26    ;                      Polygon
 db 38,35,47    ; Logo                 Polygon       
 db 10,10,20    ;      Writer          Polygon       
 db 10,10,20    ; Logo Writer          Polygon       
 db 16,16,29    ;             Duffling Polygon
 db 38,35,47    ; Logo        Duffling Polygon
 db 10,10,20    ;      Writer Duffling Polygon
 db 10,10,20    ; Logo Writer Duffling Polygon
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
                      
         
;ox1 dw 0 
;oy1 dw 0    
;ox2 dw 45
;oy2 dw 90
;ox3 dw -45
;oy3 dw 90                                           

;° POLYGON FILLER DATA °°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°

;LBuf dw ScreenHeight dup(0)      
;RBuf dw ScreenHeight dup(0)

;StartMask db 11111111b
;          db 01111111b
;          db 00111111b
;          db 00011111b                 
;          db 00001111b                
;          db 00000111b
;          db 00000011b
;          db 00000001b   
;EndMask   db 10000000b
;          db 11000000b
;          db 11100000b
;          db 11110000b
;          db 11111000b
;          db 11111100b                              
;          db 11111110b
;          db 11111111b
;SameMask db 10000000b
;         db 11000000b                       
;         db 11100000b                       
;         db 11110000b                       
;         db 11111000b                       
;         db 11111100b                       
;         db 11111110b                       
;         db 11111111b                  
;         db 0                               
;         db 01000000b
;         db 01100000b                       
;         db 01110000b                       
;         db 01111000b                       
;         db 01111100b
;         db 01111110b                       
;         db 01111111b
;         db 2 dup(0)
;         db 00100000b                               
;         db 00110000b
;         db 00111000b
;         db 00111100b
;         db 00111110b            
;         db 00111111b
;         db 3 dup(0)
;         db 00010000b
;         db 00011000b
;         db 00011100b                       
;         db 00011110b
;         db 00011111b                 
;         db 4 dup(0)
;         db 00001000b
;         db 00001100b
;         db 00001110b
;         db 00001111b
;         db 5 dup(0)
;         db 00000100b
;         db 00000110b
;         db 00000111b                               
;         db 6 dup(0)
;         db 00000010b
;         db 00000011b
;         db 7 dup(0)             
;         db 00000001b                       
;MulTable Label Word              
;i=0                              
; Rept ScreenHeight                
;  DW i * ( ScreenWidth / 8 )     
;  i=i+1         
; ENDM                                 
;Topy dw ?
;BotY dw ?
;x1 dw ?
;y1 dw ?  
;x2 dw ?                                     
;y2 dw ?             
;x3 dw ?                                   
;y3 dw ?  
;DrawSegment dw ?                                    
;° 2D ROTATION DATA °°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
;angle           dw 0             
;cosval          dw ?             
;sinval          dw ?             
;include sincos.db                
                                 
;° DUFFLING DATA °°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
; 256 entries - from -32 to 32
DuffSinus Label byte  
 include sin.db   
SinPos1 db ?                           
SinPOs2 db ?
LinjePosisjon DW ?
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
Label Font byte                                      
 include font.inc                                    
                                
CharX dw 0                      
CharY dw 0                      
Char dw 0                  
CharColor db 0
TextIndex dw 0
Sweep db 0      ; Set to 1 if clear screen...
SweepLine dw 0  ; Which line to 'sweep' next...
         
LineY1 dw 0
LineY2 dw 0
LineColor db 0

EndText db 'Eldritch BBS  +47  72 58 20 79 (23:00-07:00) þ '
        db 'Coded by Axon',13,10,13,10,36

;° CODE °°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
.CODE         
              
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
; BitP = bits 0..3 = Active plane.. Note: Stored byte is saved to all
;                    enabled planes
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
SetPlane Macro BitP                
 mov dx,3c4h                       
 mov ah,BitP
 mov al,2         
 out dx,ax     
ENDM                    
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
PROC SetPalette       
 cld                  
 push ds                
 mov ax,seg Palette
 mov ds,ax                                  
 mov si,offset Palette
 mov dx,3c8h                              
 mov al,0              
 out dx,al            
 inc dx                     
 mov cx,24            
 rep outsb                
 mov dx,3c8h          
 mov al,16            
 out dx,al            
 inc dx               
 mov cx,24            
 rep outsb            
 pop ds               
 ret                  
ENDP                  
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
PROC WaitVR
 mov dx,3DAh 
WaitStartVR: 
 in al,dx    
 test al,8   
 jz WaitStartVR
WaitEndVR:   
 in al,dx                            
 test al,8                           
 jnz WaitEndVR
 ret
ENDP                    
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
;PROC RotatePoints
; add     [angle],2
; and     [angle],255
; xor     bp,bp
; mov     si,[angle]                      ; get sin(a) & cos(a)
; movsx   ax,[sincos+si]
; mov     [sinval],ax
; add     si,64               
; and     si,255              
; movsx   ax,[sincos+si]      
; mov     [cosval],ax         
;1                           
; mov ax,[ox1]                
; imul [cosval]               
; mov di,ax                   
; mov ax,[oy1]                
; imul [sinval]               
; sub di,ax                   
; sar di,7                    
; add di,160                  
; mov [x1],di                 
; mov ax,[ox1]                
; imul [sinval]               
; mov di,ax                   
; mov ax,[oy1]                
; imul [cosval]
; add di,ax  
; sar di,7   
; add di,100 
; mov [y1],di
;2
; mov ax,[ox2]
; imul [cosval]               
; mov di,ax                   
; mov ax,[oy2]
; imul [sinval]               
; sub di,ax                   
; sar di,7                    
; add di,160                  
; mov [x2],di
; mov ax,[ox2]
; imul [sinval]               
; mov di,ax                   
; mov ax,[oy2]
; imul [cosval]
; add di,ax  
; sar di,7   
; add di,100                   
; mov [y2],di
;3
; mov ax,[ox3]
; imul [cosval]               
; mov di,ax                   
; mov ax,[oy3]
; imul [sinval]               
; sub di,ax                   
; sar di,7                    
; add di,160                  
; mov [x3],di
; mov ax,[ox3]
; imul [sinval]               
; mov di,ax                   
; mov ax,[oy3]
; imul [cosval]
; add di,ax  
; sar di,7   
; add di,100 
; mov [y3],di                  
; ret
;ENDP             
        
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
; BL = Char
; ES:DI -> ScreenPos    
PROC DrawChar
 cmp bl,' '
 je @@1                 
 sub bl,65      
 add bl,1                     
 jmp @@2                      
@@1:                          
 mov bl,0                     
@@2:                          
 xor bh,bh
 shl bx,3               ; * 8 - Each char is 8 bytes high
 add bx,offset Font     ; BX -> Char
 mov si,bx                    
 i=0                          
 rept 8                       
  mov bl,[ds:si+i]            
  mov [es:di+i*40],bl         
  i=i+1                       
 endm                         
 ret                          
ENDP                          
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
; CX= x1, DX = y1, AX = x2, BX = y2 (all must be clipped and positive)
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
;PROC ScanLeft
; cmp dx,bx      ; is y1=y2 -> Zero height?
; jne @@SL1      ; nope... continue
; ret            ; yes - quit scanning
;@@SL1:
; jl @@SL2       ; is y1 < y1 (y1 top) - if yes, continue
; xchg cx,ax     ; esle swap all x,y and color 1-2
; xchg dx,bx                                      
;@@SL2:
; push cx 	; save X start
; mov bp,bx      ; bp = y2     
; add bp,bp      ;      *2     
; cmp bp,[BotY]  ; Set new BotY if Y2*2 > BotY
; jb @@SL3   
; mov [BotY],bp      
;@@SL3:
; sub ax,cx	; ax = Width    (x2-x1)
; sub bx,dx	; bx = height   (y2-y1)
; mov bp,dx      ; bp = y1
; add bp,bp	; bp -> Ypos in Both buffers    (=y1*2)
; cmp bp,[TopY]  ; Set new TopY?
; ja @@SL4 
; mov [TopY],bp                 
;@@SL4:          
; shl eax,16     ; eax = width -> 16.16
; cdq            ; edx:eax = eax             
; movzx ebx,bx   ; ebx=bx -> Height
; idiv ebx     	; eax = Xslope 16.16      
; mov edi,eax	; edi = Xslope 
;                                      
; pop ax    	; X start (prev. push cx)
; shl eax,16     ; 16.16 - eax = X
;                          
; xchg bx,bp     ; Hadde et helvetes strev med † finne ut at hvis jeg hadde
;                ; bx og bp motsatt (bp = index til bufre og bx = counter),
;                ; s† funka det ikke... Denne enkle xchg'en fikser det!
; edi = Xslope 
; eax = X         
; bx = Height, bp = pos in buffers
;@@SL5:   
; shld edx,eax,16    	; dx = Xpos
; mov [LBuf+bx],dx   
; add bx,2           
; add eax,edi	    	; inc x,xslope        
; dec bp                                     
; jnz @@SL5          
; ret                                      
;ENDP                              
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
; CX= x1, DX = y1, AX = x2, BX = y2 (all must be clipped and positive)
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
;PROC ScanRight
; cmp dx,bx      ; is y1=y2 -> Zero height?
; jne @@SR1      ; nope... continue
; ret            ; yes - quit scanning
;@@SR1:
; jl @@SR2       ; is y1 < y1 (y1 top) - if yes, continue
; xchg cx,ax     ; esle swap all x,y and color 1-2
; xchg dx,bx                                      
;@@SR2:                         
; push cx 	; save X start
; mov bp,bx      ; bp = y2     
; add bp,bp      ;      *2     
; cmp bp,[BotY]  ; Set new BotY if Y2*2 > BotY
; jb @@SR3           
; mov [BotY],bp                            
;@@SR3:
; sub ax,cx	; ax = Width    (x2-x1)
; sub bx,dx	; bx = height   (y2-y1)
; mov bp,dx      ; bp = y1     
; add bp,bp	; bp -> Ypos in Both buffers    (=y1*2)
; cmp bp,[TopY]  ; Set new TopY?
; ja @@SR4
; mov [TopY],bp                 
;@@SR4:
; shl eax,16     ; eax = width -> 16.16
; cdq            ; edx:eax = eax
; movzx ebx,bx   ; ebx=bx -> Height
; idiv ebx     	; eax = Xslope 16.16
; mov edi,eax 	; edi = Xslope 
;                                      
; pop ax    	; X start (prev. push cx)
; shl eax,16     ; 16.16 - eax = X
;                                               
; xchg bx,bp     ; Hadde et helvetes strev med † finne ut at hvis jeg hadde
;                ; bx og bp motsatt (bp = index til bufre og bx = counter),
;                ; s† funka det ikke... Denne enkle xchg'en fikser det!
; edi = Xslope              
; eax = X         
; bx = Height, bp = pos in buffers
;@@SR5:      
; shld edx,eax,16    	; dx = Xpos
; mov [RBuf+bx],dx
; add bx,2           
; add eax,edi	    	; inc x,xslope        
; dec bp                           
; jnz @@SR5
; ret                              
;ENDP                              
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
;HorizLine MACRO  
; mov ax,dx 
; and ax,7       ;; ax = pixels in left byte       
; shr dx,3       
; inc dx         ;; byte # for first whole
; add di,dx          
; mov bx,cx          
; and bx,7       ;; bx = pixels in right byte      
; shr cx,3       ;; last byte
; sub cx,dx      ;; Number of whole bytes inbetween
; js @@InSameByte
; mov si,ax                
; mov al,[StartMask+si]    
; stosb                    
; cmp cx,0                 
; je @@SkipMiddle          
; mov al,255               
; rep stosb                
;@@SkipMiddle:             
; mov si,bx                
; mov al,[EndMask+si]      
; mov [es:di],al           
; jmp @@Finito             
;@@InSameByte:             
; mov si,ax              ;; Pixel # for left
; shl si,3                                      
; add si,bx              ;; Pixel # for right
; mov al,[SameMask+si]
; mov [es:di],al 
;@@Finito:                     
;ENDM                        
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ   
; Draws Polygon                                            
; ABsolutt ikke kjappest mulige versjon... F†r optimere senere...         
; Gj›r om til mode 0Dh
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ   
;PROC FillPoly  
; mov bx,[TopY] 
; mov bp,[BotY]    
; mov ax,[DrawSegment]
; mov es,ax        
;DrawLoop:         
; Mov di,[MulTable+bx]
; mov dx,[LBuf+bx]         
; mov cx,[RBuf+bx]       ; cx = lengde p† linje 
; push bx     
; HorizLine   
; pop bx      
; add bx,2    
; cmp bx,bp                    
; jl DrawLoop          
; ret                  
;ENDP                  
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
;PROC Polygon
; mov [TopY],ScreenHeight*2
; mov [BotY],0                      
; mov cx,[x1]   	      	; Linje 1-2
; mov dx,[y1]                       
; mov ax,[x2]                       
; mov bx,[y2]                       
; cmp dx,bx                         
; jb @@1                            
; Call ScanLeft                     
; jmp @@2                           
;@@1:                               
; Call ScanRight                    
;@@2:                               
; mov cx,[x2]   	      	; Linje 2-3
; mov dx,[y2]                       
; mov ax,[x3]                       
; mov bx,[y3]                       
; cmp dx,bx                         
; jb @@3                            
; Call ScanLeft
; jmp @@4                           
;@@3:                               
; Call ScanRight
;@@4:                               
; mov cx,[x1]	      	; Linje 1-3
; mov dx,[y1]          
; mov ax,[x3]          
; mov bx,[y3]          
; cmp dx,bx            
; jb @@5     
; Call ScanRight
; jmp @@6     
;@@5:                      
; Call ScanLeft 
;@@6:                          
; Call FillPoly
; ret                    
;ENDP                    

;ClearBuffer Macro 
; mov ax,SEG Buffer
; mov es,ax  
; xor di,di        
; mov ecx,2000     
; xor eax,eax      
; cld             
; rep stosd
;ENDM      
;                
;CopyBuffer Macro
; push ds        
; mov ax,seg buffer
; mov ds,ax      
; xor si,si 
; mov ax,0a000h                
; mov es,ax        
; xor di,di        
; mov ecx,2000           
; cld                           
; rep movsd
; pop ds
;ENDM            
                
PROC HLine
 mov ax,[LineY1]
 imul ax,40       
 mov di,ax
 mov cx,10
 mov eax,0      
 rep stosd
 ret      
ENDP      
          
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
PROC HandleText
 cmp [Sweep],1 
 jne NoSweepInProgress         
 mov ax,[SweepLine]
 mov [LineY1],ax
 Call HLine    
 inc [SweepLine]
 cmp [SweepLine],150    ; Last line to sweep
 jb NotSweepFinished
 mov [Sweep],0                             
NotSweepFinished:                          
 Jmp EndHandleText                         
NoSweepInProgress:                         
 mov bx,[TextIndex]
 inc [TextIndex]
 mov cl,[Text+bx]                          
 cmp cl,254             ; 254 = Sweep-clear text
 jne NotSweep                              
 mov [Sweep],1                             
 mov [SweepLine],50     ; 1st line to sweep
 mov [CharX],0                             
 mov [CharY],0                             
 Jmp EndHandleText                         
NotSweep:                                  
 cmp cl,253             ; 253 = Start over 
 jne CheckDelay
 mov [TextIndex],0                         
 jmp NoSweepInProgress                     
CheckDelay:     
 cmp cl,252     
 jne RegularText
 Jmp EndHandleText
RegularText:   
 mov ax,0a000h 
 mov es,ax     
 mov di,[chary]
 imul di,320            ; 40*8
 add di,(40*50)
 add di,[charx]               
 mov bl,cl                    
 Call DrawChar                
 inc [CharX]                  
 cmp [CharX],40               
 jb NotNextLine               
 mov [CharX],0
 inc [CharY]  
NotNextLine:
EndHandleText:
 ret
ENDP
 

PROC DrawDuff                 
 mov ax,SEG Picture
 mov fs,ax
 mov ax,0a000h     
; moc ax,seg buffer            
 mov es,ax         
 xor di,di         
; Call WaitVR      
 xor bx,bx                                  
 mov bl,[SinPos1]       ; Sinus-start
 mov cx,ScreenHeight       ; Number of lines to draw
 xor dx,dx              ; Linje-teller                                       
DuffLoop:                                   
 xor ax,ax                                  
 mov al,[DuffSinus+bx]
 cbw               
 add ax,dx              ; al = linje + sinus
 inc bl                 ; Sinus-index
 inc dl                 ; Linje nummer -> 0-200 + sinus
 add ax,[LinjePosisjon]
 cmp ax,0       
 jl SkipLine    
 cmp ax,199                                           
 jg SkipLine                                          
 movzx esi,ax   
 lea si,[esi+esi*4]
 shl si,3               ; * 40 : SI -> Offset in picture
 Jmp DontSkip                                           
SkipLine:                                               
 mov si,7960
DontSkip:                                               
; Draw Line     
 push cx   
 push ds      
 push fs
 pop ds
 mov cx,10
 rep movsd
 pop ds
 pop cx
;       
 dec cx                                                 
 jnz DuffLoop                                           
 ret            
ENDP                       
             
           
;° EntryPoint from DOS °°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
EntryPoint:   
 mov ax,@data 
 mov ds,ax      
 cld         
; Set up Mode 0Dh (320x200x16 - planar mode)
 mov ax,0dh                          
 int 10h                             
 Call SetPalette
; Copy logo to screen
 SetPlane 1          
 push ds             
 mov ax,logoseg      
 mov ds,ax           
 mov ax,0a000h       
 mov es,ax           

 xor si,si           
 mov di,(40*8)       
 mov ecx,(40*27)     
 rep movsb           

 xor si,si           
 mov di,(40*162)
 mov ecx,(40*27)     
 rep movsb           

 pop ds              
;
; mov ax,seg buffer   
; mov [DrawSegment],ax
 mov [SinPos1],0
 mov [LinjePosisjon],0
 mov [Sweep],0
 mov [TextIndex],0
MainLoop:       
; Call RotatePoints
; ClearBuffer    
; Call Polygon   
 Call WaitVR     
; SetPlane 8
; CopyBuffer
 SetPlane 4 
 Call DrawDuff
 add [SinPos1],1
 SetPlane 2
 Call HandleText
      
 in al,60h     
 cmp al,1      
 jne MainLoop  
               
 mov ax,3      
 int 10h
 mov dx,offset EndText
 mov ah,09h
 int 21h
 mov ax,4c00h  
 int 21h       
End EntryPoint 
