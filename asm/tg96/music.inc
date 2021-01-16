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
                   
driver_msg db "Driver : $"
AskConfigMsg db "Press M for Manual config, any other key for AutoDetection.",13,10,36
                
extrn   module:byte
                
;° CODE °°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
                     
PROC StartMusic      
 mov edx,offset AskConfigMsg
 Call WriteLn
 mov ah,0
 int 16h 
 cmp al,'M'
 je @@Manual
 cmp al,'m'
 je @@Manual
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
@@Auto:     
 call music_autoinit
 jnc @@1 
 mov ecx,[music_error]       
 mov edx,[music_errors+ecx*4]
 Call WriteLn
 mov ax,4c00h
 int 21h    
@@1:            
 mov edx,offset driver_msg
 Call WriteLn         
 mov edx,ebx          
 Call WriteLn         
;ÄÄÄÄÄÄÄÄ
; Start Playing
;ÄÄÄÄÄÄÄÄ
 mov esi,offset module
 call music_autoplay
 jnc @@2 
 call music_autoexit
 mov ecx,[music_error]       
 mov edx,[music_errors+ecx*4]
 Call WriteLn
 mov ax,4c00h
 int 21h    
@@2:     
 ret     
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
@@Manual:
 mov esi,0
 call music_autoconfig
 jnc @@3
 mov ecx,[music_error]       
 mov edx,[music_errors+ecx*4]
 Call WriteLn
 mov ax,4c00h
 int 21h    
@@3:        
;ÄÄÄÄÄÄÄÄ
; Start Playing
;ÄÄÄÄÄÄÄÄ
 mov esi,offset module
 call music_autoplay
 jnc @@2 
 call music_autoexit
 mov ecx,[music_error]       
 mov edx,[music_errors+ecx*4]
 Call WriteLn
 mov ax,4c00h
 int 21h    
 ret
ENDP     
         
PROC StopMusic
 call music_autostop
 call music_autoexit
 ret     
ENDP     
         
                      
            
            
;ÄÄÄÄÄÄÄÄ   
@@Manual: