; TimeOut.. 1996 Excess...

.486p              
.MODEL FLAT;,syscall
.STACK 16384
;JUMPS
LOCALS                                
.CODE                                 
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
include c:\excess\timeout\support.inc  
include c:\excess\timeout\macros.inc        
include c:\excess\timeout\music\music.old
include c:\excess\timeout\3d\3dengine.inc
include c:\excess\timeout\gfx\gfx.inc
include c:\excess\timeout\anim\anim.inc
include c:\excess\timeout\diverse.inc
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
;° DATA °°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
Screen dd ?                           
Buffer dd ?
ColorShades dd ?                                         
gouraud dd ?                                             
BackGround dd ?                       

FramesPassed dd ?                                
Clrshades label byte                             
 include 64x256.db                               
                                      
include c:\excess\timeout\3d\blob32.inc
include c:\excess\timeout\3d\blob2.inc
include c:\excess\timeout\3d\blob6.inc
include c:\excess\timeout\3d\blob9.inc
include c:\excess\timeout\3d\blob11.inc
                                   
;label excess1 byte                   
; include c:\excess\timeout\gfx\excess1.db
                               
label regards byte             
 include c:\excess\timeout\gfx\regards.db
label regardspal byte             
 include c:\excess\timeout\gfx\regpal.db
                                     
                                     
objects label byte                                              
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
obj1 label byte                                                 
 dd 0                   ; 1=active, 0=not active, -1=last object in list
 dd 0                   ; Vertex/face Rotation
 dd 3                   ; Phong/Bump/Plain/PhongTex/FacetTexture shading
 dd 160,100,700         ; Object's center in 3D
 dd 0,0,0               ; Rotation angles
 dd 9 dup(?)            ; Matrix      
 dd 3 dup(?)            ; Constants for 7 imul rotation
 dd 431                 ; Number of Vertexes/Coordinates
 dd 862                 ; Number of Polys/Faces
 dd coords1             ; Pointer to list of coordinates
 dd Polygon1            ; Pointer to polygon data
 dd vertexnormals1      ; Pointer to vertex normals
 dd facenormals1        ; Pointer to face normals
 dd phongshade          ; Pointer to PhongMap
 dd bumcoords1          ; Pointer to bump coordinates
 dd BlobTex             ; Pointer to BumpMap
 dd ?                   ; Pointer to texture coordinates
 dd ?                   ; Pointer to Texture Map
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
obj2 label byte                    
 dd 0                   ; 1=active, 0=not active, -1=last object in list
 dd 0                   ; Vertex/face Rotation
 dd 3                   ; Phong/Bump/Plain/PhongTex/FacetTexture/FlatBump
 dd 160,100,700         ; Object's center in 3D
 dd 0,0,0               ; Rotation angles
 dd 9 dup(?)            ; Matrix
 dd 3 dup(?)            ; Constants for 7 imul rotation
 dd 679                 ; Number of Vertexes/Coordinates
 dd 1354                ; Number of Polys/Faces
 dd coords2             ; Pointer to list of coordinates
 dd Polygon2            ; Pointer to polygon data
 dd vertexnormals2      ; Pointer to vertex normals
 dd facenormals2        ; Pointer to face normals
 dd phongshade          ; Pointer to PhongMap
 dd bumcoords2          ; Pointer to bump coordinates
 dd tex_a               ; Pointer to BumpMap
 dd ?                   ; Pointer to texture coordinates
 dd ?                   ; Pointer to Texture Map
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
obj3 label byte                  
 dd 0                   ; 1=active, 0=not active, -1=last object in list
 dd 0                   ; Vertex/face Rotation
 dd 1                   ; Phong/Bump/Plain/PhongTex/FacetTexture/FlatBump
 dd 160,100,400         ; Object's center in 3D
 dd 0,0,0               ; Rotation angles
 dd 9 dup(?)            ; Matrix    
 dd 3 dup(?)            ; Constants for 7 imul rotation
 dd 475                 ; Number of Vertexes/Coordinates
 dd 946                 ; Number of Polys/Faces
 dd coords3             ; Pointer to list of coordinates
 dd Polygon3            ; Pointer to polygon data
 dd vertexnormals3      ; Pointer to vertex normals
 dd facenormals3        ; Pointer to face normals
 dd phongshade          ; Pointer to PhongMap
 dd bumcoords3          ; Pointer to bump coordinates
 dd bumpmap1            ; Pointer to BumpMap
 dd ?                   ; Pointer to texture coordinates
 dd ?                   ; Pointer to Texture Map
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
obj4 label byte                    
 dd 0                   ; 1=active, 0=not active, -1=last object in list
 dd 0                   ; Vertex/face Rotation
 dd 1                   ; Phong/Bump/Plain/PhongTex/FacetTexture/FlatBump
 dd 160,100,700         ; Object's center in 3D
 dd 0,1024,0            ; Rotation angles
 dd 9 dup(?)            ; Matrix    
 dd 3 dup(?)            ; Constants for 7 imul rotation
 dd 1247                ; Number of Vertexes/Coordinates
 dd 2490                ; Number of Polys/Faces
 dd coords4             ; Pointer to list of coordinates
 dd Polygon4            ; Pointer to polygon data
 dd vertexnormals4      ; Pointer to vertex normals
 dd facenormals4        ; Pointer to face normals
 dd phongshade          ; Pointer to PhongMap
 dd bumcoords4          ; Pointer to bump coordinates
 dd bumpmap1            ; Pointer to BumpMap
 dd ?                   ; Pointer to texture coordinates
 dd ?                   ; Pointer to Texture Map
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
obj5 label byte   
 dd 0                   ; 1=active, 0=not active, -1=last object in list
 dd 0                   ; Vertex/face Rotation
 dd 1                   ; Phong/Bump/Plain/PhongTex/FacetTexture/FlatBump
 dd 160,100,800         ; Object's center in 3D
 dd 0,1024,0            ; Rotation angles
 dd 9 dup(?)            ; Matrix    
 dd 3 dup(?)            ; Constants for 7 imul rotation
 dd 1125                ; Number of Vertexes/Coordinates
 dd 2246                ; Number of Polys/Faces
 dd coords5             ; Pointer to list of coordinates
 dd Polygon5            ; Pointer to polygon data
 dd vertexnormals5      ; Pointer to vertex normals
 dd facenormals5        ; Pointer to face normals
 dd mixedbuf            ;phongshade          ; Pointer to PhongMap
 dd bumcoords5          ; Pointer to bump coordinates
 dd bumpmap1            ; Pointer to BumpMap
 dd ?                   ; Pointer to texture coordinates
 dd ?                   ; Pointer to Texture Map
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
                        
 dd -1                            
                        
