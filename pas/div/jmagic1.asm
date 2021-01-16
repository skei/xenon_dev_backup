roughriding = 7
        ideal
        jumps
        model small
        stack 100h
        codeseg
        p386
;include 'macros.inc'
;include 'line.inc'
copyframe:
        mov     dx, 3c4h
        mov     ah, [byte cs:plane]
        and     ah, 0fh
        jz      clearbuffer
        mov     al, 02
        out     dx, ax
        push    framebuf+80
        pop     ds
        push    0a000h+40*5
        pop     es
        xor     si, si
        xor     di, di
        mov     cx, 80*400/4
        rep     movsd
clearbuffer:
        push    framebuf+80
        pop     es
        xor     di, di
        mov     cx, 80*400/4
        xor     eax, eax
        rep     stosd
        mov     ax, [cs:plane]
        ror     ax, 1
        mov     [cs:plane], ax
        ret
main:   push    cs
        pop     ds
        mov     ax, 0012h
        int     10h
        mov     bl, 15
@@palloop:
        mov     dx, 03dah
        in      al, dx
        mov     dx, 03c0h
        mov     al, bl
        out     dx, al
        out     dx, al
        dec     bl
        jns     @@palloop
        call    clearbuffer
l2:     cmp     [byte cs:plane], 0
        jz      @@skip
        push    framebuf+80
        pop     ds
        mov     cx, viivoja
        mov     si, offset babe+2
@@p:    push    cx si
        mov     eax, [dword cs:bumpy+0]
        mov     [dword cs:bumpy+4], eax
        call    fibonacci
        and     ax, roughriding
        mov     [cs:bumpy+0], ax
        call    fibonacci
        and     ax, roughriding
        mov     [cs:bumpy+2], ax
        xor     dx, dx
        mov     ax, [cs:si]
        mov     di, 320
        div     di
        shl     ax, 1
        shl     dx, 1
        mov     cx, ax
        mov     ax, dx
        push    ax
        xor     dx, dx
        mov     ax, [cs:si+2]
        mov     di, 320
        div     di
        shl     ax, 1
        shl     dx, 1
        mov     bx, dx
        mov     dx, ax
        pop     ax
        add     ax, [cs:bumpy+4]
        add     bx, [cs:bumpy+0]

        add     cx, [cs:bumpy+4+2]
        add     dx, [cs:bumpy+0+2]
;        call    viivoitin
        pop     si cx
        add     si, 4
        loop    @@p
@@skip:
        call    copyframe
        mov     ah, 1
        int     16h
        jz      l2
        mov     ah, 0
        int     16h
puis:   mov ax,3h
        int 10h
        mov ax,4c00h
        int 21h
fibonacci:
        mov     ax, [cs:fibo1]
        mov     dx, [cs:fibo2]
        mov     [cs:fibo2], ax
        add     ax, dx
        mov     [cs:fibo1], ax
        ret    
palette:db 56,63,63
        db 48,55,55
        db 48,55,55
        db 40,47,47
        db 48,55,55
        db 40,47,47
        db 40,47,47
        db 32,39,39
        db 48,55,55
        db 40,47,47
        db 40,47,47
        db 32,39,39
        db 40,47,47
        db 32,39,39
        db 32,39,39
        db 24,31,31
babe:   db 'babe', 208,7
viivoja=347    
plane   dw 1111h
bumpy   dw 0, 0, 0, 0
fibo1   dw 56153
fibo2   dw 34234
segment framebuf
        db 80*512 dup (0)
ends
end     main
