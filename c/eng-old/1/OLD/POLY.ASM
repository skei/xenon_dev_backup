.386p
                NAME    POLY
                EXTRN   __8087 :BYTE
                EXTRN   _fltused_ :BYTE
                EXTRN   GTexScanleft_ :BYTE
                EXTRN   TexScanleft_ :BYTE
                EXTRN   __ShadeTable :BYTE
                EXTRN   __Texture1 :BYTE
                EXTRN   Line_ :BYTE
                EXTRN   __Buffer :BYTE
DGROUP          GROUP   CONST,CONST2,_DATA,_BSS
_TEXT           SEGMENT PARA PUBLIC USE32 'CODE'
                ASSUME  CS:_TEXT ,DS:DGROUP,SS:DGROUP
                PUBLIC  PolyWireFrame_ 
                PUBLIC  ScanRight_ 
                PUBLIC  TexScanLeft_ 
                PUBLIC  GTexScanLeft_ 
                PUBLIC  TexFillPoly_ 
                PUBLIC  GTexFillPoly_ 
                PUBLIC  TexPoly_ 
                PUBLIC  GTexPoly_ 
float2int_:     sub     esp,00000008H
                fld     dword ptr +0cH[esp]
                fadd    qword ptr L49
                fstp    qword ptr [esp]
                mov     eax,dword ptr [esp]
                add     eax,80000000H
                add     esp,00000008H
                ret     0004H
                mov     eax,eax
float2fixed_:   sub     esp,00000008H
                fld     dword ptr +0cH[esp]
                fadd    qword ptr L50
                fstp    qword ptr [esp]
                mov     eax,dword ptr [esp]
                add     eax,80000000H
                add     esp,00000008H
                ret     0004H
                mov     eax,eax
CEIL_:          push    edx
                sub     esp,00000008H
                fld     dword ptr +10H[esp]
                fadd    qword ptr L51
                fstp    qword ptr [esp]
                mov     edx,dword ptr [esp]
                add     edx,80000000H
                mov     eax,edx
                sar     eax,10H
                test    dx,0ffffH
                je      short L1
                inc     eax
L1:             add     esp,00000008H
                pop     edx
                ret     0004H
                mov     eax,eax
PolyWireFrame_: push    ebx
                push    ecx
                push    edx
                push    esi
                sub     esp,00000028H
                mov     ebx,dword ptr [eax]
                mov     edx,dword ptr __Buffer
                mov     dword ptr [esp],ebx
                push    edx
                mov     ebx,dword ptr +4H[eax]
                mov     eax,dword ptr +8H[eax]
                push    0000000fH
                mov     dword ptr +10H[esp],eax
                mov     eax,dword ptr +8H[esp]
                push    dword ptr +10H[ebx]
                fld     dword ptr +0cH[eax]
                fadd    qword ptr L52
                mov     dword ptr +10H[esp],ebx
                fstp    qword ptr +20H[esp]
                call    near ptr float2int_
                push    dword ptr +0cH[ebx]
                mov     ecx,eax
                call    near ptr float2int_
                mov     ebx,eax
                mov     eax,dword ptr +8H[esp]
                mov     esi,dword ptr +1cH[esp]
                push    dword ptr +10H[eax]
                add     esi,80000000H
                call    near ptr float2int_
                mov     edx,eax
                mov     eax,esi
                call    near ptr Line_
                mov     ebx,dword ptr __Buffer
                mov     eax,dword ptr [esp]
                push    ebx
                fld     dword ptr +0cH[eax]
                mov     eax,dword ptr +0cH[esp]
                push    0000000fH
                fadd    qword ptr L52
                push    dword ptr +10H[eax]
                fstp    qword ptr +28H[esp]
                call    near ptr float2int_
                mov     ecx,eax
                mov     eax,dword ptr +10H[esp]
                push    dword ptr +0cH[eax]
                call    near ptr float2int_
                mov     ebx,eax
                mov     eax,dword ptr +8H[esp]
                mov     esi,dword ptr +24H[esp]
                push    dword ptr +10H[eax]
                add     esi,80000000H
                call    near ptr float2int_
                mov     edx,eax
                mov     eax,esi
                call    near ptr Line_
                mov     eax,dword ptr +4H[esp]
                mov     ecx,dword ptr __Buffer
                mov     eax,dword ptr +0cH[eax]
                push    ecx
                mov     dword ptr +28H[esp],eax
                push    0000000fH
                fld     dword ptr +2cH[esp]
                mov     eax,dword ptr +10H[esp]
                fadd    qword ptr L52
                push    dword ptr +10H[eax]
                fstp    qword ptr +18H[esp]
                call    near ptr float2int_
                mov     ecx,eax
                mov     eax,dword ptr +10H[esp]
                push    dword ptr +0cH[eax]
                call    near ptr float2int_
                mov     ebx,eax
                mov     eax,dword ptr +0cH[esp]
                mov     esi,dword ptr +14H[esp]
                push    dword ptr +10H[eax]
                add     esi,80000000H
                call    near ptr float2int_
                mov     edx,eax
                mov     eax,esi
                call    near ptr Line_
                add     esp,00000028H
                pop     esi
                pop     edx
                pop     ecx
                pop     ebx
                ret     
