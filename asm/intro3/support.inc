;컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
; EDX -> '$'-terminated string
PROC Writeln
 mov ah,09h
 int 21h
 ret            
ENDP
;컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
setborder2 MACRO color
 push ax
 push dx
 cli  
 mov dx,3dah
 in al,dx
 mov dx,3c0h
 mov al,11h+32
 out dx,al
 mov al,color
 out dx,al
 sti 
 pop dx
 pop ax
 ENDM

setborder MACRO color
 setborder2 color
ENDM
;컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
PROC WaitVR   
 mov dx,3DAh      
@@1:               
 in al,dx          
 test al,8         
 jnz @@1
@@2:                
 in al,dx     
 test al,8     
 jz @@2            
 ret          
ENDP          
;컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
PROC WaitCX
@@1:
 Call WaitVR
 dec cx 
 jnz @@1
 ret
ENDP 
;컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
; ESI -> Palette
PROC SetPalette         
 mov dx,3c8h         
 xor al,al              
 out dx,al         
 inc dx        
 mov ecx,768    
 rep outsb        
 ret                
ENDP                   
;컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
