; Various DemoRoutines..

.486p
.MODEL FLAT
;JUMPS
LOCALS
.CODE

public GTfiller_

public _outerTX
public _outerTY
public _outerC

public _outerTXadder
public _outerTYadder
public _outerCadder

public _innerTXadder
public _innerTYadder
public _innerCadder

public _LeftX
public _RightX
public _LeftXadder
public _RightXadder

;° DATA °°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
; _texture1 dd ?

;° CODE °°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

; -----

; EAX = - structure pos (outer) -
; EBX =
; ECX = - linelength -
; EDX =             TXint:TXfrac
; ESI = TEXTUREı    TYint:TYfrac
; EDI = - screen pos -
; EBP = SHADETABLEı Cint:Cfrac

; Notes:
; ı 64k aligned (lower word = 0)
; EAX - struc pos pointer... Points to LAST item. counts back to zero
; left/right x in structure: = y*320+x
; ------------------------------
; initialized at entry
; esi = 64k aligned texture
; edi = 64k aligned shade table
PROC GTfiller_ NEAR
 push ebp
 mov ebp,edi
@@outer:
 mov edi,12345678h      ; left x / screenpos
_LeftX = $-4
 mov ecx,12345678h      ; right x
_RightX = $-4
 shr edi,10
 shr ecx,10
 sub ecx,edi            ; right-left = linelength
 jz skipline

 mov dx,1122h           ; 22 11
_outerTX = $-2
 mov si,1122h           ; 22 11
_outerTY = $-2
 mov bp,1122h           ; 22 11
_outerC = $-2
 push eax               ; poly-height
; ----------
@@inner:
 mov eax,esi       ; eax = texture + ty*256
 mov al,dh         ; eax = texture + ty*256+tx
 mov ebx,ebp       ; ebx = shadetable + c*256
 mov bl,[eax]      ; ebx = shadetable + c*256 + texel
 mov al,[ebx]      ; al = pixel to plot
 mov [edi],al      ; put pixel
 inc edi           ; inc screenpos
 add dx,1234h ; inc tx/u
_innerTXadder = $-2
 add si,1234h ; inc ty/v
_innerTYadder = $-2
 add bp,1234h ; inc color
_innerCadder = $-2
 dec ecx           ; dec linelength counter
 jnz @@inner
; ----------
skipline:
 add word ptr [_outerTX],1122h    ; 22 11
_outerTXadder = $-2
 add word ptr [_outerTY],1122h    ; 22 11
_outerTYadder = $-2
 add word ptr [_outerC],1122h     ; 22 11
_outerCadder = $-2
 add dword ptr [_LeftX],12345678h
_LeftXadder = $-4
 add dword ptr [_RightX],12345678h
_RightXadder = $-4

 pop eax           ; restore struc pos pointer
 dec eax
 jnz @@outer         ; dec if not finished
 pop ebp
ENDP

;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
end                                  