ScanRight_:     push    ebx
                push    ecx
                push    esi
                push    edi
                push    ebp
                sub     esp,00000018H
                mov     ebx,eax
                mov     esi,edx
                sub     esp,00000004H
                mov     edx,dword ptr +10H[eax]
                mov     dword ptr [esp],edx
                call    near ptr float2fixed_
                mov     edx,eax
                sar     eax,10H
                test    dx,0ffffH
                je      short L2
                inc     eax
L2:             mov     dword ptr +14H[esp],eax
                sub     esp,00000004H
                mov     eax,dword ptr +10H[esi]
                mov     dword ptr [esp],eax
                call    near ptr float2fixed_
                mov     edx,eax
                sar     edx,10H
                test    ax,0ffffH
                je      short L3
                inc     edx
L3:             mov     edi,dword ptr +14H[esp]
                mov     ecx,edx
                cmp     edx,edi
                je      near ptr L9
                fld     dword ptr +0cH[esi]
                fsub    dword ptr +0cH[ebx]
                fild    dword ptr +14H[esp]
                fld     dword ptr +10H[esi]
                fsub    dword ptr +10H[ebx]
                fxch    st(1)
                fsub    dword ptr +10H[ebx]
                fxch    st(1)
                fdivp   st(2),st
                fmul    st,st(1)
                fxch    st(1)
                fld     qword ptr L53
                fxch    st(1)
                fadd    st,st(1)
                mov     ebp,dword ptr +14H[esp]
                fstp    qword ptr +8H[esp]
                fxch    st(1)
                fadd    dword ptr +0cH[ebx]
                mov     edi,dword ptr +8H[esp]
                faddp   st(1),st
                add     edi,80000000H
                fstp    qword ptr [esp]
                mov     eax,dword ptr [esp]
                mov     esi,edi
                add     eax,80008000H
                cmp     ebp,000000c7H
                jg      near ptr L9
                test    edx,edx
                jl      near ptr L9
                test    ebp,ebp
                jge     short L4
                mov     edx,ebp
                neg     edx
                imul    edi,edx
                xor     ebx,ebx
                mov     dword ptr +14H[esp],ebx
                add     eax,edi
L4:             cmp     ecx,000000c7H
                jle     short L5
                mov     ecx,000000c7H
L5:             mov     edx,dword ptr +14H[esp]
                cmp     edx,dword ptr __y1
                jge     short L6
                mov     dword ptr __y1,edx
L6:             cmp     ecx,dword ptr __y2
                jle     short L7
                mov     dword ptr __y2,ecx
L7:             mov     ebx,dword ptr +14H[esp]
                lea     edx,+0H[ebx*4]
                add     edx,ebx
                mov     ebx,offset DGROUP:_Edge
                shl     edx,02H
                add     ebx,edx
                mov     edx,dword ptr +14H[esp]
                cmp     ecx,edx
                jle     short L9
L8:             mov     edi,eax
                add     ebx,00000014H
                inc     edx
                sar     edi,10H
                add     eax,esi
                mov     dword ptr -10H[ebx],edi
                cmp     edx,ecx
                jl      short L8
L9:             add     esp,00000018H
                pop     ebp
                pop     edi
                pop     esi
                pop     ecx
                pop     ebx
                ret     
                mov     eax,eax
TexScanLeft_:   push    ebx
                push    ecx
                push    esi
                push    edi
                push    ebp
                sub     esp,0000005cH
                mov     esi,eax
                sub     esp,00000004H
                mov     ebx,dword ptr +10H[eax]
                mov     dword ptr [esp],ebx
                call    near ptr float2fixed_
                mov     ebx,eax
                sar     eax,10H
                test    bx,0ffffH
                je      short L10
                inc     eax
L10:            mov     dword ptr +4cH[esp],eax
                sub     esp,00000004H
                mov     eax,dword ptr +10H[edx]
                mov     dword ptr [esp],eax
                call    near ptr float2fixed_
                mov     ebx,eax
                sar     eax,10H
                test    bx,0ffffH
                je      short L11
                inc     eax
