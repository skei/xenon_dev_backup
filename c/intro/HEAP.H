 Allocate Memory Block
 In   : AX = 0501h
        BX:CX = Size of memory block to allocate in bytes
 Out  : If function was successful   : Carry flag is clear
                                       BX:CX = Linear address
                                       SI:DI = Memory block handle
        If function was unsuccessful : Carry flag is set

 Free Memory Block
 In   : AX = 0502h
        SI:DI = Handle of memory block to free
 Out  : If function was successful   : Carry flag is clear
        If function was unsuccessful : Carry flag is set

