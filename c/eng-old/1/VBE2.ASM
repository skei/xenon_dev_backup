.386p
                NAME    VBE2
                EXTRN   strncpy_ :BYTE
                EXTRN   int386x_ :BYTE
                EXTRN   memset_ :BYTE
                EXTRN   int386_ :BYTE
DGROUP          GROUP   CONST,CONST2,_DATA,_BSS
_TEXT           SEGMENT PARA PUBLIC USE32 'CODE'
                ASSUME  CS:_TEXT ,DS:DGROUP,SS:DGROUP
                PUBLIC  Set_Vesa_ 
                PUBLIC  AllocDosMem_ 
                PUBLIC  FreeDosMem_ 
                PUBLIC  MapPhysicalToLinear_ 
                PUBLIC  UnmapPhysicalToLinear_ 
                PUBLIC  VBE2_GetModeInfo_ 
                PUBLIC  VBE2_GetInfo_ 
                PUBLIC  VesaInit_ 
                PUBLIC  VesaUnInit_ 
                PUBLIC  VesaGetStart_ 
                PUBLIC  VesaSetStart_ 
                PUBLIC  VesaGetScanlines_ 
                PUBLIC  VesaSetScanlines_ 
Set_Vesa_:      push    ebx
                push    edx
                sub     esp,0000001cH
                mov     edx,00004f02H
                mov     ebx,esp
                mov     dword ptr +4H[esp],eax
                mov     eax,00000010H
                mov     dword ptr [esp],edx
                mov     edx,esp
                call    near ptr int386_
                add     esp,0000001cH
                pop     edx
                pop     ebx
                ret     
                lea     eax,+0H[eax]
AllocDosMem_:   push    ebx
                push    edx
                sub     esp,0000001cH
                mov     edx,00000100H
                mov     ebx,esp
                add     eax,0000001fH
                mov     dword ptr [esp],edx
                shr     eax,04H
                mov     edx,esp
                mov     dword ptr +4H[esp],eax
                mov     eax,00000031H
                call    near ptr int386_
                cmp     dword ptr +18H[esp],00000000H
                je      short L1
                xor     eax,eax
                jmp     short L2
L1:             mov     edx,dword ptr [esp]
                and     edx,0000ffffH
                shl     edx,04H
                mov     eax,dword ptr +0cH[esp]
                mov     dword ptr [edx],eax
                lea     eax,+10H[edx]
L2:             add     esp,0000001cH
                pop     edx
                pop     ebx
                ret     
                nop     
FreeDosMem_:    push    ebx
                push    edx
                sub     esp,0000001cH
                mov     edx,00000101H
                sub     eax,00000010H
                mov     dword ptr [esp],edx
                mov     ebx,esp
                mov     eax,dword ptr [eax]
                mov     edx,esp
                mov     dword ptr +0cH[esp],eax
                mov     eax,00000031H
                call    near ptr int386_
                add     esp,0000001cH
                pop     edx
                pop     ebx
                ret     
                mov     eax,eax
MapPhysicalToLinear_:
                push    ebx
                sub     esp,0000001cH
                mov     ebx,00000800H
                mov     word ptr +8H[esp],ax
                mov     word ptr [esp],bx
                mov     ebx,eax
                mov     word ptr +14H[esp],dx
                shr     ebx,10H
                mov     eax,edx
                mov     edx,esp
                mov     word ptr +4H[esp],bx
                shr     eax,10H
                mov     ebx,esp
                mov     word ptr +10H[esp],ax
                mov     eax,00000031H
                call    near ptr int386_
                cmp     dword ptr +18H[esp],00000000H
                je      short L3
                xor     eax,eax
                add     esp,0000001cH
                pop     ebx
                ret     
L3:             xor     eax,eax
                mov     ax,word ptr +4H[esp]
                mov     edx,eax
                xor     eax,eax
                shl     edx,10H
                mov     ax,word ptr +8H[esp]
                add     eax,edx
                add     esp,0000001cH
                pop     ebx
                ret     
                lea     eax,+0H[eax]
