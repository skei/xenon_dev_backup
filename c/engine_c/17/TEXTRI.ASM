.486p
.MODEL FLAT
;JUMPS
LOCALS

b equ byte ptr
w equ word ptr
d equ dword ptr

_TEXT SEGMENT public use32 'CODE'

;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
extrn _buffer      : dword
extrn _usetexture1 : dword
extrn _tricoords   : dword
extrn _texcoords   : dword

public textri_
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
align 4
; 960-mul tabell = 600 dwords = 2.4K
i=0
rept 200
 dd (i*960)+320
 i=i+1
endm
Mul960 label dword
rept 400
 dd (i*960)+320
 i=i+1
endm
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

; EAX, EBX, ECX
PROC textri_ NEAR
 push ebp
 mov esi,eax
 mov edi,ebx
 mov ebp,ecx
 mov eax,[_buffer]
 mov d [FillerScreen],eax
 mov eax,[_usetexture1]
 mov d [BLAHUT1],eax
 mov eax,d [_TriCoords+esi]
 mov ebx,d [_TriCoords+edi]
 mov ecx,d [_TriCoords+ebp]
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
; Sjekk om X'ene er innenfor 320
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
 mov edx,01400000h
 cmp edx,eax
 sbb dx,dx
 cmp edx,ebx
 sbb dx,0
 cmp edx,ecx
 sbb dx,0
 mov [NeedClip],dx
 cmp dx,-3      ; Alle X'ene er over 320, s†,,QUIT
 je @@QuitPoly
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
; Sjekk om vi trenger † clippe Y
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
 mov dx,200
 cmp dx,ax
 sbb [NeedClip],0
 cmp dx,bx
 sbb [NeedClip],0
 cmp dx,cx
 sbb [NeedClip],0
; Sort‚r
 cmp ax,bx
 jl @@1
 xchg eax,ebx
 xchg esi,edi
@@1:
 cmp ax,cx
 jl @@2
 xchg eax,ecx
 xchg esi,ebp
@@2:
 cmp bx,cx
 jl @@3
 xchg ebx,ecx
 xchg edi,ebp
@@3:
; Y-ene i riktig rekkef›lge... N† sjekk X'er
 cmp ax,bx       ;Y1=Y2 ? Is†fall, sjekk om X'ene m† swappes
 jne @@4
 cmp eax,ebx
 jl @@4
 xchg eax,ebx
 xchg esi,edi
@@4:
 cmp bx,cx       ;Y2=Y3 ? Is†fall, sjekk om X'ene m† swappes
 jne @@5
 cmp ebx,ecx
 jl @@5
 xchg ebx,ecx
 xchg edi,ebp
@@5:
; 1     1     1     1 2
;  2   2     2 3     3
;3       3
; N† burde de v‘re sortert etter Y...
 cmp ax,199
 jg @@QuitPoly
 cmp cx,0
 jl @@QuitPoly
 mov d [y1],eax
 mov d [y2],ebx
 mov d [y3],ecx
 mov eax,d [_TexCoords+esi]
 mov ebx,d [_TexCoords+edi]
 mov ecx,d [_TexCoords+ebp]
 mov d [ty1],eax
 mov d [ty2],ebx
 mov d [ty3],ecx

; Utregning av sloper..
 mov [Parts],0
; Slope13
 mov eax,d [y3]
 sub eax,d [y1]
 xor ebx,ebx
 mov [y3_minus_y1],ax
 test ax,ax
 jz @@QuitPoly
 mov bx,ax
 cdq
 idiv ebx
 mov [slope13],eax
; Slope12
 mov eax,d [y2]
 sub eax,d [y1]
 xor ebx,ebx
 mov [y2_minus_y1],ax
 test ax,ax
 jz @@s1
 or [Parts],1
 mov bx,ax
 cdq
 idiv ebx
 mov [slope12],eax
@@s1:
; Slope23
 mov eax,d [y3]
 sub eax,d [y2]
 xor ebx,ebx
 mov [y3_minus_y2],ax
 test ax,ax
 jz @@s2
 or [Parts],2
 mov bx,ax
 cdq
 idiv ebx
 mov [slope23],eax
@@s2:
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
 cmp [Parts],3
 jne @@Skip4
; X4 = x1 + S13*(y2-y1)
 mov eax,[Slope13]
 movsx ebx,[y2_minus_y1]
 imul ebx
 shld ebp,eax,16
 add bp,[x1]
 cmp bp,[x2]            ; bp = x4
 je @@QuitPoly
 mov [x4],bp            ; *************
