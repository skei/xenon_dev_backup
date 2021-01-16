;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
    
DOSSEG
.MODEL MEDIUM
.STACK
.386
; JUMPS         
LOCALS
    
;° DATA °°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
.DATA    
               
Vars dw 20 dup(?)
Xpos dw ?
LineX dw ?
LineC db ?
Counter db ?
                   
;° CODE °°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
.CODE                              

PROC WaitVR
 mov dx,3DAh
WaitEndVR:
 in al,dx
 test al,8              ; Vent hvis Retrace allerede er i gang
 jnz WaitEndVR
WaitStartVR:
 in al,dx
 test al,8              ; Vent hvis VRbit = 0 dvs. vi er i skjerm
 jz WaitStartVR         ; vent
 ret
ENDP

PROC VertLine
 mov al,[LineC]
 mov di,[LineX]
 cmp di,319
 jg finito
 cmp di,0
 jl finito      
 i=0   
 rept 200
  mov [es:di+i],al
  i=i+320
 endm          
finito:        
 ret           
ENDP           
               
PROC Clear     
 xor ebx,ebx        
More:          
 mov ax,[Vars+ebx*2]
 cmp ax,16
 jge SkipThisLine
 inc [Vars+ebx*2]
 mov cx,bx     
 shl cx,4                       ; * 16
 add cx,[Vars+ebx*2]
 mov [LineX],cx 
 call VertLine 
SkipThisLine:  
 inc ebx       
 cmp bx,[Xpos]
 jb more
 ret   
ENDP   
               
;° EntryPoint from DOS °°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
EntryPoint:     
 mov ax,@DATA   
 mov ds,ax
 mov ax,13h
 int 10h
 mov ax,0a000h
 mov es,ax
 mov [Xpos],0
 mov [LineC],15
 i=0
 rept 20
  mov [Vars+i],0
  i=i+2
 endm
 mov [counter],34
 mov ah,0
 int 16h
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
TestLoop:        
 inc [Xpos]  
 Call Clear  
; Wait for retrace
 Call WaitVR
 dec [Counter]
 jnz TestLoop
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
 mov ah,0
 int 16h
 mov ax,3
 int 10h
 mov ax,4c00h
 int 21h
END EntryPoint
