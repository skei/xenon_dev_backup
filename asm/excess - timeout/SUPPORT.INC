b equ byte ptr    
w equ word ptr    
d equ dword ptr   

;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
; EDX -> '$'-terminated strin
 PROC PrintText
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
 Call PrintText
 ret           
ENDP           
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
;Puts a Decimal Value
;Calls:  CL = Nr of Digits  
;     EBX = Value to print
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ 
PROC PrintDecimal
 pushad                     
 movzx ecx,cl
 dec ecx
 jecxz OnlyOne
 mov ebp,ecx
 xor ch,ch
 mov eax,ebx
PutLoop:
 xor edx,edx
 mov ebx,10
 div ebx
 push dx
 loop PutLoop
 movzx ebx,ax
 mov ecx,ebp
 xor ch,ch
 add bx,'0';30h
 mov ah,2
 mov dx,bx
 int 21h
GetLoop:     
 pop bx
OnlyOne_1:   
 add bx,30h
 mov ah,2
 mov dx,bx
 int 21h
 loop GetLoop
 popad
 Ret  
OnlyOne:
 mov ecx,1
 jmp OnlyOne_1
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

 in al,60h 
 cmp al,1  
 jne @@3   
; Jmp DemoEnd
 pop eax 
 mov eax,offset DemoEnd
 push eax
@@3:       
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
 ret
ENDP
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
;ÉÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ»
;º Returns EAX with a random number with CL bits in size.                º
;º                                                                       º
;º   The algorithem was from an article in Doctor Dobbs Journal          º
;º   issue date  MAY 1991                                                º
;º                                                                       º
;º  NOTE: the initial random number is taken from the CMOS clock         º
;ÈÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ¼
Random PROC
        push edx
        push ebx
        xor eax,eax
        mov bl,byte ptr Random_Number
        and bl,1

Gen_bit:	; make n bit numbers
        shl eax,1

	mov edx,Random_Number

        shr edx,9
        xor bl,dl

        shr edx,5
        xor bl,dl

    	bt ebx,1
	rcr Random_Number,1
        setc bl
	or  al,bl

	dec cl
        jnz Gen_bit
        pop ebx
        pop edx
	ret
Random  Endp

align 4
Random_Number     DD 0


;ÉÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ»
;º Get inital random number from CMOS time				º
;ÈÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ¼
Randomize PROC

        push    es
        mov     ax,0EE00h               ; GET DOS32 selector values
        int     31h
        mov     es,bx

        mov     al,0                    ; Get seconds
        out     70h,al
        in      al,71h
        shl     eax,8
        mov     al,2                    ; Get minute
        out     70h,al
        in      al,71h
        shl     eax,8
        xor     eax,es:[046Ch]             ; throw in number of ticks
        pop     es
        not     eax
        mov     Random_Number,eax
        ret

Randomize ENDP