; Tx4 = tx1 + Txs13*(y2-y1)
 mov ax,[tx3]
 sub ax,[tx1]
 shl ax,8
 imul [y2_minus_y1]
 idiv [y3_minus_y1]
 mov si,[tx1]
 shl si,8
 add ax,si           ; si = tx4
 mov [tx4],ax        ; ************** 8.8
; Ty4 = ty1 + Tys13*(y2-y1)
 mov ax,[ty3]
 sub ax,[ty1]
 shl ax,8
 imul [y2_minus_y1]
 idiv [y3_minus_y1]
 mov si,[ty1]
 shl si,8
 add ax,si
 mov [ty4],ax           ; *****************
@@Skip4:
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
; Vi sjekker for hvilket, og utf›rer relevant polygon-filler rutine
 mov ax,[y2]
 cmp ax,[y1]
 je @@FlatTop             ; Y1=y2 - Flat Top
 cmp ax,[y3]            ; Y2=y3 - Flat Bottom
 je @@FlatBottom
 mov eax,[slope12]
 cmp eax,[slope13]
 je @@QuitPoly
 jl @@FlatRight
 Jmp @@FlatLeft
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
@@FlatTop:
; 1 2
;  3
; F›rst konstantene til Code-segmentet...
; LeftXAdder = Slope13
 mov eax,[Slope13]
 mov w [LeftFracAdder],ax
 rol eax,16
 mov w [LeftIntAdder],ax
; RightXAdder = Slope23
 mov eax,[Slope23]
 mov w [RightFracAdder],ax
 rol eax,16
 mov w [RightIntAdder],ax
; LeftTxAdder = (tx3-tx1)/(y3-y1) 8.8
 mov ax,[tx3]
 sub ax,[tx1]
 shl ax,8
 cwd
 idiv [y3_minus_y1]
 mov w [LeftTxAdder],ax
; LeftTyAdder = (ty3-ty1)/(y3-y1) 8.8
 mov ax,[ty3]
 sub ax,[ty1]
 shl ax,8
 cwd
 idiv [y3_minus_y1]
 mov w [LeftTyAdder],ax
; TxAdder = (tx2-tx1)/(x2-x1)
 mov ax,[tx2]
 sub ax,[tx1]
 shl ax,8
 cwd
 mov bx,[x2]
 sub bx,[x1]
 jz @@QuitPoly
 idiv bx
 mov w [TxAdder],ax
; TyAdder = (ty2-ty1)/(x2-x1)
 mov ax,[ty2]
 sub ax,[ty1]
 shl ax,8
 cwd
 idiv bx
 mov w [TyAdder],ax
; Det var konstantene, n† kommer registrene... phew...
; EAX = TXPos:0 | TYPos:0
 mov ax,[Tx1]
 shl eax,16
 mov ax,[Ty1]
 shl eax,8
; EDX = 0 : LeftX
 movzx edx,[x1]
; FillerScreen = (y*320) + [_buffer]
 movsx ebx,[y1]
 shl ebx,2
 mov ebx,[mul960+ebx]
 add ebx,[_buffer]
 mov d [FillerScreen],ebx
; EBP = 0 : RightX
 movzx ebp,[x2]
 inc ebp
; ESI = Height : -
 movzx esi,[y3_minus_y1]
 mov [PolyHeight],esi

 call @@Filler
 pop ebp
 ret
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
@@FlatBottom:
; 1
;2 3
; F›rst konstantene til Code-segmentet...
; LeftXAdder = Slope12
 mov eax,[Slope12]
 rol eax,16
; add ax,320
 mov w [LeftIntAdder],ax
 rol eax,16
 mov w [LeftFracAdder],ax
; RightXAdder = Slope13
 mov eax,[Slope13]
 rol eax,16
; add ax,320
 mov w [RightIntAdder],ax
 rol eax,16
 mov w [RightFracAdder],ax
; LeftTxAdder = (tx2-tx1)/(y2-y1) 8.8
 mov ax,[tx2]
 sub ax,[tx1]
 shl ax,8
 cwd
 idiv [y2_minus_y1]
 mov w [LeftTxAdder],ax
; LeftTyAdder = (ty2-ty1)/(y2-y1) 8.8
 mov ax,[ty2]
 sub ax,[ty1]
 shl ax,8
 cwd
 idiv [y2_minus_y1]
 mov w [LeftTyAdder],ax
; TxAdder = (tx3-tx2)/(x3-x2)
 mov ax,[tx3]
 sub ax,[tx2]
 shl ax,8
 cwd
 mov bx,[x3]
 sub bx,[x2]
 jz @@QuitPoly
 idiv bx
 mov w [TxAdder],ax
