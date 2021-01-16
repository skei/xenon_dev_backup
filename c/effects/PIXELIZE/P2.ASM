; Project... FixedPoint... samme adder & value i inner og outer.. push/pop
; deretter.. RotoZoom buffer... 160x100

.486p
.MODEL FLAT
.STACK     
;JUMPS     
LOCALS          
.CODE      
           
;° DATA °°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
Screen dd ?
              
BitMap label byte
 include bitmap.db
Palette label byte
 include palette.db
SizeAdder db ?
    
;° CODE °°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
           
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

; ESI -> Palette
PROC SetPalette                    
 cld                               
 mov dx,3c8h                       
 xor al,al
 out dx,al       
 inc dx            
 mov ecx,768    
@@1:        
 mov al,[esi]
 inc esi           
 out dx,al             
 dec ecx      
 jnz @@1 
 ret                
ENDP                   
              
           
; AL = Pixelize size
; ESI -> Bitmap
; EDI -> Buffer/screen
_esi dd ?
         
PROC Pixelize   
; mov ebp,esi    
; sub ebp,edi    
; mov dword ptr [ScrBufDiff],ebp
 mov [_esi],esi
 mov ebp,200
 mov al,[esi] 
 xor cx,cx   
Outer:          
 xor bx,bx   
 i=0               
 rept 320
 local @@1    
  mov [edi+i],al
  add bl,dl         
  jno @@1           
  mov al,[esi+i]
@@1:                
  i=i+1             
 endm               
 ;                  
 xor bx,bx     
 add edi,320
 add [_esi],320
 add cl,dl     
 jno @@2       
 mov esi,[_esi]
; mov esi,edi                    ; humm.... Denne peker jo p† skjermen nuh!
; add esi,12345678h
;ScrBufDiff = $-4
@@2:               
 dec ebp       
 jnz Outer
 ret
ENDP     
              
          
;° EntryPoint from DOS °°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
EntryPoint:      
 mov eax,0EE02h
 int 31h
 mov edi,0A0000h
 sub edi,ebx
 mov [Screen],edi
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
 mov ax,13h
 int 10h
 mov esi,offset palette
 call SetPalette
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
 Mov [SizeAdder],-1
 mov bl,254
@@1:                 
 add bl,[SizeAdder]
 cmp bl,254      
 jna @@2         
 neg [SizeAdder] 
@@2:             
 cmp bl,1
 jnb @@3   
 neg [SizeAdder]
@@3:       
 Call WaitVR
 mov dl,bl;;al,bl
 mov esi,offset BitMap
 mov edi,[Screen]
 push ebx   
 Call Pixelize
 pop ebx    
 in al,60h     
 cmp al,1      
 jne @@1       
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
 mov ax,3 
 int 10h  
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
 mov ax,4c00h
 int 21h  
End EntryPoint