Whitepalette db 768 dup(63)          
blankpalette db 768 dup(0)  
                        
Label AnimPal byte                 
 include animpal.db     
                        
RXindex db 0
RYindex db 0
GXindex db 0
GYindex db 0   
BXindex db 0      
BYindex db 0      
                   
rgb label byte 
 include rgb1.db
               
Label LightPath byte
 include lightpat.db 
               
MixedBuf label byte
 db 65536 dup(0)
             
fadeteller db ?
ristepeker dw ?     
risteliste      db 40 dup(2)
                db 64 dup(3)
                db 64 dup(4)
                db 64 dup(3) 
                db 40 dup(2)
ristelistesize = 272
risteliste2     db 10 dup(2)
                db 16 dup(3)
                db 16 dup(4)
                db 16 dup(3)
                db 10 dup(2)
ristelistesize2 = 68

label sinus3d word
 include sin3d.db  
label cosinus3d word
 include cos3d.db
                    
WobTexture dd ?                      
WobSinus label byte         
 include wobsin.db    
WobCosinus label byte 
 include wobcos.db    
InnerAdder dw ?         ; Global
OuterAdder dw ?         ; Global
Initialadd dw ?         ; Global
Initial dw ?        
                  
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
;° CODE °°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
                  
;°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
                    
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



PROC MultipleLight1 
 mov esi,offset palette1
 mov al,0          
 mov ecx,768           
 Call SetPalette       
; mov esi,offset excess1
; mov edi,[screen]  
; mov ecx,64000     
; rep movsd         
 mov [rxindex],64  
 mov [ryindex],64  
 mov [gxindex],64  
 mov [gyindex],64  
 mov [bxindex],64    
 mov [byindex],64  
                   
 mov [obj5.status],1
 mov [obj5.shadetype],1
 mov [obj5.phongmap],offset mixedbuf
 mov [obj5.xpos],160
 mov [obj5.ypos],100
 mov [obj5.zpos],600
@@1:                       
 Call MixBuffers    
 call clearbuffer   
; call OwnClearBuffer
 mov ebp,offset objects
 Call CalcAllMatrixes
 mov ebp,offset objects
 Call DrawAllObjects
 Call waitvr        
 Call CopyBuffer    
                                                                   
 mov ebp,offset obj5 
 add [ebp.xangle],60
 and [ebp.xangle],4095
 add [ebp.yangle],40  
 and [ebp.yangle],4095
 add [ebp.zangle],18  
 and [ebp.zangle],4095
                      
 xor eax,eax        
 mov al,[rxindex]   
 mov bl,[lightpath+eax]
 mov b [rxorigo],bl 
                       
 mov al,[ryindex]      
 mov bl,[lightpath+eax]
 mov b [ryorigo],bl     
                      
 mov al,[gxindex]     
 mov bl,[lightpath+eax]
 mov b [gxorigo],bl      
                       
 mov al,[gyindex]      
 mov bl,[lightpath+eax]
 mov b [gyorigo],bl     
                      
 mov al,[bxindex]     
 mov bl,[lightpath+eax]
 mov b [bxorigo],bl     
                      
 mov al,[byindex]     
 mov bl,[lightpath+eax]
 mov b [byorigo],bl     

 mov eax,[nms_modulepos]
 shr eax,16           
 cmp ax,38            
 jb @@1               
 ret                  
ENDP                  
                     
PROC MultipleLight2  
; cld              
; mov dx,3c8h      
; xor al,al        
; out dx,al        
; inc dx            
; mov cx,768       
; mov al,63        
;@@pp1:            
; out dx,al        
; dec cx           
; jnz @@pp1        
; mov esi,offset palette1
; mov al,0         
; mov ecx,768      
; Call SetPalette  
;                  
 mov [framespassed],0
@@1:               
 Call MixBuffers     
 call clearbuffer   
; call OwnClearBuffer
 mov ebp,offset objects
 Call CalcAllMatrixes
 mov ebp,offset objects
 Call DrawAllObjects
 Call waitvr        
 Call CopyBuffer   
                                                                   
 mov ebp,offset obj5
 add [ebp.xangle],60
 and [ebp.xangle],4095
 add [ebp.yangle],40
 and [ebp.yangle],4095
 add [ebp.zangle],18
 and [ebp.zangle],4095
                    
 xor eax,eax        
 add [rxindex],3
 mov al,[rxindex]   
 mov bl,[lightpath+eax]
 mov b [rxorigo],bl 
                       
 add [ryindex],4
 mov al,[ryindex]      
 mov bl,[lightpath+eax]
 mov b [ryorigo],bl     
                      
 add [gxindex],4
 mov al,[gxindex]     
 mov bl,[lightpath+eax]
 mov b [gxorigo],bl      
                       
 add [gyindex],6
 mov al,[gyindex]      
 mov bl,[lightpath+eax]
 mov b [gyorigo],bl     
                      
 add [bxindex],4
 mov al,[bxindex]     
 mov bl,[lightpath+eax]
 mov b [bxorigo],bl     
                      
 add [byindex],2      
 mov al,[byindex]     
 mov bl,[lightpath+eax]
 mov b [byorigo],bl     
                   
 mov eax,[nms_modulepos]
 shr eax,16        
 cmp ax,42
 jb @@1            
 ret               
