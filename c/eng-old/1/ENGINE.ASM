.386p
                NAME    ENGINE
                EXTRN   __8087 :BYTE
                EXTRN   _fltused_ :BYTE
                EXTRN   VesaUnInit_ :BYTE
                EXTRN   _VesaPtr :BYTE
                EXTRN   VesaInit_ :BYTE
                EXTRN   `IF@DPOW` :BYTE
                EXTRN   strcmp_ :BYTE
                EXTRN   free_ :BYTE
                EXTRN   fclose_ :BYTE
                EXTRN   fseek_ :BYTE
                EXTRN   fgetc_ :BYTE
                EXTRN   malloc_ :BYTE
                EXTRN   ftell_ :BYTE
                EXTRN   fread_ :BYTE
                EXTRN   filelength_ :BYTE
                EXTRN   fopen_ :BYTE
                EXTRN   WobblePolyGouraudTexture_ :BYTE
                EXTRN   memset_ :BYTE
                EXTRN   __CHP :BYTE
                EXTRN   GTexScanleft_ :BYTE
                EXTRN   TexScanleft_ :BYTE
DGROUP          GROUP   CONST,CONST2,_DATA,_BSS
_TEXT           SEGMENT PARA PUBLIC USE32 'CODE'
                ASSUME  CS:_TEXT ,DS:DGROUP,SS:DGROUP
                PUBLIC  Matrix_Identity_ 
                PUBLIC  Matrix_Translation_ 
                PUBLIC  Matrix_Rotation_ 
                PUBLIC  Matrix_RotationX_ 
                PUBLIC  Matrix_RotationY_ 
                PUBLIC  Matrix_RotationZ_ 
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
                PUBLIC  Vector_Length_ 
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
                PUBLIC  VesaGTexFillPoly_ 
                PUBLIC  TexPoly_ 
                PUBLIC  GTexPoly_ 
                PUBLIC  Spline_Calc_ 
                PUBLIC  Spline_Coeffs_ 
                PUBLIC  Spline_Init_ 
                PUBLIC  Spline_GetValues_ 
                PUBLIC  Spline_Add_ 
                PUBLIC  Light_Calc_ 
                PUBLIC  Handle_Light_ 
                PUBLIC  Camera_CreateMatrix_Free_ 
                PUBLIC  Camera_CreateMatrix_Aim_ 
                PUBLIC  Object_CreateMatrix_ 
                PUBLIC  Object_CreateMatrix_Inherit_ 
                PUBLIC  Object_Visible_ 
                PUBLIC  SortWorldObject_ 
                PUBLIC  Handle_Object_ 
                PUBLIC  Render_Setup_ 
                PUBLIC  Render_Draw_ 
                PUBLIC  Load3DS_ 
                PUBLIC  Object_CalcFaceNormals_ 
                PUBLIC  Object_CalcVertexNormals_ 
                PUBLIC  Object_Center_ 
                PUBLIC  Object_CalcBoundingSphere_ 
                PUBLIC  Object_LoadFile_ 
                PUBLIC  Object_Free_ 
                PUBLIC  Object_SetHierarchy_ 
                PUBLIC  Object_SetPos_ 
                PUBLIC  Object_AddPos_ 
                PUBLIC  Object_SetRot_ 
                PUBLIC  Object_AddRot_ 
                PUBLIC  Object_FindByName_ 
                PUBLIC  Camera_SetPos_ 
                PUBLIC  Camera_SetRot_ 
                PUBLIC  Camera_SetTarget_ 
                PUBLIC  Camera_AddTarget_ 
                PUBLIC  Camera_AddRot_ 
                PUBLIC  Camera_AddPos_ 
                PUBLIC  ClosestColor_ 
                PUBLIC  Calc_ShadeTable_ 
                PUBLIC  Calc_ShadeHighLight_ 
                PUBLIC  Setup_Intensity_ 
                PUBLIC  Calc_PhongIntensity_ 
                PUBLIC  Calc_PhongMap_ 
                PUBLIC  Calc_GouraudMap_ 
                PUBLIC  VesaCalc_ShadeTable_ 
                PUBLIC  VesaCalc_ShadeHighLight_ 
                PUBLIC  Engine_Init_ 
                PUBLIC  Engine_UnInit_ 
float2int_:     sub     esp,00000008H
                fld     dword ptr +0cH[esp]
                fadd    qword ptr L276
                fstp    qword ptr [esp]
                mov     eax,dword ptr [esp]
                add     eax,80000000H
                add     esp,00000008H
                ret     0004H
                mov     eax,eax
float2fixed_:   sub     esp,00000008H
                fld     dword ptr +0cH[esp]
                fadd    qword ptr L277
                fstp    qword ptr [esp]
                mov     eax,dword ptr [esp]
                add     eax,80000000H
                add     esp,00000008H
                ret     0004H
                mov     eax,eax
float2fixed12_4_:
                sub     esp,00000008H
                fld     dword ptr +0cH[esp]
                fadd    qword ptr L278
                fstp    qword ptr [esp]
                mov     eax,dword ptr [esp]
                add     eax,80000000H
                add     esp,00000008H
                ret     0004H
                mov     eax,eax
CEIL_:          push    edx
                sub     esp,00000008H
                fld     dword ptr +10H[esp]
                fadd    qword ptr L279
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
Matrix_RotationX_:
                push    edx
                sub     esp,00000010H
                mov     edx,eax
                fld     dword ptr +18H[esp]
                fld     st(0)
                fsin    
                fxch    st(1)
                fcos    
                fxch    st(1)
                fstp    dword ptr +0cH[esp]
                mov     eax,edx
                fstp    dword ptr +8H[esp]
                call    near ptr Matrix_Identity_
                mov     eax,dword ptr +8H[esp]
                mov     dword ptr +14H[edx],eax
                mov     eax,dword ptr +0cH[esp]
                fld     dword ptr +0cH[esp]
                mov     dword ptr +18H[edx],eax
                fchs    
                mov     eax,dword ptr +8H[esp]
                fstp    dword ptr +24H[edx]
                mov     dword ptr +28H[edx],eax
                add     esp,00000010H
                pop     edx
                ret     0004H
Matrix_RotationY_:
                push    edx
                sub     esp,00000010H
                mov     edx,eax
                fld     dword ptr +18H[esp]
                fld     st(0)
                fsin    
                fxch    st(1)
                fcos    
                fxch    st(1)
                fstp    dword ptr +0cH[esp]
                mov     eax,edx
                fstp    dword ptr +8H[esp]
                call    near ptr Matrix_Identity_
                mov     eax,dword ptr +8H[esp]
                mov     dword ptr [edx],eax
                mov     eax,dword ptr +0cH[esp]
                fld     dword ptr +0cH[esp]
                mov     dword ptr +8H[edx],eax
                fchs    
                mov     eax,dword ptr +8H[esp]
                fstp    dword ptr +20H[edx]
                mov     dword ptr +28H[edx],eax
                add     esp,00000010H
                pop     edx
                ret     0004H
                nop     
Matrix_RotationZ_:
                push    edx
                sub     esp,00000010H
                mov     edx,eax
                fld     dword ptr +18H[esp]
                fld     st(0)
                fsin    
                fxch    st(1)
                fcos    
                fxch    st(1)
                fstp    dword ptr +0cH[esp]
                mov     eax,edx
                fstp    dword ptr +8H[esp]
                call    near ptr Matrix_Identity_
                mov     eax,dword ptr +8H[esp]
                mov     dword ptr [edx],eax
                mov     eax,dword ptr +0cH[esp]
                fld     dword ptr +0cH[esp]
                mov     dword ptr +4H[edx],eax
                fchs    
                mov     eax,dword ptr +8H[esp]
                fstp    dword ptr +10H[edx]
                mov     dword ptr +14H[edx],eax
                add     esp,00000010H
                pop     edx
                ret     0004H
                nop     
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
Matrix_Inverse_: push    ebx
                sub     esp,00000040H
                mov     ebx,eax
                sub     esp,00000004H
                fld     dword ptr +2cH[edx]
                fchs    
                fstp    dword ptr [esp]
                sub     esp,00000004H
                fld     dword ptr +1cH[edx]
                fchs    
                fstp    dword ptr [esp]
                sub     esp,00000004H
                fld     dword ptr +0cH[edx]
                fchs    
                mov     eax,ebx
                fstp    dword ptr [esp]
                call    near ptr Matrix_Translation_
                mov     eax,esp
                call    near ptr Matrix_Identity_
                mov     eax,dword ptr [edx]
                mov     dword ptr [esp],eax
                mov     eax,dword ptr +10H[edx]
                mov     dword ptr +4H[esp],eax
                mov     eax,dword ptr +20H[edx]
                mov     dword ptr +8H[esp],eax
                mov     eax,dword ptr +4H[edx]
                mov     dword ptr +10H[esp],eax
                mov     eax,dword ptr +14H[edx]
                mov     dword ptr +14H[esp],eax
                mov     eax,dword ptr +24H[edx]
                mov     dword ptr +18H[esp],eax
                mov     eax,dword ptr +8H[edx]
                mov     dword ptr +20H[esp],eax
                mov     eax,dword ptr +18H[edx]
                mov     dword ptr +24H[esp],eax
                mov     eax,dword ptr +28H[edx]
                mov     edx,esp
                mov     dword ptr +28H[esp],eax
                mov     eax,ebx
                call    near ptr Matrix_Mult_
                add     esp,00000040H
                pop     ebx
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
Vector_Length_: push    ebx
                push    ebp
                mov     ebp,esp
                sub     esp,00000008H
                and     esp,0fffffff8H
                fld     dword ptr +4H[eax]
                fmul    st,st(0)
                fld     dword ptr [eax]
                fmul    st,st(0)
                faddp   st(1),st
                fld     dword ptr +8H[eax]
                fmul    st,st(0)
                faddp   st(1),st
                fstp    dword ptr +4H[esp]
                test    dword ptr +4H[esp],7fffffffH
                je      short L10
                fld     dword ptr +4H[esp]
                fsqrt   
                fstp    dword ptr [esp]
                jmp     short L11
L10:            xor     ebx,ebx
                mov     dword ptr [esp],ebx
L11:            fld     dword ptr [esp]
                mov     esp,ebp
                pop     ebp
                pop     ebx
                ret     
                mov     eax,eax
Line_:          push    esi
                push    edi
                push    ebp
                sub     esp,00000014H
                mov     esi,ecx
                cmp     eax,ebx
                jge     short L12
                mov     ecx,00000001H
                sub     ebx,eax
                mov     dword ptr [esp],ecx
                mov     dword ptr +8H[esp],ebx
                jmp     short L13
L12:            mov     ecx,eax
                sub     ecx,ebx
                mov     ebx,0ffffffffH
                mov     dword ptr +8H[esp],ecx
                mov     dword ptr [esp],ebx
L13:            cmp     edx,esi
                jge     short L14
                mov     edi,00000001H
                sub     esi,edx
                mov     dword ptr +4H[esp],edi
                mov     dword ptr +0cH[esp],esi
                jmp     short L15
L14:            mov     ebx,edx
                sub     ebx,esi
                mov     esi,0ffffffffH
                mov     dword ptr +0cH[esp],ebx
                mov     dword ptr +4H[esp],esi
L15:            mov     ebx,dword ptr +8H[esp]
                mov     ebp,dword ptr +0cH[esp]
                cmp     ebx,ebp
                jg      short L16
                mov     ebx,ebp
L16:            mov     esi,ebx
                xor     ecx,ecx
                sar     esi,1
                mov     dword ptr +10H[esp],ecx
                mov     edi,esi
                test    ebx,ebx
                jle     near ptr L21
L17:            cmp     eax,dword ptr _MinX
                jl      short L18
                cmp     eax,dword ptr _MaxX
                jg      short L18
                cmp     edx,dword ptr _MinY
                jl      short L18
                cmp     edx,dword ptr _MaxY
                jg      short L18
                mov     ecx,dword ptr _ScreenWidth
                imul    ecx,edx
                mov     ebp,dword ptr +28H[esp]
                add     ecx,eax
                add     ebp,ecx
                mov     cl,byte ptr +24H[esp]
                mov     byte ptr +0H[ebp],cl
L18:            add     edi,dword ptr +8H[esp]
                cmp     edi,ebx
                jl      short L19
                mov     ecx,dword ptr [esp]
                sub     edi,ebx
                add     eax,ecx
L19:            add     esi,dword ptr +0cH[esp]
                cmp     esi,ebx
                jl      short L20
                mov     ecx,dword ptr +4H[esp]
                sub     esi,ebx
                add     edx,ecx
L20:            mov     ebp,dword ptr +10H[esp]
                inc     ebp
                mov     dword ptr +10H[esp],ebp
                cmp     ebx,ebp
                jg      short L17
L21:            add     esp,00000014H
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
                je      near ptr L34
                mov     esi,offset DGROUP:_s
L22:            mov     al,byte ptr [esi]
                cmp     al,dl
                je      short L24
                cmp     al,00H
                je      short L23
                inc     esi
                mov     al,byte ptr [esi]
                cmp     al,dl
                je      short L24
                inc     esi
                cmp     al,00H
                jne     short L22
L23:            sub     esi,esi
L24:            mov     eax,esi
                sub     eax,offset DGROUP:_s
                mov     edi,dword ptr _ScreenWidth
                mov     byte ptr +4H[esp],al
                mov     eax,ebp
                imul    eax,edi
                add     eax,dword ptr [esp]
                mov     ebx,dword ptr +18H[esp]
                add     eax,eax
                add     eax,ebx
                xor     ebx,ebx
L25:            xor     edx,edx
                mov     dl,byte ptr +4H[esp]
                mov     dl,byte ptr _FontData[ebx+edx*8]
                test    dl,80H
                je      short L26
                mov     word ptr [eax],cx
L26:            test    dl,40H
                je      short L27
                mov     word ptr +2H[eax],cx
L27:            test    dl,20H
                je      short L28
                mov     word ptr +4H[eax],cx
L28:            test    dl,10H
                je      short L29
                mov     word ptr +6H[eax],cx
L29:            test    dl,08H
                je      short L30
                mov     word ptr +8H[eax],cx
L30:            test    dl,04H
                je      short L31
                mov     word ptr +0aH[eax],cx
L31:            test    dl,02H
                je      short L32
                mov     word ptr +0cH[eax],cx
L32:            test    dl,01H
                je      short L33
                mov     word ptr +0eH[eax],cx
L33:            lea     edx,+0H[edi*2]
                inc     ebx
                add     eax,edx
                cmp     ebx,00000008H
                jl      short L25
L34:            mov     dword ptr _ScreenWidth,edi
                add     esp,00000008H
                pop     ebp
                pop     edi
                pop     esi
                ret     0004H
                mov     eax,eax
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
                je      short L36
L35:            mov     edx,dword ptr +14H[esp]
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
                jne     short L35
L36:            add     esp,00000004H
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
L37:            mov     ah,byte ptr [ecx]
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
                jl      short L37
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
                push    dword ptr +14H[ebx]
                fld     dword ptr +10H[eax]
                fadd    qword ptr L280
                mov     dword ptr +10H[esp],ebx
                fstp    qword ptr +20H[esp]
                call    near ptr float2int_
                push    dword ptr +10H[ebx]
                mov     ecx,eax
                call    near ptr float2int_
                mov     ebx,eax
                mov     eax,dword ptr +8H[esp]
                mov     esi,dword ptr +1cH[esp]
                push    dword ptr +14H[eax]
                add     esi,80000000H
                call    near ptr float2int_
                mov     edx,eax
                mov     eax,esi
                call    near ptr Line_
                mov     ebx,dword ptr __Buffer
                mov     eax,dword ptr [esp]
                push    ebx
                fld     dword ptr +10H[eax]
                mov     eax,dword ptr +0cH[esp]
                push    0000000fH
                fadd    qword ptr L280
                push    dword ptr +14H[eax]
                fstp    qword ptr +28H[esp]
                call    near ptr float2int_
                mov     ecx,eax
                mov     eax,dword ptr +10H[esp]
                push    dword ptr +10H[eax]
                call    near ptr float2int_
                mov     ebx,eax
                mov     eax,dword ptr +8H[esp]
                mov     esi,dword ptr +24H[esp]
                push    dword ptr +14H[eax]
                add     esi,80000000H
                call    near ptr float2int_
                mov     edx,eax
                mov     eax,esi
                call    near ptr Line_
                mov     eax,dword ptr +4H[esp]
                mov     ecx,dword ptr __Buffer
                mov     eax,dword ptr +10H[eax]
                push    ecx
                mov     dword ptr +28H[esp],eax
                push    0000000fH
                fld     dword ptr +2cH[esp]
                mov     eax,dword ptr +10H[esp]
                fadd    qword ptr L280
                push    dword ptr +14H[eax]
                fstp    qword ptr +18H[esp]
                call    near ptr float2int_
                mov     ecx,eax
                mov     eax,dword ptr +10H[esp]
                push    dword ptr +10H[eax]
                call    near ptr float2int_
                mov     ebx,eax
                mov     eax,dword ptr +0cH[esp]
                mov     esi,dword ptr +14H[esp]
                push    dword ptr +14H[eax]
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
                mov     edx,dword ptr +14H[eax]
                mov     dword ptr [esp],edx
                call    near ptr float2fixed_
                mov     edx,eax
                sar     eax,10H
                test    dx,0ffffH
                je      short L38
                inc     eax
L38:            mov     dword ptr +14H[esp],eax
                sub     esp,00000004H
                mov     eax,dword ptr +14H[esi]
                mov     dword ptr [esp],eax
                call    near ptr float2fixed_
                mov     edx,eax
                sar     edx,10H
                test    ax,0ffffH
                je      short L39
                inc     edx
L39:            mov     edi,dword ptr +14H[esp]
                mov     ecx,edx
                cmp     edx,edi
                je      near ptr L45
                fld     dword ptr +10H[esi]
                fsub    dword ptr +10H[ebx]
                fild    dword ptr +14H[esp]
                fld     dword ptr +14H[esi]
                fsub    dword ptr +14H[ebx]
                fxch    st(1)
                fsub    dword ptr +14H[ebx]
                fxch    st(1)
                fdivp   st(2),st
                fmul    st,st(1)
                fxch    st(1)
                fld     qword ptr L281
                fxch    st(1)
                fadd    st,st(1)
                mov     ebp,dword ptr +14H[esp]
                fstp    qword ptr +8H[esp]
                fxch    st(1)
                fadd    dword ptr +10H[ebx]
                mov     edi,dword ptr +8H[esp]
                faddp   st(1),st
                add     edi,80000000H
                fstp    qword ptr [esp]
                mov     eax,dword ptr [esp]
                mov     esi,edi
                add     eax,80008000H
                cmp     ebp,000000c7H
                jg      near ptr L45
                test    edx,edx
                jl      near ptr L45
                test    ebp,ebp
                jge     short L40
                mov     edx,ebp
                neg     edx
                imul    edi,edx
                xor     ebx,ebx
                mov     dword ptr +14H[esp],ebx
                add     eax,edi
L40:            cmp     ecx,000000c7H
                jle     short L41
                mov     ecx,000000c7H
L41:            mov     edx,dword ptr +14H[esp]
                cmp     edx,dword ptr __y1
                jge     short L42
                mov     dword ptr __y1,edx
L42:            cmp     ecx,dword ptr __y2
                jle     short L43
                mov     dword ptr __y2,ecx
L43:            mov     ebx,dword ptr +14H[esp]
                lea     edx,+0H[ebx*4]
                add     edx,ebx
                mov     ebx,offset DGROUP:_Edge
                shl     edx,02H
                add     ebx,edx
                mov     edx,dword ptr +14H[esp]
                cmp     ecx,edx
                jle     short L45
L44:            mov     edi,eax
                add     ebx,00000014H
                inc     edx
                sar     edi,10H
                add     eax,esi
                mov     dword ptr -10H[ebx],edi
                cmp     edx,ecx
                jl      short L44
