; 3D Matrix rotations - ProtMode.... Axon/XD
                                            
;DOSSEG                                     
.486p ;.386                                 
.MODEL FLAT ;.MODEL MEDIUM                  
.STACK                                      
;JUMPS                                      
LOCALS                                      
.CODE                                       
                                            
b equ byte ptr
w equ word ptr
d equ dword ptr
s equ short

;� DATA 같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
Screen dd ?                                 
                                        ; 320+320+320
Buffer Label Byte                       ; 같같같200+
 db 960*600 dup(?)                      ; 같굇같200+
BufferStart dd (Buffer+((960*200)+320)) ; 같같같200+
                                                    
include tables.inc                                  
                                                                 
vcosx dd ?      ; temp storage for object matrix calculation
vsinx dd ?      ; can be used if needed during draw
vcosy dd ?
vsiny dd ?
vcosz dd ?
vsinz dd ?
                   
xangle dw ?
yangle dw ?
zangle dw ?
matrix dd 9 dup(?)      ;3x3 rotation matrix
                        
NumCoords = 1327
include e:\axon\asc\nys_face.inc

;� CODE 같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같

;같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
; SINE - 16 bit theta to 32bit sin(@)
; In:   AX - theta  0 - 65536 (0-360)
; Out:  EAX - sin (@)   (-4000h to 4000h)
;같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
; COSINE - 16 bit theta to 32bit cos(@)
; In:   AX - theta  0 - 65536 (0-360)
; Out:  EAX - cos (@)   (-4000h to 4000h)
;같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
; Notes:
; calculate sin into eax, from ax, smashes bx
; after imul by sin, shr eax,14 to compensate for decimal factor!
;  eg:  mov eax,sin(@)                                           
;       mov ebx,32bitnumber
;       imul ebx
;       shrd eax,edx,14
;       eax = ebx*sin(@)
;
;       mov ax,sin(@)
;       mov bx,16bitnumber
;       imul bx
;       shrd ax,dx,14
;       eax = bx*sin(@)
; eax is only a sign extended ax and will contain either ffffxxxx or 0000xxxx
;같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
PROC cosine
 add ax,4000h
Sine:
 shr ax,2
 cmp ax,2000h
 jge s q3o4                         ; quadrant 3 or 4
 cmp ax,1000h
 jl s q0                            ; quad 1                     
 mov ebx,1fffh
 sub bx,ax
 jmp s half_sine                    ; quad 2
q0:
 movzx ebx,ax
 jmp s half_sine
q3o4:
 cmp ax,3000h
 jl s q3                                         
 mov ebx,3fffh
 sub bx,ax
 call half_sine                     ; quad 4
 neg eax
 ret
q3:
 and ax,0fffh
 movzx ebx,ax                       ; quad 3
 call half_sine
 neg eax
 ret                                                             
half_sine:
 xor eax,eax
 mov ax,w sinus[ebx*2]
 ret
ENDP        
            
;같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
; CALC_MATRIX - generate temp matrix, 12 imul's, from object esi
; In:   xangle, yangle, zangle - angles - 0-65535
;       xangle[esi*2] ???        
; Out:  matrix - resulting rotation matrix 
;       ESI = ESI                          
; Notes:                                   
;              x                         y                      z
;x=  cz * cy - sx * sy * sz   - sz * cy - sx * sy * cz     - cx * sy
;y=         sz * cx                   cx * cz                - sx
;z=  cz * sy + sx * sz * cy   - sy * sz + sx * cy * cz       cx * cy
;같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
PROC calc_matrix                           
 mov ax,[xangle]                           
 neg ax                                    
 push eax                                  
 call cosine    
 mov [vcosx],eax
 pop eax        
 call sine      
 mov [vsinx],eax
 mov ebp,eax                        ; bp = sx
 neg eax                 
 mov [matrix+20],eax     
                         
 mov ax,[zangle]         
 neg ax                  
 push eax                
 call cosine             
 mov [vcosz],eax           
 mov edi,eax                        ; di = cz
 pop eax    
 call sine  
 mov [vsinz],eax
 mov edx,eax                        ; dx = sz
                                           
 mov ax,[yangle]
 neg ax         
 push eax       
 call cosine    
 mov [vcosy],eax  
 pop eax    
 call sine
 mov [vsiny],eax                      ; ax = sy
            
 mov ebx,edx                        ; save sz
            
 mov ecx,eax                        ; save sy
            
 imul ebx                           ; bx = - sy * sz
 shr eax,14 
 movsx ebx,ax
 neg ebx    
 mov [matrix+28],ebx
            
 mov eax,ecx                        ; si = cz * sy
 imul edi
 shr eax,14
 movsx esi,ax
 mov [matrix+24],esi
      
 mov eax,[vcosy]
                
 imul edi                           ; di = cy * cz
 shr eax,14     
 movsx edi,ax   
 mov [matrix+0],edi
      
 mov eax,[vsinz]
 mov ecx,[vcosy]
                
 imul ecx                           ; cx = - sz * cy
 shr eax,14     
 movsx ecx,ax   
 neg ecx        
 mov [matrix+4],ecx                        
      
 mov eax,ebp
 imul esi
 shr eax,14
 movsx esi,ax
 neg esi
 add [matrix+4],esi
      
 mov eax,ebp
 imul edi
 shr eax,14
 movsx edi,ax
 add [matrix+28],edi
      
 mov eax,ebp
 imul ebx
 shr eax,14
 movsx ebx,ax
 add [matrix+0],ebx
                                           
 mov eax,ebp
 imul ecx
 shr eax,14
 movsx ecx,ax
 neg ecx
 add [matrix+24],ecx
        
 mov esi,[vcosx]
                
 mov eax,[vcosy]
 imul esi                           ; cx * cy
 shr eax,14     
 movsx eax,ax   
 mov [matrix+32],eax
      
 mov eax,[vsiny]
 imul esi                           ;-cx * sy
 shr eax,14     
 movsx eax,ax   
 neg eax                                   
 mov [matrix+8],eax
                
 mov eax,[vsinz]
 imul esi                           ; cx * sz
 shr eax,14     
 movsx eax,ax   
 mov [matrix+12],eax
                
 mov eax,[vcosz]
 imul esi                           ; cx * cz
 shr eax,14     
 movsx eax,ax   
 mov [matrix+16],eax
      
 ret  
