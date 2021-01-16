.386p
                NAME    ENGINE
                EXTRN   __8087 :BYTE
                EXTRN   _fltused_ :BYTE
                EXTRN   `IF@DPOW` :BYTE
                EXTRN   free_ :BYTE
                EXTRN   fclose_ :BYTE
                EXTRN   malloc_ :BYTE
                EXTRN   fread_ :BYTE
                EXTRN   fopen_ :BYTE
                EXTRN   fseek_ :BYTE
                EXTRN   ftell_ :BYTE
                EXTRN   __CHP :BYTE
                EXTRN   memset_ :BYTE
                EXTRN   GTexScanleft_ :BYTE
                EXTRN   TexScanleft_ :BYTE
DGROUP          GROUP   CONST,CONST2,_DATA,_BSS
_TEXT           SEGMENT PARA PUBLIC USE32 'CODE'
                ASSUME  CS:_TEXT ,DS:DGROUP,SS:DGROUP
                PUBLIC  Matrix_Identity_ 
                PUBLIC  Matrix_Translation_ 
                PUBLIC  Matrix_Rotation_ 
                PUBLIC  Matrix_Mult_ 
                PUBLIC  Matrix_Copy_ 
                PUBLIC  Matrix_Inverse_ 
                PUBLIC  Vector_Normalize_ 
                PUBLIC  Vector_Dot_ 
                PUBLIC  Vector_Cross_ 
                PUBLIC  Vector_Add_ 
                PUBLIC  Vector_Sub_ 
                PUBLIC  Vector_Trans_ 
                PUBLIC  Vector_Trans3x3_ 
                PUBLIC  Vector_inv_Trans3x3_ 
                PUBLIC  Vector_Distance_ 
                PUBLIC  Line_ 
                PUBLIC  DrawChar_ 
                PUBLIC  DrawString_ 
                PUBLIC  SetPalette_ 
                PUBLIC  PolyWireFrame_ 
                PUBLIC  ScanRight_ 
                PUBLIC  TexScanLeft_ 
                PUBLIC  GTexScanLeft_ 
                PUBLIC  TexFillPoly_ 
                PUBLIC  GTexFillPoly_ 
                PUBLIC  TexPoly_ 
                PUBLIC  GTexPoly_ 
                PUBLIC  Handle_Light_ 
                PUBLIC  CalcLight_ 
                PUBLIC  Camera_CreateMatrix_Free_ 
                PUBLIC  Camera_CreateMatrix_Aim_ 
                PUBLIC  Object_CreateMatrix_ 
                PUBLIC  Object_CreateMatrix_Inherit_ 
                PUBLIC  Object_Visible_ 
                PUBLIC  SortWorldObject_ 
                PUBLIC  Handle_Object_ 
                PUBLIC  Render_ 
                PUBLIC  f_read_ 
                PUBLIC  FileSize_ 
                PUBLIC  Object_LoadFile_ 
                PUBLIC  Object_Free_ 
                PUBLIC  Object_SetHierarchy_ 
                PUBLIC  Camera_SetRot_ 
                PUBLIC  Object_SetPos_ 
                PUBLIC  Camera_AddRot_ 
                PUBLIC  Object_AddPos_ 
                PUBLIC  Camera_SetTarget_ 
                PUBLIC  Object_SetRot_ 
                PUBLIC  Camera_AddTarget_ 
                PUBLIC  Object_AddRot_ 
                PUBLIC  Camera_SetPos_ 
                PUBLIC  Camera_AddPos_ 
                PUBLIC  ClosestColor_ 
                PUBLIC  Calc_ShadeTable_ 
                PUBLIC  Calc_ShadeHighLight_ 
                PUBLIC  Calc_PhongIntensity_ 
                PUBLIC  Calc_PhongMap_ 
                PUBLIC  Calc_GouraudMap_ 
float2int_:     sub     esp,00000008H
                fld     dword ptr +0cH[esp]
                fadd    qword ptr L168
                fstp    qword ptr [esp]
                mov     eax,dword ptr [esp]
                add     eax,80000000H
                add     esp,00000008H
                ret     0004H
                mov     eax,eax
float2fixed_:   sub     esp,00000008H
                fld     dword ptr +0cH[esp]
                fadd    qword ptr L169
                fstp    qword ptr [esp]
                mov     eax,dword ptr [esp]
                add     eax,80000000H
                add     esp,00000008H
                ret     0004H
                mov     eax,eax
float2fixed12_4_:
                sub     esp,00000008H
                fld     dword ptr +0cH[esp]
                fadd    qword ptr L170
                fstp    qword ptr [esp]
                mov     eax,dword ptr [esp]
                add     eax,80000000H
                add     esp,00000008H
                ret     0004H
                mov     eax,eax
CEIL_:          push    edx
                sub     esp,00000008H
                fld     dword ptr +10H[esp]
                fadd    qword ptr L171
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
Matrix_Identity_:
                mov     dword ptr +4H[eax],00000000H
                mov     dword ptr +8H[eax],00000000H
                mov     dword ptr +0cH[eax],00000000H
                mov     dword ptr +10H[eax],00000000H
                mov     dword ptr +14H[eax],3f800000H
                mov     dword ptr +18H[eax],00000000H
                mov     dword ptr +1cH[eax],00000000H
                mov     dword ptr +20H[eax],00000000H
                mov     dword ptr +24H[eax],00000000H
                mov     dword ptr +28H[eax],3f800000H
                mov     dword ptr +2cH[eax],00000000H
                mov     dword ptr +30H[eax],00000000H
                mov     dword ptr +34H[eax],00000000H
                mov     dword ptr +38H[eax],00000000H
                mov     dword ptr +3cH[eax],3f800000H
                mov     dword ptr [eax],3f800000H
                ret     
Matrix_Translation_:
                push    edx
                mov     dword ptr +4H[eax],00000000H
                mov     dword ptr +8H[eax],00000000H
                mov     dword ptr +10H[eax],00000000H
                mov     dword ptr +14H[eax],3f800000H
                mov     dword ptr +18H[eax],00000000H
                mov     dword ptr +20H[eax],00000000H
                mov     dword ptr +24H[eax],00000000H
                mov     dword ptr +28H[eax],3f800000H
                mov     dword ptr +30H[eax],00000000H
                mov     dword ptr +34H[eax],00000000H
                mov     dword ptr +38H[eax],00000000H
                mov     dword ptr +3cH[eax],3f800000H
                mov     edx,dword ptr +8H[esp]
                mov     dword ptr [eax],3f800000H
                mov     dword ptr +0cH[eax],edx
                mov     edx,dword ptr +0cH[esp]
                mov     dword ptr +1cH[eax],edx
                mov     edx,dword ptr +10H[esp]
                mov     dword ptr +2cH[eax],edx
                pop     edx
                ret     000cH
Matrix_Rotation_:
                push    edx
                sub     esp,00000034H
                mov     edx,eax
                fld     dword ptr +3cH[esp]
                fld     st(0)
                fsin    
                fld     dword ptr +40H[esp]
                fld     st(0)
                fsin    
                fld     dword ptr +44H[esp]
                fld     st(0)
                fsin    
                fxch    st(5)
                fcos    
                fxch    st(3)
                fcos    
                fxch    st(1)
                fcos    
                fxch    st(3)
                fstp    qword ptr +8H[esp]
                fxch    st(3)
                fst     dword ptr +1cH[esp]
                fmul    st,st(3)
                fld     st(3)
                fxch    st(3)
                fstp    dword ptr +2cH[esp]
                fxch    st(2)
                fmul    dword ptr +2cH[esp]
                fld     st(3)
                fxch    st(5)
                fstp    dword ptr +28H[esp]
                fxch    st(4)
                fmul    dword ptr +28H[esp]
                fxch    st(1)
                fstp    dword ptr +18H[esp]
                fld     qword ptr +8H[esp]
                fst     dword ptr +24H[esp]
                fmulp   st(3),st
                mov     dword ptr +0cH[edx],00000000H
                mov     eax,dword ptr +18H[esp]
                fld     dword ptr +1cH[esp]
                mov     dword ptr +8H[edx],eax
                fchs    
                fmul    dword ptr +18H[esp]
                mov     dword ptr +1cH[edx],00000000H
                fld     dword ptr +24H[esp]
                fchs    
                fmul    dword ptr +18H[esp]
                mov     dword ptr +2cH[edx],00000000H
                fld     st(1)
                fmul    dword ptr +2cH[esp]
                mov     dword ptr +30H[edx],00000000H
                fxch    st(2)
                fmul    dword ptr +28H[esp]
                fxch    st(4)
                fstp    dword ptr +18H[edx]
                fld     st(0)
                fmul    dword ptr +2cH[esp]
                fxch    st(6)
                fstp    dword ptr [edx]
                fmul    dword ptr +28H[esp]
                fxch    st(2)
                fstp    dword ptr +4H[edx]
                fld     dword ptr +24H[esp]
                fmul    dword ptr +28H[esp]
                mov     dword ptr +34H[edx],00000000H
                fsubp   st(1),st
                fld     dword ptr +24H[esp]
                fmul    dword ptr +2cH[esp]
                mov     dword ptr +38H[edx],00000000H
                faddp   st(3),st
                fld     dword ptr +1cH[esp]
                fmul    dword ptr +28H[esp]
                mov     dword ptr +3cH[edx],3f800000H
                fld     dword ptr +1cH[esp]
                fmul    dword ptr +2cH[esp]
                fxch    st(5)
                fstp    dword ptr +28H[edx]
                faddp   st(5),st
                fxch    st(3)
                fsubp   st(1),st
                fxch    st(2)
                fstp    dword ptr +10H[edx]
                fstp    dword ptr +14H[edx]
                fxch    st(1)
                fstp    dword ptr +20H[edx]
                fstp    dword ptr +24H[edx]
                add     esp,00000034H
                pop     edx
                ret     000cH
                mov     eax,eax
Matrix_Mult_:   push    ebx
                push    ecx
                push    esi
                push    edi
                push    ebp
                sub     esp,00000054H
                mov     dword ptr +40H[esp],eax
                mov     dword ptr +4cH[esp],edx
                mov     eax,dword ptr +4cH[esp]
                xor     edx,edx
                add     eax,00000010H
                mov     dword ptr +44H[esp],edx
                mov     dword ptr +48H[esp],eax
                mov     ebp,00000010H
L2:             mov     edi,dword ptr +44H[esp]
                mov     eax,dword ptr +40H[esp]
                add     eax,edi
                xor     esi,esi
                mov     dword ptr +50H[esp],eax
L3:             mov     edx,dword ptr +50H[esp]
                mov     ecx,dword ptr +48H[esp]
                xor     eax,eax
                mov     ebx,edi
                mov     dword ptr [esp+edi],eax
                mov     eax,dword ptr +4cH[esp]
                add     ecx,esi
                add     eax,esi
L4:             fld     dword ptr [edx]
                fmul    dword ptr [eax]
                add     eax,00000004H
                fadd    dword ptr [esp+ebx]
                add     edx,00000010H
                fstp    dword ptr [esp+ebx]
                cmp     eax,ecx
                jne     short L4
                add     esi,00000010H
                add     edi,00000010H
                cmp     esi,00000040H
                jne     short L3
                mov     edx,dword ptr +44H[esp]
                add     edx,00000004H
                mov     dword ptr +44H[esp],edx
                cmp     ebp,edx
                jne     short L2
                mov     edx,dword ptr +40H[esp]
                xor     eax,ecx
L5:             add     edx,00000004H
                fld     dword ptr [esp+eax*4]
                inc     eax
                fstp    dword ptr -4H[edx]
                cmp     eax,00000010H
                jl      short L5
                add     esp,00000054H
                pop     ebp
                pop     edi
                pop     esi
                pop     ecx
                pop     ebx
                ret     
                lea     eax,+0H[eax]
Matrix_Copy_:   push    ebx
                mov     ebx,edx
                mov     edx,eax
                mov     eax,ebx
                add     ebx,00000040H
L6:             add     edx,00000004H
                fld     dword ptr [eax]
                add     eax,00000004H
                fstp    dword ptr -4H[edx]
                cmp     eax,ebx
                jne     short L6
                pop     ebx
                ret     
                nop     