L11:            mov     ebx,dword ptr +4cH[esp]
                mov     edi,eax
                cmp     eax,ebx
                je      near ptr L15
                fld     dword ptr +0cH[edx]
                fld     dword ptr +10H[edx]
                fsub    dword ptr +10H[esi]
                fxch    st(1)
                fsub    dword ptr +0cH[esi]
                fld1    
                fdivrp  st(2),st
                fmul    st,st(1)
                fld     dword ptr +34H[edx]
                fld     dword ptr +30H[edx]
                fsub    dword ptr +30H[esi]
                fxch    st(1)
                fsub    dword ptr +34H[esi]
                fxch    st(1)
                fmul    st,st(3)
                fxch    st(1)
                fmulp   st(3),st
                fild    dword ptr +4cH[esp]
                fsub    dword ptr +10H[esi]
                fld     st(2)
                fmul    st,st(1)
                fld     st(2)
                fmul    st,st(2)
                fld     st(5)
                fmulp   st(3),st
                fxch    st(4)
                fld     qword ptr L54
                fxch    st(1)
                fadd    st,st(1)
                fxch    st(4)
                fadd    st,st(1)
                fxch    st(6)
                fadd    st,st(1)
                fxch    st(4)
                fstp    qword ptr +8H[esp]
                fxch    st(5)
                fstp    qword ptr [esp]
                fxch    st(2)
                fstp    qword ptr +28H[esp]
                mov     ebx,dword ptr +8H[esp]
                mov     ebp,dword ptr [esp]
                mov     edx,dword ptr +28H[esp]
                fxch    st(2)
                fadd    dword ptr +30H[esi]
                fxch    st(2)
                fadd    dword ptr +34H[esi]
                fxch    st(1)
                fadd    dword ptr +0cH[esi]
                mov     esi,dword ptr +4cH[esp]
                add     ebx,80000000H
                add     ebp,80000000H
                add     edx,80000000H
                fadd    st,st(3)
                fxch    st(2)
                fadd    st,st(3)
                fxch    st(1)
                faddp   st(3),st
                mov     dword ptr +44H[esp],ebx
                mov     dword ptr +50H[esp],ebx
                fxch    st(1)
                fstp    qword ptr +20H[esp]
                mov     dword ptr +58H[esp],ebp
                fstp    qword ptr +18H[esp]
                mov     dword ptr +48H[esp],edx
                mov     dword ptr +54H[esp],edx
                fstp    qword ptr +10H[esp]
                mov     ecx,dword ptr +20H[esp]
                mov     ebx,dword ptr +18H[esp]
                mov     edx,dword ptr +10H[esp]
                add     ecx,80008000H
                add     ebx,80008000H
                add     edx,80008000H
                cmp     esi,000000c7H
                jg      near ptr L15
                test    eax,eax
                jl      near ptr L15
                test    esi,esi
                jge     short L12
                mov     eax,esi
                mov     esi,dword ptr +44H[esp]
                neg     eax
                imul    esi,eax
                imul    ebp,eax
                add     ebx,ebp
                imul    eax,dword ptr +48H[esp]
                add     edx,eax
                xor     eax,eax
                add     ecx,esi
                mov     dword ptr +4cH[esp],eax
L12:            cmp     edi,000000c7H
                jle     short L13
                mov     edi,000000c7H
L13:            mov     esi,dword ptr +4cH[esp]
                lea     eax,+0H[esi*4]
                add     eax,esi
                shl     eax,02H
                add     eax,offset DGROUP:_Edge
                cmp     edi,esi
                jle     short L15
L14:            mov     ebp,ecx
                sar     ebp,10H
                inc     esi
                mov     dword ptr [eax],ebp
                mov     ebp,dword ptr +50H[esp]
                mov     dword ptr +8H[eax],ebx
                add     ecx,ebp
                mov     ebp,dword ptr +58H[esp]
                mov     dword ptr +0cH[eax],edx
                add     ebx,ebp
                mov     ebp,dword ptr +54H[esp]
                add     eax,00000014H
                add     edx,ebp
                cmp     esi,edi
                jl      short L14
L15:            add     esp,0000005cH
                pop     ebp
                pop     edi
                pop     esi
                pop     ecx
                pop     ebx
                ret     
                nop     
GTexScanLeft_:  push    ebx
                push    ecx
                push    esi
                push    edi
                push    ebp
                sub     esp,0000007cH
                mov     esi,eax
                mov     edi,edx
                sub     esp,00000004H
                mov     edx,dword ptr +10H[eax]
                mov     dword ptr [esp],edx
                call    near ptr float2fixed_
                mov     edx,eax
                sar     edx,10H
                test    ax,0ffffH
                je      short L16
                inc     edx
L16:            mov     dword ptr +64H[esp],edx
                sub     esp,00000004H
                mov     eax,dword ptr +10H[edi]
                mov     dword ptr [esp],eax
                call    near ptr float2fixed_
                mov     edx,eax
                sar     eax,10H
                test    dx,0ffffH
                je      short L17
                inc     eax
