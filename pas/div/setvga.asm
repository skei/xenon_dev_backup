;DOSSEG
.MODEL MEDIUM
.386
.STACK
;JUMPS
LOCALS

;° DATA °°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
.DATA

;° CODE °°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
.CODE

;° EntryPoint from DOS °°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
EntryPoint:
 mov ax,@data
 mov ds,ax

 mov ax,13h
 int 10h

 mov ax,4c00h
 int 21h
End EntryPoint