Matrix_Inverse_: push    ecx
                push    esi
                push    edi
                sub     esp,0000004cH
                mov     edi,eax
                fld     dword ptr [edi]
                fmul    dword ptr +14H[eax]
                fld     st(0)
                fmul    dword ptr +28H[eax]
                fmul    dword ptr +3cH[eax]
                fxch    st(1)
                fmul    dword ptr +38H[eax]
                fmul    dword ptr +2cH[eax]
                fsubp   st(1),st
                fld     dword ptr [edi]
                fmul    dword ptr +18H[eax]
                fld     st(0)
                fmul    dword ptr +24H[eax]
                fmul    dword ptr +3cH[eax]
                fsubp   st(2),st
                fmul    dword ptr +34H[eax]
                fmul    dword ptr +2cH[eax]
                faddp   st(1),st
                fld     dword ptr [edi]
                fmul    dword ptr +1cH[eax]
                fld     st(0)
                fmul    dword ptr +24H[eax]
                fmul    dword ptr +38H[eax]
                faddp   st(2),st
                fmul    dword ptr +28H[eax]
                fmul    dword ptr +34H[eax]
                fsubp   st(1),st
                fld     dword ptr +4H[edi]
                fmul    dword ptr +10H[eax]
                fld     st(0)
                fmul    dword ptr +28H[eax]
                fmul    dword ptr +3cH[eax]
                fsubp   st(2),st
                fmul    dword ptr +38H[eax]
                fmul    dword ptr +2cH[eax]
                faddp   st(1),st
                fld     dword ptr +4H[edi]
                fmul    dword ptr +18H[eax]
                fld     st(0)
                fmul    dword ptr +20H[eax]
                fmul    dword ptr +3cH[eax]
                faddp   st(2),st
                fmul    dword ptr +30H[eax]
                fmul    dword ptr +2cH[eax]
                fsubp   st(1),st
                fld     dword ptr +4H[edi]
                fmul    dword ptr +1cH[eax]
                fld     st(0)
                fmul    dword ptr +20H[eax]
                fmul    dword ptr +38H[eax]
                fsubp   st(2),st
                fmul    dword ptr +30H[eax]
                fmul    dword ptr +28H[eax]
                faddp   st(1),st
                fld     dword ptr +8H[edi]
                fmul    dword ptr +10H[eax]
                fld     st(0)
                fmul    dword ptr +24H[eax]
                fmul    dword ptr +3cH[eax]
                faddp   st(2),st
                fmul    dword ptr +34H[eax]
                fmul    dword ptr +2cH[eax]
                fsubp   st(1),st
                fld     dword ptr +8H[edi]
                fmul    dword ptr +14H[eax]
                fld     st(0)
                fmul    dword ptr +20H[eax]
                fmul    dword ptr +3cH[eax]
                fsubp   st(2),st
                fmul    dword ptr +30H[eax]
                fmul    dword ptr +2cH[eax]
                faddp   st(1),st
                fld     dword ptr +8H[edi]
                fmul    dword ptr +1cH[eax]
                fld     st(0)
                fmul    dword ptr +20H[eax]
                fmul    dword ptr +34H[eax]
                faddp   st(2),st
                fmul    dword ptr +30H[eax]
                fmul    dword ptr +24H[eax]
                fsubp   st(1),st
                fld     dword ptr +0cH[edi]
                fmul    dword ptr +10H[eax]
                fld     st(0)
                fmul    dword ptr +24H[eax]
                fmul    dword ptr +38H[eax]
                fsubp   st(2),st
                fmul    dword ptr +34H[eax]
                fmul    dword ptr +28H[eax]
                faddp   st(1),st
                fld     dword ptr +0cH[edi]
                fmul    dword ptr +14H[eax]
                fld     st(0)
                fmul    dword ptr +20H[eax]
                fmul    dword ptr +38H[eax]
                faddp   st(2),st
                fmul    dword ptr +30H[eax]
                fmul    dword ptr +28H[eax]
                fsubp   st(1),st
                fld     dword ptr +0cH[edi]
                fmul    dword ptr +18H[eax]
                fld     st(0)
                fmul    dword ptr +20H[eax]
                fmul    dword ptr +34H[eax]
                fsubp   st(2),st
                fmul    dword ptr +30H[eax]
                fmul    dword ptr +24H[eax]
                faddp   st(1),st
                fstp    dword ptr +40H[esp]
                fld1    
                fdiv    dword ptr +40H[esp]
                fld     dword ptr +14H[edi]
                fmul    dword ptr +28H[eax]
                fmul    dword ptr +3cH[eax]
                fld     dword ptr +14H[edi]
                fmul    dword ptr +38H[eax]
                fmul    dword ptr +2cH[eax]
                fsubp   st(1),st
                fld     dword ptr +18H[edi]
                fmul    dword ptr +24H[eax]
                fmul    dword ptr +3cH[eax]
                fsubp   st(1),st
                fld     dword ptr +18H[edi]
                fmul    dword ptr +34H[eax]
                fmul    dword ptr +2cH[eax]
                faddp   st(1),st
                fld     dword ptr +1cH[edi]
                fmul    dword ptr +24H[eax]
                fmul    dword ptr +38H[eax]
                faddp   st(1),st
                fld     dword ptr +1cH[edi]
                fmul    dword ptr +34H[eax]
                fmul    dword ptr +28H[eax]
                fsubp   st(1),st
                fxch    st(1)
                fstp    dword ptr +44H[esp]
                fmul    dword ptr +44H[esp]
                fstp    dword ptr [esp]
                fld     dword ptr +4H[eax]
                fchs    
                fmul    dword ptr +28H[edi]
                fmul    dword ptr +3cH[edi]
                fld     dword ptr +4H[edi]
                fmul    dword ptr +38H[edi]
                fmul    dword ptr +2cH[edi]
                faddp   st(1),st
                fld     dword ptr +8H[edi]
                fmul    dword ptr +24H[edi]
                fmul    dword ptr +3cH[edi]
                faddp   st(1),st
                fld     dword ptr +8H[edi]
                fmul    dword ptr +34H[edi]
                fmul    dword ptr +2cH[edi]
                fsubp   st(1),st
                fld     dword ptr +0cH[edi]
                fmul    dword ptr +24H[edi]
                fmul    dword ptr +38H[edi]
                fsubp   st(1),st
                fld     dword ptr +0cH[edi]
                fmul    dword ptr +34H[edi]
                fmul    dword ptr +28H[edi]
                faddp   st(1),st
                fmul    dword ptr +44H[esp]
                fstp    dword ptr +4H[esp]
                fld     dword ptr +4H[edi]
                fmul    dword ptr +18H[edi]
                fmul    dword ptr +3cH[edi]
                fld     dword ptr +4H[edi]
                fmul    dword ptr +38H[edi]
                fmul    dword ptr +1cH[edi]
                fsubp   st(1),st
                fld     dword ptr +8H[edi]
                fmul    dword ptr +14H[edi]
                fmul    dword ptr +3cH[edi]
                fsubp   st(1),st
                fld     dword ptr +8H[edi]
                fmul    dword ptr +34H[edi]
                fmul    dword ptr +1cH[edi]
                faddp   st(1),st
                fld     dword ptr +0cH[edi]
                fmul    dword ptr +14H[edi]
                fmul    dword ptr +38H[edi]
                faddp   st(1),st
                fld     dword ptr +0cH[edi]
                fmul    dword ptr +34H[edi]
                fmul    dword ptr +18H[edi]
                fsubp   st(1),st
                fmul    dword ptr +44H[esp]
                fstp    dword ptr +8H[esp]
                fld     dword ptr +4H[edi]
                fchs    
                fmul    dword ptr +18H[edi]
                fmul    dword ptr +2cH[edi]
                fld     dword ptr +4H[edi]
                fmul    dword ptr +28H[edi]
                fmul    dword ptr +1cH[edi]
                faddp   st(1),st
                fld     dword ptr +8H[edi]
                fmul    dword ptr +14H[edi]
                fmul    dword ptr +2cH[edi]
                faddp   st(1),st
                fld     dword ptr +8H[edi]
                fmul    dword ptr +24H[edi]
                fmul    dword ptr +1cH[edi]
                fsubp   st(1),st
                fld     dword ptr +0cH[edi]
                fmul    dword ptr +14H[edi]
                fmul    dword ptr +28H[edi]
                fsubp   st(1),st
                fld     dword ptr +0cH[edi]
                fmul    dword ptr +24H[edi]
                fmul    dword ptr +18H[edi]
                faddp   st(1),st
                fmul    dword ptr +44H[esp]
                fstp    dword ptr +0cH[esp]
                fld     dword ptr +10H[edi]
                fchs    
                fmul    dword ptr +28H[edi]
                fmul    dword ptr +3cH[edi]
                fld     dword ptr +10H[edi]
                fmul    dword ptr +38H[edi]
                fmul    dword ptr +2cH[edi]
                faddp   st(1),st
                fld     dword ptr +18H[edi]
                fmul    dword ptr +20H[edi]
                fmul    dword ptr +3cH[edi]
                faddp   st(1),st
                fld     dword ptr +18H[edi]
                fmul    dword ptr +30H[edi]
                fmul    dword ptr +2cH[edi]
                fsubp   st(1),st
                fld     dword ptr +1cH[edi]
                fmul    dword ptr +20H[edi]
                fmul    dword ptr +38H[edi]
                fsubp   st(1),st
                fld     dword ptr +1cH[edi]
                fmul    dword ptr +30H[edi]
                fmul    dword ptr +28H[edi]
                faddp   st(1),st
                fmul    dword ptr +44H[esp]
                fstp    dword ptr +10H[esp]
                fld     dword ptr [edi]
                fmul    dword ptr +28H[edi]
                fmul    dword ptr +3cH[edi]
                fld     dword ptr [edi]
                fmul    dword ptr +38H[edi]
                fmul    dword ptr +2cH[edi]
                fsubp   st(1),st
                fld     dword ptr +8H[edi]
                fmul    dword ptr +20H[edi]
                fmul    dword ptr +3cH[edi]
                fsubp   st(1),st
                fld     dword ptr +8H[edi]
                fmul    dword ptr +30H[edi]
                fmul    dword ptr +2cH[edi]
                faddp   st(1),st
                fld     dword ptr +0cH[edi]
                fmul    dword ptr +20H[edi]
                fmul    dword ptr +38H[edi]
                faddp   st(1),st
                fld     dword ptr +0cH[edi]
                fmul    dword ptr +30H[edi]
                fmul    dword ptr +28H[edi]
                fsubp   st(1),st
                fld     dword ptr +44H[esp]
                fxch    st(1)
                fmul    st,st(1)
                fstp    dword ptr +14H[esp]
                fld     dword ptr [edi]
                fchs    
                fmul    dword ptr +18H[edi]
                fmul    dword ptr +3cH[edi]
                fld     dword ptr [edi]
                fmul    dword ptr +38H[edi]
                fmul    dword ptr +1cH[edi]
                faddp   st(1),st
                fld     dword ptr +8H[edi]
                fmul    dword ptr +10H[edi]
                fmul    dword ptr +3cH[edi]
                faddp   st(1),st
                fld     dword ptr +8H[edi]
                fmul    dword ptr +30H[edi]
                fmul    dword ptr +1cH[edi]
                fsubp   st(1),st
                fld     dword ptr +0cH[edi]
                fmul    dword ptr +10H[edi]
                fmul    dword ptr +38H[edi]
                fsubp   st(1),st
                fld     dword ptr +0cH[edi]
                fmul    dword ptr +30H[edi]
                fmul    dword ptr +18H[edi]
                faddp   st(1),st
                fmul    st,st(1)
                fstp    dword ptr +18H[esp]
                fld     dword ptr [edi]
                fmul    dword ptr +18H[edi]
                fmul    dword ptr +2cH[edi]
                fld     dword ptr [edi]
                fmul    dword ptr +28H[edi]
                fmul    dword ptr +1cH[edi]
                fsubp   st(1),st
                fld     dword ptr +8H[edi]
                fmul    dword ptr +10H[edi]
                fmul    dword ptr +2cH[edi]
                fsubp   st(1),st
                fld     dword ptr +8H[edi]
                fmul    dword ptr +20H[edi]
                fmul    dword ptr +1cH[edi]
                faddp   st(1),st
                fld     dword ptr +0cH[edi]
                fmul    dword ptr +10H[edi]
                fmul    dword ptr +28H[edi]
                faddp   st(1),st
                fld     dword ptr +0cH[edi]
                fmul    dword ptr +20H[edi]
                fmul    dword ptr +18H[edi]
                fsubp   st(1),st
                fmul    st,st(1)
                fstp    dword ptr +1cH[esp]
                fld     dword ptr +10H[edi]
                fmul    dword ptr +24H[edi]
                fmul    dword ptr +3cH[edi]
                fld     dword ptr +10H[edi]
                fmul    dword ptr +34H[edi]
                fmul    dword ptr +2cH[edi]
                fsubp   st(1),st
                fld     dword ptr +14H[edi]
                fmul    dword ptr +20H[edi]
                fmul    dword ptr +3cH[edi]
                fsubp   st(1),st
                fld     dword ptr +14H[edi]
                fmul    dword ptr +30H[edi]
                fmul    dword ptr +2cH[edi]
                faddp   st(1),st
                fld     dword ptr +1cH[edi]
                fmul    dword ptr +20H[edi]
                fmul    dword ptr +34H[edi]
                faddp   st(1),st
                fld     dword ptr +1cH[edi]
                fmul    dword ptr +30H[edi]
                fmul    dword ptr +24H[edi]
                fsubp   st(1),st
                fmul    st,st(1)
                fstp    dword ptr +20H[esp]
                fld     dword ptr [edi]
                fchs    
                fmul    dword ptr +24H[edi]
                fmul    dword ptr +3cH[edi]
                fld     dword ptr [edi]
                fmul    dword ptr +34H[edi]
                fmul    dword ptr +2cH[edi]
                faddp   st(1),st
                fld     dword ptr +4H[edi]
                fmul    dword ptr +20H[edi]
                fmul    dword ptr +3cH[edi]
                faddp   st(1),st
                fld     dword ptr +4H[edi]
                fmul    dword ptr +30H[edi]
                fmul    dword ptr +2cH[edi]
                fsubp   st(1),st
                fld     dword ptr +0cH[edi]
                fmul    dword ptr +20H[edi]
                fmul    dword ptr +34H[edi]
                fsubp   st(1),st
                fld     dword ptr +0cH[edi]
                fmul    dword ptr +30H[edi]
                fmul    dword ptr +24H[edi]
                faddp   st(1),st
                fmulp   st(1),st
                fstp    dword ptr +24H[esp]
                fld     dword ptr [edi]
                fmul    dword ptr +14H[edi]
                fmul    dword ptr +3cH[edi]
                fld     dword ptr [edi]
                fmul    dword ptr +34H[edi]
                fmul    dword ptr +1cH[edi]
                fsubp   st(1),st
                fld     dword ptr +4H[edi]
                fmul    dword ptr +10H[edi]
                fmul    dword ptr +3cH[edi]
                fsubp   st(1),st
                fld     dword ptr +4H[edi]
                fmul    dword ptr +30H[edi]
                fmul    dword ptr +1cH[edi]
                faddp   st(1),st
                fld     dword ptr +0cH[edi]
                fmul    dword ptr +10H[edi]
                fmul    dword ptr +34H[edi]
                faddp   st(1),st
                fld     dword ptr +0cH[edi]
                fmul    dword ptr +30H[edi]
                fmul    dword ptr +14H[edi]
                fsubp   st(1),st
                fld     dword ptr +44H[esp]
                fxch    st(1)
                fmul    st,st(1)
                fstp    dword ptr +28H[esp]
                fld     dword ptr [edi]
                fchs    
                fmul    dword ptr +14H[edi]
                fmul    dword ptr +2cH[edi]
                fld     dword ptr [edi]
                fmul    dword ptr +24H[edi]
                fmul    dword ptr +1cH[edi]
                faddp   st(1),st
                fld     dword ptr +4H[edi]
                fmul    dword ptr +10H[edi]
                fmul    dword ptr +2cH[edi]
                faddp   st(1),st
                fld     dword ptr +4H[edi]
                fmul    dword ptr +20H[edi]
                fmul    dword ptr +1cH[edi]
                fsubp   st(1),st
                fld     dword ptr +0cH[edi]
                fmul    dword ptr +10H[edi]
                fmul    dword ptr +24H[edi]
                fsubp   st(1),st
                fld     dword ptr +0cH[edi]
                fmul    dword ptr +20H[edi]
                fmul    dword ptr +14H[edi]
                faddp   st(1),st
                fmul    st,st(1)
                fstp    dword ptr +2cH[esp]
                fld     dword ptr +10H[edi]
                fchs    
                fmul    dword ptr +24H[edi]
                fmul    dword ptr +38H[edi]
                fld     dword ptr +10H[edi]
                fmul    dword ptr +34H[edi]
                fmul    dword ptr +28H[edi]
                faddp   st(1),st
                fld     dword ptr +14H[edi]
                fmul    dword ptr +20H[edi]
                fmul    dword ptr +38H[edi]
                faddp   st(1),st
                fld     dword ptr +14H[edi]
                fmul    dword ptr +30H[edi]
                fmul    dword ptr +28H[edi]
                fsubp   st(1),st
                fld     dword ptr +18H[edi]
                fmul    dword ptr +20H[edi]
                fmul    dword ptr +34H[edi]
                fsubp   st(1),st
                fld     dword ptr +18H[edi]
                fmul    dword ptr +30H[edi]
                fmul    dword ptr +24H[edi]
                faddp   st(1),st
                fmul    st,st(1)
                fstp    dword ptr +30H[esp]
                fld     dword ptr [edi]
                fmul    dword ptr +24H[edi]
                fmul    dword ptr +38H[edi]
                fld     dword ptr [edi]
                fmul    dword ptr +34H[edi]
                fmul    dword ptr +28H[edi]
                fsubp   st(1),st
                fld     dword ptr +4H[edi]
                fmul    dword ptr +20H[edi]
                fmul    dword ptr +38H[edi]
                fsubp   st(1),st
                fld     dword ptr +4H[edi]
                fmul    dword ptr +30H[edi]
                fmul    dword ptr +28H[edi]
                faddp   st(1),st
                fld     dword ptr +8H[edi]
                fmul    dword ptr +20H[edi]
                fmul    dword ptr +34H[edi]
                faddp   st(1),st
                fld     dword ptr +8H[edi]
                fmul    dword ptr +30H[edi]
                fmul    dword ptr +24H[edi]
                fsubp   st(1),st
                fmul    st,st(1)
                fstp    dword ptr +34H[esp]
                fld     dword ptr [edi]
                fchs    
                fmul    dword ptr +14H[edi]
                fmul    dword ptr +38H[edi]
                fld     dword ptr [edi]
                fmul    dword ptr +34H[edi]
                fmul    dword ptr +18H[edi]
                faddp   st(1),st
                fld     dword ptr +4H[edi]
                fmul    dword ptr +10H[edi]
                fmul    dword ptr +38H[edi]
                faddp   st(1),st
                fld     dword ptr +4H[edi]
                fmul    dword ptr +30H[edi]
                fmul    dword ptr +18H[edi]
                fsubp   st(1),st
                fld     dword ptr +8H[edi]
                fmul    dword ptr +10H[edi]
                fmul    dword ptr +34H[edi]
                fsubp   st(1),st
                fld     dword ptr +8H[edi]
                fmul    dword ptr +30H[edi]
                fmul    dword ptr +14H[edi]
                faddp   st(1),st
                fmulp   st(1),st
                fstp    dword ptr +38H[esp]
                fld     dword ptr [edi]
                fmul    dword ptr +14H[edi]
                fmul    dword ptr +28H[edi]
                fld     dword ptr [edi]
                fmul    dword ptr +24H[edi]
                fmul    dword ptr +18H[edi]
                fsubp   st(1),st
                fld     dword ptr +4H[edi]
                fmul    dword ptr +10H[edi]
                fmul    dword ptr +28H[edi]
                fsubp   st(1),st
                fld     dword ptr +4H[edi]
                fmul    dword ptr +20H[edi]
                fmul    dword ptr +18H[edi]
                faddp   st(1),st
                fld     dword ptr +8H[edi]
                fmul    dword ptr +10H[edi]
                fmul    dword ptr +24H[edi]
                faddp   st(1),st
                fld     dword ptr +8H[edi]
                fmul    dword ptr +20H[edi]
                fmul    dword ptr +14H[edi]
                fsubp   st(1),st
                fmul    dword ptr +44H[esp]
                mov     ecx,00000040H
                mov     esi,esp
                fstp    dword ptr +3cH[esp]
                push    edi
                mov     eax,ecx
                shr     ecx,02H
                repne   movsd    
                mov     cl,al
                and     cl,03H
                repne   movsb    
                pop     edi
                add     esp,0000004cH
                pop     edi
                pop     esi
                pop     ecx
                ret     
Vector_Normalize_:
                push    ebx
                sub     esp,00000008H
                mov     ebx,eax
                fld     dword ptr +4H[edx]
                fmul    st,st(0)
                fld     dword ptr [edx]
                fmul    st,st(0)
                faddp   st(1),st
                fld     dword ptr +8H[edx]
                fmul    st,st(0)
                faddp   st(1),st
                fsqrt   
                fstp    dword ptr [esp]
                test    dword ptr [esp],7fffffffH
                je      short L7
                fld1    
                fld     dword ptr [edx]
                fxch    st(1)
                fdiv    dword ptr [esp]
                fxch    st(1)
                fmul    st,st(1)
                fstp    dword ptr [ebx]
                fld     dword ptr +4H[edx]
                fmul    st,st(1)
                fstp    dword ptr +4H[ebx]
                fmul    dword ptr +8H[edx]
                fstp    dword ptr +8H[ebx]
                add     esp,00000008H
                pop     ebx
                ret     
L7:             mov     dword ptr +4H[ebx],00000000H
                mov     dword ptr +8H[ebx],00000000H
                mov     dword ptr [ebx],00000000H
                add     esp,00000008H
                pop     ebx
                ret     
                lea     eax,+0H[eax]
Vector_Dot_:    fld     dword ptr +8H[esp]
                fmul    dword ptr +14H[esp]
                fld     dword ptr +4H[esp]
                fmul    dword ptr +10H[esp]
                faddp   st(1),st
                fld     dword ptr +0cH[esp]
                fmul    dword ptr +18H[esp]
                faddp   st(1),st
                ret     0018H
                nop     