L17:            mov     edx,dword ptr +64H[esp]
                mov     dword ptr +78H[esp],eax
                cmp     eax,edx
                je      near ptr L21
                fld     dword ptr +10H[edi]
                fld     dword ptr +0cH[edi]
                fld     dword ptr +30H[edi]
                fxch    st(2)
                fsub    dword ptr +10H[esi]
                fxch    st(1)
                fsub    dword ptr +0cH[esi]
                fxch    st(2)
                fsub    dword ptr +30H[esi]
                fld1    
                fdivrp  st(2),st
                fxch    st(2)
                fmul    st,st(1)
                fxch    st(2)
                fmul    st,st(1)
                fld     dword ptr +38H[edi]
                fld     dword ptr +34H[edi]
                fsub    dword ptr +34H[esi]
                fxch    st(1)
                fsub    dword ptr +38H[esi]
                fxch    st(1)
                fmul    st,st(3)
                fxch    st(1)
                fmulp   st(3),st
                fild    dword ptr +64H[esp]
                fsub    dword ptr +10H[esi]
                fld     st(4)
                fmul    st,st(1)
                fld     st(3)
                fmul    st,st(2)
                fxch    st(3)
                fst     dword ptr +50H[esp]
                fmul    st,st(2)
                fxch    st(5)
                fst     dword ptr +4cH[esp]
                fmulp   st(2),st
                fxch    st(5)
                fld     qword ptr L55
                fxch    st(1)
                fadd    st,st(1)
                fxch    st(4)
                fadd    st,st(1)
                fxch    st(4)
                fstp    qword ptr +38H[esp]
                fld     dword ptr +50H[esp]
                fxch    st(4)
                fstp    qword ptr +28H[esp]
                fld     dword ptr +4cH[esp]
                fxch    st(4)
                fadd    st,st(1)
                fxch    st(4)
                fadd    st,st(1)
                fxch    st(4)
                fstp    qword ptr +10H[esp]
                fxch    st(3)
                fstp    qword ptr +30H[esp]
                mov     ebp,dword ptr +38H[esp]
                mov     edx,dword ptr +28H[esp]
                mov     edi,dword ptr +30H[esp]
                fxch    st(4)
                fadd    dword ptr +0cH[esi]
                fxch    st(1)
                fadd    dword ptr +30H[esi]
                fxch    st(3)
                fadd    dword ptr +34H[esi]
                fxch    st(4)
                fadd    dword ptr +38H[esi]
                add     ebp,80000000H
                add     edx,80000000H
                add     edi,80000000H
                fxch    st(1)
                fadd    st,st(2)
                fxch    st(3)
                fadd    st,st(2)
                fxch    st(4)
                fadd    st,st(2)
                fxch    st(1)
                faddp   st(2),st
                mov     dword ptr +68H[esp],ebp
                fxch    st(2)
                fstp    qword ptr +8H[esp]
                mov     dword ptr +5cH[esp],edx
                mov     dword ptr +74H[esp],edx
                fxch    st(2)
                fstp    qword ptr [esp]
                fstp    qword ptr +18H[esp]
                mov     dword ptr +58H[esp],edi
                mov     dword ptr +70H[esp],edi
                fstp    qword ptr +20H[esp]
                mov     ebx,dword ptr +8H[esp]
                mov     ecx,dword ptr [esp]
                mov     edx,dword ptr +10H[esp]
                mov     esi,dword ptr +20H[esp]
                mov     edi,dword ptr +64H[esp]
                add     ebx,80008000H
                add     edx,80000000H
                add     ecx,80008000H
                mov     dword ptr +54H[esp],edx
                mov     dword ptr +6cH[esp],edx
                mov     edx,dword ptr +18H[esp]
                add     esi,80008000H
                add     edx,80008000H
                cmp     edi,000000c7H
                jg      near ptr L21
                test    eax,eax
                jl      near ptr L21
                test    edi,edi
                jge     short L18
                mov     eax,edi
                neg     eax
                imul    ebp,eax
                mov     edi,dword ptr +5cH[esp]
                imul    edi,eax
                add     ecx,edi
                mov     edi,dword ptr +54H[esp]
                imul    edi,eax
                add     edx,edi
                imul    eax,dword ptr +58H[esp]
                add     ebx,ebp
                xor     ebp,ebp
                mov     dword ptr +64H[esp],ebp
                add     esi,eax
L18:            cmp     dword ptr +78H[esp],000000c7H
                jle     short L19
                mov     dword ptr +78H[esp],000000c7H
L19:            mov     edi,dword ptr +64H[esp]
                lea     eax,+0H[edi*4]
                add     eax,edi
                shl     eax,02H
                add     eax,offset DGROUP:_Edge
                cmp     edi,dword ptr +78H[esp]
                jge     short L21
L20:            mov     ebp,ebx
                sar     ebp,10H
                mov     dword ptr [eax],ebp
                mov     ebp,dword ptr +68H[esp]
                inc     edi
                add     ebx,ebp
                mov     ebp,dword ptr +74H[esp]
                mov     dword ptr +8H[eax],ecx
                add     ecx,ebp
                mov     ebp,dword ptr +6cH[esp]
                mov     dword ptr +0cH[eax],edx
                add     edx,ebp
                mov     ebp,dword ptr +70H[esp]
                mov     dword ptr +10H[eax],esi
                add     esi,ebp
                mov     ebp,dword ptr +78H[esp]
                add     eax,00000014H
                cmp     edi,ebp
                jl      short L20
L21:            add     esp,0000007cH
                pop     ebp
                pop     edi
                pop     esi
                pop     ecx
                pop     ebx
                ret     
                mov     eax,eax
TexFillPoly_:   push    esi
                push    edi
                push    ebp
                sub     esp,00000018H
                mov     esi,eax
                mov     dword ptr +0cH[esp],ebx
                mov     dword ptr +10H[esp],ecx
                cmp     eax,edx
                je      near ptr L27
                cmp     eax,000000c7H
                jg      near ptr L27
                test    edx,edx
                jl      near ptr L27
                cmp     eax,edx
                jge     near ptr L27
                shl     eax,02H
                add     eax,esi
                shl     eax,02H
                mov     dword ptr +8H[esp],eax
                lea     eax,+0H[esi*4]
                add     eax,esi
                shl     eax,06H
                mov     dword ptr +4H[esp],eax
                lea     eax,+0H[edx*4]
                add     eax,edx
                shl     eax,06H
                mov     dword ptr [esp],eax
