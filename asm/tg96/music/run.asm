;DOSSEG
.486p ;.386
.MODEL FLAT ;.MODEL MEDIUM
.STACK
;JUMPS
LOCALS
.CODE

include general.mac
 extrn   music_autoconfig:near           ;Novice Functions
 extrn   music_autoinit:near
 extrn   music_autoexit:near
 extrn   music_autoplay:near
 extrn   music_autostop:near
 extrn   music_autodetect:near           ;Special Functions
 extrn   music_autoidentify:near
 extrn   music_detectcard:near           ;Advanced Functions
 extrn   music_setcard:near              ;Regarding Soundcard
 extrn   music_initcard:near
 extrn   music_exitcard:near
 extrn   music_identify:near             ;Advanced Functions
 extrn   music_setformat:near            ;Regarding Module
 extrn   music_initplay:near
 extrn   music_exitplay:near
 extrn   music_play:near                 ;Advanced Functions
 extrn   music_stop:near
 extrn   music_loadconfig:near
 extrn   music_saveconfig:near
 extrn   music_set_setupproc:near
 extrn   music_unset_setupproc:near
 extrn   music_jmp_tick:dword            ;Special calls
 extrn   music_jmp_row:dword
 extrn   music_jmp_cmd:dword
 extrn   music_jmp_end:dword
 extrn   music_modulepos:dword           ;Variables
 extrn   music_drivers:dword             ;Advanced Variables
 extrn   music_formats:dword
 extrn   music_error:dword               ;Error codes
 extrn   music_errors:dword
 
 
;° DATA °°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
                   
old_stackptr dd	0
scancode db 0
syntax db 1," "
configfile db "player.cfg",0
system_msg db "System : $"
driver_msg db "Driver : $"
format_msg db "Format : $"
             
extrn   module:byte

;° CODE °°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
 
PROC init
 lea esi,configfile                
 call music_autoconfig             
 jc init_fail                      
 mov edx,eax                       
 mov ah,9h                         
 int 21h           
 lea edx,system_msg
 mov ah,9h         
 int 21h           
 mov edx,ecx       
 mov ah,9          
 int 21h           
 lea edx,driver_msg
 mov ah,9h         
 int 21h           
 mov edx,ebx       
 mov ah,9          
 int 21h           
; Load ParamStr(1) til buffer MODULE
                                    
 lea esi,module                         ; Point til buffer som loades
 call music_autoplay                                                 
 jnc init_done                                                       
 call music_autoexit                                                 
init_fail:                                                           
 mov edx,music_error                                                 
 mov edx,music_errors[edx*4]                                         
 mov ah,9h                                                           
 int 21h                                                             
 stc                                                                 
 ret                                                                 
init_done:                                                           
 mov ebx,eax                                                     
 lea edx,format_msg                                              
 mov ah,9h                                                       
 int 21h                                                         
 mov edx,ebx                                                     
 mov ah,9
 int 21h
 clc 
 ret
ENDP 
         
PROC Exit
 call music_autostop
 call music_autoexit
ret

;° EntryPoint from DOS °°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
EntryPoint:
 mov old_stackptr,esp
 mov ax,ds
 mov es,ax
 Call init
 jc fail
 mov ah,0
 int 16h    
 call exit
fail:   
 mov esp,old_stackptr
 mov eax,4c00h
 int 21h
End EntryPoint
