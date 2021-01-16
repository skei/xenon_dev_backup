; Include file for DOS32

b EQU byte ptr             
w EQU word ptr             
d EQU dword ptr            

; ESI -> Palette
PROC SetPalette         
 cld
 mov dx,3c8h         
 xor al,al              
 out dx,al       
 inc dx          
 mov cx,768      
 rep outsb     
 ret                
ENDP                   

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

