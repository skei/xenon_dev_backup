global anim : byte
                     
;° DATA °°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
AnimSpeed dd ?       
FLI_Screen dd ?         ; Pointer to Buffer to use for unpacking FLI
Animation dd ?          ; Pointer to start of module                
NumChunks dd ?                                                      
NumFrames dw ?                                                      
                                                                    
;° CODE °°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
                                                                    
PROC CopyAnimToScreen                                               
 mov esi,[FLI_Screen]             
 add esi,16080                  ; 50*320+80
 mov edi,[Screen]
 mov ecx,100
@@1:                 
 mov ebx,160
@@1_1:
 mov al,[esi]
 inc esi
 mov ah,al
 mov [edi],ax
 mov [edi+320],ax
 add edi,2
 dec ebx
 jnz @@1_1

 add esi,160
 add edi,320         
 dec ecx             
 jnz @@1             
 ret                 
ENDP                 
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
; FrameHeader             
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
; ofs size                                                            
; 0     4       Bytes in this frame. Less then 64K                    
; 4     2       0F1FAh                                                
; 6     2       Number of chunks in this frame                        
; 8     8       Zeros                                                 
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
; ESI -> Start of frame
; 
;PROC HandleFrame
HandleFrame MACRO
 mov ebx,[esi]          ; EBX = FrameSize
 movzx eax,w [esi+6]    ; AX = Number of chunke
 mov [NumChunks],eax   
 add esi,16             ; Skip FrameHeader
 cmp ebx,0              ; If size = 0 then exit
 je FinitoFrame                                
NextChunk:                                
 cmp [NumChunks],0
 je FinitoFrame  
 dec [NumChunks]      
; Chunk Header: Dword - size
;                word - chunk type
 mov ax,[esi+4]         ; Chunk Type
 add esi,6              ; Skip Chunk Header.
 cmp ax,11                                  
 je FLI_COLOR    
 cmp ax,12       
 je FLI_LC       
 cmp ax,13       
 je FLI_BLACK    
 cmp ax,15      
 je FLI_BRUN    
 cmp ax,16              
 je FLI_COPY    
 jmp NextChunk                             
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
FLI_COLOR:    
 mov bx,[esi]           ; number of packets in this chunk (always 1?)
 add esi,2              ; skip the NumberOfPackets           
;Next is the packets
 mov al,0               ; start at color 0                   
 xor ecx,ecx            ; reset CX
Color_Loop:                                      
 or bx,bx               ; set flags
 jz NextChunk           ; exit if no more packages
 dec bx                                           
 mov cl,[esi]           ; first byte in packet tells how many colors to skip
 add al,cl              ; add skiped colors to start to get new start
 mov dx,3C8h            ; PEL Address Write Mode Register
;;;;; out dx,al              ;
 inc dx                 ;
 mov cl,[esi+1]         ; next byte in packet tells how many colors to change
 or cl,cl               ; set the flags                                      
 jnz Jump_Over          ; if NumbersToChange=0 then NumbersToChange=256
 inc ch                 ; CH=1 and CL=0 => CX=256
Jump_Over:                                        
 add al,cl              ; update the color to start at
 lea ecx,[ecx*2+ecx]
 add esi,2              ; skip the NumbersToSkip and NumbersToChange bytes
 rep lodsb;;outsb              ; put the color data to the VGA card
 jmp Color_Loop
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
FLI_LC:                                           
 movzx edi,w [esi]      ; put LinesToSkip into DI
 shl edi,6              ; shl 6 = *64
 lea edi,[edi*4+edi]    ; *64*5 = *320
 add edi,[FLI_Screen]
 mov bx,[esi+2]         ; put LinesToChange into BX
 add esi,4              ; skip the LinesToSkip and LinesToChange words
 xor ecx,ecx            ; reset cx
Line_Loop:                        
 or bx,bx              
 jz NextChunk           ; exit if no more lines to change
 dec bx                                                
 mov dl,[esi]           ; put PacketsInLine into DL    
 inc esi                ; skip the PacketsInLine byte  
 push edi               ; save the offset address of this line
Pack_Loop:                                                    
 or dl,dl                                              
 jz Next_Line           ; exit if no more packets in this line
 dec dl                                                       
 mov cl,[esi]           ; put BytesToSkip into CL      
 add edi,ecx            ; update the offset address    
 mov cl,[esi+1]         ; put BytesOfDataToCome into CL
 or cl,cl                                              
 jns Copy_Bytes         ; no SIGN means that CL number of data is to come
                        ; else next data should be put -CL number of times
 mov al,[esi+2]         ; put the byte to be repeated into AL
 add esi,3              ; skip the packet              
 neg cl                 ; repeat -CL times
 xor ch,ch                           
 rep stosb
 jmp Pack_Loop                                         
Copy_Bytes:                                            
 add esi,2              ; skip the two count bytes at the start of the packet
 rep movsb
 jmp Pack_Loop                                         
Next_Line:                                             
 pop edi                ; restore the old offset address of the current line
 add edi,320            ; offset address to the next line
 jmp Line_Loop                                           
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
FLI_BLACK:                                             
 mov edi,[FLI_screen]                                  
 xor eax,eax                                           
 mov ecx,16000
@@1haha:                ; ****
 mov [edi],eax                                         
 add edi,4                                             
 dec ecx                                               
 jnz @@1haha
 jmp nextchunk                                         
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
FLI_BRUN:     
 mov edi,[FLI_Screen]
 mov ebx,200            ; numbers of lines in a screen 
 xor ecx,ecx                                            
Line_Loop2:                                            
 mov dl,[esi]           ; put PacketsInLine into DL    
 inc esi                ; skip the PacketsInLine byte }
 push edi               ; save the offset address of this line 
Pack_Loop2:                                                    
 or dl,dl               
 jz Next_Line2          ; exit if no more packets in this line
 dec dl                                                       
 mov cl,[esi]           ; put BytesOfDataToCome into CL
 or cl,cl                                   
 js Copy_Bytes2         ; SIGN meens that CL number of data is to come
                        ; else next data should be put -CL number of times
 mov al,[esi+1]         ; put the byte to be repeated into AL
 add esi,2              ; skip the packet
 rep stosb
 jmp Pack_Loop2         ; finish with this packet
Copy_Bytes2:                                     
 inc esi                ; skip the count byte at the start of the packet
 neg cl                 ; repeat -CL times
 rep movsb
 jmp Pack_Loop2         
Next_Line2:                              
 pop edi                ; restore the old offset address of the current line
 add edi,320            ; offset address to the next line
 dec ebx                 ; any more lines to draw?
 jnz Line_Loop2      
 jmp NextChunk       
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
FLI_COPY:            
 mov edi,[FLI_screen]
 mov ecx,16000       
@@5:                 
 mov eax,[esi]       
 add esi,4           
 mov [edi],eax       
 add edi,4           
 dec ecx             
 jnz @@5             
 jmp NextChunk       
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
FinitoFrame:
; ret                                       
;ENDP   
ENDM    