L45:            add     esp,00000018H
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
                mov     edx,dword ptr +14H[eax]
                mov     dword ptr [esp],edx
                call    near ptr float2fixed_
                mov     edx,eax
                sar     edx,10H
                test    ax,0ffffH
                je      short L46
                inc     edx
L46:            mov     dword ptr +54H[esp],edx
                sub     esp,00000004H
                mov     eax,dword ptr +14H[esi]
                mov     dword ptr [esp],eax
                call    near ptr float2fixed_
                mov     edx,eax
                sar     eax,10H
                test    dx,0ffffH
                je      short L47
                inc     eax
L47:            mov     edx,dword ptr +54H[esp]
                mov     edi,eax
                cmp     eax,edx
                je      near ptr L51
                fld     dword ptr +14H[esi]
                fsub    dword ptr +14H[ebx]
                fadd    qword ptr L282
                fstp    qword ptr +10H[esp]
                mov     edx,dword ptr +10H[esp]
                mov     ecx,dword ptr __DivTable
                add     edx,80000000H
                fld     dword ptr +10H[esi]
                mov     edx,dword ptr [ecx+edx*4]
                fsub    dword ptr +10H[ebx]
                mov     dword ptr +3cH[esp],edx
                fmul    dword ptr +3cH[esp]
                fld     dword ptr +2cH[esi]
                fld     dword ptr +28H[esi]
                fsub    dword ptr +28H[ebx]
                fxch    st(1)
                fsub    dword ptr +2cH[ebx]
                fxch    st(1)
                fmul    dword ptr +3cH[esp]
                fxch    st(1)
                fmul    dword ptr +3cH[esp]
                fild    dword ptr +54H[esp]
                fsub    dword ptr +14H[ebx]
                fld     st(3)
                fmul    st,st(1)
                fld     st(3)
                fmul    st,st(2)
                fld     st(3)
                fmulp   st(3),st
                fxch    st(5)
                fld     qword ptr L283
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
                fadd    dword ptr +10H[ebx]
                fxch    st(3)
                fadd    dword ptr +28H[ebx]
                fxch    st(2)
                fadd    dword ptr +2cH[ebx]
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
                jg      near ptr L51
                test    eax,eax
                jl      near ptr L51
                test    esi,esi
                jge     short L48
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
L48:            cmp     edi,000000c7H
                jle     short L49
                mov     edi,000000c7H
L49:            mov     esi,dword ptr +54H[esp]
                lea     eax,+0H[esi*4]
                add     eax,esi
                shl     eax,02H
                add     eax,offset DGROUP:_Edge
                cmp     edi,esi
                jle     short L51
L50:            mov     ebp,ecx
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
                jl      short L50
L51:            add     esp,00000064H
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
                mov     edx,dword ptr +14H[eax]
                mov     dword ptr [esp],edx
                call    near ptr float2fixed_
                mov     edx,eax
                sar     edx,10H
                test    ax,0ffffH
                je      short L52
                inc     edx
L52:            mov     dword ptr +64H[esp],edx
                sub     esp,00000004H
                mov     eax,dword ptr +14H[edi]
                mov     dword ptr [esp],eax
                call    near ptr float2fixed_
                mov     edx,eax
                sar     eax,10H
                test    dx,0ffffH
                je      short L53
                inc     eax
L53:            mov     edx,dword ptr +64H[esp]
                mov     dword ptr +78H[esp],eax
                cmp     eax,edx
                je      near ptr L57
                fld     dword ptr +14H[edi]
                fld     dword ptr +10H[edi]
                fld     dword ptr +28H[edi]
                fxch    st(2)
                fsub    dword ptr +14H[esi]
                fxch    st(1)
                fsub    dword ptr +10H[esi]
                fxch    st(2)
                fsub    dword ptr +28H[esi]
                fld1    
                fdivrp  st(2),st
                fxch    st(2)
                fmul    st,st(1)
                fxch    st(2)
                fmul    st,st(1)
                fld     dword ptr +30H[edi]
                fld     dword ptr +2cH[edi]
                fsub    dword ptr +2cH[esi]
                fxch    st(1)
                fsub    dword ptr +30H[esi]
                fxch    st(1)
                fmul    st,st(3)
                fxch    st(1)
                fmulp   st(3),st
                fild    dword ptr +64H[esp]
                fsub    dword ptr +14H[esi]
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
                fld     qword ptr L284
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
                fadd    dword ptr +10H[esi]
                fxch    st(1)
                fadd    dword ptr +28H[esi]
                fxch    st(3)
                fadd    dword ptr +2cH[esi]
                fxch    st(4)
                fadd    dword ptr +30H[esi]
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
                jg      near ptr L57
                test    eax,eax
                jl      near ptr L57
                test    edi,edi
                jge     short L54
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
L54:            cmp     dword ptr +78H[esp],000000c7H
                jle     short L55
                mov     dword ptr +78H[esp],000000c7H
L55:            mov     edi,dword ptr +64H[esp]
                lea     eax,+0H[edi*4]
                add     eax,edi
                shl     eax,02H
                add     eax,offset DGROUP:_Edge
                cmp     edi,dword ptr +78H[esp]
                jge     short L57
L56:            mov     ebp,ebx
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
                jl      short L56
L57:            add     esp,0000007cH
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
                je      near ptr L63
                cmp     eax,000000c7H
                jg      near ptr L63
                test    edx,edx
                jl      near ptr L63
                cmp     eax,edx
                jge     near ptr L63
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
L58:            mov     ecx,dword ptr +8H[esp]
                mov     eax,dword ptr +8H[esp]
                mov     ebx,dword ptr +8H[esp]
                mov     edx,dword ptr +8H[esp]
                mov     ecx,dword ptr _Edge+4H[ecx]
                mov     eax,dword ptr _Edge+8H[eax]
                mov     ebx,dword ptr _Edge[ebx]
                mov     edx,dword ptr _Edge+0cH[edx]
                cmp     ebx,0000013fH
                jg      near ptr L62
                test    ecx,ecx
                jl      near ptr L62
                cmp     ecx,0000013fH
                jle     short L59
                mov     ecx,0000013fH
L59:            test    ebx,ebx
                jge     short L60
                mov     esi,dword ptr +0cH[esp]
                neg     ebx
                imul    esi,ebx
                add     eax,esi
                imul    ebx,dword ptr +10H[esp]
                add     edx,ebx
                xor     ebx,ebx
L60:            sub     ecx,ebx
                mov     dword ptr +14H[esp],ecx
                test    ecx,ecx
                jle     short L62
                mov     ecx,dword ptr +4H[esp]
                mov     ebp,dword ptr +14H[esp]
                add     ecx,ebx
                mov     ebx,dword ptr __Buffer
                xor     esi,esi
                add     ebx,ecx
                test    ebp,ebp
                jle     short L62
                mov     ebp,dword ptr +14H[esp]
L61:            mov     ecx,edx
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
                jl      short L61
L62:            mov     edx,dword ptr +8H[esp]
                mov     ebx,dword ptr +4H[esp]
                mov     ecx,dword ptr [esp]
                add     edx,00000014H
                add     ebx,00000140H
                mov     dword ptr +8H[esp],edx
                mov     dword ptr +4H[esp],ebx
                cmp     ebx,ecx
                jl      near ptr L58
L63:            add     esp,00000018H
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
                je      near ptr L69
                cmp     eax,000000c7H
                jg      near ptr L69
                test    edx,edx
                jl      near ptr L69
                cmp     eax,edx
                jge     near ptr L69
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
L64:            mov     esi,dword ptr +0cH[esp]
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
                jg      near ptr L68
                test    esi,esi
                jl      near ptr L68
                cmp     esi,0000013fH
                jle     short L65
                mov     esi,0000013fH
L65:            test    ecx,ecx
                jge     short L66
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
L66:            sub     esi,ecx
                mov     dword ptr +18H[esp],esi
                test    esi,esi
                jle     near ptr L68
                mov     esi,dword ptr +8H[esp]
                mov     ebp,dword ptr +18H[esp]
                add     ecx,esi
                mov     esi,dword ptr __Buffer
                xor     edi,edi
                add     esi,ecx
                test    ebp,ebp
                jle     short L68
L67:            mov     ecx,edx
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
                jl      short L67
L68:            mov     edx,dword ptr +0cH[esp]
                mov     ebx,dword ptr +8H[esp]
                mov     ecx,dword ptr +4H[esp]
                add     edx,00000014H
                add     ebx,00000140H
                mov     dword ptr +0cH[esp],edx
                mov     dword ptr +8H[esp],ebx
                cmp     ebx,ecx
                jl      near ptr L64
L69:            add     esp,0000001cH
                pop     ebp
                pop     edi
                pop     esi
                ret     0004H
                lea     eax,+0H[eax]
VesaGTexFillPoly_:
                push    esi
                push    edi
                push    ebp
                sub     esp,0000001cH
                mov     esi,eax
                mov     dword ptr +10H[esp],ebx
                mov     dword ptr +14H[esp],ecx
                cmp     eax,edx
                je      near ptr L75
                cmp     eax,000000c7H
                jg      near ptr L75
                test    edx,edx
                jl      near ptr L75
                cmp     eax,edx
                jge     near ptr L75
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
L70:            mov     esi,dword ptr +0cH[esp]
                mov     edx,dword ptr +0cH[esp]
                mov     eax,dword ptr +0cH[esp]
                mov     ecx,dword ptr +0cH[esp]
                mov     ebx,dword ptr +0cH[esp]
                mov     esi,dword ptr _Edge+4H[esi]
                mov     edx,dword ptr _Edge+8H[edx]
                mov     eax,dword ptr _Edge+0cH[eax]
                mov     ecx,dword ptr _Edge[ecx]
                mov     ebx,dword ptr _Edge+10H[ebx]
                cmp     ecx,0000013fH
                jg      near ptr L74
                test    esi,esi
                jl      near ptr L74
                cmp     esi,0000013fH
                jle     short L71
                mov     esi,0000013fH
L71:            test    ecx,ecx
                jge     short L72
                mov     edi,dword ptr +10H[esp]
                neg     ecx
                imul    edi,ecx
                add     edx,edi
                mov     edi,dword ptr +14H[esp]
                imul    edi,ecx
                add     eax,edi
                imul    ecx,dword ptr +2cH[esp]
                add     ebx,ecx
                xor     ecx,ecx
L72:            sub     esi,ecx
                mov     dword ptr +18H[esp],esi
                test    esi,esi
                jle     near ptr L74
                add     ecx,dword ptr +8H[esp]
                mov     esi,dword ptr __VesaBuffer
                add     ecx,ecx
                mov     edi,dword ptr +18H[esp]
                add     esi,ecx
                xor     ecx,ecx
                test    edi,edi
                jle     short L74
L73:            mov     edi,eax
                sar     edi,08H
                and     edi,0000ff00H
                mov     dword ptr [esp],edi
                mov     edi,edx
                sar     edi,10H
                mov     ebp,dword ptr [esp]
                and     edi,000000ffH
                add     edi,ebp
                mov     ebp,dword ptr __Texture1
                add     edi,ebp
                movsx   edi,byte ptr [edi]
                mov     dword ptr [esp],edi
                mov     edi,ebx
                sar     edi,08H
                mov     ebp,dword ptr [esp]
                and     edi,0000ff00H
                add     edi,ebp
                lea     ebp,+0H[edi*2]
                mov     edi,dword ptr __VesaShadeTable
                add     esi,00000002H
                add     ebp,edi
                inc     ecx
                mov     di,word ptr +0H[ebp]
                mov     ebp,dword ptr +10H[esp]
                mov     word ptr -2H[esi],di
                mov     edi,dword ptr +14H[esp]
                add     edx,ebp
                mov     ebp,dword ptr +2cH[esp]
                add     eax,edi
                mov     edi,dword ptr +18H[esp]
                add     ebx,ebp
                cmp     ecx,edi
                jl      short L73
L74:            mov     edx,dword ptr +0cH[esp]
                mov     ebx,dword ptr +8H[esp]
                mov     ecx,dword ptr +4H[esp]
                add     edx,00000014H
                add     ebx,00000140H
                mov     dword ptr +0cH[esp],edx
                mov     dword ptr +8H[esp],ebx
                cmp     ebx,ecx
                jl      near ptr L70
L75:            add     esp,0000001cH
                pop     ebp
                pop     edi
                pop     esi
                ret     0004H
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
                fld     dword ptr +14H[eax]
                mov     eax,dword ptr [esp]
                mov     dword ptr __y2,ebx
                xor     edx,edx
                fcomp   dword ptr +14H[eax]
                fstsw   ax
                sahf    
                jae     short L76
                mov     edx,00000001H
L76:            mov     eax,dword ptr +8H[esp]
                mov     ebx,dword ptr [esp+edx*4]
                fld     dword ptr +14H[eax]
                fcomp   dword ptr +14H[ebx]
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
                call    near ptr TexScanLeft_
                mov     edx,dword ptr [esp+ebx*4]
                mov     eax,dword ptr [esp+ecx]
                call    near ptr ScanRight_
                mov     ecx,dword ptr [esp+ebx*4]
                mov     ebx,dword ptr [esp+edi]
                fld     dword ptr +14H[ecx]
                fcomp   dword ptr +14H[ebx]
                fstsw   ax
                sahf    
                jae     short L80
                mov     edx,ebx
                mov     eax,ecx
                call    near ptr ScanRight_
                jmp     short L81
L80:            mov     edx,ecx
                mov     eax,ebx
                call    near ptr TexScanleft_
L81:            mov     eax,dword ptr +28H[esi]
                mov     eax,dword ptr +0d0H[eax]
                mov     dword ptr __Texture1,eax
                mov     eax,dword ptr +4H[esp]
                fld     dword ptr +14H[eax]
                mov     eax,dword ptr [esp]
                fsub    dword ptr +14H[eax]
                mov     eax,dword ptr +8H[esp]
                fld     dword ptr +14H[eax]
                mov     eax,dword ptr [esp]
                fsub    dword ptr +14H[eax]
                mov     eax,dword ptr +4H[esp]
                fld     dword ptr +10H[eax]
                mov     eax,dword ptr [esp]
                fsub    dword ptr +10H[eax]
                fmul    st,st(1)
                mov     eax,dword ptr +8H[esp]
                fld     dword ptr +10H[eax]
                mov     eax,dword ptr [esp]
                fsub    dword ptr +10H[eax]
                mov     eax,dword ptr +4H[esp]
                fld     dword ptr +28H[eax]
                mov     eax,dword ptr [esp]
                fsub    dword ptr +28H[eax]
                fmul    st,st(3)
                mov     eax,dword ptr +8H[esp]
                fld     dword ptr +28H[eax]
                mov     eax,dword ptr [esp]
                fsub    dword ptr +28H[eax]
                mov     eax,dword ptr +4H[esp]
                fld     dword ptr +2cH[eax]
                mov     eax,dword ptr [esp]
                fsub    dword ptr +2cH[eax]
                fmulp   st(5),st
                mov     eax,dword ptr +8H[esp]
                fxch    st(2)
                fmul    st,st(5)
                fld     dword ptr +2cH[eax]
                mov     eax,dword ptr [esp]
                fxch    st(3)
                fmul    st,st(6)
                fxch    st(3)
                fsub    dword ptr +2cH[eax]
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
                fadd    qword ptr L285
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
                nop     
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
                mov     ecx,dword ptr +14H[eax]
                mov     dword ptr __y1,edx
                cmp     ecx,43470000H
                jle     short L82
                mov     eax,dword ptr +4H[esp]
                cmp     dword ptr +14H[eax],43470000H
                jle     short L82
                mov     eax,dword ptr +8H[esp]
                cmp     dword ptr +14H[eax],43470000H
                jg      near ptr L90
L82:            mov     eax,dword ptr [esp]
                cmp     dword ptr +10H[eax],439f8000H
                jle     short L83
                mov     eax,dword ptr +4H[esp]
                cmp     dword ptr +10H[eax],439f8000H
                jle     short L83
                mov     eax,dword ptr +8H[esp]
                cmp     dword ptr +10H[eax],439f8000H
                jg      near ptr L90
L83:            mov     eax,dword ptr +4H[esp]
                fld     dword ptr +14H[eax]
                mov     eax,dword ptr [esp]
                xor     edx,edx
                fcomp   dword ptr +14H[eax]
                fstsw   ax
                sahf    
                jae     short L84
                mov     edx,00000001H
L84:            mov     eax,dword ptr +8H[esp]
                mov     ebx,dword ptr [esp+edx*4]
                fld     dword ptr +14H[eax]
                fcomp   dword ptr +14H[ebx]
                fstsw   ax
                sahf    
                jae     short L85
                mov     edx,00000002H
L85:            lea     ebx,+1H[edx]
                cmp     ebx,00000002H
                jle     short L86
                xor     ebx,ebx
L86:            lea     eax,-1H[edx]
                test    eax,eax
                jge     short L87
                mov     eax,00000002H
L87:            lea     edi,+0H[eax*4]
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
                fld     dword ptr +14H[ebx]
                fcomp   dword ptr +14H[ecx]
                fstsw   ax
                sahf    
                jae     short L88
                mov     edx,ecx
                mov     eax,ebx
                call    near ptr ScanRight_
                jmp     short L89
L88:            mov     edx,ebx
                mov     eax,ecx
                call    near ptr GTexScanleft_
L89:            mov     eax,dword ptr +28H[esi]
                mov     eax,dword ptr +0d0H[eax]
                mov     dword ptr __Texture1,eax
                mov     eax,dword ptr +4H[esp]
                fld     dword ptr +14H[eax]
                mov     eax,dword ptr [esp]
                fsub    dword ptr +14H[eax]
                mov     eax,dword ptr +8H[esp]
                fld     dword ptr +14H[eax]
                mov     eax,dword ptr [esp]
                fsub    dword ptr +14H[eax]
                mov     eax,dword ptr +4H[esp]
                fld     dword ptr +10H[eax]
                mov     eax,dword ptr [esp]
                fsub    dword ptr +10H[eax]
                fxch    st(1)
                fstp    dword ptr +18H[esp]
                fmul    dword ptr +18H[esp]
                mov     eax,dword ptr +8H[esp]
                fld     dword ptr +10H[eax]
                mov     eax,dword ptr [esp]
                fsub    dword ptr +10H[eax]
                mov     eax,dword ptr +4H[esp]
                fld     dword ptr +28H[eax]
                mov     eax,dword ptr [esp]
                fsub    dword ptr +28H[eax]
                fmul    dword ptr +18H[esp]
                mov     eax,dword ptr +8H[esp]
                fld     dword ptr +28H[eax]
                mov     eax,dword ptr [esp]
                fsub    dword ptr +28H[eax]
                mov     eax,dword ptr +4H[esp]
                fld     dword ptr +2cH[eax]
                mov     eax,dword ptr [esp]
                fsub    dword ptr +2cH[eax]
                fmul    dword ptr +18H[esp]
                mov     eax,dword ptr +8H[esp]
                fxch    st(5)
                fstp    dword ptr +20H[esp]
                fld     dword ptr +2cH[eax]
                mov     eax,dword ptr [esp]
                fxch    st(3)
                fmul    dword ptr +20H[esp]
                fxch    st(3)
                fsub    dword ptr +2cH[eax]
                fxch    st(1)
                fmul    dword ptr +20H[esp]
                fxch    st(1)
                fmul    dword ptr +20H[esp]
                mov     eax,dword ptr +4H[esp]
                fxch    st(3)
                fsubp   st(4),st
                fld     dword ptr +30H[eax]
                mov     eax,dword ptr [esp]
                fld1    
                fxch    st(1)
                fsub    dword ptr +30H[eax]
                mov     eax,dword ptr +8H[esp]
                fmul    dword ptr +18H[esp]
                fxch    st(1)
                fdivrp  st(5),st
                fld     dword ptr +30H[eax]
                mov     eax,dword ptr [esp]
                fxch    st(2)
                fsubp   st(3),st
                fxch    st(1)
                fsub    dword ptr +30H[eax]
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
                fadd    qword ptr L286
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
                call    near ptr VesaGTexFillPoly_
