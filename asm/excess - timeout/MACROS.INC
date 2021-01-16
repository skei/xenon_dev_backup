;иммммммммммммммммммммммммммммммммммммммммммммммммммммммммммммммммм╩
;╨         USFUL MACROS FOR YOUR PROTECTED MODE PROGRAMS           ╨
;╨Written by Adam Seychell                                         ╨
;╨                                                                 ╨
;хммммммммммммммммммммммммммммммммммммммммммммммммммммммммммммммммм╪
          
TRUE  EQU 1
FALSE EQU 0
CRLF  EQU 10,13
          
Halt MACRO
 mov ax,4c00h
 int 21h  
ENDM      
          
;╟ FILE ╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟
        
;ддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддд
;                    OPEN A FILE
;ддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддд
; OpenFile     <Handle>,<'path\filename'>
; Example:      OpenFile      myfile,'c:\gif\bird.gif'
;ддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддд
OpenFile macro X, FileName, nameOFS
local skip_opfi                    
 IFB <nameOFS>                     
  mov edx,offset file_to_open&X    ; DS:EDX   points to file name
 ELSE                              
  mov edx,nameOFS                  
 ENDIF                             
 mov ax,3D02h		; open a file function
 Int 21h                           
 mov FileHandle&X , AX             
 jmp skip_opfi                     
file_to_open&X db FileName,0       
FileHandle&X   dw 0                
skip_opfi:                         
endm                               
                                   
;ддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддд
;              	         CREATE A FILE
;ддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддд
; Usage:       CreateFile     <Handle>,<'path\filename'>
; Example:      CreateFile      myfile,'drum.voc'
;ддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддд
CreateFile macro X, FileName , nameSEG , nameOFS
local skip_crfi                       
 jmp skip_crfi                        
file_to_open&X db FileName,0         
FileHandle&X   dw 0                  
skip_crfi:                            
 IFB <nameOFS>                        
  mov edx,offset file_to_open&X    ; DS:EDX   points to file name
 ELSE                                 
  mov edx,nameOFS
 ENDIF         
 mov ax,3C02h            ; creat a file function
 xor ecx,ecx
 Int 21h       
 mov FileHandle&X , AX
endm 
     
;ддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддд
;               	  CLOSE A FILE
;ддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддд
; Usage:         Close    <Handel>
; Example:       Close     myfile
;ддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддд
Close  macro  X
 mov ah,03Eh         ; close a file function
 mov BX,FileHandle&X
 int 21h
endm 
               
;ддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддд
;                   READ BLOCK FROM A FILE
;ддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддд
; Usage:   	BlockRead    <handel>,<32bit offset>,<bytes to read>
; Example:      BlockRead   myfile, EDX ,700000
;ддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддд
BlockRead  macro   X , POINTER, BYTE_COUNT
 push dword ptr BYTE_COUNT
 push dword ptr POINTER
 pop edx
 pop ecx
 mov BX,FileHandle&X
 mov ah,3Fh
 int 21h
endm 
                     
;ддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддд
;                   WRITE A BLOCK TO A FILE
; Usage:     BlockWrite  <handel>,<32bit offset>,<bytes to read>
; Example:     BlockWrite   myfile, EDX ,700000
;ддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддд
BlockWrite  macro  X , POINTER, BYTE_COUNT
 push dword ptr BYTE_COUNT
 push dword ptr POINTER
 pop edx
 pop ecx
 mov BX,FileHandle&X
 mov ah,40h
 int 21h
endm    
        
;ддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддд
; mode: 0 from start of file
;       1 current file position
;       2 end of file
Seek MACRO handle,origin,offset
 mov al,origin
 mov ah,42h   
 mov bx, mov BX,FileHandle&handle
 mov edx,offset
 int 21h
ENDM
    
;╟ STRING ╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟
    
;ддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддд
;              DISPLAY A STRING ON SCREEN
;ддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддд
; Usage:        Write   <' string to display '>
;ддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддд
Write MACRO STRING_, COLOR5_T
LOCAL TEXT5_T , skip_wrln
 push eax
 push edx
 jmp skip_wrln
 IFB <STRING_>
  TEXT5_T DB 36                
 ELSE 
  TEXT5_T DB STRING_,36
 ENDIF
skip_wrln:
 MOV EDX,OFFSET TEXT5_T
 mov ah,9
 int 21h
 pop edx
 pop eax
ENDM 
     
;ддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддд
;              DISPLAY A STRING ON SCREEN WITH CARAGE RETURN
;ддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддд
; Usage:       Writeln   <' string to display '>
;ддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддд
Writeln MACRO STRING_
LOCAL TEXT3_S                  
local skip_wrln                                               
 push eax
 push edx
 jmp skip_wrln
 IFB <STRING_>
  TEXT3_S DB 13,10,36
 ELSE
  TEXT3_S DB STRING_,13,10,36
 ENDIF
skip_wrln:
 mov EDX,offset TEXT3_S
 mov ah,9
 Int 21h
 pop edx
 pop eax
ENDM 

