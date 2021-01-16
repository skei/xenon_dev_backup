;DOSSEG    
.486p ;.386
.MODEL FLAT ;.MODEL MEDIUM
.STACK     
;JUMPS     
LOCALS     
.CODE      
           
;° DATA °°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
screen dd ?
StartMessage db 'Xenon Development - Intro #3 - 1995',13,10,'$'
;° CODE °°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
 
include e:\intro3\support.inc
include e:\intro3\wobtun\wobtun.asm
;include e:\intro3\music\music.asm

;° EntryPoint from DOS °°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
EntryPoint:
 mov eax,0EE02h
 int 31h            
 mov edi,0A0000h
 sub edi,ebx
 mov [Screen],edi
 
 mov edx,offset StartMessage                                   
 Call Writeln                                                  
; Call StartMusic
; jc Fail
 
; mov cx,150
; Call WaitCX
 mov ax,13h
 int 10h
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
; Wobbling-Tunell
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
 mov esi,offset WobTunPal
 Call SetPalette                 
 mov [SinusStart],0100h
 mov [SinusAdder],0100h
 mov [RotZoom],0
MainLoop:
 Call DoWobTun
 in al,60h             
 cmp al,1              
 jne MainLoop
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
 mov ax,3      
 int 10h
; Call StopMusic
Fail:
 mov ax,4c00h
 int 21h
End EntryPoint