L22:            mov     ecx,dword ptr +8H[esp]
                mov     eax,dword ptr +8H[esp]
                mov     ebx,dword ptr +8H[esp]
                mov     edx,dword ptr +8H[esp]
                mov     ecx,dword ptr _Edge+4H[ecx]
                mov     eax,dword ptr _Edge+8H[eax]
                mov     ebx,dword ptr _Edge[ebx]
                mov     edx,dword ptr _Edge+0cH[edx]
                cmp     ebx,0000013fH
                jg      near ptr L26
                test    ecx,ecx
                jl      near ptr L26
                cmp     ecx,0000013fH
                jle     short L23
                mov     ecx,0000013fH
L23:            test    ebx,ebx
                jge     short L24
                mov     esi,dword ptr +0cH[esp]
                neg     ebx
                imul    esi,ebx
                add     eax,esi
                imul    ebx,dword ptr +10H[esp]
                add     edx,ebx
                xor     ebx,ebx
L24:            sub     ecx,ebx
                mov     dword ptr +14H[esp],ecx
                test    ecx,ecx
                jle     short L26
                mov     ecx,dword ptr +4H[esp]
                mov     ebp,dword ptr +14H[esp]
                add     ecx,ebx
                mov     ebx,dword ptr __Buffer
                xor     esi,esi
                add     ebx,ecx
                test    ebp,ebp
                jle     short L26
                mov     ebp,dword ptr +14H[esp]
L25:            mov     ecx,edx
                mov     edi,eax
                sar     ecx,08H
                sar     edi,10H
                and     ecx,0000ff00H
                and     edi,000000ffH
                add     ecx,edi
                mov     edi,dword ptr __Texture1
                inc     ebx
                inc     esi
                mov     cl,byte ptr [ecx+edi]
                mov     edi,dword ptr +10H[esp]
                mov     byte ptr -1H[ebx],cl
                mov     ecx,dword ptr +0cH[esp]
                add     edx,edi
                add     eax,ecx
                cmp     esi,ebp
                jl      short L25
L26:            mov     edx,dword ptr +8H[esp]
                mov     ebx,dword ptr +4H[esp]
                mov     ecx,dword ptr [esp]
                add     edx,00000014H
                add     ebx,00000140H
                mov     dword ptr +8H[esp],edx
                mov     dword ptr +4H[esp],ebx
                cmp     ebx,ecx
                jl      near ptr L22
L27:            add     esp,00000018H
                pop     ebp
                pop     edi
                pop     esi
                ret     
                mov     eax,eax
GTexFillPoly_:  push    esi
                push    edi
                push    ebp
                sub     esp,0000001cH
                mov     esi,eax
                mov     dword ptr +10H[esp],ebx
                mov     dword ptr +14H[esp],ecx
                cmp     eax,edx
                je      near ptr L33
                cmp     eax,000000c7H
                jg      near ptr L33
                test    edx,edx
                jl      near ptr L33
                cmp     eax,edx
                jge     near ptr L33
                shl     eax,02H
                add     eax,esi
                shl     eax,02H
                mov     dword ptr +0cH[esp],eax
                lea     eax,+0H[esi*4]
                add     eax,esi
                shl     eax,06H
                mov     dword ptr +8H[esp],eax
                lea     eax,+0H[edx*4]
                add     eax,edx
                shl     eax,06H
                mov     dword ptr +4H[esp],eax
L28:            mov     esi,dword ptr +0cH[esp]
                mov     ebx,dword ptr +0cH[esp]
                mov     edx,dword ptr +0cH[esp]
                mov     ecx,dword ptr +0cH[esp]
                mov     eax,dword ptr +0cH[esp]
                mov     esi,dword ptr _Edge+4H[esi]
                mov     ebx,dword ptr _Edge+8H[ebx]
                mov     edx,dword ptr _Edge+0cH[edx]
                mov     ecx,dword ptr _Edge[ecx]
                mov     eax,dword ptr _Edge+10H[eax]
                cmp     ecx,0000013fH
                jg      near ptr L32
                test    esi,esi
                jl      near ptr L32
                cmp     esi,0000013fH
                jle     short L29
                mov     esi,0000013fH
L29:            test    ecx,ecx
                jge     short L30
                mov     edi,dword ptr +10H[esp]
                neg     ecx
                imul    edi,ecx
                add     ebx,edi
                mov     edi,dword ptr +14H[esp]
                imul    edi,ecx
                add     edx,edi
                imul    ecx,dword ptr +2cH[esp]
                add     eax,ecx
                xor     ecx,ecx
L30:            sub     esi,ecx
                mov     dword ptr +18H[esp],esi
                test    esi,esi
                jle     near ptr L32
                mov     esi,dword ptr +8H[esp]
                mov     ebp,dword ptr +18H[esp]
                add     ecx,esi
                mov     esi,dword ptr __Buffer
                xor     edi,edi
                add     esi,ecx
                test    ebp,ebp
                jle     short L32