L90:            add     esp,0000002cH
                pop     ebp
                pop     edi
                pop     esi
                pop     edx
                pop     ecx
                pop     ebx
                ret     
Spline_Calc_:   push    edx
                sub     esp,0000000cH
                mov     edx,dword ptr +1cH[esp]
                fld     dword ptr +14H[esp]
                fld     st(0)
                fmul    st,st(1)
                fld     st(0)
                fmul    st,st(2)
                fld     dword ptr +10H[eax]
                fmul    st,st(1)
                fld     dword ptr +1cH[eax]
                fmul    st,st(3)
                faddp   st(1),st
                fld     dword ptr +28H[eax]
                fmul    st,st(4)
                faddp   st(1),st
                fadd    dword ptr +34H[eax]
                fld1    
                fdiv    dword ptr +18H[esp]
                fxch    st(1)
                fmul    st,st(1)
                fstp    dword ptr [edx]
                fld     dword ptr +14H[eax]
                fmul    st,st(2)
                fld     dword ptr +20H[eax]
                fmul    st,st(4)
                faddp   st(1),st
                fld     dword ptr +2cH[eax]
                fmul    st,st(5)
                faddp   st(1),st
                fadd    dword ptr +38H[eax]
                fmul    st,st(1)
                fstp    dword ptr +4H[edx]
                fld     dword ptr +18H[eax]
                fmulp   st(2),st
                fld     dword ptr +24H[eax]
                fmulp   st(3),st
                fxch    st(2)
                faddp   st(1),st
                fld     dword ptr +30H[eax]
                fmulp   st(3),st
                faddp   st(2),st
                fxch    st(1)
                fadd    dword ptr +3cH[eax]
                fmulp   st(1),st
                fstp    dword ptr +8H[edx]
                add     esp,0000000cH
                pop     edx
                ret     000cH
                nop     
Spline_Coeffs_: push    ebx
                push    ecx
                push    esi
                push    edi
                push    ebp
                sub     esp,0000000cH
                lea     ebx,-1H[edx]
                mov     ecx,edx
                lea     esi,+1H[edx]
                lea     edi,+2H[edx]
                test    ebx,ebx
                jge     short L91
                add     ebx,dword ptr +10H[eax]
L91:            mov     ebp,dword ptr +10H[eax]
                cmp     esi,ebp
                jl      short L92
                sub     esi,ebp
L92:            mov     ebp,dword ptr +10H[eax]
                cmp     edi,ebp
                jl      short L93
                sub     edi,ebp
L93:            mov     ebp,dword ptr +18H[eax]
                shl     ecx,06H
                mov     dword ptr [esp],ebp
                add     ebp,ecx
                shl     ebx,06H
                fld     dword ptr +4H[ebp]
                mov     ebp,dword ptr [esp]
                fld     dword ptr L287
                fxch    st(1)
                fmul    st,st(1)
                add     ebp,ebx
                shl     esi,06H
                fsub    dword ptr +4H[ebp]
                mov     ebp,dword ptr [esp]
                add     ebp,esi
                fld     dword ptr +4H[ebp]
                fmul    st,st(2)
                shl     edi,06H
                mov     ebp,dword ptr [esp]
                shl     edx,06H
                add     ebp,edi
                fsubp   st(1),st
                fadd    dword ptr +4H[ebp]
                mov     ebp,dword ptr [esp]
                add     ebp,edx
                fstp    dword ptr +10H[ebp]
                mov     ebp,dword ptr +18H[eax]
                mov     dword ptr +8H[esp],ebp
                add     ebp,ebx
                fld     dword ptr +4H[ebp]
                mov     ebp,dword ptr +8H[esp]
                fld     dword ptr L288
                fxch    st(1)
                fmul    st,st(1)
                add     ebp,ecx
                fld     dword ptr +4H[ebp]
                mov     ebp,dword ptr +8H[esp]
                fmul    dword ptr L289
                add     ebp,esi
                fsubp   st(1),st
                fld     dword ptr +4H[ebp]
                mov     ebp,dword ptr +8H[esp]
                fmul    dword ptr L290
                add     ebp,edi
                faddp   st(1),st
                fsub    dword ptr +4H[ebp]
                mov     ebp,dword ptr +8H[esp]
                add     ebp,edx
                fstp    dword ptr +1cH[ebp]
                mov     ebp,dword ptr +18H[eax]
                mov     dword ptr +8H[esp],ebp
                add     ebp,esi
                mov     dword ptr +4H[esp],ebp
                mov     ebp,dword ptr +8H[esp]
                add     ebp,ebx
                mov     dword ptr [esp],ebp
                mov     ebp,dword ptr +4H[esp]
                fld     dword ptr +4H[ebp]
                mov     ebp,dword ptr [esp]
                fsub    dword ptr +4H[ebp]
                mov     ebp,dword ptr +8H[esp]
                add     ebp,edx
                fstp    dword ptr +28H[ebp]
                mov     ebp,dword ptr +18H[eax]
                mov     dword ptr [esp],ebp
                add     ebp,ecx
                fld     dword ptr +4H[ebp]
                mov     ebp,dword ptr [esp]
                fmulp   st(1),st
                add     ebp,edx
                fstp    dword ptr +34H[ebp]
                mov     ebp,dword ptr +18H[eax]
                mov     dword ptr [esp],ebp
                add     ebp,ecx
                fld     dword ptr +8H[ebp]
                mov     ebp,dword ptr [esp]
                fmul    st,st(1)
                add     ebp,ebx
                fsub    dword ptr +8H[ebp]
                mov     ebp,dword ptr [esp]
                add     ebp,esi
                fld     dword ptr +8H[ebp]
                mov     ebp,dword ptr [esp]
                fmulp   st(2),st
                add     ebp,edi
                fsubrp  st(1),st
                fadd    dword ptr +8H[ebp]
                mov     ebp,dword ptr [esp]
                add     ebp,edx
                fstp    dword ptr +14H[ebp]
                mov     ebp,dword ptr +18H[eax]
                mov     dword ptr [esp],ebp
                add     ebp,ebx
                fld     dword ptr +8H[ebp]
                mov     ebp,dword ptr [esp]
                fld     dword ptr L288
                fxch    st(1)
                fmul    st,st(1)
                add     ebp,ecx
                fld     dword ptr +8H[ebp]
                mov     ebp,dword ptr [esp]
                fld     dword ptr L289
                fxch    st(1)
                fmul    st,st(1)
                add     ebp,esi
                fsubp   st(2),st
                fld     dword ptr +8H[ebp]
                mov     ebp,dword ptr [esp]
                fld     dword ptr L290
                fxch    st(1)
                fmul    st,st(1)
                add     ebp,edi
                faddp   st(3),st
                fxch    st(2)
                fsub    dword ptr +8H[ebp]
                mov     ebp,dword ptr [esp]
                add     ebp,edx
                fstp    dword ptr +20H[ebp]
                mov     ebp,dword ptr +18H[eax]
                mov     dword ptr [esp],ebp
                add     ebp,esi
                mov     dword ptr +4H[esp],ebp
                mov     ebp,dword ptr [esp]
                add     ebp,ebx
                mov     dword ptr +8H[esp],ebp
                mov     ebp,dword ptr +4H[esp]
                fld     dword ptr +8H[ebp]
                mov     ebp,dword ptr +8H[esp]
                fsub    dword ptr +8H[ebp]
                mov     ebp,dword ptr [esp]
                add     ebp,edx
                fstp    dword ptr +2cH[ebp]
                mov     ebp,dword ptr +18H[eax]
                mov     dword ptr +8H[esp],ebp
                add     ebp,ecx
                fld     dword ptr +8H[ebp]
                mov     ebp,dword ptr +8H[esp]
                fmul    st,st(3)
                add     ebp,edx
                fstp    dword ptr +38H[ebp]
                mov     ebp,dword ptr +18H[eax]
                mov     dword ptr +8H[esp],ebp
                add     ebp,ecx
                fld     dword ptr +0cH[ebp]
                mov     ebp,dword ptr +8H[esp]
                fld     dword ptr L287
                fxch    st(1)
                fmul    st,st(1)
                add     ebp,ebx
                fsub    dword ptr +0cH[ebp]
                mov     ebp,dword ptr +8H[esp]
                add     ebp,esi
                fld     dword ptr +0cH[ebp]
                mov     ebp,dword ptr +8H[esp]
                fmulp   st(2),st
                add     ebp,edi
                fsubrp  st(1),st
                fadd    dword ptr +0cH[ebp]
                mov     ebp,dword ptr +8H[esp]
                add     ebp,edx
                fstp    dword ptr +18H[ebp]
                mov     ebp,dword ptr +18H[eax]
                mov     dword ptr +8H[esp],ebp
                add     ebp,ebx
                fld     dword ptr +0cH[ebp]
                mov     ebp,dword ptr +8H[esp]
                fmul    st,st(3)
                add     ebp,ecx
                fld     dword ptr +0cH[ebp]
                fmulp   st(2),st
                mov     ebp,dword ptr +8H[esp]
                fsubrp  st(1),st
                add     ebp,esi
                fld     dword ptr +0cH[ebp]
                fmulp   st(2),st
                mov     ebp,dword ptr +8H[esp]
                faddp   st(1),st
                add     edi,ebp
                fsub    dword ptr +0cH[edi]
                mov     edi,ebp
                fstp    dword ptr +24H[edx+edi]
                mov     edi,dword ptr +18H[eax]
                fld     dword ptr +0cH[edi+esi]
                fsub    dword ptr +0cH[edi+ebx]
                fstp    dword ptr +30H[edx+edi]
                mov     eax,dword ptr +18H[eax]
                fmul    dword ptr +0cH[ecx+eax]
                fstp    dword ptr +3cH[edx+eax]
                add     esp,0000000cH
                pop     ebp
                pop     edi
                pop     esi
                pop     ecx
                pop     ebx
                ret     
                lea     eax,+0H[eax]
Spline_Init_:   push    ebx
                push    ecx
                push    edx
                mov     ecx,eax
                mov     edx,dword ptr +10H[eax]
                xor     ebx,ebx
                test    edx,edx
                jle     short L95
L94:            mov     edx,ebx
                mov     eax,ecx
                call    near ptr Spline_Coeffs_
                inc     ebx
                cmp     ebx,dword ptr +10H[ecx]
                jl      short L94
L95:            pop     edx
                pop     ecx
                pop     ebx
                ret     
                lea     eax,+0H[eax]
Spline_GetValues_:
                push    ebx
                push    ecx
                push    esi
                push    ebp
                sub     esp,00000008H
                mov     esi,eax
                mov     ecx,dword ptr +0cH[edx]
                mov     ebx,dword ptr +10H[edx]
                lea     eax,+1H[ecx]
                cmp     eax,ebx
                jl      short L96
                sub     eax,ebx
L96:            shl     eax,06H
                mov     ebx,dword ptr +18H[edx]
                shl     ecx,06H
                mov     eax,dword ptr [ebx+eax]
                mov     ebp,dword ptr [ecx+ebx]
                sub     eax,ebp
                mov     dword ptr +4H[esp],eax
                fild    dword ptr +4H[esp]
                fild    dword ptr [ecx+ebx]
                fsubr   dword ptr +4H[edx]
                fdivrp  st(1),st
                mov     eax,dword ptr +0cH[edx]
                push    esi
                shl     eax,06H
                push    40000000H
                add     eax,ebx
                fstp    dword ptr +8H[esp]
                push    dword ptr +8H[esp]
                call    near ptr Spline_Calc_
                add     esp,00000008H
                pop     ebp
                pop     esi
                pop     ecx
                pop     ebx
                ret     
Spline_Add_:    push    ebx
                push    edx
                push    edi
                mov     edx,eax
                fld     dword ptr +8H[edx]
                fadd    dword ptr +4H[eax]
                fstp    dword ptr +4H[eax]
                mov     eax,dword ptr +0cH[eax]
                mov     ebx,dword ptr +10H[edx]
                inc     eax
                cmp     eax,ebx
                jl      short L97
                sub     eax,ebx
L97:            shl     eax,06H
                mov     ebx,dword ptr +18H[edx]
                fild    dword ptr [ebx+eax]
                fcomp   dword ptr +4H[edx]
                fstsw   ax
                sahf    
                jae     short L98
                inc     dword ptr +0cH[edx]
L98:            mov     edi,dword ptr +10H[edx]
                cmp     edi,dword ptr +0cH[edx]
                jne     short L99
                mov     dword ptr +4H[edx],00000000H
                mov     dword ptr +0cH[edx],00000000H
L99:            pop     edi
                pop     edx
                pop     ebx
                ret     
                lea     eax,+0H[eax]
Light_Calc_:    push    ebx
                push    ecx
                push    edx
                push    esi
                push    edi
                push    ebp
                sub     esp,00000120H
                mov     ecx,eax
                lea     edi,+3cH[eax]
                xor     esi,esi
                xor     ebp,ebp
                jmp     short L101
L100:           mov     edx,edi
                mov     ebx,dword ptr +10H[eax]
                mov     eax,esp
                call    near ptr Matrix_Inverse_
                mov     eax,esp
                add     ebx,esi
                push    eax
                mov     eax,dword ptr +3cH[ebx]
                push    eax
                mov     edx,dword ptr +38H[ebx]
                push    edx
                mov     eax,dword ptr +34H[ebx]
                push    eax
                lea     eax,+40H[ebx]
                call    near ptr Vector_Trans_
                mov     eax,esp
                push    eax
                mov     edx,dword ptr +18H[ebx]
                push    edx
                mov     eax,dword ptr +14H[ebx]
                push    eax
                mov     edx,dword ptr +10H[ebx]
                inc     ebp
                push    edx
                lea     eax,+1cH[ebx]
                add     esi,000000ccH
                call    near ptr Vector_Trans_
L101:           mov     eax,dword ptr _World
                cmp     ebp,dword ptr +0cH[eax]
                jl      short L100
                xor     ebx,ebx
                mov     esi,dword ptr +0c0H[ecx]
                mov     dword ptr +0f4H[esp],ebx
                test    esi,esi
                jle     near ptr L117
                mov     dword ptr +0f8H[esp],ebx
L102:           mov     edx,dword ptr +0f8H[esp]
                mov     esi,dword ptr +0c4H[ecx]
                add     esi,edx
                test    byte ptr [esi],01H
                je      near ptr L114
                lea     edi,+88H[esp]
                mov     dword ptr +0fcH[esp],edx
                mov     dword ptr +100H[esp],edx
                lea     esi,+1cH[esi]
                xor     ebx,ebx
                xor     ebp,ebp
                mov     dword ptr +118H[esp],ebx
                mov     dword ptr +104H[esp],ebx
                movsd   
                movsd   
                movsd   
                jmp     near ptr L113
L103:           mov     ebx,dword ptr +10H[ebx]
                add     ebx,ebp
                mov     esi,dword ptr [ebx]
                cmp     esi,00000002H
                jb      short L104
                jbe     near ptr L106
                cmp     esi,00000004H
                je      near ptr L108
                jmp     near ptr L112
L104:           cmp     esi,00000001H
                jne     near ptr L112
                lea     edi,+0b8H[esp]
                lea     esi,+1cH[ebx]
                movsd   
                movsd   
                movsd   
                lea     edi,+94H[esp]
                lea     esi,+88H[esp]
                movsd   
                movsd   
                movsd   
                lea     edi,+40H[esp]
                lea     esi,+0b8H[esp]
                movsd   
                movsd   
                movsd   
                fld     dword ptr +98H[esp]
                fmul    dword ptr +44H[esp]
                fld     dword ptr +94H[esp]
                fmul    dword ptr +40H[esp]
                faddp   st(1),st
                fld     dword ptr +9cH[esp]
                fmul    dword ptr +48H[esp]
                faddp   st(1),st
                fmul    dword ptr +8cH[ebx]
                fldz    
                fxch    st(1)
                fstp    dword ptr +108H[esp]
                fcomp   dword ptr +108H[esp]
                fstsw   ax
                sahf    
                jbe     short L105
                xor     edx,edx
                mov     dword ptr +108H[esp],edx
L105:           fld     dword ptr +118H[esp]
                fadd    dword ptr +108H[esp]
                jmp     near ptr L111
L106:           lea     edi,+0c4H[esp]
                lea     esi,+40H[ebx]
                movsd   
                movsd   
                movsd   
                mov     edi,dword ptr +100H[esp]
                mov     eax,dword ptr +0c4H[ecx]
                add     eax,edi
                lea     edi,+0b8H[esp]
                lea     esi,+4H[eax]
                movsd   
                movsd   
                movsd   
                lea     edi,+4cH[esp]
                lea     esi,+0c4H[esp]
                lea     edx,+70H[esp]
                movsd   
                movsd   
                movsd   
                lea     eax,+7cH[esp]
                fld     dword ptr +4cH[esp]
                lea     edi,+0dcH[esp]
                lea     esi,+0b8H[esp]
                fld     dword ptr +50H[esp]
                movsd   
                movsd   
                movsd   
                fld     dword ptr +54H[esp]
                lea     edi,+58H[esp]
                lea     esi,+88H[esp]
                fxch    st(2)
                fsub    dword ptr +0dcH[esp]
                fxch    st(1)
                fsub    dword ptr +0e0H[esp]
                fxch    st(2)
                fsub    dword ptr +0e4H[esp]
                fxch    st(1)
                fstp    dword ptr +70H[esp]
                movsd   
                movsd   
                movsd   
                fxch    st(1)
                fstp    dword ptr +74H[esp]
                lea     edi,+0a0H[esp]
                lea     esi,+7cH[esp]
                fstp    dword ptr +78H[esp]
                call    near ptr Vector_Normalize_
                movsd   
                movsd   
                movsd   
                fld     dword ptr +5cH[esp]
                fmul    dword ptr +0a4H[esp]
                fld     dword ptr +58H[esp]
                fmul    dword ptr +0a0H[esp]
                faddp   st(1),st
                fld     dword ptr +60H[esp]
                fmul    dword ptr +0a8H[esp]
                faddp   st(1),st
                fmul    dword ptr +8cH[ebx]
                fldz    
                fxch    st(1)
                fstp    dword ptr +10cH[esp]
                fcomp   dword ptr +10cH[esp]
                fstsw   ax
                sahf    
                jbe     short L107
                xor     eax,eax
                mov     dword ptr +10cH[esp],eax
L107:           fld     dword ptr +118H[esp]
                fadd    dword ptr +10cH[esp]
                jmp     near ptr L111
L108:           lea     edi,+0b8H[esp]
                lea     esi,+40H[ebx]
                movsd   
                movsd   
                movsd   
                mov     eax,dword ptr +100H[esp]
                mov     esi,dword ptr +0c4H[ecx]
                add     esi,eax
                lea     edi,+0c4H[esp]
                lea     esi,+4H[esi]
                movsd   
                movsd   
                movsd   
                lea     edi,+64H[esp]
                lea     esi,+0b8H[esp]
                lea     edx,+70H[esp]
                movsd   
                movsd   
                movsd   
                lea     eax,+7cH[esp]
                fld     dword ptr +64H[esp]
                lea     edi,+0e8H[esp]
                lea     esi,+0c4H[esp]
                fld     dword ptr +68H[esp]
                movsd   
                movsd   
                movsd   
                fld     dword ptr +6cH[esp]
                lea     edi,+0acH[esp]
                lea     esi,+88H[esp]
                fxch    st(2)
                fsub    dword ptr +0e8H[esp]
                fxch    st(1)
                fsub    dword ptr +0ecH[esp]
                fxch    st(2)
                fsub    dword ptr +0f0H[esp]
                fxch    st(1)
                fstp    dword ptr +70H[esp]
                movsd   
                movsd   
                movsd   
                fxch    st(1)
                fstp    dword ptr +74H[esp]
                lea     edi,+0d0H[esp]
                lea     esi,+7cH[esp]
                fstp    dword ptr +78H[esp]
                call    near ptr Vector_Normalize_
                movsd   
                movsd   
                movsd   
                fld     dword ptr +0b0H[esp]
                fmul    dword ptr +0d4H[esp]
                fld     dword ptr +0acH[esp]
                fmul    dword ptr +0d0H[esp]
                faddp   st(1),st
                fld     dword ptr +0b4H[esp]
                fmul    dword ptr +0d8H[esp]
                faddp   st(1),st
                fmul    dword ptr +8cH[ebx]
                fldz    
                fxch    st(1)
                fstp    dword ptr +114H[esp]
                fcomp   dword ptr +114H[esp]
                fstsw   ax
                sahf    
                jbe     short L109
                xor     edx,edx
                mov     dword ptr +114H[esp],edx