; TyAdder = (ty3-ty2)/(x3-x2)
 mov ax,[ty3]
 sub ax,[ty2]
 shl ax,8
 cwd
 idiv bx
 mov w [TyAdder],ax
; Det var konstantene, n† kommer registrene... phew...
; EAX = TXPos:0 | TYPos:0
 mov ax,[Tx1]
 shl eax,16
 mov ax,[Ty1]
 shl eax,8
; EDX = 0 : LeftX
 movzx edx,[x1]
; FillerScreen = (y*320) + [Buffer]
 movsx ebx,[y1]
 shl ebx,2
 mov ebx,[Mul960+ebx]
 add ebx,[_buffer]
 mov d [FillerScreen],ebx
; EBP = 0 : RightX
 mov ebp,edx
 inc ebp
; ESI = Height : -
 movzx esi,[y3_minus_y1]
 mov [PolyHeight],esi

 call @@Filler
 pop ebp
 ret

@@FlatLeft:
; 1
;  2
;3
; F›rst konstantene til Code-segmentet...
; LeftXAdder = Slope13
 mov eax,[Slope13]
 rol eax,16
; add ax,320
 mov w [LeftIntAdder],ax
 rol eax,16
 mov w [LeftFracAdder],ax
; RightXAdder = Slope12
 mov eax,[Slope12]
 rol eax,16
; add ax,320
 mov w [RightIntAdder],ax
 rol eax,16
 mov w [RightFracAdder],ax
; LeftTxAdder = (tx3-tx1)/(y3-y1) 8.8
 mov ax,[tx3]
 sub ax,[tx1]
 shl ax,8
 cwd
 idiv [y3_minus_y1]
 mov w [LeftTxAdder],ax
; LeftTyAdder = (ty3-ty1)/(y3-y1) 8.8
 mov ax,[ty3]
 sub ax,[ty1]
 shl ax,8
 cwd
 idiv [y3_minus_y1]
 mov w [LeftTyAdder],ax
; TxAdder = (tx2-tx4)/(x2-x4)
 mov ax,[tx2]
 shl ax,8
 sub ax,[tx4]
 cwd
 mov bx,[x2]
 sub bx,[x4]
 jz @@QuitPoly
 idiv bx
 mov w [TxAdder],ax
; TyAdder = (ty2-ty4)/(x2-x4)
 mov ax,[ty2]
 shl ax,8
 sub ax,[ty4]
 cwd
 idiv bx
 mov w [TyAdder],ax
; Det var konstantene, n† kommer registrene... phew...
; EAX = TXPos:0 | TYPos:0
 mov ax,[Tx1]
 shl eax,16
 mov ax,[Ty1]
 shl eax,8
; EDX = 0 : LeftX
 movzx edx,[x1]
; FillerScreen = (y*320) + [Buffer]
 movsx ebx,[y1]
 shl ebx,2
 mov ebx,[Mul960+ebx]
 add ebx,[_buffer]
 mov d [FillerScreen],ebx
; EBP = 0 : RightX
 mov ebp,edx
 inc ebp
; ESI = Height : -
 movzx esi,[y2_minus_y1]
 mov [PolyHeight],esi

 Call @@Filler
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
; RightXAdder = Slope23
 push eax
; push ecx
 mov eax,[Slope23]
 rol eax,16
; add ax,320
 mov w [RightIntAdder],ax
 rol eax,16
 mov w [RightFracAdder],ax
; pop ecx
 pop eax
; ESI = Height : -
 movzx esi,[y3_minus_y2]
 mov [PolyHeight],esi

 call @@Filler
 pop ebp
 ret

@@FlatRight:
; 1
;2
;  3
; F›rst konstantene til Code-segmentet...
; LeftXAdder = Slope12
 mov eax,[Slope12]
 mov w [LeftFracAdder],ax
 rol eax,16
; add ax,320
 mov w [LeftIntAdder],ax
; RightXAdder = Slope13
 mov eax,[Slope13]
 mov w [RightFracAdder],ax
 rol eax,16
; add ax,320
 mov w [RightIntAdder],ax
; LeftTxAdder = (tx2-tx1)/(y2-y1) 8.8
 mov ax,[tx2]
 sub ax,[tx1]
 shl ax,8
 cwd
 idiv [y2_minus_y1]
 mov w [LeftTxAdder],ax
; LeftTyAdder = (ty2-ty1)/(y2-y1) 8.8
 mov ax,[ty2]
 sub ax,[ty1]
 shl ax,8
 cwd
 idiv [y2_minus_y1]
 mov w [LeftTyAdder],ax
