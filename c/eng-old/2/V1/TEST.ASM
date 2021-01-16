.386p
                NAME    TEST
                EXTRN   _cstart_ :BYTE
                EXTRN   World_Load_ :BYTE
                EXTRN   __CHK :BYTE
DGROUP          GROUP   CONST,CONST2,_DATA,_BSS
_TEXT           SEGMENT BYTE PUBLIC USE32 'CODE'
                ASSUME  CS:_TEXT ,DS:DGROUP,SS:DGROUP
                PUBLIC  main_ 
main_:          push    00000004H
                call    near ptr __CHK
                mov     eax,offset DGROUP:L1
                jmp     near ptr World_Load_
_TEXT           ENDS

CONST           SEGMENT DWORD PUBLIC USE32 'DATA'
L1              DB      74H,65H,73H,74H,2eH,6fH,62H,6aH
                DB      00H
CONST           ENDS

CONST2          SEGMENT DWORD PUBLIC USE32 'DATA'
CONST2          ENDS

_DATA           SEGMENT DWORD PUBLIC USE32 'DATA'
_DATA           ENDS

_BSS            SEGMENT DWORD PUBLIC USE32 'BSS'
_BSS            ENDS

                END
