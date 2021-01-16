;XMS utils

;DOSSEG
.MODEL MEDIUM
.386
.STACK
;JUMPS
LOCALS

b EQU byte ptr
w EQU word ptr
d EQU dword ptr
 
;° DATA °°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
.DATA
 
; Entry point of the XMS routine in memory
XMSEntryPoint dd ? ;POINTER

Label EMMParamBlock dword
 BytesToMoveLo dw ?       ; Low word of bytes to move. NB: Must be even! 
 BytesToMoveHi dw ?       ; High word of bytes to move 
 SourceHandle dw ?        ; Handle number of source (SH=0=conventional memory 
 SourceOffsetLo dw ?      ; Low word of source offset, or OFS if SH=0 
 SourceOffsetHi dw ?      ; High word of source offset, or SEG if SH=0
 DestinationHandle dw ?   ; Handle number of destination (DH=0 => conventional memory)
 DestinationOffsetLo dw ? ; Low word of destination offset, or OFS if DH=0
 DestinationOffsetHi dw ? ; High word of destination offset, or SEG if DH=0
 
;° CODE °°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
.CODE
    
                                                               
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
; checks if there is a XMS driver installed, and in case it initialize
; the XMSEntryPoint Variable.                  
; AL=80h, Yes                                 
; Else    No                                                            
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ  
PROC XMSAvailable
 mov  ax,4300h                  ; AX = 4300h => INSTALLATION CHECK 
 int  2Fh                       ; use int 2Fh XMS
 cmp  al,80h                    ; is a XMS driver installed? 
 jne  @@1                                                    
 mov  ax,4310h                  ; AX = 4310h => GET DRIVER ADDRESS 
 int  2Fh                                                          
 mov  w [XMSEntryPoint+0],BX    ; initialize low word of XMSEntryPoint 
 mov  w [XMSEntryPoint+2],ES    ; initialize high word of XMSEntryPoint 
@@1:                                                                   
ENDP     
         
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
; returns size of largest contiguous block of XMS in Kb's in AX
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
PROC XMSMaxAvail                              
 mov  ah,08h                    ; 'Query free extended memory' function
 call d [XMSEntryPoint]                                
ENDP                            
                                                          
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
; allocates specified numbers of Kb's of XMS and return a handle
; to this XMS block             
; DX = Number of Kb's to Get    
; Returns handle in DX          
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
PROC XMSGetMem                    
 mov  ah,09h                    ; 'Allocate extended memory block' function 
 call d [XMSEntryPoint]                                                     
ENDP                                                                        
                                                                            
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ      
; DX=Handle                                                                 
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ      
PROC XMSFreeMem                                                             
 mov  ah,0Ah                    ; 'Free extended memory block' function 
 call d [XMSEntryPoint]                                                     
ENDP                                                                    
                                                                        
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
                                
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
PROC XMSMove
 push ds                        
 push es                        
 push ds                        
 pop  es                        
 mov  ah,0Bh                    ; 'Move extended memory block' function
 lds  si,EMMParamBlock          ; DS:SI -> data to pass to the XMS routine
 call w [es:XMSEntryPoint]      
 pop  es                        
 pop  ds                        
ENDP                     
                         
                                                                        
;° EntryPoint from DOS °°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°  
EntryPoint:                                                             
 mov ax,@data                                                           
 mov ds,ax                                                              
                                                                        
 mov ax,4c00h                                                           
 int 21h                                                                
End EntryPoint                                                          
