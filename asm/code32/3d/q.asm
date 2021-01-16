;같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
; USER3DX - scale 3d point into 2d point on y axis only
; In:   EDI - x point
;       ESI - z point
; Out:  EDI - x point
;       ESI - z point
;같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
;Proc user3dx
; bp must always be non-zero
; cmul eax,edi,ratiox            ; MACRO
; idiv esi
; mov edi,eax
; ret
;ENDP

;같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
; CHECKFRONT: checks if a side is visible. (counter-clockwise)
; In:   (EDI,EBP) - xy of point 1
;       (ESI,ECX) - xy of point 2
;       (EDX,EBX) - xy of point 3
; Out:  ECX < 0 if side counter-clockwise
; Notes: routine courtesy of "RAZOR"
; eg:   call checkfront
;       cmp ecx,0
;       jng dontdraw
;같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
PROC checkfront
 cmp edi,esi
 jng s cfc
 mov eax,edi
 mov edi,esi
 mov esi,edx
 mov edx,eax
 mov eax,ebp
 mov ebp,ecx
 mov ecx,ebx
 mov ebx,eax
cfc:
 mov eax,edx                        ; ax = x3
 sub eax,edi                        ; ax = x3 - _x1
 sub ecx,ebp                        ; cx = _y2 - _y1
 imul ecx                           ; ax = (x3-_x1)*(_y2-_y1)
 mov ecx,eax                        ; save it...
 mov eax,esi                        ; ax = _x2
 sub eax,edi                        ; ax = _x2 - _x1
 sub ebx,ebp                        ; bx = y3 - _y1
 imul ebx                           ; ax = (_x2-_x1)*(y3-_y1)
 sub ecx,eax                        ; cx = (x3-_x1)*(_y2-_y1)-(_x2-_x1)*(y3-_y1)
 ret
ENDP

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

;같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
; MATRIX - generate temp matrix, 12 imul's, from object esi
; In:   ESI - Object # to get angles from
;       v_vxs[esi*2] - object x angle (0-65536)
;       v_vys[esi*2] - object y angle (0-65536)
;       v_vzs[esi*2] - object z angle (0-65536)
; Out:  tmatrix - resulting rotation matrix (excluding camera matrix)
;       ESI = ESI
; Notes:
;              x                         y                      z
;x=  cz * cy - sx * sy * sz   - sz * cy - sx * sy * cz     - cx * sy
;y=         sz * cx                   cx * cz                - sx
;z=  cz * sy + sx * sz * cy   - sy * sz + sx * cy * cz       cx * cy
;같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
PROC matrix
 push esi

 mov ax,v_vxs[esi*2]
 neg ax
 push eax
 call _cosine
 mov vcosx,eax
 pop eax
 call _sine
 mov vsinx,eax
 mov ebp,eax                        ; bp = sx
 neg eax
 mov [tmatrix+20],eax

 mov ax,v_vzs[esi*2]
 neg ax
 push eax
 call cosine
 mov vcosz,eax
 mov edi,eax                        ; di = cz
 pop eax
 call sine
 mov vsinz,eax
 mov edx,eax                        ; dx = sz

 mov ax,v_vys[esi*2]
 neg ax
 push eax
 call cosine
 mov vcosy,eax
 pop eax
 call sine
 mov vsiny,eax                      ; ax = sy

 mov ebx,edx                        ; save sz

 mov ecx,eax                        ; save sy

 imul ebx                           ; bx = - sy * sz
 shr eax,14
 movsx ebx,ax
 neg ebx
 mov [tmatrix+28],ebx

 mov eax,ecx                        ; si = cz * sy
 imul edi
 shr eax,14
 movsx esi,ax
 mov [tmatrix+24],esi

 mov eax,vcosy

 imul edi                           ; di = cy * cz
 shr eax,14
 movsx edi,ax
 mov [tmatrix+0],edi

 mov eax,vsinz
 mov ecx,vcosy

 imul ecx                           ; cx = - sz * cy
 shr eax,14
 movsx ecx,ax
 neg ecx
 mov [tmatrix+4],ecx

 mov eax,ebp
 imul esi
 shr eax,14
 movsx esi,ax
 neg esi
 add [tmatrix+4],esi

 mov eax,ebp
 imul edi
 shr eax,14
 movsx edi,ax
 add [tmatrix+28],edi

 mov eax,ebp
 imul ebx
 shr eax,14
 movsx ebx,ax
 add [tmatrix+0],ebx

 mov eax,ebp
 imul ecx
 shr eax,14
 movsx ecx,ax
 neg ecx
 add [tmatrix+24],ecx

 mov esi,vcosx

 mov eax,vcosy
 imul esi                           ; cx * cy
 shr eax,14
 movsx eax,ax
 mov [tmatrix+32],eax

 mov eax,vsiny
 imul esi                           ;-cx * sy
 shr eax,14
 movsx eax,ax
 neg eax
 mov [tmatrix+8],eax

 mov eax,vsinz
 imul esi                           ; cx * sz
 shr eax,14
 movsx eax,ax
 mov [tmatrix+12],eax

 mov eax,vcosz
 imul esi                           ; cx * cz
 shr eax,14
 movsx eax,ax
 mov [tmatrix+16],eax

 pop esi
 ret