Vector_Cross_:  fld     dword ptr +8H[esp]
                fmul    dword ptr +18H[esp]
                fld     dword ptr +0cH[esp]
                fmul    dword ptr +10H[esp]
                fld     dword ptr +4H[esp]
                fmul    dword ptr +14H[esp]
                fld     dword ptr +0cH[esp]
                fmul    dword ptr +14H[esp]
                fld     dword ptr +4H[esp]
                fmul    dword ptr +18H[esp]
                fld     dword ptr +8H[esp]
                fmul    dword ptr +10H[esp]
                fxch    st(2)
                fsubp   st(5),st
                fsubp   st(3),st
                fsubp   st(1),st
                fxch    st(1)
                fstp    dword ptr +4H[eax]
                fstp    dword ptr +8H[eax]
                fstp    dword ptr [eax]
                ret     0018H
                lea     eax,+0H[eax]
Vector_Add_:    fld     dword ptr +4H[esp]
                fld     dword ptr +8H[esp]
                fld     dword ptr +0cH[esp]
                fxch    st(2)
                fadd    dword ptr +10H[esp]
                fxch    st(1)
                fadd    dword ptr +14H[esp]
                fxch    st(2)
                fadd    dword ptr +18H[esp]
                fxch    st(2)
                fstp    dword ptr +4H[eax]
                fxch    st(1)
                fstp    dword ptr +8H[eax]
                fstp    dword ptr [eax]
                ret     0018H
                lea     eax,+0H[eax]
Vector_Sub_:    fld     dword ptr +4H[esp]
                fld     dword ptr +8H[esp]
                fld     dword ptr +0cH[esp]
                fxch    st(2)
                fsub    dword ptr +10H[esp]
                fxch    st(1)
                fsub    dword ptr +14H[esp]
                fxch    st(2)
                fsub    dword ptr +18H[esp]
                fxch    st(2)
                fstp    dword ptr +4H[eax]
                fxch    st(1)
                fstp    dword ptr +8H[eax]
                fstp    dword ptr [eax]
                ret     0018H
                lea     eax,+0H[eax]
Vector_Trans_:  push    edx
                mov     edx,eax
                mov     eax,dword ptr +14H[esp]
                fld     dword ptr +0cH[esp]
                fmul    dword ptr +4H[eax]
                fld     dword ptr +8H[esp]
                fmul    dword ptr [eax]
                faddp   st(1),st
                fld     dword ptr +10H[esp]
                fmul    dword ptr +8H[eax]
                faddp   st(1),st
                fadd    dword ptr +0cH[eax]
                fld     dword ptr +0cH[esp]
                fxch    st(1)
                fstp    dword ptr [edx]
                fmul    dword ptr +14H[eax]
                fld     dword ptr +8H[esp]
                fmul    dword ptr +10H[eax]
                faddp   st(1),st
                fld     dword ptr +10H[esp]
                fmul    dword ptr +18H[eax]
                faddp   st(1),st
                fadd    dword ptr +1cH[eax]
                fld     dword ptr +0cH[esp]
                fxch    st(1)
                fstp    dword ptr +4H[edx]
                fmul    dword ptr +24H[eax]
                fld     dword ptr +8H[esp]
                fmul    dword ptr +20H[eax]
                faddp   st(1),st
                fld     dword ptr +10H[esp]
                fmul    dword ptr +28H[eax]
                faddp   st(1),st
                fadd    dword ptr +2cH[eax]
                fstp    dword ptr +8H[edx]
                pop     edx
                ret     0010H
                mov     eax,eax
Vector_Trans3x3_:
                push    edx
                mov     edx,eax
                mov     eax,dword ptr +14H[esp]
                fld     dword ptr +0cH[esp]
                fmul    dword ptr +4H[eax]
                fld     dword ptr +8H[esp]
                fmul    dword ptr [eax]
                faddp   st(1),st
                fld     dword ptr +10H[esp]
                fmul    dword ptr +8H[eax]
                faddp   st(1),st
                fld     dword ptr +0cH[esp]
                fxch    st(1)
                fstp    dword ptr [edx]
                fmul    dword ptr +14H[eax]
                fld     dword ptr +8H[esp]
                fmul    dword ptr +10H[eax]
                faddp   st(1),st
                fld     dword ptr +10H[esp]
                fmul    dword ptr +18H[eax]
                faddp   st(1),st
                fld     dword ptr +0cH[esp]
                fxch    st(1)
                fstp    dword ptr +4H[edx]
                fmul    dword ptr +24H[eax]
                fld     dword ptr +8H[esp]
                fmul    dword ptr +20H[eax]
                faddp   st(1),st
                fld     dword ptr +10H[esp]
                fmul    dword ptr +28H[eax]
                faddp   st(1),st
                fstp    dword ptr +8H[edx]
                pop     edx
                ret     0010H
                lea     eax,+0H[eax]
Vector_inv_Trans3x3_:
                push    edx
                mov     edx,eax
                mov     eax,dword ptr +14H[esp]
                fld     dword ptr +0cH[esp]
                fmul    dword ptr +10H[eax]
                fld     dword ptr +8H[esp]
                fmul    dword ptr [eax]
                faddp   st(1),st
                fld     dword ptr +10H[esp]
                fmul    dword ptr +20H[eax]
                faddp   st(1),st
                fld     dword ptr +0cH[esp]
                fxch    st(1)
                fstp    dword ptr [edx]
                fmul    dword ptr +14H[eax]
                fld     dword ptr +8H[esp]
                fmul    dword ptr +4H[eax]
                faddp   st(1),st
                fld     dword ptr +10H[esp]
                fmul    dword ptr +24H[eax]
                faddp   st(1),st
                fld     dword ptr +0cH[esp]
                fxch    st(1)
                fstp    dword ptr +4H[edx]
                fmul    dword ptr +18H[eax]
                fld     dword ptr +8H[esp]
                fmul    dword ptr +8H[eax]
                faddp   st(1),st
                fld     dword ptr +10H[esp]
                fmul    dword ptr +28H[eax]
                faddp   st(1),st
                fstp    dword ptr +8H[edx]
                pop     edx
                ret     0010H
                lea     eax,+0H[eax]
Vector_Distance_:
                push    ebx
                push    ebp
                mov     ebp,esp
                sub     esp,00000014H
                and     esp,0fffffff8H
                fld     dword ptr [edx]
                fsub    dword ptr [eax]
                fmul    st(0),st
                fld     dword ptr +4H[edx]
                fsub    dword ptr +4H[eax]
                fmul    st(0),st
                fld     dword ptr +8H[edx]
                fxch    st(1)
                faddp   st(2),st
                fsub    dword ptr +8H[eax]
                fmul    st(0),st
                faddp   st(1),st
                fstp    dword ptr +0cH[esp]
                test    dword ptr +0cH[esp],7fffffffH
                je      short L8
                fld     dword ptr +0cH[esp]
                fsqrt   
                fstp    dword ptr +10H[esp]
                jmp     short L9
L8:             xor     ebx,ebx
                mov     dword ptr +10H[esp],ebx
L9:             fld     dword ptr +10H[esp]
                mov     esp,ebp
                pop     ebp
                pop     ebx
                ret     
                nop     
Line_:          push    esi
                push    edi
                push    ebp
                sub     esp,00000014H
                mov     esi,ecx
                cmp     eax,ebx
                jge     short L10
                mov     ecx,00000001H
                sub     ebx,eax
                mov     dword ptr [esp],ecx
                mov     dword ptr +8H[esp],ebx
                jmp     short L11
L10:            mov     ecx,eax
                sub     ecx,ebx
                mov     ebx,0ffffffffH
                mov     dword ptr +8H[esp],ecx
                mov     dword ptr [esp],ebx
L11:            cmp     edx,esi
                jge     short L12
                mov     edi,00000001H
                sub     esi,edx
                mov     dword ptr +4H[esp],edi
                mov     dword ptr +0cH[esp],esi
                jmp     short L13
L12:            mov     ebx,edx
                sub     ebx,esi
                mov     esi,0ffffffffH
                mov     dword ptr +0cH[esp],ebx
                mov     dword ptr +4H[esp],esi
L13:            mov     ebx,dword ptr +8H[esp]
                mov     ebp,dword ptr +0cH[esp]
                cmp     ebx,ebp
                jg      short L14
                mov     ebx,ebp
L14:            mov     esi,ebx
                xor     ecx,ecx
                sar     esi,1
                mov     dword ptr +10H[esp],ecx
                mov     edi,esi
                test    ebx,ebx
                jle     near ptr L19
L15:            cmp     eax,dword ptr _MinX
                jl      short L16
                cmp     eax,dword ptr _MaxX
                jg      short L16
                cmp     edx,dword ptr _MinY
                jl      short L16
                cmp     edx,dword ptr _MaxY
                jg      short L16
                mov     ecx,dword ptr _ScreenWidth
                imul    ecx,edx
                mov     ebp,dword ptr +28H[esp]
                add     ecx,eax
                add     ebp,ecx
                mov     cl,byte ptr +24H[esp]
                mov     byte ptr +0H[ebp],cl
L16:            add     edi,dword ptr +8H[esp]
                cmp     edi,ebx
                jl      short L17
                mov     ecx,dword ptr [esp]
                sub     edi,ebx
                add     eax,ecx
L17:            add     esi,dword ptr +0cH[esp]
                cmp     esi,ebx
                jl      short L18
                mov     ecx,dword ptr +4H[esp]
                sub     esi,ebx
                add     edx,ecx
L18:            mov     ebp,dword ptr +10H[esp]
                inc     ebp
                mov     dword ptr +10H[esp],ebp
                cmp     ebx,ebp
                jg      short L15
L19:            add     esp,00000014H
                pop     ebp
                pop     edi
                pop     esi
                ret     0008H
                lea     eax,+0H[eax]
DrawChar_:      push    esi
                push    edi
                push    ebp
                sub     esp,00000008H
                mov     edi,dword ptr _ScreenWidth
                mov     dword ptr [esp],eax
                mov     ebp,edx
                mov     dl,bl
                cmp     bl,20H
                je      near ptr L32
                mov     esi,offset DGROUP:_s
L20:            mov     al,byte ptr [esi]
                cmp     al,dl
                je      short L22
                cmp     al,00H
                je      short L21
                inc     esi
                mov     al,byte ptr [esi]
                cmp     al,dl
                je      short L22
                inc     esi
                cmp     al,00H
                jne     short L20
L21:            sub     esi,esi
L22:            mov     edi,dword ptr _ScreenWidth
                imul    ebp,edi
                mov     eax,esi
                sub     eax,offset DGROUP:_s
                mov     byte ptr +4H[esp],al
                mov     eax,dword ptr [esp]
                mov     edx,dword ptr +18H[esp]
                add     eax,ebp
                xor     ebx,ebx
                add     eax,edx
L23:            xor     edx,edx
                mov     dl,byte ptr +4H[esp]
                mov     dl,byte ptr _FontData[ebx+edx*8]
                test    dl,80H
                je      short L24
                mov     byte ptr [eax],cl
L24:            test    dl,40H
                je      short L25
                mov     byte ptr +1H[eax],cl
L25:            test    dl,20H
                je      short L26
                mov     byte ptr +2H[eax],cl
L26:            test    dl,10H
                je      short L27
                mov     byte ptr +3H[eax],cl
L27:            test    dl,08H
                je      short L28
                mov     byte ptr +4H[eax],cl
L28:            test    dl,04H
                je      short L29
                mov     byte ptr +5H[eax],cl
L29:            test    dl,02H
                je      short L30
                mov     byte ptr +6H[eax],cl
L30:            test    dl,01H
                je      short L31
                mov     byte ptr +7H[eax],cl
L31:            inc     ebx
                add     eax,edi
                cmp     ebx,00000008H
                jl      short L23
L32:            mov     dword ptr _ScreenWidth,edi
                add     esp,00000008H
                pop     ebp
                pop     edi
                pop     esi
                ret     0004H
                lea     eax,+0H[eax]
DrawString_:    push    esi
                push    edi
                push    ebp
                sub     esp,00000004H
                mov     esi,eax
                mov     ebp,edx
                mov     word ptr [esp],cx
                mov     ah,byte ptr [ebx]
                mov     edi,ebx
                test    ah,ah
                je      short L34
L33:            mov     edx,dword ptr +14H[esp]
                mov     ecx,dword ptr -2H[esp]
                mov     eax,esi
                xor     ebx,ebx
                push    edx
                sar     ecx,10H
                mov     bl,byte ptr [edi]
                mov     edx,ebp
                inc     edi
                call    near ptr DrawChar_
                mov     dl,byte ptr [edi]
                add     esi,00000008H
                test    dl,dl
                jne     short L33
L34:            add     esp,00000004H
                pop     ebp
                pop     edi
                pop     esi
                ret     0004H
                mov     eax,eax
SetPalette_:    push    ebx
                push    ecx
                push    edx
                sub     esp,00000004H
                mov     ecx,eax
                xor     edx,edx
                mov     dword ptr [esp],edx
L35:            mov     ah,byte ptr [ecx]
                mov     bh,byte ptr +2H[ecx]
                mov     al,byte ptr [esp]
                mov     bl,byte ptr +1H[ecx]
                mov     dx,03c8H
                out     dx,al
                inc     dx
                mov     al,ah
                out     dx,al
                mov     al,bl
                out     dx,al
                mov     al,bh
                out     dx,al
                mov     ebx,dword ptr [esp]
                inc     ebx
                add     ecx,00000003H
                mov     dword ptr [esp],ebx
                cmp     ebx,00000100H
                jl      short L35
                add     esp,00000004H
                pop     edx
                pop     ecx
                pop     ebx
                ret     
                lea     eax,+0H[eax]
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
                fadd    qword ptr L172
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
                fadd    qword ptr L172
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
                fadd    qword ptr L172
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
                je      short L36
                inc     eax
L36:            mov     dword ptr +14H[esp],eax
                sub     esp,00000004H
                mov     eax,dword ptr +10H[esi]
                mov     dword ptr [esp],eax
                call    near ptr float2fixed_
                mov     edx,eax
                sar     edx,10H
                test    ax,0ffffH
                je      short L37
                inc     edx
L37:            mov     edi,dword ptr +14H[esp]
                mov     ecx,edx
                cmp     edx,edi
                je      near ptr L43
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
                fld     qword ptr L173
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
                jg      near ptr L43
                test    edx,edx
                jl      near ptr L43
                test    ebp,ebp
                jge     short L38
                mov     edx,ebp
                neg     edx
                imul    edi,edx
                xor     ebx,ebx
                mov     dword ptr +14H[esp],ebx
                add     eax,edi
L38:            cmp     ecx,000000c7H
                jle     short L39
                mov     ecx,000000c7H
L39:            mov     edx,dword ptr +14H[esp]
                cmp     edx,dword ptr __y1
                jge     short L40
                mov     dword ptr __y1,edx
L40:            cmp     ecx,dword ptr __y2
                jle     short L41
                mov     dword ptr __y2,ecx
L41:            mov     ebx,dword ptr +14H[esp]
                lea     edx,+0H[ebx*4]
                add     edx,ebx
                mov     ebx,offset DGROUP:_Edge
                shl     edx,02H
                add     ebx,edx
                mov     edx,dword ptr +14H[esp]
                cmp     ecx,edx
                jle     short L43
L42:            mov     edi,eax
                add     ebx,00000014H
                inc     edx
                sar     edi,10H
                add     eax,esi
                mov     dword ptr -10H[ebx],edi
                cmp     edx,ecx
                jl      short L42
L43:            add     esp,00000018H
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
                sub     esp,00000064H
                mov     ebx,eax
                mov     esi,edx
                sub     esp,00000004H
                mov     edx,dword ptr +10H[eax]
                mov     dword ptr [esp],edx
                call    near ptr float2fixed_
                mov     edx,eax
                sar     edx,10H
                test    ax,0ffffH
                je      short L44
                inc     edx
L44:            mov     dword ptr +54H[esp],edx
                sub     esp,00000004H
                mov     eax,dword ptr +10H[esi]
                mov     dword ptr [esp],eax
                call    near ptr float2fixed_
                mov     edx,eax
                sar     eax,10H
                test    dx,0ffffH
                je      short L45
                inc     eax
