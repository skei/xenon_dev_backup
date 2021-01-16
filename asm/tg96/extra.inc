b equ byte ptr    
w equ word ptr    
d equ dword ptr   

;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
; EDX -> '$'-terminated string
PROC Writeln
 mov ah,09h
 int 21h
 ret            
ENDP
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
HexTable db '0123456789ABCDEF'
StringToPrint db '0000h$'
; EAX = Number          
PROC PrintHexWord      
 and eax,0000ffffh
    
 mov ebx,eax           
 and ebx,0000F000h     
 shr ebx,12            
 mov cl,[HexTable+ebx] 
 mov b [StringToPrint],cl
                       
 mov ebx,eax           
 and ebx,00000F00h     
 shr ebx,8             
 mov cl,[HexTable+ebx] 
 mov b [StringToPrint+1],cl
                       
 mov ebx,eax           
 and ebx,000000F0h
 shr ebx,4
 mov cl,[HexTable+ebx] 
 mov b [StringToPrint+2],cl
                       
 mov ebx,eax           
 and ebx,0000000Fh
 mov cl,[HexTable+ebx] 
 mov b [StringToPrint+3],cl
 
 mov edx,offset StringToPrint
 Call Writeln
 ret
ENDP 
                       
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
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
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
; ESI -> Palette
; AL = StartColor
; ECX = number of entries (colors*3)
PROC SetPalette                    
 cld                               
 mov dx,3c8h                       
; xor al,al
 out dx,al       
 inc dx            
; mov cx,768      
@@1:  
 mov al,[esi]
 inc esi  
 out dx,al
 dec ecx
 jnz @@1
 ret                
ENDP                   
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
PROC BlackPalette
 cld      
 mov dx,3c8h
 xor al,al
 out dx,al       
 inc dx            
 mov cx,768
 xor al,al
@@1:
 out dx,al
 dec cx
 jnz @@1
 ret                
ENDP                   
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
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
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
PROC WaitECX
@@1:      
 Call WaitVR
 dec ecx         
 jnz @@1
 ret  
ENDP  
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
; ESI -> Source
; EDI -> Destination
; ECX = Number of bytes to copy ( nultiply of 4 for dword moves)
PROC CopyBlock                                                  
 shr ecx,2
 rep movsd
 ret      
ENDP  
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
PROC FadePalette
; ESI -> FromPalette
; EDI -> ToPalette
; AL = StartColor to fade
; ECX = Number of entries to fade (*3)
 mov edx,ecx    
 mov ebp,esi
@@A:        
 mov bl,[ebp]
 cmp bl,[edi]
 je @@2     
 jb @@1     
 dec b [ebp]
 jmp @@2          
@@1:        
 inc b [ebp]
@@2:        
 inc ebp
 inc edi    
 dec edx
 jnz @@A
 push eax
 Call WaitVR
 pop eax
 Call SetPalette
 ret
ENDP