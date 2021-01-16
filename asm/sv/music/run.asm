.386
.model flat,syscall
.stack 4096
.code

                extrn   nms_init:near
                extrn   nms_exit:near

                extrn   nms_autosetup:near
                extrn   nms_manualsetup:near    ;Doesn't work yet
                extrn   nms_configsetup:near    ;Doesn't work yet

                extrn   nms_driverinit:near
                extrn   nms_driverexit:near

                extrn   nms_moduleinit:near
                extrn   nms_moduleexit:near
                extrn   nms_modulestart:near
                extrn   nms_modulestop:near

                extrn   nms_error:dword
                extrn   nms_errors:dword

;ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿
;³°°°°°°°°°°°°°° Code °°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°³
;ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ

start32:                jmp     start32_real
                        db      "WATCOM... NOT!"

                extrn   GUS_cmdlist:dword
;                extrn   AWE32_cmdlist:dword
;                extrn   SB_cmdlist:dword
;                extrn   PAS_cmdlist:dword
;                extrn   NOSOUND_cmdlist:dword
drivers                 label   dword           ;Simply include pointers to the soundcard you wish to support here:
                        dd      GUS_cmdlist
;                        dd      SB_cmdlist
                        dd      -1

                extrn   DOS32_cmdlist:dword
;                extrn   DOS4G_cmdlist:dword
systems                 label   dword           ;Ofcoz only DOS32 should be supported!
                        dd      DOS32_cmdlist
                        dd      -1
 
                extrn   MOD_cmdlist:dword
;                extrn   S3M_cmdlist:dword
;                extrn   DSM_cmdlist:dword
formats                 label   dword           ;And the module type you want!
                        dd      MOD_cmdlist
                        dd      -1
 
start32_real:           mov     old_stackptr,esp
                        mov     ax,ds
                        mov     es,ax
                        call    init
                        jc      fail
                        call    main
			call	exit
fail:                   mov     esp,old_stackptr
                        mov     eax,4c00h
			int	21h

;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄ´ Init ÃÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

init:                   lea     eax,systems
                        call    nms_init
                        jc      init_fail

                        lea     eax,drivers
                        call    nms_autosetup
                        jc      init_fail

                        call    nms_driverinit
                        jc      init_fail

                        clc
                        ret
init_fail:              mov     edx,nms_error
init_fail2:             mov     edx,nms_errors[edx*4]
                        mov     ah,9h
                        int     21h
                        stc
                        ret

;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄ´ Main ÃÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

main:                   lea     eax,formats
                        lea     ebx,module
                        call    nms_moduleinit
                        jc      main_fail
                        call    nms_modulestart
                        call    readkey
                        call    nms_modulestop
                        call    nms_moduleexit
main_fail:              ret
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄ´ Exit ÃÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

exit:                   call    nms_driverexit
                        call    nms_exit
                        ret

;ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿
;³°°°°°°°°°°°°°° Subroutines °°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°³
;ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ

dosshell:               mov     ax,0ee02h
                        int     31h

dosshell_l1:            cmp     dword ptr [edi],'SMOC'
                        jne     dosshell_notit
                        cmp     dword ptr 4[edi],'=CEP'
                        je      dosshell_thatsit
dosshell_notit:         mov     al,0
                        mov     ecx,100
                        repne   scasb
                        cmp     byte ptr [edi],0
                        je      dosshell_fail
                        jmp     dosshell_l1
dosshell_thatsit:       add     edi,8

                        mov     ax,4b00h
                        mov     edx,edi
                        lea     esi,syntax
                        xor     edi,edi
                        int     21h
                        jc      dosshell_fail
                        ret
dosshell_fail:          xor     eax,eax
                        int     16h
                        ret

keypressed:             cmp     scancode,0
                        jne     keypressed_1
                        mov     ah,1
                        int     16h
                        jne     keypressed_1
                        clc
                        ret
keypressed_1:           stc
                        ret
readkey:                mov     al,scancode
                        mov     scancode,0
                        or      al,al
                        jne     readkey_1
                        xor     ah,ah
                        int     16h
                        or      al,al
                        jne     readkey_1
                        mov     scancode,ah
                        or      ah,ah
                        jne     readkey_1
                        mov     al,'c'-64
readkey_1:              ret

;ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿
;³°°°°°°°°°°°°°° Data °°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°³
;ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ
old_stackptr		dd	0
scancode		db	0

syntax                  db      1," "

                extrn   module:byte
;ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿
;³°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°³
;ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ
			end start32