UnmapPhysicalToLinear_:
                push    ebx
                push    edx
                sub     esp,0000001cH
                mov     edx,00000801H
                mov     ebx,esp
                mov     word ptr [esp],dx
                mov     edx,eax
                mov     word ptr +8H[esp],ax
                shr     edx,10H
                mov     eax,00000031H
                mov     word ptr +4H[esp],dx
                mov     edx,esp
                call    near ptr int386_
                add     esp,0000001cH
                pop     edx
                pop     ebx
                ret     
                lea     eax,+0H[eax]
VBE2_GetModeInfo_:
                push    ebx
                push    ecx
                push    esi
                sub     esp,0000005cH
                mov     esi,eax
                mov     ecx,edx
                mov     ebx,0000000cH
                lea     eax,+50H[esp]
                xor     edx,edx
                call    near ptr memset_
                mov     ebx,00000032H
                mov     eax,esp
                xor     edx,edx
                shr     esi,04H
                call    near ptr memset_
                mov     edx,00004f01H
                lea     eax,+34H[esp]
                mov     dword ptr +18H[esp],ecx
                mov     word ptr +22H[esp],si
                xor     ebx,ebx
                mov     ecx,00000300H
                mov     esi,00000010H
                mov     dword ptr +1cH[esp],edx
                mov     dword ptr [esp],ebx
                mov     dword ptr +3cH[esp],ebx
                lea     ebx,+34H[esp]
                mov     dword ptr +34H[esp],ecx
                mov     dx,ds
                mov     dword ptr +38H[esp],esi
                mov     eax,esp
                mov     word ptr +56H[esp],dx
                mov     dx,ds
                lea     ecx,+50H[esp]
                mov     eax,esp
                mov     word ptr +50H[esp],dx
                lea     edx,+34H[esp]
                mov     dword ptr +48H[esp],eax
                mov     eax,00000031H
                call    near ptr int386x_
                add     esp,0000005cH
                pop     esi
                pop     ecx
                pop     ebx
                ret     
                nop     
VBE2_GetInfo_:  push    ebx
                push    ecx
                push    edx
                push    edi
                push    ebp
                sub     esp,0000005cH
                mov     ecx,eax
                mov     ebx,0000000cH
                lea     eax,+50H[esp]
                mov     edi,00000300H
                xor     edx,edx
                mov     ebp,00000010H
                call    near ptr memset_
                mov     ebx,00000032H
                mov     eax,esp
                xor     edx,edx
                shr     ecx,04H
                call    near ptr memset_
                mov     edx,00004f00H
                lea     eax,+34H[esp]
                mov     word ptr +22H[esp],cx
                mov     dword ptr +34H[esp],edi
                mov     dword ptr +38H[esp],ebp
                xor     ebx,ebx
                lea     ecx,+50H[esp]
                mov     dword ptr +1cH[esp],edx
                mov     dword ptr +18H[esp],ebx
                mov     dword ptr [esp],ebx
                mov     dx,ds
                mov     dword ptr +3cH[esp],ebx
                mov     eax,esp
                mov     word ptr +56H[esp],dx
                mov     dx,ds
                lea     ebx,+34H[esp]
                mov     eax,esp
                mov     word ptr +50H[esp],dx
                lea     edx,+34H[esp]
                mov     dword ptr +48H[esp],eax
                mov     eax,00000031H
                call    near ptr int386x_
                add     esp,0000005cH
                pop     ebp
                pop     edi
                pop     edx
                pop     ecx
                pop     ebx
                ret     
                lea     eax,+0H[eax]
VesaInit_:      push    ecx
                push    esi
                push    edi
                push    ebp
                sub     esp,00000054H
                mov     dword ptr +4cH[esp],eax
                mov     dword ptr +48H[esp],edx
                mov     dword ptr +50H[esp],ebx
                mov     eax,00000200H
                call    near ptr AllocDosMem_
                xor     edx,edx
                xor     esi,esi
                mov     edi,eax
                mov     dword ptr +40H[esp],eax
                mov     dword ptr +44H[esp],edx
                test    eax,eax
                je      near ptr L8
                mov     ebx,00000200H
                call    near ptr memset_
                mov     ebx,00000004H
                mov     edx,offset DGROUP:L11
                mov     eax,edi
                call    near ptr strncpy_
                mov     eax,edi
                call    near ptr VBE2_GetInfo_
                mov     eax,00008100H
                call    near ptr AllocDosMem_
                mov     dword ptr +38H[esp],eax
                test    eax,eax
                je      near ptr L7
                add     eax,00000fffH
                xor     al,al
                and     ah,0f0H
                mov     ecx,eax
                mov     eax,dword ptr +0eH[edi]
                mov     ebp,dword ptr +0eH[edi]
                xor     ax,ax
                and     ebp,0000ffffH
                shr     eax,0cH
                add     ebp,eax
