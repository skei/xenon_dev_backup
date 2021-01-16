; Baby-tro

;DOSSEG
.MODEL MEDIUM
.386
.STACK
;JUMPS
LOCALS

Segment BabySeg
 include baby.db
ends    

FontColor = 32
                        
;° DATA °°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
.DATA                      
                           
Label Font byte            
 include font.inc       
                        
Label Palette byte      
 include babypal.db     
                                      
; Space, A-Z            
                                       
; 1234567890                           
; [\]^_`abcd                           
                                            
; .,'!/+-()þ:                            
; efghijklmno                          
Text label byte                        
;    1234567890123456789012345678901234567890
 db '             WELL WELLf WELLeee JUST HAD'
 db '             TO CODE A ^K INTROf BUT    '
 db '             DIDNgT HAVE MORE THAN A FEW'
 db '             HOURSeeee SO THIS IS ITeee '
 db '             NOT TOO EXCITING EFFECTS IN'
 db '             HERE om                    '
 db '                                        '
 db '             BTWo NOW IS A PERFECT TIME '
 db '             FOR ALL OF YOU TO STAND UPf'
 db '             AND IN UNION YELL OUT o    '
 db '                                        '
 db '             gCONGRATILATIONSf AXONhg   '
 db '                                        ' 
 db '             BECAUSE THURSDAY [\e OCTe  '
 db '             SONDREf MY SONf WAS BORNh  '
 db '                                        ' 
 db 'GUESS WHOgS THE PROUDEST GUY IN HEREh om'
 db '                                        '
 DB 'TOO BAD THIS PARTY DOESNgT HAVE A bK    '
 DB 'COMPO INSTEADf SO THAT I COULD HAVE     '
 db 'INCLUDED SOME LOVELY gCHILDISHg ADLIB   '
 db 'MUSIC IN HERE om                        '
 db '                                        '
 db 'AXONoCODEf TMKiINFoGFXf MUSICo HAHh     '
 db '                                     ESC'
                                              
;° CODE °°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
.CODE                                         

PROC WaitVR
 push ax
 push dx
 mov dx,3DAh 
WaitStartVR: 
 in al,dx    
 test al,8   
 jnz WaitStartVR
WaitEndVR:   
 in al,dx                            
 test al,8                           
 jz WaitEndVR                        
 pop dx                              
 pop ax                              
 ret                    
ENDP                    
                        
                        
; BH = Char             
; ES:DI -> ScreenPos    
PROC DrawChar           
 cmp bl,' '             
 je @@1                 
 sub bl,65
 add bl,1
 jmp @@2      
@@1:   
 mov bl,0
@@2:                 
; Now, draw...                       
 xor bh,bh
 shl bx,3
 mov dx,8               
@@3:                    
 mov al,[ds:Font+bx]    
 inc bx                 
 clc                    
 rcl al,1
 jnc N1
 mov byte ptr [es:di],FontColor
N1:           
 rcl al,1
 jnc N2
 mov byte ptr [es:di+1],FontColor
N2:          
 rcl al,1
 jnc N3
 mov byte ptr [es:di+2],FontColor
N3:          
 rcl al,1
 jnc N4
 mov byte ptr [es:di+3],FontColor
N4:          
 rcl al,1
 jnc N5
 mov byte ptr [es:di+4],FontColor
N5:                 
 rcl al,1
 jnc N6
 mov byte ptr [es:di+5],FontColor
N6:           
 rcl al,1
 jnc N7
 mov byte ptr [es:di+6],FontColor
N7:           
 rcl al,1
 jnc N8
 mov byte ptr [es:di+7],FontColor
N8:           
 add di,320 
 dec dx
 jnz @@3    
 ret         
ENDP       
               
Proc WriteText
 mov si,offset Text
 mov ax,0a000h
 mov es,ax    
 mov di,0          
 mov bp,25    
@@1:                        
 mov cx,40         
@@2:    
 Call WaitVR
 mov bl,[ds:si]
 inc si  
 push si 
 push di
 push cx
 Call DrawChar
 pop cx
 pop di  
 pop si  
 add di,8
 in al,60h
 cmp al,1
 je @@3  
 dec cx  
 jnz @@2
        
 add di,2240    ; 320*8
 dec bp                     
 jnz @@1 
@@3:     
 ret 
ENDP          
                            
;° EntryPoint from DOS °°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
EntryPoint:                 
 mov ax,@data               
 mov ds,ax                  
 cld           
      
 mov ax,13h    
 int 10h      
; Set Palette 
 mov dx,3c8h
 mov al,0
 out dx,al
 inc dx
 mov si,offset palette
 mov cx,768
 rep outsb         
; Copy picture to screen
 push ds  
 mov ax,babyseg
 mov ds,ax    
 xor si,si    
 mov ax,0a000h
 mov es,ax    
 xor di,di    
 mov cx,16000 
 rep movsd
 pop ds 
 Call WriteText
        
WaitForKey:
 in al,60h  
 cmp al,1
 jne WaitForKey
    
 mov ax,3     
 int 10h             
 mov ax,4c00h 
 int 21h      
End EntryPoint
