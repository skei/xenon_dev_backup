;DOSSEG
.486p ;.386
.MODEL FLAT,syscall
.STACK 4096
;JUMPS     
LOCALS     
.CODE      

include nms.inc
extrn   module:byte
               
;° DATA °°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
Screen dd ?    

extrn GUS_cmdlist:dword
extrn SB_cmdlist:dword
extrn NS_cmdlist:dword
extrn DSK_cmdlist:dword
drivers label dword
dd GUS_cmdlist
dd SB_cmdlist
dd NS_cmdlist
dd DSK_cmdlist
dd -1
   
extrn DOS32_cmdlist:dword
;extrn DOS4G_cmdlist:dword
systems label dword
dd DOS32_cmdlist
;dd DOS4G_cmdlist
dd -1
   
extrn MOD_cmdlist:dword
extrn S3M_cmdlist:dword
extrn DSM_cmdlist:dword
extrn XM_cmdlist:dword
formats label dword
dd MOD_cmdlist
dd DSM_cmdlist
dd S3M_cmdlist
dd XM_cmdlist
dd -1        
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
dec_divider dd 1,10,100,1000,10000,100000,1000000,10000000,100000000,1000000000
digit       dd 0
                                          
;° MACROS °°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
          
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
; DISPLAY A STRING ON SCREEN
; Usage: Write   <' string to display '>
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
Write MACRO STRING_, COLOR5_T
LOCAL TEXT5_T , skip_wrln
 push eax
 push edx
 jmp skip_wrln
 IFB <STRING_>
  TEXT5_T DB 36                
 ELSE 
  TEXT5_T DB STRING_,36
 ENDIF
skip_wrln:
 MOV EDX,OFFSET TEXT5_T
 mov ah,9
 int 21h
 pop edx
 pop eax
ENDM 
     
               
;° CODE °°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
               
PrintDEC PROC
        pushad
        mov     esi,4*5
        mov     ecx,eax
        xor     ebp,ebp
divloop:
        mov     eax,ecx
        xor     edx,edx
        div     dec_divider[esi]
        mov     ecx,edx                     ; reuse remainder
        or      ebp,ebp                     ; skip leading zeros
        jnz     valid_DECnumber
        or      esi,esi                     ; last zero has to be printed
        jz      valid_DECnumber
        or      al,al
        jz      F1
valid_DECnumber:
        inc     ebp
        add     al,'0'                      ; use quotient as digit number
        mov     dl,al
        mov     ah,2
        int     21h
F1:
        sub     esi,4
        jge     divloop
        popad
        ret
printDEC ENDP

printHEX  PROC
        pushad
        mov     [digit],eax
        mov     ecx,8
        xor     ebp,ebp
rolloop:
        rol     [digit],4
        mov     al,byte PTR [digit]
        and     al,0fh
        cmp     al,10
        jb      j8
        add     al,'A'-'0'-10
        or      ebp,ebp                     ; number needs leading zero if its first >=10
        jnz     valid_HEXnumber
        push    eax
        mov     dl,'0'                      ; the leading zero
        mov     ah,02h
        int     21h
        pop     eax
j8:
        or      ebp,ebp                     ; skip leading zeros
        jnz     valid_HEXnumber
        or      al,al
        jz      F1
valid_HEXnumber:  
        inc     ebp
        add     al,'0'
        mov     dl,al
        mov     ah,02h
        int     21h
F2:
        loop    rolloop
        or      ebp,ebp
        jnz     F3
        mov     ah,2
        mov     dl,'0'
        int     21h
F3:   
        mov     dl,'h'
        int     21h
        popad
        ret
printHEX ENDP
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
PROC Init
 lea eax,systems
 call nms_init
 jc init_fail
 mov edx,eax                    ; Write out system name
 mov ah,9h                      
 int 21h                        
 lea eax,drivers                ; Autodetect driver
 call nms_manualsetup           ; _autosetup
 jc init_fail
 call nms_driverinit            ; Init driver
 jc init_fail
 mov edx,eax                    ; Write out the soundcard name
 mov ah,9h
 int 21h  
 clc                            ; Succesfull!
 ret                              
init_fail:                        
 mov edx,nms_error                
init_fail2:                       
 mov edx,nms_errors[edx*4]        
 mov ah,9h                        
 int 21h                          
 stc                            ; Failed
 ret
ENDP 
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
PROC Main                         
 lea eax,formats                  
 lea ebx,module                   
 call nms_moduleinit            ; Init module
 jc main_fail                     
 mov edx,eax                    ; Write out format name
 mov ah,9h
 int 21h
 call nms_modulestart           ; Play
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
; Quickie Hack of a 'ModPlayer' .... hmmm
intro:                                 
 mov     edx,0600h                ; gotoXY
 xor     ebx,ebx                   
 mov     ah,2                      
 int     10h                       
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
 mov     dl,''                    
 mov     ah,2                      
 int     21h                       
 mov     dl,' '
 int     21h
innerloop:
 mov     eax,0C00h
 int     21h                      ; flush keyboardbuffer
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
 mov     edx,3DAh
@@B2:              
 in      al,dx                     
 test    al,8
 jnz     @@B2
@@B3:              
 in      al,dx
 test    al,8
 jz      @@B3
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
 mov edx,0800h               ; gotoXY
 xor ebx,ebx
 mov ah,2
 int 10h
 write 'PatternRow: '          ; display song infos
 mov ebx,[nms_ModulePos]
 xor eax,eax 
 mov ax,bx   
 call PrintDEC
 write '  OrderPos: '
 mov ebx,[nms_ModulePos]
 xor eax,eax 
 shld eax,ebx,16
 call PrintDEC
 write '  '
 mov     ah,01h                   ; getch
 int     16h
 cmp     ah,1                     ; esc ?
 jne     innerloop
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
 mov     eax,0C00h
 int     21h                      ; flush keyboardbuffer
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
 call nms_modulestop            ; Stop
 call nms_moduleexit            ; Uninit module
 ret       
main_fail:
 mov edx,nms_error              ; Error num
 mov edx,nms_errors[edx*4]      ; Error list
 mov ah,9h                        
 int 21h                        ; Write out error
 ret       
ENDP
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
PROC Exit
 call nms_driverexit            ; Uninit driver
 call nms_exit                  ; Uninit nms
 ret
ENDP 
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
;° EntryPoint from DOS °°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
EntryPoint:
 mov ax,ds                      ;For safety
 mov es,ax                      ;NMS needs it!
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
 mov eax,0EE02h
 int 31h
 mov edi,0A0000h
 sub edi,ebx
 mov [Screen],edi
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
 call init
 jc fail
 call main
 call exit
fail:
 mov eax,4c00h
 int 21h
End EntryPoint

 