L45:            mov     edx,dword ptr +54H[esp]
                mov     edi,eax
                cmp     eax,edx
                je      near ptr L49
                fld     dword ptr +10H[esi]
                fsub    dword ptr +10H[ebx]
                fadd    qword ptr L174
                fstp    qword ptr +10H[esp]
                mov     edx,dword ptr +10H[esp]
                mov     ecx,dword ptr __DivTable
                add     edx,80000000H
                fld     dword ptr +0cH[esi]
                mov     edx,dword ptr [ecx+edx*4]
                fsub    dword ptr +0cH[ebx]
                mov     dword ptr +3cH[esp],edx
                fmul    dword ptr +3cH[esp]
                fld     dword ptr +34H[esi]
                fld     dword ptr +30H[esi]
                fsub    dword ptr +30H[ebx]
                fxch    st(1)
                fsub    dword ptr +34H[ebx]
                fxch    st(1)
                fmul    dword ptr +3cH[esp]
                fxch    st(1)
                fmul    dword ptr +3cH[esp]
                fild    dword ptr +54H[esp]
                fsub    dword ptr +10H[ebx]
                fld     st(3)
                fmul    st,st(1)
                fld     st(3)
                fmul    st,st(2)
                fld     st(3)
                fmulp   st(3),st
                fxch    st(5)
                fld     qword ptr L175
                fxch    st(1)
                fadd    st,st(1)
                fxch    st(5)
                fadd    st,st(1)
                fxch    st(4)
                fadd    st,st(1)
                fxch    st(5)
                fstp    qword ptr +18H[esp]
                fxch    st(3)
                fstp    qword ptr +8H[esp]
                fxch    st(3)
                fstp    qword ptr +28H[esp]
                mov     edx,dword ptr +18H[esp]
                mov     ebp,dword ptr +8H[esp]
                mov     esi,dword ptr +28H[esp]
                fxch    st(2)
                fadd    dword ptr +0cH[ebx]
                fxch    st(3)
                fadd    dword ptr +30H[ebx]
                fxch    st(2)
                fadd    dword ptr +34H[ebx]
                add     edx,80000000H
                add     ebp,80000000H
                add     esi,80000000H
                fxch    st(3)
                fadd    st,st(1)
                fxch    st(2)
                fadd    st,st(1)
                fxch    st(3)
                faddp   st(1),st
                mov     dword ptr +4cH[esp],edx
                mov     dword ptr +5cH[esp],edx
                fxch    st(1)
                fstp    qword ptr [esp]
                mov     dword ptr +60H[esp],ebp
                fxch    st(1)
                fstp    qword ptr +20H[esp]
                mov     dword ptr +50H[esp],esi
                mov     dword ptr +58H[esp],esi
                fstp    qword ptr +30H[esp]
                mov     ecx,dword ptr [esp]
                mov     edx,dword ptr +20H[esp]
                mov     ebx,dword ptr +30H[esp]
                mov     esi,dword ptr +54H[esp]
                add     ecx,80008000H
                add     edx,80008000H
                add     ebx,80008000H
                cmp     esi,000000c7H
                jg      near ptr L49
                test    eax,eax
                jl      near ptr L49
                test    esi,esi
                jge     short L46
                mov     eax,esi
                mov     esi,dword ptr +4cH[esp]
                neg     eax
                imul    esi,eax
                imul    ebp,eax
                add     edx,ebp
                imul    eax,dword ptr +50H[esp]
                add     ebx,eax
                xor     eax,eax
                add     ecx,esi
                mov     dword ptr +54H[esp],eax
L46:            cmp     edi,000000c7H
                jle     short L47
                mov     edi,000000c7H
L47:            mov     esi,dword ptr +54H[esp]
                lea     eax,+0H[esi*4]
                add     eax,esi
                shl     eax,02H
                add     eax,offset DGROUP:_Edge
                cmp     edi,esi
                jle     short L49
L48:            mov     ebp,ecx
                sar     ebp,10H
                inc     esi
                mov     dword ptr [eax],ebp
                mov     ebp,dword ptr +5cH[esp]
                mov     dword ptr +8H[eax],edx
                add     ecx,ebp
                mov     ebp,dword ptr +60H[esp]
                mov     dword ptr +0cH[eax],ebx
                add     edx,ebp
                mov     ebp,dword ptr +58H[esp]
                add     eax,00000014H
                add     ebx,ebp
                cmp     esi,edi
                jl      short L48
L49:            add     esp,00000064H
                pop     ebp
                pop     edi
                pop     esi
                pop     ecx
                pop     ebx
                ret     
                lea     eax,+0H[eax]
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
                je      short L50
                inc     edx
L50:            mov     dword ptr +64H[esp],edx
                sub     esp,00000004H
                mov     eax,dword ptr +10H[edi]
                mov     dword ptr [esp],eax
                call    near ptr float2fixed_
                mov     edx,eax
                sar     eax,10H
                test    dx,0ffffH
                je      short L51
                inc     eax
L51:            mov     edx,dword ptr +64H[esp]
                mov     dword ptr +78H[esp],eax
                cmp     eax,edx
                je      near ptr L55
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
                fld     qword ptr L176
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
                jg      near ptr L55
                test    eax,eax
                jl      near ptr L55
                test    edi,edi
                jge     short L52
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
L52:            cmp     dword ptr +78H[esp],000000c7H
                jle     short L53
                mov     dword ptr +78H[esp],000000c7H
L53:            mov     edi,dword ptr +64H[esp]
                lea     eax,+0H[edi*4]
                add     eax,edi
                shl     eax,02H
                add     eax,offset DGROUP:_Edge
                cmp     edi,dword ptr +78H[esp]
                jge     short L55
L54:            mov     ebp,ebx
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
                jl      short L54
L55:            add     esp,0000007cH
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
                je      near ptr L61
                cmp     eax,000000c7H
                jg      near ptr L61
                test    edx,edx
                jl      near ptr L61
                cmp     eax,edx
                jge     near ptr L61
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
L56:            mov     ecx,dword ptr +8H[esp]
                mov     eax,dword ptr +8H[esp]
                mov     ebx,dword ptr +8H[esp]
                mov     edx,dword ptr +8H[esp]
                mov     ecx,dword ptr _Edge+4H[ecx]
                mov     eax,dword ptr _Edge+8H[eax]
                mov     ebx,dword ptr _Edge[ebx]
                mov     edx,dword ptr _Edge+0cH[edx]
                cmp     ebx,0000013fH
                jg      near ptr L60
                test    ecx,ecx
                jl      near ptr L60
                cmp     ecx,0000013fH
                jle     short L57
                mov     ecx,0000013fH
L57:            test    ebx,ebx
                jge     short L58
                mov     esi,dword ptr +0cH[esp]
                neg     ebx
                imul    esi,ebx
                add     eax,esi
                imul    ebx,dword ptr +10H[esp]
                add     edx,ebx
                xor     ebx,ebx
L58:            sub     ecx,ebx
                mov     dword ptr +14H[esp],ecx
                test    ecx,ecx
                jle     short L60
                mov     ecx,dword ptr +4H[esp]
                mov     ebp,dword ptr +14H[esp]
                add     ecx,ebx
                mov     ebx,dword ptr __Buffer
                xor     esi,esi
                add     ebx,ecx
                test    ebp,ebp
                jle     short L60
                mov     ebp,dword ptr +14H[esp]
L59:            mov     ecx,edx
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
                jl      short L59
L60:            mov     edx,dword ptr +8H[esp]
                mov     ebx,dword ptr +4H[esp]
                mov     ecx,dword ptr [esp]
                add     edx,00000014H
                add     ebx,00000140H
                mov     dword ptr +8H[esp],edx
                mov     dword ptr +4H[esp],ebx
                cmp     ebx,ecx
                jl      near ptr L56
L61:            add     esp,00000018H
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
                je      near ptr L67
                cmp     eax,000000c7H
                jg      near ptr L67
                test    edx,edx
                jl      near ptr L67
                cmp     eax,edx
                jge     near ptr L67
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
L62:            mov     esi,dword ptr +0cH[esp]
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
                jg      near ptr L66
                test    esi,esi
                jl      near ptr L66
                cmp     esi,0000013fH
                jle     short L63
                mov     esi,0000013fH
L63:            test    ecx,ecx
                jge     short L64
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
L64:            sub     esi,ecx
                mov     dword ptr +18H[esp],esi
                test    esi,esi
                jle     near ptr L66
                mov     esi,dword ptr +8H[esp]
                mov     ebp,dword ptr +18H[esp]
                add     ecx,esi
                mov     esi,dword ptr __Buffer
                xor     edi,edi
                add     esi,ecx
                test    ebp,ebp
                jle     short L66
L65:            mov     ecx,edx
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
                jl      short L65
L66:            mov     edx,dword ptr +0cH[esp]
                mov     ebx,dword ptr +8H[esp]
                mov     ecx,dword ptr +4H[esp]
                add     edx,00000014H
                add     ebx,00000140H
                mov     dword ptr +0cH[esp],edx
                mov     dword ptr +8H[esp],ebx
                cmp     ebx,ecx
                jl      near ptr L62
L67:            add     esp,0000001cH
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
                jae     short L68
                mov     edx,00000001H
L68:            mov     eax,dword ptr +8H[esp]
                mov     ebx,dword ptr [esp+edx*4]
                fld     dword ptr +10H[eax]
                fcomp   dword ptr +10H[ebx]
                fstsw   ax
                sahf    
                jae     short L69
                mov     edx,00000002H
L69:            lea     ebx,+1H[edx]
                cmp     ebx,00000002H
                jle     short L70
                xor     ebx,ebx
L70:            lea     eax,-1H[edx]
                test    eax,eax
                jge     short L71
                mov     eax,00000002H
L71:            lea     edi,+0H[eax*4]
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
                jae     short L72
                mov     edx,ebx
                mov     eax,ecx
                call    near ptr ScanRight_
                jmp     short L73
L72:            mov     edx,ecx
                mov     eax,ebx
                call    near ptr TexScanleft_
L73:            mov     eax,dword ptr +28H[esi]
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
                fadd    qword ptr L177
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
                jle     short L74
                mov     eax,dword ptr +4H[esp]
                cmp     dword ptr +10H[eax],43470000H
                jle     short L74
                mov     eax,dword ptr +8H[esp]
                cmp     dword ptr +10H[eax],43470000H
                jg      near ptr L82
L74:            mov     eax,dword ptr [esp]
                cmp     dword ptr +0cH[eax],439f8000H
                jle     short L75
                mov     eax,dword ptr +4H[esp]
                cmp     dword ptr +0cH[eax],439f8000H
                jle     short L75
                mov     eax,dword ptr +8H[esp]
                cmp     dword ptr +0cH[eax],439f8000H
                jg      near ptr L82
L75:            mov     eax,dword ptr +4H[esp]
                fld     dword ptr +10H[eax]
                mov     eax,dword ptr [esp]
                xor     edx,edx
                fcomp   dword ptr +10H[eax]
                fstsw   ax
                sahf    
                jae     short L76
                mov     edx,00000001H
L76:            mov     eax,dword ptr +8H[esp]
                mov     ebx,dword ptr [esp+edx*4]
                fld     dword ptr +10H[eax]
                fcomp   dword ptr +10H[ebx]
                fstsw   ax
                sahf    
                jae     short L77
                mov     edx,00000002H
L77:            lea     ebx,+1H[edx]
                cmp     ebx,00000002H
                jle     short L78
                xor     ebx,ebx
L78:            lea     eax,-1H[edx]
                test    eax,eax
                jge     short L79
                mov     eax,00000002H
L79:            lea     edi,+0H[eax*4]
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
                jae     short L80
                mov     edx,ecx
                mov     eax,ebx
                call    near ptr ScanRight_
                jmp     short L81
L80:            mov     edx,ebx
                mov     eax,ecx
                call    near ptr GTexScanleft_
L81:            mov     eax,dword ptr +28H[esi]
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
                fadd    qword ptr L178
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
L82:            add     esp,0000002cH
                pop     ebp
                pop     edi
                pop     esi
                pop     edx
                pop     ecx
                pop     ebx
                ret     
                lea     eax,+0H[eax]
Handle_Light_:  push    ebx
                push    ecx
                push    esi
                push    edi
                push    ebp
                mov     ebx,eax
                mov     eax,dword ptr [eax]
                add     edx,00000038H
                cmp     eax,00000002H
                jb      short L83
                lea     esi,+28H[ebx]
                lea     ecx,+34H[ebx]
                jbe     short L84
                cmp     eax,00000004H
                je      short L85
                jmp     near ptr L87
L83:            cmp     eax,00000001H
                je      near ptr L86
                jmp     near ptr L87
L84:            mov     eax,ecx
                call    near ptr Matrix_Identity_
                fld     dword ptr +1cH[ebx]
                fld     dword ptr +20H[ebx]
                fld     dword ptr +24H[ebx]
                mov     eax,ecx
                fxch    st(2)
                fadd    dword ptr +40H[ebx]
                fxch    st(1)
                fadd    dword ptr +50H[ebx]
                fxch    st(2)
                fadd    dword ptr +60H[ebx]
                fxch    st(1)
                fstp    dword ptr +40H[ebx]
                fxch    st(1)
                fstp    dword ptr +50H[ebx]
                fstp    dword ptr +60H[ebx]
                call    near ptr Matrix_Mult_
                push    ecx
                mov     ebp,dword ptr +24H[ebx]
                push    ebp
                mov     eax,dword ptr +20H[ebx]
                push    eax
                mov     edx,dword ptr +1cH[ebx]
                push    edx
                mov     eax,esi
                call    near ptr Vector_Trans_
                jmp     short L87
L85:            mov     eax,ecx
                call    near ptr Matrix_Identity_
                fld     dword ptr +1cH[ebx]
                fld     dword ptr +20H[ebx]
                fld     dword ptr +24H[ebx]
                mov     eax,ecx
                fxch    st(2)
                fadd    dword ptr +40H[ebx]
                fxch    st(1)
                fadd    dword ptr +50H[ebx]
                fxch    st(2)
                fadd    dword ptr +60H[ebx]
                fxch    st(1)
                fstp    dword ptr +40H[ebx]
                fxch    st(1)
                fstp    dword ptr +50H[ebx]
                fstp    dword ptr +60H[ebx]
                call    near ptr Matrix_Mult_
                push    ecx
                mov     edx,dword ptr +24H[ebx]
                push    edx
                mov     ecx,dword ptr +20H[ebx]
                push    ecx
                mov     edi,dword ptr +1cH[ebx]
                push    edi
                mov     eax,esi
                call    near ptr Vector_Trans_
                pop     ebp
                pop     edi
                pop     esi
                pop     ecx
                pop     ebx
                ret     
L86:            push    edx
                mov     ecx,dword ptr +0cH[ebx]
                push    ecx
                mov     esi,dword ptr +8H[ebx]
                push    esi
                mov     edi,dword ptr +4H[ebx]
                push    edi
                lea     eax,+10H[ebx]
                call    near ptr Vector_inv_Trans3x3_
L87:            pop     ebp
                pop     edi
                pop     esi
                pop     ecx
                pop     ebx
                ret     
                mov     eax,eax
CalcLight_:     push    ebx
                push    ecx
                push    esi
                push    edi
                push    ebp
                sub     esp,000000e8H
                mov     ecx,eax
                mov     dword ptr +0c8H[esp],edx
                xor     edx,edx
                mov     ebx,dword ptr +6cH[eax]
                mov     dword ptr +0b8H[esp],edx
                test    ebx,ebx
                jle     near ptr L102
                add     eax,00000028H
                mov     dword ptr +0bcH[esp],edx
                mov     dword ptr +0b4H[esp],eax
L88:            mov     edi,dword ptr +0bcH[esp]
                mov     eax,dword ptr +70H[ecx]
                mov     esi,dword ptr +0b4H[esp]
                add     eax,edi
                push    esi
                mov     ebp,dword ptr +2cH[eax]
                push    ebp
                mov     edx,dword ptr +28H[eax]
                push    edx
                mov     ebx,dword ptr +24H[eax]
                push    ebx
                lea     eax,+40H[esp]
                xor     esi,esi
                call    near ptr Vector_Trans3x3_
                mov     eax,dword ptr +0c8H[esp]
                mov     dword ptr +0e0H[esp],esi
                mov     ebp,dword ptr +0cH[eax]
                mov     dword ptr +0ccH[esp],esi
                test    ebp,ebp
                jle     near ptr L99
                mov     dword ptr +0c0H[esp],edi
                mov     dword ptr +0c4H[esp],edi
                xor     ebp,ebp
L89:            mov     ebx,dword ptr +0c8H[esp]
                mov     ebx,dword ptr +10H[ebx]
                add     ebx,ebp
                mov     esi,dword ptr [ebx]
                cmp     esi,00000002H
                jb      short L90
                jbe     near ptr L92
                cmp     esi,00000004H
                je      near ptr L94
                jmp     near ptr L98
L90:            cmp     esi,00000001H
                jne     near ptr L98
                lea     edi,+0a8H[esp]
                lea     esi,+10H[ebx]
                movsd   
                movsd   
                movsd   
                lea     edi,+54H[esp]
                lea     esi,+30H[esp]
                movsd   
                movsd   
                movsd   
                lea     edi,+9cH[esp]
                lea     esi,+0a8H[esp]
                movsd   
                movsd   
                movsd   
                fld     dword ptr +58H[esp]
                fmul    dword ptr +0a0H[esp]
                fld     dword ptr +54H[esp]
                fmul    dword ptr +9cH[esp]
                faddp   st(1),st
                fld     dword ptr +5cH[esp]
                fmul    dword ptr +0a4H[esp]
                faddp   st(1),st
                fmul    dword ptr +74H[ebx]
                fldz    
                fxch    st(1)
                fstp    dword ptr +0d0H[esp]
                fcomp   dword ptr +0d0H[esp]
                fstsw   ax
                sahf    
                jbe     short L91
                xor     esi,esi
                mov     dword ptr +0d0H[esp],esi
L91:            fld     dword ptr +0e0H[esp]
                fadd    dword ptr +0d0H[esp]
                jmp     near ptr L97
