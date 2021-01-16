.386
.model flat,syscall
.stack 4096
.code

                include nms.inc

;ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿
;³°°°°°°°°°°°°°° Code °°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°³
;ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ

start32:                mov     ax,ds                           ;For safety
                        mov     es,ax                           ;NMS needs it!

			call	init
			jc	fail

                        call    main

                        call    exit

fail:                   mov     eax,4c00h
			int	21h

;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄ´ Init ÃÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

init:
                        mov     eax,0ee02h                      ;Get screen offset
                        int     31h
                        mov     eax,0b8000h
                        sub     eax,ebx
                        mov     screen,eax

                        lea     eax,systems                     ;Init
			call	nms_init
			jc	init_fail
                        mov     edx,eax                         ;Write out system name
			mov	ah,9h
			int	21h

                        lea     eax,drivers                     ;Autodetect driver
			call	nms_autosetup
			jc	init_fail

                        call    nms_driverinit                  ;Init driver
			jc	init_fail
                        mov     edx,eax                         ;Write out the soundcard name
			mov	ah,9h
			int	21h

                        clc                                     ;Succesfull!
			ret

init_fail:              mov     edx,nms_error
init_fail2:             mov     edx,nms_errors[edx*4]
                        mov     ah,9h
                        int     21h
                        stc                                     ;Failed
			ret

;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄ´ Main ÃÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

main:			lea	eax,formats
			lea	ebx,module
                        call    nms_moduleinit                  ;Init module
			jc	main_fail
                        mov     edx,eax                         ;Write out format name
			mov	ah,9h
			int	21h

                        call    nms_modulestart                 ;Play

                        lea     eax,vblank                      ;Routine to jump to every vblank
                        call    nms_activatesync

                        mov     nms_jmp_tick,offset tick        ;Jump to tick every tick
                        mov     nms_jmp_row,offset row          ;Jump to row every row
                        mov     nms_jmp_cmd,offset cmd          ;Jump to cmd every every special command
                        mov     nms_jmp_end,offset end          ;Jump to end every time module hits end

                        xor     eax,eax
                        int     16h                             ;Wait for key

                        mov     nms_jmp_tick,offset nms_dummy   ;Restore
                        mov     nms_jmp_row,offset nms_dummy    ;Restore
                        mov     nms_jmp_cmd,offset nms_dummy    ;Restore
                        mov     nms_jmp_end,offset nms_dummy    ;Restore

                        call    nms_deactivatesync              ;Deactivate

                        call    nms_modulestop                  ;Stop

                        call    nms_moduleexit                  ;Uninit module
			ret

main_fail:              mov     edx,nms_error                   ;Error num
                        mov     edx,nms_errors[edx*4]           ;Error list
                        mov     ah,9h
                        int     21h                             ;Write out error
			ret
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄ´ Exit ÃÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

exit:                   call    nms_driverexit                  ;Uninit driver
                        call    nms_exit                        ;Uninit nms
			ret

;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄ´ Other ÃÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

vblank                  PROC    USES ebx
                        mov     ebx,screen
                        inc     dword ptr [ebx]
                        ret
vblank                  ENDP

tick                    PROC    USES ebx
                        mov     ebx,screen
                        inc     dword ptr 4[ebx]
                        ret
tick                    ENDP

row                     PROC    USES ebx
                        mov     ebx,screen
                        inc     dword ptr 8[ebx]
                        ret
row                     ENDP

cmd                     PROC    USES eax edx                    ;Special Commands XM='S',MOD='8'
                        lea     edx,cmdtext                     ;AL is the syntax byte
                        mov     ah,9h
                        int     21h
                        ret
cmdtext                 db      'Special Command activated!',13,10,36
cmd                     ENDP

end                     PROC
                        lea     edx,endtext
                        mov     ah,9h
                        int     21h
                        ret
endtext                 db      'End of module reached!',13,10,36
end                     ENDP

;ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿
;³°°°°°°°°°°°°°° Data °°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°³
;ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ

screen                  dd      0b8000h

; Simply include the drivers you'd like to support...
; Some linkers links files set with extern even if not used, so remark the
;  externals too..
;
; Example:
;
;                extrn   GUS_cmdlist:dword
;                ;extrn   SB_cmdlist:dword
;                ;extrn   NS_cmdlist:dword
;                ;extrn   DSK_cmdlist:dword
;drivers                 label   dword
;                        dd      GUS_cmdlist
;                        ;dd      SB_cmdlist
;                        ;dd      NS_cmdlist
;                        ;dd      DSK_cmdlist
;                        dd      -1
;
; Which will only support GUS
;
                extrn   GUS_cmdlist:dword
		extrn	SB_cmdlist:dword
                extrn   NS_cmdlist:dword
                extrn   DSK_cmdlist:dword
drivers                 label   dword
                        dd      GUS_cmdlist
                        dd      SB_cmdlist
                        dd      NS_cmdlist
                        dd      DSK_cmdlist
                        dd      -1

		extrn	DOS32_cmdlist:dword
		extrn	DOS4G_cmdlist:dword
systems                 label   dword
			dd	DOS32_cmdlist
			dd	DOS4G_cmdlist
                        dd      -1

		extrn	MOD_cmdlist:dword
		extrn	S3M_cmdlist:dword
		extrn	DSM_cmdlist:dword
		extrn	XM_cmdlist:dword
formats 		label	dword
                        dd      MOD_cmdlist
                        dd      DSM_cmdlist
                        dd      S3M_cmdlist
			dd	XM_cmdlist
			dd	-1

                extrn   module:byte
;ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿
;³°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°³
;ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ
			end start32
