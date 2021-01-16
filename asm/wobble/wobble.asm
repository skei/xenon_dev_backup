;Wobble

.Model Medium
DosSeg
.Stack 1024
.386

PicWidth EQU 160
PicHeight EQU 100

.data

Label Picture word
 include Wobble.pic
Label Palette word
 Include Wobble.pal 
label Sinus word
 include sinus.inc

SinStart dw 200 dup(0)
SinEnd   dw 200 dup(0) 

LeftSinIndex dw 0
RightSinIndex dw 0

screenSeg dw 0a000h

.code


SetPalette proc near	; Setter paletten
 mov si,seg Palette
 mov ds,si
 mov si,offset Palette
 mov cx,768
 mov dx,3C8h
 xor al,al
 out dx,al
 inc dx
 rep outsb
 ret
SetPalette endp

ClearScreen Proc Near
 mov di,[ScreenSeg]
 mov es,di
 mov di,0
 mov eax,0h
 mov cx,8000
 rep stosd
 ret
ClearScreen EndP 

SetScreenAdr Proc Near	;BX = Screen Offset. Segment = 0a000h
 mov dx,3d4h
 mov al,0dh
 mov ah,bl
 out dx,ax
 dec al
 mov ah,bh
 out dx,ax
 ret
SetScreenAdr EndP 

;Tegner en linje p† skjermen. AX = Ypos b†de p† skjerm og i bilde i memory
; BX = Left Xpos. CX = Right Xpos
DrawOneLine Proc Near
 pusha
 cld
 Mov si,seg picture
 mov ds,si
 mov si,offset picture	;DS:SI -> Picture in memory
 push ax
;AX = Ypos
 mov dx,PicWidth
 Mul dx
 add si,ax
 pop ax 

 Mov di,[ScreenSeg]
 mov es,di
 push ax
 ; AX = Ypos
 mov dx,320
 mul dx
 mov di,ax

; BX = LeftX
 add di,bx		; ES:DI = $a000:Yline*320+Leftx
 pop ax
; CX = RightX
 sub cx,bx		; CX = Rightx-leftx = Lengde p† inje

 mov dx,0
 mov ax,PicWidth*256
 div cx
 mov bx,ax	; BH = Hele BL=rest/256-deler
 mov dx,0
 
tegnlinje:
; DS:SI -> Picutre in Memory
; ES:DI -> ScreenPos
; CX = Width of draw-line
; BH = Hele pic increments
; BL = /256 inc's
 mov al,[DS:SI]
 push bx
 mov bl,bh
 mov bh,0
 add si,bx
 pop bx
 push bx
 and bx,255
 add dx,bx
 pop bx
 cmp dx,256
 jb notadddivider
 sub dx,256
 add si,1
notadddivider:
 stosb
 loop tegnlinje
 popa
 ret
DrawOneLine EndP

proc    WaitVR		; Venter p† vertical retrace
	push ax
	push dx
        mov     dx,3DAh
WaitStartVR:
        in      al,dx
        test    al,8
        je      WaitStartVR
WaitEndVR:
        in      al,dx
        test    al,8
        jne     WaitEndVR
        pop dx
        pop ax
        ret
endp    WaitVR

 

Start:
 Mov ax,13h
 int 10h
 Call SetPalette
 mov [leftsinindex],64
; mov bx,1		;Xpos 1
; mov cx,300		;Xpos 2
MainLoop:
 call waitvr
; mov bx,[ScreenSeg]	; Tweaked Mode
; sub bx,0a000h
; shl bx,4
; Call SetScreenAdr
; call ClearScreen
 mov ax,0
; call setsinus
flerelinjer:
 mov bx,[rightsinindex]
 and bx,0ffh
 add bx,bx
 add bx,ax
 add bx,ax
 and bx,1ffh
 mov bx,ds:sinus[bx]
 shr bx,1
 add bx,160
 mov cx,bx

 mov bx,[leftsinindex]
 and bx,0ffh
 add bx,bx
 add bx,ax
 add bx,ax
 and bx,1ffh
 mov bx,ds:sinus[bx]
 shr bx,1

 Call DrawOneLIne
 inc ax
 cmp ax,PicHeight
 jb flerelinjer
 add [leftsinindex],3
 add [rightsinindex],2
notmax: 

; mov ax,[ScreenSeg]		;Tweaked Mode !
; Cmp ax,0a000h
; jne nota000
; mov [ScreenSeg],0a3e8h
; jmp finsetnewscr
nota000: 
; mov [ScreenSeg],0a000h
finsetnewscr:

 mov dx,60h
 in al,dx
 cmp al,1
 jne MainLoop
 mov ax,3h
 int 10h
 Mov ax,4c00h
 int 21h
End Start