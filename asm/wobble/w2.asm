DOSSEG
.MODEL MEDIUM
.386
.STACK
JUMPS

BufferSeg SEGMENT       ; Fylles i omgang 1
Buffer db 64000 dup(0)
ENDS

PICWIDTH EQU 220
PICHEIGHT EQU 40

SpeedX1 EQU 3
SpeedX2 EQU 2
InPicX1 EQU 3
InPicX2 EQU 2
SINDIV EQU 2
SinWidth EQU 50

GraphSeg SEGMENT
label picture byte
 include eldpic.inc
label palette byte
 include eldpal.inc
ENDS

.DATA
Sinus label word        ; 256 Words (0-255)
 include sinus.inc

SinX1Index db 0
SinY1Index db 0
SinX2Index db 0
SinY2Index db 0

.CODE

;-----------------------------------------------------------------------------
; SetPalette - set the VGA color palette.
;-----------------------------------------------------------------------------
PROC SetPalette
 mov cx,768
 mov ax,seg Palette
 mov ds,ax
 mov si,offset palette
 mov     dx,3C8h
 xor     al,al
 out     dx,al
 inc     dx
 rep     outsb
 ret
ENDP


;-----------------------------------------------------------------------------
; WaitVR - Waits the Next VGA Vertical Retrace Ending.
;-----------------------------------------------------------------------------

proc    WaitVR
        mov     dx,3DAh
WaitStartVR:
        in      al,dx
        test    al,8
        je      WaitStartVR
WaitEndVR:
        in      al,dx
        test    al,8
        jne     WaitEndVR
        ret
endp    WaitVR


;------------------------------------------------------------
; BX = x1
; CX = X2
; SI -> Start of current line in picture
; DI -> Start Screen-pos
;------------------------------------------------------------
PROC DrawOneLine
 push ds
 mov ax,seg picture
 mov ds,ax
 mov eax,PicWidth
 shl eax,16
 cdq
 sub cx,bx
 movsx ebx,cx
 idiv ebx       ; EAX = 16.16 # to add to pic-index     þ Array IDIV þ
 mov ebx,0
DrawLoop:
 rol ebx,16
 mov dl,[ds:si+bx]
 mov dh,dl
 rol ebx,16
 add ebx,eax
 mov [es:di],dl
; mov [es:di],dx
 inc di
; inc di
 loop DrawLoop
 pop ds
 ret
ENDP

;------------------------------------------------------------
; Kalkulerer skjerm, og bilde-pos. DS/ES:SI/DI. CX
;------------------------------------------------------------
PROC CalcPos
 mov cx,0
 add [sinX1Index],SpeedX1
 add [SinX2Index],SpeedX2
 xor bh,bh
 mov bl,[Sinx1Index]
 shl bx,1
 xor dh,dh
 mov dl,[Sinx2Index]
 shl dx,1
 xor si,si              ; Picture startpos
 xor di,di              ; Screen startpos
CalcLoop:
 push dx
 mov ax,320
 mul cx                 ; ax = Y current line   þ FLERN MUL þ
 mov di,ax
 pop dx

 mov ax,[sinus+bx]
 add bx,2*InPicX1
 and bx,01ffh
 shr ax,SINDIV          ; sinus / 8 = 0-31
 add di,ax              ; di = y (ax) + leftsinus
 mov bp,ax              ; bp = Left

 push bx
 mov bx,dx
 mov ax,[sinus+bx]
 add bx,2*InPicX2
 and bx,01ffh
 shr ax,SINDIV          ; Ax = right
 mov dx,bx
 pop bx

 push cx        ; Ypos
 push bx        ; Left
 push dx

 mov bx,bp
 mov cx,ax
 add cx,200


 call DrawOneLine
 pop dx
 pop bx
 pop cx


 add si,PicWidth

 inc cx
 cmp cx,PicHeight
 jb CalcLoop
 ret
ENDP

;------------------------------------------------------------
; Entry point from DOS
;------------------------------------------------------------
EntryPoint:
 mov ax,13h
 int 10h
 cld
 Call SetPalette
 mov ax,@data
 mov ds,ax
 mov ax,0a000h
 mov es,ax
 assume cs:@code,ds:@data
WFK:
 Call WaitVR
 Call CalcPos
 in al,60h
 cmp al,1
 jne WFK

 mov ax,3h
 int 10h
 mov ax,4c00h
 int 21h
End EntryPoint

