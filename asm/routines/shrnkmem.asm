        ; EXE program has all available memory allocated to it
        ; Release all memory except the amount currently being used
        ; End of stack is end of non-heap portion of program
        MOV BX,SP
        ADD BX,15       ;convert SP into paragraphs
        SHR BX,4
        MOV AX,SS       ;calculate size of program using ES PSP address
        ADD BX,AX
        MOV AX,ES
        SUB BX,AX
        MOV AH,4AH      ;resize memory block with PSP
        INT 21H         ;address in ES