L109:           mov     esi,dword ptr +0fcH[esp]
                mov     eax,dword ptr +0c4H[ecx]
                add     eax,esi
                lea     edx,+34H[ebx]
                add     eax,00000010H
                call    near ptr Vector_Distance_
                fstp    dword ptr +110H[esp]
                fld     dword ptr +90H[ebx]
                fsub    dword ptr +110H[esp]
                fldz    
                fxch    st(1)
                fstp    dword ptr +110H[esp]
                fcomp   dword ptr +110H[esp]
                fstsw   ax
                sahf    
                jae     short L110
                fld     dword ptr +110H[esp]
                fdiv    dword ptr +90H[ebx]
                fmul    dword ptr +114H[esp]
                fstp    dword ptr +114H[esp]
L110:           fld     dword ptr +118H[esp]
                fadd    dword ptr +114H[esp]
L111:           fstp    dword ptr +118H[esp]
L112:           mov     ebx,dword ptr +104H[esp]
                inc     ebx
                add     ebp,000000ccH
                mov     dword ptr +104H[esp],ebx
L113:           mov     ebx,dword ptr _World
                mov     eax,dword ptr +104H[esp]
                cmp     eax,dword ptr +0cH[ebx]
                jl      near ptr L103
L114:           mov     eax,dword ptr _World
                fld     dword ptr +118H[esp]
                fadd    dword ptr +1cH[eax]
                fmul    dword ptr L291
                fstp    dword ptr +118H[esp]
                cmp     dword ptr +118H[esp],42fe0000H
                jle     short L115
                mov     dword ptr +118H[esp],42fe0000H
L115:           fldz    
                fcomp   dword ptr +118H[esp]
                fstsw   ax
                sahf    
                jbe     short L116
                xor     ebp,ebp
                mov     dword ptr +118H[esp],ebp
L116:           push    dword ptr +118H[esp]
                call    near ptr float2int_
                mov     edi,dword ptr +0f4H[esp]
                movsx   ax,byte ptr _Intensity[eax]
                mov     ebx,dword ptr +0f8H[esp]
                mov     dword ptr +11cH[esp],eax
                mov     eax,dword ptr +0c4H[ecx]
                inc     edi
                add     eax,ebx
                fild    word ptr +11cH[esp]
                mov     dword ptr +0f4H[esp],edi
                fstp    dword ptr +30H[eax]
                lea     esi,+50H[ebx]
                mov     ebp,dword ptr +0c0H[ecx]
                mov     dword ptr +0f8H[esp],esi
                cmp     edi,ebp
                jl      near ptr L102
L117:           add     esp,00000120H
                pop     ebp
                pop     edi
                pop     esi
                pop     edx
                pop     ecx
                pop     ebx
                ret     
                nop     
Handle_Light_:  push    ebx
                push    ecx
                push    esi
                push    edi
                push    ebp
                mov     ebx,eax
                mov     eax,dword ptr [eax]
                lea     ecx,+4cH[ebx]
                cmp     eax,00000002H
                jb      short L118
                add     edx,00000038H
                lea     esi,+34H[ebx]
                cmp     eax,00000002H
                jbe     short L119
                cmp     eax,00000004H
                je      short L120
                jmp     near ptr L122
L118:           cmp     eax,00000001H
                je      near ptr L121
                jmp     near ptr L122
L119:           mov     eax,ecx
                call    near ptr Matrix_Identity_
                fld     dword ptr +28H[ebx]
                fld     dword ptr +2cH[ebx]
                fld     dword ptr +30H[ebx]
                mov     eax,ecx
                fxch    st(2)
                fadd    dword ptr +58H[ebx]
                fxch    st(1)
                fadd    dword ptr +68H[ebx]
                fxch    st(2)
                fadd    dword ptr +78H[ebx]
                fxch    st(1)
                fstp    dword ptr +58H[ebx]
                fxch    st(1)
                fstp    dword ptr +68H[ebx]
                fstp    dword ptr +78H[ebx]
                call    near ptr Matrix_Mult_
                push    ecx
                mov     ebp,dword ptr +30H[ebx]
                push    ebp
                mov     eax,dword ptr +2cH[ebx]
                push    eax
                mov     edx,dword ptr +28H[ebx]
                push    edx
                mov     eax,esi
                call    near ptr Vector_Trans_
                jmp     short L122
L120:           mov     eax,ecx
                call    near ptr Matrix_Identity_
                fld     dword ptr +28H[ebx]
                fld     dword ptr +2cH[ebx]
                fld     dword ptr +30H[ebx]
                mov     eax,ecx
                fxch    st(2)
                fadd    dword ptr +58H[ebx]
                fxch    st(1)
                fadd    dword ptr +68H[ebx]
                fxch    st(2)
                fadd    dword ptr +78H[ebx]
                fxch    st(1)
                fstp    dword ptr +58H[ebx]
                fxch    st(1)
                fstp    dword ptr +68H[ebx]
                fstp    dword ptr +78H[ebx]
                call    near ptr Matrix_Mult_
                push    ecx
                mov     edx,dword ptr +30H[ebx]
                push    edx
                mov     ecx,dword ptr +2cH[ebx]
                push    ecx
                mov     edi,dword ptr +28H[ebx]
                push    edi
                mov     eax,esi
                call    near ptr Vector_Trans_
                pop     ebp
                pop     edi
                pop     esi
                pop     ecx
                pop     ebx
                ret     
L121:           push    ecx
                mov     ecx,dword ptr +0cH[ebx]
                push    ecx
                mov     esi,dword ptr +8H[ebx]
                push    esi
                mov     edi,dword ptr +4H[ebx]
                push    edi
                lea     eax,+10H[ebx]
                call    near ptr Vector_inv_Trans3x3_
L122:           pop     ebp
                pop     edi
                pop     esi
                pop     ecx
                pop     ebx
                ret     
                lea     eax,+0H[eax]
Camera_CreateMatrix_Free_:
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
                sub     esp,00000004H
                fld     dword ptr +4H[edx]
                fchs    
                lea     eax,+38H[edx]
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
                lea     eax,+0cH[esp]
                fstp    dword ptr [esp]
                call    near ptr Matrix_Rotation_
                mov     eax,dword ptr _Camera
                mov     edx,esp
                add     eax,00000038H
                call    near ptr Matrix_Mult_
                add     esp,00000040H
                pop     edx
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
                push    esi
                mov     ebx,eax
                mov     esi,edx
                push    dword ptr +38H[eax]
                push    dword ptr +34H[eax]
                lea     ecx,+3cH[eax]
                push    dword ptr +30H[eax]
                mov     eax,ecx
                lea     edx,+7cH[ebx]
                call    near ptr Matrix_Rotation_
                mov     eax,ecx
                add     ecx,00000040H
L123:           add     edx,00000004H
                fld     dword ptr [eax]
                add     eax,00000004H
                fstp    dword ptr -4H[edx]
                cmp     eax,ecx
                jne     short L123
                fld     dword ptr +18H[ebx]
                fld     dword ptr +1cH[ebx]
                fld     dword ptr +20H[ebx]
                lea     ecx,+38H[esi]
                lea     eax,+3cH[ebx]
                mov     edx,ecx
                fxch    st(2)
                fadd    dword ptr +48H[ebx]
                fxch    st(1)
                fadd    dword ptr +58H[ebx]
                fxch    st(2)
                fadd    dword ptr +68H[ebx]
                fxch    st(1)
                fstp    dword ptr +48H[ebx]
                fxch    st(1)
                fstp    dword ptr +58H[ebx]
                fstp    dword ptr +68H[ebx]
                call    near ptr Matrix_Mult_
                push    ecx
                mov     edx,dword ptr +20H[ebx]
                push    edx
                mov     ecx,dword ptr +1cH[ebx]
                push    ecx
                mov     esi,dword ptr +18H[ebx]
                push    esi
                lea     eax,+24H[ebx]
                call    near ptr Vector_Trans_
                pop     esi
                pop     ecx
                pop     ebx
                ret     
Object_CreateMatrix_Inherit_:
                push    ecx
                push    esi
                push    edi
                push    ebp
                sub     esp,0000000cH
                mov     ecx,eax
                mov     esi,edx
                mov     edi,ebx
                push    dword ptr +38H[eax]
                push    dword ptr +34H[eax]
                lea     ebx,+3cH[eax]
                push    dword ptr +30H[eax]
                mov     eax,ebx
                lea     edx,+7cH[ecx]
                call    near ptr Matrix_Rotation_
                fld     dword ptr +18H[ecx]
                fld     dword ptr +1cH[ecx]
                fld     dword ptr +20H[ecx]
                mov     eax,ebx
                add     ebx,00000040H
                fxch    st(2)
                fadd    dword ptr +48H[ecx]
                fxch    st(1)
                fadd    dword ptr +58H[ecx]
                fxch    st(2)
                fadd    dword ptr +68H[ecx]
                fxch    st(1)
                fstp    dword ptr +48H[ecx]
                fxch    st(1)
                fstp    dword ptr +58H[ecx]
                fstp    dword ptr +68H[ecx]
L124:           add     edx,00000004H
                fld     dword ptr [eax]
                add     eax,00000004H
                fstp    dword ptr -4H[edx]
                cmp     eax,ebx
                jne     short L124
                lea     ebx,+3cH[edi]
                lea     eax,+3cH[ecx]
                mov     edx,ebx
                call    near ptr Matrix_Mult_
                push    ebx
                mov     edx,dword ptr +20H[ecx]
                push    edx
                mov     ebx,dword ptr +1cH[ecx]
                push    ebx
                mov     edi,dword ptr +18H[ecx]
                push    edi
                lea     eax,+10H[esp]
                add     esi,00000038H
                call    near ptr Vector_Trans_
                push    esi
                mov     ebp,dword ptr +0cH[esp]
                push    ebp
                mov     eax,dword ptr +0cH[esp]
                push    eax
                mov     edx,dword ptr +0cH[esp]
                push    edx
                lea     eax,+24H[ecx]
                call    near ptr Vector_Trans_
                add     esp,0000000cH
                pop     ebp
                pop     edi
                pop     esi
                pop     ecx
                ret     
                mov     eax,eax
Object_Visible_: push    ebx
                push    ecx
                sub     esp,0000000cH
                mov     ebx,eax
                fld     dword ptr +2cH[ebx]
                fadd    dword ptr +0bcH[eax]
                fcomp   dword ptr _min_clip_z
                fstsw   ax
                sahf    
                jae     short L125
                xor     eax,eax
                jmp     near ptr L131
L125:           fld     dword ptr +2cH[ebx]
                fsub    dword ptr +0bcH[ebx]
                fcomp   dword ptr _max_clip_z
                fstsw   ax
                sahf    
                jbe     short L126
                xor     eax,eax
                add     esp,0000000cH
                pop     ecx
                pop     ebx
                ret     
L126:           fld     dword ptr _halfscreenwidth_vd
                fmul    dword ptr +2cH[ebx]
                call    near ptr __CHP
                fistp   dword ptr +4H[esp]
                mov     eax,dword ptr +4H[esp]
                fld     dword ptr +24H[ebx]
                mov     edx,eax
                mov     dword ptr [esp],eax
                neg     edx
                fadd    dword ptr +0bcH[ebx]
                mov     dword ptr [esp],edx
                fild    dword ptr [esp]
                fcompp  st(1)
                fstsw   ax
                sahf    
                jbe     short L127
                xor     eax,eax
                add     esp,0000000cH
                pop     ecx
                pop     ebx
                ret     
L127:           fld     dword ptr +24H[ebx]
                fsub    dword ptr +0bcH[ebx]
                fild    dword ptr +4H[esp]
                fcompp  st(1)
                fstsw   ax
                sahf    
                jae     short L128
                xor     eax,eax
                add     esp,0000000cH
                pop     ecx
                pop     ebx
                ret     
L128:           fld     dword ptr _halfscreenheight_vd
                fmul    dword ptr +2cH[ebx]
                call    near ptr __CHP
                fistp   dword ptr +8H[esp]
                mov     eax,dword ptr +8H[esp]
                fld     dword ptr +28H[ebx]
                mov     ecx,eax
                mov     dword ptr [esp],eax
                neg     ecx
                fadd    dword ptr +0bcH[ebx]
                mov     dword ptr [esp],ecx
                fild    dword ptr [esp]
                fcompp  st(1)
                fstsw   ax
                sahf    
                jbe     short L129
                xor     eax,eax
                add     esp,0000000cH
                pop     ecx
                pop     ebx
                ret     
L129:           fld     dword ptr +28H[ebx]
                fsub    dword ptr +0bcH[ebx]
                fild    dword ptr +8H[esp]
                fcompp  st(1)
                fstsw   ax
                sahf    
                jae     short L130
                xor     eax,eax
                add     esp,0000000cH
                pop     ecx
                pop     ebx
                ret     
L130:           mov     eax,00000001H
L131:           add     esp,0000000cH
                pop     ecx
                pop     ebx
                ret     
                nop     
SortWorldObject_:
                push    ebx
                push    ecx
                push    edx
                push    esi
                push    edi
                push    ebp
                mov     ecx,eax
                mov     ebx,00000400H
                mov     eax,offset DGROUP:L309
                xor     edx,edx
                call    near ptr memset_
                mov     edx,dword ptr +14H[ecx]
                xor     eax,eax
                test    edx,edx
                jle     short L133
                xor     edx,edx
L132:           mov     ebx,dword ptr +20H[ecx]
                mov     ebx,dword ptr [edx+ebx]
                mov     ebx,dword ptr +24H[ebx]
                and     ebx,000000ffH
                inc     dword ptr L309[ebx*4]
                inc     eax
                mov     edi,dword ptr +14H[ecx]
                add     edx,00000004H
                cmp     eax,edi
                jl      short L132
L133:           xor     eax,eax
                xor     edx,edx
L134:           mov     si,word ptr L309[eax]
                add     eax,00000004H
                movsx   ebx,dx
                add     edx,esi
                mov     dword ptr L308[eax],ebx
                cmp     eax,00000400H
                jne     short L134
                mov     ebp,dword ptr +14H[ecx]
                xor     edx,edx
                test    ebp,ebp
                jle     short L136
                xor     ebx,ebx
L135:           mov     eax,dword ptr +20H[ecx]
                mov     esi,dword ptr [ebx+eax]
                mov     ax,word ptr +24H[esi]
                xor     ah,ah
                cwde    
                mov     di,word ptr L309[eax*4]
                movsx   edi,di
                mov     ebp,dword ptr +24H[ecx]
                shl     edi,02H
                add     edi,ebp
                mov     dword ptr [edi],esi
                mov     esi,dword ptr L309[eax*4]
                inc     esi
                add     ebx,00000004H
                mov     dword ptr L309[eax*4],esi
                inc     edx
                mov     edi,dword ptr +14H[ecx]
                cmp     edx,edi
                jl      short L135
L136:           mov     ebx,00000400H
                mov     eax,offset DGROUP:L309
                xor     edx,edx
                call    near ptr memset_
                mov     ebp,dword ptr +14H[ecx]
                xor     edx,edx
                test    ebp,ebp
                jle     short L138
                xor     eax,eax
L137:           mov     ebx,dword ptr +24H[ecx]
                mov     ebx,dword ptr [ebx+eax]
                mov     ebx,dword ptr +24H[ebx]
                and     ebx,0000ff00H
                sar     ebx,08H
                inc     dword ptr L309[ebx*4]
                inc     edx
                mov     edi,dword ptr +14H[ecx]
                add     eax,00000004H
                cmp     edx,edi
                jl      short L137
L138:           xor     eax,eax
                xor     edx,edx
L139:           mov     bx,word ptr L309[eax]
                add     eax,00000004H
                movsx   esi,dx
                add     edx,ebx
                mov     dword ptr L308[eax],esi
                cmp     eax,00000400H
                jne     short L139
                mov     ebp,dword ptr +14H[ecx]
                xor     ebx,ebx
                test    ebp,ebp
                jle     short L141
                xor     edx,edx
L140:           mov     eax,dword ptr +24H[ecx]
                mov     esi,dword ptr [edx+eax]
                mov     eax,dword ptr +24H[esi]
                and     eax,0000ff00H
                sar     eax,08H
                cwde    
                mov     di,word ptr L309[eax*4]
                movsx   edi,di
                mov     ebp,dword ptr +20H[ecx]
                shl     edi,02H
                add     edi,ebp
                mov     dword ptr [edi],esi
                mov     esi,dword ptr L309[eax*4]
                inc     esi
                add     edx,00000004H
                mov     dword ptr L309[eax*4],esi
                inc     ebx
                mov     edi,dword ptr +14H[ecx]
                cmp     ebx,edi
                jl      short L140
L141:           pop     ebp
                pop     edi
                pop     esi
                pop     edx
                pop     ecx
                pop     ebx
                ret     
                nop     
Handle_Object_: push    ebx
                push    ecx
                push    esi
                push    edi
                push    ebp
                sub     esp,00000018H
                mov     ebx,eax
                mov     dword ptr +14H[esp],edx
                test    eax,eax
                je      near ptr L153
                test    byte ptr [eax],01H
                je      near ptr L152
                mov     eax,ebx
                call    near ptr Object_Visible_
                cmp     eax,00000001H
                jne     near ptr L152
                mov     eax,dword ptr _World
                inc     dword ptr +18H[eax]
                mov     esi,dword ptr +0c0H[ebx]
                xor     ecx,ecx
                test    esi,esi
                jle     near ptr L145
                lea     edi,+3cH[ebx]
                xor     edx,edx
L142:           mov     eax,dword ptr +0c4H[ebx]
                mov     dword ptr [edx+eax],0ffffffffH
                mov     eax,dword ptr +0c4H[ebx]
                push    edi
                mov     esi,dword ptr +0cH[edx+eax]
                push    esi
                mov     ebp,dword ptr +8H[edx+eax]
                push    ebp
                mov     esi,dword ptr +4H[edx+eax]
                push    esi
                lea     eax,+10H[esp]
                call    near ptr Vector_Trans_
                mov     ebp,dword ptr +8H[esp]
                cmp     ebp,3f800000H
                jg      short L143
                mov     eax,dword ptr +0c4H[ebx]
                mov     dword ptr +18H[edx+eax],0bf800000H
                jmp     near ptr L144
L143:           mov     esi,dword ptr +0c4H[ebx]
                mov     dword ptr +18H[edx+esi],ebp
                fld1    
                fld     dword ptr _viewdistance
                fld     dword ptr [esp]
                fmul    st,st(1)
                fxch    st(2)
                fdiv    dword ptr +8H[esp]
                fxch    st(2)
                fmul    st,st(2)
                fadd    dword ptr L293
                mov     eax,dword ptr +0c4H[ebx]
                fstp    dword ptr +10H[edx+eax]
                fmul    dword ptr +4H[esp]
                fmulp   st(1),st
                fadd    dword ptr L294
                mov     eax,dword ptr +0c4H[ebx]
                fstp    dword ptr +14H[edx+eax]
                mov     eax,dword ptr +0c4H[ebx]
                push    dword ptr +10H[edx+eax]
                call    near ptr float2int_
                mov     esi,dword ptr +0c4H[ebx]
                mov     dword ptr +34H[edx+esi],eax
                mov     eax,dword ptr +0c4H[ebx]
                push    dword ptr +14H[edx+eax]
                call    near ptr float2int_
                mov     esi,dword ptr +0c4H[ebx]
                mov     dword ptr +38H[edx+esi],eax
                mov     eax,dword ptr +0c4H[ebx]
                push    dword ptr +28H[edx+eax]
                call    near ptr float2int_
                mov     esi,dword ptr +0c4H[ebx]
                mov     dword ptr +3cH[edx+esi],eax
                mov     eax,dword ptr +0c4H[ebx]
                push    dword ptr +2cH[edx+eax]
                call    near ptr float2int_
                mov     esi,dword ptr +0c4H[ebx]
                mov     dword ptr +40H[edx+esi],eax
                mov     eax,dword ptr +0c4H[ebx]
                push    dword ptr +30H[edx+eax]
                call    near ptr float2int_
                mov     esi,dword ptr +0c4H[ebx]
                mov     dword ptr +44H[edx+esi],eax