ENDP               
                    
PROC MixBuffers     
 mov ecx,07f7fh         ; 127:127       Y:X
@@1:                
 mov cl,127         
@@2:                
 xor eax,eax        
 mov edx,ecx        
 add dx,1234h   ; 34 12
rXorigo = $-2     
rYorigo = $-1     
 mov al,[phongshade+edx]
 shr al,1           
 shl eax,5          
 mov edx,ecx        
 add dx,1234h       
gXorigo = $-2       
gYorigo = $-1       
 mov bl,[phongshade+edx]       
 shr bl,1           
 add al,bl          
 shl eax,5          
 mov edx,ecx          
 add dx,1234h       
bXorigo = $-2       
bYorigo = $-1       
 mov bl,[phongshade+edx]       
 shr bl,1           
 add al,bl          
 mov bl,[rgb+eax]
 mov [MixedBuf+ecx],bl
 dec cl             
 jnz @@2            
 dec ch             
 jnz @@1            
 ret                
ENDP                
                    
PROC TimerHandler near  
 inc [FramesPassed]     
 ret                  
ENDP                
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
PROC GetFrameNum   
 mov eax,[FramesPassed]
 mov [FramesPassed],0
 ret                 
ENDP                

;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

; ESI -> textbuffer
; EDI -> screen/Background buffer
; CL -> Color      
_width dd ? 
_height dd ?         
dowritershit MACRO   
 mov eax,[nms_modulepos];[CurrRow]
 cmp ax,62
 jb skipfadeup                   
 dec [credcolor]                 
 cmp [credcolor],0               
 jge fadefinito                  
 mov [credcolor],0               
 jmp fadefinito                  
skipfadeup:                      
 cmp ax,32                       
 jb skipfadedown                 
 inc [credcolor]                 
 cmp [credcolor],31              
 jbe skipfadedown                
 mov [credcolor],31              
fadefinito:                      
skipfadedown:                    
                                 
; cmp [credcolor],0              
; jle ikketegn                   
                                 
 mov esi,(offset cred_gfx) + (320*10)
 mov edi,[buffer]                
 add edi,(293*960)+320           
 mov eax,320                     
 mov [_width],320                
 mov [_Height],14                
 mov cl,[credcolor]              
 cmp cl, 0                       
 jle ikketegn                    
@@0:                             
; push esi                       
 push edi                        
@@1:                             
 mov dl,[esi]                    
 inc esi                         
 cmp dl,0                        
 je @@2                          
 mov byte ptr [edi],cl           
@@2:                             
 inc edi                         
 dec eax                         
 jnz @@1                         
;                                
 pop edi                         
; pop esi                        
 mov eax,[_width]                
                                 
; add esi,320                    
 add edi,960                     
 dec [_height]                   
 jnz @@0                         
ikketegn:                        
ENDM                             
                                 
credcolor db ?                   
PROC Credits                     
 mov [credcolor],0               
;ÄÄÄÄÄÄÄÄ                        
 mov esi,offset palette1         
 mov al,0                        
 mov ecx,768                     
 Call Setpalette                 
                                 
 mov eax,offset clrshades        
 mov [Colorshades],eax           
 mov [xlightorigo],192
 mov [ylightorigo],64            
;ÄÄÄÄÄÄÄÄ                        
 mov [FramesPassed],0            
; mov eax,offset tex_1           
; mov [Background],eax           
 mov [obj1.status],1             
 mov [obj1.xpos],160             
 mov [obj1.ypos],100             
 mov [obj1.zpos],700             
; mov eax,[gouraud]              
; mov [obj1.phongmap],eax        
@@3:                             
 Call ClearBuffer                
 mov ebp,offset objects          
 Call CalcAllMatrixes            
 mov ebp,offset objects          
 Call DrawAllObjects             
                                 
 dowritershit                    
                                 
 Call WaitVR                     
 Call CopyBuffer                 
                                 
 Call GetFrameNum                
 mov ebp,offset obj1             
 mov ecx,5                       
 imul ecx,eax                    
 add [ebp.xangle],ecx            
 and [ebp.xangle],4095           
 mov ecx,4                       
 imul ecx,eax                    
 add [ebp.yangle],ecx            
 and [ebp.yangle],4095           
 mov ecx,3                       
 imul ecx,eax                    
 add [ebp.zangle],ecx            
 and [ebp.zangle],4095           
                                 
 add [Xlightorigo],5             
 add [ylightorigo],0             
                                 
 mov eax,[nms_modulepos]
 shr eax,16 
 cmp ax,1
 jb @@3                          
 mov [obj1.status],0             
 ret                             
ENDP                    

;°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°

PROC DoAnim      
 mov [Animation],offset anim
 mov edi,[Buffer];[screen]
 mov [FLI_Screen],edi
 mov esi,[Animation] 
 mov ax,[esi+6]      
 mov [NumFrames],ax
 add esi,128                
@@1:              
 push esi         
;;;; int 3 
; Call HandleFrame
 handleframe
 pop edi            
 sub edi,esi        
 and edi,1          
 add esi,edi        
; Wait for right timer-pos...
@@2:                
 mov eax,[FramesPassed]
 cmp eax,[AnimSpeed]
 jb @@2             
 Mov [FramesPassed],0
 push esi  
; push edi 
 Call WaitVR
 Call CopyAnimToScreen
; pop edi                   
 pop esi            
 dec [NumFrames]    
 jnz @@1            
 ret                         
ENDP                         

;°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°

