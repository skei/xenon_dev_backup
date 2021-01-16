===============================================================================
     Date: 01-05-95    Time: 05:04a     Number: 402    
     From: JARNO HEIKKINEN               Refer: 0       
       To: NIKO SIPILA                Board ID: SIDEWALK        Recvd: No 
  Subject: suck this             2/2        51: DGI.Demos      Status: Public 
-------------------------------------------------------------------------------
EID: VISION-X .99CMSGID: 68:100/411.0
USERNOTE:=complex/jeskola!=
úÿ [ ...Continued From Previous Message ]

        add     cx, [cs:bumpy+4+2]
        add     dx, [cs:bumpy+0+2]
        call    viivoitin
        pop     si cx
        add     si, 4
        loop    @@p
@@skip: VGA_border 0
        vga_fullvsync
        VGA_border 3
        call    copyframe
        VGA_border 6
        mov     ah, 1
        int     16h
        jz      l2
        mov     ah, 0
        int     16h
puis:   vga_textmode
        exitdos
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
bss_hunk babe, "babe", 2000
viivoja=347
plane   dw 1111h
bumpy   dw 0, 0, 0, 0
fibo1   dw 56153
fibo2   dw 34234
segment framebuf
        db 80*512 dup (0)
ends
end     main
---
 þ SPEED 1.40 [NR] þ SUOMI FINLAND PERKELE!

--- ViSiON-X FiDOMAIL
 * Origin: I love Jeskola! (68:100/411)
===============================================================================