L144:           inc     ecx
                mov     ebp,dword ptr +0c0H[ebx]
                add     edx,00000050H
                cmp     ecx,ebp
                jl      near ptr L142
L145:           test    byte ptr +2H[ebx],01H
                je      near ptr L148
                mov     eax,dword ptr +0c0H[ebx]
                xor     ecx,ecx
                test    eax,eax
                jle     near ptr L148
                lea     edi,+3cH[ebx]
                xor     edx,edx
L146:           mov     eax,dword ptr +0c4H[ebx]
                add     eax,edx
                test    byte ptr [eax],01H
                je      near ptr L147
                push    edi
                mov     esi,dword ptr +24H[eax]
                push    esi
                mov     ebp,dword ptr +20H[eax]
                push    ebp
                mov     esi,dword ptr +1cH[eax]
                push    esi
                lea     eax,+10H[esp]
                call    near ptr Vector_Trans3x3_
                fld     dword ptr [esp]
                fmul    dword ptr L292
                fadd    dword ptr L295
                mov     eax,dword ptr +0c4H[ebx]
                fstp    dword ptr +28H[edx+eax]
                fld     dword ptr +4H[esp]
                fmul    dword ptr L292
                fadd    dword ptr L295
                mov     eax,dword ptr +0c4H[ebx]
                fstp    dword ptr +2cH[edx+eax]
                fld     dword ptr [esp]
                fmul    dword ptr L292
                fadd    dword ptr L295
                sub     esp,00000004H
                fstp    dword ptr [esp]
                call    near ptr float2int_
                mov     esi,dword ptr +0c4H[ebx]
                mov     dword ptr +3cH[edx+esi],eax
                fld     dword ptr +4H[esp]
                fmul    dword ptr L292
                fadd    dword ptr L295
                sub     esp,00000004H
                fstp    dword ptr [esp]
                call    near ptr float2int_
                mov     esi,dword ptr +0c4H[ebx]
                mov     dword ptr +40H[edx+esi],eax
                mov     eax,dword ptr +0c4H[ebx]
                mov     dword ptr +44H[edx+eax],0000003fH
L147:           inc     ecx
                mov     eax,dword ptr +0c0H[ebx]
                add     edx,00000050H
                cmp     ecx,eax
                jl      near ptr L146
L148:           mov     edi,dword ptr +0c8H[ebx]
                mov     ecx,dword ptr +0ccH[ebx]
                xor     ebp,ebp
                test    edi,edi
                jle     near ptr L152
L149:           mov     edx,dword ptr [ecx]
                mov     esi,dword ptr +4H[ecx]
                mov     edi,dword ptr +8H[ecx]
                cmp     dword ptr +18H[edx],0bf800000H
                je      short L151
                cmp     dword ptr +18H[esi],0bf800000H
                je      short L151
                cmp     dword ptr +18H[edi],0bf800000H
                je      short L151
                test    byte ptr [ebx],02H
                jne     short L150
                fld     dword ptr +10H[esi]
                fsub    dword ptr +10H[edx]
                fld     dword ptr +14H[edi]
                fsub    dword ptr +14H[edx]
                fmulp   st(1),st
                fld     dword ptr +10H[edi]
                fsub    dword ptr +10H[edx]
                fld     dword ptr +14H[esi]
                fsub    dword ptr +14H[edx]
                fmulp   st(1),st
                fsubp   st(1),st
                fldz    
                fcompp  st(1)
                fstsw   ax
                sahf    
                jb      short L151
L150:           or      byte ptr [edx],01H
                or      byte ptr [esi],01H
                or      byte ptr [edi],01H
                fld     dword ptr +18H[edx]
                fadd    dword ptr +18H[esi]
                fadd    dword ptr +18H[edi]
                mov     eax,dword ptr _World
                call    near ptr __CHP
                fistp   dword ptr +24H[ecx]
                mov     edx,dword ptr +14H[eax]
                mov     esi,dword ptr +20H[eax]
                mov     dword ptr [esi+edx*4],ecx
                inc     dword ptr +14H[eax]
                mov     dword ptr +28H[ecx],ebx
L151:           inc     ebp
                mov     eax,dword ptr +0c8H[ebx]
                add     ecx,0000002cH
                cmp     ebp,eax
                jl      near ptr L149
L152:           mov     edx,dword ptr +14H[esp]
                mov     eax,dword ptr +110H[ebx]
                call    near ptr Handle_Object_
L153:           add     esp,00000018H
                pop     ebp
                pop     edi
                pop     esi
                pop     ecx
                pop     ebx
                ret     
Render_Setup_:  push    ebx
                push    ecx
                push    edx
                mov     eax,dword ptr _World
                mov     dword ptr +14H[eax],00000000H
                mov     dword ptr +18H[eax],00000000H
                mov     eax,dword ptr +8H[eax]
                mov     edx,dword ptr [eax]
                cmp     edx,00000001H
                jb      short L156
                jbe     short L154
                cmp     edx,00000002H
                je      short L155
                jmp     short L156
L154:           call    near ptr Camera_CreateMatrix_Free_
                jmp     short L156
L155:           call    near ptr Camera_CreateMatrix_Aim_
L156:           mov     ecx,dword ptr _World
                mov     ecx,dword ptr +4H[ecx]
                test    ecx,ecx
                je      short L160
L157:           test    byte ptr [ecx],04H
                je      short L158
                mov     edx,dword ptr _World
                mov     eax,ecx
                mov     ebx,dword ptr +114H[ecx]
                mov     edx,dword ptr +8H[edx]
                call    near ptr Object_CreateMatrix_Inherit_
                jmp     short L159
L158:           mov     edx,dword ptr _World
                mov     eax,ecx
                mov     edx,dword ptr +8H[edx]
                call    near ptr Object_CreateMatrix_
L159:           mov     ecx,dword ptr +110H[ecx]
                test    ecx,ecx
                jne     short L157
L160:           mov     eax,dword ptr _World
                mov     edx,dword ptr +8H[eax]
                mov     eax,dword ptr +4H[eax]
                call    near ptr Handle_Object_
                mov     eax,dword ptr _World
                call    near ptr SortWorldObject_
                pop     edx
                pop     ecx
                pop     ebx
                ret     
                nop     
Render_Draw_:   push    ebx
                push    edx
                mov     ebx,dword ptr _World
                mov     ebx,dword ptr +14H[ebx]
                dec     ebx
                test    ebx,ebx
                jl      short L162
                lea     edx,+0H[ebx*4]
L161:           mov     eax,dword ptr _World
                sub     edx,00000004H
                mov     eax,dword ptr +20H[eax]
                mov     eax,dword ptr +4H[edx+eax]
                dec     ebx
                call    near ptr WobblePolyGouraudTexture_
                test    edx,edx
                jge     short L161
L162:           pop     edx
                pop     ebx
                ret     
                nop     
Load3DS_:       push    ebx
                push    ecx
                push    edx
                push    esi
                push    edi
                push    ebp
                sub     esp,000000c8H
                xor     edx,edx
                mov     dword ptr +9cH[esp],edx
                mov     edx,offset DGROUP:L296
                mov     ebx,00000001H
                call    near ptr fopen_
                mov     ecx,eax
                mov     ebp,eax
                mov     eax,dword ptr +10H[eax]
                mov     edx,00000006H
                call    near ptr filelength_
                mov     dword ptr +98H[esp],eax
                mov     eax,offset DGROUP:_chunk
                call    near ptr fread_
                cmp     word ptr _chunk,4d4dH
                je      short L163
                xor     eax,eax
                jmp     near ptr L201
L163:           test    byte ptr +0cH[ebp],10H
                jne     near ptr L196
                mov     eax,ebp
                mov     ebx,dword ptr +98H[esp]
                call    near ptr ftell_
                cmp     eax,ebx
                jge     near ptr L196
                mov     ebx,00000001H
                mov     edx,00000006H
                mov     eax,offset DGROUP:_chunk
                mov     ecx,ebp
                call    near ptr fread_
                mov     ax,word ptr _chunk
                cmp     ax,4700H
                jb      short L166
                jbe     near ptr L187
                cmp     ax,0b004H
                jb      short L165
                jbe     near ptr L195
                cmp     ax,0b020H
                jb      short L164
                jbe     near ptr L188
                cmp     ax,0b021H
                jbe     near ptr L195
                cmp     ax,0b022H
                je      near ptr L195
                jmp     near ptr L194
L164:           cmp     ax,0b010H
                je      near ptr L195
                jmp     near ptr L194
L165:           cmp     ax,0b002H
                jae     near ptr L195
                cmp     ax,0b000H
                je      near ptr L195
                jmp     near ptr L194
L166:           cmp     ax,4110H
                jb      short L168
                jbe     near ptr L176
                cmp     ax,4140H
                jb      short L167
                jbe     near ptr L182
                cmp     ax,4160H
                jb      near ptr L194
                jbe     near ptr L185
                cmp     ax,4600H
                je      near ptr L186
                jmp     near ptr L194
L167:           cmp     ax,4120H
                je      near ptr L179
                jmp     near ptr L194
L168:           cmp     ax,4000H
                jb      short L169
                jbe     short L170
                cmp     ax,4100H
                je      near ptr L195
                jmp     near ptr L194
L169:           cmp     ax,3d3dH
                je      near ptr L195
                jmp     near ptr L194
L170:           mov     eax,ebp
                mov     ecx,dword ptr +9cH[esp]
                call    near ptr ftell_
                test    ecx,ecx
                jne     short L171
                mov     eax,00000118H
                call    near ptr malloc_
                mov     dword ptr +90H[esp],eax
                mov     dword ptr +9cH[esp],eax
                jmp     short L172
L171:           mov     eax,00000118H
                mov     edx,dword ptr +90H[esp]
                call    near ptr malloc_
                mov     dword ptr +90H[esp],eax
                mov     dword ptr +110H[edx],eax
L172:           mov     eax,dword ptr +90H[esp]
                mov     dword ptr +18H[eax],38d1b717H
                mov     dword ptr +1cH[eax],38d1b717H
                mov     dword ptr +20H[eax],38d1b717H
                mov     dword ptr +30H[eax],38d1b717H
                mov     dword ptr +34H[eax],38d1b717H
                mov     dword ptr +38H[eax],38d1b717H
                mov     dword ptr +114H[eax],00000000H
                mov     dword ptr +110H[eax],00000000H
                mov     dword ptr +0c0H[eax],00000000H
                mov     dword ptr +0c8H[eax],00000000H
                xor     dl,dl
                mov     dword ptr [eax],00000001H
L173:           mov     eax,ebp
                call    near ptr fgetc_
                movsx   esi,dl
                inc     dl
                mov     byte ptr [esp+esi],al
                movsx   eax,dl
                cmp     byte ptr -1H[esp+eax],00H
                jne     short L173
                mov     edi,dword ptr +90H[esp]
                mov     esi,esp
                add     edi,00000004H
                push    edi
L174:           mov     al,byte ptr [esi]
                mov     byte ptr [edi],al
                cmp     al,00H
                je      short L175
                mov     al,byte ptr +1H[esi]
                add     esi,00000002H
                mov     byte ptr +1H[edi],al
                add     edi,00000002H
                cmp     al,00H
                jne     short L174
L175:           pop     edi
                jmp     near ptr L195
L176:           mov     ebx,00000001H
                mov     edx,00000002H
                lea     eax,+0b8H[esp]
                mov     ecx,ebp
                call    near ptr fread_
                xor     eax,eax
                mov     edx,dword ptr +90H[esp]
                mov     ax,word ptr +0b8H[esp]
                mov     dword ptr +0c0H[edx],eax
                xor     eax,eax
                mov     ax,word ptr +0b8H[esp]
                imul    eax,50H
                xor     edi,edi
                xor     esi,esi
                call    near ptr malloc_
                mov     dword ptr +0c4H[edx],eax
                jmp     near ptr L178
L177:           mov     ebx,00000001H
                mov     edx,00000004H
                lea     eax,+84H[esp]
                mov     ecx,ebp
                call    near ptr fread_
                mov     ebx,00000001H
                mov     edx,00000004H
                lea     eax,+80H[esp]
                mov     ecx,ebp
                call    near ptr fread_
                mov     ebx,00000001H
                mov     edx,00000004H
                lea     eax,+7cH[esp]
                mov     ecx,ebp
                call    near ptr fread_
                fld     dword ptr +84H[esp]
                mov     eax,dword ptr +90H[esp]
                fld     dword ptr +0e4H[esp]
                fxch    st(1)
                fmul    st,st(1)
                mov     eax,dword ptr +0c4H[eax]
                fstp    dword ptr +4H[esi+eax]
                fld     dword ptr +80H[esp]
                mov     eax,dword ptr +90H[esp]
                fmul    st,st(1)
                mov     eax,dword ptr +0c4H[eax]
                fstp    dword ptr +8H[esi+eax]
                fld     dword ptr +7cH[esp]
                mov     eax,dword ptr +90H[esp]
                fmulp   st(1),st
                mov     eax,dword ptr +0c4H[eax]
                fstp    dword ptr +0cH[esi+eax]
                mov     eax,dword ptr +90H[esp]
                mov     eax,dword ptr +0c4H[eax]
                mov     dword ptr +28H[esi+eax],00000000H
                mov     eax,dword ptr +90H[esp]
                mov     eax,dword ptr +0c4H[eax]
                mov     dword ptr +2cH[esi+eax],00000000H
                mov     eax,dword ptr +90H[esp]
                add     esi,00000050H
                mov     eax,dword ptr +0c4H[eax]
                inc     edi
                mov     dword ptr -20H[esi+eax],427c0000H
L178:           xor     eax,eax
                mov     ax,word ptr +0b8H[esp]
                cmp     edi,eax
                jge     near ptr L195
                jmp     near ptr L177
L179:           mov     ebx,00000001H
                mov     edx,00000002H
                lea     eax,+0b4H[esp]
                mov     ecx,ebp
                call    near ptr fread_
                xor     eax,eax
                mov     edx,dword ptr +90H[esp]
                mov     ax,word ptr +0b4H[esp]
                mov     dword ptr +0c8H[edx],eax
                xor     eax,eax
                mov     ax,word ptr +0b4H[esp]
                imul    eax,2cH
                xor     edi,edi
                xor     esi,esi
                call    near ptr malloc_
                mov     dword ptr +0ccH[edx],eax
                jmp     near ptr L181
L180:           mov     ebx,00000001H
                mov     edx,00000002H
                lea     eax,+0acH[esp]
                mov     ecx,ebp
                call    near ptr fread_
                mov     ebx,00000001H
                mov     edx,00000002H
                lea     eax,+0a8H[esp]
                mov     ecx,ebp
                call    near ptr fread_
                mov     ebx,00000001H
                mov     edx,00000002H
                lea     eax,+0c4H[esp]
                mov     ecx,ebp
                call    near ptr fread_
                mov     ebx,00000001H
                mov     edx,00000002H
                lea     eax,+0b0H[esp]
                mov     ecx,ebp
                call    near ptr fread_
                xor     eax,eax
                mov     ax,word ptr +0acH[esp]
                imul    eax,50H
                mov     edx,dword ptr +90H[esp]
                mov     edx,dword ptr +0c4H[edx]
                add     eax,edx
                mov     edx,dword ptr +90H[esp]
                mov     edx,dword ptr +0ccH[edx]
                mov     dword ptr [edx+esi],eax
                xor     eax,eax
                mov     ax,word ptr +0a8H[esp]
                imul    eax,50H
                mov     edx,dword ptr +90H[esp]
                mov     edx,dword ptr +0c4H[edx]
                add     edx,eax
                mov     eax,dword ptr +90H[esp]
                mov     eax,dword ptr +0ccH[eax]
                mov     dword ptr +4H[esi+eax],edx
                xor     eax,eax
                mov     ax,word ptr +0c4H[esp]
                imul    eax,50H
                mov     edx,dword ptr +90H[esp]
                mov     edx,dword ptr +0c4H[edx]
                add     edx,eax
                mov     eax,dword ptr +90H[esp]
                mov     eax,dword ptr +0ccH[eax]
                mov     dword ptr +8H[esi+eax],edx
                mov     eax,dword ptr +90H[esp]
                add     esi,0000002cH
                mov     eax,dword ptr +0ccH[eax]
                mov     edx,dword ptr +90H[esp]
                inc     edi
                mov     dword ptr -4H[esi+eax],edx
L181:           xor     eax,eax
                mov     ax,word ptr +0b4H[esp]
                cmp     edi,eax
                jge     near ptr L195
                jmp     near ptr L180
L182:           mov     eax,dword ptr +90H[esp]
                mov     ebx,00000001H
                mov     dl,byte ptr +1H[eax]
                mov     ecx,ebp
                or      dl,80H
                xor     edi,edi
                mov     byte ptr +1H[eax],dl
                mov     edx,00000002H
                lea     eax,+0a4H[esp]
                xor     esi,esi
                call    near ptr fread_
                jmp     short L184
L183:           mov     ebx,00000001H
                mov     edx,00000004H
                lea     eax,+1cH[esp]
                mov     ecx,ebp
                call    near ptr fread_
                mov     ebx,00000001H
                mov     edx,00000004H
                lea     eax,+18H[esp]
                mov     ecx,ebp
                call    near ptr fread_
                fld     dword ptr +1cH[esp]
                mov     eax,dword ptr +90H[esp]
                fld     dword ptr +0e8H[esp]
                fxch    st(1)
                fmul    st,st(1)
                mov     eax,dword ptr +0c4H[eax]
                fstp    dword ptr +28H[esi+eax]
                fmul    dword ptr +18H[esp]
                mov     eax,dword ptr +90H[esp]
                add     esi,00000050H
                mov     eax,dword ptr +0c4H[eax]
                inc     edi
                fstp    dword ptr -24H[esi+eax]
L184:           xor     eax,eax
                mov     ax,word ptr +0a4H[esp]
                cmp     edi,eax
                jl      short L183
L185:           mov     ebx,00000001H
                mov     edx,00000004H
                lea     eax,+78H[esp]
                mov     ecx,ebp
                call    near ptr fread_
                mov     ebx,00000001H
                mov     edx,00000004H
                lea     eax,+74H[esp]
                mov     ecx,ebp
                call    near ptr fread_
                mov     ebx,00000001H
                mov     edx,00000004H
                lea     eax,+70H[esp]
                mov     ecx,ebp
                call    near ptr fread_
                mov     ebx,00000001H
                mov     edx,00000004H
                lea     eax,+6cH[esp]
                mov     ecx,ebp
                call    near ptr fread_
                mov     ebx,00000001H
                mov     edx,00000004H
                lea     eax,+68H[esp]
                mov     ecx,ebp
                call    near ptr fread_
                mov     ebx,00000001H
                mov     edx,00000004H
                lea     eax,+64H[esp]
                mov     ecx,ebp
                call    near ptr fread_
                mov     ebx,00000001H
                mov     edx,00000004H
                lea     eax,+60H[esp]
                mov     ecx,ebp
                call    near ptr fread_
                mov     ebx,00000001H
                mov     edx,00000004H
                lea     eax,+5cH[esp]
                mov     ecx,ebp
                call    near ptr fread_
                mov     ebx,00000001H
                mov     edx,00000004H
                lea     eax,+58H[esp]
                mov     ecx,ebp
                call    near ptr fread_
                mov     ebx,00000001H
                mov     edx,00000004H
                lea     eax,+54H[esp]
                mov     ecx,ebp
                call    near ptr fread_
                mov     ebx,00000001H
                mov     edx,00000004H
                lea     eax,+50H[esp]
                mov     ecx,ebp
                call    near ptr fread_
                mov     ebx,00000001H
                mov     edx,00000004H
                lea     eax,+4cH[esp]
                mov     ecx,ebp
                call    near ptr fread_
                jmp     near ptr L195
