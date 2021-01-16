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
        ;setborder2 color
        ENDM