;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
PROC AnimPart
 mov edi,[Screen]
 mov ecx,16000 
 xor eax,eax 
 rep stosd   
 mov esi,offset Animpal
 xor al,al           
 mov ecx,768         
 Call SetPalette     
                
 mov [AnimSpeed],4          
 Call DoAnim    
             
 mov esi,offset whitepalette
 xor al,al                  
 mov ecx,768                
 Call SetPalette            
 mov esi,offset timeoutgfx  
 mov edi,[Screen]           
 mov ecx,64000              
 Call CopyBlock             
                
 mov ebx,64     
@@1:            
 push ebx       
 mov esi,offset whitepalette
 mov edi,offset timeoutpal
 xor al,al      
 mov ecx,768    
 Call FadePalette
 Call waitvr                
 mov esi,offset whitepalette
 xor al,al      
 mov ecx,768    
 call setpalette
 pop ebx        
 dec ebx          
 jnz @@1          
 ret              
ENDP              
                  
;°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
                  
PROC Shake        
; Shake/fade      
 mov ebx,64               
@@2:              
 push ebx         
 mov esi,offset timeoutpal
 mov edi,offset blankpalette
 xor al,al                  
 mov ecx,768                
 Call FadePalette           
 Call waitvr                
 mov esi,offset timeoutpal
 xor al,al                  
 mov ecx,768                
 call setpalette            
                            
 mov cl,3    
 Call random            ; ret: EAX random (bits = bl)
 shl eax,6                
 lea esi,[eax*4+eax]
 mov cl,3    
 call random 
 add esi,eax              
 add esi,offset timeoutgfx
 mov edi,[Screen]
 mov ecx,64000
 Call CopyBlock             
                
 pop ebx                  
 dec ebx                  
 jnz @@2                  
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
; Text - fade opp - rist - fade ned
; Shake/fade          
 mov [fadeteller],4
 mov ebx,256 
@@3:                        
 push ebx                   
 dec [fadeteller]
 jnz @@321321
 mov [fadeteller],4
 mov esi,offset timeoutpal
 mov edi,offset textpal
 xor al,al                  
 mov ecx,768                
 Call FadePalette           
 Call waitvr                
 mov esi,offset timeoutpal
 xor al,al                  
 mov ecx,768                                                 
 call setpalette            
@@321321:                    
 mov cl,1          
 Call random            ; ret: EAX random (bits = bl)
 shl eax,6             
 lea esi,[eax*4+eax]   
 mov cl,1          
 call random           
 add esi,eax                
 add esi,offset theythought
                           
 mov edi,[Screen]           
 mov ecx,64000              
 Call CopyBlock            
            
 pop ebx                   
 dec ebx                    
 jnz @@3               
;ÄÄÄÄÄÄÄÄ              
 mov [ristepeker],0
@@123:            
 Call waitvr                
 xor ecx,ecx
 mov cx,[ristepeker]  
 mov cl,[risteliste+ecx]
 Call random            ; ret: EAX random (bits = bl)
 shl eax,6             
 lea esi,[eax*4+eax]   
 xor ecx,ecx           
 mov cx,[ristepeker]
 mov cl,[risteliste+ecx]
 call random           
 add esi,eax                
 add esi,offset theythought
 mov edi,[Screen]           
 mov ecx,64000
 Call CopyBlock             
 inc [ristepeker]
 cmp [ristepeker],ristelistesize
 jb @@123   
;ÄÄÄÄÄÄÄÄ         
 mov edi,offset blankpalette
 mov ecx,(768/4)       
 xor eax,eax           
 rep stosd             
;ÄÄÄÄÄÄÄÄ              
 mov [fadeteller],4
 mov ebx,256           
@@4:                   
 push ebx              
 dec [fadeteller]      
 jnz @@3213212         
 mov [fadeteller],4
 mov esi,offset textpal
 mov edi,offset blankpalette
 xor al,al                  
 mov ecx,768                
 Call FadePalette           
 Call waitvr                
 mov esi,offset textpal
 xor al,al                  
 mov ecx,768                                                 
 call setpalette            
@@3213212:                            
 mov cl,1          
 Call random            ; ret: EAX random (bits = bl)
 shl eax,6             
 lea esi,[eax*4+eax]   
 mov cl,1         
 call random           
 add esi,eax           
 add esi,offset theythought
                           
 mov edi,[Screen]          
 mov ecx,64000             
 Call CopyBlock            
                            
 pop ebx                   
 dec ebx                   
 jnz @@4          
                  
 mov edi,[Screen]           
 mov ecx,16000              
 xor eax,eax                
 rep stosd                  
                            
 ret                        
ENDP                        
xposindex db ?              
yposindex db ?              
fadevar dd ?                
                            
PROC Yeah_shake_it          
 mov ebx,64               
@@2:                   
 push ebx              
 mov esi,offset regardspal
 mov edi,offset blankpalette
 xor al,al                  
 mov ecx,768                
 Call FadePalette           
 Call waitvr                
 mov esi,offset regardspal
 xor al,al                  
 mov ecx,768                
 call setpalette       
                            
 mov cl,3                 
 Call random            ; ret: EAX random (bits = bl)
 shl eax,6                
 lea esi,[eax*4+eax]      
 mov cl,3                 
 call random              
 add esi,eax              
 add esi,(offset regards)+(28*320)
 mov edi,[Screen] 
 mov ecx,64000         
 Call CopyBlock             
                       
 pop ebx               
 dec ebx               
 jnz @@2                  
                        
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
; Text - fade opp - rist - fade ned
; Shake/fade                
 mov [fadeteller],4         
 mov ebx,256                
@@3:                        
 push ebx                   
 dec [fadeteller]           
 jnz @@321321               
 mov [fadeteller],4         
 mov esi,offset regardspal
 mov edi,offset textpal2
 xor al,al                  
 mov ecx,768                
 Call FadePalette           
 Call waitvr                
 mov esi,offset regardspal
 xor al,al                  
 mov ecx,768                                                 
 call setpalette            