L186:           mov     ebx,00000001H
                mov     edx,00000004H
                lea     eax,+48H[esp]
                mov     ecx,ebp
                call    near ptr fread_
                mov     ebx,00000001H
                mov     edx,00000004H
                lea     eax,+44H[esp]
                mov     ecx,ebp
                call    near ptr fread_
                mov     ebx,00000001H
                mov     edx,00000004H
                lea     eax,+40H[esp]
                mov     ecx,ebp
                call    near ptr fread_
                jmp     near ptr L195
L187:           mov     ebx,00000001H
                mov     edx,00000004H
                lea     eax,+3cH[esp]
                mov     ecx,ebp
                call    near ptr fread_
                mov     ebx,00000001H
                mov     edx,00000004H
                lea     eax,+38H[esp]
                mov     ecx,ebp
                call    near ptr fread_
                mov     ebx,00000001H
                mov     edx,00000004H
                lea     eax,+34H[esp]
                mov     ecx,ebp
                call    near ptr fread_
                mov     ebx,00000001H
                mov     edx,00000004H
                lea     eax,+30H[esp]
                mov     ecx,ebp
                call    near ptr fread_
                mov     ebx,00000001H
                mov     edx,00000004H
                lea     eax,+2cH[esp]
                mov     ecx,ebp
                call    near ptr fread_
                mov     ebx,00000001H
                mov     edx,00000004H
                lea     eax,+28H[esp]
                mov     ecx,ebp
                call    near ptr fread_
                mov     ebx,00000001H
                mov     edx,00000004H
                lea     eax,+24H[esp]
                mov     ecx,ebp
                call    near ptr fread_
                mov     ebx,00000001H
                mov     edx,00000004H
                lea     eax,+20H[esp]
                mov     ecx,ebp
                call    near ptr fread_
                jmp     near ptr L195
L188:           xor     esi,esi
                mov     edi,00000001H
L189:           mov     edx,00000002H
                lea     eax,+88H[esp]
                mov     ecx,ebp
                mov     ebx,edi
                inc     esi
                call    near ptr fread_
                cmp     esi,00000005H
                jl      short L189
                mov     ebx,00000001H
                mov     edx,00000002H
                lea     eax,+0a4H[esp]
                mov     ecx,ebp
                call    near ptr fread_
                xor     esi,esi
                xor     eax,eax
                mov     edi,dword ptr _Camera
                mov     ax,word ptr +0a4H[esp]
                mov     ebx,00000001H
                mov     dword ptr +88H[edi],eax
                xor     eax,eax
                mov     edx,00000002H
                mov     ax,word ptr +0a4H[esp]
                mov     ecx,ebp
                shl     eax,06H
                mov     dword ptr +0a0H[esp],esi
                call    near ptr malloc_
                mov     dword ptr +90H[edi],eax
                lea     eax,+88H[esp]
                add     edi,00000078H
                call    near ptr fread_
                jmp     near ptr L193
L190:           mov     ebx,00000001H
                mov     edx,00000002H
                lea     eax,+0c0H[esp]
                mov     ecx,ebp
                call    near ptr fread_
                mov     ebx,00000001H
                mov     edx,00000002H
                lea     eax,+88H[esp]
                mov     ecx,ebp
                call    near ptr fread_
                mov     ebx,00000001H
                mov     edx,00000002H
                lea     eax,+0bcH[esp]
                mov     ecx,ebp
                call    near ptr fread_
                xor     ebx,ebx
                mov     dword ptr +94H[esp],ebx
L191:           mov     cl,byte ptr +94H[esp]
                xor     edx,edx
                mov     eax,00000001H
                mov     dx,word ptr +0bcH[esp]
                shl     eax,cl
                test    edx,eax
                je      short L192
                mov     ebx,00000001H
                mov     edx,00000004H
                lea     eax,+14H[esp]
                mov     ecx,ebp
                call    near ptr fread_
L192:           mov     ecx,dword ptr +94H[esp]
                inc     ecx
                mov     dword ptr +94H[esp],ecx
                cmp     ecx,00000010H
                jl      short L191
                mov     ebx,00000001H
                mov     edx,00000004H
                lea     eax,+84H[esp]
                mov     ecx,ebp
                call    near ptr fread_
                mov     ebx,00000001H
                mov     edx,00000004H
                lea     eax,+80H[esp]
                mov     ecx,ebp
                call    near ptr fread_
                mov     ebx,00000001H
                mov     edx,00000004H
                lea     eax,+7cH[esp]
                mov     ecx,ebp
                call    near ptr fread_
                xor     edx,edx
                mov     eax,dword ptr +18H[edi]
                mov     dx,word ptr +0c0H[esp]
                mov     dword ptr [esi+eax],edx
                mov     eax,dword ptr +18H[edi]
                lea     edx,[eax+esi]
                mov     eax,dword ptr +84H[esp]
                mov     dword ptr +4H[edx],eax
                mov     eax,dword ptr +18H[edi]
                lea     edx,[eax+esi]
                mov     eax,dword ptr +80H[esp]
                mov     dword ptr +8H[edx],eax
                mov     eax,dword ptr +18H[edi]
                lea     edx,[eax+esi]
                mov     eax,dword ptr +7cH[esp]
                mov     dword ptr +0cH[edx],eax
                mov     edx,dword ptr +0a0H[esp]
                inc     edx
                add     esi,00000040H
                mov     dword ptr +0a0H[esp],edx
L193:           xor     eax,eax
                mov     edx,dword ptr +0a0H[esp]
                mov     ax,word ptr +0a4H[esp]
                cmp     eax,edx
                jle     short L195
                jmp     near ptr L190
L194:           mov     ebx,00000001H
                mov     edx,dword ptr _chunk+2H
                mov     eax,ebp
                sub     edx,00000006H
                call    near ptr fseek_
L195:           cmp     word ptr _chunk,0b001H
                jne     near ptr L163
L196:           mov     edx,dword ptr +9cH[esp]
                test    edx,edx
                je      near ptr L200
                fld     dword ptr +0e8H[esp]
                fmul    dword ptr L297
                fstp    dword ptr +8cH[esp]
L197:           mov     eax,edx
                call    near ptr Object_CalcFaceNormals_
                mov     eax,edx
                call    near ptr Object_CalcVertexNormals_
                mov     eax,edx
                call    near ptr Object_Center_
                mov     eax,edx
                call    near ptr Object_CalcBoundingSphere_
                test    byte ptr +1H[edx],80H
                jne     short L199
                mov     ecx,dword ptr +0c0H[edx]
                xor     esi,esi
                test    ecx,ecx
                jle     short L199
                fld     dword ptr +8cH[esp]
                xor     eax,eax
L198:           mov     edi,dword ptr +0c4H[edx]
                fld     dword ptr +1cH[edi+eax]
                fmul    st,st(1)
                fadd    st,st(1)
                fstp    dword ptr +28H[edi+eax]
                mov     edi,dword ptr +0c4H[edx]
                fld     dword ptr +20H[edi+eax]
                fmul    st,st(1)
                fadd    st,st(1)
                inc     esi
                fstp    dword ptr +2cH[edi+eax]
                mov     edi,dword ptr +0c0H[edx]
                add     eax,00000050H
                cmp     esi,edi
                jl      short L198
                fstp    st(0)
L199:           mov     edx,dword ptr +110H[edx]
                test    edx,edx
                jne     short L197
L200:           mov     eax,dword ptr +9cH[esp]
L201:           add     esp,000000c8H
                pop     ebp
                pop     edi
                pop     esi
                pop     edx
                pop     ecx
                pop     ebx
                ret     0008H
Object_CalcFaceNormals_:
                push    ebx
                push    ecx
                push    edx
                push    esi
                push    edi
                sub     esp,00000030H
                mov     ebx,eax
                xor     edx,edx
                mov     ecx,dword ptr +0c8H[eax]
                mov     dword ptr +20H[esp],edx
                test    ecx,ecx
                jle     near ptr L205
L202:           mov     ecx,dword ptr +0ccH[ebx]
                add     ecx,edx
                mov     esi,dword ptr +4H[ecx]
                mov     eax,dword ptr [ecx]
                mov     edi,dword ptr +8H[ecx]
                fld     dword ptr +0cH[edi]
                fld     dword ptr +8H[esi]
                fsub    dword ptr +8H[eax]
                fxch    st(1)
                fsub    dword ptr +0cH[eax]
                fxch    st(1)
                fst     dword ptr +10H[esp]
                fmul    st,st(1)
                fld     dword ptr +4H[edi]
                fld     dword ptr +0cH[esi]
                fsub    dword ptr +0cH[eax]
                fxch    st(1)
                fsub    dword ptr +4H[eax]
                fstp    dword ptr +4H[esp]
                fld     st(0)
                fmul    dword ptr +4H[esp]
                fld     dword ptr +8H[edi]
                fld     dword ptr +4H[esi]
                fsub    dword ptr +4H[eax]
                fxch    st(1)
                fsub    dword ptr +8H[eax]
                fld     st(1)
                fmul    st,st(1)
                fxch    st(4)
                fmulp   st(1),st
                fxch    st(1)
                fmulp   st(5),st
                fld     dword ptr +10H[esp]
                fmul    dword ptr +4H[esp]
                fxch    st(1)
                fsubp   st(4),st
                fxch    st(4)
                fsubp   st(1),st
                fxch    st(2)
                fst     dword ptr +24H[esp]
                fmul    dword ptr +24H[esp]
                fxch    st(2)
                fst     dword ptr +28H[esp]
                fmul    dword ptr +28H[esp]
                fxch    st(3)
                fsubp   st(1),st
                fxch    st(2)
                faddp   st(1),st
                fxch    st(1)
                fst     dword ptr +2cH[esp]
                fmul    dword ptr +2cH[esp]
                faddp   st(1),st
                fsqrt   
                fstp    dword ptr +1cH[esp]
                test    dword ptr +1cH[esp],7fffffffH
                je      short L203
                fld1    
                fld     dword ptr +24H[esp]
                fxch    st(1)
                fdiv    dword ptr +1cH[esp]
                fxch    st(1)
                fmul    st,st(1)
                fld     dword ptr +28H[esp]
                fmul    st,st(2)
                fld     dword ptr +2cH[esp]
                fxch    st(2)
                fstp    dword ptr +0cH[ecx]
                mov     eax,dword ptr +0ccH[ebx]
                fxch    st(1)
                fmulp   st(2),st
                fstp    dword ptr +10H[edx+eax]
                mov     eax,dword ptr +0ccH[ebx]
                fstp    dword ptr +14H[edx+eax]
                jmp     short L204
L203:           mov     dword ptr +0cH[ecx],00000000H
                mov     eax,dword ptr +0ccH[ebx]
                mov     dword ptr +10H[edx+eax],00000000H
                mov     eax,dword ptr +0ccH[ebx]
                mov     dword ptr +14H[edx+eax],00000000H
L204:           mov     esi,dword ptr +20H[esp]
                add     edx,0000002cH
                inc     esi
                mov     edi,dword ptr +0c8H[ebx]
                mov     dword ptr +20H[esp],esi
                cmp     esi,edi
                jl      near ptr L202
L205:           add     esp,00000030H
                pop     edi
                pop     esi
                pop     edx
                pop     ecx
                pop     ebx
                ret     
                lea     eax,+0H[eax]
Object_CalcVertexNormals_:
                push    ebx
                push    ecx
                push    edx
                push    esi
                push    edi
                push    ebp
                sub     esp,0000001cH
                mov     edi,eax
                xor     edx,edx
                mov     ebx,dword ptr +0c0H[eax]
                mov     dword ptr +8H[esp],edx
                test    ebx,ebx
                jle     near ptr L214
                xor     ebp,ebp
L206:           mov     ecx,dword ptr +0c4H[edi]
                xor     eax,eax
                xor     esi,esi
                mov     dword ptr +14H[esp],eax
                mov     dword ptr +10H[esp],eax
                mov     dword ptr +18H[esp],eax
                mov     dword ptr +0cH[esp],eax
                mov     eax,dword ptr +0c8H[edi]
                add     ecx,ebp
                test    eax,eax
                jle     short L210
                mov     eax,dword ptr +0ccH[edi]
                xor     ebx,ebx
L207:           cmp     ecx,dword ptr [eax]
                je      short L208
                cmp     ecx,dword ptr +4H[eax]
                je      short L208
                cmp     ecx,dword ptr +8H[eax]
                jne     short L209
L208:           fld     dword ptr +14H[esp]
                fld     dword ptr +10H[esp]
                mov     edx,dword ptr +0ccH[edi]
                fld     dword ptr +18H[esp]
                fxch    st(2)
                fadd    dword ptr +0cH[edx+ebx]
                fxch    st(1)
                fadd    dword ptr +10H[edx+ebx]
                fxch    st(2)
                fadd    dword ptr +14H[edx+ebx]
                fxch    st(1)
                fstp    dword ptr +14H[esp]
                mov     edx,dword ptr +0cH[esp]
                fxch    st(1)
                fstp    dword ptr +10H[esp]
                inc     edx
                fstp    dword ptr +18H[esp]
                mov     dword ptr +0cH[esp],edx
L209:           add     eax,0000002cH
                inc     esi
                mov     edx,dword ptr +0c8H[edi]
                add     ebx,0000002cH
                cmp     esi,edx
                jl      short L207
L210:           cmp     dword ptr +0cH[esp],00000000H
                je      short L211
                fild    dword ptr +0cH[esp]
                fld     dword ptr +14H[esp]
                fld1    
                fdivrp  st(2),st
                fmul    st,st(1)
                fld     dword ptr +10H[esp]
                fmul    st,st(2)
                fld     dword ptr +18H[esp]
                fmulp   st(3),st
                fxch    st(1)
                fstp    dword ptr +14H[esp]
                fstp    dword ptr +10H[esp]
                fstp    dword ptr +18H[esp]
L211:           fld     dword ptr +14H[esp]
                fmul    st(0),st
                fld     dword ptr +10H[esp]
                fmul    st(0),st
                faddp   st(1),st
                fld     dword ptr +18H[esp]
                fmul    st(0),st
                faddp   st(1),st
                fsqrt   
                fstp    dword ptr +4H[esp]
                test    dword ptr +4H[esp],7fffffffH
                je      short L212
                fld1    
                fld     dword ptr +14H[esp]
                fxch    st(1)
                fdiv    dword ptr +4H[esp]
                fxch    st(1)
                fmul    st,st(1)
                fld     dword ptr +10H[esp]
                fmul    st,st(2)
                mov     eax,dword ptr +0c4H[edi]
                fld     dword ptr +18H[esp]
                fxch    st(2)
                fstp    dword ptr +1cH[eax+ebp]
                mov     eax,dword ptr +0c4H[edi]
                fxch    st(1)
                fmulp   st(2),st
                fstp    dword ptr +20H[eax+ebp]
                mov     eax,dword ptr +0c4H[edi]
                fstp    dword ptr +24H[eax+ebp]
                jmp     short L213
L212:           mov     eax,dword ptr +0c4H[edi]
                mov     dword ptr +1cH[eax+ebp],00000000H
                mov     eax,dword ptr +0c4H[edi]
                mov     dword ptr +20H[eax+ebp],00000000H
                mov     eax,dword ptr +0c4H[edi]
                mov     dword ptr +24H[eax+ebp],00000000H
L213:           mov     ecx,dword ptr +8H[esp]
                add     ebp,00000050H
                inc     ecx
                mov     esi,dword ptr +0c0H[edi]
                mov     dword ptr +8H[esp],ecx
                cmp     ecx,esi
                jl      near ptr L206
L214:           add     esp,0000001cH
                pop     ebp
                pop     edi
                pop     esi
                pop     edx
                pop     ecx
                pop     ebx
                ret     
                mov     eax,eax
Object_Center_: push    ebx
                push    ecx
                push    edx
                push    esi
                push    edi
                push    ebp
                sub     esp,00000024H
                mov     ebx,eax
                mov     edx,0c6fffe00H
                mov     ecx,46fffe00H
                mov     dword ptr +0cH[esp],edx
                mov     dword ptr +14H[esp],edx
                mov     dword ptr +10H[esp],edx
                mov     dword ptr +1cH[esp],ecx
                mov     dword ptr +18H[esp],ecx
                mov     dword ptr +20H[esp],ecx
                mov     edx,dword ptr +0c0H[eax]
                xor     ecx,ecx
                test    edx,edx
                jle     near ptr L222
                mov     edx,dword ptr +0c4H[eax]
L215:           fld     dword ptr +4H[edx]
                fcomp   dword ptr +10H[esp]
                fstsw   ax
                sahf    
                jbe     short L216
                mov     eax,dword ptr +4H[edx]
                mov     dword ptr +10H[esp],eax
L216:           fld     dword ptr +4H[edx]
                fcomp   dword ptr +1cH[esp]
                fstsw   ax
                sahf    
                jae     short L217
                mov     eax,dword ptr +4H[edx]
                mov     dword ptr +1cH[esp],eax
L217:           fld     dword ptr +8H[edx]
                fcomp   dword ptr +0cH[esp]
                fstsw   ax
                sahf    
                jbe     short L218
                mov     eax,dword ptr +8H[edx]
                mov     dword ptr +0cH[esp],eax
L218:           fld     dword ptr +8H[edx]
                fcomp   dword ptr +18H[esp]
                fstsw   ax
                sahf    
                jae     short L219
                mov     eax,dword ptr +8H[edx]
                mov     dword ptr +18H[esp],eax
L219:           fld     dword ptr +0cH[edx]
                fcomp   dword ptr +14H[esp]
                fstsw   ax
                sahf    
                jbe     short L220
                mov     eax,dword ptr +0cH[edx]
                mov     dword ptr +14H[esp],eax
L220:           fld     dword ptr +0cH[edx]
                fcomp   dword ptr +20H[esp]
                fstsw   ax
                sahf    
                jae     short L221
                mov     eax,dword ptr +0cH[edx]
                mov     dword ptr +20H[esp],eax
L221:           inc     ecx
                mov     esi,dword ptr +0c0H[ebx]
                add     edx,00000050H
                cmp     ecx,esi
                jl      short L215
L222:           fld     dword ptr +1cH[esp]
                fld     dword ptr +10H[esp]
                fsub    st,st(1)
                fld     dword ptr L298
                fxch    st(1)
                fmul    st,st(1)
                fld     dword ptr +14H[esp]
                fld     dword ptr +18H[esp]
                fld     dword ptr +0cH[esp]
                fsub    st,st(1)
                fxch    st(2)
                fld     dword ptr +20H[esp]
                fxch    st(1)
                fsub    st,st(1)
                fxch    st(3)
                fmul    st,st(5)
                fxch    st(3)
                fmulp   st(5),st
                mov     edi,dword ptr +0c0H[ebx]
                xor     ecx,ecx
                fxch    st(3)
                faddp   st(5),st
                faddp   st(1),st
                fxch    st(2)
                faddp   st(1),st
                fxch    st(1)
                fstp    dword ptr +8H[esp]
                fstp    dword ptr +4H[esp]
                fstp    dword ptr [esp]
                test    edi,edi
                jle     short L224
                fld     dword ptr +4H[esp]
                fld     dword ptr +8H[esp]
                xor     edx,edx
                fld     dword ptr [esp]
L223:           mov     eax,dword ptr +0c4H[ebx]
                fld     dword ptr +4H[edx+eax]
                fsub    st,st(1)
                fstp    dword ptr +4H[edx+eax]
                mov     eax,dword ptr +0c4H[ebx]
                fld     dword ptr +8H[edx+eax]
                fsub    st,st(2)
                fstp    dword ptr +8H[edx+eax]
                mov     eax,dword ptr +0c4H[ebx]
                fld     dword ptr +0cH[edx+eax]
                fsub    st,st(3)
                inc     ecx
                fstp    dword ptr +0cH[edx+eax]
                mov     ebp,dword ptr +0c0H[ebx]
                add     edx,00000050H
                cmp     ecx,ebp
                jl      short L223
                fstp    st(0)
                fstp    st(0)
                fstp    st(0)