L31:            mov     ecx,edx
                sar     ecx,08H
                and     ecx,0000ff00H
                mov     dword ptr [esp],ecx
                mov     ecx,ebx
                sar     ecx,10H
                mov     ebp,dword ptr [esp]
                and     ecx,000000ffH
                add     ebp,ecx
                mov     ecx,dword ptr __Texture1
                add     ecx,ebp
                movsx   ecx,byte ptr [ecx]
                mov     dword ptr [esp],ecx
                mov     ecx,eax
                sar     ecx,08H
                mov     ebp,dword ptr [esp]
                and     ecx,0000ff00H
                add     ebp,ecx
                mov     ecx,dword ptr __ShadeTable
                inc     esi
                add     ebp,ecx
                inc     edi
                mov     cl,byte ptr +0H[ebp]
                mov     ebp,dword ptr +10H[esp]
                mov     byte ptr -1H[esi],cl
                mov     ecx,dword ptr +14H[esp]
                add     ebx,ebp
                mov     ebp,dword ptr +2cH[esp]
                add     edx,ecx
                mov     ecx,dword ptr +18H[esp]
                add     eax,ebp
                cmp     edi,ecx
                jl      short L31
L32:            mov     edx,dword ptr +0cH[esp]
                mov     ebx,dword ptr +8H[esp]
                mov     ecx,dword ptr +4H[esp]
                add     edx,00000014H
                add     ebx,00000140H
                mov     dword ptr +0cH[esp],edx
                mov     dword ptr +8H[esp],ebx
                cmp     ebx,ecx
                jl      near ptr L28
L33:            add     esp,0000001cH
                pop     ebp
                pop     edi
                pop     esi
                ret     0004H
                lea     eax,+0H[eax]
TexPoly_:       push    ebx
                push    ecx
                push    edx
                push    esi
                push    edi
                push    ebp
                sub     esp,00000028H
                mov     esi,eax
                mov     eax,dword ptr [eax]
                mov     dword ptr +8H[esp],eax
                mov     eax,dword ptr +4H[esi]
                mov     dword ptr +4H[esp],eax
                mov     eax,dword ptr +8H[esi]
                mov     edx,0000270fH
                mov     dword ptr [esp],eax
                mov     eax,dword ptr +4H[esp]
                mov     ebx,0ffffd8f1H
                mov     dword ptr __y1,edx
                fld     dword ptr +10H[eax]
                mov     eax,dword ptr [esp]
                mov     dword ptr __y2,ebx
                xor     edx,edx
                fcomp   dword ptr +10H[eax]
                fstsw   ax
                sahf    
                jae     short L34
                mov     edx,00000001H
L34:            mov     eax,dword ptr +8H[esp]
                mov     ebx,dword ptr [esp+edx*4]
                fld     dword ptr +10H[eax]
                fcomp   dword ptr +10H[ebx]
                fstsw   ax
                sahf    
                jae     short L35
                mov     edx,00000002H
L35:            lea     ebx,+1H[edx]
                cmp     ebx,00000002H
                jle     short L36
                xor     ebx,ebx
L36:            lea     eax,-1H[edx]
                test    eax,eax
                jge     short L37
                mov     eax,00000002H
L37:            lea     edi,+0H[eax*4]
                lea     ecx,+0H[edx*4]
                mov     ebp,dword ptr [esp+edi]
                mov     eax,dword ptr [esp+ecx]
                mov     edx,ebp
                call    near ptr TexScanLeft_
                mov     edx,dword ptr [esp+ebx*4]
                mov     eax,dword ptr [esp+ecx]
                call    near ptr ScanRight_
                mov     ecx,dword ptr [esp+ebx*4]
                mov     ebx,dword ptr [esp+edi]
                fld     dword ptr +10H[ecx]
                fcomp   dword ptr +10H[ebx]
                fstsw   ax
                sahf    
                jae     short L38
                mov     edx,ebx
                mov     eax,ecx
                call    near ptr ScanRight_
                jmp     short L39
L38:            mov     edx,ecx
                mov     eax,ebx
                call    near ptr TexScanleft_
