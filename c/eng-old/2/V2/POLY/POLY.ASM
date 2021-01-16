.386p
                NAME    POLY
                EXTRN   __8087 :BYTE
                EXTRN   __init_387_emulator :BYTE
                EXTRN   _fltused_ :BYTE
                EXTRN   __CHK :BYTE
DGROUP          GROUP   CONST,CONST2,_DATA,_BSS
_TEXT           SEGMENT BYTE PUBLIC USE32 'CODE'
                ASSUME  CS:_TEXT ,DS:DGROUP,SS:DGROUP
                PUBLIC  Polygon_ 
float2int_:     push    0000000cH
                call    near ptr __CHK
                sub     esp,00000008H
                fld     dword ptr +0cH[esp]
                fadd    qword ptr L10
L1:             fstp    qword ptr [esp]
                mov     eax,dword ptr [esp]
                add     eax,80000000H
                add     esp,00000008H
                ret     0004H
float2fixed_:   push    0000000cH
                call    near ptr __CHK
                sub     esp,00000008H
                fld     dword ptr +0cH[esp]
                fadd    qword ptr L11
                jmp     short L1
CEIL_:          push    0000000cH
                call    near ptr __CHK
                push    edx
                push    dword ptr +8H[esp]
                call    near ptr float2fixed_
                mov     edx,eax
                sar     eax,10H
                test    dx,0ffffH
                je      short L2
                inc     eax
L2:             pop     edx
                ret     0004H
Polygon_:       push    0000001cH
                call    near ptr __CHK
                push    ebx
                push    ecx
                push    edx
                sub     esp,00000008H
                mov     ebx,dword ptr [eax]
                mov     edx,dword ptr +4H[eax]
                mov     ecx,dword ptr +8H[eax]
                fld     dword ptr +4H[ebx]
                fcomp   dword ptr +4H[edx]
                fstsw   ax
                sahf    
                jae     short L3
                mov     eax,ebx
                mov     ebx,edx
                mov     edx,eax
L3:             fld     dword ptr +4H[ebx]
                fcomp   dword ptr +4H[ecx]
                fstsw   ax
                sahf    
                jae     short L4
                mov     eax,ebx
                mov     ebx,ecx
                mov     ecx,eax
L4:             fld     dword ptr +4H[edx]
                fcomp   dword ptr +4H[ecx]
                fstsw   ax
                sahf    
                jae     short L5
                mov     eax,edx
                mov     edx,ecx
                mov     ecx,eax
L5:             fld     dword ptr +4H[ebx]
                fcomp   dword ptr +4H[ecx]
                fstsw   ax
                sahf    
                je      near ptr L9
                fld     dword ptr +4H[ebx]
                fcomp   dword ptr +4H[edx]
                fstsw   ax
                sahf    
                je      short L6
                fld     dword ptr +4H[edx]
                fcomp   dword ptr +4H[ecx]
                fstsw   ax
                sahf    
                je      near ptr L8
L6:             fld     dword ptr [ebx]
                fcomp   dword ptr [edx]
                fstsw   ax
                sahf    
                jae     short L7
                mov     eax,ebx
                mov     ebx,edx
                mov     edx,eax
L7:             fld     dword ptr +4H[edx]
                fsub    dword ptr +4H[ebx]
                fld1    
                fdivrp  st(1),st
                fstp    dword ptr +4H[esp]
                fld     dword ptr [edx]
                fsub    dword ptr [ebx]
                fmul    dword ptr +4H[esp]
                sub     esp,00000004H
                fstp    dword ptr [esp]
                call    near ptr float2fixed_
                mov     dword ptr _Lslope,eax
                fld     dword ptr [ecx]
                fsub    dword ptr [edx]
                fmul    dword ptr +4H[esp]
                sub     esp,00000004H
                fstp    dword ptr [esp]
                call    near ptr float2fixed_
                mov     dword ptr _Rslope,eax
                fld     dword ptr +0cH[ecx]
                fsub    dword ptr +0cH[ebx]
                fmul    dword ptr +4H[esp]
                sub     esp,00000004H
                fstp    dword ptr [esp]
                call    near ptr float2fixed_
                mov     dword ptr __outerUadder,eax
                fld     dword ptr +10H[ecx]
                fsub    dword ptr +10H[ebx]
                fmul    dword ptr +4H[esp]
                sub     esp,00000004H
                fstp    dword ptr [esp]
                call    near ptr float2fixed_
                mov     dword ptr __outerVadder,eax
                fld     dword ptr [edx]
                fsub    dword ptr [ebx]
                fld1    
                fdivrp  st(1),st
                fstp    dword ptr [esp]
                fld     dword ptr +0cH[edx]
                fsub    dword ptr +0cH[ebx]
                fmul    dword ptr [esp]
                sub     esp,00000004H
                fstp    dword ptr [esp]
                call    near ptr float2fixed_
                mov     dword ptr __innerUadder,eax
                fld     dword ptr +10H[edx]
                fsub    dword ptr +10H[ebx]
                fmul    dword ptr [esp]
                sub     esp,00000004H
                fstp    dword ptr [esp]
                call    near ptr float2fixed_
                mov     dword ptr __innerVadder,eax
                jmp     short L9
L8:             fld     dword ptr [edx]
                fcomp   dword ptr [ecx]
                fstsw   ax
                sahf    
L9:             add     esp,00000008H
                pop     edx
                pop     ecx
                pop     ebx
                ret     
_TEXT           ENDS

CONST           SEGMENT DWORD PUBLIC USE32 'DATA'
L10             DB      00H,00H,00H,80H,00H,00H,30H,43H
L11             DB      00H,00H,00H,80H,00H,00H,30H,42H
CONST           ENDS

CONST2          SEGMENT DWORD PUBLIC USE32 'DATA'
CONST2          ENDS

_DATA           SEGMENT DWORD PUBLIC USE32 'DATA'
_DATA           ENDS

_BSS            SEGMENT DWORD PUBLIC USE32 'BSS'
                PUBLIC  __innerUadder 
                PUBLIC  _Lslope 
                PUBLIC  __outerUadder 
                PUBLIC  __outerVadder 
                PUBLIC  __innerVadder 
                PUBLIC  _Rslope 
                ORG     00000000H
__innerUadder   LABEL   BYTE
                ORG     00000004H
_Lslope         LABEL   BYTE
                ORG     00000008H
__outerUadder   LABEL   BYTE
                ORG     0000000cH
__outerVadder   LABEL   BYTE
                ORG     00000010H
__innerVadder   LABEL   BYTE
                ORG     00000014H
_Rslope         LABEL   BYTE
                ORG     00000018H
_BSS            ENDS

                END