ENDP

;같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
;
; _temp_rotate_point - 32 bit _rotate_point point using _tmatrix
; In:
;    EBX - x point
;    ECX - y point
;    EBP - z point
;     _tmatrix - 32 bit rotation matrix - set up by "_temp_matrix" routine
; Out:
;    EBX - x point
;    ECX - y point
;    EBP - z point
;
; Notes:
;  Same as _rotate_point and _rotate_by_camera
;같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같

_temp_rotate_point:
         mov eax,_tmatrix+8                 ; solve x = bx(0)+cx(1)+bp(2)
         imul ebp
         shrd eax,edx,14
         mov edi,eax
         mov eax,_tmatrix+4
         imul ecx
         shrd eax,edx,14
         add edi,eax
         mov eax,_tmatrix+0
         imul ebx
         shrd eax,edx,14
         add edi,eax                        ; di = new x

         mov eax,_tmatrix+20                ; solve y = bx(3)+cx(4)+bp(5)
         imul ebp
         shrd eax,edx,14
         mov esi,eax
         mov eax,_tmatrix+16
         imul ecx
         shrd eax,edx,14
         add esi,eax
         mov eax,_tmatrix+12
         imul ebx
         shrd eax,edx,14
         add esi,eax                        ; si = new y

         mov eax,_tmatrix+32                ; solve z = bx(6)+cx(7)+bp(8)
         imul ebp
         shrd eax,edx,14
         mov ebp,eax
         mov eax,_tmatrix+28
         imul ecx
         shrd eax,edx,14
         add ebp,eax
         mov eax,_tmatrix+24
         imul ebx
         shrd eax,edx,14
         add ebp,eax                        ; bp = new z

         mov ecx,esi
         mov ebx,edi

         ret

;같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
;
; _sqrt: Routine courtesy TRAN
;
; In:
;   EAX - number to take root of
; Out:
;   EAX - root
;
;같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
_sqrtbasetbl db 0,1,4,9,16,25,36,49,64,81,100,121,144,169,196,225

         align 4
_sqrt:
         pushad
         mov ebp,eax
         bsr ebx,eax
         jnz short _sqrtf0
         xor ebx,ebx
_sqrtf0:
         shr ebx,3
         lea eax,[ebx*8]
         mov cl,32
         sub cl,al
         rol ebp,cl
         mov eax,ebp
         movzx eax,al
         mov edi,offset _sqrtbasetbl
         mov ecx,10h
_sqrtl0:
         scasb
         je short _sqrtl0d
         jb short _sqrtl0d2
         loop _sqrtl0
         inc edi
_sqrtl0d2:
         dec edi
         inc cl
_sqrtl0d:
         movzx edx,byte ptr [edi-1]
         dec cl
         xor cl,0fh
         mov edi,ecx
         mov ecx,ebx
         jecxz short _sqrtdone
         sub eax,edx
_sqrtml:
         shld eax,ebp,8
         rol ebp,8
         mov ebx,edi
         shl ebx,5
         xor edx,edx
         mov esi,eax
         div ebx
         rol edi,4
         add edi,eax
         add ebx,eax
_sqrtf2:
         imul eax,ebx
         mov edx,eax
         mov eax,esi
         sub eax,edx
         jc short _sqrtf1
         loop _sqrtml
_sqrtdone:
         mov [esp+28],edi
         popad
         ret
_sqrtf1:
         dec ebx
         dec edi
         movzx eax,bl
         and al,1fh
         jmp _sqrtf2