L39:            mov     eax,dword ptr +28H[esi]
                mov     eax,dword ptr +7cH[eax]
                mov     dword ptr __Texture1,eax
                mov     eax,dword ptr +4H[esp]
                fld     dword ptr +10H[eax]
                mov     eax,dword ptr [esp]
                fsub    dword ptr +10H[eax]
                mov     eax,dword ptr +8H[esp]
                fld     dword ptr +10H[eax]
                mov     eax,dword ptr [esp]
                fsub    dword ptr +10H[eax]
                mov     eax,dword ptr +4H[esp]
                fld     dword ptr +0cH[eax]
                mov     eax,dword ptr [esp]
                fsub    dword ptr +0cH[eax]
                fmul    st,st(1)
                mov     eax,dword ptr +8H[esp]
                fld     dword ptr +0cH[eax]
                mov     eax,dword ptr [esp]
                fsub    dword ptr +0cH[eax]
                mov     eax,dword ptr +4H[esp]
                fld     dword ptr +30H[eax]
                mov     eax,dword ptr [esp]
                fsub    dword ptr +30H[eax]
                fmul    st,st(3)
                mov     eax,dword ptr +8H[esp]
                fld     dword ptr +30H[eax]
                mov     eax,dword ptr [esp]
                fsub    dword ptr +30H[eax]
                mov     eax,dword ptr +4H[esp]
                fld     dword ptr +34H[eax]
                mov     eax,dword ptr [esp]
                fsub    dword ptr +34H[eax]
                fmulp   st(5),st
                mov     eax,dword ptr +8H[esp]
                fxch    st(2)
                fmul    st,st(5)
                fld     dword ptr +34H[eax]
                mov     eax,dword ptr [esp]
                fxch    st(3)
                fmul    st,st(6)
                fxch    st(3)
                fsub    dword ptr +34H[eax]
                fxch    st(1)
                fsubp   st(4),st
                fmulp   st(5),st
                fld1    
                fxch    st(2)
                fsubp   st(1),st
                fxch    st(4)
                fsubp   st(3),st
                fdivrp  st(1),st
                fxch    st(1)
                fmul    st,st(1)
                fxch    st(2)
                fmulp   st(1),st
                fxch    st(1)
                fstp    dword ptr +1cH[esp]
                fadd    qword ptr L56
                push    dword ptr +1cH[esp]
                fstp    qword ptr +10H[esp]
                call    near ptr float2fixed_
                mov     ebx,dword ptr +0cH[esp]
                mov     edx,dword ptr __y2
                mov     esi,dword ptr __y1
                mov     ecx,eax
                add     ebx,80000000H
                mov     eax,esi
                call    near ptr TexFillPoly_
                add     esp,00000028H
                pop     ebp
                pop     edi
                pop     esi
                pop     edx
                pop     ecx
                pop     ebx
                ret     
GTexPoly_:      push    ebx
                push    ecx
                push    edx
                push    esi
                push    edi
                push    ebp
                sub     esp,0000002cH
                mov     esi,eax
                mov     eax,dword ptr [eax]
                mov     dword ptr +8H[esp],eax
                mov     eax,dword ptr +4H[esi]
                mov     dword ptr +4H[esp],eax
                mov     ebx,0ffffd8f1H
                mov     eax,dword ptr +8H[esi]
                mov     edx,0000270fH
                mov     dword ptr [esp],eax
                mov     dword ptr __y2,ebx
                mov     ecx,dword ptr +10H[eax]
                mov     dword ptr __y1,edx
                cmp     ecx,43470000H
                jle     short L40
                mov     eax,dword ptr +4H[esp]
                cmp     dword ptr +10H[eax],43470000H
                jle     short L40
                mov     eax,dword ptr +8H[esp]
                cmp     dword ptr +10H[eax],43470000H
                jg      near ptr L48
L40:            mov     eax,dword ptr [esp]
                cmp     dword ptr +0cH[eax],439f8000H
                jle     short L41
                mov     eax,dword ptr +4H[esp]
                cmp     dword ptr +0cH[eax],439f8000H
                jle     short L41
                mov     eax,dword ptr +8H[esp]
                cmp     dword ptr +0cH[eax],439f8000H
                jg      near ptr L48
L41:            mov     eax,dword ptr +4H[esp]
                fld     dword ptr +10H[eax]
                mov     eax,dword ptr [esp]
                xor     edx,edx
                fcomp   dword ptr +10H[eax]
                fstsw   ax
                sahf    
                jae     short L42
                mov     edx,00000001H
L42:            mov     eax,dword ptr +8H[esp]
                mov     ebx,dword ptr [esp+edx*4]
                fld     dword ptr +10H[eax]
                fcomp   dword ptr +10H[ebx]
                fstsw   ax
                sahf    
                jae     short L43
                mov     edx,00000002H
L43:            lea     ebx,+1H[edx]
                cmp     ebx,00000002H
                jle     short L44
                xor     ebx,ebx
L44:            lea     eax,-1H[edx]
                test    eax,eax
                jge     short L45
                mov     eax,00000002H
L45:            lea     edi,+0H[eax*4]
                lea     ecx,+0H[edx*4]
                mov     ebp,dword ptr [esp+edi]
                mov     eax,dword ptr [esp+ecx]
                mov     edx,ebp
                call    near ptr GTexScanLeft_
                mov     edx,dword ptr [esp+ebx*4]
                mov     eax,dword ptr [esp+ecx]
                call    near ptr ScanRight_
                mov     ebx,dword ptr [esp+ebx*4]
                mov     ecx,dword ptr [esp+edi]
                fld     dword ptr +10H[ebx]
                fcomp   dword ptr +10H[ecx]
                fstsw   ax
                sahf    
                jae     short L46
                mov     edx,ecx
                mov     eax,ebx
                call    near ptr ScanRight_
                jmp     short L47
L46:            mov     edx,ebx
                mov     eax,ecx
                call    near ptr GTexScanleft_