ENDP  
      
;같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
; ROTATE_POINT - 32 bit _rotate_point point using _tmatrix
; In:   EBX - x point
;       ECX - y point
;       EBP - z point
;       matrix - 32 bit rotation matrix - set up by "calc_matrix" routine
; Out:  EBX - x point
;       ECX - y point
;       EBP - z point
;같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
PROC Rotate_point
 mov eax,[matrix+8]                 ; solve x = bx(0)+cx(1)+bp(2)
 imul ebp          
 shrd eax,edx,14   
 mov edi,eax       
 mov eax,[matrix+4]
 imul ecx          
 shrd eax,edx,14   
 add edi,eax
 mov eax,[matrix+0]
 imul ebx          
 shrd eax,edx,14   
 add edi,eax                        ; di = new x
                   
 mov eax,[matrix+20]                ; solve y = bx(3)+cx(4)+bp(5)
 imul ebp           
 shrd eax,edx,14    
 mov esi,eax        
 mov eax,[matrix+16]
 imul ecx           
 shrd eax,edx,14    
 add esi,eax        
 mov eax,[matrix+12]
 imul ebx           
 shrd eax,edx,14    
 add esi,eax                        ; si = new y
             
                    
 mov eax,[matrix+32]                ; solve z = bx(6)+cx(7)+bp(8)
 imul ebp           
 shrd eax,edx,14    
 mov ebp,eax        
 mov eax,[matrix+28]
 imul ecx           
 shrd eax,edx,14    
 add ebp,eax        
 mov eax,[matrix+24]
 imul ebx           
 shrd eax,edx,14    
 add ebp,eax                        ; bp = new z
                    
 mov ecx,esi        
 mov ebx,edi        
 ret                
ENDP                
                                            
PROC WaitVR      
 mov dx,3DAh     
WaitStartVR:     
 in al,dx
 test al,8
 jnz WaitStartVR        ; Vent hvis retrace allerede igang
WaitEndVR:                                                
 in al,dx                                                 
 test al,8                                                
 jz WaitEndVR           ; Vent til retrace starter
 ret 
ENDP
                 
PROC ClearBuffer 
 mov edi,offset Buffer
 add edi,(320*600)+320
 mov ebx,200
@@1:
 mov ecx,80
 xor eax,eax
 rep stosd
 add edi,640
 dec ebx
 jnz @@1
 ret             
ENDP             
                 
PROC CopyBuffer
 mov esi,offset Buffer
 add esi,(320*600)+320
 mov edi,[Screen]
 mov ebx,200     
@@1:             
 mov ecx,80      
 rep movsd
 add esi,640
 dec ebx
 jnz @@1
 ret             
ENDP             


PROC Testing     
 Call calc_matrix
 xor esi,esi     
@@1:             
 movsx ebx,[coords1+esi*8]
 movsx ecx,[coords1+esi*8+2]
 movsx ebp,[coords1+esi*8+4] 
 push esi                  
 Call Rotate_Point                                
                
 mov edi,offset Buffer
 sal ebx,1
 add ebx,480
 sal ecx,1
 add ecx,300
 imul ecx,960
 add edi,ecx                                      
 add edi,ebx                                      
 mov b [edi],15                                   
         
 pop esi     
 inc esi     
 cmp esi,NumCoords
 jb @@1 
 ret    
ENDP             
                
;� EntryPoint from DOS 같같같같같같같같같같같같같같같같같같같같같같같�
EntryPoint:         
; mov ax,@data      
; mov ds,ax         
 mov eax,0EE02h     
 int 31h            
 mov edi,0A0000h    
 sub edi,ebx
 mov [Screen],edi
 cld
    
 mov ax,13h
 int 10h
MainLoop:
 add [xangle],128
 add [yangle],256
 add [zangle],256
 Call ClearBuffer
 Call Testing
 Call WaitVR    
 Call CopyBuffer
 in al,60h   
 cmp al,1    
 jne MainLoop
                 
 mov ax,3h
 int 10h

 mov ax,4c00h    
 int 21h
End EntryPoint