L92:            lea     edi,+3cH[esp]
                lea     esi,+28H[ebx]
                movsd   
                movsd   
                movsd   
                mov     edx,dword ptr +0c4H[esp]
                mov     eax,dword ptr +70H[ecx]
                add     eax,edx
                lea     edi,+0a8H[esp]
                lea     esi,+18H[eax]
                movsd   
                movsd   
                movsd   
                lea     edi,+6cH[esp]
                lea     esi,+3cH[esp]
                lea     edx,+0cH[esp]
                movsd   
                movsd   
                movsd   
                lea     eax,+24H[esp]
                fld     dword ptr +6cH[esp]
                lea     edi,+60H[esp]
                lea     esi,+0a8H[esp]
                fld     dword ptr +70H[esp]
                movsd   
                movsd   
                movsd   
                fld     dword ptr +74H[esp]
                mov     edi,esp
                lea     esi,+30H[esp]
                fxch    st(2)
                fsub    dword ptr +60H[esp]
                fxch    st(1)
                fsub    dword ptr +64H[esp]
                fxch    st(2)
                fsub    dword ptr +68H[esp]
                fxch    st(1)
                fstp    dword ptr +0cH[esp]
                fxch    st(1)
                fstp    dword ptr +10H[esp]
                fstp    dword ptr +14H[esp]
                call    near ptr Vector_Normalize_
                movsd   
                movsd   
                movsd   
                lea     edi,+78H[esp]
                lea     esi,+24H[esp]
                movsd   
                movsd   
                movsd   
                fld     dword ptr +4H[esp]
                fmul    dword ptr +7cH[esp]
                fld     dword ptr [esp]
                fmul    dword ptr +78H[esp]
                faddp   st(1),st
                fld     dword ptr +8H[esp]
                fmul    dword ptr +80H[esp]
                faddp   st(1),st
                fmul    dword ptr +74H[ebx]
                fldz    
                fxch    st(1)
                fstp    dword ptr +0d4H[esp]
                fcomp   dword ptr +0d4H[esp]
                fstsw   ax
                sahf    
                jbe     short L93
                xor     ebx,ebx
                mov     dword ptr +0d4H[esp],ebx
L93:            fld     dword ptr +0e0H[esp]
                fadd    dword ptr +0d4H[esp]
                jmp     near ptr L97
L94:            lea     edi,+0a8H[esp]
                lea     esi,+28H[ebx]
                movsd   
                movsd   
                movsd   
                mov     edx,dword ptr +0c4H[esp]
                mov     eax,dword ptr +70H[ecx]
                add     eax,edx
                lea     edi,+3cH[esp]
                lea     esi,+18H[eax]
                movsd   
                movsd   
                movsd   
                lea     edi,+18H[esp]
                lea     esi,+0a8H[esp]
                lea     edx,+0cH[esp]
                movsd   
                movsd   
                movsd   
                lea     eax,+24H[esp]
                fld     dword ptr +18H[esp]
                lea     edi,+48H[esp]
                lea     esi,+3cH[esp]
                fld     dword ptr +1cH[esp]
                movsd   
                movsd   
                movsd   
                fld     dword ptr +20H[esp]
                lea     edi,+84H[esp]
                lea     esi,+30H[esp]
                fxch    st(2)
                fsub    dword ptr +48H[esp]
                fxch    st(1)
                fsub    dword ptr +4cH[esp]
                fxch    st(2)
                fsub    dword ptr +50H[esp]
                fxch    st(1)
                fstp    dword ptr +0cH[esp]
                fxch    st(1)
                fstp    dword ptr +10H[esp]
                fstp    dword ptr +14H[esp]
                call    near ptr Vector_Normalize_
                movsd   
                movsd   
                movsd   
                lea     edi,+90H[esp]
                lea     esi,+24H[esp]
                movsd   
                movsd   
                movsd   
                fld     dword ptr +88H[esp]
                fmul    dword ptr +94H[esp]
                fld     dword ptr +84H[esp]
                fmul    dword ptr +90H[esp]
                faddp   st(1),st
                fld     dword ptr +8cH[esp]
                fmul    dword ptr +98H[esp]
                faddp   st(1),st
                fmul    dword ptr +74H[ebx]
                fldz    
                fxch    st(1)
                fstp    dword ptr +0dcH[esp]
                fcomp   dword ptr +0dcH[esp]
                fstsw   ax
                sahf    
                jbe     short L95
                xor     esi,esi
                mov     dword ptr +0dcH[esp],esi
L95:            mov     edi,dword ptr +0c0H[esp]
                mov     eax,dword ptr +70H[ecx]
                add     eax,edi
                lea     edx,+18H[eax]
                lea     eax,+28H[ebx]
                call    near ptr Vector_Distance_
                fstp    dword ptr +0d8H[esp]
                fld     dword ptr +78H[ebx]
                fsub    dword ptr +0d8H[esp]
                fldz    
                fxch    st(1)
                fstp    dword ptr +0d8H[esp]
                fcomp   dword ptr +0d8H[esp]
                fstsw   ax
                sahf    
                jae     short L96
                fld     dword ptr +0d8H[esp]
                fdiv    dword ptr +78H[ebx]
                fmul    dword ptr +0dcH[esp]
                fstp    dword ptr +0dcH[esp]
L96:            fld     dword ptr +0e0H[esp]
                fadd    dword ptr +0dcH[esp]
L97:            fstp    dword ptr +0e0H[esp]
L98:            mov     eax,dword ptr +0ccH[esp]
                mov     edx,dword ptr +0c8H[esp]
                add     ebp,00000084H
                inc     eax
                mov     ebx,dword ptr +0cH[edx]
                mov     dword ptr +0ccH[esp],eax
                cmp     eax,ebx
                jl      near ptr L89
L99:            fld     dword ptr +0e0H[esp]
                fmul    dword ptr L179
                fstp    dword ptr +0e0H[esp]
                cmp     dword ptr +0e0H[esp],42fe0000H
                jle     short L100
                mov     dword ptr +0e0H[esp],42fe0000H
L100:           fldz    
                fcomp   dword ptr +0e0H[esp]
                fstsw   ax
                sahf    
                jbe     short L101
                xor     ebp,ebp
                mov     dword ptr +0e0H[esp],ebp
L101:           push    dword ptr +0e0H[esp]
                call    near ptr float2int_
                mov     ebp,dword ptr +0b8H[esp]
                movsx   ax,byte ptr _PhongIntensity[eax]
                mov     esi,dword ptr +0bcH[esp]
                mov     dword ptr +0e4H[esp],eax
                mov     eax,dword ptr +70H[ecx]
                inc     ebp
                add     eax,esi
                fild    word ptr +0e4H[esp]
                mov     dword ptr +0b8H[esp],ebp
                fstp    dword ptr +38H[eax]
                lea     edi,+3cH[esi]
                mov     edx,dword ptr +6cH[ecx]
                mov     dword ptr +0bcH[esp],edi
                cmp     ebp,edx
                jl      near ptr L88
L102:           add     esp,000000e8H
                pop     ebp
                pop     edi
                pop     esi
                pop     ecx
                pop     ebx
                ret     
                mov     eax,eax
Camera_CreateMatrix_Free_:
                push    ebx
                push    edx
                sub     esp,00000040H
                mov     edx,eax
                sub     esp,00000004H
                fld     dword ptr +0cH[eax]
                fchs    
                fstp    dword ptr [esp]
                sub     esp,00000004H
                fld     dword ptr +8H[edx]
                fchs    
                fstp    dword ptr [esp]
                lea     ebx,+38H[edx]
                sub     esp,00000004H
                fld     dword ptr +4H[edx]
                fchs    
                mov     eax,ebx
                fstp    dword ptr [esp]
                call    near ptr Matrix_Translation_
                sub     esp,00000004H
                fld     dword ptr +18H[edx]
                fchs    
                fstp    dword ptr [esp]
                sub     esp,00000004H
                fld     dword ptr +14H[edx]
                fchs    
                fstp    dword ptr [esp]
                sub     esp,00000004H
                fld     dword ptr +10H[edx]
                fchs    
                fstp    dword ptr [esp]
                lea     eax,+0cH[esp]
                lea     edx,+0cH[esp]
                call    near ptr Matrix_Rotation_
                mov     eax,ebx
                call    near ptr Matrix_Mult_
                add     esp,00000040H
                pop     edx
                pop     ebx
                ret     
                lea     eax,+0H[eax]
Camera_CreateMatrix_Aim_:
                push    ebx
                push    edx
                push    esi
                push    edi
                sub     esp,000000dcH
                mov     edx,eax
                sub     esp,00000004H
                fld     dword ptr +0cH[eax]
                fchs    
                fstp    dword ptr [esp]
                sub     esp,00000004H
                fld     dword ptr +8H[edx]
                fchs    
                fstp    dword ptr [esp]
                lea     ebx,+38H[edx]
                sub     esp,00000004H
                fld     dword ptr +4H[edx]
                fchs    
                fstp    dword ptr [esp]
                mov     eax,ebx
                lea     edi,+70H[esp]
                call    near ptr Matrix_Translation_
                lea     esi,+28H[edx]
                movsd   
                movsd   
                movsd   
                lea     edi,+70H[esp]
                lea     esi,+1cH[edx]
                movsd   
                movsd   
                movsd   
                lea     edi,+58H[esp]
                lea     esi,+4H[edx]
                movsd   
                movsd   
                movsd   
                lea     edi,+0c4H[esp]
                lea     esi,+70H[esp]
                lea     eax,+40H[esp]
                movsd   
                movsd   
                movsd   
                lea     edx,+7cH[esp]
                fld     dword ptr +0c4H[esp]
                lea     edi,+4cH[esp]
                lea     esi,+58H[esp]
                fld     dword ptr +0c8H[esp]
                movsd   
                movsd   
                movsd   
                fld     dword ptr +0ccH[esp]
                lea     edi,+0d0H[esp]
                lea     esi,+40H[esp]
                fxch    st(2)
                fsub    dword ptr +4cH[esp]
                fxch    st(1)
                fsub    dword ptr +50H[esp]
                fxch    st(2)
                fsub    dword ptr +54H[esp]
                fxch    st(1)
                fstp    dword ptr +7cH[esp]
                fxch    st(1)
                fstp    dword ptr +80H[esp]
                fstp    dword ptr +84H[esp]
                call    near ptr Vector_Normalize_
                movsd   
                movsd   
                movsd   
                lea     edi,+0b8H[esp]
                lea     esi,+64H[esp]
                movsd   
                movsd   
                movsd   
                fld     dword ptr +0d4H[esp]
                fmul    dword ptr +0c0H[esp]
                fld     dword ptr +0d8H[esp]
                fmul    dword ptr +0b8H[esp]
                fld     dword ptr +0d0H[esp]
                fmul    dword ptr +0bcH[esp]
                fld     dword ptr +0d8H[esp]
                fmul    dword ptr +0bcH[esp]
                fld     dword ptr +0d0H[esp]
                fmul    dword ptr +0c0H[esp]
                lea     edi,+94H[esp]
                lea     esi,+40H[esp]
                fld     dword ptr +0d4H[esp]
                movsd   
                movsd   
                movsd   
                fmul    dword ptr +0b8H[esp]
                lea     edi,+0a0H[esp]
                lea     esi,+88H[esp]
                fxch    st(2)
                fsubp   st(5),st
                fsubp   st(3),st
                fsubp   st(1),st
                fxch    st(2)
                fstp    dword ptr +88H[esp]
                fstp    dword ptr +8cH[esp]
                fstp    dword ptr +90H[esp]
                movsd   
                movsd   
                movsd   
                fld     dword ptr +98H[esp]
                fmul    dword ptr +0a8H[esp]
                fld     dword ptr +9cH[esp]
                fmul    dword ptr +0a0H[esp]
                fld     dword ptr +94H[esp]
                fmul    dword ptr +0a4H[esp]
                fld     dword ptr +9cH[esp]
                fmul    dword ptr +0a4H[esp]
                fld     dword ptr +94H[esp]
                fmul    dword ptr +0a8H[esp]
                fld     dword ptr +98H[esp]
                fmul    dword ptr +0a0H[esp]
                mov     eax,esp
                fxch    st(2)
                fsubp   st(5),st
                fsubp   st(3),st
                fsubp   st(1),st
                fxch    st(2)
                fstp    dword ptr +0acH[esp]
                fstp    dword ptr +0b0H[esp]
                fstp    dword ptr +0b4H[esp]
                call    near ptr Matrix_Identity_
                mov     eax,dword ptr +88H[esp]
                mov     dword ptr [esp],eax
                mov     eax,dword ptr +8cH[esp]
                mov     dword ptr +4H[esp],eax
                mov     eax,dword ptr +90H[esp]
                mov     dword ptr +8H[esp],eax
                mov     eax,dword ptr +0acH[esp]
                mov     dword ptr +10H[esp],eax
                mov     eax,dword ptr +0b0H[esp]
                mov     dword ptr +14H[esp],eax
                mov     eax,dword ptr +0b4H[esp]
                mov     dword ptr +18H[esp],eax
                mov     eax,dword ptr +40H[esp]
                mov     dword ptr +20H[esp],eax
                mov     eax,dword ptr +44H[esp]
                mov     dword ptr +24H[esp],eax
                mov     eax,dword ptr +48H[esp]
                mov     edx,esp
                mov     dword ptr +28H[esp],eax
                mov     eax,ebx
                call    near ptr Matrix_Mult_
                add     esp,000000dcH
                pop     edi
                pop     esi
                pop     edx
                pop     ebx
                ret     
                lea     eax,+0H[eax]
Object_CreateMatrix_:
                push    ebx
                push    ecx
                mov     ebx,eax
                push    dword ptr +24H[eax]
                push    dword ptr +20H[eax]
                lea     ecx,+28H[eax]
                push    dword ptr +1cH[eax]
                mov     eax,ecx
                add     edx,00000038H
                call    near ptr Matrix_Rotation_
                fld     dword ptr +10H[ebx]
                fld     dword ptr +14H[ebx]
                fld     dword ptr +18H[ebx]
                mov     eax,ecx
                fxch    st(2)
                fadd    dword ptr +34H[ebx]
                fxch    st(1)
                fadd    dword ptr +44H[ebx]
                fxch    st(2)
                fadd    dword ptr +54H[ebx]
                fxch    st(1)
                fstp    dword ptr +34H[ebx]
                fxch    st(1)
                fstp    dword ptr +44H[ebx]
                fstp    dword ptr +54H[ebx]
                call    near ptr Matrix_Mult_
                pop     ecx
                pop     ebx
                ret     
                lea     eax,+0H[eax]
Object_CreateMatrix_Inherit_:
                push    ecx
                mov     ecx,eax
                mov     edx,ebx
                push    dword ptr +24H[eax]
                push    dword ptr +20H[eax]
                lea     ebx,+28H[eax]
                push    dword ptr +1cH[eax]
                mov     eax,ebx
                add     edx,00000028H
                call    near ptr Matrix_Rotation_
                fld     dword ptr +10H[ecx]
                fld     dword ptr +14H[ecx]
                fld     dword ptr +18H[ecx]
                mov     eax,ebx
                fxch    st(2)
                fadd    dword ptr +34H[ecx]
                fxch    st(1)
                fadd    dword ptr +44H[ecx]
                fxch    st(2)
                fadd    dword ptr +54H[ecx]
                fxch    st(1)
                fstp    dword ptr +34H[ecx]
                fxch    st(1)
                fstp    dword ptr +44H[ecx]
                fstp    dword ptr +54H[ecx]
                call    near ptr Matrix_Mult_
                pop     ecx
                ret     
                lea     eax,+0H[eax]
Object_Visible_: push    ebx
                push    ecx
                push    esi
                sub     esp,00000014H
                mov     ebx,eax
                add     eax,00000028H
                push    eax
                mov     edx,dword ptr +18H[ebx]
                push    edx
                mov     ecx,dword ptr +14H[ebx]
                push    ecx
                mov     esi,dword ptr +10H[ebx]
                push    esi
                lea     eax,+10H[esp]
                call    near ptr Vector_Trans_
                mov     eax,dword ptr +68H[ebx]
                mov     dword ptr +10H[esp],eax
                fld     dword ptr +8H[esp]
                fadd    dword ptr +10H[esp]
                fcomp   dword ptr _min_clip_z
                fstsw   ax
                sahf    
                jae     short L103
                xor     eax,eax
                jmp     near ptr L109
L103:           fld     dword ptr +8H[esp]
                fsub    dword ptr +10H[esp]
                fcomp   dword ptr _max_clip_z
                fstsw   ax
                sahf    
                jbe     short L104
                xor     eax,eax
                add     esp,00000014H
                pop     esi
                pop     ecx
                pop     ebx
                ret     
L104:           fld     dword ptr [esp]
                fadd    dword ptr +10H[esp]
                fld     dword ptr +8H[esp]
                fchs    
                fstp    dword ptr +0cH[esp]
                fcomp   dword ptr +0cH[esp]
                fstsw   ax
                sahf    
                jae     short L105
                xor     eax,eax
                add     esp,00000014H
                pop     esi
                pop     ecx
                pop     ebx
                ret     
L105:           fld     dword ptr [esp]
                fsub    dword ptr +10H[esp]
                fcomp   dword ptr +8H[esp]
                fstsw   ax
                sahf    
                jbe     short L106
                xor     eax,eax
                add     esp,00000014H
                pop     esi
                pop     ecx
                pop     ebx
                ret     
L106:           fld     dword ptr +4H[esp]
                fadd    dword ptr +10H[esp]
                fcomp   dword ptr +0cH[esp]
                fstsw   ax
                sahf    
                jae     short L107
                xor     eax,eax
                add     esp,00000014H
                pop     esi
                pop     ecx
                pop     ebx
                ret     
L107:           fld     dword ptr +4H[esp]
                fsub    dword ptr +10H[esp]
                fcomp   dword ptr +8H[esp]
                fstsw   ax
                sahf    
                jbe     short L108
                xor     eax,eax
                add     esp,00000014H
                pop     esi
                pop     ecx
                pop     ebx
                ret     