L4:             xor     edi,edi
                mov     di,si
                add     edi,edi
                add     edi,ebp
                cmp     word ptr [edi],0ffffH
                je      near ptr L6
                mov     ebx,00000100H
                mov     eax,ecx
                xor     edx,edx
                call    near ptr memset_
                xor     edx,edx
                mov     eax,ecx
                mov     dx,word ptr [edi]
                call    near ptr VBE2_GetModeInfo_
                cmp     dword ptr +28H[ecx],00000000H
                je      near ptr L5
                xor     edx,edx
                xor     eax,eax
                mov     dx,word ptr +50H[esp]
                mov     al,byte ptr +19H[ecx]
                cmp     eax,edx
                jne     near ptr L5
                cmp     byte ptr +18H[ecx],01H
                jne     near ptr L5
                mov     ebx,dword ptr +4cH[esp]
                cmp     bx,word ptr +12H[ecx]
                jne     near ptr L5
                mov     edx,dword ptr +48H[esp]
                cmp     dx,word ptr +14H[ecx]
                jne     short L5
                mov     ax,word ptr [edi]
                or      ah,40H
                and     eax,0000ffffH
                mov     edx,00400000H
                mov     dword ptr +3cH[esp],eax
                mov     eax,dword ptr +28H[ecx]
                call    near ptr MapPhysicalToLinear_
                xor     edx,edx
                mov     dl,byte ptr +1fH[ecx]
                mov     dword ptr _RedMaskSize,edx
                xor     edx,edx
                mov     dl,byte ptr +20H[ecx]
                mov     dword ptr _RedFieldPosition,edx
                xor     edx,edx
                mov     dl,byte ptr +21H[ecx]
                mov     dword ptr _GreenMaskSize,edx
                xor     edx,edx
                mov     dl,byte ptr +22H[ecx]
                mov     dword ptr _GreenFieldPosition,edx
                xor     edx,edx
                mov     dl,byte ptr +23H[ecx]
                mov     dword ptr _BlueMaskSize,edx
                xor     edx,edx
                mov     dl,byte ptr +24H[ecx]
                mov     dword ptr _BlueFieldPosition,edx
                xor     edx,edx
                mov     dword ptr +44H[esp],eax
                mov     dx,word ptr +10H[ecx]
                mov     dword ptr _VesaPtr,eax
                mov     dword ptr _BytesPerScanLine,edx
                jmp     short L6
L5:             inc     esi
                jmp     near ptr L4
L6:             mov     eax,dword ptr +38H[esp]
                mov     ecx,00000101H
                sub     eax,00000010H
                mov     dword ptr +1cH[esp],ecx
                lea     ebx,+1cH[esp]
                mov     eax,dword ptr [eax]
                lea     edx,+1cH[esp]
                mov     dword ptr +28H[esp],eax
                mov     eax,00000031H
                call    near ptr int386_
L7:             mov     eax,dword ptr +40H[esp]
                mov     esi,00000101H
                sub     eax,00000010H
                mov     dword ptr [esp],esi
                mov     ebx,esp
                mov     eax,dword ptr [eax]
                mov     edx,esp
                mov     dword ptr +0cH[esp],eax
                mov     eax,00000031H
                call    near ptr int386_
L8:             cmp     dword ptr +44H[esp],00000000H
                je      short L9
                mov     eax,dword ptr +3cH[esp]
                mov     dword ptr _VesaMode,eax
                mov     eax,00000001H
                jmp     short L10
L9:             xor     eax,eax
L10:            add     esp,00000054H
                pop     ebp
                pop     edi
                pop     esi
                pop     ecx
                ret     
                nop     
VesaUnInit_:    push    ebx
                push    edx
                sub     esp,0000001cH
                mov     eax,dword ptr _VesaPtr
                mov     ebx,00000801H
                mov     edx,eax
                mov     word ptr [esp],bx
                mov     ebx,esp
                shr     eax,10H
                mov     word ptr +8H[esp],dx
                mov     edx,esp
                mov     word ptr +4H[esp],ax
                mov     eax,00000031H
                call    near ptr int386_
                add     esp,0000001cH
                pop     edx
                pop     ebx
                ret     
                mov     eax,eax