L224:           fld     dword ptr +18H[ebx]
                fld     dword ptr +1cH[ebx]
                fld     dword ptr +20H[ebx]
                fxch    st(2)
                fadd    dword ptr [esp]
                fxch    st(1)
                fadd    dword ptr +8H[esp]
                fxch    st(2)
                fadd    dword ptr +4H[esp]
                fxch    st(2)
                fstp    dword ptr +1cH[ebx]
                fxch    st(1)
                fstp    dword ptr +20H[ebx]
                fstp    dword ptr +18H[ebx]
                add     esp,00000024H
                pop     ebp
                pop     edi
                pop     esi
                pop     edx
                pop     ecx
                pop     ebx
                ret     
Object_CalcBoundingSphere_:
                push    ebx
                push    ecx
                push    edx
                push    esi
                push    edi
                push    ebp
                mov     ebp,esp
                sub     esp,0000000cH
                and     esp,0fffffff8H
                mov     ecx,eax
                mov     esi,dword ptr +0c0H[eax]
                xor     edx,edx
                xor     ebx,ebx
                mov     dword ptr [esp],edx
                test    esi,esi
                jle     short L229
                mov     edx,dword ptr +0c4H[eax]
                add     edx,00000004H
L225:           fld     dword ptr +4H[edx]
                fmul    st,st(0)
                fld     dword ptr [edx]
                fmul    st,st(0)
                faddp   st(1),st
                fld     dword ptr +8H[edx]
                fmul    st,st(0)
                faddp   st(1),st
                fstp    dword ptr +8H[esp]
                test    dword ptr +8H[esp],7fffffffH
                je      short L226
                fld     dword ptr +8H[esp]
                fsqrt   
                fstp    dword ptr +4H[esp]
                jmp     short L227
L226:           xor     edi,edi
                mov     dword ptr +4H[esp],edi
L227:           fld     dword ptr +4H[esp]
                fcomp   dword ptr [esp]
                fstsw   ax
                sahf    
                jbe     short L228
                mov     eax,dword ptr +4H[esp]
                mov     dword ptr [esp],eax
L228:           inc     ebx
                mov     edi,dword ptr +0c0H[ecx]
                add     edx,00000050H
                cmp     ebx,edi
                jl      short L225
L229:           mov     eax,dword ptr [esp]
                mov     dword ptr +0bcH[ecx],eax
                mov     esp,ebp
                pop     ebp
                pop     edi
                pop     esi
                pop     edx
                pop     ecx
                pop     ebx
                ret     
                nop     
Object_LoadFile_:
                push    ebx
                push    ecx
                push    edx
                push    esi
                push    edi
                push    ebp
                sub     esp,00000020H
                mov     edx,offset DGROUP:L296
                mov     ebx,00000001H
                call    near ptr fopen_
                mov     edx,00000004H
                mov     esi,eax
                mov     ecx,eax
                lea     eax,+8H[esp]
                call    near ptr fread_
                mov     eax,00000118H
                call    near ptr malloc_
                mov     ebp,eax
                mov     edi,eax
                mov     dword ptr [eax],00000001H
                add     eax,000000bcH
                mov     dword ptr -0a4H[eax],38d1b717H
                mov     dword ptr -0a0H[eax],38d1b717H
                mov     dword ptr -9cH[eax],38d1b717H
                mov     dword ptr -8cH[eax],38d1b717H
                mov     dword ptr -88H[eax],38d1b717H
                mov     ebx,00000001H
                mov     dword ptr -84H[eax],38d1b717H
                mov     edx,00000004H
                mov     dword ptr +58H[eax],00000000H
                mov     ecx,esi
                mov     dword ptr +54H[eax],00000000H
                call    near ptr fread_
                mov     ebx,00000001H
                mov     edx,00000004H
                lea     eax,+4H[esp]
                mov     ecx,esi
                call    near ptr fread_
                mov     eax,dword ptr +4H[esp]
                mov     edx,eax
                mov     dword ptr +0c0H[ebp],eax
                shl     eax,02H
                add     eax,edx
                shl     eax,04H
                call    near ptr malloc_
                mov     dword ptr +0c4H[ebp],eax
                xor     edx,edx
                mov     ebx,dword ptr +4H[esp]
                mov     dword ptr +18H[esp],edx
                test    ebx,ebx
                jle     near ptr L231
                xor     ebp,ebp
L230:           mov     ebx,00000001H
                mov     eax,dword ptr +0c4H[edi]
                mov     edx,00000004H
                add     eax,ebp
                mov     ecx,esi
                add     eax,00000004H
                call    near ptr fread_
                mov     ebx,00000001H
                mov     eax,dword ptr +0c4H[edi]
                mov     edx,00000004H
                add     eax,ebp
                mov     ecx,esi
                add     eax,00000008H
                call    near ptr fread_
                mov     ebx,00000001H
                mov     eax,dword ptr +0c4H[edi]
                mov     edx,00000004H
                add     eax,ebp
                mov     ecx,esi
                add     eax,0000000cH
                call    near ptr fread_
                mov     ebx,00000001H
                mov     eax,dword ptr +0c4H[edi]
                mov     edx,00000004H
                add     eax,ebp
                mov     ecx,esi
                add     eax,00000028H
                call    near ptr fread_
                mov     ebx,00000001H
                mov     eax,dword ptr +0c4H[edi]
                mov     edx,00000004H
                add     eax,ebp
                mov     ecx,esi
                add     eax,0000002cH
                call    near ptr fread_
                mov     eax,dword ptr +0c4H[edi]
                mov     dword ptr +30H[eax+ebp],427c0000H
                mov     ebx,00000001H
                mov     eax,dword ptr +0c4H[edi]
                mov     edx,00000004H
                add     eax,ebp
                mov     ecx,esi
                add     eax,0000001cH
                call    near ptr fread_
                mov     ebx,00000001H
                mov     eax,dword ptr +0c4H[edi]
                mov     edx,00000004H
                add     eax,ebp
                mov     ecx,esi
                add     eax,00000020H
                call    near ptr fread_
                mov     ebx,00000001H
                mov     eax,dword ptr +0c4H[edi]
                mov     edx,00000004H
                add     eax,ebp
                mov     ecx,esi
                add     eax,00000024H
                call    near ptr fread_
                mov     ecx,dword ptr +18H[esp]
                add     ebp,00000050H
                inc     ecx
                mov     edx,dword ptr +4H[esp]
                mov     dword ptr +18H[esp],ecx
                cmp     ecx,edx
                jl      near ptr L230
L231:           mov     ebx,00000001H
                mov     edx,00000004H
                mov     eax,esp
                mov     ecx,esi
                call    near ptr fread_
                mov     eax,dword ptr [esp]
                mov     edx,eax
                mov     dword ptr +0c8H[edi],eax
                shl     eax,02H
                sub     eax,edx
                shl     eax,02H
                sub     eax,edx
                shl     eax,02H
                call    near ptr malloc_
                mov     dword ptr +0ccH[edi],eax
                xor     ebx,ebx
                mov     ecx,dword ptr [esp]
                mov     dword ptr +1cH[esp],ebx
                test    ecx,ecx
                jle     near ptr L233
                xor     ebp,ebp
L232:           mov     ebx,00000001H
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
                imul    edx,dword ptr +14H[esp],50H
                mov     eax,dword ptr +0c4H[edi]
                add     eax,edx
                mov     edx,dword ptr +0ccH[edi]
                mov     dword ptr [edx+ebp],eax
                imul    edx,dword ptr +10H[esp],50H
                mov     eax,dword ptr +0c4H[edi]
                add     eax,edx
                mov     edx,dword ptr +0ccH[edi]
                mov     dword ptr +4H[edx+ebp],eax
                imul    edx,dword ptr +0cH[esp],50H
                mov     eax,dword ptr +0c4H[edi]
                add     edx,eax
                mov     eax,dword ptr +0ccH[edi]
                mov     dword ptr +8H[eax+ebp],edx
                mov     ebx,00000001H
                mov     eax,dword ptr +0ccH[edi]
                mov     ecx,esi
                add     eax,ebp
                mov     edx,00000004H
                add     eax,0000000cH
                call    near ptr fread_
                mov     ebx,00000001H
                mov     eax,dword ptr +0ccH[edi]
                mov     edx,00000004H
                add     eax,ebp
                mov     ecx,esi
                add     eax,00000010H
                call    near ptr fread_
                mov     ebx,00000001H
                mov     eax,dword ptr +0ccH[edi]
                mov     edx,00000004H
                add     eax,ebp
                mov     ecx,esi
                add     eax,00000014H
                call    near ptr fread_
                mov     eax,dword ptr +0ccH[edi]
                mov     dword ptr +28H[eax+ebp],edi
                mov     eax,dword ptr +1cH[esp]
                add     ebp,0000002cH
                inc     eax
                mov     edx,dword ptr [esp]
                mov     dword ptr +1cH[esp],eax
                cmp     eax,edx
                jl      near ptr L232
L233:           mov     eax,esi
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
Object_Free_:   push    ebx
                push    ecx
                push    edx
                push    esi
                mov     edx,eax
                mov     ebx,dword ptr +110H[eax]
                test    ebx,ebx
                je      short L234
                mov     eax,ebx
                call    near ptr Object_Free_
L234:           mov     ecx,dword ptr +0c4H[edx]
                test    ecx,ecx
                je      short L235
                mov     eax,ecx
                call    near ptr free_
L235:           mov     esi,dword ptr +0ccH[edx]
                test    esi,esi
                je      short L236
                mov     eax,esi
                call    near ptr free_
L236:           mov     eax,edx
                call    near ptr free_
                pop     esi
                pop     edx
                pop     ecx
                pop     ebx
                ret     
                lea     eax,+0H[eax]
Object_SetHierarchy_:
                mov     dword ptr +110H[eax],ebx
                mov     dword ptr +114H[eax],edx
                cmp     ecx,00000001H
                jne     short L237
                or      byte ptr [eax],04H
L237:           ret     
                lea     eax,+0H[eax]
Object_SetPos_: push    edx
                mov     edx,dword ptr +8H[esp]
                mov     dword ptr +18H[eax],edx
                mov     edx,dword ptr +0cH[esp]
                mov     dword ptr +1cH[eax],edx
                mov     edx,dword ptr +10H[esp]
                mov     dword ptr +20H[eax],edx
                pop     edx
                ret     000cH
                mov     eax,eax
Object_AddPos_: fld     dword ptr +18H[eax]
                fld     dword ptr +1cH[eax]
                fld     dword ptr +20H[eax]
                fxch    st(2)
                fadd    dword ptr +4H[esp]
                fxch    st(1)
                fadd    dword ptr +8H[esp]
                fxch    st(2)
                fadd    dword ptr +0cH[esp]
                fxch    st(2)
                fstp    dword ptr +1cH[eax]
                fxch    st(1)
                fstp    dword ptr +20H[eax]
                fstp    dword ptr +18H[eax]
                ret     000cH
                nop     
Object_SetRot_: push    edx
                mov     edx,dword ptr +8H[esp]
                mov     dword ptr +30H[eax],edx
                mov     edx,dword ptr +0cH[esp]
                mov     dword ptr +34H[eax],edx
                mov     edx,dword ptr +10H[esp]
                mov     dword ptr +38H[eax],edx
                pop     edx
                ret     000cH
                mov     eax,eax
Object_AddRot_: fld     dword ptr +30H[eax]
                fld     dword ptr +34H[eax]
                fld     dword ptr +38H[eax]
                fxch    st(2)
                fadd    dword ptr +4H[esp]
                fxch    st(1)
                fadd    dword ptr +8H[esp]
                fxch    st(2)
                fadd    dword ptr +0cH[esp]
                fxch    st(2)
                fstp    dword ptr +34H[eax]
                fxch    st(1)
                fstp    dword ptr +38H[eax]
                fstp    dword ptr +30H[eax]
                ret     000cH
                nop     
Object_FindByName_:
                push    ebx
                push    ecx
                push    edx
                push    esi
                mov     esi,eax
                xor     ecx,ecx
                test    ebx,ebx
                je      short L241
L238:           test    ecx,ecx
                jne     short L241
                lea     eax,+4H[ebx]
                mov     edx,esi
                call    near ptr strcmp_
                test    eax,eax
                jne     short L239
                mov     ecx,00000001H
L239:           test    ecx,ecx
                jne     short L240
                mov     ebx,dword ptr +110H[ebx]
L240:           test    ebx,ebx
                jne     short L238
L241:           mov     eax,ebx
                pop     esi
                pop     edx
                pop     ecx
                pop     ebx
                ret     
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
L242:           mov     bl,byte ptr [eax]
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
                jge     short L243
                mov     ecx,ebx
                mov     byte ptr +10H[esp],dl
L243:           inc     edx
                add     eax,00000003H
                cmp     edx,00000100H
                jl      short L242
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
L244:           mov     ebp,dword ptr [esp]
                mov     ebx,dword ptr +8H[esp]
                mov     esi,dword ptr +4H[esp]
                add     ebp,ebx
L245:           movsx   edx,byte ptr +2H[esi]
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
                jne     short L245
                mov     esi,dword ptr +8H[esp]
                add     esi,00000100H
                inc     edi
                mov     dword ptr +8H[esp],esi
                cmp     edi,00000040H
                jl      short L244
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
L246:           mov     edx,dword ptr +6cH[esp]
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
                jle     near ptr L248
                mov     ebp,dword ptr +20H[esp]
                add     ebp,dword ptr +24H[esp]
L247:           fld     dword ptr +0cH[esp]
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
                jl      near ptr L247
L248:           mov     ebp,dword ptr +24H[esp]
                mov     edi,dword ptr +2cH[esp]
                inc     ebp
                add     edi,00000003H
                mov     dword ptr +24H[esp],ebp
                mov     dword ptr +2cH[esp],edi
                cmp     ebp,00000100H
                jl      near ptr L246
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
L249:           mov     edx,dword ptr [esp]
                movsx   ax,byte ptr [edx]
                fld1    
                mov     dword ptr +54H[esp],eax
                fld     dword ptr L299
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
                jge     near ptr L251
                mov     edi,dword ptr +1cH[esp]
                mov     ebx,dword ptr +20H[esp]
                mov     ecx,dword ptr +28H[esp]
                add     edi,ebx
                add     edi,ecx
L250:           fld     dword ptr +10H[esp]
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
                jl      near ptr L250
L251:           mov     eax,dword ptr +28H[esp]
                mov     ebp,dword ptr [esp]
                inc     eax
                add     ebp,00000003H
                mov     dword ptr +28H[esp],eax
                mov     dword ptr [esp],ebp
                cmp     eax,00000100H
                jl      near ptr L249
                add     esp,00000058H
                pop     ebp
                pop     edi
                pop     esi
                pop     ecx
                ret     000cH
                nop     
Setup_Intensity_:
                push    edx
                sub     esp,00000010H
                mov     dword ptr +4H[esp],eax
                mov     eax,00000080H
                mov     edx,dword ptr +4H[esp]
                sub     eax,edx
                mov     dword ptr [esp],eax
                fild    dword ptr [esp]
                fmul    dword ptr L300
                fild    dword ptr +4H[esp]
                fstp    dword ptr +0cH[esp]
                xor     edx,edx
                fstp    dword ptr +8H[esp]
                fld     dword ptr +0cH[esp]
                fld     dword ptr +8H[esp]
                fld     dword ptr L301
L252:           fld     st(2)
                fmul    st,st(1)
                fld     st(3)
                inc     edx
                fxch    st(1)
                call    near ptr __CHP
                fistp   dword ptr [esp]
                mov     al,byte ptr [esp]
                fadd    st,st(2)
                mov     byte ptr _Intensity-1H[edx],al
                fstp    st(3)
                cmp     edx,00000080H
                jl      short L252
                fstp    st(0)
                fstp    st(0)
                fstp    dword ptr +0cH[esp]
                add     esp,00000010H
                pop     edx
                ret     
                mov     eax,eax
Calc_PhongIntensity_:
                push    ebx
                push    ecx
                push    edx
                push    ebp
                mov     ebp,esp
                sub     esp,00000030H
                and     esp,0fffffff8H
                mov     edx,dword ptr +14H[ebp]
                mov     dword ptr +2cH[esp],edx
                mov     edx,dword ptr +18H[ebp]
                mov     dword ptr +1cH[esp],edx
                mov     edx,dword ptr +1cH[ebp]
                mov     dword ptr +28H[esp],edx
                mov     edx,dword ptr +20H[ebp]
                mov     dword ptr +20H[esp],edx
                mov     edx,dword ptr +24H[ebp]
                mov     dword ptr +24H[esp],edx
                mov     edx,0c0490fdbH
                mov     ebx,427c0000H
                mov     dword ptr +18H[esp],edx
                xor     edx,edx
L253:           fld     dword ptr +18H[esp]
                fcos    
                fld     dword ptr +28H[esp]
                fmul    st,st(1)
                fld     dword ptr +2cH[esp]
                fxch    st(1)
                fmul    st,st(1)
                fmul    dword ptr +1cH[esp]
                fld     dword ptr +24H[esp]
                fld     st(3)
                call    near ptr `IF@DPOW`
                fstp    st(3)
                fld     dword ptr +20H[esp]
                fmulp   st(3),st
                fxch    st(2)
                fmulp   st(1),st
                faddp   st(1),st
                fmul    qword ptr L302
                fst     dword ptr +14H[esp]
                fcomp   dword ptr L303
                fstsw   ax
                sahf    
                jbe     short L254
                mov     dword ptr +14H[esp],ebx
L254:           fldz    
                fcomp   dword ptr +14H[esp]
                fstsw   ax
                sahf    
                jbe     short L255
                xor     ecx,ecx
                mov     dword ptr +14H[esp],ecx
L255:           fld     dword ptr +18H[esp]
                fld     dword ptr +14H[esp]
                call    near ptr __CHP
                inc     edx
                fistp   dword ptr +10H[esp]
                fadd    qword ptr L304
                mov     al,byte ptr +10H[esp]
                fstp    dword ptr +18H[esp]
                mov     byte ptr _Intensity-1H[edx],al
                cmp     edx,00000080H
                jl      near ptr L253
                mov     esp,ebp
                pop     ebp
                pop     edx
                pop     ecx
                pop     ebx
                ret     0014H
                lea     eax,+0H[eax]
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
L256:           mov     ecx,esi
                mov     edx,0ffffff80H
                imul    ecx,esi
L257:           mov     eax,edx
                imul    eax,edx
                lea     edi,[ecx+eax]
                mov     dword ptr [esp],edi
                fild    dword ptr [esp]
                fsqrt   
                call    near ptr __CHP
                fistp   dword ptr +4H[esp]
                cmp     ebp,dword ptr +4H[esp]
                jge     short L258
                mov     dword ptr +4H[esp],ebp
L258:           mov     edi,dword ptr +4H[esp]
                mov     eax,ebp
                sub     eax,edi
                inc     ebx
                mov     al,byte ptr _PhongIntensity[eax]
                inc     edx
                mov     byte ptr -1H[ebx],al
                cmp     edx,00000080H
                jne     short L257
                inc     esi
                cmp     esi,edx
                jne     short L256
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
L259:           mov     esi,dword ptr [esp]
                mov     edx,0ffffff80H
                mov     ebx,dword ptr +4H[esp]
                imul    esi,esi
L260:           mov     eax,edx
                imul    eax,edx
                add     eax,esi
                mov     dword ptr +8H[esp],eax
                fild    dword ptr +8H[esp]
                fsqrt   
                fsubr   qword ptr L305
                call    near ptr __CHP
                fistp   dword ptr +8H[esp]
                mov     al,byte ptr +8H[esp]
                cmp     al,7fH
                jl      short L261
                mov     al,7fH