L108:           mov     eax,00000001H
L109:           add     esp,00000014H
                pop     esi
                pop     ecx
                pop     ebx
                ret     
                mov     eax,eax
SortWorldObject_:
                push    ebx
                push    ecx
                push    edx
                push    esi
                push    edi
                push    ebp
                mov     ecx,eax
                mov     ebx,00000400H
                mov     eax,offset DGROUP:L189
                xor     edx,edx
                call    near ptr memset_
                mov     edx,dword ptr +14H[ecx]
                xor     eax,eax
                test    edx,edx
                jle     short L111
                xor     edx,edx
L110:           mov     ebx,dword ptr +1cH[ecx]
                mov     ebx,dword ptr [edx+ebx]
                mov     ebx,dword ptr +24H[ebx]
                and     ebx,000000ffH
                inc     dword ptr L189[ebx*4]
                inc     eax
                mov     edi,dword ptr +14H[ecx]
                add     edx,00000004H
                cmp     eax,edi
                jl      short L110
L111:           xor     eax,eax
                xor     edx,edx
L112:           mov     si,word ptr L189[eax]
                add     eax,00000004H
                movsx   ebx,dx
                add     edx,esi
                mov     dword ptr __Buffer[eax],ebx
                cmp     eax,00000400H
                jne     short L112
                mov     ebp,dword ptr +14H[ecx]
                xor     edx,edx
                test    ebp,ebp
                jle     short L114
                xor     ebx,ebx
L113:           mov     eax,dword ptr +1cH[ecx]
                mov     esi,dword ptr [ebx+eax]
                mov     ax,word ptr +24H[esi]
                xor     ah,ah
                cwde    
                mov     di,word ptr L189[eax*4]
                movsx   edi,di
                mov     ebp,dword ptr +20H[ecx]
                shl     edi,02H
                add     edi,ebp
                mov     dword ptr [edi],esi
                mov     esi,dword ptr L189[eax*4]
                inc     esi
                add     ebx,00000004H
                mov     dword ptr L189[eax*4],esi
                inc     edx
                mov     edi,dword ptr +14H[ecx]
                cmp     edx,edi
                jl      short L113
L114:           mov     ebx,00000400H
                mov     eax,offset DGROUP:L189
                xor     edx,edx
                call    near ptr memset_
                mov     ebp,dword ptr +14H[ecx]
                xor     edx,edx
                test    ebp,ebp
                jle     short L116
                xor     eax,eax
L115:           mov     ebx,dword ptr +20H[ecx]
                mov     ebx,dword ptr [ebx+eax]
                mov     ebx,dword ptr +24H[ebx]
                and     ebx,0000ff00H
                sar     ebx,08H
                inc     dword ptr L189[ebx*4]
                inc     edx
                mov     edi,dword ptr +14H[ecx]
                add     eax,00000004H
                cmp     edx,edi
                jl      short L115
L116:           xor     eax,eax
                xor     edx,edx
L117:           mov     bx,word ptr L189[eax]
                add     eax,00000004H
                movsx   esi,dx
                add     edx,ebx
                mov     dword ptr __Buffer[eax],esi
                cmp     eax,00000400H
                jne     short L117
                mov     ebp,dword ptr +14H[ecx]
                xor     ebx,ebx
                test    ebp,ebp
                jle     short L119
                xor     edx,edx
L118:           mov     eax,dword ptr +20H[ecx]
                mov     esi,dword ptr [edx+eax]
                mov     eax,dword ptr +24H[esi]
                and     eax,0000ff00H
                sar     eax,08H
                cwde    
                mov     di,word ptr L189[eax*4]
                movsx   edi,di
                mov     ebp,dword ptr +1cH[ecx]
                shl     edi,02H
                add     edi,ebp
                mov     dword ptr [edi],esi
                mov     esi,dword ptr L189[eax*4]
                inc     esi
                add     edx,00000004H
                mov     dword ptr L189[eax*4],esi
                inc     ebx
                mov     edi,dword ptr +14H[ecx]
                cmp     ebx,edi
                jl      short L118
L119:           pop     ebp
                pop     edi
                pop     esi
                pop     edx
                pop     ecx
                pop     ebx
                ret     
                nop     
Handle_Object_: push    ecx
                push    esi
                push    edi
                push    ebp
                sub     esp,0000001cH
                mov     ecx,eax
                mov     dword ptr +14H[esp],edx
                mov     ebp,ebx
                test    eax,eax
                je      near ptr L132
                test    byte ptr [eax],01H
                je      near ptr L131
                test    byte ptr [ecx],04H
                je      short L120
                mov     edx,dword ptr +14H[esp]
                mov     eax,ecx
                mov     ebx,dword ptr +90H[ecx]
                call    near ptr Object_CreateMatrix_Inherit_
                jmp     short L121
L120:           mov     edx,dword ptr +14H[esp]
                mov     eax,ecx
                call    near ptr Object_CreateMatrix_
L121:           mov     edx,dword ptr +14H[esp]
                mov     eax,ecx
                call    near ptr Object_Visible_
                cmp     eax,00000001H
                jne     near ptr L131
                add     dword ptr +18H[ebp],eax
                mov     esi,dword ptr +6cH[ecx]
                xor     ebx,ebx
                test    esi,esi
                jle     near ptr L125
                lea     edi,+28H[ecx]
                xor     edx,edx
L122:           mov     eax,dword ptr +70H[ecx]
                push    edi
                mov     esi,dword ptr +8H[edx+eax]
                push    esi
                mov     esi,dword ptr +4H[edx+eax]
                push    esi
                mov     esi,dword ptr [edx+eax]
                push    esi
                lea     eax,+10H[esp]
                call    near ptr Vector_Trans_
                cmp     dword ptr +8H[esp],3f800000H
                jg      short L123
                mov     eax,dword ptr +70H[ecx]
                mov     dword ptr +14H[edx+eax],0bf800000H
                jmp     short L124
L123:           mov     eax,dword ptr +70H[ecx]
                lea     esi,[eax+edx]
                mov     eax,dword ptr [esp]
                mov     dword ptr +18H[esi],eax
                mov     eax,dword ptr +70H[ecx]
                lea     esi,[eax+edx]
                mov     eax,dword ptr +4H[esp]
                mov     dword ptr +1cH[esi],eax
                mov     eax,dword ptr +70H[ecx]
                lea     esi,[eax+edx]
                mov     eax,dword ptr +8H[esp]
                mov     dword ptr +20H[esi],eax
                mov     eax,dword ptr +70H[ecx]
                lea     esi,[eax+edx]
                mov     eax,dword ptr +8H[esp]
                mov     dword ptr +14H[esi],eax
                fld1    
                fld     dword ptr _viewdistance
                fld     dword ptr [esp]
                fmul    st,st(1)
                fxch    st(2)
                fdiv    dword ptr +8H[esp]
                fxch    st(2)
                fmul    st,st(2)
                fadd    dword ptr L181
                mov     eax,dword ptr +70H[ecx]
                fstp    dword ptr +0cH[edx+eax]
                fmul    dword ptr +4H[esp]
                fmulp   st(1),st
                fadd    dword ptr L182
                mov     eax,dword ptr +70H[ecx]
                fstp    dword ptr +10H[edx+eax]
L124:           inc     ebx
                mov     eax,dword ptr +6cH[ecx]
                add     edx,0000003cH
                cmp     ebx,eax
                jl      near ptr L122
L125:           test    byte ptr +2H[ecx],01H
                je      short L127
                mov     edx,dword ptr +6cH[ecx]
                xor     ebx,ebx
                test    edx,edx
                jle     short L127
                lea     esi,+28H[ecx]
                xor     edx,edx
L126:           mov     eax,dword ptr +70H[ecx]
                push    esi
                mov     edi,dword ptr +2cH[edx+eax]
                push    edi
                mov     edi,dword ptr +28H[edx+eax]
                push    edi
                mov     edi,dword ptr +24H[edx+eax]
                push    edi
                lea     eax,+10H[esp]
                call    near ptr Vector_Trans3x3_
                fld     dword ptr [esp]
                fmul    dword ptr L180
                fadd    dword ptr L180
                mov     eax,dword ptr +70H[ecx]
                fstp    dword ptr +30H[edx+eax]
                fld     dword ptr +4H[esp]
                fmul    dword ptr L180
                mov     eax,dword ptr +70H[ecx]
                fadd    dword ptr L180
                inc     ebx
                fstp    dword ptr +34H[edx+eax]
                mov     eax,dword ptr +6cH[ecx]
                add     edx,0000003cH
                cmp     ebx,eax
                jl      short L126
L127:           mov     edx,ebp
                mov     eax,ecx
                xor     ebx,ebx
                call    near ptr CalcLight_
                mov     dword ptr +18H[esp],ebx
                mov     esi,dword ptr +74H[ecx]
                mov     edx,dword ptr +78H[ecx]
                test    esi,esi
                jle     near ptr L131
L128:           mov     ebx,dword ptr [edx]
                mov     edi,dword ptr +4H[edx]
                mov     esi,dword ptr +8H[edx]
                cmp     dword ptr +14H[ebx],0bf800000H
                je      short L130
                cmp     dword ptr +14H[edi],0bf800000H
                je      short L130
                cmp     dword ptr +14H[esi],0bf800000H
                je      short L130
                test    byte ptr [ecx],02H
                jne     short L129
                fld     dword ptr +0cH[edi]
                fsub    dword ptr +0cH[ebx]
                fld     dword ptr +10H[esi]
                fsub    dword ptr +10H[ebx]
                fmulp   st(1),st
                fld     dword ptr +0cH[esi]
                fsub    dword ptr +0cH[ebx]
                fld     dword ptr +10H[edi]
                fsub    dword ptr +10H[ebx]
                fmulp   st(1),st
                fsubp   st(1),st
                fldz    
                fcompp  st(1)
                fstsw   ax
                sahf    
                jb      short L130
L129:           fld     dword ptr +14H[ebx]
                fadd    dword ptr +14H[edi]
                fadd    dword ptr +14H[esi]
                call    near ptr __CHP
                fistp   dword ptr +24H[edx]
                mov     eax,dword ptr +14H[ebp]
                mov     ebx,dword ptr +1cH[ebp]
                mov     dword ptr [ebx+eax*4],edx
                inc     dword ptr +14H[ebp]
                mov     dword ptr +28H[edx],ecx
L130:           mov     edi,dword ptr +18H[esp]
                add     edx,0000002cH
                inc     edi
                mov     ebx,dword ptr +74H[ecx]
                mov     dword ptr +18H[esp],edi
                cmp     edi,ebx
                jl      near ptr L128
L131:           mov     edx,dword ptr +14H[esp]
                mov     ebx,ebp
                mov     eax,dword ptr +8cH[ecx]
                call    near ptr Handle_Object_
L132:           add     esp,0000001cH
                pop     ebp
                pop     edi
                pop     esi
                pop     ecx
                ret     
                nop     
Render_:        push    ebx
                push    ecx
                push    edx
                push    esi
                push    edi
                mov     ecx,eax
                mov     dword ptr +18H[eax],00000000H
                mov     dword ptr +14H[eax],00000000H
                mov     eax,dword ptr +8H[eax]
                mov     edx,dword ptr [eax]
                cmp     edx,00000001H
                jb      short L135
                jbe     short L133
                cmp     edx,00000002H
                je      short L134
                jmp     short L135
L133:           call    near ptr Camera_CreateMatrix_Free_
                jmp     short L135
L134:           call    near ptr Camera_CreateMatrix_Aim_
L135:           mov     edx,dword ptr +0cH[ecx]
                xor     esi,esi
                test    edx,edx
                jle     short L137
                xor     ebx,ebx
L136:           mov     eax,dword ptr +10H[ecx]
                mov     edx,dword ptr +8H[ecx]
                add     eax,ebx
                inc     esi
                call    near ptr Handle_Light_
                mov     edi,dword ptr +0cH[ecx]
                add     ebx,00000084H
                cmp     esi,edi
                jl      short L136
L137:           mov     ebx,ecx
                mov     edx,dword ptr +8H[ecx]
                mov     eax,dword ptr +4H[ecx]
                call    near ptr Handle_Object_
                mov     eax,ecx
                call    near ptr SortWorldObject_
                mov     ebx,dword ptr +14H[ecx]
                dec     ebx
                test    ebx,ebx
                jl      short L139
                lea     edx,+0H[ebx*4]
L138:           sub     edx,00000004H
                mov     eax,dword ptr +1cH[ecx]
                mov     eax,dword ptr +4H[edx+eax]
                dec     ebx
                call    near ptr GTexPoly_
                test    edx,edx
                jge     short L138
L139:           mov     edx,00000001H
                mov     eax,dword ptr __Buffer
                mov     ebx,dword ptr +18H[ecx]
                add     eax,0000f780H
                call    near ptr memset_
                pop     edi
                pop     esi
                pop     edx
                pop     ecx
                pop     ebx
                ret     
                mov     eax,eax
f_read_:        push    ecx
                push    esi
                push    edi
                mov     esi,dword ptr _fp
                mov     ecx,edx
                mov     edi,eax
                push    edi
                mov     eax,ecx
                shr     ecx,02H
                repne   movsd    
                mov     cl,al
                and     cl,03H
                repne   movsb    
                pop     edi
                add     dword ptr _fp,edx
                pop     edi
                pop     esi
                pop     ecx
                ret     
                nop     
FileSize_:      push    ebx
                push    ecx
                push    edx
                push    esi
                mov     ecx,eax
                call    near ptr ftell_
                mov     ebx,00000002H
                mov     eax,ecx
                xor     edx,edx
                call    near ptr fseek_
                mov     eax,ecx
                call    near ptr ftell_
                xor     ebx,ebx
                xor     edx,edx
                mov     esi,eax
                mov     eax,ecx
                call    near ptr fseek_
                mov     eax,esi
                pop     esi
                pop     edx
                pop     ecx
                pop     ebx
                ret     
Object_LoadFile_:
                push    ebx
                push    ecx
                push    edx
                push    esi
                push    edi
                push    ebp
                sub     esp,00000020H
                mov     edx,offset DGROUP:L183
                mov     ebx,00000001H
                call    near ptr fopen_
                mov     edx,00000004H
                mov     esi,eax
                mov     ecx,eax
                lea     eax,+8H[esp]
                call    near ptr fread_
                mov     eax,00000094H
                call    near ptr malloc_
                mov     ebp,eax
                mov     edi,eax
                mov     dword ptr [eax],00000001H
                add     eax,00000068H
                mov     dword ptr -58H[eax],00000000H
                mov     dword ptr -54H[eax],00000000H
                mov     dword ptr -4cH[eax],00000000H
                mov     ebx,00000001H
                mov     dword ptr -48H[eax],00000000H
                mov     edx,00000004H
                mov     dword ptr +28H[eax],00000000H
                mov     ecx,esi
                mov     dword ptr +24H[eax],00000000H
                call    near ptr fread_
                mov     ebx,00000001H
                mov     edx,00000004H
                lea     eax,+4H[esp]
                mov     ecx,esi
                call    near ptr fread_
                mov     eax,dword ptr +4H[esp]
                mov     edx,eax
                mov     dword ptr +6cH[ebp],eax
                shl     eax,04H
                sub     eax,edx
                shl     eax,02H
                call    near ptr malloc_
                mov     dword ptr +70H[ebp],eax
                xor     edx,edx
                mov     ebx,dword ptr +4H[esp]
                mov     dword ptr +18H[esp],edx
                test    ebx,ebx
                jle     near ptr L141
                xor     ebp,ebp
L140:           mov     ebx,00000001H
                mov     edx,00000004H
                mov     eax,dword ptr +70H[edi]
                mov     ecx,esi
                add     eax,ebp
                call    near ptr fread_
                mov     ebx,00000001H
                mov     eax,dword ptr +70H[edi]
                mov     edx,00000004H
                add     eax,ebp
                mov     ecx,esi
                add     eax,00000004H
                call    near ptr fread_
                mov     ebx,00000001H
                mov     eax,dword ptr +70H[edi]
                mov     edx,00000004H
                add     eax,ebp
                mov     ecx,esi
                add     eax,00000008H
                call    near ptr fread_
                mov     ebx,00000001H
                mov     eax,dword ptr +70H[edi]
                mov     edx,00000004H
                add     eax,ebp
                mov     ecx,esi
                add     eax,00000030H
                call    near ptr fread_
                mov     ebx,00000001H
                mov     eax,dword ptr +70H[edi]
                mov     edx,00000004H
                add     eax,ebp
                mov     ecx,esi
                add     eax,00000034H
                call    near ptr fread_
                mov     eax,dword ptr +70H[edi]
                mov     dword ptr +38H[eax+ebp],427c0000H
                mov     ebx,00000001H
                mov     eax,dword ptr +70H[edi]
                mov     edx,00000004H
                add     eax,ebp
                mov     ecx,esi
                add     eax,00000024H
                call    near ptr fread_
                mov     ebx,00000001H
                mov     eax,dword ptr +70H[edi]
                mov     edx,00000004H
                add     eax,ebp
                mov     ecx,esi
                add     eax,00000028H
                call    near ptr fread_
                mov     ebx,00000001H
                mov     eax,dword ptr +70H[edi]
                mov     edx,00000004H
                add     eax,ebp
                mov     ecx,esi
                add     eax,0000002cH
                call    near ptr fread_
                mov     ecx,dword ptr +18H[esp]
                add     ebp,0000003cH
                inc     ecx
                mov     edx,dword ptr +4H[esp]
                mov     dword ptr +18H[esp],ecx
                cmp     ecx,edx
                jl      near ptr L140