@@321321:                    
 mov cl,1                   
 Call random            ; ret: EAX random (bits = bl)
 shl eax,6                  
 lea esi,[eax*4+eax]        
 mov cl,1                   
 call random                
 add esi,eax                
 add esi,offset wewere 
                            
 mov edi,[Screen]           
 mov ecx,64000              
 Call CopyBlock             
                            
 pop ebx                    
 dec ebx                    
 jnz @@3                    
;ÄÄÄÄÄÄÄÄ rist:
 mov [ristepeker],0
@@123:            
 Call waitvr                
 xor ecx,ecx                
 mov cx,[ristepeker]        
 mov cl,[risteliste2+ecx]    
 Call random            ; ret: EAX random (bits = bl)
 shl eax,6                  
 lea esi,[eax*4+eax]        
 xor ecx,ecx                
 mov cx,[ristepeker]        
 mov cl,[risteliste2+ecx]    
 call random                
 add esi,eax                
 add esi,offset wewere
 mov edi,[Screen]           
 mov ecx,64000              
 Call CopyBlock             
 inc [ristepeker]               
 cmp [ristepeker],ristelistesize2
 jb @@123                       
;ÄÄÄÄÄÄÄÄ             
 mov edi,offset blankpalette
 mov ecx,(768/4)       
 xor eax,eax           
 rep stosd             
;ÄÄÄÄÄÄÄÄ ned:
 mov [fadeteller],4;
 mov ebx,256           
@@4:                   
 push ebx              
 dec [fadeteller]      
 jnz @@3213212         
 mov [fadeteller],4;   
 mov esi,offset textpal2
 mov edi,offset blankpalette
 xor al,al                  
 mov ecx,768                
 Call FadePalette           
 Call waitvr                
 mov esi,offset textpal2
 xor al,al                  
 mov ecx,768                                                 
 call setpalette            
@@3213212:                            
 mov cl,4               
 Call random            ; ret: EAX random (bits = bl)
 shl eax,6              
 lea esi,[eax*4+eax]    
 mov cl,4               
 call random            
 add esi,eax            
 add esi,offset wewere  
                           
 mov edi,[Screen]          
 mov ecx,64000             
 Call CopyBlock            
                            
 pop ebx                   
 dec ebx                   
 jnz @@4                
                        
 mov edi,[Screen]           
 mov ecx,16000              
 xor eax,eax                
 rep stosd              
@@999:                  
 mov eax,[nms_modulepos]
 shr eax,16       
 cmp ax,34
 jb @@999               
 ret                    
ENDP                    
                        
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
                        
PROC A_Part             
@@1:                        
 mov eax,[nms_modulepos]
 shr eax,16             
 cmp ax,9               
 jb @@1                 
 mov [fadevar],64       
 mov edi,offset whitepalette
 mov ecx,(768/4)            
 mov eax,1f1f1f1fh          
 rep stosd                  
                            
 mov esi,offset whitepalette
 xor al,al                  
 mov ecx,768                
 call setpalette            
                    
 mov eax,offset clrshades        
 mov [Colorshades],eax           
 mov [xlightorigo],64   
 mov [ylightorigo],64            
 mov [FramesPassed],0       
 mov eax,offset background1
 mov [Background],eax
 mov [obj2.status],1 
 mov [obj2.zpos],500
 mov [xposindex],1 
 mov [yposindex],1 
; mov [obj2.ypos],100 
; mov [obj2.zpos],350
 mov eax,[gouraud] 
 mov [obj2.phongmap],eax
@@3:                         
; Call ClearBuffer            
 Call CopyBack1
 mov ebp,offset objects      
 Call CalcAllMatrixes        
 mov ebp,offset objects      
 Call DrawAllObjects         
 Call WaitVR                 
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
 cmp [fadevar],0            
 je @@ert     
 dec [fadevar]      
 jz @@ert
 mov esi,offset whitepalette
 mov edi,offset palette1
 xor al,al                  
 mov ecx,768                
 Call FadePalette           
 mov esi,offset whitepalette
 xor al,al                  
 mov ecx,768                                                 
 call setpalette            
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
@@ert:                      
 Call CopyBuffer             
 Call GetFrameNum            
 mov ebp,offset obj2        
 mov ecx,10   
 imul ecx,eax               
 add [ebp.xangle],ecx       
 and [ebp.xangle],4095      
 mov ecx,7    
 imul ecx,eax           
 add [ebp.yangle],ecx   
 and [ebp.yangle],4095
 mov ecx,8
 imul ecx,eax      
 add [ebp.zangle],ecx
 and [ebp.zangle],4095
                     
; shr eax,2        
                   
 shr al,1  
 mov ebp,offset obj2
           
 xor ecx,ecx
 mov cl,[xposindex]
 add [XposIndex],al
 xor edx,edx
 mov dx,[sinus3d+ecx*2]     
 mov [ebp.xpos],edx
                   
 xor ecx,ecx            
 mov cl,[yposindex]     
 add [yposIndex],al
 xor edx,edx       
 mov dx,[cosinus3d+ecx*2]
 mov [ebp.ypos],edx
                     
 mov eax,[nms_modulepos]
 shr eax,16          
 cmp ax,13 
 jb @@3             
 mov [obj2.status],0
 ret                 
ENDP                 
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
PROC DrawTwirl
 mov dx,[RotZoom]    
 mov esi,offset twirlmap    
 mov edi,[Screen]
 mov ecx,100             
Outer1:                                      
 rept 160        
  xor ebx,ebx    
  mov bx,[esi]                  ;; AX = GetPixel-offset in texture
  add esi,2        
  add bx,dx                     ;; Add X-offset/Rotation
  add ebx,[twirltexture]
  mov al,[ebx]     
  mov ah,al         
  mov [edi],ax   
  mov [edi+320],ax
  add edi,2       
 endm               
 add edi,320      
 dec ecx            
 jnz Outer1          
 ret                