; TxAdder = (tx4-tx2)/(x4-x2)
 mov ax,[tx4]
 mov bx,[tx2]
 shl bx,8
 sub ax,bx
 cwd
 mov bx,[x4]
 sub bx,[x2]
 jz @@QuitPoly
 idiv bx
 mov w [TxAdder],ax
; TyAdder = (ty4-ty2)/(x4-x2)
 mov ax,[ty4]
 mov cx,[ty2]
 shl cx,8
 sub ax,cx
 cwd
 idiv bx
 mov w [TyAdder],ax
; Det var konstantene, n† kommer registrene... phew...
; EAX = TXPos:0 | TYPos:0
 mov ax,[Tx1]
 shl eax,16
 mov ax,[Ty1]
 shl eax,8
; EDX = 0 : LeftX
 movzx edx,[x1]
; FillerScreen = (y*320) + [Buffer]
 movsx ebx,[y1]
 shl ebx,2
 mov ebx,[Mul960+ebx]
 add ebx,[_buffer]
 mov d [FillerScreen],ebx
; EBP = 0 : RightX
 mov ebp,edx
 inc ebp
; ESI = Height : -
 movzx esi,[y2_minus_y1]
 mov [PolyHeight],esi
 Call @@Filler
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
 push eax
 push edx
; LeftXAdder = Slope23
 mov eax,[Slope23]
 mov w [LeftFracAdder],ax
 rol eax,16
; add ax,320
 mov w [LeftIntAdder],ax
; LeftTxAdder = (tx3-tx2)/(y3-y2) 8.8
 mov ax,[tx3]
 sub ax,[tx2]
 shl ax,8
 cwd
 idiv [y3_minus_y2]
 mov w [LeftTxAdder],ax
; LeftTyAdder = (ty3-ty2)/(y3-y2) 8.8
 mov ax,[ty3]
 sub ax,[ty2]
 shl ax,8
 cwd
 idiv [y3_minus_y2]
 mov w [LeftTyAdder],ax

 pop edx
 pop eax
; ESI = Height : -
; ESI = Height : -
 movzx esi,[y3_minus_y2]
 mov [PolyHeight],esi

 call @@Filler
 pop ebp
 ret
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
@@QuitPoly:
 pop ebp
 ret
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
@@Filler:
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
;EAX *  Xint:XFrac    : YInt:YFrac
;EBX    temp.......................
;ECX
;EDX *  RightFrac     : LeftInt (+y*320)
;ESI                  : temp
;EDI    Screen-pos...............
;EBP *  LeftFrac      : RightInt (+y*320)
; [PolyHeight]
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
@@Outer:
 push bp                ; Save RightInt
 sub bp,dx              ; Right - Left = Line-length
 jz @@SkipLine
 movsx edi,dx
 add edi,12345678h      ; SelfMod code - sett inn verdien direkte her!
FillerScreen = $-4
 push eax               ; Save Texture index
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
@@Inner:
 xor ebx,ebx
 shld ebx,eax,8
 mov bh,ah          ; EBX = Texture Index

 add eax,12345678h      ; .. 78 56 34 12
TXAdder = $-2
TYAdder = $-4
 add ebx,12345678h      ;; [_usetexture1]
BLAHUT1 = $-4
 mov bl,[ebx]
 mov [edi],bl                   ;<- Page fault!
 inc edi
 dec bp
 jnz @@Inner
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
 pop eax
@@SkipLine:
 add d [FillerScreen],960
 add eax,12345678h
LeftTXAdder = $-2
LeftTyAdder = $-4
 pop bp
 add edx,12340000h      ; .. 00 00 34 12
RightFracAdder = $-2
 adc ebp,12345678h      ; .. 78 56 34 12
RightIntAdder = $-4
LeftFracAdder = $-2
 adc dx,1234h
LeftIntAdder = $-2
 dec [PolyHeight]
 jnz @@Outer
 ret
ENDP
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
align 4
PolyHeight dd ?

y1 dw ?
x1 dw ?
y2 dw ?
x2 dw ?
y3 dw ?
x3 dw ?
x4 dw ?

ty1 dw ?
tx1 dw ?
ty2 dw ?
tx2 dw ?
ty3 dw ?
tx3 dw ?
ty4 dw ?
tx4 dw ?

Slope12 dd ?
Slope13 dd ?
Slope23 dd ?
y2_minus_y1 dw ?
y3_minus_y1 dw ?
y3_minus_y2 dw ?
Parts db ?      ; Bit 1 = HasTop, Bit 2 = HasBottom
NeedClip dw ?
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
ENDS _TEXT
end
