.386p

extrn MAIN_:byte

;public c0_entry

;_STACK SEGMENT PARA STACK USE32 'STACK'
; ORG 00000400H
;_STACK ENDS
;
;_DATA SEGMENT DWORD PUBLIC USE32 'DATA'
;_DATA ENDS
;
;_BSS SEGMENT DWORD PUBLIC USE32 'BSS'
;_BSS ENDS

DGROUP GROUP STACK,_BSS,DATA,CONST,_DATA

_TEXT SEGMENT DWORD PUBLIC USE32 'CODE'

ASSUME  CS:_TEXT,DS:DGROUP,ES:DGROUP,SS:DGROUP

; 께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께�

c0_entry:
 jmp     short __main
 db 'WATCOM'

__main:

 sti

 cld

 mov ax,_DATA
 mov ds,ax
 mov es,ax
 mov ss,ax

;again:
; in al,60h
; cmp al,1
; jne again

; mov ax,0ffffh
; mov [ds:0b8000h],ax

 call near ptr MAIN_

 mov ax,4c00h
 int 21h

; 께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께�

_TEXT ENDS

_DATA SEGMENT DWORD PUBLIC USE32 'DATA'
_DATA ENDS

CONST SEGMENT WORD PUBLIC USE32 'DATA'
CONST ENDS

DATA SEGMENT WORD PUBLIC USE32 'DATA'
DATA ENDS

_BSS SEGMENT WORD PUBLIC USE32 'BSS'
_BSS ENDS

STACK SEGMENT PARA STACK USE32 'STACK'
 ORG     00001000H
STACK ENDS

end c0_entry

