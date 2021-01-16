;DOSSEG
.486
.MODEL FLAT
.STACK
;JUMPS
LOCALS

;° CODE °°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
.CODE
     
Include Sintab.inc

;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
; Rotation function, spherical coordinates.  Uses 4 multiplies.
; AX = Theta, BX = Phi, ECX = p, ESI -> 4x4 matrix
; ESI updated as follows--
; -siné  -cosécosí  -cosésiní  0 
;  cosé  -sinécosí  -sinésiní  0 
;  0      siní      -cosí      0 
;  0      0          p         1 
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
PROC CreateRotateMatrix
 pushad
 mov [esi+14*4],ecx                ;set rho   [2][3]
    
 call _Sin                         ;After sin/cos calls,
 mov edi,edx                       ;EDI = Sin Theta
 call _Cos                         ;EBP = Cos Theta
 mov ebp,edx                       ;EBX = Sin Phi
 mov ax,bx                         ;ECX = Cos Phi
 call _Sin
 mov ebx,edx
 call _Cos
 mov ecx,edx
    
 neg edi
 mov [esi],edi                     ;-sinT   [0][0]
 
 mov eax,edi
 imul ecx                          ;-sinTcosP [1][1]
 shrd eax,edx,16                   ;fixed point mul - get it back 
 mov [esi+5*4],eax                 ;into 16.16 format.
 
 mov eax,edi                       ;-sinTsinP [2][1]
 imul ebx
 shrd eax,edx,16
 mov [esi+6*4],eax
    
 mov [esi+4*4],ebp                 ;cosT [0][1]
    
 neg ebp                           ;-cosTcosP [1][0]
 mov eax,ebp
 imul ecx
 shrd eax,edx,16
 mov [esi+1*4],eax
    
 mov eax,ebp                       ;-cosTsinP [2][0]
 imul ebx
 shrd eax,edx,16
 mov [esi+2*4],eax
    
 mov [esi+9*4],ebx                 ;sinP [1][2]
    
 neg ecx                           ;-cosP [2][2]
 mov [esi+10*4],ecx
    
 popad
 ret
ENDP 
              
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
;Transform list of points.
;ESI->X,Y,Z triplets
;EDI->X,Y list to be filled (words)
;CX = number of points to process
;EBX->rotation matrix
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
ALIGN 2       
XTemp dd ?    
YTemp dd ?    
ZTemp dd ?    
NewX  dd ?    
NewY  dd ?    
PROC XformPts 
_XformPts:
 push cx      
              
 lodsd                             ;matrix multiply stuff.
 mov [XTemp],eax                   ;this is intentionally left 
 imul [dword ebx]                  ;without the obvious loop.
 shrd eax,edx,16
 mov ecx,eax                       ;calc NewX in cx
 lodsd
 mov [YTemp],eax
 imul [dword ebx+4*4]
 shrd eax,edx,16
 add ecx,eax
 lodsd
 mov [ZTemp],eax
 imul [dword ebx+8*4]
 shrd eax,edx,16
 add ecx,eax
 add ecx,[ebx+12*4]                ;xtranslate
 mov [NewX],ecx
    
 mov eax,[XTemp]                   ;calc NewY
 imul [dword ebx+1*4]                          
 shrd eax,edx,16
 mov ecx,eax                       
 mov eax,[YTemp]
 imul [dword ebx+5*4]
 shrd eax,edx,16
 add ecx,eax
 mov eax,[ZTemp]
 imul [dword ebx+9*4]
 shrd eax,edx,16
 add ecx,eax
 add ecx,[ebx+13*4]
 mov [NewY],ecx
    
 mov eax,[XTemp]                   ;calc NewZ
 imul [dword ebx+2*4]                          
 shrd eax,edx,16
 mov ecx,eax                       
 mov eax,[YTemp]
 imul [dword ebx+6*4]
 shrd eax,edx,16
 add ecx,eax
 mov eax,[ZTemp]
 imul [dword ebx+10*4]
 shrd eax,edx,16
 add ecx,eax
 add ecx,[ebx+14*4]
    
 mov eax,[NewX]                    ;Projection X*D/Z
 shl eax,DISTANCE
 xor edx,edx                       ;clear high dword.
 cdq
 idiv ecx
 adc ax,1
 add ax,160                        ;adjust to screen center
 or ax,ax
 jns noxclip0
 xor ax,ax
noxclip0:
 cmp ax,320
 jl noxcliphi
 mov ax,319
noxcliphi:
 stosw                             ;store it.
              
 mov eax,[NewY]                    ;Projection Y*D/Z
 shl eax,DISTANCE
 xor edx,edx
 cdq
 idiv ecx
 adc ax,1
 neg ax
 add ax,100                        ;adjust to screen center.
 or ax,ax
 jns noyclip0
 xor ax,ax
noyclip0:
 cmp ax,200
 jl noycliphi
 mov ax,199
noycliphi:
 stosw                             ;store it.
    
 pop cx
 dec cx       
 jnz _XformPts
 ret
ENDP 
 
;Sin/Cos
; determines sin or cos of angle in ax (0..511)
; returns EDX = 32 bit angle.
; I was lazy enough to generate a 2k sin table to keep from determining
; signs, so this function is very simple.
; ax is preserved on Sines but killed on Cos's cuz I'm still lazy.  It
; doesn't matter tho since Sin is calculated before Cos.
_Cos:    
    add ax,128                       ;sin (T+90)
_Sin:
    push ax esi
    and eax,511                      ;take care of big numbers.
    shl ax,2
    mov esi,offset sintab
    add esi,eax
    mov edx,[esi]
    pop esi ax
    ret             


;° EntryPoint from DOS °°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
EntryPoint:
 mov ax,4c00h
 int 21h
End EntryPoint