;같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
;
; _calc_normal: calculate surface normal
;
; In:
;    _lx1 - x of point 1 on triangle
;    _ly1 - y of point 1 on triangle
;    _lz1 - z of point 1 on triangle
;    _lx2 - x of point 2 on triangle
;    _ly2 - y of point 2 on triangle
;    _lz2 - z of point 2 on triangle
;    _lx3 - x of point 3 on triangle
;    _ly3 - y of point 3 on triangle
;    _lz3 - z of point 3 on triangle
;
; Out:
;    EBX = finx = x of surface normal of triangle
;    ECX = finy = y of surface normal of triangle
;    EBP = finz = z of surface normal of triangle
;
; Notes:
; _x2 = _x2 - _x1
; _y2 = _y2 - _y1
; z2 = z2 - z1
;
; x3 = x3 - _x1
; y3 = y3 - _y1
; z3 = z3 - z1
;
; x = _y2 * z3 - z2 * y3
; y = z2 * x3 - _x2 * z3
; z = _x2 * y3 - _y2 * x3
;
; a = SQR(x ^ 2 + y ^ 2 + z ^ 2)
;
; x = INT(x / a * 256 + .5)
; y = INT(y / a * 256 + .5)
; z = INT(z / a * 256 + .5)
;
; This worked for me on the first try!
;
; If you wanted to get the equation of a plane, you could do this after:
;  d = - x * _x1 - y * _y1 - z * z1
;
;같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같

         nshl = 8

_calc_normal:
         mov ebx,_lx1
         mov ecx,_ly1
         mov ebp,_lz1

         sub _lx2,ebx
         sub _ly2,ecx
         sub _lz2,ebp

         sub _lx3,ebx
         sub _ly3,ecx
         sub _lz3,ebp

         mov eax,_ly2
         mov ebx,_lz3
         imul ebx
         mov ecx,eax

         mov eax,_lz2
         mov ebx,_ly3
         imul ebx
         sub ecx,eax

         mov finx,ecx                       ; save x of normal

         mov eax,_lz2
         mov ebx,_lx3
         imul ebx
         mov ecx,eax

         mov eax,_lx2
         mov ebx,_lz3
         imul ebx
         sub ecx,eax

         mov finy,ecx                       ; save y of normal

         mov eax,_lx2
         mov ebx,_ly3
         imul ebx
         mov ecx,eax

         mov eax,_ly2
         mov ebx,_lx3
         imul ebx
         sub ecx,eax

         mov finz,ecx                       ; save z of normal

calc_testloop:
         cmp finx,32768                     ; make sure (normal^2)*2 is < 2^32
         jge calc_shrit
         cmp finy,32768
         jge calc_shrit
         cmp finz,32768
         jge calc_shrit

         cmp finx,-32768
         jle calc_shrit
         cmp finy,-32768
         jle calc_shrit
         cmp finz,-32768
         jg ok_2_bite_dust

calc_shrit:
         shr finx,1                         ; calculations will be too large if squared, div by 2
         test finx,40000000h
         jz no_neg_calc1
         or finx,80000000h
no_neg_calc1:
         shr finy,1
         test finy,40000000h
         jz no_neg_calc2
         or finy,80000000h
no_neg_calc2:
         shr finz,1
         test finz,40000000h
         jz no_neg_calc3
         or finz,80000000h
no_neg_calc3:
         jmp calc_testloop

ok_2_bite_dust:
         mov eax,finx                       ; x^2
         mov edi,eax                        ; objects
         imul edi
         mov edi,eax

         mov eax,finy                       ; y^2
         mov esi,eax
         imul esi
         mov esi,eax

         mov eax,finz                       ; z^2
         mov ebp,eax
         imul ebp

         add eax,esi
         add eax,edi

         call _sqrt                         ; get square root of number

         mov ecx,eax
         or ecx,ecx
         je lam_abort                       ; should never happen!

         mov eax,finx
         cdq
         shl eax,nshl                       ; set unit vector to 2^nshl (256)
         idiv ecx
         mov finx,eax

         mov eax,finy
         cdq
         shl eax,nshl
         idiv ecx
         mov finy,eax

         mov eax,finz
         cdq
         shl eax,nshl
         idiv ecx
         mov finz,eax

         mov ebx,finx
         mov ecx,finy
         mov ebp,finz

lam_abort:
         ret

