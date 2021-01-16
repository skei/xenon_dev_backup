;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
; Textured Tunnel
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

DOSSEG         
.MODEL MEDIUM
.STACK
.386
; JUMPS
LOCALS

SEGMENT XposSeg
Xpos Label byte
 include Xdata.inc
ENDS            
                
SEGMENT YPosSeg 
YPos label byte
 include YData.inc
ENDS    
 
SEGMENT TextureSeg
Texture label byte
 include tex.db
ENDS

SEGMENT BufferSeg
Buffer label byte
 db 64000 dup(?)
ENDS 
                
;° DATA °°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
.DATA          
    
Palette label byte
 include pal.inc
        
saved_di1 dw ?
saved_di2 dw ?
dx1 dw ?
dx2 dw ?

;° CODE °°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
.CODE   
        
Label Sinus Byte
 include sinus.db
        
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
           
PROC SetPalette
 push ds       
 mov dx,3c8h
 xor al,al 
 out dx,al 
 inc dx    
 mov cx,768
 mov ax,SEG Palette
 mov ds,ax    
 mov si,OFFSET Palette
 rep outsb    
 pop ds        
 ret           
ENDP           
               
PROC DrawTunnel1
; Set-up Segments
 mov di,[saved_di1]
 push ds          
 mov ax,SEG Texture
 mov ds,ax                  
 mov ax,SEG Buffer
 mov es,ax                  
 mov ax,SEG Xpos
 mov fs,ax                  
 mov ax,SEG Ypos            
 mov gs,ax        
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
 mov cx,100                 
; BH/L=X/Y pos, DH/L = Adders
DrawLoop1:
rept 160          
 mov bh,[fs:di]   
 mov bl,[gs:di]                   
 add bh,dh        
 add bl,dl        
 mov al,[ds:bx]   
 mov [es:di],al   
 inc di                     
 inc di                     
endm              
 add di,320    
 dec cx       
 jnz DrawLoop1
 pop ds          
 ret             
ENDP             
                 
PROC DrawTunnel2
; Set-up Segments
 mov di,[saved_di2]
 push ds          
 mov ax,SEG Texture         
 mov ds,ax                  
 mov ax,SEG Buffer
 mov es,ax                  
 mov ax,SEG Xpos            
 mov fs,ax                  
 mov ax,SEG Ypos            
 mov gs,ax        
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
 mov cx,100                 
; BH/L=X/Y pos, DH/L = Adders
DrawLoop2:    
rept 160          
 mov bh,[fs:di]   
 mov bl,[gs:di]
 add bh,dh
 add bl,dl        
 mov al,[ds:bx] 
; shl al,4       
 mov [es:di],al   
 inc di                     
 inc di                     
endm              
 add di,320       
 dec cx           
 jnz DrawLoop2    
 pop ds           
 ret              
ENDP              
                  
PROC Update_DI    
 cmp [saved_di1],0
 jne @@1         
 mov [saved_di1],321
 jmp @@2   
@@1:               
 cmp [saved_di1],321
 jne @@2   
 mov [saved_di1],0  
@@2:       
 cmp [saved_di2],1
 jne @@3
 mov [saved_di2],320
 jmp @@4
@@3:    
 cmp [saved_di2],320
 jne @@4   
 mov [saved_di2],1
@@4:       
 ret           
ENDP

PROC CopyBuffer
 mov ax,0a000h
 mov es,ax
 xor di,di
 mov ax,SEG BUFFER
 push ds
 mov ds,ax
 xor si,si
 mov cx,16000
 rep movsd
 pop ds
 ret
ENDP 
               
;° EntryPoint from DOS °°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
EntryPoint:      
 mov ax,@DATA    
 mov ds,ax       
 mov ax,13h      
 int 10h         
; Call SetPalette
 mov [saved_di1],0
 mov [saved_di2],320
 mov [dx1],0140h
 mov [dx2],0280h   
TestLoop:                
 mov dx,[dx1]   
 Call DrawTunnel1
 mov dx,[dx2]   
; Call DrawTunnel2
 add [dx1],0200h        ; zoom:rotate
 add [dx2],0100h
 Call Update_di
 Call WaitVR
 Call CopyBuffer
 in al,60h         
 cmp al,1
 jne TestLoop
 mov ax,3h
 int 10h
 mov ax,4c00h
 int 21h
END EntryPoint