L141:           mov     ebx,00000001H
                mov     edx,00000004H
                mov     eax,esp
                mov     ecx,esi
                call    near ptr fread_
                mov     eax,dword ptr [esp]
                mov     edx,eax
                mov     dword ptr +74H[edi],eax
                shl     eax,02H
                sub     eax,edx
                shl     eax,02H
                sub     eax,edx
                shl     eax,02H
                call    near ptr malloc_
                mov     dword ptr +78H[edi],eax
                xor     ebx,ebx
                mov     ecx,dword ptr [esp]
                mov     dword ptr +1cH[esp],ebx
                test    ecx,ecx
                jle     near ptr L143
                xor     ebp,ebp
L142:           mov     ebx,00000001H
                mov     edx,00000004H
                lea     eax,+14H[esp]
                mov     ecx,esi
                call    near ptr fread_
                mov     ebx,00000001H
                mov     edx,00000004H
                lea     eax,+10H[esp]
                mov     ecx,esi
                call    near ptr fread_
                mov     ebx,00000001H
                mov     edx,00000004H
                lea     eax,+0cH[esp]
                mov     ecx,esi
                call    near ptr fread_
                imul    edx,dword ptr +14H[esp],3cH
                mov     eax,dword ptr +70H[edi]
                add     edx,eax
                mov     eax,dword ptr +78H[edi]
                mov     dword ptr [eax+ebp],edx
                imul    eax,dword ptr +10H[esp],3cH
                mov     edx,dword ptr +70H[edi]
                add     eax,edx
                mov     edx,dword ptr +78H[edi]
                mov     dword ptr +4H[edx+ebp],eax
                imul    edx,dword ptr +0cH[esp],3cH
                mov     eax,dword ptr +70H[edi]
                add     eax,edx
                mov     edx,dword ptr +78H[edi]
                mov     dword ptr +8H[edx+ebp],eax
                mov     ecx,esi
                mov     eax,dword ptr +78H[edi]
                mov     ebx,00000001H
                add     eax,ebp
                mov     edx,00000004H
                add     eax,0000000cH
                call    near ptr fread_
                mov     ebx,00000001H
                mov     eax,dword ptr +78H[edi]
                mov     edx,00000004H
                add     eax,ebp
                mov     ecx,esi
                add     eax,00000010H
                call    near ptr fread_
                mov     ebx,00000001H
                mov     eax,dword ptr +78H[edi]
                mov     edx,00000004H
                add     eax,ebp
                mov     ecx,esi
                add     eax,00000014H
                call    near ptr fread_
                add     ebp,0000002cH
                mov     eax,dword ptr +78H[edi]
                mov     ecx,dword ptr +1cH[esp]
                mov     dword ptr -4H[eax+ebp],edi
                inc     ecx
                mov     edx,dword ptr [esp]
                mov     dword ptr +1cH[esp],ecx
                cmp     ecx,edx
                jl      near ptr L142
L143:           mov     eax,esi
                call    near ptr fclose_
                mov     eax,edi
                add     esp,00000020H
                pop     ebp
                pop     edi
                pop     esi
                pop     edx
                pop     ecx
                pop     ebx
                ret     
                mov     eax,eax
Object_Free_:   push    ebx
                push    ecx
                push    edx
                push    esi
                mov     edx,eax
                mov     ebx,dword ptr +8cH[eax]
                test    ebx,ebx
                je      short L144
                mov     eax,ebx
                call    near ptr Object_Free_
L144:           mov     ecx,dword ptr +70H[edx]
                test    ecx,ecx
                je      short L145
                mov     eax,ecx
                call    near ptr free_
L145:           mov     esi,dword ptr +78H[edx]
                test    esi,esi
                je      short L146
                mov     eax,esi
                call    near ptr free_
L146:           pop     esi
                pop     edx
                pop     ecx
                pop     ebx
                ret     
Object_SetHierarchy_:
                mov     dword ptr +8cH[eax],ebx
                mov     dword ptr +90H[eax],edx
                cmp     ecx,00000001H
                jne     short L147
                or      byte ptr [eax],04H
L147:           ret     
                lea     eax,+0H[eax]
Object_SetPos_:
Camera_SetRot_: push    edx
                mov     edx,dword ptr +8H[esp]
                mov     dword ptr +10H[eax],edx
                mov     edx,dword ptr +0cH[esp]
                mov     dword ptr +14H[eax],edx
                mov     edx,dword ptr +10H[esp]
                mov     dword ptr +18H[eax],edx
                pop     edx
                ret     000cH
                mov     eax,eax
Object_AddPos_:
Camera_AddRot_: fld     dword ptr +10H[eax]
                fld     dword ptr +14H[eax]
                fld     dword ptr +18H[eax]
                fxch    st(2)
                fadd    dword ptr +4H[esp]
                fxch    st(1)
                fadd    dword ptr +8H[esp]
                fxch    st(2)
                fadd    dword ptr +0cH[esp]
                fxch    st(2)
                fstp    dword ptr +14H[eax]
                fxch    st(1)
                fstp    dword ptr +18H[eax]
                fstp    dword ptr +10H[eax]
                ret     000cH
                nop     
Object_SetRot_:
Camera_SetTarget_:
                push    edx
                mov     edx,dword ptr +8H[esp]
                mov     dword ptr +1cH[eax],edx
                mov     edx,dword ptr +0cH[esp]
                mov     dword ptr +20H[eax],edx
                mov     edx,dword ptr +10H[esp]
                mov     dword ptr +24H[eax],edx
                pop     edx
                ret     000cH
                mov     eax,eax
Object_AddRot_:
Camera_AddTarget_:
                fld     dword ptr +1cH[eax]
                fld     dword ptr +20H[eax]
                fld     dword ptr +24H[eax]
                fxch    st(2)
                fadd    dword ptr +4H[esp]
                fxch    st(1)
                fadd    dword ptr +8H[esp]
                fxch    st(2)
                fadd    dword ptr +0cH[esp]
                fxch    st(2)
                fstp    dword ptr +20H[eax]
                fxch    st(1)
                fstp    dword ptr +24H[eax]
                fstp    dword ptr +1cH[eax]
                ret     000cH
                nop     
Camera_SetPos_: push    edx
                mov     edx,dword ptr +8H[esp]
                mov     dword ptr +4H[eax],edx
                mov     edx,dword ptr +0cH[esp]
                mov     dword ptr +8H[eax],edx
                mov     edx,dword ptr +10H[esp]
                mov     dword ptr +0cH[eax],edx
                pop     edx
                ret     000cH
                mov     eax,eax
Camera_AddPos_: fld     dword ptr +4H[eax]
                fld     dword ptr +8H[eax]
                fld     dword ptr +0cH[eax]
                fxch    st(2)
                fadd    dword ptr +4H[esp]
                fxch    st(1)
                fadd    dword ptr +8H[esp]
                fxch    st(2)
                fadd    dword ptr +0cH[esp]
                fxch    st(2)
                fstp    dword ptr +8H[eax]
                fxch    st(1)
                fstp    dword ptr +0cH[eax]
                fstp    dword ptr +4H[eax]
                ret     000cH
                nop     
ClosestColor_:  push    esi
                push    edi
                sub     esp,0000001cH
                mov     byte ptr [esp],dl
                mov     byte ptr +8H[esp],bl
                mov     byte ptr +0cH[esp],cl
                xor     dl,dl
                mov     ecx,7fffffffH
                mov     byte ptr +10H[esp],dl
                xor     edx,edx
L148:           mov     bl,byte ptr [eax]
                mov     byte ptr +18H[esp],bl
                mov     bl,byte ptr +1H[eax]
                mov     byte ptr +14H[esp],bl
                mov     bl,byte ptr +2H[eax]
                mov     esi,dword ptr +15H[esp]
                mov     byte ptr +4H[esp],bl
                mov     ebx,dword ptr -3H[esp]
                sar     esi,18H
                sar     ebx,18H
                sub     ebx,esi
                mov     esi,ebx
                imul    esi,ebx
                mov     edi,dword ptr +5H[esp]
                mov     ebx,dword ptr +11H[esp]
                sar     edi,18H
                sar     ebx,18H
                sub     edi,ebx
                mov     ebx,edi
                imul    ebx,edi
                mov     edi,dword ptr +9H[esp]
                add     esi,ebx
                mov     ebx,dword ptr +1H[esp]
                sar     edi,18H
                sar     ebx,18H
                sub     edi,ebx
                mov     ebx,edi
                imul    ebx,edi
                add     ebx,esi
                cmp     ebx,ecx
                jge     short L149
                mov     ecx,ebx
                mov     byte ptr +10H[esp],dl
L149:           inc     edx
                add     eax,00000003H
                cmp     edx,00000100H
                jl      short L148
                mov     al,byte ptr +10H[esp]
                add     esp,0000001cH
                pop     edi
                pop     esi
                ret     
Calc_ShadeTable_:
                push    ebx
                push    ecx
                push    esi
                push    edi
                push    ebp
                sub     esp,00000010H
                mov     dword ptr +4H[esp],eax
                mov     dword ptr [esp],edx
                xor     edi,edi
                add     eax,00000300H
                mov     dword ptr +8H[esp],edi
                mov     dword ptr +0cH[esp],eax
L150:           mov     ebp,dword ptr [esp]
                mov     ebx,dword ptr +8H[esp]
                mov     esi,dword ptr +4H[esp]
                add     ebp,ebx
L151:           movsx   edx,byte ptr +2H[esi]
                imul    edx,edi
                mov     eax,edx
                sar     edx,1fH
                shl     edx,06H
                sbb     eax,edx
                sar     eax,06H
                movsx   edx,byte ptr +1H[esi]
                imul    edx,edi
                movsx   ecx,al
                mov     eax,edx
                sar     edx,1fH
                shl     edx,06H
                sbb     eax,edx
                sar     eax,06H
                movsx   edx,byte ptr [esi]
                imul    edx,edi
                movsx   ebx,al
                mov     eax,edx
                sar     edx,1fH
                shl     edx,06H
                sbb     eax,edx
                sar     eax,06H
                inc     ebp
                movsx   edx,al
                mov     eax,dword ptr +4H[esp]
                add     esi,00000003H
                call    near ptr ClosestColor_
                mov     ecx,dword ptr +0cH[esp]
                mov     byte ptr -1H[ebp],al
                cmp     esi,ecx
                jne     short L151
                mov     esi,dword ptr +8H[esp]
                add     esi,00000100H
                inc     edi
                mov     dword ptr +8H[esp],esi
                cmp     edi,00000040H
                jl      short L150
                add     esp,00000010H
                pop     ebp
                pop     edi
                pop     esi
                pop     ecx
                pop     ebx
                ret     
Calc_ShadeHighLight_:
                push    ecx
                push    esi
                push    edi
                push    ebp
                sub     esp,00000058H
                mov     esi,eax
                mov     dword ptr +20H[esp],edx
                mov     dword ptr +40H[esp],ebx
                xor     edx,edx
                mov     dword ptr +2cH[esp],eax
                mov     dword ptr +24H[esp],edx
L152:           mov     edx,dword ptr +6cH[esp]
                mov     dword ptr +44H[esp],edx
                mov     edx,dword ptr +70H[esp]
                mov     dword ptr +48H[esp],edx
                mov     edx,dword ptr +74H[esp]
                mov     dword ptr +0cH[esp],edx
                mov     edx,dword ptr +2cH[esp]
                movsx   ax,byte ptr [edx]
                mov     dword ptr +54H[esp],eax
                fild    word ptr +54H[esp]
                fsub    dword ptr +6cH[esp]
                fild    dword ptr +40H[esp]
                fld1    
                fdivrp  st(1),st
                movsx   ax,byte ptr +1H[edx]
                fxch    st(1)
                fmul    st,st(1)
                mov     dword ptr +54H[esp],eax
                movsx   ax,byte ptr +2H[edx]
                fild    word ptr +54H[esp]
                mov     dword ptr +54H[esp],eax
                fsub    dword ptr +70H[esp]
                fild    word ptr +54H[esp]
                fsub    dword ptr +74H[esp]
                fxch    st(1)
                fmul    st,st(3)
                fxch    st(1)
                fmulp   st(3),st
                mov     ebx,dword ptr +40H[esp]
                xor     edi,edi
                fxch    st(1)
                fstp    dword ptr +34H[esp]
                fstp    dword ptr +8H[esp]
                fstp    dword ptr +38H[esp]
                test    ebx,ebx
                jle     near ptr L154
                mov     ebp,dword ptr +20H[esp]
                add     ebp,dword ptr +24H[esp]
L153:           fld     dword ptr +0cH[esp]
                fld     dword ptr +48H[esp]
                fld     dword ptr +44H[esp]
                fxch    st(2)
                call    near ptr __CHP
                fxch    st(1)
                call    near ptr __CHP
                fxch    st(2)
                call    near ptr __CHP
                fxch    st(1)
                fistp   dword ptr +50H[esp]
                mov     al,byte ptr +50H[esp]
                fxch    st(1)
                fistp   dword ptr +50H[esp]
                movsx   ecx,al
                mov     al,byte ptr +50H[esp]
                fistp   dword ptr +50H[esp]
                movsx   ebx,al
                mov     al,byte ptr +50H[esp]
                add     ebp,00000100H
                movsx   edx,al
                mov     eax,esi
                inc     edi
                call    near ptr ClosestColor_
                fld     dword ptr +44H[esp]
                fld     dword ptr +48H[esp]
                fld     dword ptr +0cH[esp]
                mov     ecx,dword ptr +40H[esp]
                mov     byte ptr -100H[ebp],al
                fxch    st(2)
                fadd    dword ptr +34H[esp]
                fxch    st(1)
                fadd    dword ptr +8H[esp]
                fxch    st(2)
                fadd    dword ptr +38H[esp]
                fxch    st(1)
                fstp    dword ptr +44H[esp]
                fxch    st(1)
                fstp    dword ptr +48H[esp]
                fstp    dword ptr +0cH[esp]
                cmp     edi,ecx
                jl      near ptr L153
L154:           mov     ebp,dword ptr +24H[esp]
                mov     edi,dword ptr +2cH[esp]
                inc     ebp
                add     edi,00000003H
                mov     dword ptr +24H[esp],ebp
                mov     dword ptr +2cH[esp],edi
                cmp     ebp,00000100H
                jl      near ptr L152
                xor     edx,edx
                mov     ebx,dword ptr +40H[esp]
                mov     dword ptr +28H[esp],edx
                mov     edx,0000003fH
                sub     edx,ebx
                inc     edx
                mov     dword ptr +18H[esp],edx
                mov     edx,ebx
                shl     edx,08H
                mov     dword ptr [esp],esi
                mov     dword ptr +1cH[esp],edx
L155:           mov     edx,dword ptr [esp]
                movsx   ax,byte ptr [edx]
                fld1    
                mov     dword ptr +54H[esp],eax
                fld     dword ptr L184
                fld     st(0)
                fild    word ptr +54H[esp]
                fstp    dword ptr +4cH[esp]
                fsub    dword ptr +4cH[esp]
                faddp   st(2),st
                fild    dword ptr +18H[esp]
                fld1    
                fdivrp  st(1),st
                movsx   ax,byte ptr +1H[edx]
                fxch    st(2)
                fmul    st,st(2)
                mov     dword ptr +54H[esp],eax
                fld     st(1)
                fild    word ptr +54H[esp]
                movsx   ax,byte ptr +2H[edx]
                fstp    dword ptr +14H[esp]
                mov     dword ptr +54H[esp],eax
                fsub    dword ptr +14H[esp]
                fild    word ptr +54H[esp]
                fld1    
                fxch    st(1)
                fstp    dword ptr +10H[esp]
                fxch    st(3)
                fsub    dword ptr +10H[esp]
                fxch    st(3)
                faddp   st(1),st
                fld1    
                faddp   st(3),st
                fmul    st,st(3)
                fxch    st(2)
                fmulp   st(3),st
                mov     ebp,dword ptr +40H[esp]
                fstp    dword ptr +3cH[esp]
                fstp    dword ptr +4H[esp]
                fstp    dword ptr +30H[esp]
                cmp     ebp,00000040H
                jge     near ptr L157
                mov     edi,dword ptr +1cH[esp]
                mov     ebx,dword ptr +20H[esp]
                mov     ecx,dword ptr +28H[esp]
                add     edi,ebx
                add     edi,ecx
