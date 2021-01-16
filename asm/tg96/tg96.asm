.486p             
.MODEL FLAT,syscall
.STACK 4096
;JUMPS            
LOCALS            
.CODE             
                  
include extra.inc
include music.inc
                            
;° DATA °°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
old_stackptr dd ?           
Screen dd ?                 
                            
WelcomeTxt db 'IN ABSENTIA CHRISTI - Xenon Development 1995',13,10,36
LineFeed db 13,10,36

BlankPalette label byte
 db 768 dup(0)
Palette1 label byte         
 include e:\tg96\gfx\xd1pal.db
XD_Pic1 label byte
 include e:\tg96\gfx\xd1raw.db 
                  
;° CODE °°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
             
                            

;° EntryPoint from DOS °°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
EntryPoint:
 mov eax,0EE02h
 int 31h
 mov edi,0A0000h
 sub edi,ebx           
 mov [Screen],edi      
 cld                                                                 
 mov old_stackptr,esp
 mov ax,ds         
 mov es,ax         

 mov edx,offset WelcomeTxt
 Call WriteLn
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
 ; Allocate 64K for BUFFER 
; mov ax,0ee42h             
; mov edx,90000h        
; int 31h                   
; jc QuitDemo               
; mov [Buffer],edx          
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
 Call StartMusic
; mov ecx,350            ; 5 sek
; call waitECX
 
; mov ax,13h                
; int 10h                                            
                                                    
; Call BlackPalette                                  
                                                    
; mov esi,offset XD_Pic1                             
; mov edi,[Screen]                                   
; mov ecx,64000                                      
; Call CopyBlock                                     

 mov eax,0EE02h
 int 31h
 mov edi,0B8000h
 sub edi,ebx           
Tst:        
 mov ah,2
 mov bh,0
 mov dh,0
 mov dl,0
 int 10h 
 mov eax,[music_modulepos]      ; SongPos:Row
 push eax
 and eax,0ffffh
 Call PrintHexWord
 mov edx,offset LineFeed
 Call WriteLn
 pop eax        
 shr eax,16     
 Call PrintHexWord
 
; mov [edi],al
; shr eax,16
; mov [edi+2],al
 in al,60h  
 cmp al,1
 jne Tst
         
; rept 64                                            
;  mov esi,offset BlankPalette                       
;  mov edi,offset Palette1
;  mov ecx,24
;  mov al,0
;  Call FadePalette
; endm
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
; mov ah,0
; int 16h
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
QuitDemo:                  
 Call StopMusic
 mov ax,3                  
 int 10h                   
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
; All Allocated memory is automatically freed upon termination, so we don't
; have to free it here.....
 mov esp,[old_stackptr]
 mov ax,4c00h
 int 21h
End EntryPoint
