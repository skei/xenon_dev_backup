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
 
extrn   module:byte
;旼컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴커
;낡같같같같같같� Code 같같같같같같같같같같같같같같같같같같같같같같같같같같같같�
;읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴켸
 
 extrn   GUS_cmdlist:dword
; extrn   AWE32_cmdlist:dword
; extrn   SB_cmdlist:dword
; extrn   PAS_cmdlist:dword
; extrn   NOSOUND_cmdlist:dword
drivers label   dword           ;Simply include pointers to the soundcard you wish to support here:
 dd      GUS_cmdlist
; dd      SB_cmdlist
 dd      -1
 
extrn   DOS32_cmdlist:dword
; extrn   DOS4G_cmdlist:dword
systems label   dword           ;Ofcoz only DOS32 should be supported!
 dd      DOS32_cmdlist
 dd      -1
 
 extrn   MOD_cmdlist:dword
; extrn   S3M_cmdlist:dword
; extrn   DSM_cmdlist:dword
formats label   dword           ;And the module type you want!
 dd      MOD_cmdlist
 dd      -1

;컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
; Initialize music system
; 
;컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
Proc InitMusic
 lea eax,systems
 call nms_init 
 jc init_fail
 lea eax,drivers
 call nms_autosetup
 jc init_fail
 call nms_driverinit
 jc init_fail
 clc
 ret
init_fail:
 mov edx,nms_error
init_fail2:
 mov edx,nms_errors[edx*4]
 mov ah,9h
 int 21h
 stc 
 ret 
ENDP
    
PROC StartMusic
 lea eax,formats
 lea ebx,module
 call nms_moduleinit
 jc main_fail  
 call nms_modulestart
 ret
main_fail:     
 ret           
ENDP           
               
PROC StopMusic
 call nms_modulestop
 call nms_moduleexit
 ret
ENDP 
  
