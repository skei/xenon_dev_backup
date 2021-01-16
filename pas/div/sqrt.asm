; °°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
;  Function to calc 16 bit square root of 32 bit number in DIúSI, ret in AX
; °°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
.CODE
SqRoot  PROC
        xor     dx,dx
        mov     ax,1
        xor     bx,bx
        mov     cx,2
SRML:
        REPT 32
        sub     si,ax
        sbb     di,dx
        jc      SRExit
        add     ax,cx           ; Add 2
        adc     dx,bx           ; Add 0
        ENDM
        jmp     SRML
SRExit: clc
        rcr     dx,1
        rcr     ax,1
        ret
SqRoot  ENDP