ENDP                        
twirltexture dd ? 
RotZoom dw ?     
                 
Proc DoTwirl       
 mov esi,offset twirlpal
 xor eax,eax     
 mov ecx,768     
 Call SetPalette 
 mov [twirltexture],offset twirltex
 mov [rotzoom],0000h
@@1:                
 Call WaitVR     
 Call DrawTwirl     
 add [rotzoom],0fe03h
 mov eax,[nms_modulepos]
 shr eax,16 
 cmp ax,15  
 jb @@1     
; Set palette to white
 mov dx,3c8h          
 xor al,al  
 out dx,al       
 inc dx            
 mov cx,768 
 mov al,63  
@@qwe1:     
 out dx,al  
 dec cx     
 jnz @@qwe1 
 mov ecx,5  
 Call WaitECX          
 mov esi,offset twirlpal;2              #2 r›daktig...
 xor eax,eax                                          
 mov ecx,768     
 Call SetPalette 
@@11:  
 Call WaitVR     
 Call DrawTwirl     
 add [rotzoom],0fe03h
 mov eax,[nms_modulepos]                              
 mov ecx,eax                                          
 shr eax,16                                           
 cmp ax,16                                            
 jb @@11                                              
 mov eax,ecx                                          
 cmp ax,32                                            
 jb @@11                                              
@@2:                                                  
 Call WaitVR                                          
 mov esi,offset twirlpal                              
 mov edi,offset blankpalette                          
 xor al,al                                            
 mov ecx,768                                          
 Call FadePalette
 mov esi,offset twirlpal
 xor eax,eax
 mov ecx,768
 Call SetPalette
 Call DrawTwirl     
 add [rotzoom],0fe03h
 mov eax,[nms_modulepos]
 shr eax,16    
 cmp ax,17     
 jb @@2        
 ret           
ENDP                         
               
PROC DoKalkun1
 cld                    
 mov edi,[Screen]
 mov ecx,16000
 xor eax,eax  
 rep stosd    
 mov esi,offset palette1
 xor al,al    
 mov ecx,768  
 call setpalette
                    
 mov eax,offset clrshades        
 mov [Colorshades],eax           
 mov [xlightorigo],64   
 mov [ylightorigo],64            
 mov [FramesPassed],0   
 mov eax,offset background1             ; ??
 mov [Background],eax                   ; ??
 mov [obj3.status],1                     
 mov [obj3.xpos],160
 mov [obj3.ypos],100            
 mov [obj3.zpos],380
 mov [obj3.phongmap],offset pmap
; mov eax,[gouraud]             
; mov [obj2.phongmap],eax       
@@3:                            
; Call ClearBuffer              
 Call CopyBack1                 
 mov ebp,offset objects         
 Call CalcAllMatrixes           
 mov ebp,offset objects         
 Call DrawAllObjects         
 Call WaitVR                 
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
 Call CopyBuffer             
 Call GetFrameNum            
 mov ebp,offset obj3
 mov ecx,10         
 imul ecx,eax               
 add [ebp.xangle],ecx       
 and [ebp.xangle],4095      
 mov ecx,9    
 imul ecx,eax        
 add [ebp.yangle],ecx
 and [ebp.yangle],4095
 mov ecx,8          
 imul ecx,eax       
 add [ebp.zangle],ecx
 and [ebp.zangle],4095
                     
 add [ebp.zpos],1
 mov eax,[nms_modulepos]
 shr eax,16          
 cmp ax,21       
 jb @@3             
 mov [obj3.status],0
 ret                 
ENDP                 
                 
PROC KalkunRowHandler1             
 mov eax,[nms_modulepos]           
 and ax,7                          
 jnz @@1              
 neg [angleadder]     
@@1:                  
 ret                  
ENDP                  
                                           
angleadder dd ?  
mappingtype dd ?
                                           
PROC DoKalkun2                             
 mov [obj3.status],1                       
 mov [angleadder],30                       
 mov [nms_jmp_row],offset KalkunRowHandler1
@@3:                                       
 Call CopyBack1                            
 mov ebp,offset objects                    
 Call CalcAllMatrixes                      
 mov ebp,offset objects                    
 Call DrawAllObjects                       
 Call WaitVR                               
 Call CopyBuffer                           
 Call GetFrameNum                          
 mov ebp,offset obj3                       
 mov ecx,22                                
 imul ecx,eax                              
 add [ebp.xangle],ecx                      
 and [ebp.xangle],4095                     
 mov ecx,[angleadder]            ; ???     
 imul ecx,eax                              
 add [ebp.yangle],ecx                      
 and [ebp.yangle],4095                     
 mov ecx,15                                
 imul ecx,eax                              
 add [ebp.zangle],ecx                      
 and [ebp.zangle],4095                     
                                           
 mov eax,[nms_modulepos]                   
 shr eax,16                                
 cmp ax,23                                 
 jb @@3                                    
 mov [nms_jmp_row],offset nms_dummy
 mov [obj3.status],0                       
 ret                                       
ENDP                                       
                                           
PROC KalkunRowHandler2
 mov eax,[nms_modulepos]           
 and ax,7                          
 jnz @@1                                   
 neg [angleadder]
@@1:       
 mov eax,[nms_modulepos]           
 and ax,15 
 jnz @@2   
 neg [mappingtype]
@@2:       
           
 ret                                       
ENDP                                       
                                           
PROC DoKalkun3                             
 mov [obj3.status],1                       
 mov [angleadder],30
 mov [mappingtype],1
 mov [nms_jmp_row],offset KalkunRowHandler2
@@3:                                       
 Call CopyBack1                            
 mov ebp,offset objects                    
 Call CalcAllMatrixes
 mov [obj3.shadetype],0
 cmp [mappingtype],1
 jne @@litt
 mov [obj3.shadetype],1