L261:           cmp     al,0eH
                jg      short L262
                mov     al,ch
L262:           movsx   eax,al
                sar     eax,1
                inc     ebx
                mov     cl,al
                mov     eax,edi
                inc     edx
                mov     byte ptr -1H[ebx+eax],cl
                cmp     edx,00000080H
                jne     short L260
                mov     ebx,dword ptr +4H[esp]
                mov     edx,dword ptr [esp]
                add     ebx,00000100H
                inc     edx
                mov     dword ptr +4H[esp],ebx
                mov     dword ptr [esp],edx
                cmp     ebx,00010000H
                jne     short L259
                mov     dword ptr __PhongMap,edi
                mov     esp,ebp
                pop     ebp
                pop     edi
                pop     esi
                pop     edx
                pop     ecx
                pop     ebx
                ret     
                mov     eax,eax
VesaCalc_ShadeTable_:
                push    ebx
                push    ecx
                push    esi
                push    edi
                push    ebp
                sub     esp,00000014H
                mov     dword ptr +8H[esp],eax
                mov     dword ptr +0cH[esp],edx
                xor     esi,esi
                lea     eax,+200H[edx]
                mov     dword ptr [esp],esi
                mov     dword ptr +4H[esp],eax
L263:           mov     ecx,dword ptr +0cH[esp]
                mov     ebx,dword ptr [esp]
                mov     eax,dword ptr [esp]
                mov     edi,dword ptr +4H[esp]
                add     ecx,ebx
                add     eax,edi
                mov     ebx,dword ptr +8H[esp]
                mov     dword ptr +10H[esp],eax
L264:           movsx   edx,byte ptr [ebx]
                imul    edx,esi
                mov     eax,edx
                sar     edx,1fH
                shl     edx,06H
                sbb     eax,edx
                sar     eax,06H
                movsx   edx,byte ptr +1H[ebx]
                imul    edx,esi
                mov     ebp,eax
                mov     eax,edx
                sar     edx,1fH
                shl     edx,06H
                sbb     eax,edx
                sar     eax,06H
                movsx   edx,byte ptr +2H[ebx]
                imul    edx,esi
                mov     edi,eax
                mov     eax,edx
                sar     edx,1fH
                shl     edx,06H
                sbb     eax,edx
                sar     eax,06H
                sar     ebp,1
                sar     edi,1
                shl     ebp,0aH
                shl     edi,05H
                add     ecx,00000002H
                add     edi,ebp
                sar     eax,1
                add     ebx,00000003H
                add     eax,edi
                mov     ebp,dword ptr +10H[esp]
                mov     word ptr -2H[ecx],ax
                cmp     ecx,ebp
                jne     short L264
                mov     eax,dword ptr [esp]
                add     eax,00000200H
                inc     esi
                mov     dword ptr [esp],eax
                cmp     esi,00000040H
                jl      near ptr L263
                add     esp,00000014H
                pop     ebp
                pop     edi
                pop     esi
                pop     ecx
                pop     ebx
                ret     
VesaCalc_ShadeHighLight_:
                push    ecx
                push    esi
                push    edi
                push    ebp
                sub     esp,00000054H
                mov     dword ptr +10H[esp],eax
                mov     dword ptr +4H[esp],edx
                mov     dword ptr +38H[esp],ebx
                mov     esi,eax
                xor     edi,edi
                xor     ebp,ebp
L265:           mov     eax,dword ptr +68H[esp]
                mov     dword ptr +3cH[esp],eax
                mov     eax,dword ptr +6cH[esp]
                mov     dword ptr +40H[esp],eax
                mov     eax,dword ptr +70H[esp]
                mov     dword ptr +1cH[esp],eax
                movsx   ax,byte ptr [esi]
                mov     dword ptr +50H[esp],eax
                fild    word ptr +50H[esp]
                fsub    dword ptr +68H[esp]
                fild    dword ptr +38H[esp]
                fld1    
                fdivrp  st(1),st
                movsx   ax,byte ptr +1H[esi]
                fxch    st(1)
                fmul    st,st(1)
                mov     dword ptr +50H[esp],eax
                movsx   ax,byte ptr +2H[esi]
                fild    word ptr +50H[esp]
                mov     dword ptr +50H[esp],eax
                fsub    dword ptr +6cH[esp]
                fild    word ptr +50H[esp]
                fsub    dword ptr +70H[esp]
                fxch    st(1)
                fmul    st,st(3)
                fxch    st(1)
                fmulp   st(3),st
                mov     ebx,dword ptr +38H[esp]
                mov     edx,ebp
                fxch    st(1)
                fstp    dword ptr +28H[esp]
                fstp    dword ptr +2cH[esp]
                fstp    dword ptr +24H[esp]
                cmp     ebp,ebx
                jge     near ptr L267
                mov     eax,dword ptr +4H[esp]
                fld     dword ptr +40H[esp]
                add     eax,edi
L266:           fld     dword ptr +3cH[esp]
                fld     st(1)
                fld     dword ptr +1cH[esp]
                fld     dword ptr +3cH[esp]
                fld     st(4)
                fld     dword ptr +1cH[esp]
                fxch    st(5)
                call    near ptr __CHP
                fxch    st(4)
                call    near ptr __CHP
                fxch    st(3)
                call    near ptr __CHP
                add     eax,00000200H
                inc     edx
                fxch    st(4)
                fistp   dword ptr [esp]
                fxch    st(2)
                fistp   dword ptr +0cH[esp]
                fxch    st(2)
                fistp   dword ptr +8H[esp]
                mov     ecx,dword ptr [esp]
                mov     ebx,dword ptr +0cH[esp]
                sar     ecx,1
                sar     ebx,1
                shl     ecx,0aH
                shl     ebx,05H
                fxch    st(1)
                fadd    dword ptr +28H[esp]
                add     ecx,ebx
                mov     ebx,dword ptr +8H[esp]
                fxch    st(1)
                fadd    dword ptr +2cH[esp]
                sar     ebx,1
                fxch    st(2)
                fadd    dword ptr +24H[esp]
                add     ebx,ecx
                mov     ecx,dword ptr +38H[esp]
                mov     word ptr -200H[eax],bx
                fxch    st(1)
                fstp    dword ptr +3cH[esp]
                fxch    st(1)
                fstp    st(2)
                fstp    dword ptr +1cH[esp]
                cmp     edx,ecx
                jl      near ptr L266
                fstp    dword ptr +40H[esp]
L267:           add     edi,00000002H
                add     esi,00000003H
                cmp     edi,00000200H
                jne     near ptr L265
                mov     eax,0000003fH
                sub     eax,dword ptr +38H[esp]
                inc     eax
                mov     dword ptr +14H[esp],eax
                mov     eax,dword ptr +38H[esp]
                xor     edi,edi
                shl     eax,09H
                mov     esi,dword ptr +10H[esp]
                mov     dword ptr +18H[esp],eax
                mov     ebp,00000040H
L268:           movsx   ax,byte ptr [esi]
                fld1    
                mov     dword ptr +50H[esp],eax
                fld     dword ptr L306
                fld     st(0)
                fild    word ptr +50H[esp]
                fstp    dword ptr +48H[esp]
                fsub    dword ptr +48H[esp]
                faddp   st(2),st
                fild    dword ptr +14H[esp]
                fld1    
                fdivrp  st(1),st
                movsx   ax,byte ptr +1H[esi]
                fxch    st(2)
                fmul    st,st(2)
                mov     dword ptr +50H[esp],eax
                fld     st(1)
                fild    word ptr +50H[esp]
                movsx   ax,byte ptr +2H[esi]
                fstp    dword ptr +44H[esp]
                mov     dword ptr +50H[esp],eax
                fsub    dword ptr +44H[esp]
                fild    word ptr +50H[esp]
                fld1    
                fxch    st(1)
                fstp    dword ptr +4cH[esp]
                fxch    st(3)
                fsub    dword ptr +4cH[esp]
                fxch    st(3)
                faddp   st(1),st
                fld1    
                faddp   st(3),st
                fmul    st,st(3)
                fxch    st(2)
                fmulp   st(3),st
                mov     eax,dword ptr +38H[esp]
                fstp    dword ptr +34H[esp]
                fstp    dword ptr +20H[esp]
                fstp    dword ptr +30H[esp]
                cmp     ebp,eax
                jle     near ptr L270
                mov     edx,dword ptr +18H[esp]
                add     edx,dword ptr +4H[esp]
                fld     dword ptr +48H[esp]
                add     edx,edi
L269:           fld     st(0)
                fld     dword ptr +44H[esp]
                fld     dword ptr +4cH[esp]
                fld     st(3)
                fld     dword ptr +44H[esp]
                fld     dword ptr +4cH[esp]
                fxch    st(5)
                call    near ptr __CHP
                fxch    st(4)
                call    near ptr __CHP
                fxch    st(3)
                call    near ptr __CHP
                add     edx,00000200H
                inc     eax
                fxch    st(4)
                fistp   dword ptr +8H[esp]
                fxch    st(2)
                fistp   dword ptr +0cH[esp]
                mov     ecx,dword ptr +8H[esp]
                mov     ebx,dword ptr +0cH[esp]
                sar     ecx,1
                sar     ebx,1
                shl     ecx,0aH
                shl     ebx,05H
                fxch    st(2)
                fistp   dword ptr +0cH[esp]
                add     ecx,ebx
                mov     ebx,dword ptr +0cH[esp]
                fxch    st(1)
                fadd    dword ptr +34H[esp]
                sar     ebx,1
                fxch    st(1)
                fadd    dword ptr +20H[esp]
                add     ebx,ecx
                fxch    st(2)
                fadd    dword ptr +30H[esp]
                mov     word ptr -200H[edx],bx
                fxch    st(1)
                fstp    st(3)
                fxch    st(1)
                fstp    dword ptr +44H[esp]
                fstp    dword ptr +4cH[esp]
                cmp     eax,00000040H
                jl      near ptr L269
                fstp    dword ptr +48H[esp]
L270:           add     edi,00000002H
                add     esi,00000003H
                cmp     edi,00000200H
                jne     near ptr L268
                add     esp,00000054H
                pop     ebp
                pop     edi
                pop     esi
                pop     ecx
                ret     000cH
                lea     eax,+0H[eax]
Engine_Init_:   push    esi
                push    edi
                push    ebp
                sub     esp,00000004H
                mov     edi,eax
                mov     esi,edx
                mov     dword ptr _ScreenWidth,eax
                mov     dword ptr _ScreenHeight,edx
                xor     edx,edx
                dec     eax
                mov     dword ptr _MinX,edx
                mov     dword ptr _MaxX,eax
                mov     eax,esi
                mov     dword ptr _MinY,edx
                dec     eax
                mov     edx,edi
                mov     dword ptr _MaxY,eax
                mov     eax,438c0000H
                sar     edx,1fH
                mov     dword ptr _viewdistance,eax
                mov     eax,edi
                sub     eax,edx
                sar     eax,1
                mov     dword ptr [esp],eax
                mov     edx,esi
                mov     eax,esi
                sar     edx,1fH
                sub     eax,edx
                sar     eax,1
                fld     dword ptr L307
                fild    dword ptr [esp]
                fmul    st,st(1)
                mov     dword ptr [esp],eax
                mov     ebp,457a0000H
                fild    dword ptr [esp]
                fmulp   st(2),st
                mov     dword ptr _max_clip_z,ebp
                mov     edx,41200000H
                fstp    dword ptr _halfscreenwidth_vd
                mov     dword ptr _min_clip_z,edx
                fstp    dword ptr _halfscreenheight_vd
                cmp     ecx,00000001H
                jne     short L271
                and     ebx,0000ffffH
                xor     edx,edx
                xor     eax,eax
                mov     dx,si
                mov     ax,di
                call    near ptr VesaInit_
                test    eax,eax
                je      near ptr L273
                mov     eax,dword ptr _ScreenWidth
                imul    eax,dword ptr _ScreenHeight
                add     eax,eax
                mov     ebx,0001f400H
                call    near ptr malloc_
                xor     edx,edx
                mov     dword ptr __VesaBuffer,eax
                call    near ptr memset_
                mov     eax,dword ptr _VesaPtr
                mov     ebx,00020000H
                mov     dword ptr __VesaScreen,eax
                mov     eax,00020000H
                mov     edx,0000007fH
                call    near ptr malloc_
                mov     dword ptr __VesaShadeTable,eax
                jmp     short L272
L271:           mov     eax,edi
                imul    eax,esi
                mov     ebx,0000fa00H
                xor     edx,edx
                call    near ptr malloc_
                mov     dword ptr __Buffer,eax
                call    near ptr memset_
                mov     eax,000a0000H
                mov     ebx,00010000H
                mov     dword ptr __Screen,eax
                mov     eax,00010000H
                mov     edx,0000007fH
                call    near ptr malloc_
                mov     dword ptr __ShadeTable,eax
L272:           call    near ptr memset_
                mov     eax,00000028H
                mov     esi,dword ptr +14H[esp]
                call    near ptr malloc_
                mov     dword ptr _World,eax
                mov     eax,000000b0H
                shl     esi,02H
                call    near ptr malloc_
                mov     dword ptr _Camera,eax
                mov     eax,esi
                call    near ptr malloc_
                mov     edx,dword ptr _World
                mov     dword ptr +20H[edx],eax
                mov     eax,esi
                call    near ptr malloc_
                mov     edx,dword ptr _World
                mov     dword ptr +24H[edx],eax
                mov     eax,00000001H
L273:           add     esp,00000004H
                pop     ebp
                pop     edi
                pop     esi
                ret     0004H
                mov     eax,eax
Engine_UnInit_: push    edx
                mov     edx,eax
                mov     eax,dword ptr _World
                mov     eax,dword ptr +20H[eax]
                call    near ptr free_
                mov     eax,dword ptr _World
                mov     eax,dword ptr +24H[eax]
                call    near ptr free_
                mov     eax,dword ptr _World
                call    near ptr free_
                mov     eax,dword ptr _Camera
                call    near ptr free_
                cmp     edx,00000001H
                jne     short L274
                mov     eax,dword ptr __VesaBuffer
                call    near ptr free_
                mov     eax,dword ptr __VesaShadeTable
                call    near ptr free_
                call    near ptr VesaUnInit_
                jmp     short L275
L274:           mov     eax,dword ptr __Buffer
                call    near ptr free_
                mov     eax,dword ptr __ShadeTable
                call    near ptr free_
L275:           mov     eax,00000003H
                int     10H
                pop     edx
                ret     
_TEXT           ENDS

CONST           SEGMENT DWORD PUBLIC USE32 'DATA'
L276            DB      00H,00H,00H,80H,00H,00H,30H,43H
L277            DB      00H,00H,00H,80H,00H,00H,30H,42H
L278            DB      00H,00H,00H,80H,00H,00H,0f0H,42H
L279            DB      00H,00H,00H,80H,00H,00H,30H,42H
L280            DB      00H,00H,00H,80H,00H,00H,30H,43H
L281            DB      00H,00H,00H,80H,00H,00H,30H,42H
L282            DB      00H,00H,00H,80H,00H,00H,0f0H,42H
L283            DB      00H,00H,00H,80H,00H,00H,30H,42H
L284            DB      00H,00H,00H,80H,00H,00H,30H,42H
L285            DB      00H,00H,00H,80H,00H,00H,30H,42H
L286            DB      00H,00H,00H,80H,00H,00H,30H,42H
L287            DB      00H,00H,40H,40H
L288            DB      00H,00H,00H,40H
L289            DB      00H,00H,0a0H,40H
L290            DB      00H,00H,80H,40H
L291            DB      00H,00H,0feH,42H
L292            DB      00H,00H,0feH,42H
L293            DB      00H,00H,20H,43H
L294            DB      00H,00H,0c8H,42H
L295            DB      00H,00H,00H,43H
L296            DB      72H,62H,00H
L297            DB      00H,00H,00H,3fH
L298            DB      00H,00H,00H,3fH
L299            DB      00H,00H,7cH,42H
L300            DB      00H,00H,00H,3cH
L301            DB      00H,00H,00H,3fH
L302            DB      00H,00H,00H,00H,00H,80H,4fH,40H
L303            DB      00H,00H,7cH,42H
L304            DB      44H,17H,41H,54H,0fbH,21H,0a9H,3fH
L305            DB      00H,00H,00H,00H,00H,0c0H,5fH,40H
L306            DB      00H,00H,7cH,42H
L307            DB      0a1H,0eH,6aH,3bH
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
                PUBLIC  _Intensity 
                PUBLIC  __innervadd 
                PUBLIC  __y2 
                PUBLIC  __y1 
                PUBLIC  __innerv 
                PUBLIC  __inneru 
                PUBLIC  __inneruadd 
                PUBLIC  _MinX 
                PUBLIC  _MinY 
                PUBLIC  __VesaPhongMap 
                PUBLIC  __VesaShadeTable 
                PUBLIC  _viewdistance 
                PUBLIC  __Texture1 
                PUBLIC  __Texture2 
                PUBLIC  __ShadeTable 
                PUBLIC  _halfscreenheight_vd 
                PUBLIC  _ScreenHeight 
                PUBLIC  _Camera 
                PUBLIC  _halfscreenwidth_vd 
                PUBLIC  _min_clip_z 
                PUBLIC  _World 
                PUBLIC  __PhongMap 
                PUBLIC  _MaxY 
                PUBLIC  _MaxX 
                PUBLIC  _max_clip_z 
                PUBLIC  _ScreenWidth 
                PUBLIC  __DivTable 
                PUBLIC  __Screen 
                PUBLIC  __VesaScreen 
                PUBLIC  __VesaBuffer 
                PUBLIC  __Buffer 
                PUBLIC  _chunk 
                ORG     00000000H
_Edge           LABEL   BYTE
                ORG     00000fa0H
_Intensity      LABEL   BYTE
                ORG     00001020H
__innervadd     LABEL   BYTE
                ORG     00001024H
__y2            LABEL   BYTE
                ORG     00001028H
__y1            LABEL   BYTE
                ORG     0000102cH
__innerv        LABEL   BYTE
                ORG     00001030H
__inneru        LABEL   BYTE
                ORG     00001034H
__inneruadd     LABEL   BYTE
                ORG     00001038H
_MinX           LABEL   BYTE
                ORG     0000103cH
_MinY           LABEL   BYTE
                ORG     00001040H
__VesaPhongMap  LABEL   BYTE
                ORG     00001044H
__VesaShadeTable LABEL   BYTE
                ORG     00001048H
_viewdistance   LABEL   BYTE
                ORG     0000104cH
__Texture1      LABEL   BYTE
                ORG     00001050H
__Texture2      LABEL   BYTE
                ORG     00001054H
__ShadeTable    LABEL   BYTE
                ORG     00001058H
_halfscreenheight_vd LABEL   BYTE
                ORG     0000105cH
_ScreenHeight   LABEL   BYTE
                ORG     00001060H
_Camera         LABEL   BYTE
                ORG     00001064H
_halfscreenwidth_vd LABEL   BYTE
                ORG     00001068H
_min_clip_z     LABEL   BYTE
                ORG     0000106cH
_World          LABEL   BYTE
                ORG     00001070H
__PhongMap      LABEL   BYTE
                ORG     00001074H
_MaxY           LABEL   BYTE
                ORG     00001078H
_MaxX           LABEL   BYTE
                ORG     0000107cH
_max_clip_z     LABEL   BYTE
                ORG     00001080H
_ScreenWidth    LABEL   BYTE
                ORG     00001084H
__DivTable      LABEL   BYTE
                ORG     00001088H
__Screen        LABEL   BYTE
                ORG     0000108cH
__VesaScreen    LABEL   BYTE
                ORG     00001090H
__VesaBuffer    LABEL   BYTE
                ORG     00001094H
__Buffer        LABEL   BYTE
                ORG     00001098H
_chunk          LABEL   BYTE
                ORG     0000109cH
L308            LABEL   BYTE
                ORG     000010a0H
L309            LABEL   BYTE
                ORG     000014a0H
_BSS            ENDS

                END