L47:            mov     eax,dword ptr +28H[esi]
                mov     eax,dword ptr +7cH[eax]
                mov     dword ptr __Texture1,eax
                mov     eax,dword ptr +4H[esp]
                fld     dword ptr +10H[eax]
                mov     eax,dword ptr [esp]
                fsub    dword ptr +10H[eax]
                mov     eax,dword ptr +8H[esp]
                fld     dword ptr +10H[eax]
                mov     eax,dword ptr [esp]
                fsub    dword ptr +10H[eax]
                mov     eax,dword ptr +4H[esp]
                fld     dword ptr +0cH[eax]
                mov     eax,dword ptr [esp]
                fsub    dword ptr +0cH[eax]
                fxch    st(1)
                fstp    dword ptr +18H[esp]
                fmul    dword ptr +18H[esp]
                mov     eax,dword ptr +8H[esp]
                fld     dword ptr +0cH[eax]
                mov     eax,dword ptr [esp]
                fsub    dword ptr +0cH[eax]
                mov     eax,dword ptr +4H[esp]
                fld     dword ptr +30H[eax]
                mov     eax,dword ptr [esp]
                fsub    dword ptr +30H[eax]
                fmul    dword ptr +18H[esp]
                mov     eax,dword ptr +8H[esp]
                fld     dword ptr +30H[eax]
                mov     eax,dword ptr [esp]
                fsub    dword ptr +30H[eax]
                mov     eax,dword ptr +4H[esp]
                fld     dword ptr +34H[eax]
                mov     eax,dword ptr [esp]
                fsub    dword ptr +34H[eax]
                fmul    dword ptr +18H[esp]
                mov     eax,dword ptr +8H[esp]
                fxch    st(5)
                fstp    dword ptr +20H[esp]
                fld     dword ptr +34H[eax]
                mov     eax,dword ptr [esp]
                fxch    st(3)
                fmul    dword ptr +20H[esp]
                fxch    st(3)
                fsub    dword ptr +34H[eax]
                fxch    st(1)
                fmul    dword ptr +20H[esp]
                fxch    st(1)
                fmul    dword ptr +20H[esp]
                mov     eax,dword ptr +4H[esp]
                fxch    st(3)
                fsubp   st(4),st
                fld     dword ptr +38H[eax]
                mov     eax,dword ptr [esp]
                fld1    
                fxch    st(1)
                fsub    dword ptr +38H[eax]
                mov     eax,dword ptr +8H[esp]
                fmul    dword ptr +18H[esp]
                fxch    st(1)
                fdivrp  st(5),st
                fld     dword ptr +38H[eax]
                mov     eax,dword ptr [esp]
                fxch    st(2)
                fsubp   st(3),st
                fxch    st(1)
                fsub    dword ptr +38H[eax]
                fxch    st(2)
                fmul    st,st(4)
                fxch    st(2)
                fmul    dword ptr +20H[esp]
                fxch    st(3)
                fsubp   st(5),st
                fsubrp  st(2),st
                fxch    st(3)
                fmul    st,st(2)
                fxch    st(1)
                fmulp   st(2),st
                fstp    dword ptr +14H[esp]
                fxch    st(1)
                fadd    qword ptr L57
                fxch    st(1)
                fstp    dword ptr +24H[esp]
                push    dword ptr +24H[esp]
                fstp    qword ptr +10H[esp]
                call    near ptr float2fixed_
                push    eax
                mov     ebx,dword ptr +10H[esp]
                push    dword ptr +18H[esp]
                add     ebx,80000000H
                call    near ptr float2fixed_
                mov     edx,dword ptr __y2
                mov     esi,dword ptr __y1
                mov     ecx,eax
                mov     eax,esi
                call    near ptr GTexFillPoly_
L48:            add     esp,0000002cH
                pop     ebp
                pop     edi
                pop     esi
                pop     edx
                pop     ecx
                pop     ebx
                ret     
_TEXT           ENDS

CONST           SEGMENT DWORD PUBLIC USE32 'DATA'
L49             DB      00H,00H,00H,80H,00H,00H,30H,43H
L50             DB      00H,00H,00H,80H,00H,00H,30H,42H
L51             DB      00H,00H,00H,80H,00H,00H,30H,42H
L52             DB      00H,00H,00H,80H,00H,00H,30H,43H
L53             DB      00H,00H,00H,80H,00H,00H,30H,42H
L54             DB      00H,00H,00H,80H,00H,00H,30H,42H
L55             DB      00H,00H,00H,80H,00H,00H,30H,42H
L56             DB      00H,00H,00H,80H,00H,00H,30H,42H
L57             DB      00H,00H,00H,80H,00H,00H,30H,42H
CONST           ENDS

CONST2          SEGMENT DWORD PUBLIC USE32 'DATA'
CONST2          ENDS

_DATA           SEGMENT DWORD PUBLIC USE32 'DATA'
_DATA           ENDS

_BSS            SEGMENT PARA PUBLIC USE32 'BSS'
                PUBLIC  _Edge 
                PUBLIC  __innervadd 
                PUBLIC  __y2 
                PUBLIC  __y1 
                PUBLIC  __innerv 
                PUBLIC  __inneru 
                PUBLIC  __inneruadd 
                ORG     00000000H
_Edge           LABEL   BYTE
                ORG     00000fa0H
__innervadd     LABEL   BYTE
                ORG     00000fa4H
__y2            LABEL   BYTE
                ORG     00000fa8H
__y1            LABEL   BYTE
                ORG     00000facH
__innerv        LABEL   BYTE
                ORG     00000fb0H
__inneru        LABEL   BYTE
                ORG     00000fb4H
__inneruadd     LABEL   BYTE
                ORG     00000fb8H
_BSS            ENDS

                END