@@litt:  
 mov ebp,offset objects
 Call DrawAllObjects
 Call WaitVR                               
 Call CopyBuffer                           
 Call GetFrameNum                          
 mov ebp,offset obj3                       
 mov ecx,22                                
 imul ecx,eax                              
 add [ebp.xangle],ecx                      
 and [ebp.xangle],4095                     
 mov ecx,[angleadder]            ; ???     
 imul ecx,eax                              
 add [ebp.yangle],ecx                      
 and [ebp.yangle],4095                     
 mov ecx,15                                
 imul ecx,eax                              
 add [ebp.zangle],ecx                      
 and [ebp.zangle],4095                     
                                           
 mov eax,[nms_modulepos]                   
 shr eax,16                                
 cmp ax,25
 jb @@3                                    
 mov [nms_jmp_row],offset nms_dummy
 mov [obj3.status],0                       
 ret                                       
ENDP                                       
                      
PROC DoKalkun4        
 mov [obj3.shadetype],0
 mov [obj3.status],1                     
@@3:                            
 Call CopyBack1                 
 mov ebp,offset objects         
 Call CalcAllMatrixes           
 mov ebp,offset objects         
 Call DrawAllObjects         
 Call WaitVR                 
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
 Call CopyBuffer             
 Call GetFrameNum            
 mov ebp,offset obj3  
 mov ecx,22   
 imul ecx,eax               
 add [ebp.xangle],ecx       
 and [ebp.xangle],4095      
 mov ecx,30           
 imul ecx,eax         
 add [ebp.yangle],ecx 
 and [ebp.yangle],4095
 mov ecx,15           
 imul ecx,eax         
 add [ebp.zangle],ecx 
 and [ebp.zangle],4095
                      
 add [ebp.xpos],2
 mov eax,[nms_modulepos]
 shr eax,16          
 cmp ax,27
 jb @@3             
 mov [obj3.status],0
 ret                 
ENDP                 
                 
PROC Init320x256 
mov dx, 03D4h    
mov al, 011h     
out dx,al        
mov dx, 03D5h    
in al,dx         
and al, 07Fh     
out dx,al        
mov dx, 03C2h    
mov al, 0E3h     
out dx,al        
mov dx, 03D4h    
mov al, 00h
out dx,al 
mov dx, 03D5h 
mov al, 05Fh
out dx,al
mov dx, 03D4h
mov al, 01h
out dx,al 
mov dx, 03D5h
mov al, 04Fh
out dx,al
mov dx, 03D4h 
mov al, 02h      
out dx,al 
mov dx, 03D5h
mov al, 050h     
out dx,al
mov dx, 03D4h
mov al, 03h
out dx,al 
mov dx, 03D5h
mov al, 082h
out dx,al     
mov dx, 03D4h
mov al, 04h
out dx,al 
mov dx, 03D5h
mov al, 054h 
out dx,al
mov dx, 03D4h
mov al, 05h
out dx,al     
mov dx, 03D5h
mov al, 080h
out dx,al
mov dx, 03D4h
mov al, 06h
out dx,al 
mov dx, 03D5h
mov al, 020h
out dx,al
mov dx, 03D4h
mov al, 07h   
out dx,al 
mov dx, 03D5h
mov al, 03Eh
out dx,al
mov dx, 03D4h
mov al, 08h
out dx,al 
mov dx, 03D5h
mov al, 00h   
out dx,al
mov dx, 03D4h
mov al, 09h
out dx,al 
mov dx, 03D5h
mov al, 041h
out dx,al
mov dx, 03D4h
mov al, 010h
out dx,al 
mov dx, 03D5h 
mov al, 0FFh
out dx,al
mov dx, 03D4h
mov al, 011h
out dx,al    
mov dx, 03D5h
mov al, 00h
out dx,al
mov dx, 03D4h
mov al, 012h
out dx,al 
mov dx, 03D5h
mov al, 0FEh
out dx,al
mov dx, 03D4h
mov al, 013h
out dx,al 
mov dx, 03D5h
mov al, 028h
out dx,al     
mov dx, 03D4h
mov al, 014h
out dx,al 
mov dx, 03D5h
mov al, 00h  
out dx,al
mov dx, 03D4h
mov al, 015h
out dx,al 
mov dx, 03D5h
mov al, 0FFh
out dx,al
mov dx, 03D4h
mov al, 016h
out dx,al 
mov dx, 03D5h
mov al, 010h
out dx,al
mov dx, 03D4h
mov al, 017h  
out dx,al 
mov dx, 03D5h
mov al, 0E3h
out dx,al
mov dx, 03C4h
mov al, 01h
out dx,al 
mov dx, 03C5h
mov al, 01h
out dx,al
mov dx, 03C4h
mov al, 03h
out dx,al 
mov dx, 03C5h
mov al, 00h
out dx,al
mov dx, 03C4h
mov al, 04h
out dx,al 
mov dx, 03C5h   
mov al, 06h
out dx,al
mov dx, 03CEh
mov al, 05h
out dx,al    
mov dx, 03CFh
mov al, 040h
out dx,al
mov dx, 03CEh
mov al, 06h
out dx,al 
mov dx, 03CFh   
mov al, 05h
out dx,al
ret
ENDP    
           
SetPlane MACRO plane
 mov dx,03c4h
 mov al,2          
 mov ah,plane      
 out dx,ax
ENDM      
          
PROC ShowPic1
 Call Init320x256 
; set 512 rows
 mov dx,3d4h
 mov al,9       
 out dx,al      
 inc dx         
 in al,dx       
 and al,0e0h  
 or al,0                  
 out dx,al    
              
 mov esi,offset gfxpalette
 xor eax,eax      
 mov ecx,768      
 Call SetPalette  
                  
 cld              
 SetPlane 1  
 mov esi,offset gfxP1
 mov edi,[Screen]  
 mov ecx,40960     