VesaGetStart_:  push    ebx
                push    ecx
                push    esi
                sub     esp,0000001cH
                mov     esi,eax
                mov     ecx,edx
                mov     edx,00004f07H
                mov     ebx,esp
                mov     dword ptr [esp],edx
                mov     edx,00000001H
                mov     eax,00000010H
                mov     word ptr +4H[esp],dx
                mov     edx,esp
                call    near ptr int386_
                mov     eax,dword ptr +8H[esp]
                mov     word ptr [esi],ax
                mov     eax,dword ptr +0cH[esp]
                mov     word ptr [ecx],ax
                add     esp,0000001cH
                pop     esi
                pop     ecx
                pop     ebx
                ret     
                nop     
VesaSetStart_:  push    ebx
                sub     esp,0000001cH
                mov     ebx,00004f07H
                mov     word ptr +8H[esp],ax
                mov     word ptr +0cH[esp],dx
                mov     edx,esp
                mov     dword ptr [esp],ebx
                xor     ebx,ebx
                mov     eax,00000010H
                mov     word ptr +4H[esp],bx
                mov     ebx,esp
                call    near ptr int386_
                add     esp,0000001cH
                pop     ebx
                ret     
VesaGetScanlines_:
                push    ecx
                push    esi
                push    edi
                sub     esp,0000001cH
                mov     esi,eax
                mov     edi,edx
                mov     ecx,ebx
                mov     edx,00004f06H
                mov     ebx,esp
                mov     dword ptr [esp],edx
                mov     edx,00000003H
                mov     eax,00000010H
                mov     word ptr +4H[esp],dx
                mov     edx,esp
                call    near ptr int386_
                mov     eax,dword ptr +4H[esp]
                mov     word ptr [esi],ax
                mov     eax,dword ptr +8H[esp]
                mov     word ptr [edi],ax
                mov     eax,dword ptr +0cH[esp]
                mov     word ptr [ecx],ax
                add     esp,0000001cH
                pop     edi
                pop     esi
                pop     ecx
                ret     
VesaSetScanlines_:
                push    ebx
                push    edx
                sub     esp,0000001cH
                mov     edx,00004f06H
                mov     ebx,esp
                mov     word ptr +8H[esp],ax
                mov     dword ptr [esp],edx
                mov     edx,00000002H
                mov     eax,00000010H
                mov     word ptr +4H[esp],dx
                mov     edx,esp
                call    near ptr int386_
                add     esp,0000001cH
                pop     edx
                pop     ebx
                ret     
_TEXT           ENDS

CONST           SEGMENT DWORD PUBLIC USE32 'DATA'
L11             DB      56H,42H,45H,32H,00H
CONST           ENDS

CONST2          SEGMENT DWORD PUBLIC USE32 'DATA'
CONST2          ENDS

_DATA           SEGMENT DWORD PUBLIC USE32 'DATA'
_DATA           ENDS

_BSS            SEGMENT DWORD PUBLIC USE32 'BSS'
                PUBLIC  _VesaPtr 
                PUBLIC  _BytesPerScanLine 
                PUBLIC  _GreenFieldPosition 
                PUBLIC  _VesaMode 
                PUBLIC  _GreenMaskSize 
                PUBLIC  _BlueFieldPosition 
                PUBLIC  _BlueMaskSize 
                PUBLIC  _RedMaskSize 
                PUBLIC  _RedFieldPosition 
                ORG     00000000H
_VesaPtr        LABEL   BYTE
                ORG     00000004H
_BytesPerScanLine LABEL   BYTE
                ORG     00000008H
_GreenFieldPosition LABEL   BYTE
                ORG     0000000cH
_VesaMode       LABEL   BYTE
                ORG     00000010H
_GreenMaskSize  LABEL   BYTE
                ORG     00000014H
_BlueFieldPosition LABEL   BYTE
                ORG     00000018H
_BlueMaskSize   LABEL   BYTE
                ORG     0000001cH
_RedMaskSize    LABEL   BYTE
                ORG     00000020H
_RedFieldPosition LABEL   BYTE
                ORG     00000024H
_BSS            ENDS

                END
