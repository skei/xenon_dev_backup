;Macroer for EGA

Allplanes MACRO
 Mov dx,3c4h
 mov ax,0f02h
 out dx,ax
ENDM

SetWriteMode1 MACRO
 Mov DX,3ceh
 mov ax,0105
 out dx,ax
ENDM

SetWriteMode0 MACRO
 mov dx,3ceh
 mov ax,0005h
 out dx,ax
ENDM

ReadBitPlane MACRO BitP
 mov dx,3ceh
 mov ah,BitP
 mov al,4
 out dx,ax
ENDM

BitPlan Macro BitP
 mov dx,3c4h
 mov ah,BitP
 mov al,2
 out dx,ax
ENDM

PutPixel16 MACRO x,y,plan
 mov ax,0a000h
 mov es,ax
 mov ax,y
 mov bx,40
 mul bx
 mov bx,x
 shr bx,3
 add ax,bx
 mov di,ax
 readbitplane plan
 mov cl,[es:di]
 mov bx,x
 and bl,7
 or cl,bl
 bitplan plan
 mov [es:di],cl
ENDM
