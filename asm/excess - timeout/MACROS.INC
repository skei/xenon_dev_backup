;浜様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様�
;�         USFUL MACROS FOR YOUR PROTECTED MODE PROGRAMS           �
;�Written by Adam Seychell                                         �
;�                                                                 �
;藩様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様�
          
TRUE  EQU 1
FALSE EQU 0
CRLF  EQU 10,13
          
Halt MACRO
 mov ax,4c00h
 int 21h  
ENDM      
          
;� FILE 旭旭旭旭旭旭旭旭旭旭旭旭旭旭旭旭旭旭旭旭旭旭旭旭旭旭旭旭旭旭旭
        
;陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳�
;                    OPEN A FILE
;陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳�
; OpenFile     <Handle>,<'path\filename'>
; Example:      OpenFile      myfile,'c:\gif\bird.gif'
;陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳�
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
                                   
;陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳�
;              	         CREATE A FILE
;陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳�
; Usage:       CreateFile     <Handle>,<'path\filename'>
; Example:      CreateFile      myfile,'drum.voc'
;陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳�
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
     
;陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳�
;               	  CLOSE A FILE
;陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳�
; Usage:         Close    <Handel>
; Example:       Close     myfile
;陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳�
Close  macro  X
 mov ah,03Eh         ; close a file function
 mov BX,FileHandle&X
 int 21h
endm 
               
;陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳�
;                   READ BLOCK FROM A FILE
;陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳�
; Usage:   	BlockRead    <handel>,<32bit offset>,<bytes to read>
; Example:      BlockRead   myfile, EDX ,700000
;陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳�
BlockRead  macro   X , POINTER, BYTE_COUNT
 push dword ptr BYTE_COUNT
 push dword ptr POINTER
 pop edx
 pop ecx
 mov BX,FileHandle&X
 mov ah,3Fh
 int 21h
endm 
                     
;陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳�
;                   WRITE A BLOCK TO A FILE
; Usage:     BlockWrite  <handel>,<32bit offset>,<bytes to read>
; Example:     BlockWrite   myfile, EDX ,700000
;陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳�
BlockWrite  macro  X , POINTER, BYTE_COUNT
 push dword ptr BYTE_COUNT
 push dword ptr POINTER
 pop edx
 pop ecx
 mov BX,FileHandle&X
 mov ah,40h
 int 21h
endm    
        
;陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳�
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
    
;� STRING 旭旭旭旭旭旭旭旭旭旭旭旭旭旭旭旭旭旭旭旭旭旭旭旭旭旭旭旭旭旭
    
;陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳�
;              DISPLAY A STRING ON SCREEN
;陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳�
; Usage:        Write   <' string to display '>
;陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳�
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
     
;陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳�
;              DISPLAY A STRING ON SCREEN WITH CARAGE RETURN
;陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳�
; Usage:       Writeln   <' string to display '>
;陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳�
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

