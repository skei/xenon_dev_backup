.386
LOCALS

GLOBAL Writeln:Proc
GLOBAL WaitVR:Proc
GLOBAL CopyScreen:Proc
GLOBAL ClearScreen:Proc
GLOBAL SetPalette:Proc
GLOBAL BlankPalette:Proc
GLOBAL Random:Proc

GLOBAL RandSeed:Word
                  
;� DATA ��������������������������������������������������������������
SEGMENT DATA PARA PUBLIC USE16
                  
RandSeed dw 1234h

ENDS DATA                         
;� CODE ��������������������������������������������������������������
SEGMENT CODE PARA PUBLIC Use16
Assume cs:code,ds:data
   
;���������������������������������������������������������������������
; DS:DX - '$'-terminated string                                   
;���������������������������������������������������������������������
PROC Writeln
 mov     ah,9         
 int     21h           
 ret                                                       
ENDP

;���������������������������������������������������������������������
; Venter p� Retrace    
;���������������������������������������������������������������������
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

;���������������������������������������������������������������������
; DS:SI -> Buffer
;���������������������������������������������������������������������
PROC CopyToScreen
 mov ax,0a000h    
 mov es,ax             
 xor di,di             
 mov cx,16000          
 rep movsd             
 ret                   
ENDP                   
                       
;���������������������������������������������������������������������
; Blanker VRAM         
;���������������������������������������������������������������������
PROC ClearScreen
 mov ax,0a000h     
 mov es,ax         
 xor di,di         
 mov cx,16000                                              
 mov eax,0         
 rep stosd         
 ret               
ENDP               

;���������������������������������������������������������������������
; DS:SI = Palette  
;���������������������������������������������������������������������
PROC SetPalette
 mov dx,3c8h         
 xor al,al              
 out dx,al       
 inc dx          
 mov cx,768      
 rep outsb      
 ret                
ENDP                   

;���������������������������������������������������������������������
;���������������������������������������������������������������������
PROC BlankPalette
 mov dx,3c8h         
 xor al,al              
 out dx,al       
 inc dx          
 mov cx,768      
@@1:
 out dx,al
 dec cx
 jnz @@1
 ret                
ENDP

;���������������������������������������������������������������������
; Random - Returns a random number of 16 bits
; Out: AX - random value.
; Modified: AX, DX, Flags.
;-----------------------------------------------------------------------------
PROC Random            
 mov ax,[RandSeed]
 mov dx,8405h
 mul dx
 inc ax
 mov [RandSeed],ax
 ret
ENDP
   
   
ENDS CODE
end