@@1:               
 mov al,[esi]      
 inc esi           
 mov [edi],al      
 inc edi           
 dec ecx           
 jnz @@1           
 SetPlane 2               
 mov esi,offset gfxP2 
 mov edi,[Screen]  
 mov ecx,40960     
@@2:               
 mov al,[esi]      
 inc esi           
 mov [edi],al      
 inc edi           
 dec ecx           
 jnz @@2           
 SetPlane 4        
 mov esi,offset gfxP3 
 mov edi,[Screen]  
 mov ecx,40960     
@@3:               
 mov al,[esi]      
 inc esi           
 mov [edi],al      
 inc edi           
 dec ecx                  
 jnz @@3           
 SetPlane 8  
 mov esi,offset gfxP4 
 mov edi,[Screen]  
 mov ecx,40960     
@@4:               
 mov al,[esi]      
 inc esi           
 mov [edi],al      
 inc edi           
 dec ecx              
 jnz @@4           
;ÄÄÄÄÄÄÄÄ                       
 mov edi,offset blankpalette
 mov ecx,(768/4)       
 xor eax,eax           
 rep stosd                      
;ÄÄÄÄÄÄÄÄ                       
@@vent1:                        
 mov eax,[nms_modulepos]        
 shr eax,16                     
 cmp ax,29                      
 jb @@vent1                     
;ÄÄÄÄÄÄÄÄ                       
 mov ax,13h                     
 int 10h                        
 ret                            
ENDP                            
                                
;PROC DoKalkun5                  
; mov esi,offset palette1        
; xor al,al                      
; mov ecx,768                    
; call setpalette                
;                                
; mov [obj5.shadetype],1
; mov [obj5.phongmap],offset pmap
; mov [obj5.tex2map],offset bumpmap1
; mov [obj5.status],1
; mov [obj5.ypos],-100
;@@3:                            
; Call CopyBack1                 
; mov ebp,offset objects         
; Call CalcAllMatrixes           
; mov ebp,offset objects         
; Call DrawAllObjects            
; Call WaitVR                    
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
; Call CopyBuffer                
; Call GetFrameNum               
; mov ebp,offset obj5
; mov ecx,10                     
; imul ecx,eax                   
; add [ebp.xangle],ecx           
; and [ebp.xangle],4095          
; mov ecx,8                      
; imul ecx,eax                   
; add [ebp.yangle],ecx           
; and [ebp.yangle],4095          
; mov ecx,13                     
; imul ecx,eax         
; add [ebp.zangle],ecx 
; and [ebp.zangle],4095
;                      
; cmp [ebp.ypos],100 
; jnl @@321          
; add [ebp.ypos],1   
;@@321:              
; mov eax,[nms_modulepos]
; shr eax,16          
; cmp ax,33          
; jb @@3             
; mov [obj5.status],0
; ret                 
;ENDP                 
                    

PROC Rist_regardar
 mov esi,offset regardspal
 xor eax,eax      
 mov ecx,768      
 Call SetPalette  
 mov esi,(offset regards) + (28*320)
 mov edi,[screen]                   
 mov ecx,16000                      
 rep movsd        
@@3:
 mov eax,[nms_modulepos]
 shr eax,16        
 cmp ax,33        
 jb @@3           
 ret              
ENDP               
                   
PROC Slaengutbitmap
 mov edi,[screen]  
 xor eax,eax       
 mov ecx,16000     
 rep stosd         
                   
 mov esi,offset lastpal
 mov al,0              
 mov ecx,768           
 Call SetPalette       
 mov esi,offset lastlogo
 mov edi,[screen]       
 mov ecx,16000          
 rep movsd              
                        
 mov ecx,70*5      
 Call waitecx           
                        
 mov ax,3          
 int 10h           
 ret                    
ENDP                    
                        
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
;°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
                                           
Proc Demo               
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
 Call MultipleLight2
 Call Credits  
 Call AnimPart 
 mov ecx,210   
 call waitecx  
 Call Shake    
 Call A_Part   
 Call DoTwirl  
 Call DoKalkun1
 Call DoKalkun2
 Call DoKalkun3
 Call DoKalkun4
 Call ShowPic1
 Call rist_regardar
 Call Yeah_shake_it
 Call MultipleLight1
 Call MultipleLight2
 Call SlaengUtBitmap
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
 ret                
ENDP                   
                    
;°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
           
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
;° EntryPoint from DOS °°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
EntryPoint:        
; call debug_run   
 mov eax,0EE02h    
 int 31h           
 mov edi,0A0000h   
 sub edi,ebx          
 mov [Screen],edi  
 cld    
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
 ; Allocate 64K for Gouraud shade table
 mov ax,0ee42h                         
 mov edx,10000h    
 int 31h           
 jc DemoEnd         
 mov [Gouraud],edx    
 Call CalcTex               
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
 ; Allocate 9*64K for Background buffer (for 3D)
 mov ax,0ee42h                                  
 mov edx,90000h                                 
 int 31h                                        
 jc DemoEnd                                     
 mov [Buffer],edx                               
 call randomize                                 
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
 call autoMusic_Init
 lea eax,timerhandler                                                 
 call nms_activatesync                                                
 Call Music_Start                                                     
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
 mov ax,13h                                                           
 int 10h                                                              
 Call Demo
;         mov ah,0                   
;         int 16h                            
DemoEnd:                                                              
 Call Music_Stop                                                      
 call nms_deactivatesync
 
 mov eax, 0ee40h
 int 31h      
 mov eax, 0ee40h
 int 31h      
              
 mov ax,3h                                                            
 int 10h      
 writeln "'det m† bare bli goal det' - Excess '96"
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
 mov ax,4c00h                                                         
 int 21h                    
End EntryPoint              
 