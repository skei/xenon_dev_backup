;DOSSEG
.MODEL MEDIUM
.386
.STACK
;JUMPS
LOCALS
                                    
ITER = 51
       
;° DATA °°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
.DATA  
             
StartX DD ?
StartY DD -100           ; -100
Mirror DW 200 * 320-1   ; 200*320-1
Real   DD ?
Imag   DD ?
Total  DD ?
       
;° CODE °°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
.CODE  
       
PROC Mandel
 mov AX, 3300H
 xor DI, DI  
 mov AX, 0A000H
 mov ES, AX  
TopOfY:      
 mov EAX,[StartY]
 imul EAX, 14680
 mov [Imag], EAX   
 mov [StartX], -160
TopOfX:    
 mov EAX, [StartX]
 imul EAX, 10813
 mov EBP, EAX   
 sub EBP, 576717
 mov Real, EBP  
 mov ESI, [Imag]
 xor CL, CL     
 mov EAX, EBP   
 sar EAX, 10    
 mov EDX, EAX   
 imul EDX       
 mov EBX, EAX   
 mov EAX, ESI   
 sar EAX, 10    
 mov EDX, EAX   
 imul EDX       
 add EBX, EAX   
 sal EBX, 2     
 sal EBP, 3     
 add EBX, EBP   
 cmp EBX, -3932160
 jl BailOut     
 sar EBP, 3     
TopOfIter:      
 mov EAX, EBP   
 sar EAX, 10    
 mov EBX, EAX   
 imul EBX, EAX  
 mov [Total], EBX 
 mov EAX, ESI   
 sar EAX, 10    
 push EAX       
 mov EDX, EAX
 imul EDX   
 sub EBX, EAX
 add [Total], EAX
 mov EAX, EBP
 sar EAX, 10
 pop EDX    
 imul EDX   
 sal EAX, 1 
 mov EBP, EBX
 add EBP, Real
 mov ESI, EAX
 add ESI, Imag
 cmp [Total], 16 * 1048576
 ja OutIter 
 inc CL     
 cmp CL, ITER
 jne TopOfIter
 xor CL, CL 
OutIter:        
BailOut:    
 add CL, 16 
 mov ES:[DI], CL
 inc DI     
 mov BX, DI 
 add BX, [Mirror]
 mov ES:[BX], CL 
 inc [StartX]
 cmp [StartX], 160
 jne TopOfX  
 sub [Mirror], 640
 inc [StartY]
 cmp [StartY], 1
 jne TopOfY  
 ret         
ENDP         
             
;° EntryPoint from DOS °°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
EntryPoint:  
 mov ax,@data
 mov ds,ax   
             
 mov AX, 0013H
 int 10H
 Call Mandel
 xor ah,ah 
 int 16h
 mov ax,3
 int 10h
 mov ax,4c00h
 int 21h    
End EntryPoint
