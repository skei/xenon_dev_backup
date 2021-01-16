.MODEL MEDIUM
.386

; GFX.OBJ
; Axon / Xenon Development
; Various VGA (Mode 13h & Mode-X) stuff in one global (.OBJ) file.....

GLOBAL SetModeX:proc
GLOBAL WaitVR:proc
GLOBAL SetPalette:proc
GLOBAL Delay:proc
GLOBAL Random:proc

.DATA

RandSeed DW 1234h

.CODE
;-----------------------------------------------------------------------------
; SetModeX - Sets the VGA in Tweaked ModeX 320x200x256.
;-----------------------------------------------------------------------------
proc    SetModeX
        mov     ax,0013h                ; Sets VGA linear 320x200x256
        int     10h
        mov     dx,3C4h                 ; Disable Chain-Four
        mov     ax,0604h
        out     dx,ax
        mov     dx,3C4h                 ; Enable Write to All Four Planes
        mov     ax,0F02h
        out     dx,ax
        mov     ax,0A000h               ; Clear Display Memory
        mov     es,ax
        xor     di,di
        xor     ax,ax
        mov     cx,8000h
        cld
        rep     stosw
        mov     dx,3D4h                 ; Reprogram CRT Controller:
        mov     ax,00014h               ; turn off dword mode
        out     dx,ax
        mov     ax,0e317h               ; turn on byte mode
        out     dx,ax
        mov     ax,00109h               ; cell height (0009 = 400, etc)
        out     dx,ax
        mov     dx,3D4h                 ; Sets Logical Screen Width
        mov     al,13h
        mov     ah,320/8
        out     dx,ax
        ret
endp    SetModeX

;-----------------------------------------------------------------------------
; WaitVR - Waits until Vertical Retrace is in progress (not updating screen)
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

;-----------------------------------------------------------------------------
; SetPalette - set the CX entries of the VGA color palette.
; In:
;   DS:SI - Palette structure address.
;-----------------------------------------------------------------------------

SetPalette      proc
;		Call WaitVR
                mov     dx,3C8h
                xor     al,al
                out     dx,al
                inc     dx
                mov	cx,768
                rep     outsb
                ret
SetPalette      endp

;-----------------------------------------------------------------------------
; Delay - Sleeps during any amount of time. Uses the VGA Vertical retrace.
; In:
;   CX - Number of ticks to speed (ticks at 70Hz).
;-----------------------------------------------------------------------------

Delay           proc
DelayLoop:      call    WaitVR
                loop    DelayLoop
                ret
Delay           endp

;-----------------------------------------------------------------------------
; Random - Returns a random number of 16 bits, modified version of the
;  ripped System unit random routine of the Borland Pascal 7.0.
; Out:
;  AX - random value.
;-----------------------------------------------------------------------------
Random          proc
                mov     ax,[RandSeed]
                mov     dx,8405h
                mul     dx
                inc     ax
                mov     [RandSeed],ax
                ret
Random          endp

End