L156:           fld     dword ptr +10H[esp]
                fld     dword ptr +14H[esp]
                fld     dword ptr +4cH[esp]
                fxch    st(2)
                call    near ptr __CHP
                fxch    st(1)
                call    near ptr __CHP
                fxch    st(2)
                call    near ptr __CHP
                fxch    st(1)
                fistp   dword ptr +50H[esp]
                mov     al,byte ptr +50H[esp]
                fxch    st(1)
                fistp   dword ptr +50H[esp]
                movsx   ecx,al
                mov     al,byte ptr +50H[esp]
                fistp   dword ptr +50H[esp]
                movsx   ebx,al
                mov     al,byte ptr +50H[esp]
                add     edi,00000100H
                movsx   edx,al
                mov     eax,esi
                inc     ebp
                call    near ptr ClosestColor_
                fld     dword ptr +4cH[esp]
                fld     dword ptr +14H[esp]
                fld     dword ptr +10H[esp]
                mov     byte ptr -100H[edi],al
                fxch    st(2)
                fadd    dword ptr +3cH[esp]
                fxch    st(1)
                fadd    dword ptr +4H[esp]
                fxch    st(2)
                fadd    dword ptr +30H[esp]
                fxch    st(1)
                fstp    dword ptr +4cH[esp]
                fxch    st(1)
                fstp    dword ptr +14H[esp]
                fstp    dword ptr +10H[esp]
                cmp     ebp,00000040H
                jl      near ptr L156
L157:           mov     eax,dword ptr +28H[esp]
                mov     ebp,dword ptr [esp]
                inc     eax
                add     ebp,00000003H
                mov     dword ptr +28H[esp],eax
                mov     dword ptr [esp],ebp
                cmp     eax,00000100H
                jl      near ptr L155
                add     esp,00000058H
                pop     ebp
                pop     edi
                pop     esi
                pop     ecx
                ret     000cH
                nop     
Calc_PhongIntensity_:
                push    ebx
                push    ecx
                push    edx
                push    ebp
                mov     ebp,esp
                sub     esp,00000030H
                and     esp,0fffffff8H
                mov     edx,dword ptr +14H[ebp]
                mov     dword ptr +28H[esp],edx
                mov     edx,dword ptr +18H[ebp]
                mov     dword ptr +2cH[esp],edx
                mov     edx,dword ptr +1cH[ebp]
                mov     dword ptr +24H[esp],edx
                mov     edx,dword ptr +20H[ebp]
                mov     dword ptr +1cH[esp],edx
                mov     edx,dword ptr +24H[ebp]
                mov     dword ptr +20H[esp],edx
                mov     edx,0c0490fdbH
                mov     ebx,427c0000H
                mov     dword ptr +18H[esp],edx
                xor     edx,edx
L158:           fld     dword ptr +18H[esp]
                fcos    
                fld     dword ptr +24H[esp]
                fmul    st,st(1)
                fld     dword ptr +28H[esp]
                fxch    st(1)
                fmul    st,st(1)
                fmul    dword ptr +2cH[esp]
                fld     dword ptr +20H[esp]
                fld     st(3)
                call    near ptr `IF@DPOW`
                fstp    st(3)
                fld     dword ptr +1cH[esp]
                fmulp   st(3),st
                fxch    st(2)
                fmulp   st(1),st
                faddp   st(1),st
                fmul    qword ptr L185
                fst     dword ptr +14H[esp]
                fcomp   dword ptr L186
                fstsw   ax
                sahf    
                jbe     short L159
                mov     dword ptr +14H[esp],ebx
L159:           fldz    
                fcomp   dword ptr +14H[esp]
                fstsw   ax
                sahf    
                jbe     short L160
                xor     ecx,ecx
                mov     dword ptr +14H[esp],ecx
L160:           fld     dword ptr +18H[esp]
                fld     dword ptr +14H[esp]
                call    near ptr __CHP
                inc     edx
                fistp   dword ptr +10H[esp]
                fadd    qword ptr L187
                mov     al,byte ptr +10H[esp]
                fstp    dword ptr +18H[esp]
                mov     byte ptr _PhongIntensity-1H[edx],al
                cmp     edx,00000080H
                jl      near ptr L158
                mov     al,byte ptr _PhongIntensity+7fH
                mov     byte ptr _PhongIntensity+80H,al
                mov     esp,ebp
                pop     ebp
                pop     edx
                pop     ecx
                pop     ebx
                ret     0014H
                nop     
Calc_PhongMap_: push    ebx
                push    ecx
                push    edx
                push    esi
                push    edi
                push    ebp
                sub     esp,00000008H
                mov     esi,0ffffff80H
                mov     ebp,0000007fH
                mov     ebx,dword ptr __PhongMap
L161:           mov     ecx,esi
                mov     edx,0ffffff80H
                imul    ecx,esi
L162:           mov     eax,edx
                imul    eax,edx
                lea     edi,[ecx+eax]
                mov     dword ptr [esp],edi
                fild    dword ptr [esp]
                fsqrt   
                call    near ptr __CHP
                fistp   dword ptr +4H[esp]
                cmp     ebp,dword ptr +4H[esp]
                jge     short L163
                mov     dword ptr +4H[esp],ebp
L163:           mov     edi,dword ptr +4H[esp]
                mov     eax,ebp
                sub     eax,edi
                inc     ebx
                mov     al,byte ptr _PhongIntensity[eax]
                inc     edx
                mov     byte ptr -1H[ebx],al
                cmp     edx,00000080H
                jne     short L162
                inc     esi
                cmp     esi,edx
                jne     short L161
                add     esp,00000008H
                pop     ebp
                pop     edi
                pop     esi
                pop     edx
                pop     ecx
                pop     ebx
                ret     
Calc_GouraudMap_:
                push    ebx
                push    ecx
                push    edx
                push    esi
                push    edi
                push    ebp
                mov     ebp,esp
                sub     esp,0000000cH
                and     esp,0fffffff8H
                mov     edi,dword ptr __PhongMap
                mov     ebx,00010000H
                mov     eax,edi
                xor     edx,edx
                call    near ptr memset_
                mov     edx,0ffffff80H
                mov     edi,dword ptr __PhongMap
                xor     ebx,ebx
                mov     dword ptr [esp],edx
                mov     dword ptr +4H[esp],ebx
                mov     ch,0eH
L164:           mov     esi,dword ptr [esp]
                mov     edx,0ffffff80H
                mov     ebx,dword ptr +4H[esp]
                imul    esi,esi
L165:           mov     eax,edx
                imul    eax,edx
                add     eax,esi
                mov     dword ptr +8H[esp],eax
                fild    dword ptr +8H[esp]
                fsqrt   
                fsubr   qword ptr L188
                call    near ptr __CHP
                fistp   dword ptr +8H[esp]
                mov     al,byte ptr +8H[esp]
                cmp     al,7fH
                jl      short L166
                mov     al,7fH
L166:           cmp     al,0eH
                jg      short L167
                mov     al,ch
L167:           movsx   eax,al
                sar     eax,1
                inc     ebx
                mov     cl,al
                mov     eax,edi
                inc     edx
                mov     byte ptr -1H[ebx+eax],cl
                cmp     edx,00000080H
                jne     short L165
                mov     ebx,dword ptr +4H[esp]
                mov     edx,dword ptr [esp]
                add     ebx,00000100H
                inc     edx
                mov     dword ptr +4H[esp],ebx
                mov     dword ptr [esp],edx
                cmp     ebx,00010000H
                jne     short L164
                mov     dword ptr __PhongMap,edi
                mov     esp,ebp
                pop     ebp
                pop     edi
                pop     esi
                pop     edx
                pop     ecx
                pop     ebx
                ret     
_TEXT           ENDS

CONST           SEGMENT DWORD PUBLIC USE32 'DATA'
L168            DB      00H,00H,00H,80H,00H,00H,30H,43H
L169            DB      00H,00H,00H,80H,00H,00H,30H,42H
L170            DB      00H,00H,00H,80H,00H,00H,0f0H,42H
L171            DB      00H,00H,00H,80H,00H,00H,30H,42H
L172            DB      00H,00H,00H,80H,00H,00H,30H,43H
L173            DB      00H,00H,00H,80H,00H,00H,30H,42H
L174            DB      00H,00H,00H,80H,00H,00H,0f0H,42H
L175            DB      00H,00H,00H,80H,00H,00H,30H,42H
L176            DB      00H,00H,00H,80H,00H,00H,30H,42H
L177            DB      00H,00H,00H,80H,00H,00H,30H,42H
L178            DB      00H,00H,00H,80H,00H,00H,30H,42H
L179            DB      00H,00H,0feH,42H
L180            DB      00H,00H,7cH,42H
L181            DB      00H,00H,20H,43H
L182            DB      00H,00H,0c8H,42H
L183            DB      72H,62H,00H
L184            DB      00H,00H,7cH,42H
L185            DB      00H,00H,00H,00H,00H,80H,4fH,40H
L186            DB      00H,00H,7cH,42H
L187            DB      44H,17H,41H,54H,0fbH,21H,0a9H,3fH
L188            DB      00H,00H,00H,00H,00H,0c0H,5fH,40H
CONST           ENDS

CONST2          SEGMENT DWORD PUBLIC USE32 'DATA'
CONST2          ENDS

_DATA           SEGMENT DWORD PUBLIC USE32 'DATA'
                PUBLIC  _PhongIntensity 
                PUBLIC  _FontData 
                PUBLIC  _s 
_PhongIntensity DB      09H,09H,0aH,0aH,0aH,0aH,0bH,0bH
                DB      0bH,0cH,0cH,0cH,0cH,0dH,0dH,0dH
                DB      0dH,0eH,0eH,0eH,0eH,0fH,0fH,0fH
                DB      10H,10H,10H,10H,11H,11H,11H,11H
                DB      12H,12H,12H,12H,13H,13H,13H,13H
                DB      13H,14H,14H,14H,14H,15H,15H,15H
                DB      15H,16H,16H,16H,16H,16H,17H,17H
                DB      17H,17H,18H,18H,18H,18H,18H,18H
                DB      19H,19H,19H,19H,19H,1aH,1aH,1aH
                DB      1aH,1aH,1aH,1bH,1bH,1bH,1bH,1bH
                DB      1bH,1cH,1cH,1cH,1cH,1cH,1cH,1cH
                DB      1dH,1dH,1dH,1dH,1dH,1dH,1dH,1dH
                DB      1eH,1eH,1eH,1eH,1eH,1fH,1fH,1fH
                DB      20H,20H,21H,21H,22H,23H,24H,25H
                DB      27H,28H,2aH,2cH,2eH,2fH,31H,33H
                DB      35H,37H,39H,3bH,3cH,3dH,3eH,3eH
                DB      3eH
_FontData       DB      00H,00H,00H,00H,00H,00H,00H,00H
                DB      7cH,0eeH,0eeH,0feH,0eeH,0eeH,0eeH,00H
                DB      0fcH,0eeH,0eeH,0fcH,0eeH,0eeH,0fcH,00H
                DB      7cH,0eeH,0e0H,0e0H,0e0H,0eeH,7cH,00H
                DB      0fcH,0eeH,0eeH,0eeH,0eeH,0eeH,0fcH,00H
                DB      0feH,0eeH,0e0H,0fcH,0e0H,0eeH,0feH,00H
                DB      0feH,0eeH,0e0H,0fcH,0e0H,0e0H,0e0H,00H
                DB      7cH,0eeH,0e0H,0eeH,0e6H,0feH,7eH,00H
                DB      0eeH,0eeH,0eeH,0feH,0eeH,0eeH,0eeH,00H
                DB      7cH,38H,38H,38H,38H,38H,7cH,00H
                DB      0feH,0eeH,0eH,0eeH,0eeH,0eeH,7cH,00H
                DB      0eeH,0eeH,0ecH,0f8H,0ecH,0eeH,0eeH,00H
                DB      0e0H,0e0H,0e0H,0e0H,0e0H,0eeH,0feH,00H
                DB      0c6H,0eeH,0feH,0eeH,0eeH,0eeH,0eeH,00H
                DB      0fcH,0eeH,0eeH,0eeH,0eeH,0eeH,0eeH,00H
                DB      7cH,0eeH,0eeH,0eeH,0eeH,0eeH,7cH,00H
                DB      0fcH,0eeH,0eeH,0fcH,0e0H,0e0H,0e0H,00H
                DB      7cH,0eeH,0eeH,0eeH,0eeH,0ecH,7eH,00H
                DB      0fcH,0eeH,0eeH,0fcH,0eeH,0eeH,0eeH,00H
                DB      7eH,0eeH,0e0H,7cH,0eH,0eeH,0fcH,00H
                DB      0feH,0baH,38H,38H,38H,38H,38H,00H
                DB      0eeH,0eeH,0eeH,0eeH,0eeH,0eeH,7cH,00H
                DB      0eeH,0eeH,0eeH,0eeH,6cH,38H,10H,00H
                DB      0eeH,0eeH,0eeH,0eeH,0feH,0eeH,0c6H,00H
                DB      0eeH,0eeH,6cH,38H,6cH,0eeH,0eeH,00H
                DB      0eeH,0eeH,0eeH,7eH,0eH,0eeH,0fcH,00H
                DB      0feH,0eeH,0eH,7cH,0e0H,0eeH,0feH,00H
                DB      38H,78H,38H,38H,38H,38H,7cH,00H
                DB      7cH,0eeH,0eH,7cH,0e0H,0eeH,0feH,00H
                DB      7cH,0eeH,0eH,7cH,0eH,0eeH,7cH,00H
                DB      0eeH,0eeH,0eeH,7eH,0eH,0eH,0eH,00H
                DB      0feH,0eeH,0e0H,0fcH,0eH,0eeH,7cH,00H
                DB      7cH,0eeH,0e0H,0feH,0eeH,0eeH,7cH,00H
                DB      0feH,0eeH,0eH,1cH,38H,38H,38H,00H
                DB      7cH,0eeH,0eeH,7cH,0eeH,0eeH,7cH,00H
                DB      7cH,0eeH,0eeH,7eH,0eH,0eeH,7cH,00H
                DB      7cH,0eeH,0eeH,0feH,0eeH,0eeH,7cH,00H
                DB      38H,38H,38H,38H,00H,38H,38H,00H
                DB      00H,00H,00H,00H,00H,38H,38H,00H
                DB      38H,38H,00H,00H,00H,00H,00H,00H
                DB      0eeH,0eeH,00H,00H,00H,00H,00H,00H
                DB      7cH,0eeH,0eH,1cH,38H,00H,38H,00H
                DB      00H,38H,38H,00H,38H,38H,00H,00H
                DB      00H,00H,00H,7eH,00H,00H,00H,00H
                DB      00H
_s              DB      20H,41H,42H,43H,44H,45H,46H,47H
                DB      48H,49H,4aH,4bH,4cH,4dH,4eH,4fH
                DB      50H,51H,52H,53H,54H,55H,56H,57H
                DB      58H,59H,5aH,31H,32H,33H,34H,35H
                DB      36H,37H,38H,39H,30H,21H,2eH,27H
                DB      22H,3fH,3aH,2dH,00H
_DATA           ENDS

_BSS            SEGMENT PARA PUBLIC USE32 'BSS'
                PUBLIC  _Edge 
                PUBLIC  _fp 
                PUBLIC  __innervadd 
                PUBLIC  __y2 
                PUBLIC  __y1 
                PUBLIC  __innerv 
                PUBLIC  __inneru 
                PUBLIC  __inneruadd 
                PUBLIC  _MinX 
                PUBLIC  _MinY 
                PUBLIC  _viewdistance 
                PUBLIC  __ShadeTable 
                PUBLIC  _halfscreenheight_vd 
                PUBLIC  _ScreenHeight 
                PUBLIC  _halfscreenwidth_vd 
                PUBLIC  _min_clip_z 
                PUBLIC  __PhongMap 
                PUBLIC  _MaxY 
                PUBLIC  _MaxX 
                PUBLIC  _max_clip_z 
                PUBLIC  _ScreenWidth 
                PUBLIC  __DivTable 
                PUBLIC  __Screen 
                PUBLIC  __Texture1 
                PUBLIC  __Texture2 
                PUBLIC  __Buffer 
                ORG     00000000H
_Edge           LABEL   BYTE
                ORG     00000fa0H
_fp             LABEL   BYTE
                ORG     00000fa4H
__innervadd     LABEL   BYTE
                ORG     00000fa8H
__y2            LABEL   BYTE
                ORG     00000facH
__y1            LABEL   BYTE
                ORG     00000fb0H
__innerv        LABEL   BYTE
                ORG     00000fb4H
__inneru        LABEL   BYTE
                ORG     00000fb8H
__inneruadd     LABEL   BYTE
                ORG     00000fbcH
_MinX           LABEL   BYTE
                ORG     00000fc0H
_MinY           LABEL   BYTE
                ORG     00000fc4H
_viewdistance   LABEL   BYTE
                ORG     00000fc8H
__ShadeTable    LABEL   BYTE
                ORG     00000fccH
_halfscreenheight_vd LABEL   BYTE
                ORG     00000fd0H
_ScreenHeight   LABEL   BYTE
                ORG     00000fd4H
_halfscreenwidth_vd LABEL   BYTE
                ORG     00000fd8H
_min_clip_z     LABEL   BYTE
                ORG     00000fdcH
__PhongMap      LABEL   BYTE
                ORG     00000fe0H
_MaxY           LABEL   BYTE
                ORG     00000fe4H
_MaxX           LABEL   BYTE
                ORG     00000fe8H
_max_clip_z     LABEL   BYTE
                ORG     00000fecH
_ScreenWidth    LABEL   BYTE
                ORG     00000ff0H
__DivTable      LABEL   BYTE
                ORG     00000ff4H
__Screen        LABEL   BYTE
                ORG     00000ff8H
__Texture1      LABEL   BYTE
                ORG     00000ffcH
__Texture2      LABEL   BYTE
                ORG     00001000H
__Buffer        LABEL   BYTE
                ORG     00001004H
L189            LABEL   BYTE
                ORG     00001404H
_BSS            ENDS

                END
